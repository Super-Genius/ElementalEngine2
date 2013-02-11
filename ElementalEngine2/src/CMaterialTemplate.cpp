///============================================================================
/// \file	CMaterialTemplate.cpp
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

#include "stdafx.h"
#include "CMaterialTemplate.h"

REGISTER_COMPONENT(CMaterialTemplate);

/// IMaterial functions
void CMaterialTemplate::Apply( UINT pass, IMaterial * mat, ISceneSettings * settings )
{
	if( m_Effect )
	{
		m_Effect->Apply( pass, mat, settings );
	}
}

void CMaterialTemplate::SetRenderStates( UINT pass )
{
	//Set texture stage states and render states
	if( pass >= 0 && pass < m_SamplerStates.size() )
	{
		UINT i = 0;
		for( ; i < m_SamplerStates[ pass ].size(); i++ )
		{
			//set textures for these
			for( int j = 0; j < (int)m_SamplerStates[ pass ][ i ].size(); j++ )
			{
				ENUMSAMPLERSTATE mode = m_SamplerStates[ pass ][ i ][ j ].m_Mode;
				TEXTURESTAGEPARAM val = m_SamplerStates[ pass ][ i ][ j ].m_Value;
				m_Renderer->SetSamplerState( i, mode, val );
			}
		}
	}
	//Set texture stage states and render states
	map< ENUMRENDERSTATE, UINT >::iterator iter = m_RenderStates.begin();
	for( ;iter != m_RenderStates.end(); iter++ )
	{
		ENUMRENDERSTATE mode = (*iter).first;
		UINT val = (*iter).second;
		m_Renderer->SetRenderState( mode, val );
	}
}


CMaterialTemplate::CMaterialTemplate(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE( CShaderManager, CMaterialTemplate, IMaterialTemplate, parentName, name )
{
	DeInit();
	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface);
	m_Renderer = renderInterface.m_RendererInterface;
	
	CREATEEFFECTINTERFACE addmsg;
	addmsg.m_Name = GetName();
	static DWORD msgHash_AddMaterial = CHashString(_T("AddMaterialTemplate")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddMaterial, sizeof( CREATEEFFECTINTERFACE ), &addmsg );
	
	m_bWireFrameEnabled = false;
}

