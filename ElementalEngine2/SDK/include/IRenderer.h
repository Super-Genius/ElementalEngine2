///============================================================================
/// \file	IRenderer.h
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

#ifndef _IRENDERER_H
#define _IRENDERER_H

namespace ElementalEngine
{
class IRenderContext;
class IRenderer;
class ITextureObject;
class IBaseTextureObject;
class IVertexBufferObject;
class IIndexBuffer;
class IEffect;
class IShader;
class IConfig;
class ILightObject;
class IMaterial;

/// COLORCORRECTION
/// struct for adjusting brightness, contrast, and gamma
struct COLORCORRECTION
{
	COLORCORRECTION()
	{
		fBrightness = 0.5;
		fContrast = 0.5;
		fGamma = 1.0;
	}
	/// brightness - [0,1] 0.5 = normal
	float fBrightness;
	/// contrast - [0,1] 0.5 = normal
	float fContrast;
	/// gamma - 1.0 = normal (exponent)
	float fGamma;
};

/// RENDERERPROFILEINFO
/// performance info on framerate, triangles-per-frame, and texture memory
typedef struct RENDERERPROFILEINFO
{
	float FrameRate;
	int TrianglesPerFrame;
	int UsedTextureMemory;

}RENDERERPROFILEINFO;

/// GETRENDERERINTERFACEPARAMS
/// used to get RendererInterfaces
typedef struct GETRENDERERINTERFACEPARAMS
{
	IRenderer *m_RendererInterface;
	GETRENDERERINTERFACEPARAMS()
	{
		m_RendererInterface = NULL;
	}
}GETRENDERERINTERFACEPARAMS;

/// CONFIGLIST
/// List of pointers to IConfig interfaces
typedef list<IConfig *> CONFIGLIST;

/// NAMELIST
/// list of pointers to IHashString interfaces representing hash names
typedef list<IHashString *> NAMELIST;

/// MATRIXMODE - enumeration of the possible matrix modes
enum MATRIXMODE{ PROJECTION_MATRIX, VIEW_MATRIX, WORLD_MATRIX };

/// BLENDMODE - enumeration of the types of blend modes
enum BLENDMODE{ BLEND_NORMAL = 0, BLEND_ADDITIVE };

/// DRAWPRIMITIVETYPE - enumeration of the draw primitive types
enum DRAWPRIMITIVETYPE{ DP_TRIANGLELIST, DP_TRIANGLEFAN, DP_TRIANGLESTRIP, DP_POINTLIST, DP_LINELIST };

/// IRenderer
/// Interface for a Renderer (Inherits from IComponent)
class IRenderer : public IComponent
{
public:
	// virtual destructor for RTTI and override for derived classes
	virtual ~IRenderer() {};
	// Messages:
	/// Message that must be implemented that passes back an IRenderer inteface
	/// to the delivering struct.
	/// \param size - size of the message
	/// \param params - void pointer to the data of the message
	virtual DWORD OnGetRendererInterface( DWORD size, void* params) = 0;	
	
	/// Sets the working directories
	/// \param size - size of the message
	/// \param params - void pointer to the data of the message
	virtual DWORD OnSetDirectories( DWORD size, void* params ) = 0;

    /// Initializes the Renderer
	/// \param window = The handle to the window to be associated with the render context
	/// \param width = the width of the frame buffer
	/// \param height = the height of the frame buffer
	/// \param depthbits = the number of bits for depth
	/// \param colorbits = the number of bits for color
	/// \return bool = true on success
	virtual bool Initialize( HWND window, bool fullscreen, const int width, const int height, const int depthbits, const int colorbits ) = 0;

	/// This enables drawing to PBuffers before blitting to the screen
	/// \param width = the width of the PBuffer(must be a power of 2, usually 512)
	/// \param height = the height of the PBuffer(must be a power of 2, usually 512)
	virtual void UsePBuffers( const int width, const int height ) = 0;

	/// This DeInitializes the renderer
	virtual void DeInitialize( void ) = 0;

	/// Resets the renderer, must be reinitialized
	virtual void Reset( void ) = 0;

	/// Resizes the video screen
	/// \param width = the width of the screen
	/// \param height = the height of the screen
	virtual void ResizeScreen( const int& width, const int& height ) = 0;

