///==========================================================================
/// \file	CParticleRenderObject.h
/// \brief	Implementation of CParticleRenderObject interface for rendering
/// \date	3/9/2005
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
///==========================================================================

#ifndef	_CParticleRenderObject_H_
#define _CParticleRenderObject_H_

using namespace std;

#define PARTICLE_ROTATION_RESOLUTION 360
extern UINT MAX_PARTICLE_COUNT;

namespace ElementalEngine 
{

class CParticleEmitter;
//over life variations
enum PARTICLE_KEYFRAME_VARIATIONS { 
	PK_VELOCITY_VARY = 0,//	PK_MOTION_RAND_VARY is calculated into this and spin and weight varies
	PK_SIZE_VARY,
	PK_SPIN_VARY,
	PK_WEIGHT_VARY,
	PK_LIFE_VARY, //this is max life
	PK_VARIATIONS_MAX,
};

enum PARTICLE_KEYFRAME_LIFE { 
	PK_VELOCITY_LIFE = 0,
	PK_SIZE_LIFE,
	PK_MOTION_RAND_LIFE,
	PK_SPIN_LIFE,
	PK_WEIGHT_LIFE,
	PK_COLOR_R_LIFE,
	PK_COLOR_G_LIFE,
	PK_COLOR_B_LIFE,
	PK_COLOR_A_LIFE,
	PK_INSTANCE_MAX,
};

struct PARTICLE
{
	float x;
	float y;
	float z;
	float velocityX;
	float velocityY;
	float velocityZ;
	float weightvelocity;
	float curlife;
	int	currentkeyframe[ PK_INSTANCE_MAX ];
	float variations[ PK_VARIATIONS_MAX ];
	//fps
	UINT  curframe;
	float frametimer; //subtract to 0, increment curframe, reset to max, just for speed so no divides
	float frametimermax; //
	bool bRandomInitialFrame;
};

struct PARTICLE_VERTEX
{
	Vec3 Position;
	float TexCoord[2];
	Vec3 PosOffset;
	Vec4 Rotation;
	DWORD Color;
};

struct PARTICLE_KEYINSTANCES
{
	int					m_KeyFrames[ PARTICLE_VARIABLE_MAX ];
	float				m_UpdatedVariations[ PARTICLE_VARIABLE_MAX ];
	int					m_VerticesWritten;
	int					m_CurOffset;
	float				m_NumParticles;
	PARTICLE_KEYINSTANCES( )
	{
		m_VerticesWritten = 0;
		m_NumParticles = 0;
		m_CurOffset = 0;
		memset( m_KeyFrames, 0, sizeof( m_KeyFrames )  );
		memset( m_UpdatedVariations, 0, sizeof( m_UpdatedVariations )  );
	}
};

///==========================================================================
/// \class	CParticleRenderObject
/// \brief	Adapter for Vision Render objects
///==========================================================================
class CParticleRenderObject: public CRenderObject<>
{
protected:
	/// \brief The Default Constructor for CParticleRenderObject
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CParticleRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier);

public:
	/// \brief	The Default Destructor for CParticleRenderObject
	~CParticleRenderObject();

	/// \brief	Create the a instance of IObject
	/// \return	IComponent pointer
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Initialization for the Object
	/// \param	IManager* pManager: Parent Manager
	virtual void Init();
	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update( );
	/// \brief	The DeInit Function for the Object
	virtual void DeInit();
	
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
	virtual DWORD GetRenderPriority(){ return 99; }

	/// \brief Sets the position of the render object
	/// \param position Vec3 class reference to fill in
	virtual void SetPosition( const Vec3 &vec );

	/// \brief Sets the rotation/orientatino of the render object
	/// \param x = x rotation angle
	/// \param y = y rotation angle
	/// \param z = z rotation angle
	virtual void SetRotation( const EulerAngle &vec );

	/// \brief Sets the scaling of the render object
	/// \param x = x scale
	/// \param y = y scale
	/// \param z = z scale
	virtual void SetScaling( const Vec3 &vec );

	/// \brief Gets the position of the render object
	/// \return Vec3 = position f the object
	virtual Vec3 &GetPosition();

	/// \brief Gets bounding sphere of object. This is a 3D interface call that should be implemented for only
	///			3D objects. Might want to seperate more of this later
	/// \param position = vector that will be filled the center of the sphere
	/// \param radius = float that will be filled with the radius of the sphere
	virtual void GetBoundingSphere( Vec3 &position, float &radius );

	/// \brief vision implementation, finds out if this object has alpha
	/// and needs to be put in the alpha queue
	virtual bool IsAlphaObject();
	
	/// \brief either reads or writes the entity information to a file
	/// Currently Loads up the entity completely if reading(inside vision too)
	virtual void Serialize(IArchive &ar);

	/// \brief Gets the base sort class which implements IRenderObject::IsLessThan()
	/// child classes must not re-implement IsLessThan() if they return the BaseSortClass hash!!
	/// typically, each render object has a IsLessThan() and returns a unique BaseSortClass
	virtual IHashString * GetBaseSortClass();

	virtual IMeshObject * GetMeshObject();

	/// Gets the number of Geometry elements this renderobject possesses or if it uses geometry at all	
	virtual UINT GetNumGeometry(){ return 0; }

	/// Grabs the geometry pieces of the render object if there is any uses it for render instead
	/// of the render object render(might need to be changed)
	virtual IGeometry* GetGeometry( int index ){ return NULL; }

	bool IsKindOf( IHashString * compType );

