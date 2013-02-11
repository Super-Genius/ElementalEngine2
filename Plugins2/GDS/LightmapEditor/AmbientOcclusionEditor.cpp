///============================================================================
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

#include "StdAfx.h"
#include "AmbientOcclusionEditor.h"
#include "AmbientOcclusionVisitor.h"
#include "AmbientOcclusionEditorRenderObject.h"

IComponent *CAmbientOcclusionEditor::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CAmbientOcclusionEditor);
}

REGISTER_COMPONENT_SINGLETON(CAmbientOcclusionEditor);
REGISTER_MESSAGE_HANDLER(GenerateAmbientOcclusion, OnGenerateAmbientOcclusion, CAmbientOcclusionEditor);
REGISTER_MESSAGE_HANDLER(GlobalUpdateTick, OnGlobalUpdateTick, CAmbientOcclusionEditor);

CAmbientOcclusionEditor::CAmbientOcclusionEditor() : 
	m_hszComponentType(_T("CAmbientOcclusionEditor")), 
	m_hszAmbientOcclusionEditorROName(_T("AmbientOcclusionEditorRenderObject")), 
	m_hszAmbientOcclusionEditorROTypeName(_T("CAmbientOcclusionEditorRenderObject"))
{
	m_ToolBox = EngineGetToolBox();
}

CAmbientOcclusionEditor::~CAmbientOcclusionEditor()
{
	CACHEDATLASMAP::iterator itr = m_CachedAtlasMap.begin();
	while (itr != m_CachedAtlasMap.end())
	{
		delete itr->second;
		itr++;
	}
	m_CachedAtlasMap.clear();
}

void CAmbientOcclusionEditor::GenerateHemisphereLightSampleList( LIGHTSAMPLELIST &lightList, int sampleCount, Vec3 point, float radius )
{
	CAmbientOcclusionLight lightSample;

	// special test case
	if (sampleCount == 1)
	{
		lightSample.position.Set( 0, radius, 0 );
		lightList.push_back( lightSample );
	}
	else
	{
		for (int i=0; i < sampleCount; i++)
		{
			float x = (float)(rand()%2000) / (float)2000;
			float y = (float)(rand()%2000) / (float)2000;
			float rx = 90.0f * x;
			float ry = 360.0f * y;

			Matrix3x3 mat;
			mat.SetRotation( EulerAngle( rx, 0, ry ) );
			Vec3 translation( 0, radius, 0 );

			lightSample.position = point + mat * translation;
			lightList.push_back( lightSample );
		}
	}
}

void CAmbientOcclusionEditor::GenerateTargetObjectList( OBJECTLIST &targetList )
{
	// TODO: this is not efficient, just here for testing
	CAmbientOcclusionVisitor visitor;
	visitor.SetList( &targetList );
	visitor.SetSphere( Vec3(0,0,0), 999999.0f );
	visitor.Init();

	IVisitor *vmsg = (IVisitor*)&visitor;
	static DWORD msgHash_VisitQuadTree = CHashString(_T("VisitQuadTree")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_VisitQuadTree, sizeof(IVisitor*), vmsg );
}

void CAmbientOcclusionEditor::GenerateOccluderObjectList( OBJECTLIST &occluderList, Vec3 point, float occlusionRadius )
{
	CAmbientOcclusionVisitor visitor;
	visitor.SetList( &occluderList );
	visitor.SetSphere( point, occlusionRadius );
	visitor.Init();

	IVisitor *vmsg = (IVisitor*)&visitor;
	static DWORD msgHash_VisitQuadTree = CHashString(_T("VisitQuadTree")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_VisitQuadTree, sizeof(IVisitor*), vmsg );
}

