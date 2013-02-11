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

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NUM_FIXED_ROWS 1
#define NUM_FIXED_COLUMNS 1
/////////////////////////////////////////////////////////////////////////////
// CDatabaseView

IMPLEMENT_DYNCREATE(CDatabaseView, CView)

CDatabaseView::CDatabaseView()
{
	m_pGridCtrl = NULL;

	m_StringToDBTypeMap[_T("char")] = DBTYPE_CHAR;
	m_StringToDBTypeMap[_T("unsigned char")] = DBTYPE_UCHAR;
	m_StringToDBTypeMap[_T("short int")] = DBTYPE_SHORT;
	m_StringToDBTypeMap[_T("unsigned short int")] = DBTYPE_USHORT;
	m_StringToDBTypeMap[_T("long int")] = DBTYPE_LONG;
	m_StringToDBTypeMap[_T("unsigned long int")] = DBTYPE_ULONG;
	m_StringToDBTypeMap[_T("float")] = DBTYPE_FLOAT;
	m_StringToDBTypeMap[_T("double")] = DBTYPE_DOUBLE;
	m_StringToDBTypeMap[_T("bool")] = DBTYPE_BOOLEAN;
	m_StringToDBTypeMap[_T("string")] = DBTYPE_VOIDPTR;

	m_bActive = false;
}

CDatabaseView::~CDatabaseView()
{
	delete m_pGridCtrl;
}

BEGIN_MESSAGE_MAP(CDatabaseView, CView)
	//{{AFX_MSG_MAP(CDatabaseView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_DATABASE_ADDROW, OnDatabaseAddRow)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_ADDROW, OnUpdateDatabaseAddRow)
	ON_COMMAND(ID_DATABASE_REMOVEROW, OnDatabaseRemoveRow)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_REMOVEROW, OnUpdateDatabaseRemoveRow)
	ON_NOTIFY(GVN_BEGINLABELEDIT, IDC_GRID, OnGridBeginEdit)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDatabaseView drawing

void CDatabaseView::OnDraw(CDC* pDC)
{
}

void CDatabaseView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (m_bActive != (FALSE != bActivate))
	{
		m_bActive = (FALSE != bActivate);
		CGuiMDIFrame *mainWnd = DYNAMIC_DOWNCAST(CGuiMDIFrame, AfxGetMainWnd());

		if (bActivate)
		{
			mainWnd->LoadDocumentBars(_T("DBEditor"));
			SetActiveScene();
		}
		else
		{
			mainWnd->SaveDocumentBars(_T("DBEditor"));
		}

	}
}

