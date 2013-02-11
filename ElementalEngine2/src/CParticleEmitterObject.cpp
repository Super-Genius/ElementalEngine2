///============================================================================
/// \file	CParticleEmitterObject.cpp
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

#include "Stdafx.h"

REGISTER_COMPONENT(CParticleEmitterObject);

REGISTER_MESSAGE_HANDLER(Disable, OnDisable, CParticleEmitterObject );
REGISTER_MESSAGE_HANDLER(Enable, OnEnable, CParticleEmitterObject );
REGISTER_MESSAGE_HANDLER(Reset, OnReset, CParticleEmitterObject );
REGISTER_MESSAGE_HANDLER(Pulse, OnPulse, CParticleEmitterObject );

float CParticleEmitterObject::drand()
{
	return (float)(rand()%1000)/1000.0f;
}

///
/// Constructor / Destructor
///
CParticleEmitterObject::CParticleEmitterObject(IHashString *parentName, IHashString *name, bool bAddToHier) : 
		CRenderObject<>( _T("CParticleManager"),_T("CParticleEmitterObject"), parentName, name ),
		m_hszTypeTypeName(_T("CParticleEmitterType")),
		m_hszCEntity(_T("CEntity")),
		m_hszCParticleSystemObject(_T("CParticleSystemObject"))
{

	m_ParticleBuffer =  NULL;
	m_IRenderer = NULL;
	m_pEffect = NULL;

	m_bEnabled = false;
	m_bInit = false;
	m_bParticleSystemControlled = false;

	m_bAddToHierarchy = bAddToHier;	
	if (m_bAddToHierarchy)
	{
		AddToHierarchy();
	}

	m_bInScene = false;
//	m_bScreenAligned = false;
	m_bStartEnabled = false;

	ResetOperatingValues();
	m_nParticleNumberAccumulation = 0.0f;
	m_RemainingLife = -1.0f;	// Set to non-op value;
	m_MaxLife = 1.0f;			// Set to non-op value;

	m_nParticles = 0;
	m_Filename = "";

	m_VBOffset = 0;
	m_VerticesWritten = 0;

	m_LastPosition.Set( 0,0,0);
	m_LastPositionOffset.Set( 0,0,0 );
	m_ClipDistance = 1000; // TODO: MAKE DYNAMIC
	m_pEmitterProperties = NULL;
	m_pParticles = NULL;
	m_iParticleIndex  = 0;

	//
	// CEntity Data
	//

	m_wszEntityTypes = "";
	m_bTransformIsDirty = true;
	m_fBoundingRadius = 0.f;
	m_fBoundaryGuessRadius = 0.f;

	if (parentName)
	{
		static DWORD msgHash_GetComponentType = CHashString(_T("GetComponentType")).GetUniqueID();
		GETCOMPONENTTYPEPARAMS gctp;
		gctp.name = parentName;
		m_ToolBox->SendMessage(msgHash_GetComponentType, sizeof(gctp), &gctp);			

		static DWORD dEntity = CHashString(_T("CEntity")).GetUniqueID();
		if ((gctp.componenttype != NULL) && (gctp.componenttype->GetUniqueID() == dEntity))
		{
			// if the parent is an CEntity, attach to it!
			ATTACHSCENEOBJECTPARAMS asop;
			asop.hsAttachmentObject = name;
			asop.hsNodeAttachName = NULL;

			static DWORD msgAttachSceneObject = CHashString(_T("AttachSceneObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgAttachSceneObject, sizeof(ATTACHSCENEOBJECTPARAMS), &asop, parentName);			
		}
		static DWORD dCParticleSystemObject = CHashString(_T("CParticleSystemObject")).GetUniqueID();
		if ((gctp.componenttype != NULL) && (gctp.componenttype->GetUniqueID() == dCParticleSystemObject))
		{
			// if the parent is an CParticleSystemObject, attach to it!
			ATTACHSCENEOBJECTPARAMS asop;
			asop.hsAttachmentObject = name;
			asop.hsNodeAttachName = NULL;
			
			// Because of the message system defaults to CEntity type when none is specified, 
			// I need to resend the message to AddChildObject to CParticleSystemObject specifically.
			ISceneObject *pObject = this;
			static DWORD msgHash_AddChildObject = CHashString(_T("AddChildObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_AddChildObject, sizeof(ISceneObject*), &pObject, parentName, &m_hszCParticleSystemObject);

			static DWORD msgAttachSceneObject = CHashString(_T("AttachSceneObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgAttachSceneObject, sizeof(ATTACHSCENEOBJECTPARAMS), &asop, parentName, &m_hszCParticleSystemObject);			

			CParticleEmitterObject *pEmitterObject = this;
			static DWORD msgRegisterChildEmitter = CHashString(_T("RegisterChildEmitter")).GetUniqueID();
			m_ToolBox->SendMessage(msgRegisterChildEmitter, sizeof(CParticleEmitterObject *), &pEmitterObject, parentName, &m_hszCParticleSystemObject);			

			m_bParticleSystemControlled = true;
		}
	}
}

CParticleEmitterObject::~CParticleEmitterObject()
{
	//remove from scene manager
	RemoveBoundingObject();

	ISceneObject *pObject = this;
	static DWORD msgHash_RemoveChildObject = CHashString(_T("RemoveChildObject")).GetUniqueID();
	static CHashString psoName(_T("CParticleSystemObject"));
	if (m_bParticleSystemControlled)
	{
		EngineGetToolBox()->SendMessage(msgHash_RemoveChildObject, sizeof(ISceneObject*), &pObject, GetParentName(), &psoName);
	}
	else
		EngineGetToolBox()->SendMessage(msgHash_RemoveChildObject, sizeof(ISceneObject*), &pObject, GetParentName());

	DeInit();

	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
	SAFE_DELETE_ARRAY( m_pParticles );
	SAFE_DELETE_ARRAY( m_ParticleBuffer );
}

void CParticleEmitterObject::ResetOperatingValues()
{
	m_bFirstUpdate = true;
	m_Life = 0.0f;
	m_RemainingLife = m_MaxLife;
}

/// CREATE>SERIALIZE>INIT
/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CParticleEmitterObject::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool bAddToHier;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new CParticleEmitterObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CParticleEmitterObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

SCALE_MODE CParticleEmitterObject::GetScaleModeFromString(const char *mode) const
{
	// Mode
	static const char * szModeArray[] =
			{"SCALENORMAL", 
			"SCALEVOLUME",
			"SCALEPARTICLE",
			NULL};
	int i = 0;
	while (szModeArray[i] != NULL)
	{
			//_tcscmp
		if (0 == strcmp(szModeArray[i], mode))
			return (SCALE_MODE)i;
		i++;
	}
	return INVALID_MODE;
}

void CParticleEmitterObject::GetStringFromScaleMode(StdString &string, const SCALE_MODE mode)
{
	static const char * szModeArray[] =
			{"SCALENORMAL", 
			"SCALEVOLUME",
			"SCALEPARTICLE",
			NULL};

	int i = 0;
	while (szModeArray[i] != NULL)
		i++;
	if ((int)mode > i)
		string = "INVALID_MODE"; 
	else
		string = szModeArray[(int)mode]; 
	return;
}

/// \brief either reads or writes the entity information to a file
/// Currently Loads up the entity completely if reading(inside vision too)
void CParticleEmitterObject::Serialize(IArchive &ar)
{
	StdString version = "0.0";
	StdString name = "";

	if ( &ar == NULL )
	{
		m_ToolBox->Log(LOGERROR, _T("(%s)ParticleEmitterObject::Serialize - Archive NULL! \"%s\"\n"), this->GetName()->GetString(),	version);
		return;
	}
	if (ar.IsReading())
	{
		m_ParticleTypeName.Init( name );
		ar.Read( version );
		if ( version != _T("1.0")) // || whatever versions are to follow...
		{
			m_ToolBox->Log(LOGERROR, _T("(%s)ParticleEmitterObject::Serialize - Invalid Object Version! \"%s\"\n"), this->GetName()->GetString(),	version);
			ar.Close();
			return;
		}
		ar.Read( m_Filename );
		m_hszFilePathName.Init(m_Filename);

		ar.Read( m_v3Position );
		m_LocalTransform.SetTranslation( m_v3Position );
		ar.Read( m_eulRotation );
		m_LocalTransform.SetEuler( m_eulRotation );
		ar.Read( m_v3Scaling );
		m_LocalTransform.SetScale( m_v3Scaling );

		m_mat4ParentTransform.SetFrom4x4(m_LocalTransform.GetTransform().m);

		ar.Read( m_bEnabled );
		m_bStartEnabled = m_bEnabled; // TODO: Remove Start Enabled Logic.
		StdString TempScaleMode;
		ar.Read( TempScaleMode  );
		m_ScaleMode = GetScaleModeFromString(TempScaleMode);
		ar.Read( m_MaxLife );
		m_RemainingLife = m_MaxLife;
	}
	else
	{
//		ar.Write( m_ParticleTypeName.GetString(), _T("name") );
		ar.Write( _T("1.0"),  _T("Version"));
		ar.Write( m_Filename, _T("FileName") );
		ar.Write( m_LocalTransform.GetTranslation(), _T("Position"));
		ar.Write( m_LocalTransform.GetEuler() , _T("Rotation") );
		ar.Write( m_LocalTransform.GetScale() , _T("Scaling") );		
		ar.Write( m_bEnabled , _T("Enabled"));
		StdString TempScaleMode;
		GetStringFromScaleMode(TempScaleMode, m_ScaleMode);
		ar.Write( TempScaleMode, _T("ScaleMode"));
		ar.Write( m_MaxLife, _T("RemaingLife") );
	}

}

void CParticleEmitterObject::Init()
{
	//grab the renderer
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	renderInterfaceParam.m_RendererInterface = NULL;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam, NULL, NULL );
	m_IRenderer = renderInterfaceParam.m_RendererInterface;

	// Get the Manager
	CParticleManager *pParticleManager = dynamic_cast<CParticleManager*>(m_Manager);

	// Ask Manager for Pointer to Data. Manager will load if not loaded.
	m_pEmitterProperties = (CParticleEmitterType*)pParticleManager->LoadParticleType( &m_hszFilePathName, &m_hszTypeTypeName );
	pParticleManager->AddParticleEmitter( this );
	
	m_fBoundaryGuessRadius = m_pEmitterProperties->GetEmitterOutVelocity(0) * m_pEmitterProperties->GetEmitterLife(0); 

	// TODO: INIT / PREPROCESS KEYFRAMES IF THAT IS DONE
	// TODO: Improve IntegrateMaxNumParticles();
	m_nMaxParticles = m_pEmitterProperties->IntegrateMaxNumParticles( m_MaxLife );
//	m_ToolBox->Log(LOGINFORMATION, _T("Max Particles for %s = %d \n"), GetName()->GetString(), m_nMaxParticles);
	m_pParticles = new SParticle[ m_nMaxParticles ];
	m_iParticleIndex = 0;
	m_ParticleBuffer = new PARTICLEEMITTER_VERTEX[ m_nMaxParticles * 4 ];

	//set particle tex coords defaults, these never change unless an animated texture is used
	PARTICLEEMITTER_VERTEX *pParticle = m_ParticleBuffer;
	for( UINT i = 0; i < m_nMaxParticles; i++ )
	{
		pParticle[0].TexCoord[0] = 0;
		pParticle[0].TexCoord[1] = 0;
		pParticle[1].TexCoord[0] = 0;
		pParticle[1].TexCoord[1] = 1;
		pParticle[2].TexCoord[0] = 1;
		pParticle[2].TexCoord[1] = 0;
		pParticle[3].TexCoord[0] = 1;
		pParticle[3].TexCoord[1] = 1;

		pParticle[0].PosOffset.Set( 0, 1, 0 );
		pParticle[1].PosOffset.Set( 1, 0, 0 );
		pParticle[2].PosOffset.Set( -1, 0, 0 );
		pParticle[3].PosOffset.Set( 0, -1, 0 );

		pParticle += 4;
	}
	if (m_bEnabled)
		OnEnable(0,0);
	static CHashString pfxName(_T("Shaders/Particle.efx"));
	m_pEffect = CRenderObject<>::LoadEffect( &pfxName );

	//
	// CEntity Init
	//
	UpdateTransform();
	m_bInit = true;

}
void CParticleEmitterObject::DeInit()
{
	
}

//
// CRenderObject Functions
//
void CParticleEmitterObject::GetBoundingSphere( Vec3 &position, float &radius )
{	
	Vec3 v3temp;
	position = m_GlobalTransform.GetTranslation();
	position += m_v3LastDeletedParticle;
	position /= 2;
	v3temp = m_v3LastDeletedParticle;
	v3temp -= m_GlobalTransform.GetTranslation();
	radius = v3temp.Length();
	radius += m_fBoundaryGuessRadius;
}

/// \brief	Get the bounding box of the object
bool CParticleEmitterObject::GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation )
{
	Vec3 vMin, vMax;
	
	pos.x = m_v3Position.x;
	pos.y = m_v3Position.y;
	pos.z = m_v3Position.z;
	
	return false;
}

IHashString * CParticleEmitterObject::GetBaseSortClass()
{
	static CHashString CRO(_T("ParticleRenderObject"));
	return &CRO;
}

IMeshObject * CParticleEmitterObject::GetMeshObject()
{
	
	return NULL;
}

bool CParticleEmitterObject::IsAlphaObject()
{	
	return true;
}

bool CParticleEmitterObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{	
	//order for particles does not matter
	return ( false );
}

bool CParticleEmitterObject::IsKindOf( IHashString * compType )
{	
	static CHashString  CPROCOMP(_T("CParticleRenderObject") );
	if(CRenderObject<>::IsKindOf(compType)) 
	{
		return true;
	}
	return (compType->GetUniqueID() == CPROCOMP.GetUniqueID());
}

Vec3 &CParticleEmitterObject::GetPosition()
{
	return m_v3Position;
}

//
//	Position / Orientation / Scale Functions
//
/// \brief Set position of the render object
void CParticleEmitterObject::SetPosition( const Vec3 &vec )
{
	// update the render object position to any manager
	// that needs to know when it moves, i.e. the quad tree
	// manager
	m_v3Position.Set( vec );

	UpdateBoundingObject();
}

/// \brief Set Orientation of the render object
void CParticleEmitterObject::SetRotation( const EulerAngle &euler )
{	
	m_eulRotation.Set( euler );
	//m_RenderObject->SetOrientation( (VisVector_cl)m_Rotation);
}

/// \brief Set Scaling of the render object
void CParticleEmitterObject::SetScaling( const Vec3 &vec )
{
	m_v3Scaling.Set( vec );
	//m_RenderObject->SetScaling( (VisVector_cl)m_Scaling);
}

void CParticleEmitterObject::AddParticle( SParticle * particles, int &index, float posx, float posy, float posz, float velx, float vely, float velz, float yaw, float pitch, float roll)
{
	if (m_nParticles < m_nMaxParticles)
	{
		int iIndex = index; // Load-Hit-Store mitigation...
		iIndex = iIndex % m_nMaxParticles;
		while (particles[iIndex].active)
		{
			iIndex++;
			iIndex = iIndex % m_nMaxParticles;
		}
		SParticle &p = particles[iIndex];
		p.active = true;
		p.firstframe = true;
		p.pos.x = posx;
		p.pos.y = posy;
		p.pos.z = posz;
		p.vel.x = velx;
		p.vel.y = vely;
		p.vel.z = velz;
		p.down.x = 0;
		p.down.y = -1;
		p.down.z = 0;
		p.origin.x = posx;
		p.origin.y = posy;
		p.origin.z = posz;
		p.rot.yaw = yaw;
		p.rot.pitch = pitch;
		p.rot.roll = roll;

		CParticleEmitterType &EP = *m_pEmitterProperties;

		if (EP.GetRandomRotation())
			p.rot.roll = roll + drand() * PI * 2.0f;

		p.maxlife = EP.GetEmitterLife(m_Life);
		p.midlife = EP.GetEmitterMidLife(m_Life);
		p.life = 0.0f;
		p.slowdown = 0.0f; // // TODO: DEBUG OMG THE FIRE

		// convert rotations per second to rotations over life of the particle
		p.rot.roll *= p.maxlife;

		if (EP.GetTextureFPS() == 0)
		{
			p.bAnimTexRandomInitialFrame = true;
			p.AnimTexFrameTimer = p.AnimTexFrameTimerMax = 0;
			if (EP.GetAnimatedTexture())
			{
				IAnimatedTextureObject *at = (IAnimatedTextureObject*)EP.GetTexture();
				p.AnimTexCurFrame = (rand() % at->GetNumFrames());
			}
		}
		else
		{
			p.bAnimTexRandomInitialFrame = false;
			p.AnimTexFrameTimerMax = 1.f/EP.GetTextureFPS(); 
			p.AnimTexFrameTimer = p.AnimTexFrameTimerMax; //subtract to 0, increment curframe, reset to max, just for speed so no divides
		}
		m_nParticles++;
		index = iIndex;
	}
}

unsigned int CParticleEmitterObject::GenerateNewParticles( SParticle * particles, int index, PARTICLE_TYPES type, float l, float w, float h, float outv, float altv, float fnumber)
{
	// Do not try and spawn 0 particles or less particles.
	if (fnumber <= 0.0f)
		return 0;

	int iIndex = index;
	// Do not try and spawn more particles than we can show.
	if ((m_nParticles + (int)fnumber) >= m_nMaxParticles)
		fnumber = (float)(m_nMaxParticles - m_nParticles);

	static float DegToRad =  3.14159265f * (1.f/180.f);
	int number = 0;
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float radius = 0.0f;
	float velocity = 0.0f;
	float upVelocity = 0.0f;
	float outVelocity = 0.0f;
	float randVelocity = 0.0f;
	float inRadius = 0.0f;
	float outRadius = 0.0f;

	float arc = 0.0f;
	float phi = 0.0f;
	float theta = 0.0f;
	float sinPhi = 0.0f;

	CParticleManager &PM = *dynamic_cast<CParticleManager*>(m_Manager);

	Vec3 v3TempPos(0.0f, 0.0f, 0.0f);
	Vec3 v3TempVel(0.0f, 0.0f, 0.0f);

	float fInterpolationDistance = 0.0;
	Vec3 v3InterpolationVector(0.0f, 0.0f, 0.0f);

	switch (m_ScaleMode)
	{
		case SCALENORMAL:
		case SCALEVOLUME:
			l = m_v3Scaling.x * l;
			w = m_v3Scaling.z * w;
			h = m_v3Scaling.y * h;
			break;
		default:
			break;
	}

	switch(type) 
	{
		// Normal Point Particle. Particles Emit from a single point. 
		// Velocity is applied from the center out.
		case PT_POINT:
			m_fVolumeScale = 1.0f;
			velocity 	= outv;
			number = (int)(m_fVolumeScale * fnumber);

			for (int i = 0; i < number; i++)
			{
				v3TempVel = PM.GetRandomNormalVector();
				iIndex = iIndex + i;
				AddParticle(particles, iIndex, 
					m_v3Position.x, m_v3Position.y, m_v3Position.z,
					outv*v3TempVel.x, outv*v3TempVel.y, outv*v3TempVel.z,
					0.0, 0.0, 0.0);
			}
			break;
		
		case FAST_POINT:
			m_fVolumeScale = 1.0f;
			velocity 	= outv;
			number = (int)(m_fVolumeScale * fnumber);
			fInterpolationDistance = m_LastPositionOffset.Length() / number;
			v3InterpolationVector.Set(m_LastPositionOffset);
			v3InterpolationVector.Invert();
			v3InterpolationVector.SetLength(fInterpolationDistance);
			v3TempPos.Set(m_v3Position);
			for (int i = 0; i < number; i++)
			{
				v3TempPos += v3InterpolationVector;
				v3TempVel = PM.GetRandomNormalVector();

				iIndex = iIndex + i;
				AddParticle(particles, iIndex, 
					v3TempPos.x, v3TempPos.y, v3TempPos.z,
					outv*v3TempVel.x, outv*v3TempVel.y, outv*v3TempVel.z,
					0.0, 0.0, 0.0);
			}
			break;

		// Sphere Particle. Particles Emit in a spherical shape. 
		// Velocity is applied from the center out towards the rim.
		case SPHERE:
			m_fVolumeScale = m_v3Scaling.x * m_v3Scaling.x * m_v3Scaling.x;
			velocity 	= outv;
			radius		= l;
			number = (int)(m_fVolumeScale * fnumber);
			
			for (int i = 0; i < number; i++)
			{
				v3TempPos = PM.GetRandomNormalVector();
				x = v3TempPos.x;
				y = v3TempPos.y;
				z = v3TempPos.z;
				v3TempPos *= PM.GetRandomFloat() * radius;
				v3TempPos += m_v3Position;
				
				// Set Velocity
				if (velocity > 0.0f || velocity < 0.0f) // Code Below is slow enough that one should skip if possible.
				{
					v3TempVel.Set(x, y, z);
					v3TempVel *= velocity;
				}

				iIndex = iIndex + i;
				AddParticle(particles, iIndex,
					v3TempPos.x, v3TempPos.y, v3TempPos.z,
					v3TempVel.x, v3TempVel.y, v3TempVel.z,
					0.0f, 0.0f, 0.0f);
			}
			break;

		case BOX:
			m_fVolumeScale = m_v3Scaling.x * m_v3Scaling.y * m_v3Scaling.z;
			x 		= l;
			y 		= h; // TODO ?
			z 		= w;
			number = (int)(m_fVolumeScale * fnumber);
			

			for (int i = 0; i < number; i++)
			{
				// Set Position
				v3TempPos.x = ( PM.GetRandomFloat() * x) - (x / 2.0f);
				v3TempPos.y = ( PM.GetRandomFloat() * y) - (y / 2.0f);
				v3TempPos.z = ( PM.GetRandomFloat() * z) - (z / 2.0f);
				v3TempPos += m_v3Position;

				// Set Velocity
				v3TempVel.Set(altv*PM.GetRandomFloat(), altv*PM.GetRandomFloat(), altv*PM.GetRandomFloat());
				v3TempVel.y += outv;

				iIndex = iIndex + i;
				AddParticle(particles, iIndex,
					v3TempPos.x, v3TempPos.y, v3TempPos.z,
					v3TempVel.x, v3TempVel.y, v3TempVel.z,
					0.0f, 0.0f, 0.0f);
			}
			break;

		case PLANE:
			m_fVolumeScale = m_v3Scaling.x * m_v3Scaling.z;
			upVelocity	= outv;
			randVelocity = altv;
			x			= l;
			z			= w;
			number = (int)(m_fVolumeScale * fnumber);
						
			for (int i = 0; i < number; i++)
			{
				// Set Position
				v3TempPos.y = 0.0f;
				v3TempPos.x = ( PM.GetRandomFloat() * x) - (x / 2.0f);
				v3TempPos.z = ( PM.GetRandomFloat() * z) - (z / 2.0f);
				v3TempPos += m_v3Position;
	
				// Set Velocity
				v3TempVel.Set(randVelocity*PM.GetRandomFloat(), randVelocity*PM.GetRandomFloat(), randVelocity*PM.GetRandomFloat());
				v3TempVel.y += upVelocity;
			
				iIndex = iIndex + i;
				AddParticle(particles, iIndex,
					v3TempPos.x, v3TempPos.y, v3TempPos.z,
					v3TempVel.x, v3TempVel.y, v3TempVel.z,
					0.0f, 0.0f, 0.0f);
			}
			break;
			
		case DISK:
			m_fVolumeScale = m_v3Scaling.x * m_v3Scaling.x;
			upVelocity	= outv;
			outVelocity = altv;
			radius 		= l;
			number = (int)(m_fVolumeScale * fnumber);
			
 			for (int i = 0; i < number; i++)
			{
				// Set Position
				v3TempPos = PM.GetRandomXZDisk();
				v3TempVel.Set(v3TempPos);
				v3TempVel.Normalize();
				v3TempPos *= radius;
				v3TempPos += m_v3Position;

				// Set Velocity
				v3TempVel.Set(outVelocity*v3TempVel.x, upVelocity, outVelocity*v3TempVel.z);

				iIndex = iIndex + i;
				AddParticle(particles, iIndex,
					v3TempPos.x, v3TempPos.y, v3TempPos.z,
					v3TempVel.x, v3TempVel.y, v3TempVel.z,
					0.0f, 0.0f, 0.0f);
			}
			break;

		case RING:
			m_fVolumeScale = m_v3Scaling.x * m_v3Scaling.x;
			upVelocity		= outv;
			outVelocity		= altv;
			inRadius 		= l;
			outRadius		= w;
			number = (int)(m_fVolumeScale * fnumber);
			
 			for (int i = 0; i < number; i++)
			{
				// Set Position
				v3TempPos = PM.GetRandomNormalXZDisk();
				x = v3TempPos.x;
				z = v3TempPos.z;
				float randomband = ((PM.GetRandomFloat() * (outRadius - inRadius)) + inRadius);
				v3TempPos.x *= randomband;
				v3TempPos.z *= randomband;
				v3TempPos += m_v3Position;

				// Set Velocity
				v3TempVel.Set(outVelocity*x, upVelocity, outVelocity*z);

				iIndex = iIndex + i;
				AddParticle(particles, iIndex,
					v3TempPos.x, v3TempPos.y, v3TempPos.z,
					v3TempVel.x, v3TempVel.y, v3TempVel.z,
					0.0f, 0.0f, 0.0f);
			}
			break;
	
		case HALO:
			velocity		= outv;
			inRadius		= l;
			outRadius		= w;
			number = (int)(m_fVolumeScale * fnumber);
		
			for (int i = 0; i < number; i++)
			{

			}
			break;
	
		case SHELL:
			m_fVolumeScale = m_v3Scaling.x * m_v3Scaling.x * m_v3Scaling.x;
			velocity		= outv;
			inRadius		= l;
			outRadius		= w;
			arc				= h;
			number = (int)(m_fVolumeScale * fnumber);

			for (int i = 0; i < number; i++)
			{
				v3TempPos = PM.GetRandomNormalVector();
				x = v3TempPos.x;
				y = v3TempPos.y;
				z = v3TempPos.z;
				v3TempPos *= (PM.GetRandomFloat() * (outRadius - inRadius)) + inRadius;
				v3TempPos += m_v3Position;
				
				// Set Velocity
				if (velocity > 0.0f || velocity < 0.0f) // Code Below is slow enough that one should skip if possible.
				{
					v3TempVel.Set(x, y, z);
					v3TempVel *= velocity;
				}

				iIndex = iIndex + i;
				AddParticle(particles, iIndex,
					v3TempPos.x, v3TempPos.y, v3TempPos.z,
					v3TempVel.x, v3TempVel.y, v3TempVel.z,
					0.0f, 0.0f, 0.0f);
			}
			break;

		case CONE:
			m_fVolumeScale = m_v3Scaling.z;
			velocity		= outv;
			inRadius		= l;
			outRadius		= w;
			arc				= h;
			number = (int)(m_fVolumeScale * fnumber);
			for (int i = 0; i < number; i++)
			{
				phi = (PM.GetRandomFloat()*arc*2.0f - arc);
				theta = (PM.GetRandomFloat()*360.0f);
				sinPhi = sin(DegToRad * phi);
				// x forward
//				y = sinPhi*cos(DegToRad * theta);
//				x = cos(DegToRad * phi);
//				z = sinPhi*sin(DegToRad * theta);
				// z forward
				y = sinPhi*cos(DegToRad * theta);
				z = cos(DegToRad * phi);
				x = sinPhi*sin(DegToRad * theta);

				v3TempPos.Set(x, y, z);
				v3TempPos = m_mat4ParentTransform.TransformDirection(v3TempPos);  // TODO: Once and maybe move this to shader?
				x = v3TempPos.x;
				y = v3TempPos.y;
				z = v3TempPos.z;
				v3TempPos *= (PM.GetRandomFloat() * (outRadius - inRadius)) + inRadius;
				v3TempPos += m_v3Position;
				
				// Set Velocity
				if (velocity > 0.0f || velocity < 0.0f) // Code Below is slow enough that one should skip if possible.
				{
					v3TempVel.Set(x, y, z);
					v3TempVel *= velocity;
				}

				iIndex = iIndex + i;
				AddParticle(particles, iIndex,
					v3TempPos.x, v3TempPos.y, v3TempPos.z,
					v3TempVel.x, v3TempVel.y, v3TempVel.z,
					0.0f, 0.0f, 0.0f);
			}
			break;

		default:
			number = 0;
			assert(0); // Unknown Type or Bad Data.
			break;
	}
	return number;
}

// Get Timestep. TODO: Note, this time step is NOT paused, it could be..
float CParticleEmitterObject::GetTime()
{
	float time;
//	static DWORD msgHash_GetTime = CHashString(_T("GetTime")).GetUniqueID();
//	m_ToolBox->SendMessage(msgHash_GetTime,sizeof(float),(void*)&time );
	CHashString hszGameDeltaTimer(_T("GameDeltaTimer"));
	time = m_Timer->GetTimeDelta(&hszGameDeltaTimer);
	return time;
}

int CParticleEmitterObject::UpdateBuffer( char ** pDest, int offset, IVertexBufferObject * vb, IIndexBuffer *ib )
{
	// Skip buffer update if particles are dead and gone.
	if ( !m_nParticles )
		return 0;

	if ( m_pEmitterProperties )
	{
		m_IB = ib;
		m_VB = vb;
		m_VBOffset = offset;
		unsigned int iVertices = 4;

		// Process particles
		int nVerts = UpdateParticles( );

		// check if we have enough room in the vb
		if ((vb->GetSize() - offset) < (iVertices*nVerts))
			return 0;

//		m_ToolBox->Log(LOGWARNING, _T("Particle nVerts = %d \n"), nVerts);

		if (nVerts <= (int)vb->GetSize() - offset)
		{
			memcpy( (*pDest), m_ParticleBuffer, nVerts*sizeof( PARTICLEEMITTER_VERTEX ) );
			(*pDest) += nVerts*sizeof( PARTICLEEMITTER_VERTEX );
		}
		else
		{
			nVerts = 0;
//			m_VerticesWritten = 0;
		}

		m_bInScene = true;//reset	TODO
		return nVerts;
	}
	return 0;
}

bool CParticleEmitterObject::Update( )
{

	bool bReturn = true;
	DWORD result = 0;

	if (m_bTransformIsDirty)
	{
		UpdateTransform();
	}

	if (m_pEmitterProperties) // TODO: Only if valid particle data...
	{
		CParticleEmitterType &EP = *m_pEmitterProperties;

		// GetTime
		m_TimeDeltaSeconds = GetTime();

		// This is to prevent a first frame particle creation lag that exists due to the lack of a valid timestep on the first update.
		// TODO: Find a Better way to do this
		if (m_bFirstUpdate)
		{
			m_TimeDeltaSeconds = .033f;
			m_LastPosition = m_v3Position;
			m_v3LastDeletedParticle = m_v3Position;
			m_Life = 0.0;
		}
		m_LastPositionOffset = m_v3Position - m_LastPosition;
		m_LastPosition = m_v3Position;

		// Still Active - Generate new particles and process existing particles.
		if ( m_bEnabled && ( (m_RemainingLife > 0.0f) || (m_MaxLife < 0.0f) ) )
		{
//			m_TimeDeltaSeconds = .033f; // DEBUG LINE
			m_Life += m_TimeDeltaSeconds;
			m_RemainingLife -= m_TimeDeltaSeconds;
			
/*			float nFrameParticles = 0.0f;
			switch (m_ScaleMode)
			{
				case SCALEVOLUME:
					nFrameParticles = m_fVolumeScale * EP.GetEmitterNumber( m_Life );
					break;
				default:
					nFrameParticles = EP.GetEmitterNumber( m_Life );
					break;
			}
*/			float nFrameParticles = EP.GetEmitterNumber( m_Life );
			m_nParticleNumberAccumulation += nFrameParticles * m_TimeDeltaSeconds ;
			if( m_bFirstUpdate )
			{
				if( (m_nParticleNumberAccumulation > 0.0f) && (m_nParticleNumberAccumulation < 1.0f) )
				{
					m_nParticleNumberAccumulation = 1.0f;
				}
			}

			unsigned int iParticlesCreated = GenerateNewParticles(m_pParticles, m_iParticleIndex,
				EP.GetType(), 
				EP.GetEmitterL(m_Life), 
				EP.GetEmitterW(m_Life), 
				EP.GetEmitterH(m_Life), 
				EP.GetEmitterOutVelocity(m_Life), 
				EP.GetEmitterAltVelocity(m_Life), 
				m_nParticleNumberAccumulation);

//			m_ToolBox->Log(LOGWARNING, _T("Particles created = %d \n"), (int)iParticlesCreated);
			m_nParticleNumberAccumulation -= (float)iParticlesCreated / m_fVolumeScale;

//			if (m_nParticleNumberAccumulation < 1.0f)
//				assert(0);
//			UpdateParticles( );
		}
		// If Life > LifeLoop reset to continue effect.
		if ((m_Life > EP.GetLoopLife()) && EP.GetLooping())
			m_Life = 0.0f;
		// Deactivated w/ Existing Particles - Not Emitting, process existing particles.
		if (!m_bEnabled && ((m_MaxLife > 0.0f) && (m_RemainingLife < 0.0f) && (m_nParticles > 0)))
		{
			
		}
		// Deactivated w/o Existing Particles - All Existing Particles dead... return false.
		if ((m_MaxLife > 0.0f) && (m_RemainingLife < 0.0f) && (m_nParticles <= 0))
		{
			m_bEnabled = false;
			if (m_bStartEnabled)
			{
                bReturn = false;
			}
		}
	}
	if (m_bFirstUpdate)
		m_bFirstUpdate = false;

	if (!bReturn && m_bParticleSystemControlled)
	{
		CParticleEmitterObject *pEmitterObject = this;
		static DWORD msgDeRegisterChildEmitter = CHashString(_T("DeRegisterChildEmitter")).GetUniqueID();
		static CHashString psoName(_T("CParticleSystemObject"));
		EngineGetToolBox()->SendMessage(msgDeRegisterChildEmitter, sizeof(CParticleEmitterObject *), &pEmitterObject, GetParentName(), &psoName);
	}

	return bReturn;

}

int CParticleEmitterObject::UpdateParticles( )
{
	// Don't Update Particles beyond what is neccessary.
	if (!m_bInScene && m_MaxLife < 0)
		return 0;

	if (!m_nParticles)
		return 0;
	
	CParticleEmitterType &EP = *m_pEmitterProperties;

	// Emitter Preprocess
//	bool bAnimatedTexture = ( EP.GetTexture() && EP.GetTexture()->GetTextureType() == EE_TEXTUREANIMATED2D );
	unsigned int nVerticiesWritten = 0;
	unsigned int iParticlesDestroyed = 0;

	float fAttachToEmitter = EP.GetAttachToEmitter();
	Vec3 vAttachAdjust( m_LastPositionOffset );
    vAttachAdjust *= fAttachToEmitter;

	unsigned int i = 0;
	unsigned int iActiveIndex = 0;

	while (iActiveIndex < m_nParticles)
	{
		SParticle &p = m_pParticles[i];

		// if particle is inactive, continue.
		if (!p.active)
		{
			i++;
			continue;
		}
		p.life += m_TimeDeltaSeconds;
		
		// if particle has died of old age, deactivate and continue.
		if (p.life > p.maxlife)
		{
			p.active = false;
			m_nParticles--;
			iParticlesDestroyed++;
			m_v3LastDeletedParticle = p.pos;
			i++;
			continue;
		}

		// Particle Weight.
		Vec3 vGravity( p.down );
		vGravity *=  EP.GetParticleWeight( p.life, p.maxlife, p.midlife )* p.life;


		// Particle Slowdown.
		// UNDONE.

		float fParticleSpeed =  EP.GetParticleSpeed(p.life, p.maxlife, p.midlife);

		// Particle Position.
		p.pos.x += m_TimeDeltaSeconds * ( p.vel.x * fParticleSpeed + vGravity.x );
		p.pos.y += m_TimeDeltaSeconds * ( p.vel.y * fParticleSpeed + vGravity.y );
		p.pos.z += m_TimeDeltaSeconds * ( p.vel.z * fParticleSpeed + vGravity.z );
		// The first frame of a particle should not be offset by the change since last position:
		if (p.firstframe) 
			p.firstframe = false;
		else
			p.pos += vAttachAdjust;

		// Add ParticleVerts To Renderlist:

		if (EP.GetAnimatedTexture())
		{
			float u1, u2, v1, v2;
			EP.GetAnimatedUVs(u1, u2, v1, v2, p.life);

			// Now fill uv's for animated particles. 
			// Note: No need for normal particles as defaults are fine.
			m_ParticleBuffer[ nVerticiesWritten  + 0 ].TexCoord[0] = u1;
			m_ParticleBuffer[ nVerticiesWritten  + 0 ].TexCoord[1] = v1;
			m_ParticleBuffer[ nVerticiesWritten  + 1 ].TexCoord[0] = u1;
			m_ParticleBuffer[ nVerticiesWritten  + 1 ].TexCoord[1] = v2;		
			m_ParticleBuffer[ nVerticiesWritten  + 2 ].TexCoord[0] = u2;
			m_ParticleBuffer[ nVerticiesWritten  + 2 ].TexCoord[1] = v1;		
			m_ParticleBuffer[ nVerticiesWritten  + 3 ].TexCoord[0] = u2;
			m_ParticleBuffer[ nVerticiesWritten  + 3 ].TexCoord[1] = v2;
		}

		m_ParticleBuffer[ nVerticiesWritten + 0 ].Life.Set( p.life, p.midlife, p.maxlife, p.rot.roll );
		m_ParticleBuffer[ nVerticiesWritten + 1 ].Life.Set( p.life, p.midlife, p.maxlife, p.rot.roll );
		m_ParticleBuffer[ nVerticiesWritten + 2 ].Life.Set( p.life, p.midlife, p.maxlife, p.rot.roll );
		m_ParticleBuffer[ nVerticiesWritten + 3 ].Life.Set( p.life, p.midlife, p.maxlife, p.rot.roll );

		if (EP.GetStretchToOrigin())
		{
			m_ParticleBuffer[ nVerticiesWritten + 0 ].Position.Set(p.origin.x, p.origin.y, p.origin.z);
			m_ParticleBuffer[ nVerticiesWritten + 1 ].Position.Set(p.origin.x, p.origin.y, p.origin.z);
		}
		else
		{
			m_ParticleBuffer[ nVerticiesWritten + 0 ].Position.Set(p.pos.x, p.pos.y, p.pos.z);
			m_ParticleBuffer[ nVerticiesWritten + 1 ].Position.Set(p.pos.x, p.pos.y, p.pos.z);
		}
		m_ParticleBuffer[ nVerticiesWritten + 2 ].Position.Set(p.pos.x, p.pos.y, p.pos.z);
		m_ParticleBuffer[ nVerticiesWritten + 3 ].Position.Set(p.pos.x, p.pos.y, p.pos.z);

		nVerticiesWritten += 4;
		i++;
		iActiveIndex++;
	}
	// DEBUG = Particles Destroyed per frame output.
//	m_ToolBox->Log(LOGWARNING, _T("Particles destroyed = %d \n"), (int)iParticlesDestroyed);
	m_VerticesWritten = nVerticiesWritten; // TODO: REMOVE THIS FROM HERE
	// Return number of Verticies written.
	return nVerticiesWritten;
}
bool CParticleEmitterObject::RenderEmitter( UINT pass, IEffect * override )
{
//	PERFORMANCE_PROFILER_AUTO(CParticleEmitterObject_Render)

	CParticleEmitterType &EP = *m_pEmitterProperties;

	if( m_bInit && m_pEffect && m_IRenderer )
	{
		m_bInScene = true;		
		Matrix4x4 matworld;
		matworld.SetIdentity();
		m_IRenderer->SetMatrix( WORLD_MATRIX, (CONST FLOAT*)matworld.GetMatrix() );
		m_IRenderer->SetMaterial( 0, NULL );
		m_pEffect->Apply(0, NULL, NULL);

		float ViewMatrix[16];
		m_IRenderer->GetMatrix( VIEW_MATRIX, ViewMatrix );
		m_pEffect->SetVertexConstant( 20, ViewMatrix, 4 );

		CParticleEmitterType &EP = *m_pEmitterProperties;
		Vec4 key0, key1, key2;

		EP.GetKeyframesWeightSpeedSizeSpin(key0, key1, key2);
		m_pEffect->SetVertexConstant( 24, key0.x, key0.y, key0.z, key0.w );
		m_pEffect->SetVertexConstant( 25, key1.x, key1.y, key1.z, key1.w );
		m_pEffect->SetVertexConstant( 26, key2.x, key2.y, key2.z, key2.w );

		EP.GetKeyframesColor(key0, key1, key2);
		m_pEffect->SetVertexConstant( 27, key0.x, key0.y, key0.z, key0.w );
		m_pEffect->SetVertexConstant( 28, key1.x, key1.y, key1.z, key1.w );
		m_pEffect->SetVertexConstant( 29, key2.x, key2.y, key2.z, key2.w );

		//Make depth test false for soft particles
		//m_IRenderer->SetDepthTest( false );
		m_IRenderer->SetDepthTest( true );
		m_IRenderer->SetDepthWrite( false );	
		m_IRenderer->SetAlphaTest( true );
		DWORD alpharef = 0x00000001;
		m_IRenderer->SetRenderState( RENDERSTATE_ALPHAREF, (UINT)alpharef );
		m_IRenderer->SetRenderState( RENDERSTATE_ALPHABLENDENABLE, RENDERSTATEPARAM_TRUE );

//		TEXTUREOBJECTPARAMS depTex;
//		depTex.TextureObjectInterface = NULL;
//		static DWORD msg_GetDepthTexture = CHashString( _T("GetDepthTargetAsTexture") ).GetUniqueID();
//		m_ToolBox->SendMessage( msg_GetDepthTexture, sizeof( TEXTUREOBJECTPARAMS ), &depTex );

		// Set Flags
		if (EP.GetAdditiveBlending())
		{
			m_IRenderer->SetBlendMode( BLEND_ADDITIVE );
		}
		else
		{
			m_IRenderer->SetBlendMode( BLEND_NORMAL );	
		}
		m_IRenderer->SetRenderState( RENDERSTATE_CULLMODE, RENDERSTATEPARAM_CULLNONE );

		if( m_VerticesWritten > 0 )
		{
			//render once then set buffer false
			//m_IRenderer->SetMaterial( pass, CurParticleType->m_Material );
			m_IRenderer->SetTexture( 0, EP.GetTexture() );
//			m_IRenderer->SetTexture( 1, depTex.TextureObjectInterface );
			m_IRenderer->RenderIndexBuffer( m_IB, m_VB, 
				m_VBOffset,
				m_VerticesWritten, 0, m_VerticesWritten/2 );
		}
		
		m_IRenderer->SetBlendMode( BLEND_NORMAL );
		m_IRenderer->SetDepthTest( true );
		m_IRenderer->SetDepthWrite( true );
	}
	return true;
}
bool CParticleEmitterObject::Render( UINT pass, IEffect * override )
{
//	m_bInScene = true;
	if (m_bParticleSystemControlled)
		return true;
	return RenderEmitter(pass, override);

}

CParticleEmitterType * CParticleEmitterObject::GetParticleEmitterData()
{
	return m_pEmitterProperties;
}

//
// MESSAGES
//

DWORD CParticleEmitterObject::OnEnable(DWORD size, void *params)
{
	m_bEnabled = true;
	ResetOperatingValues();
	return MSG_HANDLED_PROCEED;
}
DWORD CParticleEmitterObject::OnDisable(DWORD size, void *params)
{
	m_bEnabled = false;
	return MSG_HANDLED_PROCEED;
}
DWORD CParticleEmitterObject::OnPulse(DWORD size, void *params)
{
	//TODO: Unimplemented: 
	//Thinking of switching emitter life to highest particle generation point
	return MSG_HANDLED_STOP;
}
DWORD CParticleEmitterObject::OnReset(DWORD size, void *params)
{
	ResetOperatingValues();
	return MSG_HANDLED_STOP;
}

// 
// ENTITY MESSAGES
//
REGISTER_MESSAGE_HANDLER( SetGlobalPosition, OnSetGlobalPosition, CParticleEmitterObject );
REGISTER_MESSAGE_HANDLER( SetGlobalEuler, OnSetGlobalEuler, CParticleEmitterObject );
REGISTER_MESSAGE_HANDLER( SetGlobalQuaternion, OnSetGlobalQuaternion, CParticleEmitterObject );
REGISTER_MESSAGE_HANDLER( SetGlobalScale, OnSetGlobalScale, CParticleEmitterObject );
REGISTER_MESSAGE_HANDLER( SetGlobalTransform, OnSetGlobalTransform, CParticleEmitterObject );

REGISTER_MESSAGE_HANDLER( SetLocalPosition, OnSetLocalPosition, CParticleEmitterObject );
REGISTER_MESSAGE_HANDLER( SetLocalEuler, OnSetLocalEuler, CParticleEmitterObject );
REGISTER_MESSAGE_HANDLER( SetLocalQuaternion, OnSetLocalQuaternion, CParticleEmitterObject );
REGISTER_MESSAGE_HANDLER( SetLocalScale, OnSetLocalScale, CParticleEmitterObject );
REGISTER_MESSAGE_HANDLER( SetLocalTransform, OnSetLocalTransform, CParticleEmitterObject );

//REGISTER_MESSAGE_HANDLER( AttachSceneObject, OnAttachSceneObject, CParticleEmitterObject );

void CParticleEmitterObject::UpdateTransform()
{
	Matrix4x4 parentGlobalTransform;

	if (m_pBaseObject)
	{
		m_pBaseObject->GetAttachTransform(&m_hsAttachNodeName, parentGlobalTransform);
	}

	// factor in the parent's rotation
	//Matrix4x4 newGlobalTransform = m_LocalTransform.GetTransform() * parentGlobalTransform;
	Matrix4x4 newGlobalTransform = parentGlobalTransform* m_LocalTransform.GetTransform();
	m_GlobalTransform.SetTransform( newGlobalTransform );
	m_mat4ParentTransform = newGlobalTransform ; //TODO FIX THIS HACK

	SetPosition( m_mat4ParentTransform.GetTranslation());
	SetRotation( m_mat4ParentTransform.GetRotation());
//	SetScaling( m_mat4ParentTransform.GetScale());
	// The transform is now clean
	m_bTransformIsDirty = false;
}
/*
void CParticleEmitterObject::SetChildrenDirtyTransformFlags()
{
	SCENEOBJECTLIST::iterator itrChild = m_Children.begin();
	while (itrChild != m_Children.end())
	{
		ISceneObject *pObject = *itrChild;
		pObject->SetDirtyTransformFlag();
		itrChild++;
	}

	// set attach object dirty transform too!
	ATTACHMENTOBJECTLIST::iterator attachListIter = m_AttachmentList.begin();
	for (; attachListIter != m_AttachmentList.end(); attachListIter++)
	{
		ISceneObject *obj = (*attachListIter);
		if (obj)
		{
			obj->SetDirtyTransformFlag();
		}
	}
}
*/
DWORD CParticleEmitterObject::OnSetGlobalPosition( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* newGlobalPosition = (Vec3*)params;

	if (!newGlobalPosition->IsValid())
	{
		m_ToolBox->Log(LOGWARNING, _T("(%s)ParticleEmitterObject::OnSetGlobalPosition received invalid number in one of the components: %f, %f, %f \n"), this->GetName()->GetString(), newGlobalPosition->x, newGlobalPosition->y, newGlobalPosition->z);
		return MSG_NOT_HANDLED;
	}
	
	// Get the parent's global transform
	Matrix4x4 parentGlobalTransform;

	if (m_pBaseObject)
	{
		m_pBaseObject->GetAttachTransform(&m_hsAttachNodeName, parentGlobalTransform);
	}

	// local position = parent's global - new global
	Matrix4x4 parentGlobalTransformInverse;
	parentGlobalTransform.GetInverse( parentGlobalTransformInverse );

	Vec3 newLocal = parentGlobalTransformInverse * *newGlobalPosition;

	// Set the local transform
	m_LocalTransform.SetTranslation( newLocal );
	// Set the global translation

	m_GlobalTransform.SetTranslation( *newGlobalPosition );

	UpdateTransform();

	return MSG_HANDLED_STOP;
}

DWORD CParticleEmitterObject::OnSetGlobalEuler( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(EulerAngle) );
	EulerAngle* newGlobalEuler = (EulerAngle*)params;

	if (!newGlobalEuler->IsValid())
	{
		m_ToolBox->Log(LOGWARNING, _T("CEntity::OnSetGlobalPosition received invalid number in one of the components: %f, %f, %f\n"),
				newGlobalEuler->GetRoll(), newGlobalEuler->GetPitch(), newGlobalEuler->GetYaw());
		return MSG_NOT_HANDLED;
	}
	
	// Get the parent's global euler
	EulerAngle parentGlobalEuler;

	if (m_pBaseObject)
	{
		Matrix4x4 parentXForm;
		m_pBaseObject->GetAttachTransform(&m_hsAttachNodeName, parentXForm);
		parentGlobalEuler.Set(parentXForm.GetRotation());
	}

	// local position = parent's global - new global
	EulerAngle newLocal = *newGlobalEuler - parentGlobalEuler;

	// Set the transform
	m_LocalTransform.SetEuler( newLocal );
	// Set the global euler

	m_GlobalTransform.SetEuler( *newGlobalEuler );
	
	UpdateTransform();

	return MSG_HANDLED_STOP;
}

DWORD CParticleEmitterObject::OnSetGlobalQuaternion( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Quaternion) );
	Quaternion* newGlobalQuaternion = (Quaternion*)params;

	if (!newGlobalQuaternion->IsValid())
	{
		m_ToolBox->Log(LOGWARNING, _T("CEntity::OnSetGlobalPosition received invalid number in one of the components: %f, %f, %f\n"),
				newGlobalQuaternion->x, newGlobalQuaternion->y, newGlobalQuaternion->z, newGlobalQuaternion->w);
		return MSG_NOT_HANDLED;
	}
	
	// Get the parent's global euler
	Quaternion parentGlobalQuaternion;

	if (m_pBaseObject)
	{
		Matrix4x4 parentXForm;
		m_pBaseObject->GetAttachTransform(&m_hsAttachNodeName, parentXForm);
		parentGlobalQuaternion.SetFromMatrix(parentXForm);
	}

	// local position = parent's global - new global
	Quaternion newLocal = *newGlobalQuaternion / parentGlobalQuaternion;

	// Set the transform
	m_LocalTransform.SetQuaternion( newLocal );
	// Set the global quaternion

	m_GlobalTransform.SetQuaternion( *newGlobalQuaternion );
	
	UpdateTransform();

	return MSG_HANDLED_STOP;
}

DWORD CParticleEmitterObject::OnSetGlobalScale( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* newGlobalScale = (Vec3*)params;

	if (!newGlobalScale->IsValid())
	{
		m_ToolBox->Log(LOGWARNING, _T("CEntity::OnSetGlobalPosition received invalid number in one of the components: %f, %f, %f\n"),
				newGlobalScale->x, newGlobalScale->y, newGlobalScale->z);
		return MSG_NOT_HANDLED;
	}

	// Get the parent's global euler
	Vec3 parentGlobalScale(1.0,1.0,1.0);	// Default Scale of 1.0,1.0,1.0, should never be zero.

	if (m_pBaseObject)
	{
		Matrix4x4 parentXForm;
		m_pBaseObject->GetAttachTransform(&m_hsAttachNodeName, parentXForm);
		parentGlobalScale.Set(parentXForm.GetScale());
	}

	// local position = parent's global - new global
	Vec3 newLocal = *newGlobalScale / parentGlobalScale;

	// Set the transform
	m_LocalTransform.SetScale( newLocal );
	// Set the global scale

	m_GlobalTransform.SetScale( *newGlobalScale );
	
	UpdateTransform();

	return MSG_HANDLED_STOP;
}

DWORD CParticleEmitterObject::OnSetGlobalTransform( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Matrix4x4) );
	Matrix4x4* newGlobalTransform = (Matrix4x4*)params;
	
	// Get the parent's global euler
	Matrix4x4 parentGlobalTransform;

	if (m_pBaseObject)
	{
		m_pBaseObject->GetAttachTransform(&m_hsAttachNodeName, parentGlobalTransform);
	}

	// multiply by invers of parent's global
	// local position = parent's global - new global
	Matrix4x4 parentGlobalTransformInverse;
	parentGlobalTransform.GetInverse( parentGlobalTransformInverse );
	Matrix4x4 newLocal = *newGlobalTransform * parentGlobalTransformInverse;

	// Set the transform
	m_LocalTransform.SetTransform( newLocal );

	m_GlobalTransform.SetTransform( *newGlobalTransform );

	UpdateTransform();

	return MSG_HANDLED_STOP;
}