	/// This function calls pre-draw routines before calling state calls and such
	/// \param defaultBufferRender - true if this is the default buffer to render
	/// \param pOverrideRenderTarget - pointer to ITextureObject interface to override location of render (default = NULL)
	virtual void BeginScene( bool defaultBufferRender, ITextureObject* pOverrideRenderTarget=NULL ) = 0;

	/// This function does post-draw and swapping of buffers if applicable
	virtual void EndScene( void ) = 0;

	/// This requires a PBuffer to be active, Blits the current PBuffer to the context passed in
	/// \param context = The IRenderContext interface returned by CreateNewContext
	/// \return bool = True if the render succeeded, false otherwise
	virtual bool RenderToContext( IRenderContext* context ) = 0;

	/// Creates a new render context and returns the interface. The concrete class depends on
	/// the implementation of the engine. The application does not need to know about the concrete class
	/// so long as an IRenderContext interface is returned.
	/// \param window = The handle to the window to be associated with the render context
	/// \param width = the width of the frame buffer
	/// \param height = the height of the frame buffer
	/// \param depthbits = the number of bits for depth
	/// \param colorbits = the number of bits for color
	/// \return IRenderContext* = The created render context or NULL if the renderer couldnt' create one
	virtual IRenderContext* CreateNewContext( HWND window, const int width, const int height, const int depthbits, const int colorbits ) = 0;

	/// Sets the dimension of the window
	/// \param width - int indicating the width
	/// \param height - int indicating the height
	virtual void SetWindowDimensions( const int width, const int height ) = 0;

	/// Sets the viewport of the renderer
	/// \param xpos - int indicating x location
	/// \param ypos - int indicating y location
	/// \param width - int indicating the width
	/// \param height - int indicating the height
	virtual void SetViewport( const int xpos, const int ypos, const int width, const int height ) = 0;

	/// Draws an axis aligned bounding box
	/// \param in_min = the minimum worldspace position of the bbox
	/// \param in_max = the maximum worldspace positino of the bbox
	/// \param colorRGBA = the color values 0-255
	virtual void DrawBoundingBox( Vec3& in_min, Vec3& in_max, int colorR = 255, int colorG = 255 , int colorB = 255, int colorA = 200) = 0;

	/// Draws an oriented bounding box
	/// \param in_min = the minimum worldspace position of the bbox
	/// \param in_max = the maximum worldspace positino of the bbox
	/// \param in_rot = the rotation matrix to orient the box
	/// \param in_pos = the position of the bounding box
	/// \param colorRGBA = the color values 0-255
	virtual void DrawOrientedBoundingBox( Vec3& in_min, Vec3& in_max, Matrix3x3& in_rot, Vec3& in_pos, int colorR = 255, int colorG = 255 , int colorB = 255 ) = 0;

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
	virtual void DrawAxis( float &in_length, Matrix3x3 &in_rot, Vec3 &in_pos, int colorXR = 255, int colorXG = 0, int colorXB = 0, int colorYR = 0, int colorYG = 255, int colorYB = 0, int colorZR = 0, int colorZG = 0, int colorZB = 255 ) = 0;

	/// Draws a sphere
	/// \param position = the position of the sphere
	/// \param radius = the radius of the sphere
	/// \param colorR = red component of color
	/// \param colorG = green component of color
	/// \param colorB = blue component of color
	/// \param iResolution = the resolution with which to render the sphere (default = 4)
	virtual void DrawSphere( Vec3 &position, float radius, int colorR, int colorG, int colorB, int iResolution=4 ) = 0;

	/// Draws a 2D mask
	/// \param xpos - float of the x position
	/// \param ypos - float of the y position
	/// \param width - float of the width of rectangular mask
	/// \param height - float of the height of rectangular mask
	/// \param texture - ststring of the texture name
	virtual void Draw2DMask(const float xpos, const float ypos, const float width, const float height, const StdString &texture) = 0;

