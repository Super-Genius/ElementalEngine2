///============================================================================
/// \file		SceneComponent.cpp
/// \brief		Header file for Scene to hook up to Elemental Engine
/// \date		04-22-2005
/// \author		Kenneth Hurley
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON(CSceneComponent);
// the messages we receive
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CSceneComponent);
REGISTER_MESSAGE_HANDLER_PRIORITY(WindowsMainCreate, OnCreate, CSceneComponent, 0x70000000);
REGISTER_MESSAGE_HANDLER(FileLoaded_wxl, OnWorldLoad, CSceneComponent);
REGISTER_MESSAGE_HANDLER(WorldModified, OnWorldModified, CSceneComponent);
REGISTER_MESSAGE_HANDLER(CreateBlankScene, OnCreateScene, CSceneComponent);
REGISTER_MESSAGE_HANDLER(CloseScene, OnCloseScene, CSceneComponent);
REGISTER_MESSAGE_HANDLER(AddObject, OnAddObject, CSceneComponent);
REGISTER_MESSAGE_HANDLER(RemoveObject, OnRemoveObject, CSceneComponent);
REGISTER_MESSAGE_HANDLER(SelectObject, OnSelectObject, CSceneComponent);
REGISTER_MESSAGE_HANDLER(ChangeObjectName, OnChangeObjectName, CSceneComponent);
REGISTER_MESSAGE_HANDLER(ChangeObjectParent, OnChangeObjectParent, CSceneComponent);
REGISTER_MESSAGE_HANDLER(GetSceneSavePath, OnGetSceneSavePath, CSceneComponent);
REGISTER_MESSAGE_HANDLER(SetActiveScene, OnSetActiveScene, CSceneComponent);
REGISTER_MESSAGE_HANDLER(HierarchyChanged, OnHierarchyChanged, CSceneComponent);
REGISTER_MESSAGE_HANDLER(GlobalUpdateTick, OnGlobalUpdateTick, CSceneComponent);
///priority message to get before anything else so that the world is destroyed
/// and the user can be prompted with a save current scene option
REGISTER_MESSAGE_HANDLER_PRIORITY(LoadFileByExtension, OnLoadNewFile, CSceneComponent, 0x70000000);
REGISTER_MESSAGE_HANDLER_PRIORITY(CreateObject, OnCreateObject, CSceneComponent, 0xa0000000);
REGISTER_MESSAGE_HANDLER_PRIORITY(InitObject, OnInitObject, CSceneComponent, 0xa0000000);

CSceneComponent::CSceneComponent()
{
	// INIT to NULL
	m_pSceneEditor = NULL;
}

// we don't want elemental or anybody else creating us.
IComponent *CSceneComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CSceneComponent);
}

// serialize function for this object
void CSceneComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CSceneComponent::GetComponentType()
{
	static CHashString compName(_T("CSceneComponent"));
	return &compName;
}

bool CSceneComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// Set the parent of the component
void CSceneComponent::SetParent(CSceneDLDockWin *parent)
{
	m_Parent = parent;
}

// message to handle OnCmdMsgs from main window
DWORD CSceneComponent::OnCmdMsg(DWORD size, void *param)
{
	CMDMESSAGE *pCmdMsg;
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDMESSAGE));
	pCmdMsg = (CMDMESSAGE *)param;

	// ZSOLT: I commented out this, because  forwarding messages to m_pSceneEditor
	// will forward the messages to m_parent as well, but we need the other windows 
	// to get the messages (e.g. Camera dock window), so we should not skip m_pSceneEditor
	// as nothing else has pointer to that windows. To avoid double message delivery, I 
	// commented out the original direct call of m_Parent->onCmdMsg. 
	// forward to parent.

	//if (m_Parent->OnCmdMsg(pCmdMsg->m_nID, pCmdMsg->m_nCode, pCmdMsg->m_pExtra,
	//	pCmdMsg->m_pHandlerInfo))
	//{
	//	pCmdMsg->retVal = true;
	//	return MSG_HANDLED_PROCEED;
	//}

	if ( m_pSceneEditor != NULL && m_pSceneEditor->OnCmdMsg(pCmdMsg->m_nID, pCmdMsg->m_nCode, pCmdMsg->m_pExtra,
		pCmdMsg->m_pHandlerInfo))
	{
		pCmdMsg->retVal = true;
		return MSG_HANDLED_PROCEED;
	}

	// it wasn't handled, so return that
	return MSG_NOT_HANDLED;
}

