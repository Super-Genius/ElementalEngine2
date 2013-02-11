///============================================================================
/// \file		TimeOfDayToolPal.cpp
/// \brief		Implementation of TimeOfDay Tool Palatte
/// \date		08-12-2005
/// \author		D. Patrick Ghiocel
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
#include "TimeOfDayToolPal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CTerrainToolPal dialog

IMPLEMENT_DYNAMIC(CTimeOfDayToolPal, CGuiDialogBar)

CTimeOfDayToolPal::CTimeOfDayToolPal()
{
	m_ToolBox = EngineGetToolBox();

	m_pEditor = SINGLETONINSTANCE(CTimeOfDayEditor);
	m_iTimeSlider = 60;
	m_fTimeRate = 0.0f;
}

CTimeOfDayToolPal::~CTimeOfDayToolPal()
{

}

void CTimeOfDayToolPal::OnGlobalUpdateTick( void )
{
	// Get Time
	float tmpTime = 0.5; // Noon
	static DWORD msgHash_GetTimeOfDay = CHashString(_T("GetTimeOfDay")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetTimeOfDay, sizeof(tmpTime), &tmpTime,  NULL, &CHashString(_T("CTimeOfDayObject")) );
	m_iTimeSlider = (int)(tmpTime * 120.0f);
	m_ctrlTimeSlider.SetPos( m_iTimeSlider );

	// make sure lighting is synchronized
	bool bLightTerrain = (m_ctrlTODLightButton.GetCheck() == BST_CHECKED);
	static DWORD msgHash_SetTimeOfDayDoLighting = CHashString(_T("SetTimeOfDayDoLighting")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetTimeOfDayDoLighting, sizeof(bool), &bLightTerrain, NULL, &CHashString(_T("CTimeOfDayObject")) );
}

BEGIN_MESSAGE_MAP(CTimeOfDayToolPal, CGuiDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_TIMERATE_EDIT, OnEnKillfocusTimerateEdit)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TIMERATE_SPIN, OnDeltaposTimerateSpin)
	ON_BN_CLICKED(IDC_TIMEOFDAY_MOTION, OnBnClickedTimeofdayMotion)
	ON_BN_CLICKED(IDC_TIMEOFDAY_LIGHT, OnBnClickedTimeofdayLight)	
	ON_BN_CLICKED(IDC_SHADOW_ENABLE, OnBnClickedShadowToggle)
	ON_UPDATE_COMMAND_UI(IDC_TODDIALOG_BUTTON, OnUpdateButtons)
	ON_BN_CLICKED(IDC_TODDIALOG_BUTTON, OnBnClickedToddialogButton)
	ON_BN_CLICKED(IDC_WEATHEREFFECTS_BUTTON, OnBnClickedTodWeatherButton)
	ON_UPDATE_COMMAND_UI(IDC_WEATHEREFFECTS_BUTTON, OnUpdateButtons)
END_MESSAGE_MAP()


void CTimeOfDayToolPal::DoDataExchange(CDataExchange* pDX)
{
	CGuiDialogBar::DoDataExchange(pDX);
	DDX_Slider( pDX, IDC_TIME_SLIDER, m_iTimeSlider );
	DDX_Text( pDX, IDC_TIMERATE_EDIT, m_fTimeRate );
	
	DDX_Control( pDX, IDC_TIME_SLIDER, m_ctrlTimeSlider );
	DDX_Control( pDX, IDC_TIMERATE_EDIT, m_ctrlTimeRateEdit );
	DDX_Control( pDX, IDC_TIMERATE_SPIN, m_ctrlTimeRateSpinner );
	DDX_Control( pDX, IDC_TIMEOFDAY_MOTION, m_ctrlTODMotionButton );
	DDX_Control( pDX, IDC_TIMEOFDAY_LIGHT, m_ctrlTODLightButton );	
	DDX_Control( pDX, IDC_SHADOW_ENABLE, m_ctrlShadowToggleButton );
	DDX_Control( pDX, IDC_TODDIALOG_BUTTON, m_ctrlTODDialogButton );
	DDX_Control( pDX, IDC_WEATHEREFFECTS_BUTTON, m_ctrlWeatherDialogButton );
		
}

LRESULT CTimeOfDayToolPal::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = HandleInitDialog(wParam, lParam);

	float tmpRate = 0.0;
	static DWORD msgHash_GetTimeOfDayRate = CHashString(_T("GetTimeOfDayRate")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetTimeOfDayRate, sizeof(tmpRate), &tmpRate,  NULL, &CHashString(_T("CTimeOfDayObject")) );
	m_fTimeRate = tmpRate;

	float tmpTime = 0.5; // Noon
	static DWORD msgHash_GetTimeOfDay = CHashString(_T("GetTimeOfDay")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetTimeOfDay, sizeof(tmpTime), &tmpTime,  NULL, &CHashString(_T("CTimeOfDayObject")) );
	m_iTimeSlider = (int)(tmpTime * 120.0f);


    UpdateData(FALSE);

	m_ctrlTimeSlider.SetRangeMin( 0, true );
	m_ctrlTimeSlider.SetRangeMax( 120, true );
	m_ctrlTODMotionButton.SetCheck( BST_UNCHECKED );
	m_ctrlTODLightButton.SetCheck( BST_UNCHECKED );
	m_ctrlShadowToggleButton.SetCheck( BST_UNCHECKED );