	/// Draws a 2d line
	/// \param startX - start of the line (x coord)
	/// \param startY - start of the line (y coord)
	/// \param endX - end of the line (x coord)
	/// \param endY - end of the line (y coord)
	/// \param r - red component of the line color
	/// \param g - green component of the line color
	/// \param b - blue component of the line color
	/// \param width - width of the line
	virtual void Draw2DLine(const float startX, const float startY, const float endX, const float endY, const int r, const int g, const int b, const float width) = 0;

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
		float uStart = 0.0f, float vStart = 0.0f, float uEnd = 1.0f, float vEnd = 1.0f) = 0;

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
	virtual void Draw3DLine(const float startX, const float startY, const float startZ, const float endX, const float endY, const float endZ, const int r, const int g, const int b, const float width, bool bOverrideTransform=true) = 0;
	
	/// Draw a 3D point
	/// \param xpos - the x coordinate position
	/// \param ypos - the y coordinate position
	/// \param zpos - the z coordinate position
	/// \param r - red component of the line color
	/// \param g - green component of the line color
	/// \param b - blue component of the line color
	/// \param size - size of the 3d point
	virtual void DrawPoint(const float xpos, const float ypos, const float zpos, const int r, const int g, const int b, const float size) = 0;

	/// Sets a texture for the current pixel shader
	/// \param stage - unsigned int of the texture stage
	/// \param texture - pointer to IBaseTextureObject interface
	/// \return - true on success
	virtual bool SetTexture( UINT stage, IBaseTextureObject * texture ) = 0;

	/// Sets a texture for the current vertex shader
	/// \param stage - unsigned int of the texture stage
	/// \param texture - pointer to IBaseTextureObject interface
	/// \return - true on success
	virtual bool SetVertexTexture( UINT stage, IBaseTextureObject * texture ) = 0;

	/// Sets the material
	/// \param pass - unsigned int of the render pass
	/// \param mat - pointer to IMaterial interface
	/// \return - true on success
	virtual bool SetMaterial( UINT pass, IMaterial * mat ) = 0;

	/// Draws a quad NOTE: must set textures before this
	/// \param verts - void pointer to vertex data
	/// \param channels - vertex stream channel descriptor
	/// \return - true on success
	virtual bool DrawQuad( void * verts, CHANNELDESCRIPTORLIST &channels ) = 0;

	/// Sets the matrix to a given value
	/// \param matmode = the matrix mode to set, either PROJECTION_MATRIX or MODELVIEW_MATRIX
	/// \param pMat = a pointer to a float[16] array holding the values of the matrix
	///				 in OGL's row major order
	virtual void SetMatrix( const MATRIXMODE matmode, const float * pMat )=0;

	/// Pushes the matrix on the stack
	/// \param matmode = the matrix mode to set, either PROJECTION_MATRIX or MODELVIEW_MATRIX
	virtual void PushMatrix( const MATRIXMODE matmode )=0;

	/// Pops the matrix off the stack
	/// \param matmode = the matrix mode to set, either PROJECTION_MATRIX or MODELVIEW_MATRIX
	virtual void PopMatrix( const MATRIXMODE matmode )=0;

	/// Gets the current matrix
	/// \param matmode = the matrix mode to set, either PROJECTION_MATRIX or MODELVIEW_MATRIX
	/// \param pMat - pointer to array of floats to be filled in with matrix data
	virtual void GetMatrix( const MATRIXMODE matmode, float * pMat ) = 0;

	/// Determines if the renderer is initialized
	/// \return - true if the renderer is initalized
	virtual bool IsInitialized() = 0;
	
	/// Transforms a screen pointer into world space
	/// \param mouseX - float position in screen space X value
	/// \param mouseY - float position in screen space Y value
	/// \param Pos - Vec3 to be filled in with world space position
	virtual void TransformScreenToWorld( float mouseX, float mouseY, Vec3 &Pos ) = 0;

	/// Transforms a world space point into screen space
	/// \param Pos - Vec3 position in world space
	/// \param mouseX - translated X int position
	/// \param mouseY - translated Y int position
	/// \param screenZ - translatez Z float postion
	/// \param cameraName - pointer to IHashString of the camera name
	virtual void TransformWorldToScreen( const Vec3 &Pos, int &mouseX, int &mouseY, float &screenZ, IHashString *cameraName ) = 0;
	
	/// Gets the dimensions of the window
	/// \param width - int width of the window
	/// \param height - int height of the window
	virtual void GetWindowDimensions( int &width, int &height ) = 0;

