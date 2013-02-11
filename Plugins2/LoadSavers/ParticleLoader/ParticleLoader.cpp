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
#include "ParticleLoader.h"
#include "PerformanceProfiler.h"

REGISTER_COMPONENT_SINGLETON(CParticleLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadParticleFile, CParticleLoader);
REGISTER_MESSAGE_HANDLER(LoadBrushFile, OnLoadParticleBrush, CParticleLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveParticleFile, CParticleLoader);
REGISTER_MESSAGE_HANDLER(SaveBrushFile, OnSaveParticleBrush, CParticleLoader);
REGISTER_MESSAGE_HANDLER(NewFile, OnNewParticleFile, CParticleLoader);
REGISTER_MESSAGE_HANDLER(NewBrushFile, OnNewParticleBrush, CParticleLoader);
//REGISTER_MESSAGE_HANDLER(UnloadFile, OnUnloadParticleFile, CParticleLoader);
//REGISTER_MESSAGE_HANDLER(UnloadBrushFile, OnUnloadParticleBrush, CParticleLoader);

CHashString CParticleLoader::m_HashName(_T("CParticleLoader"));

LSRC_MESSAGE CParticleLoader::m_LoadSaveRegisterData = {
		&CParticleLoader::m_HashName,
		_T("Particle Files (*.pml)\0" "*.pml\0"
		"\0\0"),
		_T("Particle Files (*.pml)\0" "*.pml\0"
		"\0\0"),
		true, NULL,
};

CParticleLoader::CParticleLoader()
{
	m_ToolBox = EngineGetToolBox();

	m_ParticleObjectMap.clear();

	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE), &m_LoadSaveRegisterData) != MSG_HANDLED)
		m_ToolBox->Log(LOGERROR, _T("%s(%d): Failed to register Load Save Extension"), __FILE__, __LINE__);
}

// create function for this Particle loader
IComponent *CParticleLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CParticleLoader );
}

IHashString *CParticleLoader::GetComponentType()
{
	return &m_HashName;
}

bool CParticleLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

//Check if the file is UNICODE
//for debugging purposes only.
#if 0
int IsUnicodeFile(char* szFileName)
{
    FILE *fpUnicode;
    char l_szCharBuffer[80];

    //Open the file
    if((fpUnicode= fopen(szFileName,"r")) == NULL)
    return 0; //Unable to open file

    if(!feof(fpUnicode))
    {
        fread(l_szCharBuffer,80,1,fpUnicode);
        fclose(fpUnicode);
        if(IsTextUnicode(l_szCharBuffer,80,NULL))
        {
            return 2; //Text is Unicode
        }
       else
       {
           return 1; //Text is ASCII
       }
    }
    return 0; // Some error happened
}
#endif

