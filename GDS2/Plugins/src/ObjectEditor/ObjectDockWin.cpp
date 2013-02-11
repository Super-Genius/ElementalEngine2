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

#include "stdafx.h"
#include <math.h>
#include "ObjectDockWin.h"
#include "MathUtils.h"
#include "SelectNewObjectTypeDlg.h"

// ObjectDockWin

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const static StdString formatFile = _T("EditableProperties.xgf");
const static StdString formatDir = _T("//objectProperties//");


IMPLEMENT_DYNAMIC(ObjectDockWin, CGuiControlBar)
ObjectDockWin::ObjectDockWin() : 
	m_ccamTypeName(_T("CCamera"))
{
	m_OptionTreeView = NULL;
	m_ToolBox = EngineGetToolBox();
	m_iBBoxMode = 2;
	static DWORD msgHash_SetBBoxMode = CHashString(_T("SetBBoxMode")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetBBoxMode, sizeof( m_iBBoxMode ), &m_iBBoxMode );
	m_DefaultObjectArchive = NULL;
}

ObjectDockWin::~ObjectDockWin()
{
	delete m_OptionTreeView;

	if (m_DefaultObjectArchive != NULL)
	{
		m_DefaultObjectArchive->Close();
	}
}


BEGIN_MESSAGE_MAP(ObjectDockWin, CGuiControlBar)
	ON_WM_CREATE()
	ON_NOTIFY(OT_NOTIFY_ITEMCHANGED, IDC_OPTIONSTREE_OPTIONS, OnUpdateProperties)

	// this is the menuItem, and we handle the menu clickes and updates
	ON_COMMAND(ID_VIEW_OBJECT_PROPERTIES, OnViewObjectProperties)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OBJECT_PROPERTIES, OnUpdateViewObjectProperties)

	ON_COMMAND(ID_EDIT_COPY, OnObjectCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnObjectPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)

	ON_COMMAND(IDC_OBJECT_CREATE, OnObjectCreate)
	ON_UPDATE_COMMAND_UI(IDC_OBJECT_CREATE, OnUpdateObjectCreate)
	ON_COMMAND(ID_EDIT_CLEAR, OnObjectDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)

	ON_COMMAND(ID_BOUNDINGBOX_NONE, OnBoundingboxNone)
	ON_UPDATE_COMMAND_UI(ID_BOUNDINGBOX_NONE, OnUpdateBoundingboxNone)
	ON_COMMAND(ID_BOUNDINGBOX_RENDERBOX, OnBoundingboxRenderbox)
	ON_UPDATE_COMMAND_UI(ID_BOUNDINGBOX_RENDERBOX, OnUpdateBoundingboxRenderbox)
	ON_COMMAND(ID_BOUNDINGBOX_PHYSICSBOX, OnBoundingboxPhysicsbox)
	ON_UPDATE_COMMAND_UI(ID_BOUNDINGBOX_PHYSICSBOX, OnUpdateBoundingboxPhysicsbox)
	ON_COMMAND(ID_LOCK_NONE, OnLockNone)
	ON_UPDATE_COMMAND_UI(ID_LOCK_NONE, OnUpdateLockNone)
	ON_COMMAND(ID_LOCK_XY, OnLockXy)
	ON_UPDATE_COMMAND_UI(ID_LOCK_XY, OnUpdateLockXy)
	ON_COMMAND(ID_LOCK_XZ, OnLockXz)
	ON_UPDATE_COMMAND_UI(ID_LOCK_XZ, OnUpdateLockXz)
	ON_COMMAND(ID_LOCK_YZ, OnLockYz)
	ON_UPDATE_COMMAND_UI(ID_LOCK_YZ, OnUpdateLockYz)

	ON_COMMAND(ID_MENU_OBJECT_MOVE, OnObjectMove)
	ON_UPDATE_COMMAND_UI(ID_MENU_OBJECT_MOVE, OnUpdateObjectMove)
	ON_COMMAND(ID_MENU_OBJECT_ROTATE, OnObjectRotate)
	ON_UPDATE_COMMAND_UI(ID_MENU_OBJECT_ROTATE, OnUpdateObjectRotate)
	ON_COMMAND(ID_MENU_OBJECT_SCALE, OnObjectScale)
	ON_UPDATE_COMMAND_UI(ID_MENU_OBJECT_SCALE, OnUpdateObjectScale)

	ON_UPDATE_COMMAND_UI(ID_OBJECTS_ENABLECOLLISIONS, OnUpdateObjectsEnablecollisions)
	ON_COMMAND(ID_OBJECTS_ENABLECOLLISIONS, OnObjectsEnablecollisions)
	ON_COMMAND(ID_HUG_NEG_X_OBJECT, OnHugNegXObject)
	ON_UPDATE_COMMAND_UI(ID_HUG_NEG_X_OBJECT, OnUpdateHugNegXObject)
	ON_COMMAND(ID_HUG_NEG_Y_OBJECT, OnHugNegYObject)
	ON_UPDATE_COMMAND_UI(ID_HUG_NEG_Y_OBJECT, OnUpdateHugNegYObject)
	ON_COMMAND(ID_HUG_NEG_Z_OBJECT, OnHugNegZObject)
	ON_UPDATE_COMMAND_UI(ID_HUG_NEG_Z_OBJECT, OnUpdateHugNegZObject)
	ON_COMMAND(ID_HUG_NONE, OnHugNone)
	ON_UPDATE_COMMAND_UI(ID_HUG_NONE, OnUpdateHugNone)
	ON_COMMAND(ID_HUG_POS_X_OBJECT, OnHugPosXObject)
	ON_UPDATE_COMMAND_UI(ID_HUG_POS_X_OBJECT, OnUpdateHugPosXObject)
	ON_COMMAND(ID_HUG_POS_Y_OBJECT, OnHugPosYObject)
	ON_UPDATE_COMMAND_UI(ID_HUG_POS_Y_OBJECT, OnUpdateHugPosYObject)
	ON_COMMAND(ID_HUG_POS_Z_OBJECT, OnHugPosZObject)
	ON_UPDATE_COMMAND_UI(ID_HUG_POS_Z_OBJECT, OnUpdateHugPosZObject)
	ON_COMMAND(IDC_OBJECT_ZOOMTO, OnObjectZoomto)
	ON_UPDATE_COMMAND_UI(IDC_OBJECT_ZOOMTO, OnUpdateObjectZoomto)
	ON_COMMAND(ID_LOCK_X, OnLockX)
	ON_UPDATE_COMMAND_UI(ID_LOCK_X, OnUpdateLockX)
	ON_COMMAND(ID_LOCK_Y, OnLockY)
	ON_UPDATE_COMMAND_UI(ID_LOCK_Y, OnUpdateLockY)
	ON_COMMAND(ID_LOCK_Z, OnLockZ)
	ON_UPDATE_COMMAND_UI(ID_LOCK_Z, OnUpdateLockZ)
	ON_COMMAND(ID_PAINTING_PAINT, OnPaintingPaint)
	ON_UPDATE_COMMAND_UI(ID_PAINTING_PAINT, OnUpdatePaintingPaint)
	ON_COMMAND(ID_PAINTING_ERASE, OnPaintingErase)
	ON_UPDATE_COMMAND_UI(ID_PAINTING_ERASE, OnUpdatePaintingErase)
	ON_COMMAND(ID_PAINTING_LEVELTERRAIN, OnPaintingLevelterrain)
	ON_UPDATE_COMMAND_UI(ID_PAINTING_LEVELTERRAIN, OnUpdatePaintingLevelterrain)
	ON_COMMAND(ID_PAINTING_ROTATION, OnPaintingRotation)
	ON_UPDATE_COMMAND_UI(ID_PAINTING_ROTATION, OnUpdatePaintingRotation)
	ON_COMMAND(ID_PAINTING_SCALE, OnPaintingScale)
	ON_UPDATE_COMMAND_UI(ID_PAINTING_SCALE, OnUpdatePaintingScale)