	/// Gets the dimensions of the view
	/// \param width - int width of the view
	/// \param height - int height of the view
	virtual void GetViewDimensions( int &width, int &height ) = 0;

	/// Gets the dimensions of the back buffer
	/// \param width - int width of the back buffer
	/// \param height - int height of the back buffer
	virtual void GetBackBufferDimensions( int &width, int &height ) = 0;
	
	/// Causes Begin() and End() to render to a texture
	/// \param pTexture = the texture you wish to render to, set to NULL to disable
	/// \return bool = true if the texture could be set as a render target
	virtual bool RenderToTexture( ITextureObject * pTexture ) = 0;

	/// Switches to full screen mode or windowed mode
	virtual void SetFullScreen( void ) = 0;

	/// Gets the profiling information from the renderer, such as framerate, etc	
	/// \param info = The information is retrned in this pointer
	virtual void GetFrameProfileInfo( RENDERERPROFILEINFO* info ) = 0;

	/// Set the background color
	/// \param uRed - unsigned int red component
	/// \param uGreen - unsigned int green component
	/// \param uBlue - unsigned int blue component
	/// \param Alpha - unsigned int alpha component (default = 255)
	virtual void SetBackgroundColor(UINT uRed, UINT uGreen, UINT uBlue, UINT Alpha = 255) = 0;

	/// clears the screen depth buffer and/or rendertarget
	/// \param clearDepth - bool indicating to clear the depth buffer
	/// \param clearTaget - bool indicating to clear the rendertarget
	virtual void ClearScreen( bool clearDepth, bool clearTarget ) = 0;

	/// low level z call
	/// \param fScale - float scale of the z value
	/// \param fOffset - float offset of the z value
	virtual void SetPolygonZBias( const float fScale, const float fOffset )= 0;

	/// enables and disables depth testing
	/// \param state - bool indicating to enable (true) or disable (false)
	virtual void SetDepthTest( bool state ) = 0;

	/// enables and disables depth writes
	/// \param state - bool indicating to enable (true) or disable (false)
	virtual void SetDepthWrite( bool state )= 0;

	/// sets the blend mode to either normal or additive
	/// \param mode - enum indicating the blend mode to set
	virtual void SetBlendMode( BLENDMODE mode ) = 0;

	/// sets the color mask so we don't write a certain channel
	/// \param r - bool indicating to write to the r channel (true)
	/// \param g - bool indicating to write to the g channel (true)
	/// \param b - bool indicating to write to the b channel (true)
	/// \param a - bool indicating to write to the a channel (true)
	virtual void SetColorMask( bool r, bool g, bool b, bool a ) = 0;
	
	/// Gets the color encoding (0 to 1)
	/// \param a - float alpha compoenent 
	/// \param r - float red compoenent
	/// \param g - float green compoenent
	/// \param b - float blue compoenent
	/// \returns - DWORD of the encoded value
	virtual DWORD GetColorARGB( float a, float r, float g, float b ) = 0;

	/// Gets color encoding given 0-255 values
	/// \param a - int alpha compoenent 
	/// \param r - int red compoenent
	/// \param g - int green compoenent
	/// \param b - int blue compoenent
	/// \returns - DWORD of the encoded value
	virtual DWORD GetColorARGB( int a, int r, int g, int b ) = 0;

	/// enables and disables alpha testing
	/// \param state - bool indicating to enable (true) or disable (false)
	virtual void SetAlphaTest( bool state ) = 0;

	/// Capture screenshot
	/// \param filename - pointer to TCHAR of a filename for writing out (default = NULL)
	virtual void CaptureScreen( const TCHAR *filename = NULL ) = 0;

	/// returns a vertex buffer interface based on the type of the renderer
	/// \return IVertexBufferObject * = pointer to created interface or NULL if nothing could be created
	virtual IVertexBufferObject* CreateVertexBuffer( void ) = 0;

	/// destroys a vertex buffer
	/// \param IVertexBufferObject * = pointer to vertex buffer to be destroyed
	virtual void DestroyVertexBuffer( IVertexBufferObject* pVertexBufferObject ) = 0;

	/// returns a vertex buffer interface based on the type of the renderer
	/// \return IVertexBufferObject * = pointer to created interface or NULL if nothing could be created
	virtual IIndexBuffer* CreateIndexBuffer( void ) = 0;

