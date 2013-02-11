///============================================================================
/// \file		ProfilerWindow.h
/// \brief		Window class for profiler window
/// \date		06/11/2007
/// \author		Nick Tourte
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

#ifndef _C_PROFILERWINDOW_H_
#define _C_PROFILERWINDOW_H_

class CProfilerTabCtrl;

class CProfilerWindow : public CWnd
{
	DECLARE_DYNAMIC(CProfilerWindow)

public:
	CProfilerWindow();
	virtual ~CProfilerWindow();	

	void UpdateProfiles(PERFORMANCEPROFILERTYPEMAP &map);
	void UpdateFPS(float fps);
	
protected:
	DECLARE_MESSAGE_MAP()	

	CProfilerTabCtrl *m_TabControlBox;
	bool m_bHiddenCursor;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};


#endif