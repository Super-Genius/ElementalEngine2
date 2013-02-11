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

#if !defined(AFX_MAINFRM_H__7D3800B6_8EA3_4AEC_95AA_860B10E23171__INCLUDED_)
#define AFX_MAINFRM_H__7D3800B6_8EA3_4AEC_95AA_860B10E23171__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NewObjectDialog.h"

struct ltstr
{
  bool operator()(LPCSTR s1, LPCSTR s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};

class CGeneralOptions;

class CMainFrame : public CGuiMDIFrame
{
	DECLARE_DYNAMIC(CMainFrame)

private:
	HMENU m_hMainMenu;
	HACCEL m_hMainAccel;
	list< FILETYPECREATIONINFO > m_FileTypeCreationList;
public:
	CMainFrame();
	bool OnRegisterFileTypeCreationInfo( FILETYPECREATIONINFO * pInfo );

// Attributes
public:
	// The property sheet
	COptionsDialog *m_OptDialog;

	// The first page on the property seet
	CGeneralOptions *m_GeneralOptPage;

// Operations
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
				AFX_CMDHANDLERINFO* pHandlerInfo);
	BOOL BroadcastMessage(UINT nIDmsg, WPARAM wParam, LPARAM lParam, HWND tWnd = NULL);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	void SetMenuAndAccel(HMENU menu, HACCEL accel)
	{
		m_hMainMenu = menu;
		m_hMainAccel = accel;
	}

	void GetMenuAndAccel(HMENU *menu, HACCEL *accel)
	{
		*menu = m_hMainMenu;
		*accel = m_hMainAccel;
	}

	void LoadMenuToolBar(UINT rsrc, CBitmap *bm);

	void AddToolBar(TOOLBARSTRUCT *atbs);
	void IsBarActive( TOOLBARACTIVESTRUCT *atbs );

	/// Set the active tool bar
	/// \param atbs - pointer to TOOLBARACTIVESTRUCT which contains the bar to activate
	/// \return void
	void SetActiveBar(TOOLBARACTIVESTRUCT *atbs);

	/// hides the control bar
	/// \param cBar = the control bar to hide
	/// \return void
	void HideBar(CControlBar *cBar);

	/// \brief	Get currently selected view
	/// \return	pointer to a currently selected CView
	const CView *GetActiveView() const;
	/// \brief	Select passed view as active
	/// \param	pView - pointer to a CView to select
	void SetActiveView(CView *pView);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	BOOL VerifyBarState(LPCTSTR lpszProfileName);
	void UpdateProgress(DWORD percentage);
	void PauseScene(bool bScenePaused);

protected:  // control bar embedded members
	CGuiProgStatusBar	m_wndProgStatusBar;
	CGuiToolBarTabbed m_TabbedToolBar;
	CGuiToolBarWnd	m_wndToolBar;		// our main toolbar
	CBitmap			m_bmToolbarHi;
	CBitmap			m_bmToolbarHiD;
	CResourceBar	m_ResourceBar;
	CNewObjectDialog	m_NewObjectDialog;
	/// timer
	unsigned int m_iTimer;

	bool m_bScenePaused;
	bool m_bProfilerVisible;
	
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnViewObjects();
	afx_msg void OnUpdateViewObjects(CCmdUI* pCmdUI);
	afx_msg void OnToolsOptions();
	afx_msg void OnUpdateToolsOptions(CCmdUI* pCmdUI);
	afx_msg void OnFileNewFile();
	//}}AFX_MSG
	afx_msg LRESULT OnGetStatusBar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddOptionPageMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHelpFinder();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	afx_msg void OnToolsPausescene();
	afx_msg void OnUpdateToolsPausescene(CCmdUI *pCmdUI);
	afx_msg void OnToolsShowprofiler();
	afx_msg void OnUpdateToolsShowprofiler(CCmdUI *pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__7D3800B6_8EA3_4AEC_95AA_860B10E23171__INCLUDED_)
