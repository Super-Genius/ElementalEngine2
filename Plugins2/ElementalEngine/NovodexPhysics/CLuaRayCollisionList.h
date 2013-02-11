///==========================================================================
/// \file	CLuaRayCollisionList.h
/// \brief	Wrapper for ray collision stl list
/// \date	10/08/07
/// \author	Nick Tourte
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
///==========================================================================

#ifndef _C_LUARAYCOLLISION_H_
#define _C_LUARAYCOLLISION_H_

// tolua_begin

///==========================================================================
/// \class	CLuaRayCollisionList
/// \brief	Lua Wrapper for raycast collisions.
///==========================================================================
class CLuaRayCollisionList : public list<RAYCOLLISION>
{
public:
	/// Default Constructor
	CLuaRayCollisionList();

	/// Destructor
	~CLuaRayCollisionList();

	/// Inserts a raycollision struct at the end of the list
	/// \param collisionParam = data to insert
	void InsertAtEnd(RAYCOLLISION collisionParam);

	/// Inserts a raycollision struct at the beginning of the list
	/// \param collisionParam = data to insert
	void InsertAtBegin(RAYCOLLISION collisionParam);

	/// Gets the size of the list
	/// \return size
	int GetSize();

	/// Gets the first raycollision in the list
	/// \return pointer to first raycollision
	RAYCOLLISION *GetFirst();

	/// Gets the next raycollision in the list
	/// \return pointer to next raycollision, or NULL if end of list
	RAYCOLLISION *GetNext();

	/// Clears the list
	void Clear();

// tolua_end
private:
	list<RAYCOLLISION>::iterator m_CurrentIterator;

// tolua_begin
};

// tolua_end

#endif //_C_LUARAYCOLLISION_H_