///============================================================================
/// \file		DefinedDatabase.cpp
/// \brief		Implentation of CDefinedDatabase Class
/// \date		04-13-2007
/// \author		Richard Nguyen
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

REGISTER_COMPONENT(CDefinedDatabase);
REGISTER_MESSAGE_HANDLER( MapToFileName, OnMapToFileName, CDefinedDatabase );
REGISTER_MESSAGE_HANDLER(GetAttribute, OnGetAttribute, CDefinedDatabase);
REGISTER_MESSAGE_HANDLER(GetAttributeType, OnGetAttributeType, CDefinedDatabase);
REGISTER_MESSAGE_HANDLER(GetDatabaseInfo, OnGetDatabaseInfo, CDefinedDatabase);
REGISTER_MESSAGE_HANDLER(SetAttribute, OnSetAttribute, CDefinedDatabase);
REGISTER_MESSAGE_HANDLER(GetIndex, OnGetIndex, CDefinedDatabase);

CDefinedDatabase::CDefinedDatabase( IHashString *parentName, IHashString *name ) :
	OBJECTTEMPLATE( CDatabaseManager, CDefinedDatabase, IObject, parentName, name )
{
	m_ToolBox = EngineGetToolBox();
	m_fVersion = 1.0f;
	m_hszSchemaFile.Init(_T(""));
	m_hszSchema.Init(_T(""));
	m_iNumItems = 0;
	m_Schema = NULL;
}

CDefinedDatabase::~CDefinedDatabase()
{
}

void CDefinedDatabase::Init()
{
}

bool CDefinedDatabase::Update()
{
	return true;
}

void CDefinedDatabase::DeInit()
{
	DATABASEENTRYVEC::iterator databaseEntryIterator;
	while( m_DatabaseEntryVec.begin() != m_DatabaseEntryVec.end() )
	{
		databaseEntryIterator = m_DatabaseEntryVec.begin();
		DELETEOBJECTPARAMS dop;
		dop.name = (*databaseEntryIterator)->GetName();
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		DWORD retVal = m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop );
	}

	IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CDatabaseManager *amanager = (CDatabaseManager*)( amanagerComponent );
	amanager->OnDatabaseDeInit( this );
}