/// IObject  functions
/// \brief	Create the a instance of IObject
/// \return	IComponent pointer
IComponent * CMaterialTemplate::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = (IObject*)new CMaterialTemplate(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

/// \brief	The Initialization for the Object
/// \param	IManager* pManager: Parent Manager
void  CMaterialTemplate::Init()
{
	static CShaderManager * pShaderMgr = SINGLETONINSTANCE(  CShaderManager );
	SHADERVARFUNCMESSAGE msgShaderVar;

	//Try to get the template
	CREATEEFFECTINTERFACE cef;
	string meffect = "Shaders/";
	meffect += m_EffectName;
	const TCHAR * effectName = (TCHAR*)meffect.c_str();
	CHashString matName( effectName );
	cef.m_Name = &matName;
	static DWORD msgHash_GetEffectInterface = CHashString(_T("GetEffectInterface")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetEffectInterface, sizeof(CREATEEFFECTINTERFACE ), &cef );
	m_Effect = cef.m_EffectInterface;
	if( !m_Effect )
	{
		m_ToolBox->Log( LOGERROR, _T("Material Template could not load Effect: %s\n"), effectName );
	}
	else
	{
		//load some values from the effect:
		CEffect * eff = (CEffect*)m_Effect;
		SHADERVAR_MAP * vMap = eff->GetVertexConstantVarMap();
		SHADERVAR_MAP * pMap = eff->GetPixelConstantVarMap();
		if( vMap )
		{
			SHADERVAR_MAP::iterator iter = vMap->begin();
			for(; iter != vMap->end(); ++iter )
			{
				if( m_VertexShaderVarMap.find( iter->first ) == m_VertexShaderVarMap.end() )
				{
					ShaderVariable tmpVar = iter->second;
					msgShaderVar.uid = iter->second.uid;
					pShaderMgr->OnGetShaderVarFunc( sizeof(SHADERVARFUNCMESSAGE), &msgShaderVar );
					
					if( msgShaderVar.m_ShaderVarFunc )
					{
						tmpVar.callback = msgShaderVar.m_ShaderVarFunc;
					}
					//missing entry, add, otherwise the MTT overrides the effect so keep ours.
					m_VertexShaderVarMap.insert( pair< DWORD, ShaderVariable >( iter->first, tmpVar ) );
				}
			}
		}
		if( pMap )
		{
			SHADERVAR_MAP::iterator iter = pMap->begin();
			for(; iter != pMap->end(); ++iter )
			{
				if( m_PixelShaderVarMap.find( iter->first ) == m_PixelShaderVarMap.end() )
				{
					ShaderVariable tmpVar = iter->second;
					msgShaderVar.uid = iter->second.uid;
					pShaderMgr->OnGetShaderVarFunc( sizeof(SHADERVARFUNCMESSAGE), &msgShaderVar );
					
					if( msgShaderVar.m_ShaderVarFunc )
					{
						tmpVar.callback = msgShaderVar.m_ShaderVarFunc;
					}
					//missing entry, add, otherwise the MTT overrides the effect so keep ours.
					m_PixelShaderVarMap.insert( pair< DWORD, ShaderVariable >( iter->first, tmpVar ) );
				}
			}
		}
		//load the effect's defaults if ours hasn't been set(basically inherit off the effect)
		for( int i = 0; i < (int)m_DefaultTextures.size(); i++ )
		{
			for( int j = 0; j < (int)m_DefaultTextures[i].size(); j++ )
			{
				if( m_DefaultTextures[ i ][ j ] == NULL )
				{
					m_DefaultTextures[ i ][ j ] = m_Effect->GetTexture( i, j );
				}
			}
		}		
	}
}

bool CMaterialTemplate::GetConstantInformation( IHashString * hash, UINT &reg, UINT &type, bool &pixelShader )
{
	if( !hash )
		return false;
	DWORD hashId = hash->GetUniqueID();
	if( m_VertexShaderVarMap.find( hashId ) != m_VertexShaderVarMap.end() )
	{
		reg = m_VertexShaderVarMap[ hashId ].uregister;
		type = m_VertexShaderVarMap[ hashId ].type;
		pixelShader = false;
		return true;
	}
	else if( m_PixelShaderVarMap.find( hashId ) != m_PixelShaderVarMap.end() )
	{
		reg = m_PixelShaderVarMap[ hashId ].uregister;
		type = m_PixelShaderVarMap[ hashId ].type;
		pixelShader = true;
		//Zsolt: I added the return here, because it retured false on succsess as well
		return true;
	}else
	if( m_Effect )
	{
		return m_Effect->GetConstantInformation( hash, reg, type, pixelShader );
	}
	return false;
}

/// \brief	The Update Function for the Object
/// \return	True on sucess, False on faulure
bool  CMaterialTemplate::Update()
{
	return true;
}

/// \brief	The DeInit Function for the Object
void CMaterialTemplate::DeInit()
{
	m_bIsLoading = false;
	m_EffectName = "";
	m_Directory = "";
	m_Effect = NULL;
	m_Renderer = NULL;
	m_Version = 0;
	m_bWireFrameEnabled = false;

	CREATEEFFECTINTERFACE addmsg;
	addmsg.m_Name = GetName();
	static DWORD msgHash_RemoveMaterial = CHashString(_T("RemoveMaterialTemplate")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveMaterial, sizeof( CREATEEFFECTINTERFACE ), &addmsg );

	m_RenderStates.clear();
	m_RenderStates[ RENDERSTATE_ZWRITEENABLE ] = RENDERSTATEPARAM_ZTRUE;
	m_RenderStates[ RENDERSTATE_ALPHATESTENABLE ] = RENDERSTATEPARAM_FALSE;
}

IBaseTextureObject * CMaterialTemplate::GetTexture( UINT pass, UINT stage )
{
	IBaseTextureObject * texture = NULL;
	if( pass >= 0 &&
		pass < m_DefaultTextures.size() )
	{
		if( stage >= 0 && 
			stage < m_DefaultTextures[ pass ].size() )
		{
			texture =  m_DefaultTextures[ pass ][ stage ];			
		}
	}
	if( !texture && m_Effect)
	{
		return m_Effect->GetTexture( pass, stage );
	}
	return texture;
}

/// \brief either reads or writes the entity information to a file
void  CMaterialTemplate::Serialize(IArchive &ar)
{
	m_bIsLoading = true;
	DWORD retval = 0;
	//get the directory relative to this material file	
	TCHAR drive[ MAX_PATH ];
	TCHAR dir[ MAX_PATH ];
	TCHAR fname[ MAX_PATH ];
	TCHAR ext[ MAX_PATH ];
	
	memset( drive, 0, sizeof( drive ) );
	memset( dir, 0, sizeof( dir ) );
	memset( fname, 0, sizeof( fname ) );
	memset( ext, 0, sizeof( ext ) );

	m_Directory = "";
	IHashString * curpath =	GetName();
	if( curpath )
	{
		_tsplitpath( curpath->GetString(), drive, dir, fname, ext );
		m_Directory = "";
		m_Directory += drive;
		m_Directory += dir;
	}
	LOADFILEEXTPARAMS lfep;

	
	StdString	BaseDirectory = "";
	m_ToolBox->GetDirectories( NULL, &BaseDirectory );
	BaseDirectory += "\\";

	//Material serialize function
	//Reads in similar to an template
	//Constants, Textures with filter settings, alpha test values
	//renderstate overrides, bone #
	//some flags
	int numrenderstates;
	if( ar.IsReading() )
	{
		ar.Read( m_Version, _T("version") );
		if( m_Version == 3000 )
		{
			m_bAlpha = false;
			ar.Read( m_EffectName, _T("effect"));	
			ar.Read( numrenderstates, _T("numrenderstates") );
			for( int i = 0; i < numrenderstates; i++ )
			{
				StdString mode;
				StdString value;
				ar.Read( mode, _T("mode"));
				ar.Read( value, _T("value"));
				CHashString cmode( (const TCHAR *)mode );
				CHashString cval( (const TCHAR *)value);
				if (m_Renderer)
				{
					ENUMRENDERSTATE emode = m_Renderer->StringToRenderState(cmode.GetUniqueID());
					UINT ival;
					if (emode == RENDERSTATE_ALPHAREF)
					{
						ival = _tstoi( cval.GetString() );
					}
					else
					{
						ival = m_Renderer->StringToRenderStateParam(cval.GetUniqueID());
					}
					SetRenderState( emode, ival );
				}
			}
		}
		else  //New version with the ability to inherit
			if( m_Version == 4000 )
		{		
			m_bAlpha = false;
			StdString	baseMTT;
			StdString	effect;
			ar.Read( baseMTT, _T("parent"));
			if( baseMTT != "" &&
				baseMTT != fname )//don't allow it to reference itself as a parent
			{
				//try to load it and inherit from it
				MATERIALTEMPLATEINTERFACE cef;
				CHashString matName( (const TCHAR*)baseMTT );
				cef.m_Name = &matName;
				static DWORD msgHash_GetEffectInterface = CHashString(_T("GetMaterialTemplate")).GetUniqueID();
				EngineGetToolBox()->SendMessage(msgHash_GetEffectInterface, sizeof(MATERIALTEMPLATEINTERFACE ), &cef );
				if(  !cef.m_MatInterface )
				{
					//maybe not loaded, try loading it
					StdString ShaderDirectory = "shaders\\";
					static DWORD msgHash_GetShaderDirectory = CHashString(_T("GetShaderDirectory")).GetUniqueID();
					EngineGetToolBox()->SendMessage(msgHash_GetShaderDirectory, sizeof(StdString ), &ShaderDirectory );
				
					TCHAR szLoadPath[1024];
					_stprintf( szLoadPath, "%s%s%s", (const TCHAR*)ShaderDirectory, (const TCHAR*)baseMTT, ext );
					LOADFILEEXTPARAMS lfep;
					lfep.fileName = (TCHAR*)((const TCHAR*)szLoadPath);
					lfep.bInternalLoad = true;
					static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
					DWORD retval = m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
					if(retval == MSG_HANDLED )
					{
						EngineGetToolBox()->SendMessage(msgHash_GetEffectInterface, sizeof(MATERIALTEMPLATEINTERFACE ), &cef );
					}
				}
				if(  cef.m_MatInterface )
				{
					CMaterialTemplate * cm = (CMaterialTemplate*)cef.m_MatInterface;
					//if it is loading, it means we're still serializing, which
					//further means, that we're doing a inheritance loop, so error it out
					if( !cm->m_bIsLoading )
					{
						CopyFrom( (CMaterialTemplate*)cef.m_MatInterface );
					}else
					{
						EngineGetToolBox()->Log( LOGERROR, _T("Inheritance Loop found for material template: %s\n"), fname );
					}
				}
				else
				{
					EngineGetToolBox()->Log( LOGERROR, _T("Could not find parent template for material template: %s\n"), fname );
				}
			}
			if( baseMTT == fname )
			{
				EngineGetToolBox()->Log( LOGERROR, _T("material template: %s referencing itself as a parent\n"), fname );
			}
			ar.Read( effect, _T("effect"));	
			if( effect != "" )
			{
				m_EffectName = effect;
			}
			int numconstants;
			int numtextures;
			ar.Read( numconstants, _T("numconstants") );
			ar.Read( numtextures, _T("numtextures") );
			ar.Read( numrenderstates, _T("numrenderstates") );
			for( int i = 0; i < numconstants; i++ )
			{		
				SVariableContainer var;
				//get the name of the variable.
				ar.Read( var.shadertype, _T("shadertype"));
				ar.Read( var.type, _T("type") );
				ar.Read( var.name, _T("name") );
				//get the register	
				ar.Read( var.uregister, _T("register") );
				//get the default value	
				ar.Read( var.curvalue[ 0 ], _T("x") );
				ar.Read( var.curvalue[ 1 ], _T("y") );
				ar.Read( var.curvalue[ 2 ], _T("z") );
				ar.Read( var.curvalue[ 3 ], _T("w") );
				if( var.shadertype == "pixelshader" )
				{
					CHashString varName((const TCHAR*)var.name);
					CHashString varTypeName((const TCHAR*)var.type);
					SetPixelShaderVariable( &varName,
											&varTypeName,
											var.uregister,
											var.curvalue[0],
											var.curvalue[1],
											var.curvalue[2],
											var.curvalue[3] );
				}
				else
				{
					CHashString varName((const TCHAR*)var.name);
					CHashString varTypeName((const TCHAR*)var.type);
					SetVertexShaderVariable( &varName,
											&varTypeName,
											var.uregister,
											var.curvalue[0],
											var.curvalue[1],
											var.curvalue[2],
											var.curvalue[3] );
				}
			}
			m_NumTextures =  numtextures;
			for( int i = 0; i < numtextures; i++ )
			{
				int pass;
				int stage;
				int numtexturestates;
				StdString filename;
				StdString hash;
				ar.Read( pass, _T("pass"));
				ar.Read( stage, _T("stage") );
				ar.Read( filename, _T("filename") );
				ar.Read( numtexturestates, _T("numtexturestates") );
				//load the texture
				//get the texture:
				StdString fullpath = BaseDirectory;
				fullpath += filename;
				CHashString fname( (const TCHAR * ) fullpath );
				TEXTUREOBJECTPARAMS gt;
				gt.bLoad = false;
				gt.Name = &fname;
				if (filename !=  _T("none") &&
					filename != _T("procedural"))
				{
					//Try to get the texture
					static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
					DWORD retval = m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(gt), &gt );
					if( gt.TextureObjectInterface == NULL )
					{
						//doesn't exist yet?
						lfep.fileName = const_cast<TCHAR*>((const TCHAR*)fullpath);
						lfep.bInternalLoad = true;
						static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
						m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);				
						static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
						m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(gt), &gt );
					}
				}
				SetTexture( pass, stage, gt.TextureObjectInterface );
				ReadTextureStageState( pass, stage, numtexturestates, ar );
			}
			static CHashString depthwrite(_T("depthwrite"));
			static CHashString dtrue(_T("true"));
			static CHashString dfalse(_T("false"));
			static DWORD alphatestref = CHashString(_T("alphatestref")).GetUniqueID();
			for( int i = 0; i < numrenderstates; i++ )
			{
				StdString mode;
				StdString value;
				ar.Read( mode, _T("mode"));
				ar.Read( value, _T("value"));
				CHashString cmode( (const TCHAR *)mode );
				CHashString cval( (const TCHAR *)value);
				if (m_Renderer)
				{
					ENUMRENDERSTATE emode = m_Renderer->StringToRenderState(cmode.GetUniqueID());
					UINT ival;
					if (emode == RENDERSTATE_ALPHAREF)
					{
						ival = _tstoi( cval.GetString() );
					}
					else
					{
						ival = m_Renderer->StringToRenderStateParam(cval.GetUniqueID());
					}
					SetRenderState( emode, ival );
				}
			}
		}
	}
	//important for checking inheritance loops:
	m_bIsLoading = false;
}
void CMaterialTemplate::SetRenderState( ENUMRENDERSTATE mode, UINT value )
{
	m_RenderStates[ mode ] = value;

	if( mode == RENDERSTATE_ALPHABLENDENABLE &&
		value == RENDERSTATEPARAM_TRUE )
	{
		m_bAlpha = true;
	}
	else if (mode == RENDERSTATE_FILLMODE)
	{
		m_bWireFrameEnabled = (value == RENDERSTATEPARAM_WIREFRAME);
	}
}

