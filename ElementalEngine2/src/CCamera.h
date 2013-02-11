///==========================================================================
/// \file	CCamera.h
/// \brief	Header file for CCamera
///			A camera class for DX9
/// \date	01-10-2006
/// \author Marvin Gouw
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
///==========================================================================

#ifndef	CCamera_H
#define	CCamera_H

#include "Vec3.h"
#include "Matrix3x3.h"
#include "EulerAngle.h"
#include "ICamera.h"

namespace ElementalEngine
{

///==========================================================================
/// \class	CCamera
/// \brief	Camera implementation of IObject interface
///==========================================================================
class CCamera : public CSceneObject<ICamera>
{
private:
	/// \brief The Default Constructor for CCamera
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CCamera(IHashString *parentName, IHashString *name, bool bAddToHier );

public:
	/// \brief	The Default Destructor for CCamera
	~CCamera();

	/// \brief	Create an instance of IObject
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief Init Function
	virtual void Init( void );

	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();

	virtual void UpdateTransform();

	/// \brief	The DeInit Function for the Object
	virtual void DeInit();

	/// Serialize
	virtual void Serialize(IArchive &ar);

	virtual CFrustum * GetFrustum();

	/// Updates all API matrices
	/// \param none
	/// \return none
	void Activate();
	
	
	// INTERFACE FUNCTIONS:
	virtual void BuildCamera( Vec3* position, Vec3* upvector, Vec3* viewvector,
							float nearclip, float farclip, float FOVAngle );
	/// Set the Rotation of the camera
	virtual void SetRotation( float x, float y, float z );
	virtual void SetRotation( EulerAngle * rot );
	/// Sets the Position of the camera
	virtual void SetPosition( float x, float y, float z );
	virtual void SetPosition( Vec3 * pos );
	/// Sets the Look At Vector of the camera, builds the internal view vector
	virtual void SetLookAt( float x, float y, float z);//, Vec3 &orientUp);
	virtual void SetLookAt( Vec3 * lookat );
	/// Sets the Field of View Angle of the camera in Degrees
	virtual void SetFOVAngle( float angle );
	/// Sets the clipping planes of the camera in distance relative to position
	virtual void SetClipping( float nearclip, float farclip );
	/// Sets the upvector of the camera. Default up vector is (0,0,1)
	virtual void SetUpVector( float x, float y, float z );
	virtual void SetUpVector( Vec3 * upvector );
	/// Sets the view vector of the camera
	virtual void SetViewVector( float x, float y, float z );
	virtual void SetViewVector( Vec3 * viewvector );

	/// Gets the view vector of the camera
	virtual const Vec3* GetViewVector();
	/// Gets the up vector of the camera
	virtual const Vec3* GetUpVector();
	/// Gets the position of the camera
	virtual Vec3& GetPosition();
	/// Gets the rotation of the camera
	virtual EulerAngle& GetRotation();
	/// Gets the Near clip distance of the camera
	virtual float GetNearClip();
	/// Gets the Far clip distance of the camera
	virtual float GetFarClip();

	// Messages:

