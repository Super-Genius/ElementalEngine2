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


REGISTER_COMPONENT(HierarchicalModelType);
REGISTER_MESSAGE_HANDLER( GetHierarchyTemplate, OnGetHierarchyTemplate, HierarchicalModelType );
REGISTER_MESSAGE_HANDLER( GetChildID, OnGetChildID, HierarchicalModelType );

bool					HierarchicalModelType::m_bClassInitialized = false;
CHANNELDESCRIPTORLIST	HierarchicalModelType::m_HGeomFormat;
IToolBox *				HierarchicalModelType::m_ToolBox = NULL;

void HierarchicalModelType::CheckInitializedStaticVariables()
{
	if( !m_bClassInitialized )
	{
		m_bClassInitialized = true;
		m_HGeomFormat.push_back( MESH_STREAM_VERTEX_POSITION3 );
		m_HGeomFormat.push_back( MESH_STREAM_VERTEX_NORMAL );
		m_HGeomFormat.push_back( MESH_STREAM_VERTEX_TEXTURECOORD1 );
		m_HGeomFormat.push_back( MESH_STREAM_VERTEX_TANGENT4 );	
		
		m_ToolBox = EngineGetToolBox();
	}
}

HierarchicalModelType::HierarchicalModelType(IHashString *parentName, IHashString *name, bool bAddToHier ) : 
	OBJECTTEMPLATE(CHModelManager, HierarchicalModelType, IObject, parentName, name)
{
	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}	
}


HierarchicalModelType::~HierarchicalModelType()
{
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
	DeInit();
}

void HierarchicalModelType::Init( void )
{
	//Nothing to do
}


void HierarchicalModelType::DeInit()
{
	CleanNodeMapList();
	CObjectTemplate<IObject>::DeInit();
}