END_MESSAGE_MAP()

void ObjectDockWin::OnUpdateViewObjectProperties( CCmdUI* pCmdUI ) 
{
	// checked if visible
	pCmdUI->SetCheck( IsVisible() || IsMinimized() );

	// always enabled
	pCmdUI->Enable();
}

void ObjectDockWin::OnViewObjectProperties() 
{
	if ( IsVisible() || IsMinimized() )
	{
		Close();
	}
	else
	{
		// GuiControlBar::Show redocks the controlbar if it
		// was docked or minimized before hiding.
		Show();
	}
}

void ObjectDockWin::CreateNewOptionTree(IHashString *name, IHashString *compType)
{
	SERIALIZEOBJECTPARAMS sop;
	IArchive *MemArchive;
	CHashString memType(_T("Memory"));
	CREATEARCHIVE ca;

	// create the option tree 
	if ( !m_OptionTreeView->Update(name, compType) )
	{
		ClearOptionTree();
		return;
	}

	// get data from ee and fill the tree
	UpdateOptionTree(name, compType);
	
	// Get the new data from the tree and update ee
	// this is done to get the default file directories into ee (if there are any)
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamType = &memType;
	ca.streamData = NULL;
	ca.streamSize = 0;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return;
	}
	MemArchive = ca.archive;

	CFileVersionSetter lastVersionSetter;
	m_OptionTreeView->Serialize(*MemArchive, false);

	sop.archive = MemArchive;
	sop.name = name;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

	MemArchive->Close();

	m_ObjectName = new CHashString(name);
	m_ComponentName = compType;
}

void ObjectDockWin::RestoreOptionTree(IHashString *name, IHashString *compType)
{
	// create the option tree 
	if ( !m_OptionTreeView->Update(name, compType) )
	{
		return;
	}
	// get data from ee and fill the tree (if it has entries)
	UpdateOptionTree(name, compType);
	
	m_ObjectName = name;
	m_ComponentName = compType;
}

void ObjectDockWin::UpdateOptionTree(IHashString *name, IHashString *compType)
{
	if( m_OptionTreeView->GetNumEntries(compType) > 0 )
	{
		SERIALIZEOBJECTPARAMS sop;
		IArchive *MemArchive;
		CHashString memType(_T("Memory"));
		CREATEARCHIVE ca;
		
		// make a writable archive
		ca.mode = STREAM_MODE_READ | STREAM_MODE_WRITE | ARCHIVE_MODE_WRITE;
		ca.streamType = &memType;
		ca.streamData = NULL;
		ca.streamSize = 0;
		static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
		{
			return;
		}
		MemArchive = ca.archive;

		sop.archive = MemArchive;
		sop.name = name;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

		// make archive readable
		MemArchive->SetIsWriting(false);

		// update the option tree
		m_OptionTreeView->Serialize(*MemArchive, true);

		MemArchive->Close();
	}
}

void ObjectDockWin::UpdateOptionTree()
{
	UpdateOptionTree(&m_ObjectName, &m_ComponentName);
}

