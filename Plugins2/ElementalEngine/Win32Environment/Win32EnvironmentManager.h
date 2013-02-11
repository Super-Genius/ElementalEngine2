///============================================================================
/// \file		Win32EnvironmentManager.h
/// \brief		Implementation of Win32 Environment
/// \date		06-27-2008
/// \author		Zsolt Matyas
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

#ifndef	__WIN32ENVIRONMENTMANAGER_H__
#define __WIN32ENVIRONMENTMANAGER_H__

#include "BaseMessages.h"


class CWin32EnvironmentManager : public CManager
{
	private:
	SINGLETONCONSTRUCTOROVERRIDE( CWin32EnvironmentManager );
	
	/// Default Constructor
	CWin32EnvironmentManager();
	
public:
	/// Default Destructor
	virtual ~CWin32EnvironmentManager();

	/// Function to get this component.
	///	Returns the singleton instance in this case
	static IComponent *Create(int nArgs, va_list argptr);

	void Update( DWORD tickCount );

	/// Pump Handler
	DWORD PumpHandler(DWORD size, void *data, IHashString *name, 
		IHashString *compType, MSGFUNCTION msgFunction);

	// ----------- Messages Functions ---------------//
	/// returns the default resouce path in this environment
	/// \param param = pointer to StdString: resource path
	/// \return MSG_XXX return value
	DWORD OnGetDefaultResourcePath(DWORD size, void *params);

	/// message handler to find a file
	/// \param size = size of a FINDFILE structure
	/// \param param = pointer to FINDFILE structure
	/// \return MSG_XXX return value
	DWORD OnWin32FindFile(DWORD size, void *param);

	DWORD Win32FindFirstFile( FINDFILE *cfe );

	DWORD Win32FindNextFile( FINDFILE *cfe );

	/// message handler to find a file
	/// \param size = size of a FINDFILE structure
	/// \param param = pointer to FINDFILE structure
	/// \return MSG_XXX return value
//	DWORD OnFindNextFile(DWORD size, void *param);

	/// Closes find handle after search
	/// \param size = message payload size
	/// \param param = the internal data to close a search
	/// \return MSG_XXX return value
	DWORD OnCloseFileSearch(DWORD size, void *param);
		
private:
	/// Pointer to the EE Tool Box --> perhaps later we will use it
	//IToolBox *m_ToolBox;

	// the default resource path in this environment
	StdString m_strDefaultResourcePath;

};

#endif //__WIN32ENVIRONMENTMANAGER_H__

