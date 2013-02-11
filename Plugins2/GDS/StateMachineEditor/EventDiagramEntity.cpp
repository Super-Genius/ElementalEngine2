///============================================================================
/// \file		EventDiagramEntity.cpp
/// \brief		Class implementation for CEventDiagramEntity
/// \date		06-20-2007
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

CEventDiagramEntity::CEventDiagramEntity( CDiagramEntity *parentEntity )
	: CSimpleDiagramEntity( parentEntity )
{
	SetType( _T("CQHStateMachineEvent") );
}

CEventDiagramEntity::~CEventDiagramEntity()
{
}

CDiagramEntity *CEventDiagramEntity::Clone()
{
	CEventDiagramEntity* obj = new CEventDiagramEntity( GetParentEntity() );
	obj->Copy( this );
	return obj;
}

void CEventDiagramEntity::Draw( CDC* dc, CRect rect )
{
	rect.NormalizeRect();
	CGdiObject *oldPen = dc->SelectObject( GetPen() );
	dc->SelectStockObject( WHITE_BRUSH );

	//dc->Rectangle( rect );
	dc->RoundRect( rect, CPoint( 8, 8 ) );

	rect.DeflateRect( 2, 2 );
	CRect rcText = rect;
	CGdiObject *oldFont = dc->SelectObject( GetFont() );
	COLORREF oldColor = dc->SetTextColor( GetColor() );
	int height = dc->DrawText( GetTitle(), rcText, DT_CENTER | DT_SINGLELINE );
	rect.top += height + 1;

	if( rect.top < rect.bottom )
	{
		dc->MoveTo( rect.left, rect.top );
		dc->LineTo( rect.right, rect.top );

		rect.top += 2;
		vector<StdString>::const_iterator it = m_Actions.begin();
		for( ; it != m_Actions.end() && rect.top < rect.bottom; ++it )
		{
			rcText = rect;
#ifdef _UNICODE
			CString s = "\x2022 " ;
#else
			CString s = "\x95 " ;
#endif
			s += it->c_str();
			height = dc->DrawText(s, rcText, DT_LEFT);
			rect.top += height;
		}
	}
	dc->SetTextColor(oldColor);
	dc->SelectObject( oldFont );
	dc->SelectObject( oldPen );
}

CSize CEventDiagramEntity::GetMinimumSize() const
{
	CRect rc;
	HDC hDC = GetWindowDC( NULL );
	StdString sLong = GetTitle();
	vector<StdString>::const_iterator it = m_Actions.begin();
	for( ; it != m_Actions.end(); ++it )
	{
		if( it->size() > sLong.size() )
		{
			sLong = *it;
		}
	}
	int height = DrawText( hDC, sLong.c_str(), sLong.size(), &rc, DT_CALCRECT | DT_SINGLELINE );
	height *= ( 1 + m_Actions.size() );
	height += m_Actions.empty() ? 4 : 6;
	ReleaseDC( NULL, hDC );
	return CSize( rc.Width() + 20, height );
}

void CEventDiagramEntity::ShowPopup( CPoint point, CWnd* parent )
{
	CMenu menu;
	menu.LoadMenu( IDR_EVENT_CONTEXT_MENU );
	CMenu *popupMenu = menu.GetSubMenu(0);

	UINT_PTR cmd = ID_EVENT_OVERRIDE;
	CMenu transitionMenu;
	transitionMenu.CreatePopupMenu();
	vector<StdString>::const_iterator it;
	if( !IsAbstract() )
	{
		// populate popup menu with states
		vector<StdString> states;
		GetStateLabels( states );
		for( it = states.begin(); it != states.end(); ++it )
		{
			transitionMenu.AppendMenu( MF_STRING, ++cmd, (*it).c_str() );
		}
	}
	CString menuItemString;
	popupMenu->GetMenuString( 0, menuItemString, MF_BYPOSITION );
	popupMenu->ModifyMenu( 0, MF_BYPOSITION | MF_POPUP, (UINT_PTR)transitionMenu.Detach(), menuItemString );
	
	CMenu overrideMenu;
	overrideMenu.CreatePopupMenu();
	if( !IsAbstract() )
	{
		// populate popup menu with available events
		vector<StdString> events;
		GetOverrideEventLabels( events );
		StdString title = GetTitle();
		for( it = events.begin(); it != events.end(); ++it )
		{
			UINT flags = MF_STRING;
			if( title == (*it) )
			{
				flags |= MF_CHECKED;
			}
			overrideMenu.AppendMenu( flags, ++cmd, (*it).c_str() );
		}
	}
	popupMenu->GetMenuString( 1, menuItemString, MF_BYPOSITION );
	popupMenu->ModifyMenu( 1, MF_BYPOSITION | MF_POPUP, (UINT_PTR)overrideMenu.Detach(), menuItemString );

	popupMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, parent );
}

