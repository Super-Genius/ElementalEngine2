///============================================================================
/// \file		LuaVariableList.h
/// \brief		Header file for LuaVariableList
/// \date		08-17-2005
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

#ifndef	CLUAVARIABLELIST_H
#define	CLUAVARIABLELIST_H

// tolua_begin
class CLuaVariableList
{
public:
	CLuaVariableList(void);
	~CLuaVariableList(void);

	/// add an database with VarDatabase schema to variable access list
	/// \param dbInfo - the database information to add to the list
	void AddDatabase(IHashString *fileName);

	/// scans an object for database types and adds them to variable list
	/// \param name - the name of the object to scan
	void AddObjectDatabases(IHashString *name);

	/// Retrieves a character variable from the map
	/// \param varName - variable name to retrieve
	/// \return char - value of the variable
	char GetValueChar(IHashString *varName);

	/// Retrieves a unsigned character variable from the map
	/// \param varName - variable name to retrieve
	/// \return unsigned char - value of the variable
	unsigned char GetValueUChar(IHashString *varName);

	/// Retrieves a short variable from the map
	/// \param varName - variable name to retrieve
	/// \return short - value of the variable
	short GetValueShort(IHashString *varName);

	/// Retrieves a unsigned short variable from the map
	/// \param varName - variable name to retrieve
	/// \return unsigned short - value of the variable
	unsigned short GetValueUShort(IHashString *varName);

	/// Retrieves a long variable from the map
	/// \param varName - variable name to retrieve
	/// \return long - value of the variable
	long GetValueLong(IHashString *varName);

	/// Retrieves a unsigned long variable from the map
	/// \param varName - variable name to retrieve
	/// \return unsigned long - value of the variable
	unsigned long GetValueULong(IHashString *varName);

	/// Retrieves a float variable from the map
	/// \param varName - variable name to retrieve
	/// \return float - value of the variable
	float GetValueFloat(IHashString *varName);

	/// Retrieves a double variable from the map
	/// \param varName - variable name to retrieve
	/// \return double - value of the variable
	double GetValueDouble(IHashString *varName);

	/// Retrieves a bool variable from the map
	/// \param varName - variable name to retrieve
	/// \return bool - value of the variable
	bool GetValueBool(IHashString *varName);

	/// Retrieves a TCHAR * variable from the map
	/// \param varName - variable name to retrieve
	/// \return TCHAR * - value of the variable
	TCHAR* GetValueTCHAR(IHashString *varName);

	/// Retrieves a void * variable from the map
	/// \param varName - variable name to retrieve
	/// \return void * - value of the variable
	void* GetValueVoidPtr(IHashString *varName);


	/// Sets a character variable from the map
	/// \param varName - variable name to set
	/// \param value - new value to set
	/// \return void
	void SetValueChar(IHashString *varName, char value);

	/// Sets a unsigned character variable from the map
	/// \param varName - variable name to set
	/// \param value - new value to set
	/// \return void
	void SetValueUChar(IHashString *varName, unsigned char value);

	/// Sets a short variable from the map
	/// \param varName - variable name to set
	/// \param value - new value to set
	/// \return void
	void SetValueShort(IHashString *varName, short value);

	/// Sets a unsigned short variable from the map
	/// \param varName - variable name to set
	/// \param value - new value to set
	/// \return void
	void SetValueUShort(IHashString *varName, unsigned short value);

	/// Sets a long variable from the map
	/// \param varName - variable name to set
	/// \param value - new value to set
	/// \return void
	void SetValueLong(IHashString *varName, long value);

	/// Sets a unsigned long variable from the map
	/// \param varName - variable name to set
	/// \param value - new value to set
	/// \return void
	void SetValueULong(IHashString *varName, unsigned long value);

	/// Sets a float variable from the map
	/// \param varName - variable name to set
	/// \param value - new value to set
	/// \return void
	void SetValueFloat(IHashString *varName, float value);

	/// Sets a double variable from the map
	/// \param varName - variable name to set
	/// \param value - new value to set
	/// \return void
	void SetValueDouble(IHashString *varName, double value);

	/// Sets a TCHAR * variable from the map
	/// \param varName - variable name to set
	/// \param value - new value to set
	/// \return void
	void SetValueTCHAR(IHashString *varName, TCHAR *value);

	/// Sets a void * variable from the map
	/// \param varName - variable name to set
	/// \param value - new value to set
	/// \return void
	void SetValueVoidPtr(IHashString *varName, void *value);
// tolua_end
private:
	IToolBox *m_ToolBox;
	IVariableList *m_VariableList;
// tolua_begin
};
// tolua_end

#endif // #ifndef CLUAVARIABLELIST_H