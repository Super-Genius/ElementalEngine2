///==========================================================================
/// \file	DefaultMatrix3x3Impl.cpp
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
#include "MathUtils.h"
#include "DefaultMatrix3x3Impl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMatrix3x3Implementation *CDefaultMatrix3x3Implementation::s_instance = NULL;

void CDefaultMatrix3x3Implementation::Init( void )
{
}

void CDefaultMatrix3x3Implementation::CleanUp( void )
{
}

void CDefaultMatrix3x3Implementation::Set( Matrix3x3& matrix,
										   float m00, float m01, float m02, 
										   float m10, float m11, float m12, 
										   float m20, float m21, float m22 )
{
	matrix.m[0] = m00;
	matrix.m[1] = m01;
	matrix.m[2] = m02;
	matrix.m[3] = m10;
	matrix.m[4] = m11;
	matrix.m[5] = m12;
	matrix.m[6] = m20;
	matrix.m[7] = m21;
	matrix.m[8] = m22;
}

void CDefaultMatrix3x3Implementation::SetFrom3x3( Matrix3x3& matrix, float* newM )
{
	matrix.m[0] = newM[0];
	matrix.m[1] = newM[1];
	matrix.m[2] = newM[2];
	matrix.m[3] = newM[3];
	matrix.m[4] = newM[4];
	matrix.m[5] = newM[5];
	matrix.m[6] = newM[6];
	matrix.m[7] = newM[7];
	matrix.m[8] = newM[8];
}

void CDefaultMatrix3x3Implementation::SetFrom4x4( Matrix3x3& matrix, float* newM )
{
	matrix.m[0] = newM[0];
	matrix.m[1] = newM[1];
	matrix.m[2] = newM[2];
	matrix.m[3] = newM[4];
	matrix.m[4] = newM[5];
	matrix.m[5] = newM[6];
	matrix.m[6] = newM[8];
	matrix.m[7] = newM[9];
	matrix.m[8] = newM[10];
}

void CDefaultMatrix3x3Implementation::SetFromAxisAngle( Matrix3x3& matrix, const Vec3& axis, float angle )
{
	angle *= 0.01745329252f; // degrees to radians

	double mag, s, c;
	double xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;
	double x, y, z;

	s = sinf( angle );
	c = cosf( angle );

	mag = axis.Length();

	if( mag <= 0 ) 
	{
		matrix.SetIdentity();
		return;
	}

	x = axis.x/mag;
	y = axis.y/mag;
	z = axis.z/mag;

	xx = x * x;
	yy = y * y;
	zz = z * z;
	xy = x * y;
	yz = y * z;
	zx = z * x;
	xs = x * s;
	ys = y * s;
	zs = z * s;
	one_c = 1.0f - c;

	matrix.m[0] = (float)((one_c * xx) + c);
	matrix.m[1] = (float)((one_c * xy) - zs);
	matrix.m[2] = (float)((one_c * zx) + ys);

	matrix.m[3] = (float)((one_c * xy) + zs);
	matrix.m[4] = (float)((one_c * yy) + c);
	matrix.m[5] = (float)((one_c * yz) - xs);

	matrix.m[6] = (float)((one_c * zx) - ys);
	matrix.m[7] = (float)((one_c * yz) + xs);
	matrix.m[8] = (float)((one_c * zz) + c);
}

Vec3 CDefaultMatrix3x3Implementation::GetAxisX(const Matrix3x3& matrix) const
{
	Vec3 tmpVec;
	tmpVec.Set( matrix.m[0], matrix.m[1], matrix.m[2] );
	return tmpVec;
}

Vec3 CDefaultMatrix3x3Implementation::GetAxisY(const Matrix3x3& matrix) const
{
	Vec3 tmpVec;
	tmpVec.Set( matrix.m[3], matrix.m[4], matrix.m[5] );
	return tmpVec;
}

Vec3 CDefaultMatrix3x3Implementation::GetAxisZ(const Matrix3x3& matrix) const
{
	Vec3 tmpVec;
	tmpVec.Set( matrix.m[6], matrix.m[7], matrix.m[8] );
	return tmpVec;
}

bool CDefaultMatrix3x3Implementation::Equals(const Matrix3x3& a, const Matrix3x3& b) const 
{
	for (int i = 0; i < 9; ++i)
		if (a.m[i] != b.m[i])
			return false;

	return true;
}

bool CDefaultMatrix3x3Implementation::NotEquals(const Matrix3x3& a, const Matrix3x3& b) const 
{
	for (int i = 0; i < 9; ++i)
		if (a.m[i] == b.m[i])
			return false;

		return true;
}

