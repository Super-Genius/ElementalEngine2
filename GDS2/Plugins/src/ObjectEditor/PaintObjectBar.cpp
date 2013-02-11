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

#define SCALEMIN 0
#define SCALEMAX 10
#define ROTATIONMIN -360
#define ROTATIONMAX 360

// CPaintObjectBar dialog

IMPLEMENT_DYNAMIC(CPaintObjectBar, CGuiDialogBar)
CPaintObjectBar::CPaintObjectBar()
: m_bLevelTerrainMode(false)
{
	m_PaintMode = PM_NONE;
	m_pEditor = SINGLETONINSTANCE(CObjectEditor);
	m_fBrushSize = 5.0f;
	m_fScaleMax = 1.0f;
	m_fRotMin = 0.0f;
	m_fRotMax = 360.0f;
	m_fScaleMin = 0.0f;
	m_bScale = false;
	m_bRotation = false;
	m_bLevelTerrainMode = false;
	m_iRotationAxisIndex = AXIS_Z;
}

CPaintObjectBar::~CPaintObjectBar()
{
}

void CPaintObjectBar::DoDataExchange(CDataExchange* pDX)
{
	CGuiDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJECTEDIT_SCALEMIN, m_ScaleMinEdit);
	DDX_Text(pDX, IDC_OBJECTEDIT_SCALEMIN, m_fScaleMin);
	DDX_Control(pDX, IDC_OBJECTCHECK_SCALE, m_ScaleEnable);
	DDX_Control(pDX, IDC_OBJECTEDIT_SCALEMAX, m_ScaleMaxEdit);
	DDX_Text(pDX, IDC_OBJECTEDIT_SCALEMAX, m_fScaleMax);
	DDX_Control(pDX, IDC_OBJECTCHECK_ROT, m_RotEnable);
	DDX_Control(pDX, IDC_OBJECTEDIT_ROTMIN, m_RotMinEdit);
	DDX_Text(pDX, IDC_OBJECTEDIT_ROTMIN, m_fRotMin);
	DDX_Control(pDX, IDC_OBJECTEDIT_ROTMAX, m_RotMaxEdit);
	DDX_Text(pDX, IDC_OBJECTEDIT_ROTMAX, m_fRotMax);
	DDX_Control(pDX, IDC_OBJECT_PAINT, m_PaintToggleButton);
	DDX_Control(pDX, IDC_OBJECT_ERASE, m_EraseToggleButton);
	DDX_Control(pDX, IDC_OBJECT_LEVELTERRAIN, m_LevelTerrainToggleButton);
	DDX_Control(pDX, IDC_EDIT_OBJECTBRUSHSIZE, m_EditBrushSize);
	DDX_Control(pDX, IDC_SPIN_OBJECTBRUSHSIZE, m_SpinnerBrushSize);
	DDX_Text(pDX, IDC_EDIT_OBJECTBRUSHSIZE, m_fBrushSize);
	DDX_CBIndex(pDX, IDC_OBJECTLIST_ROTAXIS, m_iRotationAxisIndex);
	DDX_Control(pDX, IDC_OBJECTLIST_ROTAXIS, m_RotationAxisBox);
	
	SendSetTerrainCursorMsg();
}


