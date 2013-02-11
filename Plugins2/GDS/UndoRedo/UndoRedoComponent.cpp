///=====================================================================
/// \file	UndoRedoComponent.cpp
/// \brief	Implementation of CUndoRedoComponent
/// \date	08/30/2007
/// \author	Andrey Ivanov
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
///=====================================================================
#include "stdafx.h"

//////////////////// EE registration ///////////////////////////////////////
REGISTER_COMPONENT_SINGLETON(CUndoRedoComponent);

// the messages we receive
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CUndoRedoComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CUndoRedoComponent);
REGISTER_MESSAGE_HANDLER(WorldSave, OnWorldSave, CUndoRedoComponent);

// undo/redo messages
REGISTER_MESSAGE_HANDLER(UndoCreateStack, OnUndoCreateStack, CUndoRedoComponent);
REGISTER_MESSAGE_HANDLER(UndoDeleteStack, OnUndoDeleteStack, CUndoRedoComponent);
REGISTER_MESSAGE_HANDLER(UndoRenameStack, OnUndoRenameStack, CUndoRedoComponent);
REGISTER_MESSAGE_HANDLER(UndoRegisterViewStack, OnUndoRegisterViewStack, CUndoRedoComponent);
REGISTER_MESSAGE_HANDLER(UndoClearStack, OnUndoClearStack, CUndoRedoComponent);
REGISTER_MESSAGE_HANDLER(UndoStartCommand, OnUndoStartCommand, CUndoRedoComponent);
REGISTER_MESSAGE_HANDLER(UndoEndCommand, OnUndoEndCommand, CUndoRedoComponent);
REGISTER_MESSAGE_HANDLER(UndoCancelCommand, OnUndoCancelCommand, CUndoRedoComponent);
REGISTER_MESSAGE_HANDLER(UndoAddAction, OnUndoAddAction, CUndoRedoComponent);
REGISTER_MESSAGE_HANDLER(RedoAddAction, OnRedoAddAction, CUndoRedoComponent);
REGISTER_MESSAGE_HANDLER(UndoSaveObject, OnUndoSaveObject, CUndoRedoComponent);
REGISTER_MESSAGE_HANDLER(RedoSaveObject, OnRedoSaveObject, CUndoRedoComponent);
REGISTER_MESSAGE_HANDLER(UndoSaveTexture, OnUndoSaveTexture, CUndoRedoComponent);
REGISTER_MESSAGE_HANDLER(RedoSaveTexture, OnRedoSaveTexture, CUndoRedoComponent);
REGISTER_MESSAGE_HANDLER(UndoIsExecuting, OnUndoIsExecuting, CUndoRedoComponent);

// get the singleton for this
IComponent *CUndoRedoComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CUndoRedoComponent);
}

CUndoRedoComponent::CUndoRedoComponent()
	: m_pToolBox(::EngineGetToolBox())
	, m_pCurrentCommandData(NULL)
	, m_bIsExecuting( false )
{
	ASSERT(m_pToolBox != NULL);
}

CUndoRedoComponent::~CUndoRedoComponent()
{
	ASSERT(m_View2Stack.empty());
	ASSERT(m_Stacks.empty());
	ASSERT(m_pCurrentCommandData == NULL);
	delete m_pCurrentCommandData;
}

//////////////////// IComponent overrides //////////////////////////////////
// serialize function for this object
void CUndoRedoComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CUndoRedoComponent::GetComponentType()
{
	static CHashString compName(_T("CUndoRedoComponent"));
	return &compName;
}

bool CUndoRedoComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

//////////////////// EE message handlers ///////////////////////////////////

