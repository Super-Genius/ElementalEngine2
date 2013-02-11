///==========================================================================
/// \file	Cal3DRenderObject.h
/// \brief	Implementation of Cal3DRenderObject interface for rendering
/// \date	3/9/2005
/// \author	Marvin Gouw
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

#ifndef	_Cal3DRenderObject_H_
#define _Cal3DRenderObject_H_

#include "CSceneObject.h"
using namespace std;

class CalModel;
class CalCoreMesh;
class CalHardwareModel;
class CalCoreModel;
class Cal3DModelManager;

///==========================================================================
/// \class	Cal3DRenderObject
/// \brief	Adapter for Cal 3d models
///==========================================================================
class Cal3DRenderObject : public CSceneObject<IRenderObject>
{
private:
	/// \brief The Default Constructor for Cal3DRenderObject
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	Cal3DRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier);

public:
	/// \brief	The Default Destructor for Cal3DRenderObject
	~Cal3DRenderObject();

	/// \brief	Create the a instance of IObject
	/// \return	IComponent pointer
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Initialization for the Object
	/// \param	IManager* pManager: Parent Manager
	virtual void Init();
	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();
	/// \brief	The DeInit Function for the Object
	virtual void DeInit();
	
	virtual OBJECTSORTTYPES GetSceneSortID() const { return RENDEROBJECT3D; }

	/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render( UINT pass, IEffect * override );

	/// \brief Compare for sorting
	/// \param obj = object to compare to
	/// \param sortMethod = a sort method specifying how to compare(set in RENDEROBJECTCOMPARE)
	/// \param curRenderPass = the current render pass to sort to
	/// \return bool = must return true if (this pointer) < (obj)
	virtual bool IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass );

	/// \brief Number of passes this render object has to go through
	/// \return UINT = returns one pass for this implementation
	virtual UINT GetNumPass() { return 1; };
	
	/// \brief This is for render priorities between classes
	/// will change as soon as a new architecture can be designed to better incorporate
	/// different renderobject types being pushed on the same sorted list
	/// \return DWORD = the priority key this returns, the higher the number,
	/// the higher its priority
	virtual DWORD GetRenderPriority(){ return 0; }

	/// \brief Sets the position of the render object
	/// \param position Vec3 class reference to fill in
	virtual void SetPosition( const Vec3 &vec ){}

	/// \brief Sets the rotation/orientatino of the render object
	/// \param x = x rotation angle
	/// \param y = y rotation angle
	/// \param z = z rotation angle
	virtual void SetRotation( const Vec3 &vec ){}

	/// \brief Sets the scaling of the render object
	/// \param x = x scale
	/// \param y = y scale
	/// \param z = z scale
	virtual void SetScaling( const Vec3 &vec ){}

	/// \brief Gets the position of the render object
	/// \return Vec3 = position f the object
	virtual Vec3 &GetPosition(){ 
		//SHOULD NOT use this because entity handles everything
		static Vec3 curpos;
		if( m_GeometryController )
		{
			curpos =  m_GeometryController->GetTranslationOffset();			
		}
		return curpos;
	}


	/// \brief Gets bounding sphere of object. This is a 3D interface call that should be implemented for only
	///			3D objects. Might want to seperate more of this later
	/// \param position = vector that will be filled the center of the sphere
	/// \param radius = float that will be filled with the radius of the sphere
	virtual void GetBoundingSphere( Vec3 &position, float &radius );

	/// \brief finds out whether object has alpha
	/// and needs to be put in the alpha queue
	virtual bool IsAlphaObject();
	
	/// \brief either reads or writes the entity information to a file
	/// Currently Loads up the entity completely if reading
	virtual void Serialize(IArchive &ar);

	/// \brief Gets the base sort class which implements IRenderObject::IsLessThan()
	/// child classes must not re-implement IsLessThan() if they return the BaseSortClass hash!!
	/// typically, each render object has a IsLessThan() and returns a unique BaseSortClass
	virtual IHashString * GetBaseSortClass();

	virtual IMeshObject * GetMeshObject();

	/// Gets the number of Geometry elements this renderobject possesses or if it uses geometry at all	
	virtual UINT GetNumGeometry();

	/// Grabs the geometry pieces of the render object if there is any uses it for render instead
	/// of the render object render(might need to be changed)
	virtual IGeometry* GetGeometry( int index );

	virtual bool IsKindOf( IHashString * compType );

	/// \brief GetBoundingBox grabs the min and max vertices
	/// child should extend off this
	virtual bool	GetBoundingBox( Vec3 &position, Vec3 &extents, EulerAngle &rotation );

	/// Lights will be propogated to Geometry for static, unskinned models
	/// Attaches a light object to this Render object
	virtual void AttachLight( ILightObject * light );
	/// Signals a detach of all light objects
	virtual void DetachAllLights();

	virtual ISceneSettings * GetSceneSettings(){ return m_SceneSettings; }

	virtual UINT GetNumMaterial() { return m_Materials.size(); }
	virtual IMaterial* GetMaterial( int index )
	{
		unsigned int i = index;
		if( (i >= 0) && (i < m_Materials.size()) )
		{
			return m_Materials[i];
		}
		return NULL;
	}

	/// Gets the attach transform (called from attachment to base)
	/// \param hsNodeName = name of node if specified
	/// \param attachXForm = returned matrix transform
	virtual void GetAttachTransform(IHashString *hsNodeName, Matrix4x4 &attachXForm);

	inline CHashString& GetFileHash()
	{
		return m_FileHash;
	}

	inline StdString& GetFilename()
	{
		return m_Filename;
	}

	inline void ResetRenderOffsetDelta() { m_bResetRenderOffsetDelta = true; }


	DWORD OnSetGlobalPosition(DWORD size, void *params);
	DWORD OnSetGlobalEuler(DWORD size, void *params);	
	DWORD OnSetScale(DWORD size, void *params);
	DWORD OnGetScale(DWORD size, void *params);
	DWORD OnGetModelFileID(DWORD size, void *params);
	DWORD OnGetGlobalPosition(DWORD size, void *params);
	DWORD OnGetGlobalEuler(DWORD size, void *params);
	//Animation messages	
	DWORD OnPlayAnimation( DWORD size, void * params );
	DWORD OnPlaySingleCycleAnimation( DWORD size, void * parasm );
	DWORD OnStopAnimation( DWORD size, void * parasm );
	DWORD OnStopAllAnimation( DWORD size, void * parasm );
	DWORD OnStopAllAnimationActions( DWORD size, void * params );
	DWORD OnGetAnimationList( DWORD size, void * params );
	//visibility
	DWORD OnSetVisibility( DWORD size, void * params );
	DWORD OnSetMatrix( DWORD size, void * params );
	DWORD OnGetBoundingSphere( DWORD size, void * params );

	DWORD OnRegisterAnimationCallback( DWORD size, void * params );
	DWORD OnUnregisterAnimationCallback( DWORD size, void * params );
	DWORD OnUnregisterAllAnimationCallbacks( DWORD size, void * params );
	DWORD OnGetAnimationId( DWORD size, void * params );	
	DWORD GetBoneInfo( DWORD size, void * params );

	DWORD OnRemoveFromQuadTree( DWORD size, void * params );
	DWORD OnAddToQuadTree( DWORD size, void * params );
	
	DWORD OnGetModelFileName( DWORD size, void * params );


	DWORD OnSaveCalCubes( DWORD size, void * params );	
	DWORD OnGetGeometryAllocations( DWORD size, void * params );

	DWORD OnGetBoundingBox( DWORD size, void* params );
	
	DWORD OnSetCalScaleVariable( DWORD size, void * params );
	DWORD OnSetLightmapData( DWORD size, void* params );
	DWORD OnGetParentName( DWORD size, void * params );
	DWORD OnSetSkeletonPose( DWORD size, void* params );
	DWORD OnEnablePhysicsAnimations( DWORD size, void * params );
	DWORD OnGetSkeletonPose( DWORD size, void * params );

	DWORD OnGetRenderObjectInterface( DWORD size, void *param );
	DWORD OnGetControllerInterface( DWORD size, void * param );

	DWORD OnSetDrawSkeleton(DWORD size, void *param);

	DWORD OnSetMaterialOverride(DWORD size, void *param);
	DWORD OnClearMaterialOverride(DWORD size, void *param);

	DWORD OnAttachSceneObject(DWORD size, void *param);	

	DWORD OnSetRenderOffset( DWORD size, void * param );
	DWORD OnGetRenderOffset( DWORD size, void * param );
	DWORD OnPauseAnimations( DWORD size, void * param );
