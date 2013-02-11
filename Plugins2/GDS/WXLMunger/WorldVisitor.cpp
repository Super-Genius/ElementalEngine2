///============================================================================
/// \file		CWorldVisitor.cpp
/// \brief		Implementation of visitor pattern to manipulate models in heirarchy
///             and provide reports
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

#include "WorldVisitor.h"

CWorldVisitor::CWorldVisitor( )
{
	m_pArchiver = NULL;
	m_pReporter = NULL;
	m_bObjectExclusion = false;
	m_vObjectTypes.clear();
	m_vModelReference.clear();
	m_iNewEntries = 0;
	m_iV3ONameEdits = 0;

	m_iMemSize = sizeof( char ) * 1024;
	m_pMemChunk = new char[m_iMemSize];

	m_ToolBox = EngineGetToolBox();

	m_szLastV3OFileName = _T("");
}

CWorldVisitor::~CWorldVisitor( )
{
	if( m_pMemChunk != NULL )
	{
		delete [] m_pMemChunk;
	}
	m_pMemChunk = NULL;
}


bool CWorldVisitor::Visit( IComponent * component, bool bVisitEnter )
{
	IObject *theObject;
	IHashString *name;
	IHashString *parentName;
	IHashString *type;

	StdString parentType;
	StdString childType;

	std::string str;

	theObject = dynamic_cast<IObject *>(component);
	// This shouldn't happen but it does for some odd reason....
	assert(theObject);
	if( theObject == NULL )
	{
		return false;
	}
	name = theObject->GetName();
	parentName = theObject->GetParentName();
	type = theObject->GetComponentType();

	//Check to see if it is a valid object (for object exclusion)
	if( !CheckObject( type->GetString() ) )
	{
		return true;
	}

	else
	{
		if ( name == NULL )
		{
			name = &CHashString(_T("NULL"));
		}	

		if( bVisitEnter == true )
		{
			//if( (m_pArchiver != NULL) && ( _tcscmp( type->GetString(), _T("CPhysicsObject") ) != 0 ) )
			if( (m_pArchiver != NULL) )
			{
				// Start the Node
				m_pArchiver->StartClass( type->GetString() );
				
				// Write out the Unique ID aka Name
				m_pArchiver->Write( name->GetString(), _T("Name") );
				theObject->Serialize( *m_pArchiver );
			}

			// Removal of CPhysShape and changes to CPhysicsObject
			if( _tcscmp( type->GetString(), _T("CPhysicsObject") ) == 0 )
			{
				
				CPhysObjectStruct tmpCPhysObject;
				StdString CPhyShapeFile;
				StdString CPhyShapeFileOld;
				// if it's parent is not a CTerrainSector Object
				if( _tcsstr( name->GetString(), _T("Terrain") ) == NULL )
				{
					static DWORD msgHash_GetModelFileName = CHashString(_T("GetModelFileName")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_GetModelFileName, sizeof(StdString*), &CPhyShapeFile, parentName, NULL );
					CPhyShapeFile += _T(".psl");
				}
				// CTerrainSector Object
				else
				{
					CPhyShapeFile = _T("maps\\terrain.psl");
				}

				IArchive *PhysObjectIn;
				IArchive *PhysObjectRead;
				CHashString memTypePhysObjectIn(_T("Memory"));
				
				int PhysObjectInMemSize = 1024 * 1024 * sizeof(char);
				char* PhysObjectInMemChunk = new char[PhysObjectInMemSize];
				memset( PhysObjectInMemChunk, 0, PhysObjectInMemSize );

				CREATEARCHIVE caPhysObjectIn;
				caPhysObjectIn.mode = STREAM_MODE_READ;
				caPhysObjectIn.streamData = PhysObjectInMemChunk;
				caPhysObjectIn.streamSize = PhysObjectInMemSize;
				caPhysObjectIn.streamType = &memTypePhysObjectIn;
				static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
				if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caPhysObjectIn) != MSG_HANDLED)
				{
					return true;
				}
				PhysObjectIn = caPhysObjectIn.archive;

				CREATESTREAM csPhysObjectIn;
				csPhysObjectIn.streamData = caPhysObjectIn.streamData;
				csPhysObjectIn.streamSize = caPhysObjectIn.streamSize;
				csPhysObjectIn.mode = STREAM_MODE_WRITE;
				static DWORD msgHash_CreateStream_Memory = CHashString(_T("CreateStream_Memory")).GetUniqueID();
				if (m_ToolBox->SendMessage(msgHash_CreateStream_Memory, sizeof(CREATESTREAM), &csPhysObjectIn) != MSG_HANDLED)
				{
					return true;
				}
				PhysObjectIn->Init(csPhysObjectIn.openStream);		    

				SERIALIZEOBJECTPARAMS sop;
				sop.name = name;
				sop.archive = PhysObjectIn;
				static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);
				PhysObjectIn->Close();
				
				CREATEARCHIVE caPhysObjectRead;
				caPhysObjectRead.mode = STREAM_MODE_WRITE;
				caPhysObjectRead.streamData = PhysObjectInMemChunk;
				caPhysObjectRead.streamSize = PhysObjectInMemSize;
				caPhysObjectRead.streamType = &memTypePhysObjectIn;
				if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caPhysObjectRead) != MSG_HANDLED)
				{
					return true;
				}
				PhysObjectRead = caPhysObjectRead.archive;

				CREATESTREAM csPhysObjectRead;
				csPhysObjectRead.streamData = caPhysObjectRead.streamData;
				csPhysObjectRead.streamSize = caPhysObjectRead.streamSize;
				csPhysObjectRead.mode = STREAM_MODE_READ;
				if (m_ToolBox->SendMessage(msgHash_CreateStream_Memory, sizeof(CREATESTREAM), &csPhysObjectRead) != MSG_HANDLED)
				{
					return true;
				}
				PhysObjectRead->Init(csPhysObjectRead.openStream);
				PhysObjectRead->Read( tmpCPhysObject.vPosition, _T("pos") );
				PhysObjectRead->Read( tmpCPhysObject.vRotation, _T("rot") );
				PhysObjectRead->Read( tmpCPhysObject.vScale, _T("scale") );
				PhysObjectRead->Read( tmpCPhysObject.fMass, _T("mass") );
				PhysObjectRead->Read( tmpCPhysObject.szDynamic, _T("dynamics") );
				PhysObjectRead->Read( CPhyShapeFileOld, _T("shapeFile") );
				PhysObjectRead->Close();
                
				// Archive the Data Back In
				IArchive *MemArchivePhysObject;	
				CHashString memTypePhysObject(_T("Memory"));
				//int sizePhysObject = 1024 * 1024;
				//char* memchunkPhysObject = new char[sizePhysObject];

				CREATEARCHIVE caPhysObject;
				caPhysObject.mode = STREAM_MODE_WRITE;
				caPhysObject.streamData = PhysObjectInMemChunk;
				caPhysObject.streamSize = PhysObjectInMemSize;
				caPhysObject.streamType = &memTypePhysObject;
				if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caPhysObject) != MSG_HANDLED)
				{
					return true;
				}
				MemArchivePhysObject = caPhysObject.archive;
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
				if (m_ToolBox->SendMessage(msgHash_CreateStream_Memory, sizeof(CREATESTREAM), &csPhysObject) != MSG_HANDLED)
				{
					return true;
				}
				MemArchivePhysObject->Init(csPhysObject.openStream);

				SERIALIZEOBJECTPARAMS sopPhysObject;
				sopPhysObject.name = name;
				sopPhysObject.archive = MemArchivePhysObject;
				m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sopPhysObject, NULL, NULL);

				MemArchivePhysObject->Close();
				//delete [] memchunkPhysObject;
				//memchunkPhysObject = NULL;
				delete [] PhysObjectInMemChunk;
				PhysObjectInMemChunk = NULL;
			}

			// Model Rename Changes
			if( (m_pReporter != NULL) && ( _tcscmp( type->GetString(), _T("CV3ORenderObject") ) == 0 ) )
			{
				IArchive *MemArchive;
				IArchive *MemArchive2;
				CHashString memType(_T("Memory"));
				memset( m_pMemChunk, '\0', m_iMemSize );

				CREATEARCHIVE ca;
				ca.mode = STREAM_MODE_READ;
				ca.streamData = m_pMemChunk;
				ca.streamSize = m_iMemSize;
				ca.streamType = &memType;
				static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
				if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
				{
					return true;
				}
				MemArchive = ca.archive;

				CREATESTREAM cs;
				cs.streamData = ca.streamData;
				cs.streamSize = ca.streamSize;
				cs.mode = STREAM_MODE_WRITE;
				static DWORD msgHash_CreateStream_Memory = CHashString(_T("CreateStream_Memory")).GetUniqueID();
				if (m_ToolBox->SendMessage(msgHash_CreateStream_Memory, sizeof(CREATESTREAM), &cs) != MSG_HANDLED)
				{
					return true;
				}
				MemArchive->Init(cs.openStream);		    

				SERIALIZEOBJECTPARAMS sop;
				sop.name = name;
				sop.archive = MemArchive;
				static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);
				MemArchive->Close();
				
				CREATEARCHIVE ca2;
				ca2.mode = STREAM_MODE_WRITE;
				ca2.streamData = m_pMemChunk;
				ca2.streamSize = m_iMemSize;
				ca2.streamType = &memType;
				if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca2) != MSG_HANDLED)
				{
					return true;
				}
				MemArchive2 = ca2.archive;

				CREATESTREAM cs2;
				cs2.streamData = ca2.streamData;
				cs2.streamSize = ca2.streamSize;
				cs2.mode = STREAM_MODE_READ;
				if (m_ToolBox->SendMessage(msgHash_CreateStream_Memory, sizeof(CREATESTREAM), &cs2) != MSG_HANDLED)
				{
					return true;
				}
				MemArchive2->Init(cs2.openStream);

				StdString wszModelFileName;
				Vec3 Position;
				Vec3 Rotation;
				Vec3 Scale;

				MemArchive2->Read( wszModelFileName );
				MemArchive2->Read( Position );
				MemArchive2->Read( Rotation );
				MemArchive2->Read( Scale );
				MemArchive2->Close();
				
				MODELMAP::iterator itr;
				bool bReArchive = false;
				while( 1 )
				{		
					itr = m_vModelReference.find( wszModelFileName );
					// If the Model Refernce already exists
					if( itr != m_vModelReference.end() )
					{
						// If its newfilename is different and it isn't an empty string
						// We Change the file name and set the archive flag
						if( (wszModelFileName != itr->second.m_wszNewFileName) &&
							(itr->second.m_wszNewFileName != StdString("")) )
						{
							wszModelFileName = itr->second.m_wszNewFileName;
							bReArchive = true;
						}
						
						// We've reached a file that has the same exact newfilename, no change neccisary
						else
						{
							break;
						}
					}
					
					
					// We change model name first (up above) and then make sure to add it as a reference and break out
					itr = m_vModelReference.find( wszModelFileName );
					if( itr == m_vModelReference.end() )
					{
						/*
						MODELINFO tmpModelInfo;
						tmpModelInfo.m_wszNewFileName = wszModelFileName;
						m_vModelReference[wszModelFileName] = tmpModelInfo;
						m_iNewEntries++;
						*/
						break;
					}
					
				}
				
				m_szLastV3OFileName = wszModelFileName;

				// Archive the object back out
				if( bReArchive == true )
				{
					IArchive *MemArchiveRE;	
					CHashString memTypeRE(_T("Memory"));
					int sizeRE = 1024 + sizeof(Vec3) * 3;
					char* memchunkRE = new char[sizeRE];

					CREATEARCHIVE caRE;
					caRE.mode = STREAM_MODE_WRITE;
					caRE.streamData = memchunkRE;
					caRE.streamSize = sizeRE;
					caRE.streamType = &memTypeRE;
					static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
					if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caRE) != MSG_HANDLED)
					{
						return true;
					}
					MemArchiveRE = caRE.archive;
					MemArchiveRE->Write( wszModelFileName );
					MemArchiveRE->Write( Position );
					MemArchiveRE->Write( Rotation );
					MemArchiveRE->Write( Scale );

					CREATESTREAM csRE;
					csRE.streamData = caRE.streamData;
					csRE.streamSize = caRE.streamSize;
					csRE.mode = STREAM_MODE_READ;
					static DWORD msgHash_CreateStream_Memory = CHashString(_T("CreateStream_Memory")).GetUniqueID();
					if (m_ToolBox->SendMessage(msgHash_CreateStream_Memory, sizeof(CREATESTREAM), &csRE) != MSG_HANDLED)
					{
						return true;
					}
					MemArchiveRE->Init(csRE.openStream);

					SERIALIZEOBJECTPARAMS sopRE;
					sopRE.name = name;
					sopRE.archive = MemArchiveRE;
					static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sopRE, NULL, NULL);

					MemArchiveRE->Close();
					delete [] memchunkRE;
					memchunkRE = NULL;
					m_iV3ONameEdits++;
				}
			}
		}

		// bVisitEnter == false
		else
		{
			if( m_pArchiver != NULL )
			{
				m_pArchiver->EndClass();
			}
		}
	}
	
	return true;
}

