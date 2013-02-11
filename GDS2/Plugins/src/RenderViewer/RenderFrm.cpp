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
#include "RenderViewer.h"
#include "RenderFrm.h"
#include ".\renderfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CRenderFrm

IMPLEMENT_DYNCREATE(CRenderFrm, CMDIChildWnd)

CRenderFrm::CRenderFrm()
{
	m_ToolBox = EngineGetToolBox();
	CreateDisplayList();
}

CRenderFrm::~CRenderFrm()
{
	RemoveDisplayList();
}


BEGIN_MESSAGE_MAP(CRenderFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CRenderFrm)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_PLAYFULLSCREEN_START, ID_PLAYFULLSCREEN_END, OnClickPlayFullScreen)
	ON_COMMAND(ID_STOP, OnStopFullScreen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CRenderFrm::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CMDIFrameWnd *mainWnd;
	RECT rectMain;
		
	// Register the doc templates we provide to the app
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, pApp->m_pMainWnd);

	// get the apps instance handle for loading resources
	m_AppInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(RenderViewerDLL.hModule);

	mainWnd->GetClientRect(&rectMain);

	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
/*	if (!m_ExampleToolPal.CreateEx(mainWnd, TBSTYLE_FLAT | TBSTYLE_WRAPABLE , WS_CHILD | WS_VISIBLE | CBRS_RIGHT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED, CRect(0, 2, 2, 4)) ||
		!m_ExampleToolPal.LoadToolBar(IDR_EXAMPLE_TOOL_PALETTE))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_ExampleToolPal.SetBorders(2, 2, 2, 2);

	m_ExampleToolPal.EnableDocking(CBRS_ORIENT_ANY);
	// make control bar notify us.

	mainWnd->DockControlBar(&m_ExampleToolPal);

	CRect tcRect;
	m_ExampleToolPal.GetToolBarCtrl().SetRows(1, FALSE, tcRect);
	int iconWidth;
	int iconHeight;
	int nRows;

	iconWidth = tcRect.Width()/NUMICONS;
	iconHeight = tcRect.Height();
	nRows = (NUMICONS / NICONSWIDE) + (((NUMICONS % NICONSWIDE) == 0) ? 0 : 1);

	tcRect.SetRect(0, 0, iconWidth * NICONSWIDE, (iconHeight * nRows));

	// Set the window size
	m_ExampleToolPal.SetWindowPos(NULL, 0, 0, tcRect.Width(), tcRect.Height(),
		SWP_NOACTIVATE  | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREDRAW);

	
	mainWnd->ShowControlBar(&m_ExampleToolPal, FALSE, FALSE);
*/	
	AfxSetResourceHandle(m_AppInst);
	
	return 0;
}

void CRenderFrm::CreateDisplayList()
{
	LPDEVMODE pMode;
	DWORD iModeNum = 0;
	BOOL bVal = 1;

	// get all possible display settings
	DISPLAY_DEVICE devInfo;
	devInfo.cb = sizeof(DISPLAY_DEVICE);
	bVal = EnumDisplayDevices(NULL, 0, &devInfo, 0);
	if (bVal == 0)
	{
		//error
		::MessageBox(NULL, _T("Could not find display device"), _T("ERROR"), MB_OK);
		return;
	}
	int iOldX = -1;
	int iOldY = -1;
	int iNewX = -1;
	int iNewY = -1;
	while(bVal)
	{
		pMode = new DEVMODE;
		//bVal = EnumDisplaySettings(devInfo.DeviceName, iModeNum, pMode);
		bVal = EnumDisplaySettings(NULL, iModeNum, pMode);
		iNewX = pMode->dmPelsWidth;
		iNewY = pMode->dmPelsHeight;

		if ( bVal && ( iNewX != iOldX || iNewY != iOldY) )
		{
			m_DisplayList.push_back(pMode);
			iOldX = iNewX;
			iOldY = iNewY;
		}
		else
		{
			delete pMode;
		}
		iModeNum++;
	}
}

void CRenderFrm::RemoveDisplayList()
{
	// clear display list
	if(m_DisplayList.size() != 0)
	{
		DISPLAYLIST::iterator iter = m_DisplayList.begin();
		DISPLAYLIST::iterator delIter;
		while(iter != m_DisplayList.end()) 
		{
			delIter = iter;
			iter++;
			delete (*delIter);
		}
	}
}

