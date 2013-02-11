///============================================================================
/// \file	CMaterial.cpp
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
#include "CMaterial.h"
#include "IAnimatedTexture.h"

#define PIXEL_SHADER_TYPE 0
#define VERTEX_SHADER_TYPE 1
const TCHAR * DEFAULT_TEXTURE = _T("Textures\\default.dds");
REGISTER_COMPONENT(CMaterial);

/// IMaterial functions
void CMaterial::ApplyMaterial( UINT pass, ISceneSettings * settings )
{
	if( m_pEffect )
	{
		m_pEffect->Apply( pass, this, settings );
	}
	
	//TODO: remove this. it will be applied by the pEffect
	static Vec4 vConstantData[16];
	::SHADERVAR_MAP::iterator iter = m_VarMap.begin();
	for(; m_VarMap.end() != iter; ++iter )
	{
		::ShaderVariable &ref = iter->second;

		if( ref.shaderType == VERTEX_SHADER_TYPE )
		{
			if( ref.callback != NULL )
			{
				int iConstants = ref.callback->GetValue( vConstantData );
				SetVertexConstant( ref.uregister, (float*)vConstantData, iConstants );
			}
			else
			{
				SetVertexConstant( ref.uregister, ref.curvalue[0], ref.curvalue[1], ref.curvalue[2], ref.curvalue[3] );
			}
		}
		else
		{
			if( ref.callback != NULL )
			{
				int iConstants = ref.callback->GetValue( vConstantData );
				SetPixelConstant( ref.uregister, (float*)vConstantData, iConstants );
			}
			else
			{
				SetPixelConstant( ref.uregister, ref.curvalue[0], ref.curvalue[1], ref.curvalue[2], ref.curvalue[3] );
			}
		}
	}
}

void CMaterial::UnApplyMaterial( UINT pass )
{
	// TODO: unapply other render states, not just wireframe mode
	//if (m_bWireFrameEnabled && m_Renderer)
	//{
	//	m_Renderer->SetRenderState(RENDERSTATE_FILLMODE, RENDERSTATEPARAM_SOLID);
	//}
}

UINT CMaterial::GetNumTextures( UINT pass )
{
	UINT nTextures = 0;
	if( pass >= 0 && pass < m_PassTextures.size() )
	{
		return (UINT)m_PassTextures[ pass ].size();
	}
	return 0;
}
IBaseTextureObject * CMaterial::GetTexture( UINT pass, UINT stage )
{
	if( pass >= 0 &&
		pass < m_PassTextures.size() )
	{
		if( stage >= 0 && 
			stage < m_PassTextures[ pass ].size() )
		{
			return m_PassTextures[ pass ][ stage ];
		}
	}
	return NULL;
}

ShaderVariable* CMaterial::GetConstant( int index )
{
	if( m_VarMap.find( index ) != m_VarMap.end() )
	{
		return &(m_VarMap[ index ]);
	}
	return NULL;
}

UINT CMaterial::GetAlphaTestValue()
{
	return 0;
}

/// Shader Matrix Relevant stuff	
void CMaterial::SetTextures( UINT pass, float timeDelta )
{
	SetRenderStates( pass );
	SetConstants( pass );
	//sets any textures
	if( pass >= 0 && pass < m_PassTextures.size() )
	{
		UINT i = 0;
		for( ; i < m_PassTextures[ pass ].size(); i++ )
		{
			//set textures for these
			IBaseTextureObject * texture = m_PassTextures[ pass ][ i ];
			if( texture )
			{
				if (texture->GetTextureType() == EE_TEXTUREANIMATED2D)
				{
					IAnimatedTextureObject *pAnimTex = NULL;
					pAnimTex = dynamic_cast<IAnimatedTextureObject*>(texture);

					if (pAnimTex)
					{
						float fFPS = pAnimTex->GetFrameRate();

						int iTextureFrame = (int)(timeDelta * fFPS);
						iTextureFrame %= pAnimTex->GetNumFrames();
						
						ITextureObject *texOut = NULL;
						float u1, v1, u2, v2;
						// TODO: allow for different uv sets, model can't use dynamic uvs
                        pAnimTex->GetFrame(iTextureFrame, u1, v1, u2, v2, &texOut);

						if (texOut)
						{
							m_Renderer->SetTexture(i, texOut);
						}
					}
				}
				else
				{
					m_Renderer->SetTexture( i, texture );
				}
			}
		}
		m_Renderer->SetTexture( i, NULL );
		m_Renderer->SetVertexTexture( 0, NULL );
	}
}

