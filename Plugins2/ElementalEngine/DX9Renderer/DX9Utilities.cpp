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

#ifdef WIN32
D3DFORMAT EEDX9FormatFromColorBits( UINT &colorbits, bool bTiled )
{
	D3DFORMAT retval;
	switch(colorbits)
	{
		case 32:
			{
				retval = D3DFMT_A8R8G8B8;
				break;
			}
		case 24:
			{
				colorbits = 32;
				retval = D3DFMT_X8R8G8B8;
				break;
			}
		case 16:
			{
				retval = D3DFMT_R5G6B5;
				break;
			}
		case 8:
			{
				//8 bit alpha, we can ask for 8 bit luminance too..again needs more support work
				//retval = D3DFMT_A8; doesn't seem widely supported?
				retval = D3DFMT_L8;
				break;
			}
		default:
			{
				retval = D3DFMT_A8R8G8B8;
				break;
			}
	};
	return retval;
}
#endif//WIN32

#ifdef WIN32
DWORD EEDX9ColorBitsFromFormat( D3DFORMAT format )
{
	switch(format)
	{
		case D3DFMT_DXT5:
		case D3DFMT_DXT3:
		case D3DFMT_DXT1:
		case D3DFMT_A8R8G8B8:
		case D3DFMT_X8R8G8B8:
			return 32;

		case D3DFMT_A8:
		case D3DFMT_L8:
			return 8;
		default:
			// if this assert is hit, add the format to the switch with the correct
			// parameters for the texture object
			assert( 0 );
			EngineGetToolBox()->Log( LOGERROR, _T("EEDX9GetColorBitsFromFormat: Unknown format.\n") );
			return 0;
	}
}
#endif//WIN32

#ifdef WIN32
bool EEDX9IsCompressedFormat( D3DFORMAT format )
{
	switch(format)
	{
		case D3DFMT_DXT5:
		case D3DFMT_DXT3:
		case D3DFMT_DXT1:
			return true;
		default:
			return false;
	}
}
#endif//WIN32

#ifdef WIN32
D3DFORMAT EEDX9FormatFromString( IHashString *hszFormat )
{
	static CHashString dxt1(_T("DXT1"));
	static CHashString dxt2(_T("DXT2"));
	static CHashString dxt3(_T("DXT3"));
	static CHashString dxt4(_T("DXT4"));
	static CHashString dxt5(_T("DXT5"));
	static CHashString a32b32g32r32f(_T("A32B32G32R32F"));
	D3DFORMAT format = D3DFMT_UNKNOWN;
	if( hszFormat )
	{
		if( dxt1.GetUniqueID() == hszFormat->GetUniqueID() ){ format = D3DFMT_DXT1; }
		else if( dxt2.GetUniqueID() == hszFormat->GetUniqueID() ){ format = D3DFMT_DXT2; }
		else if( dxt3.GetUniqueID() == hszFormat->GetUniqueID() ){ format = D3DFMT_DXT3; }
		else if( dxt4.GetUniqueID() == hszFormat->GetUniqueID() ){ format = D3DFMT_DXT4; }
		else if( dxt5.GetUniqueID() == hszFormat->GetUniqueID() ){ format = D3DFMT_DXT5; }
		else if( a32b32g32r32f.GetUniqueID() == hszFormat->GetUniqueID() ){ format = D3DFMT_A32B32G32R32F; }
	}
	return format;
}
#endif//WIN32

UINT EEDX9FormatPitch( D3DFORMAT format, UINT width, UINT bitdepth )
{
	if( format == D3DFMT_DXT1 )
	{
		return (( width / 4 )* 8);
	}
	else if( format == D3DFMT_DXT3 || format == D3DFMT_DXT5 )
	{
		return (( width / 4 )* 16);
	}
	else
		return width*(bitdepth/8);
}

