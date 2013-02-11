///============================================================================
/// \file		TimeOfDay.cpp
/// \brief		Implmentation of CTimeOfDayObject Class
/// \date		07-21-2005
/// \author		D. Patrick Ghiocel
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

#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "TimeofDay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CTimeOfDayObject);
REGISTER_MESSAGE_HANDLER(GetTimeOfDayDesc, OnGetTimeOfDayDesc, CTimeOfDayObject);
REGISTER_MESSAGE_HANDLER(GetTimeOfDayStart, OnGetTimeOfDayStart, CTimeOfDayObject);
REGISTER_MESSAGE_HANDLER(SetTimeOfDayStart, OnSetTimeOfDayStart, CTimeOfDayObject);
REGISTER_MESSAGE_HANDLER(GetTimeOfDay, OnGetTimeOfDay, CTimeOfDayObject);
REGISTER_MESSAGE_HANDLER(SetTimeOfDay, OnSetTimeOfDay, CTimeOfDayObject);
REGISTER_MESSAGE_HANDLER(GetTimeOfDayRate, OnGetTimeOfDayRate, CTimeOfDayObject);
REGISTER_MESSAGE_HANDLER(SetTimeOfDayRate, OnSetTimeOfDayRate, CTimeOfDayObject);
REGISTER_MESSAGE_HANDLER(SetTimeOfDayMotion, OnSetTimeOfDayMotion, CTimeOfDayObject);
REGISTER_MESSAGE_HANDLER(SetTimeOfDayDoFog, OnSetTimeOfDayDoFog, CTimeOfDayObject);
REGISTER_MESSAGE_HANDLER(SetTimeOfDayDoLighting, OnSetTimeOfDayDoLighting, CTimeOfDayObject);

const float CTimeOfDayObject::m_kMidnight  = 0.00f;
const float CTimeOfDayObject::m_kMorning   = 0.25f;
const float CTimeOfDayObject::m_kNoon      = 0.50f;
const float CTimeOfDayObject::m_kAfternoon = 0.60f;
const float CTimeOfDayObject::m_kDusk      = 0.65f;

	
CTimeOfDayObject::CTimeOfDayObject(IHashString *parentName, IHashString *name, bool bAddToHier) :
	m_hszNameType( _T("CTimeOfDayObject") ),
	SCENEOBJECTTEMPLATE(CRenderManager, CTimeOfDayObject, IRenderObject, parentName, name)
{
	m_ToolBox = EngineGetToolBox();

	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}

	m_fTimeStart = 0.5;

	m_vSunLightFrames.clear();
	m_vFogFrames.clear();

	m_fTime = 0.0f;
	m_fTimeRate = 0.0f;
	m_dwTickCount = 0;

	m_szLookupTexture.Init( _T("Shader\\TimeOfDayLookupTexture.tex") );
	m_pLookupTexture = NULL;

	m_bMotion = false;
	m_bFog = true;
	m_bLighting = true;
	m_TODShaderCallback = NULL;

	// create shader callback
	m_TODShaderCallback = new CTimeOfDayShaderCallback();
}

CTimeOfDayObject::~CTimeOfDayObject( void )
{
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
	DeInit();

	if (m_TODShaderCallback)
	{
		delete m_TODShaderCallback;
		m_TODShaderCallback = NULL;
	}
}

IComponent *CTimeOfDayObject::Create(int nArgs, va_list argptr)
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
			self = new CTimeOfDayObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CTimeOfDayObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CTimeOfDayObject::Init()
{
	m_fTime = m_fTimeStart;
	m_dwTickCount = GetTickCount();

	if (!m_pLookupTexture)
	{
		// find lookup texture
		TEXTUREOBJECTPARAMS gettex;
		gettex.Name = &m_szLookupTexture;
		static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(gettex), &gettex ) == MSG_HANDLED)
		{
			m_pLookupTexture = dynamic_cast< ITextureObject*>(gettex.TextureObjectInterface);
		}

/*
		// if no texture found
		if (!m_pLookupTexture)
		{
			// try to create lookup texture
			CREATETEXTUREPARAMS texmsg;
			texmsg.bitDepth = 32;
			texmsg.Name = &m_szLookupTexture;
			texmsg.sizeX = 128;
			texmsg.sizeY = 256;
			static DWORD msgHash_CreateTexture = CHashString(_T("CreateTexture")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_CreateTexture, sizeof(texmsg), &texmsg );
			m_pLookupTexture = texmsg.TextureObjectInterface;
		}
*/
	}

	UpdateLookupTexture();
}

