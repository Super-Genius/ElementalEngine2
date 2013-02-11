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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CTerrainLayerMaterialType, CDialog)

const UINT ResourceDragMessage = RegisterWindowMessage(RESOURCE_DRAG_MSG_STR);
const UINT ResourceDropMessage = RegisterWindowMessage(RESOURCE_DROP_MSG_STR);

CTerrainLayerMaterialType::CTerrainLayerMaterialType() : CDialog(CTerrainLayerMaterialType::IDD)
{
	m_pDragImage = NULL;
	m_bDragging = false;
	m_nDragIndex = -1;
	m_iCurrentSelectedLayer = -1;
	m_ToolBox = EngineGetToolBox();
	m_pSectorName = NULL;
	m_pEditor = SINGLETONINSTANCE(CTerrainEditor);
}

CTerrainLayerMaterialType::~CTerrainLayerMaterialType()
{
	delete m_pDragImage;
	m_pDragImage = NULL;
}

BOOL CTerrainLayerMaterialType::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_SPLATSCALEX_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPLATSCALEX_SPINNER)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPLATSCALEY_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPLATSCALEY_SPINNER)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPLATSCALE_PROPORTIONAL)->EnableWindow(FALSE);

	CRect listRect;
	m_TextureListCtrl.GetClientRect(listRect);
	listRect.InflateRect(CSize(0, listRect.Height()*5));
	//listRect.right -= 10;
	m_TextureListCtrl.SetWorkAreas(1, listRect);

	m_SplatScale.X = 1.0f;
	m_SplatScale.Y = 1.0f;

	m_bDragging = false;

	return TRUE;  
}
void CTerrainLayerMaterialType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SECTOR_TEXTURES_LIST, m_TextureListCtrl);
	DDX_Text(pDX, IDC_SPLATSCALEX_EDIT, m_SplatScale.X);
	DDX_Text(pDX, IDC_SPLATSCALEY_EDIT, m_SplatScale.Y);
	DDX_Check( pDX, IDC_SPLATSCALE_PROPORTIONAL, m_iSplatScaleProportional );
	DDX_Control(pDX, IDC_TERRAIN_STATUS_TEXT, m_StatusText);

	if (m_iCurrentSelectedLayer >= 0)
	{
		// keep map updated
		CHashString hszTextureName( m_TextureListCtrl.GetItemText(m_iCurrentSelectedLayer, 0) );
		m_TextureListScaleMap[hszTextureName.GetUniqueID()] = m_SplatScale;

		// send message to sector
		CHashString szAffectedLayer( m_TextureListCtrl.GetItemText( m_iCurrentSelectedLayer, 0 ) );
		TERRAINSECTORSETSTATEPARAMS sectorstate;
		sectorstate.m_iValidFields = SECTORSTATE_LAYERSCALE;
		sectorstate.m_pAffectedLayer = &szAffectedLayer;
		sectorstate.m_fLayerScaleX = m_SplatScale.X;
		sectorstate.m_fLayerScaleY = m_SplatScale.Y;
		static DWORD msgHash_TerrainSectorSetState = CHashString(_T("TerrainSectorSetState")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TerrainSectorSetState, sizeof(sectorstate), &sectorstate, m_pSectorName  );
	}

	ShowWindow(SW_SHOW);
}

BEGIN_MESSAGE_MAP(CTerrainLayerMaterialType, CDialog)
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_REMOVE_TEXTURE, OnBnClickedRemoveTexture)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SECTOR_TEXTURES_LIST, OnLvnItemchangedSectorTexturesList)
	ON_EN_CHANGE(IDC_SPLATSCALEX_EDIT, OnEnChangeSplatscalexEdit)
	ON_EN_CHANGE(IDC_SPLATSCALEY_EDIT, OnEnChangeSplatscaleyEdit)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPLATSCALEY_SPINNER, OnDeltaposSplatscaleySpinner)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPLATSCALEX_SPINNER, OnDeltaposSplatscalexSpinner)
	ON_BN_CLICKED(IDC_SPLATSCALE_PROPORTIONAL, OnBnClickedSplatscaleProportional)
END_MESSAGE_MAP()

