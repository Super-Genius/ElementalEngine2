///==========================================================================
/// \file	CPhysicsActor.h
/// \brief	Wrapper for an NxActor
/// \date	5/07/2007
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

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPhysicsActor::CPhysicsActor()
{
	m_Actor = NULL;
	m_Parent = NULL;
	m_Children.clear();

	m_ToolBox = EngineGetToolBox();
	m_CurrentScale.Set( 1.0f, 1.0f, 1.0f );

	m_Name = NULL;
}

CPhysicsActor::CPhysicsActor( NxActor* actor )
{
	m_Actor = actor;
	m_Parent = NULL;
	m_Children.clear();

	m_ToolBox = EngineGetToolBox();
	m_CurrentScale.Set( 1.0f, 1.0f, 1.0f );

	m_WasDynamic = false;

	m_Name = NULL;
	const TCHAR* actorName = actor->getName();
	if( actorName != NULL )
		m_Name = new CHashString(actor->getName());

	m_UserData = new PHYSICSUSERDATA();
}

CPhysicsActor::~CPhysicsActor()
{
	m_Children.clear();
	SAFEDELETE( m_Name );
	SAFEDELETE( m_UserData );
}

IHashString* CPhysicsActor::GetName()
{
	return m_Name;
}

void CPhysicsActor::SetParent( IPhysicsActor* actor )
{
	CPhysicsActor* physActor = (CPhysicsActor*) actor;
	m_Parent = physActor;
}

void CPhysicsActor::AddChild( IPhysicsActor* actor )
{
	CPhysicsActor* physActor = (CPhysicsActor*) actor;
	m_Children.push_back( physActor );
}

NxActor* CPhysicsActor::GetNxActor()
{
	return m_Actor;
}

void CPhysicsActor::RemoveChild( IPhysicsActor* actor )
{
	CPhysicsActor* physActor = (CPhysicsActor*) actor;
	m_Children.remove( physActor );
}

void CPhysicsActor::SetScale( Vec3& scale )
{
	// do not scale if scale is currently 1:1 or if the scale
	// has not changed
	if( scale.x == 1 && scale.y == 1 && scale.z == 1 ||
		scale == m_CurrentScale )
	{
		return;
	}

	// make sure the scale is valid
	// No 0 scales or negative scales!
	if( scale.x <= 0 && scale.y <= 0 && scale.z <= 0 )
	{
		m_ToolBox->Log( LOGWARNING, _T("CPhysicsActor::SetScale() Invalid scale!\n" ) );
		return;
	}

	NxVec3 newScale( scale.x, scale.y, scale.z );

	// unscale the old scale
	// Loop through shapes in the actor
	unsigned int numShapes = m_Actor->getNbShapes();
	NxShape*const* shapes = m_Actor->getShapes();
   	NxShape* currentShape;

	NxVec3 shapeLocalPosition;

	// for each shape type scale its dimensions
	while( numShapes-- >= 1 )
	{
		currentShape = shapes[numShapes];

		// get the shape's type
		NxShapeType type = currentShape->getType();

		switch( type )
		{
			case NX_SHAPE_BOX:
				{
					// do something
					NxBoxShape* shape = (NxBoxShape*)currentShape;
					// rescale box dimensions
					NxVec3 dimensions = shape->getDimensions();
					
					Vec3 newDimensions(dimensions.x, dimensions.y, dimensions.z);
					RescaleVector( newDimensions, scale );

					// set the shape data with the newly rescaled dimensions
					shape->setDimensions( NxVec3(newDimensions.x, newDimensions.y, newDimensions.z) );
					break;
				}
			case NX_SHAPE_SPHERE:
				{
					// do something
					NxSphereShape* shape = (NxSphereShape*)currentShape;
					float radius = shape->getRadius();
					radius /= m_CurrentScale.x;
					radius *= newScale.x;
					// set the shape data with the newly rescaled dimensions
					shape->setRadius( radius );
					break;
				}
			case NX_SHAPE_CAPSULE:
				{
					// do something
					NxCapsuleShape* shape;
					shape = (NxCapsuleShape*)currentShape;
					// rescale radius
					float radius = shape->getRadius();
					radius /= m_CurrentScale.x;
					radius *= newScale.x;
					// rescale height
					float height = shape->getHeight();
					height /= m_CurrentScale.z;
					height *= newScale.z;
					// set the shape data with the newly rescaled dimensions
					shape->setRadius( radius );
					shape->setHeight( height );
					break;
				}
			default:
				m_ToolBox->Log( LOGWARNING, _T("CPhysicsObject::SetScale() Attempting to scale on unsupported shape!\n" ) );
				return;
		}

		// get the shape's local position and rescale it
		shapeLocalPosition = currentShape->getLocalPosition();

		Vec3 newShapeLocalPosition(shapeLocalPosition.x, shapeLocalPosition.y, shapeLocalPosition.z);
		RescaleVector( newShapeLocalPosition, scale );

		currentShape->setLocalPosition( NxVec3(newShapeLocalPosition.x, newShapeLocalPosition.y, newShapeLocalPosition.z) );
	}

	// Set the current scale to the new scale so we can unscale the scale
	m_CurrentScale = scale;
}

