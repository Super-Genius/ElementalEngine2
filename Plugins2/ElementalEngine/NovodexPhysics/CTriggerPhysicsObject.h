///==========================================================================
/// \file	CTriggerPhysicsObject.h
/// \brief	Trigger volume physics object
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

#ifndef	CTRIGGERPHYSICSOBJECT_H
#define CTRIGGERPHYSICSOBJECT_H

namespace ElementalEngine
{
#define SAFEDELETE( arg ) if( arg ){ delete arg; arg = NULL; }
#define SAFEARRAYDELETE( arg ) if ( arg ){ delete[] arg; arg = NULL; }

///==========================================================================
/// \class	CTriggerPhysicsObject
/// \brief	Trigger volume physics object
///==========================================================================
class CTriggerPhysicsObject : public CPhysicsObject
{
private:
	/// Constructor
	/// \param	parentName = name of parent in hierarchy
	/// \param	name = IHashString * name of this object
	/// \param	addToHier = Add this object to the hierarchy?
	CTriggerPhysicsObject( IHashString *parentName, IHashString *name, bool addToHier );

public:
	///	Destructor
	~CTriggerPhysicsObject();

	/// Create the a instance of IObject
	/// \return	IComponent pointer
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
	/// Set the transform from the parent entity.
	virtual void SetTransform();

	/// Grab the parent's vertex data to generate the trigger volume.
	virtual void PopulateVertexData();

	/// Build the trigger actor
	/// \return A pointer to a PhysX actor representing the trigger volume
	void BuildTriggerActor();

	bool m_TriggerOnEntry;
	bool m_TriggerOnStay;
	bool m_TriggerOnLeave;

	// Mesh data
	int m_NumVerts;
	int m_NumFaces;
	Vec3* m_Verts;
	int* m_Faces;

	bool m_isMovable;
};

}

#endif //CTRIGGERPHYSICSOBJECT_H