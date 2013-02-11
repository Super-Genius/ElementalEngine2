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

#define DEFAULT_TERRAIN_WIDTH 4
#define DEFAULT_TERRAIN_HEIGHT 4
#define DEFAULT_TERRAIN_SECTOR_SIZE 125.0f
#define DEFAULT_TERRAIN_ZSCALE 1.f
#define DEFAULT_TERRAIN_BASE_TEXTURE _T("Textures\\Terrain\\ter_grass2_base4.dds")
#define DEFAULT_TERRAIN_TEXTURE_SCALE 32.f
#define	DEFAULT_TRIANGLE_RESOLUTION 32

// CTerrainSizeDlg dialog

IMPLEMENT_DYNAMIC(CTerrainSizeDlg, CDialog)
CTerrainSizeDlg::CTerrainSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTerrainSizeDlg::IDD, pParent),
	m_TerrainWidth( DEFAULT_TERRAIN_WIDTH ),
	m_TerrainHeight( DEFAULT_TERRAIN_HEIGHT ),
	m_SectorSize( DEFAULT_TERRAIN_SECTOR_SIZE ),
	m_TerrainName(_T("DefaultTerrain")),
	m_SectorZScale( DEFAULT_TERRAIN_ZSCALE ),
	m_TerrainBaseTexture( DEFAULT_TERRAIN_BASE_TEXTURE ),
	m_BaseTextureScaleY(DEFAULT_TERRAIN_TEXTURE_SCALE),
	m_BaseTextureScaleX(DEFAULT_TERRAIN_TEXTURE_SCALE),
	m_TriResolution(DEFAULT_TRIANGLE_RESOLUTION)
{
	m_pEditor = SINGLETONINSTANCE(CTerrainEditor);
}

CTerrainSizeDlg::~CTerrainSizeDlg()
{
}

void CTerrainSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TERRAIN_WIDTH, m_TerrainWidth);
	DDX_Text(pDX, IDC_EDIT_TERRAIN_HEIGHT, m_TerrainHeight);
	DDX_Text(pDX, IDC_EDIT_SECTOR_WIDTH, m_SectorSize);
	DDX_Text(pDX, IDC_TERRAIN_NAME, m_TerrainName);
	DDX_Text(pDX, IDC_EDIT_SECTOR_ZSCALE, m_SectorZScale);
	DDX_Text(pDX, IDC_TERRAIN_BASE_TEXTURE, m_TerrainBaseTexture);
	DDX_Control(pDX, IDC_CHECK_CENTER_NW, m_ButtonLeftUp);
	DDX_Control(pDX, IDC_CHECK_CENTER_N, m_ButtonUp);
	DDX_Control(pDX, IDC_CHECK_CENTER_NE, m_ButtonRightUp);
	DDX_Control(pDX, IDC_CHECK_CENTER_E, m_ButtonRight);
	DDX_Control(pDX, IDC_CHECK_CENTER_W, m_ButtonLeft);
	DDX_Control(pDX, IDC_CHECK_CENTER, m_ButtonMiddle);
	DDX_Control(pDX, IDC_CHECK_CENTER_SW, m_ButtonLeftDown);
	DDX_Control(pDX, IDC_CHECK_CENTER_S, m_ButtonDown);
	DDX_Control(pDX, IDC_CHECK_CENTER_SE, m_ButtonRightDown);
	DDX_Text(pDX, IDC_TEXTURESCALEY, m_BaseTextureScaleY);
	DDX_Text(pDX, IDC_TEXTURESCALEX, m_BaseTextureScaleX);
	DDX_Text(pDX, IDC_TRI_RESOLUTION, m_TriResolution);
	DDX_Control(pDX, IDC_MEASUREMENT_UNITS, m_MeasurementUnits);
}


