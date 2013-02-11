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

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlEx

CTreeCtrlEx::CTreeCtrlEx()
{
	m_fDragging = FALSE;
}

CTreeCtrlEx::~CTreeCtrlEx()
{
}

static UINT ResourceDropMessage = RegisterWindowMessage(RESOURCE_DROP_MSG_STR);

BEGIN_MESSAGE_MAP(CTreeCtrlEx, CTreeCtrl)
	//{{AFX_MSG_MAP(CTreeCtrlEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	//ON_REGISTERED_MESSAGE(ResourceDropMessage, OnDropMessage)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnTvnBegindrag)
	ON_NOTIFY_REFLECT(TVN_BEGINRDRAG, OnTvnBegindrag)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlEx message handlers

LRESULT CTreeCtrlEx::OnDropMessage(WPARAM wParam, LPARAM lParam)
{
	return GetParent()->SendMessage(ResourceDropMessage, wParam, lParam);
}

BOOL CTreeCtrlEx::SetCheck(HTREEITEM hItem, BOOL fCheck )
{
	ASSERT(::IsWindow(m_hWnd));
	TVITEM tvitem;
	tvitem.mask = TVIF_HANDLE | TVIF_STATE;
	tvitem.hItem = hItem;
	tvitem.stateMask = TVIS_STATEIMAGEMASK;
	tvitem.state = INDEXTOSTATEIMAGEMASK( (fCheck ? 1 : 0) );

	return (BOOL)(SendMessage(TVM_SETITEM, 0, (LPARAM)&tvitem));
}

BOOL CTreeCtrlEx::PreTranslateMessage(MSG* pMsg)
{
	// If edit control is visible in tree view control, when you send a
	// WM_KEYDOWN message to the edit control it will dismiss the edit
	// control. When the ENTER key was sent to the edit control, the
	// parent window of the tree view control is responsible for updating
	// the item's label in TVN_ENDLABELEDIT notification code.
	if (pMsg->message == WM_KEYDOWN &&
        (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
	{
		CEdit* edit = GetEditControl();
		if (edit)
		{
			edit->SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
	}    
	return CTreeCtrl::PreTranslateMessage(pMsg);
}

void CTreeCtrlEx::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_fDragging)
		return;

	HWND hwndTV = m_hWnd;
	LPNMTREEVIEW lpnmtv = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HIMAGELIST himl;    // handle to image list 
	RECT rcItem;        // bounding rectangle of item 
	DWORD dwLevel;      // heading level of item 
	DWORD dwIndent;     // amount that child items are indented 

	// Tell the tree-view control to create an image to use 
	// for dragging. 
	himl = TreeView_CreateDragImage(hwndTV, lpnmtv->itemNew.hItem); 

	// Get the bounding rectangle of the item being dragged. 
	TreeView_GetItemRect(hwndTV, lpnmtv->itemNew.hItem, &rcItem, TRUE); 

	// Get the heading level and the amount that the child items are 
	// indented. 
	dwLevel = lpnmtv->itemNew.lParam; 
	dwIndent = (DWORD)SendMessage(TVM_GETINDENT, 0, 0); 

	// Start the drag operation. 
	ImageList_BeginDrag(himl, 0, 0, 0);
	ImageList_DragEnter(hwndTV, 50, 50);  

	// Hide the mouse pointer, and direct mouse input to the 
	// parent window. 
//	ShowCursor(FALSE);
	GetParent()->SetCapture();
	m_fDragging = TRUE;
	m_DragItem = lpnmtv->itemNew.hItem;

	*pResult = 0;
}
