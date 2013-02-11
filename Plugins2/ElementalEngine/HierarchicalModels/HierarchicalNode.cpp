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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT HierarchicalNode::INVALID_NODE = 0xBADDBADD;

HierarchicalNode::HierarchicalNode()
{
	IToolBox * toolbox = EngineGetToolBox();
	m_Parent = NULL;
	m_Geometry = NULL;				
	//m_Cal3DObject = false;
	m_ParentId = INVALID_NODE;
	m_Rotation = NULL;
	m_Scaling = NULL;
	m_Translation = NULL;
	m_KeyFrames[ ROTATE ] = NULL;
	m_KeyFrames[ SCALE ] = NULL;
	m_KeyFrames[ POS ] = NULL;
	m_LastKey[ ROTATE ] = 0;
	m_LastKey[ SCALE ] = 0;
	m_LastKey[ POS ] = 0;

	ZeroOffsetScale = true;
	ZeroOffsetRotation = true;
	ZeroOffsetTranslation = true;
}

HierarchicalNode::~HierarchicalNode()
{
	//release geometry
	SAFE_RELEASE( m_Geometry );
	CleanKeys();
}

/*
bool HierarchicalNode::IsCalObject()
{
	return m_Cal3DObject;
}

void HierarchicalNode::SetCalObject( const TCHAR * classname )
{
	m_Cal3dClassName.Init( classname );
	m_Cal3DObject = true;
}
*/

void HierarchicalNode::SetObjectType( StdString szType )
{
	m_szObjectType = szType;
}

StdString HierarchicalNode::GetObjectType( void )
{
	return m_szObjectType;
}

void HierarchicalNode::SetFilePath( const TCHAR * szFilePath )
{
	m_hszFilePath.Init( szFilePath );
}

TCHAR* HierarchicalNode::GetFilePath( void )
{
	return (TCHAR*)m_hszFilePath.GetString();
}

void HierarchicalNode::SetObject( IGeometry * obj )
{
	m_Geometry = obj ;
}

void HierarchicalNode::SetParent( HierarchicalNode * parent, UINT pid )
{
	m_Parent = parent;	
	m_ParentId = pid;
	if( parent )
	{
		//set the child doubly link
		if( parent->m_Children.find( this ) == parent->m_Children.end() )
		{
			parent->m_Children.insert( this );
		}
	}
}

HierarchicalNode* HierarchicalNode::GetParent( void ) 
{
	return m_Parent;
}

UINT HierarchicalNode::GetParentIndex()
{
	return m_ParentId;
}

StdString HierarchicalNode::GetName()
{
	return m_Name;
}

void HierarchicalNode::SetName( StdString name )
{
	m_Name = name;
}

bool HierarchicalNode::IsRoot()
{
	return ( m_Parent == NULL );
}

void HierarchicalNode::CleanKeys()
{
	SAFE_DELETE_ARRAY( m_Rotation );
	SAFE_DELETE_ARRAY( m_Scaling );
	SAFE_DELETE_ARRAY( m_Translation );
	SAFE_DELETE_ARRAY( m_KeyFrames[ ROTATE ] );
	SAFE_DELETE_ARRAY( m_KeyFrames[ SCALE ] );
	SAFE_DELETE_ARRAY( m_KeyFrames[ POS ] );
	m_LastKey[ ROTATE ] = 0;
	m_LastKey[ SCALE ] = 0;
	m_LastKey[ POS ] = 0;
}

void HierarchicalNode::ResetKeyFrames( UINT sizerot, UINT sizescale, UINT sizetranslation )
{
	sizerot = sizerot == 0 ? 1 : sizerot;
	sizescale = sizescale == 0 ? 1 : sizescale;
	sizetranslation = sizetranslation == 0 ? 1 : sizetranslation;
	CleanKeys();
	m_Rotation = new Quaternion[ sizerot ];
	m_Scaling = new Vec3[ sizescale ];
	m_Translation = new Vec3[ sizetranslation ];
	m_KeyFrames[ ROTATE ] = new float[ sizerot ];
	m_KeyFrames[ SCALE ] = new float[ sizescale ];
	m_KeyFrames[ POS ] = new float[ sizetranslation ];
	m_LastKey[ ROTATE ] = sizerot - 1;
	m_LastKey[ SCALE ] = sizescale - 1;
	m_LastKey[ POS ] = sizetranslation - 1;
	//zero out the first members, default keyframes:
	m_KeyFrames[ ROTATE ][ 0 ] = 0;
	m_KeyFrames[ SCALE ][ 0 ] = 0;
	m_KeyFrames[ POS ][ 0 ] = 0;
	m_Rotation[ 0 ].Set( 0,0,0,1.0 );
	m_Scaling[ 0 ].Set( 1.0, 1.0, 1.0 );
	m_Translation[ 0 ].Set( 0, 0, 0 );


}
void HierarchicalNode::SetInitialPos( Quaternion rot, Vec3 trans, Vec3 scale)
{
	// Set Initial Node Position Data and Keyframe Data used for animations.
	ResetKeyFrames( 1, 1, 1 );

	m_KeyFrames[ ROTATE ][ 0 ] = 0; // Animations
	m_Rotation[ 0 ] = rot;
	
	m_KeyFrames[ SCALE ][ 0 ]  = 0; // Animations
	m_Scaling[0] = scale;

	m_KeyFrames[ POS ][ 0 ] = 0; // Animations
	m_Translation[ 0 ] = trans;
}

/// Get the quaternion rotation
int HierarchicalNode::GetRotation( Quaternion &q, const float time, const int currentkeyframe )
{
	int prev, next;
	float relativetime;
	GetKeyFrames( currentkeyframe, time, prev, next, relativetime, ROTATE);
	//interpolate
	if( prev != next ){
		//QuatSlerp( m_Rotation[ prev ], m_Rotation[ next ], relativetime, q );
		q = m_Rotation[prev].Slerp( m_Rotation[next], relativetime );
	}else
	{
		q = m_Rotation[ prev ];
	}
	return prev;
}

/// Get the euler rotation
int HierarchicalNode::GetRotation( EulerAngle &euler, const float time, const int currentkeyframe )
{
	//interpolate
	Quaternion q;
	int prev = GetRotation( q, time, currentkeyframe );
	//TODO: Check that this implementation's euler order is correct!!
	q.ToEuler( euler );
	return prev;
}

/// Get the translation
int HierarchicalNode::GetTranslation( Vec3 &translation, const float time,const int currentkeyframe )
{
	int prev, next;
	float relativetime;
	GetKeyFrames( currentkeyframe, time, prev, next, relativetime, POS );
	if( prev != next )
	{
		VecSlerp( m_Translation[ prev ], m_Translation[ next ], relativetime, translation );
	}else
	{
		translation = m_Translation[ prev ];
	}
	return prev;
}

/// Get the scaling
int HierarchicalNode::GetScaling( Vec3 &scaling, const float time, const int currentkeyframe )
{
	int prev, next;
	float relativetime;
	GetKeyFrames( currentkeyframe, time, prev, next, relativetime, SCALE );
	if( prev != next )
	{
		VecSlerp( m_Scaling[ prev ], m_Scaling[ next ], relativetime, scaling );
	}else
	{
		scaling = m_Scaling[ prev ];
	}
	return prev;
}
int HierarchicalNode::GetNumFrames( DWORD channel )
{
	return m_LastKey[ channel ];
}