void CDatabaseView::OnInitialUpdate()
{
	StdString schemaName;
	IHashString *hszSchemaName;
	VIEWOBJECTLIST::iterator voblIter;
	ViewFormatObject *vfo;
	DWORD row, column, rowCount;
	GV_ITEM Item;
	DATABASEINFO *dbDataInfo;

	CView::OnInitialUpdate();

	if (m_pGridCtrl == NULL)
	{
		// grab the database items
		dbDataInfo = GetDocument()->GetDatabase();

		// grab the XML schema name from the document
		hszSchemaName = dbDataInfo->m_SchemaName;
		if (hszSchemaName != NULL)
		{
			schemaName = hszSchemaName->GetString();
		}

		// now grab the schema from the manager, based on name
		m_ViewObjectList = SINGLETONINSTANCE(CDatabaseEditor)->GetViewObjectList(schemaName);

		// make sure there are some columns in the database
		if (m_ViewObjectList && m_ViewObjectList->size() > 0)
		{
			rowCount = dbDataInfo->m_nItems + NUM_FIXED_ROWS;
			// Create the Gridctrl object
			m_pGridCtrl = new CGridCtrl;
			if (!m_pGridCtrl) 
				return;

			// Create the Gridctrl window
			CRect rect;
			GetClientRect(rect);
			m_pGridCtrl->Create(rect, this, IDC_GRID);

			// fill it up with stuff
			m_pGridCtrl->SetEditable(TRUE);
			m_pGridCtrl->EnableDragAndDrop(TRUE);

			try {
				// +NUM_FIXED_ROWS for header
				m_pGridCtrl->SetRowCount(rowCount);
				m_pGridCtrl->SetColumnCount(m_ViewObjectList->size() + NUM_FIXED_COLUMNS);
				m_pGridCtrl->SetFixedRowCount(NUM_FIXED_ROWS);
				m_pGridCtrl->SetFixedColumnCount(NUM_FIXED_COLUMNS);
			}
			catch (CMemoryException* e)
			{
				e->ReportError();
				e->Delete();
				return;
			}

			voblIter = m_ViewObjectList->begin();

			// first fill in the header
			for (column=0; column<m_ViewObjectList->size()+NUM_FIXED_COLUMNS; column++)
			{
				// grab the format object at this iterator
				Item.mask = GVIF_TEXT|GVIF_FORMAT;
				Item.row = 0;
				Item.col = column;
				if (column > 0)
				{
					Item.nFormat = DT_LEFT|DT_WORDBREAK;
					vfo = *voblIter;
					Item.strText = vfo->GetName().c_str();
					++voblIter;
				}
				else
				{
					Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;
					Item.strText = _T("");
				}
				m_pGridCtrl->SetItem(&Item);
			}

			DATABASEDATA *dbData = dbDataInfo->m_DatabaseData;
			// fill rows/cols with text
			for (row = NUM_FIXED_ROWS; row < rowCount; row++)
			{
				if (dbData != NULL)
				{
					LoadData(row, dbData);
					dbData += m_ViewObjectList->size();
				}
				else
				{
					// Let the schema decide out to retrieve the data of the row
					LoadData( row, dbDataInfo->m_DatabaseName );
				}
			}

			m_pGridCtrl->AutoSize(GVS_BOTH);
		}
	}

	// register database undo stack for this view
	IHashString *pUndoStackName = GetDocument()->GetUndoStackName();
	RegisterViewUndoStack(this, pUndoStackName);
}

void CDatabaseView::LoadData(DWORD row, DATABASEDATA *dbItem)
{
	GV_ITEM Item;
	DWORD column;
	VIEWOBJECTLIST::iterator volIter;
	ViewFormatObject *curVFO;

	volIter = m_ViewObjectList->begin();
	for (column = 0; column < m_ViewObjectList->size()+NUM_FIXED_COLUMNS; column++)
	{ 
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = column;
		Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;
		if (column == 0)
		{
			TCHAR tempNum[256];
			_itot( row, tempNum, 10 );
			Item.strText = tempNum;
		}
		else
		{
			curVFO = *volIter;
			SetDataText( Item.strText, curVFO, dbItem, row, column );
			++volIter;
			if (dbItem != NULL)
			{
				++dbItem;
			}
		}

		m_pGridCtrl->SetItem(&Item);
	}
}

void CDatabaseView::LoadData( DWORD row, IHashString *hszDatabaseName )
{
	GV_ITEM Item;
	DWORD column;
	VIEWOBJECTLIST::iterator volIter;
	ViewFormatObject *curVFO;

	volIter = m_ViewObjectList->begin();
	for (column = 0; column < m_ViewObjectList->size()+NUM_FIXED_COLUMNS; column++)
	{ 
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = column;
		Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;
		if (column == 0)
		{
			TCHAR tempNum[256];
			_itot( row, tempNum, 10 );
			Item.strText = tempNum;
		}
		else
		{
			curVFO = *volIter;
			CHashString hszAttributeName( curVFO->GetName() );

			DATABASEATTRIBUTEPARAMS attributeParams;
			attributeParams.m_Index = row - NUM_FIXED_ROWS;
			attributeParams.m_AttributeName = &hszAttributeName;

			static const DWORD hashMsg_GetAttribute = CHashString(_T("GetAttribute")).GetUniqueID();
			EngineGetToolBox()->SendMessage( hashMsg_GetAttribute, sizeof( attributeParams ), &attributeParams, hszDatabaseName );

			SetDataText( Item.strText, curVFO, attributeParams.m_Data, row, column );
			++volIter;
		}

		m_pGridCtrl->SetItem(&Item);
	}
}

