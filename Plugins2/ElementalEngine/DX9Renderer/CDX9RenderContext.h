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

#ifndef CDX9RENDERCONTEX_H
#define CDX9RENDERCONTEX_H
#include "ElementalEngine.hpp"

#define MAX_BUFFERED_CHANNELS 16

namespace ElementalEngine 
{

class CDX9RenderContext : public IRenderContext
{
public:
	HWND		m_hWnd;
	int			m_Width;
	int			m_Height;
	int			m_Depthbits;
	int			m_Colorbits;
	UINT		m_ClearColor[4];
	IToolBox *	m_ToolBox;

	UINT m_CurrentRenderState[RENDERSTATE_COUNT];
	UINT m_CurrentTextureStageState[ MAX_BUFFERED_CHANNELS ][TEXTURESTAGESTATE_COUNT];
	UINT m_CurrentSamplerState[ MAX_BUFFERED_CHANNELS ][SAMPLERSTATE_COUNT];

public:
	CDX9RenderContext();
	virtual IHashString * GetComponentType();	
	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);
	virtual void CreateContext( HWND window, const int width, const int height, const int depthbits, const int colorbits );
	virtual void SetBGColor( UINT r, UINT g, UINT b, UINT a );
	virtual void GetBGColor( UINT *r, UINT *g, UINT *b, UINT *a );
	virtual void DestroyContext();
};

}	//namespace ElementalEngine 

#endif