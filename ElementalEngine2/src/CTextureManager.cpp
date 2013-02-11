///============================================================================
/// \file	CTextureManager.cpp
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

#include "Stdafx.h"
#include "CTextureManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const UINT DDSCAPS_TEXTURE = 0x00001000l;
static const UINT DDSCAPS2_VOLUME = 0x00200000l;
static const UINT DDSCAPS2_CUBEMAP = 0x00000200l;

#if !defined(MAKEFOURCC)
#   define MAKEFOURCC(ch0, ch1, ch2, ch3) \
        ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |       \
        ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif

REGISTER_COMPONENT_SINGLETON( CTextureManager );

//REGISTER_MESSAGE_HANDLER(IsTextureLoaded, OnIsTextureLoaded, CTextureManager);

REGISTER_MESSAGE_HANDLER(AddTexture, OnAddTexture, CTextureManager);
REGISTER_MESSAGE_HANDLER(SetLoadParameters, OnSetLoadParameters, CTextureManager);
REGISTER_MESSAGE_HANDLER(GetLoadParameters, OnGetLoadParameters, CTextureManager);
REGISTER_MESSAGE_HANDLER(RemoveTexture, OnRemoveTexture, CTextureManager);
REGISTER_MESSAGE_HANDLER(UnloadObject, OnUnloadObject, CTextureManager);
REGISTER_MESSAGE_HANDLER(GetTexture, OnGetTexture, CTextureManager);
REGISTER_MESSAGE_HANDLER(CreateTexture, OnCreateTexture, CTextureManager);
REGISTER_MESSAGE_HANDLER(CreateCubeTexture, OnCreateCubeTexture, CTextureManager );
REGISTER_MESSAGE_HANDLER_PRIORITY( DestroyRenderer, OnDestroyRenderer, CTextureManager, 0x70000000 );
/// \brief The Default Constructor for CTextureManager
/// \return void
CTextureManager::CTextureManager() : 
	CManager(_T("CTextureManager")), m_currentTexMemArea(TEX_MEM_VIDEO)
{
}

/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CTextureManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CTextureManager );
}

/// \brief The Default Destructor for CTextureManager
/// \return void
CTextureManager::~CTextureManager( )
{
	DeInit();
}

/// \brief Checks to see if the texture exists, if not, will do what AddTexture does, otherwise
/// returns the texture object
DWORD CTextureManager::OnGetTexture( DWORD size, void *params )
{		
	//get the param structure
	TEXTUREOBJECTPARAMS *texObjParams;
	TEXTURENAMEMAP::iterator cur;
	
	texObjParams = (TEXTUREOBJECTPARAMS *)params;
	VERIFY_MESSAGE_SIZE(sizeof(TEXTUREOBJECTPARAMS), size);
	//find the name mapping
	//look for it
	if( texObjParams->Name != NULL )
	{
		texObjParams->TextureObjectInterface = GetTexture( texObjParams->Name );
	}
	else
	{
		texObjParams->TextureObjectInterface = NULL;
	}

	return MSG_HANDLED_PROCEED;
}

/// \brief Decrements the reference count of a texture. If the texture ref count becomes 0,
/// the texture is permanently removed. Returns the TextureObjectInterface in the 
/// TEXTUREOBJECTPARAMS struct. A texture can be removed by using the TextureObjectInterface
/// or the stdstring Name
DWORD CTextureManager::OnRemoveTexture(DWORD size, void *params)
{
	//get the param structure
	TEXTUREOBJECTPARAMS *texObjParams;
	IBaseTextureObject * currentTexture = NULL;
	TEXTURENAMEMAP::iterator cur;
	
	texObjParams = (TEXTUREOBJECTPARAMS *)params;
	VERIFY_MESSAGE_SIZE(sizeof(TEXTUREOBJECTPARAMS), size);
	//check to see if removing name or pointer
	if( texObjParams->TextureObjectInterface != NULL )
	{
		//removing by pointer
		currentTexture = texObjParams->TextureObjectInterface;		
	}    
	else if( texObjParams->Name != NULL )
	{
		// removing by original name
		TEXTURENAMEMAP::iterator itr = m_TextureNameMap.find( texObjParams->Name->GetUniqueID() );
		if (itr == m_TextureNameMap.end())
		{
			// not found, try using safe name
			StdString szSafeName(texObjParams->Name->GetString());
			szSafeName.MakeSafeFileName();
			CHashString hsSafeName(szSafeName);
			itr = m_TextureNameMap.find( hsSafeName.GetUniqueID() );
		}

		if (itr != m_TextureNameMap.end())
			currentTexture = itr->second;

		// if the names do not match, we are reference the data
		// but with a different filename
		if (currentTexture && currentTexture->GetTextureName()->GetUniqueID() != 
			itr->first)
		{
			if (currentTexture->DecrementRefCount() == 0 )
			{
				// erase this reference from the map
				m_TextureNameMap.erase( itr->first );
				DeleteTextureObject( currentTexture );
			}
			return MSG_HANDLED_PROCEED;
		}
	}

	//deletion process
	if( currentTexture )
	{
		if (currentTexture->DecrementRefCount() == 0 )
		{
			DeleteTextureObject( currentTexture );
		}
		return MSG_HANDLED_PROCEED;
	}

	return MSG_NOT_HANDLED;
}