BEGIN_MESSAGE_MAP(CTerrainSizeDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_TERRAIN_TEXTURE_BROWSE, OnBnClickedTerrainTextureBrowse)
	ON_WM_CREATE()
	ON_BN_CLICKED(ID_MODIFYCURRENT, OnBnClickedModifyCurrentTerrain)
	ON_BN_CLICKED(IDC_CHECK_CENTER_NW, OnBnClickedCheckCenterNw)
	ON_BN_CLICKED(IDC_CHECK_CENTER_N, OnBnClickedCheckCenterN)
	ON_BN_CLICKED(IDC_CHECK_CENTER_NE, OnBnClickedCheckCenterNe)
	ON_BN_CLICKED(IDC_CHECK_CENTER_W, OnBnClickedCheckCenterW)
	ON_BN_CLICKED(IDC_CHECK_CENTER, OnBnClickedCheckCenter)
	ON_BN_CLICKED(IDC_CHECK_CENTER_E, OnBnClickedCheckCenterE)
	ON_BN_CLICKED(IDC_CHECK_CENTER_SW, OnBnClickedCheckCenterSw)
	ON_BN_CLICKED(IDC_CHECK_CENTER_S, OnBnClickedCheckCenterS)
	ON_BN_CLICKED(IDC_CHECK_CENTER_SE, OnBnClickedCheckCenterSe)
	ON_BN_CLICKED(ID_IMPORT, OnBnClickedImport)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SECTOR_WIDTH, OnDeltaposSpinSectorWidth)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SECTOR_ZSCALE, OnDeltaposSpinSectorZscale)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SECTOR_TRIRES, OnDeltaposSpinSectorTrires)
	ON_EN_KILLFOCUS(IDC_TRI_RESOLUTION, OnEnKillfocusTriResolution)
	ON_EN_KILLFOCUS(IDC_EDIT_SECTOR_WIDTH, OnEnKillfocusEditSectorWidth)
	ON_EN_KILLFOCUS(IDC_EDIT_SECTOR_ZSCALE, OnEnKillfocusEditSectorZscale)
	ON_EN_KILLFOCUS(IDC_EDIT_TERRAIN_HEIGHT, OnEnKillfocusEditTerrainHeight)
	ON_EN_KILLFOCUS(IDC_EDIT_TERRAIN_WIDTH, OnEnKillfocusEditTerrainWidth)
	ON_EN_KILLFOCUS(IDC_TEXTURESCALEX, OnEnKillfocusTexturescalex)
	ON_EN_KILLFOCUS(IDC_TEXTURESCALEY, OnEnKillfocusTexturescaley)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TERRAIN_HEIGHT, OnDeltaposSpinTerrainHeight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TERRAIN_WIDTH, OnDeltaposSpinTerrainWidth)
END_MESSAGE_MAP()

float CTerrainSizeDlg::ConvertToMeters(float srcSize)
{
	float outSize;
	switch (m_MeasurementUnits.GetCurSel())
	{
		case KILOMETERS:
			outSize = srcSize / 1000.0f;
			break;
		case METERS:
			outSize = srcSize;
			break;
		case CENTIMETERS:
			outSize = srcSize * 100.0f;
			break;
		case MILES:
			outSize = srcSize / 1609.3439486f;
			break;
		case FEET:
			outSize = srcSize / 3.28084f;
			break;
		case INCHES:
			outSize = srcSize / 39.3701f;
			break;
	}

	return outSize;
}

bool CTerrainSizeDlg::CheckValidData()
{
	//check if it's valid and if it's a relative path to the base data directory
	UpdateData(TRUE);
	//check 0 for terrain sizes:
	if (   m_TerrainWidth == 0 
		|| m_TerrainHeight == 0 
		|| m_SectorSize == 0
		|| m_TerrainName.IsEmpty() 
	   )
	{
		return false;
	}
	return true;
}

// CTerrainSizeDlg message handlers
void CTerrainSizeDlg::OnBnClickedOk()
{
	// Create the terrain using the given parameters:
    UpdateData(TRUE);
	//Check if everything is valid, otherwise don't go through with the OK
	if( CheckValidData() )
	{
		//Ok everything is valid
		CreateTerrain();
		OnOK();
	}
}

void CTerrainSizeDlg::OnBnClickedCancel()
{
	//Uncheck the other check boxes
	OnCancel();
}

