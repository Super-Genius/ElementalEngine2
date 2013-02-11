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

#include <math.h>
#include "stdafx.h"
#include "TerrainToolPal.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CTerrainToolPal dialog

IMPLEMENT_DYNAMIC(CTerrainToolPal, CGuiDialogBar)

CTerrainToolPal::CTerrainToolPal()
{
	m_pEditor = SINGLETONINSTANCE(CTerrainEditor);
}

CTerrainToolPal::~CTerrainToolPal()
{
}


BEGIN_MESSAGE_MAP(CTerrainToolPal, CGuiDialogBar)
	ON_BN_CLICKED(IDC_PAINT, OnBnClickedPaint)
	ON_BN_CLICKED(IDC_DEFORM, OnBnClickedDeform)
	ON_BN_CLICKED(IDC_RAISE, OnBnClickedRaise)
	ON_BN_CLICKED(IDC_LOWER, OnBnClickedLower)
	ON_EN_CHANGE(IDC_SIZE_EDIT, OnEnChangeSizeEdit)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SIZE_SPINNER, OnDeltaposSizeSpinner)
	ON_EN_CHANGE(IDC_STRENGTH_EDIT, OnEnChangeStrengthEdit)
	ON_NOTIFY(UDN_DELTAPOS, IDC_STRENGTH_SPINNER, OnDeltaposStrengthSpinner)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
	ON_UPDATE_COMMAND_UI(IDC_PAINT, OnUpdatePaint)
	ON_UPDATE_COMMAND_UI(IDC_DEFORM, OnUpdateDeform)
	ON_UPDATE_COMMAND_UI(IDC_RAISE, OnUpdateRaise)
	ON_UPDATE_COMMAND_UI(IDC_LOWER, OnUpdateLower)
	ON_UPDATE_COMMAND_UI(IDC_TERRAIN_PROP, OnUpdateTerrainProp)
	ON_UPDATE_COMMAND_UI(IDC_TERRAIN_MODIFY, OnUpdateTerrainModify)
	ON_BN_CLICKED(IDC_TERRAIN_PROP, OnBnClickedTerrainProp)
	ON_BN_CLICKED(IDC_MAXHEIGHT_ENABLE, OnBnClickedMaxheightEnable)
	ON_BN_CLICKED(IDC_MINHEIGHT_ENABLE, OnBnClickedMinheightEnable)
	ON_EN_CHANGE(IDC_MAXHEIGHT_EDIT, OnEnChangeMaxheightEdit)
	ON_NOTIFY(UDN_DELTAPOS, IDC_MAXHEIGHT_SPINNER, OnDeltaposMaxheightSpinner)
	ON_EN_CHANGE(IDC_MINHEIGHT_EDIT, OnEnChangeMinheightEdit)
	ON_NOTIFY(UDN_DELTAPOS, IDC_MINHEIGHT_SPINNER, OnDeltaposMinheightSpinner)
	ON_CBN_SELCHANGE(IDC_BRUSHSHAPE_CBX, OnCbnSelchangeBrushshape)
	ON_CBN_SELCHANGE(IDC_BRUSHOP_CBX, OnCbnSelchangeBrushshape)
	ON_BN_CLICKED(IDC_TERRAIN_MODIFY, OnBnClickedTerrainModify)
//	ON_BN_CLICKED(IDC_NAVMESH, OnBnClickedNavmesh)
//	ON_UPDATE_COMMAND_UI(IDC_NAVMESH, OnUpdateNavMesh)
	ON_WM_ENABLE()
	ON_EN_CHANGE(IDC_NEWLAYERSIZE_EDIT, OnEnChangeNewlayersizeEdit)
END_MESSAGE_MAP()


