///==========================================================================
/// \file	CLightObject.hpp
/// \brief	Implementation of CLightObject interface for adapting to Vision
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
#include "CLightObject.h"
#include "Vec3.h"
#include "CLightManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ElementalEngine;

REGISTER_COMPONENT(CLightObject);

//REGISTER_MESSAGE_HANDLER( SetDirection, OnMakeDirectional, CLightObject );
//REGISTER_MESSAGE_HANDLER( MakePointLight, OnMakePointLight, CLightObject );
//REGISTER_MESSAGE_HANDLER( GetGlobalPosition,	OnGetGlobalPosition, CLightObject );
//REGISTER_MESSAGE_HANDLER( SetGlobalPosition,	OnSetGlobalPosition, CLightObject );
REGISTER_MESSAGE_HANDLER( SetColor,		OnSetColor, CLightObject );
REGISTER_MESSAGE_HANDLER( SetAttenuation,		OnSetAttenuation, CLightObject );
REGISTER_MESSAGE_HANDLER( EnableLightShadows,		OnEnableLightShadows, CLightObject );

///
/// Constructor / Destructor
///
CLightObject::CLightObject(IHashString *parentName, IHashString *name, bool bAddToHier) : 
	SCENEOBJECTTEMPLATE( CLightManager, CLightObject, ILightObject, parentName, name )
{
	m_bTransformIsDirty = true;
	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}

	DeInit();
}

CLightObject::~CLightObject()
{
	static DWORD msgHash_UnregisterLight = CHashString("UnregisterLight").GetUniqueID();
	m_ToolBox->SendMessage(msgHash_UnregisterLight, sizeof( ILightObject *), this, NULL, NULL );
	DeInit();
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

/// \brief Set position of the render object
void CLightObject::SetPosition( const Vec3 &vec )
{
	m_vPosition = vec;
}

/// \brief Set position of the render object
void CLightObject::UpdateLightInScene( bool updateShadows, int *out_pShadowCastObjectCount )
{
	if (out_pShadowCastObjectCount)
		*out_pShadowCastObjectCount = 0;

	CSceneLightVisitor lightVisitor;
	lightVisitor.SetLight( this );

	VISITSCENESPHEREPARAMS vsp;
	vsp.visitor = (IVisitor*)&lightVisitor;
	vsp.center = &m_GlobalPos;
	vsp.radius = m_AttenDistance;
	vsp.sortID = RENDEROBJECT3D;
	static DWORD msg_VisitSceneSphere = CHashString(_T("VisitSceneSphere")).GetUniqueID();
	m_ToolBox->SendMessage( msg_VisitSceneSphere, sizeof(VISITSCENESPHEREPARAMS), &vsp, NULL, NULL );

	if( updateShadows )
	{
		lightVisitor.UpdateShadowMaps();
		if (out_pShadowCastObjectCount)
			*out_pShadowCastObjectCount = lightVisitor.GetShadowCastObjectCount();
	}
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CLightObject::Create(int nArgs, va_list argptr)
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
			self = new CLightObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CLightObject(parentName, name, bAddToHier);
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
void CLightObject::Init()
{
	//grab the renderer
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	renderInterfaceParam.m_RendererInterface = NULL;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam, NULL, NULL );
	m_IRenderer = renderInterfaceParam.m_RendererInterface;

	
	//Register this light with the manager, creates a new light mapping
	static DWORD msgHash_RegisterLight = CHashString("RegisterLight").GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterLight, sizeof( ILightObject *), this, NULL, NULL );	
	
	m_fStartTime = m_Timer->GetTime();

	m_bTransformIsDirty = true;
}