void CMaterial::SetRenderStates( UINT pass )
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
	map< ENUMRENDERSTATE, UINT >::iterator iter = m_RenderStates.begin();
	for( ;iter != m_RenderStates.end(); ++iter )
	{
		ENUMRENDERSTATE mode = (*iter).first;
		UINT val = (*iter).second;
		m_Renderer->SetRenderState( mode, val );
	}
}

void CMaterial::SetConstants( UINT pass )
{
	static CHashString constant( _T("Constant") );

	::SHADERVAR_MAP::iterator iter = m_VarMap.begin();
	for(; iter != m_VarMap.end(); ++iter )
	{
		::ShaderVariable &ref = iter->second;
		if( ref.type == constant.GetUniqueID() )
		{
			if( ref.shaderType == PIXEL_SHADER_TYPE )
			{
				if( m_pEffect )
				{
					m_pEffect->SetPixelConstant( ref.uregister, ref.curvalue[ 0 ], ref.curvalue[ 1 ],
						ref.curvalue[ 2 ], ref.curvalue[ 3 ] );	
				}
			}
			else
			{
				if( m_pEffect )
				{
					m_pEffect->SetVertexConstant( ref.uregister, ref.curvalue[ 0 ], ref.curvalue[ 1 ],
						ref.curvalue[ 2 ], ref.curvalue[ 3 ] );
				}
			}
		}
	}
}

SHADER_RENDER_FLAG CMaterial::GetShaderFlags()
{
	return m_Flags;
}

CMaterial::CMaterial(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE( CShaderManager, CMaterial, IMaterial, parentName, name )
{
	m_MaterialTemplateName = "";
	m_Directory = "";
	m_Flags = 0;
	m_pMatTemplate = NULL;
	m_Renderer = NULL;
	m_Version = 0;
	m_pEffect = NULL;
	m_RefCount = 0;

	m_RenderStates.clear();
	
	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface);
	m_Renderer = renderInterface.m_RendererInterface;
	
	CREATEEFFECTINTERFACE addmsg;
	addmsg.m_Name = GetName();
	static DWORD msgHash_AddMaterial = CHashString(_T("AddMaterial")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddMaterial, sizeof( CREATEEFFECTINTERFACE ), &addmsg );
}

