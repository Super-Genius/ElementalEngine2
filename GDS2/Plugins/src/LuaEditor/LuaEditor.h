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

#if !defined(AFX_LuaEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_LuaEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

// LuaEditor.cpp : implementation file
//

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LuaEditor.h : header file
//
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

/////////////////////////////////////////////////////////////////////////////
// CLuaEditor document


class CLuaEditor : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CLuaEditor);
	CLuaEditor();
// attributes
private:
	UINT m_PageIndex;
	RESOURCECOLLECTION m_ObjCollection;
	
// implementation
public:   
	~CLuaEditor();
    virtual CString *GetName();
	virtual DWORD GetPriority();
    virtual BOOL Initialize();

	/// File->New callback
	static void LuaCreationCallback(void *userData);

// message handlers
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);
	void OnCreate(CMDIFrameWnd *mainWnd);
	DWORD EditDocument(LPCTSTR lpszPathName);

	CLuaDockToolTab* GetOutputWnd() { return &m_ToolDockBar; }
	void DecrementRefCount_();
	void ToggleToolTabs_(BOOL bShow);
	BOOL IsToolWndVisible_() { return m_ToolDockBar.IsVisible(); }

	void GetDocSize(int &size);
	void GetDocData(char *data);

// attributes
private:
	CMultiDocTemplate *m_DocTemplate;
	IToolBox *m_ToolBox;
	/// instance of scintilla window
	HINSTANCE m_hScintilla;
	/// tool bar (build, debug, etc)
	CLuaDockToolTab	m_ToolDockBar;
};

extern AFX_EXTENSION_MODULE LuaEditorDLL;

#undef AFX_DATA
#define AFX_DATA

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LuaEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
