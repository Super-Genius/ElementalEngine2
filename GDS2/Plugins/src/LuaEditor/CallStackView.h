///============================================================================
/// \file		CallStackView.hpp
/// \brief		Header file for the Call Stack Viewer
/// \date		06-17-2005
/// \author		Josh Lulewicz
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

class CCallStack : public CWnd
{
public:
	CCallStack(void);
	~CCallStack(void);
	void Clear();
	void Write(CString strMsg);
	void GotoStackTraceLevel(int nLevel);
	void Add(const char* szDesc, const char* szFile, int nLine);
	virtual int OnSci(SCNotification* pNotify);
	int GetLevel() { return m_nCurrentLevel; };

protected:
	CLuaEditCtrl m_view;
	int m_nCurrentLevel;
	CUIntArray	m_levels, m_lines;
	CStringArray m_files;

	// Generated message map functions
protected:
	//{{AFX_MSG(CScintillaView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
