///============================================================================
/// \file		IDatabase.h
/// \brief		Header file for Database Interface
/// \date		09-13-2005
/// \author		Josh Lulewicz
/// \note		Copyright (c) 2005 Signature Devices, Inc.
///============================================================================

#ifndef _IDATABASE_H_
#define _IDATABASE_H_

class IDatabase : public IObject
{
public:
	/// Retrieve a row in the database given an index
	/// \param index = row number
	/// \return pointer to DATABASEDATA signifying the beginning of a row
	virtual DATABASEDATA *GetRow(int index) = 0;

	/// Find a row in the database given a key
	/// \param key = hash string name of key to desired row
	/// \return pointer to DATABASEDATA signifying the beginning of a row
	virtual DATABASEDATA *FindRow(IHashString *key) = 0;

	/// \brief	Return the number of rows
	///	\return	the number of rows in the database
	virtual int GetNumRows() = 0;
		
	/// \brief	Returns the number of items in the database
	///	\return the number items in the database
	virtual int GetNumItems() = 0;
};

#endif // #ifndef _IDATABASE_H_