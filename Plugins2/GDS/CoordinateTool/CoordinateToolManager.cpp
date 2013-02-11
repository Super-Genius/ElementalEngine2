///============================================================================
/// \file		CoordinateToolManager.cpp
/// \brief		Implmentation of CCoordinateToolManager Class
/// \date		07-26-2005
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

#include "CoordinateTool/CoordinateToolDefine.h"
#include "CoordinateToolManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COORDINATETOOLMANAGERPRIORITY 0x0690

REGISTER_COMPONENT_SINGLETON(CCoordinateToolManager);

REGISTER_MESSAGE_HANDLER(AddAxis, OnAddAxis, CCoordinateToolManager);
REGISTER_MESSAGE_HANDLER(RemoveAxis, OnRemoveAxis, CCoordinateToolManager);
REGISTER_MESSAGE_HANDLER(LeftClickTool, OnLeftClickTool, CCoordinateToolManager);
// This used to catch LBUTTONUP event, and registered with lower priority
// to process message after object component.
REGISTER_MESSAGE_HANDLER_PRIORITY(MouseMove, OnMouseMove, CCoordinateToolManager, 0x90000000);
// This used to remove CoordinateTool from object before its destruction.
// This is necessarry, otherwise physics will crash.
REGISTER_MESSAGE_HANDLER_PRIORITY(DeleteObject, OnDeleteObject, CCoordinateToolManager, 0x70000000);

CHashString CCoordinateToolManager::m_pickingObjectType(_T("CCoordinateToolPhysicsObject"));

StdString CCoordinateToolManager::m_xAxisBoxPostfix(_T("_CTPO_XAxis"));
StdString CCoordinateToolManager::m_yAxisBoxPostfix(_T("_CTPO_YAxis"));
StdString CCoordinateToolManager::m_zAxisBoxPostfix(_T("_CTPO_ZAxis"));

CHashString CCoordinateToolManager::m_hszCoordinateToolClass(_T("CCoordinateTool"));
StdString CCoordinateToolManager::m_wszCoordinateToolPostfix(_T("_CoordinateTool"));

CCoordinateToolManager::CCoordinateToolManager() : 
	CManager(_T("CCoordinateToolManager"), COORDINATETOOLPRIORITY, COORDINATETOOLPRIORITY)
{
	m_active = false;
	m_CoordinateToolName = NULL;

	Init();
}

CCoordinateToolManager::~CCoordinateToolManager()
{
	DeInit();
}

IComponent* CCoordinateToolManager::Create(int nArgs, va_list argptr)
{
	// Return the singleton instance of this class
	return SINGLETONINSTANCE(CCoordinateToolManager);
}

bool CCoordinateToolManager::Init()
{
	// Register editor.
	REGISTER_TOOL_MESSAGE regParams;
	m_PickableObjects.push_back(&m_pickingObjectType);
	regParams.toolName = GetComponentType();
	regParams.objectNames = &m_PickableObjects;
	static DWORD msgHash_RegisterTool = CHashString(_T("RegisterTool")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterTool, sizeof(REGISTER_TOOL_MESSAGE), &regParams);

	return CManager::Init();
}

void CCoordinateToolManager::Update(DWORD tickCount)
{
	// Get the map of managed coordinate tool objects
	IDTOOBJECTMAP* objectMap = GetObjectMap(&m_hszCoordinateToolClass);
	if (objectMap)
	{
		// Update managed objects
		for (IDTOOBJECTMAP::iterator iter = objectMap->begin(); iter != objectMap->end(); ++iter)
		{
			iter->second->Update();
		}
	}

	// Build the render lists
	// Render lists must be added each frame.
	// Add the render object to the render list
	ADDRENDERLISTPARAMS arlParams;
	//arlParams.dwFuncCallback;
	arlParams.dwListPriorityKey = RENDERPRIORITY_ALPHA+1;
	//arlParams.dwSortMethod;
	arlParams.objList = &m_renderObjects;
	//arlParams.renderContext;
	static DWORD msgHash_AddRenderList = CHashString(_T("AddRenderList")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddRenderList, sizeof(ADDRENDERLISTPARAMS), &arlParams);
}

bool CCoordinateToolManager::DeInit()
{
	// Delete the axis if it is still around.
	CleanAxis();

	return CManager::DeInit();
}

void CCoordinateToolManager::AddRenderObject(IRenderObject* renderObject)
{
	m_renderObjects.push_back(renderObject);
}

void CCoordinateToolManager::RemoveRenderObject(IRenderObject* renderObject)
{
	OBJECTLIST::iterator i = std::find(m_renderObjects.begin(), m_renderObjects.end(), renderObject);
	if (i == m_renderObjects.end())
	{
		m_ToolBox->Log(LOGERROR, _T("Attempt to remove render object which was not added"));
	}
	m_renderObjects.erase(i);
}