BEGIN_MESSAGE_MAP(CPaintObjectBar, CGuiDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
	ON_NOTIFY(UDN_DELTAPOS, IDC_OBJECTSPIN_ROTMAX, OnDeltaposObjectspinRotmax)
	ON_BN_CLICKED(IDC_OBJECTCHECK_SCALE, OnBnClickedObjectcheckScale)
	ON_BN_CLICKED(IDC_OBJECT_PAINT, OnBnClickedObjectPaint)
	ON_BN_CLICKED(IDC_OBJECT_ERASE, OnBnClickedObjectErase)
	ON_EN_KILLFOCUS(IDC_EDIT_OBJECTBRUSHSIZE, OnEnKillfocusEditObjectbrushsize)
	ON_EN_CHANGE(IDC_EDIT_OBJECTBRUSHSIZE, OnEnChangeEditObjectbrushsize)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_OBJECTBRUSHSIZE, OnDeltaposSpinObjectbrushsize)
	ON_BN_CLICKED(IDC_OBJECTCHECK_ROT, OnBnClickedObjectcheckRot)
	ON_NOTIFY(UDN_DELTAPOS, IDC_OBJECTSPIN_ROTMIN, OnDeltaposObjectspinRotmin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_OBJECTSPIN_SCALEMAX, OnDeltaposObjectspinScalemax)
	ON_NOTIFY(UDN_DELTAPOS, IDC_OBJECTSPIN_SCALEMIN, OnDeltaposObjectspinScalemin)
	ON_EN_KILLFOCUS(IDC_OBJECTEDIT_ROTMAX, OnEnKillfocusObjecteditRotmax)
	ON_EN_KILLFOCUS(IDC_OBJECTEDIT_ROTMIN, OnEnKillfocusObjecteditRotmin)
	ON_EN_KILLFOCUS(IDC_OBJECTEDIT_SCALEMAX, OnEnKillfocusObjecteditScalemax)
	ON_EN_KILLFOCUS(IDC_OBJECTEDIT_SCALEMIN, OnEnKillfocusObjecteditScalemin)
	ON_CBN_KILLFOCUS(IDC_OBJECTLIST_ROTAXIS, OnCbnKillfocusObjectlistRotaxis)
	ON_BN_CLICKED(IDC_OBJECT_LEVELTERRAIN, OnBnClickedObjectLevelterrain)
//	ON_WM_ENABLE()
ON_WM_ENABLE()
END_MESSAGE_MAP()


// CPaintObjectBar message handlers
LONG CPaintObjectBar::OnInitDialog(UINT wParam, LONG lParam)
{
	HINSTANCE appInst;
	CString temp;

	CGuiDialogBar::HandleInitDialog(wParam, lParam);

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(ObjectEditorDLL.hModule);

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init.\n");
	}

	temp.LoadString(IDS_XYZ);
	m_RotationAxisBox.AddString(temp);
	temp.LoadString(IDS_X);
	m_RotationAxisBox.AddString(temp);
	temp.LoadString(IDS_Y);
	m_RotationAxisBox.AddString(temp);
	temp.LoadString(IDS_Z);
	m_RotationAxisBox.AddString(temp);
	temp.LoadString(IDS_XY);
	m_RotationAxisBox.AddString(temp);
	temp.LoadString(IDS_XZ);
	m_RotationAxisBox.AddString(temp);
	temp.LoadString(IDS_YZ);
	m_RotationAxisBox.AddString(temp);

	m_RotationAxisBox.SetCurSel(m_iRotationAxisIndex);

	m_PaintToggleButton.SetCheck(BST_UNCHECKED);
	m_ScaleEnable.SetCheck(BST_UNCHECKED);
	m_RotEnable.SetCheck(BST_UNCHECKED);

	m_PaintToggleButton.SetIcon((HICON)LoadImage(ObjectEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_OBJECTPAINT), IMAGE_ICON, 0, 0, 0));

	m_EraseToggleButton.SetIcon((HICON)LoadImage(ObjectEditorDLL.hModule,
		MAKEINTRESOURCE(IDI_OBJECTERASE), IMAGE_ICON, 0, 0, 0));

	m_LevelTerrainToggleButton.SetIcon((HICON)LoadImage(ObjectEditorDLL.hModule,
		MAKEINTRESOURCE(IDI_OBJECTLEVELTERRAIN), IMAGE_ICON, 0, 0, 0));

	// disable scale and rotate edit boxes
	m_ScaleMinEdit.EnableWindow(FALSE);
	m_ScaleMaxEdit.EnableWindow(FALSE);
	m_RotMinEdit.EnableWindow(FALSE);
	m_RotMaxEdit.EnableWindow(FALSE);
	m_RotationAxisBox.EnableWindow(FALSE);

	// now tell the application to look in dll for resources
	AfxSetResourceHandle(appInst);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPaintObjectBar::OnBnClickedObjectPaint()
{
	if (m_PaintToggleButton.GetCheck() == 0)
	{
		SetPaintMode(PM_NONE);
	}
	else
	{
		SetPaintMode(PM_PAINT);
	}
}

