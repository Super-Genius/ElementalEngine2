///============================================================================
/// \file		XMLWorldLoader.cpp
/// \brief		Implementation file for XML based World Loader
/// \date		04-04-2005
/// \author		Kenneth Hurley
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
#include "Vec3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON(CXMLWorldLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadWorldFile, CXMLWorldLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveWorldFile, CXMLWorldLoader);
REGISTER_MESSAGE_HANDLER(GetWorldVersion, OnGetWorldVersion, CXMLWorldLoader);
REGISTER_MESSAGE_HANDLER(GetLatestVersion, OnGetLatestVersion, CXMLWorldLoader);

CHashString CXMLWorldLoader::m_HashName(_T("CXMLWorldLoader"));

LSRC_MESSAGE CXMLWorldLoader::m_LoadSaveRegisterData = {
		&CXMLWorldLoader::m_HashName,
		_T("XML World Files (*.wxl)\0" "*.wxl\0"
		"\0\0"),
		_T("XML World Files (*.wxl)\0" "*.wxl\0"
		"\0\0"),
		false, NULL,
};

CXMLWorldLoader::CXMLWorldLoader() 
{
	// save for convenience
	m_ToolBox = EngineGetToolBox();
	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE),&m_LoadSaveRegisterData);

	m_WorldFileVersion = 0.0f;
	m_fLoadingUpdateFPS = 20.0f;
}

// create function for this world loader
IComponent *CXMLWorldLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CXMLWorldLoader );
}

IHashString *CXMLWorldLoader::GetComponentType()
{
	return &m_HashName;
}

bool CXMLWorldLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// load a file if we recognize the type.
DWORD CXMLWorldLoader::OnLoadWorldFile(DWORD size, void *params)
{
	CHashString oldVersion = _T("");
	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	SERIALIZEOBJECTPARAMS sop;
	DWORD retVal;
	IXMLArchive *Archive = NULL;
	DWORD currNode = 0;
	LOADINGUPDATEPARAMS pbp;

	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;

	// use both a hashstring and a pointer so that the first iteration of the loop
	// will pass SetParentName a NULL instead of an address of a hashstring
	// hashed to ""
	CHashString hszParentType(_T("CWorld"));
	CHashString hszParentName(_T("World"));
	CHashString hszName(_T(""));
	CHashString hszTypeName(_T(""));
	CHashString streamType(_T("File"));

	// delete previous world root
	DELETEOBJECTPARAMS dop;
	dop.name = &hszParentName;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop );
	// delete is ok to fail if no world has been created yet

	// create new world root
	cop.name = &hszParentName;
	cop.parentName = NULL;
	cop.typeName = &hszParentType;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	DWORD returnValue = EngineGetToolBox()->SendMessage(msgHash_CreateObject, sizeof(cop), &cop );
	if (returnValue != MSG_HANDLED)
	{
		EngineGetToolBox()->Log(LOGERROR, _T("Failed to create World Root Object\n"));
		return MSG_ERROR;
	}

	CREATEARCHIVE ca;
	ca.streamData = (void *)pFileName;
	ca.mode = STREAM_MODE_READ;
	ca.streamType = &streamType;