IComponent *CDefinedDatabase::Create( int nArgs, va_list argptr )
{
	IObject *self;

	IHashString *name = va_arg( argptr, IHashString * );
	IHashString *parentName = va_arg( argptr, IHashString * );
	try
	{
		self = new CDefinedDatabase( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CDefinedDatabase::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		StdString name; // Dummy variable to read the name. It should already be stored as CObjectTemplate
		StdString schemaFile;
		StdString schema;

		ar.Read( m_fVersion, _T("Version") );
		ar.Read( name, _T("Name") );
		ar.Read( schemaFile, _T("SchemaFile") );
		ar.Read( schema, _T("Schema") );
		m_hszSchemaFile.Init( schemaFile );
		m_hszSchema.Init( schema );
		InitializeSchema();
		ar.Read( m_iNumItems, _T("NumItems") );
	}
	else
	{
		ar.StartClass(_T("CDefinedDatabase"));

		ar.Write( m_fVersion, _T("Version") );
		ar.Write( GetName()->GetString(), _T("Name") );
		ar.Write( m_hszSchemaFile.GetString(), _T("SchemaFile") );
		ar.Write( m_hszSchema.GetString(), _T("Schema") );
		m_iNumItems = m_DatabaseEntryVec.size();
		ar.Write( m_iNumItems, _T("NumItems") );

		DATABASEENTRYVEC::iterator databaseEntryIterator;
		for( databaseEntryIterator = m_DatabaseEntryVec.begin(); databaseEntryIterator != m_DatabaseEntryVec.end(); databaseEntryIterator++ )
		{
			(*databaseEntryIterator)->Serialize( ar );
		}

		ar.EndClass();
	}
}

CDatabaseSchema *CDefinedDatabase::GetSchema()
{
	if( m_Schema == NULL )
	{
		m_ToolBox->Log( LOGWARNING, _T("Invalid schema or no schema  has been serialized for defined database %s.\n"), GetName()->GetString() );
	}

	return m_Schema;
}

IHashString *CDefinedDatabase::GetSchemaFile()
{
	return &m_hszSchemaFile;
}

CDatabaseItem *CDefinedDatabase::GetDatabaseItem( int index )
{
	CDatabaseItem *databaseItem = NULL;
	if( index < 0 || index >= (int)m_DatabaseEntryVec.size() )
	{
		m_ToolBox->Log( LOGWARNING, _T("Index of database item out of bounds.\n") );
	}
	else
	{
		databaseItem = m_DatabaseEntryVec[ index ];
	}

	return databaseItem;
}

int CDefinedDatabase::GetNumItems()
{
	return m_iNumItems;
}

int CDefinedDatabase::GetDatabaseItemIndex( IHashString *keyAttributeName, DATABASEDATA *keyAttributeValue )
{
	int vectorSize = m_DatabaseEntryVec.size();
	bool indexFound = false;
	int index = -1;
	// TODO <rnguyen>: This is performing a linear search. Find a way to map key values to database indices.
	for( int i = 0; i < vectorSize && !indexFound; i++ )
	{
		CDatabaseItem *databaseItem = m_DatabaseEntryVec[i];
		if( databaseItem->AttributeEquals( keyAttributeName, keyAttributeValue ) )
		{
			index = i;
			indexFound = true;
		}
	}

	return index;
}
void CDefinedDatabase::InitializeSchema()
{
	LOADFILEEXTPARAMS loadFileParams;
	loadFileParams.bInternalLoad = true;
	loadFileParams.fileName = (TCHAR*)m_hszSchemaFile.GetString();
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &loadFileParams);

	IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CDatabaseManager *amanager = (CDatabaseManager*)( amanagerComponent );
	static CHashString CDSName( _T("CDatabaseSchema") );
	IDTOOBJECTMAP *objectMap = amanager->GetObjectMap( &CDSName );
	IDTOOBJECTMAP::iterator objectsIterator = objectMap->find(m_hszSchema.GetUniqueID());
	if( objectsIterator != objectMap->end() )
	{
		m_Schema = (CDatabaseSchema*)objectsIterator->second;
	}
	else
	{
		m_ToolBox->Log( LOGWARNING, _T("Specified schema could not be found.\n") );
		m_Schema = NULL;
	}
}

void CDefinedDatabase::RegisterDatabaseItem( CDatabaseItem *databaseItem )
{
	m_DatabaseEntryVec.push_back( databaseItem );
}

void CDefinedDatabase::DeregisterDatabaseItem( CDatabaseItem *databaseItem )
{
	// TODO <rnguyen>: Is there a fast we to remove an item from the vector?
	DATABASEENTRYVEC::iterator databaseEntryIterator;
	for( databaseEntryIterator = m_DatabaseEntryVec.begin();
		databaseEntryIterator != m_DatabaseEntryVec.end() && *databaseEntryIterator != databaseItem;
		databaseEntryIterator++ );

	if( *databaseEntryIterator == databaseItem )
	{
		m_DatabaseEntryVec.erase( databaseEntryIterator );
	}
}

DWORD CDefinedDatabase::OnMapToFileName( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(IHashString) );
	IHashString *hszFileName = (IHashString*)params;

	IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CDatabaseManager *amanager = (CDatabaseManager*)amanagerComponent;

	amanager->MapObjectToFileName( hszFileName, this );

	return MSG_HANDLED_STOP;
}

DWORD CDefinedDatabase::OnGetAttribute( DWORD size, void *params )
{
	DWORD retVal = MSG_NOT_HANDLED;

	VERIFY_MESSAGE_SIZE(size, sizeof(DATABASEATTRIBUTEPARAMS));
	DATABASEATTRIBUTEPARAMS *databaseAttributeParams = (DATABASEATTRIBUTEPARAMS*)params;

	if( databaseAttributeParams != NULL )
	{
		CDatabaseItem *databaseItem = GetDatabaseItem( databaseAttributeParams->m_Index );
		if( databaseItem != NULL )
		{
			retVal = databaseItem->GetAttribute( databaseAttributeParams );
		}
	}
	else
	{
		m_ToolBox->Log( LOGWARNING, _T("Database attribute parameters undefined.\n") );
	}
	return retVal;
}

