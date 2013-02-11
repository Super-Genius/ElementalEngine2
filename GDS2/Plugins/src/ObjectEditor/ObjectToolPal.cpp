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
#include <assert.h>
#include "ObjectToolPal.h"
// CObjectToolPal dialog

IMPLEMENT_DYNAMIC(CObjectToolPal, CGuiDialogBar)

CObjectToolPal::CObjectToolPal()
{
	m_fSnapToGrid = 20.0f;

	m_iLockPosIndex = 0;
	m_iLockRotIndex = 0;
	m_fLockValue = 0.0f;
	m_bButtonRotateClicked = false;
	m_bButtonMoveClicked = false;
	m_iLockIndex = m_iLockPosIndex;
	m_iHugIndex = 0;
	m_ObjectManipulateState = MANIPULATE_SELECT;

	m_pEditor = SINGLETONINSTANCE(CObjectEditor);
}

CObjectToolPal::~CObjectToolPal()
{
}

void CObjectToolPal::DoDataExchange(CDataExchange* pDX)
{
	CGuiDialogBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_OBJECT_SELECT, m_ObjectSelect );
	DDX_Control(pDX, IDC_OBJECT_MOVE, m_ObjectMove );
	DDX_Control(pDX, IDC_OBJECT_ROTATE, m_ObjectRotate );
	DDX_Control(pDX, IDC_OBJECT_SCALE, m_ObjectScale );
	DDX_Radio(pDX, IDC_OBJECT_SELECT, m_ObjectManipulateState );

	DDX_Text(pDX, IDC_EDIT_SENSITIVITY, m_fSensitivity);
	DDX_CBIndex(pDX, IDC_LOCK, m_iLockIndex);
	DDX_Control(pDX, IDC_LOCK, m_LockMovement);
	DDX_Control(pDX, IDC_LOCKVALUE, m_LockValue );
	DDX_Control(pDX, IDC_LOCKVALUE_CHECK, m_LockValueCheck );

	DDX_Control(pDX, IDC_OBJECT_CREATE, m_ObjectCreate);
	DDX_Control(pDX, ID_EDIT_CLEAR, m_ObjectDelete);
	
	DDX_CBIndex(pDX, IDC_HUGNEAREST, m_iHugIndex);
	DDX_Control(pDX, IDC_HUGNEAREST, m_HugCombo);
	DDX_Control(pDX, IDC_OBJECT_ENABLE_COLLISIONS, m_EnableCollisions);
	DDX_Control(pDX, IDC_OBJECT_ZOOMTO, m_ObjectZoomTo);
	DDX_Control(pDX, IDC_EDIT_SENSITIVITY, m_EditSensitivity);
	DDX_Control(pDX, IDC_SPIN_SENSITIVITY, m_SpinnerSensitivity);

	DDX_Control(pDX, IDC_GRID_CHECK, m_CheckGrid);
	DDX_Control(pDX, IDC_GRID_EDIT, m_EditGrid);
	DDX_Text(pDX, IDC_GRID_EDIT, m_fSnapToGrid);
	DDX_Control(pDX, IDC_GRID_SPIN, m_SpinnerGrid);

	m_EditGrid.EnableWindow(m_CheckGrid.GetCheck() == 1);
	m_SpinnerGrid.EnableWindow(m_CheckGrid.GetCheck() == 1);
	m_HugCombo.EnableWindow(m_EnableCollisions.GetCheck() == 1);
	// Enable the hug combo always for now until a more robust hug is implemented
//	m_HugCombo.EnableWindow(m_EnableCollisions.GetCheck() == false);
}

