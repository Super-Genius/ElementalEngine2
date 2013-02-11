///============================================================================
/// \file	CShaderManager.cpp
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

#include "Stdafx.h"
#include "CShaderManager.h"
#include "Vec3.h"
#include "CEffect.h"
#include "CShaderVarFuncs.h"

#include "BaseMessages.h"

REGISTER_COMPONENT_SINGLETON( CShaderManager );

REGISTER_MESSAGE_HANDLER(SetShaderCallBack, OnRegisterShaderCallback, CShaderManager );
REGISTER_MESSAGE_HANDLER(InitializeShaderManager, OnInitShaderManager, CShaderManager );
REGISTER_MESSAGE_HANDLER(RemoveShaderCallBack, OnRemoveShaderCallback, CShaderManager );
REGISTER_MESSAGE_HANDLER(GetMaterialTemplate, OnGetMaterialTemplate, CShaderManager );
REGISTER_MESSAGE_HANDLER(GetEffectInterface, OnGetEffectInterface, CShaderManager );
REGISTER_MESSAGE_HANDLER(GetShader, OnGetShaderInterface, CShaderManager );
REGISTER_MESSAGE_HANDLER(GetShaderPath, OnGetShaderPath, CShaderManager );
REGISTER_MESSAGE_HANDLER(AddShaderPath, OnAddShaderPath, CShaderManager );
REGISTER_MESSAGE_HANDLER(RegisterShaderVarFunc, OnRegisterShaderVarFunc, CShaderManager );
REGISTER_MESSAGE_HANDLER(GetShaderVarFunc, OnGetShaderVarFunc, CShaderManager );
REGISTER_MESSAGE_HANDLER(ReloadAllShaders, OnReloadAllShaders, CShaderManager );
REGISTER_MESSAGE_HANDLER(AddEffect, OnAddEffect, CShaderManager );
REGISTER_MESSAGE_HANDLER(AddMaterial, OnAddMaterial, CShaderManager );
REGISTER_MESSAGE_HANDLER(RemoveMaterial, OnRemoveMaterial, CShaderManager );
REGISTER_MESSAGE_HANDLER(AddMaterialTemplate, OnAddMaterialTemplate, CShaderManager );
REGISTER_MESSAGE_HANDLER(RemoveMaterialTemplate, OnRemoveMaterialTemplate, CShaderManager );
REGISTER_MESSAGE_HANDLER(AddShader, OnAddShader, CShaderManager );
REGISTER_MESSAGE_HANDLER(GetMaterialInterface, OnGetMaterialInterface, CShaderManager );
REGISTER_MESSAGE_HANDLER(LoadShaderPaths, OnLoadShaderPaths, CShaderManager );
REGISTER_MESSAGE_HANDLER(GetAllEffects, OnGetAllEffects, CShaderManager );
REGISTER_MESSAGE_HANDLER(GetShaderDirectory, OnGetShaderDirectory, CShaderManager );
REGISTER_MESSAGE_HANDLER(RegisterTextureCallback, OnRegisterTextureCallback, CShaderManager );
REGISTER_MESSAGE_HANDLER(GetCallbackTexture, OnGetCallbackTexture, CShaderManager );
REGISTER_MESSAGE_HANDLER_PRIORITY( DestroyRenderer, OnDestroyRenderer, CShaderManager, 0x70000000 );

#define TEMPLATE_LIST_FILE "\\templates.mlist"

