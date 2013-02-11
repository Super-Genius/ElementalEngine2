///============================================================================
/// \file	CPhysicsShape.cpp
/// \brief	Implementations of IPhysShape
/// \date	7-13-2005
/// \author	Kyle Swaim
/// \note	Copyright (c) 2005 Signature Devices, Inc.
///============================================================================

#include "StdAfx.h"

#include "NxPhysics.h"

#include "CPhysicsShape.h"
#include "NovodexMathAdapter.h"

///============================================================================
/// CPhysicsShape
///============================================================================

CPhysicsShape::~CPhysicsShape()
{
	if(desc != NULL)
	{
		delete desc;
		desc = NULL;
	}
}

void CPhysicsShape::SetLocalPosition(Vec3 & pos)
{
	desc->localPose.t = NxVec3Adapter(pos);
}

void CPhysicsShape::SetLocalOrientation(Matrix3x3 &rot)
{
	desc->localPose.M = NxMat33Adapter(rot);
}

void CPhysicsShape::SetFlags(DWORD flags)
{
	desc->shapeFlags = flags;
}

StdString &CPhysicsShape::GetShape()
{
	return m_ShapeType;
}

StdString &CPhysicsShape::GetLink()
{
	return m_Link;
}

void CPhysicsShape::SetLink(StdString &link)
{
	m_Link = link;
}

int CPhysicsShape::GetCollisionGroup()
{
	return desc->group;
}

void CPhysicsShape::SetCollisionGroup( int group )
{
	desc->group = group;
}

///============================================================================
/// CBoxPhysShape
///============================================================================

CBoxPhysShape::CBoxPhysShape()
{
	desc = new NxBoxShapeDesc();
	desc->setToDefault();
	m_ShapeType = _T("box");
}

void CBoxPhysShape::SetDimensions(Vec3 & dim)
{
	((NxBoxShapeDesc*)desc)->dimensions.set( dim.GetX(), dim.GetY(), dim.GetZ() );
}

///============================================================================
/// CSpherePhysShape
///============================================================================

CSpherePhysShape::CSpherePhysShape()
{
	desc = new NxSphereShapeDesc();
	desc->setToDefault();
	m_ShapeType = _T("sphere");
}

void CSpherePhysShape::SetRadius(float rad)
{
	((NxSphereShapeDesc*)desc)->radius = rad;
}

///============================================================================
/// CMeshPhysShape
///============================================================================

CMeshPhysShape::CMeshPhysShape()
{
	desc = new NxTriangleMeshShapeDesc();
	desc->setToDefault();
	m_ShapeType = _T("terrainMeshObject");
}

///============================================================================
/// CCapsulePhysShape
///============================================================================

CCapsulePhysShape::CCapsulePhysShape()
{
	desc = new NxCapsuleShapeDesc();
	desc->setToDefault();
	m_ShapeType = _T("capsule");
}

void CCapsulePhysShape::SetRadius(float rad)
{
	((NxCapsuleShapeDesc*)desc)->radius = rad;
}

void CCapsulePhysShape::SetHeight(float height)
{
	((NxCapsuleShapeDesc*)desc)->height = height;
}

///============================================================================
/// CConvexPhysShape
///============================================================================

CConvexPhysShape::CConvexPhysShape()
{
	desc = new NxConvexShapeDesc();
	desc->setToDefault();
	m_ShapeType = _T("convex");
}
