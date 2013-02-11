///============================================================================
/// \note   Elemental Engine
///         Copyright (c)  2005-2008 Signature Devices, Inc.
///
///         This code is redistributable under the terms of the EE License.
///
///         This code is distributed without warranty or implied warranty of
///			merchantability or fitness for a particular purpose. See the 
///			EE License for more details.
///
///         You should have received a copy of the EE License along with this
///			code; If not, write to Signature Devices, Inc.,
///			3200 Bridge Parkway Suite 102, Redwood City, CA 94086 USA.
///============================================================================

#include "stdafx.h"

#include "toluaxx.h"
#include "ldo.h"
#include "lundump.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLuaHelper* CLuaHelper::m_pThis = NULL;

CLuaHelper::CLuaHelper()
{
	L = NULL;
	m_nLoaded = 0;
	m_pThis = this;
}

CLuaHelper::~CLuaHelper()
{
}

// function which replace fputs used in standard library
extern "C" void PrintDebugString(const char* str)
{
	CDebugger::GetDebugger()->Write(str);
}

BOOL CLuaHelper::ErrorStringToFileLine(CString strError, CString &strPathName, int &nLine)
{
	CString strFileLine = strError;
	if ( strError.Left(4)=="luac" )
		strFileLine = strError.Mid(6);

	int nPos1 = strFileLine.Find(':',2);
	if ( nPos1==-1 )
		return FALSE;
	int nPos2 = strFileLine.Find(':', nPos1+1);
	if ( nPos2==-1 )
		return FALSE;

	CString strNum = strFileLine.Mid(nPos1+1, nPos2-nPos1-1);
	nLine = atoi(strNum);
	strPathName = strFileLine.Left(nPos1);

	return TRUE;
}

void CLuaHelper::GetValidLines(CLuaDoc* pDoc, const Proto* f)
{
	int i;
	int numFunctions = f->sizep;

	int *lineinfo = f->lineinfo;
	for (i=0; i < f->sizelineinfo; i++)
	{
		pDoc->AddDebugLine(lineinfo[i]);
	}

	for (i=0; i< numFunctions; i++)
	{
		 GetValidLines(pDoc, f->p[i]);
	}
}

BOOL CLuaHelper::LoadDebugLines(CLuaDoc* pDoc, CString &fileName)
{
	lua_State* L=lua_open();
	
	BOOL bOk = (luaL_loadfile(L , fileName) == 0);
   
	if ( bOk )
	{
		pDoc->RemoveAllDebugLines();

		// make sure top is a function
		if (lua_isfunction(L, -1))
		{
			Closure *f;
			StkId func;
			func = L->top - 1;
			f = clvalue(func);
			GetValidLines(pDoc, f->l.p);
		}
	}

	lua_close(L);
	
	return bOk;
}

//=============================================================================================
//=============================================================================================

void CLuaHelper::Free()
{
	if ( L )
	{
		lua_close(L);
		L = NULL;
	}

	if ( m_nLoaded )
	{
		for ( int i=m_nLoaded-1; i>=0; --i )
			FreeLibrary(m_hLoaded[i]);

		m_nLoaded = 0;
	}
}

BOOL CLuaHelper::PrepareDebugger(CString &fileName)
{
	ASSERT(L==NULL);
	L = lua_open();

	lua_register(L, "_ALERT", OutputTop);
	lua_register(L, "_ERRORMESSAGE", errormessage);

	ASSERT(m_nLoaded==0);
	lua_register(L, "loadlib", lua_loadlib);

	lua_sethook(L, hook,LUA_MASKLINE|LUA_MASKCALL|LUA_MASKRET,0);

	int status = luaL_loadfile(L, fileName);
	if ( status!=0 )
	{
		OutputTop(L);
		return FALSE;
	}

	return TRUE;
}

int CLuaHelper::OutputTop(lua_State* L)
{
	CDebugger::GetDebugger()->Write(lua_tostring(L, -1));
	CDebugger::GetDebugger()->Write("\n");
	return 0;
}

