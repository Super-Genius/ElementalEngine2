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

IMPLEMENT_DYNAMIC(CGUITabBar, CGuiDialogBar)
CGUITabBar::CGUITabBar()
	
{
	m_iXPos = 0;
	m_iYPos = 0;
	m_fZoom = 1.f;
	m_bLoaded = false;
}

CGUITabBar::~CGUITabBar()
{
}

void CGUITabBar::DoDataExchange(CDataExchange* pDX)
{
	CGuiDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NEWPAGEBT, m_NewPageBt);
	DDX_Control(pDX, IDC_NEWGROUPBT, m_NewGroupBt);
	DDX_Control(pDX, IDC_NEWBUTTONBT, m_NewButtonBt);
	DDX_Control(pDX, IDC_NEWTEXTBT, m_NewTextBt);
	DDX_Control(pDX, IDC_NEWSTATICBT, m_NewStaticBt);
	DDX_Control(pDX, IDC_CHANGECOLORBT, m_ChangeBGBt);	
	DDX_Control(pDX, IDC_FULLWINDOWBT, m_FullScreenBt);
	DDX_Control(pDX, IDC_PLAYBT, m_PlayBt);
	DDX_Control(pDX, IDC_USEITEMBT, m_ItemInst);
	DDX_Control(pDX, IDC_INSTLOCK, m_InstLock);
	DDX_Control(pDX, IDC_NEWANIMATION, m_NewAnimBt);
	DDX_Control(pDX, IDC_PLAYANIM, m_PlayAnimBt);
	DDX_Control(pDX, IDC_STOPANIM, m_StopAnimBt);	
}


BEGIN_MESSAGE_MAP(CGUITabBar, CGuiDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
	ON_UPDATE_COMMAND_UI(IDC_NEWPAGEBT, OnUpdateButtons)
	ON_UPDATE_COMMAND_UI(IDC_CHANGECOLORBT, OnUpdateButtons)
	ON_UPDATE_COMMAND_UI(IDC_NEWGROUPBT, OnUpdateButtons)
	ON_UPDATE_COMMAND_UI(IDC_NEWBUTTONBT, OnUpdateButtons)
	ON_UPDATE_COMMAND_UI(IDC_NEWTEXTBT, OnUpdateButtons)
	ON_UPDATE_COMMAND_UI(IDC_NEWSTATICBT, OnUpdateButtons)
	ON_UPDATE_COMMAND_UI(IDC_USEITEMBT, OnUpdateButtons)	
	ON_UPDATE_COMMAND_UI(IDC_FULLWINDOWBT, OnUpdateButtons)	
	ON_UPDATE_COMMAND_UI(IDC_PLAYBT, OnUpdateButtons)	
	ON_UPDATE_COMMAND_UI(IDC_INSTLOCK, OnUpdateButtons)
	ON_UPDATE_COMMAND_UI(IDC_NEWANIMATION, OnUpdateButtons)
	ON_UPDATE_COMMAND_UI(IDC_PLAYANIM, OnUpdateButtons)
	ON_UPDATE_COMMAND_UI(IDC_STOPANIM, OnUpdateButtons)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

int CGUITabBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGuiDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;	
	return 0;
}

void CGUITabBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CGuiDialogBar::OnShowWindow(bShow, nStatus);	
}

LONG CGUITabBar::OnInitDialog(UINT wParam, LONG lParam)
{
	UpdateData(false);
	m_NewPageBt.SetIcon((HICON)LoadImage(GUIEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_NEWPAGEICO), IMAGE_ICON, 0, 0, 0));	
	m_NewGroupBt.SetIcon((HICON)LoadImage(GUIEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_NEWGROUPICO), IMAGE_ICON, 0, 0, 0));	
	m_NewButtonBt.SetIcon((HICON)LoadImage(GUIEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_NEWBUTTONICO), IMAGE_ICON, 0, 0, 0));	
	m_NewTextBt.SetIcon((HICON)LoadImage(GUIEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_NEWTEXTICO), IMAGE_ICON, 0, 0, 0));	
	m_NewStaticBt.SetIcon((HICON)LoadImage(GUIEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_NEWSTATICICO), IMAGE_ICON, 0, 0, 0));	
	m_ChangeBGBt.SetIcon((HICON)LoadImage(GUIEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_CHANGEBGICO), IMAGE_ICON, 0, 0, 0));
	m_FullScreenBt.SetIcon((HICON)LoadImage(GUIEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_FULLSCREENICO), IMAGE_ICON, 0, 0, 0));
	m_PlayBt.SetIcon((HICON)LoadImage(GUIEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_PLAYICO), IMAGE_ICON, 0, 0, 0));
	m_ItemInst.SetIcon((HICON)LoadImage(GUIEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_ITEMINSTICO), IMAGE_ICON, 0, 0, 0));
	m_InstLock.SetIcon((HICON)LoadImage(GUIEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_INSTLOCKICO), IMAGE_ICON, 0, 0, 0));
	m_NewAnimBt.SetIcon((HICON)LoadImage(GUIEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_NEWANIMATION), IMAGE_ICON, 0, 0, 0));
	m_PlayAnimBt.SetIcon((HICON)LoadImage(GUIEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_PLAYGUIANIM), IMAGE_ICON, 0, 0, 0));
	m_StopAnimBt.SetIcon((HICON)LoadImage(GUIEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_STOPGUIANIM), IMAGE_ICON, 0, 0, 0));	
	return TRUE;
}