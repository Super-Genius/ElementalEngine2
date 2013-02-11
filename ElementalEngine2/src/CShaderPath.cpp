///============================================================================
/// \file	CShaderPath.cpp
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
#include "CShaderPath.h"

REGISTER_COMPONENT(CShaderPath);

map< DWORD, DWORD > CShaderPath::gs_FlagHash;

CShaderPath::CShaderPath(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE( CShaderManager, CShaderPath, IShaderPath, parentName, name )
{
	
	ADDSHADERPATH addmsg;
	addmsg.m_Name = GetName();
	static DWORD msgHash_AddShaderPath = CHashString(_T("AddShaderPath")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddShaderPath, sizeof( ADDSHADERPATH ), &addmsg );
	
	DeInit();
}

SHADERENTRY* CShaderPath::GetVertexShaderMatch( SHADER_RENDER_FLAG flags )
{
	return GetShaderMatch( VERTEXSHADERTYPE, flags );
}

SHADERENTRY* CShaderPath::GetPixelShaderMatch( SHADER_RENDER_FLAG flags )
{
	return GetShaderMatch( PIXELSHADERTYPE, flags );
}

SHADERENTRY* CShaderPath::GetShaderMatch( int type, SHADER_RENDER_FLAG flags )
{
	SHADER_RENDER_FLAG BestMatch = flags;
	SHADERMATRIX::iterator itrMatrix = m_ShaderMatrix[ type ].find( flags );
	if( itrMatrix == m_ShaderMatrix[ type ].end() )
	{
		//flag was not found in the matrix, select best match and cache it
		BestMatch = GetBestMatch( flags, type );
		assert( BestMatch != -1 );
		if( BestMatch != -1 )
		{
			m_ShaderMatrix[ type ][ flags ] = BestMatch;
		}
		else
			return NULL;
	}
	else
		BestMatch = itrMatrix->second;

	SHADERENTRYMAP::iterator itrEntry = m_ShaderEntries[type].find( BestMatch );
	if (itrEntry != m_ShaderEntries[type].end())
	{
		SHADERENTRY *pShaderEntry = &itrEntry->second;
		return pShaderEntry;
	}

	return NULL;
}

bool CShaderPath::Reload()
{
	for( int i = 0; i < SHADERTYPEMAX; i++ )
	{
		SHADERENTRYMAP::iterator itrEntry = m_ShaderEntries[i].begin();
		while (itrEntry != m_ShaderEntries[i].end())
		{
			IShader *pShader = itrEntry->second.pShader;
			if (pShader)
			{
				pShader->Reload( itrEntry->second.pShaderDefines );
			}

			++itrEntry;
		}
	}

	return true;
}

/// IObject  functions
/// \brief	Create the a instance of IObject
/// \return	IComponent pointer
IComponent * CShaderPath::Create(int nArgs, va_list argptr)
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
		self = (IObject*)new CShaderPath(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

SHADER_RENDER_FLAG CShaderPath::GetBestMatch( SHADER_RENDER_FLAG flags, int shadertype )
{
	static DWORD maskGroups[] = {
		(LIGHT_RECEIVE_HEMI|LIGHT_RECEIVE_OMNI|LIGHT_RECEIVE_DIR), 
		(SHADOW_RECEIVE_HEMI|SHADOW_RECEIVE_OMNI|SHADOW_RECEIVE_DIR), 
		(SHADOW_CAST_HEMI|SHADOW_CAST_OMNI|SHADOW_CAST_DIR), 
		(BONES_0|BONES_1|BONES_2|BONES_3|BONES_4), 
		0
	};
	DWORD maskGroupAll = 0;
	for (int group=0; maskGroups[group]; group++)
		maskGroupAll |= maskGroups[group];

	int maxMatch = -1;
	SHADER_RENDER_FLAG curmatch = -1;
	//loop through these find best match
	size_t size = m_OriginalPaths[ shadertype ].size();
	for( size_t i = 0; i < size; i++ )
	{
		//BIT MANIPULATION STUFF
		DWORD x = flags;
		DWORD y = m_OriginalPaths[ shadertype ][ i ];
		//get best match from this one
		DWORD t = ~(x^y);
		int matches = 0;
		for(int b = 0; b < SHADER_RENDER_FLAG_MAX; b++)
		{
			DWORD mask = ( 0x01 << b );

			// special cases
			if (mask & maskGroupAll)
				continue;

			matches += ( t & mask ) ? 1 : 0;
		}

		// score special mask groups
		for (int group=0; maskGroups[group]; group++)
		{
			// if no support is required in x and none is given in y for entire group mask
			if (((y | x) & maskGroups[group]) == 0)
				matches++;
			// if y fulfils at least one requirement in x regarding mask group
			if ((y & x) & maskGroups[group])
				matches++;
		}

		if( matches > maxMatch || (curmatch == (SHADER_RENDER_FLAG)-1))
		{
			curmatch = y;
			maxMatch = matches;
		}
	}
	return curmatch;
}

void CShaderPath::RecursiveShaderLoad( SHADERENTRY *pShaderEntry, DWORD iDefineCount )
{
	if (iDefineCount == 0)
		return;

	int iDefine = iDefineCount - 1;

	int iValueMin=0, iValueMax=0;
	pShaderEntry->pShaderDefines->GetDefineRange( iDefine, iValueMin, iValueMax );
	for (int i=iValueMin; i <= iValueMax; i++)
	{
		pShaderEntry->pShaderDefines->SetDefineValue( iDefine, i );

		RecursiveShaderLoad( pShaderEntry, iDefineCount - 1 );

		pShaderEntry->pShader->LoadVariant( pShaderEntry->pShaderDefines );
	}
}

/// \brief	The Initialization for the Object
/// \param	IManager* pManager: Parent Manager
void  CShaderPath::Init()
{
	CREATESHADERINTERFACE csi;
	for( int i = 0; i < SHADERTYPEMAX; i++ )
	{
		SHADERENTRYMAP::iterator itrEntry = m_ShaderEntries[i].begin();
		while (itrEntry != m_ShaderEntries[i].end())
		{
			SHADER_RENDER_FLAG flags = itrEntry->first;
			SHADERENTRY &shaderEntry = itrEntry->second;

			// initialize shader matrix with identity
			m_ShaderMatrix[ i ][flags] = flags;
			m_OriginalPaths[ i ].push_back( flags );

			CHashString hszShaderName( m_ToolBox->GetHashString(shaderEntry.hszShaderName) );
			csi.m_Name = &hszShaderName;
			csi.m_ShaderInterface = NULL;
			static DWORD msgHash_GetShader = CHashString(_T("GetShader")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetShader, sizeof(csi), &csi);
			if( csi.m_ShaderInterface )
			{
				shaderEntry.pShader = csi.m_ShaderInterface;

				if (shaderEntry.pShaderDefines)
					RecursiveShaderLoad( &shaderEntry, shaderEntry.pShaderDefines->GetDefineCount() );
			}
			else
			{
				m_ToolBox->Log( LOGERROR, _T("Could not find shader %s in Shader Path %s\n"), hszShaderName.GetString(), GetName()->GetString() );
			}

			++itrEntry;
		}
	}

	// if this shader path's renderer guid is the same as the current renderer guid
	GETRENDERERINTERFACEPARAMS renderInterface;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface);
	IRenderer *pRenderer = renderInterface.m_RendererInterface;
	assert( pRenderer != NULL );

	DWORD shaderPathGUID = pRenderer->ConstructAPIGUID( (const TCHAR*)m_API, (const TCHAR*)m_ShaderVersion, "" );
	DWORD currentGUID = pRenderer->GetRendererAPIGUID();

	if (shaderPathGUID == currentGUID)
	{
		// set all attached effects to use this path
		for( int i = 0; i < (int)m_Effects.size();i++)
		{
			CHashString effectPath(m_Effects[i]);
			CREATEEFFECTINTERFACE cef;
			cef.m_Name = &effectPath;
			static DWORD msgHash_GetEffectInterface = CHashString(_T("GetEffectInterface")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_GetEffectInterface, sizeof(CREATEEFFECTINTERFACE ), &cef ) == MSG_HANDLED)
			{
				IEffect *pEffect = cef.m_EffectInterface;
				pEffect->SetShaderPath( this );
			}
		}
	}
}

