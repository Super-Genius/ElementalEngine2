///==========================================================================
/// \file	CParticleRenderObject.hpp
/// \brief	Implementation of CParticleRenderObject interface for adapting to Vision
/// \date	3/09/2005
/// \author Marvin Gouw
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

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ElementalEngine;
MathUtil math;

UINT MAX_PARTICLE_COUNT = 1000;

REGISTER_COMPONENT(CParticleRenderObject);

REGISTER_MESSAGE_HANDLER(Disable, OnDisable, CParticleRenderObject );
REGISTER_MESSAGE_HANDLER(Enable, OnEnable, CParticleRenderObject );
REGISTER_MESSAGE_HANDLER(Reset, OnResetParticle, CParticleRenderObject );
//REGISTER_MESSAGE_HANDLER(Pulse, OnPulse, CParticleRenderObject );
REGISTER_MESSAGE_HANDLER(SetParticleTime, OnSetParticleTime, CParticleRenderObject );
REGISTER_MESSAGE_HANDLER(GetParticleEmitterInterface, OnGetParticleEmitterInterface, CParticleRenderObject);
REGISTER_MESSAGE_HANDLER(SetParticleInitialVelocity, OnSetParticleInitialVelocity, CParticleRenderObject );
REGISTER_MESSAGE_HANDLER(ReloadTextures, OnReloadTextures, CParticleRenderObject );

static const float g_IllusionTimeMultiplier = 4.f;
static const float g_IllusionTimeScale = 1.f/g_IllusionTimeMultiplier;
static const float rad2deg = 180.f/3.141592654f;

float CParticleRenderObject::m_vRandomValues[RANDOM_VALUE_COUNT];
int CParticleRenderObject::m_iRandomValueIndex = -1;

///
/// Constructor / Destructor
///
CParticleRenderObject::CParticleRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier) : 
		CRenderObject<>( _T("CParticleManager"),_T("CParticleRenderObject"), parentName, name )
{
	// init random values
	if (m_iRandomValueIndex == -1)
	{
		for (int i=0; i < RANDOM_VALUE_COUNT; i++)
		{
			m_vRandomValues[i] = ((float)rand()/(float)RAND_MAX)*2.0f - 1.0f;
		}
		m_iRandomValueIndex = 0;
	}

	m_ParticleBuffer = new PARTICLE_VERTEX[ MAX_PARTICLE_COUNT * 6 ];
	m_bEnabled = false;
	
	m_bAddToHierarchy = bAddToHier;	
	if (bAddToHier)
	{
		AddToHierarchy();
	}

	DeInit();
	m_RemainingLife = -1 ;
	m_MaxLife = 1;
	m_Filename = "";
	CurParticleType = NULL;
	m_bInScene = false;
	m_ScreenAligned = false;
	m_OldestFirst = false;
	m_LastPosition.Set( 0,0,0);
	m_LastPositionOffset.Set( 0,0,0 );
	memset( m_EmitterKeyFrames, 0, sizeof( m_EmitterKeyFrames) );
	memset( m_EmitterUpdatedVariations, 0, sizeof( m_EmitterUpdatedVariations) );
	m_ParticleEmitter = NULL;
	m_bFirstUpdate = true;
	m_LastPositionSet = false;
	m_bStartEnabled = false;
	m_InitialVelocity.Set( 0,0,0 );
	m_ClipDistance = 0;

	//
   	// CEntity Data
   	//
   	m_wszEntityTypes = "";
   	m_bTransformIsDirty = false;
   	m_fBoundingRadius = 0.f;
   	if (parentName)
   	{
   		static DWORD msgHash_GetComponentType = CHashString(_T("GetComponentType")).GetUniqueID();
   		GETCOMPONENTTYPEPARAMS gctp;
   		gctp.name = parentName;
   		m_ToolBox->SendMessage(msgHash_GetComponentType, sizeof(gctp), &gctp);			
   
   		static DWORD dEntity = CHashString(_T("CEntity")).GetUniqueID();
   		if ((gctp.componenttype != NULL) && (gctp.componenttype->GetUniqueID() == dEntity))
   		{
   			// if the parent is an entity, attach to it!
   			ATTACHSCENEOBJECTPARAMS asop;
   			asop.hsAttachmentObject = name;
   			asop.hsNodeAttachName = NULL;
   
   			static DWORD msgAttachSceneObject = CHashString(_T("AttachSceneObject")).GetUniqueID();
   			m_ToolBox->SendMessage(msgAttachSceneObject, sizeof(ATTACHSCENEOBJECTPARAMS), &asop, parentName);			
   		}
   	}
   	// even if it doesn't attach, we still want to set this guy to true initially
   	m_bDefaultAttach = true; // TODO WHY!?
}

CParticleRenderObject::~CParticleRenderObject()
{
	//remove from quad tree or any other manager that uses updateboundingbox params
	RemoveBoundingObject();

	DeInit();

	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
	SAFE_DELETE_ARRAY( m_ParticleBuffer );
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CParticleRenderObject::Create(int nArgs, va_list argptr)
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
			self = new CParticleRenderObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CParticleRenderObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

///
/// Init / Update / DeInit
///
void CParticleRenderObject::Init()
{
	//grab the renderer
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	renderInterfaceParam.m_RendererInterface = NULL;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam, NULL, NULL );
	m_IRenderer = renderInterfaceParam.m_RendererInterface;


	GETPARTICLEEMITTERMESSAGE msg;
	msg.filename = &m_ParticleTypeName;
	//try to get the particle from the manager
	static DWORD msgHash_GetParticleEmitterType = CHashString(_T("GetParticleEmitterType")).GetUniqueID();
	if( m_ToolBox->SendMessage(msgHash_GetParticleEmitterType, sizeof(GETPARTICLEEMITTERMESSAGE),&msg ) == MSG_HANDLED )
	{
		m_ParticleEmitter = dynamic_cast< CParticleEmitter * > ( msg.emittertype );		
		if( m_ParticleEmitter )
		{
			m_KeyInstances.clear();
			UINT numbrushes = m_ParticleEmitter->GetNumBrushes();
			m_KeyInstances.resize( numbrushes );
			for( UINT i = 0; i < m_Particles.size(); i++ )
			{
				m_Particles[ i ].clear();
			}
			m_Particles.resize( numbrushes );
			memset( m_EmitterKeyFrames, 0, sizeof( m_EmitterKeyFrames) );
			memset( m_EmitterUpdatedVariations, 0, sizeof( m_EmitterUpdatedVariations) );
		}
		else
		{
			m_ToolBox->Log(LOGWARNING, _T("Failed to get ParticleEmitterPointer: \"%s\"!\n"), GetName()->GetString() );
		}
	}
	else
	{
		m_ToolBox->Log(LOGWARNING, _T("Failed to get ParticleEmitterPointer: \"%s\"!\n"), GetName()->GetString() );
		m_ParticleEmitter = NULL;
	}
	//set particle tex coords defaults, these never change unless an animated texture is used
	PARTICLE_VERTEX *pParticle = m_ParticleBuffer;
	for( UINT i = 0; i < MAX_PARTICLE_COUNT; i++ )
	{
		pParticle[0].TexCoord[0] = 0;
		pParticle[0].TexCoord[1] = 0;
		pParticle[1].TexCoord[0] = 0;
		pParticle[1].TexCoord[1] = 1;
		pParticle[2].TexCoord[0] = 1;
		pParticle[2].TexCoord[1] = 0;
		pParticle[3].TexCoord[0] = 1;
		pParticle[3].TexCoord[1] = 0;
		pParticle[4].TexCoord[0] = 0;
		pParticle[4].TexCoord[1] = 1;
		pParticle[5].TexCoord[0] = 1;
		pParticle[5].TexCoord[1] = 1;
		pParticle += 6;
	}
	if (m_bEnabled)
		OnEnable(0,0);

	static CHashString oldEffect(_T("Shaders/OldParticle.efx"));
	m_pEffect = LoadEffect(&oldEffect);
	//
   	// CEntity Init
   	//
 	UpdateTransform();
}

