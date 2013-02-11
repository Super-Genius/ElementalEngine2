///=====================================================================
/// \file	ISceneObject.h
/// \brief	Interface class for Scene Object
/// \date	07/26/2007
/// \author	Brian Bazyk
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
///=====================================================================

#ifndef _ISCENEOBJECT__H__
#define _ISCENEOBJECT__H__

namespace ElementalEngine
{

class IScene;
class ISceneNode;

enum OBJECTSORTTYPES{ RENDEROBJECT3D, LIGHTOBJECT3D, SOUNDOBJECT2D, SOUNDOBJECT3D, GUIOBJECT, ENTITY, CAMERASORTTYPE, OBJECTSORTTYPECOUNT/*must be last*/ };

class ISceneObject : public IObject
{
public:
	virtual void SetDirtyTransformFlag() {}
	
	/// Updates the attachment object
	virtual void UpdateTransform() {}

	/// Gets the attach transform (called from attachment to base)
	/// \param hsNodeName = name of node if specified
	/// \param attachXForm = returned matrix transform
	virtual void GetAttachTransform(IHashString *hsNodeName, Matrix4x4 &attachXForm){}

	/// Sets the attachment info
	/// \param pObject = ISceneObject pointer to base object
	/// \param hsNodeName = name of node if specified
	virtual void SetAttachInfo(ISceneObject* pObject, IHashString* hsNodeName){}

	/// Gets the current set base attach object
	/// \return stored pointer of base attach object
	virtual ISceneObject* GetBaseAttachObject(){ return NULL; }

	/// Adds an attachment object to the internal list
	/// \param pAttachmentObject = pointer to scene object to insert
	virtual void AttachObject(ISceneObject *pAttachmentObject){}

	/// Removes an attachment object from the list
	/// \param pAttachmentObject = pointer to scene object to remove
	virtual void DetachObject(ISceneObject *pAttachmentObject){}	

	virtual IScene* GetScene() const = 0;
	virtual ISceneNode* GetSceneNode() const = 0;

	virtual void SetSceneNode( IScene *pScene, ISceneNode *pSceneNode ) = 0;

	/// Updates Global Object or adds it if not in the Scene
	virtual void UpdateGlobalObject() = 0;

	/// Updates Bounding Box Object or adds it if not in the Scene
	virtual void UpdateBoundingObject() = 0;

	/// Removes Bounding Box Object from the Scene
	virtual void RemoveBoundingObject() = 0;

	/// return the scene sort ID of the scene object (ie. RENDEROBJECT3D)
	virtual OBJECTSORTTYPES GetSceneSortID() const = 0;

	/// Gets the bounding sphere of the object
	virtual void GetBoundingSphere( Vec3 &position, float &radius ) = 0;
};

} // namespace ElementalEngine

#endif