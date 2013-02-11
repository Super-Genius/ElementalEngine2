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
#ifdef WIN32
#include <atlbase.h>
#include <dshow.h>
#include <streams.h>
#endif
#include "CDX9MoviePlayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MOVIEPLAYERPRIORITY     0x0583

REGISTER_COMPONENT_SINGLETON( CMoviePlayer );
REGISTER_MESSAGE_HANDLER(PlayMovie, OnPlayMovie, CMoviePlayer);
REGISTER_MESSAGE_HANDLER(StopMovie, OnStopMovie, CMoviePlayer);
REGISTER_MESSAGE_HANDLER(PauseMovie, OnPauseMovie, CMoviePlayer);
REGISTER_MESSAGE_HANDLER(SetMovieVolume, OnSetMovieVolume, CMoviePlayer);
REGISTER_MESSAGE_HANDLER(IsMovieDone, OnIsMovieDone, CMoviePlayer);
REGISTER_MESSAGE_HANDLER(DestroyRenderer, OnDestroyRenderer, CMoviePlayer);

IComponent *CMoviePlayer::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CMoviePlayer );
}

CMoviePlayer::CMoviePlayer() : 
	CManager(_T("CMoviePlayer"), MOVIEPLAYERPRIORITY, 0)
{
#ifdef WIN32
	m_pMoviePlayer = new CWin32MoviePlayer;
#elif defined XBOX
	m_pMoviePlayer = new CXboxMoviePlayer;
#else
	m_pMoviePlayer = NULL;
#endif
	Init();
}

CMoviePlayer::~CMoviePlayer()
{
	SAFE_DELETE( m_pMoviePlayer );
}

bool CMoviePlayer::Init()
{
	return CManager::Init();
}

bool CMoviePlayer::DeInit()
{
	StopMovie();
	return CManager::DeInit();
}

void CMoviePlayer::Update(DWORD tickCount)
{
	if (IsMovieDone())
		StopMovie();

	if (m_pMoviePlayer)
		m_pMoviePlayer->Update();
}

ITextureObject* CMoviePlayer::PlayMovie( const TCHAR *szFileName, IHashString *pTextureObjectName )
{
	StopMovie();

	ITextureObject *pTextureObject = NULL;
	if (m_pMoviePlayer)
		pTextureObject = m_pMoviePlayer->PlayMovie( szFileName, pTextureObjectName );

	// set movie volume to our stored volume
	SetVolume( m_iVolumeDb );

	return pTextureObject;
}

void CMoviePlayer::StopMovie()
{
	if (m_pMoviePlayer)
		m_pMoviePlayer->StopMovie();
}

void CMoviePlayer::PauseMovie( bool bPause )
{
	if (m_bPaused == bPause)
		return;

	if (m_pMoviePlayer)
		m_pMoviePlayer->PauseMovie( bPause );

	m_bPaused = bPause;
}

void CMoviePlayer::SetVolume( int iVolumeDb )
{
	m_iVolumeDb = iVolumeDb;

	if (m_pMoviePlayer)
		m_pMoviePlayer->SetVolume( iVolumeDb );
}

bool CMoviePlayer::IsMovieDone() const
{
	if (m_pMoviePlayer)
		return m_pMoviePlayer->IsMovieDone();
	else
		return true;
}

DWORD CMoviePlayer::OnPlayMovie(DWORD size, void *data)
{
	VERIFY_MESSAGE_SIZE(sizeof(PLAYMOVIEPARAMS), size);
	PLAYMOVIEPARAMS *pParams = ((PLAYMOVIEPARAMS*)data);

	const TCHAR *szFileName = pParams->in_movieFileName->GetString();

	ITextureObject *pTextureObject = NULL;
	if ((pTextureObject = PlayMovie(szFileName, pParams->inout_movieTargetTextureName)) != NULL)
	{
		pParams->inout_movieTargetTextureName = pTextureObject->GetName();
		pParams->out_pTargetTexture = pTextureObject;
		return MSG_HANDLED_STOP;
	}
	else
	{
		StopMovie(); // cleanup
		return MSG_ERROR;
	}
}

DWORD CMoviePlayer::OnStopMovie(DWORD size, void *data)
{
	StopMovie();
	return MSG_HANDLED_STOP;
}

