///============================================================================
/// \file	CRaycastReport.h
/// \brief	Implementations of Novodex's raycast interfaces
///			Only for use with physics
/// \date	7-7-2005
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
///============================================================================

#ifndef CRAYCASTREPORT_H
#define CRAYCASTREPORT_H

///============================================================================
/// \class	CRaycastReport
/// \brief	Implementation of Novodex's NxUserRaycastReport interface
///============================================================================
class CRaycastReport : public NxUserRaycastReport
{
public:
	/// Construct from RAYCASTLISTPARAMS.
	/// \param params
	CRaycastReport(RAYCASTLISTPARAMS *params)
	{
		message = params;
	}

	/// Callback when the ray hits a physics object.
	virtual bool onHit(const NxRaycastHit& hit)
	{
		// Get the CPhysicsObject from the user data
   		NxActor *actor = &hit.shape->getActor();

		// The object hit by the raycast has been deleted but not yet removed from the simulation
		if( actor->userData == NULL )
		{
			return true;
		}

		// Get the CPhysicsObject from the user data
		PHYSICSUSERDATA* userData = (PHYSICSUSERDATA*)hit.shape->getActor().userData;
		CPhysicsObject* physObj = userData->physObj;
		CPhysicsActor* physActor = userData->physActor;

		if( physObj != NULL )
		{
			// Send a message to the object with the collision data
			RAYCOLLISION collisionReport;
			collisionReport.distance = hit.distance;
			collisionReport.hitPoint[0] = hit.worldImpact.x;
			collisionReport.hitPoint[1] = hit.worldImpact.y;
			collisionReport.hitPoint[2] = hit.worldImpact.z;
			collisionReport.normal[0] = hit.worldNormal.x;
			collisionReport.normal[1] = hit.worldNormal.y;
			collisionReport.normal[2] = hit.worldNormal.z;
			collisionReport.object = physObj->GetName();
			collisionReport.actor = physActor->GetName();
			collisionReport.objectType = physObj->GetComponentType();
			collisionReport.parentobject = physObj->GetParentName();

			message->listRayCollisions->push_back( collisionReport );	
		}

		return true;
	}

	// the raycast message revieved from the ratrace function
	RAYCASTLISTPARAMS *message;
};

///============================================================================
/// \class	CEntityReport
/// \brief	Implementation of Novodex's NxUserRaycastReport interface
///============================================================================
class CEntityReport : public NxUserEntityReport<NxShape*>
{
public:
	/// Construct from AABBINTERSECTPARAMS.
	/// \param params = intersection params.
	CEntityReport(AABBINTERSECTPARAMS *params)
	{
		listIntersections = params->listIntersections;
		listIntersectionNames = params->listIntersectionNames;
	}

	/// Callback on the aabb intersection test results.
	/// \param nbShapes = The number of shapes intersecting the aabb.
	/// \param shapes = The list of intersecting shapes.
	virtual bool onEvent(NxU32 nbShapes, NxShape** shapes)
	{
		CPhysicsObject *physObj;

		while(nbShapes--)
		{
			bool duplicate = false;

			// Get the CPhysicsObject from the user data
			//physObj = (CPhysicsObject*)shapes[nbShapes]->getActor().userData;
			PHYSICSUSERDATA* userData = (PHYSICSUSERDATA*)shapes[nbShapes]->getActor().userData;
			physObj = userData->physObj;

			// Make sure that we don't return the same actor multiple times
			if( listIntersections != NULL || listIntersectionNames != NULL )
			{
				if( listIntersections != NULL )
				{
					std::list<IObject*>::iterator iter;

					for( iter = listIntersections->begin(); iter != listIntersections->end(); iter++ )
					{
						if( (*iter)->GetName() == physObj->GetName() )
							duplicate = true;
					}
				}
				if( listIntersectionNames != NULL )
				{
					std::list<IHashString*>::iterator iter;

					for( iter = listIntersectionNames->begin(); iter != listIntersectionNames->end(); iter++ )
					{
						if( (*iter) == physObj->GetParentName() )
							duplicate = true;
					}
				}
			}
			
			if( !duplicate )
			{
				if( listIntersections != NULL )
					listIntersections->push_back( physObj );
				if( listIntersectionNames != NULL )
					listIntersectionNames->push_back( physObj->GetParentName() );
			}
		}

		return true;
	}
	
	/// List of intersecting objects.
	std::list<IObject*> *listIntersections;

	/// List of intersecting object names.
	std::list<IHashString*> *listIntersectionNames;
};

#endif	// CRAYCASTREPORT_H