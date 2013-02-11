///============================================================================
/// \file		GDSComponent.cpp
/// \brief		Header file for GDS to link Elemental
///				engine to the MFC application
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
#include "GDSComponent.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON(CGDSComponent);
// the messages we receive
REGISTER_MESSAGE_HANDLER(GetMenuInfo, OnGetMenuInfo, CGDSComponent);
REGISTER_MESSAGE_HANDLER(SetMenuInfo, OnSetMenuInfo, CGDSComponent);
REGISTER_MESSAGE_HANDLER(RegisterFileTypeCreationInfo, OnRegisterFileTypeCreationInfo, CGDSComponent );
REGISTER_MESSAGE_HANDLER(AddToolBar, OnAddToolBar, CGDSComponent);
REGISTER_MESSAGE_HANDLER(IsToolBarActive, OnIsToolBarActive, CGDSComponent);
REGISTER_MESSAGE_HANDLER(SetActiveToolBar, OnSetActiveBar, CGDSComponent);
REGISTER_MESSAGE_HANDLER(LoadingUpdate, OnLoadingUpdate, CGDSComponent);
REGISTER_MESSAGE_HANDLER(HideToolBar, OnHideBar, CGDSComponent);
REGISTER_MESSAGE_HANDLER(GetResourceDirectory, OnGetResourceDirectory, CGDSComponent);
// receive logging messages and print them out to a file
REGISTER_MESSAGE_HANDLER_PRIORITY(LogMessage, OnLogMessage, CGDSComponent, 0x70000000);
REGISTER_MESSAGE_HANDLER(GetActiveView, OnGetActiveView, CGDSComponent);
REGISTER_MESSAGE_HANDLER(SetActiveView, OnSetActiveView, CGDSComponent);

// we don't want elemental or anybody else creating us.
IComponent *CGDSComponent::Create(int nArgs, va_list argptr)
{
	//return SINGLETONINSTANCE(CGDSComponent);
	return SINGLETONINSTANCE(CGDSComponent);
}

CGDSComponent::CGDSComponent()
{
	m_ToolBox = EngineGetToolBox();
	m_LogFile = NULL;
}

// serialize function for this object
void CGDSComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CGDSComponent::GetComponentType()
{
	static CHashString compName(_T("CGDSComponent"));
	return &compName;
}

bool CGDSComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// Set parent for this component
void CGDSComponent::SetParent(CGDSApp *parent)
{
	m_Parent = parent;
}

CMainFrame *CGDSComponent::GetMainFrame()
{
	return DYNAMIC_DOWNCAST(CMainFrame, theApp.m_pMainWnd);
}

DWORD CGDSComponent::OnGetMenuInfo(DWORD size, void *param)
{
	MENUINFOMESSAGE *pMenuInfo;
	VERIFY_MESSAGE_SIZE(size, sizeof(MENUINFOMESSAGE));
	pMenuInfo = (MENUINFOMESSAGE *)param;

	GetMainFrame()->GetMenuAndAccel(&pMenuInfo->m_hMenu, &pMenuInfo->m_hAccel);

	return MSG_HANDLED_STOP;
}

DWORD CGDSComponent::OnSetMenuInfo(DWORD size, void *param)
{
	CMainFrame *mainWnd;
	MENUINFOMESSAGE *pMenuInfo;
	VERIFY_MESSAGE_SIZE(size, sizeof(MENUINFOMESSAGE));
	pMenuInfo = (MENUINFOMESSAGE *)param;

	mainWnd = DYNAMIC_DOWNCAST(CMainFrame, theApp.m_pMainWnd);
	if ((pMenuInfo->m_hMenu != NULL) || (pMenuInfo->m_hAccel != NULL))
		mainWnd->SetMenuAndAccel(pMenuInfo->m_hMenu, pMenuInfo->m_hAccel);
	if ((pMenuInfo->m_MenuRsrc != NULL) || (pMenuInfo->m_MenuBitmap != NULL))
		mainWnd->LoadMenuToolBar(pMenuInfo->m_MenuRsrc, pMenuInfo->m_MenuBitmap);

	return MSG_HANDLED_STOP;
}

DWORD CGDSComponent::OnRegisterFileTypeCreationInfo(DWORD size, void *param)
{
	CMainFrame *mainWnd;
	FILETYPECREATIONINFO *pInfo;
	VERIFY_MESSAGE_SIZE(size, sizeof(FILETYPECREATIONINFO));
	pInfo = (FILETYPECREATIONINFO *)param;

	mainWnd = DYNAMIC_DOWNCAST(CMainFrame, theApp.m_pMainWnd);
	//try to register the filetype and its creation callback with the main app
	if( !mainWnd->OnRegisterFileTypeCreationInfo( pInfo ) )
	{
		return  MSG_ERROR;
	}
	return MSG_HANDLED_STOP;
}