BEGIN_MESSAGE_MAP(CObjectToolPal, CGuiDialogBar)
	ON_BN_CLICKED(IDC_OBJECT_ENABLE_COLLISIONS, OnBnClickedObjectEnableCollisions)
	ON_BN_CLICKED(IDC_GRID_CHECK, OnBnClickedObjectCheckGrid)
	ON_BN_CLICKED(IDC_OBJECT_SELECT, OnGenericUpdateData)
	ON_BN_CLICKED(IDC_OBJECT_MOVE, OnBnClickedObjectPosition)
	ON_BN_CLICKED(IDC_OBJECT_ROTATE, OnBnClickedObjectRotation)
	ON_BN_CLICKED(IDC_OBJECT_SCALE, OnGenericUpdateData)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
	ON_CBN_SELCHANGE(IDC_LOCK, OnCbnSelchangeLock)
	ON_BN_CLICKED(IDC_LOCKVALUE_CHECK, OnBnClickedLockValueCheck)
	ON_EN_KILLFOCUS( IDC_LOCKVALUE, OnEnKilLFocusLockValue)
	ON_CBN_SELCHANGE(IDC_HUGNEAREST, OnCbnSelchangeHugnearest)

	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SENSITIVITY, OnDeltaposSpinSensitivity)
	ON_EN_CHANGE(IDC_EDIT_SENSITIVITY, OnEnChangeEditSensitivity)
	ON_EN_KILLFOCUS(IDC_EDIT_SENSITIVITY, OnEnKillfocusEditSensitivity)

	ON_NOTIFY(UDN_DELTAPOS, IDC_GRID_SPIN, OnDeltaposSpinGrid)
	ON_EN_KILLFOCUS(IDC_GRID_EDIT, OnEnKillfocusEditGrid)
END_MESSAGE_MAP()


// CObjectToolPal message handlers

LRESULT CObjectToolPal::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
	HINSTANCE appInst;

	CGuiDialogBar::HandleInitDialog(wParam, lParam);
	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(ObjectEditorDLL.hModule);

	CString temp;

	m_fSensitivity = 1.0f;

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init.\n");
	}

	temp.LoadString(IDS_LOCK_NONE);
	m_LockMovement.AddString(temp);
	temp.LoadString(IDS_LOCK_XY);
	m_LockMovement.AddString(temp);
	temp.LoadString(IDS_LOCK_XZ);
	m_LockMovement.AddString(temp);
	temp.LoadString(IDS_LOCK_YZ);
	m_LockMovement.AddString(temp);
	temp.LoadString(IDS_LOCK_X);
	m_LockMovement.AddString(temp);
	temp.LoadString(IDS_LOCK_Y);
	m_LockMovement.AddString(temp);
	temp.LoadString(IDS_LOCK_Z);
	m_LockMovement.AddString(temp);

	temp.LoadString(IDS_HUG_NONE);
	m_HugCombo.AddString(temp);
	temp.LoadString(IDS_HUG_NEG_X);
	m_HugCombo.AddString(temp);
	temp.LoadString(IDS_HUG_POS_X);
	m_HugCombo.AddString(temp);
	temp.LoadString(IDS_HUG_NEG_Y);
	m_HugCombo.AddString(temp);
	temp.LoadString(IDS_HUG_POS_Y);
	m_HugCombo.AddString(temp);
	temp.LoadString(IDS_HUG_NEG_Z);
	m_HugCombo.AddString(temp);
	temp.LoadString(IDS_HUG_POS_Z);
	m_HugCombo.AddString(temp);

	m_LockMovement.SetCurSel(m_iLockIndex);
	m_HugCombo.SetCurSel(m_iHugIndex);

	m_ObjectCreate.SetIcon((HICON)LoadImage(ObjectEditorDLL.hModule, MAKEINTRESOURCE(IDI_OBJECTCREATE), 
		IMAGE_ICON, 0, 0, 0));

	m_ObjectDelete.ModifyStyle(0, BS_OWNERDRAW);
	m_ObjectDelete.LoadBitmaps(IDB_OBJECT_DELETE_UP, IDB_OBJECT_DELETE_DOWN, IDB_OBJECT_DELETE_SEL, IDB_OBJECT_DELETE_DISABLED);

	// load up the icons
	m_ObjectSelect.SetIcon((HICON)LoadImage(ObjectEditorDLL.hModule, MAKEINTRESOURCE(IDI_OBJECTSELECT), 
		IMAGE_ICON, 0, 0, 0));
	m_ObjectMove.SetIcon((HICON)LoadImage(ObjectEditorDLL.hModule, MAKEINTRESOURCE(IDI_OBJECTMOVE), 
		IMAGE_ICON, 0, 0, 0));
	m_ObjectRotate.SetIcon((HICON)LoadImage(ObjectEditorDLL.hModule, MAKEINTRESOURCE(IDI_OBJECTROTATE), 
		IMAGE_ICON, 0, 0, 0));
	m_ObjectScale.SetIcon((HICON)LoadImage(ObjectEditorDLL.hModule, MAKEINTRESOURCE(IDI_OBJECTSCALE), 
		IMAGE_ICON, 0, 0, 0));

	// load up the icons
	m_EnableCollisions.SetIcon((HICON)LoadImage(ObjectEditorDLL.hModule, MAKEINTRESOURCE(IDI_OBJECTCOLLISION), 
		IMAGE_ICON, 0, 0, 0));

	// load up the icons
	m_ObjectZoomTo.ModifyStyle(0, BS_OWNERDRAW);
	m_ObjectZoomTo.LoadBitmaps(IDB_OBJECT_ZOOM_UP, IDB_OBJECT_ZOOM_DOWN, IDB_OBJECT_ZOOM_SEL, IDB_OBJECT_ZOOM_DISABLED);

	// now tell the application to look in dll for resources
	AfxSetResourceHandle(appInst);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CObjectToolPal::OnBnClickedObjectEnableCollisions()
{
	m_HugCombo.EnableWindow(m_EnableCollisions.GetCheck() == 1);
}

