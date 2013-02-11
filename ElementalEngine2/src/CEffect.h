///============================================================================
/// \file	CEffect.h
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

#ifndef CEFFECT_H
#define CEFFECT_H
#include "CMaterial.h"
#include "CVertexFormat.h"

namespace ElementalEngine
{

class CEffect : public CObjectTemplate<IEffect >
{
	/// \brief The Default Constructor for CEffect
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CEffect(IHashString *parentName, IHashString *name);

public:
	~CEffect();

	/// See IEffect for the below interface functions:
	virtual void Apply( UINT pass, IMaterial * mat, ISceneSettings * settings );
	virtual UINT	GetNumTextures( UINT pass );
	virtual IBaseTextureObject * GetTexture( UINT pass, UINT stage );
	virtual IBaseTextureObject * GetTexture( IHashString * hash );
	virtual bool	GetTextureChannels( IHashString * hash, UINT &pass, UINT& stage );
	virtual UINT GetNumPass();
	virtual bool Reload();
	virtual bool HasAlpha();

	virtual bool SetVertexConstant( DWORD reg, float * data, int num );
	virtual bool SetVertexConstant( DWORD reg, float x, float y, float z, float w );
	virtual bool SetPixelConstant( DWORD reg, float * data, int num );
	virtual bool SetPixelConstant( DWORD reg, float x, float y, float z, float w );

	virtual bool SetVertexConstant( IHashString *varName, float * data, int num );
	virtual bool SetVertexConstant( IHashString *varName, float x, float y, float z, float w );
	virtual bool SetPixelConstant( IHashString *varName, float * data, int num );
	virtual bool SetPixelConstant( IHashString *varName, float x, float y, float z, float w );

	virtual bool SetPerFrameCallback( IShaderCallback * callback );
	virtual bool SetPerObjectCallback( IShaderCallback * callback );
	virtual IShaderCallback * GetFrameCallback();
	virtual IShaderCallback * GetObjectCallback();
	virtual void SetTextureStageFlags( UINT pass, UINT stage, ENUMSAMPLERSTATE mode, TEXTURESTAGEPARAM value );
	virtual bool GetConstantInformation( IHashString * hash, UINT &reg, UINT &type, bool &pixelShader );
	virtual IVertexFormat * GetVertexFormat();

	virtual void SetShaderPath( IShaderPath *pShaderPath );
	/// End IEffect interface functions

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

	virtual SHADERVAR_MAP * GetVertexConstantVarMap(){ return &m_VertexShaderVarMap; }
	virtual SHADERVAR_MAP * GetPixelConstantVarMap(){ return &m_PixelShaderVarMap; }
	
private:
	void ApplyShader( IShader *pShader, IShaderDefines *pShaderDefines, ISceneSettings *pSceneSettings, SHADER_RENDER_FLAG flags );
	bool SetVertexShaderVariable( IHashString * name, IHashString * type, UINT uregister,  float x, float y, float z, float w );
	bool SetPixelShaderVariable( IHashString * name, IHashString * type, UINT uregister,  float x, float y, float z, float w );
	void SetTexture( UINT pass, UINT stage, IBaseTextureObject * texture );
	void SetTexture( UINT pass, UINT stage, DWORD type, IHashString *pFileName, IBaseTextureObject * texture );
	void SetTextures( UINT pass );
	void SetRenderStates( UINT pass );
	void ReadTextureStageState( int pass, int stage, int numstates, IArchive &ar );
	void GetStagePassFromCompression( DWORD compressed, UINT &pass, UINT &stage );
	void GetVShaderCallback( int uid );
	void GetPShaderCallback( int uid );
	void ApplyConstants( IMaterial * mat );
	DWORD CompressStagePass( int pass, int stage );
	StdString	m_Filename;
	IShaderPath *m_pShaderPath;
	IRenderer * m_Renderer;
	UINT		m_NumTextures;
	UINT		m_NumPasses;
	bool		m_bHasAlpha;
	IShaderCallback	* m_FrameCallback;
	IShaderCallback * m_ObjectCallback;
	vector< TEXTUREVAR_VEC > m_DefaultTextures;
	vector< vector< vector< ShaderSamplerState > > >				 m_SamplerStates;
	// stage and passes stored as highword/loword
	map< DWORD, DWORD > m_HashToStageAndPassesMap;
	SHADERVAR_MAP		m_VertexShaderVarMap;
	SHADERVAR_MAP		m_PixelShaderVarMap;
	IVertexFormat *		m_VertexFormat;
	int			m_Version;

	IShader *m_pLastVertexShader;
	IShader *m_pLastPixelShader;

	// Constants
	static const DWORD m_hszConstantID;
	static const DWORD m_hszFunctionID;
};

struct SVariableContainer
{
	float curvalue[4];
	StdString shadertype;
	StdString name;
	StdString type;
	UINT uregister;
};

struct ShaderInfo
{
	StdString file;
	vector< SVariableContainer > variables;
};
} //Namespace ElementalEngine

#endif //#ifndef CEFFECT_H