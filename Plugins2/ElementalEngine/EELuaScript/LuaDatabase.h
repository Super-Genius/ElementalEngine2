///============================================================================
/// \file		LuaDatabase.h
/// \brief		Header file for LuaDatabase Structs
/// \date		08-18-2005
/// \author		Josh Lulewicz
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

#ifndef _LUADATABASE_H_
#define _LUADATABASE_H_


// tolua_begin
struct LUADATABASEDATA : DATABASEDATA
{
	char GetCharValue()
	{
		assert(type == DBTYPE_CHAR);
		return m_CharValue;
	}

	unsigned char GetUCharValue()
	{
		assert(type == DBTYPE_UCHAR);
		return m_UCharValue;
	}

	short GetShortValue()
	{
		assert(type == DBTYPE_SHORT);
		return m_ShortValue;
	}

	unsigned short GetUShortValue()
	{
		assert(type == DBTYPE_USHORT);
		return m_UShortValue;
	}

	long GetLongValue()
	{
		assert(type == DBTYPE_LONG);
		return m_LongValue;
	}

	unsigned long GetULongValue()
	{
		assert(type == DBTYPE_ULONG);
		return m_ULongValue;
	}
	float GetFloatValue()
	{
		assert(type == DBTYPE_FLOAT);
		return m_FloatValue;
	}
	double GetDoubleValue()
	{
		assert(type == DBTYPE_DOUBLE);
		return m_DoubleValue;
	}
	bool GetBoolValue()
	{
		assert(type == DBTYPE_BOOLEAN);
		return m_BoolValue;
	}
	char* GetStringValue()
	{
		assert(type == DBTYPE_VOIDPTR);
		return (char*)m_VoidPtr;
	}
};

struct LUADATABASEINFO : DATABASEINFO
{
	LUADATABASEDATA *m_LuaDatabaseData;

	IHashString* GetFileName()
	{
		return m_FileName;
	}
	IHashString* GetSchemaName()
	{
		return m_SchemaName;
	}
	int GetNumItems()
	{
		return (int)m_nItems;
	}

	void SetFileName(IHashString *name)
	{
		m_FileName = name;
	}

	DWORD SIZEOF()
	{
		return sizeof(DATABASEINFO);
	}

	void GetDatabase(IHashString *name)
	{
		m_FileName = name;

		static DWORD msgHash_GetDatabase = CHashString(_T("GetDatabase")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_GetDatabase, sizeof(DATABASEINFO), this); 

		m_LuaDatabaseData = (LUADATABASEDATA*)m_DatabaseData;
	}

	LUADATABASEDATA* GetDatabaseItem(int index)
	{
		if (m_DatabaseData != NULL)
		{
			if (m_LuaDatabaseData == NULL)
			{
				m_LuaDatabaseData = (LUADATABASEDATA*)m_DatabaseData;
			}
			return &m_LuaDatabaseData[index];
		}

		// returning a null!
		return NULL;
	}
};
// tolua_end

// tolua_begin
class CLuaDatabase
{
public:
	/// constructor 
	CLuaDatabase(TCHAR *filename);
	/// destructor
	~CLuaDatabase();

	/// Find a row in the database
	/// \param key - key value for database lookup
	/// \return void
	void FindRow(IHashString *key);

	/// Get a row in the database
	/// \param index - row number to get
	/// \return void
	void GetRow(int index);

	/// Get char value from the database
	/// \param index - column number to retrieve data from
	/// \return char item from database
	char GetCharValue(int index);

	/// Get unsigned char value from the database
	/// \param index - column number to retrieve data from
	/// \return unsigned char item from database
	unsigned char GetUCharValue(int index);

	/// Get short value from the database
	/// \param index - column number to retrieve data from
	/// \return short item from database
	short GetShortValue(int index);

	/// Get unsigned short value from the database
	/// \param index - column number to retrieve data from
	/// \return unsigned short item from database
	unsigned short GetUShortValue(int index);

	/// Get long value from the database
	/// \param index - column number to retrieve data from
	/// \return long item from database
	long GetLongValue(int index);

	/// Get unsigned long value from the database
	/// \param index - column number to retrieve data from
	/// \return unsigned long item from database
	unsigned long GetULongValue(int index);

	/// Get float value from the database
	/// \param index - column number to retrieve data from
	/// \return float item from database
	float GetFloatValue(int index);

	/// Get double value from the database
	/// \param index - column number to retrieve data from
	/// \return double item from database
	double GetDoubleValue(int index);

	/// Get bool value from the database
	/// \param index - column number to retrieve data from
	/// \return bool item from database
	bool GetBoolValue(int index);

	/// Get char* value from the database
	/// \param index - column number to retrieve data from
	/// \return char* item from database
	char* GetStringValue(int index);
	
	/// \brief	Return the number of rows
	///	\return	the number of rows in the database
	int GetNumRows();

	/// \brief	Returns the number of items in the database
	///	\param	index = index into the database
	///	\return the number items in the database
	int GetNumItems(int index);

// tolua_end
private:
	/// pointer to IDatabase interface
	IDatabase *m_Database;
	CHashString m_DatabaseName;
	/// toolbox
	IToolBox *m_ToolBox;
	/// actual database data
	LUADATABASEDATA *m_DatabaseData;
// tolua_begin
};
// tolua_end

#endif // #ifndef _LUADATABASE_H_

