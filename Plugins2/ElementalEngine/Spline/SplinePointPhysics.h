///============================================================================
/// \file		SplinePointPhysics.h
/// \brief		Declaration of CSplinePointPhysicsObject Class
/// \date		09-10-2005
/// \author		D. Patrick Ghiocel
/// \note		Copyright (c) 2005 Signature Devices, Inc.
///============================================================================

#ifndef __SPLINEPOINTPHYSICSOBJECT_H__
#define __SPLINEPOINTPHYSICSOBJECT_H__

#include "CObjectTemplate.hpp"
#include "Vec3.h"
#include "IPhysObject.h"

class CSplinePointPhysicsObject : public CObjectTemplate<IPhysObject>
{
private:
	/// Default Constructor
	CSplinePointPhysicsObject(IHashString *parentName, IHashString *name);
	
public:
	/// Default Destructor
	~CSplinePointPhysicsObject();

	static IComponent *Create(int nArgs, va_list argptr);

	/// Init
	virtual void Init();
	/// DeInit
	virtual void DeInit();

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	/// \brief	Raises a collision event when this object is collided with
	/// \brief	objCollision = pointer to the colliding object
	virtual void OnCollide( IHashString *objCollision );


private:
	/// Pointed to the EE Tool Box
	IToolBox *m_ToolBox;

	/// Class Type
	CHashString m_hszNameType;
};

#endif //__SPLINEPOINTPHYSICSOBJECT_H__

