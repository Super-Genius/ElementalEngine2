///============================================================================
/// \file	SimpleDiagramEntity.h
/// \brief	Implementation for State Machine Editor Simple diagram entity
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

class CInitializedFont : public CFont
{
public:
	CInitializedFont( HFONT hFont )
	{
		Attach( hFont );
	}
};

IToolBox *CSimpleDiagramEntity::m_ToolBox = EngineGetToolBox();

CSimpleDiagramEntity::CSimpleDiagramEntity( CDiagramEntity *parentEntity )
	: CDiagramEntity( parentEntity )
	, m_bAstract( false )
{
}

CSimpleDiagramEntity::~CSimpleDiagramEntity()
{
}

void CSimpleDiagramEntity::SetName( CString name )
{
	CDiagramEntity::SetName( name );
	
	CHashString hszName = name;
	CHashString hszType = GetType();
	ASSERT( !hszType.IsEmpty() );
	static const DWORD hashMsg_GetAbstract = CHashString( _T("GetAbstract") ).GetUniqueID();
	m_ToolBox->SendMessage( hashMsg_GetAbstract, sizeof( m_bAstract ), &m_bAstract, &hszName, &hszType );

	LoadProperties();
}

void CSimpleDiagramEntity::ShowProperties()
{
	CHashString hszName = GetName();
	CHashString hszType = GetType();
	SELECTOBJECTPARAMS sop;
	sop.object = &hszName;
	sop.compType = &hszType;
	static DWORD msgHash_SelectObject = CHashString(_T("SelectObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SelectObject, sizeof(sop), &sop );
}

void CSimpleDiagramEntity::Draw( CDC* dc, CRect rect )
{
	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );

	CRect labelRectangle( rect );
	labelRectangle.top = labelRectangle.top + 2;
	labelRectangle.left = labelRectangle.left + 2;

	dc->Rectangle( rect );
	dc->DrawText( GetTitle(), labelRectangle, DT_CENTER );
}

CDiagramEntity *CSimpleDiagramEntity::Clone()
{
	CSimpleDiagramEntity* obj = new CSimpleDiagramEntity( GetParentEntity() );
	obj->Copy( this );
	return obj;
}

void CSimpleDiagramEntity::SetRect( double left, double top, double right, double bottom )
{
	if( left != GetLeft() || top != GetTop() ||
		right != GetRight() || bottom != GetBottom() )
	{
		CDiagramEntity::SetRect( left, top, right, bottom );
		SaveProperties();
	}
}

IAttributeObject *CSimpleDiagramEntity::GetAttributeObject()
{
	CHashString hszType = GetType();
	CHashString hszName = GetName();
	return static_cast<IAttributeObject *>( GetStateMachineObject( &hszName, &hszType ) );
}

void CSimpleDiagramEntity::SaveProperties()
{
}

///	\brief	read current object state and refresh visualization properties
void CSimpleDiagramEntity::LoadProperties()
{
}

BOOL CSimpleDiagramEntity::DoMessage( UINT msg, CDiagramEntity* sender, CWnd* from )
{
	// translate messages
	switch( msg )
	{
		case ID_ENTITY_UP:
			msg = CMD_UP;
			break;
		case ID_ENTITY_DOWN:
			msg = CMD_DOWN;
			break;
		case ID_ENTITY_TOFRONT:
			msg = CMD_FRONT;
			break;
		case ID_ENTITY_TOBACK:
			msg = CMD_BOTTOM;
			break;
		case ID_ENTITY_PROPERTIES:
			msg = CMD_PROPERTIES;
			break;
	}
	return CDiagramEntity::DoMessage( msg, sender, from );
}

CPen *CSimpleDiagramEntity::GetPen() const
{
	if( IsAbstract() )
	{
		static CPen abtractObjectPen( PS_SOLID, 1, GRAY );
		return &abtractObjectPen;
	}
	else
	{
		static CPen normalObjectPen( PS_SOLID, 1, BLACK );
		return &normalObjectPen;
	}
}

CBrush *CSimpleDiagramEntity::GetBrush() const
{
	if( IsAbstract() )
	{
		static CBrush abtractObjectBrush( GRAY );
		return &abtractObjectBrush;
	}
	else
	{
		static CBrush normalObjectBrush( BLACK );
		return &normalObjectBrush;
	}
}

COLORREF CSimpleDiagramEntity::GetColor() const
{
	if( IsAbstract() )
	{
		return GRAY;
	}
	else
	{
		return BLACK;
	}
}

CFont *CSimpleDiagramEntity::GetFont() const
{
	if( IsAbstract() )
	{
		static CInitializedFont abstractObjectFont = CreateBaseFont( true );
		return &abstractObjectFont;
	}
	else
	{
		static CInitializedFont normalObjectFont = CreateBaseFont( false );
		return &normalObjectFont;
	}
}

HFONT CSimpleDiagramEntity::CreateBaseFont( bool italic )
{
	return CreateFont( -14, // lfHeight 
					   0, // lfWidth 
					   0, // lfEscapement 
					   0, // lfOrientation 
					   FW_DONTCARE, // lfWeight 
					   italic ? TRUE : FALSE, // lfItalic 
					   FALSE, // lfUnderline 
					   FALSE, // lfStrikeOut 
					   DEFAULT_CHARSET,// lfCharSet 
					   OUT_DEFAULT_PRECIS, // lfOutPrecision 
					   CLIP_DEFAULT_PRECIS, // lfClipPrecision 
					   DEFAULT_QUALITY, // lfQuality 
					   DEFAULT_PITCH, // lfPitchAndFamily 
					   _T("Arial") // lfFaceName
					  );
}
