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

#ifdef XBOX
#include "SrcXbox360/DX9PlatformXbox360.h"
#else
#include "CDX9TextureObject.h"
#define EEDX9SetTransform( device, a, b ) device->SetTransform( a, b )
#endif

D3DFORMAT EEDX9FormatFromColorBits( UINT &colorbits, bool bTiled=false );
DWORD EEDX9ColorBitsFromFormat( D3DFORMAT format );
bool EEDX9IsCompressedFormat( D3DFORMAT format );
D3DFORMAT EEDX9FormatFromString( IHashString *hszFormat );
UINT EEDX9FormatPitch( D3DFORMAT format, UINT width, UINT bitdepth );
HRESULT EED3DXLoadSurfaceFromMemory( LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 pDestTexture, UINT DestLevel, LPCVOID pSrcMemory, D3DFORMAT SrcFormat, UINT SrcColorDepth );
D3DFORMAT EEDX9RequestedInternalFormatFromFileInMemory( LPVOID pFileData, UINT iFileDataSize );
//HRESULT EEDX9Clear( bool clearDepth, bool clearTarget );
HRESULT EEDX9Present( LPDIRECT3DDEVICE9 pDevice, CDX9RenderContext *pDX9Context, UINT ClearColor[4] );
