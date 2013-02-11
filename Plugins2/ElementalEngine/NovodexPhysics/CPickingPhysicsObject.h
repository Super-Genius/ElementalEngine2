///==========================================================================
/// \file	CPickingPhysicsObject.h
/// \brief	An OOBB used for picking objects
/// \date	2/21/2007
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

#ifndef	CPICKINGPHYSICSOBJECT_H
#define CPICKINGPHYSICSOBJECT_H

class CPhysicsManager;

namespace ElementalEngine
{

///==========================================================================
/// \class	CPickingPhysicsObject
/// \brief	An OOBB used for picking objects
///==========================================================================
class CPickingPhysicsObject : public CPhysicsObject
{
protected:
	/// The Default Constructor for CPhysicsObject
	/// \param	parentName = name of parent in hierarchy
	/// \param	name = IHashString * name of this object
	/// \param	addToHier = Add this object to the hierarchy?
	CPickingPhysicsObject( IHashString *parentName, IHashString *name, bool addToHier );

public:
	/// Destructor
	~CPickingPhysicsObject();

	/// Create the a instance of IObject
	/// \param	nArgs = the number of arguements
	/// \param	argptr = the list of arguements
	/// \return	IComponent pointer
	static IComponent *Create( int nArgs, va_list argptr );

	/// Initialize the object
	virtual void Init();

	/// The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();

	/// Deinitializes the object and deallocates
	///	any allocated memory. Resets all internal data.
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
	/// Build the picing box.
	void BuildBox();

	Vec3 m_Extents;
	Vec3 m_Position;
};

}	//namespace ElementalEngine

#endif	// CPICKINGPHYSICSOBJECT_H