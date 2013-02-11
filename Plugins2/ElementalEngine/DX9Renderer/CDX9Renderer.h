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

#ifndef _CDX9Renderer_H
#define _CDX9Renderer_H
#include "IRenderer.h"
#include "CDX9VertexBufferObject.h"

#define CDX9RENDERER_DEFAULT_2DBUFFER_ALLOCATION 20000
#define MAX_BUFFERED_CHANNELS 16
#define INVALID_API_GUID	0xffffffff

namespace ElementalEngine
{
class CDX9IndexBuffer;
class CDX9VertexBufferObject;
class CDX9RenderContext;

#define CDX9RENDERER_2DVERTEX_FVF ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

struct CDX9Renderer_VertexRHW
{
	float		pos[3]; //because of the Radion x300's hardware limitations(doesn't do pretransformed vertices)
    D3DCOLOR	 color;
	float		tu,tv;
};

struct CDX9Renderer_ShaderConstant
{
	Vec4	vValue;
	bool	bChanged;

	CDX9Renderer_ShaderConstant()
	{
		vValue.Set( 0.0f, 0.0f, 0.0f, 1.0f );
		bChanged = false;
	}
};

typedef stack<DWORD> CHANGEDSHADERCONSTANTSTACK;

typedef map< CHANNELDESCRIPTORLIST, LPDIRECT3DVERTEXDECLARATION9,	VERTEXDESCRIPTIONCOMPARE > VERTEXDECLARATIONMAP;
typedef pair< CHANNELDESCRIPTORLIST, LPDIRECT3DVERTEXDECLARATION9 > VERTEXDECLARATIONMAP_PAIR;
typedef map< CHANNELDESCRIPTORLIST, DWORD, VERTEXDESCRIPTIONCOMPARE > VERTEXFVFMAP;
typedef pair< CHANNELDESCRIPTORLIST, DWORD > VERTEXFVFMAP_PAIR;
typedef map<DWORD, CConfig> CONFIGNAMEMAP;

typedef stack<UINT> STATEVALUESTACK;
typedef map<ENUMRENDERSTATE, STATEVALUESTACK> STATESTACKMAP;

typedef WORD D3DCOLORTABLETYPE;

/// \brief D3D9 Adapter for the IRenderer class
class CDX9Renderer : public IRenderer
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CDX9Renderer );

