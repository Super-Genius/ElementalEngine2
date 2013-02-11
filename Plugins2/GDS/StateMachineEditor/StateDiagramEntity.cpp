///============================================================================
/// \file		StateDiagramEntity.cpp
/// \brief		Class implementation for CStateDiagramEntity
/// \date		03-16-2007
/// \author		Richard Nguyen
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

/////////////////// CStateDiagramEntity ////////////////////////////////////////
CStateDiagramEntity::CStateDiagramEntity( CDiagramEntity *parentEntity, IHashString *pStateMachineName )
	: CSimpleDiagramEntity( parentEntity )
	, m_hszStateMachineName( pStateMachineName )
	, m_StateType( Normal )
{
	SetType( _T("CQHState") );
}

CStateDiagramEntity::~CStateDiagramEntity()
{
}

void CStateDiagramEntity::Draw( CDC* dc, CRect rect )
{
	CGdiObject *pOldPen = NULL;
	CGdiObject *pOldBrush = NULL;
	switch( m_StateType )
	{
		case Normal:
			pOldPen = dc->SelectObject( GetPen() );
			pOldBrush = dc->SelectStockObject( WHITE_BRUSH );
			dc->Rectangle( rect );
			//dc->Ellipse( rect );
			break;
		case Start:
			pOldPen = dc->SelectObject( GetPen() );
			pOldBrush = dc->SelectStockObject( WHITE_BRUSH );
			//dc->Rectangle( rect );
			dc->Ellipse( rect );
			//{
			//	int size = min( rect.Width(), rect.Height() ) * 9 / 10 - 4;
			//	CRect rcEllipse( rect.CenterPoint(), rect.CenterPoint() );
			//	rcEllipse.InflateRect( size / 2, size / 2 );
			//	CPen pen( PS_SOLID, size / 10, LIGHT_LIGHT_GRAY );
			//	dc->SelectObject( &pen );
			//	dc->Ellipse( rcEllipse );
			//}
			break;
		case End:
			pOldPen = dc->SelectObject( GetPen() );
			pOldBrush = dc->SelectStockObject( WHITE_BRUSH );
			//dc->Rectangle( rect );
			dc->Ellipse( rect );
			dc->SelectStockObject( NULL_BRUSH );
			rect.DeflateRect( 2, 2 );
			//dc->Rectangle( rect );
			dc->Ellipse( rect );
			dc->SelectStockObject( WHITE_BRUSH );
			break;
	}

	rect.DeflateRect( 2, 2 );

	CGdiObject *oldFont = dc->SelectObject( GetFont() );
	DWORD err = GetLastError();
	COLORREF oldColor = dc->SetTextColor( GetColor() );
	int oldBkMode = dc->SetBkMode( TRANSPARENT );

	dc->DrawText( GetTitle(), rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	dc->SetBkMode( oldBkMode );
	dc->SetTextColor( oldColor );
	dc->SelectObject( oldFont );
	dc->SelectObject( pOldPen );
	dc->SelectObject( pOldBrush );
}

CDiagramEntity *CStateDiagramEntity::Clone()
{
	CStateDiagramEntity* obj = new CStateDiagramEntity( GetParentEntity(), &m_hszStateMachineName );
	obj->Copy( this );
	return obj;
}

CSize CStateDiagramEntity::GetMinimumSize() const
{
	CRect rc;
	HDC hDC = GetWindowDC( NULL );
	HGDIOBJ hOldFont = SelectObject( hDC, GetFont()->GetSafeHandle() );
	StdString sTitle = GetTitle();
	int height = DrawText( hDC, sTitle.c_str(), sTitle.size(), &rc, DT_CALCRECT | DT_SINGLELINE );
	SelectObject( hDC, hOldFont );
	ReleaseDC( NULL, hDC );
	return CSize( rc.Width() + 20, height + 20 );
}

void CStateDiagramEntity::ShowPopup( CPoint point, CWnd* parent )
{
	CMenu menu;
	menu.LoadMenu( IDR_STATE_CONTEXT_MENU );
	CMenu *popupMenu = menu.GetSubMenu(0);
	ASSERT( popupMenu != NULL );
	if( GetParentEntity() == NULL || IsAbstract() )
	{
		popupMenu->EnableMenuItem( ID_STATE_UPINHIERARCHY, MF_BYCOMMAND | MF_GRAYED );
	}
	CStateMachineDiagramEditor* stateMachineEditor = (CStateMachineDiagramEditor *)parent;
	if( stateMachineEditor->IsFlatMode() )
	{
		popupMenu->EnableMenuItem( ID_EDIT_CUT, MF_BYCOMMAND | MF_GRAYED );
		popupMenu->EnableMenuItem( ID_EDIT_COPY, MF_BYCOMMAND | MF_GRAYED );
	}
	if( !stateMachineEditor->CanPaste() )
	{
		popupMenu->EnableMenuItem( ID_EDIT_PASTE, MF_BYCOMMAND | MF_GRAYED );
	}

	popupMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, parent );
}

