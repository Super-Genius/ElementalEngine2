///=====================================================================
/// \file	StateVisitor.cpp
/// \brief	Implementation of CStateVisitor
/// \date	01/27/2008
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
///=====================================================================
#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CStateVisitor::CStateVisitor( Hash2Hash &label2State )
	: m_label2State( label2State )
	, m_ar( NULL )
{
	static CHashString streamType( _T("Memory") );
	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_READ | STREAM_MODE_WRITE ;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &streamType;
	static DWORD msgHash_CreateArchive = CHashString( _T("CreateArchive") ).GetUniqueID();
	EngineGetToolBox()->SendMessage( msgHash_CreateArchive, sizeof(ca), &ca );
	m_ar = ca.archive;
}

CStateVisitor::~CStateVisitor()
{
	m_ar->Close();
}

bool CStateVisitor::Visit( IComponent *component, bool bVisitEnter )
{
	if( bVisitEnter )
	{
		static CHashString hszCQHState = _T("CQHState");
		if( hszCQHState == *component->GetComponentType() )
		{
			IAttributeObject *state = dynamic_cast<IAttributeObject *>( component );
			assert( state != NULL );
			DWORD hashName = state->GetName()->GetUniqueID();
			m_ar->SetIsWriting( true );
			m_ar->SeekTo( 0 );
			static CHashString hszName = _T("name");
			state->GetAttribute( &hszName, m_ar );
			m_ar->SetIsWriting( false );
			m_ar->SeekTo( 0 );
			StdString label;
			m_ar->Read( label, hszName.GetString() );
			CHashString hszLabel = label.c_str();
			m_label2State.insert( make_pair( hszLabel.GetUniqueID(), hashName ) );
		}
	}

	return true;
}

bool CStateVisitor::IsHierarchicalVisitor( void )
{
	return true;
}
