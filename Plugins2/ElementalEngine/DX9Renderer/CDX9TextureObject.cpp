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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CDX9TextureObject);

///
/// Constructor / Destructor
///

CDX9TextureObject::CDX9TextureObject(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE( CTextureManager, CDX9TextureObject, ITextureObject, parentName, name )
{
	m_pLocalBuffer = NULL;
	m_iLocalBufferSize = 0;
	m_Texture = NULL;
	m_RenderTargetSurface = NULL;
	m_Compressed = false;

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
	m_AutoGenMips = false;
}

CDX9TextureObject::~CDX9TextureObject()
{
	DeInit();
}

int CDX9TextureObject::GetNumLevels()
{
	ReadyData();

	if( m_Texture )
	{
		return m_Texture->GetLevelCount();
	}
	return 0;
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CDX9TextureObject::Create(int nArgs, va_list argptr)
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
		self = (IObject*)new CDX9TextureObject(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

/// \brief Loads a texture
/// \param file = the filename
bool CDX9TextureObject::LoadFromFile( const TCHAR * file )
{
	StdString szFilePath(file);
	szFilePath.MakeSafeFileName();

	DeInit();

	if (!m_Renderer)
		return false;

	LPDIRECT3DDEVICE9 pDevice = (LPDIRECT3DDEVICE9)m_Renderer->GetAPIDevice();
	if( !pDevice )
		return false;

	D3DXIMAGE_INFO srcInfo;
	LPDIRECT3DTEXTURE9 pTexture;
/*
	if( _tcsstr( file, _T(".hdr" ) )!= NULL )
	{
		bool retval = LoadHDRFile( (TCHAR*)file );
		return retval;
	}
*/
	LPVOID pFileData = NULL;
	UINT iFileDataSize = 0;
	if (!LoadFileIntoBuffer( szFilePath, pFileData, iFileDataSize, true ))
	{
		m_ToolBox->Log(LOGERROR, _T("Texture Object: %s failed to load\n"), szFilePath.c_str() );
		return false;
	}

	D3DFORMAT requestedFormat = EEDX9RequestedInternalFormatFromFileInMemory( pFileData, iFileDataSize );
	HRESULT hr = D3DXCreateTextureFromFileInMemoryEx( pDevice,
									pFileData, 
									iFileDataSize, 
									D3DX_DEFAULT, //width
									D3DX_DEFAULT, //height
									D3DX_DEFAULT, //number of mips
									0,	//usage - 0 unless for render targets
									requestedFormat,
									D3DPOOL_MANAGED,
									D3DX_DEFAULT,	//regular filter
									D3DX_DEFAULT,	//mip filter
									0, //color key, opaque black default
									&srcInfo,	//src info
									NULL, //palette
									&pTexture );
	SAFE_DELETE_ARRAY( pFileData );
	if( hr != D3D_OK )
	{
		m_ToolBox->Log(LOGERROR, _T("Texture Object: %s failed to load\n"), szFilePath.c_str() );
		return false;
	}

	D3DSURFACE_DESC textureDesc;
	pTexture->GetLevelDesc( 0, &textureDesc );
	m_Width = textureDesc.Width;
	m_Height = textureDesc.Height;
	m_Texture = pTexture;
	m_Filename = file;
	m_bRenderTarget = false;
	m_ColorDepth = EEDX9ColorBitsFromFormat( textureDesc.Format );
	m_Compressed = EEDX9IsCompressedFormat( textureDesc.Format );

	if( m_AutoGenMips )
	{
		pTexture->GenerateMipSubLevels();
	}

	return true;
}

bool CDX9TextureObject::LoadHDRFile( TCHAR * file )
{
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
	
	int iWidth;
	int iHeight;
	// Read in the HDR light probe.
	FILE* fp = fopen( file, "rb" );

	if( fp )
	{
		rgbe_header_info info;
		RGBE_ReadHeader( fp, &iWidth, &iHeight, &info );

		// We really don't need this
		float fExposure = info.exposure;
		float fGamma  = info.gamma;

		// Create a float array to read in the RGB components
		float* m_fHDRPixels = new float[3 * iWidth * iHeight];
		memset( m_fHDRPixels, 0, 3 * iWidth * iHeight * sizeof( float ) );

		RGBE_ReadPixels_RLE( fp, m_fHDRPixels, iWidth, iHeight );

		if( FAILED( D3DXCreateTexture( pDevice, iWidth, iHeight, 1, 0, 
						D3DFMT_A16B16G16R16F, D3DPOOL_MANAGED, &m_Texture ) ) )
		{
			m_ToolBox->Log( LOGERROR, _T("Could not load radience(.hdr) file\n") );
			return false;
		}

		// Convert the 32-bit floats into 16-bit floats and include the alpha component
		D3DXFLOAT16* m_fHDR = new D3DXFLOAT16[4 * iWidth * iHeight];

		int j = 0;
		for( int i = 0; i < 4 * iWidth * iHeight; i += 4 )
		{
			m_fHDR[i] = m_fHDRPixels[i - j];
			m_fHDR[i + 1] = m_fHDRPixels[i + 1 - j];
			m_fHDR[i + 2] = m_fHDRPixels[i + 2 - j];
			m_fHDR[i + 3] = 1.0f;
			j++;
		}

		// Lock the texture and copy the pixel data into it
		D3DLOCKED_RECT lr;
		if(FAILED( m_Texture->LockRect( 0, &lr, NULL, 0 ) ) )
		{
			m_ToolBox->Log( LOGERROR, _T("Could not lock radience(.hdr) file for writing\n") );
			return false;
		}

		memcpy( (D3DXFLOAT16*)lr.pBits, m_fHDR, 4 * iWidth * iHeight
				* sizeof( D3DXFLOAT16 ) );

		if(FAILED( m_Texture->UnlockRect( 0 ) ) )
		{
			m_ToolBox->Log( LOGERROR, _T("Could not unlock radience(.hdr) file for writing\n") );
			return false;
		}

		delete[] m_fHDRPixels;
		delete[] m_fHDR;

		fclose( fp );

		if( m_Texture )
		{
			m_Texture = m_Texture;
			m_Filename = file;
			m_bRenderTarget = false;		
			D3DSURFACE_DESC tempDesc;
			m_Texture->GetLevelDesc(0, &tempDesc);
			if( m_AutoGenMips )
			{
				m_Texture->GenerateMipSubLevels();
			}
			m_Width = tempDesc.Width;
			m_Height = tempDesc.Height;
			m_ColorDepth = 64;
			return true;
		}
	}
	return false;
}
bool CDX9TextureObject::MakeRenderTarget( UINT sizex, UINT sizey, UINT colordepth, DWORD type, bool bAutoGenMipMaps )
{
	DeInit();

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
	D3DFORMAT format = D3DFMT_A8R8G8B8;
	DWORD usage = D3DUSAGE_RENDERTARGET;
	UINT numMips = 1;
	if (bAutoGenMipMaps)
		numMips = 0; // generate mips
	if( type == RENDER_TARGET_DEPTH_LUMINANCE )
	{
		//TODO: make use of d3dcaps
		/*format = D3DFMT_L16;//this doesn't work on some cards@#@#@!!! including 6600. hate minimal compliance D3D cards
		format = D3DFMT_D16;
		format = D3DFMT_X1R5G5B5;//this doesn't work on some cards@#@#@!!!
		format = D3DFMT_R32F;*/
		//format = D3DFMT_G16R16F;
		format = D3DFMT_A16B16G16R16F;
		//usage = D3DUSAGE_DEPTHSTENCIL;
		usage |= D3DUSAGE_AUTOGENMIPMAP;
	}
	else if( type == RENDER_TARGET_COMPRESSED_FLOATINGPOINT )
	{
		format = D3DFMT_A16B16G16R16F;
		usage |= D3DUSAGE_AUTOGENMIPMAP;
	}
	else if( type == RENDER_TARGET_FLOATINGPOINT )
	{
		format = D3DFMT_A32B32G32R32F;
	}
	else if( type == RENDER_TARGET_DEPTH_SURFACE )
	{
		format = D3DFMT_D24X8;
		usage = D3DUSAGE_DEPTHSTENCIL;
	}
	else if( type == RENDER_TARGET_TWO_CHANNEL_FP )
	{
		format = D3DFMT_G32R32F;
	}
	else if( type == RENDER_TARGET_ONE_CHANNEL_FP )
	{
		format = D3DFMT_R32F;
		usage |= D3DUSAGE_AUTOGENMIPMAP;
	}
	else
	{		
		format = EEDX9FormatFromColorBits( colordepth );

		usage |= D3DUSAGE_AUTOGENMIPMAP;
	}
	//create new texture	
	LPDIRECT3DTEXTURE9 temptex = NULL;
	//TODO: more control over texture creation?
	if(FAILED(pDevice->CreateTexture( sizex, //width
									 sizey, //height
									numMips, //number of mips
									usage ,	//usage - 0 unless for render targets
									format,
									D3DPOOL_DEFAULT, 									
									&temptex, NULL )))
		{
//			DebugBreak();
			return false;
		}
		
	if( !temptex )
	{
		return false;
	}
	m_Texture = temptex;
	m_Height = sizey;
	m_Width = sizex;
	m_ColorDepth = colordepth;
	m_bRenderTarget = true;
	m_AutoGenMips = bAutoGenMipMaps;
	return true;
}

bool CDX9TextureObject::MakeBlankTexture( UINT sizex, UINT sizey, UINT colordepth, IHashString * hformat, UINT mips )
{
	DeInit();

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
	UINT numMips = mips;
	CDX9Renderer * crend = dynamic_cast<CDX9Renderer*>(m_Renderer);
	D3DFORMAT format = D3DFMT_A8R8G8B8;
	if( hformat )
	{
		format = EEDX9FormatFromString( hformat );
		if( format == D3DFMT_UNKNOWN )
		{
			format = D3DFMT_A8R8G8B8;
		}			
		m_Compressed = EEDX9IsCompressedFormat( format );
	}
	else
		format = EEDX9FormatFromColorBits( colordepth );

	//create new texture	
	LPDIRECT3DTEXTURE9 temptex;
	//TODO: more control over texture creation?
	if(FAILED(pDevice->CreateTexture( sizex, //width
									 sizey, //height
									numMips, //number of mips
									0,	//usage - 0 unless for render targets
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
	
	m_Texture = temptex;
	m_Height = sizey;
	m_Width = sizex;
	m_ColorDepth = colordepth;
	m_bRenderTarget = false;
	return true;
}

UINT CDX9TextureObject::GetHeight()
{
	ReadyData();

	return m_Height;
}

UINT CDX9TextureObject::GetWidth()
{
	ReadyData();

	return m_Width;
}

UINT CDX9TextureObject::GetColorDepth()
{
	ReadyData();

	return m_ColorDepth;
}

bool CDX9TextureObject::IsRenderTarget()
{
	return m_bRenderTarget;
}

void* CDX9TextureObject::Read( int level, bool bForceUpdateRead )
{
	ReadyData();

	//level currently ignored, read cannot read mipmaps
	if( m_Texture )
	{
		LPDIRECT3DDEVICE9 pDevice;
		if( !m_Renderer )
		{
			return NULL;
		}
		pDevice = (LPDIRECT3DDEVICE9)m_Renderer->GetAPIDevice();
		if( !pDevice )
		{
			return NULL;
		}

		int iTextureSize = (GetColorDepth() / 8) * GetWidth() * GetHeight();
		// delete the old buffer if our current one is the wrong size
		if (m_pLocalBuffer && (iTextureSize != m_iLocalBufferSize))
		{
			delete[] m_pLocalBuffer;
			m_pLocalBuffer = NULL;
			m_iLocalBufferSize = 0;
		}
		// allocate a new buffer if we don't have one
		if (!m_pLocalBuffer)
		{
			m_pLocalBuffer = new unsigned char[iTextureSize];
			m_iLocalBufferSize = iTextureSize;
		}
		else
		{
			// return current buffer if we aren't forcing a read
			if (!bForceUpdateRead)
				return m_pLocalBuffer;
		}
		//check what kind of a surface it is, we have to get the surface if it's a rendertarget!
		if( !m_bRenderTarget )
		{
			if( m_Compressed )
			{
				LPDIRECT3DSURFACE9 Src = NULL;
				m_Texture->GetSurfaceLevel( 0, &Src );				
				//convert format to argb first
				LPDIRECT3DSURFACE9 tempSurf = NULL;
				HRESULT hr = pDevice->CreateOffscreenPlainSurface( GetWidth(), GetHeight(),
									D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tempSurf, NULL );
				if( hr != D3D_OK ||
					tempSurf == NULL ||
					Src == NULL )
				{
					SAFE_RELEASE( Src );
					SAFE_RELEASE( tempSurf );
					return NULL;
				}

				hr = D3DXLoadSurfaceFromSurface( tempSurf, NULL, NULL, Src, NULL, NULL, D3DX_DEFAULT, 0 );			
				if( hr != D3D_OK )
				{
					return NULL;
				}

				//lock the texture and read the data
				D3DLOCKED_RECT  lockrect;
				hr = tempSurf->LockRect( &lockrect, NULL, D3DLOCK_READONLY );
				if( hr == D3D_OK )
				{
					UINT pixelsize = (GetColorDepth()/8);
					BYTE * srcbits = (BYTE*)lockrect.pBits;
					BYTE * destbits = (BYTE*)m_pLocalBuffer;
					//write the texture to the buffer
					for( UINT i = 0; i < m_Height; i++ )
					{
						memcpy( destbits, srcbits, m_Width*pixelsize );
						//move by pitch
						srcbits  +=  lockrect.Pitch;
						destbits += m_Width*pixelsize;
					}
				}
				tempSurf->UnlockRect();
				SAFE_RELEASE( tempSurf );
				SAFE_RELEASE( Src );
				return m_pLocalBuffer;
			}
			else
			{
				//lock the texture and read the data
				D3DLOCKED_RECT  lockrect;
				HRESULT hr = m_Texture->LockRect( 0, &lockrect, NULL, D3DLOCK_READONLY );
				if( hr == D3D_OK )
				{
					UINT pixelsize = (GetColorDepth()/8);
					BYTE * srcbits = (BYTE*)lockrect.pBits;
					BYTE * destbits = (BYTE*)m_pLocalBuffer;
					//write the texture to the buffer
					for( UINT i = 0; i < m_Height; i++ )
					{
						memcpy( destbits, srcbits, m_Width*pixelsize );
						//move by pitch
						srcbits  +=  lockrect.Pitch;
						destbits += m_Width*pixelsize;
					}
				}
				m_Texture->UnlockRect( 0 );
				return m_pLocalBuffer;
			}
		}else//we are render target, need surface
		{			
			LPDIRECT3DSURFACE9 RenderSurf = NULL;
			LPDIRECT3DSURFACE9 OffSurf = NULL;
			m_Texture->GetSurfaceLevel( 0, &RenderSurf );
			
			if( RenderSurf )
			{
				D3DSURFACE_DESC tDesc;
				m_Texture->GetLevelDesc(0, &tDesc );
				if( FAILED(pDevice->CreateOffscreenPlainSurface( tDesc.Width,
													tDesc.Height,
													tDesc.Format,
													D3DPOOL_SYSTEMMEM,													
													&OffSurf,
													NULL )))
				{
					RenderSurf->Release();
					return NULL;
				}
				if( FAILED( pDevice->GetRenderTargetData( RenderSurf, OffSurf ) ))
				{
					RenderSurf->Release();
					OffSurf->Release();
					return NULL;
				}
				//No need for rendertarget surface anymore
				RenderSurf->Release();
				//lock the texture and read the data
				D3DLOCKED_RECT  lockrect;
				HRESULT hr = OffSurf->LockRect( &lockrect, NULL, D3DLOCK_READONLY );
				if( hr == D3D_OK )
				{
					UINT pixelsize = (GetColorDepth()/8);
					BYTE * srcbits = (BYTE*)lockrect.pBits;
					BYTE * destbits = (BYTE*)m_pLocalBuffer;
					//write the texture to the buffer
					for( UINT i = 0; i < m_Height; i++ )
					{
						memcpy( destbits, srcbits, m_Width*pixelsize );
						//move by pitch
						srcbits  +=  lockrect.Pitch;
						destbits += m_Width*pixelsize;
					}
				}
				OffSurf->UnlockRect();
				OffSurf->Release();
			}
			return m_pLocalBuffer;

		}
	}
	return NULL;
}

bool CDX9TextureObject::Write( void *p, int level, IHashString * informat )
{
	ReadyData();

	if (!m_Renderer)
		return false;

	LPDIRECT3DDEVICE9 pDevice = (LPDIRECT3DDEVICE9)m_Renderer->GetAPIDevice();
	if( !pDevice )
		return false;

	if (m_Texture && (p || m_pLocalBuffer))
	{
		void *pData = NULL;
		if (p && m_pLocalBuffer) // if we have a local copy and we are given data to write
		{
			// then copy the given data to our localbuffer before using our local buffer
			memcpy( m_pLocalBuffer, p, m_iLocalBufferSize );
			pData = m_pLocalBuffer;
		}
		else if (p) // else if we are given data but don't have a local copy
		{
			// then just write with the data given
			pData = p;
		}
		else // else we weren't given any data
		{
			// so just write with our local copy (which we logically must have if we reached this line)
			pData = m_pLocalBuffer;
		}
		D3DFORMAT SrcFormat = EEDX9FormatFromString( informat );
		HRESULT hr = EED3DXLoadSurfaceFromMemory( pDevice, m_Texture, level, p, SrcFormat, m_ColorDepth );

		if( hr != D3D_OK )
		{
			m_ToolBox->Log( LOGERROR, _T("Could not copy surface for write.\n"));
			return false;
		}	
        return true;
	}
	m_ToolBox->Log( LOGERROR, _T("Error writing to texture.\n"));				
	return false;
}

void CDX9TextureObject::GenerateMips()
{
	ReadyData();

	if( m_Texture && m_AutoGenMips )
	{
		//This won't work with autogenerate mipmaps off
		m_Texture->AddDirtyRect(NULL);
		m_Texture->GenerateMipSubLevels();
	}
}

UINT CDX9TextureObject::GetLevelCount()
{
	if (m_Texture)
		return m_Texture->GetLevelCount();
	else
		return 0;
}

void * CDX9TextureObject::Lock( INT * pitch, int level )
{
	ReadyData();

	if( m_Texture )
	{
		D3DLOCKED_RECT  lockrect;
		HRESULT hr = m_Texture->LockRect( level, &lockrect, NULL, 0 );
		if( hr == D3D_OK )
		{
			//should return pitch as well, but that's only DX? Or Use read and write
			if( pitch ){
				*pitch = lockrect.Pitch;
			}
			return lockrect.pBits;
		}
	}
	return NULL;
}

void CDX9TextureObject::Unlock( int level )
{
	ReadyData();

	if( m_Texture )
	{
		m_Texture->UnlockRect( level );
	}
}

///
/// Init / Update / DeInit
///
void CDX9TextureObject::Init()
{
	m_Filename = "";
	m_RefCount = 1;
	m_Texture = NULL;
	
	m_Height = 0;
	m_Width = 0;
	m_ColorDepth = 0;
	m_bRenderTarget =false;
}

void CDX9TextureObject::DeInit()
{	
	if (m_Texture)
	{
		m_Texture->Release();
		m_Texture = NULL;
	}

	if (m_pLocalBuffer)
	{
		delete[] m_pLocalBuffer;
		m_pLocalBuffer = NULL;
	}

	m_Filename = "";
	m_RefCount = 1;
	m_pLocalBuffer = NULL;
	m_iLocalBufferSize = 0;
	m_Texture = NULL;

	m_Height = 0;
	m_Width = 0;
	m_ColorDepth = 0;
	m_bRenderTarget =false;
}

unsigned int CDX9TextureObject::GetReferenceCount()
{
	return m_RefCount;
}

IHashString *CDX9TextureObject::GetTextureName()
{
	return &m_Filename;

}

void CDX9TextureObject::SetTextureName(IHashString *name)
{
	m_Filename = name;

}

bool CDX9TextureObject::ReadyData()
{
	// if our data is loaded, we are ready
	if (IsDataLoaded())
		return true;

	// try to get the data loaded
	LOADFILEEXTPARAMS lfep;
	lfep.fileName = const_cast<TCHAR*>((const TCHAR*)GetName()->GetString());
	lfep.bInternalLoad = true;
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);				

	// return whether our data was loaded
	return IsDataLoaded();
}

LPDIRECT3DSURFACE9 CDX9TextureObject::GetRenderTargetSurface()
{
	if (!m_RenderTargetSurface && m_Texture)
	{
		m_Texture->GetSurfaceLevel( 0, &m_RenderTargetSurface );
	}

	return m_RenderTargetSurface;
}

void CDX9TextureObject::ReleaseRenderTargetSurface()
{
	SAFE_RELEASE( m_RenderTargetSurface );
}