void CTerrainToolPal::DoDataExchange(CDataExchange* pDX)
{
	CGuiDialogBar::DoDataExchange(pDX);

	DDX_Text( pDX, IDC_STRENGTH_EDIT, m_fBrushStrength );
	DDX_Text( pDX, IDC_SIZE_EDIT, m_fBrushSize );
	DDX_Text( pDX, IDC_MAXHEIGHT_EDIT, m_fMaxHeight);
	DDX_Check( pDX, IDC_MAXHEIGHT_ENABLE, m_bMaxCap );
	DDX_Text( pDX, IDC_MINHEIGHT_EDIT, m_fMinHeight);
	DDX_Check( pDX, IDC_MINHEIGHT_ENABLE, m_bMinCap );
	DDX_CBIndex(pDX, IDC_BRUSHSHAPE_CBX, m_iBrushShape);
	DDX_Control(pDX, IDC_BRUSHSHAPE_CBX, m_BrushShape);
	DDX_CBIndex(pDX, IDC_BRUSHOP_CBX, m_iBrushOp);
	DDX_Control(pDX, IDC_BRUSHOP_CBX, m_BrushOp);
	DDX_Control(pDX, IDC_DEFORM, m_DeformToggleButton);
	DDX_Control(pDX, IDC_PAINT, m_PaintToggleButton);
	DDX_Control(pDX, IDC_RAISE, m_RaiseToggleButton);
	DDX_Control(pDX, IDC_LOWER, m_LowerToggleButton);
	DDX_Control(pDX, IDC_SIZE_EDIT, m_BrushSizeEdit);
	DDX_Control(pDX, IDC_STRENGTH_EDIT, m_BrushStrengthEdit);
	DDX_Control(pDX, IDC_MAXHEIGHT_EDIT, m_MaxHeightEdit);
	DDX_Control(pDX, IDC_MAXHEIGHT_ENABLE, m_MaxHeightEnable);
	DDX_Control(pDX, IDC_MAXHEIGHT_SPINNER, m_MaxHeightSpinner);
	DDX_Control(pDX, IDC_MINHEIGHT_EDIT, m_MinHeightEdit);
	DDX_Control(pDX, IDC_MINHEIGHT_ENABLE, m_MinHeightEnable);
	DDX_Control(pDX, IDC_MINHEIGHT_SPINNER, m_MinHeightSpinner);

	DDX_Text( pDX, IDC_NEWLAYERSIZE_EDIT, m_fNewLayerScale);
}

