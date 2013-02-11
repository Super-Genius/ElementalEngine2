///==========================================================================
/// \file	ISceneNode.h
/// \brief	SceneNode Interface
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
///==========================================================================

#ifndef	ISCENENODE_H
#define	ISCENENODE_H

namespace ElementalEngine
{

typedef map< DWORD, ISceneObject* > IDSCENEOBJECTMAP;
typedef map< DWORD, IDSCENEOBJECTMAP > SORTIDOBJECTMAP;

class ISceneNode : public IObject
{
public:
	virtual ~ISceneNode() { }

	/// Gets the list of objects attached to the scene node.
	/// \param sortID = The sortID of the desired objects.
	/// \return The list of objects with the specified sortID.
	virtual IDSCENEOBJECTMAP* GetObjSet( DWORD sortID ) = 0;

	/// Get the scene node's AABB
	/// \param min = The scene node's AABB min.
	/// \param max = The scene node's AABB max.
	virtual void GetBoundingAABB( Vec3& min, Vec3& max ) = 0;

	/// Get the scene node's bounding sphere
	/// \param position = The scene node's bounding sphere position.
	/// \param radius = The scene node's bounding sphere radius.
	virtual void GetBoundingSphere( Vec3& position, float& radius ) = 0;

	/// Get the scene node's parent scene node.
	/// \return The scene node's parent scene node.
	virtual ISceneNode* GetParent() = 0;

	/// Get the child scene node at the specified index.
	/// \param index = The index of the desired child.
	/// \return The child scene node at the specified index.
	virtual ISceneNode* GetChild( int index ) = 0;

	// Update an ISceneObject, adds object if not currently in scene.
	/// \param object = A scene object.
	/// \return The octnode the object was added to.
	virtual ISceneNode* UpdateSceneObject( ISceneObject* object ) = 0;

	/// Remove an object from the node
	/// \param object = The object to remove.
	/// \param sortID = The object's sort id.
	/// \return bool = returns true if successfully removed
	virtual bool RemoveObject( ISceneObject* object ) = 0;

	virtual DWORD GetUniqueID() const = 0;
};

} //namespace ElementalEngine

#endif	// ISCENENODE_H