	/// destroys a index buffer
	/// \param IIndexBufferObject * = pointer to index buffer to be destroyed
	virtual void DestroyIndexBuffer( IIndexBuffer* pIndexBufferObject ) = 0;

	/// Render a vertex buffer with texture and shader
	/// \param vb - pointer to IVertexBufferObject interface
	/// \param offset - unsigned int indicating offset into vertex buffer
	/// \param numElements - unsigned int indicating number of vertex to render
	/// \param primtype - enum of the primtive type (default = DP_TRIANGLELIST)                                                 
	/// \return - true on success
	virtual bool RenderVertexBuffer( IVertexBufferObject* vb, const UINT offset, const UINT numElements, 
		 const DRAWPRIMITIVETYPE primtype = DP_TRIANGLELIST) = 0;

	/// Render a vertex buffer with an index buffer a texture and shader
	/// \param ib - pointer to IIndexBuffer interface
	/// \param vb - pointer to IVertexBufferObject interface
	/// \param offset - unsigned int indicating offset into vertex buffer
	/// \param numElements - unsigned int indicating number of elements to render
	/// \param primtype - enum of the primtive type (default = DP_TRIANGLELIST)                                                 
	/// \return - true on success
	virtual bool RenderIndexBuffer( IIndexBuffer* ib, IVertexBufferObject* vb,  const UINT offset, const UINT numElements,
		 const DRAWPRIMITIVETYPE primtype = DP_TRIANGLELIST ) = 0;

	/// Render a vertex buffer with an index buffer a texture and shader (supports offsets)
	/// \param ib - pointer to IIndexBuffer interface
	/// \param vb - pointer to IVertexBufferObject interface
	/// \param basevertex - unsigned int of the index of the vertex to start at
	/// \param numverticesused - unsgined int counting the number of vertices
	/// \param offset - unsigned int indicating offset into vertex buffer
	/// \param numElements - unsigned int indicating number of elements to render
	/// \param primtype - enum of the primtive type (default = DP_TRIANGLELIST)                                                 
	/// \return - true on success
	virtual bool RenderIndexBuffer( IIndexBuffer* ib, IVertexBufferObject* vb, 
		const UINT basevertex, const UINT numverticesused,
		const UINT offset, const UINT numElements, 
		const DRAWPRIMITIVETYPE primtype = DP_TRIANGLELIST ) = 0;

	/// Sets a stream source for a vertex buffer
	/// \param stream - unsigned int of the stream id
	/// \param vb - pointer to IVertexBufferObject interface
	/// \param offset - unsigned int indicating offset into vertex buffer
	/// \param stride - unsigned int indicating the stride of the stream
	/// \return - true on success
	virtual bool SetStreamSource( UINT stream, IVertexBufferObject * vb, UINT offset, UINT stride ) = 0;

	/// gets the api device
	/// \return - void pointer of the Device
	virtual void* GetAPIDevice( void ) = 0;

	/// enables or disables wireframe mode
	/// \param val - bool enabling wireframe on true
	virtual void SetWireframeMode( bool val ) = 0;

	/// Get a single configuration parameter
	/// \param name = pointer to hashstring specifying configuration name
	/// \return IConfig * pointer to configuration item
	virtual IConfig* GetConfiguration(IHashString* name) = 0;

	/// Sets the 2d orthographic screen size
	/// if width and height are 0, uses preset back buffer size
	/// \param left - float left coordinate (default = 0)
	/// \param right - float right coordinate (default = 0)
	/// \param bottom - float bottom coordinate (default = 0)
	/// \param top - float top coordinate (default = 0)
	virtual void SetOrtho2DScreenSize(float left=0, float right=0, float bottom=0, float top=0) = 0;
	
	/// Sets a bunch of lights up to be used in the next rendering pass
	/// \param lights - array of pointers to ILightObject interfaces for lights
	/// \param numlights - number of lights
	/// \param detailLights - array of pointers to ILightObject interfaces for detailights
	/// \param numDetailLights - number of detail lights
	/// \param pAmbientLightTerm - pointer to a float of the ambient light value
	virtual void SetLightArray( ILightObject* lights[], int numlights, ILightObject* detailLights[], 
								int numDetailLights, float *pAmbientLightTerm )  = 0;

