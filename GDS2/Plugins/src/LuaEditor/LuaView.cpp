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
// CLuaView

IMPLEMENT_DYNCREATE(CLuaView, CView)

BEGIN_MESSAGE_MAP(CLuaView, CView)
	//{{AFX_MSG_MAP(CLuaView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_LUA_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_LUA_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_LUA_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_LUA_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_LUA_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_LUA_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_LUA_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_LUA_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_LUA_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_LUA_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_LUA_EDIT_SELECTALL, OnEditSelectAll)
	ON_COMMAND(ID_LUA_EDIT_DELETE, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_LUA_EDIT_DELETE, OnUpdateEditClear)
	ON_COMMAND(ID_GDS_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_GDS_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_GDS_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_GDS_EDIT_REDO, OnUpdateEditRedo)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_LUA_EDIT_TOGGLEBREAKPOINT, OnLuaEditTogglebreakpoint)
	ON_UPDATE_COMMAND_UI(ID_LUA_EDIT_TOGGLEBREAKPOINT, OnUpdateLuaEditTogglebreakpoint)
//	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLuaView construction/destruction

CLuaView::CLuaView()
{
	m_bIsActiveView = false;
}

CLuaView::~CLuaView()
{
}

BOOL CLuaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLuaView drawing

void CLuaView::OnDraw(CDC* pDC)
{
	CLuaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CLuaView printing

BOOL CLuaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	if ( m_LuaEditor.CanCutOrClear() )
	{
		CPrintDialog *pDlg = pInfo->m_pPD;
		pDlg->m_pd.Flags &= (~PD_NOSELECTION);
	}

	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLuaView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_LuaEditor.PreparePrint(pDC, pInfo);
}


void CLuaView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	m_LuaEditor.PrintPage(pDC, pInfo);	
}

void CLuaView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_LuaEditor.EndPrint(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CLuaView diagnostics

#ifdef _DEBUG
void CLuaView::AssertValid() const
{
	CView::AssertValid();
}

void CLuaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLuaDoc* CLuaView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLuaDoc)));
	return (CLuaDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLuaView message handlers

int CLuaView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if ( !m_LuaEditor.Create(this, ID_LUA_EDIT_CTRL) )
		return -1;
	
	m_LuaEditor.SetEditorMargins();
	m_LuaEditor.SetLuaLexer();

	return 0;
}

void CLuaView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if ( ::IsWindow(m_LuaEditor.m_hWnd) )
		m_LuaEditor.SetWindowPos(NULL, 0, 0, cx, cy, 0);
}

BOOL CLuaView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR *lpnmhdr = (LPNMHDR) lParam; 

	if (lpnmhdr->hwndFrom == m_LuaEditor.m_hWnd)
	{
		*pResult = OnSci((SCNotification*)lParam);
		return TRUE;
	}
	
	return CView::OnNotify(wParam, lParam, pResult);
}

int CLuaView::OnSci(SCNotification* pNotify)
{
	CPoint pt;
	int nLine;
	switch (pNotify->nmhdr.code)
	{
		case SCN_MARGINCLICK:
			GetCursorPos(&pt);
			ScreenToClient(&pt);
			nLine = m_LuaEditor.LineFromPoint(pt);
			ToggleBreakPoint(nLine);
		break;

		case SCI_SETSAVEPOINT:
		case SCN_SAVEPOINTREACHED:
			GetDocument()->SetModifiedFlag(FALSE);
		break;

		case SCN_SAVEPOINTLEFT:
			GetDocument()->SetModifiedFlag(TRUE);
		break;
	}

	return TRUE;
}

void CLuaView::OnEditCopy() 
{
	// TODO: Add your command handler code here
	m_LuaEditor.Copy();
}

void CLuaView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_LuaEditor.CanCutOrClear());	
}

void CLuaView::OnEditCut() 
{
	m_LuaEditor.Cut();
}

void CLuaView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_LuaEditor.CanCutOrClear());	
}

void CLuaView::OnEditClear() 
{
	m_LuaEditor.Clear();
}

void CLuaView::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_LuaEditor.CanCutOrClear());	
}

void CLuaView::OnEditPaste() 
{
	m_LuaEditor.Paste();
}

void CLuaView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_LuaEditor.CanPaste());
}

void CLuaView::OnEditUndo() 
{
	m_LuaEditor.Undo();
}

void CLuaView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_LuaEditor.CanUndo());
}

void CLuaView::OnEditRedo() 
{
	m_LuaEditor.Redo();
}

void CLuaView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_LuaEditor.CanRedo());	
}

void CLuaView::OnEditSelectAll() 
{
	m_LuaEditor.SelectAll();	
}

/*
void CLuaView::Activate()
{
	CFrameWnd* pFrame = GetParentFrame();

	if (pFrame != NULL)
		pFrame->ActivateFrame();
	else
		TRACE0("Error: Can not find a frame for document to activate.\n");

	CFrameWnd* pAppFrame;
	if (pFrame != (pAppFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd))
	{
		ASSERT_KINDOF(CFrameWnd, pAppFrame);
		pAppFrame->ActivateFrame();
	}

	GetEditor()->GrabFocus();
}
*/
/*
void CLuaView::CloseFrame()
{
	CFrameWnd* pFrame = GetParentFrame();

	if (pFrame != NULL)
		pFrame->SendMessage(WM_CLOSE);
}
*/

void CLuaView::ToggleBreakPoint(int nLine)
{
	if ( m_LuaEditor.ToggleBreakpoint(nLine) )
	{
		GetDocument()->AddBreakPoint(nLine);
	}
	else
	{
		GetDocument()->RemoveBreakPoint(nLine);
	}
}

void CLuaView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	CLuaFrame *parent = DYNAMIC_DOWNCAST(CLuaFrame, this->GetParentFrame());
	// if we are already active (or deactive), don't hide/unhide the bars, 
	// because they are already visible, also if docking we will get 
	// this message in the middle of the dock routine, which causes an assert.
	if (m_bIsActiveView == (FALSE != bActivate))
	{
		AfxSetResourceHandle(parent->m_AppInst);
		return;
	}
	
	// set new state (active or deactive)
	m_bIsActiveView = (FALSE != bActivate);

	CGuiMDIFrame *mainWnd = DYNAMIC_DOWNCAST(CGuiMDIFrame, AfxGetMainWnd());

	if (bActivate)
	{
		SetActiveScene();
		mainWnd->LoadDocumentBars(_T("LuaEditor"));
	}
	else
	{
		mainWnd->SaveDocumentBars(_T("LuaEditor"));
	}

}

void CLuaView::OnLuaEditTogglebreakpoint()
{
	// TODO: Add your command handler code here
	ToggleBreakPoint(m_LuaEditor.GetCurrentLine());
}

void CLuaView::OnUpdateLuaEditTogglebreakpoint(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
}