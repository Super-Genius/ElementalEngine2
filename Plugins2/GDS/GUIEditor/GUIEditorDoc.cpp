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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_UNDO_STACK_SIZE 10

IMPLEMENT_DYNCREATE(CGUIEditorDoc, CDocument)

CGUIEditorDoc::CGUIEditorDoc()
{
	m_szSelectedPage = _T("");
	m_szSelectedGroup = _T("");
	m_szSelectedItemName = _T("");
	m_szSelectedItemType = _T("");
	m_bSelectedIsPage = false;

	CMDIFrameWnd *mainWnd;
		
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, pApp->m_pMainWnd);

	m_PageName.Create(IDD_ENTERPAGENAME, mainWnd);
	m_GroupNameBox.Create(IDD_GROUPNAMEBOX, mainWnd);
	m_UseGroupBox.Create(IDD_USEGROUPBOX, mainWnd);
	m_NewButtonBox.Create(IDD_NEWBUTTON, mainWnd);
	m_NewTextBox.Create(IDD_NEWTEXT, mainWnd);
	m_NewStaticBox.Create(IDD_NEWSTATIC, mainWnd);
	m_UseItemBox.Create(IDD_USEITEM, mainWnd);
	m_SetBGColorBox.Create(IDD_SETBGCOLOR, mainWnd);
	m_NewAnimationBox.Create(IDD_NEWANIMATION, mainWnd);
	m_KeyDataView.Create(IDD_KEYFRAMEDATA, mainWnd);
	
	CString className = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW);
	RECT rect;
	rect.bottom = rect.left = rect.right = rect.top = 0;
    m_FullScreen.CreateEx(WS_EX_TOPMOST, className, _T("GUI Fullscreen"), WS_POPUP, 
		0, 0, 0, 0, NULL, NULL);

	m_PageName.SetDocParent(this);
	m_GroupNameBox.SetDocParent(this);
	m_UseGroupBox.SetDocParent(this);
	m_NewButtonBox.SetDocParent(this);
	m_NewTextBox.SetDocParent(this);
	m_NewStaticBox.SetDocParent(this);
	m_UseItemBox.SetDocParent(this);
	m_SetBGColorBox.SetDocParent(this);
	m_FullScreen.SetDocParent(this);
	m_NewAnimationBox.SetDocParent(this);
	m_KeyDataView.SetDocParent(this);

	m_ToolBox = EngineGetToolBox();
	
	m_FrmParent = NULL;	
	m_View = NULL;
	m_TabBar = NULL;

	m_bCursorCreated = false;
	m_szCursorName = _T("");

	m_iXRes = 0;
	m_iYRes = 0;

	m_iStackPos = 0;
	m_bRedo = false;

	m_CopyStruct.name = _T("");
	m_CopyStruct.type = _T("");	

	m_iSystemIndex = 0;
	m_hszSelectedAnimName = _T("");
}

BOOL CGUIEditorDoc::OnNewDocument()
{
	Init();

	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

BOOL CGUIEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	Init();
	return CDocument::OnOpenDocument(lpszPathName);
}

void CGUIEditorDoc::Init()
{
	m_vPages.clear();

	POSITION pos = GetFirstViewPosition();
	m_View = DYNAMIC_DOWNCAST(CGUIEditorView, GetNextView( pos ));
	assert(m_View);

	m_FrmParent = DYNAMIC_DOWNCAST(CGUIEditorFrm, m_View->GetParentFrame());
	assert(m_FrmParent);

	m_FrmParent->m_Tools.SetDocParent(this);
	m_FrmParent->m_OptionTree->SetTreeUpdateSerializeFuncCallback(UpdateTreeSerializeCallback);
	m_FrmParent->m_AnimView.SetDocParent(this);
	m_FrmParent->m_GUIDataDlg.SetDocParent(this);

	static DWORD msgHash_GUIEditorDocOpen = CHashString(_T("GUIEditorDocOpen")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GUIEditorDocOpen, 0, NULL);
}

CGUIEditorDoc::~CGUIEditorDoc()
{}


BEGIN_MESSAGE_MAP(CGUIEditorDoc, CDocument)
	ON_BN_CLICKED(IDC_NEWPAGEBT, NewPagePressed)
	ON_BN_CLICKED(IDC_NEWGROUPBT, NewGroupPressed)
	ON_BN_CLICKED(IDC_NEWBUTTONBT, NewButtonPressed)
	ON_BN_CLICKED(IDC_NEWTEXTBT, NewTextPressed)
	ON_BN_CLICKED(IDC_NEWSTATICBT, NewStaticPressed)
	ON_BN_CLICKED(IDC_USEITEMBT, UseItemPressed)	
	ON_BN_CLICKED(IDC_CHANGECOLORBT, ChangeBGColorPressed)	
	ON_BN_CLICKED(IDC_FULLWINDOWBT, FullWindowPagePressed)	
	ON_BN_CLICKED(IDC_PLAYBT, PlayPressed)	
	ON_BN_CLICKED(IDC_INSTLOCK, InstLockPressed)	
	ON_BN_CLICKED(IDC_NEWANIMATION, NewAnimationPressed)	
	ON_BN_CLICKED(IDC_PLAYANIM, PlayAnimationPressed)	
	ON_BN_CLICKED(IDC_STOPANIM, StopAnimationPressed)	
END_MESSAGE_MAP()


#ifdef _DEBUG
void CGUIEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGUIEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// load an GUIEditor document
DWORD CGUIEditorDoc::NewGUISystem(UINT xRes, UINT yRes)
{
	if (m_View)
	{
		m_View->SetResolution(xRes, yRes);
	}

	m_iXRes = xRes;
	m_iYRes = yRes;

	if (m_TabBar)
	{
		TOOLBARACTIVESTRUCT tbas;
		tbas.bActive = true;
		tbas.cBar = (CControlBar*)m_TabBar;
		static DWORD msgHash_SetActiveToolBar = CHashString(_T("SetActiveToolBar")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetActiveToolBar, sizeof(TOOLBARACTIVESTRUCT), &tbas);
	}	

	GUIWORKINGRES gwr;
	gwr.iSizeX = m_iXRes;
	gwr.iSizeY = m_iYRes;
	static DWORD msgHash_SetGUIWorkingRes = CHashString(_T("SetGUIWorkingRes")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetGUIWorkingRes, sizeof(GUIWORKINGRES), &gwr);

	m_FullScreen.SetResolution(xRes, yRes);

	// go ahead and add a page to start on
	CHashString firstPage(_T("Page 0"));
	AddPage(firstPage);	

	return NO_ERROR;
}

void CGUIEditorDoc::LoadGXL(GUIPAGELIST gpl, GUIWORKINGRES gwr)
{
	if (m_View)
	{
		m_View->SetResolution(gwr.iSizeX, gwr.iSizeY);
	}

	if (m_FrmParent)
	{
		m_FrmParent->m_GUIDataDlg.SetResolution(gwr.iSizeX, gwr.iSizeY);
	}

	DWORD retval;
	m_vPages.clear();
	m_mItems.clear();
	m_vGroups.clear();
	UINT i;	
	for (i=0; i<gpl.m_vPages.size(); i++)
	{
		m_vPages.push_back(CHashString(gpl.m_vPages[i]));
		GUIELEMLIST pGel;
		static DWORD msgHash_GetPageItemList = CHashString(_T("GetPageItemList")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetPageItemList, sizeof(GUIELEMLIST), &pGel, &CHashString(gpl.m_vPages[i]), &CHashString(_T("CGUIPage")));
		for (UINT j=0; j<pGel.m_vNames.size(); j++)
		{
			m_FrmParent->m_OptionTree->RegisterObjectCreated(CHashString((const TCHAR*)pGel.m_vNames[j]));
			if (_tcscmp(pGel.m_vTypes[j], _T("CGUIGroup")) == 0)
			{
				m_vGroups.push_back(CHashString(pGel.m_vNames[j]));
				GUIELEMLIST gGel;				
				static DWORD msgHash_GetGroupItemList = CHashString(_T("GetGroupItemList")).GetUniqueID();
				retval = m_ToolBox->SendMessage(msgHash_GetGroupItemList, sizeof(GUIELEMLIST), &gGel, &CHashString(pGel.m_vNames[j]), &CHashString(_T("CGUIGroup")));
				for (UINT k=0; k<gGel.m_vNames.size(); k++)
				{
					m_FrmParent->m_OptionTree->RegisterObjectCreated(CHashString((const TCHAR*)gGel.m_vNames[k]));
					// all objects should be instances... hopefully...
					CHashString instType = _T("CGUIInstance");
					CHashString refName = _T("");
					static DWORD msgHash_GetItemReference = CHashString(_T("GetItemReference")).GetUniqueID();
					retval = m_ToolBox->SendMessage(msgHash_GetItemReference, sizeof(IHashString), &refName, &CHashString(gGel.m_vNames[k]), &instType);
					if (_tcscmp(refName.GetString(), _T("")) != 0)
					{
						INSTDATA id;
						id.m_szName = CHashString((const TCHAR*)gGel.m_vNames[k]);
						id.m_szParentName = CHashString((const TCHAR*)pGel.m_vNames[j]);
						m_mItems[refName.GetUniqueID()].push_back(id);
						FINDGUIOBJECT fgo;
						fgo.name = &refName;
						static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
						retval = m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
						if (fgo.bFound)
						{
							m_UseItemBox.AddItemToList(refName, CHashString(fgo.type));
						}
					}
				}				
			}
			else
			{
				// all objects should be instances... hopefully...
				CHashString instType = _T("CGUIInstance");
				CHashString refName = _T("");
				static DWORD msgHash_GetItemReference = CHashString(_T("GetItemReference")).GetUniqueID();
				retval = m_ToolBox->SendMessage(msgHash_GetItemReference, sizeof(IHashString), &refName, &CHashString(pGel.m_vNames[j]), &instType);
				if (_tcscmp(refName.GetString(), _T("")) != 0)
				{
					INSTDATA id;
					id.m_szName = CHashString((const TCHAR*)pGel.m_vNames[j]);
					id.m_szParentName = CHashString(gpl.m_vPages[i]);
					m_mItems[refName.GetUniqueID()].push_back(id);
					FINDGUIOBJECT fgo;
					fgo.name = &refName;
					static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
					retval = m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
					if (fgo.bFound)
					{
						m_UseItemBox.AddItemToList(refName, CHashString(fgo.type));
					}
				}
			}
		}
	}
	
	m_FrmParent->m_Tools.LoadGXL(gpl);
	m_FrmParent->m_AnimView.LoadAnimations(gpl.m_vAnimations);

	if (_tcscmp(gpl.m_szCursorName, _T("")) != 0)
	{
		m_FrmParent->m_OptionTree->RegisterObjectCreated(CHashString((const TCHAR*)gpl.m_szCursorName));
	}

	for (i=0; i<gpl.m_vAnimations.size(); i++)
	{
		m_FrmParent->m_OptionTree->RegisterObjectCreated(CHashString(gpl.m_vAnimations[i]));
		m_vAnims.push_back(CHashString(gpl.m_vAnimations[i]));
	}

	RECT rect;
	m_View->GetClientRect(&rect);
	
	VIEWPORTPARAMS gwrp;
	gwrp.offsetx = 0;
	gwrp.offsety = 0;
	gwrp.width = rect.right;
	gwrp.height = rect.bottom;
	static DWORD msgHash_SetWindowSize = CHashString(_T("SetWindowSize")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetWindowSize, sizeof(VIEWPORTPARAMS), &gwrp, NULL, NULL);

	float zf;
	zf = (float)rect.right / (float)gwr.iSizeX; 
	if (gwr.iSizeY * zf > rect.bottom)
	{
		zf = (float)rect.bottom / (float)gwr.iSizeY;
	}
	static DWORD msgHash_SetZoomFactor = CHashString(_T("SetZoomFactor")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetZoomFactor, sizeof(float), &zf);

	if (m_FrmParent)
	{
		m_FrmParent->m_GUIDataDlg.SetZoom(zf);
	}

	int poX, poY;
	poX = (rect.right / 2) - (int)(((float)gwr.iSizeX / 2.f) * zf);
	poY = (rect.bottom / 2) - (int)(((float)gwr.iSizeY / 2.f) * zf);
	POSITIONDATA pd;
	pd.m_fXPos = (float)poX;
	pd.m_fYPos = (float)poY;
	static DWORD msgHash_GUIManagerSetPageOffset = CHashString(_T("GUIManagerSetPageOffset")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GUIManagerSetPageOffset, sizeof(POSITIONDATA), &pd);
	m_View->SetPageOffset(poX, poY);

	CHashString pName( gpl.m_vPages[0] );
	static DWORD msgHash_GUIManagerSetActivePage = CHashString(_T("GUIManagerSetActivePage")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GUIManagerSetActivePage, sizeof(IHashString), &pName);
	m_View->SetSelectedPage(pName);

	m_iXRes = gwr.iSizeX;
	m_iYRes = gwr.iSizeY;

	m_FullScreen.SetResolution(m_iXRes, m_iYRes);

	if (_tcscmp(gpl.m_szCursorName, _T("")) != 0)
	{
		m_FrmParent->m_Tools.AddCursor(CHashString(gpl.m_szCursorName));
		m_bCursorCreated = true;	

		CURSORVISIBLE cv;
		cv.bVisible = false;
		static DWORD msgHash_SetCursorVisible = CHashString(_T("SetCursorVisible")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetCursorVisible, sizeof(CURSORVISIBLE), &cv, &CHashString(gpl.m_szCursorName), &CHashString(_T("CGUICursor")));

		m_szCursorName = gpl.m_szCursorName;
	}

	if (m_TabBar)
	{
		TOOLBARACTIVESTRUCT tbas;
		tbas.bActive = true;
		tbas.cBar = (CControlBar*)m_TabBar;
		static DWORD msgHash_SetActiveToolBar = CHashString(_T("SetActiveToolBar")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetActiveToolBar, sizeof(TOOLBARACTIVESTRUCT), &tbas);
	}		
}