LRESULT CTerrainToolPal::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
	m_ToolBox = EngineGetToolBox();

	BOOL bRet = HandleInitDialog(wParam, lParam);

	m_fBrushSize = 80.0f;
	m_fBrushStrength = 20.0f;
	m_bDeformLower = false;
	m_eTerrainTool = kTerrainToolSelect;
	m_fNewLayerScale = 32.0f;

	m_iBrushShape = 0;
	m_iBrushOp = 0;

	m_fBrushSizeDeform = 80.0f;
	m_fBrushStrengthDeform = 20.0f;
	m_fBrushSizePaint = 80.0f;
	m_fBrushStrengthPaint = 100.0f;

	m_fMinHeight = 0.0f;
	m_fMaxHeight = 0.0f;

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init.\n");
	}

	m_LowerToggleButton.SetCheck(BST_UNCHECKED);
	m_RaiseToggleButton.SetCheck(BST_CHECKED);
	m_DeformToggleButton.SetCheck(BST_UNCHECKED);
	m_PaintToggleButton.SetCheck(BST_UNCHECKED);

	m_MaxHeightEnable.SetCheck(false);
	m_MaxHeightEdit.EnableWindow(false);
	m_MaxHeightSpinner.EnableWindow(false);
	m_MinHeightEnable.SetCheck(false);
	m_MinHeightEdit.EnableWindow(false);
	m_MinHeightSpinner.EnableWindow(false);

	// tested and was working with ILC_COLOR32 constant az well...
	// not sure which one to set, both was wrking
	m_BrushShapeImages.Create(20, 20, ILC_COLOR16, 0, -1);

	BRUSHOBJECTLIST *pBrushList = m_pEditor->GetBrushObjects();
	BRUSHOBJECTLIST::iterator it = pBrushList->begin();
	while( it != pBrushList->end() )
	{
		int iImage = m_BrushShapeImages.Add( it->m_pBitmap, RGB(0, 0, 0) );
		COMBOBOXEXITEM item;
		ZeroMemory( &item, sizeof(item) );

		// these are needed to show the images as well
		item.mask = CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
		item.iItem = m_BrushShape.GetCount();
		
		// set the images of the comboboxItem
		item.iImage = iImage;
		item.iSelectedImage = iImage;

		item.pszText = const_cast<LPSTR>((LPCSTR)it->m_wszName);

		m_BrushShape.InsertItem( &item );
		it++;
	}
	m_BrushShape.SetImageList( &m_BrushShapeImages );
	m_BrushShape.SetCurSel( 0 );
	// Adjust brush combo right position based on strength edit spinner
	// it is not possible to set right position in resource editor 
	// because combo has images inside
	CRect rc;
	GetDlgItem(IDC_STRENGTH_SPINNER)->GetWindowRect(rc);
	int nComboRight = rc.right;
	m_BrushShape.GetWindowRect(rc);
	rc.right = nComboRight;
	m_BrushShape.SetWindowPos(NULL, 0, 0, rc.Width(), rc.Height(), SWP_NOMOVE | SWP_NOZORDER);

	// Brush Op
	COMBOBOXEXITEM item;
	ZeroMemory( &item, sizeof(item) );
	item.mask = CBEIF_TEXT;

	item.iItem = m_BrushOp.GetCount();
	item.pszText = _T("Add/Subtract");
	m_BrushOp.InsertItem( &item );

	item.iItem = m_BrushOp.GetCount();
	item.pszText = _T("Multiply");
	m_BrushOp.InsertItem( &item );

	item.iItem = m_BrushOp.GetCount();
	item.pszText = _T("Constant");
	m_BrushOp.InsertItem( &item );

	item.iItem = m_BrushOp.GetCount();
	item.pszText = _T("Smooth/Sharpen");
	m_BrushOp.InsertItem( &item );

	m_BrushOp.SetCurSel( 0 );

	CButton *button = (CButton *)GetDlgItem(IDC_TERRAIN_PROP);
	button->SetIcon((HICON)LoadImage(TerrainEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_TERRAIN_PROP), IMAGE_ICON, 32, 32, 0));

	button = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY);
	button->SetIcon((HICON)LoadImage(TerrainEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_TERRAIN_MODIFY), IMAGE_ICON, 32, 32, 0));

	m_RaiseToggleButton.SetIcon((HICON)LoadImage(TerrainEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_UP_ARROW), IMAGE_ICON, 32, 32, 0));

	m_LowerToggleButton.SetIcon((HICON)LoadImage(TerrainEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_DOWN_ARROW), IMAGE_ICON, 32, 32, 0));

	m_DeformToggleButton.SetIcon((HICON)LoadImage(TerrainEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_DEFORM), IMAGE_ICON, 32, 32, 0));

	m_PaintToggleButton.SetIcon((HICON)LoadImage(TerrainEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_PAINT), IMAGE_ICON, 32, 32, 0));

	return bRet;
}

// CTerrainToolPal message handlers

void CTerrainToolPal::OnUpdateRaise(CCmdUI* pCmdUI)
{
}

void CTerrainToolPal::OnUpdateLower(CCmdUI* pCmdUI)
{
}

void CTerrainToolPal::OnUpdateLightmapGenerate(CCmdUI* pCmdUI)
{
}

void CTerrainToolPal::OnUpdatePaint(CCmdUI* pCmdUI)
{
}

void CTerrainToolPal::OnUpdateDeform(CCmdUI* pCmdUI)
{
}

void CTerrainToolPal::OnUpdateTerrainProp(CCmdUI* pCmdUI)
{
	IHashString *sectorName;
	sectorName = m_pEditor->GetSelectedSector();
	pCmdUI->Enable(sectorName != NULL);
}

void CTerrainToolPal::OnUpdateTerrainModify(CCmdUI * pCmdUI )
{

}