void CDatabaseView::SetDataText( CString &text, ViewFormatObject *curVFO, DATABASEDATA *dbItem, int row, int column )
{
	if (curVFO->GetViewType() == _T("edit"))
	{
		TCHAR tempNum[256];
		if (dbItem != NULL)
		{
			switch(dbItem->type)
			{
				case DBTYPE_CHAR:
				case DBTYPE_UCHAR:
					tempNum[0] = dbItem->m_CharValue;
					tempNum[1] = '\0';
					text.SetString( tempNum );
					break;
				case DBTYPE_SHORT:
				case DBTYPE_USHORT:
					_itot(dbItem->m_ShortValue, tempNum, 10);
					text.SetString( tempNum );
					break;
				case DBTYPE_LONG:
				case DBTYPE_ULONG:
					_ltot(dbItem->m_ShortValue, tempNum, 10);
					text.SetString( tempNum );
					break;
				case DBTYPE_FLOAT:
					_stprintf(tempNum, _T("%-.5f"), dbItem->m_FloatValue);
					text.SetString( tempNum );
					break;
				case DBTYPE_DOUBLE:
					_stprintf(tempNum, _T("%-.5f"), dbItem->m_DoubleValue);
					text.SetString( tempNum );
					break;
				case DBTYPE_BOOLEAN:
					if (dbItem->m_BoolValue)
					{
						text.SetString( _T("true") );
					}
					else
					{
						text.SetString( _T("false") );
					}
					break;
				case DBTYPE_VOIDPTR:
				case DBTYPE_STRING:
					text.SetString( dbItem->m_TCharPtr );
			}
		}
		else
		{
			text.SetString( _T("") );
		}
	}
	else if (curVFO->GetViewType() == _T("combo"))
	{
		IComboViewObject *cvo = dynamic_cast<IComboViewObject *>(curVFO);
		COMBOLIST *cl;
		if (!m_pGridCtrl->SetCellType(row,column, 
				RUNTIME_CLASS(CGridCellCombo)))
			return;
		// if database, we are filling in data
		if (dbItem != NULL)
		{
			switch(dbItem->type)
			{
				case DBTYPE_BOOLEAN:
					if (dbItem->m_BoolValue)
					{
						text.SetString( _T("true") );
					}
					else
					{
						text.SetString( _T("false") );
					}
					break;
				case DBTYPE_VOIDPTR:
				case DBTYPE_STRING:
					text.SetString( dbItem->m_TCharPtr );
					break;
				default:
					break;
			}
		}
		else	// else fill in defaults
		{
			text.SetString( cvo->GetDefSelect().c_str() );
		}
		cl = cvo->GetComboList();
		COMBOLIST::iterator clIter;
		CStringArray options;
		for (clIter = cl->begin(); clIter != cl->end(); ++clIter)
		{
			options.Add(clIter->c_str());
		}
		CGridCellCombo *pCell = (CGridCellCombo*) m_pGridCtrl->GetCell(row,column);
		pCell->SetOptions(options);
		pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
	}
}

void CDatabaseView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_pGridCtrl->GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);
		m_pGridCtrl->MoveWindow(rect);
	}
}

