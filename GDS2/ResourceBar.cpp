///============================================================================
/// \file		ResourceBar.cpp
/// \brief		Implementation file for the Resource Bar
/// \date		05-20-2005
/// \author		Josh Lulewicz
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

// ResourceBar.cpp : implementation file
//

#include "stdafx.h"
#include ".\resourcebar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// User-defined predicate function for sorting.
// this has to be defined locally or compiler put's it in whoever
// includes the header file.  Make sure both functions match.
struct lessThan : public greater<RESOURCECOLLECTION *>
{
    bool operator()(const RESOURCECOLLECTION* x, const RESOURCECOLLECTION* y) const
    { 
		return (x->priority < y->priority); 
    };

};

/////////////////////////////////////////////////////////////////////////////
// CResourceBar

CResourceBar::CResourceBar()
{
	obOptions = NULL;
	m_sizeMinFloating=m_sizeVert=m_sizeHorz=CSize(200,150);

	// set auto delete bool for the resource doc to false
	// this is done to prevent this document from getting
	// delete() called upon it, as it is not allocated
	m_ResourceDoc.m_bAutoDelete = FALSE;
}

CResourceBar::~CResourceBar()
{
	if (obOptions != NULL)
	{
		delete obOptions;
		obOptions = NULL;
	}
}

BEGIN_MESSAGE_MAP(CResourceBar, CGuiControlBar)
	//{{AFX_MSG_MAP(CResourceBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceBar message handlers
int CResourceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// what is this for?
	obOptions = new CResourceOptions;

	CCreateContext ccc1;
    ccc1.m_pNewViewClass   = RUNTIME_CLASS(CResourceFolderView);
    ccc1.m_pCurrentDoc     = NULL;
    ccc1.m_pNewDocTemplate = NULL;
    ccc1.m_pLastView       = NULL;
    ccc1.m_pCurrentFrame   = NULL;

	CCreateContext ccc2;
    ccc2.m_pNewViewClass   = RUNTIME_CLASS(CResourceView);
    ccc2.m_pCurrentDoc     = NULL;
    ccc2.m_pNewDocTemplate = NULL;
    ccc2.m_pLastView       = NULL;
    ccc2.m_pCurrentFrame   = NULL;

	if ( !m_SplitterWnd.CreateStatic(this, 1, 2) )
		return -1;
	if ( !m_SplitterWnd.CreateView(0, 0, RUNTIME_CLASS(CResourceFolderView), CSize(128,0), &ccc1) )
		return -1;
	if ( !m_SplitterWnd.CreateView(0, 1, RUNTIME_CLASS(CResourceView), CSize(0,0), &ccc2) )
		return -1;

	m_ResourceDoc.AddView( DYNAMIC_DOWNCAST( CView, m_SplitterWnd.GetPane(0, 0) ) );
	m_ResourceDoc.AddView( DYNAMIC_DOWNCAST( CView, m_SplitterWnd.GetPane(0, 1) ) );

	SetBorders(4, 4, 6, 4);
	return 0;
}

void CResourceBar::OnSize(UINT nType, int cx, int cy) 
{
	CGuiControlBar::OnSize(nType, cx, cy);
}