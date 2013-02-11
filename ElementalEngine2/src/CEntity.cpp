///==========================================================================
/// \file	CEntity.cpp
/// \brief	CEntity implementation
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

#include "StdAfx.h"

#include "CEntity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CEntity);

// ------------------ Messages ------------------ //
REGISTER_MESSAGE_HANDLER( GetLocalPosition, OnGetLocalPosition, CEntity );
REGISTER_MESSAGE_HANDLER( GetLocalEuler, OnGetLocalEuler, CEntity );
REGISTER_MESSAGE_HANDLER( GetLocalQuaternion, OnGetLocalQuaternion, CEntity );
REGISTER_MESSAGE_HANDLER( GetLocalScale, OnGetLocalScale, CEntity );
REGISTER_MESSAGE_HANDLER( GetLocalTransform, OnGetLocalTransform, CEntity );

REGISTER_MESSAGE_HANDLER( SetLocalPosition, OnSetLocalPosition, CEntity );
REGISTER_MESSAGE_HANDLER( SetLocalEuler, OnSetLocalEuler, CEntity );
REGISTER_MESSAGE_HANDLER( SetLocalQuaternion, OnSetLocalQuaternion, CEntity );
REGISTER_MESSAGE_HANDLER( SetLocalScale, OnSetLocalScale, CEntity );
REGISTER_MESSAGE_HANDLER( SetLocalTransform, OnSetLocalTransform, CEntity );

REGISTER_MESSAGE_HANDLER( GetGlobalPosition, OnGetGlobalPosition, CEntity );
REGISTER_MESSAGE_HANDLER( GetGlobalEuler, OnGetGlobalEuler, CEntity );
REGISTER_MESSAGE_HANDLER( GetGlobalQuaternion, OnGetGlobalQuaternion, CEntity );
REGISTER_MESSAGE_HANDLER( GetGlobalScale, OnGetGlobalScale, CEntity );
REGISTER_MESSAGE_HANDLER( GetGlobalTransform, OnGetGlobalTransform, CEntity );

REGISTER_MESSAGE_HANDLER( SetGlobalPosition, OnSetGlobalPosition, CEntity );
REGISTER_MESSAGE_HANDLER( SetGlobalEuler, OnSetGlobalEuler, CEntity );
REGISTER_MESSAGE_HANDLER( SetGlobalQuaternion, OnSetGlobalQuaternion, CEntity );
REGISTER_MESSAGE_HANDLER( SetGlobalScale, OnSetGlobalScale, CEntity );
REGISTER_MESSAGE_HANDLER( SetGlobalTransform, OnSetGlobalTransform, CEntity );

REGISTER_MESSAGE_HANDLER(GetEntityType, OnGetEntityType, CEntity);
REGISTER_MESSAGE_HANDLER(GetParentName, OnGetParentName, CEntity); 

REGISTER_MESSAGE_HANDLER(IsEntityTypeOf, OnIsEntityTypeOf, CEntity); 

REGISTER_MESSAGE_HANDLER( GetBoundingRadius, OnGetBoundingRadius, CEntity );

REGISTER_MESSAGE_HANDLER( AddChildObject, OnAddChildObject, CEntity );
REGISTER_MESSAGE_HANDLER( RemoveChildObject, OnRemoveChildObject, CEntity );

REGISTER_MESSAGE_HANDLER( AttachSceneObject, OnAttachSceneObject, CEntity );
REGISTER_MESSAGE_HANDLER( AlignToSceneObject, OnAlignToSceneObject, CEntity );

