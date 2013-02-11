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
#include <math.h>


// CModelViewDlg dialog

IMPLEMENT_DYNAMIC(CModelViewDlg, CDialog)
CModelViewDlg::CModelViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModelViewDlg::IDD, pParent)
{
	m_bPlayingSelected = false;
	m_bPlayingSequence = false;
}

CModelViewDlg::~CModelViewDlg()
{
}

void CModelViewDlg::OnStopSequence()
{
	m_bPlayingSelected = false;
	m_bPlayingSequence = false;

	CString strTitle;
	VERIFY(strTitle.LoadString(IDS_PLAY_SELECTED));
	SetDlgItemText(IDC_MODELVIEW_TB_BUTTON_ANIMPLAY, strTitle);
	VERIFY(strTitle.LoadString(IDS_PLAY_LIST));
	SetDlgItemText(IDC_MODELVIEW_TB_BUTTON_PLAYSEQUENCE, strTitle);
}

CString CModelViewDlg::GetAnimationPath(int nAnimationId)
{
	ASSERT(::IsWindow(m_AnimationComboBox));
	CString szAnimationPath;
	if (nAnimationId < m_AnimationComboBox.GetCount() && nAnimationId >= 0)
	{
		m_AnimationComboBox.GetLBText(nAnimationId, szAnimationPath);
	}
	else
	{
		ASSERT(!"Invalid animation index was passed");
	}
	return szAnimationPath;
}

void CModelViewDlg::SetDlgItemFloat(int nDlgItemId, float fValue)
{
	ASSERT(::IsWindow(*GetDlgItem(nDlgItemId)));
	TCHAR szBuffer[80];
	_stprintf(szBuffer, _T("%5.2f"), fValue);
	CString szCurreentValue;
	GetDlgItemText(nDlgItemId, szCurreentValue);
	// leave current selection in edit box if no changes
	if (szCurreentValue != szBuffer)
	{
		SetDlgItemText(nDlgItemId, szBuffer);
	}
}

bool CModelViewDlg::GetDlgItemFloat(int nDlgItemId, float &fValue, bool bCheckDegrees)
{
	ASSERT(::IsWindow(*GetDlgItem(nDlgItemId)));

	TCHAR szBuffer[80];
	int res = GetDlgItemText(nDlgItemId, szBuffer, _countof(szBuffer));
	if (res == 0 || res > _countof(szBuffer))
	{
		return false;
	}

	LPTSTR pEnd = NULL;
	double dParsed = _tcstod(szBuffer, &pEnd);
	// check for correct float number parsing
	if ((errno == ERANGE) ||
		(pEnd != NULL && *pEnd != _T('\0')))
	{
		return false;
	}

	if (bCheckDegrees)
	{
		if (fabs(dParsed) > 360.0)
		{
			double dChecked = fmod(dParsed, 360.0);
			if (fabs(dChecked - dParsed) > 0.001)
			{
				dParsed = dChecked;
				SetDlgItemFloat(nDlgItemId, static_cast<float>(dParsed));
			}
		}
	}

	fValue = static_cast<float>(dParsed);
	return true;
}

void CModelViewDlg::SetPosition(const Vec3 &position)
{
	SetDlgItemFloat(IDC_MODELVIEW_TB_EDIT_LOOKOFFSETX, position.GetX());
	SetDlgItemFloat(IDC_MODELVIEW_TB_EDIT_LOOKOFFSETY, position.GetY());
	SetDlgItemFloat(IDC_MODELVIEW_TB_EDIT_LOOKOFFSETZ, position.GetZ());
}

void CModelViewDlg::GetPosition(Vec3 &position)
{
	GetDlgItemFloat(IDC_MODELVIEW_TB_EDIT_LOOKOFFSETX, position.x, false);
	GetDlgItemFloat(IDC_MODELVIEW_TB_EDIT_LOOKOFFSETY, position.y, false);
	GetDlgItemFloat(IDC_MODELVIEW_TB_EDIT_LOOKOFFSETZ, position.z, false);
}