void ObjectDockWin::MouseMoved(float mouseX, float mouseY, long posX, long posY, long deltaX, long deltaY, IHashString *selectedObject)
{
	int buttonState = m_ObjectToolPal.GetManipulateState();

	// mouse movement is scaled for global sensitivity and then scaled again
	// according to the tool in use, ie. movement speed needs to be slower than rotation
	// because movement is manipulating world coordinates and rotation is manipulating
	// degrees
	float fSensitivity = m_ObjectToolPal.GetSensitivity();
	float fDeltaX = (float)deltaX * fSensitivity;
	float fDeltaY = (float)deltaY * fSensitivity;
	float fMoveDeltaScalar = 0.15f;
	float fRotateDeltaScalar = 1.0f;
	float fScaleDeltaScalar = 0.0005f;

	// if the move button is selected
	if (buttonState == MANIPULATE_MOVE)
	{
		IHashString *pCameraName = NULL;
		CHashString hszCameraName;
		CONTEXTCAMPARAMS ccp;
		ccp.pRC = NULL;
		ccp.szCameraName = &hszCameraName;
		static DWORD msgHash_GetActiveCamera = CHashString(_T("GetActiveCamera")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetActiveCamera, sizeof(CONTEXTCAMPARAMS), &ccp ) != MSG_HANDLED)
			return;

		pCameraName = ccp.szCameraName;

		// get camera basis vectors
		Vec3 vecCamView, vecCamUp, vecCamRight;
		CAMERAVECTORSPARAMS camVectors;
		camVectors.vViewVector = &vecCamView;
		camVectors.vUpVector = &vecCamUp;
		camVectors.vRightVector = &vecCamRight;
		static DWORD msgHash_GetCameraVectors = CHashString(_T("GetCameraVectors")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetCameraVectors, sizeof(camVectors), &camVectors, pCameraName, &m_ccamTypeName ) != MSG_HANDLED)
			return;

		// calculate the move vector
		fDeltaX *= -fMoveDeltaScalar;
		fDeltaY *= -fMoveDeltaScalar; // invert y
		Vec3 vecMove; // move relative to camera
		vecMove = vecCamUp * fDeltaY + vecCamRight * fDeltaX;

		SINGLETONINSTANCE(CObjectEditor)->AddUndoObject( selectedObject );

		//SINGLETONINSTANCE(CObjectEditor)->MoveObject( selectedObject, vecMove, true, mouseX, mouseY );
		SINGLETONINSTANCE(CObjectEditor)->MoveObject( selectedObject, vecMove, false, mouseX, mouseY );
	}
	// if the rotate button is selected
	else if(buttonState == MANIPULATE_ROTATE)
	{
		EulerAngle *eulerAngles;
		EulerAngle tmp(0, 0, 0);
		eulerAngles = &tmp;
		
		fDeltaX *= -fRotateDeltaScalar;
		fDeltaY *= fRotateDeltaScalar;

		// get the active axis from the gui
		if (m_ObjectToolPal.GetLockIndex() == LOCK_X)
		{
			eulerAngles->SetRoll(0.0f);
			eulerAngles->SetPitch(fDeltaX);
			eulerAngles->SetYaw(fDeltaY);
		}
		else if (m_ObjectToolPal.GetLockIndex() == LOCK_Y)
		{
			eulerAngles->SetRoll(fDeltaX);
			eulerAngles->SetPitch(0.0f);
			eulerAngles->SetYaw(fDeltaY);
		}
		else if (m_ObjectToolPal.GetLockIndex() == LOCK_Z)
		{
			eulerAngles->SetRoll(fDeltaX);
			eulerAngles->SetPitch(fDeltaY);
			eulerAngles->SetYaw(0.0f);
		}
		else if(m_ObjectToolPal.GetLockIndex() == LOCK_XY)
		{
			eulerAngles->SetRoll(0.0f);
			eulerAngles->SetPitch(0.0f);
			eulerAngles->SetYaw(fDeltaX);
		}
		else if(m_ObjectToolPal.GetLockIndex() == LOCK_YZ)
		{
			eulerAngles->SetRoll(fDeltaX);
			eulerAngles->SetPitch(0.0f);
			eulerAngles->SetYaw(0.0f);
		}
		else if(m_ObjectToolPal.GetLockIndex() == LOCK_XZ)
		{
			eulerAngles->SetRoll(0.0f);
			eulerAngles->SetPitch(fDeltaX);
			eulerAngles->SetYaw(0.0f);
		}
		else if(m_ObjectToolPal.GetLockIndex() == LOCK_NONE)
		{
			eulerAngles->SetRoll(fDeltaX);
			eulerAngles->SetPitch(fDeltaY);
			eulerAngles->SetYaw(0.0f);
		}

		// Get the current orientation of the object
		EulerAngle currentRotation;

		static DWORD msgHash_GetGlobalEuler = CHashString(_T("GetGlobalEuler")).GetUniqueID();
		DWORD MsgRetVal = m_ToolBox->SendMessage(msgHash_GetGlobalEuler, sizeof(EulerAngle), &currentRotation, selectedObject, NULL);
		if(MsgRetVal == MSG_ERROR)
		{
			return;
		}

		// Calculate the new orientation
		eulerAngles->SetRoll( eulerAngles->GetRoll() + currentRotation.GetRoll() );
		eulerAngles->SetPitch( eulerAngles->GetPitch() + currentRotation.GetPitch() );
		eulerAngles->SetYaw( eulerAngles->GetYaw() + currentRotation.GetYaw() );

		SINGLETONINSTANCE(CObjectEditor)->AddUndoObject(selectedObject);

		// send a SetOrientation message to the selected CEntity
		static DWORD msgHash_SetGlobalEuler = CHashString(_T("SetGlobalEuler")).GetUniqueID();
		MsgRetVal = m_ToolBox->SendMessage(msgHash_SetGlobalEuler, sizeof(EulerAngle), eulerAngles, selectedObject, NULL);
		UpdateOptionTree();
	}
	// if the move button is selected
	else if(buttonState == MANIPULATE_SCALE)
	{
		Vec3 *scale;
		Vec3 tmp(0, 0, 0);
		scale = &tmp;

		fDeltaX *= fScaleDeltaScalar;
		fDeltaY *= -fScaleDeltaScalar;

		// get the active axis from the gui
		// get the active axis from the gui
		if (m_ObjectToolPal.GetLockIndex() == LOCK_X)
		{
			scale->SetX(0.0f);
			scale->SetY(fDeltaX);
			scale->SetZ(fDeltaY);
		}
		else if (m_ObjectToolPal.GetLockIndex() == LOCK_Y)
		{
			scale->SetX(fDeltaX);
			scale->SetY(0.0f);
			scale->SetZ(fDeltaY);
		}
		else if (m_ObjectToolPal.GetLockIndex() == LOCK_Z)
		{
			scale->SetX(fDeltaX);
			scale->SetY(fDeltaY);
			scale->SetZ(0.0f);
		}
		else if(m_ObjectToolPal.GetLockIndex() == LOCK_XY)
		{
			scale->SetX(0.0f);
			scale->SetY(0.0f);
			scale->SetZ(fDeltaY);
		}
		else if(m_ObjectToolPal.GetLockIndex() == LOCK_YZ)
		{
			scale->SetX(fDeltaX);
			scale->SetY(0.0f);
			scale->SetZ(0.0f);
		}
		else if(m_ObjectToolPal.GetLockIndex() == LOCK_XZ)
		{
			scale->SetX(0.0f);
			scale->SetY(fDeltaX);
			scale->SetZ(0.0f);
		}
		else if(m_ObjectToolPal.GetLockIndex() == LOCK_NONE)
		{
 			scale->SetX((fDeltaX+fDeltaY) * 0.5f);
 			scale->SetY((fDeltaX+fDeltaY) * 0.5f);
 			scale->SetZ((fDeltaX+fDeltaY) * 0.5f);
		}

		// Get the current scaling of the object
		Vec3 currentScale;

		static DWORD msgHash_GetGlobalScale = CHashString(_T("GetGlobalScale")).GetUniqueID();
		DWORD MsgRetVal = m_ToolBox->SendMessage(msgHash_GetGlobalScale, sizeof(Vec3), &currentScale, selectedObject, NULL);
		if(MsgRetVal == MSG_ERROR)
		{
			return;
		}

		// Get the current scaling of the object
		scale->SetX( scale->GetX() + currentScale.GetX() );
		scale->SetY( scale->GetY() + currentScale.GetY() );
		scale->SetZ( scale->GetZ() + currentScale.GetZ() );

		if( scale->GetX() <= 0.00f )
			scale->SetX( 0.001f );

		if( scale->GetY() <= 0.00f )
			scale->SetY( 0.001f );

		if( scale->GetZ() <= 0.00f )
			scale->SetZ( 0.001f );
		

		SINGLETONINSTANCE(CObjectEditor)->AddUndoObject(selectedObject);

		// send a SetScale message to the selected CEntity
		static DWORD msgHash_SetGlobalScale = CHashString(_T("SetGlobalScale")).GetUniqueID();
		MsgRetVal = m_ToolBox->SendMessage(msgHash_SetGlobalScale, sizeof(Vec3), scale, selectedObject, NULL);
		UpdateOptionTree();
	}
}

void ObjectDockWin::ClearOptionTree(void)
{
	m_OptionTreeView->DeInit();
}

// ObjectDockWin message handlers

