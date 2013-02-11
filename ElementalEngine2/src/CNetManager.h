///==========================================================================
/// \file	CNetManager.h
/// \brief	Network Manager
/// \date	4/1/2005
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
///==========================================================================

#ifndef CNETMANAGER_H
#define	CNETMANAGER_H

#include "CManager.h"

///==========================================================================
/// \class	CNetManager
/// \brief	Network Manager
///==========================================================================
class CNetManager : public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CNetManager );

	/// \brief	Default Destructor
	///			Since CPhysManager is implemented as a singleton the
	///			constructor is private.
	CNetManager();

public:


	/// \brief	Default Destructor
	~CNetManager( );

	/// \brief	Function to get this component.
	///	\return	The singleton instance in this case
	static IComponent *Create(int nArgs, va_list argptr);

	///	\brief	Start the manager
	///	\param	size = 0
	///	\param	data = void pointer to NULL
	///	\return DWROD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED,
	///						error
	DWORD OnStart(DWORD size, void *data);

	///	\brief	Stop the manager
	///	\param	size = 0
	///	\param	data = void pointer to NULL
	///	\return DWROD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED,
	///						error
	DWORD OnStop(DWORD size, void *data);

private:
	bool m_bIsRunning;
};

#endif	// CNETMANAGER_H