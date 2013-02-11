///============================================================================
/// \file	CLightManager.h
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

#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H
#include "CLightObject.h"

#define SHADOW_FPS_INTERVAL						1 //every 3 frames
#define MAX_SHADOW_ATTEN						2000
#define DEFAULT_SHADOW_POOL_COUNT				6
#define DEFAULT_SHADOW_GENERATION_PER_FRAME		2
#define INVALID_SHADOW_MAP_INDEX				-1
using namespace std;

struct Light_Info
{
	float	lastUpdate;
	size_t	ShadowMapIndex; //if one is assigned
	Vec3	lastLightPosition;
	IBaseTextureObject * LightPerspectiveMap;
	int numLastRendered;
	Light_Info()
	{
		numLastRendered = 0;
		lastUpdate = -1;
		ShadowMapIndex = INVALID_SHADOW_MAP_INDEX;
		LightPerspectiveMap = NULL;
	}
};

struct ShadowTexturePoolEntry
{
	IBaseTextureObject * texture;
	float					m_LastFrameSet;
};

typedef map< ILightObject *, Light_Info > LIGHTINFOMAPPING;
typedef list< IGeometry* > GEOMETRYLIST;

class CLightInFrustumVisitor : public IVisitor
{
public:
	CLightInFrustumVisitor()
	{
		m_NumShadowCasters = 0;
		m_TotalShadowCasters = 2; // pass this in from somewhere
	}

	virtual bool Visit( IComponent* component, bool bVisitEnter )
	{
		// build a sorted list of ILightObjects with lights closest to the frustum first.
		ILightObject* curLObject = (ILightObject*)component;

		// Get the distance from the object to the camera
		Vec3 lightPos;
		curLObject->GetPosition( lightPos );
		Vec3 difference = lightPos - m_CameraPos;
		float distance = difference.Length();

		// Insert object into the map
		m_SortedLightMap.insert( pair< float, ILightObject*>(distance, curLObject) );
		
		return true;
	}

	virtual bool IsHierarchicalVisitor() { return false; }

	/// Iterates through the list of lights in the frustum and
	void UpdateLightsAndShadows()
	{
		// Iterate through sorted list
		multimap<float, ILightObject*>::iterator iter = m_SortedLightMap.begin();
		for( ; iter != m_SortedLightMap.end(); iter++)
		{
			CLightObject* curLObject = (CLightObject*)iter->second;

			if( m_NumShadowCasters < m_TotalShadowCasters && curLObject->CanCastShadows() )
			{
				// Update light and shadows
				int iShadowCastObjectCount = 0; // number of objects casting shadows with this light
				curLObject->UpdateLightInScene( true, &iShadowCastObjectCount );
				// only count this as a shadow casting light if there are objects casting shadows with it
				// (ie. it generated a shadow map)
				if (iShadowCastObjectCount > 0)
					m_NumShadowCasters++;
			}
			else
			{
				// Only update light
				curLObject->UpdateLightInScene( false, NULL );
			}
		}
	}

	void SetCameraPos( Vec3& cameraPos ) { m_CameraPos = cameraPos; }

private:
	multimap<float, ILightObject*> m_SortedLightMap;
	int m_NumShadowCasters;
	int m_TotalShadowCasters;
	Vec3 m_CameraPos;
};

class CSceneLightVisitor : public IVisitor
{
public:
	CSceneLightVisitor()
	{
		m_Light = NULL;
	}

	virtual bool Visit( IComponent* component, bool bVisitEnter )
	{
		IRenderObject* renderObject = (IRenderObject*)component;

		// Attach this light
		renderObject->AttachLight( m_Light );

		if (m_Light->CanCastShadows())
		{
			ISceneSettings * scenesettings = renderObject->GetSceneSettings();
			if (scenesettings)
			{
				if (scenesettings->IsEnabled( SHADOW_CAST ))
				{
					m_ShadowCasterRenderObjects.push_back( renderObject );
				}
			}
		}
		return true;
	}

	virtual bool IsHierarchicalVisitor() { return false; }

