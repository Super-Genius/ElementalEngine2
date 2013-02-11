///============================================================================
/// \file	CEffect.cpp
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
#include "CEffect.h"
REGISTER_COMPONENT(CEffect);

const DWORD CEffect::m_hszConstantID = CHashString(_T("Constant")).GetUniqueID();
const DWORD CEffect::m_hszFunctionID = CHashString(_T("Function")).GetUniqueID();

CEffect::CEffect(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE( CShaderManager, CEffect, IEffect, parentName, name )
{
	DeInit();
	
	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface);
	m_Renderer = renderInterface.m_RendererInterface;
}

CEffect::~CEffect()
{
	DeInit();
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CEffect::Create(int nArgs, va_list argptr)
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
		self = (IObject*)new CEffect(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}



void CEffect::Serialize( IArchive &ar )
{
	//get the directory relative to this material file	
	TCHAR drive[ MAX_PATH ];
	TCHAR dir[ MAX_PATH ];
	TCHAR fname[ MAX_PATH ];
	TCHAR ext[ MAX_PATH ];
	
	memset( drive, 0, sizeof( drive ) );
	memset( dir, 0, sizeof( dir ) );
	memset( fname, 0, sizeof( fname ) );
	memset( ext, 0, sizeof( ext ) );

	StdString	Directory = "";
	IHashString * curpath =	GetName();
	if( curpath )
	{
		_tsplitpath( curpath->GetString(), drive, dir, fname, ext );
		Directory = "";
		Directory += drive;
		Directory += dir;
	}

	//Material serialize function
	//Reads in similar to an effect
	//Constants, Textures with filter settings, alpha test values
	//renderstate overrides, bone #
	//some flags
	LOADFILEEXTPARAMS lfep;
	int numconstants;
	int numtextures;
	int numrenderstates;
	int numbones;
	int numflags;
	int numFormats;
	if( ar.IsReading() )
	{
		ar.Read( m_Version, _T("version") );
		if( m_Version >= 3000 )
		{
		
			ar.Read( numconstants, _T("numconstants") );
			ar.Read( numtextures, _T("numtextures") );
			ar.Read( numrenderstates, _T("numrenderstates") );
			ar.Read( numbones, _T("numbones") );
			ar.Read( numflags, _T("numflags") );
			ar.Read( numFormats, _T("numformats") );

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
				DWORD hszTextureTypeID = m_hszConstantID;
				ar.Read( hash, _T("tag"));
				ar.Read( pass, _T("pass"));
				ar.Read( stage, _T("stage") );
				if (m_Version >= 3100)
				{
					StdString type;
					ar.Read(type, _T("type"));
					hszTextureTypeID = CHashString(type).GetUniqueID();
				}
				ar.Read( filename, _T("filename") );
				ar.Read( numtexturestates, _T("numtexturestates") );
				CHashString hashname( (const TCHAR*) hash );
				DWORD hashId = hashname.GetUniqueID();
				DWORD stageAndPass = CompressStagePass( pass, stage );
				m_HashToStageAndPassesMap[ hashId ] = stageAndPass;
				if (hszTextureTypeID == m_hszConstantID)
				{
					//load the texture
					//get the texture:
					StdString fullpath = Directory;
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
					if (filename !=  _T("none") &&
						filename != _T("procedural") )
					{
						//Try to get the texture
						static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
						DWORD retval = m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(gt), &gt );
						if( gt.TextureObjectInterface == NULL )
						{
							//doesn't exist yet?
							lfep.fileName = const_cast<TCHAR*>((const TCHAR*)fullpath);
							lfep.bInternalLoad = true;
							gt.bLoad = true;
							static DWORD msgHash_AddTexture = CHashString(_T("AddTexture")).GetUniqueID();
							retval = m_ToolBox->SendMessage(msgHash_AddTexture, sizeof(TEXTUREOBJECTPARAMS), &gt);				
							if (retval != MSG_HANDLED)
							{
								m_ToolBox->Log( LOGERROR, _T("Failed to load texture \"%s\" .\n "), (const TCHAR*)filename); 
							}
							static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
							m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(gt), &gt );
						}
					}
					SetTexture( pass, stage, gt.TextureObjectInterface );
				}
				else
				{
					CHashString hszFileName(filename);
					SetTexture( pass, stage, hszTextureTypeID, &hszFileName, NULL );
				}
				ReadTextureStageState( pass, stage, numtexturestates, ar );
			}
			for( int i = 0; i < numrenderstates; i++ )
			{
                				
			}
			for( int i = 0; i < numbones; i++ )
			{
			}
			for( int i = 0; i < numflags; i++ )
			{
			}
			//Load the VertexFormat object			
			if( m_Version >= 2100 &&
				numFormats > 0)
			{	
				static CHashString vtxFmtName(_T("CVertexFormat"));
				m_VertexFormat = dynamic_cast< IVertexFormat * > ( m_ToolBox->CreateComponent( 
					&vtxFmtName, 
					2, GetName(), NULL ) );
				if( m_VertexFormat != NULL )
				{
					for( int i = 0; i < numFormats; i++ )
					{
						m_VertexFormat->Serialize( ar );
						break;// 2 or more Vertex formats makes no sense
					}
				}
			}
		}
		else 
			if( m_Version >= 2000 )
		{	
			ar.Read( numconstants, _T("numconstants") );
			ar.Read( numtextures, _T("numtextures") );
			ar.Read( numrenderstates, _T("numrenderstates") );
			ar.Read( numbones, _T("numbones") );
			ar.Read( numflags, _T("numflags") );
			if( m_Version >= 2100 )
			{				
				ar.Read( numFormats, _T("numflags") );
			}
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
				ar.Read( pass, _T("pass"));
				ar.Read( stage, _T("stage") );
				ar.Read( filename, _T("filename") );
				ar.Read( numtexturestates, _T("numtexturestates") );

				//load the texture
				//get the texture:
				StdString fullpath = Directory;
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
				gt.TextureObjectInterface = NULL;
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
						gt.bLoad = true;
						static DWORD msgHash_AddTexture = CHashString(_T("AddTexture")).GetUniqueID();
						retval = m_ToolBox->SendMessage(msgHash_AddTexture, sizeof(TEXTUREOBJECTPARAMS), &gt);				
						if (retval != MSG_HANDLED)
						{
							m_ToolBox->Log( LOGERROR, _T("Failed to load texture \"%s\" .\n "), (const TCHAR*)filename); 
						}
						static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
						m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(gt), &gt );
					}
				}
				SetTexture( pass, stage, gt.TextureObjectInterface );
				ReadTextureStageState( pass, stage, numtexturestates, ar );
			}
			for( int i = 0; i < numrenderstates; i++ )
			{
                				
			}
			for( int i = 0; i < numbones; i++ )
			{
			}
			for( int i = 0; i < numflags; i++ )
			{
			}
			//Load the VertexFormat object			
			if( m_Version >= 2100 )
			{	
				static CHashString vtxFmtName(_T("CVertexFormat"));
				m_VertexFormat = dynamic_cast< IVertexFormat * > ( m_ToolBox->CreateComponent( 
					&vtxFmtName, 
					2, GetName(), NULL ) );
				if( m_VertexFormat != NULL )
				{
					for( int i = 0; i < numFormats; i++ )
					{
						m_VertexFormat->Serialize( ar );
						break;// 2 or more Vertex formats makes no sense
					}
				}
			}
		}
		
	}
}


