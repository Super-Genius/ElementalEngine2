// BvhFrm.cpp : implementation file
//

#include "stdafx.h"
#include "BvhLoader.h"
#include "BvhFrm.h"


// CBvhFrm

IMPLEMENT_DYNCREATE(CBvhFrm, CMDIChildWnd)

CBvhFrm::CBvhFrm()
{
}

CBvhFrm::~CBvhFrm()
{
}


BEGIN_MESSAGE_MAP(CBvhFrm, CMDIChildWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CBvhFrm message handlers

int CBvhFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CMDIFrameWnd *mainWnd;
		
	// Register the doc templates we provide to the app
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, pApp->m_pMainWnd);

	// now tell the application to look in dll for resources
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
//	m_BvhTreeDlg.SetGripperFlags(GRIPPERFLAGS_CLOSE | GRIPPERFLAGS_DOUBLE);
	if(!m_BvhTreeDlg.Create(mainWnd,IDD_TREEVIEW,WS_CHILD | WS_VISIBLE | CBRS_RIGHT
		| CBRS_FLYBY,IDD_TREEVIEW))
	{
      TRACE0("Failed to create DlgBar\n");
      return -1;      // Fail to create.
   }

	m_BvhTreeDlg.SetBorders(2, 2, 2, 2);

	m_BvhTreeDlg.EnableDocking(CBRS_ORIENT_VERT);
	// make control bar notify us.

	//mainWnd->DockControlBar(&m_BvhTreeDlg);


	mainWnd->ShowControlBar(&m_BvhTreeDlg, FALSE, FALSE);
		
	return 0;

	EnableDocking(CBRS_ALIGN_ANY);

	//m_BvhTreeDlg.Create(IDD_TREEVIEW,this);
	BvhTreeControl = (CTreeCtrl*)m_BvhTreeDlg.GetDlgItem(IDC_TREE1);
	
	//m_BvhTreeDlg.m_BvhTreeCtrl.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER |
	//	TVS_EDITLABELS |  TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT,
	//	CRect(10,10,350,510),this,7003);
	//m_BvhTreeDlg.m_BvhTreeCtrl.SetFocus();
	//DrawHeirarchy( pDoc );

	return 0;

}


void CBvhFrm::DrawHeirarchy(  CBvhDoc * pDoc )
{
	BvhTreeControl = (CTreeCtrl*)m_BvhTreeDlg.GetDlgItem(IDC_TREE1);
	
	//clear the tree first:
	if(!BvhTreeControl->DeleteAllItems())
		return;
	USES_CONVERSION;
	char buf[1024];
	CRect pRect(0,0,0,0);
	sprintf(buf, "%s.bvh", pDoc->m_DisplayName);
	//pDC->SetBkMode(TRANSPARENT);
	//pDC->TextOut( pRect.left, pRect.top, (CString)(buf) );
	HTREEITEM hParentItem = BvhTreeControl->InsertItem(LPCSTR(buf));
	//recurse into bvh:
	CBvhObject * tbvh = pDoc->m_BvhObject;
	BvhRecurseForwardPrint(tbvh->root, &pRect, hParentItem);
	//pDC->SetBkMode(OPAQUE);
}
void CBvhFrm::BvhRecurseForwardPrint(BvhNode * bNode, CRect * pRect, HTREEITEM hParentItem)
{
	USES_CONVERSION;
	char buf[255];
	sprintf(buf, "%s", bNode->name.c_str());
	if(strlen(buf) <= 0)
		return;
	//pRect->left += HIER_WIDTH;
	//pRect->top += HIER_HEIGHT;
	//pDC->TextOut( pRect->left, pRect->top, (CString)(buf) );
	HTREEITEM IamParent = BvhTreeControl->InsertItem(LPCSTR(buf),hParentItem);
	//for all of it's children, recurse:
	for(unsigned i = 0; i < bNode->child.size(); i++)
	{
		BvhRecurseForwardPrint(bNode->child[i],pRect,IamParent);
	}
	//pRect->left -= HIER_WIDTH;
}

CBvhTreeDlg * CBvhFrm::GetBvhTreeDlg()
{
	return &m_BvhTreeDlg;
}