DWORD CShaderManager::OnGetShaderInterface( DWORD size, void * params )
{
	//creates the new shader from a concrete class
	ElementalEngine::CREATESHADERINTERFACE * inter;
	VERIFY_MESSAGE_SIZE( size, sizeof( CREATESHADERINTERFACE ) );
	inter = ( CREATESHADERINTERFACE* )params;
	if( inter &&
		inter->m_Name)
	{
		IHashString * name = inter->m_Name;		
		StdString sslower = name->GetString();
		if( _tcslen( sslower ) > 0 )
		{
			sslower.MakeSafeFileName();
			CHashString lower( sslower );
			map< DWORD, IShader * >::iterator iter = m_Shaders.find( lower.GetUniqueID() );
			if( iter == m_Shaders.end() )
			{
				//try to load it?
				LOADFILEEXTPARAMS lfep;
				lfep.fileName = (TCHAR *)lower.GetString();
				lfep.bInternalLoad = true;
				static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
				iter = m_Shaders.find( lower.GetUniqueID() );
			}
			if( iter != m_Shaders.end() )
			{
				inter->m_ShaderInterface = (*iter).second;
			}
			return MSG_HANDLED_STOP;
		}	
	}
	if( inter )
	{
		inter->m_ShaderInterface = NULL;
	}
	return MSG_ERROR;
}

DWORD CShaderManager::OnGetEffectInterface( DWORD size, void * params )
{
	//creates the new shader from a concrete class
	ElementalEngine::CREATEEFFECTINTERFACE * inter;
	VERIFY_MESSAGE_SIZE( size, sizeof( CREATEEFFECTINTERFACE ) );
	inter = ( CREATEEFFECTINTERFACE* )params;
	if( inter &&
		inter->m_Name)
	{
		IHashString * name = inter->m_Name;		
		StdString sslower = name->GetString();
		if( _tcslen( sslower ) > 0 )
		{
			sslower.MakeSafeFileName();
			CHashString lower( sslower );
			map< DWORD, IEffect * >::iterator iter = m_Effects.find( lower.GetUniqueID() );
			if( iter == m_Effects.end() )
			{
				//try to load it?
				LOADFILEEXTPARAMS lfep;
				lfep.fileName = (TCHAR *)lower.GetString();
				lfep.bInternalLoad = true;
				static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
				iter = m_Effects.find( lower.GetUniqueID() );
			}
			if( iter != m_Effects.end() )
			{
				inter->m_EffectInterface = (*iter).second;
			}
			return MSG_HANDLED_STOP;
		}	
	}
	if( inter )
	{
		inter->m_EffectInterface = NULL;
	}
	return MSG_ERROR;
}


DWORD CShaderManager::OnGetMaterialInterface( DWORD size, void * params )
{
	//creates the new shader from a concrete class
	ElementalEngine::CREATEMATERIALINTERFACE * inter;
	VERIFY_MESSAGE_SIZE( size, sizeof( CREATEMATERIALINTERFACE ) );
	inter = ( CREATEMATERIALINTERFACE* )params;
	if( inter &&
		inter->m_Name)
	{
		IHashString * name = inter->m_Name;
		StdString slower = name->GetString();
		slower.MakeSafeFileName();
		CHashString lower( (const TCHAR*)slower );
		map< DWORD, IMaterial * >::iterator iter = m_Materials.find( lower.GetUniqueID() );
		if( iter != m_Materials.end() )
		{
			IMaterial* material = iter->second;
			material->IncrementRefCount();
			inter->m_MaterialInterface = material;
		}
		return MSG_HANDLED_STOP;
	}else
	{
		inter->m_MaterialInterface = NULL;
	}
	return MSG_ERROR;
}