void CObjectToolPal::OnBnClickedObjectCheckGrid()
{
	m_EditGrid.EnableWindow(m_CheckGrid.GetCheck() == 1);
	m_SpinnerGrid.EnableWindow(m_CheckGrid.GetCheck() == 1);
}

void CObjectToolPal::OnBnClickedObjectRotation()
{
	UpdateData();
	if (m_bButtonMoveClicked)
	{
		m_iLockPosIndex = m_iLockIndex;
		m_iLockIndex = m_iLockRotIndex;
		m_bButtonMoveClicked = false;
		UpdateData(FALSE);
	}
	m_bButtonRotateClicked = true;
}

void CObjectToolPal::OnBnClickedObjectPosition()
{
	UpdateData();
	if (m_bButtonRotateClicked)
	{
		m_iLockRotIndex = m_iLockIndex;
		m_iLockIndex = m_iLockPosIndex;
		m_bButtonRotateClicked = false;
		UpdateData(FALSE);
	}
	m_bButtonMoveClicked = true;
}

void CObjectToolPal::OnGenericUpdateData()
{
	UpdateData();
}

void CObjectToolPal::SetManipulateState(MANIPULATE_STATE newState)
{
	m_ObjectManipulateState = newState;
	UpdateData(FALSE);
}

void CObjectToolPal::OnCbnSelchangeLock()
{
	UpdateData();
}

void CObjectToolPal::OnBnClickedLockValueCheck( )
{
	UpdateData();
}

void CObjectToolPal::OnEnKilLFocusLockValue( )
{
	CString szBuffer;
	m_LockValue.GetWindowText( szBuffer );
	m_fLockValue = (float)atof( StdString(szBuffer) );

	UpdateData(FALSE);
}

void CObjectToolPal::OnCbnSelchangeHugnearest()
{
	UpdateData();
}

void CObjectToolPal::OnDeltaposSpinSensitivity(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	*pResult = 0;

	m_fSensitivity = m_fSensitivity - pNMUpDown->iDelta * 0.05f;
	m_fSensitivity = max( m_fSensitivity, 0.1f );
	m_fSensitivity = min( m_fSensitivity, 10.0f );

	UpdateData(FALSE);
}

void CObjectToolPal::OnEnChangeEditSensitivity()
{
}

void CObjectToolPal::OnEnKillfocusEditSensitivity()
{
	CString szBuffer;
	m_EditSensitivity.GetWindowText( szBuffer );
	m_fSensitivity = (float)atof( StdString(szBuffer) );
	m_fSensitivity = max( m_fSensitivity, 0.1f );
	m_fSensitivity = min( m_fSensitivity, 10.0f );

	UpdateData(FALSE);
}

void CObjectToolPal::OnDeltaposSpinGrid(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	*pResult = 0;

	m_fSnapToGrid = m_fSnapToGrid - pNMUpDown->iDelta * 0.01f;
	m_fSnapToGrid = max( m_fSnapToGrid, 0.001f );
	m_fSnapToGrid = min( m_fSnapToGrid, 100000.0f );

	UpdateData(FALSE);
}

void CObjectToolPal::OnEnKillfocusEditGrid()
{
	CString szBuffer;
	m_EditGrid.GetWindowText( szBuffer );
	m_fSnapToGrid = (float)atof( StdString(szBuffer) );
	m_fSnapToGrid = max( m_fSnapToGrid, 0.001f );
	m_fSnapToGrid = min( m_fSnapToGrid, 100000.0f );

	UpdateData(FALSE);
}

