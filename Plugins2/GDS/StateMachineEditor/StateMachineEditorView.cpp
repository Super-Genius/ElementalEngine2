///============================================================================
/// \file		StateMachineEditorView.cpp
/// \brief		Class implementation for CStateMachineEditorView
/// \date		03-06-2007
/// \author		Richard Nguyen
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
// CStateMachineEditorView

IMPLEMENT_DYNCREATE(CStateMachineEditorView, CView)

CStateMachineEditorView::CStateMachineEditorView()
{
	m_ToolBox = EngineGetToolBox();
	m_bActive = false;
}

CStateMachineEditorView::~CStateMachineEditorView()
{
}

BEGIN_MESSAGE_MAP(CStateMachineEditorView, CView)
	//{{AFX_MSG_MAP(CStateMachineEditorView)
	ON_COMMAND(ID_STATE_BUTTON, OnStateButton)
	ON_COMMAND(ID_EVENT_BUTTON, OnEventButton)
	ON_COMMAND(ID_ENTRY_EVENT_BUTTON, OnEntryEventButton)
	ON_COMMAND(ID_UPDATE_EVENT_BUTTON, OnUpdateEventButton)
	ON_COMMAND(ID_EXIT_EVENT_BUTTON, OnExitEventButton)
	ON_COMMAND(ID_TRANSITION_MODE, OnTransitionMode)
	ON_COMMAND(ID_VALIDATE, OnValidate)
	ON_COMMAND(ID_FLAT_MODE, OnFlatMode)
	ON_UPDATE_COMMAND_UI(ID_STATE_BUTTON, OnUpdateStateButton)
	ON_UPDATE_COMMAND_UI(ID_EVENT_BUTTON, OnUpdateEventButton)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_EVENT_BUTTON, OnUpdateEventButton)
	ON_UPDATE_COMMAND_UI(ID_UPDATE_EVENT_BUTTON, OnUpdateEventButton)
	ON_UPDATE_COMMAND_UI(ID_EXIT_EVENT_BUTTON, OnUpdateEventButton)
	ON_UPDATE_COMMAND_UI(ID_TRANSITION_MODE, OnUpdateTransitionButton)
	ON_UPDATE_COMMAND_UI(ID_VALIDATE, OnUpdateValidateButton)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStateMachineEditorView message handlers

void CStateMachineEditorView::OnStateButton() 
{
	EndDrawingTransitionMode();
	m_DiagramEditor.AddNewState();
}

void CStateMachineEditorView::OnEventButton() 
{
	ASSERT( CanAddEvent() );
	EndDrawingTransitionMode();
	m_DiagramEditor.AddNewEvent();
}

void CStateMachineEditorView::OnEntryEventButton()
{
	ASSERT( CanAddEvent() );
	EndDrawingTransitionMode();
	m_DiagramEditor.AddEntryEvent();
}

void CStateMachineEditorView::OnUpdateEventButton()
{
	ASSERT( CanAddEvent() );
	EndDrawingTransitionMode();
	m_DiagramEditor.AddUpdateEvent();
}

void CStateMachineEditorView::OnExitEventButton()
{
	ASSERT( CanAddEvent() );
	EndDrawingTransitionMode();
	m_DiagramEditor.AddExitEvent();
}

void CStateMachineEditorView::OnTransitionMode()
{
	CStateMachineEditor *pEditor = SINGLETONINSTANCE( CStateMachineEditor );
	bool bTransitionMode = pEditor->GetToolBar()->IsTransitionMode();
	m_DiagramEditor.SetTransitionDrawing( bTransitionMode );
}

void CStateMachineEditorView::OnValidate()
{
	ASSERT( !m_DiagramEditor.IsValid() );
	m_DiagramEditor.Validate();
}

void CStateMachineEditorView::OnFlatMode()
{
	CStateMachineEditor *pEditor = SINGLETONINSTANCE( CStateMachineEditor );
	bool bFlatMode = pEditor->GetToolBar()->IsFlatMode();
	m_DiagramEditor.SetFlatMode( bFlatMode );
}

void CStateMachineEditorView::OnUpdateStateButton( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( !m_DiagramEditor.IsFlatMode() );
}

void CStateMachineEditorView::OnUpdateEventButton( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( CanAddEvent() );
}