// Accepts in a complete directory path, C:\TAS\Shaders\ for example.
void CShaderManager::LoadShaderPaths(const TCHAR * czDirPath)
{
	StdString szBasePath = "";
	szBasePath  += czDirPath;
	StdString szSearchPath = "";
	szSearchPath += czDirPath;
	StdString szLoadPath;

	szSearchPath += _T("*.shp");

	BOOL done = true;

	FINDFILE fData;
	fData.pFileName = (TCHAR*) szSearchPath.c_str();

	static DWORD msgHash_searchFile = CHashString(_T("EnvironmentFindFile")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_searchFile, sizeof(FINDFILE), &fData);

	// start loading only if message was handled
	if( retval == MSG_HANDLED )
	{
		// if the return value is not 'found file' - we or done...
		done = ( fData.Result != FINDFILE_OK );
	}
	
	while (!done)
	{ 
		szLoadPath = "";
		szLoadPath += fData.pFilePath;
		//TODO: add a trailing '\' check if necessary. Are all the valid paths ending with '\'??
		szLoadPath += fData.pFileName;

		LOADFILEEXTPARAMS lfep;
		lfep.fileName = (TCHAR*)((const TCHAR*)szLoadPath);
		lfep.bInternalLoad = true;
		static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
		if (retval != MSG_HANDLED)
		{
			EngineGetToolBox()->SetErrorValue(ERR_FILE_LOAD);
			EngineGetToolBox()->Log(LOGWARNING, _T("Unable to load Shaderpath \"%s\" error %d\n"), 
				fData.pFileName, m_ToolBox->GetErrorValue());
		}
		
		// Search again using the same data
		retval = m_ToolBox->SendMessage(msgHash_searchFile, sizeof(FINDFILE), &fData);

		// if the return value is not 'found file' - than we or done...
		if ( !( fData.Result == FINDFILE_OK ) || retval != MSG_HANDLED )
		{
			done = TRUE;
	    }
	}

	// send a message to close the search
	static DWORD msgHash_CloseSearchFile = CHashString(_T("EnvironmentCloseFileSearch")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_CloseSearchFile, sizeof(FINDFILE), &fData);
} 

DWORD CShaderManager::OnRemoveShaderCallback( DWORD size, void * params )
{
	ADDSHADERCALLBACKMESSAGE * pin;
	VERIFY_MESSAGE_SIZE( size, sizeof( ADDSHADERCALLBACKMESSAGE ) );
	pin = (ADDSHADERCALLBACKMESSAGE*) params;
	if( pin && pin->m_ShaderCallback)
	{
		IHashString * name = pin->m_ShaderCallback->GetCallbackName();
		if( name )
		{
			m_CallbackMapping.erase(  name->GetUniqueID() );
			return MSG_HANDLED_STOP;
		}
	}
	return MSG_NOT_HANDLED;
}

DWORD CShaderManager::OnInitShaderManager(DWORD size, void * params )
{
	return MSG_HANDLED_STOP;
}

DWORD CShaderManager::OnRegisterShaderCallback(DWORD size, void *params)
{
	ADDSHADERCALLBACKMESSAGE * pin;
	VERIFY_MESSAGE_SIZE( size, sizeof( ADDSHADERCALLBACKMESSAGE ) );
	pin = (ADDSHADERCALLBACKMESSAGE*) params;
	if( pin && pin->m_ShaderCallback)
	{
		IHashString * name = pin->m_ShaderCallback->GetCallbackName();
		if( name )
		{			
			//add to list
			m_CallbackMapping.insert( SCALLBACKMAPELEM( name->GetUniqueID(), pin->m_ShaderCallback ) );
			return MSG_HANDLED_STOP;
		}
	}
	return MSG_NOT_HANDLED;
}

DWORD CShaderManager::OnRegisterShaderVarFunc(DWORD size, void *params)
{
	SHADERVARFUNCMESSAGE * pin;
	VERIFY_MESSAGE_SIZE( size, sizeof( SHADERVARFUNCMESSAGE ) );
	pin = (SHADERVARFUNCMESSAGE*) params;
	if( pin && pin->m_ShaderVarFunc)
	{
		RegisterShaderVarFunc( pin->m_ShaderVarFunc );
		return MSG_HANDLED_STOP;
	}
	return MSG_NOT_HANDLED;
}
void CShaderManager::RegisterShaderVarFunc( IShaderVarFunc * func )
{
	IHashString * name = func->GetName();
	if( name )
	{			
		//add to list
		m_VarFuncMapping.insert( SVARFUNCMAPELEM( name->GetUniqueID(),func ) );
	}
}