// message to handle WindowsCreate function
DWORD CSceneComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	
	// we save this for convenience
	m_pSceneEditor = SINGLETONINSTANCE(CSceneEditor);

	if( m_pSceneEditor != NULL ) 
	{
		// calling create function
		m_pSceneEditor->OnCreate((CMDIFrameWnd *)param);
	}
	else 
	{
		// error handling
		EngineGetToolBox()->Log( LOGERROR,  _T("%s(%i):Error creating Scene Editor.\n"), __FILE__, __LINE__ );
	}

	return MSG_HANDLED_PROCEED;
}

// function to handle Create object message
DWORD CSceneComponent::OnCreateObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(sizeof(CREATEOBJECTPARAMS), size);
	CREATEOBJECTPARAMS *pCOP = (CREATEOBJECTPARAMS*)param;

	// TODO: provide correct update
	//m_Parent->UpdateHierarchy();

	return MSG_HANDLED_PROCEED;
}

DWORD CSceneComponent::OnWorldLoad(DWORD size, void *param)
{
	LPTSTR name = (LPTSTR)param;
	VERIFY_MESSAGE_SIZE(size, sizeof(LPTSTR));
	m_Parent->OpenScene(name);
	return MSG_HANDLED_PROCEED;
}

DWORD CSceneComponent::OnLoadNewFile( DWORD size, void * param)
{
	LOADFILEEXTPARAMS *lfep;
	VERIFY_MESSAGE_SIZE(size, sizeof(LOADFILEEXTPARAMS));
	lfep =(LOADFILEEXTPARAMS *)param;
	//check extension
	LPCTSTR szExt = FileHelpers::GetExtension( lfep->fileName );
	//do a lower string compare
	if( szExt != NULL && !_tcsicmp(szExt, _T("wxl") ) )
	{
		//ask for a save if needed
		m_Parent->CheckSaveScene();
		m_Parent->CloseScene();
		return MSG_HANDLED_PROCEED;
	}
	return MSG_NOT_HANDLED;
}

DWORD CSceneComponent::OnWorldModified( DWORD size, void * param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(BOOL));
	m_Parent->OnWorldModified((BOOL)param);
	return MSG_HANDLED_PROCEED;
}

DWORD CSceneComponent::OnCreateScene( DWORD size, void * param)
{
	if(!m_Parent->IsSceneOpen())
	{
		m_Parent->CloseScene();
        m_Parent->CreateScene();
	}
	return MSG_HANDLED_PROCEED;
}

DWORD CSceneComponent::OnCloseScene( DWORD size, void * param)
{
	int retval;

	//does an old scene exist?
	if (m_Parent->IsSceneOpen())
	{
		retval = AfxGetApp()->GetMainWnd()->MessageBox(
			"Do you want to destroy the current scene?", "Destroy Old Scene", 
			MB_OKCANCEL | MB_SETFOREGROUND | MB_TOPMOST );
	}

	if( retval == IDOK )
	{
		m_Parent->CloseScene();
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CSceneComponent::OnGetSceneName( DWORD size, void * param)
{
	return MSG_HANDLED_PROCEED;
}

DWORD CSceneComponent::OnAddObject( DWORD size, void * param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CREATEOBJECTPARAMS));
	CREATEOBJECTPARAMS *cop = (CREATEOBJECTPARAMS *)param;
	m_Parent->AddHierarchy(cop->name, cop->typeName, cop->parentName);
	m_Parent->SelectFromHierarchy(cop->name);

	return MSG_HANDLED_PROCEED;
}