BOOL CDatabaseView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    if (m_pGridCtrl && IsWindow(m_pGridCtrl->m_hWnd))
        if (m_pGridCtrl->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
            return TRUE;

	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CDatabaseView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CDatabaseView::GetType(StdString &out, ViewFormatObject *vfo, 
							DATABASEDATA *startData)
{
	VIEWOBJECTLIST::iterator volIter;
	ViewFormatObject *curVFO;
	StdString type;

	type = vfo->GetBasicType();
	if (type.at(0) == '%')
	{
		// redirect to another control, get name of control, GET RID OF %
		type.erase(0, 1);
		// now search through names to get type map
		for (volIter = m_ViewObjectList->begin(); volIter != m_ViewObjectList->end();
			++volIter)
		{
			curVFO = *volIter;
			if (curVFO->GetName() == type)
			{
				// we found the name requested, make sure it's a string
				if (startData->type != DBTYPE_VOIDPTR)
				{
					MessageBox(_T("Type lookup, destinatation must be string"),
						_T("XML Format Error!"), MB_OK);
					out = _T("");
					return;
				}
				out = (TCHAR *)startData->m_VoidPtr;
				break;
			}
			++startData;
		}
	}
	else
	{
		out = type;
	}
}

void CDatabaseView::SaveData()
{
	DATABASEINFO *dbDataInfo;	// the database items in the document
	DATABASEDATA *dbData;		// pointer to currently working row

	// grab the database items
	dbDataInfo = GetDocument()->GetDatabase();
	
	dbData = dbDataInfo->m_DatabaseData;
	for (int row = NUM_FIXED_ROWS; row < m_pGridCtrl->GetRowCount(); ++row)
	{
		if( dbData != NULL )
		{
			SaveData(row, dbData, NULL, true);
			dbData += m_ViewObjectList->size();
		}
		else
		{
			SaveData(row, NULL, dbDataInfo->m_DatabaseName, true);
		}
	}
}

void CDatabaseView::SaveData(int row, DATABASEDATA *dbData, IHashString *hszDatabaseName,  bool bReplace)
{
	VIEWOBJECTLIST::iterator volIter = m_ViewObjectList->begin();
	for ( DWORD column = 0; column < m_ViewObjectList->size(); ++column )
	{
		DATABASEDATA *curData = NULL;
		if( dbData != NULL )
		{
			curData = dbData;
			++dbData;
		}
		else
		{
			CHashString hszAttributeName( (*volIter)->GetName() );

			DATABASEATTRIBUTEPARAMS attributeParams;
			attributeParams.m_Index = row - NUM_FIXED_ROWS;
			attributeParams.m_AttributeName = &hszAttributeName;

			static const DWORD hashMsg_GetAttribute = CHashString(_T("GetAttribute")).GetUniqueID();
			EngineGetToolBox()->SendMessage( hashMsg_GetAttribute, sizeof( attributeParams ), &attributeParams, hszDatabaseName );

			curData = attributeParams.m_Data;
		}
		SaveData(row, column, curData, *volIter, true);
		++volIter;
	}
}

void CDatabaseView::SaveData(int row, int column, DATABASEDATA *curData, ViewFormatObject *curVFO, bool bReplace)
{
	if( curData != NULL )
	{
		StdString type;
		GetType(type, curVFO, curData);
		// find map of StdString to enum
		STRINGTODBTYPEMAP::iterator stdtmIter = m_StringToDBTypeMap.find(type);
		if (stdtmIter == m_StringToDBTypeMap.end())
		{
			CString errMess;
			errMess.Format(_T("Unable to find type for %s!"),
					type.c_str());
			MessageBox(errMess, _T("XML Format Error!"), MB_OK);
			return;
		}
		DBDATATYPES dbType = stdtmIter->second;
		CString itemText;
		itemText = m_pGridCtrl->GetItemText(row, column + NUM_FIXED_COLUMNS);
		// if we are replacing existing data, free old data if allocated
		if (bReplace)
		{
			curData->Release();
		}

		switch (dbType)
		{
			case DBTYPE_CHAR:
			case DBTYPE_UCHAR:
				curData->m_CharValue = itemText.GetAt(0);
				break;
			case DBTYPE_SHORT:
			case DBTYPE_USHORT:
				curData->m_ShortValue = _tstoi(itemText);
				break;
			case DBTYPE_LONG:
			case DBTYPE_ULONG:
				curData->m_LongValue = _tstol(itemText);
				break;
			case DBTYPE_FLOAT:
				curData->m_FloatValue = (float)_tstof(itemText);
				break;
			case DBTYPE_DOUBLE:
				curData->m_DoubleValue = (double)_tstof(itemText);
				break;
			case DBTYPE_BOOLEAN:
				if (_tcscmp(itemText, _T("true")) == 0)
				{
					curData->m_BoolValue = true;
				}
				else
				{
					curData->m_BoolValue = false;
				}
				break;
			case DBTYPE_VOIDPTR:
				//if (!curData->AssignString(itemText, itemText.GetLength()))
				if (!curData->AssignString( itemText.GetBuffer( itemText.GetLength() ), itemText.GetLength()))
				{
					MessageBox(_T("Out of memory allocating string.\n"),
						_T("Memory Error!"), MB_OK);
				}
				break;
		}
		curData->type = dbType;
	}
}

void CDatabaseView::OnDatabaseAddRow()
{
	// grab the database items
	DATABASEINFO *dbDataInfo = GetDocument()->GetDatabase();
	int row = GetRowCount(dbDataInfo);
	AddRow(row);

	CUndoCommandGuard undoCommand;
	IUndoRedoAction *pAction = NULL;
	pAction = new CDatabaseRemoveRowAction(this, row);
	AddUndoAction(pAction);
	CDatabaseAddRowAction::Values values;
	pAction = new CDatabaseAddRowAction(this, row, values);
	AddRedoAction(pAction);
}

void CDatabaseView::OnUpdateDatabaseAddRow(CCmdUI *pCmdUI)
{
	if (m_ViewObjectList && (m_ViewObjectList->size() != 0))
	{
		pCmdUI->Enable();
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CDatabaseView::OnDatabaseRemoveRow()
{
	CCellRange cellRange = m_pGridCtrl->GetSelectedCellRange();
	const int gridRow = cellRange.GetMinRow();
	const int row = gridRow - NUM_FIXED_ROWS;

	CUndoCommandGuard undoCommand;
	IUndoRedoAction *pAction = NULL;
	CDatabaseAddRowAction::Values values;
	const int col = m_ViewObjectList->size();
	for (int i = 0; i < col; ++i)
	{
		CGridCellBase *pCell = NULL;
		pCell = m_pGridCtrl->GetCell(gridRow, i + NUM_FIXED_COLUMNS);
		values.push_back(pCell->GetText());
	}
	pAction = new CDatabaseAddRowAction(this, row, values);
	AddUndoAction(pAction);
	pAction = new CDatabaseRemoveRowAction(this, row);
	AddRedoAction(pAction);

	RemoveRow(row);
}

void CDatabaseView::OnUpdateDatabaseRemoveRow(CCmdUI *pCmdUI)
{
	// grab the selected range and see if it is a complete row
	CCellRange cellRange;
	
	if (m_pGridCtrl)
	{
		cellRange = m_pGridCtrl->GetSelectedCellRange();
		if ((cellRange.GetRowSpan() == 1) && 
			(m_pGridCtrl->GetColumnCount()-NUM_FIXED_COLUMNS == 
					cellRange.GetColSpan()))
		{
			pCmdUI->Enable();
			return;
		}
	}
	pCmdUI->Enable(false);
}

void CDatabaseView::OnGridBeginEdit(NMHDR *pNotifyStruct, LRESULT *result)
{
	ASSERT(m_pGridCtrl != NULL);
	ASSERT(pNotifyStruct != NULL);
	ASSERT(result != NULL);

	NM_GRIDVIEW *p = reinterpret_cast<NM_GRIDVIEW *>(pNotifyStruct);
	CGridCellBase *pCell = m_pGridCtrl->GetCell(p->iRow, p->iColumn);
	ASSERT(pCell != NULL);
	m_strOldText = pCell->GetText();
	*result = 0;
}

void CDatabaseView::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT *result)
{
	ASSERT(m_pGridCtrl != NULL);
	ASSERT(pNotifyStruct != NULL);
	ASSERT(result != NULL);

	NM_GRIDVIEW *p = reinterpret_cast<NM_GRIDVIEW *>(pNotifyStruct);
	CGridCellBase *pCell = m_pGridCtrl->GetCell(p->iRow, p->iColumn);
	ASSERT(pCell != NULL);
	CString strText = pCell->GetText();
	if (m_strOldText != strText)
	{
		int row = p->iRow - NUM_FIXED_ROWS;
		int col = p->iColumn - NUM_FIXED_COLUMNS;
		CUndoCommandGuard undoCommand;
		IUndoRedoAction *pAction = NULL;
		pAction = new CDatabaseCellChangeAction(this, row, col, m_strOldText);
		AddUndoAction(pAction);
		pAction = new CDatabaseCellChangeAction(this, row, col, strText);
		AddRedoAction(pAction);
	}
	*result = 0;
}

///	\brief	update text for the cell. This function does not add undo action
///			for this change
///	\param	row - row of the cell to modify
///	\param	col - column of the cell to modify
///	\param	text - new text for cell
void CDatabaseView::SetCellValue(int row, int col, LPCTSTR text)
{
	ASSERT(m_pGridCtrl != NULL);
	if (m_pGridCtrl != NULL)
	{
		ASSERT(row < m_pGridCtrl->GetRowCount());
		ASSERT(col < m_pGridCtrl->GetColumnCount());
		CGridCellBase *pCell = m_pGridCtrl->GetCell(row + NUM_FIXED_ROWS, col + NUM_FIXED_COLUMNS);
		ASSERT(pCell != NULL);
		if (pCell != NULL)
		{
			pCell->SetText(text);

			// grab the database items
			DATABASEINFO *dbDataInfo = GetDocument()->GetDatabase();

			DWORD rowOffset = GetDBDataCount(row);
			DATABASEDATA *dbRowInfo = dbDataInfo->m_DatabaseData;
			if( dbRowInfo != NULL )
			{
				dbRowInfo += rowOffset;
			}
			// TODO: save data from the single cell directly
			SaveData(row, dbRowInfo, dbDataInfo->m_DatabaseName);
			
			// TODO: call save after all changes only
			GetDocument()->SetDatabaseData(dbDataInfo->m_nItems, dbDataInfo->m_DatabaseData);
		}
	}
}

/// \brief	add new row to the grid. The new row will have passed index or 
//			be last row
///	\param	row - index where to add new row
void CDatabaseView::AddRow(int row)
{
	DATABASEINFO *dbDataInfo;	// the database items in the document
	DWORD newNumItems;			// new number of items in the database
	DATABASEDATA *newDBData = NULL;	// allocation pointer to new database
	DWORD newMemSize;

	// update grid
	ASSERT(row < m_pGridCtrl->GetRowCount());
	int gridRow = row + NUM_FIXED_ROWS;
	if (gridRow == m_pGridCtrl->GetRowCount())
	{
		m_pGridCtrl->InsertRow(NULL, -1);
	}
	else
	{
		m_pGridCtrl->InsertRow(NULL, gridRow);
	}
	LoadData(gridRow, newDBData);
	m_pGridCtrl->RedrawWindow();

	// grab the database items
	dbDataInfo = GetDocument()->GetDatabase();
	// new number of database items
	newNumItems = dbDataInfo->m_nItems + m_ViewObjectList->size();
	// reallocate database here
	newMemSize = sizeof(DATABASEDATA) * newNumItems;
	newDBData = (DATABASEDATA *)GlobalAlloc(GMEM_FIXED, newMemSize);
	if (newDBData == NULL)
	{
		CString errMess;
		errMess.Format(_T("Unable to allocate %d bytes for database %s\n"),
			newMemSize, dbDataInfo->m_FileName->GetString());
		MessageBox(errMess, _T("Memory allocation error!"), MB_OK);
		return;
	}

	DWORD rowOffset = GetDBDataCount(row);
	if (dbDataInfo->m_DatabaseData != NULL)
	{
		DWORD copySize = min(rowOffset, dbDataInfo->m_nItems) * sizeof(DATABASEDATA);
		if (copySize > 0)
		{
			memcpy(newDBData, dbDataInfo->m_DatabaseData, copySize);
		}
		copySize = (dbDataInfo->m_nItems - rowOffset) * sizeof(DATABASEDATA);
		if (copySize > 0)
		{
			memcpy(newDBData + GetDBDataCount(row + 1),
				   dbDataInfo->m_DatabaseData + rowOffset, copySize);
		}
		GlobalFree((HGLOBAL)dbDataInfo->m_DatabaseData);
	}

	DATABASEDATA *dbRowInfo = dbDataInfo->m_DatabaseData;
	if( dbRowInfo != NULL )
	{
		dbRowInfo += rowOffset;
	}
	SaveData( row, dbRowInfo, dbDataInfo->m_DatabaseName );

	// give the new data back to the document
	GetDocument()->SetDatabaseData(newNumItems, newDBData);
}

/// \brief	remove row with passed index from the grid
///	\param	row - index of the deleting row
void CDatabaseView::RemoveRow(int row)
{
	DATABASEINFO *dbDataInfo;	// the database items in the document
	CCellRange cellRange;

	ASSERT(row + NUM_FIXED_ROWS < m_pGridCtrl->GetRowCount());
	// grab the database items
	dbDataInfo = GetDocument()->GetDatabase();
	// then update db data block
	if (dbDataInfo->m_nItems > 0)
	{
		// determine data offsets
		DWORD rowOffset = GetDBDataCount(row);
		DATABASEDATA *dest = dbDataInfo->m_DatabaseData + rowOffset;
		DATABASEDATA *src = dest + m_ViewObjectList->size();
		// calculate memory block size
		DWORD sizeData = (dbDataInfo->m_nItems - GetDBDataCount(row + 1)) * sizeof(DATABASEDATA);
		// release existing memory blocks
		std::for_each(dest, src, mem_fun_ref(&DATABASEDATA::Release));
		// no need to reallocate memory, as adds will do it, just move the memory down
		if (sizeData > 0)
		{
			memmove(dest, src, sizeData);
		}
	}

	// give the new data back to the document
	GetDocument()->SetDatabaseData(dbDataInfo->m_nItems - m_ViewObjectList->size(),
								   dbDataInfo->m_DatabaseData);

	// update grid state
	m_pGridCtrl->DeleteRow(row + NUM_FIXED_ROWS);
	m_pGridCtrl->Invalidate();
}

///	\brief	focus cell in the grid
///	\param	row - row index of the cell (for DB)
///	\param	col - column index of the cell (for DB)
void CDatabaseView::FocusCell(int row, int col)
{
	const int gridRow = row + NUM_FIXED_ROWS;
	const int gridCol = col + NUM_FIXED_COLUMNS;
	EndGridInteraction();
	m_pGridCtrl->SetFocusCell(gridRow, gridCol);
	m_pGridCtrl->SetSelectedRange(gridRow, gridCol, gridRow, gridCol, TRUE);
	m_pGridCtrl->EnsureVisible(gridRow, gridCol);
	m_pGridCtrl->Invalidate();
}

///	\brief	focus row in the grid
///	\param	row - number of row to select(for DB)
void CDatabaseView::FocusRow(int row)
{
	EndGridInteraction();
	const int gridRow = row + NUM_FIXED_ROWS;
	int colCount = m_pGridCtrl->GetColumnCount();
	// focus should be set grid, because EnsureVisible implementation "plays"
	// with focus and may fail in some cases
	// but we need focus on the grid control anyway
	m_pGridCtrl->SetFocus();
	m_pGridCtrl->SetFocusCell(gridRow, 1);
	m_pGridCtrl->SetSelectedRange(gridRow, 1, gridRow, colCount - 1);
	m_pGridCtrl->EnsureVisible(gridRow, 1);
	m_pGridCtrl->RedrawWindow();
}

///	\description this function clears grid from the following UI highlights:
///				 - end editing for focused cell
///				 - clear selection
void CDatabaseView::EndGridInteraction()
{
	CCellID cellID = m_pGridCtrl->GetFocusCell();
	CGridCellBase *pCell = m_pGridCtrl->GetCell(cellID.row, cellID.col);
	if (pCell != NULL)
	{
		if (pCell->IsEditing())
		{
			pCell->EndEdit();
		}
		m_pGridCtrl->SetFocusCell(-1,-1);
	}
	m_pGridCtrl->SetSelectedRange(-1,-1,-1,-1);
}