bool CParticleLoader::AddLoadedParticleType( IHashString *pTypeName, IHashString *pTypeTypeName, IArchive *pArchive )
{
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = pTypeName;
	if( m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param) != MSG_HANDLED)
		return false;
	if (param.bFound == true)
		return false;
	
	CREATEOBJECTPARAMS cop;
	cop.name = pTypeName;
	cop.parentName = NULL;
	cop.typeName = pTypeTypeName;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(cop), &cop) != MSG_HANDLED)
		return false;

	SERIALIZEOBJECTPARAMS sop;
	sop.name = pTypeName;
	sop.archive = pArchive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop) != MSG_HANDLED)
		return false;

	INITOBJECTPARAMS iop;
	iop.name = pTypeName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_InitObject, sizeof(iop), &iop) != MSG_HANDLED)
		return false;

	return true;
}
/////////////////////////////////////////////////////////////
//	LOAD PARTICLE EMITTER
/////////////////////////////////////////////////////////////
DWORD CParticleLoader::OnLoadParticleFile(DWORD size, void *params)
{
	PERFORMANCE_PROFILER_AUTO(ParticleLoader_loadfile);

	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFilepath = (TCHAR *)params;

	StdString szFilename = ExtractFileName(pFilepath);
	CHashString hszFilename(szFilename);

	StdString szFilepath(pFilepath);
	CHashString hszFilepath(pFilepath);

	DWORD result;
	// Search for Object to see if it is already loaded
	// Check using the FindObject Message if the CParticleEmitter exists
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &hszFilepath;
	result = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param);
	if (param.bFound == true)
	{
		//Object Exists
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

	if( result != MSG_HANDLED )
	{
		m_ToolBox->Log(LOGWARNING, _T("Failed to load particle file: \"%s\"!\n"), szFilepath.c_str() );
		return MSG_ERROR;
	}
	XMLar = dynamic_cast<IXMLArchive *>(ca.archive);
	// break out if XMLar is NULL.
	if( !XMLar )
	{
		m_ToolBox->Log(LOGWARNING, _T("Failed to load particle file: \"%s\"!\n"), szFilepath.c_str() );
		XMLar->Close();
		return MSG_ERROR;
	}

	// Create MemArchive using IAR
	IArchive *MemArchive;
	CHashString memType(_T("Memory"));

	CREATEARCHIVE caOut;
	caOut.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
	caOut.streamData = NULL;
	caOut.streamSize = 0;
	caOut.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caOut) != MSG_HANDLED)
		return MSG_ERROR;
	MemArchive = caOut.archive;

	// EMITTER PARSE VALUES
	PARTICLEPROPERTIESKEY k;
	StdString szEmitterCheck;
	StdString szEmitterName;
	int numBrushes = 0;
	int numKeyframes = 0;
	StdString	szType;
	StdString	szBrushFilename;
	StdString	szPropertyName;

	// Archive -> GetNode (this should be "CParticleEmitter" )
	XMLar->GetNode(szEmitterCheck);

	// if this is particle line type, load it
	if ( szEmitterCheck == _T("CParticleLineType") )
	{
		static CHashString hszCParticleLineType(_T("CParticleLineType"));
		AddLoadedParticleType( &hszFilepath, &hszCParticleLineType, XMLar );
		m_ParticleObjectMap[ hszFilepath.GetUniqueID() ] = hszFilepath.GetUniqueID();
		XMLar->Close();
		MemArchive->Close();
		return MSG_HANDLED_STOP;
	}

	// if this is particle ribbon type, load it
	if ( szEmitterCheck == _T("CParticleRibbonType") )
	{
		static CHashString hszCParticleRibbonType(_T("CParticleRibbonType"));
		AddLoadedParticleType( &hszFilepath, &hszCParticleRibbonType, XMLar );
		m_ParticleObjectMap[ hszFilepath.GetUniqueID() ] = hszFilepath.GetUniqueID();
		XMLar->Close();
		MemArchive->Close();
		return MSG_HANDLED_STOP;
	}

	if ( szEmitterCheck != _T("ParticleEmitter") )
	{
		// Exit out if GetNode != CParticleEmitter
		XMLar->Close();
		MemArchive->Close();
		return MSG_ERROR;
	}
	MemArchive->SetIsWriting(true);
	// PARSE HEADER
	// READ
	XMLar->Read(szEmitterName, _T("name"));
	XMLar->Read(numBrushes, _T("brushes"));
	XMLar->Read(numKeyframes, _T("keyframes"));

	// WRITE
//	MemArchive->Write( szFilename, _T("filename") );
	MemArchive->Write( szEmitterName, _T("name"));

	MemArchive->Write( PARTICLE_SERIALIZE_NORMAL, _T("flag"));

	MemArchive->Write( numBrushes, _T("brushes"));
	MemArchive->Write( numKeyframes, _T("keyframes"));

	// PARSE BRUSHES
	for (int i = 0; i < numBrushes; i++)
	{
		XMLar->GetNode(szType);
		if (szType == _T("Brush"))
		{
			// READ
			XMLar->Read(szBrushFilename, _T("file"));
			// WRITE
			MemArchive->Write( szBrushFilename, _T("file"));
		}
		// Too many keyframes or invalid brush node
		else
		{
			XMLar->Close();
			MemArchive->Close();
			return MSG_ERROR;
		}
	}
	// PARSE KEYFRAMES
	for (int i = 0 ; i < numKeyframes ; i++)
	{
		XMLar->GetNode(szType);
		if (szType == _T("Keyframe"))
		{
			// READ
			XMLar->Read(szPropertyName, _T("type"));
			XMLar->Read(k.time, _T("time"));
			XMLar->Read(k.value, _T("value"));
			// WRITE
			MemArchive->Write( szPropertyName, _T("type"));
			MemArchive->Write( k.time, _T("time"));
			MemArchive->Write( k.value, _T("value"));
		}
		// Too many keyframes or invalid keyframe node
		else
		{
			XMLar->Close();
			MemArchive->Close();
			return MSG_ERROR;
		}
	}
	XMLar->Close();

	// Change the Memarchive to read;
	MemArchive->SetIsWriting(false);

	CHashString hszParticleManager = _T("CParticleManager");
	PARTICLELOADINFO pli;
	pli.filepath = &hszFilepath;
	pli.data = MemArchive;
	static DWORD msgHash_AddLoadedEmitter = CHashString(_T("AddLoadedEmitter")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_AddLoadedEmitter, sizeof( PARTICLELOADINFO), &pli);
	if (result != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGERROR, _T("%s(%d): ParticleLoader failed to add emitter %s to manager map\n"), __FILE__, __LINE__, hszFilepath.GetString());
	}

	// Clean up temporary data.
	MemArchive->Close();

	return MSG_HANDLED_STOP;
}