void CTerrainLayerMaterialType::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bDragging)
	{
		// release mouse capture
		VERIFY (::ReleaseCapture());
		m_bDragging = FALSE;
		// end dragging
		VERIFY (m_pDragImage->DragLeave(GetDesktopWindow()));
		m_pDragImage->EndDrag();

		CPoint pt(point);
		ClientToScreen(&pt);
		// get the CWnd pointer of the window that is under the mouse cursor
		CWnd *pDropWnd = WindowFromPoint(pt);
		if (pDropWnd == NULL)
			return;

		// dropping from our list to our list in the same spot does nothing
		if (m_nDragIndex == m_TextureListCtrl.GetDropIndex())
			return;

		// dropping on nothing also does nothing
		if (m_TextureListCtrl.GetDropIndex() == -1)
			return;

		TERRAINSECTORSWAPLAYERS swapmsg;
		CHashString szSplatTextureNameA( m_TextureListCtrl.GetItemText(m_nDragIndex, 0) );
		CHashString szSplatTextureNameB( m_TextureListCtrl.GetItemText(m_TextureListCtrl.GetDropIndex(), 0) );
		swapmsg.pSplatTextureNameA = &szSplatTextureNameA;
		swapmsg.pSplatTextureNameB = &szSplatTextureNameB;

		static DWORD msgHash_TerrainSectorSwapLayers = CHashString(_T("TerrainSectorSwapLayers")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TerrainSectorSwapLayers, sizeof(swapmsg), &swapmsg, m_pSectorName );

		// reload layers
		SetSectorName( m_pSectorName ); 
	}

	CDialog::OnLButtonUp(nFlags, point);
}
void CTerrainLayerMaterialType::OnLvnBegindragSectorTexturesList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// save the index of the item being dragged in m_nDragIndex
	m_nDragIndex = pNMListView->iItem;
	POINT pt;
	pt.x = 8;
	pt.y = 8;
	// create a drag image

	if (m_pDragImage)
		delete m_pDragImage;

	m_pDragImage = m_TextureListCtrl.CreateDragImage(m_nDragIndex, &pt);
	ASSERT (m_pDragImage);
	VERIFY(m_pDragImage->BeginDrag(0, CPoint (8, 8)));
	VERIFY(m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction));
	// set dragging flag
	m_bDragging = true;
	// capture all mouse messages
	SetCapture();

	*pResult = 0;
}

void CTerrainLayerMaterialType::OnLvnItemchangedSectorTexturesList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	// Selected an Item
	if( pNMLV->uNewState == 3 )
	{
		GetDlgItem(IDC_SPLATSCALEX_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPLATSCALEX_SPINNER)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPLATSCALEY_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPLATSCALEY_SPINNER)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPLATSCALE_PROPORTIONAL)->EnableWindow(TRUE);
		
		CHashString hszTextureName( m_TextureListCtrl.GetItemText(pNMLV->iItem, 0) );
		TEXTURESCALEMAP::iterator itr = m_TextureListScaleMap.find( hszTextureName.GetUniqueID() );
		if (itr != m_TextureListScaleMap.end())
			m_SplatScale = itr->second;
		else
		{
			m_SplatScale.X = 1.0;
			m_SplatScale.Y = 1.0;
		}

		// send a list of resource to select in the resource bar when we hit ok
		// so that we can use the property page to select a texture to paint with
		list<StdString> msg;
		POSITION Pos = m_TextureListCtrl.GetFirstSelectedItemPosition();
		while (Pos)
		{
			// item to remove
			int nItem = m_TextureListCtrl.GetNextSelectedItem(Pos);
			msg.push_back( m_TextureListCtrl.GetItemText( nItem, 0 ).GetString() );
		}

		// only send if we have something selected
		if (msg.size() > 0)
		{
			static DWORD msgHash_SetSelectedResources = CHashString(_T("SetSelectedResources")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SetSelectedResources, sizeof(list<StdString>*), &msg );
		}

		m_iCurrentSelectedLayer = pNMLV->iItem;
	}
	else
	{
		GetDlgItem(IDC_SPLATSCALEX_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPLATSCALEX_SPINNER)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPLATSCALEY_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPLATSCALEY_SPINNER)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPLATSCALE_PROPORTIONAL)->EnableWindow(FALSE);
		m_iCurrentSelectedLayer = -1;
	}

	UpdateData(FALSE);
}