void CEffect::ReadTextureStageState( int pass, int stage, int numstates, IArchive &ar )
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

///
/// Init / Update / DeInit
///
void CEffect::Init()
{
	IHashString * name =  GetName();
	if( name )
	{
		m_Filename = name->GetString();
	}
}

void CEffect::DeInit()
{	
	m_pShaderPath = NULL;
	m_Renderer = NULL;
	m_NumTextures = 0;
	m_NumPasses = 0;
	m_bHasAlpha = false;
	m_FrameCallback = NULL;
	m_ObjectCallback = NULL;
	m_VertexFormat = NULL;

	m_pLastVertexShader = NULL;
	m_pLastPixelShader = NULL;
}

/// \brief Update the object
/// \return true or false
bool CEffect::Update()
{
	return true;
}

bool CEffect::GetConstantInformation( IHashString * hash, UINT &reg, UINT &type, bool &pixelShader )
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
		return true;
	}
	else
	{
		return false;
	}
}

void CEffect::ApplyShader( IShader *pShader, IShaderDefines *pShaderDefines, ISceneSettings *pSceneSettings, SHADER_RENDER_FLAG flags )
{
	if (pSceneSettings && pShaderDefines)
	{
		pShaderDefines->ZeroValues();

		int nLights = pSceneSettings->GetActiveLightCount();
		int nDetailLights = pSceneSettings->GetDetailLightCount();
		int nShadowMaps = pSceneSettings->GetShadowMapCount();
		pShaderDefines->SetDefineValue( SHADERDEFINE_COMPLEXLIGHTS, nLights );
		pShaderDefines->SetDefineValue( SHADERDEFINE_SIMPLELIGHTS, nDetailLights );
		pShaderDefines->SetDefineValue( SHADERDEFINE_SHADOWMAPS, nShadowMaps );

		pShader->Apply( pShaderDefines );
	}
	else
		pShader->Apply( NULL );
}