bool CWorldVisitor::IsHierarchicalVisitor( void )
{
	return true;
}

void CWorldVisitor::SetArchiver( IArchive * pArchiver )
{
	if( pArchiver != NULL )
	{
		m_pArchiver = pArchiver;
	}
	else
	{
		m_pArchiver = NULL;
	}
}

void CWorldVisitor::CreateReport( StdString wszFileName )
{
	IXMLArchive* pReader = NULL;
	CREATEARCHIVE ca1;
	ca1.streamData = const_cast<TCHAR*>((const TCHAR*)wszFileName);
	ca1.mode = STREAM_MODE_READ;
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca1);
	pReader = dynamic_cast<IXMLArchive *>(ca1.archive);
	if( pReader )
	{
		StdString tmp;
		printf("LOADING REPORT: %S\n", wszFileName);
		pReader->GetNode( tmp );
		while( pReader->GetNode(tmp) && strcmp(tmp, "Model") == 0 )
		{
			StdString tmpModelName;
			MODELINFO tmpModelInfo;
			
			pReader->Read( tmpModelName );
			pReader->Read( tmpModelInfo.m_wszClassification );
			pReader->Read( tmpModelInfo.m_wszNewFileName );
            m_vModelReference[tmpModelName] = tmpModelInfo;

        }
		printf("LOADED ENTRIES: %d\n", m_vModelReference.size() );
	}
	pReader->Close();

	CREATEARCHIVE ca2;
	ca2.streamData = const_cast<TCHAR*>((const TCHAR*)wszFileName);
	ca2.mode = STREAM_MODE_WRITE;
	if (m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca2) != MSG_HANDLED)
	{
		return;
	}
	m_pReporter = dynamic_cast<IXMLArchive *>(ca2.archive);
}