/// returns offset
int CParticleRenderObject::UpdateBuffer( char ** pDest, int offset,IVertexBufferObject * vb )
{
	if( //m_bEnabled && 
		m_ParticleEmitter )
	{
		m_VB = vb;
		m_VBOffset = offset;
		int curoffset = 0;
		for( unsigned int i = 0; i < m_KeyInstances.size() ; i++ )
		{
			CurKeyInstance = &m_KeyInstances[ i ];
			CurKeyInstance->m_CurOffset = curoffset;
			curoffset += CurKeyInstance->m_VerticesWritten;
		}
		if (curoffset <= (int)vb->GetSize() - offset)
		{
			memcpy( (*pDest), m_ParticleBuffer, curoffset*sizeof( PARTICLE_VERTEX ) );
			(*pDest) += curoffset*sizeof( PARTICLE_VERTEX );
		}
		else
		{
			CurKeyInstance->m_VerticesWritten = 0;
			curoffset = 0;
		}

		m_bInScene = false;//reset	
		return curoffset;
	}
	return 0;
}

/// \brief Update the object
/// \return true or false
bool CParticleRenderObject::Update( )
{
	bool bReturn = true;
	DWORD result = 0;
	if (m_bTransformIsDirty)
	{
		UpdateTransform();
/*		static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGlobalTransform, sizeof(Matrix4x4), &m_mat4ParentTransform, GetParentName() );
		SetPosition( m_mat4ParentTransform.GetTranslation());
		SetRotation( m_mat4ParentTransform.GetRotation());
		SetScaling( m_mat4ParentTransform.GetScale());
		m_bTransformIsDirty = false;
*/	}

	if( //m_bEnabled && 
		m_ParticleEmitter )
	{
		m_nActiveParticles = 0;
		for (int i = 0; i < (int)m_KeyInstances.size(); i++)
		{
			m_nActiveParticles += (int)m_Particles[ i ].size();
		}

		// Continue if RemainingLife remains or the particle lasts forever.
		if( (m_RemainingLife > 0.0f) || (m_MaxLife < 0.0f) || m_nActiveParticles )
		{
			//count down life
			float TimeDeltaSeconds = GetTime();
			// This is to prevent a first frame particle creation lag that exists due to the lack of a valid timestep on the first update.
			if (m_bFirstUpdate)
			{
				TimeDeltaSeconds = .033f;
				m_LastPosition = m_Position;
			}
			m_LastPositionOffset = m_Position - m_LastPosition;
			m_LastPosition = m_Position;

			m_TimeDelta = TimeDeltaSeconds*1000.f;
//			m_TimeDelta = 33; // DEBUG DO NOT LEAVE IN
			m_RemainingLife -= m_TimeDelta;
			float unitlife = (m_MaxLife - m_RemainingLife)*g_IllusionTimeScale;// m_RemainingLife/m_MaxLife;

			//Update Emitter variables 
			UPDATE_EMITTER_VARIABLE( unitlife, EMITTER_NUMBER ); //
			UPDATE_EMITTER_VARIABLE( unitlife, EMITTER_SIZE ); //
			UPDATE_EMITTER_VARIABLE( unitlife, EMITTER_VELOCITY ); //
			UPDATE_EMITTER_VARIABLE( unitlife, EMITTER_WEIGHT ); // 
			UPDATE_EMITTER_VARIABLE( unitlife, EMITTER_SPIN ); //
			UPDATE_EMITTER_VARIABLE( unitlife, EMITTER_MOTION_RAND ); //
			UPDATE_EMITTER_VARIABLE( unitlife, EMITTER_ZOOM ); //
			UPDATE_EMITTER_VARIABLE( unitlife, EMITTER_VISIBILITY ); 
			UPDATE_EMITTER_VARIABLE( unitlife, EMITTER_TINT_STRENGTH ); //
			UPDATE_EMITTER_VARIABLE( unitlife, EMITTER_EMISSION_ANGLE ); // not used, instead we use the object's rotation
			UPDATE_EMITTER_VARIABLE( unitlife, EMITTER_EMISSION_RANGE ); //
			UPDATE_EMITTER_VARIABLE( unitlife, EMITTER_ACTIVE );
			UPDATE_EMITTER_VARIABLE( unitlife, EMITTER_ANGLE ); //not used for point emitters right now
			UPDATE_EMITTER_VARIABLE( unitlife, EMITTER_LIFE ); //

			UINT maxbrushparticles = MAX_PARTICLE_COUNT / m_KeyInstances.size();
			//pick particle type:
			for( unsigned int pi = 0; pi < m_KeyInstances.size(); pi++ )
			{
				CurKeyInstance = &m_KeyInstances[ pi ];
				CurKeyInstance->m_VerticesWritten = 0;//Reset written count here
				CurParticleType = (CParticleType*)m_ParticleEmitter->GetBrush( pi );
				if( CurParticleType == NULL )
				{
					continue;
				}
				//reset render state
				CurKeyInstance->m_VerticesWritten = 0;
				//spawn particles
				UPDATE_VARIABLE( unitlife, PARTICLE_NUMBER );
				UPDATE_VARIABLE( unitlife, PARTICLE_NUMBER_VARIATION );	

				float numparticles = 0.0f;
				// spawn new particles only if enabled and either we are in scene or going to die off naturally
				if (m_bEnabled && (m_bInScene || m_MaxLife >= 0) && (m_MaxLife < 0.0f || m_RemainingLife > 0.0f) )
				{				
					numparticles = m_EmitterUpdatedVariations[ EMITTER_NUMBER ] *
									GetRandomValue( CurKeyInstance->m_UpdatedVariations[ PARTICLE_NUMBER ],
													CurKeyInstance->m_UpdatedVariations[ PARTICLE_NUMBER_VARIATION ] );
					//depending on the frames per second, numparticles is a per second value, float rounding will reduce
					//any fractional value so in order to make sure the particle actually begins when first started.
					if( m_bFirstUpdate )
					{
						if( numparticles > 0.0f && numparticles < 1.0f)
						{
							CurKeyInstance->m_NumParticles = 1.0f;
						}
					}
					numparticles *= TimeDeltaSeconds;
				}
				else
				{
					numparticles = 0.0f;
				}
				CurKeyInstance->m_NumParticles += numparticles;

				//spawn the particles:
				PARTICLE p;
				memset( p.currentkeyframe,0,sizeof( p.currentkeyframe ) );
				p.curlife = 0;
				if( CurKeyInstance->m_NumParticles >= 1.f )
				{
					CurKeyInstance->m_NumParticles = 0;
					//get variation keyframes
					UPDATE_VARIABLE( unitlife, PARTICLE_LIFE_VARIATION );	//
					UPDATE_VARIABLE( unitlife, PARTICLE_SIZE_VARIATION );	//			
					UPDATE_VARIABLE( unitlife, PARTICLE_VELOCITY_VARIATION );	//			
					UPDATE_VARIABLE( unitlife, PARTICLE_SPIN_VARIATION );	//			
					UPDATE_VARIABLE( unitlife, PARTICLE_WEIGHT_VARIATION );		//	
					UPDATE_VARIABLE( unitlife, PARTICLE_MOTION_RAND_VARIATION ); //
					UPDATE_VARIABLE( unitlife, PARTICLE_LIFE ); //
					UPDATE_VARIABLE( unitlife, PARTICLE_MOTION_RAND ); //
					UPDATE_VARIABLE( unitlife, PARTICLE_VELOCITY ); //
					UPDATE_VARIABLE( unitlife, PARTICLE_SIZE ); //
					UPDATE_VARIABLE( unitlife, PARTICLE_SPIN ); //
					UPDATE_VARIABLE( unitlife, PARTICLE_WEIGHT ); //
					
							

					for( int i = 0; i < numparticles && m_Particles[ pi ].size() < maxbrushparticles; i++ )
					{
						//Set Start Location
						p.x = m_Position.x;
						p.y = m_Position.y;
						p.z = m_Position.z;
						//this should be some angle from the emitter object

						//Set Start OutVector
						Vec3 Out(0.0f,0.0f,0.0f);
						if ((int)m_EmitterUpdatedVariations[ EMITTER_EMISSION_RANGE ] > 0 )
						{
							float phi = (float)(rand()%(2*(int)m_EmitterUpdatedVariations[ EMITTER_EMISSION_RANGE ]) - m_EmitterUpdatedVariations[ EMITTER_EMISSION_RANGE ]);
							float theta = (float)(rand()%360);

							static float DegToRad =  3.14159265f * (1.f/180.f);
							float sinPhi = sin(DegToRad * phi);
							float yrand = sinPhi*cos(DegToRad * theta);
							float xrand = cos(DegToRad * phi);
							float zrand = sinPhi*sin(DegToRad * theta);

							Out.z = xrand;
							Out.x = yrand;
							Out.y = zrand;
							Out.Normalize();
						}
						else
							Out.Set(-1,0,0);
						Out = m_mat4ParentTransform.TransformDirection(Out);

						p.velocityX = Out.x;
						p.velocityY = Out.y;
						p.velocityZ = Out.z;
						p.weightvelocity = 0;
						p.curframe = 0;
						if (CurParticleType->GetTextureRate() == 0.0f)
						{
							p.bRandomInitialFrame = true;
							p.frametimer = p.frametimermax = 0;
							bool animatedtexture = (CurParticleType->m_Texture && CurParticleType->m_Texture->GetTextureType() == EE_TEXTUREANIMATED2D );
							if (animatedtexture)
							{
								IAnimatedTextureObject *at = (IAnimatedTextureObject*)CurParticleType->m_Texture;
								p.curframe = (rand() % at->GetNumFrames());
							}
						}
						else
						{
							p.bRandomInitialFrame = false;
							p.frametimermax = 1.f/CurParticleType->GetTextureRate(); 
							p.frametimer = p.frametimermax; //subtract to 0, increment curframe, reset to max, just for speed so no divides
						}
						
						p.variations[ PK_VELOCITY_VARY ] =  g_IllusionTimeMultiplier*
															m_EmitterUpdatedVariations[ EMITTER_ZOOM ] *
															m_EmitterUpdatedVariations[ EMITTER_VELOCITY ] *
//															motionrandom + GetRandomValue( CurKeyInstance->m_UpdatedVariations[ PARTICLE_VELOCITY ], CurKeyInstance->m_UpdatedVariations[ PARTICLE_VELOCITY_VARIATION ] );					
															GetRandomValue( CurKeyInstance->m_UpdatedVariations[ PARTICLE_VELOCITY ], CurKeyInstance->m_UpdatedVariations[ PARTICLE_VELOCITY_VARIATION ] );					
						p.variations[ PK_SIZE_VARY ] = m_EmitterUpdatedVariations[ EMITTER_ZOOM ] *
														m_EmitterUpdatedVariations[ EMITTER_SIZE ] *
														GetRandomValue( CurKeyInstance->m_UpdatedVariations[ PARTICLE_SIZE ], CurKeyInstance->m_UpdatedVariations[ PARTICLE_SIZE_VARIATION ] );
						p.variations[ PK_SPIN_VARY ] = rad2deg *
															m_EmitterUpdatedVariations[ EMITTER_SPIN ] *
														GetRandomValue( CurKeyInstance->m_UpdatedVariations[ PARTICLE_SPIN ], CurKeyInstance->m_UpdatedVariations[ PARTICLE_SPIN_VARIATION ] );
						p.variations[ PK_WEIGHT_VARY ] =  g_IllusionTimeScale * m_EmitterUpdatedVariations[ EMITTER_WEIGHT ] *
//														motionrandom + GetRandomValue( CurKeyInstance->m_UpdatedVariations[ PARTICLE_WEIGHT ], CurKeyInstance->m_UpdatedVariations[ PARTICLE_WEIGHT_VARIATION ] );
														GetRandomValue( CurKeyInstance->m_UpdatedVariations[ PARTICLE_WEIGHT ], CurKeyInstance->m_UpdatedVariations[ PARTICLE_WEIGHT_VARIATION ] );
						p.variations[ PK_LIFE_VARY ] =  g_IllusionTimeScale*
														m_EmitterUpdatedVariations[ EMITTER_LIFE ] *
														GetRandomValue( CurKeyInstance->m_UpdatedVariations[ PARTICLE_LIFE ], CurKeyInstance->m_UpdatedVariations[ PARTICLE_LIFE_VARIATION ] );
						//don't create a particle with 0 life
						if( p.variations[ PK_LIFE_VARY ] <= 0 )
						{
							//do nothing?
						}
						else if( m_OldestFirst )
						{
							m_Particles[ pi ].push_front( p );
						}else
						{
							m_Particles[ pi ].push_back( p );
						}
					}
				}
			}
		}else // else here means there are no extant particles
		{
			// If a paticle was spawned disabled, assume manual deletion, do not auto delete.
			if (m_bStartEnabled) 
            	bReturn = false;
		}
	}
	if( bReturn && 
		m_ParticleEmitter )
	{
		int vertoffset = 0;
		//pick particle type:
		for( unsigned int pi = 0; pi < m_KeyInstances.size(); pi++ )
		{
			CurKeyInstance = &m_KeyInstances[ pi ];
			CurParticleType = (CParticleType*)m_ParticleEmitter->GetBrush( pi );
			if( CurParticleType == NULL )
			{
				continue;
			}
//			if( m_ScreenAligned )
			RenderToBuffer( pi, vertoffset);
		}
	}

	if (m_bFirstUpdate)
		m_bFirstUpdate = false;
	return bReturn;
}