DWORD CMoviePlayer::OnPauseMovie(DWORD size, void *data)
{
	VERIFY_MESSAGE_SIZE(sizeof(bool), size);
	bool *pParams = ((bool*)data);

	PauseMovie( *pParams );
	return MSG_HANDLED_STOP;
}

DWORD CMoviePlayer::OnSetMovieVolume(DWORD size, void *data)
{
	VERIFY_MESSAGE_SIZE(sizeof(int), size);
	int *pParams = ((int*)data);

	SetVolume( *pParams );
	return MSG_HANDLED_STOP;
}

DWORD CMoviePlayer::OnIsMovieDone(DWORD size, void *data)
{
	VERIFY_MESSAGE_SIZE(sizeof(bool), size);
	bool *pParams = ((bool*)data);

	*pParams = IsMovieDone();
	return MSG_HANDLED_STOP;
}

DWORD CMoviePlayer::OnDestroyRenderer(DWORD size, void *data)
{
	DeInit();
	return MSG_HANDLED_PROCEED;
}


//-----------------------------------------------------------------------------
// Win32 Movie Player
//-----------------------------------------------------------------------------
#ifdef WIN32
CWin32MoviePlayer::CWin32MoviePlayer()
{
	m_ToolBox = EngineGetToolBox();
}

void CWin32MoviePlayer::Update()
{
	if (g_pTextureRenderer)
		g_pTextureRenderer->UpdateTexture();
}

ITextureObject* CWin32MoviePlayer::PlayMovie( const TCHAR *szFileName, IHashString *pTextureObjectName )
{
	GETRENDERERINTERFACEPARAMS grip;
	static DWORD msgGetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgGetRendererInterface, sizeof(grip), &grip );
	if (!grip.m_RendererInterface)
		return NULL;

	LPDIRECT3DDEVICE9 pd3dDevice = (LPDIRECT3DDEVICE9)grip.m_RendererInterface->GetAPIDevice();
	if (!pd3dDevice)
		return NULL;

	HRESULT hr = S_OK;
	bool bUseDynamicTextures=false, bUsePowTwoTextures=true;

    D3DCAPS9 caps;
    ZeroMemory( &caps, sizeof(D3DCAPS9) );
    hr = pd3dDevice->GetDeviceCaps( &caps );
    bUseDynamicTextures = ((caps.Caps2 & D3DCAPS2_DYNAMICTEXTURES) != 0);
    bUsePowTwoTextures = ((caps.Caps2 & D3DPTEXTURECAPS_POW2) != 0);

    CComPtr<IBaseFilter>    pFSrc;          // Source Filter
    CComPtr<IPin>           pFSrcPinOut;    // Source Filter Output Pin   
    CTextureRenderer        *pCTR=0;        // DirectShow Texture renderer

    // Create the filter graph
    if (FAILED(g_pGB.CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC)))
        return NULL;

#ifdef REGISTER_FILTERGRAPH
    // Register the graph in the Running Object Table (for debug purposes)
    AddToROT(g_pGB);
#endif

	// Create texture object name
	CHashString hszTextureObjectName;
	if (pTextureObjectName == NULL)
	{
		// Generate a unique name for texture object
		GENERATEUNIQUEOBJECTNAMEPARAMS generateNameParams;
		CHashString baseName( _T("MovieTextureTarget") );
		generateNameParams.name = &baseName;
		generateNameParams.newname = &hszTextureObjectName;
		static DWORD msgHash_GenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(generateNameParams), &generateNameParams ) == MSG_HANDLED)
		{
			// WHOOPS! gotta convert to safe filename...
			StdString szTempName = hszTextureObjectName.GetString();
			szTempName.MakeSafeFileName();
			hszTextureObjectName.Init(szTempName);
			pTextureObjectName = &hszTextureObjectName;
		}
		else
			return NULL;
	}

	// Create the Texture Renderer object
	pCTR = new CTextureRenderer(NULL, &hr, pTextureObjectName->GetString(), bUseDynamicTextures, bUsePowTwoTextures);
	if (FAILED(hr) || !pCTR)
	{
		m_ToolBox->Log( LOGERROR, _T("Could not create texture renderer object!  hr=0x%x"), hr );
        return NULL;
    }

    // Get a pointer to the IBaseFilter on the TextureRenderer, add it to graph
    g_pRenderer = g_pTextureRenderer = pCTR;
    if (FAILED(hr = g_pGB->AddFilter(g_pRenderer, L"TEXTURERENDERER")))
    {
        m_ToolBox->Log( LOGERROR, _T("Could not add renderer filter to graph!  hr=0x%x"), hr );
        return NULL;
    }

    // Add the source filter to the graph.
