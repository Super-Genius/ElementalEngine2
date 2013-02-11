///============================================================================
/// \file		DatabaseIO.cpp
/// \brief		Implementation file for Lua Script Loader
/// \date		05-31-2005
/// \author		Josh Lulewicz
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
#include "DatabaseIO.hpp"
#include "BaseMessages.h"

REGISTER_COMPONENT_SINGLETON(CDatabaseIO);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadDatabaseFile, CDatabaseIO);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveDatabaseFile, CDatabaseIO);

CHashString CDatabaseIO::m_HashName(_T("CDatabaseIO"));

LSRC_MESSAGE CDatabaseIO::m_LoadSaveRegisterData = {
		&CDatabaseIO::m_HashName,
		_T("Database XML Files (*.dxl)\0" "*.dxl\0"
		"\0\0"),
		_T("Database XML Files (*.dxl)\0" "*.dxl\0"
		"\0\0"),
		false, NULL,
};

TCHAR *dbTypeString[] = { 
			_T("char"),
			_T("unsigned char"),
			_T("short int"),
			_T("unsigned short int"),
			_T("long int"),
			_T("unsigned long int"),
			_T("float"),
			_T("double"),
			_T("bool"),
			_T("string")
};

CDatabaseIO::CDatabaseIO() 
{
	// save for convenience
	m_ToolBox = EngineGetToolBox();
	DWORD check;

	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	check = m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE),
		&m_LoadSaveRegisterData, NULL, NULL);

	if (!check)
	{
		// log error RegisterLoadSaveComponent failed!
	}

	// build map of uniqueIDs types that we recognize to function pointers
	m_DataReadFunctions[CHashString(_T("char")).GetUniqueID()] = 
			&CDatabaseIO::DBReadChar;
	m_DataReadFunctions[CHashString(_T("unsigned char")).GetUniqueID()] =
			&CDatabaseIO::DBReadUChar;
	m_DataReadFunctions[CHashString(_T("short int")).GetUniqueID()] = 
			&CDatabaseIO::DBReadShort;
	m_DataReadFunctions[CHashString(_T("unsigned short int")).GetUniqueID()] =
			&CDatabaseIO::DBReadUShort;
	m_DataReadFunctions[CHashString(_T("long int")).GetUniqueID()] = 
			&CDatabaseIO::DBReadLong;
	m_DataReadFunctions[CHashString(_T("unsigned long int")).GetUniqueID()] =
			&CDatabaseIO::DBReadULong;
	m_DataReadFunctions[CHashString(_T("float")).GetUniqueID()] = 
			&CDatabaseIO::DBReadFloat;
	m_DataReadFunctions[CHashString(_T("double")).GetUniqueID()] = 
			&CDatabaseIO::DBReadDouble;
	m_DataReadFunctions[CHashString(_T("bool")).GetUniqueID()] = 
			&CDatabaseIO::DBReadBool;
	m_DataReadFunctions[CHashString(_T("string")).GetUniqueID()] = 
			&CDatabaseIO::DBReadString;

	// build map of uniqueIDs types that we recognize to function pointers
	m_DataWriteFunctions[DBTYPE_CHAR] = &CDatabaseIO::DBWriteChar;
	m_DataWriteFunctions[DBTYPE_UCHAR] = &CDatabaseIO::DBWriteChar;
	m_DataWriteFunctions[DBTYPE_SHORT] = &CDatabaseIO::DBWriteShort;
	m_DataWriteFunctions[DBTYPE_USHORT] = &CDatabaseIO::DBWriteShort;
	m_DataWriteFunctions[DBTYPE_LONG] = &CDatabaseIO::DBWriteLong;
	m_DataWriteFunctions[DBTYPE_ULONG] = &CDatabaseIO::DBWriteLong;
	m_DataWriteFunctions[DBTYPE_FLOAT] = &CDatabaseIO::DBWriteFloat;
	m_DataWriteFunctions[DBTYPE_DOUBLE] = &CDatabaseIO::DBWriteDouble;
	m_DataWriteFunctions[DBTYPE_BOOLEAN] = &CDatabaseIO::DBWriteBool;
	m_DataWriteFunctions[DBTYPE_VOIDPTR] = &CDatabaseIO::DBWriteString;
}

// create function for this world loader
IComponent *CDatabaseIO::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CDatabaseIO );
}

IHashString *CDatabaseIO::GetComponentType()
{
	return &m_HashName;
}

