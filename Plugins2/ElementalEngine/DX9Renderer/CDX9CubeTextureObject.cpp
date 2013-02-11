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

#include "Stdafx.h"
#include "CDX9CubeTextureObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CDX9CubeTextureObject);

///
/// Constructor / Destructor
///

CDX9CubeTextureObject::CDX9CubeTextureObject(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE( CTextureManager, CDX9CubeTextureObject, ICubeTextureObject, parentName, name )
{
	memset( m_pLocalBuffer, 0, sizeof(m_pLocalBuffer) );
	m_iLocalBufferSize = 0;
	m_Texture = NULL;

	DeInit();
	
	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetRendererInterface, 
						sizeof(GETRENDERERINTERFACEPARAMS), 
						&renderInterface, 
						NULL, 
						NULL);
	m_Renderer = renderInterface.m_RendererInterface;
}

CDX9CubeTextureObject::~CDX9CubeTextureObject()
{
	DeInit();
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CDX9CubeTextureObject::Create(int nArgs, va_list argptr)
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
		self = (IObject*)new CDX9CubeTextureObject(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

/// \brief Loads a texture
/// \param file = the filename
bool CDX9CubeTextureObject::LoadFromFile( const TCHAR * file )
{
	DeInit();

	StdString szFilePath(file);
	szFilePath.MakeSafeFileName();
#ifdef XBOX
	szFilePath = SetPathDrive( szFilePath, EngineGetToolBox()->GetDrive() );
#endif

	TEXTUREOBJECTPARAMS tparm;
	//Find the texture in the manager first
	IToolBox * toolbox = EngineGetToolBox();
	if(!toolbox)
	{ 
		return false;
	}
	LPDIRECT3DDEVICE9 pDevice;
	if( !m_Renderer )
	{
		return false;
	}
	pDevice = (LPDIRECT3DDEVICE9)m_Renderer->GetAPIDevice();
	if( !pDevice )
	{
		return false;
	}
	CHashString tempname( file );
	tparm.Name = &tempname;
	static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
	DWORD msg = toolbox->SendMessage(msgHash_GetTexture, sizeof( tparm ), &tparm, GetManagerName(), NULL );
	if( msg != MSG_HANDLED )
	{
		return false;
	}
	//there's already a texture with the same name. Try to cast it to same type so we can clone it
	if( tparm.TextureObjectInterface )
	{
		CDX9CubeTextureObject * ctex = dynamic_cast< CDX9CubeTextureObject * >( tparm.TextureObjectInterface);
		if( ctex )
		{
			//yep we can clone it
			m_Filename = file;
			m_Texture = (LPDIRECT3DCUBETEXTURE9) ctex->GetAPITexture();
			m_bRenderTarget = false;
			return true;
		}else
		{
			//we're using 2 different API textures, with the same name, not good idea, return false
			return false;
		}
	}

	//create new texture
	//error for non power of two textures if debug
	D3DXIMAGE_INFO srcInfo;
	LPDIRECT3DCUBETEXTURE9 temptex;
	//TODO: more control over texture creation?
	LPVOID pFileData = NULL;
	UINT iFileDataSize = 0;
	if (!LoadFileIntoBuffer( szFilePath, pFileData, iFileDataSize, true ))
	{
		m_ToolBox->Log(LOGERROR, _T("Texture Object: %s failed to load\n"), szFilePath.c_str() );
		return false;
	}
	D3DFORMAT requestedFormat = EEDX9RequestedInternalFormatFromFileInMemory( pFileData, iFileDataSize );
	HRESULT hr = D3DXCreateCubeTextureFromFileInMemoryEx( pDevice,
									pFileData, 
									iFileDataSize, 
									D3DX_DEFAULT, //size
									D3DX_DEFAULT, //number of mips
									0,	//usage - 0 unless for render targets
									requestedFormat,
									D3DPOOL_MANAGED,
									D3DX_DEFAULT,	//regular filter
									D3DX_DEFAULT,	//mip filter
									0, //color key, opaque black default
									&srcInfo,	//src info
									NULL, //palette
									&temptex );
	SAFE_DELETE_ARRAY( pFileData );
	if( hr != D3D_OK )
	{
		m_ToolBox->Log(LOGERROR, _T("Texture Object: %s failed to load\n"), szFilePath.c_str() );
		return false;
	}

	if( temptex )
	{
		m_Texture = temptex;
		m_Filename = file;
		m_bRenderTarget = false;		
		D3DSURFACE_DESC tempDesc;
		temptex->GetLevelDesc(0, &tempDesc);
		m_Size  = tempDesc.Width;
		m_ColorDepth = tempDesc.Format;
		m_iLocalBufferSize = 0;
		return true;
	}
	return false;
	
}

UINT CDX9CubeTextureObject::GetHeight()
{
	return m_Size;
}

UINT CDX9CubeTextureObject::GetWidth()
{
	return m_Size;
}

UINT CDX9CubeTextureObject::GetColorDepth()
{
	return m_ColorDepth;
}

bool CDX9CubeTextureObject::IsRenderTarget()
{
	return m_bRenderTarget;
}

void* CDX9CubeTextureObject::Read( EE_CUBEMAPFACE faceindex )
{
	if( m_Texture )
	{
		int iTextureSize = (GetColorDepth() / 8) * GetWidth() * GetHeight();
		// delete the old buffer if our current one is the wrong size, means all buffers are invalid
		if (iTextureSize != m_iLocalBufferSize)
		{
			for( int i = 0; i < NUM_CUBE_FACES; i++ )
			{
				if( m_pLocalBuffer[ i ] )
				{
					delete m_pLocalBuffer[ i ];
					m_pLocalBuffer[ i ] = NULL;
				}
			}
			m_iLocalBufferSize = 0;
		}
		// allocate a new buffer if we don't have one
		if (!m_pLocalBuffer[ faceindex ])
		{
			m_pLocalBuffer[ faceindex ] = new unsigned char[iTextureSize];
			m_iLocalBufferSize = iTextureSize;
		}
		//lock the texture and read the data
		D3DLOCKED_RECT  lockrect;
		D3DCUBEMAP_FACES face = GetD3DCubeFaceEnum( faceindex );
		HRESULT hr = m_Texture->LockRect( face, 0, &lockrect, NULL, D3DLOCK_READONLY );
		if( hr == D3D_OK )
		{
			UINT pixelsize = (GetColorDepth()/8);
			BYTE * srcbits = (BYTE*)lockrect.pBits;
			BYTE * destbits = (BYTE*)m_pLocalBuffer[ faceindex ];
			//write the texture to the buffer
			for( UINT i = 0; i < m_Size; i++ )
			{
                memcpy( destbits, srcbits, m_Size*pixelsize );
				//move by pitch
				srcbits += lockrect.Pitch;
				destbits += m_Size*pixelsize;
			}
		}
		m_Texture->UnlockRect( face, 0 );
		return m_pLocalBuffer;
	}
	return NULL;
}

D3DCUBEMAP_FACES CDX9CubeTextureObject::GetD3DCubeFaceEnum( EE_CUBEMAPFACE faceindex )
{
	//mapping is actually direct
	return (D3DCUBEMAP_FACES)faceindex;
}

bool CDX9CubeTextureObject::Write( EE_CUBEMAPFACE faceindex, void *p )
{
	//UploadCubemapFace
	if (m_Texture && p)
	{
		void *pData =p;
		//lock the texture and write the data
		D3DLOCKED_RECT  lockrect;
		D3DCUBEMAP_FACES face = GetD3DCubeFaceEnum( faceindex );
		HRESULT hr = m_Texture->LockRect( face, 0, &lockrect, NULL, 0 );
		if( hr == D3D_OK )
		{
			UINT pixelsize = (GetColorDepth()/8);
			BYTE * destbits = (BYTE*)lockrect.pBits;
			BYTE * srcbits = (BYTE*)pData;
			//write the texture to the buffer
			for( UINT i = 0; i < m_Size; i++ )
			{
                memcpy( destbits, srcbits, m_Size*pixelsize );
				//move by pitch
				destbits += lockrect.Pitch;
				srcbits += m_Size*pixelsize;
			}
		}
		m_Texture->UnlockRect( face, 0 );
		//Update Mip maps here
#ifndef XBOX
		m_Texture->GenerateMipSubLevels();
#endif
		return true;
	}
	return false;
}

void * CDX9CubeTextureObject::Lock( EE_CUBEMAPFACE faceindex )
{
	if( m_Texture )
	{
		D3DLOCKED_RECT  lockrect;
		D3DCUBEMAP_FACES face = GetD3DCubeFaceEnum( faceindex );
		HRESULT hr = m_Texture->LockRect( face, 0, &lockrect, NULL, 0 );
		if( hr == D3D_OK )
		{
			//should return pitch as well, but that's only DX? Or Use read and write
			return lockrect.pBits;
		}
	}
	return NULL;
}

void CDX9CubeTextureObject::Unlock( EE_CUBEMAPFACE faceindex )
{
	if( m_Texture )
	{
		D3DCUBEMAP_FACES face = GetD3DCubeFaceEnum( faceindex );
		m_Texture->UnlockRect( face, 0 );
	}
}

///
/// Init / Update / DeInit
///
void CDX9CubeTextureObject::Init()
{
	m_Filename = "";
	m_RefCount = 1;
	m_Texture = NULL;

	memset( m_pLocalBuffer, 0, sizeof(m_pLocalBuffer) );
	m_Size = 0;
	m_ColorDepth = 0;
	m_iLocalBufferSize = 0;
	m_bRenderTarget = false;
}

void CDX9CubeTextureObject::DeInit()
{	
	if (m_Texture)
	{
		m_Texture->Release();
		m_Texture = NULL;
	}

	//remove all the faces
	for( int i = 0; i < NUM_CUBE_FACES; i++ )
	{
		if( m_pLocalBuffer[ i ] )
		{
			delete m_pLocalBuffer[ i ];
			m_pLocalBuffer[ i ] = NULL;
		}
	}
	m_Filename = "";
	m_RefCount = 1;
	m_Size = 0;
	m_ColorDepth = 0;
	m_iLocalBufferSize = 0;
	m_bRenderTarget = false;
}

/// \brief Update the object
/// \return true or false
bool CDX9CubeTextureObject::Update()
{
	return true;
}

unsigned int CDX9CubeTextureObject::GetReferenceCount()
{
	return m_RefCount;
}

IHashString *CDX9CubeTextureObject::GetTextureName()
{
	return &m_Filename;

}

void CDX9CubeTextureObject::SetTextureName(IHashString *name)
{
	m_Filename = name;

}

bool CDX9CubeTextureObject::InitializeCubemap( const UINT squaresize, const UINT colordepth_in, bool bRenderTarget )
{
	DeInit();

	UINT colordepth = colordepth_in;
	LPDIRECT3DDEVICE9 pDevice;
	if( !m_Renderer )
	{
		return false;
	}
	pDevice = (LPDIRECT3DDEVICE9)m_Renderer->GetAPIDevice();
	if( !pDevice )
	{
		return false;
	}
	UINT size = squaresize*squaresize* ( colordepth  / 8 );
	D3DFORMAT format = D3DFMT_A8R8G8B8;
	UINT numMips = 0;
	DWORD usage = 0;
	if( bRenderTarget )
	{
		usage = D3DUSAGE_RENDERTARGET;
		numMips = 1;
	}
	format = EEDX9FormatFromColorBits( colordepth );
	//create new texture	
	LPDIRECT3DCUBETEXTURE9 temptex;
	//TODO: more control over texture creation?
	if(FAILED(pDevice->CreateCubeTexture( squaresize, //width and height
									numMips, //number of mips
									usage,	//usage - 0 unless for render targets
									format,
									D3DPOOL_MANAGED, //TODO: managed vs. Unmanaged! unmanaged you can't lock!								
									&temptex,
									NULL)))
		{
			return false;
		}
		
	if( !temptex )
	{
		return false;
	}
	
	m_Size = squaresize;
	m_ColorDepth = colordepth;
	m_iLocalBufferSize = size;
	m_Texture = temptex;
	m_bRenderTarget = bRenderTarget;
	return true;
}
	