	/// \brief GetBoundingBox grabs the min and max vertices
	/// child should extend off this
	virtual bool	GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation );

	virtual OBJECTSORTTYPES GetSceneSortID() const { return RENDEROBJECT3D; }

	DWORD OnEnable(DWORD size, void *params);
	DWORD OnDisable(DWORD size, void *params);
	DWORD OnSetParticleTime(DWORD size, void *params);
	DWORD OnSetParticleInitialVelocity(DWORD size, void *params);
	DWORD OnResetParticle(DWORD size, void *params);
	DWORD OnGetParticleEmitterInterface(DWORD size, void *params);
	DWORD OnReloadTextures(DWORD size, void *params);

	int UpdateBuffer( char ** pDest, int offset, IVertexBufferObject * vb);
	virtual void Enable( bool enable )
	{
		m_bEnabled = enable;
	}
	virtual void SetClipDistance( float distance )
	{
		m_ClipDistance = distance;
	}
	
	virtual	bool SetCurrentLife( float life )
	{
		if( m_RemainingLife > 0 && ((m_MaxLife - life) > 0))
		{
			m_RemainingLife = m_MaxLife - life;
			return true;
		}
		return false;
	}
	virtual void SetScreenAligned( bool Enabled )
	{
		m_ScreenAligned = Enabled;
	}
private:
	void RenderToBuffer(int brushindex, int &vertoffset);
	float GetTime();
	/// \brief the Position of the center of entity, not the same as
	/// vision's entity position
	Vec3 m_Position;
	/// \brief the Rotation of the vision entity
	EulerAngle m_Rotation;
	/// \brief the Scaling of the vision entity
	Vec3 m_Scaling;
	Vec3 m_LastPositionOffset;
	Vec3 m_LastPosition;

	int					m_nActiveParticles;

	bool				m_OldestFirst;
	bool				m_ScreenAligned;
	bool				m_bEnabled;
	bool				m_bInScene;
	bool				m_bBufferSet;
	bool				m_bFirstUpdate;
	bool				m_bStartEnabled;
	int					m_VBOffset;
	float				m_ClipDistance;
	float				m_RemainingLife;
	float				m_MaxLife;
	float				m_TimeDelta;
	bool				m_LastPositionSet;
	IRenderer *			m_IRenderer;
	IVertexBufferObject *	m_VB;
	Matrix4x4			m_mat4ParentTransform;
	CHashString			m_ParticleTypeName;
	StdString			m_Filename;
	CParticleType *		CurParticleType;
	CParticleEmitter *  m_ParticleEmitter;
	PARTICLE_KEYINSTANCES * CurKeyInstance;
	int					m_EmitterKeyFrames[ EMITTER_VARIABLE_MAX ];
	float				m_EmitterUpdatedVariations[ EMITTER_VARIABLE_MAX ];

	// typename for Camera object for messages
	vector< PARTICLE_KEYINSTANCES >				m_KeyInstances;
	vector< list<PARTICLE> >	m_Particles;

	bool m_bAddToHierarchy;

	static const int RANDOM_VALUE_COUNT = 256; // must be power of two
	static const int RANDOM_VALUE_MASK = RANDOM_VALUE_COUNT-1;
	static float m_vRandomValues[RANDOM_VALUE_COUNT];
	static int m_iRandomValueIndex;

	static __forceinline float GetRandomValue()
	{
		m_iRandomValueIndex = (m_iRandomValueIndex+1) & RANDOM_VALUE_MASK;
		return m_vRandomValues[m_iRandomValueIndex];
	}
	static __forceinline float GetRandomValue(float fAverage, float fVariation)
	{
		float fMul = fAverage + fVariation*GetRandomValue();
		return fMul;
	}
	PARTICLE_VERTEX	*	m_ParticleBuffer;//6 verts per face

	IEffect *m_pEffect;
	Vec3 m_InitialVelocity;

	inline void  SETCURRENTKEY(  int &key, float timevar, int var );
	inline float  KEYFRAME_INTERPOLATE(  int &key, float time, int var );
	inline void  SETCURRENTKEY_ANDINTERPOLATE( float &out, int &key, float timevar, int var );
	inline void  UPDATE_VARIABLE( float timevar, int var );
	inline void  UPDATE_EMITTER_VARIABLE( float timevar, int var );

	//
	//	CEntity Data and Functions
	//
	private:
	bool				m_bDefaultAttach;
	/// \brief Specifies the Types the Entity fall into
	StdString			m_wszEntityTypes;
	float				m_fBoundingRadius;
	
	/// Spacial Data
	CTransform			m_LocalTransform;
	CTransform			m_GlobalTransform;
	void				UpdateTransform();
//	void				SetChildrenDirtyTransformFlags();
	public:
	DWORD	OnSetGlobalPosition( DWORD size, void* params );
	DWORD	OnSetGlobalEuler( DWORD size, void* params );
	DWORD	OnSetGlobalQuaternion( DWORD size, void* params );
	DWORD	OnSetGlobalScale( DWORD size, void* params );
	DWORD	OnSetGlobalTransform( DWORD size, void* params );

	DWORD	OnSetLocalPosition( DWORD size, void* params );
	DWORD	OnSetLocalEuler( DWORD size, void* params );
	DWORD	OnSetLocalQuaternion( DWORD size, void* params );
	DWORD	OnSetLocalScale( DWORD size, void* params );
	DWORD	OnSetLocalTransform( DWORD size, void* params );

	DWORD	OnAttachSceneObject(DWORD size, void *param);	

};

}//namespace elemental engine

#endif	// CParticleRenderObjectOBJECT_HPP
