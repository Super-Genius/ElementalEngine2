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


// CDatabaseToolPal dialog

//IMPLEMENT_DYNAMIC(CDatabaseToolPal, CDialog)
//CDatabaseToolPal::CDatabaseToolPal(CWnd* pParent /*=NULL*/)
//	: CDialog(CDatabaseToolPal::IDD, pParent)
IMPLEMENT_DYNAMIC(CDatabaseToolPal, CGuiDialogBar)
CDatabaseToolPal::CDatabaseToolPal()
{
}

CDatabaseToolPal::~CDatabaseToolPal()
{
}

void CDatabaseToolPal::DoDataExchange(CDataExchange* pDX)
{
	CGuiDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, ID_DATABASE_ADDROW, m_DatabaseAddRow);
	DDX_Control(pDX, ID_DATABASE_REMOVEROW, m_DatabaseDeleteRow);
}


BEGIN_MESSAGE_MAP(CDatabaseToolPal, CGuiDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
END_MESSAGE_MAP()


// CDatabaseToolPal message handlers

LRESULT CDatabaseToolPal::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
	HINSTANCE appInst;

	CGuiDialogBar::HandleInitDialog(wParam, lParam);

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init.\n");
	}
	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(DatabaseEditorDLL.hModule);

	m_DatabaseAddRow.ModifyStyle(0, BS_OWNERDRAW);
	m_DatabaseAddRow.LoadBitmaps(IDB_DATABASE_LINEADD_UP, IDB_DATABASE_LINEADD_DOWN, IDB_DATABASE_LINEADD_SEL, IDB_DATABASE_LINEADD_DISABLED);

	m_DatabaseDeleteRow.ModifyStyle(0, BS_OWNERDRAW);
	m_DatabaseDeleteRow.LoadBitmaps(IDB_DATABASE_LINEDEL_UP, IDB_DATABASE_LINEDEL_DOWN, IDB_DATABASE_LINEDEL_SEL, IDB_DATABASE_LINEDEL_DISABLED);

	AfxSetResourceHandle(appInst);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