BOOL CEventDiagramEntity::DoMessage( UINT msg, CDiagramEntity* sender, CWnd* from )
{
	if( msg <= ID_EVENT_OVERRIDE )
	{
		return CSimpleDiagramEntity::DoMessage( msg, sender, from );
	}

	// adjust command id to array offset
	msg -= ID_EVENT_OVERRIDE + 1;
	CHashString hszEventName = GetName();
	vector<StdString> states;
	GetStateLabels( states );
	if( states.size() > msg )
	{
		// this is a "Add Transaction" command
		static CHashString hszType = _T("CQHStateMachineTransition");
		StdString targetState = states[msg];
		CStateMachineDiagramEditor *pEditor = static_cast<CStateMachineDiagramEditor *>( from );
		CHashString hszTransition;
		hszTransition = pEditor->GetDocument()->CreateTransition( &hszEventName, targetState.c_str() );
		pEditor->AddTransition( &hszEventName, &hszTransition );

		// saving information for undo and redo operations
		CUndoCommandGuard undoCommand;
		pEditor->SaveUndoSelection();
		UndoForCreateObject( &hszEventName, &hszTransition, &hszType );
		IUndoRedoAction *pAction;
		pAction = new CStateMachineRemoveEntityAction( pEditor, &hszTransition );
		AddUndoAction( pAction );
		pAction = new CStateMachineAddEntityAction( pEditor, &hszEventName, &hszTransition );
		AddRedoAction( pAction );
		pEditor->SaveRedoSelection();

		return TRUE;
	}

	msg -= states.size();
	vector<StdString> events;
	GetOverrideEventLabels( events );
	if( events.size() > msg )
	{
		// this is a "Override" command probably
		StdString eventName = events[msg];
		// we show current event name with check mark as identification
		// but we may do nothing in case of such selection
		if( eventName != (LPCTSTR)GetTitle() )
		{
			CUndoCommandGuard undoCommand;
			CStateMachineDiagramEditor *pEditor = static_cast<CStateMachineDiagramEditor *>( from );
			pEditor->SaveUndoSelection();
			CObjectsStateUndo objectProperty( &hszEventName );

			SetEEObjectProperty( &hszEventName, _T("name"), eventName.c_str() );

			pEditor->SaveRedoSelection();
		}
		return TRUE;
	}

	ASSERT(!"Insupported command");
	return FALSE;
}

void CEventDiagramEntity::SelectionEdit( CWnd* parent )
{
	ShowPropertiesDialog( parent );
}

void CEventDiagramEntity::ShowPropertiesDialog( CWnd* parent, BOOL show )
{
	if( show )
	{
		CPropertySheet properties( GetTitle(), parent );
		properties.m_psh.dwFlags &= ~PSH_HASHELP;
		CString eventName = GetName();
		CEventActionsPage actionsPage( eventName, IsAbstract() );
		CEventTransitionsPage transitionsPage( eventName, IsAbstract() );
		properties.AddPage( &actionsPage );
		properties.AddPage( &transitionsPage );
		properties.DoModal();
		// reload properties each time, because Apply/Cancel combination 
		// may be pressed
		LoadProperties();
		parent->Invalidate();
	}
}

void CEventDiagramEntity::SaveProperties()
{
	CHashString hszName = GetName();
	EDITINGPROPERTIESPARAMS epp;
	epp.name = &hszName;
	static const DWORD hashMsg_GetEditingProperties = CHashString( _T("GetEditingProperties") ).GetUniqueID();
	m_ToolBox->SendMessage( hashMsg_GetEditingProperties, sizeof( epp ), &epp );

	TCHAR buf[16];
	CRect rect = GetRect();
	epp.params[ _T("x") ] = _ltot( rect.left, buf, 10 );
	epp.params[ _T("y") ] = _ltot( rect.top, buf, 10 );
	epp.params[ _T("width") ] = _ltot( rect.Width(), buf, 10 );
	epp.params[ _T("height") ] = _ltot( rect.Height(), buf, 10 );

	static const DWORD hashMsg_SetEditingProperties = CHashString( _T("SetEditingProperties") ).GetUniqueID();
	m_ToolBox->SendMessage( hashMsg_SetEditingProperties, sizeof( epp ), &epp );
}

///	\brief	read current object state and refresh visualization properties
void CEventDiagramEntity::LoadProperties()
{
	m_Actions.clear();
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

	GETSETSTATEMACHINENAMESPARAMS gssmn;
	static CHashString hszCQHStateMachineEvent = _T("CQHStateMachineEvent");
	static DWORD msgHash_GetEventActionHandlers = CHashString( _T("GetEventActionHandlers") ).GetUniqueID();
	DWORD result = m_ToolBox->SendMessage( msgHash_GetEventActionHandlers, sizeof( gssmn ),
										  &gssmn, &hszName, &hszCQHStateMachineEvent );

	EDITINGPROPERTIESPARAMS epp;
	epp.name = &hszName;
	static const DWORD hashMsg_GetEditingProperties = CHashString( _T("GetEditingProperties") ).GetUniqueID();
	m_ToolBox->SendMessage( hashMsg_GetEditingProperties, sizeof( epp ), &epp );

	list<IHashString *>::iterator it = gssmn.Names.begin();
	for( ;it != gssmn.Names.end(); ++it )
	{
		m_Actions.push_back( GetActionName( *it ) );
	}

	int iX = _ttoi( epp.params[ _T("x") ] );
	int iY = _ttoi( epp.params[ _T("y") ] );
	int iWidth = _ttoi( epp.params[ _T("width") ] );
	int iHeight = _ttoi( epp.params[ _T("height") ] );

	SetRect( iX, iY, iX + iWidth, iY + iHeight );
}

