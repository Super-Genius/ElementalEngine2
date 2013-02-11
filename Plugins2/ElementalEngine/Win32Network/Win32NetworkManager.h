///============================================================================
/// \file		Win32NetworkManager.h
/// \brief		Declaration of Win32 Network Manager (using Raknet)
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

#ifndef	__WIN32NETWORMANAGER_H__
#define __WIN32NETWORMANAGER_H__

#include "StdAfx.h"

class CWin32NetworkManager : public CManager
{
	private:
	SINGLETONCONSTRUCTOROVERRIDE( CWin32NetworkManager );
	
	/// Default Constructor
	CWin32NetworkManager();
	
public:
	/// Default Destructor
	virtual ~CWin32NetworkManager();

	/// Function to get this component.
	///	Returns the singleton instance in this case
	static IComponent *Create(int nArgs, va_list argptr);

	void Update( DWORD tickCount );

	/// Pump Handler
	DWORD PumpHandler(DWORD size, void *data, IHashString *name, 
		IHashString *compType, MSGFUNCTION msgFunction);

	// ----------- Messages Functions ---------------//
		
private:
	/// Pointer to the EE Tool Box
	IToolBox *m_ToolBox;

};

#endif //__WIN32NETWORMANAGER_H__

