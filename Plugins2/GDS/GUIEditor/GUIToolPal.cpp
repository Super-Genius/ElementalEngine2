// GUIToolPal.cpp : implementation file
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "GUIToolPal.h"
#include "resource.h"
#include ".\guitoolpal.h"


// CGUIToolPal dialog

IMPLEMENT_DYNAMIC(CGUIToolPal, CGuiDialogBar)
CGUIToolPal::CGUIToolPal()
{
	m_ToolBox = EngineGetToolBox();
	m_iStage = 0;
	m_szAddItemType = "";
	m_szAddItemName = "";
	m_iElementNum = 0;
	m_iHandleIndex = -1;
	m_iLastMousePosX = m_iLastMousePosY = 0;
	m_szBGName = _T("BGColor");
	m_szBGType = _T("CGUIBackground");
	m_szCurrentPage = _T("");
	m_szRectName = _T("SelectRect");
	m_szRectType = _T("CGUIRect");
	m_bSpacePressed = false;
	m_bCtrlPressed = false;
	m_bShiftPressed = false;
	m_bUpPressed = false;
	m_bDownPressed = false;
	m_bLeftPressed = false;
	m_bRightPressed = false;
}

CGUIToolPal::~CGUIToolPal()
{
}

BEGIN_MESSAGE_MAP(CGUIToolPal, CGuiDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
	ON_WM_HSCROLL()	
	ON_BN_CLICKED(ID_INFOBUTTON, OnBnClickedShowInfo)
	ON_BN_CLICKED(IDC_NEWUI, OnBnClickedNewUI)
	ON_BN_CLICKED(IDC_VIEWRESET, OnBnClickedViewReset)
	ON_BN_CLICKED(IDC_ADDPAGE, OnBnClickedAddPage)
	ON_BN_CLICKED(IDC_ADDELEMENT, OnBnClickedAddElement)
	ON_UPDATE_COMMAND_UI(ID_INFOBUTTON, OnUpdateButtons)	
	ON_UPDATE_COMMAND_UI(IDC_VIEWRESET, OnUpdateButtons)	
	ON_UPDATE_COMMAND_UI(IDC_NEWUI, OnUpdateButtons)	
	ON_UPDATE_COMMAND_UI(IDC_ADDPAGE, OnUpdateButtons)
	ON_UPDATE_COMMAND_UI(IDC_ADDELEMENT, OnUpdateButtons)
	
END_MESSAGE_MAP()


void CGUIToolPal::DoDataExchange(CDataExchange* pDX)
{
	
//	DDX_Control( pDX, ID_GUI_TOOLPAL_SHOW_PROP, m_ctrlShowToggleButton );

	CGuiDialogBar::DoDataExchange(pDX);	
}

void CGUIToolPal::OnGlobalUpdateTick( void )
{
	MOUSECOORDS mousePos;
	EngineGetToolBox()->SendMessage(_T("GetGUIMousePosition"), sizeof(MOUSECOORDS), &mousePos);
	m_InfoBox.UpdateMousePos(mousePos.m_lXPos, mousePos.m_lYPos);
	float zFact;
	EngineGetToolBox()->SendMessage(_T("GetZoomFactor"), sizeof(float), &zFact, NULL, NULL);
	m_InfoBox.UpdateZoomFactor(zFact);
}


void CGUIToolPal::CreateGUISystem()
{
	DWORD retval;
	LOADFILEEXTPARAMS lfep;
	lfep.fileName = _T("blank.gxl");
	lfep.bInternalLoad = true;
	retval = EngineGetToolBox()->SendMessage(_T("LoadFileByExtension"), sizeof(LOADFILEEXTPARAMS), &lfep, NULL, NULL);
	CHashString newVersion = _T("1.1d");
	retval = EngineGetToolBox()->SendMessage(_T("SetGUIVersion"), sizeof(CHashString), &newVersion);
	CHashString pageMe = _T("Page 1");
	retval = EngineGetToolBox()->SendMessage(_T("GUIManagerSetActivePage"), sizeof(IHashString), &pageMe, NULL, NULL);	
	bool editorMode = true;
	retval = EngineGetToolBox()->SendMessage(_T("SetGUIEditorMode"), sizeof(bool), &editorMode, NULL, NULL);
	CHashString name = _T("Button 0");
	CHashString type = _T("CGUIItem");
	m_pDockRef->CreateNewOptionTree(&name, &type);
}

void CGUIToolPal::RemoveGUISystem()
{
	ToggleGUISystem(false);
}

LRESULT CGUIToolPal::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = HandleInitDialog(wParam, lParam);
	m_GUIName.Init( _T("CGUIToolPal_Temp") );
	m_GUIVisible = false;
    UpdateData(FALSE);

	m_ResBox.Create(CGUIRESBOX_ID, AfxGetApp()->GetMainWnd());
	m_InfoBox.Create(CGUIINFOBOX_ID, AfxGetApp()->GetMainWnd());
	m_PageNameBox.Create(CGUIPAGENAMEBOX_ID, AfxGetApp()->GetMainWnd());
	m_AddElemBox.Create(CGUIADDELEMENT_ID, AfxGetApp()->GetMainWnd());
	return bRet;
}
void CGUIToolPal::OnUpdateButtons( CCmdUI* pCmdUI )
{
	
}
// CGUIToolPal message handlers
void CGUIToolPal::OnBnClickedShowToggle()
{
	bool bshow = (m_ctrlShowToggleButton.GetCheck() == BST_CHECKED );
	ToggleGUISystem( true );
}

