///============================================================================
/// \file	IShaderPath
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

#ifndef ISHADERPATH_H
#define ISHADERPATH_H


namespace ElementalEngine
{

/// SHADERMATRIX
typedef map< SHADER_RENDER_FLAG, SHADER_RENDER_FLAG > SHADERMATRIX;

/// SHADERENTRY
/// struct containing a dword name with an associated IShaderDefine and IShader
struct SHADERENTRY
{
	DWORD hszShaderName;
	IShaderDefines* pShaderDefines;
	IShader *pShader;
};

/// SHADERENTRYMAP
/// map of SHADER_RENDER_FLAGs to SHADERENTRYs
typedef map<SHADER_RENDER_FLAG, SHADERENTRY> SHADERENTRYMAP;

/// ShaderPath interface
class IShaderPath : public IObject
{
public:
	/// Destructor
	virtual ~IShaderPath() {};

	/// Reloads the shaderpath
	/// \return - true on success
	virtual bool Reload( void ) = 0;

	/// Returns the SHADERMATRIX for the vertex shaders in this path
	/// \return - pointer to the SHADERMATRIX
	virtual SHADERMATRIX* GetVertexShaderMatrix( void ) = 0;

	/// Returns the SHADERMATRIX for the pixel shaders in this path
	/// \return - pointer to the SHADERMATRIX
	virtual SHADERMATRIX* GetPixelShaderMatrix( void ) = 0;

	/// Returns the list of vertex shader entries (SHADERENTRYMAP) for this path
	/// \return - pointer to the SHADERENTRYMAP
	virtual SHADERENTRYMAP* GetVertexShaderEntries( void ) = 0;

	/// Returns the list of pixel shader entries (SHADERENTRYMAP) for this path
	/// \return - pointer to the SHADERENTRYMAP
	virtual SHADERENTRYMAP* GetPixelShaderEntries( void ) = 0;

	/// Given a SHADER_RENDER_FLAG, returns the best vertex shader match
	/// \param flags - SHADER_RENDER_FLAGs to use for matching
	/// \return - pointer to a SHADERENTRY indicating the best match
	virtual SHADERENTRY* GetVertexShaderMatch( SHADER_RENDER_FLAG flags ) = 0;

	/// Given a SHADER_RENDER_FLAG, returns the best pixel shader match
	/// \param flags - SHADER_RENDER_FLAGs to use for matching
	/// \return - pointer to a SHADERENTRY indicating the best match
	virtual SHADERENTRY* GetPixelShaderMatch( SHADER_RENDER_FLAG flags ) = 0;
};

}

#endif