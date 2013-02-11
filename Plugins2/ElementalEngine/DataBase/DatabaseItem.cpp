///============================================================================
/// \file		DatabaseItem.cpp
/// \brief		Implentation of DatabaseItem Class
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

REGISTER_COMPONENT(CDatabaseItem);

CDatabaseItem::CDatabaseItem( IHashString *parentName, IHashString *name ) :
	OBJECTTEMPLATE( CDatabaseManager, CDatabaseItem, IObject, parentName, name )
{
	m_ToolBox = EngineGetToolBox();

	if( parentName != NULL )
	{
		IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
		CDatabaseManager *amanager = (CDatabaseManager*)( amanagerComponent );
		static CHashString CDDName( _T("CDefinedDatabase") );
		IDTOOBJECTMAP *objectMap = amanager->GetObjectMap(&CDDName);
		IDTOOBJECTMAP::iterator objectsIterator = objectMap->find(parentName->GetUniqueID());
		if( objectsIterator != objectMap->end() )
		{
			m_Database = (CDefinedDatabase*)objectsIterator->second;
		}
		else
		{
			m_Database = NULL;
			m_ToolBox->Log( LOGWARNING, _T("Invalid database parent specified for entry item %s\n"), name->GetString() );
		}
	}
	else
	{
		m_Database = NULL;
		m_ToolBox->Log( LOGWARNING, _T("No database parent specified for entry item %s\n"), name->GetString() );
	}

	if( m_Database != NULL )
	{
		m_Database->RegisterDatabaseItem( this );
	}
}

CDatabaseItem::~CDatabaseItem()
{
}

void CDatabaseItem::Init()
{
}

bool CDatabaseItem::Update()
{
	// NO-OP
	return true;
}

void CDatabaseItem::DeInit()
{
	ATTRIBUTEIDTODATAMAP::iterator valuesIterator;
	for( valuesIterator = m_Values.begin(); valuesIterator != m_Values.end(); valuesIterator++ )
	{
		delete valuesIterator->second;
	}
	if( m_Database != NULL )
	{
		m_Database->DeregisterDatabaseItem( this );
	}
}