void CEffect::Apply( UINT pass, IMaterial * mat, ISceneSettings * settings )
{
	// reset last used shader pointers
	m_pLastVertexShader = NULL;
	m_pLastPixelShader = NULL;

	if (m_pShaderPath == NULL)
	{
		m_ToolBox->Log( LOGERROR, _T("CEffect::Apply() Effect %s is not connected to a shader path.\n"), GetName()->GetString() );
		return;
	}

	// apply shaders
	SHADER_RENDER_FLAG flags = 0;
	if( mat )
	{
		flags |= mat->GetShaderFlags();
	}
	if( settings )
	{
		flags |= settings->GetShaderFlags();

		// if we are not currently casting a shadow, set light counts (shadow casting requires no lights)
		if ((flags & (SHADOW_CAST_HEMI | SHADOW_CAST_OMNI | SHADOW_CAST_DIR)) == 0)
		{
			int nLights = settings->GetActiveLightCount();
			int nDetailLights = settings->GetDetailLightCount();
			int nShadowMaps = settings->GetShadowMapCount();
			if (nLights > 0 || nDetailLights > 0)
			{
				flags |= SHADER_RF_LIGHT; // look for a shader that supports lights
			}
		}
	}

	SHADERENTRY *pVertexShaderEntry = m_pShaderPath->GetVertexShaderMatch( flags );
	if (pVertexShaderEntry)
	{
		ApplyShader( pVertexShaderEntry->pShader, pVertexShaderEntry->pShaderDefines, settings, flags );
		m_pLastVertexShader = pVertexShaderEntry->pShader;
	}

	SHADERENTRY *pPixelShaderEntry = m_pShaderPath->GetPixelShaderMatch( flags );
	if (pPixelShaderEntry)
	{
		ApplyShader( pPixelShaderEntry->pShader, pPixelShaderEntry->pShaderDefines, settings, flags );
		m_pLastPixelShader = pPixelShaderEntry->pShader;
	}

	if ((flags & (SHADOW_CAST_HEMI | SHADOW_CAST_OMNI | SHADOW_CAST_DIR)) == 0)
	{
		if (flags & SHADER_RF_GLOW)
			m_Renderer->SetColorMask( true, true, true, true );
		else
			m_Renderer->SetColorMask( true, true, true, false );
	}

	// set up the textures
	if( mat )
	{
		ApplyConstants( mat );
		SetRenderStates( pass );
		
		if (settings)
		{
			float et = settings->GetElapsedTime();
			mat->SetTextures( pass, et );
		}
		else
		{
			mat->SetTextures( pass );
		}
		//check for empty material textures, default them using effect
		for( int i = 0 ; i < (int)GetNumTextures( pass ); i++ )
		{
			if( mat->GetTexture(pass, i ) == NULL )
			{
				//set textures for these using defaults form effect instead
				IBaseTextureObject * texture = NULL;
				texture = GetTexture( pass, i );
				m_Renderer->SetTexture( i, texture );
			}
		}
	}
	else
	{	
		ApplyConstants( NULL );
		SetRenderStates( pass );
		SetTextures( pass );
	}
}