DWORD CDefinedDatabase::OnGetAttributeType( DWORD size, void *params )
{
	DWORD retVal = MSG_NOT_HANDLED;

	VERIFY_MESSAGE_SIZE( size, sizeof(DATABASEATTRIBUTEPARAMS) );
	DATABASEATTRIBUTEPARAMS *databaseAttributeParams = (DATABASEATTRIBUTEPARAMS*)params;

	if( databaseAttributeParams != NULL )
	{
		CSchemaItem *schemaItem = m_Schema->GetSchemaItem( databaseAttributeParams->m_AttributeName );
		if( schemaItem != NULL )
		{
			retVal = schemaItem->GetType( databaseAttributeParams );
		}
		else
		{
			m_ToolBox->Log( LOGWARNING, _T("Could not find attribute %s.\n"), databaseAttributeParams->m_AttributeName->GetString() );
		}
	}
	else
	{
		m_ToolBox->Log( LOGWARNING, _T("Database attribute parameters undefined.\n") );
	}

	return retVal;
}

DWORD CDefinedDatabase::OnGetDatabaseInfo( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(DATABASEINFOPARAMS) );

	DATABASEINFOPARAMS *databaseInfoParams = (DATABASEINFOPARAMS*)params;
	databaseInfoParams->m_NumItems = (int)m_DatabaseEntryVec.size();

	return MSG_HANDLED_STOP;
}

DWORD CDefinedDatabase::OnSetAttribute( DWORD size, void *params )
{
	DWORD retVal = MSG_NOT_HANDLED;

	VERIFY_MESSAGE_SIZE(size, sizeof(DATABASEATTRIBUTEPARAMS));
	DATABASEATTRIBUTEPARAMS *databaseAttributeParams = (DATABASEATTRIBUTEPARAMS*)params;

	if( databaseAttributeParams != NULL )
	{
		CDatabaseItem *databaseItem = GetDatabaseItem( databaseAttributeParams->m_Index );
		if( databaseItem != NULL )
		{
			retVal = databaseItem->SetAttribute( databaseAttributeParams );
		}
	}
	else
	{
		m_ToolBox->Log( LOGWARNING, _T("Database attribute parameters undefined.\n") );
	}
	return retVal;
}

DWORD CDefinedDatabase::OnGetIndex( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(DATABASEATTRIBUTEPARAMS));
	DATABASEATTRIBUTEPARAMS *databaseAttributeParams = (DATABASEATTRIBUTEPARAMS*)params;

	if( databaseAttributeParams != NULL )
	{
		if( databaseAttributeParams->m_AttributeName != NULL )
		{
			if( m_Schema->IsKey( databaseAttributeParams->m_AttributeName ) )
			{
				DATABASEDATA databaseAttribute;
				CSchemaItem *schemaItem = m_Schema->GetSchemaItem( databaseAttributeParams->m_AttributeName );
				schemaItem->DBRead( &databaseAttribute, databaseAttributeParams->m_AttributeArchive, NULL );
				databaseAttributeParams->m_Index = GetDatabaseItemIndex( databaseAttributeParams->m_AttributeName, &databaseAttribute );
			}
			else
			{
				m_ToolBox->Log( LOGWARNING, _T("Attribute %s is not a key.\n"), databaseAttributeParams->m_AttributeName->GetString() );
			}
		}
		else
		{
			m_ToolBox->Log( LOGWARNING, _T("Attribute name undefined.\n") );
		}
	}
	else
	{
		m_ToolBox->Log( LOGWARNING, _T("Database attribute parameters undefined.\n") );
	}

	return MSG_HANDLED_STOP;
}