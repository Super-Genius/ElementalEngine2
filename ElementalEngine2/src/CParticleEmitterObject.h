///==========================================================================
/// \file	CParticleEmitterObject.h
/// \brief	Implementation of CParticleEmitterObject Render Object
/// \date	12/05/2007
/// \author	Brendan Geiger
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

#ifndef	_CParticleEmitterRenderObject_H_
#define _CParticleEmitterRenderObject_H_

#include "CRenderObject.h"
#include "IMeshObject.h"
#include "CParticleEmitterType.h"

using namespace std;
extern UINT MAX_PARTICLE_COUNT;

namespace ElementalEngine 
{

enum SCALE_MODE
{
	SCALENORMAL = 0,	// Scales Velocity, Weight, & Size and Volume
	SCALEVOLUME,		// Scales Number, Volume/Area
	SCALEPARTICLE,		// Scales Velocity, Weight, & Size
	INVALID_MODE
};

struct SParticle
{
	bool active;
	bool firstframe;

	Vec3 pos;							//w
	Vec3 origin;						//w
	Vec3 down;							//w
	EulerAngle rot;						//n - Rotation.x for spin.
	Vec3 vel;							//w

	float TexCoord[2];					//w - Shader Later
	bool bAnimTexRandomInitialFrame;	//move out to Emitter...
	UINT  AnimTexCurFrame;				//w
	float AnimTexFrameTimer;			//n -
	float AnimTexFrameTimerMax;			//move out to Emitter...

	float Color[4];						//w - Shader Later

	float maxlife;
	float midlife;
	float life;					
	float slowdown;

	SParticle( )
	{
		active = false;
		Color[0] = 1.0f;
		Color[1] = 1.0f;
		Color[2] = 1.0f;
		Color[3] = 1.0f;
		slowdown = 0.0f;
	}
};


struct PARTICLEEMITTER_VERTEX
{
	Vec3 Position;
	float TexCoord[2];
	Vec3 PosOffset;
	Vec4 Life;
	DWORD Color;
};

///==========================================================================
/// \class	CParticleEmitterObject
/// \brief	Adapter for Vision Render objects
///==========================================================================
class CParticleEmitterObject: public CRenderObject<>
{
protected:
	/// \brief The Default Constructor for CParticleEmitterObject
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CParticleEmitterObject(IHashString *parentName, IHashString *name, bool bAddToHier);

	/// \brief Resets Operating Values that are reset on Enables and Init.
	void ResetOperatingValues();
	
	/// \brief Returns SCALE_MODE enum of string for Serialization.
	SCALE_MODE GetScaleModeFromString(const char *mode) const;

	/// \brief Gets String Value of enum SCALE_MODE for Serialization.
	void GetStringFromScaleMode(StdString &string, const SCALE_MODE mode);

	/// \brief Updates Particles and writes them to a temp buffer
	int UpdateParticles();

	/// \brief Creates and Initializes data for a single particle.
	inline void AddParticle( SParticle * particles, int &index, float posx, float posy, float posz, float velx, float vely, float velz, float yaw, float pitch, float roll);

	/// \brief Spawns particles in an area and provides initial velocity based on Emitter Type.
	unsigned int GenerateNewParticles( SParticle * particles, int index, PARTICLE_TYPES type, float l, float w, float h, float outv, float altv, float fnumber);
	
	/// \brief Sends Message to OS to get Time.
	float GetTime();


public:
	/// \brief	The Default Destructor for CParticleEmitterObject
	~CParticleEmitterObject();

	/// \brief	Create the a instance of IObject
	/// \return	IComponent pointer
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Initialization for the Object
	/// \param	IManager* pManager: Parent Manager
	virtual void Init( );
	/// \brief	The Update Function for the Buffer
	/// \return	True on sucess, False on faulure
	int UpdateBuffer( char ** pDest, int offset, IVertexBufferObject * vb, IIndexBuffer *ib );
	

	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on failure
	virtual bool Update( );
	/// \brief	The DeInit Function for the Object
	virtual void DeInit( );

	virtual OBJECTSORTTYPES GetSceneSortID() const { return RENDEROBJECT3D; }

	/// \brief Render the renderobject
	/// \return current particle index offset. 
	virtual bool Render( UINT pass, IEffect * override );

	/// \brief Render the Particle Emitter
	/// \used by CParticleSystemObject. TODO MOVE TO PARTICLE BASE CLASS AS VIRTUAL
	bool RenderEmitter( UINT pass, IEffect * override );

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

