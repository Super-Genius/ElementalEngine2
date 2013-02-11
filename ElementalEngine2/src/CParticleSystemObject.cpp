///============================================================================
/// \file	CParticleSystemObject.cpp
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
///============================================================================

#include "StdAfx.h"
#include "CParticleEmitterObject.h"
#include "CParticleSystemObject.h"


REGISTER_COMPONENT(CParticleSystemObject);
REGISTER_MESSAGE_HANDLER( RegisterChildEmitter, OnRegisterChildEmitter, CParticleSystemObject );
REGISTER_MESSAGE_HANDLER( DeRegisterChildEmitter, OnDeRegisterChildEmitter, CParticleSystemObject );
REGISTER_MESSAGE_HANDLER( AttachSceneObject, OnAttachSceneObject, CParticleSystemObject );
REGISTER_MESSAGE_HANDLER( AddChildObject, OnAddChildObject, CParticleSystemObject );
REGISTER_MESSAGE_HANDLER( RemoveChildObject, OnRemoveChildObject, CParticleSystemObject );

CParticleSystemObject::CParticleSystemObject( IHashString *parentName, IHashString *name ) :
	SCENEOBJECTTEMPLATE( CParticleManager, CParticleSystemObject, IRenderObject, parentName, name )
{
	m_bAddToHierarchy = true;
	m_bQueueSelfDelete = false;
	if (m_bAddToHierarchy)
		AddToHierarchy();
	m_bTransformIsDirty = false;
	if (parentName)
	{
		static DWORD msgHash_GetComponentType = CHashString(_T("GetComponentType")).GetUniqueID();
		GETCOMPONENTTYPEPARAMS gctp;
		gctp.name = parentName;
		m_ToolBox->SendMessage(msgHash_GetComponentType, sizeof(gctp), &gctp);			

		static DWORD dEntity = CHashString(_T("CEntity")).GetUniqueID();
		if ((gctp.componenttype != NULL) && (gctp.componenttype->GetUniqueID() == dEntity))
		{
			// if the parent is an CEntity, attach to it!
			ATTACHSCENEOBJECTPARAMS asop;
			asop.hsAttachmentObject = name;
			asop.hsNodeAttachName = NULL;

			static DWORD msgAttachSceneObject = CHashString(_T("AttachSceneObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgAttachSceneObject, sizeof(ATTACHSCENEOBJECTPARAMS), &asop, parentName);			
		}
	}

}
CParticleSystemObject::~CParticleSystemObject()
{
	RemoveBoundingObject();
	if 	(m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

IEffect * CParticleSystemObject::LoadEffect(IHashString * pName)
{
	LOADFILEEXTPARAMS lfep;
	StdString shaderfile;

	if (pName != NULL)
		shaderfile = pName->GetString();

	if( shaderfile == "" )
	{ 
		shaderfile = _T("shaders/default.efx"); 
	}
	shaderfile.MakeSafeFileName();
	CHashString filename( shaderfile );
	lfep.fileName = (TCHAR*)shaderfile.c_str();
	lfep.bInternalLoad = true;
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
	if( retval == MSG_HANDLED )
	{
		CREATEEFFECTINTERFACE cef;
		cef.m_Name = &filename;
		static DWORD msgHash_GetEffectInterface = CHashString(_T("GetEffectInterface")).GetUniqueID();
		if (EngineGetToolBox()->SendMessage(msgHash_GetEffectInterface, sizeof(CREATEEFFECTINTERFACE ), &cef ) == MSG_HANDLED)
			return cef.m_EffectInterface;
	}
	return NULL;
}
void CParticleSystemObject::Init()
{
	//grab the renderer
	GETRENDERERINTERFACEPARAMS renderInterfaceParam;
	renderInterfaceParam.m_RendererInterface = NULL;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterfaceParam, NULL, NULL );
	m_IRenderer = renderInterfaceParam.m_RendererInterface;

	UpdateTransform();
	UpdateBoundingObject();
}

bool CParticleSystemObject::Update()
{
	if (m_bTransformIsDirty)
	{
		UpdateTransform();
	}
	UpdateBoundingObject();
//	m_ToolBox->Log(LOGINFORMATION, 
//		_T("Position : %f\t%f\t%f\n"),
//			m_v3Position.x, m_v3Position.y, m_v3Position.z);
	if (m_bQueueSelfDelete)
	{
		return false;
	}
	return true;
}

bool CParticleSystemObject::Render( UINT pass, IEffect * override )
{
	PARTICLEEMITTERVECTOR::iterator listIter = m_ParticleEmitters.begin();
	for(; listIter != m_ParticleEmitters.end(); listIter++)
	{
		CParticleEmitterObject *obj = (*listIter);
		if (obj)
		{
			obj->RenderEmitter(pass, override);
		}
	}
	return true;
}

void CParticleSystemObject::UpdateTransform()
{
	Matrix4x4 parentGlobalTransform;

	if (m_pBaseObject)
	{
		m_pBaseObject->GetAttachTransform(&m_hsAttachNodeName, parentGlobalTransform);
	}

	// factor in the parent's rotation
	//Matrix4x4 newGlobalTransform = m_LocalTransform.GetTransform() * parentGlobalTransform;
	Matrix4x4 newGlobalTransform = parentGlobalTransform* m_LocalTransform.GetTransform();
	m_GlobalTransform.SetTransform( newGlobalTransform );
	m_mat4ParentTransform = newGlobalTransform ; //TODO FIX THIS HACK

	m_v3Position = m_mat4ParentTransform.GetTranslation();
	m_eulRotation = m_mat4ParentTransform.GetRotation();
	SetAttachmentObjectsDirty();
	//		SetScaling( m_mat4ParentTransform.GetScale());
	// The transform is now clean
	m_bTransformIsDirty = false;
}
void CParticleSystemObject::GetAttachTransform(IHashString *hsNodeName, Matrix4x4 &attachXForm)
{
	attachXForm.SetFrom4x4(m_GlobalTransform.GetTransform().GetMatrix());
}
void CParticleSystemObject::GetBoundingSphere( Vec3 &pos, float &radius )
{
	radius = 1000.0f;
	pos = m_v3Position;
	PARTICLEEMITTERVECTOR::iterator listIter = m_ParticleEmitters.begin();
	for(; listIter != m_ParticleEmitters.end(); listIter++)
	{
		CParticleEmitterObject *obj = (*listIter);
		if (obj)
		{
			float retRadius;
			obj->GetBoundingSphere(pos, retRadius);
			if ( retRadius > radius )
				radius = retRadius;
		}
	}
}
bool CParticleSystemObject::GetBoundingBox( Vec3 &position, Vec3 &dim, EulerAngle &rotation )
{
	bool bReturn = false;
	for (UINT i = 0; i < m_ParticleEmitters.size(); i++)
	{
		Vec3 v3Position;
		Vec3 boxDimensions;
		EulerAngle eulRotation;
		// If one of the Emitters is true, return true.
		if (m_ParticleEmitters[i]->GetBoundingBox(v3Position, boxDimensions, eulRotation ) && !bReturn)
			bReturn = true;
		position += v3Position;
		if (boxDimensions.x > dim.x)
			dim.x = boxDimensions.x;
		if (boxDimensions.y > dim.y)
			dim.y = boxDimensions.y;
		if (boxDimensions.z > dim.z)
			dim.z = boxDimensions.z;
	}
	position /= (float)m_ParticleEmitters.size();
	return bReturn;
}
DWORD CParticleSystemObject::OnAddChildObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ISceneObject*));
	ISceneObject *pObject = *(ISceneObject**)param;

	AttachObject(pObject);

	return MSG_HANDLED_STOP;
}

