///============================================================================
/// \file		ShapeManager.cpp
/// \brief		Implmentation of CShapeManager Class
/// \date		07-26-2005
/// \author		D. Patrick Ghiocel
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
#include "ShapeManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BOUNDINGBOXPRIORITY 0x0680

REGISTER_COMPONENT_SINGLETON( CShapeManager );

REGISTER_MESSAGE_HANDLER( AddAxisAlignedBox, OnAddAxisAlignedBox, CShapeManager );
REGISTER_MESSAGE_HANDLER( RemoveAxisAlignedBox, OnRemoveAxisAlignedBox, CShapeManager );
REGISTER_MESSAGE_HANDLER( AddObjectOrientedBox, OnAddObjectOrientedBox, CShapeManager );
REGISTER_MESSAGE_HANDLER( RemoveObjectOrientedBox, OnRemoveObjectOrientedBox, CShapeManager );
REGISTER_MESSAGE_HANDLER( AddSphere, OnAddSphere, CShapeManager );
REGISTER_MESSAGE_HANDLER( RemoveSphere, OnRemoveSphere, CShapeManager );
REGISTER_MESSAGE_HANDLER( AddLine, OnAddLine, CShapeManager );
REGISTER_MESSAGE_HANDLER( RemoveLine, OnRemoveLine, CShapeManager );
//REGISTER_MESSAGE_HANDLER( AddPoint, OnAddPoint, CShapeManager );
//REGISTER_MESSAGE_HANDLER( RemovePoint, OnRemovePoint, CShapeManager );

CHashString CShapeManager::m_hszAxisAlignedBoxClass( _T("CAxisAlignedBoxRenderObject") );
CHashString CShapeManager::m_hszAxisAlignedBoxRenderObjectName( _T("AxisAlignedBoxRenderObject") );

CHashString CShapeManager::m_hszObjectOrientedBoxClass( _T("CObjectOrientedBoxRenderObject") );
CHashString CShapeManager::m_hszObjectOrientedBoxRenderObjectName( _T("ObjectOrientedBoxRenderObject") );

CHashString CShapeManager::m_hszSphereClass( _T("CSphereRenderObject") );
CHashString CShapeManager::m_hszSphereRenderObjectName( _T("SphereRenderObject") );

CHashString CShapeManager::m_hszLineClass( _T("CLineRenderObject") );
CHashString CShapeManager::m_hszLineRenderObjectName( _T("LineRenderObject") );

CHashString CShapeManager::m_hszPointClass( _T("CPointRenderObject") );
CHashString CShapeManager::m_hszPointRenderObjectName( _T("PointRenderObject") );

CShapeManager::CShapeManager() : 
	CManager(_T("CShapeManager"), BOUNDINGBOXPRIORITY )
{
	Init();
}

CShapeManager::~CShapeManager()
{
	DeInit();
}

IComponent* CShapeManager::Create(int nArgs, va_list argptr)
{
	// Return the singleton instance of this class
	return SINGLETONINSTANCE( CShapeManager );
}

bool CShapeManager::Init()
{
	CreateAxisAlignedBoxRenderObject();
	CreateObjectOrientedBoxRenderObject();
	CreateSphereRenderObject();
	CreateLineRenderObject();
	CreatePointRenderObject();

	m_AxisAlignedBoundingBoxes.clear();
	m_ObjectOrientedBoundingBoxes.clear();
	m_Spheres.clear();
	m_Lines.clear();
	m_Points.clear();

	return CManager::Init();
}

void CShapeManager::Update(DWORD tickCount)
{
	// Build the render lists
	// Render lists must be added each frame.

	// Add the render object to the render list
	ADDRENDERLISTPARAMS arlParams;
	//arlParams.dwFuncCallback;
	//arlParams.dwListPriorityKey;
	//arlParams.dwSortMethod;
	arlParams.objList = &m_ShapeRenderObjects;
	arlParams.dwListPriorityKey = RENDERPRIORITY_OPAQUE;
	//arlParams.renderContext;
	static DWORD msgHash_AddRenderList = CHashString(_T("AddRenderList")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddRenderList, sizeof(ADDRENDERLISTPARAMS), &arlParams );
}

