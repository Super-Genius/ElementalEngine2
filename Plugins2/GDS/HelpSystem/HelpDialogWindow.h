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
#include "afxdlgs.h"

#include <memory>

#include "ResizablePropertySheet.h"
#include "SizeBar.h"
#include "PropPageContents.h"
#include "PropPageIndex.h"
#include "PropPageSearch.h"
#include "BrowserPane.h"

// CHelpDialogWindow

class CHelpDialogWindow : public CWnd
{
	DECLARE_DYNAMIC(CHelpDialogWindow)

public:
	CHelpDialogWindow(CHelpSystemCore * helpCore);
	virtual ~CHelpDialogWindow();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnDestroy();

	/// Create help dialog window.
	BOOL CreateHelpDialogWindow();

	/// Returns true, if help dialog window is exists.
	bool IsExists() const { return m_bIsExists; }

	/// Popup window.
	void PopupWindow();

	/// Display topic by CHM reference and topic path.
	bool DisplayTopic(const CHelpSystemCore::CChm &chmRef, const std::string &path);

	/// Display HTML page from local resource.
	bool DisplayResource(const CString &name);

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

private:
	enum {
		BMN_SHOW = 12, BMN_HIDE = 13, BMN_SYNC = 9, BMN_BROWSE_PREVIOUS = 14,
		BMN_BROWSE_NEXT = 8, BMN_BROWSE_BACK = 0, BMN_BROWSE_FORWARD = 1,
		BMN_BROWSE_STOP = 2, BMN_BROWSE_REFRESH = 3, BMN_BROWSE_HOME = 4,
		BMN_BROWSE_FONT = 16, BMN_BROWSE_PRINT = 7
	};

	BOOL OnSync();
	BOOL OnShowHide();

	CHelpSystemCore * m_helpCore;
	bool m_bIsExists;

	CImageList m_toolBarImages;
	CToolBarCtrl m_toolBar;

	CResizablePropertySheet m_leftSheet;
	CPropPageContents m_contents;
	CPropPageIndex m_index;
	CPropPageSearch m_search;

	CBrowserPane m_browserPane;

	CSizeBar m_sizeBar;
};