BOOL CGUIEditorDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	TCHAR buf[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, buf);

	DWORD retVal;
	CString errMess;
	StdString oldPathName = GetPathName();
	TCHAR filePathName[_MAX_PATH];
	
	_tcscpy(filePathName, oldPathName.c_str());

	OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
    ofn.lpstrFilter = "GUI File (*.gxl)\0*.gxl\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = filePathName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
    ofn.lpstrDefExt = "gxl";

    if(!GetSaveFileName(&ofn))
    {
        return FALSE;
    }

	CHashString pType = _T("CGUIPage");
	CHashString oName, oType;

	// take the guieditor widgets out of the pages
	for (UINT i=0; i<m_vPages.size(); i++)
	{
		RemoveWidgets(m_vPages[i]);
	}

	// now actually save the file by extension
	static DWORD msgHash_SaveFileByExtension = CHashString(_T("SaveFileByExtension")).GetUniqueID();
	retVal =  EngineGetToolBox()->SendMessage(msgHash_SaveFileByExtension, sizeof(LPTSTR), filePathName );
	if (retVal != MSG_HANDLED)
	{
		errMess.Format(_T("Unable to save file %s\nError %d\n"), filePathName, GetLastError());
		MessageBox(ofn.hwndOwner, errMess, _T("Error Saving File"), MB_OK);
		return FALSE;
	}

	// Set the PathName to the new File Name
	SetPathName(filePathName);

	// put the editor widgets back
	for (UINT i=0; i<m_vPages.size(); i++)
	{
		AddWidgets(m_vPages[i]);
	}

	SetCurrentDirectory(buf);
	return TRUE;
}

void CGUIEditorDoc::NewPagePressed()
{
	m_PageName.ShowWindow(SW_SHOWNORMAL);
	m_PageName.SetActiveWindow();
	m_PageName.BringWindowToTop();	

	StdString pName = _T("Page ");
	TCHAR buf[4];
	_stprintf(buf, "%d", m_vPages.size());
	pName += buf;

	m_PageName.SetPageName(pName);
}

void CGUIEditorDoc::NewGroupPressed()
{
	if (_tcscmp(m_szSelectedPage.GetString(), _T("")) == 0)
	{
		MessageBox(NULL, _T("No page to add group to!"), _T("NOPE"), MB_OK);
	}
	else
	{
		m_GroupNameBox.ShowWindow(SW_SHOWNORMAL);
		m_GroupNameBox.SetActiveWindow();
		m_GroupNameBox.BringWindowToTop();	

		StdString gName = _T("Group ");
		TCHAR buf[4];
		_stprintf(buf, "%d", m_vGroups.size());
		gName += buf;

		m_GroupNameBox.SetGroupName(gName);
	}
}

void CGUIEditorDoc::UseGroupPressed()
{
}

void CGUIEditorDoc::NewButtonPressed()
{
	if (_tcscmp(m_szSelectedPage.GetString(), _T("")) == 0)
	{
		MessageBox(NULL, _T("No page to add button element to!"), _T("NOPE"), MB_OK);
	}
	else
	{
		m_NewButtonBox.ShowWindow(SW_SHOWNORMAL);
		m_NewButtonBox.SetActiveWindow();
		m_NewButtonBox.BringWindowToTop();
		StdString bname = _T("Button ");
		TCHAR buf[4];
		_stprintf(buf, "%d", m_mItems.size());
		bname += buf;
		m_NewButtonBox.SetButtonName(bname);
	}
}

void CGUIEditorDoc::NewTextPressed()
{
	if (_tcscmp(m_szSelectedPage.GetString(), _T("")) == 0)
	{
		MessageBox(NULL, _T("No page to add text element to!"), _T("NOPE"), MB_OK);
	}
	else
	{
		m_NewTextBox.ShowWindow(SW_SHOWNORMAL);
		m_NewTextBox.SetActiveWindow();
		m_NewTextBox.BringWindowToTop();
		StdString tName = _T("Text ");
		TCHAR buf[4];
		_stprintf(buf, "%d", m_mItems.size());
		tName += buf;
		m_NewTextBox.SetTextName(tName);	
	}
}

void CGUIEditorDoc::NewStaticPressed()
{
	if (_tcscmp(m_szSelectedPage.GetString(), _T("")) == 0)
	{
		MessageBox(NULL, _T("No page to add static element to!"), _T("NOPE"), MB_OK);
	}
	else
	{
		m_NewStaticBox.ShowWindow(SW_SHOWNORMAL);
		m_NewStaticBox.SetActiveWindow();
		m_NewStaticBox.BringWindowToTop();
		StdString sname = _T("Static ");
		TCHAR buf[4];
		_stprintf(buf, "%d", m_mItems.size());
		sname += buf;
		m_NewStaticBox.SetStaticName(sname);		
	}
}

void CGUIEditorDoc::UseItemPressed()
{
	if (_tcscmp(m_szSelectedPage.GetString(), _T("")) == 0)
	{
		MessageBox(NULL, _T("No page to add reference element to!"), _T("NOPE"), MB_OK);
	}
	else if (m_mItems.size() > 0)
	{
		m_UseItemBox.ShowWindow(SW_SHOWNORMAL);
		m_UseItemBox.SetActiveWindow();
		m_UseItemBox.BringWindowToTop();
	}
	else
	{
		MessageBox(NULL, _T("No items added yet!"), _T("NOPE"), MB_OK);
	}
}

void CGUIEditorDoc::ChangeBGColorPressed()
{
	m_SetBGColorBox.ShowWindow(SW_SHOWNORMAL);
	m_SetBGColorBox.SetActiveWindow();
	m_SetBGColorBox.BringWindowToTop();
}

void CGUIEditorDoc::FullWindowPagePressed()
{
	RECT rect;
	m_View->GetClientRect(&rect);

	float zf;
	zf = (float)rect.right / (float)m_iXRes; 
	if (m_iYRes * zf > rect.bottom)
	{
		zf = (float)rect.bottom / (float)m_iYRes;
	}
	static DWORD msgHash_SetZoomFactor = CHashString(_T("SetZoomFactor")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetZoomFactor, sizeof(float), &zf);

	if (m_FrmParent)
	{
		m_FrmParent->m_GUIDataDlg.SetZoom(zf);
	}

	int poX, poY;
	poX = (rect.right / 2) - (int)(((float)m_iXRes / 2.f) * zf);
	poY = (rect.bottom / 2) - (int)(((float)m_iYRes / 2.f) * zf);
	POSITIONDATA pd;
	pd.m_fXPos = (float)poX;
	pd.m_fYPos = (float)poY;
	static DWORD msgHash_GUIManagerSetPageOffset = CHashString(_T("GUIManagerSetPageOffset")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GUIManagerSetPageOffset, sizeof(POSITIONDATA), &pd);
	m_View->SetPageOffset(poX, poY);
	m_View->SetZoom(zf);
}

