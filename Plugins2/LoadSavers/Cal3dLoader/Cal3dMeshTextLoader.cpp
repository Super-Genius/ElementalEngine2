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
#include "Cal3dMeshTextLoader.h"
//#include "PerformanceProfiler.h"

REGISTER_COMPONENT_SINGLETON(CCal3dMeshTextLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadCal3dXMFFile, CCal3dMeshTextLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveCal3dXMFFile, CCal3dMeshTextLoader);

CHashString CCal3dMeshTextLoader::m_HashName(_T("CCal3dMeshTextLoader"));

LSRC_MESSAGE CCal3dMeshTextLoader::m_LoadSaveRegisterData = {
		&CCal3dMeshTextLoader::m_HashName,
		_T("Cal3d Mesh Files (*.xmf)\0" "*.xmf\0"
		"\0\0"),
		_T("Cal3d Mesh Files (*.xmf)\0" "*.xmf\0"
		"\0\0"),
		false, NULL,
};

CCal3dMeshTextLoader::CCal3dMeshTextLoader()
{
	m_ToolBox = EngineGetToolBox();

	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE),&m_LoadSaveRegisterData) != MSG_HANDLED)
		m_ToolBox->Log(LOGERROR, _T("%s(%d): Failed to register Load Save Extension"));
}

// create function for this Particle loader
IComponent *CCal3dMeshTextLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CCal3dMeshTextLoader );
}

IHashString *CCal3dMeshTextLoader::GetComponentType()
{
	return &m_HashName;
}

bool CCal3dMeshTextLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

/////////////////////////////////////////////////////////////
//	LOAD PARTICLE EMITTER
/////////////////////////////////////////////////////////////
DWORD CCal3dMeshTextLoader::OnLoadCal3dXMFFile(DWORD size, void *params)
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

	CHashString streamType(_T("File"));
	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_READ;
	ca.streamData = (TCHAR*)szFilepath.c_str();
	ca.streamType = &streamType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (EngineGetToolBox()->SendMessage(msgHash_CreateArchive, sizeof(ca), &ca) != MSG_HANDLED)
		return MSG_ERROR;

	static CHashString CalMeshCache(_T("CCal3DCoreMeshCache"));
	bool bResult = BuildObject( &hszFilepath, &CalMeshCache, ca.archive );
	ca.archive->Close();

	if (bResult)
		return MSG_HANDLED_STOP;
	else
		return MSG_ERROR;
}

/////////////////////////////////////////////////////////////
// SAVE PARTICLE EMITTER
/////////////////////////////////////////////////////////////
DWORD CCal3dMeshTextLoader::OnSaveCal3dXMFFile(DWORD size, void *params)
{
	return MSG_HANDLED_STOP;
}