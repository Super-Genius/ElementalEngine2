///============================================================================
/// \file	CFontManager.h
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

#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "CManager.h"
#include "IFont.h"
#include <set>

using namespace std;

#define MAX_FONT_BUFFER 40000
typedef map<DWORD, IBaseTextureObject*> GLYPHTEXTUREMAP;

class CFontManager : public CManager
{

public:

	/// \brief The Default Destructor for CFontManager
	/// \return void
	~CFontManager( );

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief	Update function for the Particle Manager
	/// \return	void
	virtual void Update( DWORD tickCount );
	
	/// \brief	DeInitialization
	/// \return	bool: Success
	virtual bool DeInit( void );

	DWORD OnCreateFont(DWORD size, void *param);

	DWORD OnDeleteFont(DWORD size, void *param);

	DWORD OnGetFontPixWidth(DWORD size, void *param);

	/// Ticks just the renderer stuffs, such that screens can be redrawn without having to worry about 
	/// ticking all the various systems 
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_PROCEED
	DWORD OnRenderUpdate(DWORD size, void *param);	
	DWORD SetFontImplementation( DWORD size, void * params );
	DWORD OnDestroyRenderer(DWORD size, void *param);
	DWORD OnAddFontGlyphTexFilename(DWORD size, void *param);
	DWORD OnGetFontGlyphTexture(DWORD size, void *param);

private:
	void InitFonts();
	void ClearGlyphTextures();
	SINGLETONCONSTRUCTOROVERRIDE( CFontManager );
	/// \brief The Default Constructor for CFontManager
	/// \return void
	CFontManager();

	//Particle v buffer
	IVertexBufferObject *			m_Dynamic2DVertexBuffer;
	BUFFERALLOCATIONSTRUCT			m_Dynamic2DVertexBufferAllocation;

	IIndexBuffer *					m_Dynamic2DIndexBuffer;
	BUFFERALLOCATIONSTRUCT			m_Dynamic2DIndexBufferAllocation;

	set< IFontObject * >			m_FontObjectList;
	UINT							m_FontVertexSize;
	IFontImplementation *			m_FontImplementation;

	GLYPHTEXTUREMAP m_FontGlyphTextureMap;
};

#endif