///============================================================================
/// \file	CSceneVisibilityManager.h
/// \brief	Definition of CSceneVisibilityManager Class
///         This files defines a CSceneVisibilityManager which manages 
///			scene visibility. Inherits from IManager.
/// \date	03/30/2005
/// \author	Marvin Gouw
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
#ifndef _CSCENEVISIBILITYMANAGER_
#define _CSCENEVISIBILITYMANAGER_

class CSceneCullVisitor : public IVisitor
{
public:
    
	virtual bool Visit( IComponent * component, bool bVisitEnter )
	{
		IRenderObject* renderObject = (IRenderObject*)component;

		// Remove light from all objects in node.
		renderObject->DetachAllLights();

		if( renderObject->IsAlphaObject() )
		{
			m_AlphaList.push_back( renderObject );
		}
		else
		{
			m_OpaqueList.push_back( renderObject );
		}

		return true;
	}

	virtual bool IsHierarchicalVisitor() { return false; }

	virtual OBJECTLIST * GetAlphaObjects() { return &m_AlphaList; }
	virtual OBJECTLIST * GetOpaqueObjects(){ return &m_OpaqueList; }


private:
	OBJECTLIST		m_AlphaList;
	OBJECTLIST		m_OpaqueList;
};

/// This manager is for sorting and culling 3D objects in the world based on the information
/// obtained from the QuadTreeManager
class CSceneVisibilityManager : public CManager
{
public:

	/// \brief The Default Destructor for CSceneVisibilityManager
	/// \return void
	~CSceneVisibilityManager( );

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief	Update function for the Scene Visibility Manager
	/// \return	void
	virtual void Update( DWORD tickCount );
	
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

	/// Called when the active scene is changed
	DWORD OnSetActiveScene(DWORD size, void *params);

	/// Sets the render context of a given scene
	DWORD OnSetSceneRenderContext(DWORD size, void *params);

private:	
	SINGLETONCONSTRUCTOROVERRIDE( CSceneVisibilityManager );
	/// \brief The Default Constructor for CRenderManager
	/// \return void
	CSceneVisibilityManager();

private:
	static CSceneVisibilityManager * m_SingletonInstance;
	IHashString *m_CurrentCamera;
	// camera type name to send messages directly to component
	CHashString	m_ccamTypeName;
	bool m_bIsRunning;

	typedef map<DWORD,IRenderContext*> SCENETORENDERCONTEXTMAP;
	SCENETORENDERCONTEXTMAP m_SceneRenderContexts;
	IRenderContext *m_pActiveSceneRenderContext;
};

#endif
