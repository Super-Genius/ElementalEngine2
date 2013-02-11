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
#include "ParticleRenameDialog.h"
#include ".\particlerenamedialog.h"


// CParticleRenameDialog dialog

IMPLEMENT_DYNAMIC(CParticleRenameDialog, CDialog)
CParticleRenameDialog::CParticleRenameDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CParticleRenameDialog::IDD, pParent)
{
}

CParticleRenameDialog::~CParticleRenameDialog()
{
}

void CParticleRenameDialog::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_PARTICLE_RENAME_EDIT, m_EditRename);
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CParticleRenameDialog, CDialog)
	ON_EN_CHANGE(IDC_PARTICLE_RENAME_EDIT, OnEnChangeParticleRenameEdit)
END_MESSAGE_MAP()


// CParticleRenameDialog message handlers

void CParticleRenameDialog::OnEnChangeParticleRenameEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here


}
void CParticleRenameDialog::SetBrushNumber(int brushnum)
{
	m_iBrushNum = brushnum;
}

BOOL CParticleRenameDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CParticleDoc * pDoc;
	pDoc = CParticleDoc::GetActiveDocument();
	assert(pDoc);

	IParticleEmitter * pEmitter;
	pEmitter = pDoc->GetEmitterPointer();
	assert(pEmitter);

	m_szInitalpath = pEmitter->GetBrush(m_iBrushNum)->GetName()->GetString();
	m_EditRename.SetWindowText(m_szInitalpath);
//	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_PARTICLE_RENAME_EDIT);
//	pEdit->SetWindowText(m_szInitalpath);

	
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CParticleRenameDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CParticleDoc * pDoc;
	pDoc = CParticleDoc::GetActiveDocument();
	assert(pDoc);

	IParticleEmitter * pEmitter;
	pEmitter = pDoc->GetEmitterPointer();
	assert(pEmitter);

	TCHAR czNewFilepath[_MAX_PATH];

//	CEdit * pEdit = (CEdit *)::GetDlgItem(this->m_hwnd, IDC_PARTICLE_RENAME_EDIT);
//	pEdit->GetWindowText(czNewFilepath, _MAX_PATH);
	m_EditRename.GetWindowText(czNewFilepath, _MAX_PATH);

	pDoc->RenameParticleComponent(PARTICLE_BRUSH, m_szInitalpath, StdString(czNewFilepath));

	CDialog::OnOK();
}