CEntity::CEntity(IHashString *parentName, IHashString *name, bool bAddToHier)
	: SCENEOBJECTTEMPLATE(CEntityManager, CEntity, ISceneObject, parentName, name), 
	m_hszWorldRootComponentType(_T("CWorld"))
{
	m_bAddToHierarchy = bAddToHier;
	m_wszEntityTypes = "";
	m_bTransformIsDirty = true;
	m_fBoundingRadius = 0.f;	

	if (bAddToHier)
	{
		AddToHierarchy();
	}

	if (parentName)
	{
		static DWORD msgHash_GetComponentType = CHashString(_T("GetComponentType")).GetUniqueID();
		GETCOMPONENTTYPEPARAMS gctp;
		gctp.name = parentName;
		m_ToolBox->SendMessage(msgHash_GetComponentType, sizeof(gctp), &gctp);			

		static DWORD dEntity = CHashString(_T("CEntity")).GetUniqueID();
		if ((gctp.componenttype != NULL) && (gctp.componenttype->GetUniqueID() == dEntity))
		{
			// if the parent is an entity, attach to it!
			ATTACHSCENEOBJECTPARAMS asop;
			asop.hsAttachmentObject = name;
			asop.pAttachmentObject = this;
			asop.hsNodeAttachName = NULL;

			static DWORD msgAttachSceneObject = CHashString(_T("AttachSceneObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgAttachSceneObject, sizeof(ATTACHSCENEOBJECTPARAMS), &asop, parentName);

			// save off a pointer to the parent object
			if( asop.pBaseObject )
			{
				m_pParent = asop.pBaseObject;
			}
		}
	}

	// even if it doesn't attach, we still want to set this guy to true initially
	m_bDefaultAttach = true;
}

CEntity::~CEntity()
{
	DeInit();
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

IComponent *CEntity::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool bAddToHier;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);

	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new CEntity(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CEntity(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CEntity::Init()
{
	UpdateTransform();
}

void CEntity::DeInit()
{
	SCENEOBJECTPARAMS params;
	params.pObject = this;	
	params.sortID = ENTITY;

	RemoveBoundingObject();
}

void CEntity::Serialize( IArchive &ar )
{
	if( &ar == NULL )
	{
		return;
	}

	StdString temp;
	if( ar.IsReading() )
	{

		// Get the data's version number
		CHashString hszVersion = _T("");

		float version;
		static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &hszVersion);
		if (retval != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGERROR, _T("CEntity Serialize: Could not get file version!"));
			assert(0);
			return;
		}
		float atofVersion = (float)_tstof(hszVersion.GetString());

		version = atofVersion;
		
		ar.Read( m_wszEntityTypes );

		if( version >= 3.0 )
		{
			// Read tranform data
			Vec3 spacial;
			EulerAngle euler;
			ar.Read( spacial, _T("Position") ); // position
			m_LocalTransform.SetTranslation( spacial );
			ar.Read( euler, _T("Rotation") ); // rotation
			m_LocalTransform.SetEuler( euler );
			ar.Read( spacial, _T("Scale") ); // scale
			m_LocalTransform.SetScale( spacial );
		}
		else if( version >= 2.0 )
		{
			// 2.5 -> 3.0 - Changed Axis to be Left-Handed X-Right, Y-Up, Z-In
			// Read tranform data
			Vec3 spacial;
			Vec3 tmp;
			EulerAngle euler;
			
			ar.Read( spacial, _T("Position") ); // position
			tmp.Set( spacial.x, spacial.z, -spacial.y );
			m_LocalTransform.SetTranslation( tmp );

			ar.Read( spacial, _T("Rotation") ); // rotation
			euler.Set( -spacial.x, -spacial.y, -spacial.z );
            m_LocalTransform.SetEuler( euler );
			
			ar.Read( spacial, _T("Scale") ); // scale
			tmp.Set( spacial.x, spacial.z, spacial.y );
			m_LocalTransform.SetScale( tmp );
		}

		// whenever we serialize in data, we need to update the transforms
		SetDirtyTransformFlag();
	}
	else
	{
		ar.Write( m_wszEntityTypes, _T("EntityTypes") );
		// Write transform data
		Vec3 spacial;
		EulerAngle euler;
		spacial = m_LocalTransform.GetTranslation();
		ar.Write( spacial, _T("Position") ); // position
		euler = m_LocalTransform.GetEuler();
		ar.Write( euler, _T("Rotation") ); // rotation
		spacial = m_LocalTransform.GetScale();
		ar.Write( spacial, _T("Scale") ); // scale
	}
}

StdString CEntity::GetEntityTypes( void )
{
	return m_wszEntityTypes;
}

void CEntity::SetEntityTypes( StdString wszEntityTypes )
{
	m_wszEntityTypes = wszEntityTypes;
}

bool CEntity::IsEntityTypeOf( StdString wszEntityType )
{
	const TCHAR* result = _tcsstr( m_wszEntityTypes, wszEntityType );

	if( result != NULL )
	{
		return true;
	}
	else
	{	
		return false;
	}
}

DWORD CEntity::OnGetLocalPosition( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* position = (Vec3*)params;

	position->Set( m_LocalTransform.GetTranslation() );

	return MSG_HANDLED_STOP; 
}

DWORD CEntity::OnGetLocalEuler( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(EulerAngle) );
	EulerAngle* euler = (EulerAngle*)params;

	euler->Set( m_LocalTransform.GetEuler() );

	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnGetLocalQuaternion( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Quaternion) );
	Quaternion* quat = (Quaternion*)params;

	quat->Set( m_LocalTransform.GetQuaternion() );

	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnGetLocalScale( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* scale = (Vec3*)params;

	scale->Set( m_LocalTransform.GetScale() );

	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnGetLocalTransform( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Matrix4x4) );
	Matrix4x4* transform = (Matrix4x4*)params;

	transform->SetFrom4x4( m_LocalTransform.GetTransform().m );

	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnSetLocalPosition( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* position = (Vec3*)params;

	m_LocalTransform.SetTranslation( *position );

	// Since we are setting the local position the global position is now dirty
	SetDirtyTransformFlag();

	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnSetLocalEuler( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(EulerAngle) );
	EulerAngle* euler = (EulerAngle*)params;

	m_LocalTransform.SetEuler( *euler );

	// Since we are setting the local position the global position is now dirty
	SetDirtyTransformFlag();

	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnSetLocalQuaternion( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Quaternion) );
	Quaternion* quaternion = (Quaternion*)params;

	m_LocalTransform.SetQuaternion( *quaternion );

	// Since we are setting the local position the global position is now dirty
	SetDirtyTransformFlag();

	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnSetLocalScale( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* scale = (Vec3*)params;

	m_LocalTransform.SetScale( *scale );

	// Since we are setting the local position the global position is now dirty
	SetDirtyTransformFlag();

	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnSetLocalTransform( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Matrix4x4) );
	Matrix4x4* transform = (Matrix4x4*)params;

	m_LocalTransform.SetTransform( *transform );

	// Since we are setting the local position the global position is now dirty
	SetDirtyTransformFlag();

	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnGetGlobalPosition( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* position = (Vec3*)params;

	position->Set( m_GlobalTransform.GetTranslation() );
	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnGetGlobalEuler( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(EulerAngle) );
	EulerAngle* euler = (EulerAngle*)params;

	euler->Set( m_GlobalTransform.GetEuler() );
	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnGetGlobalQuaternion( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Quaternion) );
	Quaternion* quaternion = (Quaternion*)params;

	quaternion->Set( m_GlobalTransform.GetQuaternion() );
	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnGetGlobalScale( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* scale = (Vec3*)params;

	scale->Set( m_GlobalTransform.GetScale() );
	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnGetGlobalTransform( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Matrix4x4) );
	Matrix4x4* transform = (Matrix4x4*)params;

	transform->SetFrom4x4( m_GlobalTransform.GetTransform().m );
	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnSetGlobalPosition( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* newGlobalPosition = (Vec3*)params;

	if (!newGlobalPosition->IsValid())
	{
		m_ToolBox->Log(LOGWARNING, _T("CEntity::OnSetGlobalPosition received invalid number in one of the components: %f, %f, %f\n"),
				newGlobalPosition->x, newGlobalPosition->y, newGlobalPosition->z);
		return MSG_NOT_HANDLED;
	}
	
	// Get the parent's global transform
	Matrix4x4 parentGlobalTransform;

	if (m_pBaseObject)
	{
		m_pBaseObject->GetAttachTransform(&m_hsAttachNodeName, parentGlobalTransform);
	}

	// local position = parent's global - new global
	Matrix4x4 parentGlobalTransformInverse;
	parentGlobalTransform.GetInverse( parentGlobalTransformInverse );

	Vec3 newLocal = parentGlobalTransformInverse * *newGlobalPosition;

	// Set the local transform
	m_LocalTransform.SetTranslation( newLocal );
	// Set the global translation

	m_GlobalTransform.SetTranslation( *newGlobalPosition );

	SetChildrenDirtyTransformFlags();

	m_bTransformIsDirty = false;

	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnSetGlobalEuler( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(EulerAngle) );
	EulerAngle* newGlobalEuler = (EulerAngle*)params;

	if (!newGlobalEuler->IsValid())
	{
		m_ToolBox->Log(LOGWARNING, _T("CEntity::OnSetGlobalPosition received invalid number in one of the components: %f, %f, %f\n"),
				newGlobalEuler->GetRoll(), newGlobalEuler->GetPitch(), newGlobalEuler->GetYaw());
		return MSG_NOT_HANDLED;
	}
	
	// Get the parent's global euler
	EulerAngle parentGlobalEuler;

	if (m_pBaseObject)
	{
		Matrix4x4 parentXForm;
		m_pBaseObject->GetAttachTransform(&m_hsAttachNodeName, parentXForm);
		parentGlobalEuler.Set(parentXForm.GetRotation());
	}

	// local position = parent's global - new global
	EulerAngle newLocal = *newGlobalEuler - parentGlobalEuler;

	// Set the transform
	m_LocalTransform.SetEuler( newLocal );
	// Set the global euler

	m_GlobalTransform.SetEuler( *newGlobalEuler );
	
	SetChildrenDirtyTransformFlags();

	m_bTransformIsDirty = false;

	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnSetGlobalQuaternion( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Quaternion) );
	Quaternion* newGlobalQuaternion = (Quaternion*)params;

	if (!newGlobalQuaternion->IsValid())
	{
		m_ToolBox->Log(LOGWARNING, _T("CEntity::OnSetGlobalPosition received invalid number in one of the components: %f, %f, %f\n"),
				newGlobalQuaternion->x, newGlobalQuaternion->y, newGlobalQuaternion->z, newGlobalQuaternion->w);
		return MSG_NOT_HANDLED;
	}
	
	// Get the parent's global euler
	Quaternion parentGlobalQuaternion;

	if (m_pBaseObject)
	{
		Matrix4x4 parentXForm;
		m_pBaseObject->GetAttachTransform(&m_hsAttachNodeName, parentXForm);
		parentGlobalQuaternion.SetFromMatrix(parentXForm);
	}

	// local position = parent's global - new global
	Quaternion newLocal = *newGlobalQuaternion / parentGlobalQuaternion;

	// Set the transform
	m_LocalTransform.SetQuaternion( newLocal );
	// Set the global quaternion

	m_GlobalTransform.SetQuaternion( *newGlobalQuaternion );
	
	SetChildrenDirtyTransformFlags();

	m_bTransformIsDirty = false;

	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnSetGlobalScale( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* newGlobalScale = (Vec3*)params;

	if (!newGlobalScale->IsValid())
	{
		m_ToolBox->Log(LOGWARNING, _T("CEntity::OnSetGlobalPosition received invalid number in one of the components: %f, %f, %f\n"),
				newGlobalScale->x, newGlobalScale->y, newGlobalScale->z);
		return MSG_NOT_HANDLED;
	}

	// Get the parent's global euler
	Vec3 parentGlobalScale(1.0,1.0,1.0);	// Default Scale of 1.0,1.0,1.0, should never be zero.

	if (m_pBaseObject)
	{
		Matrix4x4 parentXForm;
		m_pBaseObject->GetAttachTransform(&m_hsAttachNodeName, parentXForm);
		parentGlobalScale.Set(parentXForm.GetScale());
	}

	// local position = parent's global - new global
	Vec3 newLocal = *newGlobalScale / parentGlobalScale;

	// Set the transform
	m_LocalTransform.SetScale( newLocal );
	// Set the global scale

	m_GlobalTransform.SetScale( *newGlobalScale );
	
	SetChildrenDirtyTransformFlags();

	m_bTransformIsDirty = false;

	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnSetGlobalTransform( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Matrix4x4) );
	Matrix4x4* newGlobalTransform = (Matrix4x4*)params;
	
	// Get the parent's global euler
	Matrix4x4 parentGlobalTransform;

	if (m_pBaseObject)
	{
		m_pBaseObject->GetAttachTransform(&m_hsAttachNodeName, parentGlobalTransform);
	}

	// multiply by invers of parent's global
	// local position = parent's global - new global
	Matrix4x4 parentGlobalTransformInverse;
	parentGlobalTransform.GetInverse( parentGlobalTransformInverse );
	Matrix4x4 newLocal = *newGlobalTransform * parentGlobalTransformInverse;

	// Set the transform
	m_LocalTransform.SetTransform( newLocal );

	m_GlobalTransform.SetTransform( *newGlobalTransform );

	SetChildrenDirtyTransformFlags();

	m_bTransformIsDirty = false;

	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnGetEntityType(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));
	IHashString *data = (IHashString*) param;
	*data = GetEntityTypes();
	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnGetParentName(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));
	IHashString *data = (IHashString*) param;
	IHashString *pName = GetParentName();
	if (pName)
	{
		*data = *pName;
	}
	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnIsEntityTypeOf(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ISENTITYOFTYPEPARAMS));
	ISENTITYOFTYPEPARAMS *data = (ISENTITYOFTYPEPARAMS*) param;
	data->bResult = IsEntityTypeOf( StdString(data->entityType->GetString()) );
	return MSG_HANDLED_STOP;
}

