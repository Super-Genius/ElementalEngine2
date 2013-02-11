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

#ifndef CDX9Shader_H
#if(0)
#define CDX9Shader_H
#include "IShader.h"

using namespace std;

#define MAX_TEXTURE_STAGES 8

struct D3D9SamplerStates
{
	DWORD	m_Filter;
	DWORD	m_UMode;
	DWORD	m_VMode;
	D3D9SamplerStates()
	{
		m_Filter = D3DTEXF_POINT;
		m_UMode = D3DTADDRESS_WRAP;
		m_VMode = D3DTADDRESS_WRAP;
	}
};

struct ShaderVariable
{
	float curvalue[4];
	UINT type;
	UINT uid;
	UINT uregister;
};

typedef map< DWORD, ShaderVariable > SHADERVAR_MAP;
typedef pair< DWORD, ShaderVariable > SHADERVAR_PAIR;

//move once we start expanding on the shader system:
class CDX9Shader : public IShader
{
	class CDX9IncludeManager : public ID3DXInclude 
	{
	public:
		STDMETHOD(Open)(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
		STDMETHOD(Close)(LPCVOID pData);
		
	};
	
public:
	CDX9Shader();
	~CDX9Shader();
	virtual UINT GetNumStages()
	{
		return m_NumStages;
	}
	/// Sets the name of a texture stage. Used for loading a shader and associating texture stages with names
	/// that the model or other surfaces will also have
	virtual void SetTextureStage( UINT stage, IHashString * stagename );

	/// Sets the texture flags
	virtual void SetTextureStageFlags( UINT stage, DWORD filter, DWORD umode, DWORD vmode );

	///  gets the name of the stage, or NULL if there's no name assocaited with this stage( uses custom tex or stage not used )
	virtual IHashString * GetTextureStageName( UINT stage );

	/// does the stage use a custom texture
	virtual bool IsStageCustom( UINT stage );
	
	virtual void SetShaderTextureStage( UINT stage, IBaseTextureObject * texture )
	{
		if( stage >= 0 && stage < m_NumStages )
		{
			m_StageTextures[ stage ] = texture;
		}
	}
	/// Gets the custom texture associate with a stage, or NULL if the stage does not use a custom texture or does not
	/// use a texture at all
	virtual IBaseTextureObject * GetShaderTextureFromStage( UINT stage )
	{
		if( stage >= 0 && stage < m_NumStages )
		{
			return m_StageTextures[ stage ];
		}
		else
		{
			return NULL;
		}
	}

	virtual bool SetPerFrameCallback( IShaderCallback * callback )
	{
		m_FrameCallback = callback;
		return true;
	}
	virtual bool SetPerObjectCallback( IShaderCallback * callback )
	{
		m_ObjectCallback = callback;
		return true;
	}
	virtual IShaderCallback * GetFrameCallback()
	{
		return m_FrameCallback;
	}
	virtual IShaderCallback * GetObjectCallback()
	{
		return m_ObjectCallback;
	}
	virtual void Serialize( IArchive &ar )
	{
		//load from file
		return;
	}

	virtual bool LoadPShader( const TCHAR * code_string, IHashString * language );
	virtual bool LoadVShader( const TCHAR * code_string, IHashString * language );
	
	bool SetVertexShaderConstant( const int shaderconstant, float x, float y, float z, float w );
  	bool SetVertexShaderConstant( const int shaderconstant, float array[], UINT numconstants );
 	virtual bool SetVertexShaderVariable( IHashString * name, IHashString * type, UINT uregister, float x, float y, float z, float w  );
	virtual bool SetPixelShaderVariable( IHashString * name, IHashString * type, UINT uregister, float x, float y, float z, float w  );

	//DX9 specific
	void ApplyShader( LPDIRECT3DDEVICE9 pDevice );
	
	virtual bool Reload();

private:
	bool GetStringFromFile( StdString &filename, StdString &outstring );
	void ReleasePixelShader();
	void ReleaseVertexShader();
	/// maximum number of stages per pass
	UINT	m_MaxTextureStages;
	/// current number of stages set on this shader
	UINT	m_NumStages;
	IBaseTextureObject **m_StageTextures;
	CHashString *m_StageTextureNames;
	D3D9SamplerStates	* m_SamplerStates;

	IShaderCallback	* m_FrameCallback;
	IShaderCallback * m_ObjectCallback;
	IRenderer *		m_Renderer;

	StdString						m_VShaderFile;
	StdString						m_PShaderFile;

};

#endif
#endif