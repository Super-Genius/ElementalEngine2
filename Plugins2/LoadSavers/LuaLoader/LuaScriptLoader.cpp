///============================================================================
/// \file		LuaScriptLoader.cpp
/// \brief		Implementation file for Lua Script Loader
/// \date		05-31-2005
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

#include "StdAfx.h"
#include "LuaScriptLoader.hpp"
#include "BaseMessages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON(CLuaScriptLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadLuaFile, CLuaScriptLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveLuaFile, CLuaScriptLoader);

CHashString CLuaScriptLoader::m_HashName(_T("CLuaScriptLoader"));

LSRC_MESSAGE CLuaScriptLoader::m_LoadSaveRegisterData = {
		&CLuaScriptLoader::m_HashName,
		_T("Lua Script Files (*.lua)\0" "*.lua\0"
		"\0\0"),
		_T("Lua Script Files (*.lua)\0" "*.lua\0"
		"\0\0"),
		true, NULL,
};

CLuaScriptLoader::CLuaScriptLoader() 
{
	// save for convenience
	m_ToolBox = EngineGetToolBox();
	DWORD check;

	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	check = m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE),&m_LoadSaveRegisterData);

	if (!check)
	{
		// log error RegisterLoadSaveComponent failed!
	}
}

// create function for this world loader
IComponent *CLuaScriptLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CLuaScriptLoader );
}

IHashString *CLuaScriptLoader::GetComponentType()
{
	return &m_HashName;
}

bool CLuaScriptLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// load a file if we recognize the type.
DWORD CLuaScriptLoader::OnLoadLuaFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;
	// data is of type char * instead of TCHAR * because the 
	// file is plain text and we don't want to convert to wide chars!
	char *data;
	FILE *stream;

	// Open for read 
		StdString szPath(pFileName);
		szPath.MakeSafeFileName();
#ifdef XBOX
		szPath = SetPathDrive( szPath, EngineGetToolBox()->GetDrive() );
#endif

	if( (stream  = fopen( szPath, "rt" )) == NULL )
	{
		// log the file could not be opened
		// here we did return 'MSG_NOT_HANDLED', but that did not cause the file loader
		// to get to know, that the file can't be opened. Let's return MSG_ERROR instead
		return MSG_ERROR;
	}

	fseek(stream, 0, SEEK_END);
	int count = ftell(stream)+1;
	fseek(stream, 0, SEEK_SET);

	//get memory
	//data = (char *)GLOBALALLOC(count);
	data = new char[count];
	if (!data)
	{
		fclose(stream);
		// log
		return MSG_NOT_HANDLED;
	}
	
	int realCount = (int)fread(data,1,count, stream);

	data[realCount] = '\0';
	SCRIPTPARAMS script;
	CHashString fileName(pFileName);
	script.fileName = &fileName;
	script.scriptData = data;
	script.dataSize = realCount;

	// no need to check this return value because handler might not be loaded
	static DWORD msgHash_AddScript = CHashString(_T("AddScript")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddScript, sizeof(SCRIPTPARAMS), &script);

	// Close stream 
	fclose(stream);

	return MSG_HANDLED_STOP;
}

// Save a file if we recognize the type.
DWORD CLuaScriptLoader::OnSaveLuaFile(DWORD size, void *params)
{
	// Check the message to verify the size
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;
	FILE *stream;
	SCRIPTPARAMS sp;

	// make sure we have a valid file name
	if( pFileName == NULL )
	{
		return MSG_ERROR;
	}

	// init datasize
	sp.dataSize = 0;

	// get new data size
	static DWORD msgHash_GetScriptDataSize = CHashString(_T("GetScriptDataSize")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetScriptDataSize, sizeof(SCRIPTPARAMS), &sp);
    // make sure we got something
	if (sp.dataSize == 0)
	{
		// no data to save
		return MSG_ERROR;
	}
	// make sure will have enough room for the end of the string
	sp.dataSize = sp.dataSize+1;
	//get memory
	sp.scriptData = new char[sp.dataSize];

	// grab data
	static DWORD msgHash_GetScriptData = CHashString(_T("GetScriptData")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetScriptData, sizeof(SCRIPTPARAMS), &sp);

	// set file name for script
	CHashString name(pFileName);
	sp.fileName = &name;

	// Open for write 
	if( (stream  = fopen( pFileName, "wb" )) == NULL )
	{
		// log the file could not be opened
		return MSG_NOT_HANDLED;
	}
	// write data
	fwrite(sp.scriptData, 1, sp.dataSize-1, stream);
	// Close stream 
	fclose( stream );

	// no need to check this return value because handler might not be loaded
	static DWORD msgHash_AddScript = CHashString(_T("AddScript")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddScript, sizeof(SCRIPTPARAMS), &sp);

	return MSG_HANDLED_STOP;
}
