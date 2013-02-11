///==========================================================================
/// \file	LightMapObject.hpp
/// \brief	Implementation of LightMapObject interface
/// \date	3/09/2005
/// \author Marvin Gouw
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
#include "ShaderConstants.h"
#include "IEffect.h"
#include "IGeometry.h"

#include "PerformanceProfiler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define NUM_GAUSSIAN_WEIGHTS 9
#define RADTODEG 180.f/3.141592654f

float PixelWeight[ NUM_GAUSSIAN_WEIGHTS ] = { 1.0f, 0.75537f, 0.6185f, 0.4821f, 0.3461f, 0.2262f, 0.1362f, 0.09102f, 0.0552f };
const float g_HDRPackValue = 200.f;

REGISTER_COMPONENT(LightMapObject);

REGISTER_MESSAGE_HANDLER(AddLightmappableObject, OnAddObject, LightMapObject)
REGISTER_MESSAGE_HANDLER(GenerateLightmaps, OnGenerateLightmaps, LightMapObject);
REGISTER_MESSAGE_HANDLER(SetLightmapOutputDirectory, OnSetLightmapOutputDirectory, LightMapObject);

///
/// Constructor / Destructor
///

LightMapObject::LightMapObject(IHashString *parentName, IHashString *name, bool bAddToHier) : 
	OBJECTTEMPLATE(LightMapManager, LightMapObject, IObject, parentName, name)
{
	m_LightGen = NULL;
	m_IRenderer = NULL;
	m_Texdata = NULL;
	m_TempBuffer = NULL;
	m_IndirectData = NULL;
	m_BufferSize = 0;
	m_TexSize = 0;
	m_OutputDirectory = "";
	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}
	//create the lightmap generator
	IComponent *tmpComponent;
	CHashString generatorName( _T("lightmapgenerator") );
	CHashString generatorType(_T("LightMapGenerator"));
	tmpComponent = m_ToolBox->CreateComponent(&generatorType, 2, &generatorName, name );
	m_LightGen = dynamic_cast<LightMapGenerator*>(tmpComponent);
	if( !m_LightGen  )
	{
		m_ToolBox->Log( LOGERROR, _T("Could not create lightmap generator\n"));
	}
	OFACTORYADDPARAMS ofap;
	ofap.name = &generatorName;
	ofap.component = m_LightGen;
	static DWORD msgHash_AddObjectToFactory = CHashString(_T("AddObjectToFactory")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddObjectToFactory, sizeof(OFACTORYADDPARAMS), &ofap);

	DeInit();
}

LightMapObject::~LightMapObject()
{
	DeInit();	

	CHashString generatorName( _T("lightmapgenerator") );
	DELETEOBJECTPARAMS dop;
	dop.name = &generatorName;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop);

	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *LightMapObject::Create(int nArgs, va_list argptr)
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
			self = new LightMapObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new LightMapObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void LightMapObject::Init()
{
	//look at mapping to cal object names
	//static CHashString renderType1(_T("Cal3DRenderObject"));
	SETLIGHTMAPDATAMSG ldata;
	map< DWORD, LightmapCache >::iterator iter = m_UVCache.begin();
	for( ;iter != m_UVCache.end(); ++iter )
	{
		DWORD id = iter->first;
		//send messages
		const TCHAR * t = m_ToolBox->GetHashString( id );
		CHashString hashName( t );
		LightmapCache &cache = iter->second;
		ldata.cache = &cache;
		ldata.pTexture = m_TextureObjects[ id ];
		//we really don't know the type of object this is
		static DWORD msgHash_SetLightmapData = CHashString(_T("SetLightmapData")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetLightmapData, sizeof( ldata ), &ldata, &hashName );		
	}
}

DWORD LightMapObject::OnSetLightmapOutputDirectory( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( IHashString ) );
	IHashString * hash = (IHashString*)params;
	if( hash )
	{
		m_OutputDirectory = hash->GetString();
	}
	return MSG_HANDLED_STOP;
}