/// IObject  functions
/// \brief	Create the a instance of IObject
/// \return	IComponent pointer
IComponent * CMaterial::Create(int nArgs, va_list argptr)
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
		self = (IObject*)new CMaterial(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

/// \brief	The Initialization for the Object
/// \param	IManager* pManager: Parent Manager
void  CMaterial::Init()
{
	//Try to get the template
	if( m_Version <= 2000 )
	{
		const TCHAR * effect = m_EffectName;
		CHashString effName( effect );
		CREATEEFFECTINTERFACE cef;
		cef.m_Name = &effName;
		static DWORD msgHash_GetEffectInterface = CHashString(_T("GetEffectInterface")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_GetEffectInterface, sizeof(MATERIALTEMPLATEINTERFACE ), &cef );
		m_pEffect = cef.m_EffectInterface;
		if( !m_pEffect )
		{
			m_ToolBox->Log( LOGERROR, _T("Material could not load effect for OLD MATERIAL TYPE: %s\n"), effect );
		}
		if( m_pEffect )
		{
			VerifyMaterialWithEffect();
		}
	}
	
	else if( m_Version <= 3000 )
	{
		m_MaterialTemplateName.MakeSafeFileName();
		CHashString matName( m_MaterialTemplateName );
		MATERIALTEMPLATEINTERFACE cef;
		cef.m_Name = &matName;
		static DWORD msgHash_GetEffectInterface = CHashString(_T("GetMaterialTemplate")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_GetEffectInterface, sizeof(MATERIALTEMPLATEINTERFACE ), &cef );
		m_pMatTemplate = cef.m_MatInterface;
		if( !m_pMatTemplate )
		{
			m_ToolBox->Log( LOGERROR, _T("Material could not load material template: %s\n"), m_MaterialTemplateName.c_str() );
		}
		if( m_pMatTemplate )
		{
			VerifyMaterialWithTemplate();
			m_pEffect = m_pMatTemplate->GetEffect();
			if( m_pEffect )
			{
				VerifyMaterialWithEffect();
			}
		}
	}
	else if( m_Version <= 4000 )
	{

	}
}

/// \brief	The Update Function for the Object
/// \return	True on sucess, False on faulure
bool  CMaterial::Update()
{
	return true;
}

/// \brief	The DeInit Function for the Object
void CMaterial::DeInit()
{
	m_MaterialTemplateName = "";
	m_Directory = "";
	m_Flags = 0;
	m_pMatTemplate = NULL;
	m_Renderer = NULL;
	m_Version = 0;
	m_pEffect = NULL;

	CREATEEFFECTINTERFACE addmsg;
	addmsg.m_Name = GetName();
	static DWORD msgHash_RemoveMaterial = CHashString(_T("RemoveMaterial")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveMaterial, sizeof( CREATEEFFECTINTERFACE ), &addmsg );

	m_RenderStates.clear();
}

/// \brief either reads or writes the entity information to a file
void CMaterial::Serialize(IArchive &ar)
{
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

	//Material serialize function
	//Reads in similar to an template
	//Constants, Textures with filter settings, alpha test values
	//renderstate overrides, bone #
	//some flags
	LOADFILEEXTPARAMS lfep;
	int numconstants;
	int numtextures;
	int numrenderstates;
	int numbones;
	int numflags;
	if( ar.IsReading() )
	{
		ar.Read( m_Version, _T("version") );
		if( m_Version == 1000 ) //OLD cal material
		{
			//Not supported
		}
		else if( m_Version == 2000 )
		{
			ar.Read( m_EffectName, _T("effectname"));		
			ar.Read( numconstants, _T("numconstants") );
			ar.Read( numtextures, _T("numtextures") );
			ar.Read( numrenderstates, _T("numrenderstates") );
			ar.Read( numbones, _T("numbones") );
			ar.Read( numflags, _T("numflags") );
			for( int i = 0; i < numconstants; i++ )
			{
				SVariableContainer var;
				//get the name of the variable.
				ar.Read( var.type, _T("type") );
				ar.Read( var.name, _T("name") );
				//get the register	
				ar.Read( var.uregister, _T("register") );
				//get the default value	
				ar.Read( var.curvalue[ 0 ], _T("x") );
				ar.Read( var.curvalue[ 1 ], _T("y") );
				ar.Read( var.curvalue[ 2 ], _T("z") );
				ar.Read( var.curvalue[ 3 ], _T("w") );
				CHashString varName((const TCHAR*)var.name);
				CHashString varTypeName((const TCHAR*)var.type);

				SetVariable( &varName,
							&varTypeName,
							var.uregister,
							var.curvalue[ 0 ],
							var.curvalue[ 1 ],
							var.curvalue[ 2 ],
							var.curvalue[ 3 ] );  
			}
			for( int i = 0; i < numtextures; i++ )
			{
				int pass;
				int stage;
				int numtexturestates;


				StdString filename;
				ar.Read( pass, _T("pass"));
				ar.Read( stage, _T("stage") );
				ar.Read( filename, _T("filename") );
				ar.Read( numtexturestates, _T("numtexturestates") );

				//load the texture
				StdString fullpath = m_Directory;
				fullpath += filename;
				fullpath.MakeSafeFileName();
#ifdef XBOX
				// this is needed for the poke to succeed
				fullpath = SetPathDrive( fullpath, EngineGetToolBox()->GetDrive() );
#endif
				// try to poke the file, if it doesn't exist then try to load the file from the cwd
				// instead of relative to the material
				if (!CheckFileExists( fullpath ))
				{
					fullpath = filename;
					fullpath.MakeSafeFileName();
				}

				CHashString fname( (const TCHAR * ) fullpath );
				TEXTUREOBJECTPARAMS gt;
				gt.bLoad = false;
				gt.Name = &fname;
				//Try to get the texture
				static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
				retval = m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(gt), &gt );
				if( gt.TextureObjectInterface == NULL )
				{
					retval = 0;
					//doesn't exist yet?
					lfep.fileName = const_cast<TCHAR*>((const TCHAR*)fullpath);
					lfep.bInternalLoad = true;
					//This cals LoadFileByExtension
					gt.bLoad = true;
					static DWORD msgHash_AddTexture = CHashString(_T("AddTexture")).GetUniqueID();
					retval = m_ToolBox->SendMessage(msgHash_AddTexture, sizeof(TEXTUREOBJECTPARAMS), &gt);				
					if (retval != MSG_HANDLED)
						m_ToolBox->Log( LOGERROR, _T("Failed to load texture \"%s\" .\n "), (const TCHAR*)filename); 
					retval = 0;	
					static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
					retval = m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(gt), &gt );
				}
				if( !gt.TextureObjectInterface )
				{
					m_ToolBox->Log( LOGERROR, _T("Failed to open material texture %s, defaulting to default texture\n"),
							(const TCHAR*)fullpath );
					//doesn't exist yet?
					lfep.fileName = const_cast<TCHAR*>( DEFAULT_TEXTURE );
					lfep.bInternalLoad = true;
					static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);				
					CHashString defaultName( DEFAULT_TEXTURE );
					gt.Name =  &defaultName;
					static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(gt), &gt );
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
			
			for( int i = 0; i < numbones; i++ )
			{
			}
			vector< CHashString > mHashes;
			for( int i = 0; i < numflags; i++ )
			{
				StdString value;
				ar.Read( value, _T("flag") );
				CHashString t( (const TCHAR*)value );
				mHashes.push_back( t );
			}
			//now send the message to get the flag value:
			GETRENDERFLAGVALUE getFlagMsg;
			int flagsize = (int) mHashes.size();
			getFlagMsg.m_Hashes.resize( flagsize );
			for( int i = 0; i < flagsize; i++ )
			{
				getFlagMsg.m_Hashes[ i ] = &mHashes[ i ];
			}
			static DWORD msgHash_GetRenderFlagValue = CHashString(_T("GetRenderFlagValue")).GetUniqueID();
			DWORD retval = m_ToolBox->SendMessage(msgHash_GetRenderFlagValue, sizeof( GETRENDERFLAGVALUE ), &getFlagMsg );
			if( retval == MSG_HANDLED )
			{
				m_Flags |= getFlagMsg.m_ReturnValue;
			}
		}
		else if( m_Version == 3000 )
		{
			ar.Read( m_MaterialTemplateName, _T("templatename"));		
			ar.Read( numconstants, _T("numconstants") );
			ar.Read( numtextures, _T("numtextures") );
			ar.Read( numbones, _T("numbones") );
			ar.Read( numflags, _T("numflags") );
			for( int i = 0; i < numconstants; i++ )
			{
				SVariableContainer var;
				//get the name of the variable.
				ar.Read( var.name, _T("name") );
				//get the default value	
				ar.Read( var.curvalue[ 0 ], _T("x") );
				ar.Read( var.curvalue[ 1 ], _T("y") );
				ar.Read( var.curvalue[ 2 ], _T("z") );
				ar.Read( var.curvalue[ 3 ], _T("w") );
				CHashString varName((const TCHAR*)var.name);
				CHashString varTypeName((const TCHAR*)var.type);
				SetVariable( &varName ,
							&varTypeName,
							var.uregister,
							var.curvalue[ 0 ],
							var.curvalue[ 1 ],
							var.curvalue[ 2 ],
							var.curvalue[ 3 ] );  
			}
			for( int i = 0; i < numtextures; i++ )
			{
				int pass;
				int stage;
				int numtexturestates;


				StdString filename;
				ar.Read( pass, _T("pass"));
				ar.Read( stage, _T("stage") );
				ar.Read( filename, _T("filename") );
				ar.Read( numtexturestates, _T("numtexturestates") );

				//load the texture
				StdString fullpath = m_Directory;
				fullpath += filename;
				fullpath.MakeSafeFileName();
#ifdef XBOX
				// this is needed for the poke to succeed
				fullpath = SetPathDrive( fullpath, EngineGetToolBox()->GetDrive() );
#endif
				// try to poke the file, if it doesn't exist then try to load the file from the cwd
				// instead of relative to the material
				if (!CheckFileExists( fullpath ))
				{
					fullpath = filename;
					fullpath.MakeSafeFileName();
				}

				CHashString fname( (const TCHAR * ) fullpath );
				TEXTUREOBJECTPARAMS gt;
				gt.bLoad = false;
				gt.Name = &fname;
				//Try to get the texture
				static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
				retval = m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(gt), &gt );
				if( gt.TextureObjectInterface == NULL )
				{
					retval = 0;
					//doesn't exist yet?
					lfep.fileName = const_cast<TCHAR*>((const TCHAR*)fullpath);
					lfep.bInternalLoad = true;
					//This cals LoadFileByExtension
					gt.bLoad = true;
					static DWORD msgHash_AddTexture = CHashString(_T("AddTexture")).GetUniqueID();
					retval = m_ToolBox->SendMessage(msgHash_AddTexture, sizeof(TEXTUREOBJECTPARAMS), &gt);				
					if (retval != MSG_HANDLED)
						m_ToolBox->Log( LOGERROR, _T("Failed to load texture \"%s\" .\n "), (const TCHAR*)filename); 
					retval = 0;	
					static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
					retval = m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(gt), &gt );
				}
				if( !gt.TextureObjectInterface )
				{
					m_ToolBox->Log( LOGERROR, _T("Failed to open material texture %s, defaulting to default texture\n"),
							(const TCHAR*)fullpath );
					//doesn't exist yet?
					lfep.fileName = const_cast<TCHAR*>( DEFAULT_TEXTURE );
					lfep.bInternalLoad = true;
					static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);				
					CHashString defaultName( DEFAULT_TEXTURE );
					gt.Name =  &defaultName;
					static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(gt), &gt );
				}
				SetTexture( pass, stage, gt.TextureObjectInterface );
				ReadTextureStageState( pass, stage, numtexturestates, ar );
			}
			static CHashString depthwrite(_T("depthwrite"));
			static CHashString dtrue(_T("true"));
			static CHashString dfalse(_T("false"));
			
			for( int i = 0; i < numbones; i++ )
			{
			}
			vector< CHashString > mHashes;
			for( int i = 0; i < numflags; i++ )
			{
				StdString value;
				ar.Read( value, _T("flag") );
				CHashString t( (const TCHAR*)value );
				mHashes.push_back( t );
			}
			//now send the message to get the flag value:
			GETRENDERFLAGVALUE getFlagMsg;
			int flagsize = (int) mHashes.size();
			getFlagMsg.m_Hashes.resize( flagsize );
			for( int i = 0; i < flagsize; i++ )
			{
				getFlagMsg.m_Hashes[ i ] = &mHashes[ i ];
			}
			static DWORD msgHash_GetRenderFlagValue = CHashString(_T("GetRenderFlagValue")).GetUniqueID();
			DWORD retval = m_ToolBox->SendMessage(msgHash_GetRenderFlagValue, sizeof( GETRENDERFLAGVALUE ), &getFlagMsg );
			if( retval == MSG_HANDLED )
			{
				m_Flags |= getFlagMsg.m_ReturnValue;
			}
		}
		else if( m_Version == 4000 )
		{

		}
	}
}
void CMaterial::SetRenderState( ENUMRENDERSTATE mode, UINT value )
{
	m_RenderStates[ mode ] = value;

	if( mode == RENDERSTATE_ALPHABLENDENABLE )
	{
		if (value == RENDERSTATEPARAM_TRUE)
			m_Flags |= ALPHA;
		else
			m_Flags &= ~ALPHA;
	}
//	else if (mode == RENDERSTATE_FILLMODE)
//	{
//		m_bWireFrameEnabled = (value == RENDERSTATEPARAM_WIREFRAME);
//	}
}