void CMaterialTemplate::ReadRenderState( IArchive &ar )
{

}


bool CMaterialTemplate::SetVertexConstant( DWORD reg, float x, float y, float z, float w )
{
	//pass on to template/shader
	if( m_Effect )
	{
		m_Effect->SetVertexConstant( reg, x, y, z, w );
	}
	return true;
}

bool CMaterialTemplate::SetVertexConstant( DWORD reg, float * data, int num )
{
	//pass on to template/shader
	if( m_Effect )
	{
		m_Effect->SetVertexConstant( reg, data, num );
	}
	return true;
}

bool CMaterialTemplate::SetPixelConstant( DWORD reg, float x, float y, float z, float w )
{
	//pass on to template/shader
	if( m_Effect )
	{
		m_Effect->SetPixelConstant( reg, x, y, z, w );
	}
	return true;
}

bool CMaterialTemplate::SetPixelConstant( DWORD reg, float * data, int num )
{
	//pass on to template/shader
	if( m_Effect )
	{
		m_Effect->SetPixelConstant( reg, data, num );
	}
	return true;
}

void CMaterialTemplate::SetEffect( IEffect * effect )
{
	if( effect )
	{
		m_Effect = effect;
	}
}

IEffect* CMaterialTemplate::GetEffect()
{
	return m_Effect;
}

