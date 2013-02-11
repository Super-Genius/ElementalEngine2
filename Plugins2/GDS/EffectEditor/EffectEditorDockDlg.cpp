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
#include "EffectEditorDockDlg.h"
#include ".\effecteditordockdlg.h"


// CEffectEditorDockDlg dialog

IMPLEMENT_DYNAMIC(CEffectEditorDockDlg, CDialog)
CEffectEditorDockDlg::CEffectEditorDockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEffectEditorDockDlg::IDD, pParent)
{
	m_ToolBox = EngineGetToolBox();
	m_pEditor = SINGLETONINSTANCE(CEffectEditor);
}

CEffectEditorDockDlg::~CEffectEditorDockDlg()
{
}

void CEffectEditorDockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEffectEditorDockDlg, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CEffectEditorDockDlg message handlers

void CEffectEditorDockDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}