void CPhysicsActor::AddVisualization()
{
	// get the CPhysicsObject's name
	PHYSICSUSERDATA* userData = (PHYSICSUSERDATA*)m_Actor->userData;
	
	if( userData == NULL )
		return;

	CPhysicsObject* physObj = userData->physObj;
	IHashString* cpoName = physObj->GetParentName();

	// Loop through shapes in the actor
	unsigned int numShapes = m_Actor->getNbShapes();
	NxShape*const* shapes = m_Actor->getShapes();
   	NxShape* shape;

	// we need to unscale before feeding it to the shape objects since they
	// get the scale from the parent
	Vec3 invScale;
	invScale.x = 1.0f / m_CurrentScale.x;
	invScale.y = 1.0f / m_CurrentScale.y;
	invScale.z = 1.0f / m_CurrentScale.z;

	// Add visualizations for each shape
	while( numShapes-- )
	{
		shape = shapes[numShapes];
   		
		// Add shape to be rendered
		if( shape->isBox() )
		{
			NxBoxShape* boxShape = (NxBoxShape*)shape;

			Matrix4x4 localTransform;
			localTransform.SetIdentity();

			float tempRot[9];
			boxShape->getLocalOrientation().getColumnMajor( tempRot );
			localTransform.SetFrom3x3( tempRot );

			NxVec3 tempPos = boxShape->getLocalPosition();
			tempPos.x *= invScale.x;
			tempPos.y *= invScale.y;
			tempPos.z *= invScale.z;
			localTransform.SetTranslation( Vec3(tempPos.x, tempPos.y, tempPos.z) );

			NxVec3 boxDimensions = boxShape->getDimensions();
			float halfXDimension = boxDimensions.x * invScale.x;
			float halfYDimension = boxDimensions.y * invScale.y;
			float halfZDimension = boxDimensions.z * invScale.z;

			// Add a debug render object to visualize the object
			ADDOBJECTORIENTEDBOXPARAMS oobbParams;
			oobbParams.name = cpoName;
			oobbParams.min = Vec3( -halfXDimension, -halfYDimension, -halfZDimension );
			oobbParams.max = Vec3( halfXDimension, halfYDimension, halfZDimension );
			oobbParams.localTransform = localTransform;
			static DWORD msgHash_AddObjectOrientedBox = CHashString(_T("AddObjectOrientedBox")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_AddObjectOrientedBox, sizeof(ADDOBJECTORIENTEDBOXPARAMS), &oobbParams );
		}
		if( shape->isSphere() )
		{
			NxSphereShape* sphereShape = (NxSphereShape*)shape;

			float radius = sphereShape->getRadius();

			// Add a debug render object to visualize the object
			ADDSPHEREPARAMS sphereParams;
			sphereParams.name = cpoName;
			sphereParams.radius = radius * invScale.x;
			sphereParams.red = 0;
			sphereParams.green = 255; // making the sphere green to distinguish it from AABBs
			sphereParams.blue = 0;
			static DWORD msgHash_AddSphere = CHashString(_T("AddSphere")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_AddSphere, sizeof(ADDSPHEREPARAMS), &sphereParams );
		}
		if( shape->isCapsule() )
		{
			// Draw as a red box for now
			NxCapsuleShape* capsuleShape = (NxCapsuleShape*)shape;

			Matrix4x4 localTransform;
			localTransform.SetIdentity();

			float tempRot[9];
			capsuleShape->getLocalOrientation().getColumnMajor( tempRot );
			localTransform.SetFrom3x3( tempRot );

			NxVec3 tempPos = capsuleShape->getLocalPosition();
			tempPos.x *= invScale.x;
			tempPos.y *= invScale.y;
			tempPos.z *= invScale.z;
			localTransform.SetTranslation( Vec3(tempPos.x, tempPos.y, tempPos.z) );

			float halfXDimension = capsuleShape->getRadius();
			float halfYDimension = capsuleShape->getHeight() - capsuleShape->getRadius();
			float halfZDimension = capsuleShape->getRadius();

			// Add a debug render object to visualize the object
			ADDOBJECTORIENTEDBOXPARAMS oobbParams;
			oobbParams.name = cpoName;
			oobbParams.min = Vec3( -halfXDimension, -halfYDimension, -halfZDimension );
			oobbParams.max = Vec3( halfXDimension, halfYDimension, halfZDimension );
			oobbParams.localTransform = localTransform;
			oobbParams.red = 255;
			oobbParams.green = 0;
			oobbParams.blue = 0;
			static DWORD msgHash_AddObjectOrientedBox = CHashString(_T("AddObjectOrientedBox")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_AddObjectOrientedBox, sizeof(ADDOBJECTORIENTEDBOXPARAMS), &oobbParams );
		}
		if( shape->isConvexMesh() )
		{
			// not yet implemented
		}
	}
}