void CMaterialTemplate::CopyFrom(  CMaterialTemplate * from )
{
	if( from )
	{
		m_EffectName = from->m_EffectName;
		//m_Directory = from->m_Directory;
		m_RenderStates = from->m_RenderStates;
		m_bAlpha = from->m_bAlpha;
		m_bWireFrameEnabled = from->m_bWireFrameEnabled;
		m_NumPasses = from->m_NumPasses;
		m_NumTextures = from->m_NumTextures;
		m_DefaultTextures = from->m_DefaultTextures;
		m_SamplerStates = from->m_SamplerStates;
		m_VertexShaderVarMap = from->m_VertexShaderVarMap;
		m_PixelShaderVarMap = from->m_PixelShaderVarMap;
	}
}

void CMaterialTemplate::SetTexture( UINT pass, UINT stage, IBaseTextureObject * texture )
{
	int numpasses = (int)m_DefaultTextures.size();
	//reason for this way of resize is I don't want to delete the current contents
	//resize might do this?
	while( pass+1 > (UINT)numpasses )
	{
		vector< IBaseTextureObject * > newstage;
		m_DefaultTextures.push_back( newstage );
		numpasses = (int)m_DefaultTextures.size();
	}	
	int numstages = (int)m_DefaultTextures[ pass ].size();	
	while( stage + 1 > (UINT)numstages )
	{
		m_DefaultTextures[ pass ].push_back( NULL );
		numstages = (int)m_DefaultTextures[ pass ].size();
	}
	int numpasses2 = (int)m_SamplerStates.size();
	while( pass+1 > (UINT)numpasses2 )
	{
		vector< vector< ShaderSamplerState > > newsamplerstage;
		m_SamplerStates.push_back( newsamplerstage );
		numpasses2 = (int)m_SamplerStates.size();
	}	
	int numstages2 = (int)m_SamplerStates[ pass ].size();	
	while( stage + 1 > (UINT)numstages2 )
	{
		vector< ShaderSamplerState > state;
		m_SamplerStates[ pass ].push_back( state );
		numstages2 = (int)m_SamplerStates[ pass ].size();
	}
	m_DefaultTextures[ pass ][ stage ] = texture;
}