BOOL CLuaHelper::StartDebugger()
{
	int top = lua_gettop(L);
	lua_getglobal(L, "_ERRORMESSAGE");
	lua_insert(L, top);
	int status = lua_pcall(L, 0, LUA_MULTRET, top);
	return TRUE;
}

void CLuaHelper::StopDebugger()
{
	Free();
}

#define LEVELS1	12	/* size of the first part of the stack */
#define LEVELS2	10	/* size of the second part of the stack */

int CLuaHelper::errormessage(lua_State *L)
{
	int level = 1;  /* skip level 0 (it's this function) */
	int firstpart = 1;  /* still before eventual `...' */
	lua_Debug ar;
	if (!lua_isstring(L, 1))
		return lua_gettop(L);
	lua_settop(L, 1);
	lua_pushliteral(L, "\n");
	lua_pushliteral(L, "stack traceback:\n");
	while (lua_getstack(L, level++, &ar)) 
	{
		char buff[10];
		if (level > LEVELS1 && firstpart) 
		{
			/* no more than `LEVELS2' more levels? */
			if (!lua_getstack(L, level+LEVELS2, &ar))
				level--;  /* keep going */
			else 
			{
				lua_pushliteral(L, "       ...\n");  /* too many levels */
				while (lua_getstack(L, level+LEVELS2, &ar))  /* find last levels */
					level++;
			}
			firstpart = 0;
			continue;
		}
    
		sprintf(buff, "%4d-  ", level-1);
		lua_pushstring(L, buff);
		lua_getinfo(L, "Snl", &ar);
		lua_pushfstring(L, "%s:", ar.short_src);
		if (ar.currentline > 0)
			lua_pushfstring(L, "%d:", ar.currentline);
		switch (*ar.namewhat) 
		{
			case 'g':  /* global */ 
			case 'l':  /* local */
			case 'f':  /* field */
			case 'm':  /* method */
				lua_pushfstring(L, " in function `%s'", ar.name);
			break;
			default: 
			{
				if (*ar.what == 'm')  /* main? */
					lua_pushfstring(L, " in main chunk");
				else if (*ar.what == 'C')  /* C function? */
					lua_pushfstring(L, "%s", ar.short_src);
				else
					lua_pushfstring(L, " in function <%s:%d>", ar.short_src, ar.linedefined);
			}
    
		}

		lua_pushliteral(L, "\n");
		lua_concat(L, lua_gettop(L));
	}
  
	lua_concat(L, lua_gettop(L));
  
	OutputTop(L);

	return 0;
}


void CLuaHelper::line_hook (lua_State *L, lua_Debug *ar)
{
	lua_getinfo(L, "lnuS", ar);
	m_pThis->m_pAr = ar;

	if ( ar->source[0] == '@' )
		CDebugger::GetDebugger()->LineHook(ar->source+1, ar->currentline);
}

void CLuaHelper::func_hook (lua_State *L, lua_Debug *ar)
{
	lua_getinfo(L, "lnuS", ar);
	m_pThis->m_pAr = ar;
	
	const char* szSource=NULL;
	if ( ar->source[0] == '@' )
 	{
 		szSource=ar->source+1;
 	}
 	CDebugger::GetDebugger()->FunctionHook(szSource, ar->currentline, ar->event==LUA_HOOKCALL);
}
 
void CLuaHelper::hook (lua_State *L, lua_Debug *ar)
{
 	switch(ar->event)
 	{
 		case LUA_HOOKTAILRET:
 		case LUA_HOOKRET:
 						func_hook(L,ar);
 						break;
 		case LUA_HOOKCALL:func_hook(L,ar);
 						break;
 		case LUA_HOOKLINE:line_hook(L,ar);
 						break;
 	}
}

const char* CLuaHelper::GetSource() 
{ 
	return m_pAr->source+1; 
};