CBrush *CEventDiagramEntity::GetDefaultBrush()
{
	static CBrush brush(LIGHT_GRAY);
	return &brush;
}

StdString CEventDiagramEntity::GetActionName(IHashString *pEEName)
{
	IArchive *archive = GetEEObjectProperties( pEEName );
	float fVersion = 1.0f;
	archive->Read( fVersion, _T("version") );
	StdString sName;
	if ( fVersion > 1.0f )
	{
		ASSERT(!"Unsupported version of archive");
		m_ToolBox->Log(LOGWARNING, _T("%s(%d): Unsupported version of archive"), __FILE__, __LINE__);
	}
	else
	{
		archive->Read( sName, _T("name") );
		archive->Read( sName, _T("actionName") );
	}
	archive->Close();
	return sName;
}

StdString CEventDiagramEntity::GetTransitionName(IHashString *pEEName)
{
	IArchive *archive = GetEEObjectProperties( pEEName );
	float fVersion = 1.0f;
	archive->Read( fVersion, _T("version") );
	StdString sName;
	if ( fVersion > 1.0f )
	{
		ASSERT(!"Unsupported version of archive");
		m_ToolBox->Log(LOGWARNING, _T("%s(%d): Unsupported version of archive"), __FILE__, __LINE__);
	}
	else
	{
		archive->Read( sName, _T("name") );
	}
	archive->Close();
	return sName;
}

///	\brief	collect event labels for event's parent state
///	\param	labels - output list with event labels
void CEventDiagramEntity::GetOverrideEventLabels( vector<StdString> &labels ) const
{
	CDiagramEntity *pState = GetParentEntity();
	CDiagramEntity *pParentState = pState->GetParentEntity();
	// we can get event list if parent state has another parent at least
	if( pParentState != NULL )
	{
		CHashString hszName = pParentState->GetName();
		GETSETSTATEMACHINENAMESPARAMS gssmn;
		static CHashString hszCQHState = _T("CQHState");
		static DWORD msgHash_GetStateAvailableEventLabels = CHashString( _T("GetStateAvailableEventLabels") ).GetUniqueID();
		DWORD result = m_ToolBox->SendMessage( msgHash_GetStateAvailableEventLabels, sizeof( gssmn ),
											   &gssmn, &hszName, &hszCQHState );
		if( MSG_HANDLED == result )
		{
			vector<StdString> parentEvents;
			transform( gssmn.Names.begin(), gssmn.Names.end(), 
					   back_inserter( parentEvents ), mem_fun( &IHashString::GetString ) );

			hszName = pState->GetName();
			gssmn.Names.clear();
			static DWORD msgHash_GetStateEventLabels = CHashString( _T("GetStateEventLabels") ).GetUniqueID();
			result = m_ToolBox->SendMessage( msgHash_GetStateEventLabels, sizeof( gssmn ),
											 &gssmn, &hszName, &hszCQHState );
			if( MSG_HANDLED == result )
			{
				sort( parentEvents.begin(), parentEvents.end() );
				vector<StdString> overridenEvents;
				transform( gssmn.Names.begin(), gssmn.Names.end(), 
						   back_inserter( overridenEvents ), mem_fun( &IHashString::GetString ) );
				sort( overridenEvents.begin(), overridenEvents.end() );
				vector<StdString>::iterator it;
				it = lower_bound( overridenEvents.begin(), overridenEvents.end(), GetTitle() );
				if( it != overridenEvents.end() )
				{
					overridenEvents.erase( it );
				}
				set_difference( parentEvents.begin(), parentEvents.end(),
								overridenEvents.begin(), overridenEvents.end(),
								back_inserter( labels ) );
			}
		}
	}
}

///	\brief	collect state labels for current event
///	\param	labels - output list with state labels
void CEventDiagramEntity::GetStateLabels( vector<StdString> &labels ) const
{
	CHashString hszName = GetName();
	GETSETSTATEMACHINENAMESPARAMS gssmn;
	static CHashString hszType = _T("CQHStateMachineEvent");
	static DWORD msgHash = CHashString( _T("GetStateMachineStateLabels") ).GetUniqueID();
	DWORD result = m_ToolBox->SendMessage( msgHash, sizeof( gssmn ),
										  &gssmn, &hszName, &hszType );
	if( MSG_HANDLED == result )
	{
		vector<StdString> parentEvents;
		transform( gssmn.Names.begin(), gssmn.Names.end(), 
				   back_inserter( labels ), mem_fun( &IHashString::GetString ) );
	}
}