DWORD LightMapObject::OnAddObject( DWORD size, void * params )
{
	static CHashString tempName(_T("none"));
	if( !m_LightGen || 
		!params)
	{
		return MSG_ERROR;
	}
	IHashString * name;
	IHashString * type;
	VERIFY_MESSAGE_SIZE( size, sizeof( ADDLIGHTMAPPABLEOBJECT) );
	ADDLIGHTMAPPABLEOBJECT * addLightmapMsg = (ADDLIGHTMAPPABLEOBJECT*)params;
	
	name = addLightmapMsg->name;
	type = addLightmapMsg->type;
	if( !name ||
		!type )
	{
		return MSG_ERROR;
	}

	// if object already added
	if (m_TextureObjects.find( name->GetUniqueID() ) != m_TextureObjects.end())
		return MSG_HANDLED_STOP;

	TCHAR buf[1024];
	if( name )
	{
		// grab the render interface
		GETRENDERERINTERFACEPARAMS renderInterface;
		DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetRendererInterface, 
							sizeof(GETRENDERERINTERFACEPARAMS), 
							&renderInterface);
		IRenderer * Renderer = renderInterface.m_RendererInterface;
		//get geom if it already exists
		DWORD fileID = 0;
		bool hasFileID = true;
		static DWORD msgHash_GetModelFileId = CHashString(_T("GetModelFileId")).GetUniqueID();
		DWORD retVal = m_ToolBox->SendMessage(msgHash_GetModelFileId, sizeof( DWORD* ), &fileID, name, type );		
		if( retVal != MSG_HANDLED )
		{
			hasFileID = false;
		}

		map< DWORD, DWORD >::iterator iter = m_GlobalMeshCache.find( fileID );
		IComponent *tmpComponent;
		tmpComponent = m_ToolBox->CreateComponent(&CHashString("MeshParameterization"), 2, &tempName, &tempName );
		MeshParameterization * MeshParam = dynamic_cast<MeshParameterization*>(tmpComponent);
		if( !MeshParam )
		{
			m_ToolBox->Log( LOGERROR, _T("Could not create Mesh parameterizatoin\n"));
			return MSG_HANDLED_STOP;
		}
		m_MeshParams[ name->GetUniqueID() ] = MeshParam;

		GETCALPARENT GetPMsg;
		static DWORD msgHash_GetParentName = CHashString(_T("GetParentName")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetParentName, sizeof( GetPMsg ), &GetPMsg, name, type );
		Matrix4x4 transform;
		transform.SetIdentity();
		static DWORD msgHash_GetLocalMatrixTransform = CHashString(_T("GetLocalMatrixTransform")).GetUniqueID();
		DWORD testLocal = m_ToolBox->SendMessage(msgHash_GetLocalMatrixTransform, sizeof( Matrix4x4 ), &transform,
				name, type );
		if( testLocal != MSG_HANDLED )
		{
			if( GetPMsg.parent )
			{
				static CHashString entType(_T("CEntity"));
				static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_GetGlobalTransform, sizeof( Matrix4x4 ), &transform, GetPMsg.parent, &entType );
				
			}
		}

		bool bCreateTexture = true;
		if( hasFileID && iter != m_GlobalMeshCache.end() )
		{
			//ok so we have a cache already
			DWORD cachefile = iter->second;
			//copy cache file
			*MeshParam = *m_MeshParams[ cachefile ];
			MeshParam->m_Transform = NULL;
			MeshParam->m_InverseTransform = NULL;
			MeshParam->SetMeshTransform( sizeof(transform), &transform );

			LOADFILEEXTPARAMS lfep;
			lfep.bInternalLoad = true;
			//Check for loading from a cached file
			_stprintf( buf, "%s_lightmap.dds", name->GetString() );
			CHashString texHash( buf );
			lfep.fileName = buf;
			static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);

			//and now grab them back
			ITextureObject * tex = NULL;
			TEXTUREOBJECTPARAMS gettex;
			gettex.Name = &texHash;
			static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(gettex), &gettex ) == MSG_HANDLED)
			{
				tex = dynamic_cast< ITextureObject*>(gettex.TextureObjectInterface);
				if( tex )
				{
					bCreateTexture = false;
					m_TextureObjects[ name->GetUniqueID() ] = tex;
				}
			}
		}else
		{
			MeshParam->m_Transform = NULL;
			MeshParam->m_InverseTransform = NULL;
			MeshParam->SetMeshTransform( sizeof(transform), &transform );
			LOADFILEEXTPARAMS lfep;
			lfep.bInternalLoad = true;
			//Check for loading from a cached file
			_stprintf( buf, "%s_lightmap.dds", name->GetString() );
			CHashString texHash( buf );
			lfep.fileName = buf;
			static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
			_stprintf( buf, "%s.lmp", name->GetString() );
			CHashString cacheHash( buf );
			lfep.fileName = buf;
			m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);

			//and now grab them back
			ITextureObject * tex = NULL;
			TEXTUREOBJECTPARAMS gettex;
			gettex.Name = &texHash;
			static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(gettex), &gettex ) == MSG_HANDLED)
			{
				tex = dynamic_cast< ITextureObject*>(gettex.TextureObjectInterface);
				if( tex )
				{
					bCreateTexture = false;
					m_TextureObjects[ name->GetUniqueID() ] = tex;
				}
			}
			LightmapCache &cache = m_UVCache[ name->GetUniqueID() ];
			cache.Clean();
			cache.m_Hash = cacheHash.GetUniqueID();
            //Need geometry still to generate lightmaps			
			GETRENDERBUFFERALLOCATIONMSG msg;
			static DWORD msgHash_GetGeometryAllocations = CHashString(_T("GetGeometryAllocations")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetGeometryAllocations, sizeof( msg ), &msg, name, type );
			if( msg.m_GeometryAllocationsOut.size() <= 0)
			{
				m_ToolBox->Log( LOGERROR, _T("No geometry allocations for thislightmap\n"));
				return MSG_HANDLED_STOP;
			}
			MeshParam->SetNoClone();
			for( int i = 0; i < (int)msg.m_GeometryAllocationsOut.size(); i++ )
			{
				if( i == 0 )
				{
					MeshParam->AddVertexSoup( &msg.m_GeometryAllocationsOut[ i ]);
				}else
				{
					MeshParam->AddVertexSoup( &msg.m_GeometryAllocationsOut[ i ], true );
				}
			}
			//grab cache
			static DWORD msgHash_GetLightmapCache = CHashString(_T("GetLightmapCache")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_GetLightmapCache, sizeof(cache), &cache ) != MSG_HANDLED)
			{
				//regenerate:
				GENERATEATLASMESSAGE atlasCreateMsg;
				//Use incoming tex coords or generate a new atlas?
				atlasCreateMsg.useIncomingTexCoords = addLightmapMsg->bUseIncomingTexCoord;
				MeshParam->OnGenerateAtlas( sizeof( atlasCreateMsg ),&atlasCreateMsg );
			}else
			{
				if( !cache.IsBadCache() )
				{
					MeshParam->GenerateBounds();
					MeshParam->SetCache( sizeof( cache), &cache );
					MeshParam->GenerateTriangleTexelData();
				}
				else
				{
					m_ToolBox->Log( LOGERROR, _T("Bad Lightmap Cache for object: %s\n"), name->GetString() );
					delete MeshParam;
					map< DWORD, MeshParameterization * >::iterator iter = m_MeshParams.find( name->GetUniqueID() );
					if( iter != m_MeshParams.end() )
					{
						m_MeshParams.erase( iter );
					}
					return MSG_ERROR;
				}
			}
			m_GlobalMeshCache[ fileID ] = name->GetUniqueID();
		}

		_stprintf( buf, _T("%s_lightmap.dds"), name->GetString() );
		CHashString lightMapName(buf);
		//create a texture object for the lightmap

		if( bCreateTexture )
		{
			CREATETEXTUREPARAMS createtextureparams;
			CHashString DXTFormat(_T("DXT5"));
			createtextureparams.bitDepth = 32;
			createtextureparams.Name = &lightMapName;
			createtextureparams.sizeX = MeshParam->GetTextureSize();
			createtextureparams.sizeY = MeshParam->GetTextureSize();
			createtextureparams.Format = &DXTFormat;
			static DWORD msgHash_CreateTexture = CHashString(_T("CreateTexture")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_CreateTexture, sizeof(createtextureparams), &createtextureparams );
			ITextureObject * texture = dynamic_cast<ITextureObject*>( createtextureparams.TextureObjectInterface );
			m_TextureObjects[ name->GetUniqueID() ] = texture;
			if( !texture )
			{
				m_ToolBox->Log( LOGWARNING, _T("Could not create DXT5 Lightmap texture\n") );
			}
		}
		if ( m_TextureObjects[ name->GetUniqueID() ] )
		{
			ITextureObject * texture = m_TextureObjects[ name->GetUniqueID() ];
			MESHTEXTUREPAIRMSG meshTextureMsg;
			meshTextureMsg.mesh = MeshParam;
			meshTextureMsg.texture = texture;
			m_LightGen->OnAddMesh( sizeof( meshTextureMsg ), &meshTextureMsg );
		}
		LightmapCache cache;
		SETLIGHTMAPDATAMSG cacheMsg;
		MeshParam->GetCache( sizeof( cacheMsg ), &cacheMsg );
		if( !cacheMsg.cache )
		{
			return MSG_ERROR;
		}
		LightmapCache &srcCache = *cacheMsg.cache;
		srcCache.CloneTo( cache );
		_stprintf( buf, _T("%s.lmp"), name->GetString() );
		CHashString cacheName( buf );
		cache.m_Hash = cacheName.GetUniqueID();
		m_UVCache[ name->GetUniqueID() ] = cache;
		static DWORD msgHash_RegisterLightmapCache = CHashString(_T("RegisterLightmapCache")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RegisterLightmapCache, sizeof( cache ), &(m_UVCache[ name->GetUniqueID() ]) );

		
		SETLIGHTMAPDATAMSG ldata;
		ldata.cache = &srcCache;
		ldata.pTexture = m_TextureObjects[ name->GetUniqueID() ];
		static DWORD msgHash_SetLightmapData = CHashString(_T("SetLightmapData")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetLightmapData, sizeof( ldata ), &ldata, name, type );
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

