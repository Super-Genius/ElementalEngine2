///============================================================================
/// \file	CShaderManager.h
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

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

namespace ElementalEngine
{
typedef map< DWORD, IShaderCallback* > SCALLBACKMAP;
typedef pair< DWORD, IShaderCallback* > SCALLBACKMAPELEM;
typedef map< DWORD, IShaderVarFunc* > SVARFUNCMAP;
typedef pair< DWORD, IShaderVarFunc* > SVARFUNCMAPELEM;
typedef map<DWORD,ITextureCallback*> TEXTURECALLBACKMAP;

class CShaderManager : public CManager
{

public:

	/// \brief The Default Destructor for CShaderManager
	/// \return void
	~CShaderManager( );

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief	Initializes the Particle Manager
	/// \param	void
	/// \return	bool: Success
	virtual bool Init( void );
	
	DWORD OnRegisterShaderCallback(DWORD size, void *params);
	DWORD OnInitShaderManager(DWORD size, void * params );
	DWORD OnRemoveShaderCallback( DWORD size, void * params );
	DWORD OnGetEffectInterface( DWORD size, void * params );
	DWORD OnRegisterShaderVarFunc(DWORD size, void *params);
	DWORD OnGetShaderVarFunc( DWORD size, void * params );
	DWORD OnReloadAllShaders( DWORD size, void * params );
	DWORD OnAddEffect( DWORD size, void * params );
	DWORD OnGetMaterialInterface( DWORD size, void * params );
	DWORD OnAddMaterial( DWORD size, void * params );
	DWORD OnRemoveMaterial( DWORD size, void *param ); 
	DWORD OnGetShaderInterface( DWORD size, void * params );
	DWORD OnAddShader( DWORD size, void * params );
	DWORD OnGetShaderPath( DWORD size, void * params );
	DWORD OnAddShaderPath( DWORD size, void * params );
	DWORD OnLoadShaderPaths( DWORD size, void * params );	
	DWORD OnDestroyRenderer(DWORD size, void *data);
	DWORD OnGetMaterialTemplate( DWORD size, void * data );
	DWORD OnAddMaterialTemplate( DWORD size, void * params );
	DWORD OnRemoveMaterialTemplate( DWORD size, void *param ); 
	DWORD OnGetAllEffects( DWORD size, void *param );
	DWORD OnGetShaderDirectory( DWORD size, void * params );
	DWORD OnRegisterTextureCallback(DWORD size, void *params);
	DWORD OnGetCallbackTexture(DWORD size, void *params);

private:
	void RegisterShaderVarFunc( IShaderVarFunc * func );
	void RegisterCommonVarFuncs();
	void RegisterMaterialTemplates( const TCHAR * DirPath );
	void LoadShaderPaths(const TCHAR * DirPath);
	void RegisterMaterialTemplates();

	SINGLETONCONSTRUCTOROVERRIDE( CShaderManager );
	/// \brief The Default Constructor for CShaderManager
	/// \return void
	CShaderManager();
	//static void __fastcall GenericCallback(Shader_t *shader, float dtime);
	StdString	m_ShaderDirectory;
	SCALLBACKMAP m_CallbackMapping;
	SVARFUNCMAP m_VarFuncMapping;
	map< DWORD, IEffect* > m_Effects;
	map< DWORD, IMaterial*> m_Materials;
	map< DWORD, IShader * > m_Shaders;
	map< DWORD, IShaderPath*> m_ShaderPaths;
	map< DWORD, IMaterialTemplate * > m_MaterialTemplates;

	TEXTURECALLBACKMAP m_TextureCallbacks;
};

} //namespace ElementalEngine
#endif //#ifndef SHADERMANAGER_H