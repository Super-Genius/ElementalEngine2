///============================================================================
/// \file	EventActionsPage.cpp
/// \brief	Implementation for event actions property page
/// \date	12/05/2007
/// \author	Andrey Ivanov
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

IMPLEMENT_DYNAMIC(CEventActionsPage, CPropertyPage)

CHashString CEventActionsPage::m_hszCQHStateMachineEvent = _T("CQHStateMachineEvent");
CHashString CEventActionsPage::m_hszCQHStateMachineActionHandler = _T("CQHStateMachineActionHandler");

CEventActionsPage::CEventActionsPage( LPCTSTR szEventName, bool isAbstract )
	: CPropertyPage( CEventActionsPage::IDD, 0 )
	, m_hszEventName( szEventName )
	, m_bModified( FALSE )
	, m_isAbstract( isAbstract )
{
}

CEventActionsPage::~CEventActionsPage()
{
}

BEGIN_MESSAGE_MAP(CEventActionsPage, CPropertyPage)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
	ON_BN_CLICKED(IDC_UP, OnBnClickedUp)
	ON_BN_CLICKED(IDC_DOWN, OnBnClickedDown)
	ON_NOTIFY(GVN_SELCHANGED, IDC_GRID, OnGridSelChanged)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)
END_MESSAGE_MAP()

// CEventActionsPage message handlers
BOOL CEventActionsPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	SetButtonIcon( IDC_ADD );
	SetButtonIcon( IDC_REMOVE );
	SetButtonIcon( IDC_UP );
	SetButtonIcon( IDC_DOWN );

	InitalizeGrid();

	LoadActions();

	RefreshGrid();

	UpdateButtonStates();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CEventActionsPage::OnApply()
{
	if( m_isAbstract )
	{
		return TRUE;
	}

	if( !Validate() )
	{
		return FALSE;
	}

	IToolBox *pToolBox = EngineGetToolBox();
	GETSETSTATEMACHINENAMESPARAMS gssmn;
	IArchive *archive = CreateMemoryArchive();

	// prepare deleted items list
	vector<DWORD> deletedItems;
	// fill it with current action handler name hashes
	transform( m_Actions.begin(), m_Actions.end(),
			   back_inserter( deletedItems ),
			   mem_fun_ref( &CActionInfo::GetNameHash ) );

	int rowCount = m_Grid.GetRowCount();
	InfoList actions( rowCount - 1);
	for( int row = 1; row < rowCount; ++row )
	{
		CString actionName = m_Grid.GetItemText( row, COL_ACTION_NAME );
		CString name;
		CHashString hszName;
		LPCTSTR szName = GetItemData( row );
		if( szName != NULL )
		{
			hszName = szName;
			IArchive *curData = GetEEObjectProperties( &hszName );
			float fVersion;
			curData->Read( fVersion, _T("version") );
			ASSERT( fVersion == 1.0f );
			StdString s;
			curData->Read( s, _T("name") );
			curData->Close();
			name = s.c_str();
		}

		if( name.IsEmpty() )
		{
			// autogenerate name for action
			name = actionName + _T("_ActionHandler");
		}

		archive->SetIsWriting( true );
		archive->SeekTo( 0 );
		archive->Write( 1.0f, _T("version") );
		archive->Write( name, _T("name") );
		archive->Write( actionName, _T("actionName") );
		archive->SetIsWriting( false );
		if( szName == NULL )
		{
			hszName = m_hszCQHStateMachineActionHandler;
			// this is a new action handler
			if( CreateEEObject( &m_hszEventName, &m_hszCQHStateMachineActionHandler, hszName, archive ) )
			{
				SetItemData( row, hszName );
			}
			else
			{
				ShowError( IDS_ERR_CANT_CREATE_ACTION );
				return FALSE;
			}
		}
		else
		{
			hszName = szName;
			// get current action handler data
			SERIALIZEOBJECTPARAMS sop;
			sop.name = &hszName;
			sop.archive = archive;
			static DWORD msgHash_SerializeObject = CHashString( _T("SerializeObject") ).GetUniqueID();
			if( MSG_HANDLED != pToolBox->SendMessage( msgHash_SerializeObject, sizeof( sop ), &sop ) )
			{
				pToolBox->Log( LOGERROR, _T("%s(%d): Failed to serialize object\n"), __FILE__, __LINE__ );
				ShowError( IDS_ERR_CANT_UPDATE_ACTION );
				return FALSE;
			}
			INITOBJECTPARAMS iop;
			iop.name = &hszName;
			static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
			if( MSG_HANDLED != pToolBox->SendMessage( msgHash_InitObject, sizeof( iop ), &iop ) )
			{
				pToolBox->Log( LOGERROR, _T("%s(%d): Failed to initalize object\n"), __FILE__, __LINE__ );
				ShowError( IDS_ERR_CANT_UPDATE_ACTION );
				return FALSE;
			}
		}

		CActionInfo &action = actions[row - 1];
		action.hszObjectName = hszName;
		action.sName = name;
		action.sActionName = actionName;
		// we cannot pass hszName here, because this pointer be invalid out of the scope
		gssmn.Names.push_back( &action.hszObjectName );

		// remove processed action for deleted items list
		deletedItems.erase( remove( deletedItems.begin(), deletedItems.end(), hszName.GetUniqueID() ),
							deletedItems.end() );
	}

	// delete actions from EE
	vector<DWORD>::const_iterator itDeleted = deletedItems.begin();
	for( ; itDeleted != deletedItems.end(); ++itDeleted )
	{
		CHashString hszName = pToolBox->GetHashString( *itDeleted );
		DeleteEEObject( &hszName );
	}
	// adjust actions' order
	static DWORD msgHash_SetEventActionHandlers = CHashString( _T("SetEventActionHandlers") ).GetUniqueID();
	DWORD result = pToolBox->SendMessage( msgHash_SetEventActionHandlers, sizeof( gssmn ),
										  &gssmn, &m_hszEventName, &m_hszCQHStateMachineEvent );

	archive->Close();
	m_Actions.swap( actions );
	return TRUE;
}

