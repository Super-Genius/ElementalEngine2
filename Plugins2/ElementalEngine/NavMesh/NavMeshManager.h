///============================================================================
/// \file		CNavMeshManager.h
/// \brief		Header file for the NavMesh
/// \date		03-24-2005
/// \author		Brian Bazyk
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

#ifndef	CNAVMESHMANAGER_H
#define	CNAVMESHMANAGER_H

#include "CManager.h"

class CNavMeshManager : public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CNavMeshManager);
	/// \brief The Default Constructor
	/// \return void
	CNavMeshManager();
public:
	/// \brief The Default Deconstructor
	/// \return void
	virtual ~CNavMeshManager();

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent *Create(int nArgs, va_list argptr);

	// ------------------ Messages ------------------ //

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

	DWORD OnMouseMove(DWORD size, void *data);

private:
	bool m_bIsRunning;
};

#endif
