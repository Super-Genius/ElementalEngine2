///============================================================================
/// \file		EntityVisitor.h
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

#ifndef __EntityVisitor_H__
#define __EntityVisitor_H__

typedef multimap< float, IObject* > DISTANCEOBJECTMAP;

class CEntityVisitor : public IVisitor
{
public:	
	DISTANCEOBJECTMAP		m_SortedDistanceObjects;

	virtual bool Visit( IComponent* component, bool bVisitEnter );
	virtual bool IsHierarchicalVisitor();
	void SetQuery( Vec3& pos, IHashString* type );

private:
	Vec3 m_TestPosition;
	CHashString	m_EntityType;
};

#endif //__EntityVisitor_H__