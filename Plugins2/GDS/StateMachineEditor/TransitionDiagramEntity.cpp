///============================================================================
/// \file	TransitionDiagramEntity.h
/// \brief	Implementation for State Machine Editor Transition diagram entity
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////// CBaseTransitionDiagramEntity ///////////////////////////////////
CHashString CBaseTransitionDiagramEntity::m_hszTransitionType = _T("CQHStateMachineTransition");

CBaseTransitionDiagramEntity::CBaseTransitionDiagramEntity( CDiagramEntity *parentEntity )
	: CSimpleDiagramEntity( parentEntity )
	, m_iWeight( 0 )
{
	SetType( m_hszTransitionType.GetString() );
}

CBaseTransitionDiagramEntity::~CBaseTransitionDiagramEntity()
{
}

CDiagramEntity *CBaseTransitionDiagramEntity::Clone()
{
	CBaseTransitionDiagramEntity* obj = new CBaseTransitionDiagramEntity( GetParentEntity() );
	obj->Copy( this );
	return obj;
}

CGdiObject *CBaseTransitionDiagramEntity::SetPen( CDC* dc ) const
{
	return dc->SelectObject( GetPen() );
}

CGdiObject *CBaseTransitionDiagramEntity::SetBrush( CDC* dc, bool source ) const
{
	return dc->SelectStockObject( BLACK_BRUSH );
}

void CBaseTransitionDiagramEntity::DrawObject( CDC* dc, double zoom )
{
	const double arrowLength = 16.0 * zoom;
	const double shortPartLength = 12.0 * zoom;
	const double arrowWidth = 6.0 * zoom;

	CLine line = CalcLineRect() * zoom;

	const double left2right = ( line.destX > line.srcX ) ? 1.0 : -1.0;
	const double bottom2top = ( line.destY > line.srcY ) ? 1.0 : -1.0;
	const double NW2SE = left2right * bottom2top;
	const double width =  ( line.destX > line.srcX ) ? ( line.destX - line.srcX ) : ( line.srcX - line.destX );
	const double height = ( line.destY > line.srcY ) ? ( line.destY - line.srcY ) : ( line.srcY - line.destY );
	double len = sqrt( pow( width, 2) + pow( height, 2) );
	double kx = bottom2top * height / len; // cos
	double ky = left2right * width / len; // sin

	// calculate line.destY point of the arrow
	double x = line.destX - arrowLength * ky;
	double y = line.destY - arrowLength * kx;

	// calculate offset for bottom part of the arrow
	double dx = arrowWidth * kx;
	double dy = arrowWidth * ky;

	POINT arrow[4];
	// adjust bottom part coordinate
	arrow[0].x = static_cast<LONG>( x + dx );
	arrow[0].y = static_cast<LONG>( y - dy );
	// edge of the arrow
	arrow[1].x = static_cast<LONG>( line.destX );
	arrow[1].y = static_cast<LONG>( line.destY );
	// adjust bottom part coordinate
	arrow[2].x = static_cast<LONG>( x - dx );
	arrow[2].y = static_cast<LONG>( y + dy );

	// prevent arrow shifting because of value rounding 
	if( abs( line.destX - x ) < 1.0 )
	{
		arrow[0].y = arrow[2].y;
	}
	else if( abs( line.destY - y ) < 1.0 )
	{
		arrow[0].x = arrow[2].x;
	}

	// adjust end of the arrow line
	//line.destX = x;
	//line.destY = y;
	line.destX -= shortPartLength * ky;
	line.destY -= shortPartLength * kx;

	arrow[3].x = static_cast<LONG>( line.destX  );
	arrow[3].y = static_cast<LONG>( line.destY );

	// set appropriate filling
	CGdiObject *pOldPen = SetPen( dc );
	// set brush for target
	CGdiObject *pOldBrush = SetBrush( dc, false );
	dc->Polygon( arrow, _countof( arrow ) );

	// calculate center point of the diamond
	x = line.srcX + arrowLength * ky * 0.5;
	y = line.srcY + arrowLength * kx * 0.5;

	POINT diamond[4];
	// edges of the diamond
	diamond[0].x = static_cast<LONG>( line.srcX );
	diamond[0].y = static_cast<LONG>( line.srcY );
	diamond[1].x = static_cast<LONG>( x - dx );
	diamond[1].y = static_cast<LONG>( y + dy );
	diamond[3].x = static_cast<LONG>( x + dx );
	diamond[3].y = static_cast<LONG>( y - dy );

	// adjust start of the arrow line
	line.srcX += arrowLength * ky;
	line.srcY += arrowLength * kx;

	// and this point is a bottom of the diamond
	diamond[2].x = static_cast<LONG>( line.srcX );
	diamond[2].y = static_cast<LONG>( line.srcY );

	// draw diamond
	// set brush for source
	SetBrush( dc, true );
	dc->Polygon( diamond, _countof( diamond ) );

	// connect arrow and diamond
	dc->MoveTo( static_cast< int>( line.srcX ), static_cast< int>( line.srcY ) );
	dc->LineTo( static_cast< int>( line.destX ), static_cast< int>( line.destY ) );

	if( IsSelected() )
	{
		CSize szMarker = CDiagramEntity::GetMarkerSize();
		int x = static_cast< int>( ( line.srcX + line.destX ) / 2 );
		int y = static_cast< int>( ( line.srcY + line.destY ) / 2 );
		CRect rectSelect( x, y, x, y );
		rectSelect.InflateRect( szMarker.cx / 2, szMarker.cy / 2 );

		dc->SelectStockObject( BLACK_BRUSH );
		dc->Rectangle( rectSelect );
	}

	dc->SelectObject( pOldPen );
	dc->SelectObject( pOldBrush );
}

