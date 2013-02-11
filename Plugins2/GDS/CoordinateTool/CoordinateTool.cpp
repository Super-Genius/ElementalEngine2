///============================================================================
/// \file		CoordinateTool.cpp
/// \brief		Implmentation of CCoordinateTool Object Class
/// \date		10-05-2007
/// \author		D. Patrick Ghiocel, Dmitriy S. Sergeyev
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

#include "CoordinateTool.h"
#include "CoordinateToolManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CCoordinateTool);

REGISTER_MESSAGE_HANDLER(SetParent, OnSetParent, CCoordinateTool);

CHashString CCoordinateTool::m_hszNameType = _T("CCoordinateTool");
float CCoordinateTool::m_axisLength = 100;
float CCoordinateTool::m_axisBoxExtent = 1;

CCoordinateTool::CCoordinateTool(IHashString *parentName, IHashString *name) :
	SCENEOBJECTTEMPLATE(CCoordinateToolManager , CCoordinateTool, IRenderObject, parentName, name)
{
	m_vPosition.Set(0, 0, 0);

	m_physInit = false;

	m_xAxisPhysObject = NULL;
	m_yAxisPhysObject = NULL;
	m_zAxisPhysObject = NULL;

	m_xAxisPhysObjectName = NULL;
	m_yAxisPhysObjectName = NULL;
	m_zAxisPhysObjectName = NULL;

	m_IRenderer = NULL;

	// without selecting anything this should not being rendered
	m_bRenderCoordinateTool = FALSE;
}

CCoordinateTool::~CCoordinateTool()
{
}

IComponent *CCoordinateTool::Create(int nArgs, va_list argptr)
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
		self = new CCoordinateTool(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CCoordinateTool::Serialize(IArchive &ar)
{
}

void CCoordinateTool::Init()
{
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam) == MSG_HANDLED)
		m_IRenderer = renderInterfaceParam.m_RendererInterface;

	CCoordinateToolManager *pManager = SINGLETONINSTANCE(CCoordinateToolManager);
	pManager->AddRenderObject(this);
}

bool CCoordinateTool::Update()
{
	// Set render object's transform dirty
	m_bTransformIsDirty = true;

	// Set physics object's transform dirty
	if (m_xAxisPhysObject != NULL)
		((ISceneObject*)m_xAxisPhysObject)->SetDirtyTransformFlag();
	if (m_yAxisPhysObject != NULL)
		((ISceneObject*)m_yAxisPhysObject)->SetDirtyTransformFlag();
	if (m_zAxisPhysObject != NULL)
		((ISceneObject*)m_zAxisPhysObject)->SetDirtyTransformFlag();

	if (!m_physInit)
	{
		// Physics objects initialization should be done on update, otherwise
        // GDS crashed. This implemented by analogy with BoundingVolume,
		// and it stated there that it is not a good solution, but the only
		// one possible at the moment. Looks like it should be reimplemented,
		// when everything become clear with BoundingVolume too...
		m_physInit = true;

		if (UpdateTransformCustom())
		{
			CreateAxesPickingBoxes();
		}
	}

	// NOT IN USE ANY MORE (?)
	// Get the distance from the camera
	// Axis length is a function of the camera's distance from the gizmo.
	// The farther away the camera is the longer the axies.
	//m_axisLength = GetCameraDistance() * 0.5f;
	//Vec3 newExtents;

	//CHashString pickingType( _T("CPickingPhysicsObject") );
	//static DWORD msg_SetExtents = CHashString(_T("SetExtents")).GetUniqueID();

	// Update picking physics objects
	if (m_xAxisPhysObject != NULL)
	{
		//newExtents.Set(m_axisLength, m_axisBoxExtent, m_axisBoxExtent);
		//m_ToolBox->SendMessage(msg_SetExtents, sizeof(Vec3), &newExtents, m_xAxisPhysObject->GetName(), &pickingType);
		m_xAxisPhysObject->Update();
	}
	if (m_yAxisPhysObject != NULL)
	{
		//newExtents.Set(m_axisBoxExtent, m_axisLength, m_axisBoxExtent);
		//m_ToolBox->SendMessage(msg_SetExtents, sizeof(Vec3), &newExtents, m_yAxisPhysObject->GetName(), &pickingType);
		m_yAxisPhysObject->Update();
	}
	if (m_zAxisPhysObject != NULL)
	{
		//newExtents.Set(m_axisBoxExtent, m_axisBoxExtent, m_axisLength);
		//m_ToolBox->SendMessage(msg_SetExtents, sizeof(Vec3), &newExtents, m_zAxisPhysObject->GetName(), &pickingType);
		m_zAxisPhysObject->Update();
	}

	return true;
}

void CCoordinateTool::DeInit()
{
	DeleteAxesPickingBoxes();

	CCoordinateToolManager* pManager = SINGLETONINSTANCE(CCoordinateToolManager);
	pManager->RemoveRenderObject(this);
}

IHashString* CCoordinateTool::GetComponentType()
{
	return &m_hszNameType;
}