/////////////////////////////////////////////////////////////
//	LOAD PARTICLE BRUSH
/////////////////////////////////////////////////////////////
DWORD CParticleLoader::OnLoadParticleBrush(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFilepath = (TCHAR *)params;

	StdString szFilename = ExtractFileName(pFilepath);
	CHashString hszFilename(szFilename);

	StdString szFilepath(pFilepath);
	CHashString hszFilepath(pFilepath);
	
	DWORD result;

	// Search for Object to see if it is already loaded
	// Check using the FindObject Message if the CParticleEmitter exists
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &hszFilename;
	result = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param);
	if (param.bFound == true)
	{
		//Object Exists
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

	if( result != MSG_HANDLED )
	{
		m_ToolBox->Log(LOGWARNING, _T("Failed to load particle file: \"%s\"!\n"), szFilepath.c_str() );
		return MSG_ERROR;
	}
	XMLar = dynamic_cast<IXMLArchive *>(ca.archive);
	// break out if XMLar is NULL.
	if( !XMLar )
	{
		m_ToolBox->Log(LOGWARNING, _T("Failed to load particle file: \"%s\"!\n"), szFilepath.c_str() );
		XMLar->Close();
		return MSG_ERROR;
	}

	// Create MemArchive using IAR
	IArchive *MemArchive;
	CHashString memType(_T("Memory"));

	CREATEARCHIVE caOut;
	caOut.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
	caOut.streamData = NULL;
	caOut.streamSize = 0;
	caOut.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caOut) != MSG_HANDLED)
	{
		XMLar->Close();
		return MSG_ERROR;
	}
	MemArchive = caOut.archive;

	// BRUSH PARSE VALUES
	StdString szType;
	StdString szPropertyName;
	StdString szBrushCheck;
	StdString szBrushName;
	bool additive;
	int nKeyframes;
	float framerate;
	float texturerate;
	StdString szTexturePath;
	bool loop;
	float AttachToEmitter;
	StdString szShader;
	PARTICLEPROPERTIESKEY k;

	// Archive -> GetNode (this should be "ParticleBrush" )
	XMLar->GetNode(szBrushCheck);
	if ( szBrushCheck != _T("ParticleBrush") )
	{
		// Exit out if GetNode != ParticleBrush
		XMLar->Close();
		MemArchive->Close();
		return MSG_ERROR;
	}

	MemArchive->SetIsWriting(true);
	// PARSE HEADER
	// READ
	XMLar->Read( szBrushName, _T("name"));
	XMLar->Read( additive, _T("additive"));
	XMLar->Read( framerate, _T("framerate"));
	XMLar->Read( texturerate, _T("texturerate"));
	XMLar->Read( szTexturePath, _T("texture"));
	XMLar->Read( loop, _T("loop"));
	XMLar->Read( AttachToEmitter, _T("AttachToEmitter"));
	XMLar->Read( szShader, _T("shader"));

	XMLar->Read( nKeyframes, _T("keyframes"));

	// WRITE
