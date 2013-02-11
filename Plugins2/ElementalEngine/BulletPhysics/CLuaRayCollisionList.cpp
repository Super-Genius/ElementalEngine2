///==========================================================================
/// \file	CLuaRayCollisionList.cpp
/// \brief	Wrapper for ray collision stl list
/// \date	10/08/07
/// \author	Nick Tourte
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
///==========================================================================

#include "stdafx.h"

CLuaRayCollisionList::CLuaRayCollisionList()
{
	m_CurrentIterator = end();	
}

CLuaRayCollisionList::~CLuaRayCollisionList()
{
	clear();
}

void CLuaRayCollisionList::InsertAtEnd(RAYCOLLISION collisionParam)
{
	insert(end(), collisionParam);
}

void CLuaRayCollisionList::InsertAtBegin(RAYCOLLISION collisionParam)
{
	insert(begin(), collisionParam);
}

int CLuaRayCollisionList::GetSize()
{
	return (int)size();
}

RAYCOLLISION *CLuaRayCollisionList::GetFirst()
{
	m_CurrentIterator = begin();
	if (m_CurrentIterator != end())
	{
		return &(*m_CurrentIterator);
	}
	else
	{
		return NULL;
	}
}

RAYCOLLISION *CLuaRayCollisionList::GetNext()
{
	m_CurrentIterator++;
	if (m_CurrentIterator != end())
	{
		return &(*m_CurrentIterator);
	}
	else
	{
		return NULL;
	}
}

void CLuaRayCollisionList::Clear()
{
	clear();
}