/// \brief The Default Constructor for CShaderManager
/// \return void
CShaderManager::CShaderManager() : 
	CManager(_T("CShaderManager"), SHADERPRIORITY, 0 )
{
	Init();
}


/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CShaderManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CShaderManager );
}

/// \brief The Default Destructor for CShaderManager
/// \return void
CShaderManager::~CShaderManager( )
{
	DeInit();
	
}

// Initializes the manager
bool CShaderManager::Init()
{
	RegisterCommonVarFuncs();
	return true;
}

void CShaderManager::RegisterCommonVarFuncs()
{	
	static CSinCosTanTime	g_SinCosTanTimeFunc;
	RegisterShaderVarFunc( &g_SinCosTanTimeFunc );

	static CShaderVarFuncEyePos g_ShaderVarFuncEyePos;
	RegisterShaderVarFunc( &g_ShaderVarFuncEyePos );

	static CShaderVarFuncZMinMax g_ShaderVarFuncZMinMax;
	RegisterShaderVarFunc( &g_ShaderVarFuncZMinMax );

	static CShaderVarFuncFogDistance g_ShaderVarFuncFogDistance;
	RegisterShaderVarFunc( &g_ShaderVarFuncFogDistance );

	static CShaderVarFuncFogColor g_ShaderVarFuncFogColor;
	RegisterShaderVarFunc( &g_ShaderVarFuncFogColor );

	static CSceneDepthTextureCallback g_SceneDepthTextureCallback;
	static DWORD msgHash_RegisterTextureCallback = CHashString(_T("RegisterTextureCallback")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterTextureCallback, sizeof(ITextureCallback), &g_SceneDepthTextureCallback);

	static CShaderVarFuncModelToWorld g_ShaderVarFuncModelToWorld;
	RegisterShaderVarFunc( &g_ShaderVarFuncModelToWorld );
}

DWORD CShaderManager::OnGetShaderVarFunc( DWORD size, void * params )
{
	SHADERVARFUNCMESSAGE * pin;
	VERIFY_MESSAGE_SIZE( size, sizeof( SHADERVARFUNCMESSAGE ) );
	pin = (SHADERVARFUNCMESSAGE*) params;
	if( pin && m_VarFuncMapping.size() > 0 )
	{
		pin->m_ShaderVarFunc = NULL;
		if( m_VarFuncMapping.find( pin->uid ) != m_VarFuncMapping.end() )
		{
			pin->m_ShaderVarFunc = m_VarFuncMapping[ pin->uid ];
		}
	}
	return MSG_HANDLED_STOP;
}

DWORD CShaderManager::OnReloadAllShaders( DWORD size, void * params )
{
	map< DWORD, IEffect * >::iterator iter = m_Effects.begin();
	bool retval = true;
	for(;iter != m_Effects.end(); iter++ )
	{
		IEffect * eff = (*iter).second;
		if( !eff->Reload() )
		{
			retval = false;
		}
	}
	if( retval )
	{
		return MSG_HANDLED_STOP;
	}
	else
		return MSG_ERROR;
}


DWORD CShaderManager::OnAddEffect( DWORD size, void * params )
{
	ElementalEngine::CREATEEFFECTINTERFACE * inter;
	VERIFY_MESSAGE_SIZE( size, sizeof( CREATEEFFECTINTERFACE ) );
	inter = ( CREATEEFFECTINTERFACE* )params;
	if( inter &&
		inter->m_Name )
	{
		static CHashString effectName(_T("CEffect"));
		IHashString * name = inter->m_Name;
		//find in list
		//send a dummy file so we get a callback for this first phase	
		IDTOOBJECTMAP::iterator objIter;
		IDTOOBJECTMAP * objMap = GetObjectMap( &effectName  );
		objIter = objMap->find(name->GetUniqueID());
		if ( objIter != objMap->end())
		{
			IEffect *destObject = dynamic_cast<IEffect *>(objIter->second);
			if (destObject != NULL)
			{
				m_Effects.insert( pair< DWORD, IEffect *>( name->GetUniqueID(), destObject ) );
			}
			else
			{
				IHashString *name = destObject->GetName();
				IHashString *typeName = destObject->GetComponentType();
				EngineGetToolBox()->Log(LOGWARNING, 
					_T("%s object of type %s was added as an effect object and is not one.\n"),
					name->GetString(), typeName->GetString());
			}
		}
		else
		{
			m_ToolBox->Log( LOGWARNING, _T("Could not add effect file, %s\n" ), (TCHAR*)name->GetString() );
		}
		return MSG_HANDLED_STOP;
	}else
	{
		inter->m_EffectInterface = NULL;
	}
	return MSG_ERROR;
}

