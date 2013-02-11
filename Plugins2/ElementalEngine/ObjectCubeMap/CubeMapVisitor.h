///============================================================================
/// \file		CubeMapVisitor.h
/// \brief		Header file for Cube Map Visitor
/// \date		10-10-2005
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

#ifndef _CUBEMAPVISITOR_H_
#define _CUBEMAPVISITOR_H_

class CLODVisitor : public IVisitor
{
public:
	/// construction
	CLODVisitor();

	/// destruction
	~CLODVisitor();

	/// Visit a node
	/// \param component - componet to visit
	/// \param bVisitEnter - ???
	/// \return true on success false on failure
	bool Visit(IComponent * component, bool bVisitEnter);
	
	/// Is this visitor hierarchical?
	/// \param void
	/// \return true if it is false if not
	bool IsHierarchicalVisitor();

	/// Set filter for visit
	/// \param filter - filter to set
	/// \return void
	void SetFilter(const DWORDSET &filter);

	/// Get the list of relevant objects
	/// \param void
	/// \return OBJECTLIST* - pointer to an object list
	OBJECTLIST* GetEntityList();

	/// Clear the entity list
	/// \param void
	/// \return void
	void ClearEntityList();

	/// \brief	Compares two names and determines if one is a part of another
	///	\param	searchString - StdString searching for
	/// \param	compString - StdString to compare agains
	/// \return true if seachString is same as compString else false
	bool CompareNames(StdString searchString, StdString compString);

private:
	/// Tool box
	IToolBox *m_ToolBox;
	/// filter set
	DWORDSET m_FilterSet;
	/// list of relevant objects
	OBJECTLIST m_EntityList;
};

#endif	// #ifndef _CUBEMAPVISITOR_H_