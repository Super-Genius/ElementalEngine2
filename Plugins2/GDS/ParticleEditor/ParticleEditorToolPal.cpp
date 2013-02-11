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
#include "ParticleEditorToolPal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CParticleEditorToolPal, CGuiDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
	ON_UPDATE_COMMAND_UI(ID_PARTICLES_ADDBRUSH, OnUpdateButtons)
	//ON_BN_CLICKED(ID_PARTICLES_SHOWPARTICLES, OnButtonShowParticleClicked)
	//ON_BN_CLICKED(ID_PARTICLE_MOVE, OnButtonMoveParticleClicked)
	ON_BN_CLICKED(ID_PARTICLES_ADDBRUSH, OnButtonAddBrushClicked)
	ON_EN_CHANGE(IDC_SPEEDVALUE, OnEnChangeSpeedvalue)
	ON_BN_CLICKED(ID_PARTICLES_SCALE, OnBnClickedParticlesScale)
	ON_UPDATE_COMMAND_UI(ID_PARTICLES_SCALE, OnUpdateButtons)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CParticleEditorToolPal, CGuiDialogBar)

/////////////////////////////////////////////////////////////////////////////
// CParticleEditorToolPal
CParticleEditorToolPal::CParticleEditorToolPal()
{
	m_pControlData = NULL;
	m_bActive = false;
}

CParticleEditorToolPal::~CParticleEditorToolPal()
{
}
CParticleEditorDataExchange * CParticleEditorToolPal::GetData(void)
{
	return m_pControlData;
}

void CParticleEditorToolPal::SetData(CParticleEditorDataExchange * pParticleToolData)
{
	m_pControlData = pParticleToolData;
	if (m_pControlData)
	{
//		float fValue;
		TCHAR czBuffer[256];
		czBuffer[0] = '\0';
		if (m_bActive)
		{
//			fValue = m_pControlData->GetOffsetVec3()->x;
//			_stprintf(czBuffer,_T("%5.2f"), fValue);
//			m_EditLookAtOffsetX.SetWindowText( czBuffer );

		}
	}
}

void CParticleEditorToolPal::DoDataExchange(CDataExchange* pDX)
{
	CGuiDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, ID_PARTICLES_SHOWPARTICLES, m_ButtonShowParticles);
	DDX_Control(pDX, ID_PARTICLE_MOVE, m_ButtonMoveParticles);
	DDX_Control(pDX, ID_PARTICLES_ADDBRUSH, m_ButtonAddBrush);
	DDX_Control(pDX, IDC_SPEEDVALUE, m_EditMoveSpeed);
	DDX_Control(pDX, ID_PARTICLES_SCALE, m_ButtonScaleParticle);

}

LRESULT CParticleEditorToolPal::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = HandleInitDialog(wParam, lParam);

	m_bActive = true;

    UpdateData(FALSE);

	m_ButtonShowParticles.SetIcon((HICON)LoadImage(ParticleEditorDLL.hModule, 
		MAKEINTRESOURCE(ID_PARTICLES_SHOWPARTICLES), IMAGE_ICON, 0, 0, 0));	

	m_ButtonAddBrush.SetIcon((HICON)LoadImage(ParticleEditorDLL.hModule, 
		MAKEINTRESOURCE(ID_PARTICLES_ADDBRUSH), IMAGE_ICON, 0, 0, 0));	

	m_ButtonMoveParticles.SetIcon((HICON)LoadImage(ParticleEditorDLL.hModule, 
	MAKEINTRESOURCE(ID_PARTICLE_MOVE), IMAGE_ICON, 0, 0, 0));	

	m_ButtonScaleParticle.SetIcon((HICON)LoadImage(ParticleEditorDLL.hModule, 
	MAKEINTRESOURCE(ID_PARTICLES_SCALE), IMAGE_ICON, 0, 0, 0));	

	
	ResetToolBar();

	return bRet;
}

void CParticleEditorToolPal::OnButtonShowParticleClicked()
{
	if (m_pControlData)
	{
		int checkvalue = m_ButtonShowParticles.GetCheck();
		// Activate Particle System
		if ( BST_CHECKED == checkvalue)
			m_pControlData->SetRenderActive(true);
		// Deactivate Particle System
		else
			m_pControlData->SetRenderActive(false);
	}
	return;
}

void CParticleEditorToolPal::OnButtonMoveParticleClicked()
{
	if (m_pControlData)
	{
		int checkvalue = m_ButtonMoveParticles.GetCheck();

		// Activate Particle Movement
		if ( BST_CHECKED == checkvalue)
		{
			OnEnChangeSpeedvalue();
			m_pControlData->SetParticleMoving(true);
		}
		// Deactivate Particle Movement
		else
			m_pControlData->SetParticleMoving(false);

	}
	return;
}

void CParticleEditorToolPal::OnEnChangeSpeedvalue()
{
	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	m_EditMoveSpeed.GetWindowText( szBuffer, 256 );
	float fMoveSpeed = (float)atof( StdString(szBuffer) );
	if( fMoveSpeed < 0.0f )
	{
		fMoveSpeed = 0.0f;
		m_EditMoveSpeed.SetWindowText( _T("0.0") );
	}
	else if( fMoveSpeed > 3000.0f )
	{
		fMoveSpeed = 3000.0f;
		m_EditMoveSpeed.SetWindowText( _T("3000.0") );
	}
	if (m_pControlData)
	{
		m_pControlData->SetParticleMoveSpeed(fMoveSpeed);
	}
}

void CParticleEditorToolPal::OnButtonAddBrushClicked()
{
	CParticleDoc * pDoc = CParticleDoc::GetActiveDocument();
	if (!pDoc)
		return;

	pDoc->AddNewBrush();
}

void CParticleEditorToolPal::ResetToolBar()
{
	UpdateData(TRUE);
	m_ButtonShowParticles.SetCheck( BST_CHECKED );
	m_ButtonMoveParticles.SetCheck( BST_CHECKED );
}

void CParticleEditorToolPal::OnBnClickedParticlesScale()
{
	CParticleScaleDialog ScaleDialog;
	ScaleDialog.DoModal();	
}

bool CParticleEditorToolPal::AreParticlesShown() const
{
	return BST_CHECKED == m_ButtonShowParticles.GetCheck();
}

void CParticleEditorToolPal::ShowParticles( bool bShow )
{
	m_ButtonShowParticles.SetCheck( bShow ? BST_CHECKED : BST_UNCHECKED );
}

float CParticleEditorToolPal::GetMoveSpeed() const
{
	TCHAR szBuffer[32];
	m_EditMoveSpeed.GetWindowText( szBuffer, _countof(szBuffer) );
	float fMoveSpeed = (float)atof( StdString(szBuffer) );
	if( fMoveSpeed < 0.0f )
	{
		fMoveSpeed = 0.0f;
	}
	else if( fMoveSpeed > 3000.0f )
	{
		fMoveSpeed = 3000.0f;
	}
	if (m_ButtonMoveParticles.GetCheck() != BST_CHECKED)
	{
		fMoveSpeed = 0.0f;
	}
	return fMoveSpeed;
}

void CParticleEditorToolPal::SetMoveSpeed( float fMoveSpeed )
{
	if (fMoveSpeed < 0.0f)
	{
		fMoveSpeed = 0.0f;
	}
	else if( fMoveSpeed > 3000.0f )
	{
		fMoveSpeed = 3000.0f;
	}
	TCHAR szBuffer[32];
	_stprintf( szBuffer, _T("%5.2f"), fMoveSpeed );
	m_EditMoveSpeed.SetWindowText( szBuffer );
}