DWORD CSceneComponent::OnRemoveObject( DWORD size, void * param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));
	IHashString *name = (IHashString *)param;
	m_Parent->RemoveHierarchy(name);

	return MSG_HANDLED_PROCEED;
}

DWORD CSceneComponent::OnSelectObject( DWORD size, void * param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(SELECTOBJECTPARAMS));
	SELECTOBJECTPARAMS *pData = (SELECTOBJECTPARAMS*)param;
	m_Parent->SelectFromHierarchy(pData->object);
	return MSG_HANDLED_PROCEED;
}

DWORD CSceneComponent::OnChangeObjectName( DWORD size, void * param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CHANGEOBJNAME));
	CHANGEOBJNAME *pData = (CHANGEOBJNAME*)param;
	m_Parent->RenameObject( pData->oldName, pData->newName );
	return MSG_HANDLED_PROCEED;
}

DWORD CSceneComponent::OnChangeObjectParent( DWORD size, void * param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CHANGEOBJECTPARENT));
	CHANGEOBJECTPARENT *pData = (CHANGEOBJECTPARENT*)param;
	m_Parent->ChangeParentName(pData->objectName, pData->objectCompType, pData->newParentName);
	m_Parent->SelectFromHierarchy( pData->objectName );
	return MSG_HANDLED_PROCEED;
}

DWORD CSceneComponent::OnGetSceneSavePath( DWORD size, void * param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));
	IHashString *path = (IHashString *)param;
	m_Parent->GetSceneSavePath( path );
	return MSG_HANDLED_PROCEED;
}

/// message to change the currently active scene
DWORD CSceneComponent::OnSetActiveScene(DWORD size, void* data)
{
	GETSETACTIVESCENE *gsas = reinterpret_cast<GETSETACTIVESCENE*>(data);
	VERIFY_MESSAGE_SIZE(sizeof(gsas), size);

	if (gsas->pSceneName == NULL)
	{
		ASSERT(!"Empty scene name passed");
		return MSG_ERROR;
	}

	// mouse movement on the view generates OnViewActivate event. 
	// This leads to SetActiveScene messages flow
	m_Parent->UpdateHierarchy(false);
	return MSG_HANDLED_PROCEED;
}

/// message about the currently active scene hierarchy changes
DWORD CSceneComponent::OnHierarchyChanged(DWORD size, void* data)
{
	GETSETACTIVESCENE *gsas = reinterpret_cast<GETSETACTIVESCENE*>(data);
	VERIFY_MESSAGE_SIZE(sizeof(gsas), size);

	if (gsas->pSceneName == NULL)
	{
		ASSERT(!"Empty scene name passed");
		return MSG_ERROR;
	}

	// mouse movement on the view generates OnViewActivate event. 
	// This leads to SetActiveScene messages flow
	m_Parent->UpdateHierarchy();
	return MSG_HANDLED_PROCEED;
}

DWORD CSceneComponent::OnGlobalUpdateTick(DWORD size, void* param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GLOBALTICKPARAMS));
	GLOBALTICKPARAMS *globalTick;
	globalTick = (GLOBALTICKPARAMS *)param;
	if( m_pSceneEditor != NULL )
	{
		m_pSceneEditor->OnUpdateTick(globalTick->tickCount);
	}
	return MSG_HANDLED_PROCEED;
}

///	check object label changes
DWORD CSceneComponent::OnInitObject(DWORD size, void* param)
{
	INITOBJECTPARAMS *pIOP = static_cast<INITOBJECTPARAMS *>( param );
	VERIFY_MESSAGE_SIZE(size, sizeof(LPTSTR));
	if (pIOP->name != NULL)
	{
		m_Parent->OnInitObject(pIOP->name);
	}
	return MSG_HANDLED_PROCEED;
}