DWORD CTextureManager::OnUnloadObject(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(UNLOADOBJECTPARAMS), size);
	UNLOADOBJECTPARAMS *uop = (UNLOADOBJECTPARAMS*)params;
	IBaseTextureObject *pTexture = GetTexture( uop->name );
	if (!pTexture)
		return MSG_ERROR;

	pTexture->DeInit();

	return MSG_HANDLED_STOP;
}

DWORD CTextureManager::OnSetLoadParameters( DWORD size, void *params)
{
	LOADPARAMETERS * lp = (LOADPARAMETERS*)params;
	m_currentTexMemArea = lp->texMemArea;
	return MSG_HANDLED_PROCEED;
}

DWORD CTextureManager::OnGetLoadParameters( DWORD size, void *params)
{
	LOADPARAMETERS * lp = (LOADPARAMETERS*)params;
	lp->texMemArea = m_currentTexMemArea;
	return MSG_HANDLED_PROCEED;
}

/// \brief Adds a texture to the manager and returns a pointer to the object. If object already
/// exists, increments its internal reference count and returns the object. Takes
/// a structure of type TEXTUREOBJECTPARAMS
DWORD CTextureManager::OnAddTexture(DWORD size, void *params)
{
	//maybe change this to a loader? But texture objects should not exist in the hierarchy.
	//get the param structure
	TEXTUREOBJECTPARAMS *texObjParams;
	IBaseTextureObject * currentTexture = NULL;
	TEXTURENAMEMAP::iterator cur;

	texObjParams = (TEXTUREOBJECTPARAMS *)params;
	VERIFY_MESSAGE_SIZE(sizeof(TEXTUREOBJECTPARAMS), size);
	//look for it
	if( texObjParams->Name != NULL )
	{
		// convert to safe file name
		StdString szFileName = texObjParams->Name->GetString();
		szFileName.MakeSafeFileName();
		CHashString hashFile(szFileName);
		//do we have it in the mapping already?
		TEXTURENAMEMAP::iterator itr = m_TextureNameMap.find( hashFile.GetUniqueID() );
		if (itr != m_TextureNameMap.end())
			currentTexture = itr->second;

		if( currentTexture != NULL )
		{
			currentTexture->IncrementRefCount();
		}
		else
		{
			if (texObjParams->bLoad)
			{
				if (m_currentTexMemArea == TEX_MEM_VIDEO)
				{
					//new texture
					//check extension
					if( _tcsstr( hashFile.GetString(), ".hdr" )!= NULL ) 
					{
						static CHashString texName(_T("CDX9TextureObject"));
						currentTexture = (ITextureObject*)CreateTextureObject( &hashFile,  &texName);
						assert( currentTexture );
						currentTexture->SetTextureName( &hashFile );
						currentTexture->LoadFromFile(hashFile.GetString() );
					}else if( _tcsstr( hashFile.GetString(), ".ant" )!= NULL ) 
					{
						static CHashString aniTexObj(_T("CAnimatedTextureObject") );
						currentTexture = CreateTextureObject( &hashFile, &aniTexObj);
						currentTexture->LoadFromFile(hashFile.GetString() );
					}else if( _tcsstr( hashFile.GetString(), _T(".dds") ) != NULL )
					{
						//we can automatically load cubemap textures if it is a cubemap texture
						currentTexture = LoadCubeDDSTexture( &hashFile );
					}
				}

				// use the internal loader as it deals with a lot of issues
				// (ie. linear vs. tiled texture alignments/restrictions) for us.
				// and gets around the bug in DevIL on small mip levels of textures
				if( !currentTexture )
				{
					static CHashString DX9TexObj(_T("CDX9TextureObject"));
					currentTexture = CreateTextureObject( &hashFile, &DX9TexObj);
					currentTexture->SetTextureName( &hashFile );
					if (!currentTexture->LoadFromFile( hashFile.GetString() ))
					{
						DeleteTextureObject( currentTexture );
						currentTexture = NULL;
					}
				}

				//try loading file by extension
				if( !currentTexture )
				{
					currentTexture = LoadTextureByExtension( &hashFile );
				}

				if( !currentTexture )
				{
					// log message about creating or allocating memory
					m_ToolBox->Log( LOGERROR, _T("Could not create texture %s\n"),
						hashFile.GetString() );
					return MSG_ERROR;
				}
			}
			else
			{
				currentTexture = texObjParams->TextureObjectInterface;
				currentTexture->IncrementRefCount();
			}
			//add to internal list
			m_TextureNameMap[hashFile.GetUniqueID()] = currentTexture;
		}		
		//set the return value
		texObjParams->TextureObjectInterface = currentTexture;
		
	}
	else //No name/filename specified, return nothing
	{
		texObjParams->TextureObjectInterface = NULL;
	}
	return MSG_HANDLED_PROCEED;
}

