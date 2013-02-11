///============================================================================
/// \file	CParticleType.cpp
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

REGISTER_COMPONENT( CParticleType );

//REGISTER_MESSAGE_HANDLER(PumpBrushProperty, OnPumpBrushProperty, CParticleType );
//REGISTER_MESSAGE_HANDLER(GetParticleBrushPointer, OnGetParticleBrushPointer, CParticleType );

float g_ParticleBoundsMap[ PARTICLE_VARIABLE_MAX ][4] =
{//	{ time_min,	time_max,	value_min,	value_max }
	{ 0.0,		1000.0,		0.0,		2000.0},		//PARTICLE_LIFE
	{ 0.0,		1000.0,		0.0,		2000.0},		//PARTICLE_NUMBER
	{ 0.0,		1000.0,		0.0,		2000.0},		//PARTICLE_SIZE
	{ 0.0,		1000.0,		0.0,		2000.0},		//PARTICLE_VELOCITY
	{ 0.0,		1000.0,		-2000.0,	2000.0},		//PARTICLE_WEIGHT
	{ 0.0,		1000.0,		-2000.0,	2000.0},		//PARTICLE_SPIN
	{ 0.0,		1000.0,		0.0,		2000.0},		//PARTICLE_MOTION_RAND
	{ 0.0,		1000.0,		0.0,		2000.0},		//PARTICLE_VISIBILITY
	{ 0.0,		1000.0,		0.0,		2000.0},		//PARTICLE_LIFE_VARIATION
	{ 0.0,		1000.0,		0.0,		2000.0},		//PARTICLE_NUMBER_VARIATION
	{ 0.0,		1000.0,		0.0,		2000.0},		//PARTICLE_SIZE_VARIATION
	{ 0.0,		1000.0,		0.0,		2000.0},		//PARTICLE_VELOCITY_VARIATION
	{ 0.0,		1000.0,		0.0,		2000.0},		//PARTICLE_WEIGHT_VARIATION
	{ 0.0,		1000.0,		0.0,		2000.0},		//PARTICLE_SPIN_VARIATION
	{ 0.0,		1000.0,		0.0,		2000.0},		//PARTICLE_MOTION_RAND_VARIATION
	{ 0.0,		1.0,		0.0,		200.0},			//PARTICLE_SIZE_LIFE
	{ 0.0,		1.0,		0.0,		200.0},			//PARTICLE_VELOCITY_LIFE
	{ 0.0,		1.0,		-100.0,		100.0},			//PARTICLE_WEIGHT_LIFE
	{ 0.0,		1.0,		-100.0,		100.0},			//PARTICLE_SPIN_LIFE
	{ 0.0,		1.0,		0.0,		200.0},			//PARTICLE_MOTION_RAND_LIFE
	{ 0.0,		1000.0,		0.0,		1.0},			//PARTICLE_COLOR_R
	{ 0.0,		1000.0,		0.0,		1.0},			//PARTICLE_COLOR_G
	{ 0.0,		1000.0,		0.0,		1.0},			//PARTICLE_COLOR_B
	{ 0.0,		1000.0,		0.0,		1.0},			//PARTICLE_COLOR_A
};

using namespace std;

CParticleType::CParticleType(IHashString *parentName, IHashString *name, bool bAddToHier):
	OBJECTTEMPLATE(CParticleManager /*NULL??*/, CParticleType, IParticleType, parentName, name)
{
	m_Material = NULL;

	for (int i = 0; i < PARTICLE_VARIABLE_MAX; i++)
	{
		m_KeyData[ i ].m_Keys = NULL;
		m_KeyData[ i ].m_Size = 0;
		m_KeyData[ i ].m_TimeMin = g_ParticleBoundsMap [ i ][ 0 ];
		m_KeyData[ i ].m_TimeMax = g_ParticleBoundsMap [ i ][ 1 ];
		m_KeyData[ i ].m_ValueMin = g_ParticleBoundsMap [ i ][ 2 ];
		m_KeyData[ i ].m_ValueMax = g_ParticleBoundsMap [ i ][ 3 ];
	}
	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}
}

