///============================================================================
/// \file	IController.h
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

#ifndef ICONTROLLER_H
#define ICONTROLLER_H
#include <list>
using namespace std;
namespace ElementalEngine
{
/// IController
/// Interface to allow a Controller class to adjust the movement over other objects
class IController
{
public:
	/// Destructor
	virtual ~IController() {};

	/// Gets the parent controller to this controller
	/// \return - pointer to IController interface
	virtual IController* GetParent( void ) = 0;

	/// Get the list of children controllers of this controller
	/// \return - pointer to list of IController pointers
	virtual list< IController* >* GetChildren( void ) = 0;

	/// creates a double link between parent and child or nothing if a link already exists.
	/// \param surf - pointer to parent IController
	/// \return - true on success
	virtual bool SetParent( IController * surf ) = 0;

	/// creates a double link between parent and child or nothing if one already exists.
	/// \param surf - pointer to child IController
	/// \return - true on success
	virtual bool SetChild( IController * surf ) = 0;

	/// Sets the scaling from the parent
	/// \param scale - vec3 scale value
	virtual void SetScaling( Vec3 &scale ) = 0;
	
	/// Gets the scaling from the parent
	/// \retunr - vec3 of parent scale
	virtual Vec3 GetScaling() = 0;
	
	/// Sets the rotation offset from the parent
	/// \param RotationOffset - EulerAngle rotational offset from parent
	virtual void SetRotationOffset( EulerAngle &RotationOffset ) = 0;

	/// Gets the rotation offset from the parent
	/// \return - EulerAngle rotational offset from parent
	virtual EulerAngle GetRotationOffset() = 0;

	/// Sets the translation offset from the parent
	/// \param TranslationOffset - vec3 translational offset from parent
	virtual void SetTranslationOffset( Vec3 &TranslationOffset ) = 0;

	/// Gets the rotation offset from the parent
	/// \param - vec3 translational offset from parent
	virtual Vec3 GetTranslationOffset() = 0;

	/// recursively builds the rotation based on precalculated parent. Builds rotation for all its children
	/// \return - true on success
    virtual bool BuildRotation( void ) = 0;	
	
	/// Get precalculated rotation matrix
	/// \return - pointer to Matrix4x4
	virtual Matrix4x4 * GetMatrix( void ) = 0;

	/// Set the precalc matrix
	/// \param m - pointer to Matrix4x4
	virtual void SetMatrix(Matrix4x4 * m) = 0;

	/// Removes a child
	/// \param surf - pointer to child IController
	/// \return - true on success
	virtual bool RemoveChild( IController * surf ) = 0;

	/// Removes the child from the parent
	/// \return - true on success
	virtual bool RemoveParent( void ) = 0;

	/// Gets the number of children for this IController
	/// \return - unsigned int of children count
	virtual UINT GetNumChildren( void ) = 0;

	/// releases the IController
	virtual void Release( void ) = 0;
	
	/// recursively builds any linked controllers	
	/// \param c - pointer to IController
	virtual void RecurseBuild( IController * c ) = 0;
};


} //namespace ElementalEngine
#endif