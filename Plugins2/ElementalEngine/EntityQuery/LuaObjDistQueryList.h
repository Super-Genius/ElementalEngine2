///============================================================================
/// \file		LuaObjDistQueryList.h
/// \brief		Class to wrap an stl list for a specific struct in lua
/// \date		12/27/2006
/// \author		Nick Tourte
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

#ifndef _LUAOBJDISTQUERYLIST_H_
#define _LUAOBJDISTQUERYLIST_H_

// tolua_begin

class CLuaObjDistQueryList : public ODQLIST
{
public:
	CLuaObjDistQueryList(void);
	~CLuaObjDistQueryList(void);

	/// Add a IHashString and distance to the list
	/// \param name - name of LuaObject to add
	/// \return void
	void Add(IHashString *name, float distance);
	/// Remove a IHashString from the list
	/// \param name - name of LuaObject to remove
	/// \return void
	void Remove(IHashString *name);
	/// Get first element in the list
	/// \param void
	/// \return first query set in list
	OBJECTDISTANCEQUERY* GetFirst();
	/// Get next element in the list
	/// \param void
	/// \return next query set in list
	OBJECTDISTANCEQUERY* GetNext();
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
	/// List Iterator
	ODQLIST::iterator m_HashIter;
// tolua_begin
};
// tolua_end

#endif // #ifndef _LUAOBJDISTQUERYLIST_H_