#ifdef _UNICODE
    hr = g_pGB->AddSourceFilter(szFileName, L"SOURCE", &pFSrc);
#else
	WCHAR szWFileName[MAX_PATH];
	MultiByteToWideChar( CP_ACP, 0, szFileName, -1, szWFileName, MAX_PATH );
	hr = g_pGB->AddSourceFilter(szWFileName, L"SOURCE", &pFSrc);
#endif

    // If the media file was not found, inform the user.
    if (hr == VFW_E_NOT_FOUND)
    {
        m_ToolBox->Log( LOGERROR, _T("Could not add source filter to graph!  (hr==VFW_E_NOT_FOUND)\r\n\r\n")
            _T("This sample reads a media file from the DirectX SDK's media path.\r\n")
            _T("Please install the DirectX 9 SDK on this machine.") );
        return NULL;
    }
    else if(FAILED(hr))
    {
        m_ToolBox->Log( LOGERROR, _T("Could not add source filter to graph!  hr=0x%x"), hr );
        return NULL;
    }

    if (FAILED(hr = pFSrc->FindPin(L"Output", &pFSrcPinOut)))
    {
        m_ToolBox->Log( LOGERROR, _T("Could not find output pin!  hr=0x%x"), hr );
        return NULL;
    }

#ifdef NO_AUDIO_RENDERER
    // If no audio component is desired, directly connect the two video pins
    // instead of allowing the Filter Graph Manager to render all pins.

    CComPtr<IPin> pFTRPinIn;      // Texture Renderer Input Pin

    // Find the source's output pin and the renderer's input pin
    if (FAILED(hr = pFTR->FindPin(L"In", &pFTRPinIn)))
    {
        m_ToolBox->Log( LOGERROR, _T("Could not find input pin!  hr=0x%x"), hr );
        return NULL;
    }

    // Connect these two filters
    if (FAILED(hr = g_pGB->Connect(pFSrcPinOut, pFTRPinIn)))
    {
        m_ToolBox->Log( LOGERROR, _T("Could not connect pins!  hr=0x%x"), hr );
        return NULL;
    }
#else
    // Render the source filter's output pin.  The Filter Graph Manager
    // will connect the video stream to the loaded CTextureRenderer
    // and will load and connect an audio renderer (if needed).
    if (FAILED(hr = g_pGB->Render(pFSrcPinOut)))
    {
        m_ToolBox->Log( LOGERROR, _T("Could not render source output pin!  hr=0x%x"), hr );
        return NULL;
    }
#endif
   
    // Get the graph's media control, event & position interfaces
    g_pGB.QueryInterface(&g_pMC);
    g_pGB.QueryInterface(&g_pMP);
    g_pGB.QueryInterface(&g_pME);
    
    // Start the graph running;
	hr = g_pMC->Run();
    if (FAILED(hr))
    {
        m_ToolBox->Log( LOGERROR, _T("Could not run the DirectShow graph!  hr=0x%x"), hr );
        return NULL;
    }

	return g_pTextureRenderer->m_pTextureObject;
}

void CWin32MoviePlayer::StopMovie()
{
#ifdef REGISTER_FILTERGRAPH
    // Pull graph from Running Object Table (Debug)
    RemoveFromROT();
#endif

    // Shut down the graph
    if (!(!g_pMC)) g_pMC->Stop();

    if (!(!g_pMC)) g_pMC.Release();
    if (!(!g_pME)) g_pME.Release();
    if (!(!g_pMP)) g_pMP.Release();
    if (!(!g_pGB)) g_pGB.Release();
    if (!(!g_pRenderer)) g_pRenderer.Release();

	g_pTextureRenderer = NULL;
}