void CPhysicsActor::RemoveVisualization()
{
	// get the CPhysicsObject's name
	PHYSICSUSERDATA* userData = (PHYSICSUSERDATA*)m_Actor->userData;
	
	if( userData == NULL )
		return;

	CPhysicsObject* physObj = userData->physObj;
	IHashString* cpoName = physObj->GetParentName();

	// Remove oobb visualizations
	REMOVEOBJECTORIENTEDBOXPARAMS oobbParams;
	oobbParams.name = cpoName;
	static DWORD msgHash_RemoveObjectOrientedBox = CHashString(_T("RemoveObjectOrientedBox")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveObjectOrientedBox, sizeof(REMOVEOBJECTORIENTEDBOXPARAMS), &oobbParams );
	
	// Remove aabb visualizations
	REMOVEAXISALIGNEDBOXPARAMS aabbParams;
	aabbParams.name = cpoName;
	static DWORD msgHash_RemoveAxisAlignedBox = CHashString(_T("RemoveAxisAlignedBox")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveAxisAlignedBox, sizeof(REMOVEAXISALIGNEDBOXPARAMS), &oobbParams );
	
	// Remove sphere visualizations
	REMOVESPHEREPARAMS sphereParams;
	sphereParams.name = cpoName;
	static DWORD msgHash_RemoveSphere = CHashString(_T("RemoveSphere")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveSphere, sizeof(REMOVESPHEREPARAMS), &sphereParams );
}

void CPhysicsActor::GetTransform(Matrix4x4& trans)
{
	NxMat34 tmp = m_Actor->getGlobalPose();
	float* tmp2 = new float[16];
	tmp.getColumnMajor44( tmp2 );
	trans.SetFrom4x4( tmp2 );

	delete tmp2;
}

void CPhysicsActor::SetFromBoneData( BONEINFO* boneInfo )
{
	NxMat34 transform;
	transform.setRowMajor44( boneInfo->m_Matrix->GetMatrix() );

	m_Actor->setGlobalPose( transform );
}

void CPhysicsActor::SetUserData( IPhysicsObject* physObj, IPhysicsActor* physActor )
{
	if( physObj == NULL && physActor == NULL )
	{
		m_Actor->userData = NULL;
		return;
	}

	m_UserData->physObj = (CPhysicsObject*) physObj;
	m_UserData->physActor = (CPhysicsActor*) physActor;

	m_Actor->userData = m_UserData;
}