/// \brief	The Update Function for the Object
/// \return	True on sucess, False on faulure
bool  CShaderPath::Update()
{
	return true;
}

/// \brief	The DeInit Function for the Object
void CShaderPath::DeInit()
{
	SHADERDEFINESMAP::iterator itr = m_ShaderDefinesMap.begin();
	while (itr != m_ShaderDefinesMap.end())
	{
		SAFE_DELETE( itr->second );
		itr++;
	}
	m_ShaderDefinesMap.clear();
}

void CShaderPath::HashInitialize()
{
	if( gs_FlagHash.size() <= 0 )
	{
		CHashString hash[ SHADER_RENDER_FLAG_MAX ];
		hash[0].Init(_T("ALPHA")); 
		hash[1].Init(_T("ALPHATEST"));
		hash[2].Init(_T("LIGHT_RECEIVE_HEMI"));
		hash[3].Init(_T("LIGHT_RECEIVE_OMNI"));
		hash[4].Init(_T("LIGHT_RECEIVE_DIR"));
		hash[5].Init(_T("SHADOW_RECEIVE_HEMI"));
		hash[6].Init(_T("SHADOW_RECEIVE_OMNI"));
		hash[7].Init(_T("SHADOW_RECEIVE_DIR"));
		hash[8].Init(_T("SHADOW_CAST_HEMI"));
		hash[9].Init(_T("SHADOW_CAST_OMNI"));
		hash[10].Init(_T("SHADOW_CAST_DIR"));
		hash[11].Init(_T("LIGHTMAP_RECEIVE"));
		hash[12].Init(_T("LIGHTMAP_CAST"));
		hash[13].Init(_T("SPHERICAL_HARMONICS"));
		hash[14].Init(_T("AMBIENT_OCCLUSION"));
		hash[15].Init(_T("HIGH_DYNAMIC_RANGE"));
		hash[16].Init(_T("BONES_0"));
		hash[17].Init(_T("BONES_1"));
		hash[18].Init(_T("BONES_2"));
		hash[19].Init(_T("BONES_3"));
		hash[20].Init(_T("BONES_4"));
		hash[21].Init(_T("FOG"));
		hash[22].Init(_T("SHADOW_CAST"));
		hash[23].Init(_T("SHADOW_RECEIVE"));
		hash[24].Init(_T("AMBIENT_OCCLUSION_CAST"));
		hash[25].Init(_T("AMBIENT_OCCLUSION_RECEIVE"));
		hash[26].Init(_T("SHADER_RF_LIGHT"));
		hash[27].Init(_T("SHADER_RF_GLOW"));

		for( int i = 0; i < SHADER_RENDER_FLAG_MAX; i++ )
		{
			DWORD flag = ( 0x01 << i );
			gs_FlagHash.insert( pair< DWORD, DWORD >( hash[ i ].GetUniqueID(), flag ) );
		}
	}
}
/// \brief either reads or writes the entity information to a file
void  CShaderPath::Serialize(IArchive &ar)
{
	HashInitialize();
	if( ar.IsReading() )
	{
		StdString effectname;
		int numentries;
		int numeffects;
		ar.Read( m_Version, _T("Version"));
		ar.Read( m_API, _T("API"));
		ar.Read( m_ShaderVersion, _T("ShaderVersion"));
		ar.Read( numeffects, _T("numeffects") );
		for( int i = 0; i < numeffects; i++ )
		{			
			ar.Read( effectname, _T("effect"));//effect to assign to
			m_Effects.push_back( effectname );
		}
		ar.Read( numentries, _T("numentries"));
		for( int i = 0; i < numentries; i++ )
		{
			//add an entry
			int numflags;
			StdString type;
			ar.Read( type, _T("type") );
			ar.Read( numflags, _T("numflags"));
			DWORD flags = 0;
			for( int iFlag = 0; iFlag < numflags; iFlag++ )
			{
				StdString curflag;
				ar.Read( curflag );
				CHashString uniqueHash((const TCHAR * )curflag);
				DWORD uh = uniqueHash.GetUniqueID();
				if( gs_FlagHash.find( uh ) != gs_FlagHash.end() )
				{
					flags |= gs_FlagHash[ uh ];
				}
			}

			StdString shaderfile, szShaderDefineFile;
			ar.Read( shaderfile, _T("shaderfile"));
			ar.Read( szShaderDefineFile, _T("ShaderDefineFile"));

			// load the shader defines if it hasn't been loaded yet
			CShaderDefines *pShaderDefines = NULL;
			if (!szShaderDefineFile.empty())
			{
				CHashString hszShaderDefineFile(szShaderDefineFile);
				SHADERDEFINESMAP::iterator itr = m_ShaderDefinesMap.find( hszShaderDefineFile.GetUniqueID() );
				if (itr == m_ShaderDefinesMap.end())
				{
					pShaderDefines = new CShaderDefines;
					pShaderDefines->LoadShaderDefines( szShaderDefineFile );
					m_ShaderDefinesMap[hszShaderDefineFile.GetUniqueID()] = pShaderDefines;
				}
				else
				{
					pShaderDefines = itr->second;
				}
			}

			int iType = VERTEXSHADERTYPE;
			if( type == "pixelshader" )
				iType = PIXELSHADERTYPE;

			g_SerializeData[ iType ].insert( pair< StdString, DWORD >( shaderfile, flags ) );

			// check for shader entries with duplicate flags
			SHADERENTRYMAP::iterator itrShaderEntry = m_ShaderEntries[ iType ].find(flags);
			if (itrShaderEntry == m_ShaderEntries[ iType ].end())
			{
				// add the shader entry
				SHADERENTRY &shaderEntry = m_ShaderEntries[ iType ][flags];
				shaderEntry.hszShaderName = CHashString(shaderfile).GetUniqueID();
				shaderEntry.pShaderDefines = pShaderDefines;
				shaderEntry.pShader = NULL;
			}
			else
			{
				m_ToolBox->Log( LOGWARNING, _T("ShaderPath \"%s\" contains more than one shader entry (%s) with identical flags.\n"), GetName()->GetString(), shaderfile.c_str() );
			}
		}
	}
}
