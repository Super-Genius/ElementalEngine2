///============================================================================
/// \file		DatabaseSchema.cpp
/// \brief		Implentation of DatabaseSchema Class
/// \date		04-12-2007
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

REGISTER_COMPONENT(CDatabaseSchema);
REGISTER_MESSAGE_HANDLER( MapToFileName, OnMapToFileName, CDatabaseSchema );
REGISTER_MESSAGE_HANDLER( GetSchema, OnGetSchema, CDatabaseSchema );

CDatabaseSchema::CDatabaseSchema( IHashString *parentName, IHashString *name ) :
	OBJECTTEMPLATE( CDatabaseManager, CDatabaseSchema, IObject, parentName, name )
{
	m_ToolBox = EngineGetToolBox();
	m_fVersion = 1.0f;
	m_iNumItems = 0;
}

CDatabaseSchema::~CDatabaseSchema()
{
}

void CDatabaseSchema::Init()
{
}

bool CDatabaseSchema::Update()
{
	// NO-OP
	return true;
}

void CDatabaseSchema::DeInit()
{
	SCHEMAATTRIBUTELIST::iterator schemaAttributeIterator;
	while( m_SchemaAttributeList.begin() != m_SchemaAttributeList.end() )
	{
		schemaAttributeIterator = m_SchemaAttributeList.begin();
		DELETEOBJECTPARAMS dop;
		dop.name = (*schemaAttributeIterator)->GetName();
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		DWORD retVal = m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop );
	}
}

IComponent *CDatabaseSchema::Create( int nArgs, va_list argptr )
{
	IObject *self;

	IHashString *name = va_arg( argptr, IHashString * );
	IHashString *parentName = va_arg( argptr, IHashString * );
	try
	{
		self = new CDatabaseSchema( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CDatabaseSchema::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		StdString name; // Dummy variable to read the name. It should already be stored as CObjectTemplate

		ar.Read( m_fVersion, _T("Version") );
		ar.Read( name, _T("Name") );
		ar.Read( m_iNumItems, _T("NumItems") );
	}
	else
	{
		ar.StartClass(_T("CDatabaseSchema"));
		ar.Write( m_fVersion, _T("Version") );
		ar.Write( GetName()->GetString(), _T("Name") );
		ar.Write( m_iNumItems, _T("NumItems") );
		SCHEMAATTRIBUTELIST::iterator schemaAttributeIterator;
		for( schemaAttributeIterator = m_SchemaAttributeList.begin(); schemaAttributeIterator != m_SchemaAttributeList.end(); schemaAttributeIterator++ )
		{
			(*schemaAttributeIterator)->Serialize( ar );
		}
		ar.EndClass();
	}
}

void CDatabaseSchema::SerializeDatabaseItem( ATTRIBUTEIDTODATAMAP &valuesMap, IArchive &ar )
{
	SCHEMAATTRIBUTELIST::iterator attributeIterator;
	for( attributeIterator = m_SchemaAttributeList.begin(); attributeIterator != m_SchemaAttributeList.end(); attributeIterator++ )
	{
		(*attributeIterator)->SerializeDatabaseItem( valuesMap, ar );
	}
}

void CDatabaseSchema::RegisterSchemaItem( CSchemaItem *schemaItem )
{
	// Make sure we don't add duplicates.
	m_SchemaAttributeList.remove( schemaItem );
	m_SchemaAttributeList.push_back( schemaItem );
	m_SchemaAttributeMap[ schemaItem->GetLabel()->GetUniqueID() ] = schemaItem;
}

void CDatabaseSchema::DeregisterSchemaItem( CSchemaItem *schemaItem )
{
	m_SchemaAttributeList.remove( schemaItem );
	m_SchemaAttributeMap.erase( schemaItem->GetLabel()->GetUniqueID() );
}

CSchemaItem *CDatabaseSchema::GetSchemaItem( IHashString *name )
{
	CSchemaItem *schemaItem = NULL;

	IDTOOBJECTMAP::iterator iterator = m_SchemaAttributeMap.find( name->GetUniqueID() );
	if( iterator != m_SchemaAttributeMap.end() )
	{
		schemaItem = (CSchemaItem*)iterator->second;
	}
	else
	{
//		m_ToolBox->Log( LOGWARNING, _T("Schema attribute item %s could not be found.\n"), name->GetString());
	}

	return schemaItem;
}

void CDatabaseSchema::InitializeValues( ATTRIBUTEIDTODATAMAP *valuesMap )
{
	SCHEMAATTRIBUTELIST::iterator attributeIterator;
	for( attributeIterator = m_SchemaAttributeList.begin(); attributeIterator != m_SchemaAttributeList.end(); attributeIterator++ )
	{
		DWORD iteratedNameID = (*attributeIterator)->GetName()->GetUniqueID();
		if( valuesMap->find( iteratedNameID ) == valuesMap->end() )
		{
			(*attributeIterator)->SetDefaultValue( *valuesMap );
		}
	}
}

int CDatabaseSchema::GetNumItems()
{
	int items = m_SchemaAttributeList.size();
	return items;
}

bool CDatabaseSchema::IsKey( IHashString *name )
{
	bool retVal = false;
	if( name != NULL )
	{
		CSchemaItem *schemaItem = GetSchemaItem( name );
		if( schemaItem != NULL )
		{
			retVal = schemaItem->IsKey();
		}
		else
		{
//			m_ToolBox->Log( LOGWARNING, _T("Schema attribute item %s could not be found.\n"), name->GetString());
		}
	}

	return retVal;
}

DWORD CDatabaseSchema::OnMapToFileName( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(IHashString) );
	IHashString *hszFileName = (IHashString*)params;

	IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CDatabaseManager *amanager = (CDatabaseManager*)amanagerComponent;

	amanager->MapObjectToFileName( hszFileName, this );

	return MSG_HANDLED_STOP;
}

bool CDatabaseSchema::GetSchemaArchive( IArchive * archive)
{
	if( archive == NULL )
		return false;
	m_iNumItems = m_SchemaAttributeList.size();
	archive->StartClass( GetComponentType()->GetString() );
	archive->Write( GetName()->GetString(), _T("Name") );
	archive->Write( m_iNumItems, _T("Num") );

	SCHEMAATTRIBUTELIST::iterator schemaItemIterator = m_SchemaAttributeList.begin();
	for( ; schemaItemIterator != m_SchemaAttributeList.end(); schemaItemIterator++ )
	{
		(*schemaItemIterator)->WriteArchive( archive );
	}

	archive->EndClass();

	return true;

}
DWORD CDatabaseSchema::OnGetSchema( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETSCHEMAPARAMS));
	GETSCHEMAPARAMS *getSchemaParams = (GETSCHEMAPARAMS*)params;
	IXMLArchive *archive = getSchemaParams->ar;
	if( !GetSchemaArchive( archive ) )
	{
		m_ToolBox->Log( LOGERROR, _T("Attempt to get CDatabaseEntry schema. No archive specified.\n" ) );
	}

	return MSG_HANDLED_STOP;
}