void CLuaHelper::DrawStackTrace()
{
	CDebugger::GetDebugger()->ClearStackTrace();

	int nLevel = 0;
	lua_Debug ar;
	char szDesc[256];
	while ( lua_getstack (L, nLevel, &ar) )
	{
		lua_getinfo(L, "lnuS", &ar);
		if ( ar.source[0]=='@' )
		{
			szDesc[0] = '\0';
			if ( ar.name )
				strcat(szDesc, ar.name);
			strcat(szDesc, ",");
			if ( ar.namewhat )
				strcat(szDesc, ar.namewhat);
			strcat(szDesc, ",");
			if ( ar.what )
				strcat(szDesc, ar.what);
			strcat(szDesc, ",");
			if ( ar.short_src )
				strcat(szDesc, ar.short_src);

			CDebugger::GetDebugger()->AddStackTrace(szDesc, ar.source+1, ar.currentline);
		}

		++nLevel;
	};
}

void CLuaHelper::DrawLocalVariables()
{
	CDebugger::GetDebugger()->ClearLocalVariables();

	int nLevel = CDebugger::GetDebugger()->GetStackTraceLevel();
	lua_Debug ar;
	if ( lua_getstack (L, nLevel, &ar) )
	{
		int i = 1;
        const char *name, *type;
        while ((name = lua_getlocal(L, &ar, i++)) != NULL) {
			int ntype = lua_type(L, -1);
			type = lua_typename(L, ntype);
			char value[64];
			
			switch(ntype)
			{
			case LUA_TNUMBER:
				sprintf(value, "%f", lua_tonumber(L, -1));
				break;
			case LUA_TSTRING:
				sprintf(value, "%.63s", lua_tostring(L, -1));
				break;
			default:
				value[0] = '\0';
				break;
			}

			CDebugger::GetDebugger()->AddLocalVariable(name, type, value);

			lua_pop(L, 1);  /* remove variable value */
        }
	}
}

void CLuaHelper::DrawGlobalVariables()
{
	CDebugger::GetDebugger()->ClearGlobalVariables();

	lua_pushvalue(L, LUA_GLOBALSINDEX);

	lua_pushnil(L);  /* first key */
	while (lua_next(L, -2))
	{
		TRACE2("%s - %s\n",
           lua_typename(L, lua_type(L, -2)), lua_typename(L, lua_type(L, -1)));
		
		lua_pop(L, 1); // pop value, keep key for next iteration;
	}
	lua_pop(L, 1); // pop table of globals;
};

BOOL CLuaHelper::GetCalltip(const char *szWord, char *szCalltip)
{
	int nLevel = CDebugger::GetDebugger()->GetStackTraceLevel();
	lua_Debug ar;
	if ( lua_getstack (L, nLevel, &ar) )
	{
		int i = 1;
        const char *name;
        while ((name = lua_getlocal(L, &ar, i++)) != NULL) {
			if ( strcmp(name, szWord)==0 )
			{
				char szRet[64];
				Describe(szRet, -1);
				sprintf(szCalltip, "local %s : %s ", name, szRet);
				lua_pop(L, 1);  /* remove variable value */
				return TRUE;
			}

			lua_pop(L, 1);  /* remove variable value */
        }
	}

	lua_pushvalue(L, LUA_GLOBALSINDEX);

	lua_pushnil(L);  /* first key */
	while (lua_next(L, -2))
	{
		const char* name = lua_tostring(L, -2);
		if ( strcmp(name, szWord)==0 )
		{
			char szRet[64];
			Describe(szRet, -1);
			sprintf(szCalltip, "global %s : %s ", name, szRet);

			lua_pop(L, 3);  /* remove table, key, value */

			return TRUE;
		}
		
		lua_pop(L, 1); // pop value, keep key for next iteration;
	}
	lua_pop(L, 1); // pop table of globals;

	return FALSE;
}

