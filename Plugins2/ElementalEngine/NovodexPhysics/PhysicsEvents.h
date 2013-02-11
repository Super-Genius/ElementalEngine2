///==========================================================================
/// \file	PhysicsEvents.h
/// \brief	Structs for all of the various physics events
/// \date	10/24/2007
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

#ifndef	PHYSICSEVENTS_H
#define PHYSICSEVENTS_H

#define SAFEDELETE( arg ) if( arg ){ delete arg; arg = NULL; }
#define SAFEARRAYDELETE( arg ) if ( arg ){ delete[] arg; arg = NULL; }

// CPhysicsManager events
struct DELETEPHYSICSEVENT
{
	DELETEPHYSICSEVENT()
	{
		actor = NULL;
		joint = NULL;
		cloth = NULL;
	}

	CPhysicsActor* actor;
	CPhysicsJoint* joint;
	CPhysicsCloth* cloth;
};

struct CREATEPHYSICSEVENT
{
	CREATEPHYSICSEVENT()
	{
		actorDesc = NULL;
		jointDesc = NULL;
		clothDesc = NULL;
		physObject = NULL;
	}

	NxActorDesc* actorDesc;
	NxJointDesc* jointDesc;
	NxClothDesc* clothDesc;
	CPhysicsObject* physObject;
};

struct LOADPHYSICSEVENT
{
	LOADPHYSICSEVENT()
	{
		fileName = NULL;
		hasCCD = false;
		physObject = NULL;
	}
	const TCHAR* fileName;
	bool hasCCD;
	CPhysicsObject* physObject;
};

// CPhysicsObject Events
struct ENABLEGRAVITYEVENT
{
	ENABLEGRAVITYEVENT()
	{
	}
};

struct DISABLEGRAVITYEVENT
{
	DISABLEGRAVITYEVENT()
	{
	}
};

struct SETDYNAMICEVENT
{
	SETDYNAMICEVENT()
	{
	}
};

struct SETKINEMATICEVENT
{
	SETKINEMATICEVENT()
	{
	}
};

struct SETCOLLISIONGROUPEVENT
{
	SETCOLLISIONGROUPEVENT()
	{
	}

	int group;
};

struct SETTRIGGEREVENT
{
	SETTRIGGEREVENT()
	{
		triggerParams = NULL;
	}

	~SETTRIGGEREVENT()
	{
		SAFEDELETE( triggerParams );
	}

	TRIGGERPARAMS* triggerParams;
};

struct APPLYFORCEEVENT
{
	APPLYFORCEEVENT()
	{
	}

	Vec3 force;
};

struct APPLYLOCALFORCEEVENT
{
	APPLYLOCALFORCEEVENT()
	{
	}

	Vec3 force;
};

struct APPLYTORQUEEVENT
{
	APPLYTORQUEEVENT()
	{
	}

	Vec3 torque;
};

struct APPLYLOCALTORQUEEVENT
{
	APPLYLOCALTORQUEEVENT()
	{
	}

	Vec3 torque;
};

struct SETLINEARVELOCITYEVENT
{
	SETLINEARVELOCITYEVENT()
	{
	}

	Vec3 velocity;
};

struct SETANGULARVELOCITYEVENT
{
	SETANGULARVELOCITYEVENT()
	{
	}

	Vec3 velocity;
};

struct SETLINEARMOMENTUMEVENT
{
	SETLINEARMOMENTUMEVENT()
	{
	}

	Vec3 momentum;
};

struct SETANGULARMOMENTUMEVENT
{
	SETANGULARMOMENTUMEVENT()
	{
	}

	Vec3 momentum;
};

struct SETLINEARDAMPINGEVENT
{
	SETLINEARDAMPINGEVENT()
	{
	}

	float damping;
};

struct SETANGULARDAMPINGEVENT
{
	SETANGULARDAMPINGEVENT()
	{
	}

	float damping;
};

struct LOCKTRANSLATIONAXISEVENT
{
	LOCKTRANSLATIONAXISEVENT()
	{
		lockAxies = NULL;
	}

	LOCKAXIES* lockAxies;
};

struct LOCKROTATIONAXISEVENT
{
	LOCKROTATIONAXISEVENT()
	{
		lockAxies = NULL;
	}

	LOCKAXIES* lockAxies;
};

struct ENABLEPHYSICSANIMATIONSEVENT
{
	ENABLEPHYSICSANIMATIONSEVENT()
	{
	}
};

struct SETHIERARCHALDATAEVENT
{
	SETHIERARCHALDATAEVENT()
	{
		skeletonInfo = NULL;
	}

	SKELETONINFO* skeletonInfo;
};

struct SETACTORVELOCITYBYNAMEEVENT
{
	SETACTORVELOCITYBYNAMEEVENT()
	{
		actorForce = NULL;
	}

	ACTORFORCE* actorForce;
};

struct BREAKALLJOINTSEVENT
{
	BREAKALLJOINTSEVENT()
	{
	}
};

struct BREAKJOINTEVENT
{
	BREAKJOINTEVENT()
	{
		jointName = NULL;
	}

	IHashString* jointName;
};

// CEECharacterPhysicsobject Events
struct MOVECHARACTEREVENT
{
	MOVECHARACTEREVENT()
	{
	}

	Vec3 newVelocity;
};

struct SETCHARACTERPOSITIONEVENT
{
	SETCHARACTERPOSITIONEVENT()
	{
	}

	Vec3 newPosition;
};

struct SETCHARACTERHEIGHTEVENT
{
	SETCHARACTERHEIGHTEVENT()
	{
	}

	float newHeight;
};

struct SETCHARACTERWIDTHEVENT
{
	SETCHARACTERWIDTHEVENT()
	{
	}

	float newWidth;
};

struct SETLOCALPOSITIONEVENT
{
	SETLOCALPOSITIONEVENT()
	{
	}

	Vec3 newLocalPosition;
};

struct LOCKHORIZONTALAXIESEVENT
{
	LOCKHORIZONTALAXIESEVENT()
	{
	}
};

struct UNLOCKHORIZONTALAXIESEVENT
{
	UNLOCKHORIZONTALAXIESEVENT()
	{
	}
};

#endif //PHYSICSEVENTS_H