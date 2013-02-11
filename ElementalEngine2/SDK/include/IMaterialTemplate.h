///============================================================================
/// \file	IMaterialTemplate.h
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

#ifndef IMATERIALTEMPLATE_H
#define IMATERIALTEMPLATE_H

namespace ElementalEngine
{

class IEffect;
class IMaterial;

/// IMaterialTemplate is a deprecated interface and should be avoided at all costs
/// Material templates should mostly just hold renderstate settings and the effect the material is
/// linked to. This is because constants still need to be on a per material basis, as well as
/// texturestagestates such as clamp and wrap
class IMaterialTemplate : public IObject
{
public:
	/// Sets the render states for this material
	/// \param mode - enum of render state mode
	/// \param value - setting value
	virtual void SetRenderState( ENUMRENDERSTATE mode, UINT value ) = 0;
	
	/// Sets the given material's render states to the renderer
	/// \param pass - unsigned int of render pass to use
	virtual void SetRenderStates( UINT pass ) = 0;
	
	/// Sets the effect for this material
	/// \param effect - pointer to IEffect
	virtual void SetEffect( IEffect * effect ) = 0;

	/// Gets the effect for this material
	/// return - pointer to IEffect
	virtual IEffect* GetEffect() = 0;

	/// Sets A Vertex Constant
	///	\param reg - dword of the hashstring name register to set to
	/// \param data - pointer to an array of floats
	/// \param num - int number of vectors to write (4 floats = 1 vector)
	/// \return - true on successful write
	virtual bool SetVertexConstant( DWORD reg, float * data, int num ) = 0;
	
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
	virtual bool SetPixelConstant( DWORD reg, float * data, int num ) = 0;

	/// Sets A Pixel Constant
	///	\param reg - dword of the hashstring name register to set to
	/// \param x - float x value
	/// \param y - float y value
	/// \param z - float z value
	/// \param w - float w value
	/// \return - true on successful write
	virtual bool SetPixelConstant( DWORD reg, float x, float y, float z, float w ) = 0;

	/// Gets an IBaseTextureObject interface for a given texture of this material
	/// \param pass - unsigned int of the render pass to use
	/// \param stage - unsigned int of the stage to use
	/// \return - pointer to IBaseTextureObject interface to a texture
	virtual IBaseTextureObject * GetTexture( UINT pass, UINT stage ) = 0;

	/// Apply this effect using a material and a Scene Setting
	/// \param pass - int of the specific render pass
	/// \param mat - pointer to IMaterial
	/// \param settings - pointer to ISceneSettings
	virtual void Apply( UINT pass, IMaterial * mat, ISceneSettings * settings ) = 0;

	/// Gets the constant information from the name (hashstring)
	/// \param hash - pointer to an IHashString indicating the name of the register
	/// \param reg - reference to unsigned int to register
	/// \param type - reference to unsigned int to type of constant
	/// \param pixelShader - reference to bool indicating if it is a pixelShader (true) or vertexShader (false)
	/// \return - true on success
	virtual bool GetConstantInformation( IHashString* hash, UINT& reg, UINT& type, bool& pixelShader ) = 0;
	
	/// Gets the Vertex Shader Constant Variable Map
	/// \return - pointer to vertex constant variable map
	virtual SHADERVAR_MAP* GetVertexConstantVarMap( void ) = 0;

	/// Gets the Pixel Shader Constant Variable Map
	/// \return - pointer to pixel constant variable map
	virtual SHADERVAR_MAP* GetPixelConstantVarMap( void ) = 0;

	/// Gets the RenderState Map
	/// \return - pointer to render state map
	virtual RENDERSTATE_MAP* GetRenderStateMap( void ) = 0;

	/// Gets the Texture Stage Vector
	/// \return - reference to vector of texture stages
	virtual TEXTURE_VEC& GetTextureVec( void ) = 0;

	/// Gets the Texture Stage Render States Vector
	/// \return - reference to vector of texture stage render states
	virtual TEXTURESTATES_VEC& GetTextureStatesVec( void ) = 0;

	/// returns if this materialtemplate uses alpha
	/// \return - true if it has has alpha
	virtual bool HasAlpha( void ) = 0;
};

};// namespace EE

#endif // IMATERIALTEMPLATE_H