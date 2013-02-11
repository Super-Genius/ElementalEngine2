///============================================================================
/// \file		SchemaItem.cpp
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

REGISTER_COMPONENT(CSchemaItem);

CSchemaItem::CSchemaItem( IHashString *parentName, IHashString *name ) :
	OBJECTTEMPLATE( CDatabaseManager, CSchemaItem, IObject, parentName, name ),
	m_hszCharType(_T("char")),
	m_hszUCharType(_T("unsigned char")),
	m_hszShortType(_T("short int")),
	m_hszUShortType(_T("unsigned short int")),
	m_hszLongType(_T("long int")),
	m_hszULongType(_T("unsigned long int")),
	m_hszFloatType(_T("float")),
	m_hszDoubleType(_T("double")),
	m_hszBoolType(_T("bool")),
	m_hszStringType(_T("string"))
{
	m_ToolBox = EngineGetToolBox();
	m_fVersion = 1.0f;
	m_hszLabel.Init(_T(""));
	m_hszType.Init(_T("char"));
	m_hszClass.Init(_T("edit"));

	m_DataReadFunctions[m_hszCharType.GetUniqueID()]	= &CSchemaItem::DBReadChar;
	m_DataReadFunctions[m_hszUCharType.GetUniqueID()]	= &CSchemaItem::DBReadUChar;
	m_DataReadFunctions[m_hszShortType.GetUniqueID()]	= &CSchemaItem::DBReadShort;
	m_DataReadFunctions[m_hszUShortType.GetUniqueID()]	= &CSchemaItem::DBReadUShort;
	m_DataReadFunctions[m_hszLongType.GetUniqueID()]	= &CSchemaItem::DBReadLong;
	m_DataReadFunctions[m_hszULongType.GetUniqueID()]	= &CSchemaItem::DBReadULong;
	m_DataReadFunctions[m_hszFloatType.GetUniqueID()]	= &CSchemaItem::DBReadFloat;
	m_DataReadFunctions[m_hszDoubleType.GetUniqueID()]	= &CSchemaItem::DBReadDouble;
	m_DataReadFunctions[m_hszBoolType.GetUniqueID()]	= &CSchemaItem::DBReadBool;
	m_DataReadFunctions[m_hszStringType.GetUniqueID()]	= &CSchemaItem::DBReadString;

	m_DataWriteFunctions[m_hszCharType.GetUniqueID()]	= &CSchemaItem::DBWriteChar;
	m_DataWriteFunctions[m_hszUCharType.GetUniqueID()]	= &CSchemaItem::DBWriteChar;
	m_DataWriteFunctions[m_hszShortType.GetUniqueID()]	= &CSchemaItem::DBWriteShort;
	m_DataWriteFunctions[m_hszUShortType.GetUniqueID()]	= &CSchemaItem::DBWriteShort;
	m_DataWriteFunctions[m_hszLongType.GetUniqueID()]	= &CSchemaItem::DBWriteLong;
	m_DataWriteFunctions[m_hszULongType.GetUniqueID()]	= &CSchemaItem::DBWriteLong;
	m_DataWriteFunctions[m_hszFloatType.GetUniqueID()]	= &CSchemaItem::DBWriteFloat;
	m_DataWriteFunctions[m_hszDoubleType.GetUniqueID()]	= &CSchemaItem::DBWriteFloat;
	m_DataWriteFunctions[m_hszBoolType.GetUniqueID()]	= &CSchemaItem::DBWriteBool;
	m_DataWriteFunctions[m_hszStringType.GetUniqueID()]	= &CSchemaItem::DBWriteString;

	CHashString hszEditorTypeInt( _T("int") );
	CHashString hszEditorTypeFloat( _T("float") );
	CHashString hszEditorTypeDouble( _T("double") );
	CHashString hszEditorTypeBool( _T("bool") );
	CHashString hszEditorTypeString( _T("string") );

	m_TypeIDtoEditorTypeIDMap[m_hszCharType.GetUniqueID()]		= hszEditorTypeString.GetUniqueID();
	m_TypeIDtoEditorTypeIDMap[m_hszUCharType.GetUniqueID()]		= hszEditorTypeString.GetUniqueID();
	m_TypeIDtoEditorTypeIDMap[m_hszShortType.GetUniqueID()]		= hszEditorTypeInt.GetUniqueID();
	m_TypeIDtoEditorTypeIDMap[m_hszUShortType.GetUniqueID()]	= hszEditorTypeInt.GetUniqueID();
	m_TypeIDtoEditorTypeIDMap[m_hszLongType.GetUniqueID()]		= hszEditorTypeInt.GetUniqueID();
	m_TypeIDtoEditorTypeIDMap[m_hszULongType.GetUniqueID()]		= hszEditorTypeInt.GetUniqueID();
	m_TypeIDtoEditorTypeIDMap[m_hszFloatType.GetUniqueID()]		= hszEditorTypeFloat.GetUniqueID();
	m_TypeIDtoEditorTypeIDMap[m_hszDoubleType.GetUniqueID()]	= hszEditorTypeDouble.GetUniqueID();
	m_TypeIDtoEditorTypeIDMap[m_hszBoolType.GetUniqueID()]		= hszEditorTypeBool.GetUniqueID();
	m_TypeIDtoEditorTypeIDMap[m_hszStringType.GetUniqueID()]	= hszEditorTypeString.GetUniqueID();

	if( parentName != NULL )
	{
		IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
		CDatabaseManager *amanager = (CDatabaseManager*)( amanagerComponent );
		static CHashString CDSName( _T("CDatabaseSchema"));
		IDTOOBJECTMAP *objectMap = amanager->GetObjectMap( &CDSName );
		IDTOOBJECTMAP::iterator objectsIterator = objectMap->find(parentName->GetUniqueID());
		if( objectsIterator != objectMap->end() )
		{
			m_Schema = (CDatabaseSchema*)objectsIterator->second;
		}
		else
		{
			m_Schema = NULL;
			m_ToolBox->Log( LOGWARNING, _T("Invalid parent specified for schema item %s\n"), name->GetString() );
		}
	}
	else
	{
		m_Schema = NULL;
		m_ToolBox->Log( LOGWARNING, _T("No parent specified for schema item %s\n"), name->GetString() );
	}
}

