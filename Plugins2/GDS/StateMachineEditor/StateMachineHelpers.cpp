///=====================================================================
/// \file	StateMachineHelpers.cpp
/// \brief	Implementation of helper functions for state machine
/// \date	12/29/2007
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

IObject *GetStateMachineObject( IHashString *name, IHashString *type )
{
	IObject *retObject = NULL;

	static CHashString hszStateManagerType( _T("CQHStateMachineManager") );
	IComponent *amanagerComponent = EngineGetToolBox()->GetComponent( &hszStateManagerType );
	if( amanagerComponent != NULL )
	{
		CManager *amanager = dynamic_cast<CManager *>( amanagerComponent );

		CHashString hszType;
		if( type == NULL )
		{
			hszType = GetComponentType( name );
			type = &hszType;
		}
		IDTOOBJECTMAP *objectMap = amanager->GetObjectMap( type );
		if( objectMap != NULL )
		{
			IDTOOBJECTMAP::iterator objectsIterator = objectMap->find( name->GetUniqueID() );
			if( objectsIterator != objectMap->end() )
			{
				retObject = objectsIterator->second;
			}
		}
	}

	return retObject;
}