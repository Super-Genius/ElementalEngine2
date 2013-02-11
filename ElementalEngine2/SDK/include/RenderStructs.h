///============================================================================
/// \file	RenderStructs
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

#ifndef RENDERSTRUCTS_H
#define RENDERSTRUCTS_H

namespace ElementalEngine
{

class IShader;

/// SHADERVARCALLBACK
/// A function pointer for a shader variable callback
/// \return - float
typedef float (*SHADERVARCALLBACK)(void);

/// IShaderCallback
/// interface to allow shaders to callback for data thru a callback
class IShaderCallback
{
public:
	/// Destructor
	virtual ~IShaderCallback() {};
	
	/// performs a shader callback on the particular shader with a given time value
	/// \param shader - pointer to an IShader interface
	/// \param time - float indicating time
	virtual void ShaderCallback( IShader * shader, float time ) = 0;

	/// gets the name of this callback
	/// \return - pointer to IHashString of the name
	virtual IHashString * GetCallbackName( void ) = 0;
};

/// IShaderVarFunc
/// interface to allow shaders to callback for data thru a defined function
class IShaderVarFunc
{
public:
	/// Destructor
	virtual ~IShaderVarFunc() {};

	/// Gets the value from itself and sets it in the supplied vec4
	/// \param pData - pointer to vec4 to set the value in
	/// \return - returns true if success
	virtual int GetValue( Vec4 *pData ) = 0;

	/// gets the name of this function
	/// \return - pointer to IHashString of the name
	virtual IHashString * GetName( void ) = 0;
};

/// IShaderCallback
/// interface to allow shaders to callback for textures thru a callback
class ITextureCallback
{
public:
	/// Destructor
	virtual ~ITextureCallback() {};

	/// performs a shader callback on the particular shader
	/// \return - pointer to an IBaseTextureObject
	virtual IBaseTextureObject* GetTexture( void ) = 0;

	/// gets the name of this callback
	/// \return - pointer to IHashString of the name
	virtual IHashString * GetName() = 0;
};

/// ShaderSamplerState
/// Struct containing an enumerate state and its value
struct ShaderSamplerState
{
	ENUMSAMPLERSTATE	m_Mode;
	TEXTURESTAGEPARAM	m_Value;
};

/// ShaderVariable
/// Struct containing various data representing a Shader Constant
struct ShaderVariable
{
	float curvalue[4];
	IShaderVarFunc* callback;
	UINT type;
	UINT uid;
	UINT uregister;
	UINT shaderType;

	ShaderVariable()
	{
		curvalue[0] = 0.0f;
		curvalue[1] = 0.0f;
		curvalue[2] = 0.0f;
		curvalue[3] = 0.0f;
		callback = NULL;
	}
};

/// TextureVariable
/// Struct containing various data representing a Texture Stage
struct TextureVariable
{
	DWORD type;
	DWORD texnameID;
	IBaseTextureObject *tex;
};

/// SHADERVAR_MAP
/// map of dword names to shader variables
typedef map< DWORD, ShaderVariable > SHADERVAR_MAP;

/// SHADERVAR_PAIR
/// pairs up a dword name and shader variable
typedef pair< DWORD, ShaderVariable > SHADERVAR_PAIR;

/// TEXTUREVAR_VEC
/// vector of texture variables
typedef vector< TextureVariable > TEXTUREVAR_VEC;

/// RENDERSTATE_MAP
/// map of enumerated states and unsigned int values
typedef map< ENUMRENDERSTATE, UINT > RENDERSTATE_MAP;

/// TEXTURE_VEC
/// vector of vector of pointers to IBaseTexture interfaces
/// used to represent all the textures for a material ( vector[pass][texturestage] )
typedef vector< vector< IBaseTextureObject * > > TEXTURE_VEC;

/// TEXTURESTATES_VEC
/// vector of vector of vector of texture sampler states
/// used to represent the texture stages states for a material( vector[pass][texturestage][texturestagestate] )
typedef vector< vector< vector< ShaderSamplerState > > > TEXTURESTATES_VEC;

} //namespace ElementalEngine
#endif //RENDERSTRUCTS_H