//	MemArchive->Write( szFilename, _T("filename") );
//	MemArchive->Write( szFilepath, _T("filepath") );

	MemArchive->Write( szBrushName, _T("name"));
	MemArchive->Write( additive, _T("additive"));
	MemArchive->Write( framerate, _T("framerate"));
	MemArchive->Write( texturerate, _T("texturerate"));
	MemArchive->Write( szTexturePath, _T("texture"));
	MemArchive->Write( loop, _T("loop"));
	MemArchive->Write( AttachToEmitter, _T("AttachToEmitter"));
	MemArchive->Write( szShader, _T("shader"));

	MemArchive->Write( PARTICLE_SERIALIZE_NORMAL, _T("flag"));

	MemArchive->Write( nKeyframes, _T("keyframes"));

	// PARSE KEYFRAMES
	for (int i = 0 ; i < nKeyframes ; i++)
	{
		XMLar->GetNode(szType);
		if (szType == _T("Keyframe"))
		{
			// READ
			XMLar->Read(szPropertyName, _T("type"));
			XMLar->Read(k.time, _T("time"));
			XMLar->Read(k.value, _T("value"));
			// WRITE
			MemArchive->Write( szPropertyName, _T("type"));
			MemArchive->Write( k.time, _T("time"));
			MemArchive->Write( k.value, _T("value"));
		}
		// Too many keyframes or invalid keyframe node
		else
		{
			XMLar->Close();
			MemArchive->Close();
			return MSG_ERROR;
		}
	}
	XMLar->Close();

	// Change the Memarchive to read;
	MemArchive->SetIsWriting(false);

	CHashString hszParticleManager = _T("CParticleManager");
	PARTICLELOADINFO pli;
	pli.filepath = &hszFilepath;
	pli.data = MemArchive;
	static DWORD msgHash_AddLoadedBrush = CHashString(_T("AddLoadedBrush")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_AddLoadedBrush, sizeof( PARTICLELOADINFO), &pli);
	if (result != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGERROR, _T("ParticleLoader failed to add emitter to manager map"));
	}

	// Object Serialized, close memarchive;
	MemArchive->Close();

	return MSG_HANDLED_STOP;
}

/////////////////////////////////////////////////////////////
// SAVE PARTICLE BRUSH
/////////////////////////////////////////////////////////////
DWORD CParticleLoader::OnSaveParticleBrush(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFilepath = (TCHAR *)params;

	StdString szFilepath(pFilepath);
	CHashString hszFilepath(pFilepath);

	const TCHAR * czRelativeFilePath;
	czRelativeFilePath = m_ToolBox->GetRelativePath(pFilepath);

	StdString szRelativeFilepath(czRelativeFilePath);
	CHashString hszRelativeFilepath(czRelativeFilePath);

	DWORD result;
	
	// Check to see that Brush Exists
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &hszRelativeFilepath;
	result = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param);
	if (param.bFound == false)
	{
		//Object does not exist to save out.
		return MSG_ERROR;
	}

	// Create XMLArchive using IXML
	IXMLArchive *XMLar;
	CHashString streamType(_T("File"));

	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_WRITE;
	ca.streamData = pFilepath;
	ca.streamType = &streamType;
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	result = EngineGetToolBox()->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca);
	if( result == MSG_ERROR )
	{
		return MSG_ERROR;
	}

	XMLar = dynamic_cast<IXMLArchive *>(ca.archive);
	if( !XMLar )
	{
		return MSG_ERROR;
	}

	// Create MemArchive using IAR
	IArchive *MemArchive;
	CHashString memType(_T("Memory"));

	CREATEARCHIVE caOut;
	caOut.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
	caOut.streamData = NULL;
	caOut.streamSize = 0;
	caOut.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caOut) != MSG_HANDLED)
	{
		XMLar->Close();
		return MSG_ERROR;
	}
	MemArchive = caOut.archive;

	// Change the Memarchive to write
	MemArchive->SetIsWriting(true);

	// Serialize out Emitter Data
	SERIALIZEOBJECTPARAMS sop;
	sop.archive = MemArchive;
	sop.name = &hszRelativeFilepath;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);
	if (result != MSG_HANDLED)
	{
		XMLar->Close();
		MemArchive->Close();
		return MSG_ERROR;
	}

	// EMITTER PARSE VALUES
	StdString	szName(_T("ParticleBrush"));
	StdString	szKeyframe(_T("Keyframe"));

	// BRUSH PARSE VALUES
//	StdString szDummy;
	StdString szType;
	StdString szPropertyName;
	StdString szBrushName;
	bool additive;
	int nKeyframes;
	float framerate;
	float texturerate;
	StdString szTexturePath;
	bool loop;
	float AttachToEmitter;
	StdString szShader;

	float time;
	float value;
	
	// Change the Memarchive to read
	MemArchive->SetIsWriting(false);

	// WRITE