Matrix3x3 CDefaultMatrix3x3Implementation::Times(const Matrix3x3& a, const Matrix3x3& b) const
{
	Matrix3x3 tmtrx;
	const float *m1 = a.m, *m2 = b.m;
	float *m3 = tmtrx.m;

	m3[0] = m1[0]*m2[0] + m1[3]*m2[1] + m1[6]*m2[2];
	m3[1] = m1[1]*m2[0] + m1[4]*m2[1] + m1[7]*m2[2];
	m3[2] = m1[2]*m2[0] + m1[5]*m2[1] + m1[8]*m2[2];
	
	m3[3] = m1[0]*m2[3] + m1[3]*m2[4] + m1[6]*m2[5];	
	m3[4] = m1[1]*m2[3] + m1[4]*m2[4] + m1[7]*m2[5];
	m3[5] = m1[2]*m2[3] + m1[5]*m2[4] + m1[8]*m2[5];
	
	m3[6] = m1[0]*m2[6] + m1[3]*m2[7] + m1[6]*m2[8];
	m3[7] = m1[1]*m2[6] + m1[4]*m2[7] + m1[7]*m2[8];	
	m3[8] = m1[2]*m2[6] + m1[5]*m2[7] + m1[8]*m2[8];
		
	return tmtrx;
}

Matrix3x3& CDefaultMatrix3x3Implementation::TimesEquals(Matrix3x3& a, const Matrix3x3& b) const
{
	float newMatrix[16];
	const float *m1 = a.m, *m2 = b.m;

	newMatrix[0] = m1[0]*m2[0] + m1[3]*m2[1] + m1[6]*m2[2];
	newMatrix[1] = m1[1]*m2[0] + m1[4]*m2[1] + m1[7]*m2[2];
	newMatrix[2] = m1[2]*m2[0] + m1[5]*m2[1] + m1[8]*m2[2];
	
	newMatrix[3] = m1[0]*m2[3] + m1[3]*m2[4] + m1[6]*m2[5];	
	newMatrix[4] = m1[1]*m2[3] + m1[4]*m2[4] + m1[7]*m2[5];
	newMatrix[5] = m1[2]*m2[3] + m1[5]*m2[4] + m1[8]*m2[5];
	
	newMatrix[6] = m1[0]*m2[6] + m1[3]*m2[7] + m1[6]*m2[8];
	newMatrix[7] = m1[1]*m2[6] + m1[4]*m2[7] + m1[7]*m2[8];	
	newMatrix[8] = m1[2]*m2[6] + m1[5]*m2[7] + m1[8]*m2[8];
		
	for (int i=0; i<9; ++i)
		a.m[i] = newMatrix[i];

	return (a);
}

bool CDefaultMatrix3x3Implementation::IsIdentity(Matrix3x3& matrix) 
{
	for (int i=0; i<3; ++i)
		for (int j=0; j<3; ++j)
			if (j != i)
			{
				//if ((*this)(i,j) < -0.0000001f || (*this)(i,j) >  0.0000001f)
					return false;
			}
			else
			{
				//if ((*this)(i,j) < 0.9999999f || (*this)(i,j) > 1.0000001f)
					return false;
			}
			
			return true;
}

void CDefaultMatrix3x3Implementation::SetRotation(Matrix3x3& matrix, const EulerAngle& euler)
{
	static MathUtil math;
	math.EulerToMatrix( euler, matrix );
}

bool CDefaultMatrix3x3Implementation::SetInverse(Matrix3x3& matrix) 
{
	Matrix3x3 temp( matrix );
	return GetInverse( temp, matrix );
}