typedef int (*LuaRegister)(lua_State*, HWND hWnd);
int CLuaHelper::lua_loadlib(lua_State *L)
{
	HMODULE hMod = LoadLibrary(lua_tostring(L, 1));
	if ( hMod )
	{
		LuaRegister lua_reg = (LuaRegister)GetProcAddress(hMod, "LuaRegister");
		if ( lua_reg )
		{
			lua_reg(L, CDebugger::GetDebugger()->GetMainWnd());

			m_pThis->m_hLoaded[m_pThis->m_nLoaded++] = hMod;

			lua_pushboolean(L, TRUE);
			return 1;
		}
		else
		{
			luaL_argerror(L, 1, "Dll has no LuaRegister entry");
			lua_pushboolean(L, FALSE);
			return 1;
		}	
	}
	else
	{
		LPVOID lpMsgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
		);
		luaL_argerror(L, 1, (LPCTSTR)lpMsgBuf);
		LocalFree( lpMsgBuf );
		lua_pushboolean(L, FALSE);
		return 1;
	}
}

BOOL CLuaHelper::Eval(const char *szCode, char* szRet)
{
	CoverGlobals();

	int top = lua_gettop(L);	
	int status = luaL_loadbuffer(L, szCode, strlen(szCode), szCode);
	if ( status )
		sprintf(szRet, "%s", lua_tostring(L, -1));
	else
	{
		status = lua_pcall(L, 0, LUA_MULTRET, 0);  /* call main */
		if ( status )
		{
			const char* szErr = lua_tostring(L, -1);
			const char* szErr2 = strstr(szErr, ": ");
			sprintf(szRet, "%s", szErr2?(szErr2+2):szErr);
		}
		else
			Describe(szRet, -1);
	}

	lua_settop(L, top);

	RestoreGlobals();

	return !status;
}

void CLuaHelper::Describe(char *szRet, int nIndex)
{
	int ntype = lua_type(L, nIndex);
	const char* type = lua_typename(L, ntype);
	char value[64];

	switch(ntype)
	{
	case LUA_TNUMBER:
		sprintf(value, "%f", lua_tonumber(L, nIndex));
		break;
	case LUA_TSTRING:
		sprintf(value, "%.63s", lua_tostring(L, nIndex));
		break;
	default:
		value[0] = '\0';
		break;
	}
	sprintf(szRet, "%s : %.64s", type, value);
}

void CLuaHelper::CoverGlobals()
{
	lua_newtable(L);  // save there globals covered by locals

	int nLevel = CDebugger::GetDebugger()->GetStackTraceLevel();
	lua_Debug ar;
	if ( lua_getstack (L, nLevel, &ar) )
	{
		int i = 1;
        const char *name;
        while ((name = lua_getlocal(L, &ar, i++)) != NULL) { /* SAVE lvalue */
			lua_pushstring(L, name);	/* SAVE lvalue name */						
			lua_pushvalue(L, -1);	/* SAVE lvalue name name */
			lua_pushvalue(L, -1);	/* SAVE lvalue name name name */
			lua_insert(L, -4);		/* SAVE name lvalue name name */
			lua_rawget(L, LUA_GLOBALSINDEX);	/* SAVE name lvalue name gvalue */

			lua_rawset(L, -5);	// save global value in local table
								/* SAVE name lvalue */

			lua_rawset(L, LUA_GLOBALSINDEX); /* SAVE */
		}
	}
}

void CLuaHelper::RestoreGlobals()
{
	// there is table of covered globals on top

	lua_pushnil(L);  /* first key */
					/* SAVE nil */
	while (lua_next(L, -2))		/* SAVE key value */
	{
		lua_pushvalue(L, -2); /* SAVE key value key */
		lua_insert(L, -2); /* SAVE key key value */

		lua_rawset(L, LUA_GLOBALSINDEX);	// restore global
											/* SAVE key */
	}

	lua_pop(L, 1); // pop table of covered globals;
}
