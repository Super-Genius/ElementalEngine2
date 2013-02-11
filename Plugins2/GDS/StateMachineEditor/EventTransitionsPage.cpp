///============================================================================
/// \file	EventTransitionsPage.cpp
/// \brief	Implementation for event transitions property page
/// \date	12/06/2007
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

IMPLEMENT_DYNAMIC(CEventTransitionsPage, CPropertyPage)

CHashString CEventTransitionsPage::m_hszCQHStateMachineEvent = _T("CQHStateMachineEvent");
CHashString CEventTransitionsPage::m_hszCQHStateMachineTransition = _T("CQHStateMachineTransition");

CEventTransitionsPage::CEventTransitionsPage( LPCTSTR szEventName, bool isAbstract )
	: CPropertyPage( CEventTransitionsPage::IDD, 0 )
	, m_hszEventName( szEventName )
	, m_bModified( FALSE )
	, m_isAbstract( isAbstract )
{
}

CEventTransitionsPage::~CEventTransitionsPage()
{
}

BEGIN_MESSAGE_MAP(CEventTransitionsPage, CPropertyPage)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
	ON_NOTIFY(GVN_SELCHANGED, IDC_GRID, OnGridSelChanged)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)
END_MESSAGE_MAP()

// CEventTransitionsPage message handlers
BOOL CEventTransitionsPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	SetButtonIcon( IDC_ADD );
	SetButtonIcon( IDC_REMOVE );

	InitalizeGrid();

	LoadTransitions();

	RefreshGrid();

	UpdateButtonStates();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CEventTransitionsPage::OnApply()
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
	IArchive *archive = CreateMemoryArchive();

	// prepare deleted items list
	vector<DWORD> deletedItems;
	// fill it with current transition name hashes
	transform( m_Transitions.begin(), m_Transitions.end(),
			   back_inserter( deletedItems ),
			   mem_fun_ref( &CTransitionInfo::GetNameHash ) );

	int rowCount = m_Grid.GetRowCount();
	InfoList transitions( rowCount - 1);
	for( int row = 1; row < rowCount; ++row )
	{
		CHashString hszName;
		LPCTSTR szName = GetItemData( row );

		archive->SetIsWriting( true );
		archive->SeekTo( 0 );
		archive->Write( 1.0f, _T("version") );
		CString targetState = m_Grid.GetItemText( row, COL_TARGET );
		CString name;
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
			// autogenerate name for transtion based on target state name
			name = targetState;
			if( name.Right( _countof( _T("State") ) - 1 ) == _T("State") )
			{
				name = name.Left( name.GetLength() - _countof( _T("State") ) + 1 );
				name += _T("Transition");
			}
			else
			{
				name += _T("_Transition");
			}
		}
		int weight = _ttoi(m_Grid.GetItemText( row, COL_WEIGHT ) );
		archive->Write( name, _T("name") );
		archive->Write( weight, _T("weight") );
		archive->Write( targetState, _T("targetState") );
		archive->SetIsWriting( false );
		if( szName == NULL )
		{
			hszName = m_hszCQHStateMachineTransition;
			// this is a new Transition handler
			if( CreateEEObject( &m_hszEventName, &m_hszCQHStateMachineTransition, hszName, archive ) )
			{
				SetItemData( row, hszName );
			}
			else
			{
				ShowError( IDS_ERR_CANT_CREATE_TRANSITION );
				return FALSE;
			}
		}
		else
		{
			// get current transition data
			SERIALIZEOBJECTPARAMS sop;
			sop.name = &hszName;
			sop.archive = archive;
			static DWORD msgHash_SerializeObject = CHashString( _T("SerializeObject") ).GetUniqueID();
			if( MSG_HANDLED != pToolBox->SendMessage( msgHash_SerializeObject, sizeof( sop ), &sop ) )
			{
				pToolBox->Log( LOGERROR, _T("%s(%d): Failed to serialize object\n"), __FILE__, __LINE__ );
				ShowError( IDS_ERR_CANT_UPDATE_TRANSITION );
				return FALSE;
			}
			INITOBJECTPARAMS iop;
			iop.name = &hszName;
			static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
			if( MSG_HANDLED != pToolBox->SendMessage( msgHash_InitObject, sizeof( iop ), &iop ) )
			{
				pToolBox->Log( LOGERROR, _T("%s(%d): Failed to initalize object\n"), __FILE__, __LINE__ );
				ShowError( IDS_ERR_CANT_UPDATE_TRANSITION );
				return FALSE;
			}
		}

		CTransitionInfo &transition = transitions[row - 1];
		transition.hszObjectName = hszName;
		transition.sName = name;
		transition.iWeight = weight;
		transition.sTargetState = targetState;

		// remove processed Transition for deleted items list
		deletedItems.erase( remove( deletedItems.begin(), deletedItems.end(), hszName.GetUniqueID() ),
							deletedItems.end() );
	}

	// delete Transitions from EE
	vector<DWORD>::const_iterator itDeleted = deletedItems.begin();
	for( ; itDeleted != deletedItems.end(); ++itDeleted )
	{
		CHashString hszName = pToolBox->GetHashString( *itDeleted );
		DeleteEEObject( &hszName );
	}

	archive->Close();
	m_Transitions.swap( transitions );
	return TRUE;
}