void CGUIEditorDoc::PlayPressed()
{
	if (m_bCursorCreated)
	{
		CURSORVISIBLE cv;
		cv.bVisible = true;
		static DWORD msgHash_SetCursorVisible = CHashString(_T("SetCursorVisible")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetCursorVisible, sizeof(CURSORVISIBLE), &cv, &m_szCursorName, &CHashString(_T("CGUICursor")));
	}
	m_View->SetFullscreenMode(true);
	bool active = true;
	static DWORD msgHash_SetActiveMode = CHashString(_T("SetActiveMode")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetActiveMode, sizeof(bool), &active);
	m_FullScreen.ShowWindow(SW_SHOWNORMAL);
	m_FullScreen.SetActiveWindow();
	m_FullScreen.BringWindowToTop();
	RemoveWidgets(m_szSelectedPage);
}

void CGUIEditorDoc::ResolutionUpdated(StdString szResolution)
{
	if (m_FrmParent)
	{
		// update ini
		WriteINIKey(_T("LastUsedResolution"), szResolution, _T("GUIEditor"));        

		StdString x, y;
		szResolution.GetToken(_T("x"), x);
		szResolution.GetToken(_T("x"), y);

		m_iXRes = (UINT)atoi(x.c_str());
		m_iYRes = (UINT)atoi(y.c_str());

		SIZEDATA sd;
		sd.m_fXSize = (float)m_iXRes;
		sd.m_fYSize = (float)m_iYRes;
		static DWORD msgHash_SetSize = CHashString(_T("SetSize")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetSize, sizeof(SIZEDATA), &sd, &CHashString(_T("WorkingCanvas")), &CHashString(_T("CGUIBackground")));	

		GUIWORKINGRES gwr;
		gwr.iSizeX = m_iXRes;
		gwr.iSizeY = m_iYRes;
		static DWORD msgHash_SetGUIWorkingRes = CHashString(_T("SetGUIWorkingRes")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetGUIWorkingRes, sizeof(GUIWORKINGRES), &gwr);

		m_FullScreen.SetResolution(m_iXRes, m_iYRes);
		m_View->SetResolution(m_iXRes, m_iYRes);
	}
}

bool CGUIEditorDoc::CheckNameIsRegistered(CHashString name)
{
	UINT i;
	for (i=0; i<m_vPages.size(); i++)
	{
		if (_tcscmp(m_vPages[i].GetString(), name.GetString()) == 0)
		{
			return true;
		}
	}
	for (i=0; i<m_vGroups.size(); i++)
	{
		if (_tcscmp(m_vGroups[i].GetString(), name.GetString()) == 0)
		{
			return true;
		}
	}
	ITEMMAP::iterator itemIter = m_mItems.find(name.GetUniqueID());
	if (itemIter != m_mItems.end())
	{
		return true; // name is template name
	}
	itemIter = m_mItems.begin();
	for (; itemIter != m_mItems.end(); itemIter++)
	{
		for (i=0; i<itemIter->second.size(); i++)
		{
			if (_tcscmp(itemIter->second[i].m_szName.GetString(), name.GetString())==0)
			{
				return true;
			}
		}
	}		
	for (i=0; i<m_vAnims.size(); i++)
	{
		if (_tcscmp(m_vAnims[i].GetString(), name.GetString()) == 0)
		{
			return true;
		}
	}
	return false;
}

bool CGUIEditorDoc::CheckIsGroupInPage(CHashString gName)
{
	GUIELEMLIST gel;
static DWORD msgHash_GetPageItemList = CHashString(_T("GetPageItemList")).GetUniqueID();
    m_ToolBox->SendMessage(msgHash_GetPageItemList, sizeof(GUIELEMLIST), &gel, &m_szSelectedPage, &CHashString(_T("CGUIPage")));
	for (UINT i=0; i<gel.m_vNames.size(); i++)
	{
		if (_tcscmp(gel.m_vNames[i], gName.GetString()) == 0)
		{
			return true;
		}
	}
	return false;
}

bool CGUIEditorDoc::CheckIsItemInGroupOrPage(CHashString name)
{
	if (m_bSelectedIsPage)
	{
		GUIELEMLIST gel;
		static DWORD msgHash_GetPageItemList = CHashString(_T("GetPageItemList")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetPageItemList, sizeof(GUIELEMLIST), &gel, &m_szSelectedPage, &CHashString(_T("CGUIPage")));
		for (UINT i=0; i<gel.m_vNames.size(); i++)
		{
			if (_tcscmp(gel.m_vNames[i], name.GetString()) == 0)
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		GUIELEMLIST gel;
		static DWORD msgHash_GetPageItemList = CHashString(_T("GetPageItemList")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetPageItemList, sizeof(GUIELEMLIST), &gel, &m_szSelectedGroup, &CHashString(_T("CGUIGroup")));
		for (UINT i=0; i<gel.m_vNames.size(); i++)
		{
			if (_tcscmp(gel.m_vNames[i], name.GetString()) == 0)
			{
				return true;
			}
		}
		return false;
	}
}

void CGUIEditorDoc::AddPage(CHashString pName)
{
	DWORD retval;
	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;

	// create the page
	CHashString type = _T("CGUIPage");
	CHashString parName = _T("World");

	cop.name = &pName;
	cop.typeName = &type;
	cop.parentName = &parName;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
	if (retval = MSG_HANDLED)
	{
		iop.name = &pName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
	}

	CHashString cName = _T("WorkingCanvas");
	CHashString cType = _T("CGUIBackground");
	CHashString rName = _T("SelectRect");
	CHashString rType = _T("CGUIRect");
	
	SELECTRECT sr;
	sr.x1 = 0.f;
	sr.x2 = 0.f;
	sr.y1 = 0.f;
	sr.y2 = 0.f;
	static DWORD msgHash_SetRect = CHashString(_T("SetRect")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_SetRect, sizeof(SELECTRECT), &sr, &rName, &rType);

	GUIPAGEMESSAGE gpm;
	gpm.compType = &cType;
	gpm.name = &cName;
	static DWORD msgHash_AddGUIElementToPage = CHashString(_T("AddGUIElementToPage")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_AddGUIElementToPage, sizeof(GUIPAGEMESSAGE), &gpm, &pName, &type);

	gpm.compType = &rType;
	gpm.name = &rName;
	retval = m_ToolBox->SendMessage(msgHash_AddGUIElementToPage, sizeof(GUIPAGEMESSAGE), &gpm, &pName, &type);

	static DWORD msgHash_GUIManagerSetActivePage = CHashString(_T("GUIManagerSetActivePage")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_GUIManagerSetActivePage, sizeof(IHashString), &pName);

    m_szSelectedPage = pName;
	m_bSelectedIsPage = true;

	m_vPages.push_back(pName);	

	if (m_FrmParent)
	{
		m_FrmParent->m_Tools.AddPage(pName);
	}
	m_View->SetSelectedPage(pName);

	FullWindowPagePressed(); // recenter page
}

void CGUIEditorDoc::AddGroup(CHashString gName, bool isReference)
{
	CHashString groupType = _T("CGUIGroup");
	CHashString pageType = _T("CGUIPage");
	DWORD retval;
    if (!isReference)
	{
		CREATEOBJECTPARAMS cop;
		INITOBJECTPARAMS iop;
		cop.name = &gName;
		cop.parentName = &m_szSelectedPage;
		cop.typeName = &groupType;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
		if (retval == MSG_HANDLED)
		{
			iop.name = &gName;
			static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
		}

		m_vGroups.push_back(gName);	
		m_UseGroupBox.AddGroupName(gName);
	}

	GUIPAGEMESSAGE gpm;
	gpm.compType = &groupType;
	gpm.name = &gName;
	static DWORD msgHash_AddGUIElementToPage = CHashString(_T("AddGUIElementToPage")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_AddGUIElementToPage, sizeof(GUIPAGEMESSAGE), &gpm, &m_szSelectedPage, &pageType);
	
	m_szSelectedGroup = gName;	
	m_bSelectedIsPage = false;

	if (m_FrmParent)
	{
		m_FrmParent->m_Tools.AddGroup(m_szSelectedPage, gName);
		m_FrmParent->m_OptionTree->UpdateOptionTree(&gName, &groupType);
	}
	m_View->SetSelectedGroup(gName);
}

CHashString CGUIEditorDoc::AddElement(CHashString eName, CHashString eType, bool isReference)
{
	CHashString refName;
	CHashString refType = _T("CGUIInstance");
	DWORD retval;
	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	
	StdString buf = eName.GetString();
	buf += "_orig";
	CHashString objName = buf;

	if (!isReference)
	{		
		cop.name = &objName;
		cop.typeName = &eType;
		if (m_bSelectedIsPage)
		{
			cop.parentName = &m_szSelectedPage;
		}
		else
		{
			cop.parentName = &m_szSelectedGroup;
		}
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
		if (retval == MSG_HANDLED)
		{
			iop.name = &objName;
			static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
		}
		m_UseItemBox.AddItemToList(eName, eType);			
	}

	StdString refNameBuf = eName.GetString();
	refNameBuf += _T("_");	
	TCHAR numBuf[10];
	
	if (m_mItems[objName.GetUniqueID()].size() > 0)
	{
		// if the first item hasn't been renamed yet...
		if (_tcscmp(m_mItems[objName.GetUniqueID()][0].m_szName.GetString(), eName.GetString()) == 0)
		{
			bool renameCopyItem = false;
			if (m_CopyStruct.name.GetUniqueID() == eName.GetUniqueID())
			{
				renameCopyItem = true;
			}
			StdString tempName = eName.GetString();
			tempName += _T("_");
			tempName += m_mItems[objName.GetUniqueID()][0].m_szParentName.GetString();
			bool success = RenameItem(eName, CHashString((const TCHAR*) tempName), false);
			assert(success);
			m_FrmParent->m_Tools.RenameExternal(eName, CHashString((const TCHAR*) tempName));
			if (renameCopyItem == true)
			{
				m_CopyStruct.name.Init(tempName);
			}
		}
		GUIELEMLIST gel;
		if (m_bSelectedIsPage)
		{
			refNameBuf += m_szSelectedPage.GetString();
			static DWORD msgHash_GetPageItemList = CHashString(_T("GetPageItemList")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_GetPageItemList, sizeof(GUIELEMLIST), &gel, &m_szSelectedPage, &CHashString(_T("CGUIPage")));
			refName = refNameBuf;
			for (UINT i=0; i<gel.m_vNames.size(); i++)
			{
				if (_tcscmp(gel.m_vNames[i], refNameBuf) == 0)
				{
					UINT refNum = 0;
					StdString tempBuf;
					do
					{
						refNum++;
						tempBuf = refNameBuf;
						_stprintf(numBuf, "_%d", refNum);
						tempBuf += numBuf;
						refName = tempBuf;						
					}while (CheckNameIsRegistered(refName));
					break;
				}
			}			
		}
		else
		{
			refNameBuf += m_szSelectedGroup.GetString();
			static DWORD msgHash_GetGroupItemList = CHashString(_T("GetGroupItemList")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_GetGroupItemList, sizeof(GUIELEMLIST), &gel, &m_szSelectedGroup, &CHashString(_T("CGUIGroup")));
			refName = refNameBuf;
			for (UINT i=0; i<gel.m_vNames.size(); i++)
			{
				if (_tcscmp(gel.m_vNames[i], refNameBuf) == 0)
				{
					UINT refNum = 0;
					StdString tempBuf;
					do
					{
						refNum++;
						tempBuf = refNameBuf;
						_stprintf(numBuf, "_%d", refNum);
						tempBuf += numBuf;
						refName = tempBuf;						
					}while (CheckNameIsRegistered(refName));
					break;
				}
			}			
		}
	}
	else
	{
		refName = eName;
	}

	cop.name = &refName;
	cop.typeName = &refType;
	if (m_bSelectedIsPage)
	{
		cop.parentName = &m_szSelectedPage;
	}
	else
	{
		cop.parentName = &m_szSelectedGroup;
	}
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
	if (retval == MSG_HANDLED)
	{
		iop.name = &refName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
	}

	vector <CHashString> names, types;
	names.push_back(refName);
	names.push_back(objName);
	types.push_back(CHashString(_T("CGUIInstance")));
	types.push_back(eType);
	m_FrmParent->m_OptionTree->UpdateOptionTree(names, types);

	static DWORD msgHash_SetItemReference = CHashString(_T("SetItemReference")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetItemReference, sizeof(CHashString), &objName, &refName, &refType);

	m_FrmParent->m_Tools.AddElement(m_bSelectedIsPage, refType, refName);
	
	INSTDATA id;
	id.m_szName = refName;
		
	m_szSelectedItemName = refName;
	m_szSelectedItemType = refType;	

	if (m_bSelectedIsPage)
	{
		GUIPAGEMESSAGE gpm;
		gpm.name = &refName;
		gpm.compType = &refType;
		static DWORD msgHash_AddGUIElementToPage = CHashString(_T("AddGUIElementToPage")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_AddGUIElementToPage, sizeof(GUIPAGEMESSAGE), &gpm, &m_szSelectedPage, &CHashString(_T("CGUIPage")));

		id.m_szParentName = m_szSelectedPage;
		m_mItems[objName.GetUniqueID()].push_back(id);
	}		
	else
	{
		ADDGUIELEMENT age;
		age.m_CompType = &refType;
		age.m_GuiName = &refName;
		static DWORD msgHash_AddGUIElement = CHashString(_T("AddGUIElement")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_AddGUIElement, sizeof(ADDGUIELEMENT), &age, &m_szSelectedGroup, &CHashString(_T("CGUIGroup")));

		id.m_szParentName = m_szSelectedGroup;
		m_mItems[objName.GetUniqueID()].push_back(id);
	}
	
	m_bObjCreated = true;	
	return refName;
}

void CGUIEditorDoc::DeleteItem(CHashString name, CHashString type)
{
	GUIPAGEMESSAGE gpm;
	DELETEOBJECTPARAMS dop;
	gpm.name = &name;
	gpm.compType = &type;
	gpm.wasFoundInPage = false;
static DWORD msgHash_RemoveGUIElementFromPage = CHashString(_T("RemoveGUIElementFromPage")).GetUniqueID();
    m_ToolBox->SendMessage(msgHash_RemoveGUIElementFromPage, sizeof(GUIPAGEMESSAGE), &gpm, &m_szSelectedPage, &CHashString(_T("CGUIPage")));

	if (!gpm.wasFoundInPage)
	{
		static DWORD msgHash_RemoveGUIElement = CHashString(_T("RemoveGUIElement")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveGUIElement, sizeof(IHashString), &name, &m_szSelectedGroup, &CHashString(_T("CGUIGroup")));	
		m_FrmParent->m_Tools.DeleteItemFromView(name, m_szSelectedGroup);	
	}
	else
	{
		m_FrmParent->m_Tools.DeleteItemFromView(name, m_szSelectedPage);		
	}
	if (_tcscmp(type.GetString(), _T("CGUIGroup")) == 0)
	{
		for (UINT i=0; i<m_vGroups.size(); i++)
		{
			if (_tcscmp(m_vGroups[i].GetString(), name.GetString()) == 0)
			{
				m_vGroups.erase(m_vGroups.begin() + i);
				break;
			}
		}
	}
	else // if not a group, then an instance
	{
		ITEMMAP::iterator itemIter = m_mItems.begin();
		for (; itemIter != m_mItems.end(); itemIter++)
		{
			for (UINT j=0; j<itemIter->second.size(); j++)
			{
				if (_tcscmp(itemIter->second[j].m_szName.GetString(), name.GetString()) == 0)
				{
					itemIter->second.erase(itemIter->second.begin() + j);
					if (itemIter->second.size() == 0) // no more references
					{
						CHashString deleteName = m_ToolBox->GetHashString(itemIter->first);
						dop.name = &deleteName;
						static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
						m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop);

						m_UseItemBox.RemoveItemFromList(CHashString(dop.name));

						if (_tcscmp(m_CopyStruct.name.GetString(), dop.name->GetString()) == 0)
						{
							m_CopyStruct.name = _T("");
							m_CopyStruct.type = _T("");
						}
					}
					break;
				}
			}
		}		
	}	
	dop.name = &name;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop);
}