void CModelViewDlg::SetRotation(const EulerAngle &rotation)
{
	SetDlgItemFloat(IDC_MODELVIEW_TB_EDIT_ROTATIONX, rotation.GetRoll());
	SetDlgItemFloat(IDC_MODELVIEW_TB_EDIT_ROTATIONY, rotation.GetPitch());
	SetDlgItemFloat(IDC_MODELVIEW_TB_EDIT_ROTATIONZ, rotation.GetYaw());
}

void CModelViewDlg::GetRotation(EulerAngle &rotation)
{
	GetDlgItemFloat(IDC_MODELVIEW_TB_EDIT_ROTATIONX, rotation.roll, true);
	GetDlgItemFloat(IDC_MODELVIEW_TB_EDIT_ROTATIONY, rotation.pitch, true);
	GetDlgItemFloat(IDC_MODELVIEW_TB_EDIT_ROTATIONZ, rotation.yaw, true);
}

void CModelViewDlg::SetAnimationList(const ANIMATIONLIST *pAnimationList)
{
	ASSERT(::IsWindow(m_AnimationComboBox));
	m_AnimationComboBox.ResetContent();
	if (pAnimationList != NULL)
	{
		ANIMATIONLIST::const_iterator it = pAnimationList->begin();
		for (;it != pAnimationList->end(); ++it)
		{
			m_AnimationComboBox.AddString(*it);
		}
	}
}

bool CModelViewDlg::IsAnimationSelected() const
{
	return m_AnimationComboBox.GetCurSel() != CB_ERR;
}

int CModelViewDlg::GetCurrentAnimationId() const
{
	ASSERT(::IsWindow(m_AnimationComboBox));
	return m_AnimationComboBox.GetCurSel();
}

void CModelViewDlg::SetCurrentAnimationId(int nAnimationId)
{
	ASSERT(::IsWindow(m_AnimationComboBox));
	ASSERT(nAnimationId >= -1);
	ASSERT(nAnimationId < m_AnimationComboBox.GetCount());
	m_AnimationComboBox.SetCurSel(nAnimationId);
}

bool CModelViewDlg::IsSequencePresent() const
{
	return m_AnimationListBox.GetCount() > 0;
}

void CModelViewDlg::SetAnimationSequence(const ANIMATIONSEQUENCE *pAnimationSequence)
{
	ASSERT(::IsWindow(m_AnimationListBox));
	m_AnimationListBox.ResetContent();
	if (pAnimationSequence != NULL)
	{
		ANIMATIONSEQUENCE::const_iterator it = pAnimationSequence->begin();
		for (; it != pAnimationSequence->end(); ++it)
		{
			CString szAnimationPath = GetAnimationPath(*it);
			int nIndex = m_AnimationListBox.AddString(szAnimationPath);
			ASSERT(nIndex != LB_ERR);
			m_AnimationListBox.SetItemData(nIndex, *it);
		}
	}
}

void CModelViewDlg::GetAnimationSequence(ANIMATIONSEQUENCE &animationSequence) const
{
	ASSERT(::IsWindow(m_AnimationListBox));
	animationSequence.clear();
	for (int i = 0; i < m_AnimationListBox.GetCount(); i++)
	{
		int animationId = (int)m_AnimationListBox.GetItemData(i);
		animationSequence.push_back(animationId);
	}
}

void CModelViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODELVIEW_TB_BUTTON_ADDANIMATION, m_AddAnimationButton);
	DDX_Control(pDX, IDC_MODELVIEW_TB_BUTTON_REMOVEANIM, m_RemoveAnimationButton);
	DDX_Control(pDX, IDC_MODELVIEW_TB_BUTTON_CLEARANIMATIONLIST, m_ClearAnimationsButton);
	DDX_Control(pDX, IDC_MODELVIEW_TB_BUTTON_ANIMPLAY, m_PlaySelectionButton);
	DDX_Control(pDX, IDC_MODELVIEW_TB_BUTTON_PLAYSEQUENCE, m_PlaySequenceButton);
	DDX_Control(pDX, IDC_MODELVIEW_TB_LIST_ANIMATIONS, m_AnimationListBox);
	DDX_Control(pDX, IDC_MODELVIEW_TB_COMBO_ANIMSELECT, m_AnimationComboBox);
	DDX_Control(pDX, IDC_MODELVIEW_TB_BUTTON_RESET_LOOKAT, m_ResetPositionButton);
	DDX_Control(pDX, IDC_MODELVIEW_TB_BUTTON_RESET_ROTATION, m_ResetRotationButton);
}