//	MemArchive->Read( szDummy, _T("filename") );
//	MemArchive->Read( szDummy, _T("filepath") );

	MemArchive->Read( szBrushName, _T("name"));
	MemArchive->Read( additive, _T("additive"));
	MemArchive->Read( framerate, _T("framerate"));
	MemArchive->Read( texturerate, _T("texturerate"));
	MemArchive->Read( szTexturePath, _T("texture"));
	MemArchive->Read( loop, _T("loop"));
	MemArchive->Read( AttachToEmitter, _T("AttachToEmitter"));
	MemArchive->Read( szShader, _T("shader"));

	MemArchive->Read( nKeyframes, _T("keyframes"));

	XMLar->StartClass( szName );
		XMLar->Write( szBrushName, _T("name"));
		XMLar->Write( additive, _T("additive"));
		XMLar->Write( framerate, _T("framerate"));
		XMLar->Write( texturerate, _T("texturerate"));
		XMLar->Write( szTexturePath, _T("texture"));
		XMLar->Write( loop, _T("loop"));
		XMLar->Write( AttachToEmitter, _T("AttachToEmitter"));
		XMLar->Write( szShader, _T("shader"));

		XMLar->Write( nKeyframes, _T("keyframes"));

		for ( int i = 0; i < nKeyframes; i++ )
		{
			MemArchive->Read( szPropertyName, _T("type") );
			MemArchive->Read( time, _T("time") );
			MemArchive->Read( value, _T("value") );

			XMLar->StartClass( szKeyframe );
				XMLar->Write( szPropertyName, _T("type") );
				XMLar->Write( time, _T("time") );
				XMLar->Write( value, _T("value") );
			XMLar->EndClass();
		}
	XMLar->EndClass();

	MemArchive->Close();
	XMLar->Close();

	return MSG_HANDLED_STOP;
}

/////////////////////////////////////////////////////////////
// SAVE PARTICLE EMITTER
/////////////////////////////////////////////////////////////
DWORD CParticleLoader::OnSaveParticleFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFilepath = (TCHAR *)params;

	StdString szFilename = ExtractFileName(pFilepath);
	CHashString hszFilename(szFilename);

	StdString szFilepath(pFilepath);
	CHashString hszFilepath(pFilepath);

	DWORD result;
	
	// Check to see that Emitter Exists
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &hszFilepath;
	result = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param);
	if (param.bFound == false)
	{
		//Object does not exist to save out.
		return MSG_ERROR;
	}

	// Create XMLArchive using IXML
	IXMLArchive *XMLar;
	CHashString streamType(_T("File"));

	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_WRITE;
	ca.streamData = pFilepath;
	ca.streamType = &streamType;
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	result = EngineGetToolBox()->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca);
	if( result == MSG_ERROR )
	{
		return MSG_ERROR;
	}

	XMLar = dynamic_cast<IXMLArchive *>(ca.archive);
	if( !XMLar )
	{
		return MSG_ERROR;
	}

	// Create MemArchive using IAR
	IArchive *MemArchive;
	CHashString memType(_T("Memory"));

	CREATEARCHIVE caOut;
	caOut.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
	caOut.streamData = NULL;
	caOut.streamSize = 0;
	caOut.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caOut) != MSG_HANDLED)
	{
		XMLar->Close();
		return MSG_ERROR;
	}
	MemArchive = caOut.archive;

	// Change the Memarchive to write
	MemArchive->SetIsWriting(true);

	// Serialize out Emitter Data
	SERIALIZEOBJECTPARAMS sop;
	sop.archive = MemArchive;
	sop.name = &hszFilepath;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);

	// EMITTER PARSE VALUES
	StdString	szName(_T("ParticleEmitter"));
	StdString	szBrush(_T("Brush"));
	StdString	szKeyframe(_T("Keyframe"));

//	StdString	szDummy;
	float time = 0.0f;
	float value = 0.0f;
	StdString szEmitterName;
	int numBrushes = 0;
	int numKeyframes = 0;
	StdString	szBrushFilepath;
	StdString	szPropertyName;
	
	// Change the Memarchive to read
	MemArchive->SetIsWriting(false);