void CTerrainSizeDlg::CreateTerrain()
{
	IToolBox * pToolBox = EngineGetToolBox();

	UpdateData(TRUE);

	// create a blank scene if necessary
	static DWORD msgHash_CreateBlankScene = CHashString(_T("CreateBlankScene")).GetUniqueID();
	pToolBox->SendMessage(msgHash_CreateBlankScene, 0, NULL);

	// clear the sector modified map.
	m_pEditor->OnWorldModified(false);

	TERRAINCREATIONPARAMS terraincreate;
	FillTerrainCreationParam(&terraincreate);
	if (terraincreate.TerrainPath.empty())
	{
		terraincreate.TerrainPath = m_TerrainName;
	}
	if (terraincreate.TerrainName.empty())
	{
		terraincreate.TerrainName = _T("Maps\\") + terraincreate.TerrainPath + _T("\\");
	}


	if( pToolBox )
	{
		//Temp: Pause thumbnail thread:
		PauseThumbnailGeneration(true);
		//End Temp
		
		CUndoCommandGuard undoCommand;

		m_pEditor->DoAllSectorsUndo(true);

		static DWORD msgHash_TerrainManagerCreateTerrain = CHashString(_T("TerrainManagerCreateTerrain")).GetUniqueID();
		pToolBox->SendMessage(msgHash_TerrainManagerCreateTerrain, sizeof( TERRAINCREATIONPARAMS ), &terraincreate );

		m_pEditor->DoAllSectorsUndo(false);

		//Temp: Unpause thumbnail thread
		PauseThumbnailGeneration(false);
		//End Temp

		static DWORD msgHash_WorldModified = CHashString(_T("WorldModified")).GetUniqueID();
		pToolBox->SendMessage(msgHash_WorldModified, sizeof(BOOL), (void *)true);

		m_pEditor->SetAllSectorsModified();
	}
}

void CTerrainSizeDlg::UpdateTerrainData()
{
	//try to get the current loaded terrain params:
	TERRAINDESCRIPTOR terrainparams;	
	if( GetTerrainDescriptor(&terrainparams) )
	{
		//set vars
		m_SectorZScale = terrainparams.ZScale;
		m_TerrainWidth = terrainparams.TerrainWidth;
		m_TerrainHeight = terrainparams.TerrainHeight;
		m_SectorSize = terrainparams.SectorSize;
		m_TerrainBaseTexture = (const TCHAR*)terrainparams.TerrainBaseTexture;
		m_BaseTextureScaleY = terrainparams.BaseTextureScaleY;
		m_BaseTextureScaleX = terrainparams.BaseTextureScaleX;
		UpdateData( FALSE );
	}
}

BOOL CTerrainSizeDlg::OnInitDialog()
{
	if( !CDialog::OnInitDialog() )
	{
		return FALSE;
	}

	UpdateTerrainData();
	m_ButtonMiddle.SetCheck( BST_CHECKED );
	m_LastButtonChecked = &m_ButtonMiddle;
	m_CurrentButtonChecked = &m_ButtonMiddle;
	//Load Icons
	m_ArrowIcons[0][0] = (HICON)LoadImage(TerrainEditorDLL.hModule, MAKEINTRESOURCE(IDI_ARROW_LEFT_UP), IMAGE_ICON, 32, 32, 0);
	m_ArrowIcons[0][1] = (HICON)LoadImage(TerrainEditorDLL.hModule, MAKEINTRESOURCE(IDI_ARROW_UP), IMAGE_ICON, 32, 32, 0);
	m_ArrowIcons[0][2] = (HICON)LoadImage(TerrainEditorDLL.hModule, MAKEINTRESOURCE(IDI_ARROW_RIGHT_UP), IMAGE_ICON, 32, 32, 0);
	m_ArrowIcons[1][0] = (HICON)LoadImage(TerrainEditorDLL.hModule, MAKEINTRESOURCE(IDI_ARROW_LEFT), IMAGE_ICON, 32, 32, 0);
	m_ArrowIcons[1][1] = (HICON)LoadImage(TerrainEditorDLL.hModule, MAKEINTRESOURCE(IDI_ARROW_CENTER1), IMAGE_ICON, 32, 32, 0);
	m_ArrowIcons[1][2] = (HICON)LoadImage(TerrainEditorDLL.hModule, MAKEINTRESOURCE(IDI_ARROW_RIGHT), IMAGE_ICON, 32, 32, 0);
	m_ArrowIcons[2][0] = (HICON)LoadImage(TerrainEditorDLL.hModule, MAKEINTRESOURCE(IDI_ARROW_LEFT_DOWN), IMAGE_ICON, 32, 32, 0);
	m_ArrowIcons[2][1] = (HICON)LoadImage(TerrainEditorDLL.hModule, MAKEINTRESOURCE(IDI_ARROW_DOWN), IMAGE_ICON, 32, 32, 0);
	m_ArrowIcons[2][2] = (HICON)LoadImage(TerrainEditorDLL.hModule, MAKEINTRESOURCE(IDI_ARROW_RIGHT_DOWN), IMAGE_ICON, 32, 32, 0);
	m_IconBlank = (HICON)LoadImage(TerrainEditorDLL.hModule, MAKEINTRESOURCE(IDI_ARROW_BLANK), IMAGE_ICON, 32, 32, 0);
	OnBnClickedCheckCenter();
	UpdateData(false);
	m_MeasurementUnits.SetCurSel(METERS);
	return TRUE;
}

