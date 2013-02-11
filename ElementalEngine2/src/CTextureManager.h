///============================================================================
/// \file	CTextureManager.h
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

#ifndef _ITEXTUREMANAGER_H
#define _ITEXTUREMANAGER_H

#include "CManager.h"
#include "ITextureManager.h"
#include <set>
#include <map>
using namespace std;

typedef std::set< IBaseTextureObject * > TEXTURELIST;
typedef std::map<DWORD, IBaseTextureObject * > TEXTURENAMEMAP;
typedef std::pair<DWORD, IBaseTextureObject * > TEXTURENAMEMAP_ELEMENT;

struct DDSPixelFormat 
{
     UINT size;
     UINT flags;
     UINT fourcc;
     UINT bitcount;
     UINT rmask;
     UINT gmask;
     UINT bmask;
     UINT amask;
 };

struct DDSCaps 
{
     UINT caps1;
     UINT caps2;
     UINT caps3;
     UINT caps4;
 };

struct DDSHeader 
{
     UINT size;
     UINT flags;
     UINT height;
     UINT width;
     UINT pitch;
     UINT depth;
     UINT mipmapcount;
     UINT reserved[11];
     DDSPixelFormat pf;
     DDSCaps caps;
     UINT notused;
};


//Should extend off ITextureManager also but need some sort of manager Template for that
class CTextureManager : public CManager
{
public:
	~CTextureManager();

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief	DeInitialization
	/// \return	bool: Success
	virtual bool DeInit( void );

	/// \brief Sets the working directories
	DWORD OnSetDirectories( DWORD size, void * params );

public:
	//Interface Functions (Extends off ITextureManager)
	virtual DWORD OnSetLoadParameters( DWORD size, void *params);
	virtual DWORD OnGetLoadParameters( DWORD size, void *params);
	virtual DWORD OnAddTexture( DWORD size, void *params);
	virtual DWORD OnRemoveTexture( DWORD size, void *params);
	virtual DWORD OnUnloadObject( DWORD size, void *params);
	virtual DWORD OnGetTexture( DWORD size, void *params);
	virtual DWORD OnCreateTexture( DWORD size, void *params);
	virtual DWORD OnCreateCubeTexture( DWORD size, void * params );
	DWORD OnDestroyRenderer(DWORD size, void *param);
	virtual IBaseTextureObject * GetTexture(IHashString *name );


private:
	virtual bool DeleteObject( IObject* Object );
	IBaseTextureObject* CreateTextureObject( IHashString *pTextureName, IHashString *pComponentType );
	void DeleteTextureObject( IBaseTextureObject *pTextureObject );
	IBaseTextureObject * LoadTextureByExtension( IHashString * name );
	IBaseTextureObject * LoadCubeDDSTexture( IHashString * name );
	bool IsDDSCubeMap( const DDSHeader & header );
	SINGLETONCONSTRUCTOROVERRIDE( CTextureManager );

	/// \brief The Default Constructor for CTextureManager
	/// \return void
	CTextureManager();
	bool		m_bInitialized;
	std::map<DWORD, IBaseTextureObject * > m_TextureNameMap;
	/// Current area for loading textures.
	TEX_MEM_AREA m_currentTexMemArea;
};

#endif