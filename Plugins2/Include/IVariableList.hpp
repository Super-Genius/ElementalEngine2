///============================================================================
/// \file		IVariableList.h
/// \brief		Header file for easy to use variable list interface
/// \date		08-17-2005
/// \author		Kenneth Hurley
/// \note		Copyright (c) 2005 Signature Devices, Inc.
///============================================================================

#ifndef _IVARIABLELIST_H_
#define _IVARIABLELIST_H_

#include "DatabaseDefines.hpp"

class IVariableList : public IComponent
{
public:
	/// public virtual desctructor for dynamic_cast RTTI
	virtual ~IVariableList(void) {};

	/// add an database with VarDatabase schema to variable access list
	/// \param dbInfo - the database information to add to the list
	virtual void AddDatabase(DATABASEINFO *dbInfo) = 0;

	/// scans an object for database types and adds them to variable list
	/// \param name - the name of the object to scan
	virtual void AddObjectDatabases(IHashString *name) = 0;

	/// Retrieve a variable from the map
	/// \param varName = variable name to retrieve
	/// \param outValue - pointer to fill in with DATABASEDATA pointer
	/// \return bool - true if found, otherwise false
	virtual bool GetValue(IHashString *varName, DATABASEDATA **outValue) = 0;

	/// Set the variable in a map
	/// \param varName = variable name to retrieve
	/// \param outValue - pointer to fill in with DATABASEDATA pointer
	/// \return bool - true if set, otherwise false if added
	virtual bool SetValue(IHashString *varName, DATABASEDATA *outValue) = 0;
};

#endif // #ifndef _IVARIABLELIST_H_
