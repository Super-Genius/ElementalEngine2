///============================================================================
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
#include "WeatherController.h"
/*
CWeatherController::CWeatherController()
:
m_fIntensity( 0.f ),
m_eWeatherEffect( DEFAULT_WEATHER ),
m_WeatherGroupPointer( NULL ),
m_fParticleScale( 1.f )
{	
	WeatherPGroup_cl::InitWeatherPGroupProfiling();
	WeatherPGroup_cl::SetGlobalParticleScaling( m_fParticleScale );	
}

void CWeatherController::SetParticleScale( float particlescale )
{
	m_fParticleScale = particlescale;	
	WeatherPGroup_cl::SetGlobalParticleScaling( m_fParticleScale );
}

void CWeatherController::SetIntensity( float intensity )
{
	m_fIntensity = intensity;
}

void CWeatherController::UpdateWeather( )
{
	if( m_WeatherGroupPointer )
	{
		m_WeatherGroupPointer->SetIntensity( m_fIntensity );
	}
}

void CWeatherController::SetWeatherEffect( WEATHER_EFFECT eff, float intensity )
{
	m_fIntensity = intensity;
	m_eWeatherEffect = eff;
	m_RainMap.CreateFromCurrentWorld(32,32,false);
	switch( m_eWeatherEffect )
	{
	case SNOW_WEATHER:
		{
			m_WeatherGroupPointer = new SnowPGroup_cl(&m_RainMap, 4000);
			break;
		}
	case RAIN_WEATHER:
		{
			m_WeatherGroupPointer = new RainPGroup_cl(&m_RainMap, 4000);
			break;
		}
	default:
		{
			if( m_WeatherGroupPointer )
				delete m_WeatherGroupPointer;
			m_WeatherGroupPointer = NULL;
			break;
		}
	}
}
*/