public:
	~CDX9Renderer();
	
	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( int nArgs, va_list argptr );

	void		Serialize(IArchive &ar){}

	IHashString * GetComponentType(void);

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	/// \brief message that must be implemented that passes back an IRenderer inteface
	/// to the delivering struct.
	DWORD CDX9Renderer::OnGetRendererInterface( DWORD size, void *params);
	/// \brief Sets the working directories
	DWORD OnSetDirectories( DWORD size, void * params );
	/// Convert a mouse coord to a raycast position and direction
	DWORD OnGetRayFromMouseCoords( DWORD size, void * params );
	/// \brief	Convert world coordinates to screen coordinates
	DWORD OnTransformWorldToScreen(DWORD size, void *params);

	/// Start Interface Functions ///
	/// \brief Initializes the Renderer
	/// \brief Initializes the Renderer
	/// \param window = The handle to the window to be associated with the render context
	/// \param width = the width of the frame buffer
	/// \param height = the height of the frame buffer
	/// \param depthbits = the number of bits for depth
	/// \param colorbits = the number of bits for color
	/// \return bool = 
	virtual bool Initialize( HWND window,  bool fullscreen, const int width, const int height, const int depthbits, const int colorbits );
	/// \brief This enables drawing to PBuffers before blitting to the screen
	/// \param width = the width of the PBuffer(must be a power of 2, usually 512)
	/// \param height = the height of the PBuffer(must be a power of 2, usually 512)
	/// \return void
	virtual void UsePBuffers( const int width, const int height );
	/// \brief This DeInitializes the renderer
	virtual void DeInitialize();
	/// \brief Resets the renderer, must be reinitialized
	virtual void Reset();
	/// \brief Resizes the video screen
	/// \param width = the width of the screen
	/// \param height = the height of the screen
	/// \return void
	virtual void ResizeScreen( const int &width, const int &height );
	/// \brief This function calls pre-draw routines before calling state calls and such
	virtual void BeginScene( bool defaultBufferRender, ITextureObject *pOverrideRenderTarget );
	/// \brief This function does post-draw and swapping of buffers if applicable
	virtual void EndScene();
	/// \brief This requires a PBuffer to be active, Blits the current PBuffer to the context passed in
	/// \param context = The IRenderContext interface returned by CreateNewContext
	/// \return bool = True if the render succeeded, false otherwise
	virtual bool RenderToContext( IRenderContext * context );
	/// \brief Creates a new render context and returns the interface. The concrete class depends on
	/// the implementation of the engine. The application does not need to know about the concrete class
	/// so long as an IRenderContext interface is returned.
	/// \param window = The handle to the window to be associated with the render context
	/// \param width = the width of the frame buffer
	/// \param height = the height of the frame buffer
	/// \param depthbits = the number of bits for depth
	/// \param colorbits = the number of bits for color
	/// \return IRenderContext* = The created render context or NULL if the renderer couldnt' create one
	virtual IRenderContext * CreateNewContext( HWND window, const int width, const int height, const int depthbits, const int colorbits );
	//Some low level state calls:
	virtual void SetWindowDimensions( const int width, const int height );
	virtual void SetViewport( const int xpos, const int ypos, const int width, const int height );
	virtual void Draw2DMask(const float xpos, const float ypos, const float width, const float height, const StdString &texture);
	/// Draws a 2d line
	/// \param startX - start of the line (x coord)
	/// \param startY - start of the line (y coord)
	/// \param endX - end of the line (x coord)
	/// \param endY - end of the line (y coord)
	/// \param r - red component of the line color
	/// \param g - green component of the line color
	/// \param b - blue component of the line color
	/// \param width - width of the line
	/// \return void
	virtual void Draw2DLine(const float startX, const float startY, const float endX, const float endY, const int r, const int g, const int b, const float width);
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
	/// \return void
	virtual void Draw3DLine(const float startX, const float startY, const float startZ, const float endX, const float endY, const float endZ, const int r, const int g, const int b, const float width, bool bOverrideTransform);
	/// Draw a 3D point
	/// \param xpos - the x coordinate position
	/// \param ypos - the y coordinate position
	/// \param zpos - the z coordinate position
	/// \param r - red component of the line color
	/// \param g - green component of the line color
	/// \param b - blue component of the line color
	/// \param size - size of the point
	virtual void DrawPoint(const float xpos, const float ypos, const float zpos, const int r, const int g, const int b, const float size);
	/// \brief Draws a bounding box
	/// \param in_min = the minimum worldspace position of the bbox
	/// \param in_max = the maximum worldspace positino of the bbox
	/// \param colorRGBA = the color values 0-255
	virtual void DrawBoundingBox( Vec3 &in_min, Vec3 &in_max, int colorR, int colorG, int colorB, int colorA );
	virtual void DrawOrientedBoundingBox( Vec3 &in_min, Vec3 &in_max, Matrix3x3 &in_rot, Vec3 &in_pos, int colorR, int colorG, int colorB );
	virtual void DrawAxis( float &in_length, Matrix3x3 &in_rot, Vec3 &in_pos, int colorXR, int colorXG, int colorXB , int colorYR, int colorYG, int colorYB, int colorZR, int colorZG, int colorZB );
	/// Draws a sphere
	/// \param position = the position of the sphere
	/// \param radius = the radius of the sphere
	/// \param iResolution = the resolution with which to render the sphere
	virtual void DrawSphere( Vec3 &position, float radius, int colorR, int colorG, int colorB, int iResolution );
	/// \brief Sets the matrix to a given value
	/// \param matmode = the matrix mode to set, either PROJECTION_MATRIX or MODELVIEW_MATRIX
	/// \param pMat = a pointer to a float[16] array holding the values of the matrix
	///				 in OGL's row major order
	virtual void SetMatrix( const MATRIXMODE matmode, const float * pMat );
	virtual void PushMatrix( const MATRIXMODE matmode );
	virtual void PopMatrix( const MATRIXMODE matmode );
	
	virtual bool IsInitialized(){ return m_bInitialized; };
	/// \brief Gets the matrix 
	/// \param matmode = the matrix mode to retrieved, either PROJECTION_MATRIX or MODELVIEW_MATRIX
	/// \param pMat = a pointer to a float[16] array that will hold the values of the matrix
	///				 in OGL's row major order
	virtual void GetMatrix( const MATRIXMODE matmode, float * pMat );
	/// \brief Transforms screen coord(mouse coordinates) to World Coordinates
	/// \param matmode = the matrix mode to retrieved, either PROJECTION_MATRIX or MODELVIEW_MATRIX
	/// \param pMat = a pointer to a float[16] array that will hold the values of the matrix
	///				 in OGL's row major order
	virtual void TransformScreenToWorld( float mouseX, float mouseY, Vec3 &Pos );
	/// \brief Transforms a World Coordinate into screen coordinates
	/// \param pos = the world space coordinate to transform to screen coordinates
	/// \param screenX = the transformed screen X coord
	/// \param screenY = the transformed screen Y coord
	virtual void TransformWorldToScreen( const Vec3 &pos, int &screenX, int &screenY, float &screenZ, IHashString *cameraName );
	virtual void GetWindowDimensions( int &width, int &height );
	virtual void GetViewDimensions( int &width, int &height );
	virtual void GetBackBufferDimensions( int &width, int &height );
	
	/// \brief causes Begin() and End() to render to a texture
	/// \param pTexture = the texture you wish to render to, set to NULL to disable
	/// \return bool = true if the texture could be set as a render target
	virtual bool RenderToTexture( ITextureObject * pTexture );

	/// \brief	Switches to full screen mode or windowed mode
	///			If it is in windowed mode, switch to full screen. Else
	///			if already in full screen mode, switch to windowed mode.
	virtual void SetFullScreen();

	/// \brief Gets the profiling information from the renderer, such as framerate, etc	
	/// \param info = The information is retrned in this pointer
	virtual void GetFrameProfileInfo( RENDERERPROFILEINFO * info );

	// \brief	Set the background color
	virtual void SetBackgroundColor(UINT uRed, UINT uGreen, UINT uBlue, UINT Alpha);
	virtual void ClearScreen( bool clearDepth, bool clearTarget );

	/// \brief low level z call
	virtual void SetPolygonZBias( const float fScale, const float fOffset );

	/// \brief low level z call
	virtual void SetDepthWrite( bool state );

	/// enable depth compare
	virtual void SetDepthTest( bool state );

	/// enable alpha testing
	virtual void SetAlphaTest( bool state );

	///\brief	Capture screenshot
	virtual void CaptureScreen( const TCHAR *filename = NULL );

	/// \brief returns a vertex buffer interface based on the type of the renderer
	/// \return IVertexBufferObject * = pointer to created interface or NULL if nothing could be created
	virtual IVertexBufferObject * CreateVertexBuffer();

	/// \brief destroys a vertex buffer
	/// \param IVertexBufferObject * = pointer to vertex buffer to be destroyed
	virtual void DestroyVertexBuffer( IVertexBufferObject *pVertexBufferObject );

	/// \brief returns a vertex buffer interface based on the type of the renderer
	/// \return IVertexBufferObject * = pointer to created interface or NULL if nothing could be created
	virtual IIndexBuffer * CreateIndexBuffer();

	/// \brief destroys a index buffer
	/// \param IIndexBufferObject * = pointer to index buffer to be destroyed
	virtual void DestroyIndexBuffer( IIndexBuffer *pIndexBufferObject );

	/// \brief Render a vertex buffer with texture and shader
	virtual bool RenderVertexBuffer( IVertexBufferObject * vb,  const UINT offset, const UINT numElements,
		const DRAWPRIMITIVETYPE primtype = DP_TRIANGLELIST );

	/// \brief Render a vertex buffer with an index buffer a texture and shader
	virtual bool RenderIndexBuffer( IIndexBuffer * ib, IVertexBufferObject * vb,  const UINT offset, const UINT numElements,		
		const DRAWPRIMITIVETYPE primtype = DP_TRIANGLELIST );

	/// \brief this should be the main and only way to draw indexbuffers
	virtual bool RenderIndexBuffer( IIndexBuffer * ib, IVertexBufferObject * vb, 
		const UINT basevertex, const UINT numverticesused,
		const UINT offset, const UINT numElements,
		const DRAWPRIMITIVETYPE primtype = DP_TRIANGLELIST );

	virtual void SetWireframeMode( bool val );
	virtual void SetBlendMode( BLENDMODE mode );
	/// Get a single configuration parameter
	/// \param name = pointer to hashstring specifying configuration name
	/// \return IConfig * pointer to configuration item
	virtual IConfig *GetConfiguration(IHashString *name);

	virtual void * GetAPIDevice(){
		return (void*)m_pDevice;
	}
	virtual bool SetTexture( UINT stage, IBaseTextureObject * texture );
	virtual bool SetVertexTexture( UINT stage, IBaseTextureObject * texture );
	//must set textures before this
	virtual bool DrawQuad( void * verts, CHANNELDESCRIPTORLIST &channels );
	virtual void SetOrtho2DScreenSize(float left=0, float right=0, float bottom=0, float top=0);
	
	virtual bool Draw2DQuad( float x, float y, float width, float height, 
							 IBaseTextureObject * texture, 
							 DWORD color = 0xFFFFFFFF, float angle=0.f,
							 float uStart = 0.0f, float vStart = 0.0f, 
							 float uEnd = 1.0f, float vEnd = 1.0f);

	
	bool Draw2DQuadNoTex( float x, float y,  float width, float height, 
							 DWORD color = 0xFFFFFFFF, float angle=0.f,
							 float uStart = 0.0f, float vStart = 0.0f, 
							 float uEnd = 1.0f, float vEnd = 1.0f);

	bool DrawFullscreenQuad(float left=0, float right=0, float bottom=0, float top=0);

	virtual void SetColorMask( bool r, bool g, bool b, bool a );

	virtual void EnableScissorClip(bool enable);

	virtual void SetScissorRect(int x1, int y1, int x2, int y2);
	/// Sets a bunch of lights up to be used in the next rendering pass
	virtual void SetLightArray( ILightObject * lights[], int numlights, ILightObject * detailLights[], 
		int numDetailLights, float *pAmbientLightTerm );

	/// enables or disables PostProcessing
	virtual void EnablePostProcess( bool bEnable );

	/// renders the buffer back to the intermediate buffer
	virtual bool RenderPostProcessIntermediate( IEffect * effect, ITextureObject * src, ITextureObject * dest );

	/// renders the buffer back to the final buffer
	virtual bool RenderPostProcessFinal( IEffect * effect, IRenderContext * rendercontext);
	
	/// renders the buffer to the final screen buffer
	virtual bool Present( IRenderContext * rendercontext );

	/// Gets the color encoding
	virtual DWORD GetColorARGB( float a, float r, float g, float b );

	/// Gets color encoding given 0-255 values
	virtual DWORD GetColorARGB( int a, int r, int g, int b );

	//DX9 specific
	LPDIRECT3DVERTEXDECLARATION9 CDX9Renderer::GetVertexDescription( CHANNELDESCRIPTORLIST &channels, UINT streammode );
	DWORD GetVertexFVF(  CHANNELDESCRIPTORLIST &channels );
	
	virtual bool SetMaterial(UINT pass,  IMaterial * mat );

	/// Gets a combination of flags specifying the API type and pixel shader types etc
	virtual DWORD GetRendererAPIGUID();

	/// Grab the GUID given an api, shader version and misc flags input
	virtual DWORD ConstructAPIGUID( const TCHAR * api, const TCHAR * shaderversion, const TCHAR * misc );

	/// Sets an effect
	virtual void SetEffect(  UINT pass, IEffect * effect );

	/// Sets the texture's sampler state
	virtual ENUMRENDERSTATE StringToRenderState( DWORD uniqueID );
	virtual ENUMRENDERSTATEPARAM StringToRenderStateParam( DWORD uniqueID );
	virtual ENUMSAMPLERSTATE StringToSamplerState( DWORD uniqueID );
	virtual TEXTURESTAGEPARAM StringToTextureStageParam( DWORD uniqueID );
	virtual void SetSamplerState( UINT stage, ENUMSAMPLERSTATE mode, TEXTURESTAGEPARAM val );

	virtual void SetRenderState( ENUMRENDERSTATE mode, UINT val );
	virtual void PushRenderState( ENUMRENDERSTATE mode, UINT val );
	virtual void PopRenderState( ENUMRENDERSTATE mode );

	virtual bool SetStreamSource( UINT stream, IVertexBufferObject * vb, UINT offset, UINT stride );

	inline void SetPixelShader( LPDIRECT3DPIXELSHADER9 pshader );
	inline void SetVertexShader( LPDIRECT3DVERTEXSHADER9 vshader );
	inline bool SetVertexShaderConstantF( DWORD constant, const float * data, UINT count )
	{
		for( unsigned int i = 0; i < count; i++ )
		{
			if( m_VSConstants[constant+i].bChanged != true )
			{
				m_VSConstants[constant+i].bChanged = true;			
			}
			m_VSConstants[constant+i].vValue.Set( &data[i*4] );
			m_VSConstantsChanged.push( constant+i );
		}
		return true;
	}

	inline bool SetPixelShaderConstantF( DWORD constant, const float * data, UINT count )
	{
		for( unsigned int i = 0; i < count; i++ )
		{
			if( m_PSConstants[constant+i].bChanged != true )
			{
				m_PSConstants[constant+i].bChanged = true;
			}
			m_PSConstants[constant+i].vValue.Set( &data[i*4] );
			m_PSConstantsChanged.push( constant+i );
		}
		return true;
	}

	virtual bool SetDepthTarget( IBaseTextureObject * texture ) ;
	virtual bool CopyDepthTarget( IBaseTextureObject * texture );
	
	virtual void ApplyRenderTarget( ITextureObject * tex, UINT target );
	virtual void UnApplyRenderTarget( ITextureObject * tex, UINT target );
	virtual void ResetBackBuffer()
	{
		//release render target and reset to backbuffer
		LPDIRECT3DSURFACE9 backbuffer;
		m_pDevice->GetBackBuffer( 0,0,D3DBACKBUFFER_TYPE_MONO, &backbuffer );
		m_pDevice->SetRenderTarget( 0, backbuffer );
		backbuffer->Release();
	}
	
	/// Builds a perspective matrix
	virtual void BuildPerspectiveMatrix( float fovy, float aspect, float zn, float zf, Matrix4x4 &outMatrix );
	/// Builds a lookat matrix
	virtual void BuildLookAtMatrix( const Vec3 &eye, const Vec3 &at, const Vec3 &up, Matrix4x4 &outMatrix );
	/// Builds a ortho matrix
	virtual void BuildOrthoMatrix( float l, float r, float b, float t, float zn, float zf, Matrix4x4 &outMatrix );

	virtual void SetVertexShaderConstant( DWORD reg, const float *data, UINT count )
	{
		SetVertexShaderConstantF( reg, data, count );
	}
	virtual void SetPixelShaderConstant( DWORD reg, const float *data, UINT count )
	{
		SetPixelShaderConstantF( reg, data, count );
	}

	void OnDestroyRenderer();
	void UnsetTextures();