bool CDatabaseIO::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// load a file if we recognize the type.
DWORD CDatabaseIO::OnLoadDatabaseFile(DWORD size, void *params)
{
	DWORD retVal;
	IXMLArchive *Archive = NULL;
	DWORD currNode = 0;
	StdString szSchemaType;
	StdString szSchemaFile; 
	StdString szNumItems;
	StdString szRowSize;
	StdString szType;
	StdString szName;
	DWORD nItems;
	DWORD nRowSize;
	DATABASEINFO dbInfo;		// the database info structure
	DATABASEDATA *curEntry;		// current entry filling in array area
	IDTODATARWMAP::iterator itdrmIter;
	CHashString streamType(_T("File"));
	CHashString hszType;

	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;

	CREATEARCHIVE ca;
	ca.streamData = (void *)pFileName;
	ca.mode = STREAM_MODE_READ;
	ca.streamType = &streamType;
	// call the Archive factory to create an XML archive
	static DWORD msgHash_CreateXMLArchive = CHashString( _T("CreateXMLArchive") ).GetUniqueID();
	if (retVal = m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return retVal;
	}

	Archive = dynamic_cast<IXMLArchive *>(ca.archive);
	DWORD numNodes = Archive->GetNumberOfNodes();
	//TODO: Fix this so that it can be called after the GetNode() call
	// bug #0000878: Error with GetNumAttributes() in CXMLArchive 
	DWORD numAttributes = Archive->GetNumAttributes();

	StdString szNodeName;
	Archive->GetNode(szNodeName);
	// increment current node position
	++currNode;

	// must start with <Database>
	if (szNodeName != _T("Database"))
	{
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_FILE);
		EngineGetToolBox()->Log(LOGWARNING, _T("Invalid database file %s\n"), pFileName);
		Archive->Close();
		return MSG_ERROR;
	}
	else
	{
        // read the schema type of the database
		Archive->Read( szSchemaType, _T("Schema") );
		// read the schema file for the database
		if( numAttributes == 4 )
		{
			Archive->Read( szSchemaFile, _T("SchemaFile") );
			// read the number of items in the database
		}
		Archive->Read( szNumItems, _T("NumItems"));
		nItems = _tstol(szNumItems.c_str());
		// read the row size in the database
		Archive->Read(szRowSize, _T("RowSize"));
		nRowSize = _tstol(szRowSize);
		// check for no items.
		if (nItems <= 0)
		{
			EngineGetToolBox()->Log(LOGWARNING, _T("No entries in database file %s\n"), pFileName);
			Archive->Close();
			return MSG_HANDLED_STOP;
		}
	}

	dbInfo.m_DatabaseData = (DATABASEDATA *)GLOBALALLOC((nItems * sizeof(DATABASEDATA)));
	if (dbInfo.m_DatabaseData == NULL)
	{
		EngineGetToolBox()->SetErrorValue(ERR_OUT_OF_MEMORY);
		EngineGetToolBox()->Log(LOGERROR, _T("Unable to allocate %d bytes in file %s at line %d\n"), 
			(nItems * sizeof(DATABASEDATA)), __FILE__, __LINE__);
		Archive->Close();
		return MSG_ERROR;
	}

	// update progress bar