void* CPhysicsActor::GetUserData()
{
	return m_Actor->userData;
}

bool CPhysicsActor::IsDynamic()
{
	return m_Actor->isDynamic();
}

bool CPhysicsActor::IsKinematic()
{
	if( m_Actor->isDynamic() )
		return m_Actor->readBodyFlag( NX_BF_KINEMATIC );
	else
		return false;
}

bool CPhysicsActor::IsSleeping()
{
	return m_Actor->isSleeping();
}

void CPhysicsActor::SetCollisionGroup( COLLISIONGROUP group )
{
	unsigned int numShapes = m_Actor->getNbShapes();
	NxShape*const* shapes = m_Actor->getShapes();
   	NxShape* shape;

	if( group < 0 || group >= 32 )
	{
		m_ToolBox->Log( LOGERROR, _T("CPhysicsObject::SetCollisionGroup() Invalid collision group! Group must be between 1-31!\n") );
		return;
	}

	while( numShapes-- )
	{
		shape = shapes[numShapes];
   		shape->setGroup( group );
	}
}

void CPhysicsActor::SetContactGroup( unsigned int group )
{
	m_Actor->setGroup( group );
}

int CPhysicsActor::GetCollisionGroup()
{
	NxShape*const* shapes = m_Actor->getShapes();
   	NxShape* shape;

	shape = shapes[0];

	return shape->getGroup();
}

void CPhysicsActor::SetKinematic()
{
	// Make sure that the actor is not static
  	if( !m_Actor->isDynamic() )
  	{
  		return;
  	}

	m_Actor->raiseBodyFlag( NX_BF_KINEMATIC );
}

void CPhysicsActor::SetDynamic()
{
	// Make sure that the actor is not static
  	if( !m_Actor->isDynamic() )
  	{
  		return;
  	}

	m_Actor->clearBodyFlag( NX_BF_KINEMATIC );
}

void CPhysicsActor::ForceKinematic()
{
	// Make sure that the actor is not static or already kinematic
  	if( !m_Actor->isDynamic() || IsKinematic() )
  	{
  		return;
  	}

	m_Actor->raiseBodyFlag( NX_BF_KINEMATIC );
	m_WasDynamic = true;
}

void CPhysicsActor::RestoreDynamic()
{
	// Make sure that the actor is not static
  	if( !m_Actor->isDynamic() )
  	{
  		return;
  	}

	if( m_WasDynamic )
	{
		m_Actor->clearBodyFlag( NX_BF_KINEMATIC );
	}
}

void CPhysicsActor::LockTranslationAxies( LOCKAXIES* axies )
{
	// lock/unlock the X axis
	if( axies->xAxis == true )
		m_Actor->raiseBodyFlag( NX_BF_FROZEN_POS_X );
	else
		m_Actor->clearBodyFlag( NX_BF_FROZEN_POS_X );

	// lock/unlock the Y axis
	if( axies->yAxis == true )
		m_Actor->raiseBodyFlag( NX_BF_FROZEN_POS_Y );
	else
		m_Actor->clearBodyFlag( NX_BF_FROZEN_POS_Y );

	// lock/unlock the Z axis
	if( axies->zAxis == true )
		m_Actor->raiseBodyFlag( NX_BF_FROZEN_POS_Z );
	else
		m_Actor->clearBodyFlag( NX_BF_FROZEN_POS_Z );
}

void CPhysicsActor::LockRotationAxies( LOCKAXIES* axies )
{
	// lock/unlock the X axis
	if( axies->xAxis == true )
		m_Actor->raiseBodyFlag( NX_BF_FROZEN_ROT_X );
	else
		m_Actor->clearBodyFlag( NX_BF_FROZEN_ROT_X );

	// lock/unlock the Y axis
	if( axies->yAxis == true )
		m_Actor->raiseBodyFlag( NX_BF_FROZEN_ROT_Y );
	else
		m_Actor->clearBodyFlag( NX_BF_FROZEN_ROT_Y );

	// lock/unlock the Z axis
	if( axies->zAxis == true )
		m_Actor->raiseBodyFlag( NX_BF_FROZEN_ROT_Z );
	else
		m_Actor->clearBodyFlag( NX_BF_FROZEN_ROT_Z );
}