CParticleType::~CParticleType()
{
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

IComponent *CParticleType::Create(int nArgs, va_list argptr)
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
			self = new CParticleType(parentName, name, true); // not sure if this is supposed to default to true or false
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CParticleType(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CParticleType::Init()
{
		//any keys that have no values, add one value set to 0
//		FixEmptyKeys();
		//Load textures and shaders
	LoadTexturesAndShaders();
}
bool CParticleType::Update()
{
	return true;
}
void CParticleType ::DeInit()
{
	for (int i = 0; i < PARTICLE_VARIABLE_MAX; i++)
	{
		SAFEARRAYDELETE(m_KeyData[i].m_Keys);
	}
}

void CParticleType::AddKeyFrame(int property, float time, float value)
{
	KEYFRAME_DATA * k = new KEYFRAME_DATA[ m_KeyData[property].m_Size +1];
	
	for(int i = 0; i < m_KeyData[property].m_Size; i++)
	{
		k[i].time = m_KeyData[property].m_Keys[i].time;
		k[i].value = m_KeyData[property].m_Keys[i].value;
	}
	k[m_KeyData[property].m_Size].time = time;
	k[m_KeyData[property].m_Size].value = value;

	SAFEARRAYDELETE(m_KeyData[property].m_Keys);

	m_KeyData[property].m_Keys = k;
	m_KeyData[property].m_Size ++;

}

void CParticleType::SetFilename( StdString * filename )
{
	m_Filename = *filename;
}

void CParticleType::SetFilepath( StdString * filepath )
{
	m_Filepath = *filepath;
}


int CParticleType::GetKeyframeCount()
{
	int keyframes = 0;
	for (int i = 0; i < PARTICLE_VARIABLE_MAX; i++)
	{
		keyframes += m_KeyData[i].m_Size;
	}
	return keyframes;
}

DWORD CParticleType::OnPumpBrushProperty(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( PARTICLEPROPERTIESKEY ) );
	PARTICLEPROPERTIESKEY * msg = (PARTICLEPROPERTIESKEY*)params;


	static DWORD hszParticleLife				= CHashString(_T("PARTICLE_LIFE")).GetUniqueID();
	static DWORD hszParticleLifeVariation		= CHashString(_T("PARTICLE_LIFE_VARIATION")).GetUniqueID();
	static DWORD hszParticleNumber				= CHashString(_T("PARTICLE_NUMBER")).GetUniqueID();
	static DWORD hszParticleNumberVariation		= CHashString(_T("PARTICLE_NUMBER_VARIATION")).GetUniqueID();
	static DWORD hszParticleVisibility			= CHashString(_T("PARTICLE_VISIBILITY")).GetUniqueID();
	static DWORD hszParticleColorR				= CHashString(_T("PARTICLE_COLOR_R")).GetUniqueID();
	static DWORD hszParticleColorG				= CHashString(_T("PARTICLE_COLOR_G")).GetUniqueID();
	static DWORD hszParticleColorB				= CHashString(_T("PARTICLE_COLOR_B")).GetUniqueID();
	static DWORD hszParticleColorA				= CHashString(_T("PARTICLE_COLOR_A")).GetUniqueID();
	static DWORD hszParticleVelocity			= CHashString(_T("PARTICLE_VELOCITY")).GetUniqueID();
	static DWORD hszParticleVelocityVariation	= CHashString(_T("PARTICLE_VELOCITY_VARIATION")).GetUniqueID();
	static DWORD hszParticleVelocityLife		= CHashString(_T("PARTICLE_VELOCITY_LIFE")).GetUniqueID();
	static DWORD hszParticleSpin				= CHashString(_T("PARTICLE_SPIN")).GetUniqueID();
	static DWORD hszParticleSpinVariation		= CHashString(_T("PARTICLE_SPIN_VARIATION")).GetUniqueID();
	static DWORD hszParticleSpinLife			= CHashString(_T("PARTICLE_SPIN_LIFE")).GetUniqueID();
	static DWORD hszParticleSize				= CHashString(_T("PARTICLE_SIZE")).GetUniqueID();
	static DWORD hszParticleSizeVariation		= CHashString(_T("PARTICLE_SIZE_VARIATION")).GetUniqueID();
	static DWORD hszParticleSizeLife			= CHashString(_T("PARTICLE_SIZE_LIFE")).GetUniqueID();
	static DWORD hszParticleMotionRand			= CHashString(_T("PARTICLE_MOTION_RAND")).GetUniqueID();
	static DWORD hszParticleMotionRandVariation	= CHashString(_T("PARTICLE_MOTION_RAND_VARIATION")).GetUniqueID();
	static DWORD hszParticleMotionRandLife		= CHashString(_T("PARTICLE_MOTION_RAND_LIFE")).GetUniqueID();
	static DWORD hszParticleWeight				= CHashString(_T("PARTICLE_WEIGHT")).GetUniqueID();
	static DWORD hszParticleWeightVariation		= CHashString(_T("PARTICLE_WEIGHT_VARIATION")).GetUniqueID();
	static DWORD hszParticleWeightLife			= CHashString(_T("PARTICLE_WEIGHT_LIFE")).GetUniqueID();

	DWORD propertyname;
	if (msg->propertyname)
	{
		propertyname = msg->propertyname->GetUniqueID();
	}
	else
	{
		return MSG_ERROR;
	}
	if(propertyname == hszParticleLife)
		AddKeyFrame(PARTICLE_LIFE, msg->time, msg->value);
	else if(propertyname == hszParticleLifeVariation)
		AddKeyFrame(PARTICLE_LIFE_VARIATION, msg->time, msg->value);
	else if(propertyname == hszParticleNumber)
		AddKeyFrame(PARTICLE_NUMBER, msg->time, msg->value);
	else if(propertyname == hszParticleNumberVariation)
		AddKeyFrame(PARTICLE_NUMBER_VARIATION, msg->time, msg->value);
	else if(propertyname == hszParticleVisibility)
		AddKeyFrame(PARTICLE_VISIBILITY, msg->time, msg->value);
	else if(propertyname == hszParticleColorR)
		AddKeyFrame(PARTICLE_COLOR_R, msg->time, msg->value);
	else if(propertyname == hszParticleColorG)
		AddKeyFrame(PARTICLE_COLOR_G, msg->time, msg->value);
	else if(propertyname == hszParticleColorB)
		AddKeyFrame(PARTICLE_COLOR_B, msg->time, msg->value);
	else if(propertyname == hszParticleColorA)
		AddKeyFrame(PARTICLE_COLOR_A, msg->time, msg->value);
	else if(propertyname == hszParticleVelocity)
		AddKeyFrame(PARTICLE_VELOCITY, msg->time, msg->value);
	else if(propertyname == hszParticleVelocityVariation)
		AddKeyFrame(PARTICLE_VELOCITY_VARIATION, msg->time, msg->value);
	else if(propertyname == hszParticleVelocityLife)
		AddKeyFrame(PARTICLE_VELOCITY_LIFE, msg->time, msg->value);
	else if(propertyname == hszParticleSpin)
		AddKeyFrame(PARTICLE_SPIN, msg->time, msg->value);
	else if(propertyname == hszParticleSpinVariation)
		AddKeyFrame(PARTICLE_SPIN_VARIATION, msg->time, msg->value);
	else if(propertyname == hszParticleSpinLife)
		AddKeyFrame(PARTICLE_SPIN_LIFE, msg->time, msg->value);
	else if(propertyname == hszParticleSize)
		AddKeyFrame(PARTICLE_SIZE, msg->time, msg->value);
	else if(propertyname == hszParticleSizeVariation)
		AddKeyFrame(PARTICLE_SIZE_VARIATION, msg->time, msg->value);
	else if(propertyname == hszParticleSizeLife)
		AddKeyFrame(PARTICLE_SIZE_LIFE, msg->time, msg->value);
	else if(propertyname == hszParticleMotionRand)
		AddKeyFrame(PARTICLE_MOTION_RAND, msg->time, msg->value);
	else if(propertyname == hszParticleMotionRandVariation)
		AddKeyFrame(PARTICLE_MOTION_RAND_VARIATION, msg->time, msg->value);
	else if(propertyname == hszParticleMotionRandLife)
		AddKeyFrame(PARTICLE_MOTION_RAND_LIFE, msg->time, msg->value);
	else if(propertyname == hszParticleWeight)
		AddKeyFrame(PARTICLE_WEIGHT, msg->time, msg->value);
	else if(propertyname == hszParticleWeightVariation)
		AddKeyFrame(PARTICLE_WEIGHT_VARIATION, msg->time, msg->value);
	else if(propertyname == hszParticleWeightLife)
		AddKeyFrame(PARTICLE_WEIGHT_LIFE, msg->time, msg->value);

	else
		return MSG_ERROR;

	return MSG_HANDLED_STOP;
}

DWORD CParticleType::OnGetParticleBrushPointer( DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GETPARTICLETYPEMESSAGE ) );
	GETPARTICLETYPEMESSAGE * msg = (GETPARTICLETYPEMESSAGE*)params;
	
	msg->particletype = this;
		if( msg->particletype )
			return MSG_HANDLED_STOP;
	return MSG_ERROR;
}

