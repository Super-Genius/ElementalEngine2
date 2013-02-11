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


REGISTER_COMPONENT(HierarchicalAnimation);
REGISTER_MESSAGE_HANDLER( GetHierarchyTemplate, OnGetHierarchyTemplate, HierarchicalAnimation );
REGISTER_MESSAGE_HANDLER( GetAnimationTime, OnGetAnimationTime, HierarchicalAnimation );

HierarchicalAnimation::HierarchicalAnimation(IHashString *parentName, IHashString *name, bool bAddToHier ) : 
	OBJECTTEMPLATE(CHModelManager, HierarchicalAnimation, IObject, parentName, name)
{
	// if bAddToHier is set to true for whatever reason, yes, add to hierarchy... need to make sure
	// we set it correctly if we don't want it to add
	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}
}

HierarchicalAnimation::~HierarchicalAnimation()
{
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
	DeInit();
}

void HierarchicalAnimation::Init( void )
{
	//Nothing to do
}


void HierarchicalAnimation::DeInit()
{
	CleanNodeMapList();
	CObjectTemplate<IObject>::DeInit();
}

bool HierarchicalAnimation::Update()
{	
	return true;
}

void HierarchicalAnimation::Serialize(IArchive &ar)
{
	if (ar.IsReading())
	{
		ReadAnimations( ar );
	}
	else
	{
		
	}
}
	
extern void BuildMat2( Matrix4x4 &m, const Quaternion quat, const Vec3 scale, const Vec3 pos );

HierarchicalNode * HierarchicalAnimation::GetNode( UINT id )
{
	if( id >= 0 &&
		id < m_NodeMapList.size() )
	{
		return m_NodeMapList[ id ];
	}
	return NULL;
}

void HierarchicalAnimation::CleanNodeMapList()
{
	for (unsigned int id=0; id < m_NodeMapList.size(); id++)
	{
		SAFE_DELETE( m_NodeMapList[ id ] );
	}
	m_NodeMapList.clear();
}

bool	HierarchicalAnimation::ReadAnimations( IArchive &ar )
{
	UINT numgeom = 0;
	StdString name;
	int	id;
	int parentid;
	StdString type;
	UINT curid = HierarchicalNode::INVALID_NODE;

	ar.Read( numgeom );
	ar.Read( m_fTime );
	for( unsigned int i = 0; i < numgeom; i++ )
	{
		ar.Read( name );
		ar.Read( id );
		ar.Read( parentid );
		curid = AddNode( name,id, parentid );
		if( curid == HierarchicalNode::INVALID_NODE )
		{
			return false;
		}
		HierarchicalNode * node = GetNode( curid );
		//Regular geometry mesh
		if( !node )
		{
			return false;
		}
		ReadKeyframeData( ar, node );			

	}
	m_NodeMap.clear();//don't need mapping for animations, just to link up the names
	for( unsigned int i = 0; i < m_NodeMapList.size(); i++ )
	{
		HierarchicalNode * node = m_NodeMapList[ i ];
		if( node &&
			node->IsRoot() )
		{
            m_RootNodes.push_back( node );
		}
	}
	return true;
}

void HierarchicalAnimation::ReadKeyframeData( IArchive &ar, HierarchicalNode * node )
{
	int	KeySizes[ 3 ];//for 3 seperate channels, rot, scale, trans
	ar.Read( KeySizes[ 0 ] );
	ar.Read( KeySizes[ 1 ] );
	ar.Read( KeySizes[ 2 ] );
	node->ResetKeyFrames( KeySizes[ 0 ], KeySizes[ 1 ], KeySizes[ 2 ] );
	for( int i = 0; i < KeySizes[ 0 ]; i++ )
	{		
		//read rotations as quats
		ar.Read( node->m_KeyFrames[ ROTATE ][ i ] );
		ar.Read( node->m_Rotation[ i ] );
	}
	//read scaling	
	for( int i = 0; i < KeySizes[ 1 ]; i++ )
	{		
		ar.Read( node->m_KeyFrames[ SCALE ][ i ] );
		ar.Read( node->m_Scaling[ i ] );
	}
	//read translations
	for( int i = 0; i < KeySizes[ 2 ]; i++ )
	{		
		ar.Read( node->m_KeyFrames[ POS ][ i ] );
		ar.Read( node->m_Translation[ i ] );
		//Marv taking out hack:
		//reverse X axis because of mirroring that happens when Cal exports
		//node->m_Translation[ i ].x = - node->m_Translation[ i ].x;
	}
}

UINT	HierarchicalAnimation::AddNode( StdString name, int id, int parentid )
{
	//find in mapping
	HNODEMAP::iterator iter = m_NodeMap.find( id );
	HNODEMAP::iterator parentiter = m_NodeMap.find( parentid );
	if( iter != m_NodeMap.end() //child cannot exist
		) 
	{
		//found previous error
		m_ToolBox->Log(LOGINFORMATION, _T("Hierarchical Node Duplication Error\n"));
		return HierarchicalNode::INVALID_NODE;
	}
	//create new instance
	HierarchicalNode * hnode = new HierarchicalNode( );
	hnode->SetName( name );
	//insert in mapping
	m_NodeMap[ id ] = hnode;
	//insert into array list for easy iteration
	UINT curindex = m_NodeMapList.size();
	m_NodeMapList.push_back( hnode );
	hnode->m_Index = curindex;
	if( parentiter != m_NodeMap.end() )
	{
		// get the iterator
		HNODEMAP::iterator hierarhycalNodeIterator;
		
		// find the parent
		hierarhycalNodeIterator = m_NodeMap.find( parentid );

		if ( hierarhycalNodeIterator != m_NodeMap.end() )
		{
			//grab parent
			HierarchicalNode * pnode = hierarhycalNodeIterator->second;
			
			//link to parent
			hnode->SetParent( pnode, pnode->m_Index );
		}
		else
		{
			// we should not get here... this case was not handled earlier...
			assert(0);
		}
	}
	//return arraylist index
	return curindex;
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *HierarchicalAnimation::Create(int nArgs, va_list argptr)
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
			self = new HierarchicalAnimation(parentName, name, false);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new HierarchicalAnimation(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}


DWORD HierarchicalAnimation::OnGetHierarchyTemplate(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( sizeof( GETHIERARCHYTEMPLATEPARAM ), size );
	GETHIERARCHYTEMPLATEPARAM * p = (GETHIERARCHYTEMPLATEPARAM*)params;
	if( p )
	{
		p->list = &m_NodeMapList;
	}
	else
	{
		p->list = NULL;
	}
	return MSG_HANDLED_STOP;
}


DWORD HierarchicalAnimation::OnGetAnimationTime(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( sizeof( HIERARCHICALMODELGETANIMLENGTH ), size );
	HIERARCHICALMODELGETANIMLENGTH * p = (HIERARCHICALMODELGETANIMLENGTH*)params;
	if( p )
	{
		p->length = m_fTime;
		return MSG_HANDLED_STOP;
	}
	return MSG_NOT_HANDLED;
}