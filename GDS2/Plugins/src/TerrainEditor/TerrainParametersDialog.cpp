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

#define TERRAIN_PARAMS_DIALOG_PREVIEW_TEXTURE "ter_params_dial_prev_tex"
#define PREVIEW_WINDOW_RENDER_SIZE 128

// CTerrainParametersDialog dialog
IMPLEMENT_DYNAMIC(CTerrainParametersDialog, CDialog)
CTerrainParametersDialog::CTerrainParametersDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTerrainParametersDialog::IDD, pParent)
	, m_iMinHeight(0)
	, m_iMaxHeight(0)
{
	m_ToolBox = EngineGetToolBox();
	m_pSectorName = NULL;
	m_iAccumMask = 0;
	m_PreviewTexture = NULL;
	m_iCurrentSelectedLayer = -1;
}

CTerrainParametersDialog::~CTerrainParametersDialog()
{
	if (m_PreviewTexture)
	{
		// remove texture by name in case the texture has already been removed elsewhere
		CHashString namestring(_T(TERRAIN_PARAMS_DIALOG_PREVIEW_TEXTURE));
		TEXTUREOBJECTPARAMS tobj;
		tobj.bLoad = false;
		tobj.Name = &namestring;
		tobj.TextureObjectInterface = NULL;
		static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
		m_PreviewTexture = NULL;
	}
}

void CTerrainParametersDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_PIC, m_PreviewControl);
	DDX_Check( pDX, IDC_ACCUM_MASK, m_iAccumMask );
	DDX_Control(pDX, IDC_GEN_SLOPE, m_GenSlope);
	DDX_Control(pDX, IDC_EDIT_MINHEIGHT, m_MinHeightEdit);
	DDX_Control(pDX, IDC_EDIT_MAXHEIGHT, m_MaxHeightEdit);
	DDX_Control(pDX, IDC_SLIDER_MINSLOPE, m_MinSlopeSlider);
	DDX_Control(pDX, IDC_SLIDER_MAXSLOPE, m_MaxSlopeSlider);
	DDX_Text(pDX, IDC_EDIT_MINHEIGHT, m_iMinHeight);
	DDX_Text(pDX, IDC_EDIT_MAXHEIGHT, m_iMaxHeight);
	DDX_Control(pDX, IDC_GEN_TEXTURES_LIST, m_TextureListControl);
}


BEGIN_MESSAGE_MAP(CTerrainParametersDialog, CDialog)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_GENERATE_TEXTURE, OnBnClickedGenerateTexture)
	ON_EN_KILLFOCUS(IDC_EDIT_MINHEIGHT, OnEnKillfocusEditMinheight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MINHEIGHT, OnDeltaposSpinMinheight)
	ON_EN_KILLFOCUS(IDC_EDIT_MAXHEIGHT, OnEnKillfocusEditMaxheight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MAXHEIGHT, OnDeltaposSpinMaxheight)
	ON_EN_CHANGE(IDC_EDIT_MINHEIGHT, OnEnChangeEditMinheight)
	ON_EN_CHANGE(IDC_EDIT_MAXHEIGHT, OnEnChangeEditMaxheight)
	ON_BN_CLICKED(IDC_ACCUM_MASK, OnBnClickedAccumMask)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MAXSLOPE, OnNMCustomdrawSliderMaxslope)
	ON_BN_CLICKED(IDC_ADD_LAYER, OnBnClickedAddLayer)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_GEN_TEXTURES_LIST, OnLvnItemchangedGenTexturesList)
	ON_BN_CLICKED(IDC_REMOVE_GEN_LAYER, OnBnClickedRemoveGenLayer)
END_MESSAGE_MAP()


// CTerrainParametersDialog message handlers