bool CDefaultMatrix3x3Implementation::GetInverse(Matrix3x3& in, Matrix3x3& out) 
{
	float d = in.m[0] * ( in.m[4]*in.m[8] - in.m[7]*in.m[5] )
            - in.m[1] * ( in.m[3]*in.m[8] - in.m[6]*in.m[5] )
            + in.m[2] * ( in.m[3]*in.m[7] - in.m[6]*in.m[4] );
		
	if (d == 0.f)
		return false;

	d = 1.f / d;

	out.m[0] =  ( in.m[4]*in.m[8] - in.m[5]*in.m[7] ) * d;
    out.m[1] = -( in.m[1]*in.m[8] - in.m[7]*in.m[2] ) * d;
    out.m[2] =  ( in.m[1]*in.m[5] - in.m[4]*in.m[2] ) * d;

    out.m[3] = -( in.m[3]*in.m[8] - in.m[5]*in.m[6] ) * d;
    out.m[4] =  ( in.m[0]*in.m[8] - in.m[6]*in.m[2] ) * d;
    out.m[5] = -( in.m[0]*in.m[5] - in.m[3]*in.m[2] ) * d;

    out.m[6] =  ( in.m[3]*in.m[7] - in.m[6]*in.m[4] ) * d;
    out.m[7] = -( in.m[0]*in.m[7] - in.m[6]*in.m[1] ) * d;
    out.m[8] =  ( in.m[0]*in.m[4] - in.m[1]*in.m[3] ) * d;

	return true;
}
/*
void CDefaultMatrix3x3Implementation::Normalize(Matrix3x3& matrix) 
{
	this function is written for a 4x4 not a 3x3 so it will currently overwrite data and not work as advertised.

	float newMatrix[16];

	newMatrix[0] = matrix.m[0]*matrix.m[0] + matrix.m[4]*matrix.m[4] + matrix.m[8]*matrix.m[8] ;
    newMatrix[1] = matrix.m[1]*matrix.m[1] + matrix.m[5]*matrix.m[5] + matrix.m[9]*matrix.m[9] ;
    newMatrix[2] = matrix.m[2]*matrix.m[2] + matrix.m[6]*matrix.m[6] + matrix.m[10]*matrix.m[10] ;

    if (newMatrix[0] == newMatrix[1] && newMatrix[0] == newMatrix[2])
		return;

    newMatrix[0] = sqrt(newMatrix[0]) ;
    newMatrix[1] = sqrt(newMatrix[1]) ;
    newMatrix[2] = sqrt(newMatrix[2]) ;

    int closestToOne = 0 ;
    if (abs(newMatrix[1] - 1.0) < abs(newMatrix[0] - 1.0))
		closestToOne = 1 ;
    if (abs(newMatrix[2] - 1.0) < abs(newMatrix[closestToOne] - 1.0))
		closestToOne = 2 ;

	float scale = 0.0f;
	for (int i = 0 ; i < 3 ; i++) 
	{
		if (i == closestToOne) 
			continue ;
        scale = newMatrix[closestToOne] / newMatrix[i] ;
        matrix.m[i+0] *= scale ;
        matrix.m[i+4] *= scale ;
        matrix.m[i+8] *= scale ;
	}
}
*/

void CDefaultMatrix3x3Implementation::Transpose(Matrix3x3& matrix)
{
	float tmp;

	// Swap M01 and M10
	tmp = matrix.m[1];
	matrix.m[1] = matrix.m[3];
	matrix.m[3] = tmp;

	// Swap M02 and M20
	tmp = matrix.m[2];
	matrix.m[2] = matrix.m[6];
	matrix.m[6] = tmp;

	// Swap M12 and M21
	tmp = matrix.m[5];
	matrix.m[5] = matrix.m[7];
	matrix.m[7] = tmp;
}

Vec3 CDefaultMatrix3x3Implementation::TransformVector(const Matrix3x3 &matrix, const Vec3 &vec)
{
	Vec3 temp;
	temp.x = (vec.x * matrix.m[0]) + (vec.y * matrix.m[3]) + (vec.z * matrix.m[6]);
	temp.y = (vec.x * matrix.m[1]) + (vec.y * matrix.m[4]) + (vec.z * matrix.m[7]);
	temp.z = (vec.x * matrix.m[2]) + (vec.y * matrix.m[5]) + (vec.z * matrix.m[8]);
	return temp;
}

Vec3 CDefaultMatrix3x3Implementation::GetAxisX(Matrix3x3& matrix)
{
	Vec3 tmpVec;
	tmpVec.Set( matrix.m[0], matrix.m[1], matrix.m[2] );
	return tmpVec;
}

Vec3 CDefaultMatrix3x3Implementation::GetAxisY(Matrix3x3& matrix)
{
	Vec3 tmpVec;
	tmpVec.Set( matrix.m[3], matrix.m[4], matrix.m[5] );
	return tmpVec;
}

Vec3 CDefaultMatrix3x3Implementation::GetAxisZ(Matrix3x3& matrix)
{
	Vec3 tmpVec;
	tmpVec.Set( matrix.m[6], matrix.m[7], matrix.m[8] );
	return tmpVec;
}

bool CDefaultMatrix3x3Implementation::Parity( Matrix3x3 &matrix )
{
	Vec3 first = matrix.GetAxisX();
	Vec3 second = matrix.GetAxisY();
	Vec3 third = matrix.GetAxisZ();
	Vec3 cross = first.Cross( second );
	float dot = cross.Dot( third );
	if( dot > 0 )
	{
		//LH, odd parity
		return true;
	}else
	{
		return false;
	}
}