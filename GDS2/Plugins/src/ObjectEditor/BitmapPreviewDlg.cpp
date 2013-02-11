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
#include "ObjectEditor.h"
#include "BitmapPreviewDlg.h"


// CBitmapPreviewDlg dialog

IMPLEMENT_DYNAMIC(CBitmapPreviewDlg, CDialog)
CBitmapPreviewDlg::CBitmapPreviewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBitmapPreviewDlg::IDD, pParent)
{
	Create(IDD, pParent);
}

CBitmapPreviewDlg::~CBitmapPreviewDlg()
{
}

void CBitmapPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BITMAP_PREVIEW, m_BitMapPreview);
}


BEGIN_MESSAGE_MAP(CBitmapPreviewDlg, CDialog)
END_MESSAGE_MAP()


// CBitmapPreviewDlg message handlers