void ObjectDockWin::OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL /*bDisableIfNoHndler*/)
{
}

int ObjectDockWin::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	StdString basePath;
	m_ToolBox->GetDirectories(&basePath, NULL);
	basePath += formatDir;
	// save off base path
	m_szBasePath = basePath;

	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Declare variables
    CRect rcClient;
    DWORD dwStyle, dwOptions;
    
    // Get the clients rectangle
    GetClientRect(rcClient);
    
    // Setup the window style
    dwStyle = WS_CHILD | WS_VISIBLE;

    // Setup the tree options 
    dwOptions =  OT_OPTIONS_SHADEEXPANDCOLUMN | OT_OPTIONS_SHADEROOTITEMS;
	
	m_OptionTreeView = new COptionTreeView(dwStyle, rcClient, this, dwOptions, IDC_OPTIONSTREE_OPTIONS, basePath.c_str());

	m_OptionTreeView->Init(formatFile);

	if (!m_ObjectToolPal.Create(AfxGetApp()->GetMainWnd(), CObjectToolPal::IDD,
      CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY, CObjectToolPal::IDD))
	{
		TRACE0("Failed to create toolbar\n");
		return 1;      // fail to create
	}

	TOOLBARSTRUCT atbs;

	atbs.hIcon = (HICON)LoadImage(ObjectEditorDLL.hModule, 
		MAKEINTRESOURCE(IDR_OBJECTTYPE), IMAGE_ICON, 0, 0, 0);
	atbs.Text = _T("Objects");
	atbs.cBar = &m_ObjectToolPal;
	static DWORD msgHash_AddToolBar = CHashString(_T("AddToolBar")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddToolBar, sizeof(TOOLBARSTRUCT), &atbs);

	// create paint bar
	if (!m_PaintBar.Create(AfxGetApp()->GetMainWnd(), CPaintObjectBar::IDD,
      CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY, CPaintObjectBar::IDD))
	{
		TRACE0("Failed to create paint toolbar\n");
		return 1;      // fail to create
	}

	// add tool bar
	atbs.hIcon = (HICON)LoadImage(ObjectEditorDLL.hModule, 
		MAKEINTRESOURCE(IDR_OBJECTTYPE), IMAGE_ICON, 0, 0, 0);
	atbs.Text = _T("Paint Objects");
	atbs.cBar = &m_PaintBar;
	m_ToolBox->SendMessage(msgHash_AddToolBar, sizeof(TOOLBARSTRUCT), &atbs);

	return 0;
}