void CEffect::ApplyConstants( IMaterial * mat )
{
	static CShaderManager * shaderMan = SINGLETONINSTANCE(  CShaderManager );

	//Set some constants
	static Vec4 vConstantData[16]; // TODO: use max( d3dCaps.MaxVertexShaderConst, d3dCaps.MaxPixelShaderConst ) for a max size
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
			if( r.callback != NULL )
			{
				int iConstants = r.callback->GetValue( vConstantData );
				SetVertexConstant( r.uregister, (float*)vConstantData, iConstants );
			}
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
			
			if( r.callback )
			{
				int iConstants = r.callback->GetValue( vConstantData );
				SetPixelConstant( r.uregister, (float*)vConstantData, iConstants );
			}
			else
			{
				SetPixelConstant( r.uregister, r.curvalue[0], r.curvalue[1], r.curvalue[2], r.curvalue[3]);
			}
		}
	}
}

/// Shader Matrix Relevant stuff	
void CEffect::SetTextures( UINT pass )
{
	//sets any textures
	if( pass >= 0 && pass < m_DefaultTextures.size() )
	{
		UINT i = 0;
		for( ; i < m_DefaultTextures[ pass ].size(); i++ )
		{
			//set textures for these
			IBaseTextureObject * texture = GetTexture(pass, i);
			if( texture )//no texture, try to see if the effect has a default one
			{
				m_Renderer->SetTexture( i, texture );
			}
		}
	}
}

void CEffect::SetRenderStates( UINT pass )
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
}

IBaseTextureObject * CEffect::GetTexture( UINT pass, UINT stage )
{
	if( pass >= 0 &&
		pass < m_DefaultTextures.size() )
	{
		if( stage >= 0 && 
			stage < m_DefaultTextures[ pass ].size() )
		{
			TextureVariable &tv = m_DefaultTextures[ pass ][ stage ];
			if (tv.type == m_hszConstantID)
				return tv.tex;
			else if (tv.type == m_hszFunctionID)
			{
				CHashString hszTextureCallbackName( m_ToolBox->GetHashString(tv.texnameID) );
				TEXTUREOBJECTPARAMS top;
				top.bLoad = false;
				top.Name = &hszTextureCallbackName;
				static DWORD msgHash_GetCallbackTexture = CHashString(_T("GetCallbackTexture")).GetUniqueID();
				if (m_ToolBox->SendMessage(msgHash_GetCallbackTexture, sizeof(top), &top ) == MSG_HANDLED)
				{
					return top.TextureObjectInterface;
				}
			}
		}
	}
	return NULL;
}

UINT CEffect::GetNumTextures( UINT pass )
{
	return m_NumTextures;
}

UINT CEffect::GetNumPass()
{
	return m_NumPasses;
}

bool CEffect::Reload()
{
	if (m_pShaderPath)
		m_pShaderPath->Reload();

	return true;
}

bool CEffect::HasAlpha()
{
	return m_bHasAlpha;
}
bool CEffect::SetPerFrameCallback( IShaderCallback * callback )
{
	m_FrameCallback = callback;
	return true;
}
bool CEffect::SetPerObjectCallback( IShaderCallback * callback )
{
	m_ObjectCallback = callback;
	return true;
}
IShaderCallback * CEffect::GetFrameCallback()
{
	return m_FrameCallback;
}
IShaderCallback * CEffect::GetObjectCallback()
{
	return m_ObjectCallback;
}

bool  CEffect::SetVertexConstant( DWORD reg, float * data, int num )
{
	if (m_pLastVertexShader)
	{
		m_pLastVertexShader->SetConstant( reg, data, num );
		return true;
	}
	return false;
}


bool CEffect::SetVertexConstant( DWORD reg, float x, float y, float z, float w )
{
	if (m_pLastVertexShader)
	{
		m_pLastVertexShader->SetConstant( reg, x, y, z, w );
		return true;
	}
	return false;
}

bool  CEffect::SetPixelConstant( DWORD reg, float * data, int num )
{
	if (m_pLastPixelShader)
	{
		m_pLastPixelShader->SetConstant( reg, data, num );
		return true;
	}
	return false;
}

bool CEffect::SetPixelConstant( DWORD reg, float x, float y, float z, float w )
{
	if (m_pLastPixelShader)
	{
		m_pLastPixelShader->SetConstant( reg, x, y, z, w );
		return true;
	}
	return false;
}


