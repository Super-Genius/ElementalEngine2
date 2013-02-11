///==========================================================================
/// \file	DefaultQuaternionImpl.cpp
/// \brief	
/// \date	08/03/2006
/// \author	Kyle Swaim & Joshua Dudley
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
#include "DefaultQuaternionImpl.h"
#include "MathUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define PI 3.14159f

IQuaternionImplementation *CDefaultQuaternionImplementation::s_instance = NULL;

void CDefaultQuaternionImplementation::Init( void )
{
}

void CDefaultQuaternionImplementation::CleanUp( void )
{
}

bool CDefaultQuaternionImplementation::Equals(const Quaternion& a, const Quaternion& b) const
{
	if(a.x != b.x)
		return false;
	if(a.y != b.y)
		return false;
	if(a.z != b.z)
		return false;
	if(a.w != b.w)
		return false;

	return true;
}

Quaternion CDefaultQuaternionImplementation::Add(const Quaternion& a, const Quaternion& b) const
{
	return Quaternion(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w);
}

Quaternion CDefaultQuaternionImplementation::Times(const Quaternion& a, const Quaternion& b) const
{
	Quaternion temp;

	temp.x = (b.w * a.x) + (b.x * a.w) + (b.z * a.y) - (b.y * a.z);
	temp.y = (b.w * a.y) + (b.y * a.w) + (b.x * a.z) - (b.z * a.x);
	temp.z = (b.w * a.z) + (b.z * a.w) + (b.y * a.x) - (b.x * a.y);
	temp.w = (b.w * a.w) - (b.x * a.x) - (b.y * a.y) - (b.z * a.z);

	return Quaternion(temp);
}

void CDefaultQuaternionImplementation::TimesEquals(Quaternion& a, const Quaternion& b)
{
	Quaternion temp;

	temp.x = (b.w * a.x) + (b.x * a.w) + (b.z * a.y) - (b.y * a.z);
	temp.y = (b.w * a.y) + (b.y * a.w) + (b.x * a.z) - (b.z * a.x);
	temp.z = (b.w * a.z) + (b.z * a.w) + (b.y * a.x) - (b.x * a.y);
	temp.w = (b.w * a.w) - (b.x * a.x) - (b.y * a.y) - (b.z * a.z);

	a.x = temp.x;
	a.y = temp.y;
	a.z = temp.z;
	a.w = temp.w;
}

Quaternion CDefaultQuaternionImplementation::Times(const Quaternion& a, float s) const
{
	return Quaternion(s*a.x, s*a.y, s*a.z, s*a.w);
}

void CDefaultQuaternionImplementation::TimesEquals(Quaternion& a, float s)
{
	a.x *= s; 
	a.y *= s; 
	a.z *= s; 
	a.w *= s;
}

Quaternion CDefaultQuaternionImplementation::Divide(const Quaternion& a, const Quaternion& b) const
{
	Quaternion conjB( b );
	conjB.MakeInverse();
	Quaternion temp;

	temp = a * conjB;

	return Quaternion(temp);
}

void CDefaultQuaternionImplementation::DivideEquals(Quaternion& a, const Quaternion& b)
{
	Quaternion conjB( b );
	conjB.MakeInverse();
	Quaternion temp;

	temp = a * conjB;
	
	a.x = temp.x;
	a.y = temp.y;
	a.z = temp.z;
	a.w = temp.w;
}

void CDefaultQuaternionImplementation::Set(Quaternion& quat, float newX, float newY, float newZ, float newW)
{
	quat.x = newX;
	quat.y = newY;
	quat.z = newZ;
	quat.w = newW;
}

float CDefaultQuaternionImplementation::DotProduct(const Quaternion& a, const Quaternion& b) const
{
	return ( (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w) );
}

void CDefaultQuaternionImplementation::Normalize(Quaternion& quat)
{
	float n = quat.x*quat.x + quat.y*quat.y + quat.z*quat.z + quat.w*quat.w;
		
	if (n == 1)
		return; 

	n = 1.0f / sqrtf(n);
	quat.x *= n;
	quat.y *= n;
	quat.z *= n;
	quat.w *= n;
}

void CDefaultQuaternionImplementation::GetMatrix(const Quaternion& quat, Matrix4x4& out) const
{
	out.m[0] = 1.0f - 2.0f*quat.y*quat.y - 2.0f*quat.z*quat.z; 
	out.m[4] = 2.0f*quat.x*quat.y + 2.0f*quat.z*quat.w; 
	out.m[8] = 2.0f*quat.x*quat.z - 2.0f*quat.y*quat.w; 
	out.m[12] = 0.0f;

	out.m[1] = 2.0f*quat.x*quat.y - 2.0f*quat.z*quat.w; 
	out.m[5] = 1.0f - 2.0f*quat.x*quat.x - 2.0f*quat.z*quat.z; 
	out.m[9] = 2.0f*quat.y*quat.z + 2.0f*quat.x*quat.w; 
	out.m[13] = 0.0f;

	out.m[2] = 2.0f*quat.x*quat.z + 2.0f*quat.y*quat.w; 
	out.m[6] = 2.0f*quat.y*quat.z - 2.0f*quat.x*quat.w; 
	out.m[10] = 1.0f - 2.0f*quat.x*quat.x - 2.0f*quat.y*quat.y; 
	out.m[14] = 0.0f;

	out.m[3] = 0.0f; 
	out.m[7] = 0.0f; 
	out.m[11] = 0.0f; 
	out.m[15] = 1.0f;
}