void CStateMachineEditorView::OnUpdateTransitionButton(CCmdUI* pCmdUI)
{
	CDiagramEntity *layerEntity = m_DiagramEditor.GetCurrentLayerEntity();
	if( layerEntity == NULL || m_DiagramEditor.IsFlatMode() )
	{
		// we cannot have events on the root level
		pCmdUI->Enable( FALSE );
	}
	else
	{
		// check if event objects exist for current state
		CHashString hszCQHState = _T("CQHState");
		CHashString hszName = layerEntity->GetName();
		GETSETSTATEMACHINENAMESPARAMS gssmn;
		static DWORD msgHash_GetStateEventLabels = CHashString( _T("GetStateEventLabels") ).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_GetStateEventLabels, sizeof( gssmn ), &gssmn, &hszName, &hszCQHState );
		pCmdUI->Enable( !gssmn.Names.empty() );
	}
}

void CStateMachineEditorView::OnUpdateValidateButton( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(	!m_DiagramEditor.IsValid() );
}

BOOL CStateMachineEditorView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if( IsWindow( m_DiagramEditor ) &&
		m_DiagramEditor.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) )
	{
		return TRUE;
	}
	return CView::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

void CStateMachineEditorView::EndDrawingTransitionMode()
{
	CStateMachineEditor *pEditor = SINGLETONINSTANCE( CStateMachineEditor );
	pEditor->GetToolBar()->SetTransitionMode( false );
	m_DiagramEditor.SetTransitionDrawing( false );
}

bool CStateMachineEditorView::CanAddEvent()
{
	return( m_DiagramEditor.GetCurrentLayerEntity() != NULL &&
		   !m_DiagramEditor.IsFlatMode() );
}

/////////////////////////////////////////////////////////////////////////////
// CStateMachineEditorView drawing

void CStateMachineEditorView::OnDraw(CDC* pDC)
{
}

void CStateMachineEditorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	RegisterViewUndoStack( this, GetStateMachineName() );

	SINGLETONINSTANCE( CStateMachineEditor )->ShowToolBar( true );

	CRect windowRect;
	GetClientRect( windowRect );
	const DWORD style = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER;
	m_DiagramEditor.Create( style, windowRect, this );

	CStateMachineEditorDoc *pDocument;
	pDocument = static_cast<CStateMachineEditorDoc *>( GetDocument() );
	m_DiagramEditor.SetDocument( pDocument );
	m_DiagramEditor.SetStateMachineName( GetStateMachineName() );

	m_DiagramEditor.RefreshHierarchy();

	CStateMachineEditor *pEditor = SINGLETONINSTANCE( CStateMachineEditor );
	CStateMachineToolBar *pToolBar = pEditor->GetToolBar();
	pToolBar->SetTransitionMode( m_DiagramEditor.IsTransitionDrawing() );
	pToolBar->SetFlatMode( m_DiagramEditor.IsFlatMode() );
}

void CStateMachineEditorView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CView::OnActivateView( bActivate, pActivateView, pDeactiveView );

	if( m_bActive != ( FALSE != bActivate ) )
	{
		m_bActive = ( bActivate != FALSE );

		CGuiMDIFrame *mainWnd = DYNAMIC_DOWNCAST( CGuiMDIFrame, AfxGetMainWnd() );
		if( bActivate )
		{
			SetActiveScene( GetStateMachineName() );
			CStateMachineEditor *pEditor = SINGLETONINSTANCE( CStateMachineEditor );
			CStateMachineToolBar *pToolBar = pEditor->GetToolBar();
			pToolBar->SetTransitionMode( m_DiagramEditor.IsTransitionDrawing() );
			pToolBar->SetFlatMode( m_DiagramEditor.IsFlatMode() );
			mainWnd->LoadDocumentBars( _T("StateMachineEditor") );
		}
		else
		{
			mainWnd->SaveDocumentBars( _T("StateMachineEditor") );
		}
	}
}

void CStateMachineEditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	if( m_DiagramEditor.m_hWnd )
	{
		m_DiagramEditor.SetWindowPos( NULL, 0, 0, cx, cy, 0 );
	}
}

IHashString *CStateMachineEditorView::GetStateMachineName()
{
	CStateMachineEditorDoc *pDocument;
	pDocument = static_cast<CStateMachineEditorDoc *>( GetDocument() );
	ASSERT( pDocument != NULL );
	return pDocument->GetStateMachineName();
}
