///============================================================================
/// \file		TimeOfDayVisitor.h
/// \brief		Header file for Time of Day Visitor
/// \date		09-25-2005
/// \author		Josh Lulewicz
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

#ifndef CTIMEOFDAYVISITOR_H
#define CTIMEOFDAYVISITOR_H

class CTimeOfDayVisitor : public IVisitor
{
public:
	/// construction
	CTimeOfDayVisitor();

	/// destruction
	~CTimeOfDayVisitor();

	/// Visit a node
	/// \param component - componet to visit
	/// \param bVisitEnter - ???
	/// \return true on success false on failure
	bool Visit(IComponent * component, bool bVisitEnter);
	
	/// Is this visitor hierarchical?
	/// \param void
	/// \return true if it is false if not
	bool IsHierarchicalVisitor();

	/// Get the list of relevant objects
	/// \param void
	/// \return OBJECTLIST* - pointer to an object list
	OBJECTLIST* GetObjList();

	/// Clear the entity list
	/// \param void
	/// \return void
	void ClearObjList();

	/// Set type
	/// \param type - type of object to visit
	/// \return void
	void SetTypeFilter(IHashString *type);
private:
	/// Type of object to visit
	DWORD m_objTypeFilter;
	/// list of relevant objects
	OBJECTLIST m_ObjList;
};

#endif	// #ifndef CTIMEOFDAYVISITOR_H