void CEntity::UpdateTransform()
{
	if (m_bTransformIsDirty)
	{
		Matrix4x4 attachMat;
		if (m_pBaseObject)
		{
			m_pBaseObject->GetAttachTransform(&m_hsAttachNodeName, attachMat);
		}

		Matrix4x4 newMat = attachMat * m_LocalTransform.GetTransform();
		m_GlobalTransform.SetTransform(newMat);

		SetChildrenDirtyTransformFlags();
		m_bTransformIsDirty = false;
	}
}

void CEntity::SetChildrenDirtyTransformFlags()
{
	SCENEOBJECTLIST::iterator itrChild = m_Children.begin();
	while (itrChild != m_Children.end())
	{
		ISceneObject *pObject = *itrChild;
		pObject->SetDirtyTransformFlag();
		itrChild++;
	}

	// set attach object dirty transform too!
	SetAttachmentObjectsDirty();
}

void CEntity::SetDirtyTransformFlag()
{
	if (!m_bTransformIsDirty)
	{
		m_bTransformIsDirty = true;

		// if this gets dirty, it's children get dirty
		SetChildrenDirtyTransformFlags();
	}
}

bool CEntity::IsParentWorldRoot()
{
	// Get the parent's name
	IHashString* parentName = GetParentName();
	GETCOMPONENTTYPEPARAMS gctp;
	gctp.name = parentName;
	gctp.componenttype = NULL;
	static DWORD msgHash_GetComponentType = CHashString(_T("GetComponentType")).GetUniqueID();
	DWORD returnValue = m_ToolBox->SendMessage(msgHash_GetComponentType, sizeof(gctp), &gctp );
	if( returnValue != MSG_HANDLED || gctp.componenttype == NULL )
	{
		m_ToolBox->Log(LOGERROR, _T("CEntity IsParentWorldRoot: Could not get component type of CEntity parent \"%s\"!\n"), parentName->GetString() );
		return true;
	}
	return (*gctp.componenttype == m_hszWorldRootComponentType);
}