	/// enables scissor clipping
	/// \param enable - bool enabling the scissor clipping (true)
	virtual void EnableScissorClip( bool enable ) = 0;

	/// sets the scissor clipping rectangle
	/// \param x1 - int x starting coord
	/// \param y1 - int y starting coord
	/// \param x2 - int x ending coord
	/// \param y2 - int y ending coord
	virtual void SetScissorRect(int x1, int y1, int x2, int y2) = 0;

	/// enables or disables PostProcessing
	/// \param enable - bool enabling post process (true)
	virtual void EnablePostProcess( bool bEnable ) = 0;

	/// renders the buffer back to the intermediate buffer
	/// \param eff - pointer to IEffect interface of the effect to use
	/// \param src - pointer to ITextureObject interface of the source buffer (default = NULL)
	/// \param dest - pointer to ITextureObject interface of the destination buffer (default = NULL)
	/// \return - true on success
	virtual bool RenderPostProcessIntermediate( IEffect * eff, ITextureObject * src = NULL, ITextureObject * dest = NULL ) = 0;

	/// renders the buffer to the regular buffer
	/// \param eff - pointer to IEffect interface of the effect to use
	/// \param rendercontext - pointer to IRenderContext interface of destination render target
	/// \return - true on success
	virtual bool RenderPostProcessFinal( IEffect * eff, IRenderContext * rendercontext ) = 0;

	/// renders the buffer to the final screen buffer
	/// \param rendercontext - pointer to IRenderContext interface of destination render target
	/// \return - true on success
	virtual bool Present( IRenderContext* rendercontext ) = 0;

	/// Gets a combination of flags specifying the API type and pixel shader types etc
	/// \return - DWORD of the resulting combination fo flags of API Type, Pixel shader version, etc
	virtual DWORD GetRendererAPIGUID( void ) = 0;

	/// Grab the GUID given an api, shader version and misc flags input
	/// \param api - pointer to TCHAR string of the api
	/// \param shaderversion - pointer to TCHAR string of the shader version
	/// \param misc - pointer to TCHAR string of misc settings
	/// \return - DWORD of the resulting combination fo flags of API Type, Pixel shader version, etc
	virtual DWORD ConstructAPIGUID( const TCHAR * api, const TCHAR * shaderversion, const TCHAR * misc ) = 0;

	/// Sets an effect
	/// \param pass - unsigned int giving the render pass
	/// \param effect - pointer to IEffect interface of the effect to set
	virtual void SetEffect(  UINT pass, IEffect * effect ) = 0;

	/// Converts a DWORD of a hashstring to an ENUMRENDERSTATE
	/// \param uniqueID - DWORD of a CHashString render state
	/// \return - ENUMRENDERSTATE version of the CHashString
	virtual ENUMRENDERSTATE StringToRenderState( DWORD uniqueID ) = 0;

	/// Converts a DWORD of a hashstring to an ENUMRENDERSTATEPARAM
	/// \param uniqueID - DWORD of a CHashString render state param
	/// \return - ENUMRENDERSTATEPARAM version of the CHashString
	virtual ENUMRENDERSTATEPARAM StringToRenderStateParam( DWORD uniqueID ) = 0;

	/// Converts a DWORD of a hashstring to an ENUMSAMPLERSTATE
	/// \param uniqueID - DWORD of a CHashString sampler state
	/// \return - ENUMSAMPLERSTATE version of the CHashString
	virtual ENUMSAMPLERSTATE StringToSamplerState( DWORD uniqueID ) = 0;

	/// Converts a DWORD of a hashstring to an TEXTURESTAGEPARAM
	/// \param uniqueID - DWORD of a CHashString texture stage state
	/// \return - TEXTURESTAGEPARAM version of the CHashString
	virtual TEXTURESTAGEPARAM StringToTextureStageParam( DWORD uniqueID ) = 0;

	/// Sets a sampler state
	/// \param stage - unsigned int of the sampler to set
	/// \param mode - enum ENUMSAMPLERSTATE mode of the sampler to set
	/// \param val - enum TEXTURESTAGEPARAM of the value for the state of the sampler to set
	virtual void SetSamplerState( UINT stage, ENUMSAMPLERSTATE mode, TEXTURESTAGEPARAM val ) = 0;

