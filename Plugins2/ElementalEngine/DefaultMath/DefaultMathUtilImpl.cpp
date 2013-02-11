///==========================================================================
/// \file	DefaultMathUtilImpl.cpp
/// \brief	
/// \date	08/03/2006
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

///==========================================================================

#include "stdafx.h"
#include <math.h>
#include <float.h>
#include "DefaultMathUtilImpl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMathUtilImplementation *CDefaultMathUtilImplementation::s_instance = NULL;

// Annoying ugly stuff from vision implementation of matrix to euler conversions
enum MathQuatPart {QUATX, QUATY, QUATZ, QUATW};

#define EulFrmS	     0
#define EulFrmR	     1
#define EulRepNo     0
#define EulRepYes    1
#define EulParEven   0
#define EulParOdd    1
#define EulSafe	     "\000\001\002\000"
#define EulNext	     "\001\002\000\001"
// EulGetOrd unpacks all useful information about order simultaneously.
#define EulGetOrd(ord,i,j,k,h,n,s,f) {unsigned o=ord;f=o&1;o>>=1;s=o&1;o>>=1;\
    n=o&1;o>>=1;i=EulSafe[o&3];j=EulNext[i+n];k=EulNext[i+1-n];h=s?k:i;}

#define EulOrd(i,p,r,f)    (((((((i)<<1)+(p))<<1)+(r))<<1)+(f))

#define EEEulOrdXYZs EulOrd(QUATX,EulParEven,EulRepNo,EulFrmS)
#define EEEulOrdXYXs EulOrd(QUATX,EulParEven,EulRepYes,EulFrmS)
#define EEEulOrdXZYs EulOrd(QUATX,EulParOdd,EulRepNo,EulFrmS)
#define EEEulOrdXZXs EulOrd(QUATX,EulParOdd,EulRepYes,EulFrmS)
#define EEEulOrdYZXs EulOrd(QUATY,EulParEven,EulRepNo,EulFrmS)
#define EEEulOrdYZYs EulOrd(QUATY,EulParEven,EulRepYes,EulFrmS)
#define EEEulOrdYXZs EulOrd(QUATY,EulParOdd,EulRepNo,EulFrmS)
#define EEEulOrdYXYs EulOrd(QUATY,EulParOdd,EulRepYes,EulFrmS)
#define EEEulOrdZXYs EulOrd(QUATZ,EulParEven,EulRepNo,EulFrmS)
#define EEEulOrdZXZs EulOrd(QUATZ,EulParEven,EulRepYes,EulFrmS)
#define EEEulOrdZYXs EulOrd(QUATZ,EulParOdd,EulRepNo,EulFrmS)
#define EEEulOrdZYZs EulOrd(QUATZ,EulParOdd,EulRepYes,EulFrmS)
#define EEEulOrdZYXr EulOrd(X,EulParEven,EulRepNo,EulFrmR)
#define EEEulOrdXYXr EulOrd(X,EulParEven,EulRepYes,EulFrmR)
#define EEEulOrdYZXr EulOrd(X,EulParOdd,EulRepNo,EulFrmR)
#define EEEulOrdXZXr EulOrd(X,EulParOdd,EulRepYes,EulFrmR)
#define EEEulOrdXZYr EulOrd(Y,EulParEven,EulRepNo,EulFrmR)
#define EEEulOrdYZYr EulOrd(Y,EulParEven,EulRepYes,EulFrmR)
#define EEEulOrdZXYr EulOrd(Y,EulParOdd,EulRepNo,EulFrmR)
#define EEEulOrdYXYr EulOrd(Y,EulParOdd,EulRepYes,EulFrmR)
#define EEEulOrdYXZr EulOrd(Z,EulParEven,EulRepNo,EulFrmR)
#define EEEulOrdZXZr EulOrd(Z,EulParEven,EulRepYes,EulFrmR)
#define EEEulOrdXYZr EulOrd(Z,EulParOdd,EulRepNo,EulFrmR)
#define EEEulOrdZYZr EulOrd(Z,EulParOdd,EulRepYes,EulFrmR)

void CDefaultMathUtilImplementation::Init( void )
{
}

void CDefaultMathUtilImplementation::CleanUp( void )
{
}

/// Find the log base 2 of a number
float CDefaultMathUtilImplementation::Log2(float x)
{
	static float k = 1.0f / log(2.0f);
	return (log(x) * k);
}

// Find the log with an arbitrary base of x
/// \param x - the number
/// \param fLogBase - base of the log
/// \return float - Returns the log base fLogBase of x
float CDefaultMathUtilImplementation::LogBase(float x, float fLogBase)
{
	return (log(x) / log(fLogBase));
}

