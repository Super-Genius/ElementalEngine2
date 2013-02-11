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

#if !defined(AFX_HIERARCHY_H__DE30104A_F977_488A_B91A_DBF0E9B6E33D__INCLUDED_)
#define AFX_HIERARCHY_H__DE30104A_F977_488A_B91A_DBF0E9B6E33D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Hierarchy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHierarchy dialog

typedef list<struct HIERARCHYINFO *> LISTHINFO;

typedef struct HIERARCHYINFO 
{
	HIERARCHYINFO()
		: objectName(NULL)
		, compType(NULL)
		, hItem(NULL)
	{
	}

	~HIERARCHYINFO()
	{
		delete objectName;
		delete compType;
	}
	StdString	name;		// the label for the item
	IHashString	*objectName;		// the name of the object
	IHashString *compType;	// user data for the item
	HTREEITEM hItem;		// handle to tree item for this hierarchy item.
} HIERARCHYINFO;

struct HIERARCHYTREESTATE;

class CHierarchy : public CPropertyPage
{
	typedef vector< pair<StdString, StdString> > MacrosList;
// Construction
public:
	CHierarchy(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	enum { IDD = IDD_HIERARCHY };

	void ClearHierarchy();

	void CollapseItem( HTREEITEM Item )
	{
		ExpandItem( Item, TVE_COLLAPSE );
	}
	void ExpandItem( HTREEITEM Item, UINT nCode = TVE_EXPAND );

	BOOL AddHierarchyItem(HTREEITEM parent, HIERARCHYINFO *child);
	BOOL RemoveHierarchyItem(HIERARCHYINFO *child);
	HIERARCHYINFO *GetItemData(HTREEITEM item);
	void SelectItem(HTREEITEM item);
	void DeselectItem(HTREEITEM item);
	HTREEITEM GetParentItem();
	bool IsItemChecked(HTREEITEM item);
	void RenameItem(HTREEITEM item, IHashString *pNewName);
	HIERARCHYINFO* GetNextItem(HIERARCHYINFO *hi, UINT nCode);
	
	///	\brief	return CHashString with text of the item
	///	\param	item - HTREEITEM from the tree
	///	\return	CHashString with text of the item
	CHashString GetItemName(HTREEITEM item);
	
	///	\brief	select passed object in the EE
	///	\param	hi - pointer to hierarchy item info
	void SelectObject(HIERARCHYINFO *hi);

	UINT GetItemState(HTREEITEM item, UINT stateMask);

	///	\brief	save current hierarchy tree state (expand/collapse and selection)
	///	\return pointer to structure with hierarchy tree state
	HIERARCHYTREESTATE *SaveState();
	///	\brief	restore hierarchy tree state (expand/collapse and selection)
	///	\param	pState - pointer to structure with previously saved state
	void RestoreState(const HIERARCHYTREESTATE *pState);
	///	\brief	release data for structure with previously saved state
	///	\param	pState - pointer to structure
	void DeleteState(HIERARCHYTREESTATE *pState);

	void CollapseScene();
	void ExpandScene();

	void SetItemText(HTREEITEM item, LPCTSTR text)
	{
		m_HierarchyControl.SetItemText(item, text);
	}

	void setUnderUpdateFlag( bool underUpdate ) { isUnderUpdate = underUpdate; };

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHierarchy)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
	// Generated message map functions
	//{{AFX_MSG(CHierarchy)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeeplocal();
	afx_msg void OnKeepglobal();
	afx_msg LRESULT OnDropMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelChangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickTree(NMHDR *pnhm, LRESULT *result);
	afx_msg void OnDblClickTree(NMHDR *pnhm, LRESULT* result);
	afx_msg void OnBeginLabelEdit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEndLabelEdit(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	bool ChangeObjectParentName(IHashString *name, IHashString *comp, IHashString *newParent);

	list<DWORD> GetLinkableObjects(IHashString *componentType) const;

	StdString GetFileSpawnerPath(LPCTSTR szResourcePath) const;
	IXMLArchive *OpenXMLArchive(LPCTSTR szFilePath) const;
	IXMLArchive *GetFileSpawner(LPCTSTR szResourcePath) const;
	bool IsLinkAllowed(IHashString *componentType, IXMLArchive *pSpawnerArchive) const;
	static void FillMacroses(LPCTSTR szResourcePath, MacrosList &macroses);
	static StdString ApplyMacroses(const StdString &value, const MacrosList &macroses);

	///	\brief	helper function for recursive tree state processing
	///	\param	item - currently processed tree item
	///	\param	pState - pointer to structure with hierarchy state
	void SaveState(HTREEITEM item, HIERARCHYTREESTATE *pState);
	
	///	\brief	helper function for recursive tree state restore
	///	\param	item - currently processed tree item
	///	\param	pState - pointer to structure with hierarchy state
	void RestoreState(HTREEITEM item, const HIERARCHYTREESTATE *pState);

private:
	CTreeCtrlEx	m_HierarchyControl;

	// cached names that get altered after a menu item is selected
	CHashString m_hsDragObjectName;
	CHashString m_hsDragObjectComp;
	CHashString m_hsDragObjectNewParent;

	StdString m_szOldName;

	CImageList m_Icons;
	IToolBox  *m_pToolBox;

	bool isUnderUpdate;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HIERARCHY_H__DE30104A_F977_488A_B91A_DBF0E9B6E33D__INCLUDED_)
