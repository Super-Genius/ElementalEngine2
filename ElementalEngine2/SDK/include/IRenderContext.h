///============================================================================
/// \file	IRenderContext.h
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

#ifndef _IRENDERCONTEXT_H
#define _IRENDERCONTEXT_H
#include "IComponent.hpp"

namespace ElementalEngine 
{

/// IRenderContext
/// Interface for render contexts
class IRenderContext
{
public:
	/// Destructor
	virtual ~IRenderContext() {};

	/// Gets the component type of the concrete implementation of this class
	/// \return - pointer to IHashString of the string
	virtual IHashString* GetComponentType( void ) = 0;

	/// Creats a render context
	/// \param window - handle to a window
	/// \param width - int of the width of the render context
	/// \param height - int of the height of the render context
	/// \param depthbits - int of the depthbits of the render context
	/// \param colorbits - int of the colorbits of the render context
	virtual void CreateContext( HWND window, const int width, const int height, const int depthbits, const int colorbits ) = 0;

	/// Sets the background (clear) color
	/// \param r - unsigned int r component
	/// \param g - unsigned int g component
	/// \param b - unsigned int b component
	/// \param a - unsigned int a component (default = 255)
	virtual void SetBGColor( UINT r, UINT g, UINT b, UINT a = 255 ) = 0;

	/// Gets the background (clear) color
	/// \param r - pointer to unsigned int r component
	/// \param g - pointer to unsigned int g component
	/// \param b - pointer to unsigned int b component
	/// \param a - pointer to unsigned int a component
	virtual void GetBGColor( UINT* r, UINT* g, UINT* b, UINT* a ) = 0;

	/// Destroy the Render Context
	virtual void DestroyContext( void ) = 0;
};

}	//namespace ElementalEngine 

#endif // #ifndef _IRENDERCONTEXT_H