BOOL CStateDiagramEntity::DoMessage( UINT msg, CDiagramEntity* sender, CWnd* from )
{
	CStateMachineDiagramEditor *editor = static_cast<CStateMachineDiagramEditor *>( from );
	switch( msg )
	{
		case ID_STATE_UPINHIERARCHY:
			return UpInHiearchy( editor );
		case ID_STATE_ADDENTRYEVENT:
			return AddEntryEvent( editor );
		case ID_STATE_ADDUPDATEEVENT:
			return AddUpdateEvent( editor );
		case ID_STATE_ADDEXITEVENT:
			return AddExitEvent( editor );
		default:
			return CSimpleDiagramEntity::DoMessage( msg, sender, from );
	}
	return CSimpleDiagramEntity::DoMessage( msg, sender, from );
}

///	\brief	change entity parent to passed name
void CStateDiagramEntity::SetParentName( IHashString *newParentName, CStateMachineDiagramEditor* pEditor )
{
	CHashString hszName = GetName();
	const StdString label = GetTitle();

	CHANGEOBJECTPARENT cop;
	static CHashString hszType = CHashString( _T("CQHState") );
	cop.objectCompType = &hszType;
	cop.objectName = &hszName;
	cop.newParentName = newParentName;
	static const DWORD msgHash = CHashString( _T("ChangeObjectParent") ).GetUniqueID();
	m_ToolBox->SendMessage( msgHash, sizeof( cop ), &cop, &hszName, &hszType );

	pEditor->RefreshHierarchy( &hszName );
	pEditor->UpdateStateLabel( label, label );
}

void CStateDiagramEntity::SaveProperties()
{
	CHashString hszName = GetName();
	EDITINGPROPERTIESPARAMS epp;
	epp.name = &hszName;
	static const DWORD hashMsg_GetEditingProperties = CHashString( _T("GetEditingProperties") ).GetUniqueID();
	m_ToolBox->SendMessage( hashMsg_GetEditingProperties, sizeof( epp ), &epp );

	LPCTSTR *names = GetValueNames();
	TCHAR buf[16];
	CRect rect = GetRect();
	epp.params[ names[0] ] = _ltot( rect.left, buf, 10 );
	epp.params[ names[1] ] = _ltot( rect.top, buf, 10 );
	epp.params[ names[2] ] = _ltot( rect.Width(), buf, 10 );
	epp.params[ names[3] ] = _ltot( rect.Height(), buf, 10 );

	static const DWORD hashMsg_SetEditingProperties = CHashString( _T("SetEditingProperties") ).GetUniqueID();
	m_ToolBox->SendMessage( hashMsg_SetEditingProperties, sizeof( epp ), &epp );
}