void CBaseTransitionDiagramEntity::SelectionEdit( CWnd* parent )
{
}

double CBaseTransitionDiagramEntity::CalculateLength( const CLine &line )
{
	const double width =  ( line.destX > line.srcX )
							? ( line.destX - line.srcX )
							: ( line.srcX - line.destX );
	const double height = ( line.destY > line.srcY )
							? ( line.destY - line.srcY )
							: ( line.srcY - line.destY );
	return sqrt( pow( width, 2) + pow( height, 2) );
}

CBaseTransitionDiagramEntity::CLine CBaseTransitionDiagramEntity::CalcLineRect()  const
{
	CDiagramEntity *pSourceEntity = GetSource();
	CDiagramEntity *pTargetEntity = GetTarget();
	ASSERT( pSourceEntity != NULL || pTargetEntity != NULL );

	if( pSourceEntity != NULL && pTargetEntity != NULL )
	{
		// both entities are in view. Calculate short way between both rectangles
		
		// determine centers of both rectangles
		double srcX = ( pSourceEntity->GetLeft() + pSourceEntity->GetRight() ) * 0.5;
		double srcY = ( pSourceEntity->GetTop() + pSourceEntity->GetBottom() ) * 0.5;
		double destX = ( pTargetEntity->GetLeft() + pTargetEntity->GetRight() ) * 0.5;
		double destY = ( pTargetEntity->GetTop() + pTargetEntity->GetBottom() ) * 0.5;
		
		// finding short distance between connection points
		CLine lines[4];
		lines[0].srcX = srcX;
		lines[0].destX = destX;
		if( srcY > destY )
		{
			lines[0].srcY = pSourceEntity->GetTop();
			lines[0].destY = pTargetEntity->GetBottom();
		}
		else
		{
			lines[0].srcY = pSourceEntity->GetBottom();
			lines[0].destY = pTargetEntity->GetTop();
		}

		lines[1].srcY = srcY;
		lines[1].destY = destY;
		if( srcX > destX )
		{
			lines[1].srcX = pSourceEntity->GetLeft();
			lines[1].destX = pTargetEntity->GetRight();
		}
		else
		{
			lines[1].srcX = pSourceEntity->GetRight();
			lines[1].destX = pTargetEntity->GetLeft();
		}

		lines[2].srcX = lines[0].srcX;
		lines[2].srcY = lines[0].srcY;
		lines[2].destX = lines[1].destX;
		lines[2].destY = lines[1].destY;

		lines[3].srcX = lines[1].srcX;
		lines[3].srcY = lines[1].srcY;
		lines[3].destX = lines[0].destX;
		lines[3].destY = lines[0].destY;

		double shortestLineLength = CalculateLength( lines[0] );
		CLine shortestLine = lines[0];
		for( int i = 1; i < _countof( lines ); ++i )
		{
			double length = CalculateLength( lines[i] );
			if( length < shortestLineLength )
			{
				shortestLineLength = length;
				shortestLine = lines[i];
			}
		}

		return shortestLine;
	}

	CLine res;
	// one of entities is outside of current view
	if( pSourceEntity != NULL)
	{
		res.srcY = pSourceEntity->GetBottom();
		res.srcX = ( pSourceEntity->GetLeft() + pSourceEntity->GetRight() ) * 0.5;

		res.destX = res.srcX;
		res.destY = GetVirtualHeight();
	}
	else
	{
		ASSERT( pTargetEntity != NULL );

		res.destY = pTargetEntity->GetTop();
		res.destX = ( pTargetEntity->GetLeft() + pTargetEntity->GetRight() ) * 0.5;

		res.srcX = res.destX;
		res.srcY = 0;
	}
	// both entities are outside of view
	return res;
}