/// Test if an integer is a power of two
/// \param x - integer to test
/// \return bool - result of test, true if x is a power of two and false otherwise
bool CDefaultMathUtilImplementation::IsPowerOfTwo(int x)
{
	if (x < 2)
		return false;

	if (x & (x - 1))
		return false;

	return true;
}

/// Find the integer nearest to a given integer that is a power of two (greater than 0).
/// For integers directly between two powers of two the bias is towards the greater power of two.
/// \param x - the integer
/// \return int - Returns the integer nearest to x that is a power of two
int CDefaultMathUtilImplementation::NearestPowerOfTwo(int x)
{
	if (x <= 2)
		return 2;

	int i = 1<<0;
	int j = 1<<1;
	while (!(x >= i && x <= j))
	{
		i <<= 1;
		j <<= 1;
	}
	if (j-x > x-i)
		return i;
	else
		return j;
}

/// Find the nearest integer greater than a given integer that is a power of two (greater than 0).
/// \param x - the integer
/// \return int - power of two integer
int CDefaultMathUtilImplementation::UpperPowerOfTwo(int x)
{
	if (x <= 2)
		return 2;

	int j = 1<<1;
	while (x > j)
	{
		j <<= 1;
	}
	return j;
}

/// \brief	Convert radians to degrees
/// \param	radValue - The radian angle
/// \return	The value in degrees
float CDefaultMathUtilImplementation::RadToDeg( float radValue )
{
	return (radValue * 360.0f * DIV2PI);
}

/// \brief	Convert degrees to radians
/// \param	degValue - The degree angle
/// \return	The value in radians
float CDefaultMathUtilImplementation::DegToRad( float degValue )
{
	return (degValue * 2 * PI * DIV360);
}


/// \brief	Calculate the arctangent
/// \param	value - Value to find to arctangent of
/// \return	The arctangent of value
float CDefaultMathUtilImplementation::GetATan(float rise, float run)
{
	return (atan2( rise, run ) );	
}

/// \brief	Calculate the tangent
/// \param	value - Value to find to tangent of
/// \return	The tangent of value
float CDefaultMathUtilImplementation::GetTan(float value)
{
	return (tan( value ));
}

/// \brief	Calculate the arccosine
/// \param	value - Value to find to arccosine of
/// \return	The arccosine of value
float CDefaultMathUtilImplementation::GetACos( float value )
{
	return ( acos(value) );
}

/// \brief	Calculate the arcsine
/// \param	value - Value to find to arcsine of
/// \return	The arcsine of value
float CDefaultMathUtilImplementation::GetASin( float value )
{
	return ( asin(value) );
}

/// \brief	Calculate the sine of a radian value
/// \param	radAngle - Radian value to find to sine of
/// \return	The sine of value
float CDefaultMathUtilImplementation::GetSin( float radAngle )
{
	return( sin(radAngle) );
}

/// \brief	Calculate the cosine of a radian value
/// \param	radAngle - Radian value to find to cosine of
/// \return	The cosine of value
float CDefaultMathUtilImplementation::GetCos( float radAngle )
{
	return ( cos(radAngle) );
}

void CDefaultMathUtilImplementation::MatrixLookAt( const Vec3 &vPos, const Vec3 &vLookAt, const Vec3 &vUp, Matrix3x3 &dest )
{
	Vec3 viewVecX, viewVecY, viewVecZ;
	Vec3 UpVector = vUp;
	viewVecZ = vLookAt - vPos;
	if (viewVecZ.LengthSQ() < 0.000001f) // if viewVec is not long enough to generate a non-degenerate matrix
	{
		dest.SetIdentity();
		return;
	}
	// forward direction vector
	viewVecZ.Normalize(); 
	UpVector.Normalize();
	// if dir is the same as assumed up vec (fabsf(viewVecX.z) == fabsf(Vec3(0,0,1).Dot(viewVecX.z))
	float upDotV = UpVector.Dot( viewVecZ );
	if ( fabsf( upDotV ) > .999f) // viewVecX.z is the same thing as (up dot viewVecX) when up is <0,0,1>
	{
		//We really shouldn't process further since we were given an explicit up vector to use
		return;
	}
	viewVecX = UpVector.Cross(viewVecZ);
	viewVecX.Normalize();
	viewVecY  = viewVecZ.Cross(viewVecX);
	viewVecY.Normalize();
	// create roation matrix of the vectors
	Matrix3x3 rotMatrix( viewVecX.x, viewVecX.y, viewVecX.z, 
						 viewVecY.x, viewVecY.y, viewVecY.z, 
						 viewVecZ.x, viewVecZ.y, viewVecZ.z);
	dest = rotMatrix;
}