bool CEventActionsPage::Validate()
{
	int rowCount = m_Grid.GetRowCount();
	for( int row = 1; row < rowCount; ++row )
	{
		CString actionName = m_Grid.GetItemText( row, COL_ACTION_NAME );
		if( actionName.IsEmpty() )
		{
			ShowError( IDS_VALIDATE_ACTION_EMPTY_NAME );
			m_Grid.SetSelectedRange( row, COL_ACTION_NAME, row, COL_ACTION_NAME );
			m_Grid.SetFocusCell( row, COL_ACTION_NAME );
			return false;
		}
	}
	return true;
}

///	\breif	create grid and initialize columns
void CEventActionsPage::InitalizeGrid()
{
	CRect rcButton;
	GetDlgItem( IDC_ADD )->GetWindowRect( rcButton );
	ScreenToClient( rcButton );
	CRect rc;
	GetClientRect( rc );
	rc.top = rcButton.bottom;
	rc.DeflateRect( rcButton.left, rcButton.top );
	m_Grid.Create( rc, this, IDC_GRID );

	m_Grid.SetFixedRowCount( 1 );
	m_Grid.InsertColumn( FormatString( IDS_COL_ACTION_HANDLER ) );
	int width = rc.Width();
	m_Grid.ExpandColumnsToFit();
	m_Grid.SetSingleRowSelection( TRUE );
	m_Grid.SetSingleColSelection( FALSE );
	m_Grid.SetEditable( !m_isAbstract );
}

///	\brief	load grid from m_Actions array
void CEventActionsPage::RefreshGrid()
{
	m_Grid.SetRedraw( FALSE );
	m_Grid.SetRowCount( 1 );

	for( InfoList::const_iterator it = m_Actions.begin(); it != m_Actions.end(); ++it )
	{
		AddAction( *it );
	}

	if( m_Grid.GetRowCount() > 1 )
	{
		const DWORD mask = LVIS_SELECTED | LVIS_FOCUSED;
		m_Grid.SetSelectedRange( 1, 0, 1, COL_COUNT - 1 );
	}

	m_Grid.SetRedraw( TRUE );
}

///	\brief	add new action to the grid
///	\param	details of new action
void CEventActionsPage::AddAction( const CActionInfo &details )
{
	int row = m_Grid.InsertRow( details.sName.c_str() );
	m_Grid.SetItemText( row, COL_ACTION_NAME, details.sActionName.c_str() );
	SetItemData( row, details.hszObjectName );
}

///	\breif	insert new row to the grid and the specific position
///	\param	row - desired row index. if this value is -1 than new row will
///				  be appended to the grid
///	\return	index of inserted row
int CEventActionsPage::InsertRow( int row )
{
	if( row >= m_Grid.GetRowCount() )
	{
		// CGridCtrl cannot insert item to last position
		row = -1;
	}
	return m_Grid.InsertRow( _T(""), row );
}

///	\breif	move row from one position to another
///	\param	sourceRow - current row index
///	\param	targetRow - new row index
void CEventActionsPage::MoveRow( int sourceRow, int targetRow )
{
	targetRow = InsertRow( targetRow );
	if( targetRow <= sourceRow )
	{
		++sourceRow;
	}
	for( int col = m_Grid.GetColumnCount() - 1; col >= 0; --col )
	{
		CString text = m_Grid.GetItemText( sourceRow, col );
		m_Grid.SetItemText( targetRow, col, text );
	}
	LPARAM data = m_Grid.GetItemData( sourceRow, COL_DATA );
	m_Grid.SetItemData( sourceRow, COL_DATA, data );
	m_Grid.DeleteRow( sourceRow );
	//m_Grid.RedrawWindow();
}