DWORD CParticleSystemObject::OnRemoveChildObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ISceneObject*));
	ISceneObject *pObject = *(ISceneObject**)param;

	DetachObject( pObject );

	return MSG_HANDLED_STOP;
}

DWORD CParticleSystemObject::OnRegisterChildEmitter( DWORD size, void *param) 
{	
	VERIFY_MESSAGE_SIZE(size, sizeof(CParticleEmitterObject*));
	CParticleEmitterObject *pEmitterObject = *(CParticleEmitterObject**)param;
	// Pointer Valid, add to vector.
	if (pEmitterObject)
	{
		m_ParticleEmitters.push_back( pEmitterObject );
		return MSG_HANDLED_STOP;
	}
	else
		return MSG_ERROR;
}

DWORD CParticleSystemObject::OnDeRegisterChildEmitter( DWORD size, void *param) 
{	
	VERIFY_MESSAGE_SIZE(size, sizeof(CParticleEmitterObject*));
	CParticleEmitterObject *pEmitterObject = *(CParticleEmitterObject**)param;

	PARTICLEEMITTERVECTOR::iterator listIter = m_ParticleEmitters.begin();
	for(; listIter != m_ParticleEmitters.end(); listIter++)
	{
		if ((*listIter) == pEmitterObject)
		{
			m_ParticleEmitters.erase(listIter);
			return MSG_HANDLED_STOP;
		}
	}
	if (m_ParticleEmitters.empty())
		m_bQueueSelfDelete = true;
	return MSG_ERROR;
}

DWORD CParticleSystemObject::OnAttachSceneObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ATTACHSCENEOBJECTPARAMS));
	ATTACHSCENEOBJECTPARAMS asop = *(ATTACHSCENEOBJECTPARAMS*)param;
	if (asop.hsAttachmentObject)
	{
		// if a node is specified, entities don't handle the message
		// otherwise...
		if (asop.hsNodeAttachName == NULL)
		{
			static DWORD msgRegisterSceneAttachObjects = CHashString(_T("RegisterSceneAttachObjects")).GetUniqueID();
			asop.hsBaseObject = GetName();
			m_ToolBox->SendMessage(msgRegisterSceneAttachObjects, sizeof(ATTACHSCENEOBJECTPARAMS), &asop);			
			return MSG_HANDLED_STOP;
		}
	}
	return MSG_NOT_HANDLED;
}

// 
// ENTITY MESSAGES
//
REGISTER_MESSAGE_HANDLER( SetGlobalPosition, OnSetGlobalPosition, CParticleSystemObject );

DWORD CParticleSystemObject::OnSetGlobalPosition( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3* newGlobalPosition = (Vec3*)params;

	if (!newGlobalPosition->IsValid())
	{
		m_ToolBox->Log(LOGWARNING, _T("(%s)ParticleEmitterObject::OnSetGlobalPosition received invalid number in one of the components: %f, %f, %f \n"), this->GetName()->GetString(), newGlobalPosition->x, newGlobalPosition->y, newGlobalPosition->z);
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

	UpdateTransform();

	return MSG_HANDLED_STOP;
}