///============================================================================
/// \file		ResourceFolderView.h
/// \brief		Header file for ResourceFolderView
/// \date		11-17-2005
/// \author		Halbert Nakagawa
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

#pragma once

class CResourceFolderView : public CTreeView
{
protected: // create from serialization only
	CResourceFolderView();
	DECLARE_DYNCREATE(CResourceFolderView)

public:
	CResourceDoc* GetDocument();

	///brief forces the resource tree view to refresh the directory.
	///it start loading the directory returned by theApp->getResourceDir();
	void resetResourceFolder();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResourceFolderView)
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CResourceFolderView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	///brief: resets the resource path variable
	StdString getNewResoucePath();

	///brief: converts relative paths to full paths. Full paths are not modified.
	///param: p_path - the input path (can be full or relative as well) 
	///return: full path with the *.* file patter at the end
	CString convertRelativeToFullPath( LPCTSTR p_path );

// Generated message map functions
protected:
	int AddDirectories (HTREEITEM hItem, LPCTSTR pszPath);

	/// brief: Deletes node children. Called if a directory gets contracted
	/// if expanded again, the children are reloaded
	/// param: hItem - The node whose childer are to be deleted
	void DeleteAllChildren (HTREEITEM hItem);

	/// brief: Deletes the first child of a node. Used when you are sure,
	///	that it has only one child (the dummy node to make it expandable).
	/// when you expand it, you delete the dummy node first using this
	/// function.
	/// param: hItem - the node whose first child is to be deleted
	void DeleteFirstChild (HTREEITEM hItem);

	CString GetPathFromItem (HTREEITEM hItem);

	///brief: Checks the path, and if there are subdirs, adds a dummy
	///       node to hItem to make it expandable. 
	///param: pszPath - the input path (can be full or relative as well) 
	///       hItem - The node whose subnodes are set
	///return: True if the node is expandable (dummy node is added already) 
	BOOL SetButtonState (HTREEITEM hItem, LPCTSTR pszPath);

	/// not in use currently.
	int AddDrives ();
	BOOL AddDriveItem (LPCTSTR pszDrive);
	CImageList m_ilDrives;

	/// \brief	Setup the treectrl w/ the folders in the start directory
	void SetupTree();

	//{{AFX_MSG(CResourceFolderView)

	/// brief: checks the node (scans its path)
	///        and adds the subdirs if necessary 
	afx_msg void OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	/// to store the current resource directory
	StdString m_strResouceDirectory;
};

#ifndef _DEBUG  // debug version in DriveTreeView.cpp
inline CResourceDoc* CResourceFolderView::GetDocument()
   { return (CResourceDoc*)m_pDocument; }
#endif
