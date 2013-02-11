///============================================================================
/// \file		EntityTypeVisitor.cpp
/// \brief		Implementation of EntityTypeQuery System
/// \date		2-16-2007
/// \author		Richard Nguyen
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


bool CEntityTypeVisitor::IsHierarchicalVisitor()
{
	return false;
}

bool CEntityTypeVisitor::Visit( IComponent* component, bool bVisitEnter )
{
	IHashString* componentName = dynamic_cast<IObject*>(component)->GetName();
	m_ObjectsList->push_back(componentName);
	return true;
}

void CEntityTypeVisitor::SetObjectsList( OBJECTTYPEQUERYLIST* objectsList)
{
	m_ObjectsList = objectsList;
}