BOOL CTerrainParametersDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetRightPartEnableType();

	m_MinSlopeSlider.SetRange( 0, 100 );
	m_MaxSlopeSlider.SetRange( 0, 100 );
	m_MaxSlopeSlider.SetPos( 100 );

	//Initialize the preview window texture:
	CREATETEXTUREPARAMS createtex;	
	//create the cached texture
	createtex.bitDepth = 24;
	CHashString namestring(_T(TERRAIN_PARAMS_DIALOG_PREVIEW_TEXTURE));
	createtex.Name = &namestring;
	createtex.sizeX = PREVIEW_WINDOW_RENDER_SIZE;
	createtex.sizeY = PREVIEW_WINDOW_RENDER_SIZE;
	static DWORD msgHash_CreateTexture = CHashString(_T("CreateTexture")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateTexture, sizeof(createtex), &createtex );
	m_PreviewTexture = dynamic_cast< ITextureObject * >( createtex.TextureObjectInterface );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTerrainParametersDialog::GeneratePreview()
{
	RENDERLISTTOTEXTUREPARAMS params;
	unsigned char * mapbits;
	if( m_PreviewTexture && m_pSectorName)
	{
		static DWORD msgHash_GeneratePreviewOfSectorRenderObject = CHashString(_T("GeneratePreviewOfSectorRenderObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GeneratePreviewOfSectorRenderObject, sizeof( ITextureObject*), m_PreviewTexture, m_pSectorName );

		//extract the bitmap and blit
		//create the bitmap from the texture data:
		mapbits = (unsigned char*)m_PreviewTexture->Read();
		//flip bits to me in windows format?
		unsigned char r, b;
		for( int i = 0; i < PREVIEW_WINDOW_RENDER_SIZE*PREVIEW_WINDOW_RENDER_SIZE; i++ )
		{
			r = mapbits[ i*3 ];
			b = mapbits[ i*3 + 2 ];
			mapbits[ i*3 ] = b;
			mapbits[ i*3 + 2 ] = r;
		}
		HBITMAP bitmap = CreateBitmap( PREVIEW_WINDOW_RENDER_SIZE, PREVIEW_WINDOW_RENDER_SIZE, 1, 24, (void*)mapbits );
		//stretch the bitmap to be the size of the static
		SetPreviewBitmap( bitmap );
		FreeResource( bitmap );
	}
}

void CTerrainParametersDialog::SetPreviewBitmap( HBITMAP bitmap )
{
	CDC memDC;
	CDC srcMemDC;
	CBitmap *tBitmap;
	RECT staticSize;	

	// create a memory DC to blit into
	m_PreviewControl.GetClientRect( &staticSize );
	CDC *pDC = m_PreviewControl.GetDC();
	memDC.CreateCompatibleDC(pDC);
	tBitmap = new CBitmap();
	tBitmap->CreateDiscardableBitmap(pDC, staticSize.right, staticSize.bottom);

	CBitmap *oldBitmap = memDC.SelectObject(tBitmap);

	srcMemDC.CreateCompatibleDC(pDC);
	srcMemDC.SelectObject( bitmap );
	memDC.StretchBlt(0, 0, staticSize.right, staticSize.bottom, 
		&srcMemDC, 0, 0, PREVIEW_WINDOW_RENDER_SIZE, PREVIEW_WINDOW_RENDER_SIZE, SRCCOPY); 
	memDC.SelectObject(oldBitmap);

	m_PreviewControl.ReleaseDC(pDC);
	//Now set the preview window with our new stretched bitmap
	HBITMAP setbitmap = (HBITMAP)tBitmap->GetSafeHandle();	
	m_PreviewControl.SetBitmap( setbitmap );
	delete tBitmap;
}

void CTerrainParametersDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//GeneratePreview();
	UpdateData( TRUE );
	OnOK();
}

void CTerrainParametersDialog::OnOK()
{
	ShowWindow(SW_HIDE);
}

void CTerrainParametersDialog::OnBnClickedCancel()
{
	ShowWindow(SW_HIDE);
}

void CTerrainParametersDialog::OnBnClickedGenerateTexture()
{
	if (-1 == m_iCurrentSelectedLayer)
		return;
	
	if (m_GenSlope.GetCheck())
	{
		float fMinSlope = (float)m_MinSlopeSlider.GetPos() / 100.0f;
		float fMaxSlope = (float)m_MaxSlopeSlider.GetPos() / 100.0f;

		TERRAINSECTORMODIFYLAYERMASK tsmlm;
		//?tsmlm.iLayer = m_iCurrentSelectedLayer;
		tsmlm.iLayerMaskGenMode = TERRAIN_LAYERMASK_GEN_SLOPE;
		tsmlm.fMin = fMinSlope;
		tsmlm.fMax = fMaxSlope;
		tsmlm.bAccumMask = m_iAccumMask != 0;
		static DWORD msgHash_TerrainSectorModifyLayerMask = CHashString(_T("TerrainSectorModifyLayerMask")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TerrainSectorModifyLayerMask, sizeof(tsmlm), &tsmlm, m_pSectorName );
	}
	else
	{
		TERRAINSECTORMODIFYLAYERMASK tsmlm;
		//?tsmlm.iLayer = m_iCurrentSelectedLayer;
		tsmlm.iLayerMaskGenMode = TERRAIN_LAYERMASK_GEN_HEIGHT;
		tsmlm.fMin = (float)m_iMinHeight;
		tsmlm.fMax = (float)m_iMaxHeight;
		tsmlm.bAccumMask = m_iAccumMask != 0;
		static DWORD msgHash_TerrainSectorModifyLayerMask = CHashString(_T("TerrainSectorModifyLayerMask")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TerrainSectorModifyLayerMask, sizeof(tsmlm), &tsmlm, m_pSectorName );
	}
}


void CTerrainParametersDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// make sure sliders can't cross over eachother
	if (m_MinSlopeSlider.GetPos() > m_MaxSlopeSlider.GetPos())
		m_MaxSlopeSlider.SetPos( m_MinSlopeSlider.GetPos() );
	if (m_MaxSlopeSlider.GetPos() < m_MinSlopeSlider.GetPos())
		m_MinSlopeSlider.SetPos( m_MaxSlopeSlider.GetPos() );
}

void CTerrainParametersDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// make sure sliders can't cross over eachother
	if (m_MinSlopeSlider.GetPos() > m_MaxSlopeSlider.GetPos())
		m_MinSlopeSlider.SetPos( m_MaxSlopeSlider.GetPos() );
	if (m_MaxSlopeSlider.GetPos() < m_MinSlopeSlider.GetPos())
		m_MaxSlopeSlider.SetPos( m_MinSlopeSlider.GetPos() );
}

void CTerrainParametersDialog::OnEnKillfocusEditMinheight()
{
	int iHalfHeight = 999999;
	TERRAINDESCRIPTOR terrainDesc;
	static DWORD msgHash_GetTerrainDescriptor = CHashString(_T("GetTerrainDescriptor")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetTerrainDescriptor, sizeof(terrainDesc), &terrainDesc ) == MSG_HANDLED)
		iHalfHeight = (int)(terrainDesc.ZScale * 0.5f);

	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	m_MinHeightEdit.GetWindowText( szBuffer, 256 );
	m_iMinHeight = atoi( StdString(szBuffer) );
	m_iMinHeight = Clamp( m_iMinHeight, -iHalfHeight, m_iMaxHeight );

	_stprintf( szBuffer, "%i", m_iMinHeight );
	m_MinHeightEdit.SetWindowText( szBuffer );
}

void CTerrainParametersDialog::OnDeltaposSpinMinheight(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	*pResult = 0;

	int iHalfHeight = 999999;
	TERRAINDESCRIPTOR terrainDesc;
	static DWORD msgHash_GetTerrainDescriptor = CHashString(_T("GetTerrainDescriptor")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetTerrainDescriptor, sizeof(terrainDesc), &terrainDesc ) == MSG_HANDLED)
		iHalfHeight = (int)(terrainDesc.ZScale * 0.5f);

	m_iMinHeight = m_iMinHeight - pNMUpDown->iDelta;
	m_iMinHeight = Clamp( m_iMinHeight, -iHalfHeight, m_iMaxHeight );

	UpdateData(FALSE);
}

