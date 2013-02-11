///============================================================================
/// \file		HMPLoader.cpp
/// \brief		Implementation file for HMP based Heightmap Loader
/// \date		06-28-2005
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

#include "StdAfx.h"
#include "IXMLArchive.h"
#include "BaseMessages.h"
#include "IArchive.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON(CHMPLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadHMPFile, CHMPLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveHMPFile, CHMPLoader);

CHashString CHMPLoader::m_HashName(_T("CHMPLoader"));

LSRC_MESSAGE CHMPLoader::m_LoadSaveRegisterData = {
		&CHMPLoader::m_HashName,
		_T("HeightMap Files (*.hmp)\0" "*.hmp\0"
		"\0\0"),
		_T("HeightMap Files (*.hmp)\0" "*.hmp\0"
		"\0\0"),
		false, NULL,
};

CHMPLoader::CHMPLoader() 
{
	// save for convenience
	m_ToolBox = EngineGetToolBox();
	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE), &m_LoadSaveRegisterData);
}

// create function for this world loader
IComponent *CHMPLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CHMPLoader );
}

IHashString *CHMPLoader::GetComponentType()
{
	return &m_HashName;
}

bool CHMPLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// load a file if we recognize the type.
DWORD CHMPLoader::OnLoadHMPFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;

	CHashString hszTypeName(_T("CHeightmapObject"));
	CHashString hszParentName(_T(""));
	CHashString hszName( pFileName );
	CHashString streamType(_T("File"));

	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_READ | STREAM_MODE_BINARY;
	ca.streamData = pFileName;
	ca.streamType = &streamType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if( m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED )
	{
		m_ToolBox->Log(LOGERROR, _T("Unable to create archive for path '%s' in file %s at line %d\n"), pFileName, __FILE__, __LINE__);
		return MSG_ERROR;
	}

	IArchive *FileArchive = ca.archive;

	// Create the Heightmap Object
	CREATEOBJECTPARAMS cop;
	cop.parentName = &hszParentName;
	cop.typeName = &hszTypeName;
	cop.name = &hszName;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

	// Serialize the Heightmap Object
	if (FileArchive)
	{
		SERIALIZEOBJECTPARAMS sop;
		sop.name = &hszName;
		sop.archive = FileArchive;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

		// close archive
		FileArchive->Close();
	}

	// Initialize the Heightmap Object
	INITOBJECTPARAMS iop;
	iop.name = &hszName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);

	return MSG_HANDLED_STOP;
}

// Save a file if we recognize the type.
DWORD CHMPLoader::OnSaveHMPFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;

	CHashString hszName( pFileName );
	CHashString streamType(_T("File"));

	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_BINARY;
	ca.streamData = pFileName;
	ca.streamType = &streamType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if( m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED )
	{
		return MSG_ERROR;
	}

	IArchive *FileArchive = ca.archive;

	// Serialize the Heightmap Object
	if (FileArchive)
	{
		SERIALIZEOBJECTPARAMS sop;
		sop.name = &hszName;
		sop.archive = FileArchive;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

		// close archive
		FileArchive->Close();
	}

	return MSG_HANDLED_STOP;
}