//void CTerrainToolPal::OnUpdateNavMesh(CCmdUI* pCmdUI)
//{
//}
//
void CTerrainToolPal::OnBnClickedPaint()
{
	// check our button
	m_PaintToggleButton.SetCheck( BST_CHECKED );
	
	if( m_DeformToggleButton.GetCheck() == BST_CHECKED )
	{
		m_DeformToggleButton.SetCheck(BST_UNCHECKED);
		// Store the Deform Settings
		m_fBrushSizeDeform = m_fBrushSize;
		m_fBrushStrengthDeform = m_fBrushStrength;
	}

	// Load the Paint Settings
	m_fBrushSize = m_fBrushSizePaint;
	m_fBrushStrength = m_fBrushStrengthPaint;
	// Switch Modes
	m_eTerrainTool = kTerrainToolPaint;

	COLLISIONGROUP iTerrainGroup = TERRAIN;
	static DWORD msgHash_SetPickingGroups = CHashString(_T("SetPickingGroups")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_SetPickingGroups, sizeof(COLLISIONGROUP), &iTerrainGroup );

/*	}
	else
	{
		m_fBrushSizePaint = m_fBrushSize;
		m_fBrushStrengthPaint = m_fBrushStrength;
		m_eTerrainTool = kTerrainToolSelect;

		COLLISIONGROUP iDefaultGroup = ALL;
		static DWORD msgHash_SetPickingGroups = CHashString(_T("SetPickingGroups")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_SetPickingGroups, sizeof(COLLISIONGROUP), &iDefaultGroup );
	}
*/
	// update brush
	UpdateBrushSize();

	//Update GUI
	UpdateData(FALSE);
}

void CTerrainToolPal::OnBnClickedDeform()
{
	// new version: buttons can't be switch off

	// check our button
	m_DeformToggleButton.SetCheck( BST_CHECKED );
	
	// store values for paining if necessary
	if ( m_PaintToggleButton.GetCheck() == BST_CHECKED )
	{
		// uncheck paint button
		m_PaintToggleButton.SetCheck(BST_UNCHECKED);

		// Store the Paint Settings
		m_fBrushSizePaint = m_fBrushSize;
		m_fBrushStrengthPaint = m_fBrushStrength;
	}

	// Load the Deform Settings
	m_fBrushSize = m_fBrushSizeDeform;
	m_fBrushStrength = m_fBrushStrengthDeform;
	// Switch Modes
	m_eTerrainTool = kTerrainToolDeform;

	COLLISIONGROUP iTerrainGroup = TERRAIN;
	static DWORD msgHash_SetPickingGroups = CHashString(_T("SetPickingGroups")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_SetPickingGroups, sizeof(COLLISIONGROUP), &iTerrainGroup );

	// update brush
	UpdateBrushSize();

	//Update GUI
	UpdateData(FALSE);

	// original version: buttons can be switched off
/*	if( m_DeformToggleButton.GetCheck() == 1 )
	{
		if( m_PaintToggleButton.GetCheck() == 1 )
		{
			m_PaintToggleButton.SetCheck(BST_UNCHECKED);
			// Store the Paint Settings
			m_fBrushSizePaint = m_fBrushSize;
			m_fBrushStrengthPaint = m_fBrushStrength;
		}
		// Load the Deform Settings
		m_fBrushSize = m_fBrushSizeDeform;
		m_fBrushStrength = m_fBrushStrengthDeform;
		// Switch Modes
		m_eTerrainTool = kTerrainToolDeform;

		COLLISIONGROUP iTerrainGroup = TERRAIN;
		static DWORD msgHash_SetPickingGroups = CHashString(_T("SetPickingGroups")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_SetPickingGroups, sizeof(COLLISIONGROUP), &iTerrainGroup );
	}
	else
	{
		m_fBrushSizeDeform = m_fBrushSize;
		m_fBrushStrengthDeform = m_fBrushStrength;
		m_eTerrainTool = kTerrainToolSelect;

		//unsigned int iDefaultGroup = DEFAULTCOLLISIONGROUPMASK;
		COLLISIONGROUP iDefaultGroup = ALL;
		static DWORD msgHash_SetPickingGroups = CHashString(_T("SetPickingGroups")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_SetPickingGroups, sizeof(COLLISIONGROUP), &iDefaultGroup );
	}

	// update brush
	UpdateBrushSize();

	// Update GUI
	UpdateData(FALSE);
*/
}

void CTerrainToolPal::OnBnClickedRaise()
{
	m_LowerToggleButton.SetCheck(BST_UNCHECKED);
	m_RaiseToggleButton.SetCheck( BST_CHECKED );
	m_bDeformLower = false;
}