bool CEventTransitionsPage::Validate()
{
	int weights = 0;
	FillStates();
	int rowCount = m_Grid.GetRowCount();
	for( int row = 1; row < rowCount; ++row )
	{
		weights += _ttoi(m_Grid.GetItemText( row, COL_WEIGHT ) );
		CString targetState = m_Grid.GetItemText( row, COL_TARGET );
		if( targetState.IsEmpty() )
		{
			CString msg = FormatString( IDS_VALIDATE_TRANSITION_BAD_TARGET_STATE, (LPCTSTR)targetState );
			MessageBox( msg, FormatString(IDS_ERROR), MB_ICONERROR );
			m_Grid.SetSelectedRange( row, COL_TARGET, row, COL_TARGET );
			m_Grid.SetFocusCell( row, COL_TARGET );
			return false;
		}
		bool found = false;
		for( int i = m_States.GetSize() - 1; i >= 0; --i )
		{
			if( targetState == m_States.GetAt( i ) )
			{
				found = true;
				break;
			}
		}
		if( !found )
		{
			CString msg = FormatString( IDS_VALIDATE_TRANSITION_BAD_TARGET_STATE, (LPCTSTR)targetState );
			MessageBox( msg, FormatString(IDS_ERROR), MB_ICONERROR );
			m_Grid.SetFocusCell( row, COL_TARGET );
			return false;
		}
	}
	if( 0 == weights )
	{
		CString msg = FormatString( IDS_VALIDATE_TRANSITION_BAD_WEIGHT );
		MessageBox( msg, FormatString(IDS_ERROR), MB_ICONERROR );
	}
	return true;
}

///	\breif	create grid and initialize columns
void CEventTransitionsPage::InitalizeGrid()
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
	m_Grid.InsertColumn( FormatString( IDS_COL_WEIGHT ) );
	m_Grid.InsertColumn( FormatString( IDS_COL_TARGET_STATE ) );
	int width = rc.Width() / 3;
	m_Grid.SetColumnWidth( COL_WEIGHT, width );
	m_Grid.ExpandLastColumn();
	m_Grid.SetSingleRowSelection( TRUE );
	m_Grid.SetSingleColSelection( FALSE );
	m_Grid.SetEditable( !m_isAbstract );
}