void CDefaultQuaternionImplementation::SetFromMatrix( Quaternion& quat, const Matrix3x3& in )
{
	quat.w = sqrt( 1.0f + in.Get(0,0) + in.Get(1,1) + in.Get(2,2) ) / 2.0f;
	float w4 = (4.0f * quat.w);
	quat.x = ( in.Get(1,2) - in.Get(2,1) ) / w4 ;
	quat.y = ( in.Get(2,0) - in.Get(0,2) ) / w4 ;
	quat.z = ( in.Get(0,1) - in.Get(1,0) ) / w4 ;
}

void CDefaultQuaternionImplementation::SetFromMatrix( Quaternion& quat, const Matrix4x4& in )
{
	quat.w = sqrt( 1.0f + in.Get(0,0) + in.Get(1,1) + in.Get(2,2) ) / 2.0f;
	float w4 = (4.0f * quat.w);
	quat.x = ( in.Get(1,2) - in.Get(2,1) ) / w4 ;
	quat.y = ( in.Get(2,0) - in.Get(0,2) ) / w4 ;
	quat.z = ( in.Get(0,1) - in.Get(1,0) ) / w4 ;
}

void CDefaultQuaternionImplementation::MakeInverse(Quaternion& quat)
{
	quat.x = -quat.x; 
	quat.y = -quat.y; 
	quat.z = -quat.z;
}

Quaternion CDefaultQuaternionImplementation::Slerp(Quaternion a, Quaternion b, float time)
{
	float angle = DotProduct(a, b);
	//handles negative rotation by negating one quat (taking the conjugate of the quat ie. negate the imaginary parts)
	if( angle < 0 )
	{ 
		a.x = -a.x; 
		a.y = -a.y;
		a.z = -a.z;
		a.w = -a.w;
		angle = -angle;
	}
	if (abs(angle) >= 1.0f)
		return a;

	float theta = acos(angle);

	Quaternion temp;
	float sinTheta = sin(theta);
	// if theta * 2 = 180 degrees then result is undefined
	if (abs(sinTheta) < 0.01){
		temp = (a * 0.5) + (b * 0.5);
		return temp;
	}
	float scale = sin(theta * (1.0f-time)) / sinTheta;
	float invscale = sin(theta * time) / sinTheta;
 
	//calculate Quaternion.
	temp = (a * scale) + (b * invscale);
	return temp;
}
	
void CDefaultQuaternionImplementation::ToEuler(const Quaternion& quat, EulerAngle& euler)
{
	float attitude = 0.0f;
	float bank = 0.0f;
	float heading = 0.0f;

	Quaternion qTemp = quat;

	double sqw = qTemp.w * qTemp.w;
    double sqx = qTemp.x * qTemp.x;
    double sqy = qTemp.y * qTemp.y;
    double sqz = qTemp.z * qTemp.z;
	double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	double test = qTemp.x * qTemp.z + qTemp.y * qTemp.w;
	// singularity at north pole
	if( test > 0.499 * unit )
	{ 
		heading = 2 * atan2( qTemp.x, qTemp.w );
		attitude = PI/2;
		bank = 0;
	}
	// singularity at south pole
	else if( test < -0.499 * unit )
	{ 
		heading = -2 * atan2( qTemp.x, qTemp.w );
		attitude = -PI/2;
		bank = 0;
	}
	else
	{
		heading		= (float) atan2( 2.0 * (qTemp.z*qTemp.w - qTemp.x*qTemp.y) , (sqx - sqz - sqy + sqw) );
		attitude	= (float) asin( 2.0 * test/unit );
		bank		= (float) atan2( 2.0 * (qTemp.x*qTemp.w - qTemp.z*qTemp.y) , (-sqx + sqz - sqy + sqw) );
	}

	// Convert back to degrees.
	MathUtil math;
	euler.pitch = math.RadToDeg( bank );
	euler.yaw = math.RadToDeg( attitude );
	euler.roll = math.RadToDeg( heading );
}

// Assuming the angles are in degrees.
void CDefaultQuaternionImplementation::FromEuler(Quaternion& quat, EulerAngle& euler)
{
	//consitant now...
	// pitch yaw roll
	// We need to convert the input euler angle in degrees to radians
	static MathUtil math;
	float bank = math.DegToRad( euler.roll );
	float attitude = math.DegToRad( euler.yaw );
	float heading = math.DegToRad( euler.pitch );

	double c1 = cos( heading/2 );
    double s1 = sin( heading/2 );
    double c2 = cos( attitude/2 );
    double s2 = sin( attitude/2 );
    double c3 = cos( bank/2 );
    double s3 = sin( bank/2 );
    double c1c2 = c1*c2;
    double s1s2 = s1*s2;
    
  	quat.z =(float) ( c1c2*s3 + s1s2*c3 );
	quat.x =(float) ( s1*c2*c3 + c1*s2*s3 );
	quat.y =(float) ( c1*s2*c3 - s1*c2*s3 );
	quat.w =(float) ( c1c2*c3 - s1s2*s3 );
}
