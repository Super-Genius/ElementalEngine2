///============================================================================
/// \file		PrecacheLoader.cpp
/// \brief		Implementation of the loader for precache files
/// \date		12-18-2006
/// \author		D. Patrick Ghiocel
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON( CPrecacheLoader );
REGISTER_MESSAGE_HANDLER( LoadFile, OnLoadFile, CPrecacheLoader );
REGISTER_MESSAGE_HANDLER( SaveFile, OnSaveFile, CPrecacheLoader );

CHashString CPrecacheLoader::m_LoaderName(_T("CPrecacheLoader"));

LSRC_MESSAGE CPrecacheLoader::m_LoadSaveRegisterData = {
		&CPrecacheLoader::m_LoaderName,
		_T("Precache Files (*.pcf)\0" "*.pcf\0"
		"\0\0"),
		_T("Precache Files (*.pcf)\0" "*.pcf\0"
		"\0\0"),
		false, NULL,
};

CPrecacheLoader::CPrecacheLoader()
{
	m_ToolBox = EngineGetToolBox();

	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE),&m_LoadSaveRegisterData) != MSG_HANDLED)
		m_ToolBox->Log(LOGERROR, _T("%s(%d): Failed to register Load Save Extension"));
}

/// standard static component create function
IComponent *CPrecacheLoader::Create( int nArgs, va_list argptr )
{
	return SINGLETONINSTANCE( CPrecacheLoader );
}

IHashString *CPrecacheLoader::GetComponentType()
{
	return &m_LoaderName;
}

bool CPrecacheLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

DWORD CPrecacheLoader::OnLoadFile( DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;
	
	DWORD retVal;
	
	static CHashString streamType(_T("File"));
	CREATEARCHIVE ca;
	ca.streamData = (void *)pFileName;
	ca.mode = STREAM_MODE_READ;
	ca.streamType = &streamType;
	// call the Archive factory to create an XML archive
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (retVal = m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return MSG_ERROR;
	}
	IXMLArchive *FileArchive = dynamic_cast<IXMLArchive *>(ca.archive);

	StdString szNodeName;
	FileArchive->GetNode(szNodeName);
	// must start with <Precache>
	if (szNodeName != _T("Precache"))
	{
		m_ToolBox->SetErrorValue(WARN_INVALID_FILE);
		m_ToolBox->Log(LOGWARNING, _T("Invalid precache file %s\n"), pFileName);
		FileArchive->Close();
		return MSG_ERROR;
	}

	static CHashString hszWorld( _T("World") );
	static CHashString hszType( _T("CPrecacheObject") );
	CHashString hszName( pFileName );
	
	CREATEOBJECTPARAMS cop;
	cop.parentName = NULL;
	cop.typeName = &hszType;
	cop.name = &hszName;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

	SERIALIZEOBJECTPARAMS sop;
	sop.name = &hszName;
	sop.archive = FileArchive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

	INITOBJECTPARAMS iop;
	iop.name = &hszName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);

	FileArchive->Close();

	return MSG_HANDLED_PROCEED;
}

DWORD CPrecacheLoader::OnSaveFile( DWORD size, void *params)
{
#ifndef XBOX
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;
	
	DWORD retVal;

	static CHashString streamType(_T("File"));
	CREATEARCHIVE ca;
	ca.streamData = (void *)pFileName;
	ca.mode = STREAM_MODE_WRITE;
	ca.streamType = &streamType;
	// call the Archive factory to create an XML archive
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (retVal = m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return MSG_ERROR;
	}
	IXMLArchive *FileArchive = dynamic_cast<IXMLArchive *>(ca.archive);

	FileArchive->StartClass( _T("Precache") );
	CHashString hszName( pFileName );

	SERIALIZEOBJECTPARAMS sop;	
	sop.name = &hszName;
	sop.archive = FileArchive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

	FileArchive->EndClass();
	FileArchive->Close();
#endif
	return MSG_HANDLED_PROCEED;
}

TCHAR *CPrecacheLoader::GetLoadDescription( void )
{
	return _T( "Precache Files (*.pcf)\0" "*.pcf\0" );
}

TCHAR *CPrecacheLoader::GetSaveDescription( void )
{
	return _T( "Precache Files (*.pcf)\0" "*.pcf\0" );
}