Matrix4x4 CPhysicsActor::GetGlobalPose()
{
	NxMat34Adapter adapter = m_Actor->getGlobalPose();
	return adapter.ToMatrix4x4();
}

void CPhysicsActor::SetGlobalPose( Matrix4x4& transform )
{
	NxMat34Adapter adapter(transform);
	m_Actor->setGlobalPose( adapter );
}

void CPhysicsActor::MoveGlobalPose( Matrix4x4& transform )
{
	NxMat34Adapter adapter(transform);
	m_Actor->moveGlobalPose( adapter );
}

Vec3 CPhysicsActor::GetGlobalPosition()
{
	NxVec3 nxPosition = m_Actor->getGlobalPosition();
	return Vec3( nxPosition.x, nxPosition.y, nxPosition.z );
}

void CPhysicsActor::SetGlobalPosition( Vec3& pos )
{
	NxVec3 position( pos.x, pos.y, pos.z );
	m_Actor->setGlobalPosition( position );
}

Matrix3x3 CPhysicsActor::GetGlobalOrientation()
{
	Matrix3x3 orientation;
	NxMat33 tmp = m_Actor->getGlobalOrientation();
	tmp.getColumnMajor( orientation.GetMatrix() );
	return orientation;
}

void CPhysicsActor::SetGlobalOrientation( Matrix3x3& rot )
{
	NxMat33Adapter adapter(rot);
	m_Actor->setGlobalOrientation( adapter );
}

void CPhysicsActor::AddForce( Vec3& force )
{
	NxVec3 tmp( force.x, force.y, force.z );

	if( !tmp.isFinite() )
	{
		m_ToolBox->Log( LOGERROR, _T("Invalid vector given to Physics!\n"));
		return;
	}

	m_Actor->addForce( tmp, NX_IMPULSE );
}

void CPhysicsActor::AddLocalForce( Vec3& force )
{
	NxVec3 tmp( force.x, force.y, force.z );

	if( !tmp.isFinite() )
	{
		m_ToolBox->Log( LOGERROR, _T("Invalid vector given to Physics!\n"));
		return;
	}

	m_Actor->addLocalForce( tmp );
}

void CPhysicsActor::AddTorque( Vec3& torque )
{
	NxVec3 tmp( torque.x, torque.y, torque.z );

	if( !tmp.isFinite() )
	{
		m_ToolBox->Log( LOGERROR, _T("Invalid vector given to Physics!\n"));
		return;
	}

	m_Actor->addTorque( tmp );
}

void CPhysicsActor::AddLocalTorque( Vec3& torque )
{
	NxVec3 tmp( torque.x, torque.y, torque.z );

	if( !tmp.isFinite() )
	{
		m_ToolBox->Log( LOGERROR, _T("Invalid vector given to Physics!\n"));
		return;
	}

	m_Actor->addLocalTorque( tmp );
}

void CPhysicsActor::SetLinearVelocity( Vec3& velocity )
{
	if( !IsKinematic() )
	{
		NxVec3 tmp( velocity.x, velocity.y, velocity.z );

		if( !tmp.isFinite() )
		{
			m_ToolBox->Log( LOGERROR, _T("Invalid vector given to Physics!\n"));
			return;
		}

		m_Actor->setLinearVelocity( tmp );
	}
}

void CPhysicsActor::SetAngularVelocity( Vec3& velocity )
{
	if( !IsKinematic() )
	{
		NxVec3 tmp( velocity.x, velocity.y, velocity.z );

		if( !tmp.isFinite() )
		{
			m_ToolBox->Log( LOGERROR, _T("Invalid vector given to Physics!\n"));
			return;
		}

		m_Actor->setAngularVelocity( tmp );
	}
}

void CPhysicsActor::SetLinearMomentum( Vec3& momentum )
{
	if( !IsKinematic() )
	{
		NxVec3 tmp( momentum.x, momentum.y, momentum.z );

		if( !tmp.isFinite() )
		{
			m_ToolBox->Log( LOGERROR, _T("Invalid vector given to Physics!\n"));
			return;
		}

		m_Actor->setLinearMomentum( tmp );
	}
}