void CTerrainLayerMaterialType::SetSectorName( IHashString *pSectorName )
{
	if (pSectorName)
	{
		// clear all information first
		SetSectorName( NULL );

		m_pSectorName = pSectorName;

		// get information from the sector about what we should display
		std::vector<TERRAINLAYERINFOPARAMS::TerrainLayer_t> layerList;
		vector<TERRAINLAYERINFOPARAMS::TerrainLayerMask> layerMaskList;
		TERRAINLAYERINFOPARAMS infoParams;
		infoParams.pTerrainLayerList = &layerList;
		infoParams.pTerrainLayerMaskList = &layerMaskList;
		static DWORD msgHash_TerrainLayerInfo = CHashString(_T("TerrainLayerInfo")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TerrainLayerInfo, sizeof(infoParams), &infoParams, m_pSectorName );

		// prepare to display the info (layer list, etc.) returned from the sector
		// for each in layerList add to image control
		for (unsigned int i=0; i < layerList.size(); i++)
		{
			// insert into list control
			InsertTextureListItem(	i, 
									layerList[i].szDetailTexture->GetString(), 
									layerList[i].fDetailTextureScaleX, 
									layerList[i].fDetailTextureScaleY,
									false );
		}
	}
	else
	{
		// clear scale map
		m_TextureListScaleMap.clear();
		// no layer selected
		m_iCurrentSelectedLayer = -1;
		// clear list control
		while (m_TextureListCtrl.GetItemCount() > 0)
		{
			m_TextureListCtrl.RemoveLayer( 0 );
		}

		m_pSectorName = NULL;
	}
}
void CTerrainLayerMaterialType::InsertTextureListItem( int iPosition, const TCHAR *szFilePath, float fTextureScaleX, float fTextureScaleY,  bool bSendToTerrain )
{
	// must have a sector selected
	if (!m_pSectorName)
		return;

	// check if layer is already in list
	if (m_TextureListCtrl.ResourceExists( szFilePath ))
		return;

	// check for max layer count allowed
	if (m_TextureListCtrl.GetItemCount() >= m_pEditor->GetMaxLayers())
	{
		CString text;
		text.LoadString( IDS_STATUS_LAYER_LIMIT_REACHED );
		m_StatusText.SetWindowText( text );
		return;
	}
	else
	{
		m_StatusText.SetWindowText( _T("") );
	}

	// get item name at insertion location before we insert the new layer
	CHashString hszInsertLocationName(_T(""));
	if (iPosition > -1)
	{
		hszInsertLocationName = m_TextureListCtrl.GetItemText(iPosition, 0);
	}

	bool bInserted = m_TextureListCtrl.InsertLayer( iPosition, szFilePath );
	// check for insertion failed
	if (!bInserted)
		return;

	// keep scale map updated
	FLOATPAIR tmpFloatPair;
	tmpFloatPair.X = fTextureScaleX;
	tmpFloatPair.Y = fTextureScaleY;
	CHashString hszTextureName((TCHAR*)szFilePath);
	m_TextureListScaleMap[hszTextureName.GetUniqueID()] = tmpFloatPair;

	if (bSendToTerrain)
	{
		m_pEditor->InsertTerrainSectorLayer( m_pSectorName, &hszTextureName, &hszInsertLocationName, tmpFloatPair.X, tmpFloatPair.Y );
	}
}