static void CopyVector( std::vector< ElementalEngine::KEYFRAME_DATA > &inv, KEYFRAME_DATA_SET &out)
{
	KEYFRAME_DATA * p = new KEYFRAME_DATA[ inv.size() ];
	for( int i = 0; i < (int)inv.size(); i++ )
	{
		p[ i ] = inv[ i ];
	}
	out.m_Keys = p;
	out.m_Size = (int)inv.size();
}

static __forceinline void CopyKeyframeSets( KEYFRAME_DATA_SET &out, const KEYFRAME_DATA_SET &in )
{
	out.m_TimeMax = in.m_TimeMax;
	out.m_TimeMin = in.m_TimeMin;
	out.m_ValueMax= in.m_ValueMax;
	out.m_ValueMin = in.m_ValueMin;
	out.m_Size = in.m_Size;
	SAFEARRAYDELETE( out.m_Keys );
	out.m_Keys = new KEYFRAME_DATA[ in.m_Size ];
	memcpy( out.m_Keys, in.m_Keys, in.m_Size*sizeof( KEYFRAME_DATA ) );
}

void	CParticleType::Serialize_EditableProperties( IArchive *ar )
{
	if (ar->IsReading())
	{
		ar->Read( m_Name, _T("name") );
		ar->Read( m_bAdditiveBlend, _T("additive") );
		ar->Read( m_FrameRate, _T("framerate") );
		ar->Read( m_TexFrameRate, _T("texturerate") ); //for animated textures
		ar->Read( m_TextureFilename, _T("texture") );
		ar->Read( m_bTextureLoop, _T("loop") );
		ar->Read( m_fAttachToEmitter, _T("AttachToEmitter") );
		ar->Read( m_ShaderFilename, _T("shader") );
	}
	else
	{
		ar->Write( m_Name, _T("name") );
		ar->Write( m_bAdditiveBlend, _T("additive") );
		ar->Write( m_FrameRate, _T("framerate") );
		ar->Write( m_TexFrameRate, _T("texturerate") ); //for animated textures
		ar->Write( m_TextureFilename, _T("texture") );
		ar->Write( m_bTextureLoop, _T("loop") );
		ar->Write( m_fAttachToEmitter, _T("AttachToEmitter") );
		ar->Write( m_ShaderFilename, _T("shader") );
	}
}

