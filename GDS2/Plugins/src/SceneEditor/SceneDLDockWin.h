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

#if !defined(AFX_SCENEDLDOCKWIN_H__51FB2C5B_289A_4AC0_818F_5625A0D66922__INCLUDED_)
#define AFX_SCENEDLDOCKWIN_H__51FB2C5B_289A_4AC0_818F_5625A0D66922__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SceneDLDockWin.h : header file
//

#include <map>

class CSceneDLDoc;

typedef map<DWORD, HTREEITEM> NAMETOTREEMAP;

/////////////////////////////////////////////////////////////////////////////
// CSceneDLDockWin dialog

using namespace std;
typedef struct TREEINFO
{
	IHashString *parentName;
	IHashString *name;
	IHashString *compType;
} TREEINFO;

typedef list<TREEINFO> TREEINFOLIST;

class CListAllVisitor : public IVisitor
{
public:
	CListAllVisitor(TREEINFOLIST *pList )
	{
		m_pList = pList; 
	}

	bool IsHierarchicalVisitor() { return false; }
	virtual bool Visit( IComponent * component, bool bVisitEnter)
	{
		IObject *pObject = dynamic_cast<IObject *>(component);
		if (pObject)
		{
			TREEINFO ti;
			ti.name = pObject->GetName();
			ti.parentName = pObject->GetParentName();
			ti.compType = pObject->GetComponentType();
			m_pList->push_back(ti);
		}
		return true;
	}
private:
	TREEINFOLIST *m_pList;
};

class CSceneDLDockWin : public CGuiControlBar
{
	typedef map<DWORD, HIERARCHYTREESTATE*> StateMap;

	DECLARE_DYNCREATE(CSceneDLDockWin)
// Construction
public:
	CSceneDLDockWin();   // standard constructor
	~CSceneDLDockWin();   // standard destructor

	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	/// Add an object from the hierarchy tree
	/// \param parent = the parent name of the object
	/// \param name = the name of the object to insert
	void AddHierarchy(IHashString *name, IHashString *type, IHashString *parent);
	/// Remove an object from the hierarchy tree
	/// \param name = name of object to remove
	void RemoveHierarchy(IHashString *name);
	/// Select an object from the hierarchy tree
	/// \param name = name of object to select
	void SelectFromHierarchy(IHashString *name);
	/// Update the tree control
	///	\param	force - force scene tree update
	///			if value is false then tree refreshes for changed scene only
	void UpdateHierarchy(bool force = true);
	///	\brief	update hierarchy started for passed object
	///	\param	name - name of root object in updated hierarchy
	///	\param	type - type of the root object in updated hierarchy
	void UpdateHierarchy(IHashString *name, IHashString *type);
	/// creates a new blank scene
	void CreateScene();
	/// saves the current scene
	void CheckSaveScene();
	/// open a new scene file
	/// \param fname = filename to load
	/// \return DWORD = ERROR value or 0 for ok
	DWORD OpenScene(LPCTSTR fname);
	/// update the hierarchy tree in the pane
	void FreeHierarchyInfo();
	/// close the current loaded scene
	void CloseScene();
	/// Set the modified scene flag
	/// \param isModified - set flag to true or false
	void OnWorldModified(BOOL isModified);

	/// close a modified scene, prompting user if they want to save
	/// \return BOOL, false if user presses cancel
	BOOL CloseModifiedScene();

	/// When an object is clicked on by name, select it
	void LeftClickObject(IHashString *objectName);

	/// is there a scene open?
	bool IsSceneOpen() { return m_pDocument != NULL; }

	/// get the current save path for the scene
	void GetSceneSavePath( IHashString *pPath );

	bool IsObjectChecked(IHashString *objectName);

	void RenameObject( IHashString *pOldName, IHashString *pNewName );

	void ChangeParentName(IHashString *object, IHashString *compType, IHashString *parent);

	///	\brief	Set file name for opened scene document
	///	\param	pFilename - string with new filename
	void SetWorldFileName(LPCTSTR pFilename);

	///	\brief	updated object label if it has been changed
	///	\param	name - name of initialized object
	void OnInitObject(IHashString *name);

	/// \brief returns if the hierarchy panel should be updated or not
	/// \retun true, if the panel is currently under update, and it should 
	/// not be updated. 
	bool isHierarchyUnderUpdate() { return isUnderUpdate; }

// Dialog Data
	//{{AFX_DATA(CSceneDLDockWin)
	enum { IDD = IDD_SCENEDL_DLG };
	//}}AFX_DATA

protected:

	void setHierarchyUnderUpdateFlag( bool underUpdate ) { isUnderUpdate = underUpdate; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSceneDLDockWin)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation

	// Generated message map functions
	//{{AFX_MSG(CSceneDLDockWin)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileSaveScene();
	afx_msg void OnFileSaveAsScene();
	afx_msg void OnViewScene();

	// enables/checks the menu item of the toolbar. (always enabled, checked if visible)
	afx_msg void OnUpdateViewScene(CCmdUI* pCmdUI);
	afx_msg void OnFileCloseScene();
	afx_msg void OnUpdateFileCloseScene(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSaveAsScene(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSaveScene(CCmdUI *pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnUpdateSceneCollapseScene(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSceneExpandScene(CCmdUI *pCmdUI);
	afx_msg void OnSceneCollapseScene();
	afx_msg void OnSceneExpandScene();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	void RecurseDeleteChildren(HIERARCHYINFO *hi);
	HTREEITEM AddNameToHierarchy(HTREEITEM parent, IHashString *name, IHashString *compType);

	HTREEITEM GetItem(IHashString *name) const
	{
		NAMETOTREEMAP::const_iterator it = m_NameToTree.find(name->GetUniqueID());
		if (it != m_NameToTree.end())
		{
			return it->second;
		}
		return NULL;
	}
	// attributes
private:
	// this was created so that messages will come through
	CPropertySheet m_PropSheet;
	CHierarchy m_Hierarchy;
	CSceneDLDoc *m_pDocument;
	NAMETOTREEMAP m_NameToTree;
	IToolBox *m_ToolBox;
	CHashString m_hszSceneRoot;
	CHashString m_hszSelectedObject;
	CHashString m_hszActiveScene;

	StateMap m_SceneStates;

	/// true, if the hierarchy is under update (and the window should not be refreshed)
	bool isUnderUpdate;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENEDLDOCKWIN_H__51FB2C5B_289A_4AC0_818F_5625A0D66922__INCLUDED_)