void ObjectDockWin::OnUpdateProperties(NMHDR* pNotifyStruct, LRESULT* plResult)
{
	LPNMOPTIONTREE pNMOptionTree = (LPNMOPTIONTREE)pNotifyStruct;
	// the optiontree item that has been updated
	COptionTreeItem *otiItem = pNMOptionTree->otiItem;

	if (otiItem->IsActivated())
	{
		// process message from CommitData message only
		return;
	}
	
	// error check code
	// Occasionally, this function will get called when coptiontree is in the middle of 
	// clearing the tree. This will cause problems b/c the tree items in the tree have become
	// deleted and this function will attempt to update and serializes those deleted items.
	COptionTreeItem* parentItem = otiItem->GetParent();
	if (parentItem == NULL) 
	{
		return;
	}
	COptionTreeItem* childItem = parentItem->GetChild();
	if (childItem == NULL)
	{
		return;
	}
	// check if the root's first child is the updated optiontree item's sibling
	if (otiItem->GetSibling() == childItem) 
	{
		// This implies that the updated optiontree item is no longer in the tree
		// Do not update the tree
		return;
	}

	INITOBJECTPARAMS iop;
	SERIALIZEOBJECTPARAMS sop;
	IArchive *MemArchive = CreateMemoryArchive();
	if (MemArchive == NULL)
	{
		return;
	}

	// write data from view to archive
	m_OptionTreeView->Serialize(*MemArchive, false);
	MemArchive->SetIsWriting(false);

	if (IsEEObjectChanged(&m_ObjectName, MemArchive))
	{
		// save object state changes in undo stack
		CUndoCommandGuard undoCommand;
		UndoForSelectObject(&m_ObjectName, true);
		static DWORD msgHash_UndoSaveObject = CHashString(_T("UndoSaveObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_UndoSaveObject, sizeof(IHashString), &m_ObjectName);

		CFileVersionSetter lastVersionSetter;

		//MemArchive->SeekTo(0);
		sop.archive = MemArchive;
		sop.name = &m_ObjectName;
		// read data from archive to EE
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		DWORD result = m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop);
		if (MSG_HANDLED == result)
		{
			// send message to EE to initialize the object
			iop.name = &m_ObjectName;
			static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
			result = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(iop), &iop);
		}

		if (MSG_HANDLED == result)
		{
			static DWORD msgHash_RedoSaveObject = CHashString(_T("RedoSaveObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_RedoSaveObject, sizeof(IHashString), &m_ObjectName);
			UndoForSelectObject(&m_ObjectName, false);
		}
		else
		{
			// setting of properties was failed. rolled back all our changes
			undoCommand.Cancel();
			IArchive *ar = GetEEObjectProperties(&m_ObjectName);
			ASSERT(ar != NULL);
			m_OptionTreeView->Serialize(*ar, true);
			ar->Close();
		}
	}

	MemArchive->Close();
}

void ObjectDockWin::OnObjectCreate()
{
	if (!CanCreateObject())
	{
		return;
	}
	CSelectNewObjectTypeDlg selNewDlg;
	CString *newSelType;
	SELECTEDOBJECTLIST *selectedObjects;
	SELECTEDOBJECT *selectedObject;

	if (selNewDlg.DoModal() == IDOK)
	{
		// if type is selected
		if (selNewDlg.GetSelectionText(&newSelType))
		{
			CHashString hszTypeName = *newSelType;
			CHashString hszParent = _T("World");
			// See if any objects are selected
			selectedObjects = SINGLETONINSTANCE(CObjectEditor)->GetSelectedObjects();
			// if there are selected objects
			if (!selectedObjects->empty())
			{
				// grab the first one
				selectedObject = &(selectedObjects->front());
				// set the parent name
				hszParent = selectedObject->name;
			}

			CHashString hszObjectName = CreateDefaultEEObject(&hszParent, &hszTypeName, NULL);
			if (!hszObjectName.IsEmpty())
			{
				CUndoCommandGuard undoCommand;
				UndoForCreateObject(&hszParent, &hszObjectName, &hszTypeName);
			}
			else
			{
				// object creation failed for some reason
			}
		}
	}
}

void ObjectDockWin::OnObjectDelete()
{
	if (!CanDeleteObject())
	{
		return;
	}
	SELECTEDOBJECTLIST *selectedObjects;
	SELECTEDOBJECT *selectedObject;

	// See if any objects are selected
	selectedObjects = SINGLETONINSTANCE(CObjectEditor)->GetSelectedObjects();

	// if there are selected objects
	if (!selectedObjects->empty())
	{
		CUndoCommandGuard undoCommand;

		// grab the first one
		selectedObject = &(selectedObjects->front());
		CHashString hszSelectedName = selectedObject->name;

		UndoForSelectObject(&hszSelectedName, true);
		UndoForDeleteObject(&hszSelectedName);

		// clear the selected object
		SINGLETONINSTANCE(CObjectEditor)->ClearSelectedObject();

		DeleteEEObject(&hszSelectedName);

		// send out a message that the object needs to be removed
//		static DWORD msgHash_RemoveObject = CHashString(_T("RemoveObject")).GetUniqueID();
//		m_ToolBox->SendMessage(msgHash_RemoveObject, sizeof(IHashString), &dop.name);
	}
	else
	{
		// nothing selected, report this?
	}
}

void ObjectDockWin::OnObjectCopy()
{
	// See if any objects are selected
	SELECTEDOBJECTLIST *selectedObjects;
	selectedObjects = SINGLETONINSTANCE(CObjectEditor)->GetSelectedObjects();
	if (selectedObjects)
	{
		SELECTEDOBJECTLIST::iterator itrSelection = selectedObjects->begin();
		if (itrSelection != selectedObjects->end())
		{
			SELECTEDOBJECT *pSelection = &(*itrSelection);
			ClipboardCopyObjects( pSelection->name );
		}
	}
}

void ObjectDockWin::OnObjectPaste()
{
	// See if any objects are selected
	SELECTEDOBJECTLIST *selectedObjects;
	selectedObjects = SINGLETONINSTANCE(CObjectEditor)->GetSelectedObjects();
	if (selectedObjects)
	{
		SELECTEDOBJECTLIST::iterator itrSelection = selectedObjects->begin();
		if (itrSelection != selectedObjects->end())
		{
			SELECTEDOBJECT *pSelection = &(*itrSelection);
			ClipboardPasteObjects( pSelection->name );
		}
	}
}

void ObjectDockWin::OnUpdateObjectCreate(CCmdUI *pCmdUI)
{
	if (!CanCreateObject())
	{
		pCmdUI->Enable(FALSE);
	}
}

void ObjectDockWin::OnUpdateEditClear(CCmdUI *pCmdUI)
{
	if (CanDeleteObject())
	{
		pCmdUI->Enable(1);
	}
	else
	{
		pCmdUI->Enable(0);
	}
}

void ObjectDockWin::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	SELECTEDOBJECTLIST *selectedObjects;
	// See if any objects are selected
	selectedObjects = SINGLETONINSTANCE(CObjectEditor)->GetSelectedObjects();

	// if there are selected objects
	if (selectedObjects->size() != 0)
	{
		pCmdUI->Enable(1);
	}
	else
	{
		pCmdUI->Enable(0);
	}
}

void ObjectDockWin::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	SELECTEDOBJECTLIST *selectedObjects;
	// See if any objects are selected
	selectedObjects = SINGLETONINSTANCE(CObjectEditor)->GetSelectedObjects();

	// if there are selected objects
	if (!selectedObjects->empty() && !m_ClipboardVisitor.GetObjectList().empty())
	{
		pCmdUI->Enable();
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void ObjectDockWin::OnBoundingboxNone()
{
	m_iBBoxMode = 0;
	static DWORD msgHash_SetBBoxMode = CHashString(_T("SetBBoxMode")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetBBoxMode, sizeof( m_iBBoxMode ), &m_iBBoxMode );
}

void ObjectDockWin::OnUpdateBoundingboxNone(CCmdUI *pCmdUI)
{
	if( m_iBBoxMode == 0 )
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void ObjectDockWin::OnBoundingboxRenderbox()
{
	m_iBBoxMode = 1;
	static DWORD msgHash_SetBBoxMode = CHashString(_T("SetBBoxMode")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetBBoxMode, sizeof( m_iBBoxMode ), &m_iBBoxMode );
}

void ObjectDockWin::OnUpdateBoundingboxRenderbox(CCmdUI *pCmdUI)
{
	if( m_iBBoxMode == 1 )
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void ObjectDockWin::OnBoundingboxPhysicsbox()
{
	m_iBBoxMode = 2;
	static DWORD msgHash_SetBBoxMode = CHashString(_T("SetBBoxMode")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetBBoxMode, sizeof( m_iBBoxMode ), &m_iBBoxMode );
}

void ObjectDockWin::OnUpdateBoundingboxPhysicsbox(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if( m_iBBoxMode == 2 )
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void ObjectDockWin::OnLockNone()
{
	m_ObjectToolPal.SetLockIndex(LOCK_NONE);
}

void ObjectDockWin::OnUpdateLockNone(CCmdUI *pCmdUI)
{
	if (m_ObjectToolPal.GetLockIndex() == LOCK_NONE)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void ObjectDockWin::OnLockXy()
{
	m_ObjectToolPal.SetLockIndex(LOCK_XY);
}

void ObjectDockWin::OnUpdateLockXy(CCmdUI *pCmdUI)
{
	if (m_ObjectToolPal.GetLockIndex() == LOCK_XY)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void ObjectDockWin::OnLockXz()
{
	m_ObjectToolPal.SetLockIndex(LOCK_XZ);
}

void ObjectDockWin::OnUpdateLockXz(CCmdUI *pCmdUI)
{
	if (m_ObjectToolPal.GetLockIndex() == LOCK_XZ)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void ObjectDockWin::OnLockYz()
{
	m_ObjectToolPal.SetLockIndex(LOCK_YZ);
}

void ObjectDockWin::OnUpdateLockYz(CCmdUI *pCmdUI)
{
	if (m_ObjectToolPal.GetLockIndex() == LOCK_YZ)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void ObjectDockWin::OnObjectMove()
{
	TOOLBARACTIVESTRUCT tbas;
	tbas.cBar = &m_ObjectToolPal;

	// set object tool bar to active
	static DWORD msgHash_SetActiveToolBar = CHashString(_T("SetActiveToolBar")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetActiveToolBar, sizeof(tbas), &tbas);

	if (m_ObjectToolPal.GetManipulateState()== MANIPULATE_MOVE)
	{
		m_ObjectToolPal.SetManipulateState(MANIPULATE_SELECT);
	}
	else
	{
		m_ObjectToolPal.SetManipulateState(MANIPULATE_MOVE);
	}
}
void ObjectDockWin::OnUpdateObjectMove(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ObjectToolPal.GetManipulateState() == MANIPULATE_MOVE);
}

void ObjectDockWin::OnObjectRotate()
{
	TOOLBARACTIVESTRUCT tbas;
	tbas.cBar = &m_ObjectToolPal;

	// set object tool bar to active
	static DWORD msgHash_SetActiveToolBar = CHashString(_T("SetActiveToolBar")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetActiveToolBar, sizeof(tbas), &tbas);

	if (m_ObjectToolPal.GetManipulateState()== MANIPULATE_ROTATE)
	{
		m_ObjectToolPal.SetManipulateState(MANIPULATE_SELECT);
	}
	else
	{
		m_ObjectToolPal.SetManipulateState(MANIPULATE_ROTATE);
	}
}
void ObjectDockWin::OnUpdateObjectRotate(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ObjectToolPal.GetManipulateState() == MANIPULATE_ROTATE);
}

void ObjectDockWin::OnObjectScale()
{
	TOOLBARACTIVESTRUCT tbas;
	tbas.cBar = &m_ObjectToolPal;

	// set object tool bar to active
	static DWORD msgHash_SetActiveToolBar = CHashString(_T("SetActiveToolBar")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetActiveToolBar, sizeof(tbas), &tbas);

	if (m_ObjectToolPal.GetManipulateState()== MANIPULATE_SCALE)
	{
		m_ObjectToolPal.SetManipulateState(MANIPULATE_SELECT);
	}
	else
	{
		m_ObjectToolPal.SetManipulateState(MANIPULATE_SCALE);
	}
}
void ObjectDockWin::OnUpdateObjectScale(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ObjectToolPal.GetManipulateState() == MANIPULATE_SCALE);
}

void ObjectDockWin::OnUpdateObjectsEnablecollisions(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ObjectToolPal.GetCollisionCheck());
}

void ObjectDockWin::OnObjectsEnablecollisions()
{
	if (m_ObjectToolPal.GetCollisionCheck())
	{
		m_ObjectToolPal.SetCollisionCheck(0);
	}
	else
	{
		m_ObjectToolPal.SetCollisionCheck(1);
	}
		
	static DWORD msgHash_Start = CHashString(_T("Start")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_Start, 0, NULL, NULL, &CHashString(_T("CPhysManager")));
}

void ObjectDockWin::OnHugNegXObject()
{
	m_ObjectToolPal.SetHugIndex(HUG_NEG_X);
}

void ObjectDockWin::OnUpdateHugNegXObject(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_ObjectToolPal.GetCollisionCheck());
	pCmdUI->SetCheck(m_ObjectToolPal.GetHugIndex() == HUG_NEG_X);
}

void ObjectDockWin::OnHugNegYObject()
{
	m_ObjectToolPal.SetHugIndex(HUG_NEG_Y);
}

void ObjectDockWin::OnUpdateHugNegYObject(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_ObjectToolPal.GetCollisionCheck());
	pCmdUI->SetCheck(m_ObjectToolPal.GetHugIndex() == HUG_NEG_Y);
}

void ObjectDockWin::OnHugNegZObject()
{
	m_ObjectToolPal.SetHugIndex(HUG_NEG_Z);
}

void ObjectDockWin::OnUpdateHugNegZObject(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_ObjectToolPal.GetCollisionCheck());
	pCmdUI->SetCheck(m_ObjectToolPal.GetHugIndex() == HUG_NEG_Z);
}

void ObjectDockWin::OnHugNone()
{
	m_ObjectToolPal.SetHugIndex(HUG_NONE);
}

void ObjectDockWin::OnUpdateHugNone(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_ObjectToolPal.GetCollisionCheck());
	pCmdUI->SetCheck(m_ObjectToolPal.GetHugIndex() == HUG_NONE);
}

void ObjectDockWin::OnHugPosXObject()
{
	m_ObjectToolPal.SetHugIndex(HUG_POS_X);
}

void ObjectDockWin::OnUpdateHugPosXObject(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_ObjectToolPal.GetCollisionCheck());
	pCmdUI->SetCheck(m_ObjectToolPal.GetHugIndex() == HUG_POS_X);
}