void CMaterial::ReadTextureStageState( int pass, int stage, int numstates, IArchive &ar )
{
	for( int i = 0; i < numstates; i++ )
	{
		StdString mode;;
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

void CMaterial::ReadRenderState( IArchive &ar )
{
}

void CMaterial::GetShaderCallback( int uid )
{
	static CShaderManager * pShaderMgr = SINGLETONINSTANCE(  CShaderManager );
	SHADERVARFUNCMESSAGE ShaderVarMsg;

	ShaderVarMsg.uid = uid;
	pShaderMgr->OnGetShaderVarFunc( sizeof(SHADERVARFUNCMESSAGE), &ShaderVarMsg );
	if( ShaderVarMsg.m_ShaderVarFunc )
	{
		m_VarMap[ uid ].callback = ShaderVarMsg.m_ShaderVarFunc;
	}
	else
	{
		m_VarMap[ uid ].callback = NULL;
	}
}

void CMaterial::VerifyMaterialWithEffect( void )
{
	::SHADERVAR_MAP::iterator iter = m_VarMap.begin();
	while (iter != m_VarMap.end())
	{
		::ShaderVariable &ref = iter->second;
		CHashString hashName( m_ToolBox->GetHashString( ref.uid ) );
		bool pixelshader;
		if( m_pEffect->GetConstantInformation( &hashName, ref.uregister, ref.type, pixelshader ) != false )
		{
			ref.shaderType = (pixelshader) ? PIXEL_SHADER_TYPE : VERTEX_SHADER_TYPE;
			GetShaderCallback( ref.uid );
			++iter;
		}
		else
		{
			// variable doesn't have a valid register, remove it from the var map
			m_ToolBox->Log( LOGWARNING, _T("Material contained old constant NOT in the Effect: %s in %s\n"), hashName.GetString(), GetName()->GetString() );
			iter = m_VarMap.erase(iter);
		}
	}

	// Copy Vertex Shader Constant Map
	::SHADERVAR_MAP::iterator itrVarMap;
	::SHADERVAR_MAP* pVarMap;
	pVarMap = m_pEffect->GetVertexConstantVarMap();
	itrVarMap = pVarMap->begin();
	while( itrVarMap != pVarMap->end() )
	{
		if( m_VarMap.find( itrVarMap->first ) == m_VarMap.end() )
		{
			m_VarMap[itrVarMap->first] = itrVarMap->second;
		}
		itrVarMap++;
	}

	// Copy Pixel Shader Constant Map
	pVarMap = m_pEffect->GetPixelConstantVarMap();
	itrVarMap = pVarMap->begin();
	while( itrVarMap != pVarMap->end() )
	{
		if( m_VarMap.find( itrVarMap->first ) == m_VarMap.end() )
		{
			m_VarMap[itrVarMap->first] = itrVarMap->second;
		}
		itrVarMap++;
	}
}

void CMaterial::VerifyMaterialWithTemplate( void )
{
	// Remove Old Vars that are no longer in the Material Template
	::SHADERVAR_MAP::iterator iter = m_VarMap.begin();
	while (iter != m_VarMap.end())
	{
		::ShaderVariable &ref = iter->second;
		CHashString hashName( m_ToolBox->GetHashString( ref.uid ) );
		bool pixelshader;
		if( m_pMatTemplate->GetConstantInformation( &hashName, ref.uregister, ref.type, pixelshader ) != false )
		{
			ref.shaderType = (pixelshader) ? PIXEL_SHADER_TYPE : VERTEX_SHADER_TYPE;
			GetShaderCallback( ref.uid );
			++iter;
		}
		else
		{
			// variable doesn't have a valid register, remove it from the var map
			m_ToolBox->Log( LOGWARNING, _T("Material contained old constant NOT in the MaterialTemplate: %s in %s\n"), hashName.GetString(), GetName()->GetString());
			iter = m_VarMap.erase(iter);
		}
	}

	// Copy Vertex Shader Constant Map
	::SHADERVAR_MAP::iterator itrVarMap;
	::SHADERVAR_MAP* pVarMap;
	pVarMap = m_pMatTemplate->GetVertexConstantVarMap();
	itrVarMap = pVarMap->begin();
	while( itrVarMap != pVarMap->end() )
	{
		if( m_VarMap.find( itrVarMap->first ) == m_VarMap.end() )
		{
			m_VarMap[itrVarMap->first] = itrVarMap->second;
		}
		itrVarMap++;
	}

	// Copy Pixel Shader Constant Map
	pVarMap = m_pMatTemplate->GetPixelConstantVarMap();
	itrVarMap = pVarMap->begin();
	while( itrVarMap != pVarMap->end() )
	{
		if( m_VarMap.find( itrVarMap->first ) == m_VarMap.end() )
		{
			m_VarMap[itrVarMap->first] = itrVarMap->second;
		}
		itrVarMap++;
	}

	// Copy Render States Map
	::RENDERSTATE_MAP* pStateMap = m_pMatTemplate->GetRenderStateMap();
	::RENDERSTATE_MAP::iterator itrStateMap = pStateMap->begin();
	while( itrStateMap != pStateMap->end() )
	{
		if( m_RenderStates.find( itrStateMap->first ) == m_RenderStates.end() )
		{
			m_RenderStates[itrStateMap->first] = itrStateMap->second;	
		}
		itrStateMap++;
	}

	// Copy Texture & Textures States Map
	::TEXTURE_VEC &textureVec = m_pMatTemplate->GetTextureVec();
	::TEXTURESTATES_VEC &texturestatesVec = m_pMatTemplate->GetTextureStatesVec();
	for( unsigned int i = 0; i < textureVec.size(); i++ )
	{
		for( unsigned int j = 0; j < textureVec[i].size(); j++ )
		{
			if( (m_PassTextures[i][j] == NULL) && (textureVec[i][j] != NULL) )
			{
				m_PassTextures[i][j] = textureVec[i][j];
				for( unsigned int k = 0; k < texturestatesVec[i][j].size(); k++ )
				{
					m_SamplerStates[i][j][k] = texturestatesVec[i][j][k];	
				}
			}	
		}
	}
}

bool CMaterial::SetVariable( IHashString * name, IHashString * type, UINT uregister,  float x, float y, float z, float w )
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
		m_VarMap[ uid ].curvalue[ 0 ] = x;
		m_VarMap[ uid ].curvalue[ 1 ] = y;
		m_VarMap[ uid ].curvalue[ 2 ] = z;
		m_VarMap[ uid ].curvalue[ 3 ] = w;
		m_VarMap[ uid ].type = fuid;
		m_VarMap[ uid ].uregister = uregister;
		m_VarMap[ uid ].uid = uid;
		GetShaderCallback( uid );
		
		return true;
	}
	return false;
}