void CParticleRenderObject::RenderToBuffer(int brushindex, int &vertoffset)
{
	// we don't need to render to buffer if this object lasts forever and isn't currently in the scene
	if (!m_bInScene && m_MaxLife < 0)
		return;

	//Update particles
	list< PARTICLE >::iterator iter = m_Particles[ brushindex ].begin();

	float OutRGBA[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	float OutVelocity = 0.0f;
	float OutSize = 0.0f;
	float OutRotation = 0.0f;
	float OutWeight = 0.0f;
	float OutMotionRand = 0.0f;
//	float OutTint[4];
//	m_ParticleEmitter->GetTint( OutTint );
	//tint strength is how close to (1,1,1,1) the colors are tinted. The more the strength the closer
	//to the tint value, otherwise, it will be a similar color. Note tint is probabl better calculated
	//as an interpolation using tint strength, but that's just too much added calculatioin per particle!
//	OutTint[ 0 ] += (1.f - OutTint[ 0 ])*( 1.f - m_EmitterUpdatedVariations[ EMITTER_TINT_STRENGTH ]);
//	OutTint[ 1 ] += (1.f - OutTint[ 1 ])*( 1.f - m_EmitterUpdatedVariations[ EMITTER_TINT_STRENGTH ]);
//	OutTint[ 2 ] += (1.f - OutTint[ 2 ])*( 1.f - m_EmitterUpdatedVariations[ EMITTER_TINT_STRENGTH ]);
//	OutTint[ 3 ] += (1.f - OutTint[ 3 ])*( 1.f - m_EmitterUpdatedVariations[ EMITTER_TINT_STRENGTH ]);

	bool animatedtexture = 	( CurParticleType->m_Texture &&
								CurParticleType->m_Texture->GetTextureType() == EE_TEXTUREANIMATED2D );
	int CurVerticesWritten = vertoffset;
	float tdeltaseconds = m_TimeDelta*.001f;
	for( ;iter != m_Particles[ brushindex ].end();)
	{
		PARTICLE &r = (*iter);
		r.curlife += (float)tdeltaseconds;
		float liferatio = r.curlife / r.variations[ PK_LIFE_VARY ];
		if( liferatio > 1.f )
		{
			iter = m_Particles[ brushindex ].erase( iter );
		}
		else
		{
			++iter;
			SETCURRENTKEY_ANDINTERPOLATE( OutVelocity, r.currentkeyframe[ PK_VELOCITY_LIFE ] , liferatio, PARTICLE_VELOCITY_LIFE);					 
				OutVelocity *= r.variations[ PK_VELOCITY_VARY ] ;
			SETCURRENTKEY_ANDINTERPOLATE( OutSize, r.currentkeyframe[ PK_SIZE_LIFE ] , liferatio, PARTICLE_SIZE_LIFE );
				OutSize *= r.variations[ PK_SIZE_VARY];
			SETCURRENTKEY_ANDINTERPOLATE( OutRotation, r.currentkeyframe[ PK_SPIN_LIFE ] , liferatio, PARTICLE_SPIN_LIFE );
				OutRotation *=  r.variations[ PK_SPIN_VARY];
			SETCURRENTKEY_ANDINTERPOLATE( OutWeight, r.currentkeyframe[ PK_WEIGHT_LIFE ] , liferatio, PARTICLE_WEIGHT_LIFE );
				OutWeight *=  r.variations[ PK_WEIGHT_VARY];
			SETCURRENTKEY_ANDINTERPOLATE( OutMotionRand, r.currentkeyframe[ PK_MOTION_RAND_LIFE ] , liferatio, PARTICLE_MOTION_RAND_LIFE );
			//calculate velocity and position
			Vec3 v3RandomMotion(0.0,0.0,0.0);
			v3RandomMotion.x = GetRandomValue();
			v3RandomMotion.y = GetRandomValue();
			v3RandomMotion.z = GetRandomValue();
			v3RandomMotion = v3RandomMotion * OutMotionRand;

			float velocitymultiplier = OutVelocity*tdeltaseconds;
			r.x += m_InitialVelocity.x + r.velocityX*velocitymultiplier + m_LastPositionOffset.x*CurParticleType->m_fAttachToEmitter + v3RandomMotion.x;//attach to emitter
			r.y += m_InitialVelocity.y + r.velocityY*velocitymultiplier + m_LastPositionOffset.y*CurParticleType->m_fAttachToEmitter + v3RandomMotion.y;
			r.z += m_InitialVelocity.z + r.velocityZ*velocitymultiplier + m_LastPositionOffset.z*CurParticleType->m_fAttachToEmitter + v3RandomMotion.z;
			r.weightvelocity += OutWeight*tdeltaseconds;
			r.y -= r.weightvelocity*tdeltaseconds;

			SETCURRENTKEY_ANDINTERPOLATE( OutRGBA[ 0 ], r.currentkeyframe[ PK_COLOR_R_LIFE ] , liferatio, PARTICLE_COLOR_R );
			SETCURRENTKEY_ANDINTERPOLATE( OutRGBA[ 1 ], r.currentkeyframe[ PK_COLOR_G_LIFE ] , liferatio, PARTICLE_COLOR_G );
			SETCURRENTKEY_ANDINTERPOLATE( OutRGBA[ 2 ], r.currentkeyframe[ PK_COLOR_B_LIFE ] , liferatio, PARTICLE_COLOR_B );
			SETCURRENTKEY_ANDINTERPOLATE( OutRGBA[ 3 ], r.currentkeyframe[ PK_COLOR_A_LIFE ] , liferatio, PARTICLE_COLOR_A );
//			OutRGBA[ 0 ] *= OutTint[ 0 ];
//			OutRGBA[ 1 ] *= OutTint[ 1 ];
//			OutRGBA[ 2 ] *= OutTint[ 2 ];
//			OutRGBA[ 3 ] *= OutTint[ 3 ];
			
			Clamp( OutRGBA[ 0 ], 0.f, 1.f );
			Clamp( OutRGBA[ 1 ], 0.f, 1.f );
			Clamp( OutRGBA[ 2 ], 0.f, 1.f );
			Clamp( OutRGBA[ 3 ], 0.f, 1.f );

			DWORD OutColor = m_IRenderer->GetColorARGB(	OutRGBA[ 3 ], OutRGBA[ 0 ],OutRGBA[ 1 ], OutRGBA[ 2 ] );
			m_ParticleBuffer[ CurVerticesWritten + 0 ].Color = OutColor;
			m_ParticleBuffer[ CurVerticesWritten + 1 ].Color = OutColor;
			m_ParticleBuffer[ CurVerticesWritten + 2 ].Color = OutColor;
			m_ParticleBuffer[ CurVerticesWritten + 5 ].Color = OutColor;

			//check animated textures:
			if( animatedtexture )
			{
				//update particle animation
				IAnimatedTextureObject * at = (IAnimatedTextureObject * )CurParticleType->m_Texture;
				if (!r.bRandomInitialFrame)
				{
					r.frametimer -= tdeltaseconds;
					if( r.frametimer < 0 )
					{
						r.curframe++;
						if( r.curframe > at->GetNumFrames())
						{
							if( CurParticleType->m_bTextureLoop )
							{
								r.curframe = 0;						
							}else
							{
								r.curframe = at->GetNumFrames() - 1;
							}
						}
						r.frametimer = r.frametimermax;
					}
				}
				//now update uv
				UINT frame = r.curframe;
				float u1, u2, v1, v2;
				at->GetFrame( frame, u1, v1, u2, v2, NULL );
				//now fill uv
				m_ParticleBuffer[ CurVerticesWritten  + 0 ].TexCoord[0] = u1;
				m_ParticleBuffer[ CurVerticesWritten  + 0 ].TexCoord[1] = v1;
				m_ParticleBuffer[ CurVerticesWritten  + 1 ].TexCoord[0] = u1;
				m_ParticleBuffer[ CurVerticesWritten  + 1 ].TexCoord[1] = v2;				
				m_ParticleBuffer[ CurVerticesWritten  + 2 ].TexCoord[0] = u2;
				m_ParticleBuffer[ CurVerticesWritten  + 2 ].TexCoord[1] = v1;			
				m_ParticleBuffer[ CurVerticesWritten  + 5 ].TexCoord[0] = u2;
				m_ParticleBuffer[ CurVerticesWritten  + 5 ].TexCoord[1] = v2;
			}

			OutRotation = math.DegToRad( OutRotation );
			m_ParticleBuffer[ CurVerticesWritten + 0 ].Rotation.x = OutRotation;
			m_ParticleBuffer[ CurVerticesWritten + 1 ].Rotation.x = OutRotation;
			m_ParticleBuffer[ CurVerticesWritten + 2 ].Rotation.x = OutRotation;
			m_ParticleBuffer[ CurVerticesWritten + 5 ].Rotation.x = OutRotation;

			m_ParticleBuffer[ CurVerticesWritten + 0 ].PosOffset.Set( 0, OutSize, 0 ); // up
			m_ParticleBuffer[ CurVerticesWritten + 1 ].PosOffset.Set( OutSize, 0, 0 ); // right
			m_ParticleBuffer[ CurVerticesWritten + 2 ].PosOffset.Set( -OutSize, 0, 0 ); // left
			m_ParticleBuffer[ CurVerticesWritten + 5 ].PosOffset.Set( 0, -OutSize, 0 ); // down

			m_ParticleBuffer[ CurVerticesWritten + 0 ].Position.Set(r.x, r.y, r.z);
			m_ParticleBuffer[ CurVerticesWritten + 1 ].Position.Set(r.x, r.y, r.z);
			m_ParticleBuffer[ CurVerticesWritten + 2 ].Position.Set(r.x, r.y, r.z);
			m_ParticleBuffer[ CurVerticesWritten + 5 ].Position.Set(r.x, r.y, r.z);

			m_ParticleBuffer[ CurVerticesWritten  + 3 ] = m_ParticleBuffer[ CurVerticesWritten  + 2 ];
			m_ParticleBuffer[ CurVerticesWritten  + 4 ] = m_ParticleBuffer[ CurVerticesWritten  + 1 ];

			CurVerticesWritten += 6;
		}
	}
	CurKeyInstance->m_VerticesWritten = CurVerticesWritten - vertoffset;
	vertoffset = CurVerticesWritten;
}

/// \brief Update the render object( Render )
/// \return true or false
bool CParticleRenderObject::Render( UINT pass, IEffect * override )
{
	PERFORMANCE_PROFILER_AUTO(CParticleRenderObject_Render);
	if( m_ParticleEmitter && m_pEffect )
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

		//Make depth test false for soft particles
		//m_IRenderer->SetDepthTest( false );
		m_IRenderer->SetDepthTest( true );
		m_IRenderer->SetDepthWrite( false );	
		m_IRenderer->SetAlphaTest( true );
		DWORD alpharef = 0x00000001;
		m_IRenderer->SetRenderState( RENDERSTATE_ALPHAREF, (UINT)alpharef );
		m_IRenderer->SetRenderState( RENDERSTATE_ALPHABLENDENABLE, RENDERSTATEPARAM_TRUE );

		TEXTUREOBJECTPARAMS depTex;
		depTex.TextureObjectInterface = NULL;
		static DWORD msg_GetDepthTexture = CHashString( _T("GetDepthTargetAsTexture") ).GetUniqueID();
		m_ToolBox->SendMessage( msg_GetDepthTexture, sizeof( TEXTUREOBJECTPARAMS ), &depTex );
	
		//pick particle type:
		UINT curoffset = 0;
		for(  int pi = (int)m_KeyInstances.size() - 1; pi >= 0; pi-- )
		{
			CurKeyInstance = &(m_KeyInstances[ pi ]);
			CurParticleType = (CParticleType*)m_ParticleEmitter->GetBrush( pi );
			if( CurParticleType == NULL )
			{
				continue;
			}
			if( CurParticleType->m_bAdditiveBlend )
			{
				m_IRenderer->SetBlendMode( BLEND_ADDITIVE );		

			}else
			{
				m_IRenderer->SetBlendMode( BLEND_NORMAL );	
			}
			m_IRenderer->SetRenderState( RENDERSTATE_CULLMODE, RENDERSTATEPARAM_CULLNONE );
			ITextureObject * texture = (ITextureObject*)CurParticleType->m_Texture;
			if( texture && 
				texture->GetTextureType() == EE_TEXTUREANIMATED2D )
			{
				IAnimatedTextureObject * at = (IAnimatedTextureObject *)CurParticleType->m_Texture;
				float u;
				at->GetFrame( 0, u,u,u,u, &texture );
			}
			if( CurKeyInstance->m_VerticesWritten > 0 )
			{
				//render once then set buffer false
				//m_IRenderer->SetMaterial( pass, CurParticleType->m_Material );
				m_IRenderer->SetTexture( 0, texture );
				m_IRenderer->SetTexture( 1, depTex.TextureObjectInterface );
				m_IRenderer->RenderVertexBuffer( m_VB,
					m_VBOffset + CurKeyInstance->m_CurOffset,
					CurKeyInstance->m_VerticesWritten/3 );
//				char buf[512];
//				sprintf( buf, "Render off: %d, verts: %d, tex: %d\n", m_VBOffset + CurKeyInstance->m_CurOffset, CurKeyInstance->m_VerticesWritten/3, texture );
//				OutputDebugString(buf);
			}
		}
//		OutputDebugString("RENDEREND\n");
		m_IRenderer->SetBlendMode( BLEND_NORMAL );
		m_IRenderer->SetDepthTest( true );
		m_IRenderer->SetDepthWrite( true );
	}
	return true;
}

