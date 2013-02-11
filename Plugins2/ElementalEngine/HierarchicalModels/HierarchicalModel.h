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

#ifndef _HierarchicalModel_H
#define _HierarchicalModel_H

#include "stdafx.h"
#include "IRenderObject.h"
#include "CSceneObject.h"
#include "Matrix4x4.h"

#define FLOOR(x) ((int)(x) - ((x) < 0 && (x) != (int)(x)))
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))

class HierarchicalModelType;

struct HierarchicalNodeData
{
private:
	HierarchicalNode *m_pNode;  // TODO: THIS IS NOT CORRECT DATA
	HierarchicalNodeData *m_pParent;
	IToolBox *		m_ToolBox;
	IController	*	m_pController;
	IController *	m_pNodeControllerInterface;
	IRenderObject * m_pNodeRenderInterface;

	CHashString		m_hszCal3DRenderObject;

	Vec3			m_OffsetTranslation;
	EulerAngle		m_OffsetRotation;
	Vec3			m_OffsetScale;

public:
	// Render properties
	CHashString		m_RenderObject;
	bool			m_bInitialized;
	CHashString		m_hsNodeName;

	bool			m_bPhysicsControlled;

	// Offset Transform ZeroMats
	bool			m_DirtyOffsetTranslation;
	bool			m_DirtyOffsetRotation;
	bool			m_DirtyOffsetScale;

	// Global Transform Properties
	Matrix4x4		m_GlobalMatrix;
	Vec3			m_GlobalTranslation;
	EulerAngle		m_GlobalRotation;
	Quaternion		m_GlobalQuaternion;
	Vec3			m_GlobalScale;

	// Animation Key
	int				m_AnimationCurrentKeyTranslation;
	int				m_AnimationCurrentKeyRotation;
	int				m_AnimationCurrentKeyScale;

	inline void Init(){
		m_pNode = NULL;
		m_pParent = NULL;
		m_pController = NULL;
		m_ToolBox = EngineGetToolBox();

		m_bInitialized = false;
		m_DirtyOffsetScale = false;
		m_DirtyOffsetRotation = false;
		m_DirtyOffsetTranslation = false;

		m_bPhysicsControlled = false;
		m_AnimationCurrentKeyTranslation = 0;
		m_AnimationCurrentKeyRotation = 0;
		m_AnimationCurrentKeyScale = 0;
	}

	HierarchicalNodeData():
	m_hszCal3DRenderObject(_T("Cal3DRenderObject")){
		Init();
	}
	void SetInterfaces()
	{
			DWORD retval = 0;
			static DWORD msgHash_GetRenderObjectInterface = CHashString(_T("GetRenderObjectInterface")).GetUniqueID();
			static CHashString Cal3DRenderObjectType(_T("Cal3DRenderObject"));
			retval = m_ToolBox->SendMessage(msgHash_GetRenderObjectInterface, sizeof( IRenderObject * ), &m_pNodeRenderInterface, 
														&m_RenderObject, &Cal3DRenderObjectType );

			if ( retval != MSG_HANDLED )
			{
				m_ToolBox->Log(LOGERROR, _T("%s(%d): Failed to get IRenderObject Interface: %s\n"), __FILE__, __LINE__, m_hsNodeName.GetString());
			}
			static DWORD msgHash_GetControllerInterface = CHashString(_T("GetControllerInterface")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_GetControllerInterface, sizeof( IController * ), &m_pNodeControllerInterface, 
														&m_RenderObject, &Cal3DRenderObjectType);

			if ( retval != MSG_HANDLED )
			{
				m_ToolBox->Log(LOGERROR, _T("%s(%d): Failed to get IRenderObject Interface: %s\n"), __FILE__, __LINE__, m_hsNodeName.GetString());
			}
	}

	void SetFileNodeData(HierarchicalNode * node)
	{
		if (node != NULL)
			m_pNode = node;
	}

	__forceinline void BuildMat( Matrix4x4 &m, const Quaternion quat, const Vec3 scale, const Vec3 pos )
	{
		quat.GetMatrix(m);
		m.SetTranslation( pos );
		m.m[0] *= scale.x;
		m.m[1] *= scale.x;
		m.m[2] *= scale.x;
			
		m.m[4] *= scale.y;
		m.m[5] *= scale.y;
		m.m[6] *= scale.y;
			
		m.m[8] *= scale.z;
		m.m[9] *= scale.z;
		m.m[10] *= scale.z;
	}
	inline void SetParent(HierarchicalNodeData * parent)
	{
		if (parent)
			m_pParent = parent;
	}
	inline void SetController(IController * controller)
	{
		if (controller)
			m_pController = controller;
	}