/// \brief Update the object
/// \return true or false
bool CLightObject::Update()
{
	if( m_bTransformIsDirty )
	{
		Matrix4x4 parentTransform;
		parentTransform.SetIdentity();
		// Get the transform from the parent entity
		static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGlobalTransform, sizeof(Matrix4x4), &parentTransform, GetParentName() );

		m_GlobalPos = parentTransform * m_LocalPos;
		m_GlobalDir = parentTransform.TransformDirection( m_LocalDir );
 		m_bTransformIsDirty = false;

		UpdateBoundingObject();
	}

	m_fCurrentTime = m_Timer->GetTime();
	//Set light intensity based on keyframe
	if ((m_fCurrentTime - m_fStartTime) > m_fTotalTime)
	{
		if( m_bSelfDelete )
		{
			//delete self
		}else
		{
			//looping?
			m_fStartTime = m_fCurrentTime;
		}
	}
	float dispTime = m_fCurrentTime - m_fStartTime;
	if( m_bAnimated &&
		m_FadeFrames[ RED ].GetLastKeyIndex() >= 0 )
	{
		int key = m_FadeFrames[ RED ].GetKeyframeIndex( dispTime );
		int nextKey = key + 1;
		//interpolate
		m_Color[ RED ] = m_FadeFrames[ RED ].Interpolate( dispTime, key, nextKey );
		m_Color[ GREEN ] = m_FadeFrames[ GREEN].Interpolate( dispTime, key, nextKey );
		m_Color[ BLUE ] = m_FadeFrames[ BLUE ].Interpolate( dispTime, key, nextKey );
	}

	return true;
}

/// \brief Update the render object( Render )
/// \return true or false
bool CLightObject::Render( UINT pass, IEffect * override )
{
	//TODO: Render the flare?
	//temp, render the texture
	CLightManager::RenderLight( this );
	return true;
}

void CLightObject::GetBoundingSphere( Vec3 &position, float &radius )
{
	position = m_GlobalPos;
	radius = m_AttenDistance;
}

bool CLightObject::IsAlphaObject()
{	
	return false;
}

bool CLightObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{	
	//order for particles does not matter
	return ( false );
}

void CLightObject::DeInit()
{
	m_bSelfDelete  = false;
	m_Version = 0;
	m_bAnimated = false;
	m_fCurrentTime = 0.f;
	m_bTransformIsDirty = true;
	m_LocalDir.Set( .25f, .5f, 1.2f );
	SetColor( 1.0, 1.0, .5f );
	SetIntensity( 1.0f );
	m_AttenDistance = 99999.f;	
	m_bCastShadows = false;
	m_bDetailLight = false;
	m_LightType = DIRECTIONAL_LIGHT;

	//remove from scene manager
	RemoveBoundingObject();
}

/*
DWORD CLightObject::OnMakeDirectional(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( Vec3* ) );
	Vec3 * msg = (Vec3*)params;
	if( msg )
	{
		m_Direction[ 0 ] = msg->x;
		m_Direction[ 1 ] = msg->y;
		m_Direction[ 2 ] = msg->z;
	}
	m_LightType = DIRECTIONAL_LIGHT;

	return MSG_HANDLED_STOP;
}

DWORD CLightObject::OnGetGlobalPosition(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(Vec3), size);
	*(Vec3*)params = m_GlobalPos;
	return MSG_HANDLED_PROCEED;
}

DWORD CLightObject::OnSetGlobalPosition(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	SetPosition( *(Vec3*)params );
	return MSG_HANDLED_PROCEED;
}

DWORD CLightObject::OnMakePointLight(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( Vec3* ) );
	Vec3 * msg = (Vec3*)params;
	if( msg )
	{
		SetPosition( *msg );
	}
	m_LightType = OMNI_POINT_LIGHT;
	return MSG_HANDLED_STOP;
}
*/
DWORD CLightObject::OnSetAttenuation(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( float ) );
	float * msg = (float*)params;
	if( msg )
	{
		m_AttenDistance = *msg;
	}
	return MSG_HANDLED_STOP;
}