void CTerrainToolPal::OnBnClickedLower()
{
	m_RaiseToggleButton.SetCheck(BST_UNCHECKED);
	m_LowerToggleButton.SetCheck( BST_CHECKED );
	// TODO: Add your control notification handler code here
	m_bDeformLower = true;
}

void CTerrainToolPal::OnBnClickedMaxheightEnable()
{
	if( m_MaxHeightEnable.GetCheck() )
	{
		m_MaxHeightEdit.EnableWindow(TRUE);
		m_MaxHeightSpinner.EnableWindow(TRUE);
	}
	else
	{
		m_MaxHeightEdit.EnableWindow(FALSE);
		m_MaxHeightSpinner.EnableWindow(FALSE);
	}
	UpdateData(TRUE);
}

void CTerrainToolPal::OnEnChangeMaxheightEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	m_MaxHeightEdit.GetWindowText( szBuffer, 256 );
	m_fMaxHeight = (float)atof( StdString(szBuffer) );
}

void CTerrainToolPal::OnDeltaposMaxheightSpinner(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_fMaxHeight = m_fMaxHeight - pNMUpDown->iDelta * 0.1f;
	UpdateData(FALSE);
}

void CTerrainToolPal::OnBnClickedMinheightEnable()
{
	if( m_MinHeightEnable.GetCheck() )
	{
		m_MinHeightEdit.EnableWindow(TRUE);
		m_MinHeightSpinner.EnableWindow(TRUE);
	}
	else
	{
		m_MinHeightEdit.EnableWindow(FALSE);
		m_MinHeightSpinner.EnableWindow(FALSE);
	}
	UpdateData(TRUE);
}

void CTerrainToolPal::OnEnChangeMinheightEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	//CEdit *pEdit = (CEdit*)GetDlgItem( IDC_SIZE_EDIT );
	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	m_MinHeightEdit.GetWindowText( szBuffer, 256 );
	m_fMinHeight = (float)atof( StdString(szBuffer) );
}

void CTerrainToolPal::OnDeltaposMinheightSpinner(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_fMinHeight = m_fMinHeight - pNMUpDown->iDelta * 0.1f;
	UpdateData(FALSE);
}

void CTerrainToolPal::OnEnChangeSizeEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	TCHAR szBuffer[256];	szBuffer[0] = '\0';
	m_BrushSizeEdit.GetWindowText( szBuffer, 256 );

	// original: (float) atof( szBuffer )
	// rounding x to 1 decimal digit: x = floor( x * 10 +0.5 ) /10
	// note: the purpose of '+0.5' is to round 1.50 to 2. 
	// in our case:
	m_fBrushSize = (float)floor( atof( szBuffer ) * 10 + 0.5 ) /10.0f;
	if( m_fBrushSize < g_fMinBrushSize )
	{
		m_fBrushSize = g_fMinBrushSize;
		sprintf( szBuffer, "%.1f", m_fBrushSize );
		m_BrushSizeEdit.SetWindowText( szBuffer );
	}
	else if( m_fBrushSize > g_fMaxBrushSize )
	{
		m_fBrushSize = g_fMaxBrushSize;
		sprintf( szBuffer, "%.1f", m_fBrushSize );
		m_BrushSizeEdit.SetWindowText( szBuffer );
	}

	// refresh the value in the window with the rounded value
	UpdateData(FALSE);

	// we should update the brush size only when the edit field looses focus
	UpdateBrushSize();

}

bool CTerrainToolPal::UpdateBrushSize(bool clearBrush)
{
	// send the message to update rendering
	static DWORD msgSetDecalSize = CHashString(_T("SetDecalSize")).GetUniqueID();
	
	// default value is the normal size.
	float currentSize = m_fBrushSize;

	// if the clear flag is set, than we need to render with 0 size. 
	if ( clearBrush ) currentSize = 0;

	DWORD retval = m_ToolBox->SendMessage( msgSetDecalSize, sizeof(currentSize),  &currentSize, m_pEditor->getBrushName(),  m_pEditor->getBrushTypeName() );
	
	// check for messages error
	if ( retval != MSG_HANDLED ) 
	{
		m_ToolBox->Log( LOGINFORMATION, _T("TerrainToolPal: Can't set decal size! Message error! This should only happen during initialization of the toolbars.\n") );
		return false;
	}

	return true;
}