#ifdef USEXMLFILESTREAMS
	// call the Archive factory to create an XML archive
	static DWORD msgHash_CreateXMLArchiveStream = CHashString(_T("CreateXMLArchiveStream")).GetUniqueID();
	if (retVal = m_ToolBox->SendMessage(msgHash_CreateXMLArchiveStream, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
#else
	// call the Archive factory to create an XML archive
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (retVal = m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
#endif
	{
		return retVal;
	}

	Archive = dynamic_cast<IXMLArchive *>(ca.archive);
	DWORD numNodes = Archive->GetNumberOfNodes();

	// Deteriminte World Version
	int iWorldNumAttrib = Archive->GetNumAttributes();

	StdString szNodeName;
	Archive->GetNode(szNodeName);
	
	pbp.currentPosition = Archive->GetCurrentNodePosition();
	pbp.totalSize = numNodes;
	// update progress bar
	static DWORD msgHash_LoadingUpdate = CHashString(_T("LoadingUpdate")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_LoadingUpdate, sizeof(LOADINGUPDATEPARAMS), &pbp);


	// must start with <World>
	if (szNodeName != _T("CWorld"))
	{
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_FILE);
		EngineGetToolBox()->Log(LOGWARNING, _T("Invalid world file %s\n"), pFileName);
		Archive->Close();
		return MSG_ERROR;
	}

	// Version 1.0
	if( iWorldNumAttrib == 1 )
	{
		// read the name of the world
		StdString szName;
		Archive->Read( szName, _T("Name") );
		m_WorldFileVersion = 1.0f;
		static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &oldVersion);
		if (retval != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGERROR, _T("Could not get world version!"));
			return MSG_ERROR;
		}
		CHashString hszNewVersion = _T("1.0");
		static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &hszNewVersion);
		if (retval != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGERROR, _T("Could not set world version!"));
			return MSG_ERROR;
		}
	}
	// Version 1.5
	else if( iWorldNumAttrib == 2 )
	{
		StdString szName;
		Archive->Read( szName, _T("Name") );
		float fileVersion = 0;
		Archive->Read( m_WorldFileVersion, _T("Version") );
		static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &oldVersion);
		if (retval != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGERROR, _T("Could not get world version!"));
			return MSG_ERROR;
		}
		TCHAR buf[50];
		_stprintf(buf, "%.1f", m_WorldFileVersion);
		CHashString hszNewVersion(buf);
		static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &hszNewVersion);
		if (retval != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGERROR, _T("Could not set world version!"));
			return MSG_ERROR;
		}
	}
	// Unknown Version
	else
	{
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_FILE);
		EngineGetToolBox()->Log(LOGWARNING, _T("Invalid world file %s\n"), pFileName);
		Archive->Close();
		static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &oldVersion);
		return MSG_ERROR;
	}

	// failure checks
	bool componentFailed = false;
	int failedDepth = 0;

	float secondsPerFrame = 1 / m_fLoadingUpdateFPS;
	ITimer *timer = EngineGetToolBox()->GetTimer();
	float lastLoadTime = timer->GetRealTime();
	float elapsedTime = 0.0f;

	while (Archive->GetNode(szNodeName))
	{
		int currDepth = Archive->GetDepth();
		if (componentFailed == true)
		{
			// current node is child of failed
			if (currDepth > failedDepth)
			{
				continue;
			}
			// if sibling or uncle, reset comp failed flag
			else
			{
				componentFailed = false;
			}
		}

		hszTypeName.Init( szNodeName.c_str() );

		StdString parentName;
		StdString parentType;
		StdString childType;
		Archive->GotoParentNode( parentType );
		Archive->Read( parentName, _T("Name") );
		if ( parentName != _T("") )
		{
			hszParentName = parentName;
		}
		Archive->ReturnToChildNode( childType );
		if( hszTypeName.GetString() != childType )
		{
			assert( hszTypeName.GetString() == childType );
			static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &oldVersion);
			return MSG_ERROR;
		}
		//parentName = Archive->GetParentName();

		StdString szName;
		Archive->Read( szName, _T("Name") );
		hszName.Init( szName.c_str() );

		cop.parentName = &hszParentName;
		cop.typeName = &hszTypeName;
		cop.name = &hszName;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);

		if (retval != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGINFORMATION, _T("World loader: could not create component; skipping children!\n"));
			componentFailed = true;
			failedDepth = Archive->GetDepth();			
			continue;
		}

		// 1.0 -> 1.5 Readjusted scale and position on object by 1000/898
		if( m_WorldFileVersion == 1.0 )
		{
			float fConvert = 1000.0f/898.0f;
			if( hszTypeName.GetUniqueID() == CHashString(_T("CV3ORenderObject")).GetUniqueID() )
			{
				StdString szFileName;
				Vec3 vPosition(0,0,0);
				Vec3 vRotation(0,0,0);
				Vec3 vScaling(0,0,0);

				Archive->Read( szFileName, _T("FileName") );
				
				Archive->Read( vPosition, _T("Position") );
				vPosition.x = ( vPosition.x * fConvert );
				vPosition.y = ( vPosition.y * fConvert );
				vPosition.z = ( vPosition.z * fConvert );
				
				Archive->Read( vRotation, _T("Rotation") );

				Archive->Read( vScaling, _T("Scaling") );
				vScaling.x = ( vScaling.x * fConvert );
				vScaling.y = ( vScaling.y * fConvert );
				vScaling.z = ( vScaling.z * fConvert );

				IArchive *MemArchive;
				CHashString memType(_T("Memory"));

				CREATEARCHIVE caOut;
				caOut.mode = STREAM_MODE_WRITE;
				caOut.streamData = NULL;
				caOut.streamSize = 0;
				caOut.streamType = &memType;
				static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
				if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caOut) != MSG_HANDLED)
				{
					static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &oldVersion);
					return true;
				}
				MemArchive = caOut.archive;

				MemArchive->Write( szFileName, _T("FileName") );
				MemArchive->Write( vPosition, _T("Position") );
				MemArchive->Write( vRotation, _T("Rotation") );
				MemArchive->Write( vScaling, _T("Scaling") );
		
				CREATESTREAM cs;
				cs.streamData = caOut.streamData;
				cs.streamSize = caOut.streamSize;
				cs.mode = STREAM_MODE_READ;
				static DWORD msgHash_CreateStream_Memory = CHashString(_T("CreateStream_Memory")).GetUniqueID();
				if (m_ToolBox->SendMessage(msgHash_CreateStream_Memory, sizeof(CREATESTREAM), &cs) != MSG_HANDLED)
				{
					static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &oldVersion);
					return true;
				}
				MemArchive->Init(cs.openStream);

				sop.name = &hszName;
				sop.archive = MemArchive;
				static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);
				
				MemArchive->Close();
			}

			else if( hszTypeName.GetUniqueID() == CHashString(_T("CPhysicsObject")).GetUniqueID() )
			{
				//StdString szDynamics;
				StdString szType;
				StdString szShapeFileName;
				//float fMass;
				//Vec3 vPosition;
				//Vec3 vRotation;
				//Vec3 vScaling;

				//Archive->Read( vPosition, _T("pos") );
				//vPosition.SetX( vPosition.GetX() * fConvert );
				//vPosition.SetY( vPosition.GetY() * fConvert );
				//vPosition.SetZ( vPosition.GetZ() * fConvert );

				//Archive->Read( vRotation, _T("rot") );
				
				//Archive->Read( vScaling, _T("scale") );
				//vScaling.SetX( vScaling.GetX() * fConvert );
				//vScaling.SetY( vScaling.GetY() * fConvert );
				//vScaling.SetZ( vScaling.GetZ() * fConvert );

				//Archive->Read( fMass, _T("mass") );
				Archive->Read( szType, _T("Type") );
				Archive->Read( szShapeFileName, _T("filename") );

				IArchive *MemArchive;
				CHashString memType(_T("Memory"));
				//int size =	sizeof(char) * 256 * 2 + sizeof(float) + sizeof(Vec3) * 3 + 256;
				
				CREATEARCHIVE caOut;
				caOut.mode = STREAM_MODE_WRITE;
				caOut.streamData = NULL;
				caOut.streamSize = 0;
				caOut.streamType = &memType;
				static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
				if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caOut) != MSG_HANDLED)
				{
					static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &oldVersion);
					return true;
				}
				MemArchive = caOut.archive;

				//MemArchive->Write( vPosition, _T("pos") );
				//MemArchive->Write( vRotation, _T("rot") );
				//MemArchive->Write( vScaling, _T("scale") );
				//MemArchive->Write( fMass, _T("mass") );
				//MemArchive->Write( szDynamics, _T("dynamics") );
				//MemArchive->Write( szShapeFileName, _T("shapeFile") );
				MemArchive->Write( szType, _T("Type") );
				MemArchive->Write( szShapeFileName, _T("filename") );

				CREATESTREAM cs;
				cs.streamData = caOut.streamData;
				cs.streamSize = caOut.streamSize;
				cs.mode = STREAM_MODE_READ;
				static DWORD msgHash_CreateStream_Memory = CHashString(_T("CreateStream_Memory")).GetUniqueID();
				if (m_ToolBox->SendMessage(msgHash_CreateStream_Memory, sizeof(CREATESTREAM), &cs) != MSG_HANDLED)
				{
					static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &oldVersion);
					return true;
				}
				MemArchive->Init(cs.openStream);

				sop.name = &hszName;
				sop.archive = MemArchive;
				static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);

				MemArchive->Close();
			}
			else if( hszTypeName.GetUniqueID() == CHashString(_T("CTerrainConfig")).GetUniqueID() )
			{
				StdString szTerShaderLib;
				StdString szTerShaderEff;
				int iSectorRes;
				float fSectorSize;
				float fZScale;
				float fLODDistance;

				Archive->Read( szTerShaderLib, _T("TerrainShaderLibrary") );
				Archive->Read( szTerShaderEff, _T("TerrainShaderEffect") );
				Archive->Read( iSectorRes, _T("SectorResolution") );
				Archive->Read( fSectorSize, _T("SectorSize") );
				fSectorSize = fSectorSize * fConvert;
				Archive->Read( fZScale, _T("ZScale") );
				fZScale = fZScale * fConvert;
				Archive->Read( fLODDistance, _T("LODDistance") );
				
				IArchive *MemArchive;
				CHashString memType(_T("Memory"));
				
				CREATEARCHIVE caOut;
				caOut.mode = STREAM_MODE_WRITE;
				caOut.streamData = NULL;
				caOut.streamSize = 0;
				caOut.streamType = &memType;
				static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
				if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caOut) != MSG_HANDLED)
				{
					static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &oldVersion);
					return true;
				}
				MemArchive = caOut.archive;

				MemArchive->Write( szTerShaderLib, _T("TerrainShaderLibrary") );
				MemArchive->Write( szTerShaderEff, _T("TerrainShaderEffect") );
				MemArchive->Write( iSectorRes, _T("SectorResolution") );
				MemArchive->Write( fSectorSize, _T("SectorSize") );
				MemArchive->Write( fZScale, _T("ZScale") );
				MemArchive->Write( fLODDistance, _T("LODDistance") );

				CREATESTREAM cs;
				cs.streamData = caOut.streamData;
				cs.streamSize = caOut.streamSize;
				cs.mode = STREAM_MODE_READ;
				static DWORD msgHash_CreateStream_Memory = CHashString(_T("CreateStream_Memory")).GetUniqueID();
				if (m_ToolBox->SendMessage(msgHash_CreateStream_Memory, sizeof(CREATESTREAM), &cs) != MSG_HANDLED)
				{
					static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &oldVersion);
					return true;
				}
				MemArchive->Init(cs.openStream);

				sop.name = &hszName;
				sop.archive = MemArchive;
				static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);

				MemArchive->Close();
			}
			else
			{
				sop.name = &hszName;
				sop.archive = Archive;
				static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);
			}
		}
		else
		{
			sop.name = &hszName;
			sop.archive = Archive;
			static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);
		}

		iop.name = &hszName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);

		elapsedTime += timer->GetRealTime() - lastLoadTime;
		lastLoadTime = timer->GetRealTime();				
		if (elapsedTime > secondsPerFrame)
		{
			elapsedTime = 0.0f;
			pbp.currentPosition = Archive->GetCurrentNodePosition();
			pbp.totalSize = numNodes;
			m_ToolBox->SendMessage(msgHash_LoadingUpdate, sizeof(LOADINGUPDATEPARAMS), &pbp);
		}
	}

	pbp.currentPosition = 0;
	pbp.totalSize = 0;
	m_ToolBox->SendMessage(msgHash_LoadingUpdate, sizeof(LOADINGUPDATEPARAMS), &pbp);

	Archive->Close();

	static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &oldVersion);

	return MSG_HANDLED_STOP;
}

