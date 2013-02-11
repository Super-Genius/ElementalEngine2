///============================================================================
/// \file	Cal3DCoreModelCache.cpp
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT( CCal3DCoreModelCache );
IComponent *CCal3DCoreModelCache::Create(int nArgs, va_list argptr)
{
	IObject *self;
	IHashString *name = va_arg(argptr, IHashString *);
	IHashString *parentName = va_arg(argptr, IHashString *);
	try
	{
		// try and create an instance
		self = new CCal3DCoreModelCache(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CCal3DCoreModelCache::CCal3DCoreModelCache(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE(Cal3DModelManager, CCal3DCoreModelCache, IObject, parentName, name)
{
	m_pModelManager = (Cal3DModelManager*)m_Manager;
}

CCal3DCoreModelCache::~CCal3DCoreModelCache()
{
	DeInit();
}

void CCal3DCoreModelCache::Init()
{
	m_pCoreModel = new CalCoreModel(GetName()->GetString());
	m_pCalHardwareModel = NULL;

	string RelativePath;
	TCHAR drive[MAX_PATH];
	TCHAR directory[MAX_PATH];
	_tsplitpath( GetName()->GetString(), drive, directory, NULL, NULL );
	RelativePath = drive;
	RelativePath += directory;

	int tmpBufferLength = _tcslen( m_szCFG );
	int tmpLength = 0;
	// parse all lines from the model configuration file
	while( tmpLength < tmpBufferLength )
	{
		// read the next model configuration line
		StdString token;
		m_szCFG.GetToken(_T("\n"), token);
		std::string strBuffer = token;
		tmpLength += _tcslen( strBuffer.c_str() ) + 1;
				
		// find the first non-whitespace character
		std::string::size_type pos;
		pos = strBuffer.find_first_not_of(" \t");

		// check for empty lines
		if((pos == std::string::npos) || (strBuffer[pos] == '\n') || (strBuffer[pos] == '\r') || (strBuffer[pos] == 0)) continue;

		// check for comment lines
		if(strBuffer[pos] == '#') continue;

		// get the key
		std::string strKey;
		strKey = strBuffer.substr(pos, strBuffer.find_first_of(" =\t\n\r", pos) - pos);
		pos += strKey.size();

		// get the '=' character
		pos = strBuffer.find_first_not_of(" \t", pos);
		if((pos == std::string::npos) || (strBuffer[pos] != '='))
		{
			m_ToolBox->Log( LOGERROR, _T("Cal3d Load: Syntax Error in %s.\n"), GetName()->GetString() );
			break;
		}

		// find the first non-whitespace character after the '=' character
		pos = strBuffer.find_first_not_of(" \t", pos + 1);

		// get the data
		std::string strData;
		strData = strBuffer.substr(pos, strBuffer.find_first_of("\n\r", pos) - pos);

		// handle the model creation
		if(strKey == "scale")
		{
			// set rendering scale factor
			//float unifiedscaling = (float) atof(strData.c_str());
			//m_Scaling = Vec3( unifiedscaling,unifiedscaling,unifiedscaling );
		}
		else if(strKey == "skeleton")
		{
			string temp = RelativePath;
			temp += strData;
			StdString sslower = (TCHAR*)temp.c_str();
			sslower.MakeSafeFileName();
			CHashString hszFilename( sslower );

			m_hszSkeletonName = hszFilename;

			CalCoreSkeleton *pCoreSkeleton = m_pModelManager->GetCoreSkeleton( &hszFilename );
			if (pCoreSkeleton)
			{
				m_pCoreModel->setCoreSkeleton( pCoreSkeleton );
			}
			else
			{
				m_ToolBox->Log( LOGERROR, _T("Cal3d Load: Bad Skeleton %s.\n"), hszFilename.GetString() );
				break;
			}
		}
		else if(strKey == "animation")
		{
			string temp = RelativePath;
			temp += strData;
			StdString sslower = (TCHAR*)temp.c_str();
			sslower.MakeSafeFileName();
			CHashString hszFilename( sslower );

			CalCoreAnimation *pCoreAnimation = m_pModelManager->GetCoreAnimation( &hszFilename );
			if (pCoreAnimation)
			{
				int AnimationID = m_pCoreModel->addCoreAnimation( pCoreAnimation );
				m_pCoreModel->addAnimationName( strData, AnimationID );
			}
			else
			{
				m_ToolBox->Log( LOGERROR, _T("Cal3d Load: Bad Animation %s.\n"), hszFilename.GetString() );
				break;
			}
		}
		else if(strKey == "mesh")
		{
			string temp = RelativePath;
			temp += strData;
			StdString sslower = (TCHAR*)temp.c_str();
			sslower.MakeSafeFileName();
			CHashString hszFilename( sslower );

			m_hszMeshName = hszFilename;

			CalCoreMesh *pCoreMesh = m_pModelManager->GetCoreMesh( &hszFilename );
			if (pCoreMesh)
			{
				m_pCoreModel->addCoreMesh( pCoreMesh );

				int submeshCount = pCoreMesh->getCoreSubmeshCount();
				for (int submeshId=0; submeshId < submeshCount; submeshId++)
				{   
					CalCoreSubmesh *pCoreSubmesh = pCoreMesh->getCoreSubmesh(submeshId);
					pCoreSubmesh->enableTangents(0, true);
				}
			}
			else
			{
				m_ToolBox->Log( LOGERROR, _T("Cal3d Load: Bad Mesh %s.\n"), hszFilename.GetString() );
				break;
			}
		}
		else if(strKey == "material")
		{
			string temp = RelativePath;
			temp += strData;
			StdString sslower = (TCHAR*)temp.c_str();
			sslower.MakeSafeFileName();
			CHashString hszFilename( sslower );

			LOADFILEEXTPARAMS lfep;
			lfep.fileName = (TCHAR*)sslower.c_str();
			lfep.bInternalLoad = true;
			static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
			DWORD retval = EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
			if( retval == MSG_HANDLED )
			{
				CREATEMATERIALINTERFACE cef;
				cef.m_Name = &hszFilename;
				static DWORD msgHash_GetMaterialInterface = CHashString(_T("GetMaterialInterface")).GetUniqueID();
				EngineGetToolBox()->SendMessage(msgHash_GetMaterialInterface, sizeof(CREATEEFFECTINTERFACE ), &cef );
				IMaterial *mat = cef.m_MaterialInterface;							
				m_Materials.push_back( mat );
				if( !mat )
				{
					m_ToolBox->Log( LOGERROR, _T("Cal3DRenderObject could not load material file %s\n"), lfep.fileName );								
				}
			}
		}
	}

	// Load hardware allocation
	HARDWARE_ALLOCATION hardwareAlloc;
	if (m_pModelManager->LoadHardwareAllocation( 
		m_pCoreModel, 
		&m_hszMeshName, 
		m_pCoreModel->getCoreSkeleton() ? &m_hszSkeletonName : NULL, 
		hardwareAlloc))
	{
		m_VertexBufferAllocation = hardwareAlloc.vertexBufferAllocation;
		m_IndexBufferAllocation = hardwareAlloc.indexBufferAllocation;
		m_pVertexBufferInterface = hardwareAlloc.pVertexBuffer;
		m_pIndexBufferInterface = hardwareAlloc.pIndexBuffer;
		m_pCalHardwareModel = hardwareAlloc.pCalHardwareModel;
	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T("%s failed to build cal hardware model.\n"), GetName()->GetString() );
		DeInit();
	}
}

void CCal3DCoreModelCache::DeInit()
{
	// Unload hardware allocation
	if (m_pCoreModel)
	{
		m_pModelManager->UnloadHardwareAllocation( &m_hszMeshName, 
			m_pCoreModel->getCoreSkeleton() ? &m_hszSkeletonName : NULL );
	}

	m_pCalHardwareModel = NULL;
	SAFE_DELETE( m_pCoreModel );
}

void CCal3DCoreModelCache::Serialize( IArchive &ar )
{
	if (ar.IsReading())
	{
		ar.Read( m_szCFG );
	}
	else
	{
		ar.Write( m_szCFG );
	}
}