void ObjectDockWin::OnHugPosYObject()
{
	m_ObjectToolPal.SetHugIndex(HUG_POS_Y);
}

void ObjectDockWin::OnUpdateHugPosYObject(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_ObjectToolPal.GetCollisionCheck());
	pCmdUI->SetCheck(m_ObjectToolPal.GetHugIndex() == HUG_POS_Y);
}

void ObjectDockWin::OnHugPosZObject()
{
	m_ObjectToolPal.SetHugIndex(HUG_POS_Z);
}

void ObjectDockWin::OnUpdateHugPosZObject(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_ObjectToolPal.GetCollisionCheck());
	pCmdUI->SetCheck(m_ObjectToolPal.GetHugIndex() == HUG_POS_Z);
}

bool ObjectDockWin::MoveObjectToCamera( IHashString *pObjectName, float fDistanceScalar )
{
	// get active camera name
	CHashString hszCameraName;
	IHashString *pCameraName = NULL;
	CONTEXTCAMPARAMS ccp;
	ccp.pRC = NULL;
	ccp.szCameraName = &hszCameraName;
	static DWORD msgHash_GetActiveCamera = CHashString(_T("GetActiveCamera")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetActiveCamera, sizeof(CONTEXTCAMPARAMS), &ccp) != MSG_HANDLED)
		return false; // no camera

	pCameraName = ccp.szCameraName;
	// get bounding box of object
	Vec3 bboxDim, bboxOrigin;
	std::list<BBOXPARAMS> listBBoxes;
	BBOXLISTPARAMS bboxparams;
	bboxparams.listBBoxes = &listBBoxes;
	//Marv: I think this should look for the render object bounding box since this involves cameras
	static DWORD msgHash_GetAABB = CHashString(_T("GetAABB")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetAABB, sizeof(bboxparams), &bboxparams, pObjectName, &CHashString(_T("CRenderObject") ) ) != MSG_HANDLED)
		return false;

	if( listBBoxes.size() == 0 )
		return false;
	
	BBOXPARAMS &bbox = (*listBBoxes.begin());

	// get camera basis vectors
	Vec3 vecCamView, vecCamUp, vecCamRight;
	CAMERAVECTORSPARAMS camVectors;
	camVectors.vViewVector = &vecCamView;
	camVectors.vUpVector = &vecCamUp;
	camVectors.vRightVector = &vecCamRight;
	static DWORD msgHash_GetCameraVectors = CHashString(_T("GetCameraVectors")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetCameraVectors, sizeof(camVectors), &camVectors, pCameraName ) != MSG_HANDLED)
		return false;

	// get camera position
	Vec3 vecCamPos;
	static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &vecCamPos, pCameraName ) != MSG_HANDLED)
		return false;

	// get the distance that the object should be from camera
	// twice the diameter of a sphere surrounding the bbox seems to be a decent distance
	float fDistance = fDistanceScalar * sqrt(	bbox.dimX*bbox.dimX + 
												bbox.dimY*bbox.dimY + 
												bbox.dimZ*bbox.dimZ);

	// get new object position from camera
	// to find object position, go forward calculated distance along the camera's view vector 
	// from the camera's origin
	Vec3 objectPos;
	objectPos.SetX( vecCamPos.GetX() + vecCamView.GetX() * fDistance );
	objectPos.SetY( vecCamPos.GetY() + vecCamView.GetY() * fDistance );
	objectPos.SetZ( vecCamPos.GetZ() + vecCamView.GetZ() * fDistance );

	// set object position
	static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetGlobalPosition, sizeof(Vec3), &objectPos, pObjectName );
	return true;
}

