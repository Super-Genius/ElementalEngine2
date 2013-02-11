///============================================================================
/// \file		CEntityVisitor.h
/// \brief		Header file for visitor pattern to select CEntities by their
///				types
/// \date		9-1-2005
/// \author		Kyle Swaim
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

#ifndef CENTITYVISTISTOR_H
#define CENTITYVISTISTOR_H

//#include <assert.h>

#include "CEntity.h"

///============================================================================
/// \class	CEntityVisitor
/// \brief		Header file for visitor pattern to select CEntities by their
///				types
///============================================================================
class CEntityVisitor : public IVisitor
{
public:
	CEntityVisitor()
	{
		m_EntityTypeFilter = _T("");
		m_EntityList.clear();
	}

	~CEntityVisitor() {}

	bool Visit( IComponent * component, bool bVisitEnter )
	{
		IObject *theObject;
		CEntity *theEntity;
		IHashString *name;
		IHashString *parentName;
		IHashString *type;
//		IHashString *testType;
		std::string str;

		theObject = dynamic_cast<IObject *>(component);
		assert(theObject);
		name = theObject->GetName();
		parentName = theObject->GetParentName();
		type = theObject->GetComponentType();
		static CHashString testType( _T("CEntity") );

		// check if object is a CEntity
		//if( type == testType )
		if( type->GetUniqueID() == testType.GetUniqueID() )
		{
			// if its a CEntity cast it and check the types
			theEntity = dynamic_cast<CEntity *>(theObject);
			if( theEntity->IsEntityTypeOf( m_EntityTypeFilter ) )
			{
				// if the types match push it onto the stack
				m_EntityList.push_back( theObject );
			}
		}

//		delete testType;
		
		return true;
	}

	bool IsHierarchicalVisitor()
	{
//		return true;
		return false;
	}

	void SetEntityTypeFilter( StdString& type )
	{
		m_EntityTypeFilter = type;
	}

	OBJECTLIST* GetEntityList()
	{
		return &m_EntityList;
	}

private:
	StdString m_EntityTypeFilter;
	OBJECTLIST m_EntityList;
};

#endif	// #ifndef CENTITYVISTISTOR_H