//	m_ToolBox->SendMessage(msgHash_UpdateProgressBar, sizeof(LOADINGBARPARAMS), &pbp);

	dbInfo.m_nItems = 0;
	dbInfo.m_nRowSize = nRowSize;

	// set current entry we are working on
	curEntry = dbInfo.m_DatabaseData;

	UINT numRead = 0;
	while (Archive->GetNode(szNodeName))
	{
		numRead++;
		if (numRead > nItems)
		{
			m_ToolBox->Log(LOGERROR, _T("Database IO: numItems specified is less than actual read items in file %s\n"), pFileName);
			GLOBALFREE(&dbInfo.m_DatabaseData);
			dbInfo.m_DatabaseData = NULL;
			return MSG_ERROR;
		}

		if (szNodeName != _T("DBItem"))
		{
			EngineGetToolBox()->SetErrorValue(WARN_INVALID_FILE);
			EngineGetToolBox()->Log(LOGWARNING, _T("Invalid database file %s\n"), pFileName);
			Archive->Close();
			return MSG_ERROR;
		}

		// get type of this node
		Archive->Read( szType, _T("Type") );
		hszType = szType.c_str();

		// now do a look up for the read functions.
		itdrmIter = m_DataReadFunctions.find(hszType.GetUniqueID());
		if (itdrmIter != m_DataReadFunctions.end())
		{
			DATARWFUNC funcPtr;

			funcPtr = itdrmIter->second;
			// read the data.
			(this->*funcPtr)(Archive, curEntry, _T("Value"));
			dbInfo.m_nItems++;
		}
		else
		{
			EngineGetToolBox()->SetErrorValue(WARN_INVALID_FILE);
			EngineGetToolBox()->Log(LOGWARNING, _T("Invalid item type %s in database file %s\n"),
				hszType.GetString(), pFileName);
		}

		curEntry++;

		++currNode;
	}

	if (numRead < nItems)
	{
		m_ToolBox->Log(LOGERROR, _T("Database IO: numItems specified is greater than actual read items in file %s\n"), pFileName);
		GLOBALFREE(&dbInfo.m_DatabaseData);
		dbInfo.m_DatabaseData = NULL;
		return MSG_ERROR;
	}	

	Archive->Close();

	CHashString fileName(pFileName);
	CHashString hszSchemaType(szSchemaType.c_str());
	dbInfo.m_FileName = &fileName;
	dbInfo.m_SchemaName = &hszSchemaType;

	// no need to check this return value because handler might not be loaded
	static DWORD msgHash_AddDatabase = CHashString(_T("AddDatabase")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddDatabase, sizeof(DATABASEINFO), &dbInfo);

	return MSG_HANDLED_STOP;
}

// Save a file if we recognize the type.
DWORD CDatabaseIO::OnSaveDatabaseFile(DWORD size, void *params)
{
	static DWORD msgHash_UpdateProgressBar = CHashString(_T("UpdateProgressBar")).GetUniqueID();

	IXMLArchive *Archive = NULL;
	DATABASEINFO dbInfo;		// the database info structure
	StdString temp;
	DATABASEDATA *dbData;
	IDTODATARWMAP::iterator itdwmIter;	// writer function iterator
	TCHAR tempNum[256];
	CHashString hszFileName;
	DWORD i;
	DWORD retVal;

	CHashString streamType(_T("File"));

	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;

	hszFileName = pFileName;
	dbInfo.m_FileName = &hszFileName;
	// now grab the database information
	static DWORD msgHash_GetDatabase = CHashString(_T("GetDatabase")).GetUniqueID();
	if (retVal = m_ToolBox->SendMessage(msgHash_GetDatabase, sizeof(DATABASEINFO), &dbInfo) != MSG_HANDLED)
	{
		EngineGetToolBox()->SetErrorValue(WARN_OBJECT_NOT_FOUND);
		EngineGetToolBox()->Log(LOGWARNING, _T("Invalid database for file %s\n"), pFileName);
		return MSG_ERROR;
	}

	// no items in database?
	if (dbInfo.m_nItems == 0)
	{
		return MSG_HANDLED_STOP;
	}

	temp = _T("");
	if (dbInfo.m_SchemaName != NULL)
	{
		temp = dbInfo.m_SchemaName->GetString();
	}

	CREATEARCHIVE ca;
	ca.streamData = (void *)pFileName;
	ca.mode = STREAM_MODE_WRITE;
	ca.streamType = &streamType;
	// call the Archive factory to create an XML archive
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (retVal = m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return retVal;
	}

	Archive = dynamic_cast<IXMLArchive *>(ca.archive);

	Archive->CreateHeader();
	Archive->StartClass(_T("Database"));

	temp = _T("");
	if (dbInfo.m_SchemaName!= NULL)
	{
		temp = dbInfo.m_SchemaName->GetString();
	}
	// write the schema type of the database
	Archive->Write( temp,  _T("Schema") );

	temp = _T("");
	if( dbInfo.m_SchemaFile != NULL )
	{
		temp = dbInfo.m_SchemaFile->GetString();
	}
	// write the schema file for the database
	Archive->Write( temp, _T("SchemaFile") );

	// write the number of items in the database
	_ltot(dbInfo.m_nItems, tempNum, 10);
	Archive->Write(tempNum, _T("NumItems"));
	// write the row size in the database
	_ltot(dbInfo.m_nRowSize, tempNum, 10);
	Archive->Write(tempNum, _T("RowSize"));

	dbData = dbInfo.m_DatabaseData;

	for (i=0; i<dbInfo.m_nItems; i++)
	{
		// map type to write routine
		itdwmIter = m_DataWriteFunctions.find(dbData->type);
		if (itdwmIter != m_DataWriteFunctions.end())
		{
			Archive->StartClass(_T("DBItem"));
			DATARWFUNC funcPtr;

			Archive->Write(dbTypeString[dbData->type], _T("Type"));

			funcPtr = itdwmIter->second;
			// read the data., we should pass in the schema data names
			(this->*funcPtr)(Archive, dbData, _T("Value"));
			Archive->EndClass();
		}

		// next data item
		++dbData;
	}

	Archive->EndClass();	
	Archive->Close();

	return MSG_HANDLED_STOP;
}