void CPaintObjectBar::OnBnClickedObjectErase()
{
	if (m_EraseToggleButton.GetCheck() == 0)
	{
		SetPaintMode(PM_NONE);
	}
	else
	{
		SetPaintMode(PM_ERASE);
	}
}

void CPaintObjectBar::EnablePaint()
{
	//unsigned int iTerrainGroup = 0;
	//ADDCOLLISIONGROUP( iTerrainGroup, TERRAINCOLLISIONGROUP );
	COLLISIONGROUP iTerrainGroup = TERRAIN;
	static DWORD msgHash_SetPickingGroups = CHashString(_T("SetPickingGroups")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_SetPickingGroups, sizeof(COLLISIONGROUP), &iTerrainGroup );
	//Update GUI
	UpdateData(FALSE);
}

void CPaintObjectBar::DisablePaint()
{
	//unsigned int iDefaultGroup = DEFAULTCOLLISIONGROUPMASK;
	COLLISIONGROUP iDefaultGroup = ALL;
	static DWORD msgHash_SetPickingGroups = CHashString(_T("SetPickingGroups")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_SetPickingGroups, sizeof(COLLISIONGROUP), &iDefaultGroup );
	//Update GUI
	UpdateData(FALSE);
}

void CPaintObjectBar::SendSetTerrainCursorMsg()
{
	// brush size
	int brushDataSize = 8;
	// brush data
	float brushData[64];
	DWORD i;

	// make white brush
	for (i = 0; i < 64; ++i)
	{
		brushData[i] = 1.0f;
	}

	BRUSHPARAMS msg;
	msg.m_fStrength = 100.0f; // usually use too low of a strength to see so just visual the full brush
	msg.m_fSize = m_fBrushSize;

	if (m_PaintMode != PM_NONE)
	{
		msg.m_iBrushDataSize = brushDataSize;
		msg.m_pBrushData = brushData;
	}
	else
	{
		// no brush in use, dont display anything
		msg.m_iBrushDataSize = 0;
		msg.m_pBrushData = NULL;
	}

	static DWORD msgHash_SetTerrainCursor = CHashString(_T("SetTerrainCursor")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_SetTerrainCursor, sizeof(msg), &msg );
}

void CPaintObjectBar::OnEnKillfocusEditObjectbrushsize()
{
	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	m_EditBrushSize.GetWindowText( szBuffer, 256 );
	m_fBrushSize = (float)atof( StdString(szBuffer) );
	if( m_fBrushSize < 0.0f )
	{
		m_fBrushSize = 0.0f;
		m_EditBrushSize.SetWindowText( _T("0.0") );
	}
	else if( m_fBrushSize > 256.0f )
	{
		m_fBrushSize = 256.0f;
		m_EditBrushSize.SetWindowText( _T("256.0") );
	}
}

void CPaintObjectBar::OnEnChangeEditObjectbrushsize()
{
	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	m_EditBrushSize.GetWindowText( szBuffer, 256 );
	m_fBrushSize = (float)atof( StdString(szBuffer) );
	if( m_fBrushSize < 0.0f )
	{
		m_fBrushSize = 0.0f;
		m_EditBrushSize.SetWindowText( _T("0.0") );
	}
	else if( m_fBrushSize > 256.0f )
	{
		m_fBrushSize = 256.0f;
		m_EditBrushSize.SetWindowText( _T("256.0") );
	}

	SendSetTerrainCursorMsg();
}

void CPaintObjectBar::OnDeltaposSpinObjectbrushsize(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_fBrushSize = m_fBrushSize - pNMUpDown->iDelta;
	if( m_fBrushSize < 0.1f )
	{
		m_fBrushSize = 0.1f;
	}
	else if( m_fBrushSize > 256.0f )
	{
		m_fBrushSize = 256.0f;
	}

	UpdateData(FALSE);
}

void CPaintObjectBar::OnBnClickedObjectcheckRot()
{
	if (m_RotEnable.GetCheck() == 0)
	{
		SetPaintOption(PO_NONE);
	}
	else
	{
		SetPaintOption(PO_ROTATION);
	}
}