bool CMaterialTemplate::SetVertexShaderVariable( IHashString * name, IHashString * type, UINT uregister,  float x, float y, float z, float w )
{
	static CHashString constant( _T("Constant") );

	if( name )
	{
		DWORD fuid = constant.GetUniqueID();
		if( type )
		{
			fuid = type->GetUniqueID();
		}

		DWORD uid = name->GetUniqueID();
		m_VertexShaderVarMap[ uid ].curvalue[ 0 ] = x;
		m_VertexShaderVarMap[ uid ].curvalue[ 1 ] = y;
		m_VertexShaderVarMap[ uid ].curvalue[ 2 ] = z;
		m_VertexShaderVarMap[ uid ].curvalue[ 3 ] = w;
		m_VertexShaderVarMap[ uid ].type = fuid;
		m_VertexShaderVarMap[ uid ].uregister = uregister;
		m_VertexShaderVarMap[ uid ].uid = uid;
		return true;
	}
	return false;
}


bool CMaterialTemplate::SetPixelShaderVariable( IHashString * name, IHashString * type, UINT uregister,  float x, float y, float z, float w )
{
	static CHashString constant( _T("Constant") );

	if( name )
	{
		DWORD fuid = constant.GetUniqueID();
		if( type )
		{
			fuid = type->GetUniqueID();
		}

		DWORD uid = name->GetUniqueID();
		m_PixelShaderVarMap[ uid ].curvalue[ 0 ] = x;
		m_PixelShaderVarMap[ uid ].curvalue[ 1 ] = y;
		m_PixelShaderVarMap[ uid ].curvalue[ 2 ] = z;
		m_PixelShaderVarMap[ uid ].curvalue[ 3 ] = w;
		m_PixelShaderVarMap[ uid ].type = fuid;
		m_PixelShaderVarMap[ uid ].uregister = uregister;
		m_PixelShaderVarMap[ uid ].uid = uid;
		return true;
	}
	return false;
}

