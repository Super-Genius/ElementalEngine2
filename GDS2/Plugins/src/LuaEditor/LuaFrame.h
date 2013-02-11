///============================================================================
/// \file		LuaFrame.h
/// \brief		Header file for the Lua Frame class
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

#if !defined(AFX_LUAFRAME_H__5C37E669_4D8A_474B_88BD_43C4D9FB071D__INCLUDED_)
#define AFX_LUAFRAME_H__5C37E669_4D8A_474B_88BD_43C4D9FB071D__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum {
	modeNoProject,
	modeBuild,
	modeDebug,
	modeDebugBreak
} appMode;

class CLuaFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CLuaFrame)
public:
	CLuaFrame();
	//LRESULT DebugMessage(UINT nMsg, WPARAM wParam, LPARAM lParam);
	LRESULT DebugMessage(UINT nMsg, WPARAM wParam, LPARAM lParam);
// Attributes
public:
	CLuaToolPal m_LuaToolPal;
	HINSTANCE m_AppInst;
//	void SetMode(int nMode);
	int GetMode() { return m_nAppMode; };

	/// Write a message to an output window
	/// \param message - string to be written
	/// \param tab - output window you want the message to appear in
	/// \return void
	void Write(StdString *message, outputTypes tab = outputBuild);

	/// Clear the specified output window
	/// \param tab - output window you want to clear
	/// \return void
	void Clear(outputTypes tab = outputBuild);

	/// Update variable watches
	/// \param void
	/// \return void
	void UpdateWatches();

	/// Put and arrow at the current line
	/// \param nLine - current line of execution
	/// \return void
	void GotoLine(int nLine);

	/// Clear arrow markers in the Scnintilla view
	/// \param void
	/// \return void
	void ClearMarkers();

	/// Find out if we are debugging
	/// \param void
	/// \return true if we are, false if not
	bool IsDebugging() { return m_bDebugging; }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLuaFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLuaFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CDebugger* GetDebugger() { return &m_debug; }
	bool Debugging() { return m_bDebugging; }
// Generated message map functions
protected:
	//{{AFX_MSG(CLuaFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	afx_msg void OnCompile();
	afx_msg void OnExecute();
	afx_msg void OnGo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	bool CompileCode();
	int m_nAppMode;
	CDebugger m_debug;
	IToolBox *m_ToolBox;
	bool m_bDebugging;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnUpdateLuaGo(CCmdUI *pCmdUI);
	afx_msg void OnDebugStepinto();
	afx_msg void OnUpdateDebugStepinto(CCmdUI *pCmdUI);
	afx_msg void OnDebugRestart();
	afx_msg void OnUpdateDebugRestart(CCmdUI *pCmdUI);
	afx_msg void OnDebugRuntocursor();
	afx_msg void OnUpdateDebugRuntocursor(CCmdUI *pCmdUI);
	afx_msg void OnDebugStepout();
	afx_msg void OnUpdateDebugStepout(CCmdUI *pCmdUI);
	afx_msg void OnDebugStepover();
	afx_msg void OnUpdateDebugStepover(CCmdUI *pCmdUI);
	afx_msg void OnDebugStopdebugging();
	afx_msg void OnUpdateDebugStopdebugging(CCmdUI *pCmdUI);
	afx_msg void OnLuaViewToolbar();
	afx_msg void OnUpdateLuaViewToolbar(CCmdUI *pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LUAFRAME_H__5C37E669_4D8A_474B_88BD_43C4D9FB071D__INCLUDED_)