void CPaintObjectBar::OnBnClickedObjectcheckScale()
{
	if (m_ScaleEnable.GetCheck() == 0)
	{
		SetPaintOption(PO_NONE);
	}
	else
	{
		SetPaintOption(PO_SCALE);
	}
}

void CPaintObjectBar::OnBnClickedObjectLevelterrain()
{
	if (m_LevelTerrainToggleButton.GetCheck() == 0)
	{
		SetPaintOption(PO_NONE);
	}
	else
	{
		SetPaintOption(PO_LEVELTERRAIN);
	}
}

void CPaintObjectBar::OnDeltaposObjectspinRotmax(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_fRotMax = m_fRotMax - pNMUpDown->iDelta;
	if( m_fRotMax < -360.0f )
	{
		m_fRotMax = -360.0f;
	}
	else if( m_fRotMax > 360.0f )
	{
		m_fRotMax = 360.0f;
	}

	UpdateData(FALSE);
}

void CPaintObjectBar::OnDeltaposObjectspinRotmin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_fRotMin = m_fRotMin - pNMUpDown->iDelta;
	if( m_fRotMin < ROTATIONMIN )
	{
		m_fRotMin = ROTATIONMIN;
	}
	else if( m_fRotMin > ROTATIONMAX )
	{
		m_fRotMin = ROTATIONMAX;
	}

	UpdateData(FALSE);
}

void CPaintObjectBar::OnDeltaposObjectspinScalemax(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_fScaleMax = m_fScaleMax - pNMUpDown->iDelta * 0.05f;;
	if( m_fScaleMax < SCALEMIN )
	{
		m_fScaleMax = SCALEMIN;
	}
	else if( m_fScaleMax > SCALEMAX )
	{
		m_fScaleMax = SCALEMAX;
	}

	UpdateData(FALSE);
}

void CPaintObjectBar::OnDeltaposObjectspinScalemin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_fScaleMin = m_fScaleMin - pNMUpDown->iDelta * 0.05f;;
	if( m_fScaleMin < SCALEMIN )
	{
		m_fScaleMin = SCALEMIN;
	}
	else if( m_fScaleMin > SCALEMAX )
	{
		m_fScaleMin = SCALEMAX;
	}

	UpdateData(FALSE);
}

void CPaintObjectBar::OnEnKillfocusObjecteditRotmax()
{
	// TODO: Add your control notification handler code here
	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	m_RotMaxEdit.GetWindowText( szBuffer, 256 );
	m_fRotMax = (float)atof( StdString(szBuffer) );
	if( m_fRotMax < ROTATIONMIN )
	{
		m_fRotMax = ROTATIONMIN;
		m_RotMaxEdit.SetWindowText( _T("-360.0") );
	}
	else if( m_fRotMax > ROTATIONMAX )
	{
		m_fRotMax = ROTATIONMAX;
		m_RotMaxEdit.SetWindowText( _T("360.0") );
	}
}

void CPaintObjectBar::OnEnKillfocusObjecteditRotmin()
{
	// TODO: Add your control notification handler code here
	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	m_RotMinEdit.GetWindowText( szBuffer, 256 );
	m_fRotMin = (float)atof( StdString(szBuffer) );
	if( m_fRotMin < ROTATIONMIN )
	{
		m_fRotMin = ROTATIONMIN;
		m_RotMinEdit.SetWindowText( _T("-360.0") );
	}
	else if( m_fRotMin > ROTATIONMAX )
	{
		m_fRotMin = ROTATIONMAX;
		m_RotMinEdit.SetWindowText( _T("360.0") );
	}
}

void CPaintObjectBar::OnEnKillfocusObjecteditScalemax()
{
	// TODO: Add your control notification handler code here
	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	m_ScaleMaxEdit.GetWindowText( szBuffer, 256 );
	m_fScaleMax = (float)atof( StdString(szBuffer) );
	if( m_fScaleMax < SCALEMIN )
	{
		m_fScaleMax = SCALEMIN;
		m_ScaleMaxEdit.SetWindowText( _T("0.0") );
	}
	else if( m_fScaleMax > SCALEMAX )
	{
		m_fScaleMax = SCALEMAX;
		m_ScaleMaxEdit.SetWindowText( _T("10.0") );
	}
}