DWORD CShaderManager::OnAddShader( DWORD size, void * params )
{
	ElementalEngine::ADDSHADERTOMANAGER * inter;
	VERIFY_MESSAGE_SIZE( size, sizeof( ADDSHADERTOMANAGER ) );
	inter = ( ADDSHADERTOMANAGER* )params;
	if( inter &&
		inter->m_Name &&
		inter->m_CompType )
	{
		IHashString * name = inter->m_Name;
		//find in list
		//send a dummy file so we get a callback for this first phase	
		IDTOOBJECTMAP::iterator objIter;
		IDTOOBJECTMAP * objMap = GetObjectMap( inter->m_CompType );
		objIter = objMap->find(name->GetUniqueID());
		if (objIter != objMap->end())
		{
			IShader *destObject = dynamic_cast<IShader *>(objIter->second);
			if (destObject != NULL)
			{
				m_Shaders.insert( pair< DWORD, IShader *>( name->GetUniqueID(), destObject ) );
			}
			else
			{
				IHashString *name = destObject->GetName();
				IHashString *typeName = destObject->GetComponentType();
				EngineGetToolBox()->Log(LOGWARNING, 
					_T("%s object of type %s was added as a shader object and is not one.\n"),
					name->GetString(), typeName->GetString());
			}
		}
		else
		{
			m_ToolBox->Log( LOGWARNING, _T("Could not add shader file, %s\n" ), (TCHAR*)name->GetString() );
		}
		return MSG_HANDLED_STOP;
	}
	else
	{
		inter->m_ShaderInterface = NULL;
	}
	return MSG_ERROR;
}

DWORD CShaderManager::OnAddMaterial( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(CREATEMATERIALINTERFACE) );
	ElementalEngine::CREATEMATERIALINTERFACE* inter = (CREATEMATERIALINTERFACE*)params;

	if( inter && inter->m_Name )
	{
		IHashString* name = inter->m_Name;
		//find in list
		//send a dummy file so we get a callback for this first phase	
		IDTOOBJECTMAP::iterator objIter;
		static CHashString matName(_T("CMaterial"));
		IDTOOBJECTMAP * objMap = GetObjectMap( &matName );
		objIter = objMap->find(name->GetUniqueID());
		if (objIter != objMap->end())
		{
			IMaterial *destObject = dynamic_cast<IMaterial *>(objIter->second);
			if (destObject != NULL)
			{
				destObject->IncrementRefCount();
				m_Materials.insert( pair< DWORD, IMaterial *>( name->GetUniqueID(), destObject ) );
			}
			else
			{
				IHashString *name = destObject->GetName();
				IHashString *typeName = destObject->GetComponentType();
				EngineGetToolBox()->Log(LOGWARNING, 
					_T("%s object of type %s was added as a material object and is not one.\n"),
					name->GetString(), typeName->GetString());
			}
		}
		else
		{
			m_ToolBox->Log( LOGWARNING, _T("Could not add material file, %s\n" ), (TCHAR*)name->GetString() );
		}
		return MSG_HANDLED_STOP;
	}
	else
	{
		inter->m_MaterialInterface = NULL;
	}
	return MSG_ERROR;
}

