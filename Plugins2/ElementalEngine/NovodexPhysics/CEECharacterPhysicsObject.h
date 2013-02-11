///==========================================================================
/// \file	CEECharacterPhysicsObject.h
/// \brief	Uses the PhysX Character controller object to drive character's
///			physics
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

#ifndef	CEECHARACTERPHYSICSOBJECT_H
#define CEECHARACTERPHYSICSOBJECT_H

class CPhysicsManager;

namespace ElementalEngine
{

///==========================================================================
/// \class	CEECharacterPhysicsObject
/// \brief	Uses the PhysX Character controller object to drive character's
///			physics
///==========================================================================
class CEECharacterPhysicsObject : public CPhysicsObject
{
private:
	/// \brief	The Default Constructor for CPhysicsObject
	/// \param	parentName = name of parent in hierarchy
	/// \param	name = IHashString * name of this object
	/// \param	addToHier = Add this object to the hierarchy?
	CEECharacterPhysicsObject( IHashString *parentName, IHashString *name, bool addToHier );

public:
	/// \brief	Destructor
	~CEECharacterPhysicsObject();

	/// \brief	Create the a instance of IObject
	/// \param	nArgs = the number of arguements
	/// \param	argptr = the list of arguements
	/// \return	IComponent pointer
	static IComponent *Create( int nArgs, va_list argptr );

	/// \brief	Initialize the object
	virtual void Init();

	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();

	/// \brief	Deinitializes the object and deallocates
	///			any allocated memory. Resets all internal data.
	virtual void DeInit();

	/// \brief	Serialize this component to or from an archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	/// \brief	Return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// \brief	Checks if this is similar to it's base classes component
	/// \return true if it is the same type, else false
	virtual bool IsKindOf( IHashString *compType );

	// ------------------ Messages ------------------ //

	/// Move a character, the character will collide with any objects
	/// along its path.
	DWORD OnMoveCharacter( DWORD size, void* params );

	/// Set a character's position. The character 'teleport' to the new
	/// location.
	DWORD OnSetCharacterPosition( DWORD size, void* params );

	/// Set a character's height.
	DWORD OnSetCharacterHeight( DWORD size, void* params );

	/// Set a character's width.
	DWORD OnSetCharacterWidth( DWORD size, void* params );

	/// Set a character's maximum velocity.
	DWORD OnSetMaxVelocity( DWORD size, void* params );

	/// Set a character's linear velocity.
	virtual DWORD OnSetLinearVelocity( DWORD size, void* params );

	/// Lock a character's translation on X and Z axies.
	DWORD OnLockHorizontalAxies( DWORD size, void* params );

	// Unlock a character's translation on X and Z axies.
	DWORD OnUnLockHorizontalAxies( DWORD size, void* params );

	/// Get the character's local position.
	DWORD OnGetLocalPosition( DWORD size, void* params );

	/// Set the character's local position
	DWORD OnSetLocalPosition( DWORD size, void* params );

protected:
	/// Set the actor's transform
	virtual void SetTransform();

	/// Update's character state after the main update.
	void PostUpdateCharacterController();

	/// Execute all queued events
	virtual void ExecuteEventQueue();

	MOVECHARACTEREVENT* m_MoveCharacterEvent;
	SETCHARACTERPOSITIONEVENT* m_SetCharacterPositionEvent;
	SETCHARACTERHEIGHTEVENT* m_SetCharacterHeightEvent;
	SETCHARACTERWIDTHEVENT* m_SetCharacterWidthEvent;
	SETLOCALPOSITIONEVENT* m_SetLocalPositionEvent;
	LOCKHORIZONTALAXIESEVENT* m_LockHorizontalAxiesEvent;
	UNLOCKHORIZONTALAXIESEVENT* m_UnLockHorizontalAxiesEvent;

	/// Move a character.
	void MoveCharacter();

	/// Set character's position.
	void SetCharacterPosition();

	/// Set character's height.
	void SetCharacterHeight();

	/// Set character's width.
	void SetCharacterWidth();

	/// Set character's local position
	void SetLocalPosition();

	/// Lock character's X and Z axis translations.
	void LockHorizontalAxies();

	/// Unlock character's X and Z axis translations.
	void UnLockHorizontalAxies();

	/// Enable the effect of gravity on the character.
	virtual void EnableGravity();

	/// Disable the effect of gravity on the character.
	virtual void DisableGravity();

	/// Create the character's continious collision detection skeleton.
	/// \param extents = The extents of the CCD volume.
	/// \param triMesh = The CCD mesh.
	virtual void CreateCCDSkeleton( Vec3 extents, NxSimpleTriangleMesh &triMesh );

private:

	/// Build a capsule based actor for the character's physics.
	void BuildCharacterCapsuleActor();

	/// Build a box based actor for the character's physics.
	void BuildCharacterBoxActor();

	/// Get the parent's OOBB
	/// \param pos = Parent's position.
	/// \param ext = Parent's extents.
	/// \param rot = Parent's orientation.
	void GetParentExtents( Vec3& pos, Vec3& ext, EulerAngle& rot );

	bool m_SweptCollisionDetected;
	NxMat34 oldTransform;
	NxMat34 oldParentTransform;

	bool m_GravityEnable;

	// Capsule or Box
	CHashString m_Shape;

	float m_Mass;

	float m_MaxVelocity;

	float m_HeightAdjustFactor;

	Vec3 m_Extents;
	Vec3 m_Position;
	EulerAngle m_Rotation;

	NxCCDSkeleton* m_CCDSkeleton;
};

}	//namespace ElementalEngine

#endif	// CEECHARACTERPHYSICSOBJECT_H