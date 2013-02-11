///============================================================================
/// \file		LuaHashStringList.h
/// \brief		Header file for CLuaHashStringList
/// \date		09-29-2005
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

#ifndef _LUAHASHSTRINGLIST_H_
#define _LUAHASHSTRINGLIST_H_

/// set of IHashString pointers
typedef list<IHashString*> HASHSTRINGLIST;

// tolua_begin
class CLuaHashStringList : public HASHSTRINGLIST
{
public:
	CLuaHashStringList(void);
	~CLuaHashStringList(void);

	/// Add a IHashString to the list
	/// \param name - name of object to add
	/// \return void
	void Add(IHashString *name);
	/// Remove a IHashString from the list
	/// \param name - name of object to remove
	/// \return void
	void Remove(IHashString *name);
	/// Get first element in the list
	/// \param void
	/// \return IHashString* - first element in the list
	IHashString* GetFirst();
	/// Get next element in the list
	/// \param void
	/// \return IHashString* - next element in the list
	IHashString* GetNext();
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
	HASHSTRINGLIST::iterator m_HashIter;
// tolua_begin
};
// tolua_end

#endif // #ifndef _LUAHASHSTRINGLIST_H_
