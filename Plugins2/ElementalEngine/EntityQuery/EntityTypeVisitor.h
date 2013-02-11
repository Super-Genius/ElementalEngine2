///============================================================================
/// \file		EntityTypeVisitor.h
/// \brief		Visitor for CEntity which populates a list by entity type
/// \date		2-16-2007
/// \author		Richard Nguyen
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

#ifndef __EntityTypeVisitor_H__
#define __EntityTypeVisitor_H__

class CEntityTypeVisitor : public IVisitor
{
public:	
	virtual bool Visit( IComponent* component, bool bVisitEnter );
	virtual bool IsHierarchicalVisitor();

	void SetObjectsList( OBJECTTYPEQUERYLIST* objectsList );

private:
	OBJECTTYPEQUERYLIST* m_ObjectsList;
};

#endif //__EntityTypeVisitor_H__