/// \brief Update the object
/// \return true or false
bool LightMapObject::Update()
{
	
	return true;
}

void LightMapObject::DeInit()
{
	static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
	static DWORD msgHash_UnRegisterLightmapCache = CHashString(_T("UnRegisterLightmapCache")).GetUniqueID();

	map< DWORD, LightmapCache >::iterator iter = m_UVCache.begin();
	for( ;iter != m_UVCache.end(); ++iter )
	{
		DWORD id = iter->first;
		// Delete The Lightmap Texture
		TEXTUREOBJECTPARAMS tobj;
		tobj.bLoad = false;
		tobj.Name = NULL;
		tobj.TextureObjectInterface = m_TextureObjects[ id ];
		m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );

		// Delete The LMP Object
		LightmapCache &cache = iter->second;
		m_ToolBox->SendMessage(msgHash_UnRegisterLightmapCache, sizeof(cache), &cache );
	}

	m_TextureObjects.clear();
	m_IRenderer = NULL;
}

void CleanFileName( StdString &clean )
{
	//strip any preceding slashes
	const TCHAR * tClean = (const TCHAR*)clean;
	TCHAR buffer[1024];
	_tcscpy( buffer, tClean );
	TCHAR * pCleaned = &buffer[0];
	if( buffer[0] == '\\' ||
		buffer[0] == '/' )
	{
		pCleaned = &buffer[1];
	}
	clean = "";
	clean += pCleaned;
}
/// \brief either reads or writes the entity information to a file
/// Currently Loads up the entity completely if reading(inside vision too)
void LightMapObject::Serialize(IArchive &ar)
{
	if (ar.IsReading())
	{
		// Get the data's version number
		CHashString hszVersion = _T("");

		float version;
		static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &hszVersion);
		if (retval != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGERROR, _T("Cal3D Render Object Serialize: Could not get file version!"));
			assert(0);
			return;
		}

		float atofVersion = (float)_tstof(hszVersion.GetString());
		version = atofVersion;

		int numLinks;
		ar.Read( numLinks, _T("NumLinks") );
		char buf[ 1024 ];
		StdString TextureName;
		StdString CacheName;
		StdString LinkName;
		LOADFILEEXTPARAMS lfep;
		lfep.bInternalLoad = true;
		for( int i = 0; i < numLinks; i++ )
		{			
			_stprintf( buf, "Link_%d", i );
			ar.Read( LinkName, buf );
			_stprintf( buf, "TextureLink_%d", i );
			ar.Read( TextureName, buf );
			CleanFileName( TextureName );
			_stprintf( buf, "LCache_%d", i );
			ar.Read( CacheName, buf );
			CleanFileName( CacheName );
			//now load them
			_tcscpy( buf, (const TCHAR*)TextureName );
			lfep.fileName = buf;
			static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
			
			_tcscpy( buf, (const TCHAR*)CacheName );
			lfep.fileName = buf;
			m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);

			//get link hash
			CHashString link( (const TCHAR*)LinkName );
			CHashString texHash( (const TCHAR*) TextureName );
			CHashString cacheHash( (const TCHAR*)CacheName );
			DWORD linkhash = link.GetUniqueID();
			//and now grab them back
			ITextureObject * tex = NULL;
			TEXTUREOBJECTPARAMS gettex;
			gettex.Name = &texHash;
			static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(gettex), &gettex ) == MSG_HANDLED)
			{
				tex = dynamic_cast< ITextureObject*>(gettex.TextureObjectInterface);
			}
			m_TextureObjects[ linkhash ] = tex;
			LightmapCache &cache = m_UVCache[ linkhash ];
			cache.Clean();
			cache.m_Hash = cacheHash.GetUniqueID();
			//grab cache
			static DWORD msgHash_GetLightmapCache = CHashString(_T("GetLightmapCache")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_GetLightmapCache, sizeof(cache), &cache ) != MSG_HANDLED)
			{
				m_ToolBox->Log( LOGERROR, _T("Error linking lightmap cache file" ) );
			}
		}
	}
	else
	{
		//Writing
		int numLinks = m_TextureObjects.size();
		ar.Write( numLinks, _T("NumLinks") );
		map< DWORD, ITextureObject * >::iterator iter = m_TextureObjects.begin();
		TCHAR buf[1024];
		TCHAR originalDirectory[1024];
		TCHAR buf2[ 1024];
		int i = 0;
		StdString curlevel = m_OutputDirectory;
		GetCurrentDirectory( sizeof( originalDirectory ), originalDirectory );
		curlevel += "\\";
		const TCHAR * m_LevelName = (const TCHAR*)curlevel;
		
		sprintf( buf2, "%s\\%s", originalDirectory, m_LevelName );
		SetCurrentDirectory( buf2 );
			
		for( ; iter != m_TextureObjects.end(); iter++, i++ )
		{
			DWORD hash = iter->first;
			StdString hashName = m_ToolBox->GetHashString( hash );
			_stprintf( buf, "Link_%d", i );
			ar.Write( hashName, buf );
			//Save off texture data
			{
				ITextureObject * texture = m_TextureObjects[ hash ];
				_stprintf( buf, "TextureLink_%d", i );
				if( texture )
				{
					IHashString * textureHash = texture->GetName();
					if( textureHash )
					{
						_stprintf( buf2, "%s%s", m_LevelName,   textureHash->GetString() );
						ar.Write( buf2, buf );
						static DWORD msgHash_SetSaveAsFormat = CHashString(_T("SetSaveAsFormat")).GetUniqueID();
						CHashString format( _T("DXT5"));
						m_ToolBox->SendMessage(msgHash_SetSaveAsFormat, sizeof( IHashString), &format );
						/*Let's not actually reoutput files while serializing*/
						static DWORD msgHash_SaveFileByExtension = CHashString(_T("SaveFileByExtension")).GetUniqueID();
						m_ToolBox->SendMessage(msgHash_SaveFileByExtension, sizeof( LPTSTR), (void*)textureHash->GetString() );
						
					}else
					{
						ar.Write(  _T("INVALID"), buf );
					}
				}else
				{			
					ar.Write(  _T("INVALID"), buf );
				}
			}
			//Save off cache data
			{
				LightmapCache &cache = m_UVCache[ hash ];
				StdString cacheHash = m_ToolBox->GetHashString( cache.m_Hash );					
				_stprintf( buf, "%s.lmp", (const TCHAR*) hashName );
				/*
				Let's not actually reoutput files while serializing*/
				static DWORD msgHash_SaveFileByExtension = CHashString(_T("SaveFileByExtension")).GetUniqueID();
				DWORD retval = m_ToolBox->SendMessage(msgHash_SaveFileByExtension, sizeof( LPTSTR), buf );
				if( retval != MSG_HANDLED )
				{
					m_ToolBox->Log( LOGERROR, _T("Could not save lightmap cache file\n") );
				}
				_stprintf( buf2, "LCache_%d", i );
				_stprintf( buf, "%s%s", m_LevelName, (const TCHAR*) cacheHash );
				ar.Write( buf, buf2 );
			}
		}
		//Revert directory
		SetCurrentDirectory( originalDirectory );
	}
}

