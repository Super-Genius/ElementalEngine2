///============================================================================
/// \file	CParticleEmitter.cpp
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

REGISTER_COMPONENT( CParticleEmitter );

using namespace std;

CParticleEmitter::CParticleEmitter(IHashString *parentName, IHashString *name, bool bAddToHier):
	OBJECTTEMPLATE(CParticleManager /*NULL??*/, CParticleEmitter, IParticleEmitter, parentName, name)
{
	m_bAddToHierarchy = bAddToHier;
	if (m_bAddToHierarchy)
	{
		AddToHierarchy();		
	}
	m_ToolBox = EngineGetToolBox();
	SetTint(.0,.0,.0,.0);

	float EmitterBoundsMap[ EMITTER_VARIABLE_MAX ][4] =
	{//	{ time_min,	time_max,	value_min,	value_max }
		{ 0.0,		1000.0,		0.0,		2000.0},	//EMITTER_LIFE
		{ 0.0,		1000.0,		0.0,		2000.0},	//EMITTER_NUMBER
		{ 0.0,		1000.0,		0.0,		2000.0},	//EMITTER_SIZE
		{ 0.0,		1000.0,		0.0,		2000.0},	//EMITTER_VELOCITY
		{ 0.0,		1000.0,		0.0,		2000.0},	//EMITTER_WEIGHT
		{ 0.0,		1000.0,		0.0,		2000.0},	//EMITTER_SPIN
		{ 0.0,		1000.0,		0.0,		2000.0},	//EMITTER_MOTION_RAND
		{ 0.0,		1000.0,		0.0,		2000.0},	//EMITTER_ZOOM
		{ 0.0,		1000.0,		0.0,		100.0},		//EMITTER_VISIBILITY
		{ 0.0,		1000.0,		0.0,		100.0},		//EMITTER_TINT_STRENGTH
		{ 0.0,		1000.0,		-2000.0,	2000.0},	//EMITTER_EMISSION_ANGLE
		{ 0.0,		1000.0,		0.0,		360.0},		//EMITTER_EMISSION_RANGE
		{ 0.0,		1000.0,		0.0,		1.0},		//EMITTER_ACTIVE
		{ 0.0,		1000.0,		-2000.0,	2000.0},	//EMITTER_ANGLE
	};


	for (int i = 0; i < EMITTER_VARIABLE_MAX; i++)
	{
		m_KeyData[ i ].m_Keys = NULL;
		m_KeyData[ i ].m_Size = 0;
		m_KeyData[ i ].m_TimeMin = EmitterBoundsMap [ i ][ 0 ];
		m_KeyData[ i ].m_TimeMax = EmitterBoundsMap [ i ][ 1 ];
		m_KeyData[ i ].m_ValueMin = EmitterBoundsMap [ i ][ 2 ];
		m_KeyData[ i ].m_ValueMax = EmitterBoundsMap [ i ][ 3 ];
	}
}

