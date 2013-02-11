///==========================================================================
/// \file	CCoordinateToolPhysicsObject.h
/// \brief	An OOBB used for picking coordinate tool axes
/// \date	11/07/2007
/// \author	Dmitriy Sergeyev
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

#ifndef	CCOORDINATETOOLPHYSICSOBJECT_H
#define CCOORDINATETOOLPHYSICSOBJECT_H

class CPhysicsManager;

namespace ElementalEngine
{

///==========================================================================
/// \class	CCoordinateToolPhysicsObject
/// \brief	An OOBB used for picking coordinate tool axes
///==========================================================================
class CCoordinateToolPhysicsObject : public CPickingPhysicsObject
{
private:
	/// The Default Constructor for CCoordinateToolPhysicsObject. Cannot be
	/// added to hieararchy.
	/// \param	parentName = name of parent in hierarchy
	/// \param	name = IHashString * name of this object
	CCoordinateToolPhysicsObject( IHashString *parentName, IHashString *name );

public:
	/// Destructor
	~CCoordinateToolPhysicsObject();

	/// Create the a instance of IComponent
	/// \param	nArgs = the number of arguements
	/// \param	argptr = the list of arguements
	/// \return	IComponent pointer
	static IComponent *Create( int nArgs, va_list argptr );

	/// Return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is the same type, else false
	virtual bool IsKindOf( IHashString *compType );

protected:
	/// Get the parent's transform
	virtual void GetParentTransform( NxMat34& transform );
};

}	//namespace ElementalEngine

#endif	// CCOORDINATETOOLPHYSICSOBJECT_H
