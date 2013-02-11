///============================================================================
/// \file		LuaObjDistQueryList.cpp
/// \brief		Class to wrap an stl list for a specific struct in lua
/// \date		12/27/2006
/// \author		Nick Tourte
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

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLuaObjDistQueryList::CLuaObjDistQueryList(void)
{
	// init iterator
	m_HashIter = begin();
}

CLuaObjDistQueryList::~CLuaObjDistQueryList(void)
{
}

void CLuaObjDistQueryList::Add(IHashString *name, float distance)
{
	// add name to list
	OBJECTDISTANCEQUERY odq;
	odq.distance = distance;
	odq.name = name;
	push_back(odq);
}

void CLuaObjDistQueryList::Remove(IHashString *name)
{
	ODQLIST::iterator hsIter;
	
	// find name in list
	for (hsIter = begin(); hsIter != end(); ++hsIter)
	{
		// if found
		if ((*hsIter).name->GetUniqueID() == name->GetUniqueID())
		{
			// delete it
			erase(hsIter);
			// reset iterator
			m_HashIter = begin();
			return;
		}
	}
}

OBJECTDISTANCEQUERY* CLuaObjDistQueryList::GetFirst()
{
	// set iterator to head of the list
	m_HashIter = begin();

	if (m_HashIter == end())
	{
		return NULL;
	}

	// return head and advance
	return &(*m_HashIter++);
}

OBJECTDISTANCEQUERY* CLuaObjDistQueryList::GetNext()
{
	// if iterator is not at the end
	if (m_HashIter != end())
	{
		// return name and advance
		return &(*m_HashIter++);
	}
	else
	{
		// otherwise return NULL
		return NULL;
	}
}

int CLuaObjDistQueryList::GetSize()
{
	/// return size of the list
	return size();
}

void CLuaObjDistQueryList::Clear()
{
	// clear list
	clear();
}