bool CMaterial::SetVertexConstant( IHashString * hash, float x, float y, float z, float w )
{
	if( hash )
	{
		DWORD index = hash->GetUniqueID();
		if( m_VarMap.find( index ) != m_VarMap.end() )
		{
			int reg = m_VarMap[ index ].uregister;
			m_VarMap[ index ].curvalue[ 0 ] = x;
			m_VarMap[ index ].curvalue[ 1 ] = y;
			m_VarMap[ index ].curvalue[ 2 ] = z;
			m_VarMap[ index ].curvalue[ 3 ] = w;
			SetVertexConstant( reg, x, y, z, w );
		}
		else if( m_pEffect )
		{
			//find it in the effect
			UINT reg, type;
			bool shadertype;
			if( m_pEffect->GetConstantInformation( hash, reg, type, shadertype ) != false )
			{
				m_VarMap[ index ].curvalue[ 0 ] = x;
				m_VarMap[ index ].curvalue[ 1 ] = y;
				m_VarMap[ index ].curvalue[ 2 ] = z;
				m_VarMap[ index ].curvalue[ 3 ] = w;
				m_VarMap[ index ].uregister = reg;
				m_VarMap[ index ].type = type;
				m_VarMap[ index ].uid = index;
				m_VarMap[ index ].shaderType = shadertype ? PIXEL_SHADER_TYPE : VERTEX_SHADER_TYPE;
				SetVertexConstant( reg, x, y, z, w );
			}
		}
	}
	return true;
}

