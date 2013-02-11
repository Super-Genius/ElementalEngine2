///==========================================================================
/// \file	CEntity.h
/// \brief	Fundamental game entity class
///			The entity is constructed from multiple IObjects
/// \date	2-22-2005
/// \author Kyle Swaim
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

#ifndef	CENTITY_H
#define	CENTITY_H

///==========================================================================
/// \class	CEntity
/// \brief	Entity implementation of IObject interface
///==========================================================================
class CEntity : public CSceneObject<ISceneObject>
{
private:
	CHashString m_hszWorldRootComponentType;

	/// \brief The Default Constructor for CEntity
	CEntity(IHashString *parentName, IHashString *name, bool bAddToHier);

	/// \brief Specifies the Types the Entity fall into
	StdString m_wszEntityTypes;

	bool m_bAddToHierarchy;

	/// Spacial Data
	CTransform m_LocalTransform;
	CTransform m_GlobalTransform;
	float m_fBoundingRadius;

	ISceneObject* m_pParent;

	typedef list<ISceneObject*> SCENEOBJECTLIST;
	SCENEOBJECTLIST m_Children;

	void SetChildrenDirtyTransformFlags();
	virtual void SetDirtyTransformFlag();

	bool m_bDefaultAttach;

public:
	/// \brief	The Default Destructor for CEntity
	~CEntity();

	/// \brief	Create the a instance of IObject
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The DeInit Function for the Object
	virtual void DeInit();

	virtual void Init();

	/// \brief either reads or writes the entity information to a file
	/// \param ar - Archiver
	virtual void Serialize(IArchive &ar);

	virtual OBJECTSORTTYPES GetSceneSortID() const { return ENTITY; }

	/// \brief Returns the Entity Types
	/// \return EntityTypes
	StdString GetEntityTypes( void );

	/// \brief Sets the Entity Types
	/// \param wszEntityTypes - EntityTypes String to set
	void SetEntityTypes( StdString wszEntityTypes );

	/// \brief Checks to see if the suppled string is part of the "EntityTypes" qualifications
	/// \param wszEntityType - EntityType to check
	/// \return True if EntityType is in m_wszEntityTypes, else False
	bool IsEntityTypeOf( StdString wszEntityType );

	virtual void GetBoundingSphere( Vec3 &position, float &radius )
	{
		OnGetGlobalPosition(sizeof(Vec3), &position);
		radius = m_fBoundingRadius;
	}

	bool IsParentWorldRoot();

	/// Set the parent name for this object
	/// \param inName = IHashString name for parent
	virtual void SetParentName(IHashString *inName);	

	/// Updates the attachment object
	virtual void UpdateTransform();

	/// Gets the attach transform (called from attachment to base)
	/// \param hsNodeName = name of node if specified
	/// \param attachXForm = returned matrix transform
	virtual void GetAttachTransform(IHashString *hsNodeName, Matrix4x4 &attachXForm);

	/// Sets the attachment info
	/// \param pObject = ISceneObject pointer to base object
	/// \param hsNodeName = name of node if specified
	virtual void SetAttachInfo(ISceneObject* pObject, IHashString* hsNodeName);

	// ------------------ Messages ------------------ //
	DWORD OnGetLocalPosition( DWORD size, void* params );
	DWORD OnGetLocalEuler( DWORD size, void* params );
	DWORD OnGetLocalQuaternion( DWORD size, void* params );
	DWORD OnGetLocalScale( DWORD size, void* params );
	DWORD OnGetLocalTransform( DWORD size, void* params );

	DWORD OnSetLocalPosition( DWORD size, void* params );
	DWORD OnSetLocalEuler( DWORD size, void* params );
	DWORD OnSetLocalQuaternion( DWORD size, void* params );
	DWORD OnSetLocalScale( DWORD size, void* params );
	DWORD OnSetLocalTransform( DWORD size, void* params );

	DWORD OnGetGlobalPosition( DWORD size, void* params );
	DWORD OnGetGlobalEuler( DWORD size, void* params );
	DWORD OnGetGlobalQuaternion( DWORD size, void* params );
	DWORD OnGetGlobalScale( DWORD size, void* params );
	DWORD OnGetGlobalTransform( DWORD size, void* params );

	DWORD OnSetGlobalPosition( DWORD size, void* params );
	DWORD OnSetGlobalEuler( DWORD size, void* params );
	DWORD OnSetGlobalQuaternion( DWORD size, void* params );
	DWORD OnSetGlobalScale( DWORD size, void* params );
	DWORD OnSetGlobalTransform( DWORD size, void* params );

	/// Gets the entity type, stored as IHashString
	/// \param size = sizeof(IHashString)
	/// \param param = IHashString to store eType
	/// \return MSG_HANDLED_STOP
	DWORD OnGetEntityType(DWORD size, void *param);

	/// Gets the parent name of this entity
	/// \param size = sizeof(IHashString)
	/// \param param = IHashString to store parent name
	/// \return MSG_HANDLED_STOP
	DWORD OnGetParentName(DWORD size, void *param);

	DWORD OnIsEntityTypeOf(DWORD size, void *param);

	DWORD OnGetBoundingRadius(DWORD size, void *param);

	DWORD OnAddChildObject(DWORD size, void *param);
	DWORD OnRemoveChildObject(DWORD size, void *param);

	DWORD OnAttachSceneObject(DWORD size, void *param);
	DWORD OnAlignToSceneObject(DWORD size, void *param);
};

#endif	// CENTITY_H