///	\brief	get pointer to source entity for transition line drawing
CDiagramEntity *CBaseTransitionDiagramEntity::GetSource() const
{
	return NULL;
}

///	\brief	get pointer to target entity for transition line drawing
CDiagramEntity *CBaseTransitionDiagramEntity::GetTarget() const
{
	return NULL;
}

///	\breif	return virtual screen height
int CBaseTransitionDiagramEntity::GetVirtualHeight() const
{
	return 0;
}

CRect CBaseTransitionDiagramEntity::GetRect() const
{
	CLine line = CalcLineRect();
	CRect rect( static_cast< int >( line.srcX ), 
				static_cast< int >( line.srcY ), 
				static_cast< int >( line.destX ), 
				static_cast< int >( line.destY ) );
	return rect;
}

int CBaseTransitionDiagramEntity::GetHitCode( CPoint point ) const
{
	CDiagramLine line;
	line.SetRect( GetRect() );
	return line.GetHitCode( point );
}

int CBaseTransitionDiagramEntity::GetHitCode( const CPoint& point, const CRect& rect ) const
{
	CDiagramLine line;
	line.SetRect( GetRect() );
	return line.GetHitCode( point, rect );
}

BOOL CBaseTransitionDiagramEntity::BodyInRect( CRect rect ) const
{
	return FALSE;
}

HCURSOR	CBaseTransitionDiagramEntity::GetCursor( int hit ) const
{
	return NULL;
}

void CBaseTransitionDiagramEntity::ShowPopup( CPoint point, CWnd* parent )
{
	CMenu menu;
	menu.LoadMenu( IDR_TRANSITION_CONTEXT_MENU );
	menu.GetSubMenu(0)->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, parent );
}