// Save a file if we recognize the type.
DWORD CXMLWorldLoader::OnSaveWorldFile(DWORD size, void *params)
{
	// Check the message to verify the size
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;

	if( pFileName == NULL )
	{
		return MSG_ERROR;
	}

	IXMLArchive *XMLOutput;
	CREATEARCHIVE ca;
	ca.streamData = pFileName;
	ca.mode = STREAM_MODE_WRITE;

	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return MSG_ERROR;
	}

	XMLOutput = dynamic_cast<IXMLArchive *>(ca.archive);

	CWorldVisitor WorldVisitor;
	WorldVisitor.SetArchiver( XMLOutput );
	WorldVisitor.SetWorldVersion( g_fWorldVersion );
	
	CHashString hszWorldRootName(_T("World"));
	VISITHIERARCHYDATA vhd;
	vhd.name = NULL;
	vhd.visitor = &WorldVisitor;
	vhd.rootNodeName = &hszWorldRootName; // only visit the hierarchy starting from "World" node
	static DWORD msgHash_VisitHierarchy = CHashString(_T("VisitHierarchy")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_VisitHierarchy, sizeof(VISITHIERARCHYDATA), &vhd, NULL, NULL);

	XMLOutput->Close();

	return MSG_HANDLED_STOP;
}

DWORD CXMLWorldLoader::OnGetWorldVersion(DWORD size, void *params)
{
	// Check the message to verify the size
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float *version = (float*)params;

	*version = m_WorldFileVersion;

	return MSG_HANDLED_STOP;
}

DWORD CXMLWorldLoader::OnGetLatestVersion(DWORD size, void *params)
{
	// Check the message to verify the size
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float *version = (float*)params;

	*version = g_fWorldVersion;

	return MSG_HANDLED_STOP;
}
