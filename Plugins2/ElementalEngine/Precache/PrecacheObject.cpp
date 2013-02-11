///==========================================================================
/// \file	Precache.cpp
/// \brief	Implementation of Precache Resource File
/// \date	12/18/2007
/// \author	Patrick Ghiocel
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
///==========================================================================

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CPrecacheObject);

CPrecacheObject::CPrecacheObject(IHashString *parentName, IHashString *name, bool bAddToHier) : 
	OBJECTTEMPLATE(CPrecacheManager, CPrecacheObject, IObject, parentName, name)
{
	m_ToolBox = EngineGetToolBox();
	m_iVersion = 1;
	Init();
}

CPrecacheObject::~CPrecacheObject()
{
}

IComponent* CPrecacheObject::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool bAddToHier;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new CPrecacheObject(parentName, name, false);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CPrecacheObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}


	return self;
}

void CPrecacheObject::Init( void )
{

}

bool CPrecacheObject::Update( void )
{
	return true;
}


void CPrecacheObject::DeInit( void )
{
	// Save out the precache
	static DWORD msgHash_SaveFileByExtension = CHashString(_T("SaveFileByExtension")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SaveFileByExtension, sizeof( LPTSTR), (void*)GetName()->GetString() );
}

void CPrecacheObject::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		// Version
		ar.Read( m_iVersion );
	
		// Number of Entries
		unsigned int tmpCount;
		ar.Read( tmpCount );
	
		// Read Entries and insert into the list
		for( unsigned int i = 0; i < tmpCount; i++ )
		{
			CHashString hszResourceName;
			StdString szResourceName;
			ar.Read( szResourceName );
			hszResourceName.Init( szResourceName );
			m_ResourceSet.insert( hszResourceName.GetUniqueID());
		}
	}
	
	else
	{
		// Version
		ar.Write( m_iVersion, _T("Version") );
		
		// Number of Entries
		ar.Write( (int)m_ResourceSet.size(), _T("NumEntries") );

		// Write out all the Entries from the list
		RESOURCESET::iterator itr = m_ResourceSet.begin();
		StdString szPrecacheEntry( _T("PrecacheEntry") );
		while( itr != m_ResourceSet.end() )
		{
			ar.StartClass( szPrecacheEntry );
			ar.Write( m_ToolBox->GetHashString( *itr ), _T("Name") );
			ar.EndClass();
			itr++;
		}
	}
}

void CPrecacheObject::AddResource( IHashString *filename )
{
	if( filename != NULL )
	{
		RESOURCESET::iterator itr = m_ResourceSet.find( filename->GetUniqueID() );
		if( itr == m_ResourceSet.end() )
		{
			m_ResourceSet.insert( filename->GetUniqueID() );
		}
	}
}

void CPrecacheObject::RemoveResource( IHashString *filename )
{
	if( filename != NULL )
	{
		RESOURCESET::iterator itr = m_ResourceSet.find( filename->GetUniqueID() );
		if( itr != m_ResourceSet.end() )
		{
			m_ResourceSet.erase( itr );
		}
	}
}

RESOURCESET* CPrecacheObject::GetResourceList( void )
{
	return &m_ResourceSet;
}
