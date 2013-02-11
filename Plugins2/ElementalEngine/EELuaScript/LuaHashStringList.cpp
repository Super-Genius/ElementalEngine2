///============================================================================
/// \file		LuaHashStringList.cpp
/// \brief		Implementation of the CLuaHashStringList
/// \date		09-29-2005
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

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLuaHashStringList::CLuaHashStringList(void)
{
	// init iterator
	m_HashIter = begin();
}

CLuaHashStringList::~CLuaHashStringList(void)
{
}

void CLuaHashStringList::Add(IHashString *name)
{
	// add name to list
	push_back(name);
}

void CLuaHashStringList::Remove(IHashString *name)
{
	HASHSTRINGLIST::iterator hsIter;
	
	// find name in list
	for (hsIter = begin(); hsIter != end(); ++hsIter)
	{
		// if found
		if ((*hsIter) == name)
		{
			// delete it
			erase(hsIter);
			// reset iterator
			m_HashIter = begin();
			return;
		}
	}
}

IHashString* CLuaHashStringList::GetFirst()
{
	// set iterator to head of the list
	m_HashIter = begin();

	if (m_HashIter == end())
	{
		return NULL;
	}

	// return head and advance
	return (*m_HashIter++);
}

IHashString* CLuaHashStringList::GetNext()
{
	// if iterator is not at the end
	if (m_HashIter != end())
	{
		// return name and advance
		return (*m_HashIter++);
	}
	else
	{
		// otherwise return NULL
		return NULL;
	}
}

int CLuaHashStringList::GetSize()
{
	/// return size of the list
	return size();
}

void CLuaHashStringList::Clear()
{
	// clear list
	clear();
}