void CTimeOfDayObject::DeInit()
{
	if (m_pLookupTexture)
	{
		TEXTUREOBJECTPARAMS tobj;
		tobj.bLoad = false;
		tobj.Name = &m_szLookupTexture;
		tobj.TextureObjectInterface = m_pLookupTexture;
		static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
		m_pLookupTexture = NULL;
	}
}

void CTimeOfDayObject::Serialize( IArchive &ar )
{
	if(ar.IsReading())
	{
		// clear lists otherwise we will keep adding to them!!
		m_vSunLightFrames.clear();
		m_vFogFrames.clear();

		ar.Read( m_fTimeStart, _T("TimeStart") );
		ar.Read( m_fTimeRate, _T("TimeRate") );
	
		int iNumSunLightFrames = 0;
		ar.Read( iNumSunLightFrames, _T("NumSunLightFrames") );
		int iNumFogFrames = 0;
		ar.Read( iNumFogFrames, _T("NumFogFrames") );

		for( int i = 0; i < iNumSunLightFrames; i++ )
		{
			SUNLIGHTKEYFRAME tmpSunLightFrame;
			ar.Read( tmpSunLightFrame.m_wszName, _T("Name") );
			ar.Read( tmpSunLightFrame.m_AmbientColor, _T("AmbientColor") );
			ar.Read( tmpSunLightFrame.m_FullbrightColor, _T("FulLBrightColor") );
			ar.Read( tmpSunLightFrame.m_fDawnWeight, _T("DawnWeight") );
			ar.Read( tmpSunLightFrame.m_fNightWeight, _T("NightWeight") );
			m_vSunLightFrames.push_back( tmpSunLightFrame );
		}

		for( int i = 0; i < iNumFogFrames; i++ )
		{
			FOGKEYFRAME tmpFogFrame;
			ar.Read( tmpFogFrame.m_wszName, _T("Name") );
			ar.Read( tmpFogFrame.m_FogColor, _T("FogColor") );
			ar.Read( tmpFogFrame.m_fFogStart, _T("FogStart") );
			ar.Read( tmpFogFrame.m_fFogEnd, _T("FogEnd") );
			m_vFogFrames.push_back( tmpFogFrame );
		}
	}

	else
	{
		ar.Write( m_fTimeStart, _T("TimeStart") );
		ar.Write( m_fTimeRate, _T("TimeRate") );
	
		ar.Write( (int)m_vSunLightFrames.size(), _T("NumSunLightFrames") );
		ar.Write( (int)m_vFogFrames.size(), _T("NumFogFrames") );

		for( unsigned int i = 0; i < m_vSunLightFrames.size(); i++ )
		{
			ar.StartClass( _T("SunLightFrame") );
			ar.Write( m_vSunLightFrames[i].m_wszName, _T("Name") );
			ar.Write( m_vSunLightFrames[i].m_AmbientColor, _T("AmbientColor") );
			ar.Write( m_vSunLightFrames[i].m_FullbrightColor, _T("FulLBrightColor") );
			ar.Write( m_vSunLightFrames[i].m_fDawnWeight, _T("DawnWeight") );
			ar.Write( m_vSunLightFrames[i].m_fNightWeight, _T("NightWeight") );
			ar.EndClass();
		}

		for( unsigned int i = 0; i < m_vFogFrames.size(); i++ )
		{
			ar.StartClass( _T("FogFrame") );
			ar.Write( m_vFogFrames[i].m_wszName, _T("Name") );
			ar.Write( m_vFogFrames[i].m_FogColor, _T("FogColor") );
			ar.Write( m_vFogFrames[i].m_fFogStart, _T("FogStart") );
			ar.Write( m_vFogFrames[i].m_fFogEnd, _T("FogEnd") );
			ar.EndClass();
		}
	}
}

IHashString* CTimeOfDayObject::GetComponentType( void )
{
	return &m_hszNameType;
}

bool CTimeOfDayObject::IsKindOf( IHashString* compType )
{
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID() );
}

bool CTimeOfDayObject::Render( UINT pass , IEffect * override)
{

	return true;
}

bool CTimeOfDayObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CTimeOfDayObject::GetRenderPriority()
{
	return 0;
}

UINT CTimeOfDayObject::GetNumPass()
{
	return 1;
}

void CTimeOfDayObject::SetPosition( const Vec3 &vPosition )
{
	m_vPosition.Set( vPosition );
}

Vec3& CTimeOfDayObject::GetPosition()
{
	return m_vPosition;
}