void CGUIEditorDoc::DeleteItemFromTree(CHashString name)
{
	FINDGUIOBJECT fgo;
	DELETEOBJECTPARAMS dop;
	fgo.name = &name;
	static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
	if (fgo.bFound)
	{
		GUIPAGEMESSAGE gpm;
		gpm.name = &name;
		gpm.compType = fgo.type;
		gpm.wasFoundInPage = false;
		static DWORD msgHash_RemoveGUIElementFromPage = CHashString(_T("RemoveGUIElementFromPage")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveGUIElementFromPage, sizeof(GUIPAGEMESSAGE), &gpm, &m_szSelectedPage, &CHashString(_T("CGUIPage")));

		if (!gpm.wasFoundInPage)
		{
			static DWORD msgHash_RemoveGUIElement = CHashString(_T("RemoveGUIElement")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_RemoveGUIElement, sizeof(IHashString), &name, &m_szSelectedGroup, &CHashString(_T("CGUIGroup")));				
		}		
		if (_tcscmp(fgo.type->GetString(), _T("CGUIGroup")) == 0)
		{
			for (UINT i=0; i<m_vGroups.size(); i++)
			{
				if (_tcscmp(m_vGroups[i].GetString(), name.GetString()) == 0)
				{
					m_vGroups.erase(m_vGroups.begin() + i);
					break;
				}
			}
		}
		else // if not a group, then an instance
		{
			ITEMMAP::iterator itemIter = m_mItems.begin();
			for (; itemIter != m_mItems.end(); itemIter++)
			{
				for (UINT j=0; j<itemIter->second.size(); j++)
				{
					if (_tcscmp(itemIter->second[j].m_szName.GetString(), name.GetString()) == 0)
					{
						itemIter->second.erase(itemIter->second.begin() + j);
						if (itemIter->second.size() == 0) // no more references
						{
							CHashString deleteName = m_ToolBox->GetHashString(itemIter->first);
							dop.name = &deleteName;
							static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
							m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop);

							m_UseItemBox.RemoveItemFromList(CHashString(dop.name));

							if (_tcscmp(m_CopyStruct.name.GetString(), dop.name->GetString()) == 0)
							{
								m_CopyStruct.name = _T("");
								m_CopyStruct.type = _T("");
							}
						}
						break;
					}
				}
			}
		}
	}	
	dop.name = &name;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop);
}