	void SetLight( CLightObject * light )
	{
		if( light )
		{
			m_Light = light;
		}
	}
	inline int GetShadowCastObjectCount() const
	{
		return (int)m_ShadowCasterRenderObjects.size();
	}

	void UpdateShadowMaps()
	{
		if (m_ShadowCasterRenderObjects.size() == 0)
			return;

		OBJECTLIST::iterator iter;
		// reset object scene shader flags to cast shadows
		iter = m_ShadowCasterRenderObjects.begin();
		for( ; iter != m_ShadowCasterRenderObjects.end(); iter++)
		{
			IRenderObject* renderObject = (IRenderObject*)*iter;
			ISceneSettings * scenesettings = renderObject->GetSceneSettings();

			scenesettings->PushFlagState();
			//Forcifully removing these flags. Once we have a "Must include" setting for shaderflags,
			//for best fit, we won't have to do this. But waiting on the more dynamic system for shaderflags that
			//we thought up last time before implementing this feature.
			scenesettings->RemoveShaderFlag( SHADOW_RECEIVE_HEMI | SHADOW_RECEIVE_OMNI |
											SHADOW_RECEIVE_DIR | SHADOW_RECEIVE | ALPHA);
			if( m_Light->GetLightType() == OMNI_POINT_LIGHT )
			{	
				scenesettings->AddShaderFlag( SHADOW_CAST_OMNI );
			}
			else if( m_Light->GetLightType() == HEMI_POINT_LIGHT )
			{	
				scenesettings->AddShaderFlag( SHADOW_CAST_HEMI );
			}
		}

		// generate light's shadow map
		GENERATELIGHTPERSPECTIVEMAPPARAMS params;
		params.lightObject = m_Light;
		params.renderObjects = &m_ShadowCasterRenderObjects;
		static CHashString hszLightManager = CHashString( _T("CLightManager") );
		static DWORD msg_GenerateLightPerspectiveMaps = CHashString( _T("GenerateLightPerspectiveMap") ).GetUniqueID();
		DWORD retval = EngineGetToolBox()->SendMessage( msg_GenerateLightPerspectiveMaps, sizeof(GENERATELIGHTPERSPECTIVEMAPPARAMS), &params, NULL, &hszLightManager );

		// reset object scene shader flags
		iter = m_ShadowCasterRenderObjects.begin();
		for( ; iter != m_ShadowCasterRenderObjects.end(); iter++)
		{
			IRenderObject* renderObject = (IRenderObject*)*iter;
			ISceneSettings * scenesettings = renderObject->GetSceneSettings();
			scenesettings->PopFlagState();				
		}
	}

private:
	CLightObject* m_Light;
	OBJECTLIST m_ShadowCasterRenderObjects;
};

class ShadowCasterCullVisitor : public IVisitor
{
public:
    
	virtual bool Visit( IComponent * component, bool bVisitEnter )
	{
		Vec3 AABBMin;
		Vec3 AABBMax;
		//Light Affector list for this quad
		m_ObjList.clear();
		static CHashString sceneNodeTypeName( _T( "ISceneNode" ));
		if( (component->GetComponentType()->GetUniqueID()) == sceneNodeTypeName.GetUniqueID())
		{
			ISceneNode* qnode = (ISceneNode*)component;		
			qnode->GetBoundingAABB( AABBMin, AABBMax );
			
			//accumulate casters for this quad
			BuildCasterAffectorListFromQuad( qnode );
			
		}
		// always return false so we visit only this node
		return false;
	}

	virtual bool IsHierarchicalVisitor() { return false; }

	virtual void SetLightObject( ILightObject * ilight )
	{
		m_LightObject = ilight;
		if (!m_LightObject)
			return;

		//set up some fast test vars, don't like grabbing the info over and over in the loop
		if( m_LightObject->GetLightType() == OMNI_POINT_LIGHT || 
			m_LightObject->GetLightType() == HEMI_POINT_LIGHT)
		{
			m_CullDir = true;//cull also but based on omni up or down
			m_Dir[ 0 ] = 0;
			m_Dir[ 1 ] = -1;
			m_Dir[ 2 ] = 0;
			m_LightObject->GetPosition( m_Pos );
		}
		else
		{
			m_CullDir = true;
			m_LightObject->GetDirection( m_Dir );
			m_LightObject->GetPosition( m_Pos );
			m_Dir.Normalize();
		}
	}

