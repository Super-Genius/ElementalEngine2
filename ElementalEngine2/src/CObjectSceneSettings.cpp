///============================================================================
/// \file	CObjectSceneSettings.cpp
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
#include "CObjectSceneSettings.h"

//REGISTER_COMPONENT(CObjectSceneSettings);


CObjectSceneSettings::CObjectSceneSettings(IHashString *parentName, IHashString *name) 
//: OBJECTTEMPLATE( CRenderManager, CObjectSceneSettings, ISceneSettings, parentName, name )
{
	m_fElapsedTime = 0.0f;
	DeInit();	
}

void CObjectSceneSettings::SetShaderFlags( SHADER_RENDER_FLAG encoding )
{
	m_Flags = encoding;
}

void CObjectSceneSettings::AddShaderFlag( SHADER_RENDER_FLAG encoding )
{
	m_Flags |= encoding;
}

void CObjectSceneSettings::RemoveShaderFlag( SHADER_RENDER_FLAG encoding )
{
	m_Flags &= ~encoding;
}

SHADER_RENDER_FLAG CObjectSceneSettings::GetShaderFlags()
{
	return m_Flags;
}

void CObjectSceneSettings::SetShadowCast( bool val )
{
	if( val )
	{
		m_Flags |= SHADOW_CAST;
	}
	else
	{
		m_Flags &= ~SHADOW_CAST;
	}
}

void CObjectSceneSettings::SetShadowReceive( bool val )
{
	if( val )
	{
		m_Flags |= SHADOW_RECEIVE;
	}
	else
	{
		m_Flags &= ~SHADOW_RECEIVE;
	}
}

void CObjectSceneSettings::SetLightmapCast( bool val )
{
	if( val )
	{
		m_Flags |= LIGHTMAP_CAST;
	}
	else
	{
		m_Flags &= ~LIGHTMAP_CAST;
	}
}

void CObjectSceneSettings::SetLightmapReceive( bool val )
{
	if( val )
	{
		m_Flags |= LIGHTMAP_RECEIVE;
	}
	else
	{
		m_Flags &= ~LIGHTMAP_RECEIVE;
	}
}
bool CObjectSceneSettings::IsEnabled( SHADER_RENDER_FLAG encoding )
{
	return (m_Flags & encoding ) == 0 ? false : true ;
}

void CObjectSceneSettings::SetFogDistance( float val )
{
	m_Flags |= FOG;
}

void CObjectSceneSettings::SetLOD( int val )
{

}

/// IObject  functions
/// \brief	Create the a instance of IObject
/// \return	IComponent pointer
IComponent * CObjectSceneSettings::Create(va_list argptr)
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
		self = (IObject*)new CObjectSceneSettings(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

/// \brief	The Initialization for the Object
/// \param	IManager* pManager: Parent Manager
void CObjectSceneSettings::Init()
{
}

/// \brief	The Update Function for the Object
/// \return	True on sucess, False on faulure
bool CObjectSceneSettings::Update()
{
	return true;
}

/// \brief	The DeInit Function for the Object
void CObjectSceneSettings::DeInit()
{
	m_Flags = 0;
	m_iDetailLightCount = 0;
	m_iActiveLightCount = 0;
	m_iShadowMapCount = 0;
}

/// \brief either reads or writes the entity information to a file
void CObjectSceneSettings::Serialize(IArchive &ar)
{
}

void CObjectSceneSettings::PushFlagState()
{
	//saves current state
	m_SavedStates.push( m_Flags );
}

void CObjectSceneSettings::PopFlagState()
{
	m_Flags = m_SavedStates.top();
	m_SavedStates.pop();
}

void CObjectSceneSettings::SetActiveLightCount( int iLightCount )
{
	m_iActiveLightCount = min( iLightCount, MAX_COMPLEX_LIGHTS );
}

int CObjectSceneSettings::GetActiveLightCount() const
{
	return m_iActiveLightCount;
}

void CObjectSceneSettings::SetDetailLightCount( int iDetailLightCount )
{
	m_iDetailLightCount = min( iDetailLightCount, MAX_SIMPLE_LIGHTS );
}

int CObjectSceneSettings::GetDetailLightCount() const
{
	return m_iDetailLightCount;
}

void CObjectSceneSettings::SetShadowMapCount( int iShadowMapCount )
{
	m_iShadowMapCount = min( iShadowMapCount, MAXSHADOWMAPS );
}

int CObjectSceneSettings::GetShadowMapCount() const
{
	return m_iShadowMapCount;
}