//	m_ctrlTODDialogButton.EnableWindow( TRUE );
	
	m_ctrlTODDialogButton.SetIcon((HICON)LoadImage(TimeOfDayEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_TOD_DIALOG), IMAGE_ICON, 0, 0, 0));
	
	m_ctrlTODMotionButton.SetIcon((HICON)LoadImage(TimeOfDayEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_TOD_MOTION), IMAGE_ICON, 32, 32, 0));

	m_ctrlTODLightButton.SetIcon((HICON)LoadImage(TimeOfDayEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_TOD_LIGHT), IMAGE_ICON, 32, 32, 0));

	m_ctrlWeatherDialogButton.SetIcon((HICON)LoadImage(TimeOfDayEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_WEATHERICON), IMAGE_ICON, 32, 32, 0));
	m_ctrlShadowToggleButton.SetIcon((HICON)LoadImage(TimeOfDayEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_SHADOW_ICON), IMAGE_ICON, 32, 32, 0));
	return bRet;
}

void CTimeOfDayToolPal::OnEnKillfocusTimerateEdit()
{
	// TODO: Add your control notification handler code here
	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	m_ctrlTimeRateEdit.GetWindowText( szBuffer, 256 );
	m_fTimeRate = (float)atof( StdString(szBuffer) );
	if( m_fTimeRate <= 0.0f )
	{
		m_fTimeRate = 0.0f;
		m_ctrlTimeRateEdit.SetWindowText( _T("0.0") );
	}
	
	// Send Message to Update TimeRate
	static DWORD msgHash_SetTimeOfDayRate = CHashString(_T("SetTimeOfDayRate")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetTimeOfDayRate, sizeof(m_fTimeRate), &m_fTimeRate,  NULL, &CHashString(_T("CTimeOfDayObject")) );
}

void CTimeOfDayToolPal::OnDeltaposTimerateSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_fTimeRate = m_fTimeRate - pNMUpDown->iDelta * 10.0f;
	if( m_fTimeRate < 0.1f )
	{
		m_fTimeRate = 0.0f;
	}
	static DWORD msgHash_SetTimeOfDayRate = CHashString(_T("SetTimeOfDayRate")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetTimeOfDayRate, sizeof(m_fTimeRate), &m_fTimeRate,  NULL, &CHashString(_T("CTimeOfDayObject")) );
	
	UpdateData(FALSE);	
}

void CTimeOfDayToolPal::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSliderCtrl* slider = (CSliderCtrl*)pScrollBar;
	if( slider == &m_ctrlTimeSlider )
	{
		m_iTimeSlider = m_ctrlTimeSlider.GetPos();
		// Send Message to Update Time
		float tmpTime = (float)m_iTimeSlider / 120.0f;
		static DWORD msgHash_SetTimeOfDay = CHashString(_T("SetTimeOfDay")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetTimeOfDay, sizeof(tmpTime), &tmpTime, NULL, &CHashString(_T("CTimeOfDayObject")) );
	}
}

void CTimeOfDayToolPal::OnBnClickedTimeofdayMotion()
{
	// TODO: Add your control notification handler code here
	bool bTODMotion = (m_ctrlTODMotionButton.GetCheck() == BST_CHECKED);
	static DWORD msgHash_SetTimeOfDayMotion = CHashString(_T("SetTimeOfDayMotion")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetTimeOfDayMotion, sizeof(bTODMotion), &bTODMotion, NULL, &CHashString(_T("CTimeOfDayObject")) );
}

void CTimeOfDayToolPal::OnBnClickedTimeofdayLight()
{
	bool bLightTerrain = (m_ctrlTODLightButton.GetCheck() == BST_CHECKED);
	static DWORD msgHash_SetTimeOfDayDoLighting = CHashString(_T("SetTimeOfDayDoLighting")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetTimeOfDayDoLighting, sizeof(bool), &bLightTerrain, NULL, &CHashString(_T("CTimeOfDayObject")) );
	static DWORD msgHash_ReloadAllShaders = CHashString(_T("ReloadAllShaders")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_ReloadAllShaders, 0, 0 );
}

void CTimeOfDayToolPal::OnBnClickedShadowToggle()
{
	bool bShadow = (m_ctrlShadowToggleButton.GetCheck() == BST_CHECKED );
	//send state change to shadow manager
	static DWORD msgHash_SetDepthShadowMode = CHashString(_T("SetDepthShadowMode")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetDepthShadowMode, sizeof( bool * ), &bShadow );
}

void CTimeOfDayToolPal::OnUpdateButtons( CCmdUI* pCmdUI )
{
	// 
}

void CTimeOfDayToolPal::OnBnClickedToddialogButton()
{
	// TODO: Add your control notification handler code here
	m_pEditor->ShowTimeOfDayDialog(true);
}

void CTimeOfDayToolPal::OnBnClickedTodWeatherButton()
{
	// TODO: Add your control notification handler code here
	m_pEditor->ShowWeatherDialog(true);
}
