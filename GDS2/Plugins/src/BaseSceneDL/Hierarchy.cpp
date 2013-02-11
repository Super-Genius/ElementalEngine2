// Hierarchy.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern AFX_EXTENSION_MODULE BaseSceneDLDLL;

static UINT ObjectDropMessage = RegisterWindowMessage(OBJECT_DROP_MSG_STR);

/////////////////////////////////////////////////////////////////////////////
// CHierarchy dialog


CHierarchy::CHierarchy(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CHierarchy::IDD)
{
	//{{AFX_DATA_INIT(CHierarchy)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}


void CHierarchy::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHierarchy)
	DDX_Control(pDX, IDC_HIERARCHY, m_HierarchyControl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHierarchy, CPropertyPage)
	//{{AFX_MSG_MAP(CHierarchy)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(ObjectDropMessage, OnDropMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHierarchy message handlers

void CHierarchy::OnSize(UINT nType, int cx, int cy) 
{
	CRect rect;

	CPropertyPage::OnSize(nType, cx, cy);

	GetClientRect(rect);
	if (m_HierarchyControl.m_hWnd != NULL)
	{
		m_HierarchyControl.MoveWindow(rect);
	}
}

//---------------------------------------------------------------------
// Function:	RemoveHierarchyItem
// Description:	Removes a hierarchy item from the CTreeControl
// Parameters:	child = pointer to hierarchy info of object.
// Returns:		TRUE, if successful, otherwise FALSE
//---------------------------------------------------------------------
BOOL CHierarchy::RemoveHierarchyItem(HIERARCHYINFO *child)
{
	// create the item
	return m_HierarchyControl.DeleteItem(child->hItem);
}

//---------------------------------------------------------------------
// Function:	AddHierarchyItem
// Description:	Fills in the CTreeControl with the hierarchy data
// Parameters:	parent = pointer to hierarchy information of parent
//						or NULL, for root
//				child = pointer to hierarchy info of object.
// Returns:		TRUE, if successful, otherwise FALSE
//---------------------------------------------------------------------
BOOL CHierarchy::AddHierarchyItem(HIERARCHYINFO *parent, HIERARCHYINFO *child)
{
	HTREEITEM hParent;
	TVINSERTSTRUCT tvInsert;

	if (parent == NULL)
	{
		hParent = TVI_ROOT;
	}
	else
	{
		hParent = parent->hItem;
	}

	tvInsert.hParent = hParent;
	tvInsert.hInsertAfter = TVI_LAST;
	tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE;
	tvInsert.item.iImage = child->image;
	tvInsert.item.iSelectedImage = child->image;
	tvInsert.item.pszText = (LPSTR)child->label;
	tvInsert.item.lParam = (LPARAM)child;

	// create the item
	child->hItem = m_HierarchyControl.InsertItem(&tvInsert);

	if (child->enabled)
	{
		m_HierarchyControl.SetCheck(child->hItem, TRUE);
	}
	else
	{
		m_HierarchyControl.SetCheck(child->hItem, FALSE);
	}

	return TRUE;
}

void CHierarchy::ClearHierarchy()
{
	m_HierarchyControl.DeleteAllItems();
}


//---------------------------------------------------------------------
// Function:	OnInitDialog
// Description:	Come here when Initializing the dialog so we can
//				load in the Image lists for the hierarchy
// Parameters:	.
// Returns:		TRUE, unless you change focus
//---------------------------------------------------------------------
BOOL CHierarchy::OnInitDialog() 
{
	HINSTANCE appInst;

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(BaseSceneDLDLL.hModule);

	CPropertyPage::OnInitDialog();

//	MakeImageList(IDB_TREE_ICONS, m_Icons, GetDC()->GetBkColor());
	// we need to load images here.
//	m_HierarchyControl.SetImageList(&m_Icons, TVSIL_NORMAL);

	m_HierarchyControl.SetParent(this);
	AfxSetResourceHandle(appInst);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//---------------------------------------------------------------------
// Function:	OnDropMessage
// Description:	Come here when some CBaseObject is dropped on us
// Parameters:	wParam = 0
//				lParam = DROPOBJECT pointer.
// Returns:		TRUE
//---------------------------------------------------------------------
LRESULT CHierarchy::OnDropMessage(WPARAM wParam, LPARAM lParam)
{
	DROPOBJECTINFO *doInfo;
	TCHAR tBuff[256];
	const GUID *theGuid;

	doInfo = (DROPOBJECTINFO *)lParam;

	theGuid = doInfo->object->GetType();

	CSceneDLDockWin *parent;
	parent = DYNAMIC_DOWNCAST(CSceneDLDockWin, GetParent());

//	parent->InsertObject(doInfo->object

	wsprintf(tBuff, "Drag and drop of type GUID:\n"
		"{ 0x%.8x, 0x%.4x, 0x%.4x, { 0x%.2x, 0x%.2x,\n"
		"0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x }\n", theGuid->Data1, theGuid->Data2, theGuid->Data3, 
		theGuid->Data4[0], theGuid->Data4[1], theGuid->Data4[2], theGuid->Data4[3],
		theGuid->Data4[4], theGuid->Data4[5], theGuid->Data4[6], theGuid->Data4[7]);

	MessageBox(tBuff, _T("Drag Message!"));
	return TRUE;
}

