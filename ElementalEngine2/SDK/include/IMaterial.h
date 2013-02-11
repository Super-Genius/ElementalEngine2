///============================================================================
/// \file	IMaterial.h
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

#ifndef IMATERIAL_H
#define IMATERIAL_H


namespace ElementalEngine
{

class IEffect;
class IMaterial : public IObject
{
public:
	virtual ~IMaterial() {};
	/// Applies this material to the given render object
	/// \param pass - unsigned int of the render pass to use
	/// \param settings - pointer to an ISceneSettings interface
	virtual void ApplyMaterial( UINT pass, ISceneSettings* settings ) = 0;

	/// Un-applies this material to the given render object
	/// \param pass - unsigned int of the render pass to use
	virtual void UnApplyMaterial( UINT pass ) = 0;

	/// Get the number of textures for this material for a given pass
	/// \param pass - unsigned int of the render pass to use
	/// \returns - unsigned int of the number of textures 
	virtual UINT GetNumTextures( UINT pass ) = 0;

	/// Gets an IBaseTextureObject interface for a given texture of this material
	/// \param pass - unsigned int of the render pass to use
	/// \param stage - unsigned int of the stage to use
	/// \return - pointer to IBaseTextureObject interface to a texture
	virtual IBaseTextureObject* GetTexture( UINT pass, UINT stage ) = 0;
	
	/// Gets an IBaseTextureObject interface for a given texture of this material
	/// \param hash - pointer to IHashString name of texture
	/// \return - pointer to IBaseTextureObject interface to a texture
	virtual IBaseTextureObject* GetTexture( IHashString* hash ) = 0;

	/// Gets the texture stage and pass from the hash
	/// \param hash - pointer to IHashString name of texture
	/// \param pass - unsigned int of the pass this texture is used on
	/// \param stage - unsigned int of the stage this texture is used on
	/// \return - true if successfully found the texture
	virtual bool GetTextureChannels( IHashString* hash, UINT &pass, UINT& stage ) = 0;

	/// Sets the texture for a given pass and stage
	/// \param pass - unsigned int of the pass this texture is used on
	/// \param stage - unsigned int of the stage this texture is used on
	/// \param texture - pointer to IBaseTextureObject interface to a texture to be used
	virtual void SetTexture( UINT pass, UINT stage, IBaseTextureObject* texture ) = 0;
	
	/// Sets flags for a texture stage
	/// \param pass - unsigned int indicating the render pass
	/// \param stage - unsigned int indicating the texture stage
	/// \param mode - enum of sampler state mode
	/// \param value - texture stage setting value
	virtual void SetTextureStageFlags( UINT pass, UINT stage, ENUMSAMPLERSTATE mode, TEXTURESTAGEPARAM value ) = 0;
	
	/// Returns a ShaderVariable for a given constant register index
	/// \param index - int indicating the register
	/// \return - pointer to ShaderVariable 
	virtual ShaderVariable* GetConstant( int index ) = 0;

	/// Returns the alphatest value for the material
	/// \return - unsigned int indicating the alpha test value (0 to 255)
	virtual UINT GetAlphaTestValue( void ) = 0;

	/// Sets the given material's textures (and texture states) to the renderer
	/// \param pass - unsigned int of render pass to use
	/// \param timeDelta - delta change in time
	virtual void SetTextures( UINT pass, float timeDelta = 0.0f ) = 0;

	/// Sets the render states for this material
	/// \param mode - enum of render state mode
	/// \param value - setting value
	virtual void SetRenderState( ENUMRENDERSTATE mode, UINT value ) = 0;
	
	/// Sets the given material's render states to the renderer
	/// \param pass - unsigned int of render pass to use
	virtual void SetRenderStates( UINT pass ) = 0;

	/// Sets the given material's shader constants to the renderer
	/// \param pass - unsigned int of render pass to use
	virtual void SetConstants( UINT pass ) = 0;	

	/// Sets A Vertex Constant
	///	\param reg - dword of the hashstring name register to set to
	/// \param data - pointer to an array of floats
	/// \param num - int number of vectors to write (4 floats = 1 vector)
	/// \return - true on successful write
	virtual bool SetVertexConstant( DWORD reg, float* data, int num ) = 0;