void CGUIToolPal::OnBnClickedSetRes()
{
	m_ResBox.ShowWindow(SW_SHOWNORMAL);
	m_ResBox.SetActiveWindow();
	m_ResBox.BringWindowToTop();	
}

void CGUIToolPal::ToggleGUISystem( bool bshow )
{
	if( m_GUIVisible && !bshow )
	{
//		RemoveGUISystem();
	}
	else if( !m_GUIVisible && bshow )
	{
//		m_ToolBox->SendMessage(_T("InitGUIs"),0,NULL);
		CreateGUISystem();
//		m_PropertiesDlg.UpdateGUIObject();
		OnBnClickedProperties();
	}
}

void CGUIToolPal::OnBnClickedProperties()
{
	//start the properties dialog SW_HIDE
//	m_PropertiesDlg.ShowWindow( SW_SHOWNORMAL );
//	m_PropertiesDlg.SetActiveWindow();
//	m_PropertiesDlg.BringWindowToTop();	
}

void CGUIToolPal::OnBnClickedReset()
{
		
}

// this func only entered now when new ui is pressed
void CGUIToolPal::UpdateResolution(GUIWORKINGRES res)
{
	m_iResX = res.iSizeX;
	m_iResY = res.iSizeY;	
	m_InfoBox.UpdateResolution(m_iResX, m_iResY);

	DWORD retval;

	bool editorMode = true;
	retval = EngineGetToolBox()->SendMessage(_T("SetGUIEditorMode"), sizeof(bool), &editorMode, NULL, NULL);

	CHashString newVersion = _T("1.1d");
	retval = EngineGetToolBox()->SendMessage(_T("SetGUIVersion"), sizeof(CHashString), &newVersion);
    
	OnBnClickedViewReset();
	
	CWnd *pageBtn = GetDlgItem(IDC_ADDPAGE);
	if (pageBtn)
	{
		pageBtn->EnableWindow(true);
	}
}

void CGUIToolPal::OnBnClickedShowInfo()
{
	m_InfoBox.ShowWindow(SW_SHOWNORMAL);
	m_InfoBox.SetActiveWindow();
	m_InfoBox.BringWindowToTop();
}

void CGUIToolPal::OnBnClickedNewUI()
{
	m_ResBox.ShowWindow(SW_SHOWNORMAL);
	m_ResBox.SetActiveWindow();
	m_ResBox.BringWindowToTop();
}

void CGUIToolPal::OnBnClickedViewReset()
{
	GUIWORKINGRES res;
	res.iSizeX = m_iResX;
	res.iSizeY = m_iResY;
	EngineGetToolBox()->SendMessage(_T("SetGUIWorkingResolution"), sizeof(GUIWORKINGRES), &res);	
}

