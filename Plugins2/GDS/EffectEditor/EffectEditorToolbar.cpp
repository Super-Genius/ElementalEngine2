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
#include "EffectEditor.h"
#include "EffectEditorToolbar.h"
#include ".\effecteditortoolbar.h"


// CEffectEditorToolbar dialog

IMPLEMENT_DYNAMIC(CEffectEditorToolbar, CGuiDialogBar)
CEffectEditorToolbar::CEffectEditorToolbar()
{
	m_ToolBox = EngineGetToolBox();
	m_pEditor = SINGLETONINSTANCE(CEffectEditor);
}

CEffectEditorToolbar::~CEffectEditorToolbar()
{
}

void CEffectEditorToolbar::DoDataExchange(CDataExchange* pDX)
{
	CGuiDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_EFFECTS, m_ComboEffects);
}


BEGIN_MESSAGE_MAP(CEffectEditorToolbar, CGuiDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
	ON_BN_CLICKED(IDC_CHECK_CAMERALIGHT, OnBnClickedCheckCameralight)
	ON_BN_CLICKED(IDC_CHECK_GROUND, OnBnClickedCheckGround)
	ON_BN_CLICKED(IDC_CHECK_SCENELIGHT, OnBnClickedCheckScenelight)
	ON_BN_CLICKED(IDC_CHECK_EFFECTMODEL, OnBnClickedCheckEffectmodel)
	ON_CBN_SELCHANGE(IDC_COMBO_EFFECTS, OnCbnSelchangeComboEffects)
	ON_CBN_DROPDOWN(IDC_COMBO_EFFECTS, OnCbnDropdownComboEffects)
END_MESSAGE_MAP()


// CEffectEditorToolbar message handlers

LRESULT CEffectEditorToolbar::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = HandleInitDialog(wParam, lParam);
	UpdateData(FALSE);
	return bRet;
}

void CEffectEditorToolbar::OnBnClickedCheckCameralight()
{
	CEffectEditorView *pView = m_pEditor->GetActiveView();
	if (pView)
		pView->ToggleCameraLight();
}

void CEffectEditorToolbar::OnBnClickedCheckGround()
{
	CEffectEditorView *pView = m_pEditor->GetActiveView();
	if (pView)
		pView->ToggleGroundModel();
}

void CEffectEditorToolbar::OnBnClickedCheckScenelight()
{
	CEffectEditorView *pView = m_pEditor->GetActiveView();
	if (pView)
		pView->ToggleSceneLight();
}

void CEffectEditorToolbar::OnBnClickedCheckEffectmodel()
{
	CEffectEditorView *pView = m_pEditor->GetActiveView();
	if (pView)
		pView->ToggleEffectModel();
}

void CEffectEditorToolbar::OnCbnSelchangeComboEffects()
{
	int iSel = m_ComboEffects.GetCurSel();
	CString szSel;
	m_ComboEffects.GetLBText(iSel, szSel );
	CHashString hszSel( szSel );
	NAMETOEFFECTMAP::iterator itr = m_EffectMap.find( hszSel.GetUniqueID() );
	if (itr != m_EffectMap.end())
	{
		IEffect *pEffect = itr->second;
		m_pEditor->GetActiveView()->SetEffectModelEffect( pEffect );
	}
}

void CEffectEditorToolbar::OnCbnDropdownComboEffects()
{
	m_ComboEffects.ResetContent();

	// populate effect map
	GETEFFECTLIST gel;
	gel.m_pEffectList = &m_EffectMap;
	static DWORD msgHash_GetAllEffects = CHashString(_T("GetAllEffects")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetAllEffects, sizeof(gel), &gel) != MSG_HANDLED)
		return;

	// populate combobox with effects
	for (NAMETOEFFECTMAP::iterator itr=m_EffectMap.begin(); itr != m_EffectMap.end(); itr++)
	{
		m_ComboEffects.AddString( m_ToolBox->GetHashString(itr->first) );
	}
}