void CTimeOfDayObject::GetBoundingSphere( Vec3 &position, float &radius )
{
	position.Set( m_vPosition );
	radius = 1;
}

IHashString* CTimeOfDayObject::GetBaseSortClass()
{
	return GetComponentType();
}

bool CTimeOfDayObject::IsAlphaObject()
{
	return false;
}

IMeshObject* CTimeOfDayObject::GetMeshObject()
{
	return NULL;
}

DWORD CTimeOfDayObject::OnGetTimeOfDayDesc( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(TIMEOFDAYDESC) );
	TIMEOFDAYDESC *pParams = (TIMEOFDAYDESC*)in_params;

	UpdateTime();

	if (pParams->vecSunDir != NULL)
	{
		EvaluateSunDirAtTime( m_fTime, *(pParams->vecSunDir) );
	}

	float fSkyDawnWeighting, fSkyNightWeighting;
	EvaluateSkyWeights( m_fTime, fSkyDawnWeighting, fSkyNightWeighting );

	pParams->fTime = m_fTime;
	pParams->fSkyDawnWeighting = fSkyDawnWeighting;
	pParams->fSkyNightWeighting = fSkyNightWeighting;
	pParams->bFog = m_bFog;
	pParams->bLighting = m_bLighting;

	return MSG_HANDLED_STOP;
}

DWORD CTimeOfDayObject::OnGetTimeOfDayStart( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(float) );
	float *pParams = (float*)in_params;
	*pParams = m_fTimeStart;

	return MSG_HANDLED_STOP;
}

DWORD CTimeOfDayObject::OnSetTimeOfDayStart( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(float) );
	float *pParams = (float*)in_params;
	m_fTimeStart = *pParams;

	return MSG_HANDLED_STOP;
}

DWORD CTimeOfDayObject::OnGetTimeOfDay( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(float) );
	float *pParams = (float*)in_params;
	*pParams = m_fTime;

	return MSG_HANDLED_STOP;
}

DWORD CTimeOfDayObject::OnSetTimeOfDay( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(float) );
	float *pParams = (float*)in_params;
	m_fTime = *pParams;

	return MSG_HANDLED_STOP;
}

DWORD CTimeOfDayObject::OnGetTimeOfDayRate( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(float) );
	float *pParams = (float*)in_params;
	*pParams = m_fTimeRate;

	return MSG_HANDLED_STOP;
}

DWORD CTimeOfDayObject::OnSetTimeOfDayRate( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(float) );
	float *pParams = (float*)in_params;
	m_fTimeRate = *pParams;

	return MSG_HANDLED_STOP;
}

DWORD CTimeOfDayObject::OnSetTimeOfDayMotion( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(bool) );
	bool *pParams = (bool*)in_params;
	m_bMotion = *pParams;

	return MSG_HANDLED_STOP;
}

DWORD CTimeOfDayObject::OnSetTimeOfDayDoLighting( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(bool) );
	bool *pParams = (bool*)in_params;
	m_bLighting = *pParams;

	return MSG_HANDLED_STOP;
}

DWORD CTimeOfDayObject::OnSetTimeOfDayDoFog( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(bool) );
	bool *pParams = (bool*)in_params;
	m_bFog = *pParams;

	return MSG_HANDLED_STOP;
}

void CTimeOfDayObject::UpdateTime()
{
	if (!m_pLookupTexture)
		Init();

	DWORD dwDelta;
	if (m_dwTickCount == 0)
	{
		dwDelta = 0;
		m_dwTickCount = GetTickCount();
	}
	else
	{
		dwDelta = GetTickCount() - m_dwTickCount;
		m_dwTickCount += dwDelta;
	}

	// divide by number of seconds in a day to convert from day seconds to a 0..1 scalar
	if( m_bMotion )
	{
		float fDelta = m_fTimeRate * ((float)dwDelta / 1000.0f) / 86400.0f;
		m_fTime = fmodf( m_fTime + fDelta, 1.0f );
	}
}

void CTimeOfDayObject::UpdateLookupTexture()
{
	if (!m_pLookupTexture)
		return;

	int iWidth = m_pLookupTexture->GetWidth();
	int iHeight = m_pLookupTexture->GetHeight();
	int iBPP = m_pLookupTexture->GetColorDepth();
	unsigned char *pBaseData = new unsigned char[iWidth*iHeight*(iBPP/8)];
	unsigned char *pData = pBaseData;

	Vec3 vecColor;
	float invWidth  = 1.f/(float)(iWidth-1);
	float invHeight = 1.f/(float)(iHeight-1);
	for (int y=0; y < iHeight; y++)
	{
		float fTime = (float)y*invHeight;
		for (int x=0; x < iWidth; x++)
		{
			EvaluateColor( (float)x*invWidth, fTime, vecColor );
			pData[0] = (unsigned char)(vecColor.x);
			pData[1] = (unsigned char)(vecColor.y);
			pData[2] = (unsigned char)(vecColor.z);
			if (iBPP == 32)
			{
				pData[3] = 255;
				pData += 4;
			}
			else if (iBPP == 24)
			{
				pData += 3;
			}
		}
	}

	m_pLookupTexture->Write( pBaseData );
	delete[] pBaseData;
}