void CPaintObjectBar::OnEnKillfocusObjecteditScalemin()
{
	// TODO: Add your control notification handler code here
	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	m_ScaleMinEdit.GetWindowText( szBuffer, 256 );
	m_fScaleMin = (float)atof( StdString(szBuffer) );
	if( m_fScaleMin < SCALEMIN )
	{
		m_fScaleMin = SCALEMIN;
		m_ScaleMinEdit.SetWindowText( _T("0.0") );
	}
	else if( m_fScaleMin > SCALEMAX )
	{
		m_fScaleMin = SCALEMAX;
		m_ScaleMinEdit.SetWindowText( _T("10.0") );
	}
}

void CPaintObjectBar::OnCbnKillfocusObjectlistRotaxis()
{
	UpdateData(TRUE);
}

void CPaintObjectBar::SetPaintMode(PAINTMODE mode)
{
	m_PaintMode = mode;

	switch (mode)
	{
		case PM_PAINT:
			EnablePaint();
			m_PaintToggleButton.SetCheck(1);
			m_EraseToggleButton.SetCheck(0);
			break;
		case PM_ERASE:
			DisablePaint();
			m_PaintToggleButton.SetCheck(0);
			m_EraseToggleButton.SetCheck(1);
			break;
		case PM_NONE:
			DisablePaint();
			m_PaintToggleButton.SetCheck(0);
			m_EraseToggleButton.SetCheck(0);
			break;
		default:
			break;
	}
}

void CPaintObjectBar::SetPaintOption(PAINTOPTIONS option)
{
	switch (option)
	{
		case PO_ROTATION:
			// enable rotation
			m_bRotation = true;
			m_RotEnable.SetCheck(1);
			m_RotMinEdit.EnableWindow(TRUE);
			m_RotMaxEdit.EnableWindow(TRUE);
			m_RotationAxisBox.EnableWindow(TRUE);
			break;
		case PO_SCALE:
			// enable scaling
			m_bScale = true;
			m_ScaleEnable.SetCheck(1);
			m_ScaleMinEdit.EnableWindow(TRUE);
			m_ScaleMaxEdit.EnableWindow(TRUE);
			break;
		case PO_LEVELTERRAIN:
			// enable terrain leveler
            m_bLevelTerrainMode = true;
			m_LevelTerrainToggleButton.SetCheck(1);
			break;
		case PO_NOROTATION:
			// disable rotation
			m_bRotation = false;
			m_RotMinEdit.EnableWindow(FALSE);
			m_RotMaxEdit.EnableWindow(FALSE);
			m_RotEnable.SetCheck(0);
			m_RotationAxisBox.EnableWindow(FALSE);
			break;
		case PO_NOSCALE:
			// disable scaling
			m_bScale = false;
			m_ScaleMinEdit.EnableWindow(FALSE);
			m_ScaleMaxEdit.EnableWindow(FALSE);
			m_ScaleEnable.SetCheck(0);
			break;
		case PO_NOLEVELTERRAIN:
			// disable terrain leveler
            m_bLevelTerrainMode = false;
			m_LevelTerrainToggleButton.SetCheck(0);
			break;
		default:
			m_bRotation = false;
			m_bScale = false;
			m_bLevelTerrainMode = false;
			m_RotMinEdit.EnableWindow(FALSE);
			m_RotMaxEdit.EnableWindow(FALSE);
			m_ScaleMinEdit.EnableWindow(FALSE);
			m_ScaleMaxEdit.EnableWindow(FALSE);
			m_RotEnable.SetCheck(0);
			m_ScaleEnable.SetCheck(0);
			m_LevelTerrainToggleButton.SetCheck(0);
			m_RotationAxisBox.EnableWindow(FALSE);
			break;
	}
}

void CPaintObjectBar::OnEnable(BOOL bEnable)
{
	CGuiDialogBar::OnEnable(bEnable);

	// TODO: Add your message handler code here
	if (!bEnable)
	{
		DisablePaint();
	}
	else
	{
		if (m_PaintMode == PM_PAINT)
		{
			EnablePaint();
		}
	}
}
