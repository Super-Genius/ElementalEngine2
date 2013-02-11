///============================================================================
/// \file	IParticle.h
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

#ifndef _IPARTICLE_H
#define _IPARTICLE_H


namespace ElementalEngine
{
struct KEYFRAME_DATA;
struct KEYFRAME_DATA_SET;

/// PARTICLE_VARIABLE
/// Important, if you add a new particle property here, be sure to add it to both the following tables.
typedef enum PARTICLE_VARIABLE 
{
	PARTICLE_LIFE = 0,
	PARTICLE_NUMBER,
	PARTICLE_SIZE,
	PARTICLE_VELOCITY,
	PARTICLE_WEIGHT,
	PARTICLE_SPIN,
	PARTICLE_MOTION_RAND,
	PARTICLE_VISIBILITY,
	PARTICLE_LIFE_VARIATION,
	PARTICLE_NUMBER_VARIATION,
	PARTICLE_SIZE_VARIATION,
	PARTICLE_VELOCITY_VARIATION,
	PARTICLE_WEIGHT_VARIATION,
	PARTICLE_SPIN_VARIATION,
	PARTICLE_MOTION_RAND_VARIATION,
	PARTICLE_SIZE_LIFE,
	PARTICLE_VELOCITY_LIFE,
	PARTICLE_WEIGHT_LIFE,
	PARTICLE_SPIN_LIFE,
	PARTICLE_MOTION_RAND_LIFE,
	PARTICLE_COLOR_R,
	PARTICLE_COLOR_G,
	PARTICLE_COLOR_B,
	PARTICLE_COLOR_A,
	PARTICLE_VARIABLE_MAX
} PARTICLE_VARIABLE;

#ifndef PARTICLE_SERIALIZE_NORMAL
	#define PARTICLE_SERIALIZE_NORMAL	3	// 011
#endif
#ifndef PARTICLE_SERIALIZE_DOCKBAR
	#define PARTICLE_SERIALIZE_DOCKBAR	2	// 010
#endif


#define CREATE_PARTICLE_STRING_TABLE(x) const TCHAR * x[ PARTICLE_VARIABLE_MAX ] =\
{	_T("PARTICLE_LIFE"),\
	_T("PARTICLE_NUMBER"),\
	_T("PARTICLE_SIZE"),\
	_T("PARTICLE_VELOCITY"),\
	_T("PARTICLE_WEIGHT"),\
	_T("PARTICLE_SPIN"),\
	_T("PARTICLE_MOTION_RAND"),\
	_T("PARTICLE_VISIBILITY"),\
	_T("PARTICLE_LIFE_VARIATION"),\
	_T("PARTICLE_NUMBER_VARIATION"),\
	_T("PARTICLE_SIZE_VARIATION"),\
	_T("PARTICLE_VELOCITY_VARIATION"),\
	_T("PARTICLE_WEIGHT_VARIATION"),\
	_T("PARTICLE_SPIN_VARIATION"),\
	_T("PARTICLE_MOTION_RAND_VARIATION"),\
	_T("PARTICLE_SIZE_LIFE"),\
	_T("PARTICLE_VELOCITY_LIFE"),\
	_T("PARTICLE_WEIGHT_LIFE"),\
	_T("PARTICLE_SPIN_LIFE"),\
	_T("PARTICLE_MOTION_RAND_LIFE"),\
	_T("PARTICLE_COLOR_R"),\
	_T("PARTICLE_COLOR_G"),\
	_T("PARTICLE_COLOR_B"),\
	_T("PARTICLE_COLOR_A") \
}

/// IParticleType
/// interface to particle types (Inherits from IObject)
class IParticleType : public IObject
{
public:
	/// Destructor
	virtual ~IParticleType() {};

	/// Sets a keyframe of data
	/// \param channel - enum of the channel where keyframe goes
	/// \param data - keyframe data to set
	/// \return - true on success
	virtual bool SetKeyData( PARTICLE_VARIABLE channel, KEYFRAME_DATA_SET &data ) = 0;

	/// Gets a keyframe of data
	/// \param channel - enum of the channel where keyframe comes from
	/// \param data - keyframe data to set
	/// \return - true on success
	virtual bool GetKeyData( PARTICLE_VARIABLE channel, KEYFRAME_DATA_SET &data ) = 0;
	
	/// Sets blending mode for the particle
	/// \param enable - bool true for additive blending, false for normal blending
	/// \return - true on success
	virtual bool SetAdditiveBlend( bool enabled ) = 0;

	/// Gets blending mode of the particle
	/// \return - bool true for additive blending, false for normal blending
	virtual bool GetAdditiveBlend( void ) = 0;

	/// Sets the texture for the particle
	/// \param texture - pointer to ITextureObject interface
	/// \return - true on success
	virtual bool SetTexture( ITextureObject* texture ) = 0;
	
	/// Sets the animated texture subdivisions
	/// \param xdiv - unsigned int for x division amount (default 1)
	/// \param ydiv - unsigned int for y division amount (default 1)
	/// \param numAnimations - unsigned int for number of animations (default 1)
	/// \return - true on success
	virtual bool SetAnimationTextureSubdivisions(	const UINT xdiv = 1, 
													const UINT ydiv = 1, 
													const UINT numAnimations = 1 ) = 0;
	
	/// Sets the framerate for updating this particle type
	/// \param frames - float value indicating particle frame rate
	virtual void SetFrameRate( float frames ) = 0;

	/// Gets the framerate for updating this particle type
	/// \return - float value indicating particle frame rate
	virtual float GetFrameRate( void ) = 0;

	/// Sets the framerate for updating the texture for this particle type
	/// \param fps - float value indicating texture frame rate
	virtual void SetTextureRate( float fps ) = 0;

	/// Gets the framerate for updating the texture for this particle type
	/// \return - float value indicating texture frame rate
	virtual float GetTextureRate( void ) = 0;
	
	/// Gets the name of the texture
	/// \return - pointer to TCHAR of the filename
	virtual const TCHAR* GetTextureFilename( void )=0;

	/// Gets the name of the shader
	/// \return - pointer to TCHAR of the filename
	virtual const TCHAR* GetShaderFilename( void )=0;

	/// Sets the filename of the particle
	/// \param filename - pointer to StdString of the filename
	virtual void SetFilename( StdString* filename ) = 0;

	/// Gets the filename of the particle
	/// \return - pointer to TCHAR of the filename
	virtual const TCHAR* GetFilename( void ) = 0;

	/// Sets the path of the particle
	/// \param filepath - pointer to StdString of the filepath
	virtual void SetFilepath( StdString* filepath ) = 0;	

	/// Gets the path of the particle
	/// \return - pointer to TCHAR of the filename
	virtual const TCHAR* GetFilepath( void ) = 0;
};	

}//namespace ElementalEngine

#endif