void CRenderFrm::OnClickPlayFullScreen(UINT uID)
{
	LPDEVMODE pMode;
	UINT nPos = uID - ID_PLAYFULLSCREEN_START;
	UINT nXRes;
	UINT nYRes;
	
	DISPLAYLIST::iterator iter = m_DisplayList.begin();
	UINT count = 0;

	// check to make sure desired resolution is w/i the display list
	if (nPos > m_DisplayList.size())
	{
		// error
		m_ToolBox->SetErrorValue(WARN_INVALID_OPERATION);
		m_ToolBox->Log(LOGWARNING, _T("Selected resolution not found in display list"));
		return;
	}

	// find the desired resolution in the display list
	for(; iter != m_DisplayList.end(); iter++)
	{
		if (nPos == count)
		{
			pMode = *iter;
			break;
		}
		count++;
	}

	// check that the resolution mode is valid
	if(pMode == NULL)
	{
		// error
		m_ToolBox->SetErrorValue(WARN_INVALID_OPERATION);
		m_ToolBox->Log(LOGWARNING, _T("Selected resolution is not valid"));
		return;
	}

	// get the x and y resolution
	nXRes = pMode->dmPelsWidth;
	nYRes = pMode->dmPelsHeight;
	
	// go to full screen mode
	CRenderViewer *pViewer = (CRenderViewer*)GetDLLInterface();
	pViewer->ViewFullScreen(nXRes, nYRes);
}

void CRenderFrm::SetupFullScreenMenu()
{
#if 0
	// get the play full screen menu
	MENUINFOMESSAGE mim;
	HMENU hmenu;
	HMENU subMenu = NULL;

	static DWORD msgHash_GetMenuInfo = CHashString(_T("GetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);

	hmenu = mim.m_hMenu;
	if (hmenu == NULL)
	{
		return;
	}

	// find the Play FullScreen submenu to fill
	for (int i=0; i<GetMenuItemCount(hmenu); i++)
	{
		TCHAR label[50];
		GetMenuString(hmenu, i, label, 100, MF_BYPOSITION);

		// search for Render menu
		if ( _tcscmp(label, _T("&Render")) == 0 )
		{
			HMENU tempMenu = GetSubMenu(hmenu, i);
			for (int j=0; j<GetMenuItemCount(tempMenu); j++)
			{
				GetMenuString(tempMenu, j, label, 100, MF_BYPOSITION);

				// seach for Play FullScreen label
				if ( _tcscmp(label, _T("Play &FullScreen")) == 0 )
				{
					subMenu = GetSubMenu(tempMenu, j);
					break;
				}
			}
			break;
		}
	}
	
	if(subMenu == NULL)
	{
		return;
	}

	if(GetMenuItemCount(subMenu) > 1)
	{
		// menu has already been filled
		return;
	}

	// fill full screen menu w/ possible resolution settings
	int iCmndID = ID_PLAYFULLSCREEN_START;
	DISPLAYLIST::iterator iter = m_DisplayList.begin();
	LPDEVMODE pMode;
	MENUITEMINFO menuInfo;
	menuInfo.cbSize = sizeof(MENUITEMINFO);
	menuInfo.fMask = MIIM_STATE;
	menuInfo.fState = MFS_ENABLED;// | MFS_CHECKED;
	for(; iter != m_DisplayList.end(); iter++)
	{
		StdString szResString;
		StdString szX = _T("x");
		TCHAR szXRes[50];
		TCHAR szYRes[50];

		pMode = *iter;
		_itot((int)pMode->dmPelsWidth, szXRes,10);
		_itot((int)pMode->dmPelsHeight, szYRes, 10);
		szResString = szXRes + szX;
		szResString = szResString + szYRes;
		
		InsertMenu(subMenu, i, MF_BYPOSITION, iCmndID, szResString);
		SetMenuItemInfo(subMenu, iCmndID, FALSE, &menuInfo);
		i++;
		iCmndID++;
	}
#endif
}

void CRenderFrm::ActivateFrame(int nCmdShow)
{
	CMDIChildWnd::ActivateFrame(nCmdShow);
	SetupFullScreenMenu();
}

void CRenderFrm::OnStopFullScreen()
{
	// turn off the input manager
	CHashString inputManager(_T("CInputManager"));
	static DWORD msgHash_Stop = CHashString(_T("Stop")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_Stop, 0, 0, NULL, &inputManager);
	static DWORD msgHash_End = CHashString(_T("End")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_End, 0, 0, NULL, &inputManager);
	
	CRenderViewer *pViewer = (CRenderViewer*)GetDLLInterface();
	pViewer->ViewFullScreen(0, 0);
}

// CRenderFrm message handlers
