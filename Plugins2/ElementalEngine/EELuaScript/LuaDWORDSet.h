///============================================================================
/// \file		LuaDwordSet.h
/// \brief		Header file for CLuaDWORDSet
/// \date		09-15-2005
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

#ifndef _LUADWORDSET_H
#define _LUADWORDSET_H

/// set of dwords
typedef set<DWORD> UNIQUEIDSET;

// tolua_begin
class CLuaDWORDSet
{
public:
	/// Add a unique id to the set
	/// \param name - name of object to add
	/// \return void
	void Add(IHashString *name);
	/// Remove a unique id from the set
	/// \param name - name of object to remove
	/// \return void
	void Remove(IHashString *name);
	/// Get a pointer to the unique id set
	/// \param void
	/// \return void* - pointer to unique id set
	void *GetSet();
	/// Is Name in the set?
	/// \param name - name of object to check
	/// \return void
	bool InSet(IHashString *name);
	/// Clear the set
	/// \param void
	/// \return void
	void Clear();
// tolua_end
private:
	/// set of unique ids
	UNIQUEIDSET m_UniqueIDSet;
	UNIQUEIDSET *m_pUniqueIDSet;
// tolua_begin
};
// tolua_end

#endif