void CGUIEditorDoc::SetCurrentPage(CHashString name)
{
	static DWORD msgHash_GUIManagerSetActivePage = CHashString(_T("GUIManagerSetActivePage")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GUIManagerSetActivePage, sizeof(IHashString), &name);
	SELECTRECT sr;
	sr.x1 = sr.x2 = sr.y1 = sr.y2 = 0.f;
	static DWORD msgHash_SetRect = CHashString(_T("SetRect")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetRect, sizeof(SELECTRECT), &sr, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	m_szSelectedItemName = _T("");
	m_szSelectedItemType = _T("");
	m_FrmParent->m_OptionTree->ClearOptionTree();
	m_szSelectedPage = name;
	m_View->SetSelectedPage(name);
}

void CGUIEditorDoc::SetCurrentGroup(CHashString name)
{
	FINDOBJNAME fon;
	CHashString groupType = _T("CGUIGroup");
	fon.compType = &groupType;
	fon.searchName = &name;
	static DWORD msgHash_FollowObject = CHashString(_T("FollowObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_FollowObject, sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	m_szSelectedItemName = name;
	m_szSelectedItemType = _T("CGUIGroup");
	m_FrmParent->m_OptionTree->UpdateOptionTree(&m_szSelectedItemName, &m_szSelectedItemType);
	m_szSelectedGroup = name;
	m_View->SetSelectedGroup(name);
}
	
void CGUIEditorDoc::SetCurrentObject(CHashString name)
{
	FINDGUIOBJECT fgo;
	FINDOBJNAME fon;
    fgo.name = &name;
	static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
	fon.searchName = &name;
	fon.compType = fgo.type;
	static DWORD msgHash_FollowObject = CHashString(_T("FollowObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_FollowObject, sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	m_szSelectedItemName = name;
	m_szSelectedItemType = fon.compType->GetString();
	if (_tcscmp(fon.compType->GetString(), _T("CGUIInstance")) == 0)
	{
		CHashString itemRef;
		static DWORD msgHash_GetItemReference = CHashString(_T("GetItemReference")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetItemReference, sizeof(IHashString), &itemRef, &name, &CHashString(_T("CGUIInstance")));
		fgo.name = &itemRef;
		static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);

		vector <CHashString> names, types;
		names.push_back(m_szSelectedItemName);
		names.push_back(itemRef);
		types.push_back(CHashString(_T("CGUIInstance")));
		types.push_back(CHashString(fgo.type->GetString()));
		m_FrmParent->m_OptionTree->UpdateOptionTree(names, types);
	}
	else
	{
		m_FrmParent->m_OptionTree->UpdateOptionTree(&name, fon.compType);
	}

	m_View->SetSelectedObject(name, CHashString(fon.compType));
}

void CGUIEditorDoc::SerializeCursor()
{
	if (!m_bCursorCreated)
	{
		m_bCursorCreated = true;
		CREATEOBJECTPARAMS cop;
		INITOBJECTPARAMS iop;
		CHashString cName = _T("MouseCursor");
		CHashString cType = _T("CGUICursor");
		CHashString pName = _T("World");
		cop.name = &cName;
		cop.typeName = &cType;
		cop.parentName = &pName;
		iop.name = &cName;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
		if (retval == MSG_HANDLED)
		{
			static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
		}
		m_FrmParent->m_OptionTree->UpdateOptionTree(&cName, &cType);
		m_FrmParent->m_Tools.AddCursor(cName);
		m_szCursorName = cName;
	}
	else
	{
		m_FrmParent->m_OptionTree->UpdateOptionTree(&m_szCursorName, &CHashString(_T("CGUICursor")));
	}
}

void CGUIEditorDoc::ChangeBGColor(UINT r, UINT g, UINT b)
{
	m_View->SetBGColor(r, g, b);
	m_FullScreen.SetColor(r, g, b);
}

void CGUIEditorDoc::MouseMove(int x, int y)
{
	if (m_FrmParent)
	{
		m_FrmParent->m_GUIDataDlg.SetMousePosition(x, y);
	}
}
	
void CGUIEditorDoc::ZoomChange(float zf)
{
	if (m_FrmParent)
	{
		m_FrmParent->m_GUIDataDlg.SetZoom(zf);
	}
}

void CGUIEditorDoc::SelectionChange()
{
	if (_tcscmp(m_szCursorName.GetString(), _T("")) != 0)
	{
		CURSORVISIBLE cv;
		cv.bVisible = false;
		static DWORD msgHash_SetCursorVisible = CHashString(_T("SetCursorVisible")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetCursorVisible, sizeof(CURSORVISIBLE), &cv, &m_szCursorName, &CHashString(_T("CGUICursor")));
	}
}

void CGUIEditorDoc::ReturnToWindow()
{
	if (m_bCursorCreated)
	{
		CURSORVISIBLE cv;
		cv.bVisible = false;
		static DWORD msgHash_SetCursorVisible = CHashString(_T("SetCursorVisible")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetCursorVisible, sizeof(CURSORVISIBLE), &cv, &m_szCursorName, &CHashString(_T("CGUICursor")));
	}
	bool active = false;
	static DWORD msgHash_SetActiveMode = CHashString(_T("SetActiveMode")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetActiveMode, sizeof(bool), &active);
	m_View->SetFullscreenMode(false);
	m_View->ResetView();
	m_View->SetFocus();	
	AddWidgets(m_szSelectedPage);
}

void CGUIEditorDoc::CopyCurrent(CHashString name, CHashString type)
{
	if (_tcscmp(name.GetString(), _T("")) == 0)
	{		
		return;
	}
	
	m_CopyStruct.name = name;
	m_CopyStruct.type = type;	
}
	
void CGUIEditorDoc::PasteNewItem()
{
	CHashString oldVersion = _T("");
	CHashString newVersion = _T("2.0b");

	static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &oldVersion);
	static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &newVersion);

	if (_tcscmp(m_CopyStruct.type.GetString(), _T("CGUIInstance")) == 0)
	{
		// get original item name
		CHashString originalItemName = _T("");
		static DWORD msgHash_GetItemReference = CHashString(_T("GetItemReference")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_GetItemReference, sizeof(IHashString), &originalItemName, &m_CopyStruct.name, &m_CopyStruct.type);

		FINDGUIOBJECT fgo;
		fgo.name = &originalItemName;
		static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
		if (fgo.bFound)
		{
			
			UINT copyNum = 0;
			StdString copyName = originalItemName.GetString();
			
			// remove _orig specifier
			StdString token;
			copyName.GetToken(_T("_"), token);
			copyName = token;
			copyName += " copy ";

			TCHAR buf[4];
			_stprintf(buf, "%d", copyNum);
			copyName += buf;

			// if we've already copied it...
			while (CheckNameIsRegistered(CHashString((const TCHAR*)copyName)))
			{
				copyNum ++; // increment copy number
				copyName = m_CopyStruct.name.GetString();
				copyName += " copy ";
				_stprintf(buf, "%d", copyNum);
				copyName += buf;
			}
			CHashString objType(fgo.type);
			CHashString instName(copyName);
			AddElement(instName, objType, false);

			copyName += _T("_orig"); // added in addelement

			// create an archive, serialize original in
			CHashString arType = _T("Memory");
			CREATEARCHIVE ca;
			ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
			ca.streamData = NULL;
			ca.streamSize = 0;
			ca.streamType = &arType;
			static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
			assert(ca.archive);

			IArchive *ar = ca.archive;

			// serialize instance
			SERIALIZEOBJECTPARAMS sop;
			sop.archive = ar;
			sop.archive->SetIsWriting(true);
			sop.name = &m_CopyStruct.name;
			static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

			// now serialize into new instance
			sop.archive->SetIsWriting(false);
			sop.name = &instName;
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

			// serialize reference
			sop.archive = ar;
			sop.archive->SetIsWriting(true);
			sop.name = &originalItemName;
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

			// now serialize into new item
			CHashString objName = copyName;
			sop.archive->SetIsWriting(false);
			sop.name = &objName;
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

			// TODO: clean up horrid redundancy... 
			// serialization of instance above overwrites the freshly set reference, so we need 
			// to reset the reference... bah!
			static DWORD msgHash_SetItemReference = CHashString(_T("SetItemReference")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SetItemReference, sizeof(CHashString), &objName, &instName);

			// set as currently selected object
			SetCurrentObject(instName);
		}
	}
	else if (_tcscmp(m_CopyStruct.type.GetString(), _T("CGUIGroup")) == 0)
	{
		UINT copyNum = 0;
		StdString copyName = m_CopyStruct.name.GetString();
		copyName += " copy ";

		TCHAR buf[4];
		_stprintf(buf, "%d", copyNum);
		copyName += buf;

		// if we've already copied it...
		while (CheckNameIsRegistered(CHashString((const TCHAR*)copyName)))
		{
			copyNum ++; // increment copy number
			copyName = m_CopyStruct.name.GetString();
			copyName += " copy ";
			_stprintf(buf, "%d", copyNum);
			copyName += buf;
		}
		
		CHashString groupName = copyName;
		AddGroup(groupName, false);

		SetCurrentGroup(groupName);
		SetIsPageSelected(false);
		
		GUIELEMLIST gel;
		static DWORD msgHash_GetGroupItemList = CHashString(_T("GetGroupItemList")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_GetGroupItemList, sizeof(GUIELEMLIST), &gel, &m_CopyStruct.name, &m_CopyStruct.type);	
		if (retval == MSG_HANDLED)
		{
			for (UINT i=0; i<gel.m_vNames.size(); i++)
			{
				CHashString itemName = gel.m_vNames[i];
				CHashString itemType = gel.m_vTypes[i];

				// serialize item inst
				CHashString arType = _T("Memory");
				CREATEARCHIVE ca;
				ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
				ca.streamData = NULL;
				ca.streamSize = 0;
				ca.streamType = &arType;
				static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
				assert(ca.archive);

				IArchive *ar = ca.archive;

				SERIALIZEOBJECTPARAMS sop;
				sop.archive = ar;
				sop.archive->SetIsWriting(true);
				sop.name = &itemName;
				static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

				if (_tcscmp(itemType.GetString(), _T("CGUIInstance")) == 0)
				{
					CHashString refName, refType;
					refName = _T("");
					static DWORD msgHash_GetItemReference = CHashString(_T("GetItemReference")).GetUniqueID();
					retval = m_ToolBox->SendMessage(msgHash_GetItemReference, sizeof(IHashString), &refName, &itemName, &itemType);
					
					FINDGUIOBJECT fgo;
					fgo.name = &refName;
					static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
					retval = m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
					if (fgo.bFound)
					{
						// check to see if instance is shallow copy or deep copy
						StdString nameToken, parentToken, numberToken, fullName;
						fullName = itemName.GetString();
						fullName.GetToken(_T("_"), nameToken);
						fullName.GetToken(_T("_"), parentToken);
						fullName.GetToken(_T("_"), numberToken);

						CHashString hashNoOrig, objType(fgo.type);
						StdString nameNoOrig = refName.GetString(), token;
						nameNoOrig.GetToken(_T("_"), token);
						nameNoOrig = token;

						CHashString newInstance;

						// make new instance
						if (parentToken != _T(""))
						{
							// for some reason, serialization is dying... need to serialize before and after
							IArchive *objAr;
							ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
							ca.streamData = NULL;
							ca.streamSize = 0;
							ca.streamType = &arType;
							static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
							m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
							assert(ca.archive);
							objAr = ca.archive;

							SERIALIZEOBJECTPARAMS objSop;
							objSop.archive = objAr;
							objSop.archive->SetIsWriting(true);
							objSop.name = &refName;
							static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
							m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &objSop);

							hashNoOrig = nameNoOrig;
							newInstance = AddElement(hashNoOrig, objType, true);

							// serialize new created instance to match position and size
							sop.archive->SetIsWriting(false);
							sop.name = &newInstance;
							m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

							ar->Close();

							// reserialize
							objSop.archive->SetIsWriting(false);
							m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &objSop);
							objAr->Close();                            							
						}
						// make new copy
						else
						{
							// create archive and serialize backing object, since we're making new objects
							IArchive *objAr;
							ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
							ca.streamData = NULL;
							ca.streamSize = 0;
							ca.streamType = &arType;
							static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
							m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
							assert(ca.archive);
							objAr = ca.archive;

							SERIALIZEOBJECTPARAMS objSop;
							objSop.archive = objAr;
							objSop.archive->SetIsWriting(true);
							objSop.name = &refName;
							static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
							m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &objSop);

							nameNoOrig += " copy ";
							UINT copyNum = 0;

							TCHAR buf[4];
							_stprintf(buf, "%d", copyNum);
							nameNoOrig += buf;

							// if we've already copied it...
							while (CheckNameIsRegistered(CHashString((const TCHAR*)nameNoOrig)))
							{
								copyNum ++; // increment copy number
								nameNoOrig = m_CopyStruct.name.GetString();
								nameNoOrig += " copy ";
								_stprintf(buf, "%d", copyNum);
								nameNoOrig += buf;
							}

							hashNoOrig = nameNoOrig;

							newInstance = AddElement(hashNoOrig, objType, false);

							// serialize new created instance to match position and size
							sop.archive->SetIsWriting(false);
							sop.name = &newInstance;
							m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

							// need to reference and serialize new object!
							StdString newRealObject = nameNoOrig;
							newRealObject += _T("_orig");
							CHashString hashRealObject(newRealObject);
							static DWORD msgHash_SetItemReference = CHashString(_T("SetItemReference")).GetUniqueID();
							m_ToolBox->SendMessage(msgHash_SetItemReference, sizeof(CHashString), &hashRealObject, &newInstance, &CHashString(_T("CGUIInstance")));

							objSop.archive->SetIsWriting(false);
							objSop.name = &hashRealObject;
							m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &objSop);
							objAr->Close();							
						}
					}
				}
			}
		}
	}	
	m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &oldVersion);
}

