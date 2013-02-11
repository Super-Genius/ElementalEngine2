///============================================================================
/// \file		Database.h
/// \brief		Declaration of CDatabase Object Class
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

#ifndef __DATABASE_H__
#define __DATABASE_H__

typedef set<DWORD> KEYSET;

class CDatabase : public CObjectTemplate<IDatabase>
{
private:
	/// Default Constructor
	CDatabase(IHashString *parentName, IHashString *name, bool bAddToHier);
	/// Default Destructor
	~CDatabase();

public:
	static IComponent *Create(int nArgs, va_list argptr);

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	virtual void Init();

	/// Build key map for database
	/// \param void
	/// \return void
	virtual void BuildDBKeyMap();

	/// Retrieve a row in the database given an index
	/// \param index = row number
	/// \return pointer to DATABASEDATA signifying the beginning of a row
	virtual DATABASEDATA *GetRow(int index);

	/// Find a row in the database given a key
	/// \param key = hash string name of key to desired row
	/// \return pointer to DATABASEDATA signifying the beginning of a row
	virtual DATABASEDATA *FindRow(IHashString *key);

	/// message handler to push our databaseinfo
	/// if we are of this type schema
	/// \param size = sizeof(GETDATABASEBYSCHEMA)
	/// \param param = GETDATABASEBYSCHEMA *
	/// \return MSG_XXX id, if handled, etc.
	DWORD OnGetDatabaseBySchema(DWORD size, void *param);

	/// Returns the file name that this object is associated with
	/// \param size = sizeof(IHashString)
	/// \param param = IHashString with filename data to return
	/// \return MSG_HANDLED_STOP
	DWORD OnGetDBFileName(DWORD size, void *param);

	/// Returns the schema file name that this object is associated with
	/// \param size = sizeof(IHashString)
	/// \param param = IHashString with schema filename data to return
	/// \return MSG_HANDLED_STOP
	DWORD OnGetDBSchemaFileName(DWORD size, void *param);
	
	/// \brief	Return the number of rows
	///	\return	the number of rows in the database
	int GetNumRows();

	/// \brief	Returns the number of items in the database
	///	\return the number items in the database
	int GetNumItems();	

private:
	/// Pointed to the EE Tool Box
	IToolBox *m_ToolBox;

	/// Class Type
	CHashString m_hszNameType;

	CHashString m_FileName;			/// filename association

	DATABASEINFO m_DataBaseInfo;	/// pointer to my database info

	/// key list (indices of key fields)
	KEYSET m_KeySet;

	bool m_bAddToHierarchy;

	bool m_bValidDB;

};

#endif //__DATABASE_H__

