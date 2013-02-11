///============================================================================
/// \file		LuaDWORDSet.cpp
/// \brief		Implementation file for CLuaDWORDSet
/// \date		09-17-2005
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

#include "Stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CLuaDWORDSet::Add(IHashString *name)
{
	m_UniqueIDSet.insert(name->GetUniqueID());
}

void CLuaDWORDSet::Remove(IHashString *name)
{
	m_UniqueIDSet.erase(name->GetUniqueID());
}

void *CLuaDWORDSet::GetSet()
{
	m_pUniqueIDSet = &m_UniqueIDSet;
	return (void*)(&m_pUniqueIDSet);
}

bool CLuaDWORDSet::InSet(IHashString *name)
{
	UNIQUEIDSET::iterator setIter;

	// search for name
	setIter = m_UniqueIDSet.find(name->GetUniqueID());

	// if we found the name
	if (setIter != m_UniqueIDSet.end())
	{
		// it's in the set
		return true;
	}

	// not in the set
	return false;
}

void CLuaDWORDSet::Clear()
{
	// clear the set
	m_UniqueIDSet.clear();
}
