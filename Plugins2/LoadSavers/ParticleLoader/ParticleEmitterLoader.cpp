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
#include "ParticleEmitterLoader.h"
#include "PerformanceProfiler.h"

REGISTER_COMPONENT_SINGLETON(CParticleEmitterLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadParticleEmitterFile, CParticleEmitterLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveParticleEmitterFile, CParticleEmitterLoader);
//REGISTER_MESSAGE_HANDLER(UnloadFile, OnUnloadParticleEmitterFile, CParticleEmitterLoader);

CHashString CParticleEmitterLoader::m_HashName(_T("CParticleEmitterLoader"));


LSRC_MESSAGE CParticleEmitterLoader::m_LoadSaveRegisterData = {
		&CParticleEmitterLoader::m_HashName,
		_T("ParticleEmitterObject Data Files (*.eml)\0" "*.eml\0"
		"\0\0"),
		_T("ParticleEmitterObject Data Files (*.eml)\0" "*.eml\0"
		"\0\0"),
		true, NULL,
};

CParticleEmitterLoader::CParticleEmitterLoader()
{
	m_ToolBox = EngineGetToolBox();

	m_ParticleEmitterMap.clear();

	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE), &m_LoadSaveRegisterData) != MSG_HANDLED)
		m_ToolBox->Log(LOGERROR, _T("%s(%d): Failed to register Load Save Extension"));
}

// create function for this Particle loader
IComponent *CParticleEmitterLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CParticleEmitterLoader );
}

IHashString *CParticleEmitterLoader::GetComponentType()
{
	return &m_HashName;
}

bool CParticleEmitterLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

/////////////////////////////////////////////////////////////
//	LOAD PARTICLE EMITTER
/////////////////////////////////////////////////////////////
DWORD CParticleEmitterLoader::OnLoadParticleEmitterFile(DWORD size, void *params)
{
	PERFORMANCE_PROFILER_AUTO(ParticleEmitterLoader_loadfile);

	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFilepath = (TCHAR *)params;

	StdString szFilename = ExtractFileName(pFilepath);
	CHashString hszFilename(szFilename);

	StdString szFilepath(pFilepath);
	CHashString hszFilepath(pFilepath);

	DWORD result;
	// Search for Object to see if it is already loaded
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &hszFilepath;
	m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );
	if ( param.bFound == true )
	{
		//particle emitter already loaded
		return MSG_HANDLED_STOP;
	}

    // Create XMLArchive using IXML
	IXMLArchive *XMLar;
	CHashString streamType(_T("File"));

	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_READ;
	ca.streamData = pFilepath;
	ca.streamType = &streamType;
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	result = EngineGetToolBox()->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca);

	if( result == MSG_ERROR )
	{
		return MSG_ERROR;
	}

	XMLar = dynamic_cast<IXMLArchive *>(ca.archive);
	// break out if XMLar is NULL.
	if( !XMLar )
	{
		return MSG_ERROR;
	}

	static CHashString m_szParticleEmitterTypeName(_T("CParticleEmitterType"));
	CREATEOBJECTPARAMS cop;
	cop.name = &hszFilepath;
	cop.parentName = NULL;
	cop.typeName = &m_szParticleEmitterTypeName;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(cop), &cop) != MSG_HANDLED)
		return MSG_ERROR;

	//Save to the map of Particle Emitters for unloading
	m_ParticleEmitterMap[hszFilepath.GetUniqueID()] = hszFilepath.GetUniqueID();

	SERIALIZEOBJECTPARAMS sop;
	sop.name = &hszFilepath;
	sop.archive = XMLar;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop) != MSG_HANDLED)
		return MSG_ERROR;

	INITOBJECTPARAMS iop;
	iop.name = &hszFilepath;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_InitObject, sizeof(iop), &iop) != MSG_HANDLED)
		return MSG_ERROR;

	XMLar->Close();
	return MSG_HANDLED_STOP;
}

/////////////////////////////////////////////////////////////
// SAVE PARTICLE EMITTER
/////////////////////////////////////////////////////////////
DWORD CParticleEmitterLoader::OnSaveParticleEmitterFile(DWORD size, void *params)
{
	return MSG_HANDLED_STOP;
}

/////////////////////////////////////////////////////////////
// UNLOAD PARTICLE EMITTER
/////////////////////////////////////////////////////////////
DWORD CParticleEmitterLoader::OnUnloadParticleEmitterFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));	
	CHashString fileName((const TCHAR *)params);

	DWORD result;

	PARTICLEEMITTERTOOBJECTMAP::iterator itr = m_ParticleEmitterMap.find( fileName.GetUniqueID() );

	if( itr != m_ParticleEmitterMap.end() )
	{
		CHashString tmpObjectName( m_ToolBox->GetHashString( itr->second ) );
		DELETEOBJECTPARAMS dop;
		dop.name = &tmpObjectName;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		result = m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof( DELETEOBJECTPARAMS ), &dop, NULL, NULL );
		m_ParticleEmitterMap.erase( itr );
		return result;
	}
	else
	{
		return MSG_ERROR;
	}
}

