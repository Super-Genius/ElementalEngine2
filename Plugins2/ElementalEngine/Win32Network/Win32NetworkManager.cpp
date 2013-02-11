///============================================================================
/// \file		Win32NetworkManager.cpp
/// \brief		Implementation of Win32 Network Manager(using Raknet)
/// \date		06-28-2008
/// \author		Patrick Ghiocel
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



#include "StdAfx.h"

REGISTER_COMPONENT_SINGLETON(CWin32NetworkManager);


CWin32NetworkManager::CWin32NetworkManager() : 
	CManager(_T("CWin32NetworkManager"), NETWORKPRIORITY, NETWORKPRIORITY )
{

}

CWin32NetworkManager::~CWin32NetworkManager()
{
	DeInit();
}

void CWin32NetworkManager::Update( DWORD tickCount )
{
	static CHashString cwin32netpeerTypeName(_T("CWin32NetworkPeer"));

	IDTOOBJECTMAP *objectMap = GetObjectMap( &cwin32netpeerTypeName );
	if( objectMap != NULL )
	{
		IDTOOBJECTMAP::iterator objItr = objectMap->begin();
		while( objItr != objectMap->end() )
		{
			objItr->second->Update();
			objItr++;
		}
	}
}

IComponent *CWin32NetworkManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CWin32NetworkManager );
}

DWORD CWin32NetworkManager::PumpHandler(DWORD size, void *data, IHashString *name, IHashString *compType, MSGFUNCTION msgFunction)
{
	// only pump messages for certain types
	static CHashString cwin32netpeerTypeName(_T("CWin32NetworkPeer"));
	if (compType)
	{
		if (*compType == cwin32netpeerTypeName)
			return CManager::PumpHandler(size,data,name,compType,msgFunction);
	}
	return MSG_NOT_HANDLED;
}