	inline void UpdateRecurse(float curtime, HierarchicalNode* node)
	{
		//grab the quats, build, recurse to children and build them	
		m_AnimationCurrentKeyRotation = node->GetRotation( m_GlobalQuaternion, curtime, m_AnimationCurrentKeyRotation );
		m_AnimationCurrentKeyScale = node->GetScaling( m_GlobalScale, curtime, m_AnimationCurrentKeyScale  );
		m_AnimationCurrentKeyTranslation = node->GetTranslation( m_GlobalTranslation, curtime, m_AnimationCurrentKeyTranslation  );

		//add offsets
		static Matrix4x4 statMat;
		statMat.SetIdentity();
		if( m_DirtyOffsetRotation )
			statMat.SetRotation( m_OffsetRotation );
		if( m_DirtyOffsetTranslation )
			statMat.SetTranslation( m_OffsetTranslation );
		if( m_DirtyOffsetScale )
		{
			statMat.m[0] *= m_OffsetScale.x;
			statMat.m[1] *= m_OffsetScale.x;
			statMat.m[2] *= m_OffsetScale.x;
				
			statMat.m[4] *= m_OffsetScale.y;
			statMat.m[5] *= m_OffsetScale.y;
			statMat.m[6] *= m_OffsetScale.y;
				
			statMat.m[8] *= m_OffsetScale.z;
			statMat.m[9] *= m_OffsetScale.z;
			statMat.m[10] *= m_OffsetScale.z;
		}
		if( m_bPhysicsControlled )
		{
			//global physics driven
			m_GlobalMatrix = statMat;
			m_GlobalTranslation = m_OffsetTranslation;
			m_GlobalScale = m_OffsetScale;
		}else
		{
			BuildMat( m_GlobalMatrix, m_GlobalQuaternion, m_GlobalScale, m_GlobalTranslation );
			m_GlobalMatrix = m_GlobalMatrix*statMat;

			//Using local transforms
			if (m_pParent){// Recursively build from parent
				m_GlobalTranslation = m_pParent->m_GlobalTranslation + m_GlobalTranslation;
				m_GlobalScale = m_pParent->m_GlobalScale * m_GlobalScale;
				m_GlobalMatrix = m_pParent->GetGlobalMatrix() * m_GlobalMatrix;
			}
			else{// Root node, build from Controller.
				m_GlobalTranslation = m_pController->GetTranslationOffset() + m_GlobalTranslation;
				m_GlobalRotation = m_pController->GetRotationOffset();
				m_GlobalScale = m_pController->GetScaling() * m_GlobalScale;
				m_GlobalMatrix = *(m_pController->GetMatrix()) * m_GlobalMatrix;
			}
		}

		//send render message only to objects with cal stuff, points nodes not sent messages
		if( m_bInitialized )
		{
			assert(m_pNodeControllerInterface && m_pNodeRenderInterface);
			m_pNodeControllerInterface->SetScaling(m_GlobalScale);
			m_pNodeControllerInterface->SetMatrix(&m_GlobalMatrix);

			m_pNodeRenderInterface->UpdateBoundingObject();
		}
	}

	inline void UpdateGlobalMatrix()
	{

	}
	inline Matrix4x4 GetGlobalMatrix( )
	{
		return m_GlobalMatrix;
	}
	inline void SetGlobalScale( Vec3 &Scale )
	{
		m_GlobalScale.Set( Scale );
	}
	inline Vec3 GetGlobalScale( )
	{
		return m_GlobalScale;
	}
	inline void SetGlobalRotation( EulerAngle &Rotation )
	{
		m_GlobalRotation.Set( Rotation );
	}
	inline EulerAngle GetGlobalRotation( )
	{
		return m_GlobalRotation;
	}
	inline void SetGlobalTranslation( Vec3 &Translation )
	{
		m_GlobalTranslation.Set( Translation );
	}
	inline Vec3 GetGlobalTranslation( )
	{
		return m_GlobalTranslation;
	}

