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

#ifndef _C_GUITOOLS_H_
#define _C_GUITOOLS_H_

// CGUITools dialog
class CGUIEditorDoc;

enum specType {PAGE = 0, GROUP, ITEM, CURSOR, TEMPLATE, ANIMATION};
typedef struct ALLHTREE
{
	HTREEITEM m_Item;
	ALLHTREE *m_Parent;
	UINT m_iParentIndex;
	specType m_SpecType;
	vector<HTREEITEM> m_Children;
	CHashString m_szName;	
}ALLHTREE;

typedef struct TEMPLATEITEM
{
	HTREEITEM m_Item;
	CHashString m_szName;
}TEMPLATEITEM;
class CGUITools : public CDialog
{
	DECLARE_DYNAMIC(CGUITools)

public:
	CGUITools(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGUITools();

// Dialog Data
	enum { IDD = IDD_GUITOOLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CGUIEditorDoc *m_Doc;
	DECLARE_MESSAGE_MAP()	
	bool m_bDeletePressed;	
public:
	afx_msg void OnBnClickedOk();
	void AddPage(CHashString pName);
	void AddGroup(CHashString page, CHashString gName);
	void AddElement(bool focusIsPage, CHashString bType, CHashString bName);
	void AddElemTemplate(CHashString name, CHashString type);
	void AddAnimation(CHashString name);
	bool CheckIsGroupInPage(CHashString groupName);
	bool CheckIsItemInGroupOrPage(bool focusIsPage, CHashString itemName);
	void AddCursor(CHashString cName);
	void LoadGXL(GUIPAGELIST gpl);
	void SetDocParent(CGUIEditorDoc *value) { m_Doc = value; }
	void RenameExternal(CHashString oldName, CHashString newName);
	void ToggleGroupVisible(CHashString gName, bool visible);
	CTreeCtrl *m_PageHierTree;
	CImageList *m_ImageList;

	// "New page..."
	HTREEITEM m_NewPageItem;

	// "Template objects"
	HTREEITEM m_TemplateObjects;
	
	// currently selected item
	HTREEITEM m_SelectedItem;	
	// page list
	HTREEITEM m_Cursor;
	vector<HTREEITEM> m_vPages;
	vector<HTREEITEM> m_vAnims;

	vector<ALLHTREE> m_vAllItems;
	vector<TEMPLATEITEM> m_vTemplateItems;

	CHashString m_szSelectedPage;
	CHashString m_szSelectedGroup;
	bool m_bSelectedIsPage;
	bool m_bCursorAdded;
	bool m_bCtrlPressed;	
	bool m_bShiftPressed;
	bool m_bHHeld;
	bool m_bCHeld;
	bool m_bVHeld;

	afx_msg BOOL OnInitDialog();
	afx_msg void OnTvnBegindragPagehiertree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTvnSelchangedPagehiertree(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnTvnKeydownPagehiertree(NMHDR *pNMHDR, LRESULT *pResult);
	void DeleteItemFromView(CHashString name, CHashString parent);
	afx_msg void OnTvnEndlabeleditPagehiertree(NMHDR *pNMHDR, LRESULT *pResult);

	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTvnBeginlabeleditPagehiertree(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#endif
