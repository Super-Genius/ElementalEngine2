///============================================================================
/// \file	CAIManager.h
/// \brief	Definition of CAIManager Class
///         This files defines a CAIManager which is the interface
///			between AI Module and Red Phoenix.
///			Inherits from IManager
/// \date	02/15/2005
/// \author	Patrick Ghiocel
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

#ifndef _RENDERMANAGER_H__
#define _RENDERMANAGER_H__

//#include "CManager.h"
#include "IGeometry.h"
#include "IRenderObject.h"
#include "IRenderer.h"
#include <list>
#include <map>
#include <set>
#include <stack>

#define DEFAULT_LUMINANCE_SHIFT_TIME	3.f

struct RenderPair
{
	RenderPair()
	{
		geometry = NULL;
		renderObject = NULL;
		bAlpha = false;
		bGlow = false;
		fDistanceToCamera = 0.0f;
		iRenderPriority = 0;
	}
	void Render( UINT pass )
	{
		if( geometry )
		{
			static IController * control = NULL;
			static Matrix4x4 * matworld = NULL;
			static IRenderer * m_IRenderer = NULL;
			if( m_IRenderer == NULL )
			{
				// grab the render interface
				GETRENDERERINTERFACEPARAMS renderInterface;
				static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
				EngineGetToolBox()->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface);
				m_IRenderer = renderInterface.m_RendererInterface;
			}

			control = geometry->GetController();
			if( control )
			{
				matworld =  control->GetMatrix();
				m_IRenderer->SetMatrix( WORLD_MATRIX, (CONST FLOAT*)matworld->GetMatrix() );
			}
			geometry->Render();
		}
		else if( renderObject )
		{
			renderObject->Render( pass );
		}
	}
	inline void SetGeom( IGeometry * geom )
	{
		geometry = geom;
		renderObject = NULL;

		bAlpha = geometry->HasAlpha();
		bGlow = false;
		IMaterial *pMaterial = geometry->GetMaterial();
		if (pMaterial)
		{
			if (pMaterial->GetShaderFlags() & SHADER_RF_GLOW)
				bGlow = true;
		}
	}
	inline void SetRenderObject( IRenderObject * ro )
	{
		geometry = NULL;
		renderObject = ro;

		bAlpha = renderObject->IsAlphaObject();
		bGlow = false;
		IMaterial *pMaterial = ro->GetMaterial(0);
		if (pMaterial)
		{
			if (pMaterial->GetShaderFlags() & SHADER_RF_GLOW)
				bGlow = true;
		}
	}

	bool bAlpha;
	bool bGlow;
	float fDistanceToCamera;
	DWORD iRenderPriority;

private:
	//Mutually exclusive, combines Geom and ROs into one list
	IGeometry * geometry;
	IRenderObject * renderObject;
};

struct RENDERPAIRCOMPARE
{
	inline bool operator()(const RenderPair &Left, const RenderPair &Right) const
	{
		if (Left.bAlpha == Right.bAlpha)
		{
			// both objects are alpha, sort back to front
			if (Left.bAlpha)
				return Left.fDistanceToCamera > Right.fDistanceToCamera;

			// glow before non-glow
			if (Left.bGlow != Right.bGlow)
				return Right.bGlow;

			// higher render priority before lower
			if (Left.iRenderPriority != Right.iRenderPriority)
				return (Left.iRenderPriority > Right.iRenderPriority);

			// default sort front to back
			return (Left.fDistanceToCamera < Right.fDistanceToCamera);
		}
		else
		{
			// non-alpha before alpha
			return Right.bAlpha;
		}
	}
};

struct ALPHADISTANCECOMPARE
{
	__forceinline bool operator()( pair< float , RenderPair > LeftObj, pair< float , RenderPair> RightObj ) const
	{		
		return LeftObj.first > RightObj.first;
	}
};

struct GEOMDISTANCECOMPARE
{
	__forceinline bool operator()( pair< float , IGeometry *> LeftObj, pair< float , IGeometry *> RightObj ) const
	{
		return LeftObj.first < RightObj.first;
	}
};

struct GEOMTEXTURECOMPARE
{
	__forceinline bool operator()( pair< IBaseTextureObject* , IGeometry *> LeftObj, pair< IBaseTextureObject* , IGeometry *> RightObj ) const
	{		
		return LeftObj.first < RightObj.first;
	}
};

typedef vector< RenderPair > RENDEROBJECTLIST;

typedef struct SINGLEPASS
{
	RENDEROBJECTLIST m_List;
	void (*m_Callback)();
	void (*m_PostCallback)();
	SINGLEPASS()
	{
		m_Callback = NULL;
		m_PostCallback = NULL;
	}
} SINGLEPASS;

