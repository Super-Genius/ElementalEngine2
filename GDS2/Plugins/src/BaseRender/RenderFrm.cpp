// RenderFrm.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRenderFrm

IMPLEMENT_DYNCREATE(CRenderFrm, CMDIChildWnd)

CRenderFrm::CRenderFrm()
{
}

CRenderFrm::~CRenderFrm()
{
}


BEGIN_MESSAGE_MAP(CRenderFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CRenderFrm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRenderFrm message handlers

BOOL CRenderFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
    // only works if it's visible
    cs.style |= (WS_MAXIMIZE | WS_VISIBLE);
	return CMDIChildWnd::PreCreateWindow(cs);
}