CParticleEmitter::~CParticleEmitter()
{
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

IComponent *CParticleEmitter::Create(int nArgs, va_list argptr)
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
			self = new CParticleEmitter(parentName, name, false); // defaults to false
		}
		else if(nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CParticleEmitter(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CParticleEmitter::SetFilename( StdString * filename )
{
	m_Filename = *filename;
}

void CParticleEmitter::Init()
{
	// if this isn't a global script object	
}

void	CParticleEmitter::DeInit()
{
	for (int i = 0; i < EMITTER_VARIABLE_MAX; i++)
	{
		SAFEARRAYDELETE(m_KeyData[i].m_Keys);
	}

	memset( m_KeyData, 0, sizeof( m_KeyData) );
	float one = 1.f;
	memset( m_Tint, *((int*)&one), sizeof( m_Tint ) );
	m_Brushes.clear();
}

bool CParticleEmitter::Update()
{
	return true;
}
void CParticleEmitter::AddKeyFrame(int property, float time, float value)
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

DWORD	CParticleEmitter::OnAddBrush(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( PARTICLEFILEPARAMS) );
	PARTICLEFILEPARAMS * pfp = (PARTICLEFILEPARAMS *)params;

	GETPARTICLETYPEMESSAGE getparticlemsg;
	DWORD result;

	//create load and add
	getparticlemsg.filename = pfp->filename;

	//try to get the particle from the manager
	static DWORD msgHash_GetParticleType = CHashString(_T("GetParticleType")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_GetParticleType, sizeof(GETPARTICLETYPEMESSAGE),&getparticlemsg ); 
	if( result == MSG_HANDLED )
	{
		AddParticleBrush( getparticlemsg.particletype );
		if (getparticlemsg.particletype)
			return MSG_ERROR;
		return MSG_HANDLED_STOP;
	}
	else
		return MSG_ERROR;
}
int CParticleEmitter::GetKeyframeCount()
{
	int keyframes = 0;
	for (int i = 0; i < EMITTER_VARIABLE_MAX; i++)
	{
		keyframes += m_KeyData[i].m_Size;
	}
	return keyframes;
}

DWORD   CParticleEmitter::OnPumpEmitterProperty(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( PARTICLEPROPERTIESKEY ) );
	PARTICLEPROPERTIESKEY * msg = (PARTICLEPROPERTIESKEY*)params;


	static DWORD hszEmitterLife				= CHashString(_T("EMITTER_LIFE")).GetUniqueID();
	static DWORD hszEmitterNumber			= CHashString(_T("EMITTER_NUMBER")).GetUniqueID();
	static DWORD hszEmitterSize				= CHashString(_T("EMITTER_SIZE")).GetUniqueID();
	static DWORD hszEmitterVelocity			= CHashString(_T("EMITTER_VELOCITY")).GetUniqueID();
	static DWORD hszEmitterWeight			= CHashString(_T("EMITTER_WEIGHT")).GetUniqueID();
	static DWORD hszEmitterSpin				= CHashString(_T("EMITTER_SPIN")).GetUniqueID();
	static DWORD hszEmitterMotionRand		= CHashString(_T("EMITTER_MOTION_RAND")).GetUniqueID();
	static DWORD hszEmitterZoom				= CHashString(_T("EMITTER_ZOOM")).GetUniqueID();
	static DWORD hszEmitterVisibility		= CHashString(_T("EMITTER_VISIBILITY")).GetUniqueID();
	static DWORD hszEmitterTintStrength		= CHashString(_T("EMITTER_TINT_STRENGTH")).GetUniqueID();
	static DWORD hszEmitterEmissionAngle	= CHashString(_T("EMITTER_EMISSION_ANGLE")).GetUniqueID();
	static DWORD hszEmitterEmissionRange	= CHashString(_T("EMITTER_EMISSION_RANGE")).GetUniqueID();
	static DWORD hszEmitterActive			= CHashString(_T("EMITTER_ACTIVE")).GetUniqueID();
	static DWORD hszEmitterAngle			= CHashString(_T("EMITTER_ANGLE")).GetUniqueID();

	DWORD propertyname;
	if (msg->propertyname)
	{
		propertyname = msg->propertyname->GetUniqueID();
	}
	else
	{
		return MSG_ERROR;
	}
	if(propertyname == hszEmitterLife)
		AddKeyFrame(EMITTER_LIFE, msg->time, msg->value);
	else if(propertyname == hszEmitterNumber)
		AddKeyFrame(EMITTER_NUMBER, msg->time, msg->value);
	else if(propertyname == hszEmitterSize)
		AddKeyFrame(EMITTER_SIZE, msg->time, msg->value);
	else if(propertyname == hszEmitterVelocity)
		AddKeyFrame(EMITTER_VELOCITY, msg->time, msg->value);
	else if(propertyname == hszEmitterWeight)
		AddKeyFrame(EMITTER_WEIGHT, msg->time, msg->value);
	else if(propertyname == hszEmitterSpin)
		AddKeyFrame(EMITTER_SPIN, msg->time, msg->value);
	else if(propertyname == hszEmitterMotionRand)
		AddKeyFrame(EMITTER_MOTION_RAND, msg->time, msg->value);
	else if(propertyname == hszEmitterZoom)
		AddKeyFrame(EMITTER_ZOOM, msg->time, msg->value);
	else if(propertyname == hszEmitterVisibility)
		AddKeyFrame(EMITTER_VISIBILITY, msg->time, msg->value);
	else if(propertyname == hszEmitterTintStrength)
		AddKeyFrame(EMITTER_TINT_STRENGTH, msg->time, msg->value);
	else if(propertyname == hszEmitterEmissionAngle)
		AddKeyFrame(EMITTER_EMISSION_ANGLE, msg->time, msg->value);
	else if(propertyname == hszEmitterEmissionRange)
		AddKeyFrame(EMITTER_EMISSION_RANGE, msg->time, msg->value);
	else if(propertyname == hszEmitterActive)
		AddKeyFrame(EMITTER_ACTIVE, msg->time, msg->value);
	else if(propertyname == hszEmitterAngle)
		AddKeyFrame(EMITTER_ANGLE, msg->time, msg->value);
	else
		return MSG_ERROR;

	return MSG_HANDLED_STOP;
}