bool CMaterial::SetVertexConstant( IHashString * hash, float * data, int num )
{
	if( hash )
	{
		DWORD index = hash->GetUniqueID();
		if( m_VarMap.find( index ) != m_VarMap.end() )
		{
			int reg = m_VarMap[ index ].uregister;
			for( int i = 0 ; i < num &&
				i < 4 ; i++ )
			{
				m_VarMap[ index ].curvalue[ i ] = data[ i ];
			}
			SetVertexConstant( reg, data, num );
		}
		else if( m_pEffect )
		{
			//find it in the effect
			UINT reg, type;
			bool ptype;
			if( m_pEffect->GetConstantInformation( hash, reg, type, ptype ) != false )
			{
				for( int i = 0 ; i < num &&
					i < 4 ; i++ )
				{
					m_VarMap[ index ].curvalue[ i ] = data[ i ];
				}
				m_VarMap[ index ].uregister = reg;
				m_VarMap[ index ].type = type;
				m_VarMap[ index ].uid = index;
				m_VarMap[ index ].shaderType = ptype ? PIXEL_SHADER_TYPE : VERTEX_SHADER_TYPE;
				
				SetVertexConstant( reg, data, num );
			}
		}
	}
	return true;
}

bool CMaterial::SetPixelConstant( IHashString * hash, float x, float y, float z, float w )
{
	if( hash )
	{
		DWORD index = hash->GetUniqueID();
		if( m_VarMap.find( index ) != m_VarMap.end() )
		{
			int reg = m_VarMap[ index ].uregister;
			m_VarMap[ index ].curvalue[ 0 ] = x;
			m_VarMap[ index ].curvalue[ 1 ] = y;
			m_VarMap[ index ].curvalue[ 2 ] = z;
			m_VarMap[ index ].curvalue[ 3 ] = w;
			SetPixelConstant( reg, x, y, z, w );
		}
		else if( m_pEffect )
		{
			//find it in the effect
			UINT reg, type;
			bool ptype;
			if( m_pEffect->GetConstantInformation( hash, reg, type, ptype) != false )
			{
				m_VarMap[ index ].curvalue[ 0 ] = x;
				m_VarMap[ index ].curvalue[ 1 ] = y;
				m_VarMap[ index ].curvalue[ 2 ] = z;
				m_VarMap[ index ].curvalue[ 3 ] = w;
				m_VarMap[ index ].uregister = reg;
				m_VarMap[ index ].type = type;
				m_VarMap[ index ].uid = index;
				m_VarMap[ index ].shaderType = ptype ? PIXEL_SHADER_TYPE : VERTEX_SHADER_TYPE;
				
				SetPixelConstant( reg, x, y, z, w );
			}
		}
	}
	return true;
}