DWORD CGDSComponent::OnAddToolBar(DWORD size, void *param)
{
	CMainFrame *mainWnd;
	TOOLBARSTRUCT *pToolBarStruct;
	VERIFY_MESSAGE_SIZE(size, sizeof(TOOLBARSTRUCT));
	pToolBarStruct = (TOOLBARSTRUCT *)param;

	mainWnd = DYNAMIC_DOWNCAST(CMainFrame, theApp.m_pMainWnd);
	mainWnd->AddToolBar(pToolBarStruct);

	return MSG_HANDLED_STOP;
}

DWORD CGDSComponent::OnIsToolBarActive( DWORD size, void *param )
{
	CMainFrame *mainWnd;
	TOOLBARACTIVESTRUCT *pToolBarStruct;
	VERIFY_MESSAGE_SIZE(size, sizeof(TOOLBARACTIVESTRUCT));
	pToolBarStruct = (TOOLBARACTIVESTRUCT *)param;

	mainWnd = DYNAMIC_DOWNCAST(CMainFrame, theApp.m_pMainWnd);
	mainWnd->IsBarActive( pToolBarStruct );
	return MSG_HANDLED_STOP;
}

DWORD CGDSComponent::OnSetActiveBar( DWORD size, void *param )
{
	CMainFrame *mainWnd;
	TOOLBARACTIVESTRUCT *pToolBarStruct;
	VERIFY_MESSAGE_SIZE(size, sizeof(TOOLBARACTIVESTRUCT));
	pToolBarStruct = (TOOLBARACTIVESTRUCT *)param;

	mainWnd = DYNAMIC_DOWNCAST(CMainFrame, theApp.m_pMainWnd);
	mainWnd->SetActiveBar(pToolBarStruct);
	return MSG_HANDLED_STOP;
}

DWORD CGDSComponent::OnHideBar( DWORD size, void *param )
{
	CMainFrame *mainWnd;
	VERIFY_MESSAGE_SIZE(size, sizeof(CControlBar *));
	mainWnd = DYNAMIC_DOWNCAST(CMainFrame, theApp.m_pMainWnd);
	mainWnd->HideBar((CControlBar *)param);
	return MSG_HANDLED_STOP;
}

DWORD CGDSComponent::OnLoadingUpdate(DWORD size, void *param)
{
	CMainFrame *mainWnd;
	DWORD percentComplete;
	LOADINGUPDATEPARAMS *pbp;
	VERIFY_MESSAGE_SIZE(size, sizeof(LOADINGUPDATEPARAMS));
	pbp = (LOADINGUPDATEPARAMS *)param;

	// no dividing by zero!
	if (pbp->totalSize != 0)
	{
		// calculate the percentage complete
        percentComplete = (((pbp->currentPosition << 16) / pbp->totalSize) * 100) >> 16;
	}
	else 
	{
		// send zero to clear the indicator
		percentComplete = 0;
	}

	// get the main frame
	mainWnd = DYNAMIC_DOWNCAST(CMainFrame, theApp.m_pMainWnd);
	// update the bar
	mainWnd->UpdateProgress(percentComplete);

	return MSG_HANDLED_STOP;
}

DWORD CGDSComponent::OnGetResourceDirectory(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(StdString*));

	if (m_Parent == NULL)
		return MSG_ERROR;

	StdString *pResourceDir = *(StdString**)param;
	*pResourceDir = m_Parent->GetResourcePath();
	return MSG_HANDLED_STOP;
}

/// Get currently selected view
/// \param param - address of pointer to a CView
DWORD CGDSComponent::OnGetActiveView(DWORD size, void *param)
{
	const CView **ppView = reinterpret_cast<const CView**>(param);
	VERIFY_MESSAGE_SIZE(size, sizeof(ppView));
	*ppView = GetMainFrame()->GetActiveView();
	return MSG_HANDLED_STOP;
}

/// Set currently selected view
/// \param param pointer to a CView
DWORD CGDSComponent::OnSetActiveView(DWORD size, void *param)
{
	CView *pView = reinterpret_cast<CView*>(param);
	VERIFY_MESSAGE_SIZE(size, sizeof(pView));
	GetMainFrame()->SetActiveView(pView);
	return MSG_HANDLED_STOP;
}

DWORD CGDSComponent::OnLogMessage(DWORD size, void *param)
{
	LOGMESSAGESTRUCT *logMessage;
	// can't do a verify message size, as that might cause recursion
	if (size != sizeof(LOGMESSAGESTRUCT))
	{
		return MSG_ERROR;
	}

	logMessage = (LOGMESSAGESTRUCT *)param;
	{
		if (m_LogFile == NULL)
		{
			m_LogFile = fopen(_T("gds.log"), "w+");
			if (m_LogFile == NULL)
			{
				MessageBox(NULL, _T("Unable to open log file.\naborting!..."), _T("Error"), MB_OK);
			}
		}

		fprintf(m_LogFile,logMessage->msg);
		fflush(m_LogFile);
		if (logMessage->severity == LOGFATALERROR)
		{
			fclose(m_LogFile);
			static DWORD msgHash_Exit = CHashString(_T("Exit")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_Exit, 0, NULL);
		}
		return MSG_HANDLED_PROCEED;
	}

	return MSG_NOT_HANDLED;
}
