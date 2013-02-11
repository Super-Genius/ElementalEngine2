///============================================================================
/// \file	CMaterialTemplate.h
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

#ifndef CMATERIALTEMPLATE_H
#define CMATERIALTEMPLATE_H
#include "CEffect.h"
#include "CMaterial.h"

namespace ElementalEngine
{

class CMaterialTemplate : public CObjectTemplate<IMaterialTemplate>
{
	// \brief The Default Constructor for CShaderPath
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CMaterialTemplate(IHashString *parentName, IHashString *name);
public:
	virtual void SetRenderState( ENUMRENDERSTATE mode, UINT value );
	virtual void SetEffect( IEffect * effect );
	virtual IEffect* GetEffect();
	virtual void SetRenderStates( UINT pass );

	//calls the effect's, use delegation here
	virtual bool SetVertexConstant( DWORD reg, float * data, int num );
	virtual bool SetVertexConstant( DWORD reg, float x, float y, float z, float w );
	virtual bool SetPixelConstant( DWORD reg, float * data, int num );
	virtual bool SetPixelConstant( DWORD reg, float x, float y, float z, float w );

	virtual IBaseTextureObject * GetTexture( UINT pass, UINT stage );
	virtual void Apply( UINT pass, IMaterial * mat, ISceneSettings * settings );
	virtual bool GetConstantInformation( IHashString * hash, UINT &reg, UINT &type, bool &pixelShader );

	virtual SHADERVAR_MAP* GetVertexConstantVarMap(){ return &m_VertexShaderVarMap; }
	virtual SHADERVAR_MAP* GetPixelConstantVarMap(){ return &m_PixelShaderVarMap; }
	virtual RENDERSTATE_MAP* GetRenderStateMap(){ return &m_RenderStates; }
	virtual TEXTURE_VEC& GetTextureVec(){ return m_DefaultTextures; }
	virtual TEXTURESTATES_VEC& GetTextureStatesVec(){ return m_SamplerStates; }

	virtual bool HasAlpha( )
	{
		return m_bAlpha;
	}
public:
	/// IObject  functions
	/// \brief	Create the a instance of IObject
	/// \return	IComponent pointer
	static IComponent *Create(int nArgs, va_list argptr);
	/// \brief	The Initialization for the Object
	/// \param	IManager* pManager: Parent Manager
	virtual void Init();
	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();
	/// \brief	The DeInit Function for the Object
	virtual void DeInit();
	/// \brief either reads or writes the entity information to a file
	virtual void Serialize(IArchive &ar);
	
	void ApplyConstants( IMaterial * mat );
private:
	bool SetVertexShaderVariable( IHashString * name, IHashString * type, UINT uregister,  float x, float y, float z, float w );
	bool SetPixelShaderVariable( IHashString * name, IHashString * type, UINT uregister,  float x, float y, float z, float w );
	void SetTexture( UINT pass, UINT stage, IBaseTextureObject * texture );
	
	void ReadRenderState( IArchive &ar );
	void CopyFrom(  CMaterialTemplate * from );
	void SetTextureStageFlags( UINT pass, UINT stage, ENUMSAMPLERSTATE mode, TEXTURESTAGEPARAM value );
	void ReadTextureStageState( int pass, int stage, int numstates, IArchive &ar );
	
private:
	bool				m_bIsLoading;
	StdString			m_EffectName;
	StdString			m_Directory;
	RENDERSTATE_MAP m_RenderStates;
	IEffect *			m_Effect;
	IRenderer *			m_Renderer;
	int					m_Version;
	bool				m_bAlpha;
	bool				m_bWireFrameEnabled;
	SHADERVAR_MAP		m_VertexShaderVarMap;
	SHADERVAR_MAP		m_PixelShaderVarMap;
	TEXTURE_VEC			m_DefaultTextures;
	TEXTURESTATES_VEC	m_SamplerStates;
	UINT		m_NumTextures;
	UINT		m_NumPasses;
};

}; //namespace EE

#endif // CMATERIALTEMPLATE_H