bool CAmbientOcclusionEditor::GenerateAtlas( IHashString *pObjectName, IHashString *pParentName, int iTextureResolution )
{
	DWORD fileID = 0;
	bool hasFileID = true;
	static DWORD msgHash_GetModelFileId = CHashString(_T("GetModelFileId")).GetUniqueID();
	if( m_ToolBox->SendMessage( msgHash_GetModelFileId, sizeof(DWORD), &fileID, pObjectName ) != MSG_HANDLED )
	{
		hasFileID = false;
	}
	if (hasFileID)
	{
		// check for cached atlas
		CACHEDATLASMAP::iterator itrCachedAtlas = m_CachedAtlasMap.find(fileID);
		if (itrCachedAtlas != m_CachedAtlasMap.end())
		{
			// use the cached atlas
//			*itrCachedAtlas
			return true;
		}
	}

	// get transform data from object
	Matrix4x4 matObjectTransform;
	matObjectTransform.SetIdentity();
	static CHashString entType(_T("CEntity"));
	static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
	if (m_ToolBox->SendMessage( msgHash_GetGlobalTransform, sizeof(Matrix4x4), &matObjectTransform, pParentName ) != MSG_HANDLED)
		return false;

	// get render buffer data from object
	GETRENDERBUFFERALLOCATIONMSG gga;
	static DWORD msgHash_GetGeometryAllocations = CHashString(_T("GetGeometryAllocations")).GetUniqueID();
	if (m_ToolBox->SendMessage( msgHash_GetGeometryAllocations, sizeof(gga), &gga, pObjectName ) != MSG_HANDLED)
		return false;

	// create mesh param object
	CHashString hszMeshParamTypeName(_T("MeshParameterization"));
	CHashString hszMeshParamName(_T("AmbientOcclusion_MeshParamObj"));
	CREATEOBJECTPARAMS cop;
	cop.name = &hszMeshParamName;
	cop.parentName = NULL;
	cop.typeName = &hszMeshParamTypeName;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	if (m_ToolBox->SendMessage( msgHash_CreateObject, sizeof(cop), &cop ) != MSG_HANDLED)
		return false;

	// generate atlas
	try
	{
		static DWORD msgHash_SetMeshTransform = CHashString(_T("SetMeshTransform")).GetUniqueID();
		if (m_ToolBox->SendMessage( msgHash_SetMeshTransform, sizeof(Matrix4x4), &matObjectTransform, &hszMeshParamName, &hszMeshParamTypeName ) != MSG_HANDLED)
			throw false;

		static DWORD msgHash_SetNoClone = CHashString(_T("SetNoClone")).GetUniqueID();
		if (m_ToolBox->SendMessage( msgHash_SetNoClone, 0, NULL, &hszMeshParamName, &hszMeshParamTypeName ) != MSG_HANDLED)
			throw false;

		if (gga.m_GeometryAllocationsOut.size() > 0)
		{
			// add vertex and index data of first geometry allocation
			RENDERBUFFERALLOCATION *pAVIS = &gga.m_GeometryAllocationsOut[0];
			static DWORD msgHash_AddVertexIndexSoup = CHashString(_T("AddVertexIndexSoup")).GetUniqueID();
			if (m_ToolBox->SendMessage( msgHash_AddVertexIndexSoup, sizeof(RENDERBUFFERALLOCATION), pAVIS, &hszMeshParamName, &hszMeshParamTypeName ) != MSG_HANDLED)
				throw false;
			// for the rest of the geometry allocations, the vertex data is redundant so just add index data
			for (unsigned int i=1; i < gga.m_GeometryAllocationsOut.size(); i++)
			{
				RENDERBUFFERALLOCATION *pAIS = &gga.m_GeometryAllocationsOut[i];
				static DWORD msgHash_AddIndexSoup = CHashString(_T("AddIndexSoup")).GetUniqueID();
				if (m_ToolBox->SendMessage( msgHash_AddIndexSoup, sizeof(RENDERBUFFERALLOCATION), pAIS, &hszMeshParamName, &hszMeshParamTypeName ) != MSG_HANDLED)
					throw false;
			}
		}
		else
		{
			// no geometry
			throw false;
		}

		DWORD sts = iTextureResolution;
		static DWORD msgHash_SetTextureSize = CHashString(_T("SetTextureSize")).GetUniqueID();
		if (m_ToolBox->SendMessage( msgHash_SetTextureSize, sizeof(sts), &sts, &hszMeshParamName, &hszMeshParamTypeName ) != MSG_HANDLED)
			throw false;

		GENERATEATLASMESSAGE ga;
		ga.useIncomingTexCoords = false;
		static DWORD msgHash_GenerateAtlas = CHashString(_T("GenerateAtlas")).GetUniqueID();
		if (m_ToolBox->SendMessage( msgHash_GenerateAtlas, sizeof(ga), &ga, &hszMeshParamName, &hszMeshParamTypeName ) != MSG_HANDLED)
			throw false;

		SETLIGHTMAPDATAMSG lightmapDataMsg;
		lightmapDataMsg.pTexture = NULL;
		static DWORD msgHash_GetCache = CHashString(_T("GetCache")).GetUniqueID();
		if (m_ToolBox->SendMessage( msgHash_GetCache, sizeof(lightmapDataMsg), &lightmapDataMsg, &hszMeshParamName, &hszMeshParamTypeName ) != MSG_HANDLED)
			throw false;

		static DWORD msgHash_SetLightmapData = CHashString(_T("SetLightmapData")).GetUniqueID();
		if (m_ToolBox->SendMessage( msgHash_SetLightmapData, sizeof(lightmapDataMsg), &lightmapDataMsg, pObjectName ) != MSG_HANDLED)
			throw false;

/*
		vector<ParameterizationVertex> *pVertexList = new vector<ParameterizationVertex>;
		GETPARAMETERIZEDVERTICESMSG gcm;
		gcm.outList = pVertexList;
		if (m_ToolBox->SendMessage( _T("GetCollapsedMesh"), sizeof(gcm), &gcm, &hszMeshParamName ) != MSG_HANDLED)
		{
			delete pVertexList;
			throw false;
		}
*/
	} // try
	catch (bool bResult)
	{
		bResult = false; // don't know what we should do here, but quiet the compiler warning
	}

	// remove mesh param object
	DELETEOBJECTPARAMS dop;
	dop.name = &hszMeshParamName;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_DeleteObject, sizeof(dop), &dop );

	if (hasFileID)
	{
		// add to cache
//		m_CachedAtlasMap[fileID] = pVertexList;
		m_CachedAtlasMap[fileID] = NULL;
	}

	return true;
}