bool CShapeManager::DeInit()
{
	DeleteAxisAlignedBoxRenderObject();
	DeleteObjectOrientedBoxRenderObject();
	DeleteSphereRenderObject();
	DeleteLineRenderObject();
	DeletePointRenderObject();

	m_AxisAlignedBoundingBoxes.clear();
	m_ObjectOrientedBoundingBoxes.clear();
	m_Spheres.clear();
	m_Lines.clear();
	m_Points.clear();

	m_ShapeRenderObjects.clear();

	return CManager::DeInit();
}

void CShapeManager::AddShapeRenderObject( IRenderObject* shapeRenderObject )
{
	m_ShapeRenderObjects.push_back( shapeRenderObject );
}

DWORD CShapeManager::OnAddAxisAlignedBox( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( sizeof(ADDAXISALIGNEDBOXPARAMS), size );
	ADDAXISALIGNEDBOXPARAMS *pAABP = (ADDAXISALIGNEDBOXPARAMS*)params;

	CAxisAlignedBoundingBox bbox;
	//bbox.m_hszObjectName.Init( pAABP->name->GetString() );
	bbox.m_hszObjectName.Init( pAABP->name->GetString() );
	bbox.min = pAABP->min;
	bbox.max = pAABP->max;
	bbox.red = pAABP->red;
	bbox.green = pAABP->green;
	bbox.blue = pAABP->blue;
	bbox.alpha = pAABP->alpha;
	m_AxisAlignedBoundingBoxes.push_back( bbox );

	return MSG_HANDLED_STOP;
}

DWORD CShapeManager::OnRemoveAxisAlignedBox( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(sizeof(REMOVEAXISALIGNEDBOXPARAMS), size);
	REMOVEAXISALIGNEDBOXPARAMS *pRAABP = (REMOVEAXISALIGNEDBOXPARAMS*)params;

	// Remove the object from the list of CAxisAlignedBoundingBoxes
	std::vector<CAxisAlignedBoundingBox>::iterator itrBBox = m_AxisAlignedBoundingBoxes.begin();
	while( itrBBox != m_AxisAlignedBoundingBoxes.end() )
	{
		CHashString shapeName = itrBBox->m_hszObjectName;
		CHashString deleteObjectName( pRAABP->name );
		if( shapeName.GetUniqueID() == deleteObjectName.GetUniqueID() )
		{
			itrBBox = m_AxisAlignedBoundingBoxes.erase( itrBBox );
		}
		else
            itrBBox++;
	}

	return MSG_HANDLED_STOP;
}

DWORD CShapeManager::OnAddObjectOrientedBox( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( sizeof(ADDOBJECTORIENTEDBOXPARAMS), size );
	ADDOBJECTORIENTEDBOXPARAMS *pOOBP = (ADDOBJECTORIENTEDBOXPARAMS*)params;

	CObjectOrientedBoundingBox bbox;
	bbox.m_hszObjectName.Init( pOOBP->name->GetString() );
	bbox.localTransform = pOOBP->localTransform;
	bbox.min = pOOBP->min;
	bbox.max = pOOBP->max;
	bbox.red = pOOBP->red;
	bbox.green = pOOBP->green;
	bbox.blue = pOOBP->blue;
	m_ObjectOrientedBoundingBoxes.push_back( bbox );

	return MSG_HANDLED_STOP;
}