#ifdef WIN32
HRESULT EED3DXLoadSurfaceFromMemory( LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 pDestTexture, UINT DestLevel, LPCVOID pSrcMemory, D3DFORMAT SrcFormat, UINT SrcColorDepth )
{
	HRESULT hr;
	LPDIRECT3DSURFACE9 DestSurface = NULL;
	D3DSURFACE_DESC DestDesc;		

	pDestTexture->GetSurfaceLevel( DestLevel, &DestSurface );		
	hr = DestSurface->GetDesc( &DestDesc );
	if( hr != D3D_OK )
	{
		SAFE_RELEASE( DestSurface );
		return hr;
	}

	if( SrcFormat == D3DFMT_UNKNOWN )
	{
		SrcFormat = EEDX9FormatFromColorBits( SrcColorDepth );
	}
	UINT SrcPitch = EEDX9FormatPitch( SrcFormat, DestDesc.Width, SrcColorDepth );

	RECT surfsize;
	surfsize.top = 0;
	surfsize.bottom = DestDesc.Height;
	surfsize.left = 0;
	surfsize.right = DestDesc.Width;
	hr = D3DXLoadSurfaceFromMemory( DestSurface, NULL, NULL, pSrcMemory, SrcFormat,SrcPitch, NULL, &surfsize, D3DX_FILTER_NONE, 0 );
	SAFE_RELEASE( DestSurface );
	return D3D_OK;
}
#endif//WIN32

#ifdef WIN32
D3DFORMAT EEDX9RequestedInternalFormatFromFileInMemory( LPVOID pFileData, UINT iFileDataSize )
{
	return D3DFMT_UNKNOWN;
}
#endif//WIN32

#ifdef WIN32
/*
HRESULT EEDX9Clear( LPDIRECT3DDEVICE9 pDevice, bool clearFlags, bool clearTarget )
{
	DWORD clearFlags = 0;
	if( clearDepth )clearFlags  |= D3DCLEAR_ZBUFFER;
	if( clearTarget )clearFlags |= D3DCLEAR_TARGET;
	// Clear the screen
	SetColorMask(true, true, true, true);
	HRESULT hr = pDevice->Clear( 0, NULL, clearFlags, 
				D3DCOLOR_ARGB((int)m_ClearColor[0], (int)m_ClearColor[1], (int)m_ClearColor[2], (int)m_ClearColor[3]),
				1.0f, 0 );
	assert( SUCCEEDED(hr) );
}
*/

HRESULT EEDX9Present( LPDIRECT3DDEVICE9 pDevice, CDX9RenderContext *pDX9Context, UINT ClearColor[4] )
{
	HRESULT result;
	if( pDX9Context )
	{
		result = pDevice->Present( NULL, NULL, pDX9Context->m_hWnd, NULL );	
	}
	else
	{
		result = pDevice->Present( NULL, NULL, NULL, NULL );	
	}
	return result;
}
#endif//WIN32

bool GetStringFromFile( StdString &szFilepath, StdString &szOutput )
{
	CHashString streamType(_T("File"));
	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_READ;
	ca.streamData = (TCHAR*)szFilepath.c_str();
	ca.streamType = &streamType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (EngineGetToolBox()->SendMessage(msgHash_CreateArchive, sizeof(ca), &ca) != MSG_HANDLED)
		return false;

	ca.archive->Read( szOutput );
	ca.archive->Close();
	return true;
}

BOOL LoadFileIntoBuffer( const TCHAR* pFileName, LPVOID& pFileData, UINT& iFileDataSize, bool bBinary )
{
	CREATESTREAM cs;
	cs.mode = STREAM_MODE_READ | (bBinary ? STREAM_MODE_BINARY : 0);
	cs.streamData = (void*)pFileName;
	cs.streamSize = 0;
	static DWORD msgHash_CreateStream_File = CHashString(_T("CreateStream_File")).GetUniqueID();

	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_CreateStream_File, sizeof(CREATESTREAM), &cs);
	if ((retval != MSG_HANDLED) || (cs.openStream == NULL))
	{
		EngineGetToolBox()->Log( LOGERROR, _T("Error loading file into buffer: %s\n"), pFileName );
		return false;
	}
	
	cs.openStream->SeekTo(0, SEEK_END);
	iFileDataSize = cs.openStream->Tell();
	pFileData = new BYTE[iFileDataSize];
	cs.openStream->SeekTo(0,SEEK_SET);
	cs.openStream->Read( pFileData, iFileDataSize );
	cs.openStream->Close();
	delete cs.openStream;

	return true;
}