bool HierarchicalModelType::Update()
{	
	return true;
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *HierarchicalModelType::Create(int nArgs, va_list argptr)
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
			self = new HierarchicalModelType(parentName, name, false);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new HierarchicalModelType(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

HierarchicalNode * HierarchicalModelType::AddNode( StdString name, int id, int parentid )
{
	//find in mapping
	HNODEMAP::iterator iter = m_NodeMap.find( id );
	HNODEMAP::iterator parentiter = m_NodeMap.find( parentid );
	if( iter != m_NodeMap.end() //child cannot exist
		) 
	{
		//found previous error
		m_ToolBox->Log(LOGINFORMATION, _T("Hierarchical Node Duplication Error\n"));
		return NULL;
	}
	//create new instance
	HierarchicalNode * hnode = new HierarchicalNode( );
	hnode->SetName( name );
	//insert in mapping
	m_NodeMap[ id ] = hnode;
	CHashString namemap( (const TCHAR*)name );
	m_NodeNameIDMap[ namemap.GetUniqueID() ] = hnode;
	//insert into array list for easy iteration
	UINT curindex = m_NodeMapList.size();
	m_NodeMapList.push_back( hnode );
	hnode->m_Index = curindex;
	if( parentiter != m_NodeMap.end() )
	{
		//grab parent - BUG: [] is the insert operator and not a search function
		//HierarchicalNode * pnode = m_NodeMap[ parentid ];	
		
		// corrected search
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
		////////////////////////////////////////////////////////
	}
	return GetNode(curindex);
}
	
HierarchicalNode * HierarchicalModelType::GetNode( UINT id )
{
	if( id >= 0 &&
		id < m_NodeMapList.size() )
	{
		return m_NodeMapList[ id ];
	}
	return NULL;
}

void HierarchicalModelType::CleanNodeMapList()
{
	for (unsigned int id=0; id < m_NodeMapList.size(); id++)
	{
		SAFE_DELETE( m_NodeMapList[ id ] );
	}
	m_NodeMapList.clear();
}

void HierarchicalModelType::Serialize(IArchive &ar)
{
	if (ar.IsReading())
	{
		UINT numgeom = 0;
		StdString name;
		int id;
		int parentid;
		StdString type;
		StdString filename;

		ar.Read( numgeom );
		for( unsigned int i = 0; i < numgeom; i++ )
		{
			ar.Read( name );
			ar.Read( id );
			ar.Read( parentid );
			ar.Read( type );
			ar.Read( filename );
			
			HierarchicalNode * node = AddNode( name, id, parentid );
			if( node )
			{
				node->SetObjectType( type );
				//load by type
				if( type == NodeType_Cal3D )//skeletal cal3d
				{
					LoadControl( ar, node );
					LoadCalModel( ar, node, filename );
				}
				else if( type == NodeType_Cloth )
				{
					LoadControl( ar, node );
					LoadClothModel( ar, node, filename );
				}
				
				else if( type == "Geometry" )
				{
					IGeometry * geom = NULL;
					static DWORD msgHash_CreateGeometry = CHashString(_T("CreateGeometry")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_CreateGeometry, sizeof( IGeometry** ), &geom );					
					if( geom )
					{
						LoadGeometry( ar, geom );
						SetupTextures( ar, geom );
						node->SetObject( geom );
					}
				}

				else
				{							
					LoadControl( ar, node );
				} //type check
			}
			else 
			{
				//error, danger will robinson, a linking of a node is incorrect, further file reading will be bad
				m_ToolBox->Log( LOGERROR, _T("Error reading HRC file: %s\n"), GetName()->GetString() );
				return;
			}
		}
	}
	else
	{
		
	}
}
void BuildMat2( Matrix4x4 &m, const Quaternion quat, const Vec3 scale, const Vec3 pos )
{
	quat.GetMatrix(m);
	m.SetTranslation( pos );
	m.m[0] *= scale.x;
	m.m[1] *= scale.x;
	m.m[2] *= scale.x;
		
	m.m[4] *= scale.y;
	m.m[5] *= scale.y;
	m.m[6] *= scale.y;
		
	m.m[8] *= scale.z;
	m.m[9] *= scale.z;
	m.m[10] *= scale.z;
}

bool HierarchicalModelType::LoadControl(  IArchive &ar, HierarchicalNode * node )
{
	Quaternion quat;
	Vec3 translation, scale;
	
	ar.Read( quat );
	ar.Read( scale );
	ar.Read( translation );

	node->SetInitialPos( quat, translation, scale);

	return true;
}

bool HierarchicalModelType::LoadCalModel( IArchive &ar, HierarchicalNode * node, StdString filename  )
{
	if( !ar.IsReading() )return false;

	char drive[ MAX_PATH ];
	char dir[ MAX_PATH ];
	char fname[ MAX_PATH ];
	char ext[ MAX_PATH ];
	
	memset( drive, 0, sizeof( drive ) );
	memset( dir, 0, sizeof( dir ) );
	memset( fname, 0, sizeof( fname ) );
	memset( ext, 0, sizeof( ext ) );

	StdString fullpath = "";
	fullpath += filename;
	IHashString * curpath =	GetName();
	if( curpath )
	{
		_splitpath( curpath->GetString(), drive, dir, fname, ext );
		fullpath = "";
		fullpath += drive;
		fullpath += dir;
		fullpath += filename;
	}
	//node->SetCalObject( fullpath );
	node->SetFilePath( fullpath );

	// precache the cal model
	LOADFILEEXTPARAMS lfep;
	lfep.bInternalLoad = true;
	lfep.fileName = (TCHAR*) fullpath.c_str();
	static DWORD msgLoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgLoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
    
	return true;
}

bool HierarchicalModelType::LoadClothModel( IArchive &ar, HierarchicalNode * node, StdString filename  )
{
	if( !ar.IsReading() )return false;

	char drive[ MAX_PATH ];
	char dir[ MAX_PATH ];
	char fname[ MAX_PATH ];
	char ext[ MAX_PATH ];
	
	memset( drive, 0, sizeof( drive ) );
	memset( dir, 0, sizeof( dir ) );
	memset( fname, 0, sizeof( fname ) );
	memset( ext, 0, sizeof( ext ) );

	StdString fullpath = "";
	fullpath += filename;
	IHashString * curpath =	GetName();
	if( curpath )
	{
		_splitpath( curpath->GetString(), drive, dir, fname, ext );
		fullpath = "";
		fullpath += drive;
		fullpath += dir;
		fullpath += filename;
	}
	//node->SetCalObject( fullpath );
	node->SetFilePath( fullpath );

	// precache the cloth, if it has a loader
	LOADFILEEXTPARAMS lfep;
	lfep.bInternalLoad = true;
	lfep.fileName = (TCHAR*) fullpath.c_str();
	static DWORD msgLoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgLoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
    
	return true;
}

bool HierarchicalModelType::SetupTextures( IArchive &ar, IGeometry *geom )
{
	//DEPRECATED FOR NOW, GEOMETRY IS HANDLED BY CAL MODELS. IF WE EVER
	//TAKE OUT CAL MODELS, THIS FUNCTION MAY BE CALLED
	UINT numtextures;
	TEXTUREOBJECTPARAMS tobj;
	StdString	texturetag;
	StdString	texturefile;
	StdString	shader;
	StdString	effect;

	if( !ar.IsReading() ) return false;

	ar.Read( effect );
	effect.MakeSafeFileName();
	ar.Read( shader );
	shader.MakeSafeFileName();

	//load the shader
	IEffect * m_Effect = NULL;
	IShader * m_Shader = NULL;
	
	//load the shader
	LOADFILEEXTPARAMS lfep;	
	CHashString filename( effect );
	lfep.fileName = (TCHAR*)effect.c_str();
	lfep.bInternalLoad = true;
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
	if( retval == MSG_HANDLED )
	{
		CREATEEFFECTINTERFACE cef;
		cef.m_Name = &filename;
		static DWORD msgHash_GetEffectInterface = CHashString(_T("GetEffectInterface")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_GetEffectInterface, sizeof(CREATEEFFECTINTERFACE ), &cef );
		m_Effect = cef.m_EffectInterface;
		if( m_Effect )
		{
			CHashString shaderhash( shader );
			//find correct pass, should change interface here?
			//TODO: CHange to material
			/*for( unsigned int i = 0; i < m_Effect->GetNumPass(); i++ )
			{
				IHashString * curtag = m_Effect->GetShaderTag( i );
				if( curtag->GetUniqueID() == shaderhash.GetUniqueID() )
				{
					m_Shader = m_Effect->GetShaderPass( i );
					break;
				}
			}*/
		}
		else
		{
			EngineGetToolBox()->Log( LOGERROR, _T("Could not load Hierarchical Model's shader" ) );
		}
	}	
	//TODO: CHange to material
	//geom->PushShader( m_Shader );

	ar.Read( numtextures );
	for( unsigned int i = 0; i < numtextures; i++ )
	{
		//read textures
		ar.Read( texturetag );
		ar.Read( texturefile );
		texturefile.MakeSafeFileName();

		CHashString fullname( texturefile );
	
		tobj.bLoad = true;
		tobj.Name = &fullname;
		tobj.TextureObjectInterface = NULL;
		static DWORD msgHash_AddTexture = CHashString(_T("AddTexture")).GetUniqueID();
		DWORD res = m_ToolBox->SendMessage(msgHash_AddTexture, sizeof(tobj), &tobj );
		if( res == MSG_HANDLED )
		{
			// store the texture id in the user data of the map
			IBaseTextureObject * CurrentTexture = tobj.TextureObjectInterface;
			if( CurrentTexture )
			{
				//TODO: CHange to material
				//geom->SetTexture( &CHashString( texturetag ), CurrentTexture);
			}
		}
	}	
	return true;
}

bool HierarchicalModelType::LoadGeometry( IArchive &ar, IGeometry *geom )
{
	//This function reads in a geometry and allocates buffer space for it
	UINT VB_size;
	UINT IB_size;
	vector< MeshMender::Vertex > verts;
	vector< unsigned int > indices;
	vector< unsigned int > mapping;
	VERTEX_NOSKIN * pData = NULL;
	unsigned int * pIndices = NULL;

	const float minNormalCreaseCos = 0.2f;
    const float minTangentCreaseCos = 0.1f;
    const float minBinormalCreaseCos = 0.1f;
    const float weightNormalsByArea = 1.0f;

	//make sure we're reading
	if( !ar.IsReading() ) return false;

	//grab a geometry piece
	ar.Read( VB_size, _T("NumVertices") );
	ar.Read( IB_size, _T("NumIndices") );
	verts.resize( VB_size );
	indices.resize( IB_size );
	for( unsigned int i = 0; i < VB_size; i++ )
	{
		ar.Read( verts[ i ].pos.x );		
		ar.Read( verts[ i ].pos.y );
		ar.Read( verts[ i ].pos.z );
		ar.Read( verts[ i ].normal.x );		
		ar.Read( verts[ i ].normal.y );
		ar.Read( verts[ i ].normal.z );
		verts[ i ].tangent.x = 0;
		verts[ i ].tangent.y = 0;
		verts[ i ].tangent.z = 0;
		ar.Read( verts[ i ].s );		
		ar.Read( verts[ i ].t );
	}

	for( unsigned int i = 0; i < IB_size; i++ )
	{
		ar.Read( indices[ i ] );
	}
		
	MeshMender mender;
	//Use meshmender to calculate tangents etc
	mender.Mend( verts, indices, mapping, 
		minNormalCreaseCos,
		minTangentCreaseCos,
		minBinormalCreaseCos,
		weightNormalsByArea,
		MeshMender::DONT_CALCULATE_NORMALS,
		MeshMender::DONT_RESPECT_SPLITS,
		MeshMender::FIX_CYLINDRICAL );
	
	geom->ResizeVertices( verts.size(), sizeof( VERTEX_NOSKIN ), &m_HGeomFormat );
	geom->LockVertices( 0, verts.size(), (void**)&pData );		

	//copy vertices
	for( unsigned int i = 0; i <verts.size(); i++) 
	{
		pData->normal[0] = verts[ i ].normal.x;
		pData->normal[1] = verts[ i ].normal.y;
		pData->normal[2] = verts[ i ].normal.z;
		pData->pos[0] = verts[ i ].pos.x;
		pData->pos[1] = verts[ i ].pos.y;
		pData->pos[2] = verts[ i ].pos.z;
		pData->tangent[0] = verts[ i ].tangent.x;
		pData->tangent[1] = verts[ i ].tangent.y;
		pData->tangent[2] = verts[ i ].tangent.z;
		pData->tangent[3] = 1.f;
		pData->tu = verts[ i ].s;
		pData->tv = verts[ i ].t;
		pData++;
	}
	geom->UnlockVertices();

	//indices
	geom->ResizeIndices( indices.size(), sizeof( unsigned int ) );
	geom->LockIndices( 0, indices.size(), (void**)pIndices );
	for( unsigned int i = 0; i < indices.size(); i++ )
	{
		*pIndices = indices[ i ];
		pIndices++;
	}
	geom->UnlockIndices();

	return true;
}


DWORD HierarchicalModelType::OnGetHierarchyTemplate(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( sizeof( GETHIERARCHYTEMPLATEPARAM ), size );
	GETHIERARCHYTEMPLATEPARAM * p = (GETHIERARCHYTEMPLATEPARAM*)params;
	if( p )
	{
		p->list = GetHierarchyTemplate();
	}
	else
	{
		p->list = NULL;
	}
	return MSG_HANDLED_STOP;
}

HNODEVECTOR* HierarchicalModelType::GetHierarchyTemplate()
{
	return &m_NodeMapList;
}


DWORD HierarchicalModelType::OnGetChildID(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( HIERARCHICALMODELGETCHILDID ) );
	HIERARCHICALMODELGETCHILDID * p = (HIERARCHICALMODELGETCHILDID*)params;
	if( p )
	{
		p->m_RetID = GetChildID(p->m_Childname);
	}
	return MSG_HANDLED_STOP;
}

int HierarchicalModelType::GetChildID(IHashString *name)
{
	int ret = HierarchicalNode::INVALID_NODE;
	
	if( name )
	{
		// Corrected bug: this was insertion operator ('[]') and not search 
		//HierarchicalNode * node = m_NodeNameIDMap[ name->GetUniqueID() ];

		// get the iterator
		HNODEMAP::iterator hierarhycalNodeIterator;
		
		// find the parent
		hierarhycalNodeIterator = m_NodeNameIDMap.find( name->GetUniqueID() );

		// check the result
		if( hierarhycalNodeIterator != m_NodeNameIDMap.end() )
		{
			HierarchicalNode * node = hierarhycalNodeIterator->second;
			ret = node->m_Index;
		}
		else
		{
			EngineGetToolBox()->Log(LOGWARNING, _T("Failed to find valid ID for %s: invalid node.\n"), name->GetString());
		}
	}

	return ret;
}