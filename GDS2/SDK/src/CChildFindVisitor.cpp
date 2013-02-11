///=====================================================================
/// \file	CChildFindVisitor.h
/// \brief	Implementation of CChildFindVisitor class
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
#include "stdafx.h"
#include "CChildFindVisitor.h"

///	\brief	constructor
///	\param	pChildComponentType - name of child component type
CChildFindVisitor::CChildFindVisitor(IHashString *pChildComponentType)
	: m_pChildComponentType(pChildComponentType)
{
}

/// \brief	tells the callee that we want to visit depth first
bool CChildFindVisitor::IsHierarchicalVisitor()
{
	return false;
}

/// \brief	Visitor function callback
bool CChildFindVisitor::Visit(IComponent *pComponent, bool bVisitEnter)
{
	if (bVisitEnter && 
		(m_pChildComponentType == NULL || pComponent->IsKindOf(m_pChildComponentType)))
	{
		m_Children.push_back(pComponent);
	}
	return true;
}