/// \brief	Generates a rotation matrix
/// \param	vPos = The origin
/// \param	vLookAt = point to look at
void CDefaultMathUtilImplementation::MatrixLookAt( const Vec3 &vPos, const Vec3 &vLookAt, Matrix3x3 &dest )
{
	Vec3 upVec(0,1,0);
	Vec3 viewVec = vLookAt - vPos;
	viewVec.Normalize();
	if (viewVec.y > 0.999f) // same as upVec dot viewVec
	{
		upVec.Set( 0, 0, -1 ); // define a new up vec to use to build the look-at matrix so it is not degenerate
	}
	MatrixLookAt( vPos, vLookAt, upVec, dest );
}

/// \brief	Convert a rotation matrix to radian euler angles
/// \param	matrix - The rotation matrix
/// \param	angles - The array of floats to store the euler angles
void CDefaultMathUtilImplementation::MatrixToRadEuler( const Matrix3x3& matrix, EulerAngle& angles )
{
	float ang[3];
	float M[4][4];

	
	M[0][0] = matrix.m[0];
	M[0][1] = matrix.m[1];
	M[0][2] = matrix.m[2];
	M[1][0] = matrix.m[3];
	M[1][1] = matrix.m[4];
	M[1][2] = matrix.m[5];
	M[2][0] = matrix.m[6];
	M[2][1] = matrix.m[7];
	M[2][2] = matrix.m[8];

    int i,j,k,h,n,s,f;
    EulGetOrd(EEEulOrdYXZs, i, j, k, h, n, s, f);
    if (s==EulRepYes)
	{
		double sy = sqrt(M[i][j]*M[i][j] + M[i][k]*M[i][k]);
		if(sy > 16.f*FLT_EPSILON)
		{
			ang[0] = (float)atan2f(M[i][j], M[i][k]);
			ang[1] = (float)atan2f((float)sy, M[i][i]);
			ang[2] = (float)atan2f(M[j][i], -M[k][i]);
		}
		else
		{
			ang[0] = (float)atan2f(-M[j][k], M[j][j]);
			ang[1] = (float)atan2f((float)sy, M[i][i]);
			ang[2] = 0;
		}
    }
	else
	{
		double cy = sqrt(M[i][i]*M[i][i] + M[j][i]*M[j][i]);
		if(cy > 16.f*FLT_EPSILON)
		{
			ang[0] = (float)atan2f(M[k][j], M[k][k]);
			ang[1] = (float)atan2f(-M[k][i], (float)cy);
			ang[2] = (float)atan2f(M[j][i], M[i][i]);
		}
		else
		{
			ang[0] = (float)atan2f(-M[j][k], M[j][j]);
			ang[1] = (float)atan2f(-M[k][i], (float)cy);
			ang[2] = 0;
		}
    }

    if(n==EulParOdd)
	{
		ang[0] = -ang[0]; ang[1] = - ang[1]; ang[2] = -ang[2];
	}
    if(f==1)
	{
		float t = ang[0]; ang[0] = ang[2]; ang[2] = t;
	}

	//left hand rotation(clockwise around axis)
	angles.roll = ang[2];
	angles.pitch = ang[1];
	angles.yaw = -ang[0];
}

/// \brief	Convert a rotation matrix to degree euler angles
/// \param	matrix - The rotation matrix
/// \param	angles - The array of floats to store the euler angles
void CDefaultMathUtilImplementation::MatrixToEuler( const Matrix3x3& matrix, EulerAngle& angles )
{
	EulerAngle radAngles;
	MatrixToRadEuler( matrix, radAngles );

	angles.roll = RadToDeg(radAngles.roll);
	angles.pitch = RadToDeg(radAngles.pitch);
	angles.yaw = RadToDeg(radAngles.yaw);
}

void CDefaultMathUtilImplementation::MatrixToEuler( const Matrix4x4 &matrix, EulerAngle& angles )
{
	Matrix3x3 rotMat;
	rotMat.SetFrom4x4((float*)matrix.m);
	MatrixToEuler(rotMat, angles);
}

void CDefaultMathUtilImplementation::EulerLookAt( const Vec3 &pos, const Vec3 &lookAt, EulerAngle &angles )
{
	Vec3 vec = lookAt - pos;
//	float len = vec.Length();
//	vec /= len;
//	angles.x = RadToDeg( asin(vec.z) ); // pitch
//	angles.y = 0.0f; // roll
//	angles.z = -RadToDeg( atan2( vec.y, vec.x ) ) - 90; // yaw
	
	VectorToEuler(vec, angles);
}