void CPhysicsActor::SetAngularMomentum( Vec3& momentum )
{
	if( !IsKinematic() )
	{
		NxVec3 tmp( momentum.x, momentum.y, momentum.z );

		if( !tmp.isFinite() )
		{
			m_ToolBox->Log( LOGERROR, _T("Invalid vector given to Physics!\n"));
			return;
		}

		m_Actor->setAngularMomentum( tmp );
	}
}

void CPhysicsActor::SetLinearDamping( float damping )
{
	if( !IsKinematic() )
	{
		m_Actor->setLinearDamping( damping );
	}
}

void CPhysicsActor::SetAngularDamping( float damping )
{
	if( !IsKinematic() )
	{
		m_Actor->setAngularDamping( damping );
	}
}

void CPhysicsActor::GetLinearVelocity( Vec3& velocity )
{
	NxVec3 actorVel = m_Actor->getLinearVelocity();
	velocity.Set( actorVel.x, actorVel.y, actorVel.z );
}

void CPhysicsActor::GetAngularVelocity( Vec3& velocity )
{
	NxVec3 actorVel = m_Actor->getAngularVelocity();
	velocity.Set( actorVel.x, actorVel.y, actorVel.z );
}

void CPhysicsActor::GetLinearMomentum( Vec3& momentum )
{
	NxVec3 actorMom = m_Actor->getLinearMomentum();
	momentum.Set( actorMom.x, actorMom.y, actorMom.z );
}

void CPhysicsActor::GetAngularMomentum( Vec3& momentum )
{
	NxVec3 actorMom = m_Actor->getAngularMomentum();
	momentum.Set( actorMom.x, actorMom.y, actorMom.z );
}

void CPhysicsActor::LinearSweep( Vec3& displacement, void* hitResult )
{
	NxSweepQueryHit* nxHit = (NxSweepQueryHit*) hitResult;
    NxU32 flags = NX_SF_STATICS|NX_SF_DYNAMICS;
	NxVec3 tmp( displacement.x, displacement.y, displacement.z );
    m_Actor->linearSweep(tmp, flags, NULL, 1, nxHit, NULL);
}

void CPhysicsActor::RescaleVector( Vec3& vec, Vec3& newScale )
{
		// undo the current scale factor so that the scale is 1
	vec.x /= m_CurrentScale.x;
	vec.y /= m_CurrentScale.y;
	vec.z /= m_CurrentScale.z;

	// rescale with the new scale
	vec.x *= newScale.x;
	vec.y *= newScale.y;
	vec.z *= newScale.z;
}

void CPhysicsActor::SetShapeFlag( int flag, bool enable )
{
	NxShapeFlag nxShapeFlag = (NxShapeFlag) flag;

	if( m_Actor == NULL )
	{
		m_ToolBox->Log( LOGERROR, _T("CPhysicsObject::SetShapeFlag() Actor is NULL.\n") );
		return;
	}

	// Loop through shapes in the actor
	unsigned int numShapes = m_Actor->getNbShapes();
	NxShape*const* shapes = m_Actor->getShapes();
	NxShape* shape;

	while( numShapes-- )
	{
		shape = shapes[numShapes];
		shape->setFlag( nxShapeFlag, enable );
	}
}

void CPhysicsActor::SetBodyFlag( int flag, bool enable )
{
	NxBodyFlag nxFlag = (NxBodyFlag) flag;
	if( enable == true )
		m_Actor->raiseBodyFlag( nxFlag );
	else
		m_Actor->clearBodyFlag( nxFlag );
}

void CPhysicsActor::SetTrigger( TRIGGERPARAMS& triggerParams )
{
	if( triggerParams.triggerOnEnter )
	{
		SetShapeFlag( NX_TRIGGER_ON_ENTER, triggerParams.triggerOnEnter );
	}
	if( triggerParams.triggerOnExit )
	{
		SetShapeFlag( NX_TRIGGER_ON_LEAVE, triggerParams.triggerOnExit );
	}
	if( triggerParams.triggerOnStay )
	{
		SetShapeFlag( NX_TRIGGER_ON_STAY, triggerParams.triggerOnStay );
	}
}