void CGUIEditorDoc::PasteNewInstance()
{
	CHashString oldVersion = _T("");
	CHashString newVersion = _T("2.0c");

	static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &oldVersion);
	static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &newVersion);

	if (_tcscmp(m_CopyStruct.type.GetString(), _T("CGUIInstance")) == 0)
	{
		CHashString originalItemName = _T("");
		static DWORD msgHash_GetItemReference = CHashString(_T("GetItemReference")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_GetItemReference, sizeof(IHashString), &originalItemName, &m_CopyStruct.name, &m_CopyStruct.type);

		FINDGUIOBJECT fgo;
		fgo.name = &originalItemName;
		static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
		if (fgo.bFound)
		{
			CHashString arType = _T("Memory");
			CREATEARCHIVE ca;
			ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
			ca.streamData = NULL;
			ca.streamSize = 0;
			ca.streamType = &arType;
			static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
			assert(ca.archive);

			IArchive *ar = ca.archive;

			// make another archive for the instances
			m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
			assert(ca.archive);

			IArchive *instArchive = ca.archive;

			SERIALIZEOBJECTPARAMS sop;
			sop.archive = ar;
			sop.archive->SetIsWriting(true);
			sop.name = &originalItemName;
			static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
			
			CHashString originalType(fgo.type);
			CHashString hashNameNoTag;
			StdString origNoTag = originalItemName.GetString(), token;
			origNoTag.GetToken(_T("_"), token);
			origNoTag = token;
			hashNameNoTag = origNoTag;

			// serialize original instance
			sop.archive = instArchive;
			sop.archive->SetIsWriting(true);
			sop.name = &m_CopyStruct.name;
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

			// interesting little bug with the option tree...
			CHashString newName = AddElement(hashNameNoTag, originalType, true);

			// have to reserialize the original object, because creating a new tree clears it
			sop.name = &originalItemName;
			sop.archive = ar;
			sop.archive->SetIsWriting(false);				
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

			// now serialize into new instance
			sop.archive = instArchive;
			sop.archive->SetIsWriting(false);
			sop.name = &newName;
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
		}
	}
	m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &oldVersion);
}

void CGUIEditorDoc::UpdateTreeSerializeCallback(StdString name, StdString type, IArchive *ar)
{
	CHashString oldVersion = _T("");
	CHashString newVersion = _T("2.0c");

	static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &oldVersion);
	static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &newVersion);
	DWORD retval;
	CHashString eName = name;
	if (_tcscmp(type, _T("CGUIInstance")) == 0) // serialize two objects
	{
		SERIALIZEOBJECTPARAMS sop;
		sop.archive = ar;
		sop.name = &eName;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		retval = EngineGetToolBox()->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

		CHashString refName = _T("");
		static DWORD msgHash_GetItemReference = CHashString(_T("GetItemReference")).GetUniqueID();
		retval = EngineGetToolBox()->SendMessage(msgHash_GetItemReference, sizeof(IHashString), &refName, &eName, &CHashString(_T("CGUIInstance")));
		if (_tcscmp(refName.GetString(), _T("")) == 0)
		{
			return;
		}
		sop.name = &refName;
		retval = EngineGetToolBox()->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
	}
	else // just serialize one
	{
		CHashString oName = name;
		SERIALIZEOBJECTPARAMS sop;
		sop.archive = ar;
		sop.name = &oName;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		retval = EngineGetToolBox()->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
	}
	EngineGetToolBox()->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &oldVersion);
}

void CGUIEditorDoc::AddWidgets(CHashString page)
{
	DWORD retval;
	CHashString pType = _T("CGUIPage");

	CHashString cName, cType, rName, rType;
	cName = _T("WorkingCanvas");
	cType = _T("CGUIBackground");
	rName = _T("SelectRect");
	rType = _T("CGUIRect");

	GUIPAGEMESSAGE gpm;

	gpm.name = &cName;
	gpm.compType = &cType;
	static DWORD msgHash_AddGUIElementToPage = CHashString(_T("AddGUIElementToPage")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_AddGUIElementToPage, sizeof(GUIPAGEMESSAGE), &gpm, &page, &pType);

	gpm.name = &rName;
	gpm.compType = &rType;
	retval = m_ToolBox->SendMessage(msgHash_AddGUIElementToPage, sizeof(GUIPAGEMESSAGE), &gpm, &page, &pType);
}
	
void CGUIEditorDoc::RemoveWidgets(CHashString page)
{
	DWORD retval;
	CHashString pType = _T("CGUIPage");

	CHashString cName, cType, rName, rType;
	cName = _T("WorkingCanvas");
	cType = _T("CGUIBackground");
	rName = _T("SelectRect");
	rType = _T("CGUIRect");

	GUIPAGEMESSAGE gpm;

	gpm.name = &cName;
	gpm.compType = &cType;
	static DWORD msgHash_RemoveGUIElementFromPage = CHashString(_T("RemoveGUIElementFromPage")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_RemoveGUIElementFromPage, sizeof(GUIPAGEMESSAGE), &gpm, &page, &pType);

	gpm.name = &rName;
	gpm.compType = &rType;
	retval = m_ToolBox->SendMessage(msgHash_RemoveGUIElementFromPage, sizeof(GUIPAGEMESSAGE), &gpm, &page, &pType);
}

bool CGUIEditorDoc::RenameItem(CHashString oldName, CHashString newName, bool isTemplateName)
{
	DWORD retval;
	bool parentOrGroupNamed = false; // allows us to differentiate between renaming an instance or a 'template'
	if (!isTemplateName)
	{
		if (!CheckNameIsRegistered(newName))
		{
			CHANGEOBJNAME con;
			con.oldName = &oldName;
			con.newName = &newName;
			static DWORD msgHash_RenameObject = CHashString(_T("RenameObject")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_RenameObject, sizeof(CHANGEOBJNAME), &con);

			m_FrmParent->m_Tools.RenameExternal(oldName, newName);		

			m_FrmParent->m_OptionTree->RenameObject(oldName, newName);

			UINT i;		
			for (i=0; i<m_vPages.size(); i++)
			{
				if (_tcscmp(m_vPages[i].GetString(), oldName.GetString()) == 0)
				{
					m_vPages.erase(m_vPages.begin() + i);
					m_vPages.push_back(newName);
					m_View->SetSelectedPage(newName);
					m_szSelectedPage = newName;			
					parentOrGroupNamed = true;
				}
			}
			for (i=0; i<m_vGroups.size(); i++)
			{
				if (_tcscmp(m_vGroups[i].GetString(), oldName.GetString()) == 0)
				{
					m_vGroups.erase(m_vGroups.begin() + i);
					m_vGroups.push_back(newName);				
					m_View->SetSelectedGroup(newName);
					m_szSelectedGroup = newName;	
					parentOrGroupNamed = true;
				}
			}
			ITEMMAP::iterator itemIter = m_mItems.begin();
			for (; itemIter != m_mItems.end(); itemIter++)
			{
				for (UINT j=0; j<itemIter->second.size(); j++)
				{
					if (parentOrGroupNamed)
					{
						// rename based on parent name change
						if (_tcscmp(itemIter->second[j].m_szParentName.GetString(), oldName.GetString())==0)
						{
							INSTDATA id;
							id.m_szParentName = newName;
							CHashString instOldName, instNewName;

							// reset the instance name
							StdString baseName, parentName, numName, fullName, dummyToken;							
							fullName = itemIter->second[j].m_szName.GetString();
							instOldName = fullName;
							fullName.GetToken(_T("_"), baseName);
							fullName.GetToken(_T("_"), parentName);
							fullName.GetToken(_T("_"), numName);

							fullName = baseName;
							if (_tcscmp((const TCHAR*)parentName, _T("")) != 0) // parent name is part of name
							{
								parentName = newName.GetString();
								fullName += _T("_") + parentName;
							}
							if (_tcscmp((const TCHAR*)numName, _T("")) != 0) // number is part of name
							{
								fullName += _T("_") + numName;
							}					
	                        
							itemIter->second.erase(itemIter->second.begin() + j);					

							instNewName = (const TCHAR*) fullName;
							id.m_szName = instNewName;
							itemIter->second.push_back(id);

							m_FrmParent->m_Tools.RenameExternal(instOldName, instNewName);

							CHANGEOBJNAME con;
							con.oldName = &instOldName;
							con.newName = &instNewName;
							static DWORD msgHash_RenameObject = CHashString(_T("RenameObject")).GetUniqueID();
							retval = m_ToolBox->SendMessage(msgHash_RenameObject, sizeof(CHANGEOBJNAME), &con);

							m_FrmParent->m_OptionTree->RenameObject(instOldName, instNewName);
							
							j--; // need to do this because of deletion and reinsertion
						}
					}
					else
					{
						// rename based on instance name change
						if (_tcscmp(itemIter->second[j].m_szName.GetString(), oldName.GetString())==0)
						{
							INSTDATA id;
							id.m_szParentName = itemIter->second[j].m_szParentName;

							itemIter->second.erase(itemIter->second.begin() + j);					

							id.m_szName = newName;
							itemIter->second.push_back(id);
							m_View->SetSelectedObject(newName, CHashString(_T("CGUIInstance")));
						}
					}
				}
			}		
			return true;
		}
	}
	else
	{
		if (!CheckNameIsRegistered(newName))
		{
			StdString objName, fullIName, baseIName, parentIName, numIName;
			CHashString oldInstName, newInstName, oldObjName, newObjName;

			objName = oldName.GetString();
			objName += _T("_orig");
			oldObjName = objName;

			objName = newName.GetString();
			objName += _T("_orig");
			newObjName = objName;

			// find spot in map with old name
			ITEMMAP::iterator itemIter = m_mItems.find(oldObjName.GetUniqueID());
			if (itemIter == m_mItems.end())
			{
				MessageBox(NULL, _T("Template does not exist...?"), _T("Uh oh"), MB_OK);
				return false;
			}

			CHANGEOBJNAME con;
			con.oldName = &oldObjName;
			con.newName = &newObjName;
			static DWORD msgHash_RenameObject = CHashString(_T("RenameObject")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_RenameObject, sizeof(CHANGEOBJNAME), &con);	

			m_UseItemBox.RemoveItemFromList(oldName);
			FINDGUIOBJECT fgo;
			fgo.name = &newObjName;
			static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
			if (fgo.bFound)
			{
				m_UseItemBox.AddItemToList(newName, CHashString(fgo.type));
			}		

			for (UINT i=0; i<itemIter->second.size(); i++)
			{
				INSTDATA id;
				id.m_szParentName = itemIter->second[i].m_szParentName;			

				fullIName.ResetTokenPosition(); // be sure it's at 0
				fullIName = itemIter->second[i].m_szName.GetString();
				oldInstName = (const TCHAR*)fullIName;
				fullIName.GetToken(_T("_"), baseIName);
				fullIName.GetToken(_T("_"), parentIName);
				fullIName.GetToken(_T("_"), numIName);

				if(_tcscmp((const TCHAR*)baseIName, oldName.GetString()) == 0)
				{
					
					fullIName = newName.GetString();
					if (_tcscmp((const TCHAR*)parentIName, _T("")) != 0)
					{
						fullIName += _T("_") + parentIName;
					}
					if (_tcscmp((const TCHAR*)numIName, _T("")) != 0)
					{
						fullIName += _T("_") + numIName;
					}

					newInstName = (const TCHAR*) fullIName;

					itemIter->second.erase(itemIter->second.begin() + i);					

					id.m_szName = newInstName;
					
					m_mItems[newObjName.GetUniqueID()].push_back(id);

					m_FrmParent->m_Tools.RenameExternal(oldInstName, newInstName);

					CHANGEOBJNAME con;
					con.oldName = &oldInstName;
					con.newName = &newInstName;
					static DWORD msgHash_RenameObject = CHashString(_T("RenameObject")).GetUniqueID();
					retval = m_ToolBox->SendMessage(msgHash_RenameObject, sizeof(CHANGEOBJNAME), &con);

					m_FrmParent->m_OptionTree->RenameObject(oldInstName, newInstName);

					static DWORD msgHash_SetItemReference = CHashString(_T("SetItemReference")).GetUniqueID();
					retval = m_ToolBox->SendMessage(msgHash_SetItemReference, sizeof(CHashString), &newObjName, &newInstName, &CHashString(_T("CGUIInstance")));
					
					i--; // need to do this because of deletion and reinsertion

					if (oldInstName.GetUniqueID() == m_szSelectedItemName.GetUniqueID())
					{
						SetCurrentObject(newInstName);
					}
				}
			}
			m_mItems.erase(itemIter);
			return true;
		}
	}
	return false;
}

