///============================================================================
/// \file		ModelViewDockWin.h
/// \brief		Header file for World Event Dock Window
/// \date		06-06-2007
/// \author		Brian Bazyk
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

#ifndef _MODELVIEWDOCKWIN_H_
#define _MODELVIEWDOCKWIN_H_

class CModelViewDockWin : public CGuiControlBar
{
	DECLARE_DYNAMIC(CModelViewDockWin)

public:
	CModelViewDockWin();
	virtual ~CModelViewDockWin();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	CModelViewDlg m_ModelViewDialog;

private:
	IToolBox *m_ToolBox;
};

#endif // #ifndef _MODELVIEWDOCKWIN_H_