bool CCoordinateTool::IsKindOf(IHashString* compType)
{
	return (compType->GetUniqueID() == m_hszNameType.GetUniqueID());
}

bool CCoordinateTool::Render(UINT pass, IEffect * override)
{
	CCoordinateToolManager* pManager = SINGLETONINSTANCE(CCoordinateToolManager);

	// render coordinateTool only if the object is rotatable...
	if ( m_bRenderCoordinateTool )
	{
		if ((pass == 0) && (m_IRenderer))
		{
			//if (m_bTransformIsDirty && !UpdateTransformCustom())
			//{
			//	//don't render
			//	return true;
			//}

			// I wanted to reinterpret the earlier version:
			if ( m_bTransformIsDirty )
			{
				//dirty, we need to update the parameters
				if( !UpdateTransformCustom() )
				{
					// we have parameters, that can't be rendered... returning
					return true;
				}
			}
			
			// if we are here, the parameters are up-to-date
			// render with the latest matrix
			float axisLen = m_axisLength;
			Matrix3x3 axisRot;
			axisRot.SetFrom4x4(m_globalTransform.m);
			Vec3 axisPos = m_globalTransform.GetTranslation();
			m_IRenderer->DrawAxis(axisLen, axisRot, axisPos);
		}
	}
	return true;
}

bool CCoordinateTool::IsLessThan(const IRenderObject * obj, DWORD sortMethod, int curRenderPass)
{
	return true;
}

DWORD CCoordinateTool::GetRenderPriority()
{
    return 100;
}

UINT CCoordinateTool::GetNumPass()
{
	return 1;
}

void CCoordinateTool::GetBoundingSphere(Vec3 &position, float &radius)
{
	position.Set(0, 0, 0);
	radius = 999999.0f;
}

IHashString* CCoordinateTool::GetBaseSortClass()
{
	return GetComponentType();
}

bool CCoordinateTool::IsAlphaObject()
{
	return false;
}

IMeshObject* CCoordinateTool::GetMeshObject()
{
	return NULL;
}

bool CCoordinateTool::GetBoundingBox(Vec3 &pos, Vec3 &dim, EulerAngle &rotation)
{	
	return false;
}

UINT CCoordinateTool::GetNumGeometry()
{
	return 0;
}

IGeometry* CCoordinateTool::GetGeometry(int index)
{
	return NULL;
}

void CCoordinateTool::AttachLight(ILightObject * light)
{
}

void CCoordinateTool::DetachAllLights()
{
}

IMaterial* CCoordinateTool::GetMaterial(int index)
{
	return NULL;
}

UINT CCoordinateTool::GetNumMaterial()
{
	return 0;
}

ISceneSettings *CCoordinateTool::GetSceneSettings()
{
	return NULL;
}

bool CCoordinateTool::UpdateTransformCustom()
{
	// get the current rotation matrix of the parent
	static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
	static CHashString hsEntityType(_T("CEntity"));
	if (m_ToolBox->SendMessage(msgHash_GetGlobalTransform, sizeof(Matrix4x4), &m_globalTransform, GetParentName(), &hsEntityType) != MSG_HANDLED)
	{
		//Message failed, probably because our current parent can't be rotated
		m_ToolBox->Log(LOGINFORMATION, _T("Selected entity can't be rotated. CoordinateTool won't be attached to it.\n") );
		
		// Let's try doing nothing and returning false. It causes to skip rendering
		m_bRenderCoordinateTool = FALSE;
		return false;
	}

	GETBOUNDINGSPHEREPARAMS currentpar;
	currentpar.position = &Vec3( 0.f, 0.f, 0.f );

	//Get the boundingbox of the parent
	static DWORD msgHash_GetGetBoundingRadius = CHashString(_T("GetBoundingSphere")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetGetBoundingRadius, sizeof(currentpar), &currentpar, GetParentName()/*, &hsEntityType*/) == MSG_HANDLED)
	{
		// we are here, if the message is handled
		m_axisLength = currentpar.radius; 
	}
	else
	{
		//Message failed, we should not be here, if the first message failed, so how can that 
		// succeed and but the second message not. Perhaps the message handler componenets are not present
		m_ToolBox->Log(LOGWARNING, _T("%s(%i): Resizing the coordinate tool failed. Don't know what size to use... apply default size.\n"), __FILE__, __LINE__);

		// random value
		m_axisLength = 200.0f;
	}

	m_bTransformIsDirty = false;

	// everything seemed to be ok: return true. 
	return true;
}