DWORD CLightObject::OnSetColor(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( Vec3 ) );
	Vec3 * msg = (Vec3*)params;
	if( msg )
	{
		SetColor( msg->x, msg->y, msg->z );
	}
	return MSG_HANDLED_STOP;
}

/// \brief either reads or writes the entity information to a file
/// Currently Loads up the entity completely if reading(inside vision too)
void CLightObject::Serialize(IArchive &ar)
{
	StdString tmpType;
	TCHAR buf[ 256 ];

	if (ar.IsReading())
	{
		ar.Read( m_Version, _T("Version") );
		
		{
			ar.Read( m_LocalPos, _T("Position") );
			ar.Read( m_LocalDir, _T("Direction") );

			//float m_AttenDistance;
			ar.Read( m_AttenDistance, _T("Attenuation") );

			//float m_Color[4];
			ar.Read( m_Color[0], _T("ColorRed") );
			ar.Read( m_Color[1], _T("ColorGreen") );
			ar.Read( m_Color[2], _T("ColorBlue") );
			ar.Read( m_Color[3], _T("ColorAlpha") );
		
			ar.Read( tmpType, _T("LightType") );
			if( tmpType == "DIRECTIONAL_LIGHT" )
				m_LightType = DIRECTIONAL_LIGHT;
			else if( tmpType == "SPOT_LIGHT" )
				m_LightType = SPOT_LIGHT;
			else if( tmpType == "OMNI_POINT_LIGHT" )
				m_LightType = OMNI_POINT_LIGHT;
			else if( tmpType == "HEMI_POINT_LIGHT" )
				m_LightType = HEMI_POINT_LIGHT;
			else if( tmpType == "AMBIENT_LIGHT" )
				m_LightType = AMBIENT_LIGHT;
	    
			//bool m_bCastShadows;
			ar.Read( m_bCastShadows, _T("CastShadows") );
			int numKeyFrames;
			if( m_Version >= 1000 )
			{
				ar.Read( numKeyFrames, _T("NumKeyframes"));
				if( numKeyFrames > 0 )
				{
					m_bAnimated = true;
				}
				float time = 0;					
				for( int i = 0; i < numKeyFrames; i++ )
				{
					float value[3];
					sprintf( buf, "time%d", i );
					ar.Read( time, buf );
					sprintf( buf, "r%d", i );
					ar.Read(  value[ RED ], buf );
					sprintf( buf, "g%d", i );
					ar.Read(  value[ GREEN ], buf );
					sprintf( buf, "b%d", i );
					ar.Read(  value[ BLUE ], buf );
					m_FadeFrames[ RED ].AddKeyframe( time, value[ RED ] );
					m_FadeFrames[ GREEN].AddKeyframe( time, value[ GREEN ] );
					m_FadeFrames[ BLUE ].AddKeyframe( time, value[ BLUE ] );
				}
				m_fTotalTime = time;
			}
		}
	}
	else
	{
		int version = 1000;//save version
		ar.Write( version, _T("Version") );
		ar.Write( m_LocalPos, _T("Position") );
		ar.Write( m_LocalDir, _T("Direction") );

		//float m_AttenDistance;
		ar.Write( m_AttenDistance, _T("Attenuation") );
		
		//float m_Color[4];
		ar.Write( m_Color[0], _T("ColorRed") );
		ar.Write( m_Color[1], _T("ColorGreen") );
		ar.Write( m_Color[2], _T("ColorBlue") );
		ar.Write( m_Color[3], _T("ColorAlpha") );

		if( m_LightType == DIRECTIONAL_LIGHT )
			tmpType = "DIRECTIONAL_LIGHT";
		else if( m_LightType == SPOT_LIGHT )
			tmpType = "SPOT_LIGHT";
		else if( m_LightType == OMNI_POINT_LIGHT )
			tmpType = "OMNI_POINT_LIGHT";
		else if( m_LightType == HEMI_POINT_LIGHT )
			tmpType = "HEMI_POINT_LIGHT";
		else if( m_LightType == AMBIENT_LIGHT )
			tmpType = "AMBIENT_LIGHT";
		ar.Write( tmpType, _T("LightType") );

		//bool m_bCastShadows;
		ar.Write( m_bCastShadows, _T("CastShadows") );
		if( m_FadeFrames[ RED ].GetLastKeyIndex() == m_FadeFrames[ BLUE ].GetLastKeyIndex() &&
			m_FadeFrames[ BLUE ].GetLastKeyIndex() == m_FadeFrames[ GREEN ].GetLastKeyIndex() )
		{
			int numKeyFrames = (int)m_FadeFrames[ RED ].GetLastKeyIndex() + 1;
			ar.Write( numKeyFrames, _T("NumKeyframes"));
			for( int i = 0; i < numKeyFrames; i++ )
			{
				float time;
				float value[3];
				m_FadeFrames[ RED ].GetKeyframeValues( i, &time, &value[ RED ] );
				m_FadeFrames[ GREEN].GetKeyframeValues( i, NULL, &value[ GREEN ] );
				m_FadeFrames[ BLUE ].GetKeyframeValues( i, NULL, &value[ BLUE ] );
				sprintf( buf, "time%d", i );
				ar.Write( time, buf );
				sprintf( buf, "r%d", i );
				ar.Write(  value[ RED ], buf );
				sprintf( buf, "g%d", i );
				ar.Write(  value[ GREEN ], buf );
				sprintf( buf, "b%d", i );
				ar.Write(  value[ BLUE ], buf );
			}
		}else
		{
			int numKeyFrames = 0;
			ar.Write(  numKeyFrames, _T("NumKeyframes"));
		}
	}
}