void CTimeOfDayObject::EvaluateSunDirAtTime( float fTime, Vec3 &vecPos )
{
	float w = (fTime-0.5f) * 2.0f * (float)M_PI;
	vecPos.x = sinf(w);
	vecPos.y = 0.0f;
	vecPos.z = cosf(w);
	vecPos.Normalize();
}

void CTimeOfDayObject::EvaluateColor( float in_fFalloff, float in_fTime, Vec3 &out_Color )
{
	assert(in_fTime >= 0.f && in_fTime <= 1.f);
	assert(in_fFalloff >= 0.f && in_fFalloff <= 1.f);

	int nSunlightKeyframes = m_vSunLightFrames.size();
	if (nSunlightKeyframes == 0)
	{
		out_Color.Set( 1.0f, 1.0f, 1.0f );
		return;
	}
	const int rowCount = nSunlightKeyframes; // number of daytime samples
	const float timeMult = (float)(rowCount);

	// cosine interpolation in x-direction
	float fx1 = 0.5f + 0.5f*cosf(in_fFalloff*(float)M_PI);
	float fx2 = 1.f-fx1;

	in_fTime *= timeMult;
	int row = ((int)in_fTime)%rowCount;
	int nextrow = (row+1)%rowCount;

	float row_weight = fmodf(in_fTime,1.f);

	// linearely interpolate in y-direction
	float fy1 = 1.f-row_weight;
	float fy2 = 1.f-fy1;

	// interpolate across x and y of table
	Vec3 Color1, Color2;
	Vec3 ambientColor, fullbrightColor;
	// Color1
	ambientColor.Set(m_vSunLightFrames[row].m_AmbientColor.x, m_vSunLightFrames[row].m_AmbientColor.y, m_vSunLightFrames[row].m_AmbientColor.z);
	fullbrightColor.Set(m_vSunLightFrames[row].m_FullbrightColor.x, m_vSunLightFrames[row].m_FullbrightColor.y, m_vSunLightFrames[row].m_FullbrightColor.z );
	Color1 = ambientColor * fx1 + fullbrightColor * fx2;
	// Color2
	ambientColor.Set(m_vSunLightFrames[nextrow].m_AmbientColor.x, m_vSunLightFrames[nextrow].m_AmbientColor.y, m_vSunLightFrames[nextrow].m_AmbientColor.z);
	fullbrightColor.Set(m_vSunLightFrames[nextrow].m_FullbrightColor.x, m_vSunLightFrames[nextrow].m_FullbrightColor.y, m_vSunLightFrames[nextrow].m_FullbrightColor.z );
	Color2 = ambientColor * fx1 + fullbrightColor * fx2;
	// Out Color
	out_Color = Color1 * fy1 + Color2 * fy2;
}

void CTimeOfDayObject::EvaluateSkyWeights( float in_fTime, float &out_fDawnWeight, float &out_fNightWeight )
{
	assert(in_fTime >= 0.f && in_fTime <= 1.f);

	int nSunlightKeyframes = m_vSunLightFrames.size();
	if (nSunlightKeyframes == 0)
	{
		out_fDawnWeight = 1.0f;
		out_fNightWeight = 0.0f;
		return;
	}

	const int rowCount = nSunlightKeyframes; // number of daytime samples
	in_fTime *= (float)(rowCount);
	int row = ((int)in_fTime)%rowCount;
	int nextrow = (row+1)%rowCount;
	float row_weight = fmodf(in_fTime,1.f);

	// linearely interpolate in y-direction
	float fy1 = 1.f-row_weight;
	float fy2 = 1.f-fy1;

	out_fDawnWeight   = fy1*m_vSunLightFrames[row].m_fDawnWeight + fy2*m_vSunLightFrames[nextrow].m_fDawnWeight;
	out_fNightWeight  = fy1*m_vSunLightFrames[row].m_fNightWeight + fy2*m_vSunLightFrames[nextrow].m_fNightWeight;
}