	/// Sets a render state
	/// \param mode - enum ENUMRENDERSTATE mode of the render state to set
	/// \param val - unsigned int of the value to set for the render state
	virtual void SetRenderState( ENUMRENDERSTATE mode, UINT val )= 0;

	/// pushes a render state on the stack
	/// \param mode - enum ENUMRENDERSTATE mode of the render state to set
	/// \param val - unsigned int of the value to set for the render state
	virtual void PushRenderState( ENUMRENDERSTATE mode, UINT val ) = 0;

	/// pops a render state on the stack
	/// \param mode - enum ENUMRENDERSTATE mode of the render state to set
	virtual void PopRenderState( ENUMRENDERSTATE mode ) = 0;

	/// sets the texture as the depth texture, must have compatible format
	/// \param texture - pointer to IBaseTextureObject interface
	/// \return - true on success
	virtual bool SetDepthTarget( IBaseTextureObject* texture ) = 0;
	
	/// copies the current depth buffer into a texture
	/// \param texture - pointer to IBaseTextureObject interface
	/// \return - true on success
	virtual bool CopyDepthTarget( IBaseTextureObject* texture ) = 0;

	/// Apply a different render targets
	/// \param tex - pointer to ITextureObject interface
	/// \param target - unsigned int of the render target (default = 0)
	virtual void ApplyRenderTarget( ITextureObject* tex, UINT target = 0) = 0;

	/// Unapply a render targets
	/// \param tex - pointer to ITextureObject interface
	/// \param target - unsigned int of the render target (default = 0)
	virtual void UnApplyRenderTarget( ITextureObject* tex, UINT target = 0) = 0;

	/// Resets the Backbuffer
	virtual void ResetBackBuffer( void ) = 0;

	/// Set color correction for rgb to the same brightness, contrast and gamma
	/// \param fBrightness - float of brightness amount
	/// \param fContrast - float of contrast amount
	/// \param fGamma - float of gamma amount
	virtual void SetColorCorrection( float fBrightness, float fContrast, float fGamma ) = 0;
	
	/// Set color correction for each color channel seperately
	/// \param red - COLORCORRECTION struct for the red channel
	/// \param green - COLORCORRECTION struct for the green channel
	/// \param blue - COLORCORRECTION struct for the blue channel
	virtual void SetColorCorrection( COLORCORRECTION red, COLORCORRECTION green, COLORCORRECTION blue ) = 0;

	/// Builds a perspective matrix
	/// \param fovy - float value field of view in the horizontal direction
	/// \param aspect - float aspect ratio
	/// \param zn - float z-near value
	/// \param zf - float z-far value
	/// \param outMatrix - resulting perspective matrix
	virtual void BuildPerspectiveMatrix( float fovy, float aspect, float zn, float zf, Matrix4x4 &outMatrix ) = 0;

	/// Builds a lookat matrix
	/// \param eye - vec3 eye position
	/// \param at - vec3 look-at position
	/// \param up - vec3 up vector
	/// \param outMatrix - resulting look-at matrix
	virtual void BuildLookAtMatrix( const Vec3 &eye, const Vec3 &at, const Vec3 &up, Matrix4x4 &outMatrix ) = 0;
	
	/// Builds a ortho matrix
	/// \param l - float min x value
	/// \param r - float max x value
	/// \param b - float min y value
	/// \param t - float max y value
	/// \param zn - float z-near value
	/// \param zf - float z-far value
	/// \param outMatrix - resulting ortho matrix
	virtual void BuildOrthoMatrix( float l, float r, float b, float t, float zn, float zf, Matrix4x4 &outMatrix ) = 0;

	/// Sets a Vertex Shader constant
	/// \param reg - dword register number to start at
	/// \param data - pointer to an array of float data
	/// \param count - unsigned int of the number of vectors (4 floats each) to write out
	virtual void SetVertexShaderConstant( DWORD reg, const float *data, UINT count ) = 0;

	/// Sets a Pixel Shader constant
	/// \param reg - dword register number to start at
	/// \param data - pointer to an array of float data
	/// \param count - unsigned int of the number of vectors (4 floats each) to write out
	virtual void SetPixelShaderConstant( DWORD reg, const float *data, UINT count ) = 0;
};

}	//namespace ElementalEngine

#endif