DWORD LightMapObject::OnGenerateLightmaps( DWORD size, void * params )
{
	if( !m_LightGen )
	{
		return MSG_ERROR;
	}
	GETLIGHTLIST lList;
	static DWORD msgHash_GetGlobalLightArray = CHashString(_T("GetGlobalLightArray")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalLightArray, sizeof( GETLIGHTLIST ), &lList );
	for( int i = 0; i < (int)lList.lightList.size() ; i++ )
	{
		m_LightGen->OnAddLight( sizeof( ILightObject), lList.lightList[ i ] );
	}
	m_LightGen->OnCalculateLightmaps(0,0);
	//create texture buffer
	//now go through all mesh params and save off textures
	map< DWORD, MeshParameterization * >::iterator iter = m_MeshParams.begin();
	char buf[1024];
	int count = 0;
	for( ; iter != m_MeshParams.end(); iter++, count++ )
	{
		sprintf( buf, "Generating Texture Data #%d\n", count );
		OutputDebugString( buf );
		MeshParameterization * m = iter->second;
		DWORD hash = iter->first;
		floatColor * fTexel = m->GetTextureBuffer();
		floatColor * fIndirect = m->GetIndirectLightBuffer();
		SetTextureSize( m->GetTextureSize() );
		//for( int i = 0; i < m_SquaredTexSize; i++ )
		//{
			//m_Texdata[ i ] = min( (int)(fTexel[ i ]*255.f), 255 );
		//}
		NormalizeTexture( m_Texdata, fTexel, 2 );
		
		
		//Blur texture
 		BlurTextureX( m_Texdata, 3 );
		BlurTextureY( m_Texdata, 3 );/**/

		//add indirect pass
		if( fIndirect )
		{
			NormalizeTexture( m_IndirectData, fIndirect, 6 );
			BlurTextureX( m_IndirectData, 3 );
			BlurTextureY( m_IndirectData, 3 );
			BlurTextureX( m_IndirectData, 6 );
			BlurTextureY( m_IndirectData, 6 );/**/
			AddTextureToTexture( m_Texdata, m_IndirectData );
		}

		//now write to texture
		ITextureObject * texture = m_TextureObjects[ hash ];
		if( texture )
		{
			texture->Write( m_Texdata, 0 );
		}
	}				
	SETLIGHTMAPDATAMSG ldata;
	map< DWORD, LightmapCache >::iterator iter2 = m_UVCache.begin();
	for( ;iter2 != m_UVCache.end(); ++iter2 )
	{
		DWORD id = iter2->first;
		//send messages
		const TCHAR * t = m_ToolBox->GetHashString( id );
		CHashString hashName( t );
		LightmapCache &cache = iter2->second;
		ldata.cache = &cache;
		ldata.pTexture = m_TextureObjects[ id ];
		//Actually don't know type of object, can be generic renderobject so no type sent
		static DWORD msgHash_SetLightmapData = CHashString(_T("SetLightmapData")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetLightmapData, sizeof( ldata ), &ldata, &hashName );		
	}
	return MSG_HANDLED_STOP;
}