void CMaterialTemplate::SetTextureStageFlags( UINT pass, UINT stage, ENUMSAMPLERSTATE mode, TEXTURESTAGEPARAM value )
{
	if( pass >= 0 && pass < m_SamplerStates.size() &&
		stage >= 0 && stage < m_SamplerStates[ pass ].size() )
	{
		//filter
		ShaderSamplerState nsampler;
		nsampler.m_Mode = mode;		
		nsampler.m_Value = value;
		m_SamplerStates[ pass ][ stage ].push_back( nsampler );
	}
	else
	{
		//create sampler state
		int numpasses2 = (int)m_SamplerStates.size();
		while( pass+1 > (UINT)numpasses2 )
		{
			vector< vector< ShaderSamplerState > > newsamplerstage;
			m_SamplerStates.push_back( newsamplerstage );
			numpasses2 = (int)m_SamplerStates.size();
		}	
		int numstages2 = (int)m_SamplerStates[ pass ].size();	
		while( stage + 1 > (UINT)numstages2 )
		{
			vector< ShaderSamplerState > state;
			m_SamplerStates[ pass ].push_back( state );
			numstages2 = (int)m_SamplerStates[ pass ].size();
		}
		//filter
		ShaderSamplerState nsampler;
		nsampler.m_Mode = mode;		
		nsampler.m_Value = value;
		m_SamplerStates[ pass ][ stage ].push_back( nsampler );
	}
}

