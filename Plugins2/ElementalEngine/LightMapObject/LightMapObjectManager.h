///============================================================================
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

#ifndef LightMapManager_H
#define LightMapManager_H

#include "CManager.h"


class LightMapManager : public CManager
{

public:

	/// \brief The Default Destructor for LightMapManager
	/// \return void
	~LightMapManager( );

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief	Update function for the Particle Manager
	/// \return	void
	virtual void Update( DWORD tickCount );
	
/// \brief	Initializes the Particle Manager
	/// \param	void
	/// \return	bool: Success
	virtual bool Init( void );

	/// \brief	DeInitialization
	/// \return	bool: Success
	virtual bool DeInit( void );

	DWORD OnBuildLightmap( DWORD size, void * params );
	DWORD OnRegisterLightmapCache( DWORD size, void * params );
	DWORD OnUnRegisterLightmapCache( DWORD size, void * params );
	DWORD OnGetLightmapCache( DWORD size, void * params );
	DWORD OnAddLightmappableObject( DWORD size, void * params );
	DWORD OnCreateMeshParameterization( DWORD size, void * params );
	DWORD OnSetGeneratorParams( DWORD size, void * params );
	DWORD OnGetGeneratorParams( DWORD size, void * params );
	DWORD OnSetLightmapOutputDirectory( DWORD size, void * params );
	DWORD OnResizeLightmapCache( DWORD size, void * params );
private:
	SINGLETONCONSTRUCTOROVERRIDE( LightMapManager );
	/// \brief The Default Constructor for LightMapManager
	/// \return void
	LightMapManager();

	// name of one type we manage
	CHashString m_LightTypeName;
	CHashString m_worldLMapTypeName;
	CHashString m_worldTypeName;

	map< DWORD, LightmapCache * > m_LightmapCaches;
};

#endif