///	\brief	read current object state and refresh visualization properties
void CStateDiagramEntity::LoadProperties()
{
	CHashString hszName = GetName();
	if( hszName.IsEmpty() )
	{
		SetTitle( _T("") );
		return;
	}

	StdString label;
	IArchive *ar = CreateMemoryArchive();
	CHashString hszNameParam = _T("name");
	GetAttributeObject()->GetAttribute( &hszNameParam, ar );
	ar->SetIsWriting( false );
	ar->SeekTo( 0 );
	ar->Read( label, hszNameParam.GetString() );
	ar->Close();
	SetTitle( label.c_str() );

	EDITINGPROPERTIESPARAMS epp;
	epp.name = &hszName;
	static const DWORD hashMsg_GetEditingProperties = CHashString( _T("GetEditingProperties") ).GetUniqueID();
	m_ToolBox->SendMessage( hashMsg_GetEditingProperties, sizeof( epp ), &epp );

	LPCTSTR *names = GetValueNames();
	int iX = _ttoi( epp.params[ names[0] ] );
	int iY = _ttoi( epp.params[ names[1] ] );
	int iWidth = _ttoi( epp.params[ names[2] ] );
	int iHeight = _ttoi( epp.params[ names[3] ] );

	SetRect( iX, iY, iX + iWidth, iY + iHeight );
}

LPCTSTR *CStateDiagramEntity::GetValueNames() const
{
	static LPCTSTR names[4] = { _T("x"), _T("y"),
								_T("width"), _T("height") };
	return names;
}

CPen *CStateDiagramEntity::GetStartStatePen()
{
	static CPen pen( PS_SOLID, 1, GRAY );
	return &pen;
}

CPen *CStateDiagramEntity::GetEndStatePen()
{
	static CPen pen( PS_SOLID, 1, GRAY );
	return &pen;
}

void CStateDiagramEntity::SetStateType( StateType type )
{
	m_StateType = type;
}

// process Up in Hierarchy context menu item command
BOOL CStateDiagramEntity::UpInHiearchy( CStateMachineDiagramEditor *editor )
{
	CDiagramEntity *pParent = GetParentEntity();
	ASSERT( pParent != NULL );
	CHashString hszParentName = pParent->GetName();
	CDiagramEntity *pNewParent = pParent->GetParentEntity();
	CHashString hszNewParentName = m_hszStateMachineName;
	if( pNewParent != NULL )
	{
		hszNewParentName = pNewParent->GetName();
	}

	CHashString hszName = GetName();
	CUndoCommandGuard undoCommand;
	CStateMachineChangeParentAction *pAction = NULL;
	pAction = new CStateMachineChangeParentAction( editor, &hszNewParentName  );
	pAction->AddState( &hszName );
	AddRedoAction( pAction );
	editor->SaveUndoSelection();

	SetParentName( &hszNewParentName, editor );

	pAction = new CStateMachineChangeParentAction( editor, &hszParentName );
	pAction->AddState( &hszName );
	AddUndoAction( pAction );
	editor->SaveRedoSelection();

	editor->Invalidate();

	static DWORD msgHash_HierarchyChanged = CHashString( _T("HierarchyChanged") ).GetUniqueID();
	GETSETACTIVESCENE gsas = { &m_hszStateMachineName };
	m_ToolBox->SendMessage( msgHash_HierarchyChanged, sizeof( gsas ), &gsas );

	return TRUE;
}

BOOL CStateDiagramEntity::AddEntryEvent( CStateMachineDiagramEditor *editor )
{
	return AddEvent( editor, _T("entryEvent"), _T("_Entry_Event") );
}

BOOL CStateDiagramEntity::AddUpdateEvent( CStateMachineDiagramEditor *editor )
{
	return AddEvent( editor, _T("updateEvent"), _T("_Update_Event") );
}

BOOL CStateDiagramEntity::AddExitEvent( CStateMachineDiagramEditor *editor )
{
	return AddEvent( editor, _T("exitEvent"), _T("_Exit_Event") );
}