void CEffect::SetTexture( UINT pass, UINT stage, IBaseTextureObject * texture )
{
	SetTexture( pass, stage, m_hszConstantID, NULL, texture );
}

void CEffect::SetTexture( UINT pass, UINT stage, DWORD type, IHashString *pFileName, IBaseTextureObject * texture )
{
	UINT numpasses = (UINT)m_DefaultTextures.size();
	//reason for this way of resize is I don't want to delete the current contents
	//resize might do this?
	while( pass+1 > numpasses )
	{
		TEXTUREVAR_VEC newstage;
		m_DefaultTextures.push_back( newstage );
		numpasses = (UINT)m_DefaultTextures.size();
	}	

	UINT numstages = (UINT)m_DefaultTextures[ pass ].size();	
	while( stage + 1 > numstages )
	{
		TextureVariable tv;
		tv.tex = NULL;
		tv.texnameID = 0;
		tv.type = m_hszConstantID;
		m_DefaultTextures[ pass ].push_back( tv );
		numstages = (UINT)m_DefaultTextures[ pass ].size();
	}

	UINT numpasses2 = (UINT)m_SamplerStates.size();
	while( pass+1 > numpasses2 )
	{
		vector< vector< ShaderSamplerState > > newsamplerstage;
		m_SamplerStates.push_back( newsamplerstage );
		numpasses2 = (UINT)m_SamplerStates.size();
	}	

	UINT numstages2 = (UINT)m_SamplerStates[ pass ].size();	
	while( stage + 1 > numstages2 )
	{
		vector< ShaderSamplerState > state;
		m_SamplerStates[ pass ].push_back( state );
		numstages2 = (UINT)m_SamplerStates[ pass ].size();
	}

	m_DefaultTextures[ pass ][ stage ].type = type;
	m_DefaultTextures[ pass ][ stage ].texnameID = (pFileName) ? pFileName->GetUniqueID() : 0;
	m_DefaultTextures[ pass ][ stage ].tex = texture;
}

void CEffect::SetTextureStageFlags( UINT pass, UINT stage, ENUMSAMPLERSTATE mode, TEXTURESTAGEPARAM value )
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
		UINT numpasses2 = (UINT)m_SamplerStates.size();
		while( pass+1 > numpasses2 )
		{
			vector< vector< ShaderSamplerState > > newsamplerstage;
			m_SamplerStates.push_back( newsamplerstage );
			numpasses2 = (UINT)m_SamplerStates.size();
		}	
		UINT numstages2 = (UINT)m_SamplerStates[ pass ].size();	
		while( stage + 1 > numstages2 )
		{
			vector< ShaderSamplerState > state;
			m_SamplerStates[ pass ].push_back( state );
			numstages2 = (UINT)m_SamplerStates[ pass ].size();
		}
		//filter
		ShaderSamplerState nsampler;
		nsampler.m_Mode = mode;		
		nsampler.m_Value = value;
		m_SamplerStates[ pass ][ stage ].push_back( nsampler );
	}
}

bool CEffect::SetVertexShaderVariable( IHashString * name, IHashString * type, UINT uregister,  float x, float y, float z, float w )
{
	if( name )
	{
		DWORD fuid = m_hszConstantID;
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
		GetVShaderCallback( uid );
		return true;
	}
	return false;
}


IVertexFormat * CEffect::GetVertexFormat()
{
	return (IVertexFormat*)m_VertexFormat;
}

void CEffect::SetShaderPath( IShaderPath *pShaderPath )
{
	m_pShaderPath = pShaderPath;
}

bool CEffect::SetPixelShaderVariable( IHashString * name, IHashString * type, UINT uregister,  float x, float y, float z, float w )
{
	if( name )
	{
		DWORD fuid = m_hszConstantID;
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
		GetPShaderCallback( uid );
		return true;
	}
	return false;
}

bool CEffect::SetVertexConstant( IHashString *varName, float * data, int num )
{
	if( varName )
	{
		SHADERVAR_MAP::iterator mapIter = m_VertexShaderVarMap.find(varName->GetUniqueID());
		if (mapIter != m_VertexShaderVarMap.end())
		{
			DWORD reg = mapIter->second.uregister;
			return SetVertexConstant(reg, data, num);
		}
	}
	return false;
}

