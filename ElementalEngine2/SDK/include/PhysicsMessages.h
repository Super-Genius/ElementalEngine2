///============================================================================
/// \file		PhysicsMessages.h
/// \brief		Header file for Physics Messages
/// \date		10-04-2006
/// \author		Kyle Swaim
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

#ifndef	PHYSICSDEFINES_H
#define	PHYSICSDEFINES_H

struct ADDPHYSICSOBJECTPARAMS
{
	IHashString *fileName;
};

enum PHYSICSTYPES
{
	STATIC,
	DYNAMIC,
	KINEMATIC
};

enum PHYSICSSHAPES
{
	BOX,
	SPHERE,
	CAPSULE,
	CONVEXMESH,
	HEIGHTFIELD
};

// tolua_begin

typedef list<IHashString*> PHYSICSACTORLIST;

enum AXIES
{
	X,
	Y,
	Z,
	ALLAXIES
};

struct LOCKAXIES
{
	LOCKAXIES()
	{
	}

	bool xAxis;
	bool yAxis;
	bool zAxis;
};

enum COLLISIONGROUP
{
	ALL			= 0xffffffff,// = -1,
	OBJECTS		= 1,
	TERRAIN		= 2,
	CONTROL		= 3,
	NOCOLLISION = 31
};

struct COLLISIONPAIR
{
	COLLISIONGROUP groupA;
	COLLISIONGROUP groupB;
	bool enable;
};

enum CONTACTFLAGS
{
	IGNORE_PAIR		= ( 1<<0 ),
	ON_START_TOUCH	= ( 1<<1 ),
	ON_END_TOUCH	= ( 1<<2 ),
	ON_TOUCH		= ( 1<<3 ),
	ON_IMPACT		= ( 1<<4 ),
	ON_ROLL			= ( 1<<5 ),
	ON_SLIDE		= ( 1<<6 ),
	NOTIFY_FORCES	= ( 1<<7 ),
	NOTIFY_ALL		= ( ON_START_TOUCH|ON_END_TOUCH|ON_TOUCH|ON_IMPACT|ON_ROLL|ON_SLIDE|NOTIFY_FORCES )
};

struct CONTACTPAIR
{
	COLLISIONGROUP groupA;
	COLLISIONGROUP groupB;
	CONTACTFLAGS flags;
};

struct RAYCOLLISION
{
	RAYCOLLISION()
	{
		distance = 0.0f;
		object = NULL;
		actor = NULL;
		parentobject = NULL;
		hitPoint[0] = hitPoint[1] = hitPoint[2] = 0.0f;
		normal[0] = normal[1] = normal[2] = 0.0f;
	}

	float distance;
	float hitPoint[3];
	float normal[3];
	IHashString* object;
	IHashString* actor;
	IHashString* objectType;
	IHashString* parentobject;
};

struct RAYCASTPARAMS
{
	RAYCASTPARAMS()
	{
		group = ALL;
  		rayPos = NULL;
  		rayDir = NULL;
		listRayCollisions = NULL;
	}

	COLLISIONGROUP group;
	Vec3 *rayPos;
	Vec3 *rayDir;
	RAYCOLLISION *listRayCollisions;
};

typedef std::list<RAYCOLLISION> RAYCOLLISIONLIST;

struct RAYCASTLISTPARAMS
{
	RAYCASTLISTPARAMS()
	{
		group = ALL;
  		rayPos = NULL;
  		rayDir = NULL;
		listRayCollisions = NULL;
	}

	COLLISIONGROUP group;
	Vec3 *rayPos;
	Vec3 *rayDir;
	RAYCOLLISIONLIST *listRayCollisions;
};

struct OBJECTCOLLISION
{

};

struct ACTORINFO
{
	ACTORINFO()
	{
		name = NULL;
		pos = NULL;
		rot = NULL;
	}

	IHashString* name;
	Vec3* pos;
	Vec3* rot;
};

struct ACTORFORCE
{
	ACTORFORCE()
	{
		name = NULL;
		force = NULL;
	}

	IHashString* name;
	Vec3* force;
};

struct TRIGGERPARAMS
{
	TRIGGERPARAMS()
	{
		triggerOnEnter = false;
		triggerOnStay = false;
		triggerOnExit = false;
	}

	bool triggerOnEnter;
	bool triggerOnStay;
	bool triggerOnExit;
};

// tolua_end

struct AABBINTERSECTPARAMS
{
	AABBINTERSECTPARAMS()
	{
  		collisionGroup = -1;	// Default to all groups
 		min = NULL;
  		max = NULL;
 		listIntersections = NULL;
  		listIntersectionNames = NULL;
	}

	int collisionGroup;
	Vec3 *min;
	Vec3 *max;
	std::list<IObject*> *listIntersections;
	std::list<IHashString*> *listIntersectionNames;
};

struct BBOXRAYCASTPARAMS
{
	BBOXRAYCASTPARAMS()
	{
  		collisionGroup = -1;
  		objectName = NULL;
  		ray = NULL;
  		listRayCollisions = NULL;
	}

	unsigned int collisionGroup;
	IHashString *objectName;
	Vec3 *ray;
	std::list<RAYCOLLISION> *listRayCollisions;
};

#endif // PHYSICSDEFINES_H
