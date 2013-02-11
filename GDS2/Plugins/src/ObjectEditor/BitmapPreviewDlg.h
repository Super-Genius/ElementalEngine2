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

#pragma once

// CBitmapPreviewDlg dialog

class CBitmapPreviewDlg : public CDialog
{
	DECLARE_DYNAMIC(CBitmapPreviewDlg)

public:
	CBitmapPreviewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBitmapPreviewDlg();

// Dialog Data
	enum { IDD = IDD_BITMAP_WINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// Preview of the bitmap we generate
	CButton m_BitMapPreview;
};