/// \brief creats a blannk Cube texture and returns the pointer to the object
DWORD CTextureManager::OnCreateCubeTexture( DWORD size, void * params )
{
	//get the param structure
	CREATECUBETEXTUREPARAMS *texObjParams;
	ICubeTextureObject * currentTexture;
	bool bResult;
	TEXTURENAMEMAP::iterator cur;
	
	texObjParams = (CREATECUBETEXTUREPARAMS *)params;
	VERIFY_MESSAGE_SIZE(sizeof(CREATECUBETEXTUREPARAMS), size);
	texObjParams->CubeTextureObjectInterface = NULL;
	//look for it
	if( texObjParams->Name != NULL )
	{		
		static CHashString DXCubeTexObj(_T("CDX9CubeTextureObject")); 
		currentTexture = dynamic_cast< ICubeTextureObject* >(CreateTextureObject( texObjParams->Name, &DXCubeTexObj));
		assert( currentTexture );
		currentTexture->SetTextureName( texObjParams->Name );
		bResult = currentTexture->InitializeCubemap( texObjParams->size, texObjParams->bitDepth );
		//add to internal list
		if( bResult )
		{
			currentTexture->SetTextureName( texObjParams->Name );
			m_TextureNameMap[texObjParams->Name->GetUniqueID()] = currentTexture;
			//set the return value
			texObjParams->CubeTextureObjectInterface = currentTexture;		
		}else
		{
			DeleteTextureObject( currentTexture );
		}
	}
	return MSG_HANDLED_PROCEED;
}

/// \brief creates a blank texture  and returns a pointer to the object.
DWORD CTextureManager::OnCreateTexture(DWORD size, void *params)
{
	//get the param structure
	CREATETEXTUREPARAMS *texObjParams;
	ITextureObject * currentTexture;
	bool bResult;
	TEXTURENAMEMAP::iterator cur;
	
	texObjParams = (CREATETEXTUREPARAMS *)params;
	VERIFY_MESSAGE_SIZE(sizeof(CREATETEXTUREPARAMS), size);
	texObjParams->TextureObjectInterface = NULL;
	//look for it
	if( texObjParams->Name != NULL )
	{
		currentTexture = dynamic_cast< ITextureObject* >(GetTexture( texObjParams->Name ));
		if (!currentTexture)
		{
			//new texture
			CHashString hszComponentType("CDX9TextureObject");
			currentTexture = dynamic_cast< ITextureObject* >(CreateTextureObject( texObjParams->Name, &hszComponentType ));
			// add to internal list
			m_TextureNameMap[texObjParams->Name->GetUniqueID()] = currentTexture;
		}

		if( texObjParams->bRenderTargetTexture == RENDER_TARGET_NONE )
		{
			bResult = currentTexture->MakeBlankTexture( texObjParams->sizeX, texObjParams->sizeY, 
				texObjParams->bitDepth, texObjParams->Format, texObjParams->numMips );
		}		
		else 
		{			
			bResult = currentTexture->MakeRenderTarget( texObjParams->sizeX, texObjParams->sizeY, 
				texObjParams->bitDepth, texObjParams->bRenderTargetTexture, texObjParams->bAutoGenMipMaps );
		}

		if (bResult)
		{
			currentTexture->SetTextureName( texObjParams->Name );
			// success, set the return value
			texObjParams->TextureObjectInterface = currentTexture;		
		}
		else
		{
			m_TextureNameMap.erase( texObjParams->Name->GetUniqueID() );
			// creation failure
			DeleteTextureObject( currentTexture );
		}
	}
	return MSG_HANDLED_PROCEED;
}