void CMaterialTemplate::ReadTextureStageState( int pass, int stage, int numstates, IArchive &ar )
{
	for( int i = 0; i < numstates; i++ )
	{
		StdString mode;
		StdString value;
		ar.Read( mode, _T("mode"));
		ar.Read( value, _T("value"));
		CHashString cmode( (const TCHAR *)mode );
		CHashString cval( (const TCHAR *)value);
		ENUMSAMPLERSTATE estate = m_Renderer->StringToSamplerState( cmode.GetUniqueID() );
		TEXTURESTAGEPARAM evalue = m_Renderer->StringToTextureStageParam( cval.GetUniqueID() );
		SetTextureStageFlags( pass, stage, estate, evalue );
	}
}

void CMaterialTemplate::ApplyConstants( IMaterial * mat )
{
	// If there is a material dont bother with this
	//if( mat )
	//{
	//	return;
	//}

	static Vec4 vConstantData[16]; // TODO: use max( d3dCaps.MaxVertexShaderConst, d3dCaps.MaxPixelShaderConst ) for a max size

	//Set some constants
	if( m_VertexShaderVarMap.size() > 0 )
	{
		SHADERVAR_MAP::iterator iter = m_VertexShaderVarMap.begin();
		for(;iter != m_VertexShaderVarMap.end(); ++iter )
		{
			ShaderVariable r = (*iter).second;
			//Override effect var with material's var if available
			if( mat )
			{
				ShaderVariable* temp = mat->GetConstant( r.uid );
				if( temp )
				{
					r = *temp;
				}
			}

			// Check to see if it has a shadervar callback
			if( r.callback != NULL )
			{
				int iConstants = r.callback->GetValue( vConstantData );
				SetVertexConstant( r.uregister, (float*)vConstantData, iConstants );
			}
			// Else write out the value
			else
			{
				SetVertexConstant( r.uregister, r.curvalue[0], r.curvalue[1], r.curvalue[2], r.curvalue[3] );
			}
		}
	}
	if( m_PixelShaderVarMap.size() > 0 )
	{
		SHADERVAR_MAP::iterator iter = m_PixelShaderVarMap.begin();
		for(;iter != m_PixelShaderVarMap.end(); ++iter )
		{
			ShaderVariable r = (*iter).second;			
			//Override effect var with material's var if available
			if( mat )
			{
				ShaderVariable* temp = mat->GetConstant( r.uid );
				if( temp )
				{
					r = *temp;
				}
			}

			// Check to see if it has a shadervar callback
			if( r.callback != NULL )
			{
				int iConstants = r.callback->GetValue( vConstantData );
				SetPixelConstant( r.uregister, (float*)vConstantData, iConstants );
			}
			// Else write out the value
			else
			{
				SetPixelConstant( r.uregister, r.curvalue[0], r.curvalue[1], r.curvalue[2], r.curvalue[3]);
			}
		}
	}
}