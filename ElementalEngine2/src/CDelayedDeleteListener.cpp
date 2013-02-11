///=====================================================================
/// \file	CDelayedDeleteListener.cpp
///         This class implements a CDelayedDeleteListener which is a 
///			listener registered with object factory
/// \date	09/14/2005
/// \author	Kyle Swaim
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

#include "StdAfx.h"
#include "CObjectFactory.h"
#include "CDelayedDeleteListener.h"

// constructor for the listener class
CDelayedDeleteListener::CDelayedDeleteListener(CObjectFactory *parent)
{
	m_Parent = parent;
}

// function called when the listener is initialized from
// the system listener
bool CDelayedDeleteListener::Init(void)
{
	return true;
}

// Update function called from the system listener
void CDelayedDeleteListener::Update(DWORD tickCount)
{
	m_Parent->DeleteDelayedObjects();
}

// call from system listener to shutdown
bool CDelayedDeleteListener::DeInit(void)
{
	return true;
}

// returns name of manager type
IHashString* CDelayedDeleteListener::GetName()
{
	if (m_Parent)
	{
		return m_Parent->GetComponentType();
	}
	else
	{
		return NULL;	
	}
}