DWORD CEntity::OnGetBoundingRadius(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float *outVal = (float*) param;

	*outVal = m_fBoundingRadius;
	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnAddChildObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ISceneObject*));
	ISceneObject *pObject = *(ISceneObject**)param;

	m_Children.push_back( pObject );

	return MSG_HANDLED_STOP;
}

DWORD CEntity::OnRemoveChildObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ISceneObject*));
	ISceneObject *pObject = *(ISceneObject**)param;

	m_Children.remove( pObject );

	return MSG_HANDLED_STOP;
}

void CEntity::SetParentName(IHashString *inName)
{
	if (inName != NULL)
	{
		m_ParentName = inName;
		
		if (m_bDefaultAttach)
		{
			// if the attachment is still the default entity relationship, 
			// we need to attach to the new parent
			static DWORD msgHash_GetComponentType = CHashString(_T("GetComponentType")).GetUniqueID();
			GETCOMPONENTTYPEPARAMS gctp;
			gctp.name = &m_ParentName;
			m_ToolBox->SendMessage(msgHash_GetComponentType, sizeof(gctp), &gctp);			

			static DWORD dEntity = CHashString(_T("CEntity")).GetUniqueID();
			if ((gctp.componenttype != NULL) && (gctp.componenttype->GetUniqueID() == dEntity))
			{
				// if the parent is an entity, attach to it!
				ATTACHSCENEOBJECTPARAMS asop;
				asop.hsAttachmentObject = GetName();
				asop.pAttachmentObject = this;
				asop.hsNodeAttachName = NULL;

				static DWORD msgAttachSceneObject = CHashString(_T("AttachSceneObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgAttachSceneObject, sizeof(ATTACHSCENEOBJECTPARAMS), &asop, &m_ParentName);

				// save off a pointer to the parent object
				if( asop.pBaseObject )
				{
					m_pParent = asop.pBaseObject;
				}
			}
			// be sure to detach!
			else
			{
				static DWORD msgDetachSceneObject = CHashString(_T("DetachSceneObject")).GetUniqueID();
				m_ToolBox->SendMessage( msgDetachSceneObject, sizeof(IHashString*), GetName() );
			}

			// keep this true
			m_bDefaultAttach = true;
		}

		SetDirtyTransformFlag();
	}	
}
DWORD CEntity::OnAlignToSceneObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ATTACHSCENEOBJECTPARAMS));
	ATTACHSCENEOBJECTPARAMS* asop = (ATTACHSCENEOBJECTPARAMS*)param;
	if (asop->hsAttachmentObject)
	{
		// if node is specified, handle in another message (HRC)
		if (asop->hsNodeAttachName != NULL)
		{
			return MSG_NOT_HANDLED;
		}
		// if node is not specified, handle here.
		else
		{
			Matrix4x4 transform = m_GlobalTransform.GetTransform();
			static DWORD msgHash_SetGlobalTransform = CHashString(_T("SetGlobalTransform")).GetUniqueID();
			static CHashString entName(_T("CEntity"));
			m_ToolBox->SendMessage(msgHash_SetGlobalTransform, sizeof(Matrix4x4), &transform, asop->hsAttachmentObject, &entName);
			return MSG_HANDLED_STOP;
		}
	}
	return MSG_ERROR;
}
DWORD CEntity::OnAttachSceneObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ATTACHSCENEOBJECTPARAMS));
	ATTACHSCENEOBJECTPARAMS* asop = (ATTACHSCENEOBJECTPARAMS*)param;
	if (asop->hsAttachmentObject)
	{
		// if a node is specified, entities don't handle the message
		// otherwise...
		if (asop->hsNodeAttachName == NULL)
		{
			static DWORD msgRegisterSceneAttachObjects = CHashString(_T("RegisterSceneAttachObjects")).GetUniqueID();
			asop->hsBaseObject = GetName();
			m_ToolBox->SendMessage(msgRegisterSceneAttachObjects, sizeof(ATTACHSCENEOBJECTPARAMS), asop);	
			//m_ToolBox->Log(LOGINFORMATION, _T("Attach %s ==> %s\n"), asop->hsAttachmentObject->GetString(), asop->hsBaseObject->GetString()); 
			asop->pBaseObject = this;
			SetDirtyTransformFlag();
			return MSG_HANDLED_STOP;
		}
		else
		{
			return MSG_NOT_HANDLED;
		}
	}
	return MSG_ERROR;
}

void CEntity::GetAttachTransform(IHashString *hsNodeName, Matrix4x4 &attachXForm)
{
	attachXForm.SetFrom4x4(m_GlobalTransform.GetTransform().GetMatrix());
}

void CEntity::SetAttachInfo(ISceneObject* pObject, IHashString* hsNodeName)
{
	CSceneObject<ISceneObject>::SetAttachInfo(pObject, hsNodeName);

	m_bDefaultAttach = false;
}