	inline void SetOffsetScale( Vec3 &scale )
	{
		if( scale.x == 1 &&	scale.y == 1 &&	scale.z == 1 )
			m_DirtyOffsetScale = false;
		else 
			m_DirtyOffsetScale = true;
		m_OffsetScale.Set( scale );
	}
	inline Vec3 GetOffsetScale( )
	{
		return m_OffsetScale;
	}
	inline void SetOffsetRotation( EulerAngle &Rotation )
	{
		if( Rotation.roll == 0 &&	Rotation.pitch == 0 &&	Rotation.yaw == 0 )
			m_DirtyOffsetRotation = false;
		else 
			m_DirtyOffsetRotation = true;
		m_OffsetRotation.Set( Rotation );
	}
	inline EulerAngle GetOffsetRotation( )
	{
		return m_OffsetRotation;
	}
	inline void SetOffsetTranslation( Vec3 &Translation )
	{
		if( Translation.x == 0 &&	Translation.y == 0 &&	Translation.z == 0 )
			m_DirtyOffsetTranslation = false;
		else 
			m_DirtyOffsetTranslation = true;
		m_OffsetTranslation.Set( Translation );
	}
	inline Vec3 GetOffsetTranslation( )
	{
		return m_OffsetTranslation;
	}
};

class CHierarchicalModel : 
public CSceneObject<IRenderObject>
{
	/// \brief Default Constructor
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CHierarchicalModel(IHashString *parentName, IHashString *name, bool bAddToHier);
public:

	//some loader
	virtual ~CHierarchicalModel();

	/// \brief Create an instance
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief DeInit Function
	virtual void DeInit();

	/// \brief Init Function
	virtual void Init();

	/// \brief serialize
	/// \param ar - The Archive
	virtual void Serialize(IArchive &ar);

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

	/// \brief This is for render priorities between classes
	/// will change as soon as a new architecture can be designed to better incorporate
	/// different renderobject types being pushed on the same sorted list
	/// \return DWORD = the priority key this returns, the higher the number,
	/// the higher its priority
	virtual DWORD GetRenderPriority();

	/// \brief Number of passes this render object has to go through
	/// \return UINT = returns one pass for this implementation
	virtual UINT GetNumPass() { return 1; };

	/// \brief Sets the position of the render object
	/// \param position Vec3 class reference to fill in
	virtual void SetPosition( const Vec3 &vec );

	/// \brief Gets bounding sphere of object. This is a 3D interface call that should be implemented for only
	///			3D objects. Might want to seperate more of this later
	/// \param position = vector that will be filled the center of the sphere
	/// \param radius = float that will be filled with the radius of the sphere
	virtual void GetBoundingSphere( Vec3 &position, float &radius );

	virtual IHashString * GetBaseSortClass(){ return GetComponentType(); }

	/// \brief returns if this renderobject needs to be sorted in the transparency list
	/// \return bool = true if this object uses transparency
	virtual bool IsAlphaObject(){ return false; }

	IMeshObject * GetMeshObject(){ return NULL; }

	/// Gets the number of Geometry elements this renderobject possesses or if it uses geometry at all	
	virtual UINT GetNumGeometry(){ return 0; }

	/// Grabs the geometry pieces of the render object if there is any uses it for render instead
	/// of the render object render(might need to be changed)
	virtual IGeometry* GetGeometry( int index ){ return NULL; }

	virtual bool IsKindOf( IHashString * compType );

	virtual void AttachLight( ILightObject * light ) { }
	virtual void DetachAllLights() { }

	/// Gives the min and max bounds of the terrain's geometry
	/// \param aabbMin - vec to store the min bounds
	/// \param aabbMax - vec to store the max bounds
	void GetBounds( Vec3 &aabbMin, Vec3 &aabbMax );

	/// \brief GetBoundingBox default implementation
	/// child should extend off this
	virtual bool GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rot );
	
	virtual IMaterial * GetMaterial( int index ){ return NULL; }
	virtual UINT GetNumMaterial() { return 0; }
	virtual ISceneSettings * GetSceneSettings(){ return NULL; }

	void ResetAnimation();
	void ExecuteAnimation( HNODEVECTOR * list, float time, float speed);
	//Updates and interpolates and recursively builds all the hierarchal matrices
	bool Update();

	/// Gets the attach transform (called from attachment to base)
	/// \param hsNodeName = name of node if specified
	/// \param attachXForm = returned matrix transform
	virtual void GetAttachTransform(IHashString *hsNodeName, Matrix4x4 &attachXForm);

	DWORD OnSetChildPositionOffset( DWORD size, void * params );
	DWORD OnSetChildRotationOffset( DWORD size, void * params );
	DWORD OnSetChildScaleOffset( DWORD size, void * params );

	DWORD OnGetChildPositionOffset( DWORD size, void * params );
	DWORD OnGetChildRotationOffset( DWORD size, void * params );
	DWORD OnGetChildScaleOffset( DWORD size, void * params ); 

	DWORD OnGetChildID( DWORD size, void * params );
	DWORD DoAnimation( DWORD size, void * params );
	DWORD OnGetChildGlobalRotation( DWORD size, void * params );
	DWORD OnGetChildGlobalPosition( DWORD size, void * params );
	DWORD OnGetChildGlobalScale( DWORD size, void * params );
	
	DWORD OnGetChildGlobalMatrix( DWORD size, void * params );

	DWORD OnSetGlobalPosition(DWORD size, void *params);
	DWORD OnSetGlobalEuler(DWORD size, void *params);	
	DWORD OnSetScale(DWORD size, void *params);
	DWORD OnGetGlobalPosition(DWORD size, void *params);
	DWORD OnGetGlobalEuler(DWORD size, void *params);
	DWORD OnGetGlobalScale(DWORD size, void *params);
	DWORD OnGetGlobalTransform(DWORD size, void *params);
	
	DWORD OnPlayAnimation(DWORD size, void *params);

	DWORD OnGetGlobalBoneInfo(DWORD size, void *param);
	DWORD OnSetGlobalBoneInfo(DWORD size, void *param);
	DWORD OnGetOffsetBoneInfo(DWORD size, void *param);
	DWORD OnSetOffsetBoneInfo(DWORD size, void *param);

	DWORD OnGetBoundingSphere(DWORD size, void *param);	
	DWORD OnGetGeometryAllocations( DWORD size, void * params );
	DWORD OnGetGeometryAllocationByName( DWORD size, void * params );
	DWORD OnGetBoundingBox( DWORD size, void* param );
	
	DWORD OnSetSkeletonPose( DWORD size, void* param );
	DWORD OnEnablePhysicsAnimations( DWORD size, void * params );

	DWORD OnGetParentName(DWORD size, void *param);

	DWORD OnSetMaterialOverride(DWORD size, void *param);
	DWORD OnClearMaterialOverride(DWORD size, void *param);
	
	DWORD OnAttachSceneObject(DWORD size, void *param);	
	DWORD OnAlignToSceneObject(DWORD size, void *param);

