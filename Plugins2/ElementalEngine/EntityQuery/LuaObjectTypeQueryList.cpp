///============================================================================
/// \file		LuaObjectTypeQueryList.cpp
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

CLuaObjectTypeQueryList::CLuaObjectTypeQueryList(void)
{
	m_ObjectMapIterator = begin();
}

CLuaObjectTypeQueryList::~CLuaObjectTypeQueryList(void)
{
}

void CLuaObjectTypeQueryList::Add(IHashString *name)
{
	push_back(name);
}

void CLuaObjectTypeQueryList::Remove(IHashString *name)
{
	OBJECTTYPEQUERYLIST::iterator iter = begin();
	for (; iter != end(); iter++)
	{
		if ((*iter)->GetUniqueID() == name->GetUniqueID())
		{
			erase(iter);
			return;
		}
	}
}

IHashString *CLuaObjectTypeQueryList::GetFirst()
{
	IHashString *retVal = NULL;

	m_ObjectMapIterator = begin();
	if (m_ObjectMapIterator != end())
	{
		retVal = (*m_ObjectMapIterator);
		m_ObjectMapIterator++;
	}

	return retVal;
}

IHashString *CLuaObjectTypeQueryList::GetNext()
{
	IHashString *retVal = NULL;

	if (m_ObjectMapIterator != end())
	{
		retVal = (*m_ObjectMapIterator);
		m_ObjectMapIterator++;
	}

	return retVal;
}

int CLuaObjectTypeQueryList::GetSize()
{
	return size();
}

void CLuaObjectTypeQueryList::Clear()
{
	clear();
}
