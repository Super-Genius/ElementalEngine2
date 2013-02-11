///============================================================================
/// \file	IPhysShape.h
/// \brief	Implementations of IPhysShape
/// \date	7-13-2005
/// \author	Kyle Swaim
/// \note	Copyright (c) 2005 Signature Devices, Inc.
///============================================================================

#ifndef CPHYSSHAPE_H
#define CPHYSSHAPE_H

class NxShapeDesc;

namespace ElementalEngine
{

///============================================================================
/// \class	CPhysicsShape
/// \brief	Virtual base class for phys shapes
///============================================================================
class CPhysicsShape
{
public:
	/// \brief	virtual Descructor
	virtual ~CPhysicsShape();

	/// \brief	Set the local position of the shape in relation
	///			to the CPhysicsObject
	/// \param	pos - the local position
	virtual void SetLocalPosition(Vec3 & pos);
	/// \brief	Set the local orientation of the shape in relation
	///			to the CPhysicsObject
	/// \param	rot - the local orientation
	virtual void SetLocalOrientation(Matrix3x3 &rot);
	/// \brief	Set control flags
	/// \param	flags - the flags to set
	virtual void SetFlags(DWORD flags);
	/// \brief	Get the type of the shape
	/// \return	The shape type of the descriptor
	virtual StdString &GetShape();
	/// \brief	Get the linked object
	/// \return	The linked object
	virtual StdString &GetLink();
	/// \brief	Set the linked object
	/// \param	The linked object
	virtual void SetLink(StdString &link);
	/// \brief	Get the collision group
	/// \return	The collisionGroup
	virtual int GetCollisionGroup();
	/// \brief	Set the collisionGroup
	/// \param	group - The collision group
	virtual void SetCollisionGroup( int group );

	NxShapeDesc *desc;

protected:
	StdString m_ShapeType;
	StdString m_Link;
};

///============================================================================
/// \class	CBoxPhysShape
/// \brief	Box implementation of CPhysicsShape
///============================================================================
class CBoxPhysShape : public CPhysicsShape
{
public:
	/// \brief	Default Constructor
	CBoxPhysShape();

	/// \brief	Set the box dimensions
	/// \param	dim - the dimensions of the box
	void SetDimensions(Vec3 & dim);
};

///============================================================================
/// \class	CSpherePhysShape
/// \brief	Sphere implementation of CPhysicsShape
///============================================================================
class CSpherePhysShape : public CPhysicsShape
{
public:
	/// \brief	Default Constructor
	CSpherePhysShape();

	/// \brief	Set the radius of the sphere
	/// \param	rad - the radius
	void SetRadius(float rad);
};

///============================================================================
/// \class	CMeshPhysShape
/// \brief	Mesh implementation of CPhysicsShape
///============================================================================
class CMeshPhysShape : public CPhysicsShape
{
public:
	/// \brief	Default Constructor
	CMeshPhysShape();
};

///============================================================================
/// \class	CCapsulePhysShape
/// \brief	Capsule implementation of IPhysShape
///============================================================================
class CCapsulePhysShape : public CPhysicsShape
{
public:
	/// \brief	Default Constructor
	CCapsulePhysShape();

	/// \brief	Set the radius of the capsule's hemispherical ends and its trunk
	/// \param	rad - the radius
	void SetRadius(float rad);
	/// \brief	Set the distance between the two hemispherical ends of the
	///			capsule The height is along the capsule's Y axis
	void SetHeight(float height);
};

///============================================================================
/// \class	CConvexPhysShape
/// \brief	Convex mesh implementation of IPhysShape
///============================================================================
class CConvexPhysShape : public CPhysicsShape
{
public:
	/// \brief	Default Constructor
	CConvexPhysShape();
};

} // namespace ElementalEngine

#endif	// CPHYSSHAPE_H