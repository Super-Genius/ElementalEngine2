///============================================================================
/// \file	ILightObject.h
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

#ifndef _ILIGHTOBJECT_H
#define _ILIGHTOBJECT_H

namespace ElementalEngine
{
/// LIGHTOBJECTYPE
/// Types: DIRECTIONAL_LIGHT, SPOT_LIGHT, OMNI_POINT_LIGHT, HEMI_POINT_LIGHT, AMBIENT_LIGHT
enum LIGHTOBJECTTYPE { DIRECTIONAL_LIGHT, SPOT_LIGHT, OMNI_POINT_LIGHT, HEMI_POINT_LIGHT, AMBIENT_LIGHT };

/// ILightObject
/// Interface for light objects (inherits from IRenderObject)
class ILightObject : public IRenderObject
{
public:
	/// Destructor
	virtual ~ILightObject() {};

	/// Sets the color of the light
	/// \param r - float red value
	/// \param g - float green value
	/// \param b - float blue value
	virtual void SetColor( float r, float g, float b ) = 0;

	/// Sets the intensity of the light
	/// \param i - float intensity of the light
	virtual void SetIntensity( float i ) = 0;

	/// Sets the attenuation distance of the light
	/// \param distance - float of the atten distance
	virtual void SetAttenuationDistance( float distance ) = 0;	

	/// Sets the direction vector
	/// \param x - float x direction
	/// \param y - float y position
	/// \param z - float z position
	virtual void SetVector( float x, float y, float z ) = 0;

	/// Gets the color of the light
	/// \param r - array of 3 float values (R G B)
	virtual void GetColorIntensity( float r[] ) = 0;    

	/// Gets the intensity of the light
	/// \return - float
	virtual float GetIntensity( void ) = 0;

	/// Gets the direction vector
	/// \param v - array of 3 float values (X Y Z)
	virtual void GetVectorAttenuation( float v[] ) = 0;

	/// Gets the attenuation distance
	/// \return - float
	virtual float GetAttenuationDistance( void ) = 0;

	/// Gets the position of the light
	/// \param p - Vec3 of the position to be filled in
	virtual void GetPosition( Vec3 &p ) = 0;

	/// Gets the direction of the light
	/// \param p - Vec3 of the direction to be filled in
	virtual void GetDirection( Vec3 &p ) = 0;

	/// Sets the light type
	/// \param type - enum of the light types
	virtual void SetLightType( LIGHTOBJECTTYPE type )= 0;

	/// Gets the light type
	/// \return - enum of the light types
	virtual LIGHTOBJECTTYPE GetLightType( void )= 0;

	/// Enables shadows cast by this light
	/// \param enabled - bool indicating what to set this light's shadow cast flag
	virtual void EnableShadows( bool enabled ) = 0;

	/// Returns the status of the EnableShadows flag
	/// \return - true if this light is enable to can cast shadows
	virtual bool CanCastShadows( void ) = 0;

	/// Gets the Shadow Attenuation value
	/// \return - float value of shadow atten
	virtual float GetShadowAttenuation( void ) = 0;

	/// Returns the status of this light as a detail light
	/// \return - bool true if this is a detail light
	virtual bool IsDetailLight( void ) = 0;

	/// Gets the lighting priority of this light
	/// \param objectPos - vec3 position of the object this light could be shining on
	/// \param objectRadius - float value indicating the objects size of sphere of influence
	/// \param out_priority - float value indicating the weight value of priority for the light
	/// \return - bool indicating if the light affects the object at all
	virtual bool GetLightPriority( const Vec3 &objectPos, float objectRadius, float &out_priority ) = 0;
};	

}//namespace ElementalEngine

#endif