//	MemArchive->Read( szDummy, _T("filename"));
	MemArchive->Read( szEmitterName, _T("name"));
	MemArchive->Read( numBrushes, _T("brushes"));
	MemArchive->Read( numKeyframes, _T("keyframes"));

	XMLar->StartClass(szName);
		XMLar->Write(szEmitterName, _T("name"));
		XMLar->Write(numBrushes, _T("brushes"));
		XMLar->Write(numKeyframes, _T("keyframes"));

		for (int i = 0; i < numBrushes; i++)
		{	
			MemArchive->Read( szBrushFilepath, _T("Brush") );

			XMLar->StartClass( szBrush );
				XMLar->Write( szBrushFilepath, _T("file") );
			XMLar->EndClass();
		}

		for ( int i = 0; i < numKeyframes; i++ )
		{
			MemArchive->Read( szPropertyName, _T("type") );
			MemArchive->Read( time, _T("time") );
			MemArchive->Read( value, _T("value") );

			XMLar->StartClass( szKeyframe );
				XMLar->Write( szPropertyName, _T("type") );
				XMLar->Write( time, _T("time") );
				XMLar->Write( value, _T("value") );
			XMLar->EndClass();
		}
	XMLar->EndClass();

	MemArchive->Close();
	XMLar->Close();

	return MSG_HANDLED_STOP;
}

/////////////////////////////////////////////////////////////
// New Particle File
/////////////////////////////////////////////////////////////
DWORD CParticleLoader::OnNewParticleFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *czFilepath = (TCHAR *)params;

	StdString szFilename = ExtractFileName(czFilepath);
	CHashString hszFilename(szFilename);

	StdString szFilepath(czFilepath);
	CHashString hszFilepath(czFilepath);

	CREATE_EMITTER_STRING_TABLE(EmitterPropertyStringTable);

	DWORD result;
	// Search for Object to see if it is already loaded
	// Check using the FindObject Message if the CParticleEmitter exists
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &hszFilename;
	result = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param);
	if (param.bFound == true)
	{
		//Object Exists
		return MSG_HANDLED_STOP;
	}

	// Use Create Object Message to create the object (use the filename as the name)
	CHashString hszTypeName(_T("CParticleEmitter"));
	CREATEOBJECTPARAMS cop;
	cop.name = &hszFilename;
	cop.parentName = NULL;
	cop.typeName = &hszTypeName;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
	if (result == MSG_ERROR)
		return MSG_ERROR;

	// Create MemArchive using IAR
	IArchive *MemArchive;
	CHashString memType(_T("Memory"));

	CREATEARCHIVE caOut;
	caOut.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
	caOut.streamData = NULL;
	caOut.streamSize = 0;
	caOut.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caOut);
	if (result == MSG_ERROR)
		return MSG_ERROR;

	MemArchive = caOut.archive;

	// Change the Memarchive to write
	MemArchive->SetIsWriting(true);

	// Write Initial Emitter Properties:
	// WRITE INTERNAL DATA
	MemArchive->Write( szFilename, _T("filename") );

	// WRITE FILE HEADER
	MemArchive->Write( _T("NewEmitter"), _T("name"));
	MemArchive->Write( 1, _T("brushes"));
	MemArchive->Write( EMITTER_VARIABLE_MAX, _T("keyframes"));

	// WRITE BRUSHES
	for (int i = 0; i < 1; i++)
	{
		MemArchive->Write(_T("NewEmitter_NewBrush.pml"), _T("file"));
	}

	// WRITE PROPERTIES
	for (int i = 0; i < EMITTER_VARIABLE_MAX; i++)
	{
		MemArchive->Write(EmitterPropertyStringTable[i], _T("type"));
		MemArchive->Write(0.0f, _T("time"));
		MemArchive->Write(0.0f, _T("value"));

	}

	// Change the Memarchive to read
	MemArchive->SetIsWriting(false);

	// Serialize out Emitter Data
	SERIALIZEOBJECTPARAMS sop;
	sop.archive = MemArchive;
	sop.name = &hszFilename;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);
	if (result == MSG_ERROR)
	{
		MemArchive->Close();
		return MSG_ERROR;
	}

	// Object Serialized, close memarchive;
	MemArchive->Close();

	// Use the Init Message with the MemArchive
	INITOBJECTPARAMS iop;
	iop.name = &hszFilename;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);


	return MSG_HANDLED_STOP;
}

