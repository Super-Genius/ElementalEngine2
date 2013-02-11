///============================================================================
/// \file		NavMeshRenderObject.cpp
/// \brief		Implementation file for a Nav Mesh Render Object
/// \date		09-16-2005
/// \author		Brian Bazyk
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNavMeshObject *g_pNavMesh;
MathUtil math;

BYTE g_vNavColorPerimeterEdge[] = { 255, 255, 0 };
BYTE g_vNavColorSubsectorEdge[] = { 255, 0, 0 };
BYTE g_vNavColorInternalEdge[] = { 0, 0, 0 };
BYTE g_vNavColorSectorConnection[] = { 128, 128, 128 };
Vec3 g_NavVertexSize(1,1,1);

REGISTER_COMPONENT(CNavMeshRenderObject);

IComponent *CNavMeshRenderObject::Create(int nArgs, va_list argptr)
{
	IObject *self;

	IHashString *name = va_arg(argptr, IHashString *);
	IHashString *parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = new CNavMeshRenderObject(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CNavMeshRenderObject::CNavMeshRenderObject(IHashString *parentName, IHashString *name) : 
	m_hszNameType( _T("CNavMeshRenderObject") ),
	SCENEOBJECTTEMPLATE(CNavMeshManager, CNavMeshRenderObject, IRenderObject, parentName, name)
{
	m_iClosestVertexToMouseRay = INVALID_NAVINDEX;
}

CNavMeshRenderObject::~CNavMeshRenderObject()
{
	DeInit();
}

bool CNavMeshRenderObject::Update()
{
	return true;
}

void CNavMeshRenderObject::DeInit()
{
	RemoveBoundingObject();

	static DWORD msgHash_ParentRemoveBoundingBox = CHashString(_T("ParentRemoveBoundingBox")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_ParentRemoveBoundingBox, 0, NULL, &m_ParentName);
}

void CNavMeshRenderObject::Init()
{
	UpdateGlobalObject();
}

void CNavMeshRenderObject::Serialize(IArchive &ar)
{
}

// UNDONE: add lines to show adjacent subsectors, just draw arrows or something so we can see if they are mutually adjacent
// or if something is broken

bool CNavMeshRenderObject::Render( UINT pass, IEffect * override )
{
	if (pass != 0)
		return false;

	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam );
	IRenderer *pRenderer = renderInterfaceParam.m_RendererInterface;
	if (!pRenderer)
		return false;

	if (!g_pNavMesh)
		return false;

	for (int i=0; i < g_pNavMesh->GetVertexCount(); i++)
	{
//		pRenderer->DrawBoundingBox( g_pNavMesh->GetVertex(i)->m_Position-g_NavVertexSize, g_pNavMesh->GetVertex(i)->m_Position+g_NavVertexSize, 0, 255, 255, 255 );
	}

	for (int i=0; i < g_pNavMesh->GetSubsectorCount(); i++)
	{
		CNavSubsector *pSubsector = g_pNavMesh->GetSubsector(i);
		Vec3 subsectorCenter = pSubsector->GetCenter();

		for (int j=0; j < 3; j++)
		{
			CNavSubsector *pAdjSubsector = g_pNavMesh->GetSubsector( pSubsector->m_AdjacentSubsectors[j] );
			if (!pAdjSubsector)
			{
				NAVINDEX v0, v1;
				pSubsector->GetEdgeVertices( j, v0, v1 );
				pRenderer->Draw3DLine( 
					g_pNavMesh->GetVertex( v0 )->m_Position.x,
					g_pNavMesh->GetVertex( v0 )->m_Position.y, 
					g_pNavMesh->GetVertex( v0 )->m_Position.z, 
					g_pNavMesh->GetVertex( v1 )->m_Position.x, 
					g_pNavMesh->GetVertex( v1 )->m_Position.y, 
					g_pNavMesh->GetVertex( v1 )->m_Position.z, 
					g_vNavColorPerimeterEdge[0], g_vNavColorPerimeterEdge[1], g_vNavColorPerimeterEdge[2], 1.0f );
			}
			else
			{
				Vec3 dir = (pAdjSubsector->GetCenter() - subsectorCenter) * 0.5f + subsectorCenter;
				pRenderer->Draw3DLine( 
					subsectorCenter.x,
					subsectorCenter.y, 
					subsectorCenter.z, 
					dir.x, 
					dir.y, 
					dir.z, 
					g_vNavColorSectorConnection[0], g_vNavColorSectorConnection[1], g_vNavColorSectorConnection[2], 1.0f );

				NAVINDEX v0, v1;
				pSubsector->GetEdgeVertices( j, v0, v1 );
				pRenderer->Draw3DLine( 
					g_pNavMesh->GetVertex( v0 )->m_Position.x,
					g_pNavMesh->GetVertex( v0 )->m_Position.y, 
					g_pNavMesh->GetVertex( v0 )->m_Position.z, 
					g_pNavMesh->GetVertex( v1 )->m_Position.x, 
					g_pNavMesh->GetVertex( v1 )->m_Position.y, 
					g_pNavMesh->GetVertex( v1 )->m_Position.z, 
					g_vNavColorSubsectorEdge[0], g_vNavColorSubsectorEdge[1], g_vNavColorSubsectorEdge[2], 1.0f );
			}
		}
	}

	float width;
	for (int i=0; i < g_pNavMesh->GetEdgeCount(); i++)
	{
		BYTE *pColor = NULL;

		CNavEdge *pEdge = g_pNavMesh->GetEdge(i);
		// edge color
		if (pEdge->m_Flags & NAVFLAG_EDGE_PERIMETER) {
			pColor = g_vNavColorPerimeterEdge;
			width = 2.0f;
		}
		else if (pEdge->m_Flags & NAVFLAG_EDGE_SECTOR) {
			pColor = g_vNavColorSubsectorEdge;
			width = 1.5f;
		}
		else {
			pColor = g_vNavColorInternalEdge;
			width = 12.0f;
		}

		Vec3 positionA = g_pNavMesh->GetVertex(pEdge->m_vVertices[0])->m_Position;
		Vec3 positionB = g_pNavMesh->GetVertex(pEdge->m_vVertices[1])->m_Position;

		pRenderer->Draw3DLine( 
			positionA.x, 
			positionA.y, 
			positionA.z, 
			positionB.x, 
			positionB.y, 
			positionB.z, 
			pColor[0], pColor[1], pColor[2], 
			width );
	}

	float fColorLerp = 0;
	float fTime;
	static DWORD msgHash_GetTime = CHashString(_T("GetTime")).GetUniqueID();
	if (m_ToolBox->SendMessage( msgHash_GetTime, sizeof(float), &fTime ) == MSG_HANDLED)
	{
		fColorLerp = (math.GetSin(fTime*4.0f) + 1.0f) * 0.5f;
	}
	if (m_iClosestVertexToMouseRay != INVALID_NAVINDEX)
	{
		BYTE glow = (BYTE)(255.0f*fColorLerp);
		CNavVertex *vecCenter = g_pNavMesh->GetVertex(m_iClosestVertexToMouseRay);
		Vec3 vecNear = vecCenter->m_Position-g_NavVertexSize;
		Vec3 vecFar = vecCenter->m_Position+g_NavVertexSize;
		pRenderer->DrawBoundingBox( vecNear, vecFar, 0, 255, (int)glow, (int)glow );
	}

	return true;
}

bool CNavMeshRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CNavMeshRenderObject::GetRenderPriority()
{
	return 0;
}

void CNavMeshRenderObject::SetPosition( const Vec3 &vec )
{
}

Vec3 &CNavMeshRenderObject::GetPosition()
{
	static Vec3 pos(0, 0, 0);
	return pos;
}

IMeshObject* CNavMeshRenderObject::GetMeshObject()
{
	return NULL;
}

bool CNavMeshRenderObject::GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation )
{
	return false;
}