///	\breif	load current action handler states for the event
void CEventActionsPage::LoadActions()
{
	m_Actions.clear();
	IToolBox *pToolBox = EngineGetToolBox();
	// first of all create children object list
	GETSETSTATEMACHINENAMESPARAMS gssmn;
	static DWORD msgHash_GetEventActionHandlers = CHashString( _T("GetEventActionHandlers") ).GetUniqueID();
	DWORD result = pToolBox->SendMessage( msgHash_GetEventActionHandlers, sizeof( gssmn ),
										  &gssmn, &m_hszEventName, &m_hszCQHStateMachineEvent );

	// this archive will be used for extracting action details
	IArchive *archive = CreateMemoryArchive();

	list<IHashString *>::iterator it = gssmn.Names.begin();
	for( ;it != gssmn.Names.end(); ++it )
	{
		archive->SetIsWriting( true );

		// get current action handler data
		SERIALIZEOBJECTPARAMS sop;
		sop.name = *it;
		sop.archive = archive;
		static DWORD msgHash_SerializeObject = CHashString( _T("SerializeObject") ).GetUniqueID();
		result = pToolBox->SendMessage( msgHash_SerializeObject, sizeof( sop ), &sop );

		if (MSG_HANDLED == result)
		{
			// read successfully serialized data
			archive->SetIsWriting( false );
			float fVersion = 1.0f;
			archive->Read( fVersion, _T("version") );
			// check supported version
			if ( fVersion > 1.0f )
			{
				ASSERT( !"Unsupported version of archive" );
				pToolBox->Log( LOGWARNING, _T("%s(%d): Unsupported version of archive"), __FILE__, __LINE__ );
			}
			else
			{
				// and fill action's strings 
				m_Actions.resize( m_Actions.size() + 1 );
				CActionInfo &action = m_Actions.back();
				action.hszObjectName = *it;
				archive->Read( action.sName , _T("name") );
				archive->Read( action.sActionName, _T("actionName") );
			}
		}
	}

	archive->Close();
}

///	\breif	check if changes in the grid exist against original data
///	\return	TRUE if something was changed in the grid
BOOL CEventActionsPage::HasChanges() const
{
	const int size = m_Actions.size();
	if( m_Grid.GetRowCount() != size - 1)
	{
		return TRUE;
	}

	for( int i = 1; i <= size; ++i )
	{
		const CActionInfo &details = m_Actions[i - 1];
		if( m_Grid.GetItemText( i, COL_ACTION_NAME ) != details.sActionName.c_str() )
		{
			return TRUE;
		}
		if( GetItemData( i ) != details.hszObjectName.GetString() )
		{
			return TRUE;
		}
	}
	return FALSE;
}

///	\brief	update dialog buttons state based on the grid selection
void CEventActionsPage::UpdateButtonStates()
{
	GetDlgItem( IDC_ADD )->EnableWindow( !m_isAbstract );
	GetDlgItem( IDC_REMOVE )->EnableWindow( CanDelete() );
	GetDlgItem( IDC_UP )->EnableWindow( CanMoveUp() );
	GetDlgItem( IDC_DOWN )->EnableWindow( CanMoveDown() );
}

///	\brief	check if delete is allowed
///	\return true if delete operation is allowed
bool CEventActionsPage::CanDelete() const
{
	return !m_isAbstract && ( m_Grid.GetSelectedCount() > 0 );
}

///	\brief	check if move up operation is allowed
///	\return true if move up operation is allowed
bool CEventActionsPage::CanMoveUp() const
{
	if( !m_isAbstract && ( m_Grid.GetSelectedCount() > 0 ) )
	{
		return m_Grid.GetSelectedCellRange().GetMinRow() > 1;
	}
	return false;
}

///	\brief	check if move down operation is allowed
///	\return true if move down operation is allowed
bool CEventActionsPage::CanMoveDown() const
{
	if( !m_isAbstract && ( m_Grid.GetSelectedCount() > 0 ) )
	{
		int row = m_Grid.GetSelectedCellRange().GetMaxRow();
		return row + 1 < m_Grid.GetRowCount();
	}
	return false;
}

///	\breif	set button type to icon, load and set icon
void CEventActionsPage::SetButtonIcon( int id )
{
	HINSTANCE hRes = StateMachineEditorDLL.hResource;
	HICON hIcon = LoadIcon( hRes, MAKEINTRESOURCE( id ) );
	CWnd *pWnd = GetDlgItem( id );
	CButton *pBtn = reinterpret_cast< CButton * >( pWnd );
	pBtn->SetIcon( hIcon );
}

