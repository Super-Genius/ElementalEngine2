///============================================================================
/// \file		SoundLoader.cpp
/// \brief		Implementation file for Sound Loading
/// \date		10-05-2006
/// \author		Jonathan Burgess
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON(CLanguageLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadFile, CLanguageLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveFile, CLanguageLoader);

CHashString CLanguageLoader::m_HashName(_T("CLanguageLoader"));

LSRC_MESSAGE CLanguageLoader::m_LoadSaveRegisterData = {
		&CLanguageLoader::m_HashName,
		_T("Language files (*.lxl)\0" "*.lxl\0")		
		_T("\0\0"),
		_T("Language files (*.lxl)\0" "*.lxl\0")
		_T("\0\0"),
		false, NULL,
};

CLanguageLoader::CLanguageLoader() 
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
IComponent *CLanguageLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CLanguageLoader );
}

IHashString *CLanguageLoader::GetComponentType()
{
	return &m_HashName;
}

bool CLanguageLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

DWORD CLanguageLoader::OnLoadFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;

	DWORD retval = MSG_HANDLED_STOP;

	StdString fileName = pFileName;
	fileName.MakeSafeFileName();
	CHashString hszFileName(fileName.c_str());
	
	CHashString streamType(_T("File"));
	CREATEARCHIVE ca;
	
	ca.streamData = (void *)pFileName;
	ca.mode = STREAM_MODE_READ;
	ca.streamType = &streamType;
	
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if ((m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED) ||
		(ca.archive == NULL))
	{
		m_ToolBox->Log(LOGWARNING, _T("Could not create archive for language %s\n"), (const TCHAR*)fileName);
		retval = MSG_ERROR;
	}
	else
	{
		static DWORD msgAddLanguage = CHashString(_T("LoadLanguage")).GetUniqueID();
		m_ToolBox->SendMessage(msgAddLanguage, sizeof(IArchive*), ca.archive);
		ca.archive->Close();
	}

	return retval;
}

DWORD CLanguageLoader::OnSaveFile(DWORD size, void *params)
{
	// TODO
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;

	DWORD retval = MSG_HANDLED_STOP;

	return retval;
}