	/// Update or read data from camera
	DWORD OnUpdateCamera(DWORD size, void *in_params);
	/// Set camera as current active camera
	DWORD OnActivateCamera(DWORD size, void *in_params);
	/// Get the camera's view frustum
	DWORD OnGetCameraFrustum(DWORD size, void *in_params);
	/// Move the camera in absolute or relative coordinate space
	DWORD OnMoveCamera(DWORD size, void *in_params);
	/// increment or set the rotation of the camera
	DWORD OnRotateCamera(DWORD size, void *in_params);
	/// increment or set the Look At point of the camera
	DWORD OnSetLookAtPosition(DWORD size, void *in_params);
	/// get the camera's basis vectors (view, up, and right)
	DWORD OnGetCameraVectors(DWORD size, void *in_params);
	/// sets the camera's position
	DWORD OnSetGlobalPosition(DWORD size, void *in_params);
	/// gets the camera's position
	DWORD OnGetGlobalPosition(DWORD size, void *in_params);
	/// sets the camera's orientation
	DWORD OnSetGlobalEuler(DWORD size, void *in_params);
	/// gets the camera's orientation
	DWORD OnGetGlobalEuler(DWORD size, void *in_params);
	/// Removes the camera in and out of a hierarchy without getting bad destructor calls
	/// to remove itself from the hierarchy when it's not in it
	DWORD OnSetCameraHierarchyStatus(DWORD size, void *in_params);
	/// Sets the near clip plane
	DWORD OnSetNearClip(DWORD size, void *param);
	/// Sets the far clip plane
	DWORD OnSetFarClip(DWORD size, void *param);
	/// Gets the camera projection matrix
	DWORD OnGetCameraProjectionMatrix(DWORD size, void *param);
	/// Gets the camera view matrix
	DWORD OnGetCameraViewMatrix(DWORD size, void *param);

	DWORD OnSetProjection(DWORD size, void *in_params);

	DWORD OnGetNearFarClip( DWORD size, void * params );

	DWORD OnObjectInCamera( DWORD size, void * params );

	DWORD OnSetCameraZoom(DWORD size, void *param);

	/// Makes the camera use the backbuffer aspect ratio instead of the window
	DWORD OnSetCameraUseBackBufferAspectRatio(DWORD size, void * in_params );
	bool m_bInHierarchy;

	// override IsKindOf, so we masquerade as CCamera
	virtual bool IsKindOf(IHashString *compType);

	virtual OBJECTSORTTYPES GetSceneSortID() const { return CAMERASORTTYPE; }

private:
	/// Gets the right vector of the camera
	void GetRightVector( Vec3 *right );

	/// Updates the camera view if fixed on an entity
	///	or in a direction
	/// \param none
	/// \return none
	void UpdateFixedView();

	/// Fix camera view on an entity
	/// \param *pObject - the IObject to fix the camera view on
	/// \return none
	void FixViewOnEntity(IObject *pObject);

	/// Unfix view on entity 
	/// \param none
	/// \return none
	void UnFixViewOnEntity();

	/// Fix view on a position
	/// \param vPos - the vector position to look at
	/// \return none
	void FixView(Vec3 &vPos);

	/// Unfix view on position
	/// \param none
	/// \return none
	void UnFixView();
	
	bool InHierarchy();
//	void SetInHierarchy( bool val );

private:
	void UpdateFromVectors();
	void UpdateFromEuler();
	void UpdateFrustum();
	float GetVerticalFOV();
	float GetHorizontalFOV();
	void UpdateMatricies();

	bool m_bFreeze;
	bool m_bViewFixed;
	bool m_bEntityFixed;
	bool m_bUserInput;	
	float m_FOVAngle_H;//horizontal
	float m_FOVAngle_V;//vertical
	float m_fNearClip;
	float m_fFarClip;
	Vec3 m_vPosition;
	Vec3 m_vLookAt;
	Vec3 m_vUpVector;
	Vec3 m_vViewVector;
	EulerAngle m_vEulerAngles;
	CFrustum m_Frustum; //update everytime camera changes
	IObject * m_pObjEntity;
	CAMERA_PROJECTION	m_CamProjection;
	IRenderer * m_Renderer;
	bool m_bUseBackBufferScreenSize;//use backbuffer screen size or window screen size

	Matrix4x4 m_ViewMatrix;
	Matrix4x4 m_ProjectionMatrix;
	bool m_bRebuildMatricies;
	bool m_bUpdateFrustum;

	float m_fZoom;
	float m_fStartZoom;
	float m_fEndZoom;
	float m_fZoomTime;
	float m_fZoomET;
	bool m_bZooming;
	ITimer *m_Timer;
};

} //Namespace EE

#endif	// CCamera_H