/////////////////////////////////////////////////////////////
// New Particle Brush
/////////////////////////////////////////////////////////////
DWORD CParticleLoader::OnNewParticleBrush(DWORD size, void *params)
{

	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *czFilepath = (TCHAR *)params;

	StdString szFilename = ExtractFileName(czFilepath);
	CHashString hszFilename(szFilename);

	StdString szFilepath(czFilepath);
	CHashString hszFilepath(czFilepath);

	CREATE_PARTICLE_STRING_TABLE(BrushPropertyStringTable);

	DWORD result;
	// Search for Object to see if it is already loaded
	// Check using the FindObject Message if the CParticleEmitter exists
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &hszFilename;
	result = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param);
    if (param.bFound == true)
	{
		//Object Exists
		return MSG_HANDLED_STOP;
	}

	// Use Create Object Message to create the object (use the filename as the name)
	CHashString hszTypeName(_T("CParticleType"));
	CREATEOBJECTPARAMS cop;
	cop.name = &hszFilename;
	cop.parentName = NULL;
	cop.typeName = &hszTypeName;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
	if (result == MSG_ERROR)
		return MSG_ERROR;

	// Create MemArchive using IAR
	IArchive *MemArchive;
	CHashString memType(_T("Memory"));

	CREATEARCHIVE caOut;
	caOut.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
	caOut.streamData = NULL;
	caOut.streamSize = 0;
	caOut.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caOut);
	if (result == MSG_ERROR)
		return MSG_ERROR;

	MemArchive = caOut.archive;

	// Change the Memarchive to write
	MemArchive->SetIsWriting(true);

	// Write Initial Emitter Properties:
	// WRITE INTERNAL DATA
	MemArchive->Write( szFilename, _T("filename") );
	MemArchive->Write( szFilepath, _T("filepath") );

	// WRITE FILE HEADER
	MemArchive->Write( _T("NewEmitter"), _T("name"));
	MemArchive->Write( false, _T("additive"));
	MemArchive->Write( 30.0f, _T("framerate"));
	MemArchive->Write( 30.0f, _T("texturerate"));
	MemArchive->Write( _T(""), _T("texture"));
	MemArchive->Write( false, _T("loop"));
	MemArchive->Write( 0.0f, _T("AttachToEmitter"));
	MemArchive->Write( _T(""), _T("shader"));

	MemArchive->Write( PARTICLE_VARIABLE_MAX, _T("keyframes"));

	// WRITE PROPERTIES
	for (int i = 0; i < PARTICLE_VARIABLE_MAX; i++)
	{
		MemArchive->Write(BrushPropertyStringTable[i], _T("type"));
		MemArchive->Write(0.0f, _T("time"));
		MemArchive->Write(0.0f, _T("value"));
	}

	// Change the Memarchive to read
	MemArchive->SetIsWriting(false);

	// Serialize out Emitter Data
	SERIALIZEOBJECTPARAMS sop;
	sop.archive = MemArchive;
	sop.name = &hszFilename;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);
	if (result == MSG_ERROR)
	{
		MemArchive->Close();
		return MSG_ERROR;
	}

	// Object Serialized, close memarchive;
	MemArchive->Close();

	// Use the Init Message with the MemArchive
	INITOBJECTPARAMS iop;
	iop.name = &hszFilename;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);

	return MSG_HANDLED_STOP;
}

DWORD CParticleLoader::OnUnloadParticleFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));	
	CHashString fileName((const TCHAR *)params);

	DWORD result;

	PARTICLETOOBJECTMAP::iterator itr = m_ParticleObjectMap.find( fileName.GetUniqueID() );

	if( itr != m_ParticleObjectMap.end() )
	{
		CHashString tmpObjectName( m_ToolBox->GetHashString( itr->second ) );
		DELETEOBJECTPARAMS dop;
		dop.name = &tmpObjectName;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		result = m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof( DELETEOBJECTPARAMS ), &dop, NULL, NULL );
		m_ParticleObjectMap.erase( itr );
		return result;
	}
	else
	{
		return MSG_ERROR;
	}

	return MSG_HANDLED_STOP;
}

DWORD CParticleLoader::OnUnloadParticleBrush(DWORD size, void *params)
{

	return MSG_HANDLED_STOP;
}
