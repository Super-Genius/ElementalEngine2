#if !defined(AFX_HIERARCHY_H__DE30104A_F977_488A_B91A_DBF0E9B6E33D__INCLUDED_)
#define AFX_HIERARCHY_H__DE30104A_F977_488A_B91A_DBF0E9B6E33D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Hierarchy.h : header file
//

// icons in the bitmap
#define ICON_FOLDER_CLOSE	0
#define ICON_FOLDER_OPEN	1
#define ICON_GEOMETRY		2
#define ICON_LIGHT			3
#define ICON_CAMERA			4
#define ICON_MUSIC			5
#define ICON_PICTURE		6
#define ICON_SCRIPT			7
#define ICON_WORLD			8
#define ICON_SOUND			9
#define ICON_FRAME			10
#define ICON_ENVIRONMENT	11
#define ICON_TREE			12
#define ICON_TERRAIN		13

/////////////////////////////////////////////////////////////////////////////
// CHierarchy dialog

typedef list<struct HIERARCHYINFO *> LISTHINFO;

typedef struct HIERARCHYINFO 
{
	BOOL	enabled;		// whether it's enabled or not (checkmark)
	LPCSTR	label;			// the label for the item
	LISTHINFO children;		// children of this parent
	int image;				// image for the entity
	HTREEITEM hItem;		// handle to tree item for this hierarchy item.
} HIERARCHYINFO;

class CHierarchy : public CPropertyPage
{
// Construction
public:
	CHierarchy(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHierarchy)
	enum { IDD = IDD_HIERARCHY };
	CTreeCtrlEx	m_HierarchyControl;
	//}}AFX_DATA
	void ClearHierarchy();
	BOOL AddHierarchyItem(HIERARCHYINFO *parent, HIERARCHYINFO *child);
	BOOL RemoveHierarchyItem(HIERARCHYINFO *child);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHierarchy)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList m_Icons;

	// Generated message map functions
	//{{AFX_MSG(CHierarchy)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnDropMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HIERARCHY_H__DE30104A_F977_488A_B91A_DBF0E9B6E33D__INCLUDED_)