void CDefaultMathUtilImplementation::VectorToEuler( const Vec3 &direction, EulerAngle &angles )
{
	Vec3 vec = direction;
	vec.Normalize();
	angles.pitch = RadToDeg( asin(vec.y) ); // pitch
	angles.roll = 0.0f; // roll
	angles.yaw = RadToDeg( atan2( -vec.z, vec.x ) ) + 90; // yaw
}

void CDefaultMathUtilImplementation::EulerToVector( const EulerAngle &angles, Vec3 &direction )
{
	direction.y = sin( DegToRad( angles.pitch ) );
	float yawLength = cos( DegToRad( angles.pitch ) );
	direction.z = cos( DegToRad( angles.yaw ) ) * yawLength;
	direction.x = sin( DegToRad( angles.yaw ) ) * yawLength;
}

/// \brief	Convert euler angles in degrees to a rotation matrix
/// \param	angles - The array of floats in degrees
/// \param	matrix - The matrix to store the euler angles
void CDefaultMathUtilImplementation::EulerToMatrix( const EulerAngle& angles, Matrix3x3 &matrix )
{
	EulerAngle anglesRad( DegToRad(angles.roll), DegToRad(angles.pitch), DegToRad(angles.yaw) );
	RadEulerToMatrix( anglesRad, matrix );
}

void CDefaultMathUtilImplementation::EulerToMatrix( const EulerAngle& angles, Matrix4x4& matrix )
{
	EulerAngle anglesRad( DegToRad(angles.roll), DegToRad(angles.pitch), DegToRad(angles.yaw) );
	RadEulerToMatrix( anglesRad, matrix );
}

/// \brief	Convert euler angles in degrees to a rotation matrix
/// \param	angles - The array of floats in radians
/// \param	matrix - The matrix to store the euler angles
void CDefaultMathUtilImplementation::RadEulerToMatrix( const EulerAngle& angles, Matrix4x4 &matrix )
{
	Matrix3x3 matrix3x3;
	RadEulerToMatrix( angles, matrix3x3 );

	matrix.Set(	matrix3x3.m[0],	matrix3x3.m[1],	matrix3x3.m[2],	matrix.m[3],
				matrix3x3.m[3],	matrix3x3.m[4],	matrix3x3.m[5],	matrix.m[7], 
				matrix3x3.m[6],	matrix3x3.m[7],	matrix3x3.m[8],	matrix.m[11],
				matrix.m[12],	matrix.m[13],	matrix.m[14],	matrix.m[15] );
}

/// \brief	Convert euler angles in degrees to a rotation matrix
/// \param	yaw - Yaw in degrees
/// \param	pitch - Pitch in degrees
/// \param	roll - Roll in degrees
/// \param	matrix - The matrix to store the euler angles
void CDefaultMathUtilImplementation::EulerToMatrix( float yaw, float pitch, float roll, Matrix3x3 &matrix )
{
	EulerAngle euler( yaw, pitch, roll );
	EulerToMatrix(euler, matrix);
}

/// \brief	Convert euler angles in radians to a rotation matrix
/// \param	angles - The array of floats in radians
/// \param	matrix - The matrix to store the euler angles
void CDefaultMathUtilImplementation::RadEulerToMatrix( const EulerAngle& angles, Matrix3x3 &matrix )
{
	float M[4][4];
	float ang[3];
	double ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
	int i,j,k,h,n,s,f;

	//left hand rotation(clockwise around axis)
	ang[2] = (angles.roll);
	ang[0] = -angles.yaw;
	ang[1] = (angles.pitch);

	EulGetOrd(EEEulOrdYXZs, i, j, k, h, n, s, f);
	if(f==1)
	{
		double t = (double) ang[0]; ang[0] = ang[2]; ang[2] = (float) t;
	}
	  
	if(n==EulParOdd)
	{
		ang[0] = -ang[0]; ang[1] = -ang[1]; ang[2] = -ang[2];
	}
  
	ti = ang[0];  tj = ang[1];	th = ang[2];
	ci = cos(ti); cj = cos(tj); ch = cos(th);
	si = sin(ti); sj = sin(tj); sh = sin(th);
	cc = ci*ch;   cs = ci*sh;   sc = si*ch;   ss = si*sh;
	  
	if(s==EulRepYes)
	{
		M[i][i] = (float)cj;	  M[i][j] =  (float)(sj*si);    M[i][k] = (float)(sj*ci);
		M[j][i] = (float)(sj*sh);  M[j][j] = (float)(-cj*ss+cc); M[j][k] = (float)(-cj*cs-sc);
		M[k][i] = (float)(-sj*ch); M[k][j] = (float)(cj*sc+cs); M[k][k] = (float)(cj*cc-ss);
	}
	else
	{
		M[i][i] = (float)(cj*ch); M[i][j] = (float)(sj*sc-cs); M[i][k] = (float)(sj*cc+ss);
		M[j][i] = (float)(cj*sh); M[j][j] = (float)(sj*ss+cc); M[j][k] = (float)(sj*cs-sc);
		M[k][i] = (float)(-sj);	 M[k][j] = (float)(cj*si);    M[k][k] = (float)(cj*ci);
	}

	matrix.Set(M[0][0], M[0][1], M[0][2], M[1][0], M[1][1], M[1][2], M[2][0], M[2][1], M[2][2]);
}