void CTerrainLayerMaterialType::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bDragging)
	{
		CPoint pt(point);
		ClientToScreen(&pt);
		// move the drag image
		VERIFY(m_pDragImage->DragMove(pt));
		// unlock window updates
		VERIFY(m_pDragImage->DragShowNolock(FALSE));

		// save current window pointer
		CWnd* pDropWnd = WindowFromPoint(pt);

		// convert from screen coordinates to drop target client coordinates
		pDropWnd->ScreenToClient(&pt);

		// send drag message to the window we are under
		DRAGOBJECTINFO doInfo;
		doInfo.xy = pt;
		pDropWnd->SendMessage(ResourceDragMessage, 0, (LPARAM)&doInfo);

		// lock window updates
		VERIFY(m_pDragImage->DragShowNolock(TRUE));
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CTerrainLayerMaterialType::OnBnClickedRemoveTexture()
{
	POSITION Pos = m_TextureListCtrl.GetFirstSelectedItemPosition();

	CUndoCommandGuard undoCommand;
	CHashString hszROName = CTerrainEditor::GetSectorRenderObject(m_pSectorName);
	CObjectsStateUndo objectUndo(&hszROName);

	while (Pos)
	{
		// item to remove
		int nItem = m_TextureListCtrl.GetNextSelectedItem(Pos);

		// send message to sector to remove layer
		CHashString hszSplatTextureName( m_TextureListCtrl.GetItemText(nItem, 0) );
		static DWORD msgHash_TerrainSectorRemoveLayer = CHashString(_T("TerrainSectorRemoveLayer")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_TerrainSectorRemoveLayer, sizeof(IHashString*), &hszSplatTextureName, m_pSectorName);

		m_TextureListCtrl.RemoveLayer( nItem );
	}
}

void CTerrainLayerMaterialType::OnEnChangeSplatscalexEdit()
{
	CEdit *pEdit = (CEdit*)GetDlgItem( IDC_SPLATSCALEX_EDIT );
	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	pEdit->GetWindowText( szBuffer, 256 );
	m_SplatScale.X = (float)atof( StdString(szBuffer) );

	//TODO: Add Proportional Code
	if (m_iCurrentSelectedLayer >= 0)
	{
		// keep map updated
		CHashString hszTextureName( m_TextureListCtrl.GetItemText(m_iCurrentSelectedLayer, 0) );
		m_TextureListScaleMap[hszTextureName.GetUniqueID()] = m_SplatScale;

		// send message to sector
		CHashString szAffectedLayer( m_TextureListCtrl.GetItemText( m_iCurrentSelectedLayer, 0 ) );
		TERRAINSECTORSETSTATEPARAMS sectorstate;
		sectorstate.m_iValidFields = SECTORSTATE_LAYERSCALE;
		sectorstate.m_pAffectedLayer = &szAffectedLayer;
		sectorstate.m_fLayerScaleX = m_SplatScale.X;
		sectorstate.m_fLayerScaleY = m_SplatScale.Y;
		static DWORD msgHash_TerrainSectorSetState = CHashString(_T("TerrainSectorSetState")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TerrainSectorSetState, sizeof(sectorstate), &sectorstate, m_pSectorName  );
	}
}
void CTerrainLayerMaterialType::OnDeltaposSplatscaleySpinner(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	m_SplatScale.Y += (float)pNMUpDown->iDelta * 0.1f;
	//TODO: Add Proportional Code
	if( m_iSplatScaleProportional )
	{
		m_SplatScale.X += (float)pNMUpDown->iDelta * 0.1f;
	}

	*pResult = 0;
	UpdateData(FALSE);
}
void CTerrainLayerMaterialType::OnDeltaposSplatscalexSpinner(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	m_SplatScale.X += (float)pNMUpDown->iDelta * 0.1f;
	//TODO: Add Proportional Code
	if( m_iSplatScaleProportional )
	{
		m_SplatScale.Y += (float)pNMUpDown->iDelta * 0.1f;
	}

	*pResult = 0;
	UpdateData(FALSE);
}
void CTerrainLayerMaterialType::OnBnClickedSplatscaleProportional()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}
void CTerrainLayerMaterialType::OnEnChangeSplatscaleyEdit()
{
	CEdit *pEdit = (CEdit*)GetDlgItem( IDC_SPLATSCALEY_EDIT );
	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	pEdit->GetWindowText( szBuffer, 256 );
	m_SplatScale.Y = (float)atof( StdString(szBuffer) );

	//TODO: Add Proportional Code

	if (m_iCurrentSelectedLayer >= 0)
	{
		// keep map updated
		CHashString hszTextureName( m_TextureListCtrl.GetItemText(m_iCurrentSelectedLayer, 0) );
		m_TextureListScaleMap[hszTextureName.GetUniqueID()] = m_SplatScale;

		// send message to sector
		CHashString szAffectedLayer( m_TextureListCtrl.GetItemText( m_iCurrentSelectedLayer, 0 ) );
		TERRAINSECTORSETSTATEPARAMS sectorstate;
		sectorstate.m_iValidFields = SECTORSTATE_LAYERSCALE;
		sectorstate.m_pAffectedLayer = &szAffectedLayer;
		sectorstate.m_fLayerScaleX = m_SplatScale.X;
		sectorstate.m_fLayerScaleY = m_SplatScale.Y;
		static DWORD msgHash_TerrainSectorSetState = CHashString(_T("TerrainSectorSetState")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TerrainSectorSetState, sizeof(sectorstate), &sectorstate, m_pSectorName  );
	}
}

void CTerrainLayerMaterialType::OnSize(UINT nType, int cx, int cy)
{
	AdjustControlSize(&m_TextureListCtrl, cx);
	AdjustControlSize(GetDlgItem(IDC_MATERIAL_TYPE), cx);
	AdjustControlSize(GetDlgItem(IDC_REMOVE_TEXTURE), cx);
	AdjustControlSize(&m_StatusText, cx);
}

void CTerrainLayerMaterialType::AdjustControlSize(CWnd *pControl, int cx)
{
	if (pControl != NULL && ::IsWindow(*pControl))
	{
		CRect rc;
		pControl->GetWindowRect(rc);
		ScreenToClient(rc);
		int width = max(rc.left, cx - 2 * rc.left);
		pControl->SetWindowPos(NULL, 0, 0, width, rc.Height(), SWP_NOZORDER | SWP_NOMOVE);
	}
}