bool CMaterial::SetPixelConstant( IHashString * hash, float * data, int num )
{
	if( hash )
	{
		DWORD index = hash->GetUniqueID();
		if( m_VarMap.find( index ) != m_VarMap.end() )
		{
			int reg = m_VarMap[ index ].uregister;
			for( int i = 0 ; i < num &&
				i < 4 ; i++ )
			{
				m_VarMap[ index ].curvalue[ i ] = data[ i ];
			}
			SetPixelConstant( reg, data, num );
		}
		else if( m_pEffect )
		{
			//find it in the effect
			UINT reg, type;
			bool ptype;
			if( m_pEffect->GetConstantInformation( hash, reg, type, ptype ) != false )
			{
				for( int i = 0 ; i < num &&
					i < 4 ; i++ )
				{
					m_VarMap[ index ].curvalue[ i ] = data[ i ];
				}
				m_VarMap[ index ].uregister = reg;
				m_VarMap[ index ].type = type;
				m_VarMap[ index ].uid = index;
				m_VarMap[ index ].shaderType = ptype ? PIXEL_SHADER_TYPE : VERTEX_SHADER_TYPE;
				
				SetPixelConstant( reg, data, num );
			}
		}
	}
	return true;
}


bool CMaterial::SetVertexConstant( DWORD reg, float x, float y, float z, float w )
{
	if( m_pEffect )
	{
		m_pEffect->SetVertexConstant( reg, x, y, z, w );
	}
	return true;
}