BOOL CBaseTransitionDiagramEntity::DoMessage( UINT msg, CDiagramEntity* sender, CWnd* from )
{
	CStateMachineDiagramEditor *pEditor = static_cast<CStateMachineDiagramEditor *>( from );
	if( msg == ID_TRANSITION_SELECTSOURCE )
	{
		CHashString hszName = GetName();
		CSimpleDiagramEntity *pTransition = pEditor->GetEntity( &hszName );
		CHashString hszEventName = pTransition->GetParentEntity()->GetName();
		pEditor->SelectObject( &hszEventName );
		return TRUE;
	}
	else if( msg == ID_TRANSITION_SELECTTARGET )
	{
		CHashString hszName = GetName();
		IArchive *ar = GetEEObjectProperties( &hszName );

		float fVersion;
		StdString sName;
		int weight;
		StdString sTargetState;

		ar->Read( fVersion, _T("version") );
		if ( fVersion > 1.0f )
		{
			ASSERT( !"Unsupported version of archive" );
			m_ToolBox->Log( LOGWARNING, _T("%s(%d): Unsupported version of archive"), __FILE__, __LINE__ );
			ar->Close();
			return TRUE;
		}
		ar->Read( sName, _T("name") );
		ar->Read( weight, _T("weight") );
		ar->Read( sTargetState, _T("targetState") );
		ar->Close();

		CDiagramEntity *pState = pEditor->GetStateByTitle( sTargetState.c_str() );
		ASSERT( pState != NULL );
		CHashString hszStateName = pState->GetName();
		pEditor->SelectObject( &hszStateName );

		return TRUE;
	}
	return CSimpleDiagramEntity::DoMessage( msg, sender, from );
}

/////////////////// CDrivenTransitionDiagramEntity /////////////////////////////
CDrivenTransitionDiagramEntity::CDrivenTransitionDiagramEntity(
									CDiagramEntity *parentEntity,
									CStateMachineDiagramEditor *pEditor,
									CDiagramEntity *source, bool sourceInView, 
									CDiagramEntity *target, bool targetInView )
	: CBaseTransitionDiagramEntity( parentEntity )
	, m_pEditor( pEditor )
	, m_pSource( source )
	, m_pTarget( target )
	, m_bSourceInView( sourceInView )
	, m_bTargetInView( targetInView )
{
}

CDrivenTransitionDiagramEntity::~CDrivenTransitionDiagramEntity()
{
	CHashString hszName = GetName();
	CSimpleDiagramEntity *pEntity = m_pEditor->GetEntity( &hszName );
	if( pEntity != NULL )
	{
		CTransitionDiagramEntity *pTransition;
		pTransition = static_cast<CTransitionDiagramEntity *>( pEntity );
		pTransition->RemoveDerived( this );
	}
}

CGdiObject *CDrivenTransitionDiagramEntity::SetBrush( CDC* dc, bool source ) const
{
	if( source )
	{
		if( !m_bSourceInView )
		{
			return dc->SelectStockObject( WHITE_BRUSH );
		}
	}
	else
	{
		if( !m_bTargetInView )
		{
			return dc->SelectStockObject( WHITE_BRUSH );
		}
	}
	return dc->SelectObject( GetBrush() );
}

///	\brief	get pointer to source entity for transition line drawing
CDiagramEntity *CDrivenTransitionDiagramEntity::GetSource() const
{
	return m_pSource;
}

///	\brief	get pointer to target entity for transition line drawing
CDiagramEntity *CDrivenTransitionDiagramEntity::GetTarget() const
{
	return m_pTarget;
}

///	\breif	return virtual screen height
int CDrivenTransitionDiagramEntity::GetVirtualHeight() const
{
	return m_pEditor->GetVirtualSize().cy;
}

/////////////////// CTransitionDiagramEntity ///////////////////////////////
CTransitionDiagramEntity::CTransitionDiagramEntity(
	CDiagramEntity *parentEntity, CStateMachineDiagramEditor *pEditor )
	: CBaseTransitionDiagramEntity( parentEntity )
	, m_pEditor( pEditor )
	, m_pDrivenEntity1( NULL )
	, m_pDrivenEntity2( NULL )
	, m_pDrivenEntity3( NULL )
{
}