/// Process creating new window command
///	\param	size - unused
///	\param	param - unused
DWORD CUndoRedoComponent::OnCreate(DWORD size, void *param)
{
	MENUINFOMESSAGE mim;
	static DWORD msgHash_GetMenuInfo = CHashString(_T("GetMenuInfo")).GetUniqueID();
	m_pToolBox->SendMessage(msgHash_GetMenuInfo, sizeof(MENUINFOMESSAGE),&mim);

	// get the application instance handle for loading resources
	HINSTANCE appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(UndoRedoDLL.hModule);

	CMenu menu;
	menu.LoadMenu(IDR_UNDO_MENU);
	ASSERT(menu.m_hMenu != NULL);
	// we need to Merge Menus with Undo/Redo items
    // this code works now, but it may need to be updated for exact filling the menu
	const int nEditMenuPosition = 1;
	HMENU hEditMenu = GetSubMenu(mim.m_hMenu, nEditMenuPosition);

	TCHAR menuName[8];
	if (!GetMenuString(mim.m_hMenu, nEditMenuPosition, menuName, RTL_NUMBER_OF(menuName), MF_BYPOSITION) && !_tcscmp(menuName, _T("&Edit")))
	{
		ASSERT(0);
	}
	VERIFY(MergeSubPopupMenu(hEditMenu, menu.GetSubMenu(0)->m_hMenu));
	// and add accelerators
	HACCEL hAccel = ::LoadAccelerators(UndoRedoDLL.hResource, MAKEINTRESOURCE(IDR_UNDO_ACCELERATOR));
	ASSERT(hAccel != NULL);
	VERIFY(MergeAccelerators(&mim.m_hAccel, hAccel));

	VERIFY(::DestroyAcceleratorTable(hAccel));

	// TODO: add shortcut label to menu items
	static DWORD msgHash_SetMenuInfo = CHashString(_T("SetMenuInfo")).GetUniqueID();
	m_pToolBox->SendMessage(msgHash_SetMenuInfo, sizeof(MENUINFOMESSAGE),&mim);

	AfxSetResourceHandle(appInst);
	return MSG_HANDLED_PROCEED;
}

/// Process Undo/Redo menu commands and shortcuts from UI
///	\param	size - unused
///	\param	param - unused
DWORD CUndoRedoComponent::OnCmdMsg(DWORD size, void *param)
{
	CMDMESSAGE *pCmdMsg;
	VERIFY_MESSAGE_SIZE(size, sizeof(*pCmdMsg));
	pCmdMsg = (CMDMESSAGE *)param;

	bool retVal = false;
	switch (pCmdMsg->m_nID)
	{
		// TODO: Use better shortcuts for acceleration table
		case ID_UNDO_UNDO:
			switch (pCmdMsg->m_nCode)
			{
				case CN_COMMAND:
					OnUndo();
					retVal = true;
					break;
				case CN_UPDATE_COMMAND_UI:
					OnUpdateUndo(reinterpret_cast<CCmdUI *>(pCmdMsg->m_pExtra));
					retVal = true;
					break;
			}
			break;
		case ID_UNDO_REDO:
			switch (pCmdMsg->m_nCode)
			{
				case CN_COMMAND:
					OnRedo();
					retVal = true;
					break;
				case CN_UPDATE_COMMAND_UI:
					OnUpdateRedo(reinterpret_cast<CCmdUI *>(pCmdMsg->m_pExtra));
					retVal = true;
					break;
			}
			break;
	}
	// Let's MFC route message
	if (retVal)
	{
		pCmdMsg->retVal = true;
		return MSG_HANDLED_PROCEED;
	}

	// it wasn't handled
	return MSG_NOT_HANDLED;
}

///	\brief	notify editors about scene save
///	\param	size - sizeof(LPCTSTR)
///	\param	param - filename of the saved scene
///	\return EE message code
DWORD CUndoRedoComponent::OnWorldSave(DWORD size, void *param)
{
	if (m_pCurrentCommandData != NULL)
	{
		ASSERT(!"Save point should be set for completed undo command");
		m_pToolBox->Log(LOGERROR, _T("%s(%i): Can't set save point.\n"), __FILE__, __LINE__);
		return MSG_ERROR;
	}

	CUndoStack *pStack = GetStack();
	if (pStack == NULL)
	{
		ASSERT(!"Cannot find stack for save point");
		m_pToolBox->Log(LOGERROR, _T("%s(%i): Can't find stack for save point.\n"), __FILE__, __LINE__);
		return MSG_ERROR;
	}
	
	pStack->SetSavePoint();

	return MSG_HANDLED_PROCEED;
}

///	\brief	register new undo stack
///	\param	size - size of UNDOSTACKINFO structure
///	\param	param - pointer to UNDOSTACKINFO structure
DWORD CUndoRedoComponent::OnUndoCreateStack(DWORD size, void *param)
{
	UNDOSTACKINFO *pInfo = reinterpret_cast<UNDOSTACKINFO *>(param);
	VERIFY_MESSAGE_SIZE(size, sizeof(*pInfo));
	IHashString *pName = pInfo->pStackName;
	if (FindStack(pName) != m_Stacks.end())
	{
		m_pToolBox->Log(LOGERROR, _T("%s(%i): Stack with name '%s' already exists.\n"),
							__FILE__, __LINE__, pName->GetString());
		return MSG_ERROR;
	}
	CUndoStack *pStack = new CUndoStack(pName);
	m_Stacks.push_back(pStack);
	return MSG_HANDLED;
}

