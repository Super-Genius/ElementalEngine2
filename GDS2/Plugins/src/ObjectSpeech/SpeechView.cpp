// SpeechView.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpeechView

IMPLEMENT_DYNCREATE(CSpeechView, CView)

CSpeechView::CSpeechView()
{
}

CSpeechView::~CSpeechView()
{
}


BEGIN_MESSAGE_MAP(CSpeechView, CView)
	//{{AFX_MSG_MAP(CSpeechView)
//	ON_UPDATE_COMMAND_UI(ID_Speech_TEST1, OnUpdateSpeechTest1)
//	ON_UPDATE_COMMAND_UI(ID_Speech_TEST2, OnUpdateSpeechTest2)
//	ON_UPDATE_COMMAND_UI(ID_Speech_TEST3, OnUpdateSpeechTest3)
//	ON_COMMAND(ID_Speech_TEST1, OnSpeechTest1)
//	ON_COMMAND(ID_Speech_TEST2, OnSpeechTest2)
//	ON_COMMAND(ID_Speech_TEST3, OnSpeechTest3)
	//}}AFX_MSG_MAP
//	ON_WM_PAINT()
//ON_WM_UPDATEUISTATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpeechView drawing

void CSpeechView::OnDraw(CDC* pDC)
{
	CSpeechDoc * pDoc = DYNAMIC_DOWNCAST(CSpeechDoc, GetDocument());
	ASSERT_VALID(pDoc);
	// Save DC
	pDC->SaveDC();

	// Fill Screen with Blue
	CBrush brush( RGB( 150, 200, 230 ) );
	CRect rect;
	GetClientRect( &rect );
	FillRect( *pDC, &rect, brush );
	pDC->MoveTo( rect.left, rect.bottom/2 );

	// Draw White Wav Graph
	DrawWave( pDC, &rect, pDoc );

	
    
	// Draw 0-Value Line
	pDC->MoveTo( rect.left, rect.bottom/2 );
	pDC->LineTo( rect.right, rect.bottom/2 );

	// Draw Event List
	DrawEvents( pDC, &rect, pDoc );

	// Restore DC
	pDC->RestoreDC(-1);
}

/////////////////////////////////////////////////////////////////////////////
// CSpeechView diagnostics

#ifdef _DEBUG
void CSpeechView::AssertValid() const
{
	CView::AssertValid();
}

void CSpeechView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSpeechView message handlers

//void CSpeechView::OnUpdateSpeechTest1(CCmdUI* pCmdUI) 
//{
//	pCmdUI->Enable();
//}

//void CSpeechView::OnUpdateSpeechTest2(CCmdUI* pCmdUI) 
//{
//	pCmdUI->Enable();	
//}

//void CSpeechView::OnUpdateSpeechTest3(CCmdUI* pCmdUI) 
//{
//	pCmdUI->Enable();	
//}

//void CSpeechView::OnSpeechTest1() 
//{
//	MessageBox(_T("Test Menu Item 1 was selected.\n"), _T("Speech"));
//}

//void CSpeechView::OnSpeechTest2() 
//{
//	MessageBox(_T("Test Menu Item 2 was selected.\n"), _T("Speech"));	
//}

//void CSpeechView::OnSpeechTest3() 
//{
//	MessageBox(_T("Test Menu Item 3 was selected.\n"), _T("Speech"));
//}

void CSpeechView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CWinApp* pApp;
	CMDIFrameWnd *mainWnd;
	CSpeechFrm *parent;

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, pApp->m_pMainWnd);

	parent = DYNAMIC_DOWNCAST(CSpeechFrm, this->GetParentFrame());

	// don't use is window visible.
	if (parent->m_SpeechToolPal.IsVisible() != bActivate)
	{
		mainWnd->ShowControlBar(&parent->m_SpeechToolPal, bActivate, TRUE);
		if (bActivate)
		{
			AfxSetResourceHandle(ObjectSpeechDLL.hModule);
		}
		else
		{
			AfxSetResourceHandle(parent->m_AppInst);
		}
	}
	
}

//-----------------------------
// Function:	DrawWave
// Description:	Draws the wave in the current MFC context
// Parameters:	CDC *pDC, RECT *pRect
// Returns:		.
//-----------------------------
void CSpeechView::DrawWave( CDC * pDC, CRect * pRect, CSpeechDoc * pDoc )
{
	pDoc->m_cWave.DrawData( pDC, pRect );
}

//------------------------------------
// Function:	GetRatio	
// Description:	Determines where the event is with respect to the window
// Parameters:	CSpeechDoc * pDoc = the document; int i = Event we are looking at
// Returns:		Float Ratio = The ratio to be multiplied by the width of window
//------------------------------------
float CSpeechView::GetRatio( CSpeechDoc * pDoc, int i )
{
	float temp1 = (float)pDoc->m_vEventList[i]->ullAudioStreamOffset;
	float temp2 = (float)pDoc->m_cWave.GetDataSize();
	return(temp1/temp2);
}

//---------------------------------------------------------------------
// Function:	DrawEvents
// Description:	Draws the Events on the current MFC context
// Parameters:	CDC *pDC, RECT *pRect
// Returns:		.
//---------------------------------------------------------------------
void CSpeechView::DrawEvents( CDC * pDC, CRect * pRect, CSpeechDoc * pDoc )
{
	pDC->SaveDC();
	USES_CONVERSION;
	char temp[255] = "\0";
	DWORD i;
	int j;

	itoa(pDoc->m_vEventList.size(), temp, 10);
	pDC->TextOut( pRect->left, pRect->top, (CString)(temp) );

	pDC->SetTextAlign(TA_CENTER);
	j = 1;
	for( i = 0; i < pDoc->m_vEventList.size(); i++ )
	{
		pDC->TextOut( (int)GetRatio(pDoc, i)*pRect->right, pRect->bottom/2+50*j, pDoc->GrabEventString(pDoc->m_vEventList[i]->eEventId) );
		if( pDoc->m_vEventList[i]->eEventId == SPEI_VISEME )
			pDC->TextOut( (int)GetRatio(pDoc, i)*pRect->right, pRect->bottom/2+100*j, pDoc->GrabVisemeString((SPVISEMES)LOWORD(pDoc->m_vEventList[i]->lParam)) );
		j = -j;
	}
	pDC->RestoreDC(-1);
}


//void CSpeechView::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: Add your message handler code here
//	// Do not call CView::OnPaint() for painting messages
//}

//void CSpeechView::OnUpdateUIState(UINT /*nAction*/, UINT /*nUIElement*/)
//{
//	// This feature requires Windows 2000 or greater.
//	// The symbols _WIN32_WINNT and WINVER must be >= 0x0500.
//	// TODO: Add your message handler code here
//}