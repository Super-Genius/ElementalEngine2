///============================================================================
/// \file		SplineComponent.cpp
/// \brief		Implementation file for Spline Document
/// \date		11-14-2006
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
// CSplineDoc

IMPLEMENT_DYNCREATE(CSplineDoc, CDocument)

CSplineDoc::CSplineDoc()
{
}

BOOL CSplineDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSplineDoc::~CSplineDoc()
{
}


BEGIN_MESSAGE_MAP(CSplineDoc, CDocument)
	//{{AFX_MSG_MAP(CSplineDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplineDoc diagnostics

#ifdef _DEBUG
void CSplineDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSplineDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// load an example document
DWORD CSplineDoc::LoadDocument(/* IArchive &ar */)
{
	return NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////
// CSplineDoc commands
