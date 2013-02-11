///============================================================================
/// \file	IParticleObject.h
/// \date	5/22/2008
/// \author	Brendan Geiger
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

#ifndef _IPARTICLEOBJECT_H
#define _IPARTICLEOBJECT_H
#include "CParticleData.h"

namespace ElementalEngine
{

class IParticleObject : public IRenderObject
{
public:
	virtual ~IParticleObject() {};

	/// \brief Return the number of particle elements active in a given particle object.
	/// \return unsigned int of number of active particles in that particle object.
	virtual unsigned int GetNumParticles () = 0;

	// TODO: This is likely the slowest way imaginable to do this. A faster way would,
	// be to share a built structure that contains the position and rotation of all
	// indicies to copy. Fastest way would be to allow particles to snoop eachother's data.

	/// \brief Interface to alow ParticleLinks to spawn particles in other systems.
	/// \return int of index of successful spawn, -1 for failed spawn
	virtual int LinkSpawnParticle( Vec3 pos, EulerAngle rot ) = 0;
	
	/// \brief Return the number of particle elements active in a given particle object.
	/// \return bool on successful move.
	virtual bool LinkMoveParticle( int index, Vec3 pos, EulerAngle rot ) = 0;

	/// \brief Return the number of particle elements active in a given particle object.
	/// \return bool on successful move.
	virtual bool LinkRemoveParticle( int index, Vec3 pos, EulerAngle rot ) = 0;

	/// \brief Get a constant reference to the array of basic particle params:
	/// \ position, rotation
	/// \return SLocation* of array.
	virtual SParticleBase& LinkGetParticleDataArray( ) const = 0;

};

}	//namespace ElementalEngine

#endif // _IPARTICLEOBJECT_H