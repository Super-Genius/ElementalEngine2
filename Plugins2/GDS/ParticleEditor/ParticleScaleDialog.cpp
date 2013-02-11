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
#include "ParticleEditor.h"
#include "ParticleScaleDialog.h"
#include ".\particlescaledialog.h"
#include "resource.h"
#include "stdafx.h"


// CParticleScaleDialog dialog

IMPLEMENT_DYNAMIC(CParticleScaleDialog, CDialog)
CParticleScaleDialog::CParticleScaleDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CParticleScaleDialog::IDD, pParent)
{
}

CParticleScaleDialog::~CParticleScaleDialog()
{
}

void CParticleScaleDialog::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_SCALEDLGEDIT, m_EditScale);
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CParticleScaleDialog, CDialog)
	ON_EN_CHANGE(IDC_SCALEDLGEDIT, OnEnChangeScaledlgedit)
END_MESSAGE_MAP()


// CParticleScaleDialog message handlers

void CParticleScaleDialog::OnEnChangeScaledlgedit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

BOOL CParticleScaleDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Set initial value for scale.
	m_szInitialScale = _T("1.0");
	m_EditScale.SetWindowText(m_szInitialScale);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CParticleScaleDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CParticleDoc * pDoc;
	pDoc = CParticleDoc::GetActiveDocument();
	assert(pDoc);

	IParticleEmitter * pEmitter;
	pEmitter = pDoc->GetEmitterPointer();
	assert(pEmitter);
	TCHAR czNewScale[256];
	m_EditScale.GetWindowText(czNewScale, 256);
	bool bValid = true;
	for (int i = 0; (i < 256) && ( czNewScale[i] != NULL ) && bValid; i++)
	{
		if (!( isdigit(czNewScale[i]) || czNewScale[i] == '.'))
			bValid = false;
	}
	if (bValid)
	{
        float fScale = (float)atof( czNewScale );
		pDoc->ScaleParticle(fScale);
		CDialog::OnOK();
	}
	else
	{
		m_EditScale.SetWindowText(m_szInitialScale);
	}
}
