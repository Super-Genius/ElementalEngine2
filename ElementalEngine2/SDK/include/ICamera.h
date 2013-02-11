///============================================================================
/// \file	ICamera.h
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

#ifndef ICAMERA_H
#define ICAMERA_H

#include "IObject.h"

namespace ElementalEngine
{
class ICamera;
class Vec3;
class CFrustum;
class EulerAngle;

/// CAMERACREATIONPARAMS
typedef struct CAMERACREATIONPARAMS
{
	ICamera * m_CameraComponent;

} CAMERACREATIONPARAMS;

/// CAMERA_PROJECTION
enum CAMERA_PROJECTION { ORTHO_CAMERA = 0 , PERSPECTIVE_CAMERA };

/// ICamera
/// Interface functions for Camera, provides a way to set various camera parameters
class ICamera : public ISceneObject
{
public:
	/// Destructor
	virtual ~ICamera() {};

	/// Build a camera based on a given set of positions
	/// \param postion - vec3 pointer to the camera position
	/// \param upvector - vec3 pointer to the upvector for the camera
	/// \param viewvector - vec3 pointer to the vector the camera should be looking at
	/// \param nearclip - float value indicating the near clip distance
	/// \param farclip - float value indicating the far clip distance
	/// \param FOVangle - float value indicating the field-of-view angle
	virtual void BuildCamera(	Vec3* position, Vec3* upvector, Vec3* viewvector,
								float nearclip, float farclip, float FOVangle ) = 0;

	/// Set the Rotation of the camera
	/// \param x - roll
	/// \param y - pitch
	/// \param z - yaw
	virtual void SetRotation( float x, float y, float z ) = 0;

	/// Set the Rotation of the camera
	/// \param rot - pointer to euler angle rotations
	virtual void SetRotation( EulerAngle* rot ) = 0;

	/// Sets the Position of the camera
	/// \param x - float x position
	/// \param y - float y position
	/// \param z - float z position
	virtual void SetPosition( float x, float y, float z ) = 0;

	/// Sets the Position of the camera
	/// \param pos - pointer to vec3 position
	virtual void SetPosition( Vec3* pos ) = 0;
	
	/// Sets the Look At Vector of the camera, builds the internal view vector
	/// \param x - float x look-at position
	/// \param y - float y look-at position
	/// \param z - float z look-at position
	virtual void SetLookAt( float x, float y, float z ) = 0;

	/// Sets the Look At Vector of the camera, builds the internal view vector
	/// \param lookat - pointer to vec3 look-at position
	virtual void SetLookAt( Vec3* lookat ) = 0;

	/// Sets the Field of View Angle of the camera in Degrees
	/// \param angle - float of the field of view angle in degrees
	virtual void SetFOVAngle( float angle ) = 0;

	/// Sets the clipping planes of the camera in distance relative to position
	/// \param nearclip - float for nearclip distance
	/// \param farclip - float for farclip distance
	virtual void SetClipping( float nearclip, float farclip ) = 0;

	/// Sets the upvector of the camera. Default up vector is (0,0,1)
	/// \param x - float x up vector
	/// \param y - float y up vector
	/// \param z - float z up vector
	virtual void SetUpVector( float x, float y, float z ) = 0;

	/// Sets the upvector of the camera. Default up vector is (0,0,1)
	/// \param upvector - pointer to vec3 up vector
	virtual void SetUpVector( Vec3* upvector ) = 0;

	/// Sets the view vector of the camera
	/// \param x - float x view vector
	/// \param y - float y view vector
	/// \param z - float z view vector
	virtual void SetViewVector( float x, float y, float z ) = 0;

	/// Sets the view vector of the camera
	/// \param viewvector - pointer to vec3 view vector
	virtual void SetViewVector( Vec3* viewvector ) = 0;
	
	/// Gets the view vector of the camera
	/// \return - pointer to vec3 view vector
	virtual const Vec3* GetViewVector( void ) = 0;

	/// Gets the up vector of the camera
	/// \return - pointer to vec3 up vector
	virtual const Vec3* GetUpVector( void ) = 0;

	/// Gets the position of the camera
	/// \return - pointer to vec3 position
	virtual Vec3& GetPosition( void ) = 0;
	
	/// Gets the rotation of the camera
	/// \return - EulerAngle class with the camera's euler angles
	virtual EulerAngle& GetRotation( void ) = 0;

	/// Gets the Near clip distance of the camera
	/// \return - float of the near clip distance
	virtual float GetNearClip( void ) = 0;
	
	/// Gets the Far clip distance of the camera
	/// \return - float of the far clip distance
	virtual float GetFarClip( void ) = 0;

	/// Gets the frustum
	/// \return - pointer to a CFrustum
	virtual CFrustum* GetFrustum( void ) = 0;

	/// Activates this camera as the current camera for the renderer
	virtual void Activate( void ) = 0;

	/// Tells if the object is currently in the hieararchy
	/// \return - true if in the hierarchy
	virtual bool InHierarchy( void ) = 0;	
};

}	//namespace ElementalEngine

#endif