typedef vector< SINGLEPASS > PASSLIST;	//full set of passes
typedef multimap< DWORD , PASSLIST > RENDERLIST;	//Set of full set of passes
typedef pair< DWORD, PASSLIST > RENDERLIST_ELEMENT;
typedef map< UINT, IRenderContext * > RENDERCONTEXTLIST;
typedef pair< UINT, IRenderContext * > RENDERCONTEXTLIST_ELEMENT;
typedef list< IEffect * > POSTPROCESSSHADERS;
typedef map< IRenderContext*, POSTPROCESSSHADERS > POSTPROCESSCONTEXTS;
typedef map< IRenderContext*, RENDERLIST > RENDERLISTCONTEXTS;

typedef vector< IGeometry * > RENDERGEOMETRYLIST;//List of passes for a certain priority
typedef map< DWORD , RENDERGEOMETRYLIST > GEOM_RENDERLIST;	//Set of full set of passes based on priorities(VB + priority*100 )
typedef map< IRenderContext*, GEOM_RENDERLIST >  GEOMETRYLISTCONTEXTS;

//typedef map< VisBaseEntity_cl *, IRenderObject * > ADAPTERMAP;

/// \brief the Render Manager class
class CRenderManager : public CManager
{
public:

	/// \brief The Default Destructor for CRenderManager
	/// \return void
	~CRenderManager( );

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief	Update function for the Render Manager
	/// \return	void
	virtual void Update( DWORD tickCount );
	
	/// \brief	DeInitialization
	/// \return	bool: Success
	virtual bool DeInit( void );
	
	// ----- Message Handlers, must be public ----- //
	
	/// Message handler for adding a list of objects to the current scene
	/// Adds a render list to the render manager to queue up for rendering
	DWORD OnAddRenderList(DWORD size, void *params);
	/// Adds a render list to the render manager to instantly render to a texture
	DWORD OnRenderListToTexture(DWORD size, void *params);	
	/// creates a new render context and returns the ID
	DWORD OnCreateRenderContext(DWORD size, void *params);
	/// Deletes the render context whose ID is passed in
	DWORD OnDeleteRenderContext(DWORD size, void *params);
	/// Adds a rendercontext to the list to copy the next frame to
	DWORD OnDrawToRenderContext(DWORD size, void *params);	
	/// Destroys the renderer, a message must be sent before destroying
	/// the rendermanager. This message cannot be in a destructor in case concrete static instances
	/// deletes the rendermanager on a destructor call, when all of the renderer's global statics
	/// are already deallocated.
	DWORD OnDestroyRenderer(DWORD size, void *params);
	/// gets the window dimensions of the scene
	DWORD OnGetWindowSize(DWORD size, void *params);
	/// sets the window dimensions of the scene
	DWORD OnSetWindowSize(DWORD size, void *params);
	/// sets the viewport dimensions of the scene
	DWORD OnSetViewPort(DWORD size, void *params);
	/// Sets a fullscreen mode for the renderer
	DWORD OnSetFullScreen(DWORD size, void *params);
	/// sets the video screen size of the scene
	DWORD OnSetVideoSize(DWORD size, void *params);
	/// Add an object to the adapter map
	//DWORD OnAddRenderObjectToMap(DWORD size, void *params);

	/// Get the IRenderObject pointer from a VisBaseEntity_cl pointer
	//DWORD OnGetRenderObject(DWORD size, void *params);
	
	/// RenderManager is the factory for IMeshObject Interfaces
	DWORD OnCreateMeshObject(DWORD size, void *params);
	/// Gets the video size
	DWORD OnGetVideoSize( DWORD size, void *params );
	
	DWORD OnSetRenderMouse(DWORD size, void *params);
	/// \brief sets the video screen size of the scene
	DWORD OnGetRenderMouse(DWORD size, void *params);

	/// Gets a profiling snapshot of the current render frame
	DWORD OnGetRendererProfilingSnapshot(DWORD size, void *params);
	//// Gets current object culling distance
	DWORD OnGetObjectCullDistance(DWORD size, void *params);
	/// Sets current object culling distance
	DWORD OnSetObjectCullDistance(DWORD size, void *params);
	/// Adds a post process effect
	DWORD OnAddPostProcessEffect(DWORD size, void *params);
	DWORD OnRemovePostProcessEffect(DWORD size, void *params);

	///	Start the manager
	///	\param	size = 0
	///	\param	data = void pointer to NULL
	///	\return DWROD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED,
	///						error
	DWORD OnStart(DWORD size, void *data);

	///	Stop the manager
	///	\param	size = 0
	///	\param	data = void pointer to NULL
	///	\return DWROD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED,
	///						error
	DWORD OnStop(DWORD size, void *data);

	///	Sets the Renderer's Wireframe (bool)
	///	\param	size = size of message (bool)
	///	\param	data = void pointer to a (bool)
	///	\return DWROD = MSG_HANDLED_STOP
	DWORD OnSetWireFrameMode(DWORD size, void *params);