///	\brief	unregister new undo stack
///	\param	size - size of UNDOSTACKINFO structure
///	\param	param - pointer to UNDOSTACKINFO structure
DWORD CUndoRedoComponent::OnUndoDeleteStack(DWORD size, void *param)
{
	UNDOSTACKINFO *pInfo = reinterpret_cast<UNDOSTACKINFO *>(param);
	VERIFY_MESSAGE_SIZE(size, sizeof(*pInfo));
	IHashString *pName = pInfo->pStackName;
	StackList::iterator itStack = FindStack(pName);
	if (itStack == m_Stacks.end())
	{
		m_pToolBox->Log(LOGERROR, _T("%s(%i): Can't find undo stack for deletion.\n"), __FILE__, __LINE__);
		return MSG_ERROR;
	}

	DWORD hashName = pName->GetUniqueID();
	View2StackMap::iterator mapIt = m_View2Stack.begin();
	while (mapIt != m_View2Stack.end())
	{
		if (mapIt->second == *itStack)
		{
			mapIt = m_View2Stack.erase(mapIt);
		}
		else
		{
			++mapIt;
		}
	}

	delete *itStack;
	m_Stacks.erase(itStack);
	return MSG_HANDLED;
}

///	\brief	rename existing undo stack
///	\param	size - size of UNDOSTACKRENAME structure
///	\param	param - pointer to UNDOSTACKRENAME structure
DWORD CUndoRedoComponent::OnUndoRenameStack(DWORD size, void *param)
{
	UNDOSTACKRENAME *pRename = reinterpret_cast<UNDOSTACKRENAME *>(param);
	VERIFY_MESSAGE_SIZE(size, sizeof(*pRename));
	IHashString *pName = pRename->pStackName;
	CUndoStack *pStack = GetStack(pName);
	if (pStack == NULL)
	{
		m_pToolBox->Log(LOGERROR, _T("%s(%i): Can't find stack '%s'.\n"), __FILE__, __LINE__, pName->GetString());
		return MSG_ERROR;
	}
	IHashString *pNewName = pRename->pNewStackName;
	if (FindStack(pNewName) != m_Stacks.end())
	{
		m_pToolBox->Log(LOGERROR, _T("%s(%i): Can't rename stack '%s' to '%s'. Such stack exists already.\n"),
										__FILE__, __LINE__, pName->GetString(), pNewName->GetString());
		return MSG_ERROR;
	}
	pStack->SetName(pRename->pNewStackName);
	return MSG_HANDLED;
}

///	\brief	register reference from view to stack
///	\param	size - size of UNDOVIEWTOSTACK structure
///	\param	param - pointer to UNDOVIEWTOSTACK structure
DWORD CUndoRedoComponent::OnUndoRegisterViewStack(DWORD size, void *param)
{
	UNDOVIEWTOSTACK *pViewToStack = reinterpret_cast<UNDOVIEWTOSTACK *>(param);
	VERIFY_MESSAGE_SIZE(size, sizeof(*pViewToStack));
	if (pViewToStack->pStackView == NULL)
	{
		ASSERT(!"NULL pointer to CView was passsed");
		return MSG_ERROR;
	}
	if (pViewToStack->pStackName == NULL)
	{
		ASSERT(!"NULL pointer to stack name was passsed");
		return MSG_ERROR;
	}
	CUndoStack *pStack = GetStack(pViewToStack->pStackName);
	if (pStack == NULL)
	{
		ASSERT(!"Unknown stack name was passsed");
		return MSG_ERROR;
	}
	View2StackMap::iterator it = m_View2Stack.find(pViewToStack->pStackView);
	if (it != m_View2Stack.end())
	{
		ASSERT(!"View already registered");
		return MSG_ERROR;
	}
	m_View2Stack.insert(make_pair(pViewToStack->pStackView, pStack));
	return MSG_HANDLED;
}

