///============================================================================
/// \file		LuaHelper.h
/// \brief		Header file for the Lua Helper class
/// \date		06-17-2005
/// \author		Josh Lulewicz
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

#if !defined(AFX_LUAHELPER_H__FE39E254_5793_42CE_B1CA_C38E7437E9AD__INCLUDED_)
#define AFX_LUAHELPER_H__FE39E254_5793_42CE_B1CA_C38E7437E9AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct lua_State;
struct Proto;
struct lua_Debug;

class CLuaDoc;

class CLuaHelper  
{
public:
	void RestoreGlobals();
	void CoverGlobals();
	void Describe(char* szRet, int nIndex);
	BOOL Eval(const char* szCode, char* szRet);
	BOOL GetCalltip(const char *szWord, char *szCalltip);
	void DrawGlobalVariables();
	void DrawLocalVariables();
	const char* GetSource();

	static BOOL ErrorStringToFileLine(CString strError, CString &strPathName, int &nLine);
	static BOOL LoadDebugLines(CLuaDoc* pDoc, CString &fileName);

	CLuaHelper();
	virtual ~CLuaHelper();

// debugger functions
	BOOL PrepareDebugger(CString &fileName);
	BOOL StartDebugger();	
	void StopDebugger();

	void DrawStackTrace();

	lua_State* GetState() { return L; };
protected:
	void Free();
	static CLuaHelper* m_pThis;

	static void GetValidLines(CLuaDoc* pDoc, const Proto* f);
	static int OutputTop(lua_State* L);
	static int errormessage(lua_State* L);
	static int lua_loadlib(lua_State* L);
	static void line_hook (lua_State *L, lua_Debug *ar);
	static void func_hook (lua_State *L, lua_Debug *ar);
	static void hook (lua_State *L, lua_Debug *ar);

	lua_State* L;
	lua_Debug* m_pAr;
	HMODULE m_hLoaded[8192];
	int m_nLoaded;
};

#endif // !defined(AFX_LUAHELPER_H__FE39E254_5793_42CE_B1CA_C38E7437E9AD__INCLUDED_)
