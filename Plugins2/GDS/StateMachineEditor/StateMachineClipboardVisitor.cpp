///=====================================================================
/// \file	StateMachineClipboardVisitor.cpp
/// \brief	Implementation of CStateMachineClipboardVisitor
/// \date	12/16/2007
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
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CStateMachineClipboardVisitor::CStateMachineClipboardVisitor( IXMLArchive *pArchive )
	: m_pToolBox( EngineGetToolBox() )
	, m_pArchive( pArchive )
{
}

CStateMachineClipboardVisitor::~CStateMachineClipboardVisitor()
{
}

bool CStateMachineClipboardVisitor::IsHierarchicalVisitor()
{
	return true;
}

bool CStateMachineClipboardVisitor::Visit( IComponent * component, bool bVisitEnter )
{
	ASSERT( m_pArchive != NULL );
	ASSERT( m_pArchive->IsWriting() );

	IObject *pObject = dynamic_cast<IObject *>( component );
	if( pObject != NULL )
	{
		if( bVisitEnter )
		{
			IHashString *type = pObject->GetComponentType();
			IHashString *name = pObject->GetName();
			m_pArchive->StartClass( type->GetString() );
			pObject->Serialize( *m_pArchive );
			SAVEEDITINGPROPERTIESPARAMS sep;
			sep.name = name;
			sep.ar = m_pArchive;
			static const DWORD hash_SaveEditingProperties = CHashString( _T("SaveEditingProperties") ).GetUniqueID();
			m_pToolBox->SendMessage( hash_SaveEditingProperties, sizeof( sep ), &sep );
		}
		else
		{
			m_pArchive->EndClass();
		}
	}
	return true;
}