bool CNavMeshRenderObject::IsKindOf( IHashString * compType )
{	
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID());
}

DWORD CNavMeshRenderObject::OnMouseMove(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(MOUSECOORDS));
	MOUSECOORDS *pParams = (MOUSECOORDS*)param;

	MouseMove(pParams);

	return MSG_HANDLED_PROCEED;
}

float DistanceFromRayToPoint( Vec3 rayPos, Vec3 rayDir, Vec3 point )
{
	Vec3 pointVec = point - rayPos;
	float dot = rayDir.Dot( pointVec );
	TCHAR sz[100];
	_stprintf( sz, _T("Dot: %f\n"), dot );
	OutputDebugString( sz );

	if (dot > 0.0f)
	{
		// closest distance between point and the line
		return (point - rayDir*dot).Length();
	}
	else
	{
		// target point is behind the ray so rayPos is the closest point to target point
		return pointVec.Length();
	}
}

float DistanceFromRayToLineSegment( Vec3 rayPos, Vec3 rayDir, Vec3 linePointA, Vec3 linePointB )
{
	return 0;
}

float DistanceFromRayToTriangle( Vec3 rayPos, Vec3 rayDir, Vec3 triangleA, Vec3 triangleB, Vec3 triangleC )
{
	return 0;
}

void CNavMeshRenderObject::MouseMove( MOUSECOORDS *pMouseMove )
{
	RAYFROMMOUSEPARAMS rayfrommouse;
	rayfrommouse.mouseX = pMouseMove->m_fXPos;
	rayfrommouse.mouseY = pMouseMove->m_fYPos;
	Vec3 rayPos, rayDir;
	rayfrommouse.rayPos = &rayPos;
	rayfrommouse.rayDir = &rayDir;
	static DWORD msgHash_GetRayFromMouseCoords = CHashString(_T("GetRayFromMouseCoords")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetRayFromMouseCoords, sizeof(rayfrommouse), &rayfrommouse ) == MSG_HANDLED)
	{
		m_iClosestVertexToMouseRay = INVALID_NAVINDEX;
		float fClosestDistance = 99999.0f;
		for (int i=0; i < g_pNavMesh->GetVertexCount(); i++)
		{
			float fDist = DistanceFromRayToPoint( rayPos, rayDir, g_pNavMesh->GetVertex(i)->m_Position );
			if (fDist < fClosestDistance)
			{
				m_iClosestVertexToMouseRay = i;
				fClosestDistance = fDist;
			}
		}
	}
}