IObject* CCoordinateToolManager::FindByParent(IHashString* parent)
{
	// Current implementation just iterates through all managed objects.
	// It can be optimized with MAP, but for now it is unjustified, because
	// in most cases we have only one CoordinateTool object.
	IDTOOBJECTMAP* objectMap = GetObjectMap(&m_hszCoordinateToolClass);
	if (objectMap)
	{
		for (IDTOOBJECTMAP::iterator iter = objectMap->begin(); iter != objectMap->end(); ++iter)
		{
			if (iter->second->GetParentName()->GetUniqueID() == parent->GetUniqueID())
			{
				return iter->second;
			}
		}
	}
	return NULL;
}

CCoordinateToolManager::AXIS CCoordinateToolManager::GetAxisByObjectName(IHashString* name)
{
	StdString wszName(name->GetString());

	// Assume that all axis postfixes has same length.
	if (wszName.length() < m_xAxisBoxPostfix.length())
	{
		return AXIS_NONE;
	}
	StdString axisName(wszName.substr(wszName.length() - m_xAxisBoxPostfix.length(), m_xAxisBoxPostfix.length()));

	if (axisName == m_xAxisBoxPostfix)
	{
		return AXIS_X;
	}
	else if (axisName == m_yAxisBoxPostfix)
	{
		return AXIS_Y;
	}
	else if (axisName == m_zAxisBoxPostfix)
	{
		 return AXIS_Z;
	}
	
	return AXIS_NONE;
}

void CCoordinateToolManager::BuildAxis(IHashString* parentName)
{
	m_CoordinateToolName = new CHashString(_T("CoordinateToolObject"));

	// Create coordinate tool object.
	CREATEOBJECTPARAMS cop;
	cop.name = m_CoordinateToolName;
	cop.parentName = parentName;
	cop.typeName = &m_hszCoordinateToolClass;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop) != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGERROR, _T("CCoordinateToolManager: Failed to create coordinate tool object!\n"));
		return;
	}

	// Init coordinate tool object.
	INITOBJECTPARAMS iop;
	//iop.name = &hszName;
	iop.name = m_CoordinateToolName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
}

void CCoordinateToolManager::CleanAxis()
{
	if (m_CoordinateToolName != NULL)
	{
		// Remove coordinate tool
		DELETEOBJECTPARAMS dop;
		dop.name = m_CoordinateToolName;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop);

		m_CoordinateToolName = NULL;
	}
}

DWORD CCoordinateToolManager::OnAddAxis(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(ADDAXISPARAMS), size);
	ADDAXISPARAMS *ax = (ADDAXISPARAMS*)params;


	if( m_CoordinateToolName == NULL )
	{
		BuildAxis(ax->name);
	}
	else
	{
		CHashString typeName = _T("CCoordinateTool");
		static DWORD msg_SetParent = CHashString(_T("SetParent")).GetUniqueID();
		if (m_ToolBox->SendMessage(msg_SetParent, sizeof(IHashString), ax->name, m_CoordinateToolName, &m_hszCoordinateToolClass) != MSG_HANDLED)
		{
			// if all objects are deleted and the parent is world it never gets in here...
			m_ToolBox->Log(LOGERROR, _T("CCoordinateToolManager: Failed to set coordinate tool parent!\n"));
			return MSG_HANDLED_STOP;
		}
	}

	return MSG_HANDLED_STOP;
}