IHashString * CLightObject::GetBaseSortClass()
{
	static CHashString CLO(_T("CLightObject"));
	return &CLO;
}

IMeshObject * CLightObject::GetMeshObject()
{
	
	return NULL;
}

/// \brief	Get the bounding box of the object
bool CLightObject::GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation )
{
	Vec3 vMin, vMax;
	return false;
}

bool CLightObject::IsKindOf( IHashString * compType )
{	
	static CHashString  CPROCOMP(_T("CLightObject") );
	
	return (compType->GetUniqueID() == CPROCOMP.GetUniqueID());
}


DWORD CLightObject::OnEnableLightShadows(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( bool* ) );
	bool * msg = (bool*)params;
	if( msg )
	{
		EnableShadows( *msg );
	}
	return MSG_HANDLED_STOP;
}

float CLightObject::GetShadowAttenuation()
{
	float atten = GetAttenuationDistance();
	if( atten > MAX_SHADOW_ATTEN )
		return MAX_SHADOW_ATTEN;
	else
		return atten;
}

bool CLightObject::GetLightPriority( const Vec3 &objectPos, float objectRadius, float &out_priority )
{
	bool bObjectIsAffected = true;
	//grab distance of light
	float intensity = m_Color[3];
	//now calculate 3d distance squared
	Vec3 difference = objectPos - m_GlobalPos;
	float maxRadialDistance = objectRadius + m_AttenDistance;
	float distToEdgeOfObject = difference.x*difference.x + difference.y*difference.y + difference.z*difference.z;
	if (distToEdgeOfObject > maxRadialDistance*maxRadialDistance)
	{
		distToEdgeOfObject = m_AttenDistance;
		bObjectIsAffected = false;
		return false;
	}
	float distance = sqrtf( distToEdgeOfObject );
	// calculate a priority based on distance to edge of object, light radius and light intensity
	float priority = (1.0f - (distance / maxRadialDistance));
	if( GetLightType() ==  AMBIENT_LIGHT )
	{
		priority = intensity + 1.0f; // ambient lights are always higher priority
	}
	out_priority = priority;
	return bObjectIsAffected;
}