void CTerrainSizeDlg::OnBnClickedTerrainTextureBrowse()
{
	IToolBox * pToolBox = EngineGetToolBox();
	if( pToolBox )
	{
		StdString ResourcePath;
		pToolBox->GetDirectories( NULL, &ResourcePath );
		COpenTextureFileDialog FileOpenDlg(ResourcePath, this);
		m_TerrainBaseTexture = FileOpenDlg.GetFileName().c_str();
		UpdateData(false);
	}
}

void CTerrainSizeDlg::OnBnClickedModifyCurrentTerrain()
{
	UpdateData(TRUE);

	//modify the current terrain with parameters
	IToolBox * pToolBox = EngineGetToolBox();

	TERRAINDESCRIPTOR terrainDesc;
	if (!GetTerrainDescriptor(&terrainDesc))
	{
		terrainDesc.SectorResolution = 16; // conservative default terrain resolution
	}

	bool bTerrainSizeIsIncreasing = (terrainDesc.TerrainHeight < m_TerrainHeight) ||
									(terrainDesc.TerrainWidth < m_TerrainWidth);
	if (m_TerrainBaseTexture.IsEmpty() && bTerrainSizeIsIncreasing)
	{
		MessageBox(_T("Base Texture field is empty! Please find a base texture to apply to new sectors."), _T("Error!"), MB_OK|MB_ICONEXCLAMATION); 
		return;
	}

	//modify the current terrain with parameters
	TERRAINCREATIONPARAMS terraincreate;
	FillTerrainCreationParam(&terraincreate);

	terraincreate.PositiveResizeX =(( ResizeStart[0] -1 ) > 0 );
	terraincreate.PositiveResizeY =(( ResizeStart[1] -1 ) > 0 );		
	terraincreate.NegativeResizeX =(( ResizeStart[0] -1 ) < 0 );
	terraincreate.NegativeResizeY =(( ResizeStart[1] -1 ) < 0 );

	//Temp: Pause thumbnail thread:
	PauseThumbnailGeneration(true);
	//End Temp

	CUndoCommandGuard undoCommand;

	m_pEditor->DoAllSectorsUndo(true);

	static DWORD msgHash_TerrainManagerModifyTerrain = CHashString(_T("TerrainManagerModifyTerrain")).GetUniqueID();
	pToolBox->SendMessage(msgHash_TerrainManagerModifyTerrain, sizeof( TERRAINCREATIONPARAMS ), &terraincreate);

	m_pEditor->DoAllSectorsUndo(false);

	//Temp: Unpause thumbnail thread
	PauseThumbnailGeneration(false);
	//End Temp

	static DWORD msgHash_WorldModified = CHashString(_T("WorldModified")).GetUniqueID();
	pToolBox->SendMessage(msgHash_WorldModified, sizeof(BOOL), (void *)true);

	m_pEditor->SetAllSectorsModified();
}

