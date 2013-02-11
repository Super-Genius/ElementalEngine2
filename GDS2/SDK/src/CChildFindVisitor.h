///=====================================================================
/// \file	CChildFindVisitor.h
/// \brief	Declaration of CChildFindVisitor class
///			ObjectHelpers::GetChildName implementation uses this class
/// \date	10/03/2007
/// \author	Andrey Ivanov
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
///=====================================================================
#ifndef _GDS_CHILD_FIND_VISITOR_H_
#define _GDS_CHILD_FIND_VISITOR_H_

class CChildFindVisitor : public IVisitor
{
public:
	///	\brief	constructor
	///	\param	pChildComponentType - name of child component type
	///			if this value is NULL then all children will be collected
	CChildFindVisitor(IHashString *pChildComponentType);

	/// \brief	tells the callee that we want to visit depth first
	virtual bool IsHierarchicalVisitor();
	/// \brief	Visitor function callback
	virtual bool Visit(IComponent *pComponent, bool bVisitEnter);

	///	\brief	allow to get result children list
	///	\return	list with found components
	vector<IComponent *> GetChildren() const
	{
		return m_Children;
	}
private:
	IHashString			   *m_pChildComponentType;
	vector<IComponent *>	m_Children;
};


#endif //_GDS_CHILD_FIND_VISITOR_H_