void CGUIEditorDoc::InstLockPressed()
{
	if (_tcscmp(m_szSelectedItemName.GetString(), _T("")) != 0)
	{
		StdString rootName, objName, token;
		CHashString objHashName;
		rootName = m_szSelectedItemName.GetString();
		rootName.GetToken(_T("_"), token);
		rootName = token;
		objName = rootName;
		objName += _T("_orig");
		objHashName = (const TCHAR*)objName;
		if (m_mItems[objHashName.GetUniqueID()].size() > 1)
		{
			if (MessageBox(NULL, _T("WARNING: Will reposition and resize all instances of same button to match selected one. Continue?"), _T("Danger Will Robinson!"), MB_OKCANCEL|MB_ICONWARNING) == IDOK)
			{
				POSITIONDATA pd;
				SIZEDATA sd;
				DWORD retval;
				static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
				retval = m_ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &pd, &m_szSelectedItemName, &m_szSelectedItemType);
				static DWORD msgHash_GetSize = CHashString(_T("GetSize")).GetUniqueID();
				retval = m_ToolBox->SendMessage(msgHash_GetSize, sizeof(SIZEDATA), &sd, &m_szSelectedItemName, &m_szSelectedItemType);

				for (UINT i=0; i<m_mItems[objHashName.GetUniqueID()].size(); i++)
				{
					if (_tcscmp(m_mItems[objHashName.GetUniqueID()][i].m_szName.GetString(), m_szSelectedItemName.GetString()) != 0)
					{
						static DWORD msgHash_SetPosition = CHashString(_T("SetPosition")).GetUniqueID();
						retval = m_ToolBox->SendMessage(msgHash_SetPosition, sizeof(POSITIONDATA), &pd, &m_mItems[objHashName.GetUniqueID()][i].m_szName, &CHashString(_T("CGUIInstance")));
						static DWORD msgHash_SetSize = CHashString(_T("SetSize")).GetUniqueID();
						retval = m_ToolBox->SendMessage(msgHash_SetSize, sizeof(SIZEDATA), &sd, &m_mItems[objHashName.GetUniqueID()][i].m_szName, &CHashString(_T("CGUIInstance")));					
					}
				}
			}
		}
	}
}
void CGUIEditorDoc::OnCloseDocument()
{
	static DWORD msgHash_GUIEditorDocClose = CHashString(_T("GUIEditorDocClose")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GUIEditorDocClose, 0, NULL);
	static DWORD msgHash_ClearGUIObjects = CHashString(_T("ClearGUIObjects")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_ClearGUIObjects, 0, NULL);
	CDocument::OnCloseDocument();
}

void CGUIEditorDoc::CreateWidgets()
{
	// create editor specific widgets
	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	DWORD retval;

	CHashString cName, pName, rName, cType, rType;
	cName = _T("WorkingCanvas");
	pName = _T("World");
	rName = _T("SelectRect");
	cType = _T("CGUIBackground");
	rType = _T("CGUIRect");

	IToolBox *m_ToolBox = EngineGetToolBox();

	cop.name = &cName;
	cop.parentName = &pName;
	cop.typeName = &cType;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
	if (retval = MSG_HANDLED)
	{
		iop.name = &cName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
	}

	cop.name = &rName;
	cop.parentName = &pName;
	cop.typeName = &rType;
	retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
	if (retval = MSG_HANDLED)
	{
		iop.name = &rName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
	}

	POSITIONDATA pd;
	pd.m_fXPos = 0.f;
	pd.m_fYPos = 0.f;
	static DWORD msgHash_SetPosition = CHashString(_T("SetPosition")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_SetPosition, sizeof(POSITIONDATA), &pd, &cName, &cType);	

	bool editorMode = true;
	static DWORD msgHash_SetGUIEditorMode = CHashString(_T("SetGUIEditorMode")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_SetGUIEditorMode, sizeof(bool), &editorMode, NULL, NULL);
	CHashString verNum = _T("2.0c");
	static DWORD msgHash_SetGUIVersion = CHashString(_T("SetGUIVersion")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_SetGUIVersion, sizeof(CHashString), &verNum, NULL, NULL);

	bool active = false;
	static DWORD msgHash_SetActiveMode = CHashString(_T("SetActiveMode")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetActiveMode, sizeof(bool), &active, NULL, NULL);
}

void CGUIEditorDoc::ShowHideGroup(CHashString gName)
{
	if (m_bSelectedIsPage == false)
	{
		CHashString gType = _T("CGUIGroup");
		bool isVisible;
		static DWORD msgHash_GetIsGroupVisible = CHashString(_T("GetIsGroupVisible")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_GetIsGroupVisible, sizeof(bool), &isVisible, &gName, &gType);
		if (isVisible)
		{
			static DWORD msgHash_HideGroup = CHashString(_T("HideGroup")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_HideGroup, 0, NULL, &gName, &gType);			
		}
		else
		{
			static DWORD msgHash_ShowGroup = CHashString(_T("ShowGroup")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_ShowGroup, 0, NULL, &gName, &gType);
		}
		m_FrmParent->m_Tools.ToggleGroupVisible(gName, !isVisible);		
	}
}

void CGUIEditorDoc::SetCurrentAnimation(CHashString name)
{
	COptionTreeDock *optionTree = m_FrmParent->m_OptionTree;
	if (optionTree)
	{
		optionTree->UpdateOptionTree(&name, &CHashString("CGUIAnimation"));
	}
	
	m_FrmParent->m_AnimView.SetAnimation(name);
	m_hszSelectedAnimName = name;
}

void CGUIEditorDoc::NewAnimationPressed()
{
	m_NewAnimationBox.ShowWindow(SW_SHOWNORMAL);
	m_NewAnimationBox.SetActiveWindow();
	m_NewAnimationBox.BringWindowToTop();	

	StdString aName = _T("Anim ");
	TCHAR buf[4];
	_stprintf(buf, "%d", m_vAnims.size());
	aName += buf;

	m_NewAnimationBox.SetAnimName(aName);
}

void CGUIEditorDoc::PlayAnimationPressed()
{
	if (_tcscmp(m_hszSelectedAnimName.GetString(), _T("")) != 0)
	{
		static DWORD msgHash_StartGUIAnimation = CHashString(_T("StartGUIAnimation")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_StartGUIAnimation, 0, NULL, &m_hszSelectedAnimName, &CHashString(_T("CGUIAnimation")));
		if (retval != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGWARNING, _T("Could not play %s animation!\n"), m_hszSelectedAnimName.GetString());
		}
	}
}

void CGUIEditorDoc::StopAnimationPressed()
{
	if (_tcscmp(m_hszSelectedAnimName.GetString(), _T("")) != 0)
	{
		static DWORD msgHash_StopGUIAnimation = CHashString(_T("StopGUIAnimation")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_StopGUIAnimation, 0, NULL, &m_hszSelectedAnimName, &CHashString(_T("CGUIAnimation")));
		if (retval != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGWARNING, _T("Could not play %s animation!\n"), m_hszSelectedAnimName.GetString());
		}
	}
}

void CGUIEditorDoc::SetAnimBarPos(UINT pos)
{
	m_FrmParent->m_AnimView.SetAnimBarPos(pos);
}

void CGUIEditorDoc::AddAnimation(CHashString aName)
{
	if (!CheckNameIsRegistered(aName))
	{
		CHashString aType = _T("CGUIAnimation");
		CREATEOBJECTPARAMS cop;
		INITOBJECTPARAMS iop;
		cop.name = &aName;
		cop.typeName = &aType;
		cop.parentName = NULL;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
		if (retval == MSG_HANDLED)
		{
			iop.name = &aName;
			static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
		}
		
		StdString tempString(aName.GetString());
		m_FrmParent->m_AnimView.LoadAnimation(tempString);
		SetCurrentAnimation(aName);

		m_vAnims.push_back(aName);
	}
}