/// \brief	Convert euler angles in radians to a rotation matrix
/// \param	yaw - Yaw in radians
/// \param	pitch - Pitch in radians
/// \param	roll - Roll in radians
/// \param	matrix - The matrix to store the euler angles
void CDefaultMathUtilImplementation::RadEulerToMatrix( float yaw, float pitch, float roll, Matrix3x3 &matrix )
{
	EulerAngle euler( yaw, pitch, roll );
	RadEulerToMatrix(euler, matrix);
}

/// \brief	Convert a quaternion to radian euler angles
/// \param	euler - The euler angles to convert
/// \param	quat - The Quaternion to store the result	
void CDefaultMathUtilImplementation::EulerToQuaternion( EulerAngle& euler, Quaternion& quat )
{
	// matrix to quat
	quat.FromEuler( euler );
}

/// \brief	Convert a radian euler angles to quaternion
/// \param	euler - The euler angles to convert
/// \param	quat - The Quaternion to store the result
void CDefaultMathUtilImplementation::RadEulerToQuaternion( EulerAngle& euler, Quaternion& quat )
{
	static MathUtil math;
	EulerAngle degEuler;
	degEuler.roll = math.RadToDeg( euler.roll );
	degEuler.yaw = math.RadToDeg( euler.yaw );
	degEuler.pitch = math.RadToDeg( euler.pitch );
	// matrix to quat
	quat.FromEuler( degEuler );	
}

/// Returns floor of float (float without fractional part)
/// \param value - value to get floor of
/// \return floor of said value
float CDefaultMathUtilImplementation::Floor( float value )
{
	return floor(value);
}

/// Returns ceiling of float (float without fractional part, + 1)
/// \param value - value to get ceiling of
/// \return ceiling of said value
float CDefaultMathUtilImplementation::Ceil( float value )
{
	return  ceil(value);
}
/// Returns absolute value of a float
/// \param value - value to get abs of
/// \return absolute value
float CDefaultMathUtilImplementation::Fabs( float value )
{
	return fabsf(value);
}
/// Returns rounded value
/// \param value - value to round
/// \return rounded value
float CDefaultMathUtilImplementation::Round( float value )
{
	if (value >= 0.0f)
		return floor( value + 0.5f );
	else
		return ceil( value - 0.5f );
}
/// Returns value rounded to a multiple of 'multiple'
/// \param value - value to round
/// \param multiple - multiple to round value to
/// \return value rounded to multiple of 'multiple'
float CDefaultMathUtilImplementation::RoundToNearestMultiple( float value, float multiple )
{
	return Round( value / multiple ) * multiple;
}

/// Returns intersection result of sphere/sphere
/// \param sphereCenterA - center of sphere A
/// \param sphereRadiusA - radius of sphere A
/// \param sphereCenterB - center of sphere B
/// \param sphereRadiusB - radius of sphere B
/// \return intersection result
INTERSECTION_RESULT CDefaultMathUtilImplementation::IntersectSphereSphere( const Vec3 &sphereCenterA, float sphereRadiusA, const Vec3 &sphereCenterB, float sphereRadiusB )
{
	// TODO: optimize
	float d = (sphereCenterA - sphereCenterB).Length();
	if ((d + sphereRadiusA) < sphereRadiusB)
		return INTERSECTION_INSIDE; // a inside b
	else if ((d - sphereRadiusA) > sphereRadiusB)
		return INTERSECTION_OUTSIDE; // a outside b
	else
		return INTERSECTION_PARTIAL;
}