bool LightMapObject::IsKindOf( IHashString * compType )
{	
	static CHashString  CCal3dROCOMP(_T("LightMapObject") );
	
	return (compType->GetUniqueID() == CCal3dROCOMP.GetUniqueID());
}

void LightMapObject::AddTextureToTexture( BYTECOLOR * dest, BYTECOLOR * src )
{
	//Make sure we don't overflow
	static int value[4];

	for( int i = 0; i < m_SquaredTexSize; i++ )
	{
		value[0] = (int)dest[i].r + (int)src[i].r;
		value[1] = (int)dest[i].g + (int)src[i].g;
		value[2] = (int)dest[i].b + (int)src[i].b;
		value[3] = (int)dest[i].a + (int)src[i].a;
		for( int j = 0; j < 4; j++ )
		{
			value[j] = min( value[j], (int)g_HDRPackValue );
			value[j] = max( value[j], 0 );
		}
		dest[ i ].r = (BYTE)value[0];
		dest[ i ].g = (BYTE)value[1];
		dest[ i ].b = (BYTE)value[2];
		dest[ i ].a = (BYTE)value[3];
	}
}

void LightMapObject::BlurTextureX( BYTECOLOR * buffer, int curextent )
{
	static int value[4];
	for( int i = 0; i < m_SquaredTexSize; i++ )
	{
		floatColor average;
		float numAveraged = 0.f;
		int extent = curextent;
		int extenty = i/m_TexSize;//y coord
		int extentx = i%m_TexSize;//x coord
		//figure out if we're on edge, find difference between texsize and extent
		int extentXDifference = min( abs( extentx - m_TexSize ), abs( extentx) );
		int extentYDifference = min( abs( extenty - m_TexSize ), abs( extenty) );
		int extentXMax = min( extent, extentXDifference );
		int extentYMax = min( extent, extentYDifference );
		int extentMin = min( extentXMax, extentYMax ) + 1;
		//extent = min( extent, extentMin );//
		extent = min( extent, NUM_GAUSSIAN_WEIGHTS - 1);//
		for( int b = -( extent  - 1 ) ; b < extent; b++ )
		{
			int index = i + b*m_TexSize;
			int curY = b + (i/m_TexSize);
			if( curY >= 0 &&
				curY < m_TexSize &&
				index >= 0 && 
				index < m_SquaredTexSize )
			{
				if( buffer[ index ].a >= 0 )
				{
					//add to average
					int weightIndex = abs( b );
					float contribution = PixelWeight[ weightIndex ];
					average.r += ((float)buffer[ index ].r)*contribution;
					average.g += ((float)buffer[ index ].g)*contribution;
					average.b += ((float)buffer[ index ].b)*contribution;
					average.a += ((float)buffer[ index ].a)*contribution;
					numAveraged += contribution;
				}
			}
		}
		value[0] = (int)(average.r/(float)numAveraged);
		value[1] = (int)(average.g/(float)numAveraged);
		value[2] = (int)(average.b/(float)numAveraged);        	
		value[3] = (int)(average.a/(float)numAveraged);
		for( int j=0; j < 4; j++ )
		{
			value[j] = min( value[j], (int)g_HDRPackValue );
			value[j] = max( value[j], 0 );
		}
		m_TempBuffer[ i ].r = (BYTE)value[0];
		m_TempBuffer[ i ].g = (BYTE)value[1];
		m_TempBuffer[ i ].b = (BYTE)value[2];
		m_TempBuffer[ i ].a = (BYTE)value[3];
	}
	memcpy( buffer, m_TempBuffer, m_SquaredTexSize*sizeof(BYTECOLOR) );
}