void CWorldVisitor::OutputReport( void )
{
	m_pReporter->StartClass( _T("REPORT") );
	MODELMAP::iterator itr;
	itr = m_vModelReference.begin();
	while( itr != m_vModelReference.end() )
	{
		m_pReporter->StartClass( _T("Model") );
		m_pReporter->Write( itr->first, "OldName" );
		m_pReporter->Write( itr->second.m_wszClassification, "Classification" );
		m_pReporter->Write( itr->second.m_wszNewFileName, "NewName" );
		m_pReporter->EndClass();
		itr++;
	}
	m_pReporter->EndClass();
	m_pReporter->Close();

	printf("Total Entries: %d\n", m_vModelReference.size() );
	printf("New Entries: %d\n", m_iNewEntries );
	printf("V3O Edits: %d\n", m_iV3ONameEdits );
}

void CWorldVisitor::SetObjectExclusion( bool fFlag )
{
	m_bObjectExclusion = fFlag;
}

void CWorldVisitor::AddObject( StdString wszNameType )
{
	m_vObjectTypes.push_back( wszNameType );
}

bool CWorldVisitor::CheckObject( StdString wszNameType )
{
	if( !m_bObjectExclusion )
	{
		return true;
	}

	else
	{
		for(unsigned int i= 0; i < m_vObjectTypes.size(); i++ )
		{
			if( _tcscmp( m_vObjectTypes[i], wszNameType ) == 0 )
			{
				return true;
			}
		}
		return false;
	}
}