bool ObjectDockWin::MoveCameraToObject( IHashString *pObjectName, float fDistanceScalar )
{
	// get active camera name
	CHashString hszCameraName;
	IHashString *pCameraName = NULL;
	CONTEXTCAMPARAMS ccp;
	ccp.pRC = NULL;
	ccp.szCameraName = &hszCameraName;
	static DWORD msgHash_GetActiveCamera = CHashString(_T("GetActiveCamera")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetActiveCamera, sizeof(CONTEXTCAMPARAMS), &ccp ) != MSG_HANDLED)
	{
		return false; // no camera
	}

	pCameraName = ccp.szCameraName;
	// get bounding box of object
	float boundingRadius;
	
	static DWORD msgHash_GetBoundingRadius = CHashString(_T("GetBoundingRadius")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetBoundingRadius, sizeof(float), &boundingRadius, pObjectName/*, &CHashString(_T("CRenderObject") )*/ ) != MSG_HANDLED)
	{
		return false;
	}

	// get camera basis vectors
	Vec3 vecCamView, vecCamUp, vecCamRight;
	CAMERAVECTORSPARAMS camVectors;
	camVectors.vViewVector = &vecCamView;
	camVectors.vUpVector = &vecCamUp;
	camVectors.vRightVector = &vecCamRight;
	static DWORD msgHash_GetCameraVectors = CHashString(_T("GetCameraVectors")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetCameraVectors, sizeof(camVectors), &camVectors, pCameraName ) != MSG_HANDLED)
	{
		return false;
	}

	// get the distance that the camera should be from object
	// twice the diameter of a sphere surrounding the bbox seems to be a decent distance
	float fDistance = fDistanceScalar * boundingRadius;

	Vec3 objectPos;
	static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &objectPos, pObjectName) != MSG_HANDLED)
	{
		return false;
	}

	// Set the camera pos directly above the focus object
	Vec3 camPos;
	camPos.SetX( objectPos.GetX() );
	camPos.SetY( objectPos.GetY() + fDistance );
	camPos.SetZ( objectPos.GetZ() );

	// set camera position
	static DWORD msg_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
	m_ToolBox->SendMessage(msg_SetGlobalPosition, sizeof(Vec3), &camPos, pCameraName, &CHashString(_T("CCamera")) );

	// Set the camera to look at the focus object
	static DWORD msg_SetLookAtPosition = CHashString(_T("SetLookAtPosition")).GetUniqueID();
	m_ToolBox->SendMessage( msg_SetLookAtPosition, sizeof(Vec3), &objectPos, pCameraName, &CHashString(_T("CCamera")) );

	return true;
}

void ObjectDockWin::OnObjectZoomto()
{
	// get selected object, if any
	SELECTEDOBJECTLIST *selectedObjects = SINGLETONINSTANCE(CObjectEditor)->GetSelectedObjects();
	SELECTEDOBJECT *selectedObject = NULL;
	if (selectedObjects->size() != 0)
	{
		selectedObject = &(selectedObjects->front()); // grab the first one
	}
	if (!selectedObject)
		return; // no object selected

	MoveCameraToObject( selectedObject->name, 2.0f );
}

void ObjectDockWin::OnUpdateObjectZoomto(CCmdUI *pCmdUI)
{
	SELECTEDOBJECTLIST *selectedObjects;
	// See if any objects are selected
	selectedObjects = SINGLETONINSTANCE(CObjectEditor)->GetSelectedObjects();

	// if there are selected objects
	if (selectedObjects->size() != 0)
	{
		pCmdUI->Enable(1);
	}
	else
	{
		pCmdUI->Enable(0);
	}
}

void ObjectDockWin::OnLockX()
{
	m_ObjectToolPal.SetLockIndex(LOCK_X);
}

