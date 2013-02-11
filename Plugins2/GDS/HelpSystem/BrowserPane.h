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
#include "browser.h"


// CBrowserPane dialog

class CBrowserPane : public CDialog
{
	DECLARE_DYNAMIC(CBrowserPane)

public:
	CBrowserPane(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBrowserPane();

// Dialog Data
	enum { IDD = IDD_BROWSER_PANE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	DECLARE_EVENTSINK_MAP()

public:
	BOOL Create(CWnd *pParent);

	/// Display page inside the CHM file.
	/// \param[in] chmFilename Fully qualified CHM filename and path.
	/// \param[in] path Path to topic inside CHM file.
	bool DisplayCHM(const CString &chmFilename, const CString &path);

	/// Display page from local resource.
	/// \param[in] name Resource name.
	bool DisplayResource(const CString &name);

	/// Retrieve URL as is.
	/// \return true if succeed, otherwise returns false.
	bool GetURL(CString &url);

	/// Retrieve CHM filename and path.
	/// \return true if succeed and current URL is actually specifies the
	/// page insed CHM. Otherwise returns false.
	bool GetCHM(CString &chmFilename, CString &path);

	/// Navigate browser back.
	bool GoBack();

	/// Navigate browser forward.
	bool GoForward();

	/// Stop page loading.
	bool Stop();

	/// Refresh current page.
	bool Refresh();

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void NavigateErrorBrowser(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Frame, VARIANT* StatusCode, BOOL* Cancel);

private:
	static const CString MSITSPREFIX1;
	static const CString MSITSPREFIX2;

	static CString MakeCHMURL(const CString &chmFilename, const CString &path);

	CBrowser m_browser;
	bool m_bInit;
};
