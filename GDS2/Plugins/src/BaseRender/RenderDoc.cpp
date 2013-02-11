// RenderDoc.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRenderDoc

IMPLEMENT_DYNCREATE(CRenderDoc, CDocument)

CRenderDoc::CRenderDoc()
{
}

BOOL CRenderDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CRenderDoc::~CRenderDoc()
{
}


BEGIN_MESSAGE_MAP(CRenderDoc, CDocument)
	//{{AFX_MSG_MAP(CRenderDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRenderDoc diagnostics

#ifdef _DEBUG
void CRenderDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRenderDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRenderDoc serialization

void CRenderDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRenderDoc commands

BOOL CRenderDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	
	return TRUE;
}
