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
//#include <LibXML++/libxml++.h>
#include "Cal3dMeshLoader.h"
//#include "PerformanceProfiler.h"

REGISTER_COMPONENT_SINGLETON(CCal3dMeshLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadCal3dCMFFile, CCal3dMeshLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveCal3dCMFFile, CCal3dMeshLoader);
REGISTER_MESSAGE_HANDLER(UnloadFile, OnUnloadCal3dCMFFile, CCal3dMeshLoader);

CHashString CCal3dMeshLoader::m_HashName(_T("CCal3dMeshLoader"));

LSRC_MESSAGE CCal3dMeshLoader::m_LoadSaveRegisterData = {
		&CCal3dMeshLoader::m_HashName,
		_T("Cal3d Mesh Files (*.cmf)\0" "*.cmf\0"
		"\0\0"),
		_T("Cal3d Mesh Files (*.cmf)\0" "*.cmf\0"
		"\0\0"),
		true, NULL,
};

CCal3dMeshLoader::CCal3dMeshLoader()
{
	m_ToolBox = EngineGetToolBox();

	m_CMFMap.clear();

	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE),&m_LoadSaveRegisterData) != MSG_HANDLED)
		m_ToolBox->Log(LOGERROR, _T("%s(%d): Failed to register Load Save Extension"));
}

// create function for this Particle loader
IComponent *CCal3dMeshLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CCal3dMeshLoader );
}

IHashString *CCal3dMeshLoader::GetComponentType()
{
	return &m_HashName;
}

bool CCal3dMeshLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

/////////////////////////////////////////////////////////////
//	LOAD MESH FILE
/////////////////////////////////////////////////////////////
DWORD CCal3dMeshLoader::OnLoadCal3dCMFFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	
	// Get the Filepath and convert it all to lower case
	TCHAR* ptmp = (TCHAR *)params;

	// store the stdstring and chashstring version
	StdString szFilepath( ptmp );
	szFilepath.MakeSafeFileName();
	CHashString hszFilepath( szFilepath );

	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &hszFilepath;
	if( EngineGetToolBox()->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param) != MSG_HANDLED)
		return MSG_ERROR;
	if (param.bFound == true)
		return MSG_HANDLED_STOP;

	CHashString streamType(_T("Memory"));
	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ | STREAM_MODE_BINARY;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &streamType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (EngineGetToolBox()->SendMessage(msgHash_CreateArchive, sizeof(ca), &ca) != MSG_HANDLED)
		return MSG_ERROR;

	CREATESTREAM cs;
	cs.streamData =	(TCHAR*)szFilepath.c_str();
	cs.mode = STREAM_MODE_READ | STREAM_MODE_BINARY;
	static DWORD msgHash_CreateStream_File = CHashString(_T("CreateStream_File")).GetUniqueID();
	DWORD msgRet = m_ToolBox->SendMessage(msgHash_CreateStream_File, sizeof(CREATESTREAM), &cs);
	if (msgRet != MSG_HANDLED || cs.openStream == NULL)
	{
		ca.archive->Close();
		return MSG_ERROR;
	}
	cs.openStream->SeekTo(0,SEEK_END);
	int iFileSize = cs.openStream->Tell();
	cs.openStream->SeekTo(0,SEEK_SET);
	void *pFileData = new BYTE[iFileSize];
	cs.openStream->Read( pFileData, iFileSize );
	delete cs.openStream;

	ca.archive->SetIsWriting( true );
	ca.archive->Write( &pFileData, sizeof(void*) );

	ca.archive->SetIsWriting( false );
	static CHashString Cal3DCoreMeshCacheType(_T("CCal3DCoreMeshCache"));
	bool bResult = BuildObject( &hszFilepath, &Cal3DCoreMeshCacheType, ca.archive );
	m_CMFMap[hszFilepath.GetUniqueID()] = hszFilepath.GetUniqueID();
	ca.archive->Close();
	delete[] (BYTE *)pFileData;

	if (bResult)
		return MSG_HANDLED_STOP;
	else
		return MSG_ERROR;
}

/////////////////////////////////////////////////////////////
//	SAVE MESH FILE
/////////////////////////////////////////////////////////////
DWORD CCal3dMeshLoader::OnSaveCal3dCMFFile(DWORD size, void *params)
{
	return MSG_HANDLED_STOP;
}

/////////////////////////////////////////////////////////////
//	UNLOAD MESH FILE
/////////////////////////////////////////////////////////////
DWORD CCal3dMeshLoader::OnUnloadCal3dCMFFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	CHashString fileName((const TCHAR *)params);
	DWORD result;

	FILETOOBJECTMAP::iterator itr = m_CMFMap.find( fileName.GetUniqueID() );

	if( itr != m_CMFMap.end() )
	{
		CHashString tmpObjectName( m_ToolBox->GetHashString( itr->second ) );
		DELETEOBJECTPARAMS dop;
		dop.name = &tmpObjectName;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		result = m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof( DELETEOBJECTPARAMS ), &dop, NULL, NULL );
		m_CMFMap.erase( itr );
		return result;
	}
	else
	{
		return MSG_ERROR;
	}

	return MSG_HANDLED_STOP;
}