DWORD CShaderManager::OnRemoveMaterial( DWORD size, void *param )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CREATEMATERIALINTERFACE));
	CREATEMATERIALINTERFACE cmi = *(CREATEMATERIALINTERFACE*) param;

	if (cmi.m_Name)
	{
		map< DWORD, IMaterial*>::iterator matIter = m_Materials.find(cmi.m_Name->GetUniqueID());

		if (matIter != m_Materials.end())
		{
			// Kyle: Work in progress, precache database manager superscedes per-object ref counting for now
			//IMaterial* material = dynamic_cast<IMaterial*>( matIter->second );
			
			//if( material->DecrementRefCount() <= 0 )
			//{
				//delete material;
				m_Materials.erase(matIter);
			//}
		}
	}
	return MSG_HANDLED_STOP;
}

DWORD CShaderManager::OnAddShaderPath( DWORD size, void * params )
{
	ElementalEngine::ADDSHADERPATH * inter;
	VERIFY_MESSAGE_SIZE( size, sizeof( ADDSHADERPATH ) );
	inter = ( ADDSHADERPATH* )params;
	if( inter &&
		inter->m_Name )
	{
		IHashString * name = inter->m_Name;
		//find in list
		//send a dummy file so we get a callback for this first phase	
		IDTOOBJECTMAP::iterator objIter;
		static CHashString spName(_T("CShaderPath"));
		IDTOOBJECTMAP * objMap = GetObjectMap( &spName );
		objIter = objMap->find(name->GetUniqueID());
		if( objIter != objMap->end())
		{
			IShaderPath *destObject = dynamic_cast<IShaderPath *>(objIter->second);
			if (destObject != NULL)
			{
				m_ShaderPaths.insert( pair< DWORD, IShaderPath *>( name->GetUniqueID(), destObject ) );
			}
			else
			{
				IHashString *name = destObject->GetName();
				IHashString *typeName = destObject->GetComponentType();
				EngineGetToolBox()->Log(LOGWARNING, 
					_T("%s object of type %s was added as a shader object and is not one.\n"),
					name->GetString(), typeName->GetString());
			}
		}
		else
		{
			m_ToolBox->Log( LOGWARNING, _T("Could not add Shader Path file, %s\n" ), (TCHAR*)name->GetString() );
		}
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}


DWORD CShaderManager::OnGetShaderPath( DWORD size, void * params )
{
	//creates the new shader from a concrete class
	ElementalEngine::GETSHADERPATH * inter;
	VERIFY_MESSAGE_SIZE( size, sizeof( GETSHADERPATH ) );
	inter = ( GETSHADERPATH* )params;
	if( inter &&
		inter->m_Name)
	{
		IHashString * name = inter->m_Name;
		StdString sslower = name->GetString();
		if( _tcslen( sslower ) > 0 )
		{
			sslower.MakeSafeFileName();
			CHashString lower( (const TCHAR*)sslower );
			map< DWORD, IShaderPath * >::iterator iter = m_ShaderPaths.find( lower.GetUniqueID() );
			if( iter == m_ShaderPaths.end() )
			{
				//try to load it?
				LOADFILEEXTPARAMS lfep;
				lfep.fileName = (TCHAR *)lower.GetString();
				lfep.bInternalLoad = true;
				static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
				iter = m_ShaderPaths.find( lower.GetUniqueID() );
			}
			if( iter != m_ShaderPaths.end() )
			{
				inter->m_Path = (*iter).second;
			}
			return MSG_HANDLED_STOP;
		}
	}
	if( inter )
	{
		inter->m_Path = NULL;
	}
	return MSG_ERROR;
}

DWORD CShaderManager::OnGetShaderDirectory( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( StdString ));
	StdString * p = (StdString*) params;
	if( p )
	{
		*p = m_ShaderDirectory;
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

DWORD CShaderManager::OnLoadShaderPaths( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( LOADSHADERDIRECTORY ) );
	LOADSHADERDIRECTORY * p = (LOADSHADERDIRECTORY*)params;
	if( p && p->dir )
	{
		m_ShaderDirectory = p->dir;
#ifdef XBOX
		m_ShaderDirectory = SetPathDrive( m_ShaderDirectory, EngineGetToolBox()->GetDrive() );
#endif
		LoadShaderPaths( m_ShaderDirectory );
		RegisterMaterialTemplates( m_ShaderDirectory );
	}
	return MSG_HANDLED_STOP;
}

DWORD CShaderManager::OnDestroyRenderer(DWORD size, void *data)
{
	DeInit();
	return MSG_HANDLED_PROCEED;
}

DWORD CShaderManager::OnGetMaterialTemplate( DWORD size, void * data )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( MATERIALTEMPLATEINTERFACE ) );
	MATERIALTEMPLATEINTERFACE * p = (MATERIALTEMPLATEINTERFACE*)data;
	if( p )
	{
		if( p->m_Name )
		{
			DWORD hash = p->m_Name->GetUniqueID();
			map< DWORD, IMaterialTemplate * >::iterator iter = m_MaterialTemplates.find( hash );
			if( iter != m_MaterialTemplates.end() )
			{
				p->m_MatInterface = iter->second;
				return MSG_HANDLED_STOP;
			}else
			{
				//try to append a .mtt if they're specifying the name without an extension
				string temp = "";
				temp += p->m_Name->GetString();
				temp += ".mtt";
				CHashString newhash( (TCHAR*) temp.c_str() );
				hash = newhash.GetUniqueID();
				iter = m_MaterialTemplates.find( hash );
				if( iter != m_MaterialTemplates.end() )
				{
					p->m_MatInterface = iter->second;
					return MSG_HANDLED_STOP;
				}
			}
		}
	}
	return MSG_ERROR;
}
DWORD CShaderManager::OnAddMaterialTemplate( DWORD size, void * params )
{
	ElementalEngine::MATERIALTEMPLATEINTERFACE * inter;
	VERIFY_MESSAGE_SIZE( size, sizeof( MATERIALTEMPLATEINTERFACE ) );
	inter = ( MATERIALTEMPLATEINTERFACE* )params;
	if( inter &&
		inter->m_Name )
	{
		IHashString * name = inter->m_Name;
		//find in list
		//send a dummy file so we get a callback for this first phase	
		IDTOOBJECTMAP::iterator objIter;
		static CHashString mtName(_T("CMaterialTemplate"));
		IDTOOBJECTMAP * objMap = GetObjectMap( &mtName );
		objIter = objMap->find( name->GetUniqueID() );
		if (objIter != objMap->end())
		{
			IMaterialTemplate *destObject = dynamic_cast<IMaterialTemplate *>(objIter->second);
			if (destObject != NULL)
			{
				m_MaterialTemplates.insert( pair< DWORD, IMaterialTemplate *>( name->GetUniqueID(), destObject ) );
			}
			else
			{
				IHashString *name = destObject->GetName();
				IHashString *typeName = destObject->GetComponentType();
				EngineGetToolBox()->Log(LOGWARNING, 
					_T("%s object of type %s was added as a material template and is not one.\n"),
					name->GetString(), typeName->GetString());
			}
		}
		else
		{
			m_ToolBox->Log( LOGWARNING, _T("Could not add material template, %s\n" ), (TCHAR*)name->GetString() );
		}
		return MSG_HANDLED_STOP;
	}else
	{
		inter->m_MatInterface = NULL;
	}
	return MSG_ERROR;
}