// process "Add ... Event" context menu item commands
BOOL CStateDiagramEntity::AddEvent( CStateMachineDiagramEditor *editor, LPCTSTR parameterTag, LPCTSTR eventSuffix )
{
	CHashString hszName = GetName();
	StdString eventLabel = GetEEObjectProperty( &hszName, parameterTag );

	CHashString hszEventName = _T("");
	bool hasEventAlready = !eventLabel.empty();
	// check if event were created already
	if( hasEventAlready )
	{
		// verify event object name
		hszEventName = eventLabel;
		static CHashString hszTypeState = CHashString( _T("CQHState") );
		static const DWORD msgHash_GetEventNameFromLabel = CHashString( _T("GetEventNameFromLabel")).GetUniqueID();
		DWORD result = m_ToolBox->SendMessage( msgHash_GetEventNameFromLabel,
											   sizeof( IHashString ), &hszEventName,
											   &hszName, &hszTypeState );
		if( MSG_HANDLED != result )
		{
			hszEventName = _T("");
		}
	}
	else
	{
		// build new event name
		CString stateLabel = GetTitle();
		static TCHAR stateSuffix[] = _T("State");
		CString suffix = stateLabel.Right( _countof( stateSuffix ) - 1 );
		if( 0 == suffix.CompareNoCase( stateSuffix ) )
		{
			// removes existing 'state' suffix
			stateLabel.Delete( stateLabel.GetLength() - _countof( stateSuffix ) + 1,
							   _countof( stateSuffix ) );
			stateLabel.TrimRight( _T('_') );
		}
		eventLabel = stateLabel;
		eventLabel += eventSuffix;
	}

	// uncheck flat mode toolbar button. we will adjust editor's mode later
	CStateMachineEditor *pEditor = SINGLETONINSTANCE( CStateMachineEditor );
	CStateMachineToolBar *pToolBar = pEditor->GetToolBar();
	pToolBar->SetFlatMode( false );

	if( hszEventName.IsEmpty() )
	{
		CUndoCommandGuard undoCommmand;

		editor->SaveUndoSelection();
		hszEventName = editor->GetDocument()->CreateEvent( &hszName );

		IUndoRedoAction *pAction;
		pAction = new CStateMachineRemoveEntityAction( editor, &hszEventName );
		AddUndoAction( pAction );

		SetEEObjectProperty( &hszEventName, _T("name"), eventLabel );
		static CHashString hszTypeEvent = CHashString( _T("CQHStateMachineEvent") );
		UndoForCreateObject( &hszName, &hszEventName, &hszTypeEvent );
		editor->AddEvent( &hszName, &hszEventName );
		pAction = new CStateMachineAddEntityAction( editor, &hszName, &hszEventName );
		AddRedoAction( pAction );
		if( !hasEventAlready )
		{
			CObjectsStateUndo stateData( &hszName );
			SetEEObjectProperty( &hszName, parameterTag, eventLabel );
		}

		editor->SetFlatMode( false );
		editor->SelectObject( &hszEventName );
		editor->SaveRedoSelection();
	}
	else
	{
		editor->SetFlatMode( false );
		editor->SelectObject( &hszEventName );
	}
	return TRUE;
}

/////////////////// CFlatStateModeDiagramEntity ////////////////////////////////
CFlatStateModeDiagramEntity::CFlatStateModeDiagramEntity( CDiagramEntity *parentEntity,
														  IHashString *pStateMachineName )
	: CStateDiagramEntity( parentEntity, pStateMachineName )
{
}

void CFlatStateModeDiagramEntity::SelectionEdit( CWnd* parent )
{
}

LPCTSTR *CFlatStateModeDiagramEntity::GetValueNames() const
{
	static LPCTSTR names[4] = { _T("flatX"), _T("flatY"),
								_T("flatWidth"), _T("flatHeight") };
	return names;
}