/// \brief Set position of the render object
void CParticleRenderObject::SetPosition( const Vec3 &vec )
{
	// update the render object position to any manager that needs to know when it moves, i.e. the quad tree
	// manager
	m_Position.Set( vec );
/*
	if( !m_LastPositionSet )
	{
		m_LastPositionOffset = Vec3(0,0,0);
		m_LastPositionSet = true;
	}
	else
	{		
		m_LastPositionOffset = m_Position - m_LastPosition;
	}

	m_LastPosition = m_Position;
*/
	UpdateBoundingObject();
}

/// \brief Set Orientation of the render object
void CParticleRenderObject::SetRotation( const EulerAngle &euler )
{	
	m_Rotation.Set( euler );
	//m_RenderObject->SetOrientation( (VisVector_cl)m_Rotation);
}

/// \brief Set Scaling of the render object
void CParticleRenderObject::SetScaling( const Vec3 &vec )
{
	m_Scaling.Set( vec );
	//m_RenderObject->SetScaling( (VisVector_cl)m_Scaling);
}

Vec3 &CParticleRenderObject::GetPosition()
{
	return m_Position;
}

void CParticleRenderObject::GetBoundingSphere( Vec3 &position, float &radius )
{
	position = m_Position;
	radius = m_ClipDistance;
}

