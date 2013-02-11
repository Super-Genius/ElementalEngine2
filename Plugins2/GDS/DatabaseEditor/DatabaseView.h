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

#if !defined(AFX_DATABASEVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_)
#define AFX_DATABASEVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DatabaseView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDatabaseView view


typedef map<StdString, DBDATATYPES> STRINGTODBTYPEMAP;

class CDatabaseView : public CView
{
protected:
	
	CDatabaseView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDatabaseView)

// Attributes
protected:
	CGridCtrl* m_pGridCtrl;
	VIEWOBJECTLIST *m_ViewObjectList;	// the header columns for the database
	STRINGTODBTYPEMAP m_StringToDBTypeMap;
	CString	m_strOldText;
	bool	m_bActive;

// Operations
public:
	///	\brief	save all grid cells to the DB
	void SaveData();

	///	\brief	update text for the cell. This function does not add undo action
	///			for this change
	///	\param	row - row of the cell to modify (for DB)
	///	\param	col - column of the cell to modify (for DB)
	///	\param	text - new text for cell
	void SetCellValue(int row, int col, LPCTSTR text);
	/// \brief	add new row to the grid. The new row will have passed index or 
	//			be last row
	///	\param	row - index where to add new row (for DB)
	void AddRow(int row);
	/// \brief	remove row with passed index from the grid
	///	\param	row - index of the deleting row (for DB)
	void RemoveRow(int row);
	///	\brief	focus cell in the grid
	///	\param	row - row index of the cell (for DB)
	///	\param	col - column index of the cell (for DB)
	void FocusCell(int row, int col);
	///	\brief	focus row in the grid
	///	\param	row - number of row to select(for DB)
	void FocusRow(int row);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDatabaseView)
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnInitialUpdate();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDatabaseView();

	///	\brief	load grid row with data from DB
	///	\param	row - index of the grid row
	///	\param	dbItem - pointer to the first data item in the DB row
	void LoadData(DWORD row, DATABASEDATA *dbItem);
	///	\brief	load grid row with data from using GetAttribute message
	///	\param	row - index of the grid row
	///	\param	hszDatabaseName - name of the database which to retrieve the values
	void LoadData( DWORD row, IHashString *hszDatabaseName );
	///	\brief	Correctly formats the cell text from the database value
	///	\param	text - text to populate
	///	\param	dbItem - pointer to the current data item
	void SetDataText( CString &text, ViewFormatObject *curVFO, DATABASEDATA *dbItem, int row, int column );
	///	\brief	save grid row to then DB data
	///	\param	row - index of the grid row
	///	\param	dbData - pointer to the first data item in the DB row
	///	\param	hszDatabaseName - name of the database
	///	\param	bReplace - if true then this function free memory blocks in the 
	///					   source DB data
	void SaveData(int row, DATABASEDATA *dbData, IHashString *hszDatabaseName, bool bReplace = false);
	///	\brief	save grid cell to the DB data
	///	\param	row - index of the grid row
	///	\param	column - index of the grid column
	///	\param	curData - pointer to the data item in the DB row
	///	\param	curVFO - Object format
	///	\param	bReplace - if true then this function free memory blocks in the 
	///					   source DB data
	void SaveData(int row, int column, DATABASEDATA *curData, ViewFormatObject *curVFO, bool bReplace = false);
	void GetType(StdString &out, ViewFormatObject *vfo, DATABASEDATA *startData);

	///	\brief	get document
	///	\return	pointer to the view's document.
	CDatabaseDoc *GetDocument() const
	{
		return DYNAMIC_DOWNCAST(CDatabaseDoc, CView::GetDocument());
	}

	DWORD GetRowCount(DATABASEINFO *dbDataInfo) const
	{
		return dbDataInfo->m_nItems / m_ViewObjectList->size();
	}

	///	\brief	calculate number of DATABASEDATA items for passed row count
	///	\param	row - count for calculate
	///	\return	number of DATABASEDATA items
	DWORD GetDBDataCount(int row) const
	{
		return row * m_ViewObjectList->size();
	}

	///	\description this function clears grid from the following UI highlights:
	///				 - end editing for focused cell
	///				 - clear selection
	void EndGridInteraction();

	// Generated message map functions
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDatabaseAddRow();
	afx_msg void OnUpdateDatabaseAddRow(CCmdUI *pCmdUI);
	afx_msg void OnDatabaseRemoveRow();
	afx_msg void OnUpdateDatabaseRemoveRow(CCmdUI *pCmdUI);
	afx_msg void OnGridBeginEdit(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void OnGridEndEdit(NMHDR * pNotifyStruct, LRESULT * result);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATABASEVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_)