void CTerrainSizeDlg::CheckButton( CButton * but )
{
	UpdateData( true );
	if( m_LastButtonChecked == but )
		return;
	m_LastButtonChecked->SetCheck( BST_UNCHECKED );
	m_LastButtonChecked = but;

	m_ButtonLeftUp.SetIcon( m_IconBlank );
	m_ButtonUp.SetIcon( m_IconBlank );
	m_ButtonRightUp.SetIcon( m_IconBlank );
	m_ButtonRight.SetIcon( m_IconBlank );
	m_ButtonLeft.SetIcon( m_IconBlank );
	m_ButtonMiddle.SetIcon( m_IconBlank );
	m_ButtonLeftDown.SetIcon( m_IconBlank );
	m_ButtonDown.SetIcon( m_IconBlank );
	m_ButtonRightDown.SetIcon( m_IconBlank );
	m_LastButtonChecked->SetIcon( m_ArrowIcons[1][1] );

	UpdateData( false );
}

void CTerrainSizeDlg::OnBnClickedCheckCenterNw()
{
	ResizeStart[0]=0;
	ResizeStart[1]=0;
	//Uncheck the other check boxes
	CheckButton( &m_ButtonLeftUp );
	m_ButtonUp.SetIcon( m_ArrowIcons[1][2] );
	m_ButtonMiddle.SetIcon( m_ArrowIcons[2][2] );
	m_ButtonLeft.SetIcon( m_ArrowIcons[2][1] );	

}

void CTerrainSizeDlg::OnBnClickedCheckCenterN()
{
	ResizeStart[0]=0;
	ResizeStart[1]=1;
	//Uncheck the other check boxes	
	CheckButton( &m_ButtonUp );	
	m_ButtonLeftUp.SetIcon( m_ArrowIcons[1][0] );
	m_ButtonMiddle.SetIcon( m_ArrowIcons[2][1] );
	m_ButtonRightUp.SetIcon( m_ArrowIcons[1][2] );		
	m_ButtonRight.SetIcon( m_ArrowIcons[2][2] );
	m_ButtonLeft.SetIcon( m_ArrowIcons[2][0] );	
}

void CTerrainSizeDlg::OnBnClickedCheckCenterNe()
{
	ResizeStart[0]=0;
	ResizeStart[1]=2;
	//Uncheck the other check boxes
	CheckButton( &m_ButtonRightUp );	
	m_ButtonUp.SetIcon( m_ArrowIcons[1][0] );
	m_ButtonMiddle.SetIcon( m_ArrowIcons[2][0] );
	m_ButtonRight.SetIcon( m_ArrowIcons[2][1] );	
}

void CTerrainSizeDlg::OnBnClickedCheckCenterW()
{
	ResizeStart[0]=1;
	ResizeStart[1]=0;
	//Uncheck the other check boxes
	CheckButton( &m_ButtonLeft );
	m_ButtonLeftUp.SetIcon( m_ArrowIcons[0][1] );
	m_ButtonMiddle.SetIcon( m_ArrowIcons[1][2] );
	m_ButtonLeftDown.SetIcon( m_ArrowIcons[2][1] );		
	m_ButtonDown.SetIcon( m_ArrowIcons[2][2] );
	m_ButtonUp.SetIcon( m_ArrowIcons[0][2] );	
}

void CTerrainSizeDlg::OnBnClickedCheckCenter()
{
	ResizeStart[0]=1;
	ResizeStart[1]=1;
	//Uncheck the other check boxes
	CheckButton( &m_ButtonMiddle );
	m_ButtonLeftUp.SetIcon( m_ArrowIcons[0][0] );
	m_ButtonUp.SetIcon( m_ArrowIcons[0][1] );
	m_ButtonRightUp.SetIcon( m_ArrowIcons[0][2] );
	m_ButtonRight.SetIcon( m_ArrowIcons[1][2] );
	m_ButtonLeft.SetIcon( m_ArrowIcons[1][0] );
	m_ButtonLeftDown.SetIcon( m_ArrowIcons[2][0] );
	m_ButtonDown.SetIcon( m_ArrowIcons[2][1] );
	m_ButtonRightDown.SetIcon( m_ArrowIcons[2][2] );
}