void CTerrainToolPal::OnDeltaposSizeSpinner(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	// original: m_fBrushSize - pNMUpDown->iDelta * 0.1f
	// rounding x to 1 decimal digit: floor( x * 10 +0.5)/10
	// in our case:
	m_fBrushSize = floor ( (m_fBrushSize - pNMUpDown->iDelta * 0.1f ) * 10 + 0.5f) / 10.0f;
	if( m_fBrushSize < g_fMinBrushSize )
	{
		m_fBrushSize = g_fMinBrushSize;
	}
	else if( m_fBrushSize > g_fMaxBrushSize )
	{
		m_fBrushSize = g_fMaxBrushSize;
	}
	UpdateData(FALSE);

	// call the update function, to send the message about size change
	UpdateBrushSize();
}

void CTerrainToolPal::OnEnChangeStrengthEdit()
{
	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	m_BrushStrengthEdit.GetWindowText( szBuffer, 256 );

	// note: '+0.5f' to round 1.5f to 2.0f. 
	m_fBrushStrength = (float)floor( atof( szBuffer ) * 10.0f + 0.5f )/10.0f ;

	if( m_fBrushStrength < 0.0f )
	{
		m_fBrushStrength = 0.1f;
		m_BrushStrengthEdit.SetWindowText( _T("0.0") );
	} 
	else if( m_fBrushStrength > 100.0f )
	{
		m_fBrushStrength = 100.0f;
		m_BrushStrengthEdit.SetWindowText( _T("100.0") );
	}

	// refresh gui
	UpdateData(FALSE);
}

void CTerrainToolPal::OnDeltaposStrengthSpinner(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	// rounding by floor function to 1 decimal digit
	m_fBrushStrength = floor( (m_fBrushStrength - pNMUpDown->iDelta * 0.1f ) * 10.0f + 0.5f) / 10.0f;

	if( m_fBrushStrength < 0.1f )
	{
		m_fBrushStrength = 0.1f;
	}
	else if( m_fBrushStrength > 100.0f )
	{
		m_fBrushStrength = 100.0f;
	}

	UpdateData(FALSE);
}

void CTerrainToolPal::OnBnClickedTerrainProp()
{
	m_pEditor->ShowTerrainPropertiesDialog( true );
}

void CTerrainToolPal::OnCbnSelchangeBrushshape()
{
	UpdateBrushshape();
}

void CTerrainToolPal::UpdateBrushshape()
{
	// get the selected brush
	UpdateData(TRUE);

	// get current brush
	BRUSHOBJECTLIST::iterator itrBrush = m_pEditor->GetBrushObjects()->begin();
	advance(itrBrush, m_iBrushShape);

	// get the brush name
	CHashString hszBrushName(itrBrush->m_wszFileName);

	static DWORD msgSetDecalTexture = CHashString(_T("SetDecalTexture")).GetUniqueID();
	m_ToolBox->SendMessage( msgSetDecalTexture, sizeof(&hszBrushName), &hszBrushName, m_pEditor->getBrushName(), m_pEditor->getBrushTypeName() );

}

void CTerrainToolPal::OnCbnSelchangeBrushop()
{
	UpdateData(TRUE);
}

void CTerrainToolPal::OnBnClickedTerrainModify()
{
	CTerrainEditor::TerrainCreationCallback(NULL);
}

