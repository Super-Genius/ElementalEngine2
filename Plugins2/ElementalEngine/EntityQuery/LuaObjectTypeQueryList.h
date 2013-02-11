///============================================================================
/// \file		LuaObjectTypeQueryList.h
/// \brief		Class to wrap an stl list for a specific struct in lua
/// \date		2/16/2007
/// \author		Richard Nguyen
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

#ifndef _LUAOBJECTTYPEQUERYLIST_H_
#define _LUAOBJECTTYPEQUERYLIST_H_

// tolua_begin

class CLuaObjectTypeQueryList : public OBJECTTYPEQUERYLIST
{
public:
	CLuaObjectTypeQueryList(void);
	~CLuaObjectTypeQueryList(void);

	/// Add a IHashString and distance to the list
	/// \param name - name of LuaObject to add
	/// \return void
	void Add(IHashString *name);

	/// Remove a IHashString from the list
	/// \param name - name of LuaObject to remove
	/// \return void
	void Remove(IHashString *name);

	/// Get first element in the list
	/// \param void
	/// \return first name in list
	IHashString *GetFirst();

	/// Get next element in the list
	/// \param void
	/// \return next name in list
	IHashString *GetNext();

	/// Get the size of the list
	/// \param void
	/// \return size of the list

	int GetSize();

	/// Clear the list
	/// \param void
	/// \return void
	void Clear();

// tolua_end

private:
	OBJECTTYPEQUERYLIST::iterator m_ObjectMapIterator;	
// tolua_begin
};
// tolua_end

#endif // #ifndef _LUAOBJECTTYPEQUERYLIST_H_