void CTerrainSizeDlg::OnBnClickedCheckCenterE()
{
	ResizeStart[0]=1;
	ResizeStart[1]=2;
	//Uncheck the other check boxes
	CheckButton( &m_ButtonRight );	
	m_ButtonRightUp.SetIcon( m_ArrowIcons[0][1] );
	m_ButtonMiddle.SetIcon( m_ArrowIcons[1][0] );
	m_ButtonRightDown.SetIcon( m_ArrowIcons[2][1] );		
	m_ButtonDown.SetIcon( m_ArrowIcons[2][0] );
	m_ButtonUp.SetIcon( m_ArrowIcons[0][0] );	
}

void CTerrainSizeDlg::OnBnClickedCheckCenterSw()
{
	ResizeStart[0]=2;
	ResizeStart[1]=0;
	//Uncheck the other check boxes
	CheckButton( &m_ButtonLeftDown );
	m_ButtonLeft.SetIcon( m_ArrowIcons[0][1] );
	m_ButtonMiddle.SetIcon( m_ArrowIcons[0][2] );
	m_ButtonDown.SetIcon( m_ArrowIcons[1][2] );	
}

void CTerrainSizeDlg::OnBnClickedCheckCenterS()
{
	ResizeStart[0]=2;
	ResizeStart[1]=1;
	//Uncheck the other check boxes
	CheckButton( &m_ButtonDown );
	m_ButtonLeftDown.SetIcon( m_ArrowIcons[1][0] );
	m_ButtonMiddle.SetIcon( m_ArrowIcons[0][1] );
	m_ButtonRightDown.SetIcon( m_ArrowIcons[1][2] );		
	m_ButtonRight.SetIcon( m_ArrowIcons[0][2] );
	m_ButtonLeft.SetIcon( m_ArrowIcons[0][0] );	
}

void CTerrainSizeDlg::OnBnClickedCheckCenterSe()
{
	ResizeStart[0]=2;
	ResizeStart[1]=2;
	//Uncheck the other check boxes
	CheckButton( &m_ButtonRightDown );
	m_ButtonRight.SetIcon( m_ArrowIcons[0][1] );
	m_ButtonMiddle.SetIcon( m_ArrowIcons[0][0] );
	m_ButtonDown.SetIcon( m_ArrowIcons[1][0] );	
}

void CTerrainSizeDlg::OnDeltaposSpinSectorWidth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta > 0)
	{
		if (m_SectorSize > 1.0f)
		{
			m_SectorSize -= .01f;
		}
	}
	else
	{
		m_SectorSize += .01f;
	}
	UpdateData(false);
	*pResult = 0;
}


void CTerrainSizeDlg::OnDeltaposSpinSectorZscale(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta > 0)
	{
		if (m_SectorZScale > .01f)
		{
			m_SectorZScale -= .01f;
		}
	}
	else
	{
		m_SectorZScale += .01f;
	}
	UpdateData(false);
	*pResult = 0;
}

void CTerrainSizeDlg::OnDeltaposSpinSectorTrires(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta > 0)
	{
		if (m_TriResolution > 1)
		{
			m_TriResolution -= 1;
		}
	}
	else 
	{
		if (m_TriResolution <= (DWORD)m_SectorSize-1)
		m_TriResolution += 1;
	}
	UpdateData(false);

	*pResult = 0;
}

void CTerrainSizeDlg::OnEnKillfocusTriResolution()
{
	UpdateData(true);
	if (m_TriResolution < 1)
	{
		m_TriResolution = 1;
		UpdateData(false);
	}
	else if (m_TriResolution > (DWORD)m_SectorSize)
	{
		m_TriResolution = (DWORD)m_SectorSize;
		UpdateData(false);
	}
}

void CTerrainSizeDlg::OnEnKillfocusEditSectorWidth()
{
	// check for greater > 1.0f
	UpdateData(true);
	if (m_SectorSize < 1.0f)
	{
		m_SectorSize = 1.0f;
		UpdateData(false);
	}
}

