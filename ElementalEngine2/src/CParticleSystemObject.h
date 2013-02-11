///==========================================================================
/// \file	CParticleSystemObject.h
/// \brief	Implementation of CParticleSystemObject
/// \date	2/27/2008
/// \author Brendan Geiger
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
/// CParticleSystemObject is designed to be a parent object to Particle Objects to
/// allow for several particle emitters to work together to create special effects.
/// A. One example of this would be glowing points spiraling around a central point. 
/// This would be achieved by keyframed animation on the part of the System with
/// several child emitters.
/// B. Another Usage would be activating a particle emitter Y at the death of a 
/// particle in X. This can be used for sparks for example.
/// C. Particles can be slaved to particles from other emitters. an example of this would
/// be ribbons following sparks.
/// Links will allow communication between particles using a standard interface and are
/// managed by parent CParticleSystemObjects.
/// CParticleEmitterObjects, CParticleRibbonObjects, and CParticleLineObjects will remain
/// stand alone.

/* TODO:
	1. Make the CParticleEmitterObject * in PARTICLEEMITTERVECTOR to be a base class for all particle types.
	2. Move Particle stuff into a plugin. When this is done move message structs to a seperate define.
*/

#define PARTICLEEMITTERVECTOR vector<CParticleEmitterObject *>

#ifndef CPARTICLESYSTEMOBJECT_H
#define CPARTICLESYSTEMOBJECT_H

class CParticleSystemObject : public CSceneObject<IRenderObject>
{
public:
	/// \brief The Default Constructor for CParticleSystemObject
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CParticleSystemObject( IHashString *parentName, IHashString *name );

	/// \brief The Default Destructor for CParticleSystemObject
	~CParticleSystemObject();

	/// \brief	Initialize the object
	void Init();

	/// \brief Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar ){}


	static IComponent *Create(int nArgs, va_list argptr)
	{
		IObject *self;
		IHashString *name = va_arg(argptr, IHashString *);
		IHashString *parentName = va_arg(argptr, IHashString *);
		try
		{
			// try and create an instance
			self = new CParticleSystemObject(parentName, name);
		}
		catch(...)
		{
			return NULL;
		}

		return self;
	}

	/// \fn		virtual bool Update()
	/// \brief	Update the object
	bool Update();

	/// \fn		virtual void DeInit()
	/// \brief	Deinitialize the object
	void DeInit(){ }
	/// \brief Get Scene Sort ID
	/// \return - OBJECTSORTTYPES, RENDEROBJECT3D
	virtual OBJECTSORTTYPES GetSceneSortID() const { return RENDEROBJECT3D; }
	/// \brief Render the renderobject
	/// \params pass - RenderPass
	/// \params override - IEffect Override for child Particles
	/// \return true
	bool Render( UINT pass, IEffect * override );
	/// \brief Get Base Sort Class
	/// \return - IHashString to Component Type
	virtual IHashString * GetBaseSortClass(){ return GetComponentType(); }
	virtual bool IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass ) 
		{ return false; }
	/// \brief Get Render Priority
	/// \return - DWORD, 99
	virtual DWORD GetRenderPriority(void) { return 99; }
	/// \brief Get Render Priority
	/// \return - UINT, 1
	virtual UINT GetNumPass() { return 1; };
	/// \brief Get Alpha Object Flag
	/// \return - bool, true
	virtual bool IsAlphaObject(){ return true; }
	/// Get Bounding Sphere, parses through child particles for their bounding spheres.
	/// \param pos - Vec3 Reference to Position
	/// \param radius - Float Reference to Raidus
	/// \return - bool, success
	virtual void GetBoundingSphere( Vec3 &pos, float &radius );
	/// Get Bounding Box, parses through child particles for their bounding boxes.
	/// \param position - Vec3 Reference to Position
	/// \param dim - Vec3 Reference to Dimensions
	/// \param rotation - Vec3 Reference to Rotation
	/// \return - bool, success
	virtual bool GetBoundingBox( Vec3 &position, Vec3 &dim, EulerAngle &rotation );
	/// UNUSED BUT REQUIRED BY IRenderObject
	virtual UINT GetNumGeometry(){ return 0; }
	/// UNUSED BUT REQUIRED BY IRenderObject
	virtual IGeometry* GetGeometry( int index ){ return NULL; }
	/// UNUSED BUT REQUIRED BY IRenderObject
	virtual IMeshObject * GetMeshObject(){ return NULL; }
	/// UNUSED BUT REQUIRED BY IRenderObject
	virtual void AttachLight(ILightObject *) {}
	/// UNUSED BUT REQUIRED BY IRenderObject
	virtual void DetachAllLights( ) {}
	/// UNUSED BUT REQUIRED BY IRenderObject
	virtual ISceneSettings * GetSceneSettings(){ return NULL; }
	/// UNUSED BUT REQUIRED BY IRenderObject
	virtual UINT GetNumMaterial(){ return 0; }
	/// UNUSED BUT REQUIRED BY IRenderObject
	virtual IMaterial* GetMaterial( int index ) { return NULL; }
	/// UNUSED BUT REQUIRED BY IRenderObject
	IEffect * LoadEffect(IHashString * name);

	void GetAttachTransform(IHashString *hsNodeName, Matrix4x4 &attachXForm);

	void UpdateTransform();
	//// Particle System Function:
	//	bool RegisterLink( IHashString * hszParticleLink ){}
	//	bool DeRegisterLink( IHashString * hszParticleLink ){}


	//// Particle System Message Functions:
	DWORD OnRegisterChildEmitter( DWORD size, void *param); 
	DWORD OnDeRegisterChildEmitter( DWORD size, void *param);

	DWORD OnAttachSceneObject(DWORD size, void *param);	
	DWORD OnAddChildObject(DWORD size, void *param);
	DWORD OnRemoveChildObject(DWORD size, void *param);

	DWORD OnSetGlobalPosition( DWORD size, void* params );
private:
	//// Particle System Data
	PARTICLEEMITTERVECTOR	m_ParticleEmitters;
	bool					m_bAddToHierarchy;
	bool					m_bQueueSelfDelete;
	/// IRenderObject
	IRenderer *			m_IRenderer;
	float				m_ClipDistance;
	/// Spacial Data
	Vec3				m_v3Position;
	Vec3				m_v3LastPosition;
	EulerAngle			m_eulRotation;
	CTransform			m_LocalTransform;
	CTransform			m_GlobalTransform;
	Matrix4x4			m_mat4ParentTransform;

//	IDTOOBJECTMAP m_ParticleEmitters;
//	IDTOOBJECTMAP m_ParticleLinks;
//	CParticleSystemLinkMap m_ParticleSystemLinkMap;
	//TODO: Map of Particle Links to other Particles and all the links involved ParticleLinksToEmitters;
	//
};
#endif //#ifndef CPARTICLESYSTEMOBJECT_H