	virtual OBJECTLIST * GetObjects()
	{
		return &m_ObjList;
	}

	UINT GetNumObjects()
	{
		return (UINT)m_ObjList.size();
	}

	void ResetObjectFlags()
	{
		OBJECTLIST::iterator iter = m_ObjList.begin();
		for( ;iter != m_ObjList.end(); ++iter )
		{
			IRenderObject * obj = (IRenderObject *) (*iter);
			if( obj )
			{
				//we dont' test if this is really a renderobject because that would just be slow
				//if it ever breaks then you're using this wrong
				ISceneSettings * settings = obj->GetSceneSettings();
				if( settings )
				{
					settings->PopFlagState();				
				}
			}
		}
	}

	void SetOmniUp()
	{		
		m_Dir[ 1 ] = 1;
	}

	void Reset()
	{
		m_ObjList.clear();
	}
private:
	
	inline void AddObjectToLists( IRenderObject* curRObject )
	{
		ISceneSettings * scenesettings = curRObject->GetSceneSettings();
		if( !scenesettings || !scenesettings->IsEnabled( SHADOW_CAST ) )
		{
			return;
		}

		Vec3 objPos;
		float objRad;
		curRObject->GetBoundingSphere( objPos, objRad );

		Vec3 objDist = objPos - m_Pos;

		// distance check
		float dist = objRad + m_LightObject->GetAttenuationDistance();
		dist *= dist;
		float sqDist = objDist.Dot(objDist);
		if( sqDist > dist )
			return;

		//check if it's a hemi light or directional light and cull away objects based on direction.
		if( m_CullDir )
		{
			//do a dot the distance between the light and the object with the direction
			float dot;
			if( m_LightObject->GetLightType() == HEMI_POINT_LIGHT || 
				m_LightObject->GetLightType() == OMNI_POINT_LIGHT )
			{
				//hemi dot
				objDist = (objPos+m_Dir*objRad) - m_Pos;
				objDist.Normalize();
				dot = objDist.Dot( m_Dir );
				if (dot < 0.0f)
					return;
			}
			else if( m_LightObject->GetLightType() == DIRECTIONAL_LIGHT )
			{
			}
		}
		//look for a mapping for the object's registered Light Perspective Map Effect, either 
		//omni point, hemi point or directional
		//depending on the currentEffecttype set
  		m_ObjList.push_back( curRObject );
		scenesettings->PushFlagState();
		//Forcifully removing these flags. Once we have a "Must include" setting for shaderflags,
		//for best fit, we won't have to do this. But waiting on the more dynamic system for shaderflags that
		//we thought up last time before implementing this feature.
		scenesettings->RemoveShaderFlag( SHADOW_RECEIVE_HEMI | SHADOW_RECEIVE_OMNI |
										SHADOW_RECEIVE_DIR | SHADOW_RECEIVE | ALPHA);
		if( m_LightObject->GetLightType() == OMNI_POINT_LIGHT )
		{	
			scenesettings->AddShaderFlag( SHADOW_CAST_OMNI );
		}
		else if( m_LightObject->GetLightType() == HEMI_POINT_LIGHT )
		{	
			scenesettings->AddShaderFlag( SHADOW_CAST_HEMI );
		}
	}