///	\brief	load grid from m_Transitions array
void CEventTransitionsPage::RefreshGrid()
{
	m_Grid.SetRedraw( FALSE );
	m_Grid.SetRowCount( 1 );

	for( InfoList::const_iterator it = m_Transitions.begin(); it != m_Transitions.end(); ++it )
	{
		AddTransition( *it );
	}

	if( m_Grid.GetRowCount() > 1 )
	{
		const DWORD mask = LVIS_SELECTED | LVIS_FOCUSED;
		m_Grid.SetSelectedRange( 1, 0, 1, COL_COUNT - 1 );
	}

	m_Grid.SetRedraw( TRUE );
}

///	\brief	add new Transition to the grid
///	\param	details of new Transition
void CEventTransitionsPage::AddTransition( const CTransitionInfo &details )
{
	int row = m_Grid.InsertRow( details.sName.c_str() );
	LoadStates( row );
	TCHAR chWeight[16];
	m_Grid.SetItemText( row, COL_WEIGHT, _itot(details.iWeight, chWeight, 10) );
	m_Grid.SetItemText( row, COL_TARGET, details.sTargetState.c_str() );

	SetItemData( row, details.hszObjectName );

	// adjust combo cell styles
	m_Grid.SetCellType( row, COL_WEIGHT, RUNTIME_CLASS(CGridCellNumeric) );
}

///	\breif	insert new row to the grid and the specific position
///	\param	row - desired row index. if this value is -1 than new row will
///				  be appended to the grid
///	\return	index of inserted row
int CEventTransitionsPage::InsertRow( int row )
{
	if( row >= m_Grid.GetRowCount() )
	{
		// CGridCtrl cannot insert item to last position
		row = -1;
	}
	row = m_Grid.InsertRow( _T(""), row );
	// adjust combo cell styles
	m_Grid.SetCellType( row, COL_WEIGHT, RUNTIME_CLASS(CGridCellNumeric) );
	LoadStates( row );
	return row;
}

///	\breif	load current transition states for the event
void CEventTransitionsPage::LoadTransitions()
{
	m_Transitions.clear();
	IToolBox *pToolBox = EngineGetToolBox();
	// first of all create children object list
	GETSETSTATEMACHINENAMESPARAMS gssmn;
	static DWORD msgHash_GetEventTransitions = CHashString( _T("GetEventTransitions") ).GetUniqueID();
	DWORD result = pToolBox->SendMessage( msgHash_GetEventTransitions, sizeof( gssmn ),
										  &gssmn, &m_hszEventName, &m_hszCQHStateMachineEvent );

	// this archive will be used for extracting Transition details
	IArchive *archive = CreateMemoryArchive();

	list<IHashString *>::iterator it = gssmn.Names.begin();
	for( ;it != gssmn.Names.end(); ++it )
	{
		archive->SetIsWriting( true );

		// get current transition data
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
				// and fill Transition's values
				m_Transitions.resize( m_Transitions.size() + 1 );
				CTransitionInfo &transition = m_Transitions.back();
				transition.hszObjectName = *it;
				archive->Read( transition.sName , _T("name") );
				archive->Read( transition.iWeight, _T("weight") );
				archive->Read( transition.sTargetState, _T("targetState") );
			}
		}
	}

	archive->Close();
}

///	\brief	fill state list
void CEventTransitionsPage::FillStates()
{
	if( m_States.IsEmpty() )
	{
		GETSETSTATEMACHINENAMESPARAMS gssmn;
		static DWORD msgHash_GetStateMachineStateLabels = CHashString( _T("GetStateMachineStateLabels") ).GetUniqueID();
		DWORD result = EngineGetToolBox()->SendMessage( msgHash_GetStateMachineStateLabels, sizeof( gssmn ),
			&gssmn, &m_hszEventName, &m_hszCQHStateMachineEvent );
		if( MSG_HANDLED == result )
		{
			// extract string from pointers to IHashString in list to vector
			vector<CString> states;
			transform( gssmn.Names.begin(), gssmn.Names.end(), 
				back_inserter( states ), mem_fun( &IHashString::GetString ) );
			// and apply sorting
			sort( states.begin(), states.end() );

			vector<CString>::const_iterator it = states.begin();
			for( ; it != states.end(); ++it)
			{
				m_States.Add( *it );
			}
		}
	}
}