DWORD CShapeManager::OnRemoveObjectOrientedBox( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(sizeof(REMOVEOBJECTORIENTEDBOXPARAMS), size);
	REMOVEOBJECTORIENTEDBOXPARAMS *pRAABP = (REMOVEOBJECTORIENTEDBOXPARAMS*)params;

	std::vector<CObjectOrientedBoundingBox>::iterator itrBBox = m_ObjectOrientedBoundingBoxes.begin();
	while( itrBBox != m_ObjectOrientedBoundingBoxes.end() )
	{
		CHashString shapeName = itrBBox->m_hszObjectName;
		CHashString deleteObjectName( pRAABP->name );
		if( shapeName.GetUniqueID() == deleteObjectName.GetUniqueID() )
		//if( itrBBox->m_hszObjectName == *pRAABP->name )
		{
			itrBBox = m_ObjectOrientedBoundingBoxes.erase( itrBBox );
		}
		else
            itrBBox++;
	}

	return MSG_HANDLED_STOP;
}

DWORD CShapeManager::OnAddSphere( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( sizeof(ADDSPHEREPARAMS), size );
	ADDSPHEREPARAMS *pSP = (ADDSPHEREPARAMS*)params;

	// since there is currently an empty sphere drawing function
	// in the renderer we will instead add an AAB for now
	CSphere sphere;
	sphere.m_hszObjectName.Init( pSP->name->GetString() );
	sphere.radius = pSP->radius;
	sphere.red = pSP->red;
	sphere.green = pSP->green;
	sphere.blue = pSP->blue;
	sphere.resolution = pSP->resolution;
	m_Spheres.push_back( sphere );

	return MSG_HANDLED_STOP;
}

DWORD CShapeManager::OnRemoveSphere( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( sizeof(REMOVELINEPARAMS), size );
	REMOVELINEPARAMS *pRSP = (REMOVELINEPARAMS*)params;

	std::vector<CSphere>::iterator itrSphere = m_Spheres.begin();
	while( itrSphere != m_Spheres.end() )
	{
		if( itrSphere->m_hszObjectName == *pRSP->name )
		{
			itrSphere = m_Spheres.erase( itrSphere );
		}
		else
            itrSphere++;
	}

	return MSG_HANDLED_STOP;
}

DWORD CShapeManager::OnAddLine( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( sizeof(ADDLINEPARAMS), size );
	ADDLINEPARAMS *pLP = (ADDLINEPARAMS*)params;

	CLine line;
	line.m_hszObjectName.Init( pLP->name->GetString() );
	line.start = pLP->start;
	line.end = pLP->end;
	line.red = pLP->red;
	line.green = pLP->green;
	line.blue = pLP->blue;
	line.width = pLP->width;
	m_Lines.push_back( line );

	return MSG_HANDLED_STOP;
}

DWORD CShapeManager::OnRemoveLine( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( sizeof(REMOVELINEPARAMS), size );
	REMOVELINEPARAMS *pRLP = (REMOVELINEPARAMS*)params;

	std::vector<CLine>::iterator itrLine = m_Lines.begin();
	while( itrLine != m_Lines.end() )
	{
		if( itrLine->m_hszObjectName == *pRLP->name )
		{
			itrLine = m_Lines.erase( itrLine );
		}
		else
            itrLine++;
	}

	return MSG_HANDLED_STOP;
}

DWORD CShapeManager::OnAddPoint( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( sizeof(ADDPOINTPARAMS), size );
	ADDPOINTPARAMS *pPP = (ADDPOINTPARAMS*)params;

	CPoint point;
	point.m_hszObjectName.Init( pPP->name->GetString() );
	point.red = pPP->red;
	point.green = pPP->green;
	point.blue = pPP->blue;
	point.size = pPP->size;
	m_Points.push_back( point );

	return MSG_HANDLED_STOP;
}

DWORD CShapeManager::OnRemovePoint( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( sizeof(REMOVEPOINTPARAMS), size );
	REMOVEPOINTPARAMS *pRPP = (REMOVEPOINTPARAMS*)params;

	std::vector<CPoint>::iterator itrPoint = m_Points.begin();
	while( itrPoint != m_Points.end() )
	{
		if( itrPoint->m_hszObjectName == *pRPP->name )
		{
			itrPoint = m_Points.erase( itrPoint );
		}
		else
            itrPoint++;
	}

	return MSG_HANDLED_STOP;
}