	inline void BuildCasterAffectorListFromQuad( ISceneNode* qnode )
	{
		OBJECTSET::iterator iter;
		ISceneNode* curParent = qnode;
		do{
			ISceneNode* newParent = curParent->GetParent();
			if( !newParent )
			{
				//recurse the parent nodes
				Vec3 center;
				float radius;
				m_LightObject->GetBoundingSphere( center, radius );
				if( radius > MAX_SHADOW_ATTEN )
					radius = MAX_SHADOW_ATTEN;
				//RecurseGrabChildren( curParent, center, radius);	
			}
			curParent = newParent;
		}while( curParent );
	}
	
private:
	bool			m_CullDir;
	Vec3			m_Dir;
	Vec3			m_Pos;
	OBJECTLIST		m_ObjList;
	ILightObject *		m_LightObject;
};


class CLightManager : public CManager
{

public:

	/// \brief The Default Destructor for CLightManager
	/// \return void
	~CLightManager( );

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief	Update function for the Particle Manager
	/// \return	void
	virtual void Update( DWORD tickCount );
	
	/// \brief	DeInitialization
	/// \return	bool: Success
	virtual bool DeInit( void );

	DWORD OnGetLightObjPtrByName( DWORD size, void *params);
	DWORD OnEnableDepthShadows( DWORD size, void *params);
	DWORD OnRegisterLight( DWORD size, void *params);
	DWORD OnUnregisterLight( DWORD size, void *params);	

	DWORD OnGetLightShadowTexture( DWORD size, void *params);
	DWORD OnGetLightArray( DWORD size, void *params);
	DWORD OnSetShadowFrameInterval( DWORD size, void *params);
	DWORD OnGenerateLightPerspectiveMap( DWORD size, void *params);

	//temp
	static	void RenderLight( ILightObject * lightObject );
	
	//temp, find a way to automate this without so much complex hook up and to not clutter the ILight interface
	//with the ability to cast shadows
	IBaseTextureObject * GetLightShadowTexture( ILightObject * lightObject );
	Light_Info * GetLightShadowInfo( ILightObject * lightObject );
	bool HasValidShadowMap( ILightObject * lightObject );
private:
	bool GenerateLightPerspectiveMap( ILightObject*, OBJECTLIST* renderObjects );
	void CreateDepthShadowRenderTarget( IBaseTextureObject ** tex, ILightObject * lightObject );		
	void RemoveDepthShadowRenderTarget( IBaseTextureObject * tex );
	void GenerateLightPerspectiveTexture( IBaseTextureObject * tex,
											ILightObject * light,
											OBJECTLIST * renderList
											, int pass  = 0);
	void GenerateDirectionalLightPerspectiveTexture( IBaseTextureObject * tex,
											ILightObject * light,
											OBJECTLIST * renderList );


	SINGLETONCONSTRUCTOROVERRIDE( CLightManager );
	/// \brief The Default Constructor for CLightManager
	/// \return void
	CLightManager();

	//Registered lights
	float				m_CurrentTick;
	LIGHTINFOMAPPING	m_LightList;
	bool				m_bLightTextureUpdating;
	int					m_ShadowFpsInterval;
	size_t					m_ShadowMapPoolCount;
	int					m_ShadowMapMaxPerScreen;
	vector< ShadowTexturePoolEntry > m_ShadowMaps;

	// light type name that we manage
	CHashString m_LightType;
	map< ILightObject *, int > m_LastShadowGenerationFrames;
	UINT				m_LPMTempIndex;

	IBaseTextureObject * GetValidShadowMap( size_t index )
	{
		if( index >= 0 && index < m_ShadowMaps.size() )
		{
			return m_ShadowMaps[ index ].texture;
		}
		return NULL;
	}
	int GetLastUsedShadowMapIndex()
	{
		float lastIndexTick = m_CurrentTick;
		int lastIndex = INVALID_SHADOW_MAP_INDEX;
		for( size_t i = 0; i < m_ShadowMaps.size(); i++ )
		{
			ShadowTexturePoolEntry &entry = m_ShadowMaps[ i ];
			if( entry.m_LastFrameSet < m_CurrentTick )
			{
				if( entry.m_LastFrameSet < lastIndexTick )
				{
					lastIndex = (int)i;
					lastIndexTick = entry.m_LastFrameSet;
				}
			}
		}
		return lastIndex;
	}
};

#endif