private:
	void SetDefaultStates();
	void SetTextureArray( IBaseTextureObject ** textures, const UINT numTextures );
	void SetTextureStates( bool vertexlit );
	void DrawPrimUp(D3DPRIMITIVETYPE prim, UINT numprim, void * stream, UINT stride);

	void FlushShaderConstants( void );
	CHANGEDSHADERCONSTANTSTACK m_VSConstantsChanged;
	CDX9Renderer_ShaderConstant * m_VSConstants;
	CHANGEDSHADERCONSTANTSTACK m_PSConstantsChanged;
	CDX9Renderer_ShaderConstant * m_PSConstants;
	
	//Redundancy and other tests. Might want to make these functions part of the interface
	void SetVertexStream( UINT stream, CDX9VertexBufferObject * cvb );
	void SetIndices( CDX9IndexBuffer * cib );
	void SetFVF( DWORD fvf );
	void SetVertexDeclaration( LPDIRECT3DVERTEXDECLARATION9 vertdecl );
	void BuildConfigMap();
	bool BuildPostProcessBuffer( RENDER_TARGET_TYPE renderTargetType );
	void RemovePostProcessBuffer();
	void RegisterTextureStageEnumHashes();
	HRESULT SetD3DRenderState( ENUMRENDERSTATE state, UINT value );
	HRESULT SetD3DTextureStageState( UINT stage, ENUMTEXTURESTAGESTATE state, UINT value );
	HRESULT SetD3DSamplerStageState( UINT stage, ENUMSAMPLERSTATE state, UINT value );
	void AllocateFullscreenQuadVertexBuffer();

	//For fast state checking if ortho2d screen matrix is set, called by 2Dquad function
	bool m_Ortho2DScreenSizeSet;

	/// \brief The Default Constructor for CRenderManager
	/// \return void
	CDX9Renderer();

	UINT m_CurrentRenderState[RENDERSTATE_COUNT];
	UINT m_CurrentTextureStageState[ MAX_BUFFERED_CHANNELS ][TEXTURESTAGESTATE_COUNT];
	UINT m_CurrentSamplerState[ MAX_BUFFERED_CHANNELS ][SAMPLERSTATE_COUNT];

	float		m_AverageLuminosity;
	float		m_TargetLuminosity;
	float		m_ToneMapTimer;
	bool		m_bInitialized;
	int			m_iWindowWidth;
	int			m_iWindowHeight;
	int			m_Width;
	int			m_Height;
	StdString	m_ExecutableDirectory;
	StdString	m_ResourceDirectory;
	ITextureObject * m_RenderTarget;
	ITextureObject *m_PostProcessBuffer;
	LPDIRECT3DSURFACE9	m_BackBufferDepthSurface;
	CHashString m_hszPostProcessBufferName;

	///DX9
	LPDIRECT3D9			m_pD3D9;
	LPDIRECT3DDEVICE9	m_pDevice;
	D3DPRESENT_PARAMETERS m_presentParameters; // Used to explain to Direct3D how it will present things on the screen	
	
	//Redundancy state checks:
	BYTE * m_VertexStreams[ 8 ];
	BYTE * m_IndexStream;
	LPDIRECT3DPIXELSHADER9			m_PixelShaderSet;
	LPDIRECT3DVERTEXSHADER9			m_VertexShaderSet;
	LPDIRECT3DVERTEXDECLARATION9	m_VertexDeclSet;
	DWORD							m_FVFSet;
	DWORD							m_APIGUID;
	//End State redundancy checks

	UINT   m_ClearColor[ 4 ]; //argb

	//current matrices:
	D3DXMATRIX	m_ProjectionMatrix;
	D3DXMATRIX	m_ModelViewMatrix;
	D3DXMATRIX	m_WorldMatrix;

	//List of used vertex declarations
	VERTEXDECLARATIONMAP	m_VertexDeclarationList;
	//List of used FVF declarations map to channel types
	VERTEXFVFMAP			m_VertexFvFList;
	/// storage for configuration list retreived from DirectX
	CONFIGNAMEMAP m_ConfigNameMap;
	stack< D3DXMATRIX > m_MatrixStack[ 3 ];

	CDX9RenderContext * m_RenderContext;
	map< DWORD,	TEXTURESTAGEPARAM > m_TextureStageStateEnum;
	map< DWORD, ENUMSAMPLERSTATE > m_SamplerStateEnum;
	map< DWORD, ENUMRENDERSTATE > m_RenderStateEnum;
	map< DWORD, ENUMRENDERSTATEPARAM > m_RenderStateParamEnum;

	const static D3DSAMPLERSTATETYPE m_D3DSamplerStateEnum[SAMPLERSTATE_COUNT];
	const static D3DTEXTURESTAGESTATETYPE m_D3DTextureStageStateEnum[TEXTURESTAGESTATE_COUNT];
	const static D3DRENDERSTATETYPE m_D3DRenderStateEnum[RENDERSTATE_COUNT];
	const static UINT m_D3DTextureStageParamEnum[TEXTURESTAGEPARAM_COUNT];
	const static UINT m_D3DRenderStateParamEnum[RENDERSTATEPARAM_COUNT];

	STATESTACKMAP m_RenderStateStackMap;

	ITextureObject *m_pOverrideRenderTarget;

	/// this to make sure we don't set a wireframe renderstate when we want everything rendered in wireframe mode
	bool m_bWireFrameMode;
	IBaseTextureObject ** m_SetTextures;
	UINT m_iMaxTextures;
	IBaseTextureObject ** m_SetVertexTextures;
	LPDIRECT3DSURFACE9	m_LockedRenderBufferSurface;
	bool		RenderBufferSurfaceLocked;