void CShapeManager::CreateAxisAlignedBoxRenderObject()
{
	// Create the axis aligned box render object
	CREATEOBJECTPARAMS cop;
	cop.name = &m_hszAxisAlignedBoxRenderObjectName;
	cop.parentName = NULL;
	cop.typeName = &m_hszAxisAlignedBoxClass;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	if( m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop ) != MSG_HANDLED )
	{
		m_ToolBox->Log( LOGERROR, _T("CShapeManager: Failed to create axis aligned box render object!\n") );
	}
	// Init the axis aligned box render object
	INITOBJECTPARAMS iop;
	iop.name = &m_hszAxisAlignedBoxRenderObjectName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop );
}

void CShapeManager::DeleteAxisAlignedBoxRenderObject()
{
	DELETEOBJECTPARAMS dop;
	dop.name = &m_hszAxisAlignedBoxRenderObjectName;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop );
}

void CShapeManager::CreateObjectOrientedBoxRenderObject()
{
	// Create the axis aligned box render object
	CREATEOBJECTPARAMS cop;
	cop.name = &m_hszObjectOrientedBoxRenderObjectName;
	cop.parentName = NULL;
	cop.typeName = &m_hszObjectOrientedBoxClass;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	if( m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop ) != MSG_HANDLED )
	{
		m_ToolBox->Log( LOGERROR, _T("CShapeManager: Failed to create object oriented box render object!\n") );
	}
	// Init the axis aligned box render object
	INITOBJECTPARAMS iop;
	iop.name = &m_hszObjectOrientedBoxRenderObjectName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop );
}

void CShapeManager::DeleteObjectOrientedBoxRenderObject()
{
	DELETEOBJECTPARAMS dop;
	dop.name = &m_hszObjectOrientedBoxRenderObjectName;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop );
}

void CShapeManager::CreateSphereRenderObject()
{
	// Create the axis aligned box render object
	CREATEOBJECTPARAMS cop;
	cop.name = &m_hszSphereRenderObjectName;
	cop.parentName = NULL;
	cop.typeName = &m_hszSphereClass;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	if( m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop ) != MSG_HANDLED )
	{
		m_ToolBox->Log( LOGERROR, _T("CShapeManager: Failed to create sphere render object!\n") );
	}
	// Init the axis aligned box render object
	INITOBJECTPARAMS iop;
	iop.name = &m_hszSphereRenderObjectName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop );
}

void CShapeManager::DeleteSphereRenderObject()
{
	DELETEOBJECTPARAMS dop;
	dop.name = &m_hszSphereRenderObjectName;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop );
}

void CShapeManager::CreateLineRenderObject()
{
	// Create the axis aligned box render object
	CREATEOBJECTPARAMS cop;
	cop.name = &m_hszLineRenderObjectName;
	cop.parentName = NULL;
	cop.typeName = &m_hszLineClass;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	if( m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop ) != MSG_HANDLED )
	{
		m_ToolBox->Log( LOGERROR, _T("CShapeManager: Failed to create line render object!\n") );
	}
	// Init the axis aligned box render object
	INITOBJECTPARAMS iop;
	iop.name = &m_hszLineRenderObjectName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop );
}

void CShapeManager::DeleteLineRenderObject()
{
	DELETEOBJECTPARAMS dop;
	dop.name = &m_hszLineRenderObjectName;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop );
}

void CShapeManager::CreatePointRenderObject()
{
	// Create the axis aligned box render object
	CREATEOBJECTPARAMS cop;
	cop.name = &m_hszPointRenderObjectName;
	cop.parentName = NULL;
	cop.typeName = &m_hszPointClass;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	if( m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop ) != MSG_HANDLED )
	{
		m_ToolBox->Log( LOGERROR, _T("CShapeManager: Failed to create point render object!\n") );
	}
	// Init the axis aligned box render object
	INITOBJECTPARAMS iop;
	iop.name = &m_hszPointRenderObjectName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop );
}

void CShapeManager::DeletePointRenderObject()
{
	DELETEOBJECTPARAMS dop;
	dop.name = &m_hszPointRenderObjectName;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop );
}
