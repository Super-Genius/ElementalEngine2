///============================================================================
/// \file		CSpawnVisitor.h
/// \brief		Header file for visitor pattern to heirarchy
/// \date		06-04-2005
/// \author		Patrick Ghiocel
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
#ifndef _CSPAWNVISTISTOR_H_
#define _CSPAWNVISTISTOR_H_

#include <assert.h>

class CSpawnVisitor : public IVisitor
{
public:
	CSpawnVisitor( )
	{
		m_pArchiver = NULL;
	}

	bool Visit( IComponent * component, bool bVisitEnter )
	{
		IObject *theObject;
		IHashString *name;
		IHashString *parentName;
		IHashString *type;
		std::string str;

		theObject = dynamic_cast<IObject *>(component);
		assert(theObject);
		name = theObject->GetName();
		parentName = theObject->GetParentName();
		type = theObject->GetComponentType();

		if (name == NULL)
		{
			static CHashString NullName(_T("NULL"));
			name = &NullName;
		}	

		if( bVisitEnter == true )
		{
            if( m_pArchiver != NULL )
			{
				// Start the Node
				m_pArchiver->StartClass( type->GetString() );
				
				// Write out the Unique ID aka Name
				m_pArchiver->Write( name->GetString(), _T("Name") );
				theObject->Serialize( *m_pArchiver );
			}
		}

		else
		{
			if( m_pArchiver != NULL )
			{
				m_pArchiver->EndClass();
			}
		}
		
		return true;
	}

	bool IsHierarchicalVisitor( void )
	{
		return true;
	}

	void SetArchiver( IArchive * pArchiver )
	{
		if( pArchiver != NULL )
		{
			m_pArchiver = pArchiver;
		}
		else
		{
			m_pArchiver = NULL;
		}
	}

private:
	IArchive* m_pArchiver;
};

#endif		// #ifndef _CSPAWNVISTISTOR_H_