void CTerrainSizeDlg::OnEnKillfocusEditSectorZscale()
{
	// check for greater > .01
	UpdateData(true);
	if (m_SectorZScale < .01f)
	{
		m_SectorZScale = .01f;
		UpdateData(false);
	}
}

void CTerrainSizeDlg::OnEnKillfocusEditTerrainHeight()
{
	// check for greater > 1
	UpdateData(true);
	if (m_TerrainHeight < 1)
	{
		m_TerrainHeight = 1;
		UpdateData(false);
	}
}

void CTerrainSizeDlg::OnEnKillfocusEditTerrainWidth()
{
	// check for greater > 1
	UpdateData(true);
	if (m_TerrainWidth < 1)
	{
		m_TerrainWidth = 1;
		UpdateData(false);
	}
}

void CTerrainSizeDlg::OnEnKillfocusTexturescalex()
{
	UpdateData(true);
	if (m_BaseTextureScaleX < .001f)
	{
		m_BaseTextureScaleX = .001f;
		UpdateData(false);
	}
}

void CTerrainSizeDlg::OnEnKillfocusTexturescaley()
{
	UpdateData(true);
	if (m_BaseTextureScaleY < .001f)
	{
		m_BaseTextureScaleY = .001f;
		UpdateData(false);
	}
}

void CTerrainSizeDlg::OnDeltaposSpinTerrainHeight(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta > 0)
	{
		if (m_TerrainHeight > 1)
		{
			m_TerrainHeight -= 1;
		}
	}
	else
	{
		m_TerrainHeight += 1;
	}
	UpdateData(false);

	*pResult = 0;
}

void CTerrainSizeDlg::OnDeltaposSpinTerrainWidth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta > 0)
	{
		if (m_TerrainWidth > 1)
		{
			m_TerrainWidth -= 1;
		}
	}
	else
	{
		m_TerrainWidth += 1;
	}
	UpdateData(false);
	*pResult = 0;
}

void CTerrainSizeDlg::OnBnClickedImport()
{
	ShowWindow(SW_HIDE);
	//modify the current terrain with parameters
	TERRAINCREATIONPARAMS tcParms;
	FillTerrainCreationParam(&tcParms);
	tcParms.SectorSize = ConvertToMeters(m_SectorSize);

	CUndoCommandGuard undoCommand;

	m_pEditor->DoAllSectorsUndo(true);

	if (!m_GDALIO.Import(&tcParms))
	{
		undoCommand.Cancel();
		ShowWindow(SW_SHOW);
	}
	else
	{
		m_pEditor->DoAllSectorsUndo(false);
		EndDialog(IDOK);
	}
}

void CTerrainSizeDlg::PauseThumbnailGeneration(bool pause)
{
	static DWORD msgHash = CHashString(_T("PauseThumbnailGenerationThread")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash, sizeof(bool *), &pause);
}

bool CTerrainSizeDlg::GetTerrainDescriptor(TERRAINDESCRIPTOR *pDescriptor)
{
	if (pDescriptor != NULL)
	{
		memset(pDescriptor, 0, sizeof(*pDescriptor));
		static DWORD msgHash = CHashString(_T("GetTerrainDescriptor")).GetUniqueID();
		return MSG_HANDLED == EngineGetToolBox()->SendMessage(msgHash, sizeof(*pDescriptor), pDescriptor);
	}
	return false;
}

void CTerrainSizeDlg::FillTerrainCreationParam(TERRAINCREATIONPARAMS *pParams)
{
	CTerrainEditor::GetSavePathName(pParams->TerrainPath, pParams->TerrainName);
	// Create the terrain using the given parameters:
	pParams->SectorResolution = m_TriResolution;
	pParams->ZScale = m_SectorZScale;
	pParams->LODDistance = 150.0f;
	pParams->TerrainWidth = m_TerrainWidth;
	pParams->TerrainHeight = m_TerrainHeight;
	//pParams->SectorSize = ConvertToMeters(m_SectorSize);
	pParams->SectorSize = m_SectorSize;
	pParams->TerrainBaseTexture = m_TerrainBaseTexture;		
	pParams->BaseTextureScaleX = m_BaseTextureScaleX;
	pParams->BaseTextureScaleY = m_BaseTextureScaleY;
}