void CEventActionsPage::OnBnClickedAdd()
{
	int row = m_Grid.GetSelectedCellRange().GetMinRow();
	row = InsertRow( row );
	m_Grid.SetSelectedRange( row, 0, row, COL_COUNT - 1);
	m_Grid.RedrawWindow();
	SetModified();
	UpdateButtonStates();
}

void CEventActionsPage::OnBnClickedRemove()
{
	if( CanDelete() )
	{
		CCellRange selection = m_Grid.GetSelectedCellRange();
		vector<int> selectedRows;
		for( int row = selection.GetMaxRow(); row >= selection.GetMinRow(); --row )
		{
			if( m_Grid.IsCellSelected( row, COL_ACTION_NAME) )
			{
				selectedRows.push_back( row );
			}
		}

		// selected items were collected in reverse order 
		// so we may delete them one by one
		vector<int>::const_iterator it = selectedRows.begin();
		for( ; it != selectedRows.end(); ++it )
		{
			m_Grid.DeleteRow( *it );
		}

		m_Grid.RedrawWindow();
		SetModified( HasChanges() );
		UpdateButtonStates();
	}
}

void CEventActionsPage::OnBnClickedUp()
{
	if( CanMoveUp() )
	{
		CCellRange selection = m_Grid.GetSelectedCellRange();
		const int minRow = selection.GetMinRow();
		const int maxRow = selection.GetMaxRow();

		vector<int> unselectedRows;
		for( int row = minRow; row <= maxRow; ++row )
		{
			if( !m_Grid.IsCellSelected( row, COL_ACTION_NAME) )
			{
				unselectedRows.push_back( row );
			}
		}

		bool selectedRange = true;
		int bottomSelectedRow = maxRow;
		for( int row = maxRow; row >= minRow; --row )
		{
			if( binary_search( unselectedRows.begin(), unselectedRows.end(), row ) )
			{
				if( selectedRange )
				{
					MoveRow( row, bottomSelectedRow + 1 );
					selectedRange = false;
				}
			}
			else if( !selectedRange )
			{
				bottomSelectedRow = row;
				selectedRange = true;
			}
		}

		MoveRow( minRow - 1, bottomSelectedRow + 1 );
		unselectedRows.push_back( bottomSelectedRow - 1 );

		m_Grid.SetSelectedRange( minRow - 1, 0,
								 maxRow - 1, COL_COUNT - 1 );

		vector<int>::const_iterator it = unselectedRows.begin();
		for( ; it != unselectedRows.end(); ++it )
		{
			int row = *it + 1;
			m_Grid.SetSelectedRange( row, 0, row, COL_COUNT - 1, FALSE, FALSE );
		}

		m_Grid.RedrawWindow();
		UpdateButtonStates();
		SetModified( HasChanges() );
	}
}

void CEventActionsPage::OnBnClickedDown()
{
	if( CanMoveDown() )
	{
		CCellRange selection = m_Grid.GetSelectedCellRange();
		const int minRow = selection.GetMinRow();
		const int maxRow = selection.GetMaxRow();

		vector<int> unselectedRows;
		for( int row = minRow; row <= maxRow; ++row )
		{
			if( !m_Grid.IsCellSelected( row, COL_ACTION_NAME) )
			{
				unselectedRows.push_back( row );
			}
		}

		bool selectedRange = true;
		int topSelectedRow = minRow;
		for( int row = minRow; row <= maxRow; ++row )
		{
			if( binary_search( unselectedRows.begin(), unselectedRows.end(), row ) )
			{
				if( selectedRange )
				{
					MoveRow( row, topSelectedRow );
					selectedRange = false;
				}
			}
			else if( !selectedRange )
			{
				topSelectedRow = row;
				selectedRange = true;
			}
		}

		MoveRow( maxRow + 1, topSelectedRow );
		unselectedRows.push_back( topSelectedRow + 1 );

		m_Grid.SetSelectedRange( minRow + 1, 0,
								 maxRow + 1, COL_COUNT - 1 );

		vector<int>::const_iterator it = unselectedRows.begin();
		for( ; it != unselectedRows.end(); ++it )
		{
			int row = *it - 1;
			m_Grid.SetSelectedRange( row, 0, row, COL_COUNT - 1, FALSE, FALSE );
		}

		m_Grid.RedrawWindow();
		UpdateButtonStates();
		SetModified( HasChanges() );
	}
}

void CEventActionsPage::OnGridSelChanged(NMHDR *pNotifyStruct, LRESULT *result)
{
	UpdateButtonStates();
}

void CEventActionsPage::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT *result)
{
	SetModified( HasChanges() );
}

///	\brief	write error message to EE log and show message box with message
///	\param	resourceId - id of error string resource
void CEventActionsPage::ShowError( int resourceId )
{
	MessageBox( FormatString( resourceId ), FormatString( IDS_ERROR ), MB_ICONERROR );
}