void CCoordinateTool::CreateAxesPickingBoxes()
{
	// Calculate the length from the camera distance.
	//m_axisLength = GetCameraDistance() * 0.5f;

	float boxLargeExtent = m_axisLength * 0.5f; //* 2;
	float boxPos = boxLargeExtent + m_axisBoxExtent;
	StdString parentName(GetParentName()->GetString());

	// X-axis box
	Vec3 position(boxPos, 0, 0);
	Vec3 extents(boxLargeExtent, m_axisBoxExtent, m_axisBoxExtent);
	m_xAxisPhysObjectName = new CHashString( (parentName + CCoordinateToolManager::m_xAxisBoxPostfix).c_str() );
	m_xAxisPhysObject = CreatePickingBox(m_xAxisPhysObjectName, position, extents);

	// Y-axis box
	position.Set(0, boxPos, 0);
	extents.Set(m_axisBoxExtent, boxLargeExtent, m_axisBoxExtent);
	m_yAxisPhysObjectName = new CHashString( (parentName + CCoordinateToolManager::m_yAxisBoxPostfix).c_str() );
	m_yAxisPhysObject = CreatePickingBox(m_yAxisPhysObjectName, position, extents);

	// Z-axis box
	position.Set(0, 0, boxPos);
	extents.Set(m_axisBoxExtent, m_axisBoxExtent, boxLargeExtent);
	m_zAxisPhysObjectName = new CHashString( (parentName + CCoordinateToolManager::m_zAxisBoxPostfix).c_str() );
	m_zAxisPhysObject = CreatePickingBox(m_zAxisPhysObjectName, position, extents);
}

void CCoordinateTool::DeleteAxesPickingBoxes()
{
	DELETEOBJECTPARAMS delobj;
	static DWORD msgHash_DeleteObject = CHashString("DeleteObject").GetUniqueID();

	if (m_xAxisPhysObject)
	{
		delobj.name = m_xAxisPhysObjectName;
		m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &delobj);
		m_xAxisPhysObject = NULL;
		m_xAxisPhysObjectName = NULL;
	}

	if (m_yAxisPhysObject)
	{
		delobj.name = m_yAxisPhysObjectName;
		m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &delobj);
		m_yAxisPhysObject = NULL;
		m_yAxisPhysObjectName = NULL;
	}

	if (m_zAxisPhysObject)
	{
		delobj.name = m_zAxisPhysObjectName;
		m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &delobj);
		m_zAxisPhysObject = NULL;
		m_zAxisPhysObjectName = NULL;
	}
}

IObject* CCoordinateTool::CreatePickingBox(IHashString *boxName, Vec3 &position, Vec3 &extents)
{
	IComponent *physComp = m_ToolBox->CreateComponent(&CCoordinateToolManager::m_pickingObjectType, 2, boxName, GetParentName());

	OFACTORYADDPARAMS ofap;
	ofap.name = boxName;
	ofap.component = physComp;
	static DWORD msgHash_AddObjectToFactory = CHashString(_T("AddObjectToFactory")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddObjectToFactory, sizeof(OFACTORYADDPARAMS), &ofap);

	IObject * physObject = dynamic_cast<IObject*>(physComp);
	assert(physObject);

	// Create Archive
	IArchive *memArchive;	
	CHashString memType(_T("Memory"));
	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return physObject;
	}
	memArchive = ca.archive;

	memArchive->Write(position, _T("Position"));
	memArchive->Write(extents, _T("Extents"));

	memArchive->SetIsWriting(false);
	memArchive->SeekTo(0);

	physObject->Serialize(*memArchive);
	
	// Close the archive
	memArchive->Close();

	physObject->Init();

	return physObject;
}

/*
// NOT IN USE ANY MORE (?)
float CCoordinateTool::GetCameraDistance()
{
	// Get active camera name
	CHashString hszCameraName;
	IHashString *pCameraName = NULL;
	CONTEXTCAMPARAMS ccp;
	ccp.pRC = NULL;
	ccp.szCameraName = &hszCameraName;
	static DWORD msgHash_GetActiveCamera = CHashString(_T("GetActiveCamera")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetActiveCamera, sizeof(CONTEXTCAMPARAMS), &ccp ) != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGWARNING, "Failed to get active camera name.\n");
	}

    pCameraName = ccp.szCameraName;
	// get active camera name
	Vec3 camOrigin;
	static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
	// I am not sure about this componenentType... needs to be checked:
	static CHashString hsComponentType(_T("CCamera"));
	if (m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &camOrigin, pCameraName, &hsComponentType ) != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGWARNING, "Failed to get active camera position.\n");
	}

	// Get the distance from the camera to the coordinate tool
	Vec3 distanceVec = camOrigin - m_vPosition;
	
	return distanceVec.Length();
}
*/

// **** MESSAGES **** //
DWORD CCoordinateTool::OnSetParent(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));
	IHashString* name = (IHashString *)params;

	SetParentName(name);
	m_bTransformIsDirty = true;

	m_bRenderCoordinateTool = TRUE;

	// Update physics objects parents
	if (m_xAxisPhysObject != NULL)
	{
		m_xAxisPhysObject->SetParentName(name);
		((ISceneObject*)m_xAxisPhysObject)->SetDirtyTransformFlag();
	}
	if (m_yAxisPhysObject != NULL)
	{
		((ISceneObject*)m_yAxisPhysObject)->SetDirtyTransformFlag();
		m_yAxisPhysObject->SetParentName(name);
	}
	if (m_zAxisPhysObject != NULL)
	{
		((ISceneObject*)m_zAxisPhysObject)->SetDirtyTransformFlag();
		m_zAxisPhysObject->SetParentName(name);
	}

	return MSG_HANDLED_STOP;
}