bool CEffect::SetVertexConstant( IHashString *varName, float x, float y, float z, float w )
{
	if( varName )
	{
		SHADERVAR_MAP::iterator mapIter = m_VertexShaderVarMap.find(varName->GetUniqueID());
		if (mapIter != m_VertexShaderVarMap.end())
		{
			DWORD reg = mapIter->second.uregister;
			return SetVertexConstant(reg, x, y, z, w);
		}
	}
	return false;
}

bool CEffect::SetPixelConstant( IHashString *varName, float * data, int num )
{
	if( varName )
	{
		SHADERVAR_MAP::iterator mapIter = m_PixelShaderVarMap.find(varName->GetUniqueID());
		if (mapIter != m_PixelShaderVarMap.end())
		{
			DWORD reg = mapIter->second.uregister;
			return SetPixelConstant(reg, data, num);
		}
	}
	return false;
}

bool CEffect::SetPixelConstant( IHashString *varName, float x, float y, float z, float w )
{
	if( varName )
	{
		SHADERVAR_MAP::iterator mapIter = m_PixelShaderVarMap.find(varName->GetUniqueID());
		if (mapIter != m_PixelShaderVarMap.end())
		{
			DWORD reg = mapIter->second.uregister;
			return SetPixelConstant(reg, x, y, z, w);
		}
	}
	return false;
}


void CEffect::GetStagePassFromCompression( DWORD compressed, UINT &pass, UINT &stage )
{
	pass = (UINT)( compressed >> 16 );
	stage = (UINT)((compressed << 16) >> 16 );
}

void CEffect::GetVShaderCallback( int uid )
{
	static CShaderManager * pShaderMgr = SINGLETONINSTANCE(  CShaderManager );
	SHADERVARFUNCMESSAGE ShaderVarMsg;

	ShaderVarMsg.uid = uid;
	pShaderMgr->OnGetShaderVarFunc( sizeof(SHADERVARFUNCMESSAGE), &ShaderVarMsg );
	if( ShaderVarMsg.m_ShaderVarFunc )
	{
		m_VertexShaderVarMap[ uid ].callback = ShaderVarMsg.m_ShaderVarFunc;
	}
	else
	{
		m_VertexShaderVarMap[ uid ].callback = NULL;
	}
}

void CEffect::GetPShaderCallback( int uid )
{
	static CShaderManager * pShaderMgr = SINGLETONINSTANCE(  CShaderManager );
	SHADERVARFUNCMESSAGE ShaderVarMsg;

	ShaderVarMsg.uid = uid;
	pShaderMgr->OnGetShaderVarFunc( sizeof(SHADERVARFUNCMESSAGE), &ShaderVarMsg );
	if( ShaderVarMsg.m_ShaderVarFunc )
	{
		m_PixelShaderVarMap[ uid ].callback = ShaderVarMsg.m_ShaderVarFunc;
	}
	else
	{
		m_PixelShaderVarMap[ uid ].callback = NULL;
	}
}

DWORD CEffect::CompressStagePass( int pass, int stage )
{
	DWORD stageAndPass = ((DWORD)(pass << 16)) | ((DWORD)(stage));
	return stageAndPass;
}


IBaseTextureObject * CEffect::GetTexture( IHashString * hash )
{
	if( hash )
	{
		map< DWORD, DWORD >::iterator iter = m_HashToStageAndPassesMap.find( hash->GetUniqueID() );
		if( iter != m_HashToStageAndPassesMap.end() )
		{
			DWORD compressed = iter->second;
			UINT pass, stage;
			GetStagePassFromCompression( compressed, pass, stage );
			return GetTexture( pass, stage );
		}
	}
	return NULL;
}

bool	CEffect::GetTextureChannels( IHashString * hash, UINT &pass, UINT& stage )
{
	if( hash )
	{
		map< DWORD, DWORD >::iterator iter = m_HashToStageAndPassesMap.find( hash->GetUniqueID() );
		if( iter != m_HashToStageAndPassesMap.end() )
		{
			DWORD compressed = iter->second;
			GetStagePassFromCompression( compressed, pass, stage );
			return true;
		}
	}
	return false;
}