///==========================================================================
/// \file	CHeightfieldPhysicsObject.h
/// \brief	Physics object for heightfields
/// \date	3/29/2007
/// \author	Kyle Swaim
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

#ifndef	CHEIGHTFIELDPHYSICSOBJECT_H
#define CHEIGHTFIELDPHYSICSOBJECT_H

namespace ElementalEngine
{

///==========================================================================
/// \class	CHeightfieldPhysicsObject
/// \brief	Physics object for heightfields
///==========================================================================
class CHeightfieldPhysicsObject : public CPhysicsObject
{
private:
	/// Constructor
	/// \param	parentName = name of parent in hierarchy
	/// \param	name = IHashString * name of this object
	/// \param	addToHier = Add this object to the hierarchy?
	CHeightfieldPhysicsObject( IHashString *parentName, IHashString *name, bool addToHier );

public:
	///	Destructor
	~CHeightfieldPhysicsObject();

	/// Create the a instance of IObject
	/// \return	IComponent pointer
	/// \param nArgs = Number of arguements.
	/// \param va_list = List of arguements.
	/// \return The new component.
	static IComponent *Create( int nArgs, va_list argptr );

	/// Initialize the object
	virtual void Init();

	/// Update the object
	virtual bool Update();

	/// Deinitialize the object
	virtual void DeInit();

	/// Serialize this component to or from an archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	/// Return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is the same type, else false
	virtual bool IsKindOf( IHashString *compType );

protected:
	/// Build terrain heightfield
	/// \return A pointer to a PhysX actor representing the heightfield
	void BuildHeightfield();

	/// Set the actor's transform
	virtual void SetTransform();
};

}

#endif //CHEIGHTFIELDPHYSICSOBJECT_H