	DWORD OnCreateController(DWORD size, void *params);
	DWORD OnDestroyController(DWORD size, void *params);
	DWORD OnCreateGeometry(DWORD size, void * params);
	DWORD OnDestroyGeometry(DWORD size, void * params);

	/// Ticks just the renderer stuffs, such that screens can be redrawn without having to worry about 
	/// ticking all the various systems 
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_PROCEED
	DWORD OnRenderUpdate(DWORD size, void *param);

	DWORD OnCreateSceneSettings( DWORD size, void * params);
	DWORD OnDestroySceneSettings( DWORD size, void * params);
	DWORD OnGetRenderFlagValue(DWORD size, void *params);
	DWORD OnSetNullRenderer(DWORD size, void *params);

	DWORD OnCreateMaterialOverride(DWORD size, void *param);	
	/// The last part of the renderloop that adds the visible entities to the list before it is hijacked
	DWORD OnAddVisibleEntitiesToScene( DWORD size, void * param );
	DWORD OnGetDepthTargetAsTexture( DWORD size, void * param );
	DWORD OnSetFogValue( DWORD size, void * param );
	DWORD OnGetFogValue( DWORD size, void * param );
	DWORD OnSetColorCorrection( DWORD size, void * param );

	//Accessors for functions that don't require messages:
	void GetFogStartDistance( float &start, float &distance );
	void GetFogColor( Vec3 &color );
private:
	/// \brief	Adds the list of objects to the RENDERLIST
	/// \param VisibleObjList = a list of objects to insert by key into the render list
	/// \return	bool: Success
	bool AddListObjects( OBJECTLIST &VisibleObjList, DWORD priorityKey,  void (*funcCallback)(), void (*funcPostRenderCallback)(), IRenderContext * context );

	SINGLETONCONSTRUCTOROVERRIDE( CRenderManager );
	/// \brief The Default Constructor for CRenderManager
	/// \return void
	CRenderManager();


private:	
	void RenderPassList( PASSLIST &passList, bool bSort );

	/// \brief The current list of passes of geometries
	/// \var IRenderer * m_IRenderer;
	/// \brief the Interface to the renderer adapter
	IRenderer * m_IRenderer;
	/// \var DWORD m_SortMethod;
	/// \brief The method to sort renderobjects by when adding to a list
	DWORD							m_SortMethod;
	RENDERLISTCONTEXTS						m_CurrentSceneContextList;	
	/// \var RENDERCONTEXTLIST m_RenderContextList;
	/// \brief the current list of created/available render contexts
	RENDERCONTEXTLIST				m_RenderContextList;
	/// \var UINT m_NextContextID;
	/// \brief The next ID to assigned to a new render context
	UINT							m_NextContextID;	
	/// \var IHashString* m_CurrentCamera;
	/// \brief The current camera for the scene
	IHashString*					m_CurrentCamera;
	/// \var std::stack< CRenderContext > m_CurrentRenderContexts;
	/// \brief The IDs of current render contexts for ONE frame of rendering
	/// this stack is emptied at the end of each frame
	std::stack< IRenderContext * >	m_CurrentRenderContexts;
	/// \var bool m_bInit;
	/// \brief Is the renderer initialized or not
	bool							m_bInit;
	/// \brief	Map of VisBaseEntity_cl to IRenderObject
//	ADAPTERMAP						m_AdapterMap;

	Vec3							m_MouseCoord;

	POSTPROCESSCONTEXTS				m_PostProcessContextShaders;

	/// Distance used to cull objects in view
	float m_fObjectCullDistance;

	bool m_bIsRunning;
	bool m_bFrameWasRendered;
	int	 m_CurSwapBuffer;
	ITextureObject * m_DepthTexture;
	/// Does the post process shaders
	void DoPostProcess( IRenderContext *pContext );
	void CreateDepthTargetTexture();

	// storage for HashString for Camera messages type name
	CHashString m_ccameraTypeName;
	bool m_bNullRenderer;
	bool m_bUseDepthTextureMRT;

	void BindDepthTextureMRT();
	void UnbindDepthTextureMRT();

	float	m_FogStart;
	float	m_FogDistance;
	Vec3	m_FogColor;
	float	m_AverageLuminosity[2];
	int		m_CurLuminanceSlot;
	float	m_LuminanceTime;
	float	m_LuminanceShiftTime;

	typedef map< UINT, PASSLIST > PRIORITYTOPASSLISTMAP;
	typedef map< IRenderContext*, PRIORITYTOPASSLISTMAP > CONTEXTTOPRIORITYPASSMAP;
	CONTEXTTOPRIORITYPASSMAP m_RenderObjectLists;
};

#endif