void CGUIEditorDoc::SetKey(StdString objName, StdString objType)
{
	if (_tcscmp(m_hszSelectedAnimName.GetString(), _T("")) != 0)
	{
		if (_tcscmp(objName, _T("")) != 0)
		{
			CHashString hszObjName = objName;
			CHashString hszObjType = objType;
			UINT time = m_FrmParent->m_AnimView.GetAnimBarPos();
			POSITIONDATA pd;
			pd.m_fXPos = pd.m_fYPos = 0.f;
			SIZEDATA sd;
			sd.m_fXSize = sd.m_fYSize = 0.f;
			DWORD col;
			col = 0xffffffff;
			float angle = 0.f;

			bool bIsRelative = false;
			static DWORD msgIsGUIAnimationRelativePositions = CHashString(_T("IsGUIAnimationRelativePositions")).GetUniqueID();
			m_ToolBox->SendMessage(msgIsGUIAnimationRelativePositions, sizeof(bool), &bIsRelative, &m_hszSelectedAnimName, &CHashString(_T("CGUIAnimation")));

			if (bIsRelative)
			{
				static DWORD msgHash_GetPositionRelative = CHashString(_T("GetPositionRelative")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_GetPositionRelative, sizeof(POSITIONDATA), &pd, &hszObjName, &hszObjType);
			}
			else
			{
				static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &pd, &hszObjName, &hszObjType);
			}
			static DWORD msgHash_GetSize = CHashString(_T("GetSize")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetSize, sizeof(SIZEDATA), &sd, &hszObjName, &hszObjType);
			static DWORD msgHash_GetColor = CHashString(_T("GetColor")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetColor, sizeof(DWORD), &col, &hszObjName, &hszObjType);
			static DWORD msgHash_GetAngle = CHashString(_T("GetAngle")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetAngle, sizeof(float), &angle, &hszObjName, &hszObjType);

			ADDMULTICHANNELKEYTOGUIANIM aktga;
			aktga.objName = hszObjName.GetString();
			aktga.objType = hszObjType.GetString();
			aktga.key = time;

			aktga.xPos = pd.m_fXPos;
			aktga.yPos = pd.m_fYPos;
			aktga.width = sd.m_fXSize;
			aktga.height = sd.m_fYSize;

			float a, r, g, b;
			a = (float)((col & 0xff000000) >> 24);
			r = (float)((col & 0x00ff0000) >> 16);
			g = (float)((col & 0x0000ff00) >> 8);
			b = (float)((col & 0x000000ff));
			aktga.a = a;
			aktga.r = r;
			aktga.g = g;
			aktga.b = b;

			aktga.angle = angle;

			static DWORD msgHash_AddKeysToAnimation = CHashString(_T("AddKeysToAnimation")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_AddKeysToAnimation, sizeof(ADDMULTICHANNELKEYTOGUIANIM), &aktga, &m_hszSelectedAnimName, &CHashString(_T("CGUIAnimation")));

			// updates data
			m_FrmParent->m_AnimView.SetAnimation(m_hszSelectedAnimName);
		}
	}
}

void CGUIEditorDoc::AddOneKey(StdString objName, UINT key, UINT channelType)
{
	if (_tcscmp(m_hszSelectedAnimName.GetString(), _T("")) != 0)
	{
		CHashString hszName(objName);
		CHashString hszType = _T("");

		FINDGUIOBJECT fgo;
		fgo.name = &hszName;
		fgo.bFound = false;
		static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
		if (fgo.bFound)
		{
			hszType = fgo.type->GetString();
			switch(channelType)
			{
				case 0: // pos
				{
					POSITIONDATA pd;
					pd.m_fXPos = 0.f;
					pd.m_fYPos = 0.f;
					bool bIsRelative = false;
					static DWORD msgIsGUIAnimationRelativePositions = CHashString(_T("IsGUIAnimationRelativePositions")).GetUniqueID();
					m_ToolBox->SendMessage(msgIsGUIAnimationRelativePositions, sizeof(bool), &bIsRelative, &m_hszSelectedAnimName, &CHashString(_T("CGUIAnimation")));

					if (bIsRelative)
					{
						static DWORD msgHash_GetPositionRelative = CHashString(_T("GetPositionRelative")).GetUniqueID();
						m_ToolBox->SendMessage(msgHash_GetPositionRelative, sizeof(POSITIONDATA), &pd, &hszName, &hszType);
					}
					else
					{
						static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
						m_ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &pd, &hszName, &hszType);
					}
					
					ADDSINGLECHANNELKEYTOGUIANIM aktga;
					aktga.objName = objName;
					aktga.key = key;
					aktga.channel = channelType;
					aktga.val1 = pd.m_fXPos;
					aktga.val2 = pd.m_fYPos;
					static DWORD msgHash_AddOneKey = CHashString(_T("AddOneKey")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_AddOneKey, sizeof(ADDSINGLECHANNELKEYTOGUIANIM), &aktga, &m_hszSelectedAnimName, &CHashString(_T("CGUIAnimation")));
					break;
				}
				case 1: // size
				{
					SIZEDATA sd;
					sd.m_fXSize = 0.f;
					sd.m_fYSize = 0.f;
					static DWORD msgHash_GetSize = CHashString(_T("GetSize")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_GetSize, sizeof(SIZEDATA), &sd, &hszName, &hszType);
					
					ADDSINGLECHANNELKEYTOGUIANIM aktga;
					aktga.objName = objName;
					aktga.key = key;
					aktga.channel = channelType;
					aktga.val1 = sd.m_fXSize;
					aktga.val2 = sd.m_fYSize;
					static DWORD msgHash_AddOneKey = CHashString(_T("AddOneKey")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_AddOneKey, sizeof(ADDSINGLECHANNELKEYTOGUIANIM), &aktga, &m_hszSelectedAnimName, &CHashString(_T("CGUIAnimation")));
					break;
				}
				case 2: // color
				{

					DWORD col;
					static DWORD msgHash_GetColor = CHashString(_T("GetColor")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_GetColor, sizeof(DWORD), &col, &hszName, &hszType);
					
					float a, r, g, b;
					a = (float)((col & 0xff000000) >> 24);
					r = (float)((col & 0x00ff0000) >> 16);
					g = (float)((col & 0x0000ff00) >> 8);
					b = (float)((col & 0x000000ff));
					
					ADDSINGLECHANNELKEYTOGUIANIM aktga;
					aktga.objName = objName;
					aktga.key = key;
					aktga.channel = channelType;
					aktga.val1 = a;
					aktga.val2 = r;
					aktga.val3 = g;
					aktga.val4 = b;
					static DWORD msgHash_AddOneKey = CHashString(_T("AddOneKey")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_AddOneKey, sizeof(ADDSINGLECHANNELKEYTOGUIANIM), &aktga, &m_hszSelectedAnimName, &CHashString(_T("CGUIAnimation")));
					break;
				}
				case 3: // rotation
				{
					float rot;
					static DWORD msgHash_GetAngle = CHashString(_T("GetAngle")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_GetAngle, sizeof(float), &rot, &hszName, &hszType);
					
					ADDSINGLECHANNELKEYTOGUIANIM aktga;
					aktga.objName = objName;
					aktga.key = key;
					aktga.channel = channelType;
					aktga.val1 = rot;					
					static DWORD msgHash_AddOneKey = CHashString(_T("AddOneKey")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_AddOneKey, sizeof(ADDSINGLECHANNELKEYTOGUIANIM), &aktga, &m_hszSelectedAnimName, &CHashString(_T("CGUIAnimation")));
					break;
				}
				default:
				{
					break;
				}
			}

			// updates data
			m_FrmParent->m_AnimView.SetAnimation(m_hszSelectedAnimName);
		}
	}
}

void CGUIEditorDoc::DeleteOneKey(StdString objName, UINT keyInd, UINT channelType)
{
	if (_tcscmp(m_hszSelectedAnimName.GetString(), _T("")) != 0)
	{
		GUIDELETEONEKEY gdok;
		gdok.objName = objName;
		gdok.keyIndex = keyInd;
		gdok.channel = channelType;

		static DWORD msgHash_DeleteOneKey = CHashString(_T("DeleteOneKey")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DeleteOneKey, sizeof(GUIDELETEONEKEY), &gdok, &m_hszSelectedAnimName, &CHashString(_T("CGUIAnimation")));

		// updates data
		m_FrmParent->m_AnimView.SetAnimation(m_hszSelectedAnimName);
	}
}

void CGUIEditorDoc::ShowPosData(UINT key, StdString objName, float x, float y)
{
	m_KeyDataView.ShowWindow(SW_SHOWNORMAL);
	m_KeyDataView.SetActiveWindow();
	m_KeyDataView.BringWindowToTop();	

	m_KeyDataView.SetPosData(objName, key, x, y);
}

void CGUIEditorDoc::ShowSizeData(UINT key, StdString objName, float width, float height)
{
	m_KeyDataView.ShowWindow(SW_SHOWNORMAL);
	m_KeyDataView.SetActiveWindow();
	m_KeyDataView.BringWindowToTop();	

	m_KeyDataView.SetSizeData(objName, key, width, height);
}

void CGUIEditorDoc::ShowColorData(UINT key, StdString objName, float a, float r, float g, float b)
{
	m_KeyDataView.ShowWindow(SW_SHOWNORMAL);
	m_KeyDataView.SetActiveWindow();
	m_KeyDataView.BringWindowToTop();	

	m_KeyDataView.SetColorData(objName, key, a, r, g, b);
}

void CGUIEditorDoc::ShowRotationData(UINT key, StdString objName, float rot)
{
	m_KeyDataView.ShowWindow(SW_SHOWNORMAL);
	m_KeyDataView.SetActiveWindow();
	m_KeyDataView.BringWindowToTop();	

	m_KeyDataView.SetRotationData(objName, key, rot);
}

void CGUIEditorDoc::UpdateKeyData(UINT key, StdString objName, UINT keyType, float v1, float v2, float v3, float v4)
{
	if (_tcscmp(m_hszSelectedAnimName.GetString(), _T("")) != 0)
	{
		ADDSINGLECHANNELKEYTOGUIANIM ascktga;
		ascktga.channel = keyType;
		ascktga.key = key;
		ascktga.objName = objName;
		ascktga.val1 = v1;
		ascktga.val2 = v2;
		ascktga.val3 = v3;
		ascktga.val4 = v4;

		static CHashString hsAnimType(_T("CGUIAnimation"));
		static DWORD msgChangeGUIAnimKey = CHashString(_T("ChangeGUIAnimKey")).GetUniqueID();
		m_ToolBox->SendMessage(msgChangeGUIAnimKey, sizeof(ADDSINGLECHANNELKEYTOGUIANIM), &ascktga, &m_hszSelectedAnimName, &hsAnimType);
	}
}