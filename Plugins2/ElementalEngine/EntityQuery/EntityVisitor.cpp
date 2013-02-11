///============================================================================
/// \file		EntityVisitor.cpp
/// \brief		Implementation of EntityQuery System
/// \date		12-20-2006
/// \author		Patrick Ghiocel
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


bool CEntityVisitor::IsHierarchicalVisitor( void )
{
	return false;
}

void CEntityVisitor::SetQuery( Vec3& pos, IHashString *type )
{
	m_SortedDistanceObjects.clear();
	m_TestPosition = pos;

	if( type != NULL )
	{
		m_EntityType = type;
	}
	else
	{
		m_EntityType = _T("");
	}
}

/// ENTITY type
bool CEntityVisitor::Visit( IComponent* component, bool bVisitEnter )
{	
	ISceneObject* sceneObject = (ISceneObject*)component;	

	// Get the distance between the query origin and the object.
	Vec3 objPos;
	float objRad;
	sceneObject->GetBoundingSphere( objPos, objRad );
	Vec3 differenceVector = objPos - m_TestPosition;
	float distance = differenceVector.Length();

	// If an entity type was not specified then add all objects.
	if( _tcscmp(m_EntityType.GetString(), _T("") ) == 0 )
	{
		m_SortedDistanceObjects.insert( pair< float, IObject*>(distance, sceneObject) );
	}
	// If an entity type was specified then check if the object type matches before adding.
	else
	{
		// Grab the entity type
		CHashString hszEntityType;
		static DWORD msgHash_GetEntityType = CHashString(_T("GetEntityType")).GetUniqueID();
		static CHashString typeHash_CEntity = CHashString( _T("CEntity") );
		DWORD retval = EngineGetToolBox()->SendMessage( msgHash_GetEntityType, sizeof(IHashString*), (void*)(&hszEntityType), sceneObject->GetName(), &typeHash_CEntity );
		
		if( retval == MSG_HANDLED && _tcscmp(m_EntityType.GetString(), hszEntityType.GetString() ) == 0 )
		{
			m_SortedDistanceObjects.insert( pair< float, IObject*>(distance, sceneObject) );
		}
	}

	return true;
}