CTransitionDiagramEntity::~CTransitionDiagramEntity()
{
	ReleaseDrivenEntities();
}

///	\brief	read current object state and refresh visualization properties
void CTransitionDiagramEntity::LoadProperties()
{
	CHashString hszName = GetName();
	if( hszName.IsEmpty() )
	{
		SetTitle( _T("") );
		return;
	}

	IArchive *ar = GetEEObjectProperties( &hszName );

	float fVersion;
	StdString sName;
	StdString sTargetState;

	ar->Read( fVersion, _T("version") );
	if ( fVersion > 1.0f )
	{
		ASSERT( !"Unsupported version of archive" );
		m_ToolBox->Log( LOGWARNING, _T("%s(%d): Unsupported version of archive"), __FILE__, __LINE__ );
		ar->Close();
		return;
	}
	ar->Read( sName, _T("name") );
	ar->Read( m_iWeight, _T("weight") );
	ar->Read( sTargetState, _T("targetState") );

	SetTitle( sName.c_str() );

	ar->Close();

	m_targetState = sTargetState;
	// referred state may renamed and link will be dropped (or restored)
	UpdateDrivenEntities();
}

void CTransitionDiagramEntity::RemoveDerived( CDrivenTransitionDiagramEntity *pEntity )
{
	if( m_pDrivenEntity1 == pEntity )
	{
		m_pDrivenEntity1 = NULL;
	}
	else if( m_pDrivenEntity2 == pEntity )
	{
		m_pDrivenEntity2 = NULL;
	}
	else if( m_pDrivenEntity3 == pEntity )
	{
		m_pDrivenEntity3 = NULL;
	}
	else
	{
		ASSERT( false );
	}
}

void CTransitionDiagramEntity::ReleaseDrivenEntities()
{
	if( m_pDrivenEntity1 != NULL )
	{
		m_pDrivenEntity1->GetParentContainer()->Remove( m_pDrivenEntity1 );
		m_pDrivenEntity1 = NULL;
	}

	if( m_pDrivenEntity2 != NULL )
	{
		m_pDrivenEntity2->GetParentContainer()->Remove( m_pDrivenEntity2 );
		m_pDrivenEntity2 = NULL;
	}

	if( m_pDrivenEntity3 != NULL )
	{
		m_pDrivenEntity3->GetParentContainer()->Remove( m_pDrivenEntity3 );
		m_pDrivenEntity3 = NULL;
	}
}

void CTransitionDiagramEntity::UpdateDrivenEntities()
{
	CDiagramEntity *pSourceEntity = GetParentEntity();
	ASSERT( pSourceEntity != NULL );
	CDiagramEntity *pTargetState = m_pEditor->GetStateByTitle( m_targetState.c_str() );

	ReleaseDrivenEntities();

	CDiagramEntity *pSourceParent = pSourceEntity->GetParentEntity();
	CDiagramEntity *pState = GetEntitiyInView( pSourceParent, pTargetState );

	bool targetInView = ( pState == pTargetState );

	m_pDrivenEntity1 = new CDrivenTransitionDiagramEntity( pSourceParent, m_pEditor,
														   pSourceEntity, true,
														   pState, targetInView );
	m_pDrivenEntity1->SetName( GetName() );
	m_pEditor->AddObject( pSourceParent, m_pDrivenEntity1, false );
	m_pDrivenEntity1->GetParentContainer()->Bottom( m_pDrivenEntity1 );

	if( !targetInView )
	{
		CDiagramEntity *pTargetParent = pTargetState->GetParentEntity();
		pState = GetEntitiyInView( pTargetParent, pSourceEntity );

		m_pDrivenEntity2 = new CDrivenTransitionDiagramEntity( pTargetParent, m_pEditor,
															   pState, false,
															   pTargetState, true );
		m_pDrivenEntity2->SetName( GetName() );
		m_pEditor->AddObject( pTargetParent, m_pDrivenEntity2, false );
		m_pDrivenEntity2->GetParentContainer()->Bottom( m_pDrivenEntity2 );
	}

	// state machine may be saved incorrectly
	if( pTargetState != NULL )
	{
		pState = m_pEditor->GetFlatModeEntity( pSourceParent->GetName() );
		ASSERT( pState != NULL );
		pTargetState = m_pEditor->GetFlatModeEntity( pTargetState->GetName() );
		ASSERT( pTargetState != NULL );
		m_pDrivenEntity3 = new CDrivenTransitionDiagramEntity( NULL, m_pEditor,
															   pState, true,
															   pTargetState, true );
		m_pDrivenEntity3->SetName( GetName() );
		m_pEditor->AddFlatModeObject( m_pDrivenEntity3 );
		m_pDrivenEntity3->GetParentContainer()->Bottom( m_pDrivenEntity3 );
	}
	else
	{
		ASSERT( !"Target state is unknown" );
	}
}

