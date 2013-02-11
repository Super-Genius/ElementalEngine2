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
#include "Cal3dLoader.h"
//#include "PerformanceProfiler.h"

REGISTER_COMPONENT_SINGLETON(CCal3dLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadCal3dCFGFile, CCal3dLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveCal3dCFGFile, CCal3dLoader);
REGISTER_MESSAGE_HANDLER(UnloadFile, OnUnloadCal3dCFGFile, CCal3dLoader);

CHashString CCal3dLoader::m_HashName(_T("CCal3dLoader"));

LSRC_MESSAGE CCal3dLoader::m_LoadSaveRegisterData = {
		&CCal3dLoader::m_HashName,
		_T("Cal3d Model Files (*.cfg)\0" "*.cfg\0"
		"\0\0"),
		_T("Cal3d Model Files (*.cfg)\0" "*.cfg\0"
		"\0\0"),
		true, NULL,
};

CCal3dLoader::CCal3dLoader()
{
	m_ToolBox = EngineGetToolBox();

	m_CFGMap.clear();

	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE),&m_LoadSaveRegisterData) != MSG_HANDLED)
		m_ToolBox->Log(LOGERROR, _T("%s(%d): Failed to register Load Save Extension"));
}

// create function for this Particle loader
IComponent *CCal3dLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CCal3dLoader );
}

IHashString *CCal3dLoader::GetComponentType()
{
	return &m_HashName;
}

bool CCal3dLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

/////////////////////////////////////////////////////////////
//	LOAD PARTICLE EMITTER
/////////////////////////////////////////////////////////////

/*
DWORD CCal3dLoader::OnLoadCal3dCFGFile(DWORD size, void *params)
{
//	PERFORMANCE_PROFILER_AUTO(Cal3dLoader_loadfile);

	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFilepath = (TCHAR *)params;

	StdString szFilename = ExtractFileName(pFilepath);
	CHashString hszFilename(szFilename);

	StdString szFilepath(pFilepath);
	CHashString hszFilepath(pFilepath);

	// create a new Memory Archive
	IArchive *MemArchive;
	CHashString streamType(_T("Memory"));
	CREATEARCHIVE ca;
	DWORD retVal;
	ca.streamData = NULL;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamType = &streamType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (retVal = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return MSG_ERROR;
	}
	MemArchive = (ca.archive);

	///create the cal model
	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	SERIALIZEOBJECTPARAMS sop;
	
	CHashString hszParentName( _T("World") );
	CHashString hszTypeName(_T("Cal3DRenderObject"));	

	TCHAR buf[_MAX_PATH];
	_tcscpy( buf, (TCHAR*)pFilepath );
//	ConformSlashes( buf );
	TCHAR * lwr = _tcslwr( buf );
	CHashString modelname( lwr );
	StdString szModelname( lwr );

	// only load an effect file once
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &modelname;
	m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );
	if ( param.bFound == true )
	{
		MemArchive->Close();
		// effect already loaded
		return MSG_HANDLED_STOP;
	}

	cop.parentName = &hszParentName;
	cop.typeName = &hszTypeName;
	cop.name = &modelname;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL)

	Vec3 v3Zero;
	Vec3 v3Unit;
	v3Zero.Set(.0f,.0f,.0f);
	v3Unit.Set(1.0f,1.0f,1.0f);
	StdString szShader(_T(""));

	MemArchive->Write(szModelname, _T("Filepath"));
	MemArchive->Write(v3Zero, _T("Position"));
	MemArchive->Write(v3Zero, _T("Rotation"));
	MemArchive->Write(v3Unit, _T("Scale"));
	MemArchive->Write(szShader, _T("Shader"));

	MemArchive->SetIsWriting(false);

	sop.name = &modelname;
	sop.archive = MemArchive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL)
	

	iop.name = &modelname;
	if( retval == MSG_HANDLED )
	{
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL)
	}
	MemArchive->Close();
	return MSG_HANDLED_STOP;
}
*/

DWORD CCal3dLoader::OnLoadCal3dCFGFile(DWORD size, void *params)
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

	static CHashString Cal3DCoreModelCacheType(_T("CCal3DCoreModelCache"));
	bool bResult = BuildObject( &hszFilepath, &Cal3DCoreModelCacheType, ca.archive );
	m_CFGMap[hszFilepath.GetUniqueID()] = hszFilepath.GetUniqueID();
	ca.archive->Close();

	if (bResult)
		return MSG_HANDLED_STOP;
	else
		return MSG_ERROR;
}


/////////////////////////////////////////////////////////////
// SAVE PARTICLE EMITTER
/////////////////////////////////////////////////////////////
DWORD CCal3dLoader::OnSaveCal3dCFGFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));

	return MSG_HANDLED_STOP;
}

DWORD CCal3dLoader::OnUnloadCal3dCFGFile(DWORD size, void* params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	CHashString fileName((const TCHAR *)params);
	DWORD result;

	FILETOOBJECTMAP::iterator itr = m_CFGMap.find( fileName.GetUniqueID() );

	if( itr != m_CFGMap.end() )
	{
		CHashString tmpObjectName( m_ToolBox->GetHashString( itr->second ) );
		DELETEOBJECTPARAMS dop;
		dop.name = &tmpObjectName;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		result = m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof( DELETEOBJECTPARAMS ), &dop, NULL, NULL );
		m_CFGMap.erase( itr );
		return result;
	}
	else
	{
		return MSG_ERROR;
	}

	return MSG_HANDLED_STOP;
}