bool CParticleRenderObject::IsAlphaObject()
{	
	return true;
}

bool CParticleRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{	
	//order for particles does not matter
	return ( false );
}

void CParticleRenderObject::DeInit()
{
	
}


/// \brief either reads or writes the entity information to a file
/// Currently Loads up the entity completely if reading(inside vision too)
void CParticleRenderObject::Serialize(IArchive &ar)
{
	// Get the data's version number
	CHashString hszVersion = _T("");

	static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &hszVersion);
	if (retval != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGERROR, _T("Particle Render Object Serialize: Could not get file version!"));
		assert(0);
		return;
	}
	float version = (float)_tstof(hszVersion.GetString());

	//create
	//Need to save off:
	//position
	//rotation
	//scaling
	//file name
	//color tints and any other attributes
	//checking vision to see what attributes need saving
	if (ar.IsReading())
	{		
		ar.Read( m_Filename );
		m_ParticleTypeName.Init(m_Filename);

		if ( version < 2.5 )
		{
			ar.Read( m_Position );
			ar.Read( m_Rotation );
			ar.Read( m_Scaling );
		}
		else
		{
			m_Position.Set(0.0,0.0,0.0);
			m_Rotation.Set(0.0,0.0,0.0);
			m_Scaling.Set(1.0,1.0,1.0);
		}
		m_LocalTransform.SetTranslation( m_Position );
		m_LocalTransform.SetEuler( m_Rotation );
		m_LocalTransform.SetScale( m_Scaling );
		m_mat4ParentTransform.SetFrom4x4(m_LocalTransform.GetTransform().m);

		ar.Read( m_bEnabled );
		// Set Initial Logic.

// 		ar.Write( m_LocalTransform.GetTranslation(), _T("Position"));
// 		ar.Write( m_LocalTransform.GetEuler() , _T("Rotation") );
// 		ar.Write( m_LocalTransform.GetScale() , _T("Scaling") );	
		m_bStartEnabled = m_bEnabled;
		ar.Read( m_ClipDistance);
		ar.Read( m_MaxLife );
		ar.Read( m_ScreenAligned );
		m_RemainingLife = m_MaxLife;
	}
	else
	{
		//ar.StartClass(_T("CParticleRenderObject"));
		ar.Write( m_Filename, _T("FileName") );
//		ar.Write( m_Position, _T("Position"));
//		ar.Write( m_Rotation, _T("Rotation") );
//		ar.Write( m_Scaling, _T("Scaling") );		
		ar.Write( m_bEnabled , _T("Enabled"));
		ar.Write( m_ClipDistance, _T("ClipDistance"));
		ar.Write( m_MaxLife, _T("RemaingLife") );
		ar.Write( m_ScreenAligned, _T("ScreenAligned") );
		//ar.EndClass();
	}
}