bool CMaterial::SetVertexConstant( DWORD reg, float * data, int num )
{
	if( m_pEffect )
	{
		m_pEffect->SetVertexConstant( reg, data, num );
	}
	return true;
}

bool CMaterial::SetPixelConstant( DWORD reg, float x, float y, float z, float w )
{
	if( m_pEffect )
	{
		m_pEffect->SetPixelConstant( reg, x, y, z, w );
	}
	return true;
}

bool CMaterial::SetPixelConstant( DWORD reg, float * data, int num )
{
	if( m_pEffect )
	{
		m_pEffect->SetPixelConstant( reg, data, num );
	}
	return true;
}

void CMaterial::SetMaterialTemplate( IMaterialTemplate * mattemplate )
{
	if( mattemplate )
	{
		m_pMatTemplate = mattemplate;
	}
}

void CMaterial::SetEffect( IEffect * effect )
{
	if( effect )
	{
		m_pEffect = effect;
	}
}

void CMaterial::SetTexture( UINT pass, UINT stage, IBaseTextureObject * texture )
{
	UINT numpasses = (UINT)m_PassTextures.size();
	//reason for this way of resize is I don't want to delete the current contents
	//resize might do this?
	while( pass+1 > numpasses )
	{
		vector< IBaseTextureObject * > newstage;
		vector< vector< ShaderSamplerState > > newsamplerstage;
		m_PassTextures.push_back( newstage );	
		m_SamplerStates.push_back( newsamplerstage );
		numpasses = (UINT)m_PassTextures.size();
	}	
	UINT numstages = (UINT)m_PassTextures[ pass ].size();	
	while( stage + 1 > numstages )
	{
		vector< ShaderSamplerState > state;
		m_PassTextures[ pass ].push_back( NULL );
		m_SamplerStates[ pass ].push_back( state );
		numstages = (UINT)m_PassTextures[ pass ].size();
	}

	m_PassTextures[ pass ][ stage ] = texture;
}

void CMaterial::SetShaderFlag( SHADER_RENDER_FLAG flag )
{
	m_Flags = flag;
}

void CMaterial::SetTextureStageFlags( UINT pass, UINT stage, ENUMSAMPLERSTATE mode, TEXTURESTAGEPARAM value )
{
	if( pass >= 0 && pass < m_SamplerStates.size() &&
		stage >= 0 && stage < m_SamplerStates[ pass ].size() )
	{
		//filter
		ShaderSamplerState nsampler;
		nsampler.m_Mode = mode;		
		nsampler.m_Value = value;
		m_SamplerStates[ pass ][ stage ].push_back( nsampler );
	}else
	{
		m_ToolBox->Log( LOGERROR, _T("Invalid Sampler state (Out of Bounds) pass (%d) stage(%d) in file %s\n  Could mean problems with previous texture load.\n"), 
			pass, stage, GetName()->GetString() );
	}
}

void CMaterial::PushFlagState()
{
	m_FlagStates.push(m_Flags);
}

void CMaterial::PopFlagState()
{
	if (m_FlagStates.size() > 0)
	{
		m_Flags = m_FlagStates.top();
		m_FlagStates.pop();
	}
}

IMaterialTemplate * CMaterial::GetMaterialTemplate( )
{
	return m_pMatTemplate;
}

IEffect* CMaterial::GetEffect()
{
	if( m_pEffect != NULL )
	{
		return m_pEffect;	
	}
	return NULL;
}

unsigned int CMaterial::GetRefCount()
{
	return m_RefCount;
}

unsigned int CMaterial::IncrementRefCount()
{
	m_RefCount++;
	return m_RefCount;
}

unsigned int CMaterial::DecrementRefCount()
{
	m_RefCount--;
	return m_RefCount;
}

IBaseTextureObject * CMaterial::GetTexture( IHashString * hash )
{
	if( m_pEffect )
	{
		return m_pEffect->GetTexture( hash );
	}
	return NULL;
}

bool CMaterial::GetTextureChannels( IHashString * hash, UINT &pass, UINT& stage )
{
	if( m_pEffect )
	{
		return m_pEffect->GetTextureChannels( hash, pass, stage );
	}
	return false;
}