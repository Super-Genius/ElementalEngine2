///============================================================================
/// \file	CMaterial.h
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

#ifndef MATERIAL_H
#define MATERIAL_H
#include "IMaterial.h"
#include <stack>

class CMaterial : public CObjectTemplate<IMaterial>
{	
	/// \brief The Default Constructor for CShaderPath
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CMaterial(IHashString *parentName, IHashString *name);
public:
	/// IMaterial functions
	virtual void ApplyMaterial( UINT pass, ISceneSettings * settings );

	virtual void UnApplyMaterial( UINT pass );
	/// 
	virtual UINT GetNumTextures( UINT pass );
	virtual IBaseTextureObject * GetTexture( UINT pass, UINT stage );
	/// Gets the texture from a hash(i.e. Lightmap )
	virtual IBaseTextureObject * GetTexture( IHashString * hash );
	/// Gets the texture stage and pass from the hash(i.e. Lightmap is on stage 4 of pass 0)
	virtual bool	GetTextureChannels( IHashString * hash, UINT &pass, UINT& stage );
	//TODO:
	virtual UINT GetAlphaTestValue();
	virtual IMaterialTemplate * GetMaterialTemplate( );

	/// Shader Matrix Relevant stuff	
	virtual void SetTextures( UINT pass, float timeDelta = 0.0f );
	virtual void SetRenderStates( UINT pass );
	virtual void SetConstants( UINT pass );	
	virtual SHADER_RENDER_FLAG GetShaderFlags();
	
	virtual bool SetVertexConstant( DWORD reg, float * data, int num );
	virtual bool SetVertexConstant( DWORD reg, float x, float y, float z, float w );
	virtual bool SetPixelConstant( DWORD reg, float * data, int num );
	virtual bool SetPixelConstant( DWORD reg, float x, float y, float z, float w );

	virtual bool SetVertexConstant( IHashString * hash, float * data, int num );
	virtual bool SetVertexConstant( IHashString * hash, float x, float y, float z, float w );
	virtual bool SetPixelConstant( IHashString * hash, float * data, int num );
	virtual bool SetPixelConstant( IHashString * hash, float x, float y, float z, float w );

	virtual ShaderVariable* GetConstant( int index );
	virtual void SetTexture( UINT pass, UINT stage, IBaseTextureObject * texture );
	virtual void SetShaderFlag( SHADER_RENDER_FLAG flag );
	virtual void SetTextureStageFlags( UINT pass, UINT stage, ENUMSAMPLERSTATE mode, TEXTURESTAGEPARAM value );

	virtual void SetMaterialTemplate( IMaterialTemplate * mattemplate );
	
	virtual void SetEffect( IEffect * effect );
	virtual void SetRenderState( ENUMRENDERSTATE mode, UINT value );

	virtual void PushFlagState();
	virtual void PopFlagState();

	virtual IEffect* GetEffect();

	virtual unsigned int GetRefCount();
	virtual unsigned int IncrementRefCount();
	virtual unsigned int DecrementRefCount();
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
	
private:
	bool SetVariable( IHashString * name, IHashString * type, UINT uregister,  float x, float y, float z, float w );
	void ReadTextureStageState( int pass, int stage, int numstates, IArchive &ar );
	void ReadRenderState( IArchive &ar );
	void GetShaderCallback( int uid );

	void VerifyMaterialWithEffect( void );
	void VerifyMaterialWithTemplate( void );
	
private:
	// Material Data
	TEXTURE_VEC m_PassTextures;
	TEXTURESTATES_VEC m_SamplerStates;

	RENDERSTATE_MAP m_RenderStates;
	SHADERVAR_MAP m_VarMap;
	SHADER_RENDER_FLAG  m_Flags;
	stack<SHADER_RENDER_FLAG>	m_FlagStates;
	IRenderer* m_Renderer;
	
	StdString m_Directory;
	int m_Version;
		
	StdString m_EffectName;	
	IEffect* m_pEffect;

	StdString m_MaterialTemplateName;
	IMaterialTemplate* m_pMatTemplate;

	StdString m_MaterialParent;
	IMaterial* m_pMaterialParent;

	unsigned int m_RefCount;
};

#endif