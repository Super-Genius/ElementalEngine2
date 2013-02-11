///============================================================================
/// \file		CSaveVisitor.cpp
/// \brief		Implementation file for Save Visitor
/// \date		05-17-2007
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

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSaveVisitor::CSaveVisitor( IXMLArchive* pArchive )
	: m_pToolBox( EngineGetToolBox() )
	, m_pArchive( pArchive )
	, m_ar( NULL )
{
	assert( m_pArchive != NULL );
}

CSaveVisitor::~CSaveVisitor()
{
	if( m_ar != NULL )
	{
		m_ar->Close();
	}
}

bool CSaveVisitor::Visit( IComponent *component, bool bVisitEnter )
{
	if( bVisitEnter )
	{
		IObject *object;
		IHashString *name;
		IHashString *type;

		object = dynamic_cast<IObject *>(component);
		assert(object);
		name = object->GetName();
		type = object->GetComponentType();

		static const DWORD hashCQHState = CHashString( _T("CQHState") ).GetUniqueID();
		static const DWORD hashCQHStateMachineEvent = CHashString( _T("CQHStateMachineEvent") ).GetUniqueID();

		const DWORD hashType = type->GetUniqueID();
		const bool hasEditingProperties = ( hashType == hashCQHState )||( hashType == hashCQHStateMachineEvent );

		bool abstract = false;
		static const DWORD hashMsg_GetAbstract = CHashString( _T("GetAbstract") ).GetUniqueID();
		m_pToolBox->SendMessage( hashMsg_GetAbstract, sizeof( abstract ), &abstract, name, type );
		if( !abstract )
		{
			BeginDerivedState();
			m_pArchive->StartClass( type->GetString() );
			object->Serialize( *m_pArchive );
			SaveEditingProperties( name, m_pArchive );
			m_started.push_back( true );
			m_objects.push_back( object );
		}
		else if( hashType == hashCQHState )
		{
			m_started.push_back( false );
			m_objects.push_back( object );
			BeginDerivedState();
			SaveEditingProperties( name, m_pArchive );
		}
		else if( hashType == hashCQHStateMachineEvent )
		{
			BeginDerivedState();
			m_started.push_back( true );
			m_objects.push_back( object );
			BeginDerivedEvent();
			SaveEditingProperties( name, m_pArchive );
		}
		else
		{
			m_started.push_back( false );
			m_objects.push_back( object );
		}
	}
	else
	{
		if( m_started.back() )
		{
			m_pArchive->EndClass();
		}
		m_started.pop_back();
		m_objects.pop_back();
	}

	return true;
}

bool CSaveVisitor::IsHierarchicalVisitor( void )
{
	return true;
}

void CSaveVisitor::BeginDerivedState()
{
	if( !m_started.empty() && !m_started.back() )
	{
		StdString label = GetLabel( m_objects.back() );

		m_pArchive->StartClass( _T("CQHDerivedState") );
		m_pArchive->Write( 1.1f, _T("version") );
		m_pArchive->Write( label, _T("parentState") );
		m_started.back() = true;
	}
}

void CSaveVisitor::BeginDerivedEvent()
{
	StdString label = GetLabel( m_objects.back() );

	m_pArchive->StartClass( _T("CQHDerivedEvent") );
	m_pArchive->Write( 1.0f, _T("version") );
	m_pArchive->Write( label , _T("parentEvent") );
}

void CSaveVisitor::CreateLabelArchive()
{
	if( m_ar == NULL )
	{
		static CHashString streamType( _T("Memory") );
		CREATEARCHIVE ca;
		ca.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
		ca.streamData = NULL;
		ca.streamSize = 0;
		ca.streamType = &streamType;
		static DWORD msgHash_CreateArchive = CHashString( _T("CreateArchive") ).GetUniqueID();
		m_pToolBox->SendMessage( msgHash_CreateArchive, sizeof( ca ), &ca );
		m_ar = ca.archive;
	}
}

void CSaveVisitor::SaveEditingProperties( IHashString *name, IXMLArchive *ar )
{
	SAVEEDITINGPROPERTIESPARAMS sep;
	sep.name = name;
	sep.ar = ar;
	static const DWORD hash_SaveEditingProperties = CHashString( _T("SaveEditingProperties") ).GetUniqueID();
	m_pToolBox->SendMessage( hash_SaveEditingProperties, sizeof( sep ), &sep );
}

StdString CSaveVisitor::GetLabel( IObject *object )
{
	IAttributeObject *attr = dynamic_cast<IAttributeObject *>( object );
	assert( attr != NULL );

	CreateLabelArchive();

	m_ar->SetIsWriting( true );
	m_ar->SeekTo( 0 );
	static CHashString hszName = _T("name");
	attr->GetAttribute( &hszName, m_ar );
	m_ar->SetIsWriting( false );
	m_ar->SeekTo( 0 );
	StdString label;
	m_ar->Read( label, hszName.GetString() );
	return label;
}