DWORD CAmbientOcclusionEditor::OnGlobalUpdateTick(DWORD size, void *params)
{
    VERIFY_MESSAGE_SIZE(size, sizeof(GLOBALTICKPARAMS));
/*
	CHashString hszOutputDir(_T("maps/lightmaps/"));
	static DWORD msgHash_SetLightmapOutputDirectory = CHashString(_T("SetLightmapOutputDirectory")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetLightmapOutputDirectory, sizeof(IHashString), &hszOutputDir );

	IComponent *pComponent = m_ToolBox->CreateComponent(&m_hszAmbientOcclusionEditorROTypeName, 2, &m_hszAmbientOcclusionEditorROName, NULL );
	CAmbientOcclusionEditorRenderObject *pObject = dynamic_cast<CAmbientOcclusionEditorRenderObject*>(pComponent);
	if( !pObject  )
	{
		m_ToolBox->Log( LOGERROR, _T("Could not create ambient occlusion generator\n"));
	}

	pObject->GenerateAmbientOcclusion();
	delete pObject;
*/

	return MSG_HANDLED_PROCEED;
}

DWORD CAmbientOcclusionEditor::OnGenerateAmbientOcclusion( DWORD size, void *params )
{
	CHashString hszOutputDir(_T("maps/lightmaps/"));
	static DWORD msgHash_SetLightmapOutputDirectory = CHashString(_T("SetLightmapOutputDirectory")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetLightmapOutputDirectory, sizeof(IHashString), &hszOutputDir );

	IComponent *pComponent = m_ToolBox->CreateComponent(&m_hszAmbientOcclusionEditorROTypeName, 2, &m_hszAmbientOcclusionEditorROName, NULL );
	CAmbientOcclusionEditorRenderObject *pObject = dynamic_cast<CAmbientOcclusionEditorRenderObject*>(pComponent);
	if( !pObject  )
	{
		m_ToolBox->Log( LOGERROR, _T("Could not create ambient occlusion generator\n"));
	}

	pObject->GenerateAmbientOcclusion();

	delete pObject;
/*	CREATEOBJECTPARAMS cop;
	cop.name = &m_hszAmbientOcclusionEditorROName;
	cop.typeName = &m_hszAmbientOcclusionEditorROTypeName;
	cop.parentName = NULL;
	m_ToolBox->SendMessage(_T("CreateObject"), sizeof(cop), &cop);

	m_ToolBox->SendMessage(_T("GenerateAmbientOcclusion"), 0, NULL, &m_hszAmbientOcclusionEditorROName);

	DELETEOBJECTPARAMS dop;
	dop.name = &m_hszAmbientOcclusionEditorROName;
	m_ToolBox->SendMessage(_T("DeleteObject"), sizeof(dop), &dop);
*/
	return MSG_HANDLED_PROCEED;
}




IComponent *CAmbientOcclusionEditorRenderObject::Create( int nArgs, va_list argptr )
{
	IObject *self;
	IHashString *name = va_arg(argptr, IHashString *);
	IHashString *parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		self = new CAmbientOcclusionEditorRenderObject(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}