DWORD CCoordinateToolManager::OnRemoveAxis(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(REMOVEAXISPARAMS), size);
	REMOVEAXISPARAMS *rx = (REMOVEAXISPARAMS*)params;
	
	// Set the coordinate tool's parent to the world
	CHashString typeName = _T("CCoordinateTool");
	CHashString worldObject( _T("World") );
	static DWORD msg_SetParent = CHashString(_T("SetParent")).GetUniqueID();
	if (m_ToolBox->SendMessage(msg_SetParent, sizeof(IHashString), &worldObject, m_CoordinateToolName, &m_hszCoordinateToolClass) != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGERROR, _T("CCoordinateToolManager: Failed to set coordinate tool parent!\n"));
		return MSG_HANDLED_STOP;
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CCoordinateToolManager::OnLeftClickTool(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(MOUSECLICKPARAMS));
	MOUSECLICKPARAMS *mouseClick = (MOUSECLICKPARAMS*)param;

	// We're only able to process click on CCoordinateToolPhysicsObject.
	// And we're registered for such object type.
	if (mouseClick->pickableCompType->GetUniqueID() != m_pickingObjectType.GetUniqueID())
	{
		m_ToolBox->Log(LOGWARNING, _T("CCoordinateToolManager: Received left click on unknown pickable object type %s\n"), mouseClick->pickableCompType->GetString());
		return MSG_NOT_HANDLED;
	}

	if (m_active)
	{
		// Already active, just gracefully ignore message in this case.
		return MSG_HANDLED_PROCEED;
	}

	EDITORGETOBJECTTOOLCONFIGPARAMS getConfig;
	static DWORD msgHash_EditorGetObjectToolConfig = CHashString(_T("EditorGetObjectToolConfig")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_EditorGetObjectToolConfig, sizeof(EDITORGETOBJECTTOOLCONFIGPARAMS), &getConfig) != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGERROR, _T("CCoordinateToolManager: Failed to retrieve editor object tool configuration.\n"));
		// Anyway return message handled code, to prevent tool click
		// processing by the rest of editor manager routines, which can cause
		// undesirable consequences.
		return MSG_HANDLED_PROCEED;
	}

	if (getConfig.bActive)
	{
		// Save previous object tool configuration.
		m_objectToolConfig.modifyFlags = OTCONFIG_MODIFY_LOCK_STATE;
		m_objectToolConfig.objectLockState = getConfig.objectLockState;

		// Retrieve picked axis.
		AXIS axis = GetAxisByObjectName(mouseClick->pickableObject);
		if (axis == AXIS_NONE)
		{
			// We get in here when pickable object name is unknown.
			m_ToolBox->Log(LOGERROR, _T("CCoordinateToolManager: Unknown object name %s\n"), mouseClick->pickableObject->GetString());
			return MSG_HANDLED_PROCEED;
		}

		// Fill out new object tool config.
		EDITORSETOBJECTTOOLCONFIGPARAMS setConfig;
		setConfig.modifyFlags = OTCONFIG_MODIFY_LOCK_STATE;
		setConfig.objectLockState = LOCK_NONE;
		switch (getConfig.objectManipulateState)
		{
			case MANIPULATE_MOVE:
			case MANIPULATE_SCALE:
			{
				switch (axis)
				{
					case AXIS_X: setConfig.objectLockState = LOCK_YZ; break;
					case AXIS_Y: setConfig.objectLockState = LOCK_XZ; break;
					case AXIS_Z: setConfig.objectLockState = LOCK_XY; break;
				}
				break;
			}
			case MANIPULATE_ROTATE:
			{
				switch (axis)
				{
					case AXIS_X: setConfig.objectLockState = LOCK_XZ; break;
					case AXIS_Y: setConfig.objectLockState = LOCK_XY; break;
					case AXIS_Z: setConfig.objectLockState = LOCK_YZ; break;
				}
				break;
			}
			default:
			{
				// Not affect other manipulations.
				return MSG_HANDLED_PROCEED;
			}
		}

		static DWORD msgHash_EditorSetObjectToolConfig = CHashString(_T("EditorSetObjectToolConfig")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_EditorSetObjectToolConfig, sizeof(EDITORSETOBJECTTOOLCONFIGPARAMS), &setConfig) != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGERROR, _T("CCoordinateToolManager: Failed to set editor object tool configuration.\n"));
			return MSG_HANDLED_PROCEED;
		}

		// Successfully activated.
		m_active = true;
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CCoordinateToolManager::OnMouseMove(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(MOUSECOORDS));
	MOUSECOORDS *message = (MOUSECOORDS*)param;

	if (!m_active)
	{
		return MSG_NOT_HANDLED;
	}

	if (!message->m_LButtonDown)
	{
		// Left button is up.
		m_active = false;

		static DWORD msgHash_EditorSetObjectToolConfig = CHashString(_T("EditorSetObjectToolConfig")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_EditorSetObjectToolConfig, sizeof(EDITORSETOBJECTTOOLCONFIGPARAMS), &m_objectToolConfig) != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGERROR, _T("CCoordinateToolManager: Failed to restore editor object tool configuration.\n"));
			return MSG_HANDLED_PROCEED;
		}
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CCoordinateToolManager::OnDeleteObject(DWORD size, void *param)
{
	// This message has its priority raised so that it gets called before the
  	// parent entity's delete object. We MUST to remove coordinate tool
  	// for entity, before its deletion. Otherwise physics will crash.
  	// Is there any better solution, except to catch all DeleteObject messages?
  
  	VERIFY_MESSAGE_SIZE(sizeof(DELETEOBJECTPARAMS), size);
  	DELETEOBJECTPARAMS* dop = (DELETEOBJECTPARAMS*)param;
  
  	IObject * object = FindByParent(dop->name);
  	if (object != NULL)
  	{
  		// Remove coordinate tool for this parent object.
  		DELETEOBJECTPARAMS dop;
  		dop.name = object->GetName();
  		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
  		m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop);

		m_CoordinateToolName = NULL;
  	}

	return MSG_HANDLED_PROCEED;
}
