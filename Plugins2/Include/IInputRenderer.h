///============================================================================
/// \file		IInputRenderer.h
/// \brief		Definition file for Input Renderer System
/// \date		05-21-2009
/// \author		Marivaldo Cabral
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

#ifndef IINPUTRENDERER_H__
#define IINPUTRENDERER_H__


/// \class IInputRenderer
/// \brief Interface bridge to Ogre and EE input renderer for Wii
class IInputRenderer
{

public:

	/// Default Destructor
	virtual ~IInputRenderer() {}

	/// Start Interface Functions ///
	/// \brief Initializes the Renderer
	/// \return bool = 
	virtual bool Initialize() = 0;

	/// \brief This DeInitializes the renderer
	virtual void DeInitialize()= 0;

	/// This function calls pre-draw routines before calling state calls and such
	/// \param defaultBufferRender - true if this is the default buffer to render
	/// \param pOverrideRenderTarget - pointer to ITextureObject interface to override location of render (default = NULL)
	virtual void BeginScene( bool defaultBufferRender, ITextureObject *pOverrideRenderTarget = NULL )= 0;

	/// \brief This function does post-draw and swapping of buffers if applicable
	virtual void EndScene()= 0;

		/// Sets the viewport of the renderer
	/// \param xpos - int indicating x location
	/// \param ypos - int indicating y location
	/// \param width - int indicating the width
	/// \param height - int indicating the height
	virtual void SetViewport( const int xpos, const int ypos, const int width, const int height )= 0;

	/// \brief Draws a bounding box
	/// \param in_min = the minimum worldspace position of the bbox
	/// \param in_max = the maximum worldspace positino of the bbox
	/// \param colorRGBA = the color values 0-255
	virtual void DrawBoundingBox( Vec3 &in_min, Vec3 &in_max, int colorR, int colorG, int colorB, int colorA )= 0;

		/// Draws an axis
	/// \param in_length = the length in screen space of an axis line
	/// \param in_rot = the rotation matrix to orient the axis
	/// \param in_pos = the position of the axis
	/// \param colorXR = red component of color of the X axis line
	/// \param colorXG = green component of color of the X axis line
	/// \param colorXB = blue component of color of the X axis line
	/// \param colorYR = red component of color of the Y axis line
	/// \param colorYG = green component of color of the Y axis line
	/// \param colorYB = blue component of color of the Y axis line
	/// \param colorZR = red component of color of the Z axis line
	/// \param colorZG = green component of color of the Z axis line
	/// \param colorZB = blue component of color of the Z axis line
	virtual void DrawAxis( float &in_length, Matrix3x3 &in_rot, Vec3 &in_pos, int colorXR = 255, int colorXG = 0, int colorXB = 0, int colorYR = 0, int colorYG = 255, int colorYB = 0, int colorZR = 0, int colorZG = 0, int colorZB = 255 )= 0;

	/// Draws a sphere
	/// \param position = the position of the sphere
	/// \param radius = the radius of the sphere
	/// \param colorR = red component of color
	/// \param colorG = green component of color
	/// \param colorB = blue component of color
	/// \param iResolution = the resolution with which to render the sphere (default = 4)
	virtual void DrawSphere( Vec3 &position, float radius, int colorR, int colorG, int colorB, int iResolution=4 )= 0;

	/// Draws a 2D mask
	/// \param xpos - float of the x position
	/// \param ypos - float of the y position
	/// \param width - float of the width of rectangular mask
	/// \param height - float of the height of rectangular mask
	/// \param texture - ststring of the texture name
	virtual void Draw2DMask(const float xpos, const float ypos, const float width, const float height, const StdString &texture)= 0;

	/// Draws a 2d line
	/// \param startX - start of the line (x coord)
	/// \param startY - start of the line (y coord)
	/// \param endX - end of the line (x coord)
	/// \param endY - end of the line (y coord)
	/// \param r - red component of the line color
	/// \param g - green component of the line color
	/// \param b - blue component of the line color
	/// \param width - width of the line
	virtual void Draw2DLine(const float startX, const float startY, const float endX, const float endY, const int r, const int g, const int b, const float width)= 0;

	/// Draws a 2D Quad
	/// \param x - float specifiying the left position
	/// \param y - float specifiying the top position
	/// \param width - float specifiying the width
	/// \param height - float specifiying the height
	/// \param texture - pointer to IBaseTextureObject interface
	/// \param color - DWORD specifying the color (default = 0xFFFFFFFF)
	/// \param angle - float giving rotational angle
	/// \param uStart - float specifying the U component of the first texture coord (default = 0.0)
	/// \param vStart - float specifying the V component of the first texture coord (default = 0.0)
	/// \param uEnd - float specifying the U component of the second texture coord (default = 1.0)
	/// \param vEnd - float specifying the V component of the second texture coord (default = 1.0)
	/// \return - true on success
	virtual bool Draw2DQuad( float x, float y, float width, float height, 
		IBaseTextureObject * texture, DWORD color = 0xFFFFFFFF, float angle=0.f,
		float uStart = 0.0f, float vStart = 0.0f, float uEnd = 1.0f, float vEnd = 1.0f)= 0;

	/// Draws a 3d line
	/// \param startX - start of the line (x coord)
	/// \param startY - start of the line (y coord)
	/// \param startZ - start of the line (z coord)
	/// \param endX - end of the line (x coord)
	/// \param endY - end of the line (y coord)
	/// \param endZ - end of the line (z coord)
	/// \param r - red component of the line color
	/// \param g - green component of the line color
	/// \param b - blue component of the line color
	/// \param width - width of the line
	/// \param bOverrideTransform - bool indicating to override transform (default = true)
	virtual void Draw3DLine(const float startX, const float startY, const float startZ, const float endX, const float endY, const float endZ, const int r, const int g, const int b, const float width, bool bOverrideTransform=true)= 0;
	
	/// Draw a 3D point
	/// \param xpos - the x coordinate position
	/// \param ypos - the y coordinate position
	/// \param zpos - the z coordinate position
	/// \param r - red component of the line color
	/// \param g - green component of the line color
	/// \param b - blue component of the line color
	/// \param size - size of the 3d point
	virtual void DrawPoint(const float xpos, const float ypos, const float zpos, const int r, const int g, const int b, const float size)= 0;

	/// Draws a quad NOTE: must set textures before this
	/// \param verts - void pointer to vertex data
	/// \param channels - vertex stream channel descriptor
	/// \return - true on success
	virtual bool DrawQuad( void * verts, CHANNELDESCRIPTORLIST &channels )= 0;



};

#endif /// IINPUTRENDERER_H__