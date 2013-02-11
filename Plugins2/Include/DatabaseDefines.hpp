///============================================================================
/// \file		DatabaseDefines.h
/// \brief		Header file for Database structures and messages
/// \date		08-02-2005
/// \author		Kenneth Hurley
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

#ifndef _DATABASEDEFINES_H
#define _DATABASEDEFINES_H

class IDatabase;

namespace ElementalEngine
{

enum DBDATATYPES
{
	DBTYPE_CHAR = 0,
	DBTYPE_UCHAR = 1,
	DBTYPE_SHORT = 2,
	DBTYPE_USHORT= 3,
	DBTYPE_LONG = 4,
	DBTYPE_ULONG = 5,
	DBTYPE_FLOAT = 6,
	DBTYPE_DOUBLE = 7,
	DBTYPE_BOOLEAN = 8,
	DBTYPE_VOIDPTR = 9,			/// careful, this won't serialize to XML, unless TCHAR *
	DBTYPE_STRING = 10
};

} // namespace ElementalEngine

typedef struct DATABASEDATA
{
	DBDATATYPES type;					/// type contained in here
	union
	{
		char	m_CharValue;			/// chars
		unsigned char m_UCharValue;
		short m_ShortValue;				/// shorts
		unsigned short m_UShortValue;
		long m_LongValue;				/// longs
		unsigned long m_ULongValue;
		float m_FloatValue;				/// floating point
		double m_DoubleValue;		
		bool m_BoolValue;				/// bool
		TCHAR *m_TCharPtr;				/// TCHAR *pointer
		void *m_VoidPtr;				/// pointer to anything
	};
	
	// constructor
	DATABASEDATA()
		: type(ElementalEngine::DBTYPE_BOOLEAN)
		, m_BoolValue(false)
	{
	};

	// copy constructor
	DATABASEDATA(DATABASEDATA &dbd)
		: type(ElementalEngine::DBTYPE_BOOLEAN)
	{
		Assign(dbd);
	};

	// destructor
	~DATABASEDATA()
	{
		Release();
	};

	// operator=
	DATABASEDATA& operator=(DATABASEDATA &dbd)
	{
		Assign(dbd);
		return *this;
	};

	void Assign(const DATABASEDATA &dbd)
	{
		if (dbd.type == ElementalEngine::DBTYPE_VOIDPTR)
		{
			AssignString(dbd.m_TCharPtr, _tcslen(dbd.m_TCharPtr));
		}
		else
		{
			Release();
			type = dbd.type;
			m_DoubleValue = dbd.m_DoubleValue;
		}
	}

	bool AssignString(TCHAR *text, size_t len)
	{
		Release();
		type = ElementalEngine::DBTYPE_VOIDPTR;
		const size_t size = sizeof(TCHAR) * (len + 1);
		m_TCharPtr = (TCHAR *)GLOBALALLOC(size);
		if (m_TCharPtr == NULL)
		{
			IToolBox *pToolBox = EngineGetToolBox();
			pToolBox->SetErrorValue(ERR_OUT_OF_MEMORY);
			pToolBox->Log(LOGERROR, _T("%s(%i): Unable to allocate %d bytes in file\n"), 
										__FILE__, __LINE__, size);
		}
		else
		{
			memcpy(m_TCharPtr, text, size);
		}
		return m_TCharPtr != NULL;
	}

	void Release()
	{
		if (type == ElementalEngine::DBTYPE_VOIDPTR && m_TCharPtr != NULL)
		{
			GLOBALFREE(m_TCharPtr);
			m_TCharPtr = NULL;
		}
	}

} DATABASEDATA;

typedef map<DWORD, DATABASEDATA *> KEYTODBDATAMAP;
//
typedef struct DATABASEINFO
{
	IHashString *m_DatabaseName;		/// database name
	IHashString *m_FileName;			/// database file name
	IHashString *m_SchemaName;			/// the schema associated with this data
	IHashString *m_SchemaFile;			/// the schema file associated with this data
	DWORD m_RefCount;					/// the initial reference count
	DWORD m_nItems;						/// the number of items in the struct
	DWORD m_nRowSize;					/// the size of a row in the database
	DATABASEDATA *m_DatabaseData;		/// array of structs
	KEYTODBDATAMAP m_DBDataMap;			/// map of keys to database data pointers
	DATABASEINFO()
	{
		m_DatabaseName = NULL;
		m_FileName = NULL;
		m_SchemaName = NULL;
		m_SchemaFile = NULL;
		m_RefCount = 0;
		m_nItems = 0;
		m_nRowSize = 0;
		m_DatabaseData = NULL;
	};
} DATABASEINFO;

typedef list<DATABASEINFO *> DATABASEINFOLIST;

// structure for messages to databases to retrieve
// a specific schema type of database
typedef struct GETDATABASEBYSCHEMA
{
	IHashString *m_SchemaName;			/// the schema associated with this data
	DATABASEINFOLIST *m_dbInfoList;		/// the list to add to if match
	GETDATABASEBYSCHEMA()
	{
		m_SchemaName = NULL;
		m_dbInfoList = NULL;
	};
} GETDATABASEBYSCHEMA;

// structure for messages to databases to create
// a database object
typedef struct DBCREATEPARAMS
{
	TCHAR *fileName;		// database file name
	IDatabase **dbObj;		// database object created
} DBCREATEPARAMS;

// type definition for a database value mapped to its attribute name ID.
typedef map<DWORD, DATABASEDATA*> ATTRIBUTEIDTODATAMAP;


struct DBFINDDATABASE
{
	IHashString *m_DatabaseFileName;
	IHashString *m_DatabaseObjName;
	bool m_bIsFound;
	
	DBFINDDATABASE()
	{
		m_DatabaseFileName = NULL;
		m_DatabaseObjName = NULL;
		m_bIsFound = false;
	};

};


// Struct passed in messages when getting and setting data entry attributes
// tolua_begin
typedef struct TYPEDATABASEINFO
{
	IHashString *phszDBName;		/// database name
	IHashString *phszFilepath;		/// database file name
	IArchive *pArchive;
	TYPEDATABASEINFO()
	{
		phszDBName = NULL;
		phszFilepath = NULL;
		pArchive = NULL;
	}
} TYPEDATABASEINFO;

typedef struct DATABASEATTRIBUTEPARAMS
{
	int m_Index;
	IHashString *m_AttributeName;
	DATABASEDATA *m_Data;
	IArchive *m_AttributeTypeArchive;
	IArchive *m_AttributeArchive;

	DATABASEATTRIBUTEPARAMS()
	{
		m_Index = -1;
		m_Data = NULL;
		m_AttributeName = NULL;
		m_AttributeTypeArchive = NULL;
		m_AttributeArchive = NULL;
	}
} DATABASEATTRIBUTEPARAMS;

// Struct passed in messages when retrieving database information
typedef struct DATABASEINFOPARAMS
{
	int m_NumItems;

	DATABASEINFOPARAMS()
	{
		m_NumItems = 0;
	}
}
DATABASEINFOPARAMS;

// tolua_end

typedef struct LOADGLOBALATTRIBUTESPARAMS
{
	IHashString *Name;
	IHashString *SchemaFile;
	IHashString *SchemaName;

	LOADGLOBALATTRIBUTESPARAMS()
	{
		Name = NULL;
		SchemaFile = NULL;
		SchemaName = NULL;
	}
}
LOADGLOBALATTRIBUTESPARAMS;
#endif // #ifndef _DATABASEDEFINES_H