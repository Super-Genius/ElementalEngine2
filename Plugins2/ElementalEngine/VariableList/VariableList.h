///============================================================================
/// \file		VariableList.h
/// \brief		Declaration of CVariableList Object Class
/// \date		08-02-2005
/// \author		Kenneth Hurley
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

#ifndef __VARIABLELIST_H__
#define __VARIABLELIST_H__

typedef map<DWORD, DATABASEDATA *> VARIABLEMAP;

class CVariableList : public IVariableList
{
private:
    /// Default Constructor
	CVariableList();

public:
    /// Default Destructor
	virtual ~CVariableList();

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

	/// add an database with VarDatabase schema to variable access list
	/// \param dbInfo - the database information to add to the list
	virtual void AddDatabase(DATABASEINFO *dbInfo);

	/// scans an object for database types and adds them to variable list
	/// \param name - the name of the object to scan
	virtual void AddObjectDatabases(IHashString *name);

	/// Retrieve a variable from the map
	/// \param varName = variable name to retrieve
	/// \param outValue - pointer to fill in with DATABASEDATA pointer
	/// \return bool - true if found, otherwise false
	virtual bool GetValue(IHashString *varName, DATABASEDATA **outValue);

	/// Set the variable in a map
	/// \param varName = variable name to retrieve
	/// \param outValue - pointer to fill in with DATABASEDATA pointer
	/// \return bool - true if set, otherwise false if added
	virtual bool SetValue(IHashString *varName, DATABASEDATA *outValue);

    virtual void Destroy(){delete this;};


private:
	/// Pointed to the EE Tool Box
	IToolBox *m_ToolBox;

	/// Class Type
	CHashString m_hszNameType;

	VARIABLEMAP m_Variables;	/// pointer to my variable list

};

#endif //__VARIABLELIST_H__