///	\brief	clear undo stack. This is useful for operations that cannot be undone
///	\param	size - unused
///	\param	param - unused
DWORD CUndoRedoComponent::OnUndoClearStack(DWORD size, void *param)
{
	ASSERT(m_pCurrentCommandData == NULL);
	UNDOSTACKINFO *pInfo = reinterpret_cast<UNDOSTACKINFO *>(param);
	VERIFY_MESSAGE_SIZE(size, sizeof(*pInfo));
	if (NULL != pInfo->pStackName)
	{
		StackList::iterator itStack = FindStack(pInfo->pStackName);
		if (itStack == m_Stacks.end())
		{
			m_pToolBox->Log(LOGERROR, _T("%s(%i): Can't find undo stack '%s' for clearing .\n"), __FILE__, __LINE__, pInfo->pStackName->GetString());
			return MSG_ERROR;
		}
		(*itStack)->ClearStack();
	}
	else
	{
		CUndoStack *pStack = GetStack();
		if (pStack == NULL)
		{
			return MSG_ERROR;
		}
		pStack->ClearStack();
	}
	return MSG_HANDLED;
}

///	\brief	start collecting data for next undo command
///	\param	size - unused
///	\param	param - unused
DWORD CUndoRedoComponent::OnUndoStartCommand(DWORD size, void *param)
{
	if (m_pCurrentCommandData != NULL)
	{
		ASSERT(!"Undo command is started already");
		m_pToolBox->Log(LOGWARNING, _T("New undo command is started already\n"));
		return MSG_ERROR;
	}
	else
	{
		m_pCurrentCommandData = new CUndoCommandData();
		return MSG_HANDLED;
	}
}

///	\brief	end collecting data for undo command and place it into undo stack
///	\param	size - unused
///	\param	param - unused
DWORD CUndoRedoComponent::OnUndoEndCommand(DWORD size, void *param)
{
	if (m_pCurrentCommandData == NULL)
	{
		// this error means that command end may occur early then it expects
		ASSERT(!"Undo command was ended");
		m_pToolBox->Log(LOGERROR, _T("Undo command was ended\n"));
		return MSG_ERROR;
	}
	// check if this is valid command
	if (m_pCurrentCommandData->HasUndoActions())
	{
		CUndoStack *pStack = GetStack();
		if (pStack == NULL)
		{
			delete m_pCurrentCommandData;
			m_pCurrentCommandData = NULL;
			return MSG_ERROR;
		}
		m_pCurrentCommandData->CompactData();
		pStack->AddUndo(m_pCurrentCommandData);
	}
	else
	{
		delete m_pCurrentCommandData;
	}
	m_pCurrentCommandData = NULL;
	return MSG_HANDLED;
}

///	\brief	clear collected data for undo command
///	\param	size - unused
///	\param	param - unused
///	\return EE message code
DWORD CUndoRedoComponent::OnUndoCancelCommand(DWORD size, void *param)
{
	ASSERT(m_pCurrentCommandData != NULL);
	delete m_pCurrentCommandData;
	m_pCurrentCommandData = NULL;
	return MSG_HANDLED;
}

///	\brief	add new undo action to undo stack
///	\param	size - size of IUndoRedoAction
///	\param	param - pointer to IUndoRedoAction
///	\return EE message code
DWORD CUndoRedoComponent::OnUndoAddAction(DWORD size, void *param)
{
	if (m_pCurrentCommandData == NULL)
	{
		assert(!"UndoStartCommand was not sent");
		m_pToolBox->Log(LOGERROR, _T("Adding undo action without started command\n"));
		return MSG_ERROR;
	}

	VERIFY_MESSAGE_SIZE(size, sizeof(IUndoRedoAction));
	IUndoRedoAction *pAction = static_cast<IUndoRedoAction*>(param);
	if (pAction == NULL)
	{
		return MSG_ERROR;
	}

	return AddAction(pAction, true);
}

///	\brief	add new redo action to undo stack
///	\param	size - size of IUndoRedoAction
///	\param	param - pointer to IUndoRedoAction
///	\return EE message code
DWORD CUndoRedoComponent::OnRedoAddAction(DWORD size, void *param)
{
	if (m_pCurrentCommandData == NULL)
	{
		assert(!"UndoStartCommand was not sent");
		m_pToolBox->Log(LOGERROR, _T("Adding redo action without started command\n"));
		return MSG_ERROR;
	}

	VERIFY_MESSAGE_SIZE(size, sizeof(IUndoRedoAction));
	IUndoRedoAction *pAction = static_cast<IUndoRedoAction*>(param);
	if (pAction == NULL)
	{
		return MSG_ERROR;
	}

	return AddAction(pAction, false);
}