DWORD CParticleEmitterObject::OnSetLocalPosition( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* position = (Vec3*)params;

	m_LocalTransform.SetTranslation( *position );

	// Since we are setting the local position the global position is now dirty
	SetDirtyTransformFlag();

	return MSG_HANDLED_STOP;
}

DWORD CParticleEmitterObject::OnSetLocalEuler( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(EulerAngle) );
	EulerAngle* euler = (EulerAngle*)params;

	m_LocalTransform.SetEuler( *euler );

	// Since we are setting the local position the global position is now dirty
	SetDirtyTransformFlag();

	return MSG_HANDLED_STOP;
}

DWORD CParticleEmitterObject::OnSetLocalQuaternion( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Quaternion) );
	Quaternion* quaternion = (Quaternion*)params;

	m_LocalTransform.SetQuaternion( *quaternion );

	// Since we are setting the local position the global position is now dirty
	SetDirtyTransformFlag();

	return MSG_HANDLED_STOP;
}

DWORD CParticleEmitterObject::OnSetLocalScale( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* scale = (Vec3*)params;

	m_LocalTransform.SetScale( *scale );

	// Since we are setting the local position the global position is now dirty
	SetDirtyTransformFlag();

	return MSG_HANDLED_STOP;
}

DWORD CParticleEmitterObject::OnSetLocalTransform( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Matrix4x4) );
	Matrix4x4* transform = (Matrix4x4*)params;

	m_LocalTransform.SetTransform( *transform );

	// Since we are setting the local position the global position is now dirty
	SetDirtyTransformFlag();

	return MSG_HANDLED_STOP;
}

DWORD CParticleEmitterObject::OnAttachSceneObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ATTACHSCENEOBJECTPARAMS));
	ATTACHSCENEOBJECTPARAMS asop = *(ATTACHSCENEOBJECTPARAMS*)param;
	if (asop.hsAttachmentObject)
	{
		// if a node is specified, entities don't handle the message
		// otherwise...
		if (asop.hsNodeAttachName == NULL)
		{
			static DWORD msgRegisterSceneAttachObjects = CHashString(_T("RegisterSceneAttachObjects")).GetUniqueID();
			asop.hsBaseObject = GetName();
			m_ToolBox->SendMessage(msgRegisterSceneAttachObjects, sizeof(ATTACHSCENEOBJECTPARAMS), &asop);			
			return MSG_HANDLED_STOP;
		}
	}
	return MSG_NOT_HANDLED;
}