IComponent *CDatabaseItem::Create( int nArgs, va_list argptr )
{
	IObject *self;

	IHashString *name = va_arg( argptr, IHashString * );
	IHashString *parentName = va_arg( argptr, IHashString * );
	try
	{
		self = new CDatabaseItem( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CDatabaseItem::Serialize( IArchive &ar )
{
	CDatabaseSchema *schema = GetSchema();
	if( schema != NULL )
	{
		if( ar.IsReading() )
		{
			schema->SerializeDatabaseItem( m_Values, ar );
		}
		else
		{
			ar.StartClass(_T("CDatabaseItem"));

			schema->SerializeDatabaseItem( m_Values, ar );

			ar.EndClass();
		}
	}
}


ATTRIBUTEIDTODATAMAP *CDatabaseItem::GetValuesMap()
{
	if( GetSchema()->GetNumItems() > (int)m_Values.size() )
	{
		GetSchema()->InitializeValues( &m_Values );
	}
	return &m_Values;
}

DATABASEDATA * CDatabaseItem::GetDataByName( IHashString * phszName )
{
	return m_Values[ phszName->GetUniqueID() ];
}

DWORD CDatabaseItem::GetAttribute( DATABASEATTRIBUTEPARAMS *databaseAttributeParams )
{
	DWORD retVal = MSG_NOT_HANDLED;
	if( databaseAttributeParams != NULL )
	{
		CSchemaItem *schemaItem = GetSchemaItem( databaseAttributeParams->m_AttributeName );
		if( schemaItem != NULL )
		{
			databaseAttributeParams->m_Data = GetDataByName( schemaItem->GetLabel() );
			if( databaseAttributeParams->m_AttributeTypeArchive != NULL )
			{
				IArchive *ar = databaseAttributeParams->m_AttributeTypeArchive;
				ar->SetIsWriting( true );
				ar->Write( schemaItem->GetType()->GetString() );
			}
			if( databaseAttributeParams->m_AttributeArchive != NULL )
			{
				databaseAttributeParams->m_AttributeArchive->SetIsWriting( true );
				schemaItem->SerializeDatabaseItem( m_Values, *databaseAttributeParams->m_AttributeArchive );
			}
			else
			{
				m_ToolBox->Log( LOGWARNING, _T("No archive specified for GetAttribute.\n") );
			}

			retVal = MSG_HANDLED_STOP;
		}
	}
	else
	{
		m_ToolBox->Log( LOGWARNING, _T("Attribute parameters not defined.\n") );
	}

	return retVal;
}


void CDatabaseItem::GetClassType( IHashString *attributeName, IHashString *classType )
{
	CSchemaItem *schemaItem = GetSchemaItem( attributeName );
	if( schemaItem != NULL && classType != NULL )
	{
		(*classType) = schemaItem->GetClass()->GetString();
	}
}


DWORD CDatabaseItem::GetAttributeType( DATABASEATTRIBUTEPARAMS *databaseAttributeParams )
{
	DWORD retVal = MSG_NOT_HANDLED;
	if( databaseAttributeParams != NULL )
	{
		CSchemaItem *schemaItem = GetSchemaItem( databaseAttributeParams->m_AttributeName );
		if( schemaItem != NULL )
		{
			retVal = schemaItem->GetType( databaseAttributeParams );
		}
	}
	else
	{
		m_ToolBox->Log( LOGWARNING, _T("Attribute parameters not defined.\n") );
	}

	return retVal;
}


void CDatabaseItem::GetAttributeType( IHashString *attributeName, IHashString *attributeType )
{
	CSchemaItem *schemaItem = GetSchemaItem( attributeName );
	if( schemaItem != NULL )
	{
		schemaItem->GetEditorType( attributeType );
	}
}


DWORD CDatabaseItem::SetAttribute( DATABASEATTRIBUTEPARAMS *databaseAttributeParams )
{
	DWORD retVal = MSG_NOT_HANDLED;
	if( databaseAttributeParams != NULL )
	{
		CSchemaItem *schemaItem = GetSchemaItem( databaseAttributeParams->m_AttributeName );
		if( schemaItem != NULL )
		{
			if( databaseAttributeParams->m_Data != NULL )
			{
				m_Values[ schemaItem->GetLabel()->GetUniqueID() ]->Assign( *databaseAttributeParams->m_Data );
			}
			else if( databaseAttributeParams->m_AttributeArchive != NULL )
			{
				databaseAttributeParams->m_AttributeArchive->SetIsWriting( false );
				schemaItem->SerializeDatabaseItem( m_Values, *databaseAttributeParams->m_AttributeArchive );
			}
			else
			{
				m_ToolBox->Log( LOGWARNING, _T("No data or archive specified for SetAttribute.\n") );
			}

			retVal = MSG_HANDLED_STOP;
		}
	}
	else
	{
		m_ToolBox->Log( LOGWARNING, _T("Attribute parameters not defined.\n") );
	}

	return retVal;
}


bool CDatabaseItem::AttributeEquals( IHashString *attributeName, DATABASEDATA *attributeValue )
{
	DATABASEDATA *databaseValue = m_Values[attributeName->GetUniqueID()];
	bool retVal = false;

	if( databaseValue->type == DBTYPE_CHAR )
	{
		retVal = databaseValue->m_CharValue == attributeValue->m_CharValue;
	}
	else if( databaseValue->type == DBTYPE_UCHAR )
	{
		retVal = databaseValue->m_UCharValue == attributeValue->m_UCharValue;
	}
	else if( databaseValue->type == DBTYPE_SHORT )
	{
		retVal = databaseValue->m_ShortValue == attributeValue->m_ShortValue;
	}
	else if( databaseValue->type == DBTYPE_USHORT )
	{
		retVal = databaseValue->m_UShortValue == attributeValue->m_UShortValue;
	}
	else if( databaseValue->type == DBTYPE_LONG )
	{
		retVal = databaseValue->m_LongValue == attributeValue->m_LongValue;
	}
	else if( databaseValue->type == DBTYPE_ULONG )
	{
		retVal = databaseValue->m_ULongValue == attributeValue->m_ULongValue;
	}
	else if( databaseValue->type == DBTYPE_FLOAT )
	{
		retVal = databaseValue->m_FloatValue == attributeValue->m_FloatValue;
	}
	else if( databaseValue->type == DBTYPE_DOUBLE )
	{
		retVal = databaseValue->m_DoubleValue == attributeValue->m_DoubleValue;
	}
	else if( databaseValue->type == DBTYPE_BOOLEAN )
	{
		retVal = databaseValue->m_BoolValue == attributeValue->m_BoolValue;
	}
	else if( databaseValue->type == DBTYPE_STRING )
	{
		retVal = _tcscmp( databaseValue->m_TCharPtr, attributeValue->m_TCharPtr ) == 0;
	}

	return retVal;
}


CDatabaseSchema *CDatabaseItem::GetSchema()
{
	if( m_Database != NULL )
	{
		return m_Database->GetSchema();
	}
	else
	{
		m_ToolBox->Log( LOGWARNING, _T("Item entry database not defined.\n") );
	}
	return NULL;
}

CSchemaItem *CDatabaseItem::GetSchemaItem( IHashString *name )
{
	CSchemaItem *schemaItem = NULL;
	if( name != NULL )
	{
		CDatabaseSchema *schema = GetSchema();
		if( schema != NULL )
		{
			schemaItem = schema->GetSchemaItem( name );
		}
	}
	else
	{
		m_ToolBox->Log( LOGWARNING, _T("Schema item attribute name not specified.\n") );
	}

	return schemaItem;
}