#if !defined(AFX_SCENEDLDOCKWIN_H__51FB2C5B_289A_4AC0_818F_5625A0D66922__INCLUDED_)
#define AFX_SCENEDLDOCKWIN_H__51FB2C5B_289A_4AC0_818F_5625A0D66922__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SceneDLDockWin.h : header file
//

class CSceneDLDoc;

// this is used to map GUIDS or object to icons
typedef struct GUIDTOICON
{
	const GUID *type;
	DWORD iconID;
} GUIDTOICON;

/////////////////////////////////////////////////////////////////////////////
// CSceneDLDockWin dialog
// {2C294076-10A3-4761-918D-444925A9F5E6}
static const GUID WORLD_GUID = 
{ 0x2c294076, 0x10a3, 0x4761, { 0x91, 0x8d, 0x44, 0x49, 0x25, 0xa9, 0xf5, 0xe6 } };

using namespace std;

typedef map<CBaseObject *, struct HIERARCHYINFO *> MAPOBJTOHIER;

class CSceneDLDockWin : public CGuiControlBar
{
	DECLARE_DYNCREATE(CSceneDLDockWin)
// Construction
public:
	CSceneDLDockWin();   // standard constructor
	~CSceneDLDockWin();   // standard destructor
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	DWORD OpenScene(LPCTSTR fname);
	DWORD CloseScene(LPCTSTR fname);
	BOOL AddObjectInfo(CBaseObject *object, BOOL enabled);

// Dialog Data
	//{{AFX_DATA(CSceneDLDockWin)
	enum { IDD = IDD_SCENEDL_DLG };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSceneDLDockWin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// attributes
protected:
	// this was created so that messages will come through
	CPropertySheet m_PropSheet;
	CHierarchy m_Hierarchy;
	CDocument *m_pDocument;
	MAPOBJTOHIER m_ObjToHier;

	void FreeList(MAPOBJTOHIER *lhi);


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSceneDLDockWin)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileSaveScene();
	afx_msg void OnFileSaveAsScene();
	afx_msg void OnViewScene();
	afx_msg void OnUpdateViewScene(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnAddHierarchy(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRemoveHierarchy(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClearHierarchy(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENEDLDOCKWIN_H__51FB2C5B_289A_4AC0_818F_5625A0D66922__INCLUDED_)