void	CParticleType::Serialize( IArchive &ar )
{
	CREATE_PARTICLE_STRING_TABLE(ParticlePropertyStringTable);
	int nKeyframes = 0;
	StdString	szPropertyName;
	int flag = PARTICLE_SERIALIZE_NORMAL;

	if( &ar == NULL )
	{
		return;
	}
	if (ar.IsReading())
	{
		// READ FILENAME & FILEPATH
//		ar.Read( m_Filename, _T("filename") );
//		ar.Read( m_Filepath, _T("filepath") );

		// READ BASIC DATA
		Serialize_EditableProperties(&ar);

		// READ SERIALIZE FLAG
		ar.Read( flag, _T("flag") );

		if (flag == PARTICLE_SERIALIZE_NORMAL)
		{
			// READ KEYFRAMES
			ar.Read( nKeyframes, _T("keyframes") );
			for (int i = 0 ; i < nKeyframes ; i++)
			{
				PARTICLEPROPERTIESKEY k;
				ar.Read( szPropertyName, _T("type"));
				ar.Read( k.time, _T("time"));
				ar.Read( k.value, _T("value"));
				CHashString hszTemp( szPropertyName );
				k.propertyname = &hszTemp;

				OnPumpBrushProperty( sizeof(PARTICLEPROPERTIESKEY), &k);
			}

			// This is primative data security. If a particle property is missing,
			// it will fill it with zero values.
			FixEmptyKeys();
		}
	}
	else
	{
		nKeyframes = GetKeyframeCount();
		// WRITE FILENAME & FILEPATH
//		ar.Write( m_Filename, _T("filename") );
//		ar.Write( m_Filepath, _T("filepath") );

		// WRITE BASIC DATA
		Serialize_EditableProperties(&ar);

		// WRITE KEYFRAMES
		ar.Write( nKeyframes, _T("keyframes") );
		for (int i = 0 ; i < PARTICLE_VARIABLE_MAX ; i++)
		{
			for (int j = 0 ; j < m_KeyData[i].m_Size; j++)
			{
				StdString szTempPropertyName( ParticlePropertyStringTable[i] );
				ar.Write( szTempPropertyName, _T("type"));
				ar.Write( m_KeyData[i].m_Keys[j].time, _T("time"));
				ar.Write( m_KeyData[i].m_Keys[j].value, _T("value"));
			}
		}
	}
}

