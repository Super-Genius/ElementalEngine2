///============================================================================
/// \file		TimeOfDayVisitor.cpp
/// \brief		Implementation file for CTimeOfDayVisitor
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


#include "stdafx.h"

CTimeOfDayVisitor::CTimeOfDayVisitor()
{
	m_ObjList.clear();
	m_objTypeFilter = 0;
}

CTimeOfDayVisitor::~CTimeOfDayVisitor() 
{
}

bool CTimeOfDayVisitor::Visit(IComponent * component, bool bVisitEnter)
{	
	IObject *theObject;
	IHashString *compType;

	theObject = dynamic_cast<IObject *>(component);
	
	// if the object is invalid
	if (!theObject)
	{
		// log error
		return false;
	}

	// get the component type of the object
	compType = theObject->GetComponentType();

	// if the component type is correct
	if (compType->GetUniqueID() == m_objTypeFilter)
	{
		// add it to the list
		m_ObjList.push_back(theObject);
	}

	return true;
}

bool CTimeOfDayVisitor::IsHierarchicalVisitor()
{
	return false;
}

OBJECTLIST* CTimeOfDayVisitor::GetObjList()
{
	return &m_ObjList;
}

void CTimeOfDayVisitor::ClearObjList()
{
	return m_ObjList.clear();
}

void CTimeOfDayVisitor::SetTypeFilter(IHashString *type)
{
	m_objTypeFilter = type->GetUniqueID();
}