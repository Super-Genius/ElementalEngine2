///============================================================================
/// \file		LuaVisitor.h
/// \brief		Header file for the CLuaVisitor Class
/// \date		10-09-2005
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

#ifndef _LUAVISITOR_H_
#define _LUAVISITOR_H_

// tolua_begin
class CLuaVisitor
{
// tolua_end
private:
	SINGLETONCONSTRUCTOROVERRIDE(CLuaVisitor);
public:
	// constructor
	CLuaVisitor();
	// destructor
	virtual ~CLuaVisitor(void);

// tolua_begin
	/// Get the visit function for the hash list visitor
	/// \param void
	/// \return void pointer that is the visit function name
	void *GetVisitFuncName();

	/// Get the visit function for the DWORD visitor
	/// \param void
	/// \return void pointer that is the visit function name
	void *GetVisitFuncDWORD();

	/// Get the Lua HashString list
	/// \param void
	/// \return pointer to a CLuaHashStringList
	CLuaHashStringList *GetHashStringList();

	/// Get the DWORD set
	/// \param void
	/// \return point to a LuaDWORDSet
	CLuaDWORDSet *GetDWORDSet();

	/// Clear the dword set
	/// \param void
	/// \return void
	void ClearDWORDSet();

	/// Clear the Lua HashString list
	/// \param void
	/// \return void
	void ClearHashStringList();
// tolua_end

private:
	/// List of lua hashstrings
	CLuaHashStringList m_LuaHashStringList;
	// Set of DWORDs
	CLuaDWORDSet m_DWORDSet;
	// DWORD Set visitor
	CVisitNodeDWORD m_VisitorNodeDWORD;
	// LuaHashstring List visitor
	CVisitNodeName m_VisitorNodeName;
// tolua_begin
};
// tolua_end

#endif // #ifndef _LUAVISITOR_H_