	/// \brief MSG To Enable Emitter
	/// \params size - 0
	/// \params params - null
	/// \return - MSG_XXX
	DWORD OnEnable(DWORD size, void *params);
	/// \brief MSG To Disable Emitter
	/// \params size - 0
	/// \params params - null
	/// \return - MSG_XXX
	DWORD OnDisable(DWORD size, void *params);
	/// \brief MSG To Pulse Emitter to time of Greatest Number
	/// \params size - 0
	/// \params params - null
	/// \return - MSG_XXX
	DWORD OnPulse(DWORD size, void *params);
	/// \brief MSG To Reset Emitter to 0 Time
	/// \params size - 0
	/// \params params - null
	/// \return - MSG_XXX
	DWORD OnReset(DWORD size, void *params);

	void SetDirtyTransformFlag()
	{
		m_bTransformIsDirty = true;
		SetAttachmentObjectsDirty();
	}
	float drand();

	bool IsInScene( void )
	{
		return m_bInScene;
	}

	virtual void Enable( bool enable )
	{
		m_bEnabled = enable;
	}
	virtual void SetClipDistance( float distance )
	{
		m_ClipDistance = distance;
	}
	
	/*
	virtual void SetScreenAligned( bool Enabled )
	{
		m_pEmitterProperties->GetScreenAligned();
	}
	*/
	CParticleEmitterType * GetParticleEmitterData();
private:
	PARTICLE_TYPES m_typeID;

	IEffect *m_pEffect;

	Vec3 m_LastPositionOffset;
	Vec3 m_LastPosition;
	Vec3 m_v3BoundaryCenterPoint;

	/// our type's type name
	CHashString m_hszTypeTypeName;
	/// name of our type
	CHashString m_hszFilePathName;
	CHashString m_hszCEntity;
	CHashString m_hszCParticleSystemObject;

	PARTICLEEMITTER_VERTEX	*	m_ParticleBuffer;//6 verts per face
//	Vec3 m_InitialVelocity;

	// Position Data.
	Vec3 m_v3Position;
	EulerAngle m_eulRotation;

	// Scale Value / Mode
	SCALE_MODE			m_ScaleMode;
	Vec3				m_v3Scaling;
	float				m_fVolumeScale;

	// Boundary Volume Calculation
	Vec3 m_v3LastDeletedParticle;	// Used for Fast Boundary Sphere Calculation.
	float				m_fBoundingRadius;
	float				m_fBoundaryGuessRadius;

	// Emitter Properties
	bool				m_bInit;
	// Serialize - Flag for Particle starting Enabled.
	bool				m_bStartEnabled;
//	bool				m_bScreenAligned;
//	ITextureObject *	m_Texture;
	// Serialize - MaxLife of Particle
	float				m_MaxLife;
//	float				m_fAttachToEmitter;
	CParticleEmitterType  *m_pEmitterProperties;
	unsigned int		m_iParticleIndex;
	// Op - Bounding Sphere Radius, Render cull value. TODO: calc dynamically, set to 1000.
	float				m_ClipDistance;


	// Process Properties
	// Op - Flag for Enabled Particle.
	bool				m_bEnabled;
	// Op - Flag for Particle with Particle System parent.
	bool				m_bParticleSystemControlled;
	// Op - Flag for Particle in Scene for render culling.
	bool				m_bInScene;
	// Op - Flag for first Update after an Enable.
	bool				m_bFirstUpdate;	
	// Op - Flag for first frame Last Position Init: false->true;
//	bool				m_LastPositionSet;
	// Op - Flag for Add to Hierarchy for Destructor so it removes.
	bool				m_bAddToHierarchy;
	// Op - Float number. Particles are generated fractionally due to (timestep * number) being less than zero.
	//      this value stores the fractional value across frames so it can accumulate. it is decremented
	//		when new particles are created.
	float				m_nParticleNumberAccumulation;
	// Op - The Max Number of Particles Spawned by a system.
	UINT				m_nMaxParticles;
	// Op - Number of Spawned Particles, Inc on Activate, Dec on DeActivate. Will be used for performance tracking.
	UINT				m_nParticles;
	// Op - Time remainging, counts down. (Seconds)
	float				m_RemainingLife;
	// Op - Time alive, counts up. (Seconds)
	float				m_Life;
	// Op - Time in frame. (Seconds)
	float				m_TimeDeltaSeconds;

	IRenderer *			m_IRenderer;
	MathUtil			m_Math;
	SParticle			*m_pParticles;
	IVertexBufferObject *	m_VB;
	IIndexBuffer		*m_IB;
	int					m_VBOffset;
	int					m_VerticesWritten;
	Matrix4x4			m_mat4ParentTransform;
	CHashString			m_ParticleTypeName;
	StdString			m_Filename;

	//
	//	CEntity Data and Functions
	//
	private:
	/// \brief Specifies the Types the Entity fall into
	StdString			m_wszEntityTypes;

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

#endif	// _CParticleEmitterRenderObject_H_