IHashString * CParticleRenderObject::GetBaseSortClass()
{
	static CHashString CRO(_T("ParticleRenderObject"));
	return &CRO;
}

IMeshObject * CParticleRenderObject::GetMeshObject()
{
	
	return NULL;
}

DWORD CParticleRenderObject::OnGetParticleEmitterInterface(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(IParticleEmitter **), size);
	IParticleEmitter** p = (IParticleEmitter**)params;
	*p = m_ParticleEmitter;
	return MSG_HANDLED_STOP;
}

DWORD CParticleRenderObject::OnDisable(DWORD size, void *params)
{
	m_bEnabled = false;
	return MSG_HANDLED_PROCEED;
}

DWORD CParticleRenderObject::OnEnable(DWORD size, void *params)
{
	m_bEnabled = true;
	//Grab current time	
	m_RemainingLife = m_MaxLife;
	memset( m_EmitterKeyFrames, 0, sizeof( m_EmitterKeyFrames) );
	memset( m_EmitterUpdatedVariations, 0, sizeof( m_EmitterUpdatedVariations) );
	// After being disabled, the first frame will fail to render without reseting first update due to the m_LastTickCount - m_CurrentTick
	m_bFirstUpdate = true;
	return MSG_HANDLED_PROCEED;
}

DWORD CParticleRenderObject::OnResetParticle(DWORD size, void *params)
{
	m_RemainingLife = m_MaxLife;

	m_KeyInstances.clear();
	if (m_ParticleEmitter)
	{
		UINT numbrushes = m_ParticleEmitter->GetNumBrushes();
		m_KeyInstances.resize( numbrushes );
		for( UINT i = 0; i < m_Particles.size(); i++ )
		{
			m_Particles[ i ].clear();
		}
		m_Particles.resize( numbrushes );
	}
	memset( m_EmitterKeyFrames, 0, sizeof( m_EmitterKeyFrames) );
	memset( m_EmitterUpdatedVariations, 0, sizeof( m_EmitterUpdatedVariations) );
	// After being disabled, the first frame will fail to render without reseting first update due to the m_LastTickCount - m_CurrentTick
	m_bFirstUpdate = true;
	return MSG_HANDLED_STOP;
}
float CParticleRenderObject::GetTime()
{
	float time;
//	static DWORD msgHash_GetTime = CHashString(_T("GetTime")).GetUniqueID();
//	m_ToolBox->SendMessage(msgHash_GetTime,sizeof(float),(void*)&time );
	CHashString hszGameDeltaTimer(_T("GameDeltaTimer"));
	time = m_Timer->GetTimeDelta(&hszGameDeltaTimer);
	return time;
}