private:
	//internal manager-like variables such as dynamic vertex buffers for drawing 2D elements fast
	CDX9VertexBufferObject *m_FullscreenQuadVertexBuffer;
	BUFFERALLOCATIONSTRUCT m_FullscreenQuadVBAllocation;
	map< IBaseTextureObject *, LPDIRECT3DSURFACE9 > m_TextureSurfs;
	map< int, LPDIRECT3DSURFACE9 > m_OldTargets;

public:
	D3DFORMAT GetD3DDepthBufferFormat( UINT &depthbits );

	// Color Correction
public:
	/// Set color correction for rgb to the same brightness, contrast and gamma
	virtual void SetColorCorrection( float fBrightness, float fContrast, float fGamma );
	/// Set color correction for each color channel seperately
	virtual void SetColorCorrection( COLORCORRECTION red, COLORCORRECTION green, COLORCORRECTION blue );
private:
	/// Builds a table to remap color values (used internally by SetColorCorrection)
	void BuildColorRemappingTable( D3DCOLORTABLETYPE *GammaRamp, float fBrightness, float fContrast, float fGamma );

	// Line Device
	ID3DXLine *m_pD3DXLine;

	IEffect *m_pDefaultEffect;
};

}//end namespace EE
#endif //#ifndef _CDX9Renderer_H