void CDatabaseIO::DBReadChar(IArchive *ar, DATABASEDATA *data, TCHAR *tag)
{
	data->type = DBTYPE_CHAR;
	ar->Read((TCHAR &)data->m_CharValue);
}

void CDatabaseIO::DBReadUChar(IArchive *ar, DATABASEDATA *data, TCHAR *tag)
{
	data->type = DBTYPE_UCHAR;
	ar->Read((TCHAR &)data->m_UCharValue);
}

void CDatabaseIO::DBReadShort(IArchive *ar, DATABASEDATA *data, TCHAR *tag)
{
	data->type = DBTYPE_SHORT;
	ar->Read((short &)data->m_ShortValue);
}

void CDatabaseIO::DBReadUShort(IArchive *ar, DATABASEDATA *data, TCHAR *tag)
{
	data->type = DBTYPE_USHORT;
	ar->Read((short &)data->m_UShortValue);
}

void CDatabaseIO::DBReadLong(IArchive *ar, DATABASEDATA *data, TCHAR *tag)
{
	data->type = DBTYPE_LONG;
	ar->Read((int &)data->m_LongValue);
}

void CDatabaseIO::DBReadULong(IArchive *ar, DATABASEDATA *data, TCHAR *tag)
{
	data->type = DBTYPE_ULONG;
	ar->Read((int &)data->m_ULongValue);
}

void CDatabaseIO::DBReadFloat(IArchive *ar, DATABASEDATA *data, TCHAR *tag)
{
	data->type = DBTYPE_FLOAT;
	ar->Read((float &)data->m_FloatValue);
}

void CDatabaseIO::DBReadDouble(IArchive *ar, DATABASEDATA *data, TCHAR *tag)
{
	data->type = DBTYPE_DOUBLE;
	ar->Read((double &)data->m_DoubleValue);
}

void CDatabaseIO::DBReadBool(IArchive *ar, DATABASEDATA *data, TCHAR *tag)
{
	data->type = DBTYPE_BOOLEAN;
	ar->Read((bool &)data->m_BoolValue);
}

void CDatabaseIO::DBReadString(IArchive *ar, DATABASEDATA *data, TCHAR *tag)
{
	StdString tempStr;
	ar->Read(tempStr);
	data->AssignString( (TCHAR*)tempStr.c_str(), tempStr.length());
}

void CDatabaseIO::DBWriteChar(IArchive *ar, DATABASEDATA *data, TCHAR *tag)
{
	ar->Write((TCHAR &)data->m_CharValue, tag);
}

void CDatabaseIO::DBWriteShort(IArchive *ar, DATABASEDATA *data, TCHAR *tag)
{
	ar->Write((short &)data->m_ShortValue, tag);
}

void CDatabaseIO::DBWriteLong(IArchive *ar, DATABASEDATA *data, TCHAR *tag)
{
	ar->Write((int &)data->m_LongValue, tag);
}

void CDatabaseIO::DBWriteFloat(IArchive *ar, DATABASEDATA *data, TCHAR *tag)
{
	ar->Write((float &)data->m_FloatValue, tag);
}

void CDatabaseIO::DBWriteDouble(IArchive *ar, DATABASEDATA *data, TCHAR *tag)
{
	ar->Write((double &)data->m_DoubleValue, tag);
}

void CDatabaseIO::DBWriteBool(IArchive *ar, DATABASEDATA *data, TCHAR *tag)
{
	ar->Write((bool &)data->m_BoolValue, tag);
}

void CDatabaseIO::DBWriteString(IArchive *ar, DATABASEDATA *data, TCHAR *tag)
{
	ar->Write((TCHAR *)data->m_VoidPtr, tag);
}
