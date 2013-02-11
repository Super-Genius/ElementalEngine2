///============================================================================
/// \file	IParticleEmitter.h
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

#ifndef _IPARTICLEEMITTER_H
#define _IPARTICLEEMITTER_H

namespace ElementalEngine
{

/// PARTICLE_EMITTER_VARIABLE
/// Important, if you add a new particle property here, be sure to add it to both the following tables;
enum PARTICLE_EMITTER_VARIABLE
{
	EMITTER_LIFE = 0,
	EMITTER_NUMBER,
	EMITTER_SIZE,
	EMITTER_VELOCITY,
	EMITTER_WEIGHT,
	EMITTER_SPIN,
	EMITTER_MOTION_RAND,
	EMITTER_ZOOM,
	EMITTER_VISIBILITY,
	EMITTER_TINT_STRENGTH,
	EMITTER_EMISSION_ANGLE,
	EMITTER_EMISSION_RANGE,
	EMITTER_ACTIVE,
	EMITTER_ANGLE,
	EMITTER_VARIABLE_MAX
};

#ifndef PARTICLE_SERIALIZE_NORMAL
	#define PARTICLE_SERIALIZE_NORMAL	3	// 011
#endif
#ifndef PARTICLE_SERIALIZE_DOCKBAR
	#define PARTICLE_SERIALIZE_DOCKBAR	2	// 010
#endif

/// CREATE_EMITTER_STRING_TABLE
#define CREATE_EMITTER_STRING_TABLE(x) const TCHAR * x[ EMITTER_VARIABLE_MAX ] = \
{\
	_T("EMITTER_LIFE"),\
	_T("EMITTER_NUMBER"),\
	_T("EMITTER_SIZE"),\
	_T("EMITTER_VELOCITY"),\
	_T("EMITTER_WEIGHT"),\
	_T("EMITTER_SPIN"),\
	_T("EMITTER_MOTION_RAND"),\
	_T("EMITTER_ZOOM"),\
	_T("EMITTER_VISIBILITY"),\
	_T("EMITTER_TINT_STRENGTH"),\
	_T("EMITTER_EMISSION_ANGLE"),\
	_T("EMITTER_EMISSION_RANGE"),\
	_T("EMITTER_ACTIVE"),\
	_T("EMITTER_ANGLE")\
}

/// IParticleEmitter
/// Interface for a particle emitter (inherits from IObject)
class IParticleEmitter : public IObject
{
public:
	/// Destructor
	virtual ~IParticleEmitter() {};

	/// Sets a keyframe of data
	/// \param channel - enum of the channel where keyframe goes
	/// \param data - keyframe data to set
	/// \return - true on success
	virtual bool SetKeyData( PARTICLE_EMITTER_VARIABLE channel, KEYFRAME_DATA_SET &data ) = 0;

	/// Gets a keyframe of data
	/// \param channel - enum of the channel where keyframe comes from
	/// \param data - keyframe data to set
	/// \return - true on success
	virtual bool GetKeyData( PARTICLE_EMITTER_VARIABLE channel, KEYFRAME_DATA_SET &data ) = 0;

	/// Adds a ParticleBrush to this ParticleEmitter
	/// \param particle - pointer to IParticleType interface
	virtual void AddParticleBrush( IParticleType* particle ) = 0;

	/// Removes a ParticleBrush to this ParticleEmitter
	/// \param particle - pointer to IParticleType interface
	virtual void RemoveBrush( IParticleType* particle ) = 0;

	/// Sets the visibility of the brush
	/// \param particle - pointer to IParticleType interface
	/// \param visible - bool indicating the visibility of the brush
	virtual void SetBrushVisibility( IParticleType* particle, bool visible ) = 0;

	/// Moves the brush up/down in the stack of brushes
	/// \param bUp - bool indicating up (true) or down (false)
	/// \param index - unsigned int indicating the amount to move
	virtual void MoveBrush( bool bUp, UINT index ) = 0;

	/// Gets the number of brushes 
	/// \return - unsigned int of the number of brushes
	virtual UINT GetNumBrushes( void ) = 0;
	
	/// Gets a specific brush
	/// \param index - unsigned int of the index to lookup
	/// \return - pointer to IParticleType interface
	virtual IParticleType * GetBrush( UINT index ) = 0;

	/// Sets the tint on the emitter
	/// \param r - float red component
	/// \param g - float green component
	/// \param b - float blue component
	/// \param a - float alpha component
	virtual void SetTint( float r, float g, float b, float a ) = 0;

	/// Gets the tint on the emitter
	/// \param tintout - array of 4 floats (R G B A) indiciating the ting
	virtual void GetTint(float tintout[]) = 0;
	
	/// Sets the filename for this emitter
	/// \param filename - pointer to StdString of the filename
	virtual void SetFilename( StdString* filename ) = 0;

	/// Gets the filename for this emitter
	/// \return - pointer to TCHAR of the filename string
	virtual const TCHAR* GetFilename( void ) = 0;
};	

}//namespace ElementalEngine

#endif
