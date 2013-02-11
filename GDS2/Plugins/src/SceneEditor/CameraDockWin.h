///============================================================================
/// \file		CameraDockWin.h
/// \brief		Header file for Camera Dock Window
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

#ifndef _CAMERADOCKWIN_H_
#define _CAMERADOCKWIN_H_

#include "CameraDlg.h"

class CCameraDockWin : public CGuiControlBar
{
	DECLARE_DYNAMIC(CCameraDockWin)

public:
	CCameraDockWin();
	virtual ~CCameraDockWin();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// message handlers
	/// called to set the checked state of the Camera toolbar menu Item depending on the current state
	/// of the toolbar. If visible: menuItem is checked.
	void OnUpdateViewCameraToolbar( CCmdUI* pCmdUI );

	/// Called if the 'camera toolbar' menuItem was clicked. Shows/Hides the camera
	/// panel.
	void OnViewCameraToolbar();

public:
	/// pointer to our camera dialog
	CCameraDlg* m_CameraDialog;

private:
	/// toolbox reference for convenience 
	IToolBox *m_ToolBox;
};

#endif // #ifndef _CAMERADOCKWIN_H_
