///============================================================================
/// \file	CSpriteManager.h
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

#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include "CManager.h"

#define MAX_SPRITE_ELEMENTS 32768 //16384

struct SPRITEDRAWENTRY
{
	DWORD vbindex;
	UINT	offset;
	UINT	size;
	IBaseTextureObject * texture;
	IEffect			* effect;
};

struct VBFILLINFO
{
	int offset;
	int size;
	IVertexBufferObject * vb;
	VBFILLINFO(){
		offset = 0;
		size = 0;
		vb = 0;
	}
};

typedef map< DWORD, VBFILLINFO  > VBVERTTYPEMAPPING;
class CSpriteManager : public CManager
{
public:
	static void RenderCallBack();

public:

	/// \brief The Default Destructor for CSpriteManager
	/// \return void
	~CSpriteManager( );

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
	
	DWORD OnAddSprite(DWORD size, void *params);
	
private:
	SINGLETONCONSTRUCTOROVERRIDE( CSpriteManager );
	/// \brief The Default Constructor for CSpriteManager
	/// \return void
	CSpriteManager();
	VBVERTTYPEMAPPING m_VBs; 
	vector< SPRITEDRAWENTRY > m_DrawEntries;
	bool	m_bLastFrameRendered;

};

#endif //#ifndef SPRITEMANAGER_H