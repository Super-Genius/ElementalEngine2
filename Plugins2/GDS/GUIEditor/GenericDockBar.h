///============================================================================
/// \file       DockScriptTree.h
/// \brief      Header file for GDS State Editor dialog docking wrapper
/// \date       06-24-2005
/// \author     Brent Arias
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

#ifndef _CDOCSCRIPTTREE_
#define _CDOCSCRIPTTREE_
#include "guicontrolbar.h"

class CGenericDockBar : public CGuiControlBar
{
	DECLARE_DYNAMIC(CGenericDockBar)

public:

	/// Creates a dock bar that can house a dialog
	/// \param innerWnd - dialog box to implant
	/// \param title - title of the dock bar
	/// \param id - id of dialog box
    CGenericDockBar(CDialog *innerWnd, UINT id, CHashString title);
    ~CGenericDockBar(void);

	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

    ///Receives messages not through MFC, but through EE instead.
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    ///Receives messages not through MFC, but through EE instead.
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	DECLARE_MESSAGE_MAP()
 
private:
    /// the dialog bar located inside the dockbar
	CDialog *m_pInnerWnd;
	CHashString m_szTitle;
	UINT m_iID;
public:
	afx_msg void OnShowHide();
};


#endif