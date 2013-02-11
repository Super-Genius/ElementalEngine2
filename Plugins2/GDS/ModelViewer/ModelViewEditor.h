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

#if !defined(AFX_ModelViewEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_ModelViewEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

// ModelViewEditor.h : header file
//
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

class CModelViewDlg;
class CModelViewToolBar;
/////////////////////////////////////////////////////////////////////////////
// CModelViewEditor document

class CModelViewEditor : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CModelViewEditor);
	CModelViewEditor();

// implementation
public:   
	~CModelViewEditor();
	static void ModelViewEditorCreationCallback(void *userData);
	virtual CString *GetName();
	virtual DWORD GetPriority();
    virtual BOOL Initialize();
	CModelViewToolBar* GetToolBar();
	CModelViewDlg* GetDockDlg();

	// message handlers
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	void OnCreate(CMDIFrameWnd *mainWnd);
	DWORD EditDocument(LPCTSTR name/* IArchive &ar */);

	CModelViewRender* FindView(LPCTSTR szModelName);

	/// Calls Update on all views
	void Update();
	CModelViewRender* GetActiveView();

private:
	void DisplayToolbar(bool display);

	// attributes
private:
	CModelViewToolBar m_ModelViewToolBar;
	CModelViewDockWin m_ModelViewDockWin;	

	/// boolean if the toolbar is active
	bool m_bToolBarIsActive;
	CMultiDocTemplate *m_DocTemplate;
	IToolBox *m_ToolBox;
	UINT m_PageIndex;
	RESOURCECOLLECTION m_ObjCollection;

};

extern AFX_EXTENSION_MODULE ModelViewEditorDLL;

#undef AFX_DATA
#define AFX_DATA

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ModelViewEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