///	\brief	serialize current object state for undo action
///	\param	size - sizeof(IHashString*)
///	\param	param - pointer to IHashString with name of object to serialize
DWORD CUndoRedoComponent::OnUndoSaveObject(DWORD size, void *param)
{
	ASSERT(m_pCurrentCommandData != NULL);

	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));
	IHashString *pObjectName = GetName(param);
	if (pObjectName == NULL)
	{
		return MSG_ERROR;
	}

	IUndoRedoAction *pAction = new CObjectSerializeAction(pObjectName);
	DWORD res = AddAction(pAction, true);
	if (res == MSG_HANDLED)
	{
		res = m_DependantProcessors.ProcessObject(pObjectName, true);
	}
	else
	{
		delete pAction;
	}

	return res;
}

///	\brief	serialize current object state for redo action
///	\param	size - sizeof(IHashString*)
///	\param	param - pointer to IHashString with name of object to serialize
DWORD CUndoRedoComponent::OnRedoSaveObject(DWORD size, void *param)
{
	ASSERT(m_pCurrentCommandData != NULL);

	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));
	IHashString *pObjectName = GetName(param);
	if (pObjectName == NULL)
	{
		return MSG_ERROR;
	}

	DWORD res = m_DependantProcessors.ProcessObject(pObjectName, false);
	if (MSG_HANDLED != res)
	{
		return res;
	}

	IUndoRedoAction *pAction = new CObjectSerializeAction(pObjectName);
	res = AddAction(pAction, false);
	if (res != MSG_HANDLED)
	{
		delete pAction;
	}
	return res;
}

///	\brief	serialize current texture data for undo action
///	\param	size - sizeof(IHashString*)
///	\param	param - pointer to IHashString with name of texture to serialize
DWORD CUndoRedoComponent::OnUndoSaveTexture(DWORD size, void *param)
{
	ASSERT(m_pCurrentCommandData != NULL);

	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));
	IHashString *pTextureName = GetName(param);
	if (pTextureName == NULL)
	{
		return MSG_ERROR;
	}

	IUndoRedoAction *pAction = new CTextureDataAction(pTextureName);
	DWORD res = AddAction(pAction, true);
	if (res != MSG_HANDLED)
	{
		delete pAction;
	}
	return res;
}

///	\brief	serialize current texture data for redo action
///	\param	size - sizeof(IHashString*)
///	\param	param - pointer to IHashString with name of texture to serialize
DWORD CUndoRedoComponent::OnRedoSaveTexture(DWORD size, void *param)
{
	ASSERT(m_pCurrentCommandData != NULL);

	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));
	IHashString *pTextureName = GetName(param);
	if (pTextureName == NULL)
	{
		return MSG_ERROR;
	}

	IUndoRedoAction *pAction = new CTextureDataAction(pTextureName);
	DWORD res = AddAction(pAction, false);
	if (res != MSG_HANDLED)
	{
		delete pAction;
	}
	return res;
}

///	\brief	check if undo(redo) action is executing now
///	\param	size - sizeof(bool)
///	\param	param - pointer to bool for output bool value
///	\return EE message code
DWORD CUndoRedoComponent::OnUndoIsExecuting(DWORD size, void *param)
{
	bool *pData = static_cast<bool*>(param);
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	*pData = m_bIsExecuting;
	return MSG_HANDLED;
}

//////////////////// UI message handlers ///////////////////////////////////

/// \brief	undo last command from stack
void CUndoRedoComponent::OnUndo()
{
	CUndoStack *pStack = GetStack();
	if (pStack != NULL && pStack->CanUndo())
	{
		CWinApp* pApp = AfxGetApp();
		pApp->BeginWaitCursor();
		m_bIsExecuting = true;
		pStack->Undo();
		m_bIsExecuting = false;
		// send out a message that the scene is modified according to save point value in the stack
		static DWORD msgHash_WorldModified = CHashString(_T("WorldModified")).GetUniqueID();
		m_pToolBox->SendMessage(msgHash_WorldModified, sizeof(BOOL), (void *)!pStack->IsSavePoint());

		pApp->EndWaitCursor();
	}
}

/// \brief	update Undo menu item state
void CUndoRedoComponent::OnUpdateUndo(CCmdUI* pCmdUI)
{
	CUndoStack *pStack = GetStack();
	pCmdUI->Enable(pStack != NULL && pStack->CanUndo());
}

