///============================================================================
/// \file		VisitNodeName.cpp
/// \brief		Implementation for Visit Node Name
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


#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CVisitNodeName::CVisitNodeName(CLuaHashStringList *hashList)
{
	m_NameList = hashList;
}

CVisitNodeName::~CVisitNodeName()
{

}

bool CVisitNodeName::Visit(IComponent *component, bool bVisitEnter)
{
	IObject *theObject;

	theObject = dynamic_cast<IObject *>(component);

	// if the object is invalid
	if (!theObject)
	{
		// log error
		return false;
	}

	// add name to list
	m_NameList->push_back(theObject->GetName());

	return true;
}