DWORD CShaderManager::OnRemoveMaterialTemplate( DWORD size, void *param )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(MATERIALTEMPLATEINTERFACE));
	MATERIALTEMPLATEINTERFACE cmi = *(MATERIALTEMPLATEINTERFACE*) param;
	if (cmi.m_Name)
	{
		map< DWORD, IMaterialTemplate*>::iterator matIter = m_MaterialTemplates.find(cmi.m_Name->GetUniqueID());
		if (matIter != m_MaterialTemplates.end())
		{
			m_MaterialTemplates.erase(matIter);
		}
	}
	return MSG_HANDLED_STOP;
}

void CShaderManager::RegisterMaterialTemplates( const TCHAR * ShaderDir )
{
	string TemplateDefinitions = "";
	TemplateDefinitions += ShaderDir;

	StdString szBasePath = "";
	szBasePath  += TemplateDefinitions;
	StdString szSearchPath = "";
	szSearchPath += TemplateDefinitions;
	StdString szLoadPath;

	szSearchPath += _T("*.mtt");

	// variables for searching
	BOOL done = true;
	FINDFILE fData;
	fData.pFileName = (TCHAR*) szSearchPath.c_str();

	// send a message to search - it is in the environment plugin
	static DWORD msgHash_searchFile = CHashString(_T("EnvironmentFindFile")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_searchFile, sizeof(FINDFILE), &fData);

	// start loading only if message was handled
	if( retval == MSG_HANDLED )
	{
		// if the return value is not 'found file' - than we or done...
		done = ( fData.Result != FINDFILE_OK );
	}

	while (!done)
	{ 
		szLoadPath = "";
		szLoadPath += fData.pFilePath;
		szLoadPath += fData.pFileName;
		LOADFILEEXTPARAMS lfep;
		lfep.fileName = (TCHAR*)((const TCHAR*)szLoadPath);
		lfep.bInternalLoad = true;
		static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
		if (retval != MSG_HANDLED)
		{
			EngineGetToolBox()->SetErrorValue(ERR_FILE_LOAD);
			EngineGetToolBox()->Log(LOGWARNING, _T("Unable to load Material Template \"%s\" error %d\n"), 
				fData.pFileName, m_ToolBox->GetErrorValue());
		}

		// Search again using the same data
		retval = m_ToolBox->SendMessage(msgHash_searchFile, sizeof(FINDFILE), &fData);

		// if the return value is not 'found file' - than we or done...
		if ( !( fData.Result == FINDFILE_OK ) || retval != MSG_HANDLED )
		{
			done = TRUE;
	    }
	}

	// send a message to close the search
	static DWORD msgHash_CloseSearchFile = CHashString(_T("EnvironmentCloseFileSearch")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_CloseSearchFile, sizeof(FINDFILE), &fData);
}