void CWin32MoviePlayer::PauseMovie( bool bPause )
{
	HRESULT hr = S_OK;
	if (g_pMC && bPause)
		hr = g_pMC->Pause();
	else if (g_pMC && !bPause)
		hr = g_pMC->Run();

    if (FAILED(hr))
    {
		m_ToolBox->Log( LOGERROR, _T("Could not pause/run the DirectShow graph!  hr=0x%x"), hr );
    }
}

void CWin32MoviePlayer::SetVolume( int iVolumeDb )
{
	if (g_pGB)
	{
		IBasicAudio *pBA = NULL;
		HRESULT hr = g_pGB.QueryInterface( (IBasicAudio**)&pBA );
		if (SUCCEEDED(hr))
		{
			pBA->put_Volume( iVolumeDb * 100 );
			SAFE_RELEASE(pBA);
		}
	}
}

bool CWin32MoviePlayer::IsMovieDone() const
{
	long lEventCode;
    LONG_PTR lParam1, lParam2;
    HRESULT hr;

    if (!g_pME)
        return true;

	bool bDone = false;

	// Check for completion events
    hr = g_pME->GetEvent(&lEventCode, &lParam1, &lParam2, 0);
    if (SUCCEEDED(hr))
    {
        // If we have reached the end of the media file
        if (EC_COMPLETE == lEventCode) 
        {
            bDone = true;
        }

        // Free any memory associated with this event
        hr = g_pME->FreeEventParams(lEventCode, lParam1, lParam2);
    }
	return bDone;
}

//-----------------------------------------------------------------------------
// Define GUID for Texture Renderer
// {71771540-2017-11cf-AE26-0020AFD79767}
//-----------------------------------------------------------------------------
struct __declspec(uuid("{71771540-2017-11cf-ae26-0020afd79767}")) CLSID_TextureRenderer;

//-----------------------------------------------------------------------------
// CTextureRenderer constructor
//-----------------------------------------------------------------------------
CTextureRenderer::CTextureRenderer( LPUNKNOWN pUnk, HRESULT *phr, const TCHAR *szTextureObjectName, bool bUseDynamicTextures, bool bUsePowTwoTextures )
                                  : CBaseVideoRenderer(__uuidof(CLSID_TextureRenderer), 
                                    NAME("Texture Renderer"), pUnk, phr), 
									m_TextureMutex(_T("TextureMutex"))
{
	m_ToolBox = EngineGetToolBox();
	m_bUseDynamicTextures = bUseDynamicTextures;
	m_bUsePowTwoTextures = bUsePowTwoTextures;
	m_hszTextureObjectName = szTextureObjectName;
	m_pTextureObject = NULL;
	m_pTexture = NULL;
    m_lVidWidth = 0;
    m_lVidHeight = 0;
    m_lVidPitch = 0;

	m_pTextureBuffer = NULL;
	m_iTextureBufferSize = 0;

    ASSERT(phr);
    if (phr)
        *phr = S_OK;
}


//-----------------------------------------------------------------------------
// CTextureRenderer destructor
//-----------------------------------------------------------------------------
CTextureRenderer::~CTextureRenderer()
{
	if (m_pTextureObject)
	{
		TEXTUREOBJECTPARAMS tobj;
		tobj.bLoad = false;
		tobj.Name = &m_hszTextureObjectName;
		tobj.TextureObjectInterface = NULL;
		static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );

		m_pTextureObject = NULL;
	}

	SAFE_DELETE_ARRAY( m_pTextureBuffer );
}


//-----------------------------------------------------------------------------
// CheckMediaType: This method forces the graph to give us an R8G8B8 video
// type, making our copy to texture memory trivial.
//-----------------------------------------------------------------------------
HRESULT CTextureRenderer::CheckMediaType(const CMediaType *pmt)
{
    HRESULT   hr = E_FAIL;
    VIDEOINFO *pvi=0;
    
    CheckPointer(pmt,E_POINTER);

    // Reject the connection if this is not a video type
    if( *pmt->FormatType() != FORMAT_VideoInfo ) {
        return E_INVALIDARG;
    }
    
    // Only accept RGB24 video
    pvi = (VIDEOINFO *)pmt->Format();

    if(IsEqualGUID( *pmt->Type(),    MEDIATYPE_Video)  &&
       IsEqualGUID( *pmt->Subtype(), MEDIASUBTYPE_RGB24))
    {
        hr = S_OK;
    }
    
    return hr;
}