IBaseTextureObject * CTextureManager::GetTexture(IHashString *name )
{
	if( name != NULL )
	{
		StdString szSafeName = name->GetString();
		szSafeName.MakeSafeFileName();
		CHashString hsSafeName(szSafeName);
		//do we have it in the mapping already?
		TEXTURENAMEMAP::iterator cur = m_TextureNameMap.find( hsSafeName.GetUniqueID() );
		if( cur != m_TextureNameMap.end() )
		{
			//found one already loaded
			return (cur->second);
		}
	}
	return NULL;
}

/// \brief	DeInitializes the CTextureManager
/// \return	bool: Success
bool CTextureManager::DeInit( void )
{
	m_TextureNameMap.clear();
	return CManager::DeInit();
}

bool CTextureManager::DeleteObject( IObject* Object )
{
#ifdef _DEBUG
	IBaseTextureObject *pTextureObject = static_cast<IBaseTextureObject*>(Object);
	//	m_TextureNameMap.erase( pTextureObject->GetTextureName()->GetUniqueID() );
	// we cannot call GetTextureName() at this moment
	// check if object already deleted from m_TextureNameMap already
	TEXTURENAMEMAP::iterator it = m_TextureNameMap.begin();
	for( ; it != m_TextureNameMap.end(); ++it )
	{
		if( it->second == pTextureObject )
		{
			assert( !"Texture should be removed from m_TextureNameMap by texture name" );
			m_TextureNameMap.erase( it );
			break;
		}
	}
#endif

	return CManager::DeleteObject( Object );
}

IBaseTextureObject* CTextureManager::CreateTextureObject( IHashString *pTextureName, IHashString *pComponentType )
{
	IBaseTextureObject *pTextureObject = dynamic_cast<IBaseTextureObject*>(m_ToolBox->CreateComponent( 
		pComponentType, 2, pTextureName, NULL ) );

	OFACTORYADDPARAMS ofap;
	ofap.component = pTextureObject;
	ofap.name = pTextureObject->GetName();
	static DWORD msgHash_AddObjectToFactory = CHashString(_T("AddObjectToFactory")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddObjectToFactory, sizeof(OFACTORYADDPARAMS), &ofap);

	return pTextureObject;
}

void CTextureManager::DeleteTextureObject( IBaseTextureObject *pTextureObject )
{
	m_TextureNameMap.erase( pTextureObject->GetTextureName()->GetUniqueID() );
	DELETEOBJECTPARAMS dop;
	dop.name = pTextureObject->GetName();
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop );
}

IBaseTextureObject * CTextureManager::LoadTextureByExtension( IHashString * name )
{
	TEXTUREOBJECTPARAMS gt;
	gt.bLoad = false;
	gt.Name = name;
	LOADFILEEXTPARAMS lfep;
	lfep.fileName = const_cast<TCHAR*>((const TCHAR*)name->GetString());
	lfep.bInternalLoad = true;
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);				
	static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(gt), &gt );
	return gt.TextureObjectInterface;
}

IBaseTextureObject * CTextureManager::LoadCubeDDSTexture( IHashString* name )
{
	//Open the data stream
	DWORD result;

	IArchive *FileArchive;
	CHashString streamType(_T("File"));
	CREATEARCHIVE ca;

	ca.mode = STREAM_MODE_READ | STREAM_MODE_BINARY;
	ca.streamData = (void*)name->GetString();
	ca.streamType = &streamType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
	
	IBaseTextureObject *pTexture = NULL;
	if( result == MSG_HANDLED )
	{
		FileArchive = dynamic_cast<IArchive *>(ca.archive);
		//Check DDS
		UINT fourcc;
		FileArchive->Read( fourcc );
		if( EE_ENDIANSWAP32(fourcc) != MAKEFOURCC( 'D', 'D', 'S', ' ' ) )
		{
			return NULL;
		}
		DDSHeader header;
		char * pheader = (char*)&header;
		//DDS, start loading header
		for( int i = 0; i < sizeof( header ); i++ )
		{
			FileArchive->Read( (*pheader) );
			pheader++;
		}
		//close the file, no longer needed
		FileArchive->Close();
		if( IsDDSCubeMap( header ) )
		{
			static CHashString DX9CubeTexObj(_T("CDX9CubeTextureObject")); 
			pTexture = CreateTextureObject( name, &DX9CubeTexObj);
			pTexture->LoadFromFile( name->GetString() );
		}
	}
	return pTexture;
}

bool CTextureManager::IsDDSCubeMap( const DDSHeader & header )
{
	if( EE_ENDIANSWAP32(header.caps.caps2) & DDSCAPS2_CUBEMAP )
	{
		return true;
	}
	else
		return false;
}

DWORD CTextureManager::OnDestroyRenderer(DWORD size, void *param)
{
	DeInit();

	return MSG_HANDLED_PROCEED;
}