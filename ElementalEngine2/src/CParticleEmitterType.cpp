///============================================================================
/// \file	CParticleEmitterType.cpp
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

#include "StdAfx.h"

REGISTER_COMPONENT( CParticleEmitterType );

IComponent *CParticleEmitterType::Create(int nArgs, va_list argptr)
{
	IObject *self;
	IHashString *name = va_arg(argptr, IHashString *);
	IHashString *parentName = va_arg(argptr, IHashString *);
	try
	{
		// try and create an instance
		self = new CParticleEmitterType(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CParticleEmitterType::CParticleEmitterType(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE(CParticleManager, CParticleEmitterType, IObject, parentName, name)
{
	m_pTexture = NULL;
	m_pParticleManager = (CParticleManager*)m_Manager;

	m_nCachedParticleArraySize = 0;

	m_bRandomRotation		= false;
	m_bAdditiveBlending		= false;
	m_bScreenAligned		= true;
	m_bLooping				= false;
	m_bStretchToOrigin		= false;

	m_nKeysNumber = 0;
	m_keysNumber = NULL;
	m_nKeysL = 0;
	m_keysL = NULL;
	m_nKeysW = 0;
	m_keysW = NULL;
	m_nKeysH = 0;
	m_keysH = NULL;	
	m_nKeysOutVelocity = 0;
	m_keysOutVelocity = NULL;
	m_nKeysAltVelocity = 0;
	m_keysAltVelocity = NULL;
	m_nKeysLife = 0;
	m_keysLife = NULL;
	m_nKeysMidlife = 0;
	m_keysMidlife = NULL;

	m_fAttachToEmitter = 0.0f;

	m_fTextureFPS = 0.0f;
	m_fTextureScale = 0.0f;
	m_bAnimatedTexture = false;

	memset(m_fSpeed, 0, sizeof(float)*3);
	memset(m_fSize, 0, sizeof(float)*3);
	memset(m_fLife, 0, sizeof(float)*3);
	memset(m_fSpin, 0, sizeof(float)*3);
	memset(m_fSpin, 0, sizeof(float)*3);
	memset(m_fSpin, 1, sizeof(float)*12);
}

CParticleEmitterType::~CParticleEmitterType()
{
	DeInit();
}

PARTICLE_TYPES CParticleEmitterType::GetTypeFromString(const char *type) const
{
		// Type
		static const char * szTypeArray[] =
				{"POINT",
				"FAST_POINT",
				"SPHERE",
				"BOX",
				"PLANE",
				"DISK",
				"RING",
				"HALO",
				"SHELL",
				"JET",
				"CONE",
				"PILLAR",
				NULL};
		int i = 0;
		while (szTypeArray[i] != NULL)
		{
			 //_tcscmp
			if (0 == strcmp(szTypeArray[i], type))
				return (PARTICLE_TYPES)i;
			i++;
		}
		return INVALID_TYPE;
}

void CParticleEmitterType::CreateKeys(IArchive &ar, SParticleKeyframe *&keys, unsigned int numkeys  )
{
	// Hammering an existing array, bug. Likely a LEAK. TODO: reload data?
	if (keys)
	{
		assert(0);
		return;
	}
	// No keys. Damn. Likely FATAL
	if (!numkeys)
	{
		assert(0);
		return;
	}
	keys = new SParticleKeyframe[numkeys];
    for (unsigned int i = 0; i < numkeys; i++)
	{
		ar.Read(keys[i].time, _T("Keyt"));
		ar.Read(keys[i].value, _T("Keyv"));
	}
	return;
}

void CParticleEmitterType::Serialize( IArchive &ar )
{
	CHashString hszVersion10(_T("1.0"));
	CHashString hszVersion11(_T("1.1"));
	if (ar.IsReading())
	{
		// Version
		StdString szVersionTemp;
		ar.Read(szVersionTemp, _T("Version"));
		m_hszVersion.Init(szVersionTemp);

		// Type
		StdString szTypeTemp;
		ar.Read(szTypeTemp, _T("Type"));
		m_Type = GetTypeFromString(szTypeTemp);

		// Texture Data
		StdString szTextureNameTemp;
		ar.Read(szTextureNameTemp, _T("TextureName"));
		m_hszTextureName.Init(szTextureNameTemp);
		ar.Read(m_fTextureFPS, _T("TextureFPS"));
		ar.Read(m_fTextureScale, _T("TextureScale"));

		// Boolean Flags
		ar.Read(m_bRandomRotation, _T("RandomRotation"));
		ar.Read(m_bAdditiveBlending,	_T("AdditiveBlending"));
		ar.Read(m_bScreenAligned, _T("ScreenAligned"));
		ar.Read(m_bLooping,	_T("Looping"));
		ar.Read(m_bStretchToOrigin, _T("StretchToOrigin"));

		// Floats
		if (!_tcscmp(m_hszVersion.GetString(), hszVersion11.GetString()))
			ar.Read(m_fAttachToEmitter, _T("AttachToEmitter"));
		ar.Read(m_fLoopLife, _T("LoopLife"));

		// Emitter Keyframes
		ar.Read(m_nKeysNumber, _T("nNumber"));
		CreateKeys(ar, m_keysNumber, m_nKeysNumber);
		ar.Read(m_nKeysL, _T("nNumber"));
		CreateKeys(ar, m_keysL, m_nKeysL);
		ar.Read(m_nKeysW, _T("nNumber"));
		CreateKeys(ar, m_keysW, m_nKeysW);
		ar.Read(m_nKeysH, _T("nNumber"));
		CreateKeys(ar, m_keysH, m_nKeysH);
		ar.Read(m_nKeysOutVelocity, _T("nNumber"));
		CreateKeys(ar, m_keysOutVelocity, m_nKeysOutVelocity);
		ar.Read(m_nKeysAltVelocity, _T("nNumber"));
		CreateKeys(ar, m_keysAltVelocity, m_nKeysAltVelocity);
		ar.Read(m_nKeysLife, _T("nNumber"));
		CreateKeys(ar, m_keysLife, m_nKeysLife);
		ar.Read(m_nKeysMidlife, _T("nNumber"));
		CreateKeys(ar, m_keysMidlife, m_nKeysMidlife);

		// Particle Keyframes
		ar.Read(m_fSpeed[0], _T("Speed0"));
		ar.Read(m_fSpeed[1], _T("Speed1"));		
		ar.Read(m_fSpeed[2], _T("Speed2"));
		ar.Read(m_fSize[0], _T("Size0"));
		ar.Read(m_fSize[1], _T("Size1"));
		ar.Read(m_fSize[2], _T("Size2"));
		ar.Read(m_fSpin[0], _T("Spin0"));
		ar.Read(m_fSpin[1], _T("Spin1"));
		ar.Read(m_fSpin[2], _T("Spin2"));
		m_fSpin[0] *= EEDEG2RAD; 
		m_fSpin[1] *= EEDEG2RAD; 
		m_fSpin[2] *= EEDEG2RAD; 
		ar.Read(m_fWeight[0], _T("Weight0"));
		ar.Read(m_fWeight[1], _T("Weight1"));
		ar.Read(m_fWeight[2], _T("Weight2"));
		
		// Color
		for (int i = 0; i < 3; i++)
		{
			ar.Read(m_fColor[i][0], _T("ColorR"));
			ar.Read(m_fColor[i][1], _T("ColorG"));
			ar.Read(m_fColor[i][2], _T("ColorB"));
			ar.Read(m_fColor[i][3], _T("ColorA"));
		}
	}
	else
	{

	}
}

void CParticleEmitterType::Init()
{
	//grab the renderer
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	renderInterfaceParam.m_RendererInterface = NULL;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam, NULL, NULL );
	m_IRenderer = renderInterfaceParam.m_RendererInterface;

	if (m_pParticleManager)
	{
        m_pTexture = m_pParticleManager->LoadTexture( m_hszTextureName.GetString() );
		if ( m_pTexture )
		{
			if ( m_pTexture->GetTextureType() == EE_TEXTUREANIMATED2D )
			{
  				IAnimatedTextureObject * at = (IAnimatedTextureObject *)m_pTexture;
  				m_bAnimatedTexture = true;
				float u;
  				at->GetFrame( 0, u,u,u,u, &m_pUseTexture );
			}
			else
			{
				m_pUseTexture = m_pTexture;
				m_bAnimatedTexture = false;
			}
		}
		else
		{
			m_ToolBox->Log(LOGERROR, _T("ParticleEmitter \"UNKNOWN\" failed to load Texture \"%s\".\n"), m_hszTextureName.GetString()); //TODO FIX UNKNOWN
		}
	}
}

void CParticleEmitterType ::DeInit()
{
	if (m_pTexture)
	{
		TEXTUREOBJECTPARAMS tobj;
		tobj.bLoad = false;
		tobj.Name = &m_hszTextureName;
		tobj.TextureObjectInterface = NULL;
		static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
		m_pTexture = NULL;
	}
	SAFE_DELETE_ARRAY( m_keysNumber );
	SAFE_DELETE_ARRAY( m_keysL );
	SAFE_DELETE_ARRAY( m_keysW );
	SAFE_DELETE_ARRAY( m_keysH );
	SAFE_DELETE_ARRAY( m_keysOutVelocity );
	SAFE_DELETE_ARRAY( m_keysAltVelocity );
	SAFE_DELETE_ARRAY( m_keysLife );
	SAFE_DELETE_ARRAY( m_keysMidlife );

}