DWORD CParticleRenderObject::OnSetParticleTime(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( float ) );
	float settime = *(float *)params;
	settime /= g_IllusionTimeScale;

	m_RemainingLife = m_MaxLife - settime;
	memset( m_EmitterKeyFrames, 0, sizeof( m_EmitterKeyFrames) );
	memset( m_EmitterUpdatedVariations, 0, sizeof( m_EmitterUpdatedVariations) );
	return MSG_HANDLED_STOP;
}

DWORD CParticleRenderObject::OnSetParticleInitialVelocity(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	m_InitialVelocity = *(Vec3*)params;

	return MSG_HANDLED_STOP;
}

/// \brief	Get the bounding box of the object
bool CParticleRenderObject::GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation )
{
	Vec3 vMin, vMax;
	
	pos.x = m_Position.x;
	pos.y = m_Position.y;
	pos.z = m_Position.z;
	
	return false;
}

bool CParticleRenderObject::IsKindOf( IHashString * compType )
{	
	static CHashString  CPROCOMP(_T("CParticleRenderObject") );
	if(CRenderObject<>::IsKindOf(compType)) 
	{
		return true;
	}
	return (compType->GetUniqueID() == CPROCOMP.GetUniqueID());
}

void  CParticleRenderObject::SETCURRENTKEY(  int &key, float timevar, int var )
{	
// Modifications to the graph in the editor can result in the Render Object being on a key that no longer exists,
// make sure the current keyframe is within bounds of the key data and handle looping.
// ToDo: make sure these if statements do not need to exist, they are SLOW.
	// make sure the current keyframe is within bounds of the key data and handle looping
	while (key >= CurParticleType->m_KeyData[var].m_Size)
	{
			key = CurParticleType->m_KeyData[var].m_Size - 1; // set emitter variable to last frame
	}

	while(	(CurParticleType->m_KeyData[ var ].m_Keys != NULL ) && 
			((CurParticleType->m_KeyData[ var ].m_Size) - 1 != key) && 
			(CurParticleType->m_KeyData[ var ].m_Keys[ (key + 1 )].time <= timevar) ) 
		{ 
			key++;
		}
}

float  CParticleRenderObject::KEYFRAME_INTERPOLATE(  int &key, float time, int var )
{
/*	assert( var < PARTICLE_VARIABLE_MAX && var >= 0 );
	int nextkey = ( key == CurParticleType->m_KeyData[ var ].m_Size - 1 ) ? key : key + 1 ;
	float value = KEYFRAME_DATA::Interpolate( time, CurParticleType->m_KeyData[ var ].m_Keys[ key ],
        CurParticleType->m_KeyData[ var ].m_Keys[ nextkey ] );
	return value;
*/
	float ret = KEYFRAME_DATA::Interpolate( time, 
		CurParticleType->m_KeyData[ var ].m_Keys[ key ],
		CurParticleType->m_KeyData[ var ].m_Keys[ (( key == CurParticleType->m_KeyData[ var ].m_Size - 1 ) ? key : key + 1 )] );

	return ret;
}

void  CParticleRenderObject::SETCURRENTKEY_ANDINTERPOLATE( float &out, int &key, float timevar, int var )
{
	SETCURRENTKEY( key, timevar, var );
	out = KEYFRAME_INTERPOLATE( key, timevar, var );

}

void  CParticleRenderObject::UPDATE_VARIABLE( float timevar, int var )
{
	SETCURRENTKEY( CurKeyInstance->m_KeyFrames[ var ], timevar, var );
	CurKeyInstance->m_UpdatedVariations[ var ] = KEYFRAME_INTERPOLATE( CurKeyInstance->m_KeyFrames[ var ], timevar, var );
}

void  CParticleRenderObject::UPDATE_EMITTER_VARIABLE( float timevar, int var )
{
	assert( var < EMITTER_VARIABLE_MAX && var >= 0 );

	// make sure the current keyframe is within bounds of the key data and handle looping
	if (m_EmitterKeyFrames[var] >= m_ParticleEmitter->m_KeyData[var].m_Size)
	{
			m_EmitterKeyFrames[var] = m_ParticleEmitter->m_KeyData[var].m_Size - 1; // set emitter variable to last frame
	}

	while(	(m_ParticleEmitter->m_KeyData[ var ].m_Keys != NULL ) &&
        	((m_ParticleEmitter->m_KeyData[ var ].m_Size) - 1 != m_EmitterKeyFrames[ var ]) &&
			(m_ParticleEmitter->m_KeyData[ var ].m_Keys[ (m_EmitterKeyFrames[ var ] + 1 )].time <= timevar) )
			{
                m_EmitterKeyFrames[ var ]++;
			}
			m_EmitterUpdatedVariations[ var ] = 
						KEYFRAME_DATA::Interpolate( timevar,
								m_ParticleEmitter->m_KeyData[ var ].m_Keys[ m_EmitterKeyFrames[ var ] ],
								m_ParticleEmitter->m_KeyData[ var ].m_Keys[ (( m_EmitterKeyFrames[ var ] == m_ParticleEmitter->m_KeyData[ var ].m_Size - 1 ) ? m_EmitterKeyFrames[ var ] : m_EmitterKeyFrames[ var ] + 1 )] );
}
// 
// ENTITY MESSAGES
//
REGISTER_MESSAGE_HANDLER( SetGlobalPosition, OnSetGlobalPosition, CParticleRenderObject );
REGISTER_MESSAGE_HANDLER( SetGlobalEuler, OnSetGlobalEuler, CParticleRenderObject );
REGISTER_MESSAGE_HANDLER( SetGlobalQuaternion, OnSetGlobalQuaternion, CParticleRenderObject );
REGISTER_MESSAGE_HANDLER( SetGlobalScale, OnSetGlobalScale, CParticleRenderObject );
REGISTER_MESSAGE_HANDLER( SetGlobalTransform, OnSetGlobalTransform, CParticleRenderObject );