CSchemaItem::~CSchemaItem()
{
}

void CSchemaItem::Init()
{
}

bool CSchemaItem::Update()
{
	// NO-OP
	return true;
}

void CSchemaItem::DeInit()
{
	if( m_Schema != NULL )
	{
		m_Schema->DeregisterSchemaItem( this );
	}
}

IComponent *CSchemaItem::Create( int nArgs, va_list argptr )
{
	IObject *self;

	IHashString *name = va_arg( argptr, IHashString * );
	IHashString *parentName = va_arg( argptr, IHashString * );
	try
	{
		self = new CSchemaItem( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CSchemaItem::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		StdString name; // Dummy variable to read the name. It should already be stored as CObjectTemplate
		StdString type;
		StdString attributeClass;

		ar.Read( m_fVersion, _T("Version") );
		ar.Read( name, _T("Name") );
		ar.Read( type, _T("Type") );
		ar.Read( attributeClass, _T("Class") );
		m_hszLabel.Init( name );
		m_hszType.Init( type );
		m_hszClass.Init( attributeClass );
		DBRead( &m_DefaultValue, &ar, _T("Default") ); // IMPORTANT: DBRead is dependent on m_hszType. Make sure m_hszType has been initialized before calling DBRead.

		// NOTE: We are assuming the order in which schema items are instantiated will be
		//       the order in which to read data items.
		if( m_Schema != NULL )
		{
			m_Schema->RegisterSchemaItem( this );
		}
	}
	else
	{
		ar.StartClass(_T("CSchemaItem"));

		ar.Write( m_fVersion, _T("Version") );
		ar.Write( m_hszLabel.GetString(), _T("Name") );
		ar.Write( m_hszType.GetString(), _T("Type") );
		ar.Write( m_hszClass.GetString(), _T("Class") );
		DBWrite( &m_DefaultValue, &ar, _T("Default") );

		ar.EndClass();
	}
}

void CSchemaItem::SerializeDatabaseItem( ATTRIBUTEIDTODATAMAP &valuesMap, IArchive &ar )
{
	if( valuesMap.find( m_hszLabel.GetUniqueID() ) == valuesMap.end() || valuesMap[m_hszLabel.GetUniqueID()] == NULL )
	{
		valuesMap[m_hszLabel.GetUniqueID()] = new DATABASEDATA( m_DefaultValue );
	}

	DATABASEDATA *value = valuesMap[m_hszLabel.GetUniqueID()];
	TCHAR *tag = (TCHAR*)m_hszLabel.GetString();
	if( ar.IsReading() )
	{
		DBRead( value, &ar, tag );
	}
	else
	{
		DBWrite( value, &ar, tag );
	}
}

void CSchemaItem::SetDefaultValue( ATTRIBUTEIDTODATAMAP &valuesMap )
{
	ATTRIBUTEIDTODATAMAP::iterator valuesIterator = valuesMap.find( m_hszLabel.GetUniqueID() );
	if( valuesIterator != valuesMap.end() )
	{
		(*valuesIterator->second).Assign( m_DefaultValue );
	}
	else
	{
		valuesMap[m_hszLabel.GetUniqueID()] = new DATABASEDATA( m_DefaultValue );
	}
}

IHashString *CSchemaItem::GetLabel()
{
	return &m_hszLabel;
}

IHashString *CSchemaItem::GetType()
{
	return &m_hszType;
}

IHashString *CSchemaItem::GetClass()
{
	return &m_hszClass;
}

void CSchemaItem::GetEditorType( IHashString *typeString )
{
	if( typeString != NULL )
	{
		DWORD editorTypeID = m_TypeIDtoEditorTypeIDMap[ m_hszType.GetUniqueID() ];
		*typeString = m_ToolBox->GetHashString( editorTypeID );
	}
}

DWORD CSchemaItem::GetType( DATABASEATTRIBUTEPARAMS *databaseAttributeParams )
{
	DWORD retVal = MSG_NOT_HANDLED;
	if( databaseAttributeParams != NULL )
	{
		
		if( databaseAttributeParams->m_AttributeTypeArchive != NULL )
		{
			IArchive *ar = databaseAttributeParams->m_AttributeTypeArchive;
			ar->SetIsWriting( true );
			ar->SeekTo( 0 );
			ar->Write( m_hszType.GetString() );
		}
		else
		{
			m_ToolBox->Log( LOGWARNING, _T("No archive specified for GetAttributeType.\n") );
		}

		retVal = MSG_HANDLED_STOP;
	}
	else
	{
		m_ToolBox->Log( LOGWARNING, _T("Attribute parameters not defined.\n") );
	}

	return retVal;
}

bool CSchemaItem::IsKey()
{
	bool retVal = false;
	if( m_hszClass == _T("key") )
	{
		retVal = true;
	}

	return retVal;
}

void CSchemaItem::DBRead( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	IDTODATARWMAP::iterator itdrmIter = m_DataReadFunctions.find(m_hszType.GetUniqueID());
	if( itdrmIter != m_DataReadFunctions.end() )
	{
		DATARWFUNC funcPtr;

		funcPtr = itdrmIter->second;
		// read the data.
		(this->*funcPtr)( value, ar, tag );
	}
	else
	{
		m_ToolBox->SetErrorValue(WARN_INVALID_FILE);
		m_ToolBox->Log(LOGWARNING, _T("Invalid item type %s in database schema\n"),
			m_hszType.GetString());
	}
}

void CSchemaItem::WriteArchive( IArchive *ar )
{
	ar->StartClass( _T("edit") );
	ar->Write( m_hszType.GetString(), _T("type") );
	ar->Write( m_hszLabel.GetString(), _T("name") );
	ar->Write( IsKey(), _T("key") );
	ar->EndClass();
}

void CSchemaItem::DBReadChar( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	value->type = DBTYPE_CHAR;
	ar->Read((TCHAR &)value->m_CharValue);
}

void CSchemaItem::DBReadUChar( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	value->type = DBTYPE_UCHAR;
	ar->Read((TCHAR &)value->m_UCharValue);
}

void CSchemaItem::DBReadShort( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	value->type = DBTYPE_SHORT;
	ar->Read((short &)value->m_ShortValue);
}

void CSchemaItem::DBReadUShort( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	value->type = DBTYPE_USHORT;
	ar->Read((short &)value->m_UShortValue);
}

void CSchemaItem::DBReadLong( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	value->type = DBTYPE_LONG;
	ar->Read((int &)value->m_LongValue);
}

void CSchemaItem::DBReadULong( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	value->type = DBTYPE_ULONG;
	ar->Read((int &)value->m_ULongValue);
}

void CSchemaItem::DBReadFloat( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	value->type = DBTYPE_FLOAT;
	ar->Read((float &)value->m_FloatValue);
}

void CSchemaItem::DBReadDouble( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	value->type = DBTYPE_DOUBLE;
	ar->Read((double &)value->m_DoubleValue);
}

void CSchemaItem::DBReadBool( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	value->type = DBTYPE_BOOLEAN;
	ar->Read((bool &)value->m_BoolValue);
}

void CSchemaItem::DBReadString( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	value->type = DBTYPE_STRING;
	StdString szReadString;
	ar->Read( szReadString );

	value->AssignString( (TCHAR *)szReadString.c_str(), _tcslen(szReadString) );
}


void CSchemaItem::DBWrite( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	IDTODATARWMAP::iterator itdrmIter = m_DataWriteFunctions.find(m_hszType.GetUniqueID());
	if( itdrmIter != m_DataWriteFunctions.end() )
	{
		DATARWFUNC funcPtr;

		funcPtr = itdrmIter->second;
		// read the data.
		(this->*funcPtr)( value, ar, tag );
	}
	else
	{
		m_ToolBox->SetErrorValue(WARN_INVALID_FILE);
		m_ToolBox->Log(LOGWARNING, _T("Invalid item type %s in database schema\n"),
			m_hszType.GetString());
	}
}

void CSchemaItem::DBWriteChar( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	ar->Write((TCHAR &)value->m_CharValue, tag);
}

void CSchemaItem::DBWriteShort( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	ar->Write((short &)value->m_ShortValue, tag);
}

void CSchemaItem::DBWriteLong( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	ar->Write((int &)value->m_LongValue, tag);
}

void CSchemaItem::DBWriteFloat( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	ar->Write((float &)value->m_FloatValue, tag);
}

void CSchemaItem::DBWriteDouble( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	ar->Write((double &)value->m_DoubleValue, tag);
}

void CSchemaItem::DBWriteBool( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	ar->Write((bool &)value->m_BoolValue, tag);
}

void CSchemaItem::DBWriteString( DATABASEDATA *value, IArchive *ar, const TCHAR *tag )
{
	ar->Write(value->m_TCharPtr, tag);
}