///	\brief	load states list for combo box filling for specific row
///	\param	row - index of row to fill
void CEventTransitionsPage::LoadStates( int row )
{
	FillStates();

	m_Grid.SetCellType( row, COL_TARGET, RUNTIME_CLASS(CGridCellCombo) );
	CGridCellCombo *pCell = (CGridCellCombo*) m_Grid.GetCell( row, COL_TARGET );
	pCell->SetOptions( m_States );
	pCell->SetStyle( CBS_DROPDOWNLIST );
}

///	\breif	check if changes in the grid exist against original data
///	\return	TRUE if something was changed in the grid
BOOL CEventTransitionsPage::HasChanges() const
{
	const int size = m_Transitions.size();
	if( m_Grid.GetRowCount() != size - 1)
	{
		return TRUE;
	}

	for( int i = 1; i <= size; ++i )
	{
		const CTransitionInfo &transition = m_Transitions[i - 1];
		if( _ttoi( m_Grid.GetItemText( i, COL_WEIGHT ) ) != transition.iWeight )
		{
			return TRUE;
		}
		if( m_Grid.GetItemText( i, COL_TARGET ) != transition.sTargetState.c_str() )
		{
			return TRUE;
		}
		if( GetItemData( i ) != transition.hszObjectName.GetString() )
		{
			return TRUE;
		}
	}
	return FALSE;
}

///	\brief	update dialog buttons state based on the grid selection
void CEventTransitionsPage::UpdateButtonStates()
{
	GetDlgItem( IDC_ADD )->EnableWindow( !m_isAbstract );
	GetDlgItem( IDC_REMOVE )->EnableWindow( CanDelete() );
}

///	\brief	check if delete is allowed
///	\return true if delete operation is allowed
bool CEventTransitionsPage::CanDelete() const
{
	return !m_isAbstract && ( m_Grid.GetSelectedCount() > 0 );
}

///	\breif	set button type to icon, load and set icon
void CEventTransitionsPage::SetButtonIcon( int id )
{
	HINSTANCE hRes = StateMachineEditorDLL.hResource;
	HICON hIcon = LoadIcon( hRes, MAKEINTRESOURCE( id ) );
	CWnd *pWnd = GetDlgItem( id );
	CButton *pBtn = reinterpret_cast< CButton * >( pWnd );
	pBtn->SetIcon( hIcon );
}

void CEventTransitionsPage::OnBnClickedAdd()
{
	int row = m_Grid.GetSelectedCellRange().GetMinRow();
	row = InsertRow( row );
	m_Grid.SetSelectedRange( row, 0, row, COL_COUNT - 1);
	m_Grid.RedrawWindow();
	SetModified();
	UpdateButtonStates();
}

void CEventTransitionsPage::OnBnClickedRemove()
{
	if( CanDelete() )
	{
		CCellRange selection = m_Grid.GetSelectedCellRange();
		vector<int> selectedRows;
		for( int row = selection.GetMaxRow(); row >= selection.GetMinRow(); --row )
		{
			if( m_Grid.IsCellSelected( row, COL_WEIGHT) &&
				m_Grid.IsCellSelected( row, COL_TARGET) )
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

void CEventTransitionsPage::OnGridSelChanged(NMHDR *pNotifyStruct, LRESULT *result)
{
	UpdateButtonStates();
}

void CEventTransitionsPage::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT *result)
{
	SetModified( HasChanges() );
}

///	\brief	write error message to EE log and show message box with message
///	\param	resourceId - id of error string resource
void CEventTransitionsPage::ShowError( int resourceId )
{
	CString sTitle;
	sTitle.LoadString( IDS_ERROR );
	CString sError;
	sError.LoadString( resourceId );
	MessageBox( sError, sTitle, MB_ICONERROR );
}