/// \brief	redo last rolled back command
void CUndoRedoComponent::OnRedo()
{
	CUndoStack *pStack = GetStack();
	if (pStack != NULL && pStack->CanRedo())
	{
		CWinApp* pApp = AfxGetApp();
		pApp->BeginWaitCursor();
		m_bIsExecuting = true;
		pStack->Redo();
		m_bIsExecuting = false;
		// send out a message that the scene is modified according to save point value in the stack
		static DWORD msgHash_WorldModified = CHashString(_T("WorldModified")).GetUniqueID();
		m_pToolBox->SendMessage(msgHash_WorldModified, sizeof(BOOL), (void *)!pStack->IsSavePoint());
		pApp->EndWaitCursor();
	}
}

/// \brief	update Redo menu item state
void CUndoRedoComponent::OnUpdateRedo(CCmdUI* pCmdUI)
{
	CUndoStack *pStack = GetStack();
	pCmdUI->Enable(pStack != NULL && pStack->CanRedo());
}

//////////////////// Helper functions //////////////////////////////////////

///	\brief	verify passed EE parameter for passing pointer to IHashString
///	\param	size - sizeof(IHashString*)
///	\param	param - pointer to IHashString with name of object to serialize
///	\return pointer to IHashString with non-empty name
///			NULL pointer otherwise
IHashString *CUndoRedoComponent::GetName(void *param) const
{
	IHashString *pObjectName = reinterpret_cast<IHashString *>(param);
	if (pObjectName != NULL)
	{
		if (pObjectName->IsEmpty())
		{
			pObjectName = NULL;
		}
	}
	if (pObjectName == NULL)
	{
		m_pToolBox->Log(LOGERROR, _T("%s(%i): Empty name was passed.\n"), __FILE__, __LINE__);
	}
	return pObjectName;
}

///	\brief	add new action to current command
///	\param	pAction - pointer to new action
///	\param	bUndo - specify target list for action
///					if true then pAction will be added to undo list
DWORD CUndoRedoComponent::AddAction(IUndoRedoAction *pAction, bool bUndo)
{
	ASSERT(m_pCurrentCommandData != NULL);
	DWORD res = pAction->Initialize();
	if (res == MSG_HANDLED)
	{
		m_pCurrentCommandData->AddAction(pAction, bUndo);
	}
	else
	{
		// TODO: notify error?
	}
	return res;
}

///	\brief	get pointer to stack by active scene or current view
///	\return	pointer to active stack or NULL otherwise
CUndoStack *CUndoRedoComponent::GetStack()
{
	CHashString hszScene = GetActiveScene();
	if (hszScene.IsEmpty())
	{
		CView* pActiveView = NULL;
		static DWORD msgHash_GetActiveView = CHashString(_T("GetActiveView")).GetUniqueID();
		if (MSG_HANDLED != m_pToolBox->SendMessage(msgHash_GetActiveView, sizeof(pActiveView), &pActiveView) &&
			pActiveView == NULL)
		{
			m_pToolBox->Log(LOGERROR, _T("%s(%i): GetActiveView failed\n"), __FILE__, __LINE__);
			return NULL;
		}
		return GetStack(pActiveView);
	}
	return GetStack(&hszScene);
}

///	\brief	get pointer to stack by assigned view
///	\param	pView - pointer to assigned view.
///	\return	pointer to assigned stack or NULL otherwise
CUndoStack *CUndoRedoComponent::GetStack(CView *pView)
{
	View2StackMap::iterator it = m_View2Stack.find(pView);
	if (it == m_View2Stack.end())
	{
		m_pToolBox->Log(LOGERROR, _T("%s(%i): Get undo stack by view failed\n"), __FILE__, __LINE__);
		return NULL;
	}
	return it->second;
}

///	\brief	get pointer to stack by stack name
///	\param	pName - pointer to IHashString with name of the stack
///	\return	pointer to found stack or NULL otherwise
CUndoStack *CUndoRedoComponent::GetStack(IHashString *pName)
{
	StackList::iterator it = FindStack(pName);
	if (it == m_Stacks.end())
	{
		m_pToolBox->Log(LOGERROR, _T("%s(%i): Get undo stack by name '%s' failed\n"), __FILE__, __LINE__, pName->GetString());
		return NULL;
	}
	return *it;
}

///	\brief	get stack list iterator by stack name
///	\param	pName - pointer to IHashString with name of the stack
///	\return	iterator in m_Stacks container
CUndoRedoComponent::StackList::iterator CUndoRedoComponent::FindStack(IHashString *pName)
{
	return find_if(m_Stacks.begin(), m_Stacks.end(), bind2nd(mem_fun(&CUndoStack::operator ==), pName));
}
