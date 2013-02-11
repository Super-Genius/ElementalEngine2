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

#if !defined(AFX_DATABASEDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_DATABASEDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DatabaseDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDatabaseDoc document
class CDatabaseView;

class CDatabaseDoc : public CDocument
{
protected:
	
	CDatabaseDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDatabaseDoc)

// Operations
public:
	virtual ~CDatabaseDoc();

	/// get the actual database for this document
	/// \return info of DATABASE structures, etc.
	DATABASEINFO *GetDatabase() { return &m_DatabaseInfo; }

	/// Set the database data and size to new data
	/// \param nItems - new size of the database in items
	/// \param data - Global alloced new data
	void SetDatabaseData(DWORD nItems, DATABASEDATA *data);

	///	\brief	provide access to current undo stack name
	///	\return	pointer to IHashString with name of the stack
	IHashString *GetUndoStackName()
	{
		return &m_UndoStackName;
	}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDatabaseDoc)
protected:
	virtual BOOL OnNewDocument();
	virtual void OnCloseDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CDatabaseDoc)
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI *pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CDatabaseView *GetView();

private:
	DATABASEINFO m_DatabaseInfo;
	CHashString m_hszFileName;
	CHashString m_SchemaName;
	CHashString m_UndoStackName;
	IToolBox *m_ToolBox;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATABASEDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