void CTerrainParametersDialog::OnEnKillfocusEditMaxheight()
{
	int iHalfHeight = 999999;
	TERRAINDESCRIPTOR terrainDesc;
	static DWORD msgHash_GetTerrainDescriptor = CHashString(_T("GetTerrainDescriptor")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetTerrainDescriptor, sizeof(terrainDesc), &terrainDesc ) == MSG_HANDLED)
		iHalfHeight = (int)(terrainDesc.ZScale * 0.5f);

	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	m_MaxHeightEdit.GetWindowText( szBuffer, 256 );
	m_iMaxHeight = atoi( StdString(szBuffer) );
	m_iMaxHeight = Clamp( m_iMaxHeight, m_iMinHeight, iHalfHeight );

	_stprintf( szBuffer, "%i", m_iMaxHeight );
	m_MaxHeightEdit.SetWindowText( szBuffer );
}

void CTerrainParametersDialog::OnDeltaposSpinMaxheight(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	*pResult = 0;

	int iHalfHeight = 999999;
	TERRAINDESCRIPTOR terrainDesc;
	static DWORD msgHash_GetTerrainDescriptor = CHashString(_T("GetTerrainDescriptor")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetTerrainDescriptor, sizeof(terrainDesc), &terrainDesc ) == MSG_HANDLED)
		iHalfHeight = (int)(terrainDesc.ZScale * 0.5f);

	m_iMaxHeight = m_iMaxHeight - pNMUpDown->iDelta ;
	m_iMaxHeight = Clamp( m_iMaxHeight, m_iMinHeight, iHalfHeight );

	UpdateData(FALSE);
}

void CTerrainParametersDialog::OnEnChangeEditMinheight()
{
}

void CTerrainParametersDialog::OnEnChangeEditMaxheight()
{
}

void CTerrainParametersDialog::OnBnClickedAccumMask()
{
	UpdateData(TRUE);
}

void CTerrainParametersDialog::OnNMCustomdrawSliderMaxslope(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CTerrainParametersDialog::OnBnClickedAddLayer()
{
	IToolBox * pToolBox = EngineGetToolBox();
	if( pToolBox )
	{
		StdString BaseDirectory;
		StdString ResourcePath;
		pToolBox->GetDirectories( &BaseDirectory, &ResourcePath );
		COpenTextureFileDialog FileOpenDlg(ResourcePath, this, _T("Select Texture"));
		m_TextureListControl.InsertLayer(-1, FileOpenDlg.GetFileName());
		UpdateData(false);
		return;
	}
}

void CTerrainParametersDialog::OnLvnItemchangedGenTexturesList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	// Selected an Item
	
	if( pNMLV->uNewState == 3 )
	{
		SetRightPartEnableType(TRUE);
		m_iCurrentSelectedLayer = pNMLV->iItem;
	}
	else
	{
		SetRightPartEnableType();
		m_iCurrentSelectedLayer = -1;
	}

	UpdateData(FALSE);
}

void CTerrainParametersDialog::OnBnClickedRemoveGenLayer()
{
	POSITION Pos = m_TextureListControl.GetFirstSelectedItemPosition();
	while (Pos)
	{
		// item to remove
		int nItem = m_TextureListControl.GetNextSelectedItem(Pos);
		m_TextureListControl.RemoveLayer(nItem);
	}
}

void CTerrainParametersDialog::SetRightPartEnableType(BOOL type)
{
	GetDlgItem(IDC_EDIT_MINHEIGHT)->EnableWindow(type);
	GetDlgItem(IDC_SPIN_MINHEIGHT)->EnableWindow(type);
	GetDlgItem(IDC_EDIT_MAXHEIGHT)->EnableWindow(type);
	GetDlgItem(IDC_SPIN_MAXHEIGHT)->EnableWindow(type);
	GetDlgItem(IDC_GEN_SLOPE)->EnableWindow(type);
	GetDlgItem(IDC_GEN_HEIGHT)->EnableWindow(type);
	GetDlgItem(IDC_ACCUM_MASK)->EnableWindow(type);
	m_MinSlopeSlider.EnableWindow(type);
	m_MaxSlopeSlider.EnableWindow(type);
	GetDlgItem(IDC_GENERATE_TEXTURE)->EnableWindow(type);
}
