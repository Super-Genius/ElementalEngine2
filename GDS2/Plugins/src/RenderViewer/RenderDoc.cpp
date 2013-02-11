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
#include "RenderViewer.h"
#include "RenderDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CRenderDoc

IMPLEMENT_DYNCREATE(CRenderDoc, CDocument)

CRenderDoc::CRenderDoc()
{
}

BOOL CRenderDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
	{
		return FALSE;
	}
	// set correct view title
	CString sTitle;
	sTitle.LoadString(IDS_VIEW_TITLE);
	CDocument::SetTitle(sTitle);
	return TRUE;
}

CRenderDoc::~CRenderDoc()
{
}


BEGIN_MESSAGE_MAP(CRenderDoc, CDocument)
END_MESSAGE_MAP()


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

BOOL CRenderDoc::CanCloseFrame(CFrameWnd* pFrame)
{
	// this stops world view from closing
	// but this does not prevent from closing GDS
	return FALSE;
}

// CRenderDoc commands
