///==========================================================================
/// \file	DefaultEulerImpl.cpp
/// \brief	
/// \date	07/16/2007
/// \author	Marvin Gouw
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

#include "stdafx.h"
#include <math.h>
#include "DefaultEulerImplementation.h"
#include "MathUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define PI 3.14159f

IEulerImplementation *CDefaultEulerImplementation::s_instance = NULL;

void CDefaultEulerImplementation::Init( void )
{
}

void CDefaultEulerImplementation::CleanUp( void )
{
}

bool CDefaultEulerImplementation::Equals(const EulerAngle& a, const EulerAngle& b) const
{
	if(a.roll != b.roll )
		return false;
	if(a.pitch != b.pitch)
		return false;
	if(a.yaw != b.yaw)
		return false;

	return true;
}

EulerAngle CDefaultEulerImplementation::Add(const EulerAngle& a, const EulerAngle& b) const
{
	return EulerAngle(a.roll+b.roll, a.pitch+b.pitch, a.yaw+b.yaw );
}

EulerAngle CDefaultEulerImplementation::Subtract(const EulerAngle& a, const EulerAngle& b) const
{
	return EulerAngle(a.roll-b.roll, a.pitch-b.pitch, a.yaw-b.yaw );
}

EulerAngle CDefaultEulerImplementation::Times(const EulerAngle& a, float s) const
{
	return EulerAngle(s*a.roll, s*a.pitch, s*a.yaw);
}

void CDefaultEulerImplementation::TimesEquals(EulerAngle& a, float s)
{
	a.roll *= s; 
	a.pitch *= s; 
	a.yaw *= s; 
}

void CDefaultEulerImplementation::Set(EulerAngle& euler, float roll, float pitch, float yaw )
{
	euler.roll= roll;
	euler.pitch = pitch;
	euler.yaw = yaw;
}

void CDefaultEulerImplementation::GetMatrix(const EulerAngle& euler, Matrix4x4& out) const
{
	//use math util	
	static MathUtil math;
	math.EulerToMatrix( euler, out );
}

void CDefaultEulerImplementation::GetMatrix(const EulerAngle& euler, Matrix3x3& out) const
{
	//use math util	
	static MathUtil math;
	math.EulerToMatrix( euler, out );
}

void CDefaultEulerImplementation::SetFromMatrix( EulerAngle& euler, const Matrix3x3& in )
{
	//use math util
	static MathUtil math;
	math.MatrixToEuler( in, euler );

}

void CDefaultEulerImplementation::SetFromMatrix( EulerAngle& euler, const Matrix4x4& in )
{
	//use math util
	static MathUtil math;
	math.MatrixToEuler( in, euler );
}

void CDefaultEulerImplementation::MakeInverse(EulerAngle& euler)
{
	euler.roll = -euler.roll; 
	euler.pitch = -euler.pitch; 
	euler.yaw = -euler.yaw;
}
	
void CDefaultEulerImplementation::ToQuaternion(const EulerAngle& euler, Quaternion &quat)
{
	EulerAngle temp;
	temp.roll = euler.roll;
	temp.pitch = euler.pitch;
	temp.yaw = euler.yaw;	
	quat.FromEuler( temp );
}

// Assuming the angles are in degrees.
void CDefaultEulerImplementation::FromQuaternion(EulerAngle& euler, Quaternion &quat)
{
	quat.ToEuler( euler );
	
}