	/// Sets A Vertex Constant
	///	\param	reg - dword of the hashstring name register to set to
	/// \param x - float x value
	/// \param y - float y value
	/// \param z - float z value
	/// \param w - float w value
	/// \return - true on successful write
	virtual bool SetVertexConstant( DWORD reg, float x, float y, float z, float w ) = 0;

	/// Sets A Pixel Constant
	///	\param reg - dword of the hashstring name register to set to
	/// \param data - pointer to an array of floats
	/// \param num - int number of vectors to write (4 floats = 1 vector)
	/// \return - true on successful write
	virtual bool SetPixelConstant( DWORD reg, float* data, int num ) = 0;

	/// Sets A Pixel Constant
	///	\param reg - dword of the hashstring name register to set to
	/// \param x - float x value
	/// \param y - float y value
	/// \param z - float z value
	/// \param w - float w value
	/// \return - true on successful write
	virtual bool SetPixelConstant( DWORD reg, float x, float y, float z, float w ) = 0;
	
	/// Sets A Vertex Constant
	///	\param varName - pointer of the IHashstring register name to set to
	/// \param data - pointer to an array of floats
	/// \param num - int number of vectors to write (4 floats = 1 vector)
	/// \return - true on successful write
	virtual bool SetVertexConstant( IHashString* hash, float* data, int num ) = 0;

	/// Sets A Vertex Constant
	///	\param varName - pointer of the IHashstring register name to set to
	/// \param x - float x value
	/// \param y - float y value
	/// \param z - float z value
	/// \param w - float w value
	/// \return - true on successful write
	virtual bool SetVertexConstant( IHashString* hash, float x, float y, float z, float w ) = 0;

	/// Sets A Pixel Constant
	///	\param varName - pointer of the IHashstring register name to set to
	/// \param data - pointer to an array of floats
	/// \param num - int number of vectors to write (4 floats = 1 vector)
	/// \return - true on successful write
	virtual bool SetPixelConstant( IHashString* hash, float* data, int num ) = 0;
	
	/// Sets A Pixel Constant
	///	\param varName - pointer of the IHashstring register name to set to
	/// \param x - float x value
	/// \param y - float y value
	/// \param z - float z value
	/// \param w - float w value
	/// \return - true on successful write
	virtual bool SetPixelConstant( IHashString* hash, float x, float y, float z, float w ) = 0;

	/// Sets the Material Template for this material (IMaterialTemplate is deprecated)
	/// \param mattemplate - pointer to an IMaterialTemplate
	virtual void SetMaterialTemplate( IMaterialTemplate* mattemplate ) = 0;
	
	/// Returns a Material Template interface for this material (IMaterialTemplate is deprecated)
	/// \return - pointer to an IMaterialInterface
	virtual IMaterialTemplate* GetMaterialTemplate( void ) = 0;

	/// Sets the render flags for this material
	/// \param flag - a SHADER_RENDER_FLAG (dword)
	virtual void SetShaderFlag( SHADER_RENDER_FLAG flag ) = 0;

	/// Gets the render flags for this material
	/// \return - SHADER_RENDER_FLAG (dword)
	virtual SHADER_RENDER_FLAG GetShaderFlags( void )= 0;

	/// Sets the effect for this material
	/// \param effect - pointer to IEffect
	virtual void SetEffect( IEffect* effect ) = 0;

	/// Gets the effect for this material
	/// return - pointer to IEffect
	virtual IEffect* GetEffect( void ) = 0;

	/// Pushes the current SHADER_RENDER_FLAG onto a stack
	virtual void PushFlagState( void ) = 0;

	/// Pops the current SHADER_RENDER_FLAG on the stack off
	virtual void PopFlagState( void ) = 0;

	/// Get the number of render objects that are referencing this material
	/// \return - unsigned int of the current reference count
	virtual unsigned int GetRefCount( void ) = 0;

	/// Increments the ref count and returns that value
	/// \return - unsigned int of the new, incremented-by-1 reference count
	virtual unsigned int IncrementRefCount( void ) = 0;

	/// Decrements the ref count and returns that value
	/// \return - unsigned int of the new, decremented-by-1 reference count
	virtual unsigned int DecrementRefCount( void ) = 0;
};
}
#endif
