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

#if !defined(AFX_GUIEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_GUIEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

// GUIEditor.cpp : implementation file
//

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUIEditor.h : header file
//
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

class CGUIResBox;
class CGUITabBar;

/////////////////////////////////////////////////////////////////////////////
// CGUIEditor document


class CGUIEditor : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CGUIEditor);
	CGUIEditor();
// attributes
private:
	UINT m_PageIndex;
	RESOURCECOLLECTION m_ObjCollection;
	
// implementation
public:   
	~CGUIEditor();
    virtual CString *GetName();
	virtual DWORD GetPriority();
    virtual BOOL Initialize();

// message handlers
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);
	void OnCreate(CMDIFrameWnd *mainWnd);
	DWORD EditDocument();

	void LoadGXL(GUIPAGELIST gpl, TCHAR* fileName);	

	void CreateDocView(UINT xRes, UINT yRes);
	
	static void GUIEditorCreationCallback(void *userData);	

	void CreateWidgets();

	void SetDocOpen(bool value)
	{
		m_bDocOpen = value; 
		// TODO: get ken to show me how to hide tab
	}

	void UpdateBarPos(UINT val);

// attributes
private:
	CMultiDocTemplate *m_DocTemplate;
	IToolBox *m_ToolBox;
	CGUIResBox m_ResBox;
	CGUITabBar m_TabBar;
	bool m_bDocOpen;
	bool m_bCreated;

	void SetupTabBar();
};

extern AFX_EXTENSION_MODULE GUIEditorDLL;

#undef AFX_DATA
#define AFX_DATA

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
