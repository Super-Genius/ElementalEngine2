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

#ifndef CDX9MoviePlayer_H
#define CDX9MoviePlayer_H

#include "CManager.h"

namespace ElementalEngine
{

#ifdef WIN32

class CLockableMutex
{
public:
   inline CLockableMutex( LPCTSTR szName ) throw()
      : m_hMutex( ::CreateMutex( NULL, FALSE, szName ) )
   {
   }

   inline ~CLockableMutex() throw()
   {
      if(m_hMutex) ::CloseHandle( m_hMutex );
   }

   inline void Lock( DWORD dwTimeout )
   {
      DWORD dwWaitResult = ::WaitForSingleObject( m_hMutex, dwTimeout );
      switch( dwWaitResult )
      {
      case WAIT_OBJECT_0:
         break;
      case WAIT_TIMEOUT:
         break;
      default:
		  break;
      }
   }

   inline void Unlock() throw()
   {
      ::ReleaseMutex( m_hMutex );
   }

private:
   HANDLE m_hMutex;              // Mutex Handle
};

class CAutoLockT
{
// Attributes
private:
	CLockableMutex* m_pObject;                 // the locked object

// Ctor/dtor
public:
   // Critical Section Ctor
   inline CAutoLockT( CLockableMutex* pObject ) throw()
      : m_pObject( pObject )
   {
      m_pObject->Lock(INFINITE);
   }

    // Mutex Ctor
   inline CAutoLockT( CLockableMutex* pObject, DWORD dwTimeout )
      : m_pObject(pObject)
   {
      m_pObject->Lock( dwTimeout );
   }

   // Ctor
   inline ~CAutoLockT()
   {
      m_pObject->Unlock();
   }
};

class CTextureRenderer : public CBaseVideoRenderer
{
public:
    CTextureRenderer(LPUNKNOWN pUnk,HRESULT *phr, const TCHAR *szTextureObjectName, bool bUseDynamicTextures, bool bUsePowTwoTextures);
    ~CTextureRenderer();

    HRESULT CheckMediaType(const CMediaType *pmt );     // Format acceptable?
    HRESULT SetMediaType(const CMediaType *pmt );       // Video format notification
    HRESULT DoRenderSample(IMediaSample *pMediaSample); // New video sample

	HRESULT UpdateTexture();

    bool m_bUseDynamicTextures;
    bool m_bUsePowTwoTextures;
    LONG m_lVidWidth;   // Video width
    LONG m_lVidHeight;  // Video Height
    LONG m_lVidPitch;   // Video Pitch

	IToolBox *m_ToolBox;
	CHashString			m_hszTextureObjectName;
	ITextureObject*		m_pTextureObject;
	LPDIRECT3DTEXTURE9	m_pTexture;
	D3DFORMAT			m_TextureFormat;
	CLockableMutex		m_TextureMutex;

	BYTE *m_pTextureBuffer;
	LONG m_iTextureBufferSize;
};

class CWin32MoviePlayer : public IMoviePlayer
{
public:
	CWin32MoviePlayer();
	virtual void Update();
	virtual ITextureObject* PlayMovie( const TCHAR *szFileName, IHashString *pTextureObjectName );
	virtual void StopMovie();
	virtual void PauseMovie( bool bPause );
	virtual void SetVolume( int iVolumeDb );
	virtual bool IsMovieDone() const;

	IToolBox *m_ToolBox;
	CComPtr<IGraphBuilder>  g_pGB;          // GraphBuilder
	CComPtr<IMediaControl>  g_pMC;          // Media Control
	CComPtr<IMediaPosition> g_pMP;          // Media Position
	CComPtr<IMediaEvent>    g_pME;          // Media Event
	CComPtr<IBaseFilter>    g_pRenderer;    // our custom renderer
	CTextureRenderer*		g_pTextureRenderer;
};

#endif//WIN32

class CMoviePlayer : public CManager
{	
private:
	SINGLETONCONSTRUCTOROVERRIDE(CMoviePlayer);
	CMoviePlayer();

public:
	~CMoviePlayer();

	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Initialization for the Object
	virtual bool Init();
	/// \brief	The Update Function for the Object
	virtual void Update(DWORD tickCount);
	/// \brief	The DeInit Function for the Object
	virtual bool DeInit();

	ITextureObject* PlayMovie( const TCHAR *szFileName, IHashString *pTextureObjectName );
	void StopMovie();
	void PauseMovie( bool bPause );
	void SetVolume( int iVolumeDb );
	bool IsMovieDone() const;

	DWORD OnPlayMovie(DWORD size, void *data);
	DWORD OnStopMovie(DWORD size, void *data);
	DWORD OnPauseMovie(DWORD size, void *data);
	DWORD OnSetMovieVolume(DWORD size, void *data);
	DWORD OnIsMovieDone(DWORD size, void *data);
	DWORD OnDestroyRenderer(DWORD size, void *data);

private:
	bool m_bPaused;
	int m_iVolumeDb;
	IMoviePlayer *m_pMoviePlayer;
};

} //namespace EE

#endif