///	\brief	look up hierarchy for parent state which matches passed parent
///	\param	pParent	- pointer to parent of the desired view		
///	\param	pEntity - pointer to entity to look up
///	\return	pointer to entity which has pParent as parent and pEntity as child
///			NULL if such entity does not exist
CDiagramEntity *CTransitionDiagramEntity::GetEntitiyInView( CDiagramEntity *pParent, CDiagramEntity *pEntity )
{
	if( pEntity == NULL )
	{
		return NULL;
	}
	CDiagramEntity *pEntityParent = pEntity->GetParentEntity();
	while( pParent != pEntityParent )
	{
		pEntity = pEntityParent;
		if( pEntity == NULL)
		{
			break;
		}
		pEntityParent = pEntity->GetParentEntity();
	}

	return pEntity;
}

///	this function refreshes all entities with passed labels. If old label is
///	equal to new label then this means that state references should now be 
///	updated. Nevertheless object should update own state based on passed label
/// because user may select to not update references
///	\param	oldLabel - old state label
///	\param	newLabel - new state label.
void CTransitionDiagramEntity::OnStateUpdate( const StdString &oldLabel, const StdString &newLabel )
{
	if( m_targetState == oldLabel )
	{
		CHashString hszName = GetName();
		CObjectsStateUndo objectState( &hszName );
		if( oldLabel != newLabel )
		{
			m_targetState = newLabel;
			SetEEObjectProperty( &hszName, _T("targetState"), newLabel );
			// we need to update driven entities after updating
		}
		UpdateDrivenEntities();
	}
}

///	check if object refers to state with passed label
///	\param	label - label of the state
///	\return	true if object refers to the state
bool CTransitionDiagramEntity::IsStateReferred( const StdString &label ) const
{
	return m_targetState == label;
}

/////////////////// CDraggingTransitionDiagramEntity ///////////////////////////
CDraggingTransitionDiagramEntity::CDraggingTransitionDiagramEntity( CDiagramEntity *parentEntity, 
											CDiagramEntity *source, CDiagramEntity *target)
	: CBaseTransitionDiagramEntity( parentEntity )
	, m_pSource( source )
	, m_pTarget( target )
{
}

CGdiObject *CDraggingTransitionDiagramEntity::SetPen( CDC* dc ) const
{
	static CPen pPen( PS_DASH, 1, GRAY );
	return dc->SelectObject( &pPen );
}

CGdiObject *CDraggingTransitionDiagramEntity::SetBrush( CDC* dc, bool bInView ) const
{
	return dc->SelectStockObject( WHITE_BRUSH );
}

///	\brief	get pointer to source entity for transition line drawing
CDiagramEntity *CDraggingTransitionDiagramEntity::GetSource() const
{
	return m_pSource;
}

///	\brief	get pointer to target entity for transition line drawing
CDiagramEntity *CDraggingTransitionDiagramEntity::GetTarget() const
{
	return m_pTarget;
}