BEGIN_MESSAGE_MAP(CModelViewDlg, CDialog)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog)
	ON_BN_CLICKED(IDC_MODELVIEW_TB_BUTTON_ADDANIMATION, OnBnClickedModelviewTbButtonAddanimation)
	ON_BN_CLICKED(IDC_MODELVIEW_TB_BUTTON_ANIMPLAY, OnBnClickedModelviewTbButtonAnimplay)
	ON_BN_CLICKED(IDC_MODELVIEW_TB_BUTTON_CLEARANIMATIONLIST, OnBnClickedModelviewTbButtonClearanimationlist)
	ON_BN_CLICKED(IDC_MODELVIEW_TB_BUTTON_PLAYSEQUENCE, OnBnClickedModelviewTbButtonPlaysequence)
	ON_BN_CLICKED(IDC_MODELVIEW_TB_BUTTON_REMOVEANIM, OnBnClickedModelviewTbButtonRemoveanim)
	ON_BN_CLICKED(IDC_MODELVIEW_TB_BUTTON_RESET_LOOKAT, OnBnClickedModelviewTbButtonResetLookat)
	ON_BN_CLICKED(IDC_MODELVIEW_TB_BUTTON_RESET_ROTATION, OnBnClickedModelviewTbButtonResetRotation)
	ON_EN_CHANGE(IDC_MODELVIEW_TB_EDIT_LOOKOFFSETX, OnEnChangeModelviewTbEditLookoffset)
	ON_EN_CHANGE(IDC_MODELVIEW_TB_EDIT_LOOKOFFSETY, OnEnChangeModelviewTbEditLookoffset)
	ON_EN_CHANGE(IDC_MODELVIEW_TB_EDIT_LOOKOFFSETZ, OnEnChangeModelviewTbEditLookoffset)
	ON_EN_CHANGE(IDC_MODELVIEW_TB_EDIT_ROTATIONX, OnEnChangeModelviewTbEditRotation)
	ON_EN_CHANGE(IDC_MODELVIEW_TB_EDIT_ROTATIONY, OnEnChangeModelviewTbEditRotation)
	ON_EN_CHANGE(IDC_MODELVIEW_TB_EDIT_ROTATIONZ, OnEnChangeModelviewTbEditRotation)

	ON_UPDATE_COMMAND_UI(IDC_MODELVIEW_TB_BUTTON_ADDANIMATION, OnUpdateAddAnimation)
	ON_UPDATE_COMMAND_UI(IDC_MODELVIEW_TB_BUTTON_CLEARANIMATIONLIST, OnUpdateClearAnimationSequence)
	ON_UPDATE_COMMAND_UI(IDC_MODELVIEW_TB_BUTTON_REMOVEANIM, OnUpdateRemoveAnimation)
END_MESSAGE_MAP()


// CModelViewDlg message handlers

LRESULT CModelViewDlg::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = HandleInitDialog(wParam, lParam);
	UpdateData(FALSE);

	m_ResetPositionButton.SetIcon((HICON)LoadImage(ModelViewEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_RESETLOOKAT), IMAGE_ICON, 0, 0, 0));	
	m_ResetRotationButton.SetIcon((HICON)LoadImage(ModelViewEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_RESETLOOKAT), IMAGE_ICON, 0, 0, 0));	

	return bRet;
}

void CModelViewDlg::OnBnClickedModelviewTbButtonAddanimation()
{
	int nSelection = m_AnimationComboBox.GetCurSel();
	if (nSelection != CB_ERR)
	{
		CString szPath;
		m_AnimationComboBox.GetLBText(nSelection, szPath);
		int nIndex = m_AnimationListBox.AddString(szPath);
		m_AnimationListBox.SetItemData(nIndex, nSelection);
	}
}