void CGUIToolPal::LeftMousePressed()
{
	if (m_iStage == 1) // set pos stage
	{
		DWORD retval;
		m_iStage = 2;
		MOUSECOORDS mouse;
		m_ToolBox->SendMessage(_T("GetGUIMousePosition"), sizeof(MOUSECOORDS), &mouse, NULL, NULL);

		POSITIONDATA pd;
		pd.m_fXPos = (float)mouse.m_lXPos;
		pd.m_fYPos = (float)mouse.m_lYPos;
		m_fElementPosX = pd.m_fXPos;
		m_fElementPosY = pd.m_fYPos;
		retval = m_ToolBox->SendMessage(_T("SetPosition"), sizeof(POSITIONDATA), &pd, &m_szAddItemName, &m_szAddItemType);		
	}

	// check for selection, but only if we're not moving the screen or zooming
	else if ((!m_bSpacePressed) && (!m_bCtrlPressed) && (!m_bShiftPressed)) 
	{
		// see if we have an object selected already first...
		DWORD retval;
		CHashString rectName = _T("SelectRect");
		CHashString rectType = _T("CGUIRect");
		retval = m_ToolBox->SendMessage(_T("GetHandleIndex"), sizeof(int), &m_iHandleIndex, &rectName, &rectType);
		if (m_iHandleIndex > -1)
		{
			m_iStage = 4 + m_iHandleIndex;
			return;
		}

		// if not, see if we can select one
		MOUSECOORDS mouse;
		m_ToolBox->SendMessage(_T("GetGUIMousePosition"), sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		OBJUNDERCURSOR ouc;
		CHashString pageName = m_szCurrentPage;
		CHashString pageType = _T("CGUIPage");
		ouc.m_fPosX = (float)mouse.m_lXPos;
		ouc.m_fPosY = (float)mouse.m_lYPos;
		ouc.m_Name = NULL;
		ouc.m_CompType = NULL;
		retval = m_ToolBox->SendMessage(_T("GetObjUnderCursor"), sizeof(OBJUNDERCURSOR), &ouc, &pageName, &pageType);		
		if (ouc.m_Name)
		{
			if ((ouc.m_Name->GetUniqueID() != CHashString(_T("BackgroundColor")).GetUniqueID()) &&
				(ouc.m_Name->GetUniqueID() != CHashString(_T("SelectRect")).GetUniqueID()))
			{
				POSITIONDATA pd;
				retval = m_ToolBox->SendMessage(_T("GetPosition"), sizeof(POSITIONDATA), &pd, ouc.m_Name, ouc.m_CompType);
				SIZEDATA sd;
				retval = m_ToolBox->SendMessage(_T("GetSize"), sizeof(SIZEDATA), &sd, ouc.m_Name, ouc.m_CompType);
				CHashString rectName = _T("SelectRect");
				CHashString rectType = _T("CGUIRect");
				SELECTRECT sr;
				sr.x1 = pd.m_fXPos;
				sr.x2 = pd.m_fXPos + sd.m_fXSize;
				sr.y1 = pd.m_fYPos;
				sr.y2 = pd.m_fYPos + sd.m_fYSize;
				retval = m_ToolBox->SendMessage(_T("SetRect"), sizeof(SELECTRECT), &sr, &rectName, &rectType);
				if (m_szSelectedItemName.GetUniqueID() != ouc.m_Name->GetUniqueID())
				{
					m_pDockRef->RestoreOptionTree(ouc.m_Name, ouc.m_CompType);
					m_szSelectedItemName = *dynamic_cast<CHashString*>(ouc.m_Name);
					m_szSelectedItemType = *dynamic_cast<CHashString*>(ouc.m_CompType);
					m_iStage = 3;
				}
			}
			else // didn't click an object other than the background
			{
				CHashString rectName = _T("SelectRect");
				CHashString rectType = _T("CGUIRect");
				SELECTRECT sr;
				sr.x1 = 0.f;
				sr.x2 = 0.f;
				sr.y1 = 0.f;
				sr.y2 = 0.f;
				m_ToolBox->SendMessage(_T("SetRect"), sizeof(SELECTRECT), &sr, &rectName, &rectType);
				m_pDockRef->ClearOptionTree();
				m_szSelectedItemName = _T("");
				m_iStage = 0;
			}
		}
		else // didn't click anything 
		{
			CHashString rectName = _T("SelectRect");
			CHashString rectType = _T("CGUIRect");
			SELECTRECT sr;
			sr.x1 = 0.f;
			sr.x2 = 0.f;
			sr.y1 = 0.f;
			sr.y2 = 0.f;
			m_ToolBox->SendMessage(_T("SetRect"), sizeof(SELECTRECT), &sr, &rectName, &rectType);
			m_pDockRef->ClearOptionTree();
			m_szSelectedItemName = _T("");
			m_iStage = 0;
		}
	}
}
	
void CGUIToolPal::LeftMouseReleased()
{
	if (m_iStage == 2) // setting size, holding button
	{
		m_iStage = 0;
		m_pDockRef->CreateNewOptionTree(&m_szAddItemName, &m_szAddItemType);
		m_szSelectedItemName = m_szAddItemName;
	}
	else if (m_iStage >= 4) 
	{
		m_iStage = 0;
		m_pDockRef->RestoreOptionTree(&m_szSelectedItemName, &m_szSelectedItemType);
	}
}

void CGUIToolPal::MouseMove()
{
	if (m_iStage == 2) // set size stage
	{
        DWORD retval;
		MOUSECOORDS mouse;
		m_ToolBox->SendMessage(_T("GetGUIMousePosition"), sizeof(MOUSECOORDS), &mouse, NULL, NULL);
        SIZEDATA sd;
		sd.m_fXSize = (float)(mouse.m_lXPos) - m_fElementPosX;
		sd.m_fYSize = (float)(mouse.m_lYPos) - m_fElementPosY;
		retval = m_ToolBox->SendMessage(_T("SetSize"), sizeof(SIZEDATA), &sd, &m_szAddItemName, &m_szAddItemType);
		
		CHashString rectName = _T("SelectRect");
		CHashString rectType = _T("CGUIRect");
		SELECTRECT rect;
		rect.x1 = m_fElementPosX;
		rect.x2 = (float)mouse.m_lXPos;
		rect.y1 = m_fElementPosY;
		rect.y2 = (float)mouse.m_lYPos;
		retval = m_ToolBox->SendMessage(_T("SetRect"), sizeof(SELECTRECT), &rect, &rectName, &rectType);
	}
	else if (m_iStage == 4) // center of object selected, changing position
	{
		DWORD retval;
		MOUSECOORDS mouse;
		m_ToolBox->SendMessage(_T("GetGUIMousePosition"), sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		int mouseDispX = mouse.m_lXPos - m_iLastMousePosX;
		int mouseDispY = mouse.m_lYPos - m_iLastMousePosY;
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
        POSITIONDATA pd;
		retval = m_ToolBox->SendMessage(_T("GetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);
		pd.m_fXPos += (float)mouseDispX;
		pd.m_fYPos += (float)mouseDispY;
		retval = m_ToolBox->SendMessage(_T("SetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);
		FINDOBJNAME fon;
		fon.searchName = &m_szSelectedItemName;
		fon.compType = &m_szSelectedItemType;
		retval = m_ToolBox->SendMessage(_T("FollowObject"), sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	}
	else if (m_iStage == 5) // top left handle
	{
		DWORD retval;
		MOUSECOORDS mouse;
		m_ToolBox->SendMessage(_T("GetGUIMousePosition"), sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		int mouseDispX = mouse.m_lXPos - m_iLastMousePosX;
		int mouseDispY = mouse.m_lYPos - m_iLastMousePosY;
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
        POSITIONDATA pd;
		retval = m_ToolBox->SendMessage(_T("GetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);
		pd.m_fXPos += (float)mouseDispX;
		pd.m_fYPos += (float)mouseDispY;
		retval = m_ToolBox->SendMessage(_T("SetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);
		SIZEDATA sd;
		retval = m_ToolBox->SendMessage(_T("GetSize"), sizeof(SIZEDATA), &sd, &m_szSelectedItemName, &m_szSelectedItemType);		
		sd.m_fXSize -= (float)mouseDispX;
		sd.m_fYSize -= (float)mouseDispY;
		retval = m_ToolBox->SendMessage(_T("SetSize"), sizeof(SIZEDATA), &sd, &m_szSelectedItemName, &m_szSelectedItemType);		
		FINDOBJNAME fon;
		fon.searchName = &m_szSelectedItemName;
		fon.compType = &m_szSelectedItemType;
		retval = m_ToolBox->SendMessage(_T("FollowObject"), sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	}
	else if (m_iStage == 6) // top middle handle
	{
		DWORD retval;
		MOUSECOORDS mouse;
		m_ToolBox->SendMessage(_T("GetGUIMousePosition"), sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		int mouseDispX = mouse.m_lXPos - m_iLastMousePosX;
		int mouseDispY = mouse.m_lYPos - m_iLastMousePosY;
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
        POSITIONDATA pd;
		retval = m_ToolBox->SendMessage(_T("GetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);
		pd.m_fYPos += (float)mouseDispY;
		retval = m_ToolBox->SendMessage(_T("SetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);
		SIZEDATA sd;
		retval = m_ToolBox->SendMessage(_T("GetSize"), sizeof(SIZEDATA), &sd, &m_szSelectedItemName, &m_szSelectedItemType);		
		sd.m_fYSize -= (float)mouseDispY;
		retval = m_ToolBox->SendMessage(_T("SetSize"), sizeof(SIZEDATA), &sd, &m_szSelectedItemName, &m_szSelectedItemType);		
		FINDOBJNAME fon;
		fon.searchName = &m_szSelectedItemName;
		fon.compType = &m_szSelectedItemType;
		retval = m_ToolBox->SendMessage(_T("FollowObject"), sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	}
	else if (m_iStage == 7) // top right handle
	{
		DWORD retval;
		MOUSECOORDS mouse;
		m_ToolBox->SendMessage(_T("GetGUIMousePosition"), sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		int mouseDispX = mouse.m_lXPos - m_iLastMousePosX;
		int mouseDispY = mouse.m_lYPos - m_iLastMousePosY;
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
        POSITIONDATA pd;
		retval = m_ToolBox->SendMessage(_T("GetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);
		pd.m_fYPos += (float)mouseDispY;
		retval = m_ToolBox->SendMessage(_T("SetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);
		SIZEDATA sd;
		retval = m_ToolBox->SendMessage(_T("GetSize"), sizeof(SIZEDATA), &sd, &m_szSelectedItemName, &m_szSelectedItemType);		
		sd.m_fXSize += (float)mouseDispX;
		sd.m_fYSize -= (float)mouseDispY;
		retval = m_ToolBox->SendMessage(_T("SetSize"), sizeof(SIZEDATA), &sd, &m_szSelectedItemName, &m_szSelectedItemType);		
		FINDOBJNAME fon;
		fon.searchName = &m_szSelectedItemName;
		fon.compType = &m_szSelectedItemType;
		retval = m_ToolBox->SendMessage(_T("FollowObject"), sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	}
	else if (m_iStage == 8) // middle right handle
	{
		DWORD retval;
		MOUSECOORDS mouse;
		m_ToolBox->SendMessage(_T("GetGUIMousePosition"), sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		int mouseDispX = mouse.m_lXPos - m_iLastMousePosX;
		int mouseDispY = mouse.m_lYPos - m_iLastMousePosY;
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
        SIZEDATA sd;
		retval = m_ToolBox->SendMessage(_T("GetSize"), sizeof(SIZEDATA), &sd, &m_szSelectedItemName, &m_szSelectedItemType);		
		sd.m_fXSize += (float)mouseDispX;		
		retval = m_ToolBox->SendMessage(_T("SetSize"), sizeof(SIZEDATA), &sd, &m_szSelectedItemName, &m_szSelectedItemType);		
		FINDOBJNAME fon;
		fon.searchName = &m_szSelectedItemName;
		fon.compType = &m_szSelectedItemType;
		retval = m_ToolBox->SendMessage(_T("FollowObject"), sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	}
	else if (m_iStage == 9) // bottom right handle
	{
		DWORD retval;
		MOUSECOORDS mouse;
		m_ToolBox->SendMessage(_T("GetGUIMousePosition"), sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		int mouseDispX = mouse.m_lXPos - m_iLastMousePosX;
		int mouseDispY = mouse.m_lYPos - m_iLastMousePosY;
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
        SIZEDATA sd;
		retval = m_ToolBox->SendMessage(_T("GetSize"), sizeof(SIZEDATA), &sd, &m_szSelectedItemName, &m_szSelectedItemType);		
		sd.m_fXSize += (float)mouseDispX;		
		sd.m_fYSize += (float)mouseDispY;
		retval = m_ToolBox->SendMessage(_T("SetSize"), sizeof(SIZEDATA), &sd, &m_szSelectedItemName, &m_szSelectedItemType);		
		FINDOBJNAME fon;
		fon.searchName = &m_szSelectedItemName;
		fon.compType = &m_szSelectedItemType;
		retval = m_ToolBox->SendMessage(_T("FollowObject"), sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	}
	else if (m_iStage == 10) // bottom middle handle
	{
		DWORD retval;
		MOUSECOORDS mouse;
		m_ToolBox->SendMessage(_T("GetGUIMousePosition"), sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		int mouseDispX = mouse.m_lXPos - m_iLastMousePosX;
		int mouseDispY = mouse.m_lYPos - m_iLastMousePosY;
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
        SIZEDATA sd;
		retval = m_ToolBox->SendMessage(_T("GetSize"), sizeof(SIZEDATA), &sd, &m_szSelectedItemName, &m_szSelectedItemType);		
		sd.m_fYSize += (float)mouseDispY;
		retval = m_ToolBox->SendMessage(_T("SetSize"), sizeof(SIZEDATA), &sd, &m_szSelectedItemName, &m_szSelectedItemType);		
		FINDOBJNAME fon;
		fon.searchName = &m_szSelectedItemName;
		fon.compType = &m_szSelectedItemType;
		retval = m_ToolBox->SendMessage(_T("FollowObject"), sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	}
	else if (m_iStage == 11) // bottom left handle
	{
		DWORD retval;
		MOUSECOORDS mouse;
		m_ToolBox->SendMessage(_T("GetGUIMousePosition"), sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		int mouseDispX = mouse.m_lXPos - m_iLastMousePosX;
		int mouseDispY = mouse.m_lYPos - m_iLastMousePosY;
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
        POSITIONDATA pd;
		retval = m_ToolBox->SendMessage(_T("GetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);
		pd.m_fXPos += (float)mouseDispX;
		retval = m_ToolBox->SendMessage(_T("SetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);
		SIZEDATA sd;
		retval = m_ToolBox->SendMessage(_T("GetSize"), sizeof(SIZEDATA), &sd, &m_szSelectedItemName, &m_szSelectedItemType);		
		sd.m_fXSize -= (float)mouseDispX;
		sd.m_fYSize += (float)mouseDispY;
		retval = m_ToolBox->SendMessage(_T("SetSize"), sizeof(SIZEDATA), &sd, &m_szSelectedItemName, &m_szSelectedItemType);		
		FINDOBJNAME fon;
		fon.searchName = &m_szSelectedItemName;
		fon.compType = &m_szSelectedItemType;
		retval = m_ToolBox->SendMessage(_T("FollowObject"), sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	}
	else if (m_iStage == 12) // middle left handle
	{
		DWORD retval;
		MOUSECOORDS mouse;
		m_ToolBox->SendMessage(_T("GetGUIMousePosition"), sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		int mouseDispX = mouse.m_lXPos - m_iLastMousePosX;
		int mouseDispY = mouse.m_lYPos - m_iLastMousePosY;
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
        POSITIONDATA pd;
		retval = m_ToolBox->SendMessage(_T("GetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);
		pd.m_fXPos += (float)mouseDispX;
		retval = m_ToolBox->SendMessage(_T("SetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);
		SIZEDATA sd;
		retval = m_ToolBox->SendMessage(_T("GetSize"), sizeof(SIZEDATA), &sd, &m_szSelectedItemName, &m_szSelectedItemType);		
		sd.m_fXSize -= (float)mouseDispX;
		retval = m_ToolBox->SendMessage(_T("SetSize"), sizeof(SIZEDATA), &sd, &m_szSelectedItemName, &m_szSelectedItemType);		
		FINDOBJNAME fon;
		fon.searchName = &m_szSelectedItemName;
		fon.compType = &m_szSelectedItemType;
		retval = m_ToolBox->SendMessage(_T("FollowObject"), sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	}
	else
	{
		MOUSECOORDS mouse;
		m_ToolBox->SendMessage(_T("GetGUIMousePosition"), sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
	}
}

void CGUIToolPal::OnBnClickedAddButton()
{
	if (m_iStage == 0) // not already adding an element
	{
		DWORD retval;
		m_iElementNum++;
		m_szAddItemType = _T("CGUIItem");
		
		StdString szTemp;
		_stprintf(szTemp, "Button %d", m_iElementNum);
		m_szAddItemName = szTemp;
		CHashString pageName = _T("Working Page");

		CREATEOBJECTPARAMS cop;
		INITOBJECTPARAMS iop;

		cop.parentName = &pageName;
		cop.typeName = &m_szAddItemType;
		cop.name = &m_szAddItemName;
		retval = m_ToolBox->SendMessage(_T("CreateObject"), sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);
		iop.name = &m_szAddItemName;
		if( retval == MSG_HANDLED )
		{
			retval = m_ToolBox->SendMessage(_T("InitObject"), sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);
		}

		GUIPAGEMESSAGE gmsgPage;
		gmsgPage.name = &m_szAddItemName;
		gmsgPage.compType = &m_szAddItemType;
		retval = m_ToolBox->SendMessage(_T("AddGUIElementToPage"), sizeof(GUIPAGEMESSAGE), &gmsgPage, &pageName, NULL);

		m_iStage = 1;
	}
}

void CGUIToolPal::OnBnClickedAddPage()
{
	m_PageNameBox.ShowWindow(SW_SHOWNORMAL);
	m_PageNameBox.SetActiveWindow();
	m_PageNameBox.BringWindowToTop();
}

void CGUIToolPal::AddPage(CHashString name)
{
	DWORD retval;

	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;

	CHashString hszParentName(_T("World"));
	CHashString hszTypeName(_T("CGUIPage"));
	cop.parentName = &hszParentName;
	cop.typeName = &hszTypeName;
	cop.name = &name;
	retval = m_ToolBox->SendMessage(_T("CreateObject"), sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);
	iop.name = &name;
	if( retval == MSG_HANDLED )
	{
		retval = m_ToolBox->SendMessage(_T("InitObject"), sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);
	}
	retval = EngineGetToolBox()->SendMessage(_T("GUIManagerSetActivePage"), sizeof(IHashString), &name, NULL, NULL);		
	m_PageNames.push_back(name);

	// init and add background
	cop.parentName = &name;
	cop.name = &m_szBGName;
	cop.typeName = &m_szBGType;
	retval = m_ToolBox->SendMessage(_T("CreateObject"), sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);
	iop.name = &m_szBGName;
	if( retval == MSG_HANDLED )
	{
		retval = m_ToolBox->SendMessage(_T("InitObject"), sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);
	}

	GUIPAGEMESSAGE gpm;
	gpm.name = &m_szBGName;
	gpm.compType = &m_szBGType;
	retval = m_ToolBox->SendMessage(_T("AddGUIElementToPage"), sizeof(GUIPAGEMESSAGE), &gpm, &name, NULL);			

	// init and add rect
	cop.name = &m_szRectName;
	cop.typeName = &m_szRectType;
	retval = m_ToolBox->SendMessage(_T("CreateObject"), sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);
	iop.name = &m_szRectName;
	if( retval == MSG_HANDLED )
	{
		retval = m_ToolBox->SendMessage(_T("InitObject"), sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);
	}

	gpm.name = &m_szRectName;
	gpm.compType = &m_szRectType;
	retval = m_ToolBox->SendMessage(_T("AddGUIElementToPage"), sizeof(GUIPAGEMESSAGE), &gpm, &name, NULL);			

	POSITIONDATA pd;
	pd.m_fXPos = 0.f;
	pd.m_fYPos = 0.f;
	retval = m_ToolBox->SendMessage(_T("SetPosition"), sizeof(POSITIONDATA), &pd, &m_szBGName, &m_szBGType);

	SIZEDATA sd;
	sd.m_fXSize = (float)m_iResX;
	sd.m_fYSize = (float)m_iResY;
	retval = m_ToolBox->SendMessage(_T("SetSize"), sizeof(SIZEDATA), &sd, &m_szBGName, &m_szBGType);

	OnBnClickedViewReset();

	CWnd *elemBtn = GetDlgItem(IDC_ADDELEMENT);
	if (elemBtn)
	{
		elemBtn->EnableWindow(true);
	}

	m_AddElemBox.AddPageName(name);
	m_szCurrentPage = name;

	bool activeMode = false;
	retval = EngineGetToolBox()->SendMessage(_T("SetActiveMode"), sizeof(bool), &activeMode, NULL, NULL);
}

void CGUIToolPal::OnBnClickedAddElement()
{
	m_AddElemBox.ShowWindow(SW_SHOWNORMAL);
	m_AddElemBox.SetActiveWindow();
	m_AddElemBox.BringWindowToTop();
	m_AddElemBox.LoadTypes();
	m_AddElemBox.SetCurrentPage(m_szCurrentPage);
}

void CGUIToolPal::AddElemExternal(GUIPAGEMESSAGE gpm)
{
	m_szAddItemName = gpm.name;
	m_szAddItemType = gpm.compType;
	m_ObjectNames.push_back(m_szAddItemName);
	m_ObjectTypes.push_back(m_szAddItemType);
	m_iStage = 1; // set pos!
}

void CGUIToolPal::KeyPressed(char keyDown)
{
	if (keyDown == VK_SPACE)
	{
		m_bSpacePressed = true;
	}
	else if (keyDown == VK_CONTROL)
	{
		m_bCtrlPressed = true;
	}
	else if (keyDown == VK_SHIFT)
	{
		m_bShiftPressed = true;
	}
	else if ((keyDown == VK_UP) && (!m_bUpPressed))
	{
		m_bUpPressed = true;
		if (_tcscmp(m_szSelectedItemName.GetString(), "")!=0)
		{
			POSITIONDATA pd;
			m_ToolBox->SendMessage(_T("GetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);
			pd.m_fYPos -= 1.f;
			m_ToolBox->SendMessage(_T("SetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);			
			FINDOBJNAME fon;
			fon.searchName = &m_szSelectedItemName;
			fon.compType = &m_szSelectedItemType;
			m_ToolBox->SendMessage(_T("FollowObject"), sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
		}
	}
	else if ((keyDown == VK_DOWN) && (!m_bDownPressed))
	{
		m_bDownPressed = true;
		if (_tcscmp(m_szSelectedItemName.GetString(), "")!=0)
		{
			POSITIONDATA pd;
			m_ToolBox->SendMessage(_T("GetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);
			pd.m_fYPos += 1.f;
			m_ToolBox->SendMessage(_T("SetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);			
			FINDOBJNAME fon;
			fon.searchName = &m_szSelectedItemName;
			fon.compType = &m_szSelectedItemType;
			m_ToolBox->SendMessage(_T("FollowObject"), sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
		}
	}
	else if ((keyDown == VK_LEFT) && (!m_bLeftPressed))
	{
		m_bLeftPressed = true;
		if (_tcscmp(m_szSelectedItemName.GetString(), "")!=0)
		{
			POSITIONDATA pd;
			m_ToolBox->SendMessage(_T("GetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);
			pd.m_fXPos -= 1.f;
			m_ToolBox->SendMessage(_T("SetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);	
			FINDOBJNAME fon;
			fon.searchName = &m_szSelectedItemName;
			fon.compType = &m_szSelectedItemType;
			m_ToolBox->SendMessage(_T("FollowObject"), sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
		}
	}
	else if ((keyDown == VK_RIGHT) && (!m_bRightPressed))
	{
		m_bRightPressed = true;
		if (_tcscmp(m_szSelectedItemName.GetString(), "")!=0)
		{
			POSITIONDATA pd;
			m_ToolBox->SendMessage(_T("GetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);
			pd.m_fXPos += 1.f;
			m_ToolBox->SendMessage(_T("SetPosition"), sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);		
			FINDOBJNAME fon;
			fon.searchName = &m_szSelectedItemName;
			fon.compType = &m_szSelectedItemType;
			m_ToolBox->SendMessage(_T("FollowObject"), sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
		}
	}
}

void CGUIToolPal::KeyReleased(char keyDown)
{
	if (keyDown == VK_SPACE)
	{
		m_bSpacePressed = false;
	}
	else if (keyDown == VK_CONTROL)
	{
		m_bCtrlPressed = false;
	}
	else if (keyDown == VK_SHIFT)
	{
		m_bShiftPressed = false;
	}
	else if (keyDown == VK_UP)
	{
		m_bUpPressed = false;
	}
	else if (keyDown == VK_DOWN)
	{
		m_bDownPressed = false;
	}
	else if (keyDown == VK_LEFT)
	{
		m_bLeftPressed = false;
	}
	else if (keyDown == VK_RIGHT)
	{
		m_bRightPressed = false;
	}
}