protected:
	bool IsValidChild( UINT i ){ return (i >= 0 && i < m_HierarchicalNodeData.size() ); }
	void InitHierarchicalNodeData( HNODEVECTOR &Template );
	void BuildInternalRenderObjects();
	void ClearHierarchicalNodeData();
	void CreateCalRenderObject( HierarchicalNodeData &rnode, HierarchicalNode * node, int id);
	void CreateClothRenderObject( HierarchicalNodeData &rnode, HierarchicalNode * node, int id);
	HierarchicalNode * GetRoot();
	void RecurseSetupTransforms( HierarchicalNode * node );
	void RecalculateBoundingSphere( Vec3 &position, float &radius );
	void RecalculateBoundingBox( Vec3& position, Vec3& extents, EulerAngle& rotation );
	void GetParentTransform( Matrix4x4& transform );

	void RenderGlobalNode( HierarchicalNode * node );
	HNODEVECTOR *		m_CurrentAnimation;
	HNODEVECTOR *		m_CoreHierarchy;
	// Major Data Storage Class
	vector< HierarchicalNodeData > m_HierarchicalNodeData;
	IController	*		m_RootController;
	CHashString			m_CoreModel;
	IRenderer *			m_IRenderer;
	CHModelManager *	m_HierarchicalModelManager;
	HierarchicalNode *	m_pCachedRootNode;

	HierarchicalModelType *m_pHierType;

	float			m_AnimTime;
	float			m_MaxAnimTime;
	float			m_AnimSpeed;
	bool			m_bFirstUpdate;
	bool			m_bPlaying;
	bool			m_bAnimLoop;
	bool			m_bFreezeLastFrame;
	bool			m_bUseGlobalOffsetTransforms; //activated when physics is active
												  //offset transforms are treated as global
	bool m_bAddToHierarchy;
	
	Vec3 SerializedPosition;
	Vec3 SerializedScaling;
	EulerAngle SerializedRotation;
private:
	CHashString m_hierAnimTypeName;
	void CheckDirtyTransform();
	__forceinline void BuildMat( Matrix4x4 &m, const EulerAngle euler, const Vec3 scale, const Vec3 pos )
	{
		m.SetRotation( euler );
		m.SetTranslation( pos );
		m.m[0] *= scale.x;
		m.m[1] *= scale.x;
		m.m[2] *= scale.x;
			
		m.m[4] *= scale.y;
		m.m[5] *= scale.y;
		m.m[6] *= scale.y;
			
		m.m[8] *= scale.z;
		m.m[9] *= scale.z;
		m.m[10] *= scale.z;
	}

	bool m_bEnablePhysicsAnimation;
};

#endif