static StdString GrabKeyFrameData( std::vector< ElementalEngine::KEYFRAME_DATA > &out, IXMLArchive * ar )
{
	StdString szNodeName;
	out.clear();
	bool bNextNode = ar->GetNode(szNodeName);
	while( bNextNode )
	{
		if( szNodeName == _T("Keyframe") )
		{				
			ElementalEngine::KEYFRAME_DATA k;
			ar->Read( k.time, _T("time") );
			ar->Read( k.value, _T("value") );
			out.push_back( k );
		}else
			break;	
		bNextNode = ar->GetNode(szNodeName);
	}	
	return szNodeName;
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

void CParticleEmitter::CheckAndFixEmptyKeyframe( KEYFRAME_DATA_SET &arg , int property)
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
void CParticleEmitter::FixEmptyKeys()
{
	for( int i = 0; i < EMITTER_VARIABLE_MAX; i++ )
	{
		CheckAndFixEmptyKeyframe( m_KeyData[ i ], i );
	}
}

void CParticleEmitter::Serialize(IArchive &ar)
{
//	unsigned int bSerializeMode;

	unsigned int numBrushes = 0;
	unsigned int numKeyframes = 0;
	int flag = PARTICLE_SERIALIZE_NORMAL;

	StdString	szPropertyName;

	if( &ar == NULL )
	{
		return;
	}
	if (ar.IsReading())
	{
//		ar.Read( serializationmode, _T("serialization_mode" )

		// READ FILENAME
//		ar.Read( m_Filename, _T("filename"));

		// READ BASIC DATA
		ar.Read( m_Name, _T("name"));

		// READ SERIALIZE TYPE FLAG
		ar.Read( flag, _T("flag"));

		ar.Read( numBrushes, _T("brushes"));
		ar.Read( numKeyframes, _T("keyframes"));

		if (flag == PARTICLE_SERIALIZE_NORMAL)
		{
			StdString	szBrushFilename;
			// READ BRUSHES
			for (unsigned int i = 0 ; i < numBrushes ; i++)
			{
				ar.Read( szBrushFilename, _T("Brush"));
				CHashString hszTemp( szBrushFilename );
				PARTICLEFILEPARAMS pfp;
				pfp.filename = &hszTemp;
				
				OnAddBrush(sizeof(PARTICLEFILEPARAMS), &pfp);
			}
			// READ KEYFRAMES
			for (unsigned int i = 0 ; i < numKeyframes ; i++)
			{
				PARTICLEPROPERTIESKEY k;
				ar.Read( szPropertyName, _T("type"));
				ar.Read( k.time, _T("time"));
				ar.Read( k.value, _T("value"));
				CHashString hszTemp( szPropertyName );
				k.propertyname = &hszTemp;

				OnPumpEmitterProperty( sizeof(PARTICLEPROPERTIESKEY), &k);
			}
			// This is primative data security. If a particle property is missing,
			// it will fill it with zero values.
			FixEmptyKeys();
		}
	}
	else
	{
		CREATE_EMITTER_STRING_TABLE(EmitterPropertyStringTable);
		// WRITE BASIC DATA
		numBrushes = GetNumBrushes();
		numKeyframes = GetKeyframeCount();
		// WRITE FILENAME
//		ar.Write( m_Filename, _T("filename"));
		// WRITE BASIC DATA
		ar.Write( m_Name, _T("name"));
		ar.Write( numBrushes, _T("brushes"));
		ar.Write( numKeyframes, _T("keyframes"));


		PARTICLE_INSTANCE_VECTOR::iterator iter;
		if( m_Brushes.size() > 0)
		{
			iter = m_Brushes.begin();
			for( ;iter != m_Brushes.end(); iter++ )
			{
				IParticleType * p = &*iter->m_ParticleType;
				StdString szBrushPath(p->GetName()->GetString());
				ar.Write( szBrushPath, _T("Brush"));
			}
		}
		for (int i = 0 ; i < EMITTER_VARIABLE_MAX ; i++)
		{
			for (int j = 0 ; j < m_KeyData[i].m_Size; j++)
			{
				StdString szTempPropertyName( EmitterPropertyStringTable[i] );
				ar.Write( szTempPropertyName, _T("type"));
				ar.Write( m_KeyData[i].m_Keys[j].time, _T("time"));
				ar.Write( m_KeyData[i].m_Keys[j].value, _T("value"));
			}
		}
	}
}

bool CParticleEmitter::SetKeyData( PARTICLE_EMITTER_VARIABLE channel, KEYFRAME_DATA_SET &data )
{
	if( channel >= 0 && channel < EMITTER_VARIABLE_MAX )
	{
		CopyKeyframeSets( m_KeyData[ channel ], data );
		return true;
	}
	return false;
}

bool CParticleEmitter::GetKeyData( PARTICLE_EMITTER_VARIABLE channel, KEYFRAME_DATA_SET &data )
{
	if( channel >= 0 && channel < EMITTER_VARIABLE_MAX )
	{
		CopyKeyframeSets( data, m_KeyData[ channel ] );
		return true;
	}
	return false;
}

void CParticleEmitter::AddParticleBrush( IParticleType * particle )
{
	if( particle )
	{
		ParticleTypeInstance p;
		p.m_Visible = true;
		p.m_ParticleType = particle;
		m_Brushes.push_back( p );
	}
}

void CParticleEmitter::SetBrushVisibility( IParticleType * particle, bool visible )
{
	if( particle )
	{
		PARTICLE_INSTANCE_VECTOR::iterator iter;
		iter = m_Brushes.begin();
		for (; iter != m_Brushes.end(); iter++)
		{
			if (particle == &*iter->m_ParticleType)
			{
				//found
				ParticleTypeInstance &p = *iter;
				p.m_Visible = visible;
			}
		}
	}
}

void CParticleEmitter::RemoveBrush( IParticleType * particle )
{

	if( particle )
	{
		PARTICLE_INSTANCE_VECTOR::iterator iter;
		iter = m_Brushes.begin();
		while (iter != m_Brushes.end())
		{	
			IParticleType * pDeleteParticleBrush = &*iter->m_ParticleType;
			if (particle == pDeleteParticleBrush)
			{
				//found
				iter = m_Brushes.erase( iter );
			}
			else
				iter++;
		}
	}
}

UINT CParticleEmitter::GetNumBrushes () 
{
	return (UINT)m_Brushes.size();
}

void CParticleEmitter::MoveBrush( bool bUp, UINT index )
{
	// You cannot move the object beyond the top of the map.
	if (bUp && (0 >= index))
		return;

    PARTICLE_INSTANCE_VECTOR::iterator oldLocIter;
	PARTICLE_INSTANCE_VECTOR::iterator newLocIter;

	int i = 0;
	for (oldLocIter = m_Brushes.begin();
		oldLocIter != m_Brushes.end();
		oldLocIter++, i++)
	{
		if (index == i)
		{
			break;
		}
	}

	if (bUp)
		i-=2;
	else
	{
		// You cannot move the object beyond the bottom of the map.
		if (i == m_Brushes.size())
			return;
	}
	newLocIter = m_Brushes.begin();
	for (int j = 0; j <= i; j++)
	{
		newLocIter++;
	}
	if (newLocIter != m_Brushes.end())
	{
		std::iter_swap( newLocIter, oldLocIter );
		assert(&*newLocIter->m_ParticleType);
		assert(&*oldLocIter->m_ParticleType);
	}
}

IParticleType * CParticleEmitter::GetBrush( UINT index )
{

	PARTICLE_INSTANCE_VECTOR::iterator iter;
	if (m_Brushes.size() > 0 &&
		index >= 0 &&
		index < m_Brushes.size() )
	{
		if (m_Brushes[index].m_ParticleType)
			return m_Brushes[index].m_ParticleType;
	}
	return NULL;
}

void CParticleEmitter::SetTint( float r, float g, float b, float a )
{
	m_Tint[ 0 ]= r;
	m_Tint[ 1 ] =g;
	m_Tint[ 2 ] = b;
	m_Tint[ 3 ] = a;
}

void CParticleEmitter::GetTint( float tintout[])
{
	tintout[0] = m_Tint[0];
	tintout[1] = m_Tint[1];
	tintout[2] = m_Tint[2];
	tintout[3] = m_Tint[3];
}

void CParticleEmitter::DebugOutputLists()
{
	PARTICLE_INSTANCE_VECTOR::iterator iter;
	iter = m_Brushes.begin();
	m_ToolBox->Log(2, "ParticleEmitter output -START- \n" );
	for (; iter != m_Brushes.end(); iter++)
	{
		if (&*iter->m_ParticleType)
			if (&*iter->m_ParticleType->GetName())
				m_ToolBox->Log(2, "ParticleEmitter output %s \n", &*iter->m_ParticleType->GetName()->GetString());
	}
	m_ToolBox->Log(2, "ParticleEmitter output -END- \n" );

}

const TCHAR * CParticleEmitter::GetFilename()
{
	return m_Filename;
}