void CParticleType::CheckAndFixEmptyKeyframe( KEYFRAME_DATA_SET &arg , int property)
{
	if(!arg.m_Keys )
	{
		m_ToolBox->Log(LOGWARNING, _T("%s(%d): %s missing key data for property %d.\n"), __FILE__, __LINE__, this->GetName()->GetString(), property );
		arg.m_Keys = new KEYFRAME_DATA;
		arg.m_Keys->time = 0;
		arg.m_Keys->value = 0;
		arg.m_Size = 1;
	}
}
//any keys that have no values, add one value set to 0
void CParticleType::FixEmptyKeys()
{
	for( int i = 0; i < PARTICLE_VARIABLE_MAX; i++ )
	{
		CheckAndFixEmptyKeyframe( m_KeyData[ i ], i );
	}
}

void CParticleType::LoadTexturesAndShaders()
{
	// load the texture from the file
	const TCHAR * t = m_TextureFilename;
	CHashString fullname( t );
	
	TEXTUREOBJECTPARAMS tobj;
	tobj.bLoad = true;
	tobj.Name = &fullname;
	tobj.TextureObjectInterface = NULL;
	static DWORD msgHash_AddTexture = CHashString(_T("AddTexture")).GetUniqueID();
	DWORD res = EngineGetToolBox()->SendMessage(msgHash_AddTexture, sizeof(tobj), &tobj );
	if( res == MSG_HANDLED )
	{
		// store the texture id in the user data of the map
		m_Texture = (IBaseTextureObject *)tobj.TextureObjectInterface;
	}else
	{
		m_Texture = NULL;
	}

	//TODO:
	/*if( retval == MSG_HANDLED )
	{
		LOADFILEEXTPARAMS lfep;
		TCHAR * lower = _tcslwr( (TCHAR*)((const TCHAR*)m_ShaderFilename ));
		CHashString filename( lower  );
		lfep.fileName = (TCHAR*)filename.GetString();
		lfep.bInternalLoad = true;
		static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
		DWORD retval = EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
		if( retval == MSG_HANDLED )
		{
			CREATEEFFECTINTERFACE cef;
			cef.m_Name = &filename;
			static DWORD msgHash_GetEffectInterface = CHashString(_T("GetEffectInterface")).GetUniqueID();
			EngineGetToolBox()->SendMessage(msgHash_GetEffectInterface, sizeof(CREATEEFFECTINTERFACE ), &cef );
			IEffect * MyEffect = cef.m_EffectInterface;
			if( MyEffect )
			{
				m_Shader = MyEffect->GetShaderPass( 0 );
			}else
			{
				EngineGetToolBox()->Log( LOGERROR, _T("Could not load particle shader" ) );
				m_Shader = NULL;
			}
		}
	}*/
}

bool CParticleType::SetAdditiveBlend( bool enabled )
{
	m_bAdditiveBlend = enabled;
	return true;
}
bool CParticleType::SetTexture( ITextureObject * texture )
{
	m_Texture = (IBaseTextureObject*)texture;
	return true;
}
bool CParticleType::SetAnimationTextureSubdivisions( const UINT xdiv, const UINT ydiv,
				const UINT numAnimations )
{
	
	return true;
}
bool CParticleType::SetMaterial( IMaterial * mat )
{
	m_Material = mat;
	return true;
}

bool CParticleType::SetKeyData( PARTICLE_VARIABLE channel, KEYFRAME_DATA_SET &data )
{
	if( channel >= 0 && channel < PARTICLE_VARIABLE_MAX )
	{
		CopyKeyframeSets( m_KeyData[ channel ], data );
		return true;
	}
	return false;
}

bool CParticleType::GetKeyData( PARTICLE_VARIABLE channel, KEYFRAME_DATA_SET &data )
{
	if( channel >= 0 && channel < PARTICLE_VARIABLE_MAX )
	{
		CopyKeyframeSets( data, m_KeyData[ channel ] );
		return true;
	}
	return false;
}

bool CParticleType::GetAdditiveBlend()
{
	return m_bAdditiveBlend;
}
const TCHAR * CParticleType::GetTextureFilename()
{
	return m_TextureFilename;
}
const TCHAR * CParticleType::GetShaderFilename()
{
	return m_ShaderFilename;
}

const TCHAR * CParticleType::GetFilename()
{
	return m_Filename;
}

const TCHAR * CParticleType::GetFilepath()
{
	return m_Filepath;
}

void CParticleType::SetFrameRate( float frames )
{
	//TODO: implement
	m_FrameRate = frames;
}

void CParticleType::SetTextureRate( float fps )
{
	m_TexFrameRate = fps;
}

float CParticleType::GetFrameRate()
{
	return m_FrameRate;
}

float CParticleType::GetTextureRate()
{
	return m_TexFrameRate;
}
