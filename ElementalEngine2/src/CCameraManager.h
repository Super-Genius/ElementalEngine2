///============================================================================
/// \file	CCameraManager.h
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

#ifndef _CAMERAMANAGER_H__
#define _CAMERAMANAGER_H__

#include "CManager.h"
#include "ICamera.h"

typedef map< IRenderContext*, ICamera* > CONTEXTCAMERAMAP;

/// \brief the Render Manager class
class CCameraManager : public CManager
{
public:

	/// \brief The Default Destructor for CCameraManager
	/// \return void
	~CCameraManager( );

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief	Initializes the Render Manager
	/// \param	void
	/// \return	bool: Success
	virtual bool Init( void );

	/// \brief	Update function for the Render Manager
	/// \return	void
	virtual void Update( DWORD tickCount );
	
	/// \brief	DeInitialization
	/// \return	bool: Success
	virtual bool DeInit( void );
	
	// ----- Message Handlers, must be public ----- //
	
	/// Message handler for adding a list of objects to the current scene
	/// \brief Adds a render list to the render manager to queue up for rendering
	DWORD OnGetActiveCamera(DWORD size, void *params);
	DWORD OnSetActiveCamera(DWORD size, void *params);
	DWORD OnGetActiveCameraPosition(DWORD size, void *params);
	DWORD OnSetActiveCameraPosition(DWORD size, void *params);
	DWORD OnGetActiveCameraOrientation(DWORD size, void *params);
	DWORD OnSetActiveCameraOrientation(DWORD size, void *params);
	DWORD OnGetActiveCameraZMinMax(DWORD size, void *params);
	DWORD OnCreateCamera( DWORD size, void * params );
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

	DWORD OnDestroyRenderer(DWORD size, void *data);

	DWORD OnSetActiveCameraZoom(DWORD size, void *param);

private:
	SINGLETONCONSTRUCTOROVERRIDE( CCameraManager );
	/// \brief The Default Constructor for CCameraManager
	/// \return void
	CCameraManager();

	/// Initializes the default camera if none is set
	void InitializeDefaultCamera();

	inline ICamera* GetCamera( DWORD nameID )
	{
		IDTOOBJECTMAP *pObjectMap = GetObjectMap(&m_ccameraTypeName);
		IDTOOBJECTMAP::iterator itrCamera = pObjectMap->find(nameID);
		if (itrCamera != pObjectMap->end())
			return (ICamera*)itrCamera->second;
		else
			return NULL;
	}

private:
	CHashString m_DefaultCamera;
	CHashString m_ccameraTypeName;
	ICamera *   m_DefaultCameraObject;
	CONTEXTCAMERAMAP m_ConCamMap;

	bool m_bIsRunning;
};

#endif
