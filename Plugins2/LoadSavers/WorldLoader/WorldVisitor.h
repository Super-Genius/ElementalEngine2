///============================================================================
/// \file		CWorldVisitor.h
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
#ifndef _CWORLDVISTISTOR_H_
#define _CWORLDVISTISTOR_H_

#include <assert.h>

class CWorldVisitor : public IVisitor
{
public:
	CWorldVisitor( )
	{
		m_pArchiver = NULL;
		m_fWorldVersion = 1.0f;
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
		static CHashString nullHash(_T("NULL"));

		if (name == NULL)
		{
			name = &nullHash;
		}

		if( bVisitEnter == true )
		{
            if( m_pArchiver != NULL )
			{
				// Start the Node
				m_pArchiver->StartClass( type->GetString() );
				// Write out the Unique ID aka Name
				m_pArchiver->Write( name->GetString(), _T("Name") );
				static CHashString worldHash(_T("CWorld"));

				// Custom Serialize Function for CWorld
				if( type->GetUniqueID() == worldHash.GetUniqueID() )
				{
					m_pArchiver->Write( m_fWorldVersion, "Version" );

				}
				// Call Serialize on the Object
				else
				{
					theObject->Serialize( *m_pArchiver );
				}
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

	void SetWorldVersion( float fWorldVersion )
	{
		m_fWorldVersion = fWorldVersion;
	}

private:
	IArchive* m_pArchiver;

	float m_fWorldVersion;
};

#endif		// #ifndef _CWORLDVISTISTOR_H_