//-----------------------------------------------------------------------------
// SetMediaType: Graph connection has been made. 
//-----------------------------------------------------------------------------
HRESULT CTextureRenderer::SetMediaType(const CMediaType *pmt)
{
    HRESULT hr;

    UINT uintWidth = 2;
    UINT uintHeight = 2;

    // Retrive the size of this media type
    VIDEOINFO *pviBmp;                      // Bitmap info header
    pviBmp = (VIDEOINFO *)pmt->Format();

    m_lVidWidth  = pviBmp->bmiHeader.biWidth;
    m_lVidHeight = abs(pviBmp->bmiHeader.biHeight);
    m_lVidPitch  = (m_lVidWidth * 3 + 3) & ~(3); // We are forcing RGB24

	if (m_bUsePowTwoTextures)
    {
        while( (LONG)uintWidth < m_lVidWidth )
        {
            uintWidth = uintWidth << 1;
        }
        while( (LONG)uintHeight < m_lVidHeight )
        {
            uintHeight = uintHeight << 1;
        }
    }
    else
    {
        uintWidth = m_lVidWidth;
        uintHeight = m_lVidHeight;
    }

	hr = E_UNEXPECTED;
//	if( m_bUseDynamicTextures )
	m_bUseDynamicTextures = false;
	{
		CREATETEXTUREPARAMS createtex;
		createtex.bitDepth = 24;
		createtex.sizeX = uintWidth;
		createtex.sizeY = uintHeight;
		createtex.Name = &m_hszTextureObjectName;
		static DWORD msgHash_CreateTexture = CHashString(_T("CreateTexture")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateTexture, sizeof(createtex), &createtex );
		m_pTextureObject = dynamic_cast< ITextureObject*>( createtex.TextureObjectInterface );
	}

    if (m_pTextureObject == NULL)
    {
        m_ToolBox->Log( LOGERROR, _T("Could not create the D3DX texture!  hr=0x%x"), hr );
        return hr;
    }
	m_pTexture = (LPDIRECT3DTEXTURE9)m_pTextureObject->GetAPITexture();


    // CreateTexture can silently change the parameters on us
    D3DSURFACE_DESC ddsd;
    ZeroMemory(&ddsd, sizeof(ddsd));
    if ( FAILED( hr = m_pTexture->GetLevelDesc( 0, &ddsd ) ) )
	{
        m_ToolBox->Log( LOGERROR, _T("Could not get level Description of D3DX texture! hr = 0x%x"), hr );
        return hr;
    }

    CComPtr<IDirect3DSurface9> pSurf; 
    if (SUCCEEDED(hr = m_pTexture->GetSurfaceLevel(0, &pSurf)))
        pSurf->GetDesc(&ddsd);

    // Save format info
    m_TextureFormat = ddsd.Format;

    if (m_TextureFormat != D3DFMT_X8R8G8B8 &&
        m_TextureFormat != D3DFMT_A1R5G5B5) {
        m_ToolBox->Log( LOGERROR, _T("Texture is format we can't handle! Format = 0x%x"), m_TextureFormat );
        return VFW_E_TYPE_NOT_ACCEPTED;
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
// DoRenderSample: A sample has been delivered. Copy it to the texture.
//-----------------------------------------------------------------------------
HRESULT CTextureRenderer::DoRenderSample( IMediaSample * pSample )
{
    BYTE  *pBmpBuffer; // Bitmap buffer, texture buffer

    CheckPointer(pSample,E_POINTER);
    CheckPointer(m_pTexture,E_UNEXPECTED);

	CAutoLockT sLock(&m_TextureMutex);

	// Get the video bitmap buffer
    pSample->GetPointer( &pBmpBuffer );

	if (m_pTextureBuffer == NULL || m_iTextureBufferSize != pSample->GetSize())
	{
		SAFE_DELETE_ARRAY( m_pTextureBuffer );
		m_iTextureBufferSize = pSample->GetSize();
		m_pTextureBuffer = new BYTE[m_iTextureBufferSize];
	}

	memcpy( m_pTextureBuffer, pBmpBuffer, m_iTextureBufferSize );

	return S_OK;
}

HRESULT CTextureRenderer::UpdateTexture()
{
    BYTE  *pBmpBuffer, *pTxtBuffer; // Bitmap buffer, texture buffer
    LONG  lTxtPitch;                // Pitch of bitmap, texture

    BYTE  * pbS = NULL;
    DWORD * pdwS = NULL;
    DWORD * pdwD = NULL;
    UINT row, col, dwordWidth;

    CheckPointer(m_pTextureBuffer,E_UNEXPECTED);
    CheckPointer(m_pTexture,E_UNEXPECTED);

	CAutoLockT sLock(&m_TextureMutex);

	pBmpBuffer = m_pTextureBuffer;

	// Lock the Texture
    D3DLOCKED_RECT d3dlr;
    if( m_bUseDynamicTextures )
    {
        if( FAILED(m_pTexture->LockRect(0, &d3dlr, 0, D3DLOCK_DISCARD)))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture->LockRect(0, &d3dlr, 0, 0)))
            return E_FAIL;
    }

	// Get the texture buffer & pitch
    pTxtBuffer = static_cast<byte *>(d3dlr.pBits);
    lTxtPitch = d3dlr.Pitch;

    // Copy the bits    

    if (m_TextureFormat == D3DFMT_X8R8G8B8) 
    {
        // Instead of copying data bytewise, we use DWORD alignment here.
        // We also unroll loop by copying 4 pixels at once.
        //
        // original BYTE array is [b0][g0][r0][b1][g1][r1][b2][g2][r2][b3][g3][r3]
        //
        // aligned DWORD array is     [b1 r0 g0 b0][g2 b2 r1 g1][r3 g3 b3 r2]
        //
        // We want to transform it to [ff r0 g0 b0][ff r1 g1 b1][ff r2 g2 b2][ff r3 b3 g3]
        // below, bitwise operations do exactly this.

        dwordWidth = m_lVidWidth / 4; // aligned width of the row, in DWORDS
                                      // (pixel by 3 bytes over sizeof(DWORD))

        for( row = 0; row< (UINT)m_lVidHeight; row++)
        {
            pdwS = ( DWORD*)pBmpBuffer;
            pdwD = ( DWORD*)(pTxtBuffer + (m_lVidHeight-row-1) * (lTxtPitch));

            for( col = 0; col < dwordWidth; col ++ )
            {
                pdwD[0] =  pdwS[0] | 0xFF000000;
                pdwD[1] = ((pdwS[1]<<8)  | 0xFF000000) | (pdwS[0]>>24);
                pdwD[2] = ((pdwS[2]<<16) | 0xFF000000) | (pdwS[1]>>16);
                pdwD[3] = 0xFF000000 | (pdwS[2]>>8);
                pdwD +=4;
                pdwS +=3;
            }

            // we might have remaining (misaligned) bytes here
            pbS = (BYTE*) pdwS;
            for( col = 0; col < (UINT)m_lVidWidth % 4; col++)
            {
                *pdwD = 0xFF000000     |
                        (pbS[2] << 16) |
                        (pbS[1] <<  8) |
                        (pbS[0]);
                pdwD++;
                pbS += 3;           
            }

            pBmpBuffer  += m_lVidPitch;
        }// for rows
    }

    if (m_TextureFormat == D3DFMT_A1R5G5B5) 
    {
        for(int y = 0; y < m_lVidHeight; y++ ) 
        {
            BYTE *pBmpBufferOld = pBmpBuffer;
            pdwD = ( DWORD*)(pTxtBuffer + (m_lVidHeight-y-1) * (lTxtPitch));

            for (int x = 0; x < m_lVidWidth; x++) 
            {
                *(WORD *)pdwD = (WORD)
                    (0x8000 +
                    ((pBmpBuffer[2] & 0xF8) << 7) +
                    ((pBmpBuffer[1] & 0xF8) << 2) +
                    (pBmpBuffer[0] >> 3));

                pdwD += 2;
                pBmpBuffer += 3;
            }

            pBmpBuffer = pBmpBufferOld + m_lVidPitch;
        }
    }

    // Unlock the Texture
    if (FAILED(m_pTexture->UnlockRect(0)))
        return E_FAIL;

	return S_OK;
}

#endif//WIN32