REGISTER_MESSAGE_HANDLER( SetLocalPosition, OnSetLocalPosition, CParticleRenderObject );
REGISTER_MESSAGE_HANDLER( SetLocalEuler, OnSetLocalEuler, CParticleRenderObject );
REGISTER_MESSAGE_HANDLER( SetLocalQuaternion, OnSetLocalQuaternion, CParticleRenderObject );
REGISTER_MESSAGE_HANDLER( SetLocalScale, OnSetLocalScale, CParticleRenderObject );
REGISTER_MESSAGE_HANDLER( SetLocalTransform, OnSetLocalTransform, CParticleRenderObject );

REGISTER_MESSAGE_HANDLER( AttachSceneObject, OnAttachSceneObject, CParticleRenderObject );

void CParticleRenderObject::UpdateTransform()
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
	//		SetScaling( m_mat4ParentTransform.GetScale());
	// The transform is now clean
	m_bTransformIsDirty = false;
}
/*
void CParticleRenderObject::SetChildrenDirtyTransformFlags()
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


DWORD CParticleRenderObject::OnSetGlobalPosition( DWORD size, void* params )
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

//	SetChildrenDirtyTransformFlags(); // TODO: NO CHILDREN FOR PARTICLEEMITTERS MAYBE PARTICLE SYSTEMS

	UpdateTransform();

	return MSG_HANDLED_STOP;
}

DWORD CParticleRenderObject::OnSetGlobalEuler( DWORD size, void* params )
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
	
//	SetChildrenDirtyTransformFlags(); // TODO: NO CHILDREN FOR PARTICLEEMITTERS MAYBE PARTICLE SYSTEMS

	m_bTransformIsDirty = false;

	return MSG_HANDLED_STOP;
}

DWORD CParticleRenderObject::OnSetGlobalQuaternion( DWORD size, void* params )
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
	
//	SetChildrenDirtyTransformFlags(); // TODO: NO CHILDREN FOR PARTICLEEMITTERS MAYBE PARTICLE SYSTEMS

	m_bTransformIsDirty = false;

	return MSG_HANDLED_STOP;
}

DWORD CParticleRenderObject::OnSetGlobalScale( DWORD size, void* params )
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
	
//	SetChildrenDirtyTransformFlags(); // TODO: NO CHILDREN FOR PARTICLEEMITTERS MAYBE PARTICLE SYSTEMS

	m_bTransformIsDirty = false;

	return MSG_HANDLED_STOP;
}

DWORD CParticleRenderObject::OnSetGlobalTransform( DWORD size, void* params )
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

//	SetChildrenDirtyTransformFlags(); // TODO: NO CHILDREN FOR PARTICLEEMITTERS MAYBE PARTICLE SYSTEMS

	m_bTransformIsDirty = false;

	return MSG_HANDLED_STOP;
}

DWORD CParticleRenderObject::OnSetLocalPosition( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* position = (Vec3*)params;

	m_LocalTransform.SetTranslation( *position );

	// Since we are setting the local position the global position is now dirty
	SetDirtyTransformFlag();

	return MSG_HANDLED_STOP;
}

DWORD CParticleRenderObject::OnSetLocalEuler( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(EulerAngle) );
	EulerAngle* euler = (EulerAngle*)params;

	m_LocalTransform.SetEuler( *euler );

	// Since we are setting the local position the global position is now dirty
	SetDirtyTransformFlag();

	return MSG_HANDLED_STOP;
}

DWORD CParticleRenderObject::OnSetLocalQuaternion( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Quaternion) );
	Quaternion* quaternion = (Quaternion*)params;

	m_LocalTransform.SetQuaternion( *quaternion );

	// Since we are setting the local position the global position is now dirty
	SetDirtyTransformFlag();

	return MSG_HANDLED_STOP;
}

DWORD CParticleRenderObject::OnSetLocalScale( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* scale = (Vec3*)params;

	m_LocalTransform.SetScale( *scale );

	// Since we are setting the local position the global position is now dirty
	SetDirtyTransformFlag();

	return MSG_HANDLED_STOP;
}

DWORD CParticleRenderObject::OnSetLocalTransform( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Matrix4x4) );
	Matrix4x4* transform = (Matrix4x4*)params;

	m_LocalTransform.SetTransform( *transform );

	// Since we are setting the local position the global position is now dirty
	SetDirtyTransformFlag();

	return MSG_HANDLED_STOP;
}

DWORD CParticleRenderObject::OnAttachSceneObject(DWORD size, void *param)
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
DWORD CParticleRenderObject::OnReloadTextures(DWORD size, void *params)
{
	// THIS IS FILTHY CODE, DO NOT COPY AND PASTE ANYWHERE.
	int nBrushes = m_ParticleEmitter->GetNumBrushes();
	for (int i = 0; i < nBrushes; i++)
	{
		CParticleType * pParticle = (CParticleType*)m_ParticleEmitter->GetBrush(i);
		// THIS IS FILTHY CODE, DO NOT COPY AND PASTE ANYWHERE.
		const TCHAR * t = m_ParticleEmitter->GetBrush(i)->GetTextureFilename();
		CHashString fullname( t );
		
		IBaseTextureObject * p_OldTexture = pParticle->m_Texture;
		// THIS IS FILTHY CODE, DO NOT COPY AND PASTE ANYWHERE.
		TEXTUREOBJECTPARAMS tobj;
		tobj.bLoad = true;
		tobj.Name = &fullname;
		tobj.TextureObjectInterface = NULL;
		// THIS IS FILTHY CODE, DO NOT COPY AND PASTE ANYWHERE.
		static DWORD msgHash_AddTexture = CHashString(_T("AddTexture")).GetUniqueID();
		DWORD res = EngineGetToolBox()->SendMessage(msgHash_AddTexture, sizeof(tobj), &tobj );
		if( res == MSG_HANDLED )
		{
			// store the texture id in the user data of the map
			pParticle->m_Texture = (IBaseTextureObject *)tobj.TextureObjectInterface;

			// DELETE OLD TEXTURE
/*
			DELETEOBJECTPARAMS dop;
			dop.name = p_OldTexture->GetName();
			static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
			DWORD retVal = m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop );
*/
		}
		else
		{
			return MSG_ERROR;
		}
	// THIS IS FILTHY CODE, DO NOT COPY AND PASTE ANYWHERE.
	}
	return MSG_HANDLED_STOP;
}