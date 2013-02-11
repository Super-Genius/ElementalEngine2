///============================================================================
/// \file		Database.cpp
/// \brief		Implmentation of CDatabase Object Class
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

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CDatabase);
REGISTER_MESSAGE_HANDLER(GetDatabaseBySchema, OnGetDatabaseBySchema, CDatabase);
REGISTER_MESSAGE_HANDLER(GetDBFileName, OnGetDBFileName, CDatabase);
REGISTER_MESSAGE_HANDLER(GetDBSchemaFileName, OnGetDBSchemaFileName, CDatabase);


CDatabase::CDatabase(IHashString *parentName, IHashString *name, bool bAddToHier) :
	m_hszNameType( _T("CDatabase") ),
	OBJECTTEMPLATE(CDatabaseManager , CDatabase, IDatabase, parentName, name)
{
	m_bAddToHierarchy = bAddToHier;
	if (m_bAddToHierarchy)
	{
		AddToHierarchy();
	}		
	m_ToolBox = EngineGetToolBox();

	m_bValidDB = false;
}

CDatabase::~CDatabase( void )
{
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
	DATABASEDATA *dbData = m_DataBaseInfo.m_DatabaseData;
	delete[] m_DataBaseInfo.m_DatabaseData;
}

IComponent *CDatabase::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool bAddToHier;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new CDatabase(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CDatabase(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CDatabase::Serialize( IArchive &ar )
{
	StdString temp;
	if (ar.IsReading())
	{
		// load the filename
		ar.Read(temp);
		m_FileName = temp.c_str();
	}
	else
	{
		// save the filename
		ar.Write(m_FileName.GetString(), _T("FileName"));
	}

}

void CDatabase::Init()
{
	// grab the database from the database manager
	m_DataBaseInfo.m_FileName = &m_FileName;
	static DWORD msgHash_GetDatabase = CHashString(_T("GetDatabase")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetDatabase, sizeof(DATABASEINFO),	&m_DataBaseInfo);

	if (m_DataBaseInfo.m_DatabaseData == NULL)
	{
		m_bValidDB = false;
		return;
	}

	m_bValidDB = true;

	// make a local copy of the databasedata
	int size = m_DataBaseInfo.m_nItems;
	DATABASEDATA *dbData = m_DataBaseInfo.m_DatabaseData;
	m_DataBaseInfo.m_DatabaseData = new DATABASEDATA[size];
	DATABASEDATA *debug = m_DataBaseInfo.m_DatabaseData;

	for(int i=0; i<size; i++)
	{
		DATABASEDATA tbData(dbData[i]);
		m_DataBaseInfo.m_DatabaseData[i] = tbData;
		//m_DataBaseInfo.m_DatabaseData[i] = dbData[i];
	}

	// build the key map
	BuildDBKeyMap();
}

IHashString* CDatabase::GetComponentType( void )
{
	return &m_hszNameType;
}

bool CDatabase::IsKindOf( IHashString* compType )
{
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

void CDatabase::BuildDBKeyMap()
{
/*	DWORD colID = 0;
	DWORD numberOfColumns;
	DATABASEDATA *dbItem;
	StdString keyName;
	CHashString newKeyName;
	DWORD i;
	DWORD j;
	KEYSET::iterator ksIter;
	TCHAR tempNum[256];
	DWORD numKeys = 0;
	DWORD foundKeys = 0;

	// get the number of columns for this schema
	numberOfColumns = m_DataBaseInfo.m_nRowSize;

	// build key list by going through the view objects
	for (volIter = vol->begin(); volIter != vol->end(); ++volIter)
	{
		// and see which ones are used as keys
		if ((*volIter)->IsKey())
		{
			// for the ones that are add them to the key set
			m_KeySet.insert(colID);
		}
		// increment column id count
		++colID;
	}

	// get the number of keys
	numKeys = m_KeySet.size();
	// get pointer to database data
	dbItem = m_DataBaseInfo.m_DatabaseData;

	// loop though dbItem rows and build key string
	for (i = 0; i < m_DataBaseInfo.m_nItems / numberOfColumns; ++i)
	{
		// go through columns looking for keys
		for (j = 0; j < numberOfColumns; ++j)
		{
			// check if this index is in the map
			ksIter = m_KeySet.find(j);

			// if it is...do your stuff
			if (ksIter != m_KeySet.end())
			{
				// found a key
				++foundKeys;
				// it is so build string
				if (dbItem != NULL)
				{
					// depending on the type insert the key into the string
					switch(dbItem[j].type)
					{
						case DBTYPE_CHAR:
						case DBTYPE_UCHAR:
							tempNum[0] = dbItem[j].m_CharValue;
							tempNum[1] = '\0';
							keyName += tempNum;
							break;
						case DBTYPE_SHORT:
						case DBTYPE_USHORT:
							_itot(dbItem[j].m_ShortValue, tempNum, 10);
							keyName += tempNum;
							break;
						case DBTYPE_LONG:
						case DBTYPE_ULONG:
							_ltot(dbItem[j].m_ShortValue, tempNum, 10);
							keyName += tempNum;
							break;
						case DBTYPE_FLOAT:
							_stprintf(tempNum, _T("%-.5f"), dbItem[j].m_FloatValue);
							keyName += tempNum;
							break;
						case DBTYPE_DOUBLE:
							_stprintf(tempNum, _T("%-.5f"), dbItem[j].m_DoubleValue);
							keyName += tempNum;
							break;
						case DBTYPE_VOIDPTR:
							keyName += (TCHAR *)dbItem[j].m_VoidPtr;
							break;
						default:
							break;
					}
				}

				// if the key value doesn't have any data don't insert colons
				if (_tcscmp(keyName, _T("")) != 0)
				{
					// insert double colons
					if (foundKeys >= 1 && foundKeys < numKeys)
					{
						keyName += _T("::");
					}
				}
			}
		}

		// hash key name
		newKeyName = keyName;
		// add string to map
		m_DataBaseInfo.m_DBDataMap[newKeyName.GetUniqueID()] = dbItem;
		// reset the string
		keyName = _T("");
		// increment pointer
		dbItem += numberOfColumns;
		// reset the found keys
		foundKeys = 0;
	}
*/
}

/// Retrieve a row in the database given an index
DATABASEDATA* CDatabase::GetRow(int index)
{
	if (m_bValidDB == false)
	{
		return NULL;
	}

	DWORD numberOfColumns;
	DATABASEDATA *dbItem;
	DWORD i;

	// get the number of columns for this schema
	numberOfColumns = m_DataBaseInfo.m_nRowSize;

	// grab the start of the items
	dbItem = m_DataBaseInfo.m_DatabaseData;

	// advance pointer to desired row
	for (i = 0; i < m_DataBaseInfo.m_nItems / numberOfColumns; ++i)
	{
		// if we are at the row
		if (i == index)
		{
			// return the database item
			return dbItem;
		}

		// increment pointer
		dbItem += numberOfColumns;
	}

	// row invalid returning NULL!
	return NULL;
}

/// Find a row in the database given a key
DATABASEDATA* CDatabase::FindRow(IHashString *key)
{
	if (m_bValidDB == false)
	{
		return NULL;
	}
	DWORD id;
	KEYTODBDATAMAP::iterator ktdmIter;
	// grab the key to dbdata map
	KEYTODBDATAMAP *keyMap = &m_DataBaseInfo.m_DBDataMap;

	// if key is not NULL!
	if (key)
	{
		// get the unique id for the key
		id = key->GetUniqueID();
		// hunt for key in map
		ktdmIter =  keyMap->find(id);

		// see if we got anything...
		if (ktdmIter != keyMap->end())
		{
			// we did return it!
			return ktdmIter->second;
		}
	}

	// not found returning NULL!
	return NULL;
}


// push our databaseinfo, if we are of this type schema
DWORD CDatabase::OnGetDatabaseBySchema(DWORD size, void *params)
{
	if (m_bValidDB == false)
	{
		return MSG_NOT_HANDLED;
	}
	GETDATABASEBYSCHEMA *gdbbs;
	IHashString *srcSchema1;
	IHashString *srcSchema2;
	// verify the message we have received is the the right size
	VERIFY_MESSAGE_SIZE(size, sizeof(GETDATABASEBYSCHEMA));

	// cast params to a GETDATABASEBYSCHEMA
	gdbbs = (GETDATABASEBYSCHEMA *)params;

	srcSchema1 = gdbbs->m_SchemaName;
	srcSchema2 = m_DataBaseInfo.m_SchemaName;
	if ((srcSchema1 != NULL) && srcSchema2 != NULL)
	{
		if (srcSchema1->GetUniqueID() == srcSchema2->GetUniqueID())
		{
			if (gdbbs->m_dbInfoList != NULL)
			{
				gdbbs->m_dbInfoList->push_back(&m_DataBaseInfo);
			}
		}
	}

	return MSG_HANDLED_PROCEED;
}

int CDatabase::GetNumRows()
{
	if (m_bValidDB == false)
	{
		return 0;
	}

	DWORD numberOfColumns;

	// get the number of columns for this schema
	numberOfColumns = m_DataBaseInfo.m_nRowSize;
	
	// calculate the number of rows
	int iNumRows = m_DataBaseInfo.m_nItems / numberOfColumns;

	return iNumRows;
}

int CDatabase::GetNumItems()
{
	if (m_bValidDB == false)
	{
		return 0;
	}
	return m_DataBaseInfo.m_nItems;
}

DWORD CDatabase::OnGetDBFileName(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));

	IHashString *outFile = (IHashString*)param;
	*outFile = m_FileName.GetString();
	return MSG_HANDLED_STOP;
}

DWORD CDatabase::OnGetDBSchemaFileName(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));
	
	IHashString *outFile = (IHashString*)param;
	if (m_bValidDB)
	{
		*outFile = *m_DataBaseInfo.m_SchemaFile;
	}
	return MSG_HANDLED_STOP;
}