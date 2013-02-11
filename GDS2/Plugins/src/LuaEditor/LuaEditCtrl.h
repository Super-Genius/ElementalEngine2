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

#if !defined(AFX_LUAEDITOR_H__C002AF8D_DFEF_4577_A494_10F2B3DF2458__INCLUDED_)
#define AFX_LUAEDITOR_H__C002AF8D_DFEF_4577_A494_10F2B3DF2458__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LuaEditor.h : header file
//

#include "Platform.h"
#include "scintilla.h"

/////////////////////////////////////////////////////////////////////////////
// CLuaEditCtrl window

class CProjectFile;
class CLuaDoc;

class CLuaEditCtrl : public CWnd
{
// Construction
public:
	CLuaEditCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLuaEditCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetLuaLexer();
	BOOL PreparePrint(CDC* pDC, CPrintInfo* pInfo);
	void PrintPage(CDC* pDC, CPrintInfo* pInfo);
	void EndPrint(CDC* pDC, CPrintInfo* pInfo);
	void SetStackTraceLevel(int nLevel);
	void SetCallStackMargins();
	int GetCurrentLine();
	void GotoLastLine();
	CString GetLine(int nLine);
	void ClearAll();
	void AddText(const char* szText);
	void SetReadOnly(BOOL bReadOnly);
	void SetEditorMargins();
	void GrabFocus();
	void Copy();
	void Paste();
	BOOL CanPaste();
	void Clear();
	void Cut();
	BOOL CanCutOrClear();
	void SelectAll();
	void Redo();
	BOOL CanRedo();
	void Undo();
	BOOL CanUndo();
	void SetBreakPointsIn(CLuaDoc *pDoc);
	void GotoLine(int nLine);
	void SetBreakpoint(int nLine);
	void ClearAllBreakpoints();
	BOOL Save(CFile* pFile);
	BOOL Load(CFile* pFile);
	BOOL Load(char *loadData, int size);
	BOOL ToggleBreakpoint(int nLine);
	int LineFromPoint(CPoint& pt);
	BOOL Create(CWnd* pParentWnd, UINT nCtrlID);
	void ClearMarkers();
	virtual ~CLuaEditCtrl();
	int GetDataSize();
	void GetData(char *data);
	void SetSavePoint();

// thread safe functions
	//void tsAddText(const char *szMsg) {	AddText(szMsg); };
	//void tsSetReadOnly(BOOL bReadOnly) { SetReadOnly(bReadOnly); };

	void Print(bool showDialog);
	CharacterRange GetSelection();
protected:
	int (*m_fnScintilla)(void*,int,int,int);
	void *m_ptrScintilla;
	CUIntArray	m_pages;
	BOOL m_bShowCalltips;
	CString m_strCallTip;

	int Sci(int nCmd, int wParam=0, int lParam=0);

	// Generated message map functions
protected:
	//{{AFX_MSG(CLuaEditCtrl)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LUAEDITOR_H__C002AF8D_DFEF_4577_A494_10F2B3DF2458__INCLUDED_)
