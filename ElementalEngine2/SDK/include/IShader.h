///============================================================================
/// \file	IShader.h
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

#ifndef ISHADER_H
#define ISHADER_H

namespace ElementalEngine
{

/// SHADERDEFINEENUM
enum SHADERDEFINEENUM
{
	SHADERDEFINE_COMPLEXLIGHTS, 
	SHADERDEFINE_SIMPLELIGHTS, 
	SHADERDEFINE_SHADOWMAPS, 
	SHADERDEFINE_COUNT // do not use
};

/// SHADERDEFINESENCODING
/// Shader Defines Encoding struct
/// Stores an encoding of shader definitions
struct SHADERDEFINESENCODING
{
	unsigned long iEncoding;
};

/// Shader Defines Interface
/// Used to generate encoding's of shader definitions or to decode
/// a stored shader definition encoding.
class IShaderDefines
{
public:
	/// Sets the values from a SHADERDEFINESENCODING
	/// \param encoding - SHADERDEFINESENCODING struct to be used to encode
	virtual void SetValuesFromEncoding( const SHADERDEFINESENCODING &encoding ) = 0;

	/// Gets the values from a SHADERDEFINESENCODING
	/// \return SHADERDEFINESENCODING struct for this current IShaderDefine
	virtual SHADERDEFINESENCODING GetValuesEncoding( void ) const = 0;

	/// Gets the number of Shader Defines
	/// \return - dword of number of shader defines
	virtual DWORD GetDefineCount( void ) const = 0;

	/// Gets the index for a shader define based on name
	/// \param szDefineName - pointer to TCHAR of the name of the shader define
	/// \return - dword of the index to that shader define
	virtual DWORD GetDefineIndex( const TCHAR* szDefineName ) const = 0;

	/// Gets the name for a shader define based on index
	/// \param iDefineIndex - dword of the index to look up
	/// \return - pointer to the name of that index
	virtual const char* GetDefineName( DWORD iDefineIndex ) const = 0;
	
	/// Gets the range of values for a given index
	/// \param iDefineIndex - index to look up
	/// \param out_iValueMin - referenced int indicating minimum value
	/// \param out_iValueMax - referenced int indicating maximum value
	virtual void GetDefineRange( DWORD iDefineIndex, int &out_iValueMin, int &out_iValueMax ) const = 0;

	/// Gets the value for a shader define based on index
	/// \param iDefineIndex - dword of the index to look up
	/// \return - int of the value for that index
	virtual int GetDefineValue( DWORD iDefineIndex ) const = 0;

	/// Sets the value for a shader define based on index
	/// \param iDefineIndex - dword of the index to set
	/// \param iValue - int of the value to set for that index
	virtual void SetDefineValue( DWORD iDefineIndex, int iValue ) = 0;

	/// Sets the value for a shader define based on SHADERDEFINEENUM
	/// \param eDefine - SHADERDEFINEENUM of the index to set
	/// \param iValue - int of the value to set for that SHADERDEFINEENUM
	virtual void SetDefineValue( SHADERDEFINEENUM eDefine, int iValue ) = 0;

	/// Sets all the values to zero
	virtual void ZeroValues( void ) = 0;
};

///Shader interface
class IShader : public IObject
{
public:
	/// Destructor
	virtual ~IShader() {};

	/// Applies the ShaderDefines to this shader
	/// \param pDefines - pointer to IShaderDefines to apply
	/// \return - bool on success
	virtual bool Apply( IShaderDefines* pDefines ) = 0;

	/// Reloads the shaders based on the ShaderDefines
	/// \param pDefines - pointer to IShaderDefines to use for reloading
	/// \return - bool on success
	virtual bool Reload( IShaderDefines* pDefines ) = 0;

	/// Loads the shaders based on the ShaderDefines
	/// \param pDefines - pointer to IShaderDefines to use for reloading
	/// \param bCompile - flag indicating to compile the variants for this shader
	/// \return - bool on success
	virtual bool LoadVariant( IShaderDefines* pDefines, bool bCompile=false ) = 0;

	/// Sets a constant
	///	\param reg - dword of the hashstring name register to set to
	/// \param data - pointer to an array of floats
	/// \param num - int number of vectors to write (4 floats = 1 vector)
	/// \return - true on successful write
	virtual bool SetConstant( DWORD reg, float* data, int num ) = 0;
	
	/// Sets a constant
	///	\param	reg - dword of the hashstring name register to set to
	/// \param x - float x value
	/// \param y - float y value
	/// \param z - float z value
	/// \param w - float w value
	/// \return - true on successful write
	virtual bool SetConstant( DWORD reg, float x, float y, float z, float w ) = 0;
};

}

#endif