///============================================================================
/// \file	IEffect.h
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

#ifndef IEFFECT_H
#define IEFFECT_H


namespace ElementalEngine
{

class IEffect : public IObject
{
public:
	virtual ~IEffect() {};
	/// Apply this effect using a material and a Scene Setting
	/// \param pass - int of the specific render pass
	/// \param mat - pointer to IMaterial
	/// \param settings - pointer to ISceneSettings
	virtual void Apply( UINT pass, IMaterial* mat, ISceneSettings* settings ) = 0;

	/// Returns the number of textures for this particular pass
	/// \param pass - unsigned int of the specific pass
	/// \return - unsigned interger to the number of textures
	virtual UINT GetNumTextures( UINT pass ) = 0;

	/// Returns the texture interface for a particular pass and stage
	/// \param pass - unsigned int of the specific pass
	/// \param stage - unsigned int of the speicific stage
	/// \return - pointer to texture interface
	virtual IBaseTextureObject* GetTexture( UINT pass, UINT stage ) = 0;

	/// Returns the texture interface for a particular name
	/// \param hash - pointer to IHashstring of name of stage
	/// \return - pointer to texture interface
	virtual IBaseTextureObject* GetTexture( IHashString* hash ) = 0;

	/// Gets the texture stage and pass from the hash(i.e. Lightmap is on stage 4 of pass 0)
	/// \param hash - pointer to IHashstring of name of stage
	/// \param pass - reference unsigned int indicating the render pass
	/// \param stage - reference unsigned int indicating the texture stage
	/// \return - true on successful find
	virtual bool GetTextureChannels( IHashString* hash, UINT& pass, UINT& stage ) = 0;

	/// Gets number of passes for this effect
	/// \return - unsigned int of the number of passes for this effect
	virtual UINT GetNumPass( void ) = 0;
	
	/// Serialize function
	/// \param ar - archive
	virtual void Serialize( IArchive& ar ) = 0;

	/// reloads the shaders
	/// \return - true on sucessful reload
	virtual bool Reload( void ) = 0;

	/// returns if this effect uses alpha
	/// \return - true if it has has alpha
	virtual bool HasAlpha( void ) = 0;

	/// get the interface to the vertex format
	/// \return - pointer to IVertexFormat interface
	virtual IVertexFormat* GetVertexFormat( void ) = 0;
	
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
	virtual bool SetVertexConstant( IHashString* varName, float* data, int num ) = 0;

	/// Sets A Vertex Constant
	///	\param varName - pointer of the IHashstring register name to set to
	/// \param x - float x value
	/// \param y - float y value
	/// \param z - float z value
	/// \param w - float w value
	/// \return - true on successful write
	virtual bool SetVertexConstant( IHashString* varName, float x, float y, float z, float w ) = 0;

	/// Sets A Pixel Constant
	///	\param varName - pointer of the IHashstring register name to set to
	/// \param data - pointer to an array of floats
	/// \param num - int number of vectors to write (4 floats = 1 vector)
	/// \return - true on successful write
	virtual bool SetPixelConstant( IHashString* varName, float* data, int num ) = 0;

	/// Sets A Pixel Constant
	///	\param varName - pointer of the IHashstring register name to set to
	/// \param x - float x value
	/// \param y - float y value
	/// \param z - float z value
	/// \param w - float w value
	/// \return - true on successful write
	virtual bool SetPixelConstant( IHashString* varName, float x, float y, float z, float w ) = 0;

	/// Gets the constant information from the name (hashstring)
	/// \param hash - pointer to an IHashString indicating the name of the register
	/// \param reg - reference to unsigned int to register
	/// \param type - reference to unsigned int to type of constant
	/// \param pixelShader - reference to bool indicating if it is a pixelShader (true) or vertexShader (false)
	/// \return - true on success
	virtual bool GetConstantInformation( IHashString* hash, UINT& reg, UINT& type, bool& pixelShader ) = 0;

	/// Sets flags for a texture stage
	/// \param pass - unsigned int indicating the render pass
	/// \param stage - unsigned int indicating the texture stage
	/// \param mode - enum of sampler state mode
	/// \param value - texture stage setting value
	virtual void SetTextureStageFlags( UINT pass, UINT stage, ENUMSAMPLERSTATE mode, TEXTURESTAGEPARAM value ) = 0;

	/// Sets a function to be called once per frame for this effect
	/// \param callback - pointer to a IShaderCallback interface (function pointer)
	/// \return - true on success
	virtual bool SetPerFrameCallback( IShaderCallback* callback ) = 0;
	
	/// Sets a function to be called once per render object for this effect
	/// \param callback - pointer to a IShaderCallback interface (function pointer)
	/// \return - true on success
	virtual bool SetPerObjectCallback( IShaderCallback* callback ) = 0;

	/// Gets the function to be called once per frame for this effect
	/// \return - pointer to a IShaderCallback interface (function pointer)
	virtual IShaderCallback* GetFrameCallback( void ) = 0;

	/// Gets the function to be called once per render object for this effect
	/// \return - pointer to a IShaderCallback interface (function pointer)
	virtual IShaderCallback* GetObjectCallback() = 0;

	/// Gets the Vertex Shader Constant Variable Map
	/// \return - pointer to vertex constant variable map
	virtual SHADERVAR_MAP* GetVertexConstantVarMap( void ) = 0;
	
	/// Gets the Pixel Shader Constant Variable Map
	/// \return - pointer to pixel constant variable map
	virtual SHADERVAR_MAP* GetPixelConstantVarMap( void ) = 0;

	/// Sets the shader path for looking up effects
	/// \param pShaderPath - pointer to an IShaderPath interface
	virtual void SetShaderPath( IShaderPath* pShaderPath ) = 0;
};

} //namespace ElementalEngine
#endif