private:
	void UpdateBoundingSphere();
	void LoadCalHardwareModel();
	bool AllocateHardwareVertices();
	void BuildBoneNameLinks();
	void InitAnimation();
	void GetParentTransform( Matrix4x4& transform );
	DWORD ProcessAnimationCallbackEvent( DWORD size, void * params, bool addModel );
	void SetMaterialOverrideValues(IHashString *hsName, IHashString *hsType, IHashString *hsValue, 
		Vec4* vecValue, IHashString *hsMaterial, bool bClear);
	void StopAllAnimationCycles( float fDelay );
	void StopAllAnimationActions();
	void UnregisterAllAnimationCallbacks();

	CalBone* GetBone( const TCHAR *szBoneName );
	Vec3 GetBoneGlobalPosition( CalBone *pCalBone, bool bUseRootBoneOffset=true );
	Vec3 GetBoneGlobalPositionNoTranslation( CalBone *pCalBone, bool bUseRootBoneOffset=true );
	Vec3 GetBoneModelPosition( CalBone *pCalBone );
	void UpdateRenderOffset();

	Cal3DModelManager *m_pModelManager;

	bool m_bIsVisible;
	bool m_bDebugShowSkeleton;

	/// \brief the Filename of the XX entity
	CHashString m_FileHash;
	StdString m_Filename;
	StdString m_ShaderFile;
	IRenderer * m_IRenderer;

	//Cal3D Object
	CalModel * m_calModel;
	CalHardwareModel * m_calHardwareModel;
	IController * m_GeometryController;
	vector< IGeometry* > m_Geometry;
	Vec3		m_RenderOffset;
	Vec3		m_PrevRenderOffset;
	CalBone *m_pRenderOffsetBone;
	Vec3		m_PrevFrameRenderOffset;
	Vec3		m_DeltaRenderOffset;
	bool m_bResetRenderOffsetDelta;
	map< DWORD, Matrix4x4 > m_WorldBoneMatrices;
	map< DWORD, CHashString > m_BoneIdToHashMap;

	int	m_calHardwareVertexCount;
	int m_calHardwareIndexCount;
	bool	m_bSkinnedModel;
	float				m_LastUpdateTick;

	//global allocation for this model
	BUFFERALLOCATIONSTRUCT m_IndexBufferAllocation;
	BUFFERALLOCATIONSTRUCT m_VertexBufferAllocation;
	IIndexBuffer * m_IndexBufferInterface;
	IVertexBufferObject * m_VertexBufferInterface;

	vector< IMaterial *  > m_Materials;
	ISceneSettings *	m_SceneSettings;
	int					m_NumLights;
	float				m_AmbientLightTerm[4];
	ILightObject *		m_Lights[ MAX_LIGHTS ];
	float				m_LightPriorities[ MAX_LIGHTS ];
	bool m_bAddToHierarchy;
	bool m_bHasAlphaSet;
	bool m_bHasAlpha;
	bool m_bCallbacksCalled;

	Vec3 SerializedPosition;
	Vec3 SerializedScaling;
	EulerAngle SerializedRotation;
	bool m_bEnablePhysicsAnimation;

	float m_fObjectAlpha;
	Vec3 m_CachedPosition;
	float m_CachedRadius;
	static float m_BoundingTestThreshold;

	float m_fElapsedTime;
	bool	m_bPaused;

	class CActiveAnimation
	{
	public:
//		bool m_bCycle;
		int m_AnimID;
	};
	list<CActiveAnimation> m_ActiveAnimationCycles;
};

#endif	// Cal3DRenderObjectOBJECT_HPP