//void CTerrainToolPal::OnBnClickedNavmesh()
//{
//	CHashString navmeshType(_T("CNavMeshObject"));
//
//	// make sure the user feels like waiting for a few minutes and warn them of what they are about to do
//	if (MessageBox( _T("Generating a navigation mesh may take several minutes.\n\nContinue?"), _T("Navigation Mesh"), MB_ICONQUESTION|MB_YESNO ) == IDNO)
//		return; // user hit NO
//
//	// attempt to send generate nav mesh message
//	static DWORD msgHash_GenerateNavMesh = CHashString(_T("GenerateNavMesh")).GetUniqueID();
//	DWORD result = EngineGetToolBox()->SendMessage(msgHash_GenerateNavMesh, 0, NULL, NULL, &navmeshType );
//
//	// if we error'd then report it
//	if (result == MSG_ERROR)
//	{
//		MessageBox( _T("Failed to generate a new navigation mesh.") );
//		return;
//	}
//
//	// if no nav mesh objects handled our first attempt
//	if (result == MSG_NOT_HANDLED)
//	{
//		// no nav mesh objects were found, so try to make one
//		CHashString navmeshName(_T("NavMesh"));
//		CREATEOBJECTPARAMS cop;
//		cop.name = &navmeshName;
//		cop.parentName = NULL;
//		cop.typeName = &navmeshType;
//		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
//		EngineGetToolBox()->SendMessage(msgHash_CreateObject, sizeof(cop), &cop );
//
//		INITOBJECTPARAMS iop;
//		iop.name = &navmeshName;
//		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
//		EngineGetToolBox()->SendMessage(msgHash_InitObject, sizeof(iop), &iop );
//
//		// make second attempt to generate nav mesh
//		static DWORD msgHash_GenerateNavMesh = CHashString(_T("GenerateNavMesh")).GetUniqueID();
//		result = EngineGetToolBox()->SendMessage(msgHash_GenerateNavMesh, 0, NULL, &navmeshName, &navmeshType );
//
//		// if we error'd again then report it
//		if (result == MSG_ERROR)
//		{
//			MessageBox( _T("Failed to generate a new navigation mesh.") );
//			return;
//		}
//
//		// if it still wasn't handled then we failed to create the object itself
//		if (result == MSG_NOT_HANDLED)
//		{
//			MessageBox( _T("Failed to create a navigation mesh object!") );
//			return;
//		}
//	}
//}

/*
void CTerrainToolPal::Disable()
{
	m_LowerToggleButton.SetCheck(BST_UNCHECKED);
	m_RaiseToggleButton.SetCheck(BST_CHECKED);
	m_DeformToggleButton.SetCheck(BST_UNCHECKED);
	m_PaintToggleButton.SetCheck(BST_UNCHECKED);

	m_bDeformLower = false;
	m_eTerrainTool = kTerrainToolSelect;

	unsigned int iDefaultGroup = DEFAULTCOLLISIONGROUPMASK;
	static DWORD msgHash_SetPickingGroups = CHashString(_T("SetPickingGroups")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_SetPickingGroups, sizeof(unsigned int), &iDefaultGroup );
	//Update GUI
	UpdateData(FALSE);
}
*/

void CTerrainToolPal::OnEnable(BOOL bEnable)
{
	CGuiDialogBar::OnEnable(bEnable);

	// if we are disabled, disable the buttons so we aren't painting while resizing, etc
	if (!bEnable)
	{
		// save the current tool to be able to restore it.
		m_eTerrainToolLastInUse = m_eTerrainTool;

		// set tool to selection, to not to render the decal when something else is done
		m_eTerrainTool = kTerrainToolSelect;

		// clear the current brush from the terrain
		UpdateBrushSize( true );

		//unsigned int iDefaultGroup = DEFAULTCOLLISIONGROUPMASK;
		COLLISIONGROUP iDefaultGroup = ALL;
		static DWORD msgHash_SetPickingGroups = CHashString(_T("SetPickingGroups")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_SetPickingGroups, sizeof(COLLISIONGROUP), &iDefaultGroup );
	}
	else
	{
		// restoring the previous state
		if ( m_eTerrainToolLastInUse != NULL )
		{
			m_eTerrainTool = m_eTerrainToolLastInUse;

			//unsigned int iTerrainGroup = 0;
			//ADDCOLLISIONGROUP( iTerrainGroup, TERRAINCOLLISIONGROUP );
			COLLISIONGROUP iTerrainGroup = TERRAIN;
			static DWORD msgHash_SetPickingGroups = CHashString(_T("SetPickingGroups")).GetUniqueID();
			EngineGetToolBox()->SendMessage(msgHash_SetPickingGroups, sizeof(COLLISIONGROUP), &iTerrainGroup );

			OnCbnSelchangeBrushshape();

			// we restore the original brush size
			UpdateBrushSize();
		}
	}
}

void CTerrainToolPal::OnEnChangeNewlayersizeEdit()
{
	UpdateData();
}
