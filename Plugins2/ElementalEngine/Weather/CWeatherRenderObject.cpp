///============================================================================
/// \file		CWeatherRenderObject.cpp
/// \brief		Implementation of CWeatherRenderObject Class
/// \date		08-21-2005
/// \author		Marvin Gouw
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
/*#include "CWeatherRenderObject.h"
#include "ITextureManager.h"
#include <Vision.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//==========================================================================

REGISTER_COMPONENT(CWeatherRenderObject);

IComponent *CWeatherRenderObject::Create(va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = new CWeatherRenderObject(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CWeatherRenderObject::CWeatherRenderObject(IHashString *parentName, IHashString *name) : 
	CRenderObject( _T("CParticleManager"),_T("CWeatherRenderObject"), parentName, name)
{
	m_WeatherController = NULL;
	DeInit();
	AddToHierarchy();
}

CWeatherRenderObject::~CWeatherRenderObject()
{
	// remove bounding boxed object
	RemoveBoundingObject();

	DeInit();
	RemoveFromHierarchy();
}

void CWeatherRenderObject::Init()
{
	if( !m_WeatherController )
	{
		m_WeatherController = new VisMapWeatherController();
	}
	m_WeatherController->SetParticleScale( m_fParticleScale );
	m_WeatherController->SetWeatherEffect( m_eWeatherEffect, m_fIntensity );

	UpdateGlobalObject();
}

void CWeatherRenderObject::DeInit()
{	
	m_fIntensity =  0.f ;
	m_eWeatherEffect =  DEFAULT_WEATHER;
	m_fParticleScale =  1.f;
	if( m_WeatherController )
	{
		delete m_WeatherController;
		m_WeatherController = NULL;
	}
}

void CWeatherRenderObject::Serialize(IArchive &ar)
{
	StdString szEffect;		
	if (ar.IsReading())
	{	
		// deinit before changing anything
		DeInit();

		ar.Read( m_fIntensity, _T("Intensity") );	
		ar.Read( m_fParticleScale, _T("ParticleScale") );	
		ar.Read( szEffect, _T("Effect") );
		if( szEffect == "RAIN_WEATHER" )
		{
			m_eWeatherEffect = RAIN_WEATHER;
		}
		else if( szEffect == "SNOW_WEATHER" )
		{
			m_eWeatherEffect = SNOW_WEATHER;
		}
		else
		{
			m_eWeatherEffect = DEFAULT_WEATHER;
		}
	}
	else
	{
		if( m_eWeatherEffect == RAIN_WEATHER )
		{
			szEffect = "RAIN_WEATHER";
		}
		else if( m_eWeatherEffect == SNOW_WEATHER )
		{
			szEffect = "SNOW_WEATHER";
		}
		else
		{
			szEffect = "DEFAULT_WEATHER";
		}
		ar.Write( m_fIntensity, _T("Intensity") );	
		ar.Write( m_fParticleScale, _T("ParticleScale") );	
		ar.Write( szEffect, _T("Effect") );		
	}
}

bool CWeatherRenderObject::Render( UINT pass )
{
	m_WeatherController->UpdateWeather(); 
	
	return true;
}

bool CWeatherRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return ( obj > this );
}

DWORD CWeatherRenderObject::GetRenderPriority()
{
	return 0;//hacky..sky uses 999999...99 shouldn't really matter for this one
}

void CWeatherRenderObject::SetPosition( const IVec3 &vec )
{
}

IVec3 &CWeatherRenderObject::GetPosition()
{
	static CVec3 zero(0,0,0);
	return zero;
}

IMeshObject* CWeatherRenderObject::GetMeshObject()
{
	return NULL;
}


bool CWeatherRenderObject::IsKindOf( IHashString * compType )
{	
	static CHashString  CSBCOMP(_T("CWeatherRenderObject") );
	if(CRenderObject::IsKindOf(compType)) 
	{
		return true;
	}
	return (compType->GetUniqueID() == CSBCOMP.GetUniqueID());
}*/