///============================================================================
/// \file		WorldEventDockWin.h
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

#ifndef _WORLDEVENTDOCKWIN_H_
#define _WORLDEVENTDOCKWIN_H_

class CWorldEventDockWin : public CGuiControlBar
{
	DECLARE_DYNAMIC(CWorldEventDockWin)

public:
	CWorldEventDockWin();
	virtual ~CWorldEventDockWin();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	void SetSelectedObject(IHashString *pObjectName);

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	CWorldEventDlg m_WorldEventDialog;
};

#endif // #ifndef _WORLDEVENTDOCKWIN_H_
