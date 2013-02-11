///=====================================================================
/// \file	CSceneObject.h
/// \brief	Base class for Scene Object
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

#ifndef _CSCENEOBJECT__H__
#define _CSCENEOBJECT__H__

namespace ElementalEngine
{

typedef list<ISceneObject*> ATTACHMENTOBJECTLIST;

/// SCENEOBJECTTEMPLATE is a #define to define OBJECT type classes
/// \param managerName = The manager name that manages this object
/// \param className = The class name we represent or masquerade as
/// \param parentName = the name of the parent object if any, can be NULL
/// \param name = our unique name in the system
#define SCENEOBJECTTEMPLATE(managerName, className, baseClass, parentName, name) \
	CSceneObject<baseClass>(_T(#managerName), _T(#className), parentName, name)

/// \class CSceneObject
/// \brief	This class defines a base class for objects that exist in the scene
/// ie. entities, render objects, physics objects etc.  
/// They provide a method for entities to update their children directly and a place 
/// where common code involving the updates of transform data can exist.
template<class baseClass>
class CSceneObject : public CObjectTemplate<baseClass>
{
protected:
	CSceneObject(const TCHAR *theManager, const TCHAR *className,
		IHashString *parentName, IHashString *name);

public:
	/// virtual destructor
	virtual ~CSceneObject();

	virtual void SetDirtyTransformFlag();
	virtual void UpdateGlobalObject();
	virtual void UpdateBoundingObject();
	virtual void RemoveBoundingObject();

	/// Gets the attach transform (called from attachment to base)
	/// \param hsNodeName = name of node if specified
	/// \param attachXForm = returned matrix transform
	virtual void GetAttachTransform(IHashString *hsNodeName, Matrix4x4 &attachXForm){}

	/// Sets the attachment info
	/// \param pObject = ISceneObject pointer to base object
	/// \param hsNodeName = name of node if specified
	virtual void SetAttachInfo(ISceneObject* pObject, IHashString* hsNodeName)
	{
		m_pBaseObject = pObject;
		if (hsNodeName)
		{
			m_hsAttachNodeName = hsNodeName;
		}
		else
		{
			m_hsAttachNodeName.Init(_T(""));
		}
	}

	/// Gets the current set base attach object
	/// \return stored pointer of base attach object
	virtual ISceneObject* GetBaseAttachObject(){ return m_pBaseObject; }

	/// Adds an attachment object to the internal list
	/// \param pAttachmentObject = pointer to scene object to insert
	virtual void AttachObject(ISceneObject *pAttachmentObject)
	{
		m_AttachmentList.push_back(pAttachmentObject);

		// This is neccessary to update attach info on first frame of attach.
		SetDirtyTransformFlag();
	}

	/// Removes an attachment object from the list
	/// \param pAttachmentObject = pointer to scene object to remove
	virtual void DetachObject(ISceneObject *pAttachmentObject)
	{
		ATTACHMENTOBJECTLIST::iterator listIter = m_AttachmentList.begin();
		for(; listIter != m_AttachmentList.end(); listIter++)
		{
			if ((*listIter) == pAttachmentObject)
			{
				m_AttachmentList.erase(listIter);
				break;
			}
		}
	}

	virtual IScene* GetScene() const
	{
		return m_pScene;
	}
	virtual ISceneNode* GetSceneNode() const
	{
		return m_pSceneNode;
	}
	virtual void SetSceneNode( IScene *pScene, ISceneNode *pSceneNode )
	{
		m_pScene = pScene;
		m_pSceneNode = pSceneNode;
	}

	virtual void GetBoundingSphere( Vec3 &position, float &radius )
	{
		position.Set( 0,0,0 );
		radius = 0;
	}

protected:
	virtual void SetAttachmentObjectsDirty()
	{
		ATTACHMENTOBJECTLIST::iterator listIter = m_AttachmentList.begin();
		for(; listIter != m_AttachmentList.end(); listIter++)
		{
			ISceneObject *obj = (*listIter);
			if (obj)
			{
				obj->SetDirtyTransformFlag();
			}
		}	
	}

	bool m_bTransformIsDirty;
	CHashString m_hsObjectGroup;
	CHashString m_hsManagerName;	

	ISceneObject *m_pBaseObject;
	CHashString m_hsAttachNodeName;

	ATTACHMENTOBJECTLIST m_AttachmentList;

private:
	IScene *m_pScene;
	ISceneNode *m_pSceneNode;
	DWORD m_iSceneSortID;
};

template<class baseClass>
CSceneObject<baseClass>::CSceneObject(const TCHAR *theManager, const TCHAR *className,
		IHashString *parentName, IHashString *name) : 
	CObjectTemplate<baseClass>(theManager, className, parentName, name)
{
	m_pScene = NULL;
	m_pSceneNode = NULL;
	m_iSceneSortID = 0;
	m_bTransformIsDirty = true;

	ISceneObject *pObject = this;
	static DWORD msgHash_AddChildObject = CHashString(_T("AddChildObject")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_AddChildObject, sizeof(ISceneObject*), &pObject, GetParentName());

	m_hsObjectGroup = _T("CSceneObject");
	m_hsManagerName = _T("CSceneObjectManager");

	m_pBaseObject = NULL;
	m_hsAttachNodeName = _T("");

	// Add to SceneObjectManager
	IComponent *managerComponent = EngineGetToolBox()->CreateComponent(&m_hsManagerName, 0);
	if (managerComponent)
	{
		IManager *manager = dynamic_cast<IManager*>(managerComponent);
		if (manager)
		{
			manager->AddObject(this, &m_hsObjectGroup);
		}
	}	
}

template<class baseClass>
CSceneObject<baseClass>::~CSceneObject()
{
	RemoveBoundingObject();

	ISceneObject *pObject = this;
	static DWORD msgHash_RemoveChildObject = CHashString(_T("RemoveChildObject")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_RemoveChildObject, sizeof(ISceneObject*), &pObject, GetParentName());

	// Remove from SceneObjectManager
	IComponent *managerComponent = EngineGetToolBox()->CreateComponent(&m_hsManagerName, 0);
	if (managerComponent)
	{
		IManager *manager = dynamic_cast<IManager*>(managerComponent);
		if (manager)
		{
			manager->DeleteObject(this);
		}
	}
}

template<class baseClass>
void CSceneObject<baseClass>::SetDirtyTransformFlag()
{
	m_bTransformIsDirty = true;
	SetAttachmentObjectsDirty();
}

template<class baseClass>
void CSceneObject<baseClass>::UpdateGlobalObject()
{
	assert( m_pSceneNode == NULL ); // global object shouldn't be in a scene node

	if (m_pScene == NULL) // if not already in scene
	{
		// object is not yet in a scene so add it
		SCENEOBJECTPARAMS params;
		params.pObject = this;
		params.sortID = GetSceneSortID();
		params.bBoundedObject = false;
		static DWORD msgHash_UpdateBoundingBox = CHashString("UpdateBoundingBox").GetUniqueID();
		m_ToolBox->SendMessage(msgHash_UpdateBoundingBox, sizeof( SCENEOBJECTPARAMS), &params );
	}
}

template<class baseClass>
void CSceneObject<baseClass>::UpdateBoundingObject()
{
	if (m_pScene)
	{
		// only bounded scene objects should be calling UpdateBoundingObject()
		assert( m_pSceneNode != NULL );
		m_pScene->UpdateObject( this );
	}
	else
	{
		m_iSceneSortID = GetSceneSortID();

		// object is not yet in a scene so add it
		SCENEOBJECTPARAMS params;
		params.pObject = this;
		params.sortID = m_iSceneSortID;
		params.bBoundedObject = true;
		static DWORD msgHash_UpdateBoundingBox = CHashString("UpdateBoundingBox").GetUniqueID();
		m_ToolBox->SendMessage(msgHash_UpdateBoundingBox, sizeof( SCENEOBJECTPARAMS), &params );
	}
}

template<class baseClass>
void CSceneObject<baseClass>::RemoveBoundingObject()
{
	if (m_pSceneNode)
	{
		bool bObjectRemoved = m_pSceneNode->RemoveObject( this );
		assert( bObjectRemoved );
		m_pSceneNode = NULL;
		m_pScene = NULL;
	}
	else if (m_pScene)
	{
		// not a bounded object, remove as a global
		m_pScene->RemoveGlobalObject( this );
		m_pScene = NULL;
	}
}

} // namespace ElementalEngine
#endif
