///============================================================================
/// \file		SchemaItem.h
/// \brief		Declaration of CSchemaItem class which maps a database 
///				attribute to a type
/// \date		04-12-2007
/// \author		Richard Nguyen
/// \note		Copyright (c) 2007 Signature Devices, Inc.
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

#ifndef __SCHEMAITEM_H__
#define __SCHEMAITEM_H__

class CSchemaItem;

typedef void (CSchemaItem::*DATARWFUNC)( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );
typedef map<DWORD, DATARWFUNC> IDTODATARWMAP;

class CSchemaItem : public CObjectTemplate<IObject>
{
public:
	// Default constructor
	CSchemaItem( IHashString *parentName, IHashString *name );

	// Default destructor
	virtual ~CSchemaItem();

	/// \fn		virtual void Init()
	/// \brief	Initialize the object
	void Init();

	/// \fn		virtual void Update()
	/// \brief	Update the object
	bool Update();

	/// \fn		virtual void DeInit()
	/// \brief	Deinitialize the object
	void DeInit();

	static IComponent *Create( int nArgs, va_list argptr );

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	void Serialize( IArchive &ar );

	/// Serialize a single database item's value to the archive.
	void SerializeDatabaseItem( ATTRIBUTEIDTODATAMAP &valuesMap, IArchive &ar );

	/// Sets the default value to the values map.
	void SetDefaultValue( ATTRIBUTEIDTODATAMAP &valuesMap );

	/// Returns the attribute name label.
	IHashString *GetLabel();

	/// Returns the name of the data type.
	IHashString *GetType();

	/// Returns the name of the editor class
	IHashString *GetClass();

	/// Returns the name of the editor-supported data type.
	/// TODO: Editor and engine should be referencing the same data pool.
	void GetEditorType( IHashString *typeString );

	/// Writes the name of the data type to the attribute type archive
	DWORD GetType( DATABASEATTRIBUTEPARAMS *databaseAttributeParams );

	/// Returns true if the schema items is a key attribute.
	bool IsKey();

	void DBRead( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );
	void DBWrite( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );

	virtual void WriteArchive( IArchive *ar );

private:
	void DBReadChar( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );
	void DBReadUChar( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );
	void DBReadShort( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );
	void DBReadUShort( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );
	void DBReadLong( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );
	void DBReadULong( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );
	void DBReadFloat( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );
	void DBReadDouble( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );
	void DBReadBool( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );
	void DBReadString( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );

	void DBWriteChar( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );
	void DBWriteShort( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );
	void DBWriteLong( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );
	void DBWriteFloat( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );
	void DBWriteDouble( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );
	void DBWriteBool( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );
	void DBWriteString( DATABASEDATA *value, IArchive *ar, const TCHAR *tag );

	IToolBox			*m_ToolBox;
	float				m_fVersion;
	CHashString			m_hszLabel;
	CHashString			m_hszType;
	CHashString			m_hszClass;
	DATABASEDATA		m_DefaultValue;

	IDTODATARWMAP		m_DataReadFunctions;
	IDTODATARWMAP		m_DataWriteFunctions;

	CHashString			m_hszCharType;
	CHashString			m_hszUCharType;
	CHashString			m_hszShortType;
	CHashString			m_hszUShortType;
	CHashString			m_hszLongType;
	CHashString			m_hszULongType;
	CHashString			m_hszFloatType;
	CHashString			m_hszDoubleType;
	CHashString			m_hszBoolType;
	CHashString			m_hszStringType;

	CDatabaseSchema		*m_Schema;
	map<DWORD, DWORD>	m_TypeIDtoEditorTypeIDMap;
	map<DWORD, DWORD>	m_TypeIDtoClassTypeIDMap;
};

#endif //#ifndef __SCHEMAITEM_H__