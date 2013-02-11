///============================================================================
/// \file		CWorldLoader.cpp
/// \brief		Implementation of visitor pattern to load a WXL into a heirarchy
/// \date		08-09-2005
/// \author		Patrick Ghiocel
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

#include "WorldLoader.h"
#include "WorldVisitor.h"

CWorldLoader::CWorldLoader()
{
	m_ToolBox = EngineGetToolBox();
}

bool CWorldLoader::LoadWorld( StdString wszFilename )
{
	CREATEOBJECTPARAMS cop;
	SERIALIZEOBJECTPARAMS sop;
	DWORD retVal;
	IXMLArchive *Archive = NULL;

	TCHAR *pFileName = const_cast<TCHAR*>((const TCHAR*)wszFilename);

	// use both a hashstring and a pointer so that the first iteration of the loop
	// will pass SetParentName a NULL instead of an address of a hashstring
	// hashed to ""
	CHashString hszParentName(_T("World"));
	CHashString hszName(_T(""));
	CHashString hszTypeName(_T(""));
	CHashString streamType(_T("File"));

	// make sure the hierarchy is cleared, a world is unloaded
	static DWORD msgHash_ClearHierarchy = CHashString(_T("ClearHierarchy")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_ClearHierarchy, 0, NULL, NULL, NULL);
	
	CREATEARCHIVE ca;
	ca.streamData = (void *)pFileName;
	ca.mode = STREAM_MODE_READ;
	ca.streamType = &streamType;
	// call the Archive factory to create an XML archive
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (retVal = m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return false;
	}

	Archive = dynamic_cast<IXMLArchive *>(ca.archive);
	DWORD numNodes = Archive->GetNumberOfNodes();

	StdString szNodeName;
	Archive->GetNode(szNodeName);
	
	// must start with <World>
	if (szNodeName != _T("CWorld"))
	{
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_FILE);
		EngineGetToolBox()->Log(LOGWARNING, _T("Invalid world file %s\n"), pFileName);
		Archive->Close();
		return false;
	}
	else
	{
		// read the name of the world
		StdString szName;
		Archive->Read( szName, _T("Name") );
	}

	while (Archive->GetNode(szNodeName))
	{
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
			//ASSERT( hszTypeName.GetString() == childType );
			return false;
		}

		StdString szName;
		Archive->Read( szName, _T("Name") );
		hszName.Init( szName.c_str() );

		cop.parentName = &hszParentName;
		cop.typeName = &hszTypeName;
		cop.name = &hszName;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);

		if( _tcscmp( hszTypeName.GetString(), _T("CPhysicsObject") ) )
		{
			sop.name = &hszName;
			sop.archive = Archive;
			static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);
		}
		else
		{
			IXMLArchive* PhysArchive = NULL;
			PhysArchive = dynamic_cast<IXMLArchive*>(Archive);
			if( PhysArchive == NULL )
			{
				return true;
			}
			PhysArchive->GotoParentNode( parentType );
			PhysArchive->ReturnToChildNode( childType );

			CPhysObjectStruct tmpCPhysObject;
			StdString CPhyShapeFile;
			// if it's parent is not a CTerrainSector Object
			if( parentType != _T("CTerrainSector") )
			{
				// Get the Model Name
				static DWORD msgHash_GetModelFileName = CHashString(_T("GetModelFileName")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_GetModelFileName, sizeof(StdString*), &CPhyShapeFile, &hszParentName, NULL );
				CPhyShapeFile += _T(".psl");

				// Read Old Format
				PhysArchive->Read( tmpCPhysObject.szName, _T("Name") );
				PhysArchive->Read( tmpCPhysObject.vPosition, _T("pos") );
				PhysArchive->Read( tmpCPhysObject.vRotation, _T("rot") );
				PhysArchive->Read( tmpCPhysObject.vScale, _T("scale") );
				PhysArchive->Read( tmpCPhysObject.fMass, _T("mass") );
				PhysArchive->Read( tmpCPhysObject.szDynamic, _T("dynamics") );
				PhysArchive->Read( tmpCPhysObject.iNumShapes, _T("numshapes") );

				for(int i = 0; i < tmpCPhysObject.iNumShapes; i++ )
				{
					StdString tmpPhysShape;
					PhysArchive->GetNode( tmpPhysShape );
					if( tmpPhysShape != _T("CPhysShape") )
					{
						return true;
					}
				}                   				
			}
			// CTerrainSector Object
			else
			{
				CPhyShapeFile = _T("maps\\terrain.psl");

				PhysArchive->Read( tmpCPhysObject.szName, _T("Name") );
				PhysArchive->Read( tmpCPhysObject.vPosition, _T("pos") );
				PhysArchive->Read( tmpCPhysObject.vRotation, _T("rot") );
				PhysArchive->Read( tmpCPhysObject.vScale, _T("scale") );
				PhysArchive->Read( tmpCPhysObject.fMass, _T("mass") );
				PhysArchive->Read( tmpCPhysObject.szDynamic, _T("dynamics") );
				PhysArchive->Read( tmpCPhysObject.iNumShapes, _T("numshapes") );

				for(int i = 0; i < tmpCPhysObject.iNumShapes; i++ )
				{
					StdString tmpPhysShape;
					PhysArchive->GetNode( tmpPhysShape );
					if( tmpPhysShape != _T("CPhysShape") )
					{
						return true;
					}
				}     
			}	

			// Archive the Data Back In
			IArchive *MemArchivePhysObject;	
			CHashString memTypePhysObject(_T("Memory"));
			int sizePhysObject = 1024 * 1024 *sizeof(char);
			char* memchunkPhysObject = new char[sizePhysObject];
			memset( memchunkPhysObject, 0, sizePhysObject );

			CREATEARCHIVE caPhysObject;
			caPhysObject.mode = STREAM_MODE_WRITE;
			caPhysObject.streamData = memchunkPhysObject;
			caPhysObject.streamSize = sizePhysObject;
			caPhysObject.streamType = &memTypePhysObject;
			static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caPhysObject) != MSG_HANDLED)
			{
				return true;
			}
			MemArchivePhysObject = caPhysObject.archive;
			//MemArchivePhysObject->Write( tmpCPhysObject.szName, _T("Name") );
			MemArchivePhysObject->Write( tmpCPhysObject.vPosition, _T("pos") );
			MemArchivePhysObject->Write( tmpCPhysObject.vRotation, _T("rot") );
			MemArchivePhysObject->Write( tmpCPhysObject.vScale, _T("scale") );
			MemArchivePhysObject->Write( tmpCPhysObject.fMass, _T("mass") );
			MemArchivePhysObject->Write( tmpCPhysObject.szDynamic, _T("dynamics") );
			MemArchivePhysObject->Write( CPhyShapeFile, _T("shapeFile") );
			
			CREATESTREAM csPhysObject;
			csPhysObject.streamData = caPhysObject.streamData;
			csPhysObject.streamSize = caPhysObject.streamSize;
			csPhysObject.mode = STREAM_MODE_READ;
			static DWORD msgHash_CreateStream_Memory = CHashString(_T("CreateStream_Memory")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_CreateStream_Memory, sizeof(CREATESTREAM), &csPhysObject) != MSG_HANDLED)
			{
				return true;
			}
			MemArchivePhysObject->Init(csPhysObject.openStream);

			SERIALIZEOBJECTPARAMS sopPhysObject;
			sopPhysObject.name = &hszName;
			sopPhysObject.archive = MemArchivePhysObject;
			static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sopPhysObject, NULL, NULL);

			MemArchivePhysObject->Close();
			delete [] memchunkPhysObject;
			memchunkPhysObject = NULL;
		}
	}

	Archive->Close();

	return true;
}

bool CWorldLoader::SaveWorld( StdString wszFilename )
{
	TCHAR *pFileName = const_cast<TCHAR*>((const TCHAR*)wszFilename);

	if( pFileName == NULL )
	{
		return false;
	}

	IXMLArchive *XMLOutput;
	CREATEARCHIVE ca;
	ca.streamData = pFileName;
	ca.mode = STREAM_MODE_WRITE;

	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return false;
	}

	XMLOutput = dynamic_cast<IXMLArchive *>(ca.archive);

	CWorldVisitor WorldVisitor;
	WorldVisitor.SetArchiver( XMLOutput );
	
	VISITHIERARCHYDATA vhd;
	vhd.name = NULL;
	vhd.visitor = &WorldVisitor;

	static DWORD msgHash_VisitHierarchy = CHashString(_T("VisitHierarchy")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_VisitHierarchy, sizeof(VISITHIERARCHYDATA), &vhd, NULL, NULL);

	XMLOutput->Close();

	return true;
}