void LightMapObject::BlurTextureY( BYTECOLOR * buffer, int curextent )
{
	static int value[4];
	for( int i = 0; i < m_SquaredTexSize; i++ )
	{
		int extent = curextent;
		//we're on index i, figure out how far it is from the edge,
		//we must decrease our extent the closer it is to the edge
		int extenty = i/m_TexSize;//y coord
		int extentx = i%m_TexSize;//x coord
		//figure out if we're on edge, find difference between texsize and extent
		int extentXDifference = min( abs( extentx - m_TexSize ), abs( extentx) );
		int extentYDifference = min( abs( extenty - m_TexSize ), abs( extenty) );
		int extentXMax = min( extent, extentXDifference );
		int extentYMax = min( extent, extentYDifference );
		int extentMin = min( extentXMax, extentYMax ) + 1;
		floatColor average;
		float numAveraged = 0.f;
		//extent = min( extent, extentMin );//
		extent = min( extent, NUM_GAUSSIAN_WEIGHTS - 1 );//
		for( int b = -( extent - 1 ) ; b < extent; b++ )
		{
			int index = i + b;
			int curX = b + (i%m_TexSize);
			if( curX >= 0 &&
				curX < m_TexSize &&
				index >= 0 &&
				index < m_SquaredTexSize )
			{
				if( buffer[ index ].a >= 0 )
				{
					int weightIndex = abs( b );
					//add to average
					float contribution = PixelWeight[ weightIndex ];
					average.r += ((float)buffer[ index ].r)*contribution;
					average.g += ((float)buffer[ index ].g)*contribution;
					average.b += ((float)buffer[ index ].b)*contribution;
					average.a += ((float)buffer[ index ].a)*contribution;
					numAveraged += contribution;
				}
			}
		}
		value[0] = (int)(average.r/(float)numAveraged);
		value[1] = (int)(average.g/(float)numAveraged);
		value[2] = (int)(average.b/(float)numAveraged);        	
		value[3] = (int)(average.a/(float)numAveraged);
		for( int j=0; j < 4; j++ )
		{
			value[j] = min( value[j], (int)g_HDRPackValue );
			value[j] = max( value[j], 0 );
		}
		m_TempBuffer[ i ].r = (BYTE)value[0];
		m_TempBuffer[ i ].g = (BYTE)value[1];
		m_TempBuffer[ i ].b = (BYTE)value[2];
		m_TempBuffer[ i ].a = (BYTE)value[3];
	}
	memcpy( buffer, m_TempBuffer, m_SquaredTexSize*sizeof(BYTECOLOR) );
}