void CModelViewDlg::OnBnClickedModelviewTbButtonAnimplay()
{
	CModelViewRender *pView = SINGLETONINSTANCE(CModelViewEditor)->GetActiveView();
	if (pView)
	{
		if (!m_bPlayingSelected)
		{
			if (m_bPlayingSequence)
				pView->StopSequence();

			m_bPlayingSelected = true;
			pView->PlayAnimation( true, GetCurrentAnimationId() );

			CString strTitle;
			VERIFY(strTitle.LoadString(IDS_STOP_ANIMATION));
			SetDlgItemText(IDC_MODELVIEW_TB_BUTTON_ANIMPLAY, strTitle);
			VERIFY(strTitle.LoadString(IDS_PLAY_LIST));
			SetDlgItemText(IDC_MODELVIEW_TB_BUTTON_PLAYSEQUENCE, strTitle);
		}
		else if (!m_bPlayingSequence)
		{
			pView->StopSequence();
		}
	}
}

void CModelViewDlg::OnBnClickedModelviewTbButtonClearanimationlist()
{
	m_AnimationListBox.ResetContent();
}

void CModelViewDlg::OnBnClickedModelviewTbButtonPlaysequence()
{
	CModelViewRender *pView = SINGLETONINSTANCE(CModelViewEditor)->GetActiveView();
	if (pView)
	{
		if (!m_bPlayingSequence)
		{
			if (m_bPlayingSelected)
				pView->StopSequence();

			m_bPlayingSequence = true;

			ANIMATIONSEQUENCE animationSequence;
			GetAnimationSequence( animationSequence );
			pView->PlaySequence( true, animationSequence );

			CString strTitle;
			VERIFY(strTitle.LoadString(IDS_STOP_ANIMATION));
			SetDlgItemText(IDC_MODELVIEW_TB_BUTTON_PLAYSEQUENCE, strTitle);
			VERIFY(strTitle.LoadString(IDS_PLAY_SELECTED));
			SetDlgItemText(IDC_MODELVIEW_TB_BUTTON_ANIMPLAY, strTitle);
		}
		else if (!m_bPlayingSelected)
		{
			pView->StopSequence();
		}
	}
}

void CModelViewDlg::OnBnClickedModelviewTbButtonRemoveanim()
{
	int nSelection = m_AnimationListBox.GetCurSel();
	if (nSelection != LB_ERR)
	{
		m_AnimationListBox.DeleteString(nSelection);
		if (nSelection >= m_AnimationListBox.GetCount())
		{
			nSelection = m_AnimationListBox.GetCount() - 1;
		}
		m_AnimationListBox.SetCurSel(nSelection);
	}
}

void CModelViewDlg::OnBnClickedModelviewTbButtonResetLookat()
{
	CModelViewRender *pView = SINGLETONINSTANCE(CModelViewEditor)->GetActiveView();
	if (pView)
		pView->ResetPosition();
}

void CModelViewDlg::OnBnClickedModelviewTbButtonResetRotation()
{
	CModelViewRender *pView = SINGLETONINSTANCE(CModelViewEditor)->GetActiveView();
	if (pView)
		pView->ResetRotation();
}

void CModelViewDlg::OnEnChangeModelviewTbEditLookoffset()
{
	CModelViewRender *pView = SINGLETONINSTANCE(CModelViewEditor)->GetActiveView();
	if (pView)
		pView->PositionChanged();
}

void CModelViewDlg::OnEnChangeModelviewTbEditRotation()
{
	CModelViewRender *pView = SINGLETONINSTANCE(CModelViewEditor)->GetActiveView();
	if (pView)
		pView->RotationChanged();
}

void CModelViewDlg::OnUpdateAddAnimation(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsAnimationSelected());
}

void CModelViewDlg::OnUpdateClearAnimationSequence(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsSequencePresent());
}

void CModelViewDlg::OnUpdateRemoveAnimation(CCmdUI* pCmdUI)
{
	bool bEnabled = m_AnimationListBox.GetCurSel() != LB_ERR;
	pCmdUI->Enable(bEnabled);
}