DWORD CShaderManager::OnGetAllEffects( DWORD size, void *param )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETEFFECTLIST));
	
	GETEFFECTLIST* tmp = (GETEFFECTLIST*)param;
	if (tmp->m_pEffectList)
	{
		tmp->m_pEffectList->insert( m_Effects.begin(), m_Effects.end() );
	}

	return MSG_HANDLED_STOP;
}

DWORD CShaderManager::OnRegisterTextureCallback(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(ITextureCallback), size);
	ITextureCallback *pCallback = (ITextureCallback*)params;
	m_TextureCallbacks[pCallback->GetName()->GetUniqueID()] = pCallback;
	return MSG_HANDLED_PROCEED;
}

DWORD CShaderManager::OnGetCallbackTexture(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(TEXTUREOBJECTPARAMS), size);
	TEXTUREOBJECTPARAMS *pTOP = (TEXTUREOBJECTPARAMS*)params;
	TEXTURECALLBACKMAP::iterator itr = m_TextureCallbacks.find( pTOP->Name->GetUniqueID() );
	if (itr != m_TextureCallbacks.end())
	{
		pTOP->TextureObjectInterface = itr->second->GetTexture();
		return MSG_HANDLED_STOP;
	}
	else
	{
		pTOP->TextureObjectInterface = NULL;
		return MSG_ERROR;
	}
}