void LightMapObject::NormalizeTexture( BYTECOLOR * buffer, floatColor * fTexel, float fHalfBloomRange  )
{
	static int value[4];
	int HalfBloomRange = (int)fHalfBloomRange;
	float fDivBloom = 1 / (fHalfBloomRange*2.f);
	//convert to half power
	for( int i = 0; i < m_SquaredTexSize; i++ )
	{
		floatColor fval = fTexel[ i ];
		if( fval.a < 0.f )				
		{
			//not lit, find lit nearby
			floatColor average;
			float numAveraged = 0;
			for( int a = -HalfBloomRange; a <= HalfBloomRange; a++ )
			{
				for( int b = -HalfBloomRange; b <= HalfBloomRange; b++ )
				{
					int index = i + b*m_TexSize + a;
					int curX = a + (i%m_TexSize);
					int curY = b + (i/m_TexSize);
					if( curX >= 0 &&
						curX < m_TexSize &&
						curY >= 0 &&
						curY < m_TexSize &&
						index >= 0 && index < m_SquaredTexSize )
					{
						if( fTexel[ index ].a >= 0 )
						{
							//add to average
							float contribution = fDivBloom * ( fHalfBloomRange - fabs((float)a) + fHalfBloomRange - fabs((float)b) );
							average.a += min( fTexel[ index ].a*contribution, 1.f );
							average.r += min( fTexel[ index ].r*contribution, 1.f );
							average.g += min( fTexel[ index ].g*contribution, 1.f );
							average.b += min( fTexel[ index ].b*contribution, 1.f );
							numAveraged += contribution;
						}
					}
				}
			}
			if( numAveraged > 0 )
			{
				fval.a = (average.a/(float)numAveraged);
				fval.r = (average.r/(float)numAveraged);
				fval.g = (average.g/(float)numAveraged);        	
				fval.b = (average.b/(float)numAveraged);
			}else
			{
				
				fval.a = 0;
				fval.r = 0;
				fval.g = 0;        	
				fval.b = 0;
			}
		}
		value[0] = (int)(fval.r*g_HDRPackValue);
		value[1] = (int)(fval.g*g_HDRPackValue);
		value[2] = (int)(fval.b*g_HDRPackValue);     	
		value[3] = (int)(fval.a*g_HDRPackValue);
		for( int j=0; j < 4; j++ )
		{
			value[j] = min( value[j], (int)g_HDRPackValue );
			value[j] = max( value[j], 0 );
		}
		buffer[ i ].r = (BYTE)value[0];
		buffer[ i ].g = (BYTE)value[1];
		buffer[ i ].b = (BYTE)value[2];
		buffer[ i ].a = 255;//(BYTE)value[3];
	}		
}