void ObjectDockWin::OnUpdateLockX(CCmdUI *pCmdUI)
{
	if (m_ObjectToolPal.GetLockIndex() == LOCK_X)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void ObjectDockWin::OnLockY()
{
	m_ObjectToolPal.SetLockIndex(LOCK_Y);
}

void ObjectDockWin::OnUpdateLockY(CCmdUI *pCmdUI)
{
	if (m_ObjectToolPal.GetLockIndex() == LOCK_Y)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void ObjectDockWin::OnLockZ()
{
	m_ObjectToolPal.SetLockIndex(LOCK_Z);
}

void ObjectDockWin::OnUpdateLockZ(CCmdUI *pCmdUI)
{
	if (m_ObjectToolPal.GetLockIndex() == LOCK_Z)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void ObjectDockWin::OnPaintingPaint()
{
	TOOLBARACTIVESTRUCT tbas;
	tbas.cBar = &m_PaintBar;

	// set paint bar to active
	static DWORD msgHash_SetActiveToolBar = CHashString(_T("SetActiveToolBar")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetActiveToolBar, sizeof(tbas), &tbas);

	if (m_PaintBar.GetPaintMode() == PM_PAINT)
	{
		// disable paint button
		m_PaintBar.SetPaintMode(PM_NONE);
	}
	else
	{
		// enable paint button
		m_PaintBar.SetPaintMode(PM_PAINT);
	}
	
}

void ObjectDockWin::OnUpdatePaintingPaint(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_PaintBar.GetPaintMode() == PM_PAINT);
}

void ObjectDockWin::OnPaintingErase()
{
	// TODO: Add your command handler code here
	TOOLBARACTIVESTRUCT tbas;
	tbas.cBar = &m_PaintBar;

	// set paint bar to active
	static DWORD msgHash_SetActiveToolBar = CHashString(_T("SetActiveToolBar")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetActiveToolBar, sizeof(tbas), &tbas);
	
	if (m_PaintBar.GetPaintMode() == PM_ERASE)
	{
		// disable erase button
		m_PaintBar.SetPaintMode(PM_NONE);
	}
	else
	{
		// enable erase button
		m_PaintBar.SetPaintMode(PM_ERASE);
	}
}

void ObjectDockWin::OnUpdatePaintingErase(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_PaintBar.GetPaintMode() == PM_ERASE);
}

void ObjectDockWin::OnPaintingLevelterrain()
{
	TOOLBARACTIVESTRUCT tbas;
	tbas.cBar = &m_PaintBar;

	// set paint bar to active
	static DWORD msgHash_SetActiveToolBar = CHashString(_T("SetActiveToolBar")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetActiveToolBar, sizeof(tbas), &tbas);
	
	if (m_PaintBar.TerrainLevelerEnabled())
	{
		// disable terrain leveler button
		m_PaintBar.SetPaintOption(PO_NOLEVELTERRAIN);
	}
	else
	{
		// enable terrain leveler button
		m_PaintBar.SetPaintOption(PO_LEVELTERRAIN);
	}
}

void ObjectDockWin::OnUpdatePaintingLevelterrain(CCmdUI *pCmdUI)
{
	if (m_PaintBar.GetPaintMode() == PM_PAINT)
	{
		pCmdUI->Enable();

		pCmdUI->SetCheck(m_PaintBar.TerrainLevelerEnabled());
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void ObjectDockWin::OnPaintingRotation()
{
	TOOLBARACTIVESTRUCT tbas;
	tbas.cBar = &m_PaintBar;

	// set paint bar to active
	static DWORD msgHash_SetActiveToolBar = CHashString(_T("SetActiveToolBar")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetActiveToolBar, sizeof(tbas), &tbas);
	
	if (m_PaintBar.RotationEnabled())
	{
		// disable rotation
		m_PaintBar.SetPaintOption(PO_NOROTATION);
	}
	else
	{
		// enable rotation
		m_PaintBar.SetPaintOption(PO_ROTATION);
	}
}

void ObjectDockWin::OnUpdatePaintingRotation(CCmdUI *pCmdUI)
{
	if (m_PaintBar.GetPaintMode() == PM_PAINT)
	{
		pCmdUI->Enable();

		pCmdUI->SetCheck(m_PaintBar.RotationEnabled());
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void ObjectDockWin::OnPaintingScale()
{
	TOOLBARACTIVESTRUCT tbas;
	tbas.cBar = &m_PaintBar;

	// set paint bar to active
	static DWORD msgHash_SetActiveToolBar = CHashString(_T("SetActiveToolBar")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetActiveToolBar, sizeof(tbas), &tbas);
	
	if (m_PaintBar.ScaleEnabled())
	{
		// disable rotation
		m_PaintBar.SetPaintOption(PO_NOSCALE);
	}
	else
	{
		// enable rotation
		m_PaintBar.SetPaintOption(PO_SCALE);
	}
}

void ObjectDockWin::OnUpdatePaintingScale(CCmdUI *pCmdUI)
{
	if (m_PaintBar.GetPaintMode() == PM_PAINT)
	{
		pCmdUI->Enable();

		pCmdUI->SetCheck(m_PaintBar.ScaleEnabled());
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

bool ObjectDockWin::ClipboardCopyObjects( IHashString *pRootObject )
{
	m_ClipboardVisitor.Reset();

	VISITHIERARCHYDATA vhd;
	vhd.name = pRootObject;
	vhd.visitor = &m_ClipboardVisitor;
	static DWORD msgHash_VisitHierarchy = CHashString(_T("VisitHierarchy")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_VisitHierarchy, sizeof(vhd), &vhd) != MSG_HANDLED)
		return false;

	return true;
}

bool ObjectDockWin::ClipboardPasteObjects( IHashString *pRootObject )
{
	CLIPBOARDOBJECTLIST::iterator itrObject;
	CLIPBOARDOBJECTLIST &objectList = m_ClipboardVisitor.GetObjectList();

	CFileVersionSetter versionSetter;

	bool bFirstObject = true;

	// create a map of old names to new names while generating new names
	CHashString tempNewName;
	map<DWORD,DWORD> oldToNewNameMap;
	for (itrObject = objectList.begin(); itrObject != objectList.end(); itrObject++)
	{
		GENERATEUNIQUEOBJECTNAMEPARAMS guonp;
		guonp.name = &itrObject->szName;
		guonp.newname = &tempNewName;
		static DWORD msgHash_GenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(guonp), &guonp ) != MSG_HANDLED)
			return false;

		oldToNewNameMap[guonp.name->GetUniqueID()] = guonp.newname->GetUniqueID();
	}

	CUndoCommandGuard undoCommand;

	for (itrObject = objectList.begin(); itrObject != objectList.end(); itrObject++)
	{
		map<DWORD,DWORD>::iterator itrNameMap;

		// look up the new name to use for the pasted object from the original name
		itrNameMap = oldToNewNameMap.find( itrObject->szName.GetUniqueID() );
		ASSERT( itrNameMap != oldToNewNameMap.end() );
		CHashString szNewName( m_ToolBox->GetHashString( itrNameMap->second ) );

		// it's ok if the lookup fails for the parent, if the parent wasn't
		// in the subtree then use the given root
		CHashString szNewParent;
		itrNameMap = oldToNewNameMap.find( itrObject->szParent.GetUniqueID() );
		if (itrNameMap != oldToNewNameMap.end())
			szNewParent.Init( m_ToolBox->GetHashString( itrNameMap->second ) );
		else
			szNewParent.Init( pRootObject->GetString() );

		CREATEOBJECTPARAMS cop;
		cop.name = &szNewName;
		cop.parentName = &szNewParent;
		cop.typeName = &itrObject->szComponentType;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		if (!m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(cop), &cop ) == MSG_HANDLED)
		{
			break;
		}

		itrObject->pArchive->SeekTo(0);
		itrObject->pArchive->SetIsWriting( false );
		SERIALIZEOBJECTPARAMS sop;
		sop.name = &szNewName;
		sop.archive = itrObject->pArchive;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		if (!m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop ) == MSG_HANDLED)
		{
			break;
		}

		INITOBJECTPARAMS iop;
		iop.name = &szNewName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		if (!m_ToolBox->SendMessage(msgHash_InitObject, sizeof(iop), &iop ) == MSG_HANDLED)
		{
			break;
		}

		UndoForCreateObject(&szNewParent, &szNewName, &itrObject->szComponentType);

		// send out a message that an object needs to be added
		static DWORD msgHash_AddObject = CHashString(_T("AddObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_AddObject, sizeof(cop), &cop);

		if (bFirstObject)
		{
			// set root object to editable selected
			SINGLETONINSTANCE(CObjectEditor)->SetEditableSelected( &szNewName, false );
			bFirstObject = false;
		}
	}

	// check for failure within loop
	if (itrObject != objectList.end())
		return false;

	return true;
}

bool ObjectDockWin::CanCreateObject() const
{
	static CHashString hszEmpty;
	return GetActiveScene() != hszEmpty;
}

bool ObjectDockWin::CanDeleteObject() const
{
	// See if any objects are selected
	SELECTEDOBJECTLIST *selectedObjects;
	selectedObjects = SINGLETONINSTANCE(CObjectEditor)->GetSelectedObjects();
	// if there are selected objects
	return !selectedObjects->empty();
}
