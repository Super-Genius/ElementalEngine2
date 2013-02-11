///==========================================================================
/// \file	DefaultMatrix4x4Impl.cpp
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
#include "DefaultMatrix4x4Impl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMatrix4x4Implementation *CDefaultMatrix4x4Implementation::s_instance = NULL;

void CDefaultMatrix4x4Implementation::Init( void )
{
}

void CDefaultMatrix4x4Implementation::CleanUp( void )
{
}

void CDefaultMatrix4x4Implementation::SetFromVectors(Matrix4x4 &matrix, const Vec3 &forwardVec, const Vec3 &upVec, 
								const Vec3 &rightVec, const Vec3 &translation)
{
	Set(matrix, rightVec.x, rightVec.y, rightVec.z, 0.0f,
				upVec.x, upVec.y, upVec.z, 0.0f,				
				forwardVec.x, forwardVec.y, forwardVec.z, 0.0f, 
				translation.x, translation.y, translation.z, 1.0f);
}

void CDefaultMatrix4x4Implementation::Set( Matrix4x4& matrix, float m00, float m01, float m02, float m03,
															  float m10, float m11, float m12, float m23,
															  float m20, float m21, float m22, float m13,
															  float m30, float m31, float m32, float m33 )
{
	matrix.m[0] = m00;
	matrix.m[1] = m01;
	matrix.m[2] = m02;
	matrix.m[3] = m03;

	matrix.m[4] = m10;
	matrix.m[5] = m11;
	matrix.m[6] = m12;
	matrix.m[7] = m13;

	matrix.m[8] =  m20;
	matrix.m[9] =  m21;
	matrix.m[10] = m22;
	matrix.m[11] = m23;

	matrix.m[12] = m30;
	matrix.m[13] = m31;
	matrix.m[14] = m32;
	matrix.m[15] = m33;
}

void CDefaultMatrix4x4Implementation::SetFrom3x3( Matrix4x4& matrix, float* newM )
{
	matrix.m[0] = newM[0];
	matrix.m[1] = newM[1];
	matrix.m[2] = newM[2];
	matrix.m[3] = 0;

	matrix.m[4] = newM[3];
	matrix.m[5] = newM[4];
	matrix.m[6] = newM[5];
	matrix.m[7] = 0;

	matrix.m[8] = newM[6];
	matrix.m[9] = newM[7];
	matrix.m[10] = newM[8];
	matrix.m[11] = 0;

	matrix.m[12] = 0;
	matrix.m[13] = 0;
	matrix.m[14] = 0;
	matrix.m[15] = 1;
}

void CDefaultMatrix4x4Implementation::SetFrom4x4( Matrix4x4& matrix, float* newM )
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
	matrix.m[9] = newM[9];
	matrix.m[10] = newM[10];
	matrix.m[11] = newM[11];

	matrix.m[12] = newM[12];
	matrix.m[13] = newM[13];
	matrix.m[14] = newM[14];
	matrix.m[15] = newM[15];
}

bool CDefaultMatrix4x4Implementation::Equals(const Matrix4x4& a, const Matrix4x4& b) const
{
	for (int i = 0; i < 16; ++i)
		if (a.m[i] != b.m[i])
			return false;

	return true;
}

bool CDefaultMatrix4x4Implementation::NotEquals(const Matrix4x4& a, const Matrix4x4& b) const 
{
	for (int i = 0; i < 16; ++i)
		if (a.m[i] == b.m[i])
			return false;

		return true;
}

Matrix4x4 CDefaultMatrix4x4Implementation::Times(const Matrix4x4& a, const Matrix4x4& b) const
{
	Matrix4x4 tmtrx;
	const float *m1 = a.m, *m2 = b.m;
	float *m3 = tmtrx.m;

	m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
	m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
	m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
	m3[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];
		
	m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
	m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
	m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
	m3[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];
		
	m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
	m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
	m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];	
	m3[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];
		
	m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
	m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
	m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
	m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];

	return tmtrx;
}

Matrix4x4& CDefaultMatrix4x4Implementation::TimesEquals(Matrix4x4& a, const Matrix4x4& b) const
{
	float newMatrix[16];
	const float *m1 = a.m, *m2 = b.m;

	newMatrix[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
	newMatrix[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
	newMatrix[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
	newMatrix[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];
		
	newMatrix[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
	newMatrix[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
	newMatrix[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
	newMatrix[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];
		
	newMatrix[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
	newMatrix[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
	newMatrix[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
	newMatrix[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];
		
	newMatrix[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
	newMatrix[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
	newMatrix[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
	newMatrix[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
		
	for (int i=0; i<16; ++i)
		a.m[i] = newMatrix[i];

	return (a);
}

bool CDefaultMatrix4x4Implementation::IsIdentity(Matrix4x4& matrix) 
{
	for (int i=0; i<4; ++i)
		for (int j=0; j<4; ++j)
			if (j != i)
			{
				if ( matrix.Get(i,j) < -0.0000001f || matrix.Get(i,j) >  0.0000001f )
					return false;
			}
			else
			{
				if ( matrix.Get(i,j) < 0.9999999f || matrix.Get(i,j) > 1.0000001f )
					return false;
			}
			
			return true;
}

void CDefaultMatrix4x4Implementation::SetTranslation(Matrix4x4& matrix, const Vec3& trans)
{
	matrix.m[12] = trans.x;
	matrix.m[13] = trans.y;
	matrix.m[14] = trans.z;
}

void CDefaultMatrix4x4Implementation::SetRotation(Matrix4x4& matrix, const EulerAngle& euler)
{
	static MathUtil math;
	math.EulerToMatrix( euler, matrix );
}

void CDefaultMatrix4x4Implementation::SetScale(Matrix4x4& matrix, const Vec3& scale) 
{
	ResetScale(matrix);
	ApplyScale(matrix, scale);
}

void CDefaultMatrix4x4Implementation::SetScaleWithTranslation(Matrix4x4 &matrix, const Vec3 &scale)
{
	Vec3 oldScale = GetScale(matrix);
	ResetScale(matrix);
	ApplyScale(matrix, scale);
	
	Vec3 trans = GetTranslation(matrix);

	// unset old scale
	if (oldScale.x != 0.0f)
	{
		trans.x = trans.x / oldScale.x;
	}
	if (oldScale.y != 0.0f)
	{
		trans.y = trans.y / oldScale.y;
	}
	if (oldScale.z != 0.0f)
	{
		trans.z = trans.z / oldScale.z;
	}

	// set new scale
	trans *= scale;

	SetTranslation(matrix, trans);
}
	
void CDefaultMatrix4x4Implementation::ResetScale(Matrix4x4 &matrix)
{
	Vec3 xAxis, yAxis, zAxis;
	xAxis = GetAxisX(matrix);
	yAxis = GetAxisY(matrix);
	zAxis = GetAxisZ(matrix);

	xAxis.Normalize();
	yAxis.Normalize();
	zAxis.Normalize();

	matrix.m[0] = xAxis.x;
	matrix.m[1] = xAxis.y;
	matrix.m[2] = xAxis.z;
			
	matrix.m[4] = yAxis.x;
	matrix.m[5] = yAxis.y;
	matrix.m[6] = yAxis.z;
			
	matrix.m[8] = zAxis.x;
	matrix.m[9] = zAxis.y;
	matrix.m[10] = zAxis.z;
}

void CDefaultMatrix4x4Implementation::ApplyScale(Matrix4x4& matrix, const Vec3& scale)
{
	matrix.m[0] *= scale.x;
	matrix.m[1] *= scale.x;
	matrix.m[2] *= scale.x;
			
	matrix.m[4] *= scale.y;
	matrix.m[5] *= scale.y;
	matrix.m[6] *= scale.y;
			
	matrix.m[8] *= scale.z;
	matrix.m[9] *= scale.z;
	matrix.m[10] *= scale.z;
}

Vec3 CDefaultMatrix4x4Implementation::GetAxisX(Matrix4x4& matrix)
{
	Vec3 tmpVec;
	tmpVec.Set( matrix.m[0], matrix.m[1], matrix.m[2] );
	return tmpVec;
}

Vec3 CDefaultMatrix4x4Implementation::GetAxisY(Matrix4x4& matrix)
{
	Vec3 tmpVec;
	tmpVec.Set( matrix.m[4], matrix.m[5], matrix.m[6] );
	return tmpVec;
}

Vec3 CDefaultMatrix4x4Implementation::GetAxisZ(Matrix4x4& matrix)
{
	Vec3 tmpVec;
	tmpVec.Set( matrix.m[8], matrix.m[9], matrix.m[10] );
	return tmpVec;
}

Vec3 CDefaultMatrix4x4Implementation::GetTranslation(Matrix4x4& matrix)
{
	Vec3 tmpVec;
	tmpVec.Set( matrix.m[12], matrix.m[13], matrix.m[14] );
	return tmpVec;
}

EulerAngle CDefaultMatrix4x4Implementation::GetRotation(Matrix4x4& matrix)
{
	EulerAngle tmpVec;

	// Get 3x3 matrix
	Matrix3x3 mat;
	mat.SetFrom4x4( matrix.m );

	// convert matrix to eulers
	static MathUtil math;
	//math.MatrixToRadEuler( mat, tmpVec );
	math.MatrixToEuler( mat, tmpVec );

	return tmpVec;
}

Vec3 CDefaultMatrix4x4Implementation::GetScale(Matrix4x4& matrix)
{
	Vec3 tmpVec;
	Vec3 xScale = matrix.GetAxisX();
	Vec3 yScale = matrix.GetAxisY();
	Vec3 zScale = matrix.GetAxisZ();
	
	tmpVec.Set( xScale.Length(), yScale.Length(), zScale.Length() );
	return tmpVec;
}

Vec4 CDefaultMatrix4x4Implementation::GetRow( Matrix4x4& matrix, int row )
{
	return Vec4( matrix.m[0+row], matrix.m[4+row], matrix.m[8+row], matrix.m[12+row] );
}

bool CDefaultMatrix4x4Implementation::SetInverse(Matrix4x4& matrix) 
{
	Matrix4x4 temp( matrix );
	return GetInverse( temp, matrix );
}

bool CDefaultMatrix4x4Implementation::GetInverse(Matrix4x4& in, Matrix4x4& out) 
{
	// both these funcs @ http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm
	
	float determinant = GetDeterminant(in);

	if (determinant == 0.f)
	{
		return false;
	}

	float scale = 1.f / determinant;

	float m00 = in.m[0];
	float m10 = in.m[1];
	float m20 = in.m[2];
	float m30 = in.m[3];
	float m01 = in.m[4];
	float m11 = in.m[5];
	float m21 = in.m[6];
	float m31 = in.m[7];
	float m02 = in.m[8];
	float m12 = in.m[9];
	float m22 = in.m[10];
	float m32 = in.m[11];
	float m03 = in.m[12];
	float m13 = in.m[13];
	float m23 = in.m[14];
	float m33 = in.m[15];

    out.m[0] = scale * ((m12*m23*m31)-(m13*m22*m31)+
		(m13*m21*m32)-(m11*m23*m32)-
		(m12*m21*m33)+(m11*m22*m33));
	out.m[4] = scale * ((m03*m22*m31)-(m02*m23*m31)-
		(m03*m21*m32)+(m01*m23*m32)+
		(m02*m21*m33)-(m01*m22*m33));
	out.m[8] = scale * ((m02*m13*m31)-(m03*m12*m31)+
		(m03*m11*m32)-(m01*m13*m32)-
		(m02*m11*m33)+(m01*m12*m33));
	out.m[12] = scale * ((m03*m12*m21)-(m02*m13*m21)-
		(m03*m11*m22)+(m01*m13*m22)+
		(m02*m11*m23)-(m01*m12*m23)); 
	out.m[1] = scale * ((m13*m22*m30)-(m12*m23*m30)-
		(m13*m20*m32)+(m10*m23*m32)+
		(m12*m20*m33)-(m10*m22*m33)); 
	out.m[5] = scale * ((m02*m23*m30)-(m03*m22*m30)+
		(m03*m20*m32)-(m00*m23*m32)-
		(m02*m20*m33)+(m00*m22*m33));
	out.m[9] = scale * ((m03*m12*m30)-(m02*m13*m30)-
		(m03*m10*m32)+(m00*m13*m32)+
		(m02*m10*m33)-(m00*m12*m33));
	out.m[13] = scale * ((m02*m13*m20)-(m03*m12*m20)+
		(m03*m10*m22)-(m00*m13*m22)-
		(m02*m10*m23)+(m00*m12*m23)); 
	out.m[2] = scale * ((m11*m23*m30)-(m13*m21*m30)+
		(m13*m20*m31)-(m10*m23*m31)-
		(m11*m20*m33)+(m10*m21*m33)); 
	out.m[6] = scale * ((m03*m21*m30)-(m01*m23*m30)-
		(m03*m20*m31)+(m00*m23*m31)+
		(m01*m20*m33)-(m00*m21*m33)); 
	out.m[10] = scale * ((m01*m13*m30)-(m03*m11*m30)+
		(m03*m10*m31)-(m00*m13*m31)-
		(m01*m10*m33)+(m00*m11*m33));  
	out.m[14] = scale * ((m03*m11*m20)-(m01*m13*m20)-
		(m03*m10*m21)+(m00*m13*m21)+
		(m01*m10*m23)-(m00*m11*m23));  
	out.m[3] = scale * ((m12*m21*m30)-(m11*m22*m30)-
		(m12*m20*m31)+(m10*m22*m31)+
		(m11*m20*m32)-(m10*m21*m32));  
	out.m[7] = scale * ((m01*m22*m30)-(m02*m21*m30)+
		(m02*m20*m31)-(m00*m22*m31)-
		(m01*m20*m32)+(m00*m21*m32));  
	out.m[11] = scale * ((m02*m11*m30)-(m01*m12*m30)-
		(m02*m10*m31)+(m00*m12*m31)+
		(m01*m10*m32)-(m00*m11*m32));   
	out.m[15] = scale * ((m01*m12*m20)-(m02*m11*m20)+
		(m02*m10*m21)-(m00*m12*m21)-
		(m01*m10*m22)+(m00*m11*m22));   

   	return true;
}

/// Transposes a matrix
void CDefaultMatrix4x4Implementation::Transpose( Matrix4x4& inout )
{
	// 0 1 2 3 
	// 4 5 6 7
	// 8 9 10 11
	// 12 13 14 15
	Matrix4x4 t;
	t.m[ 0 ] = inout.m[ 0 ];
	t.m[ 1 ] = inout.m[ 4 ];
	t.m[ 2 ] = inout.m[ 8 ];
	t.m[ 3 ] = inout.m[ 12 ];

	t.m[ 4 ] = inout.m[ 1 ];
	t.m[ 5 ] = inout.m[ 5 ];
	t.m[ 6 ] = inout.m[ 9 ];
	t.m[ 7 ] = inout.m[ 13 ];

	t.m[ 8 ] = inout.m[ 2 ];
	t.m[ 9 ] = inout.m[ 6 ];
	t.m[ 10 ] = inout.m[ 10 ];
	t.m[ 11 ] = inout.m[ 14 ];

	t.m[ 12 ] = inout.m[ 3 ];
	t.m[ 13 ] = inout.m[ 7 ];
	t.m[ 14 ] = inout.m[ 11 ];
	t.m[ 15 ] = inout.m[ 15 ];

	inout = t;
}

float CDefaultMatrix4x4Implementation::GetDeterminant(const Matrix4x4 &mat)
{
	float m00 = mat.m[0];
	float m10 = mat.m[1];
	float m20 = mat.m[2];
	float m30 = mat.m[3];
	float m01 = mat.m[4];
	float m11 = mat.m[5];
	float m21 = mat.m[6];
	float m31 = mat.m[7];
	float m02 = mat.m[8];
	float m12 = mat.m[9];
	float m22 = mat.m[10];
	float m32 = mat.m[11];
	float m03 = mat.m[12];
	float m13 = mat.m[13];
	float m23 = mat.m[14];
	float m33 = mat.m[15];

	float det;
	det = (m03 * m12 * m21 * m30) -
		(m02 * m13 * m21 * m30) -
		(m03 * m11 * m22 * m30) + 
		(m01 * m13 * m22 * m30) + 
		(m02 * m11 * m23 * m30) -
		(m01 * m12 * m23 * m30) -
		(m03 * m12 * m20 * m31) + 
		(m02 * m13 * m20 * m31) + 
		(m03 * m10 * m22 * m31) -
		(m00 * m13 * m22 * m31) -
		(m02 * m10 * m23 * m31) + 
		(m00 * m12 * m23 * m31) + 
		(m03 * m11 * m20 * m32) -
		(m01 * m13 * m20 * m32) -
		(m03 * m10 * m21 * m32) + 
		(m00 * m13 * m21 * m32) + 
		(m01 * m10 * m23 * m32) -
		(m00 * m11 * m23 * m32) -
		(m02 * m11 * m20 * m33) + 
		(m01 * m12 * m20 * m33) + 
		(m02 * m10 * m21 * m33) -
		(m00 * m12 * m21 * m33) -
		(m01 * m10 * m22 * m33) + 
		(m00 * m11 * m22 * m33);
    
	return det;
}

Vec3 CDefaultMatrix4x4Implementation::TransformVector(const Matrix4x4 &matrix, const Vec3 &vec)
{
	Vec3 temp;
	temp.x = (vec.x * matrix.m[0]) + (vec.y * matrix.m[4]) + (vec.z * matrix.m[8]) + matrix.m[12];
	temp.y = (vec.x * matrix.m[1]) + (vec.y * matrix.m[5]) + (vec.z * matrix.m[9]) + matrix.m[13];
	temp.z = (vec.x * matrix.m[2]) + (vec.y * matrix.m[6]) + (vec.z * matrix.m[10]) + matrix.m[14];
	return temp;
}

Vec4 CDefaultMatrix4x4Implementation::TransformVector(const Matrix4x4 &matrix, const Vec4 &vec)
{
	Vec4 temp;
	temp.x = (vec.x * matrix.m[0]) + (vec.y * matrix.m[4]) + (vec.z * matrix.m[8])  + (vec.w * matrix.m[12]);
	temp.y = (vec.x * matrix.m[1]) + (vec.y * matrix.m[5]) + (vec.z * matrix.m[9])  + (vec.w * matrix.m[13]);
	temp.z = (vec.x * matrix.m[2]) + (vec.y * matrix.m[6]) + (vec.z * matrix.m[10]) + (vec.w * matrix.m[14]);
	temp.w = (vec.w * matrix.m[3]) + (vec.y * matrix.m[7]) + (vec.z * matrix.m[11]) + (vec.w * matrix.m[15]);
	return temp;
}

Vec3 CDefaultMatrix4x4Implementation::TransformDirection(const Matrix4x4 &matrix, const Vec3 &vec)
{
	Vec3 temp;
	temp.x = (vec.x * matrix.m[0]) + (vec.y * matrix.m[4]) + (vec.z * matrix.m[8]);
	temp.y = (vec.x * matrix.m[1]) + (vec.y * matrix.m[5]) + (vec.z * matrix.m[9]);
	temp.z = (vec.x * matrix.m[2]) + (vec.y * matrix.m[6]) + (vec.z * matrix.m[10]);
	return temp;
}

Matrix3x3 CDefaultMatrix4x4Implementation::Get3x3(const Matrix4x4 &matrix)
{
	Matrix3x3 outMat;
	outMat.SetFrom4x4((float*)&matrix.m[0]);
	return outMat;
}

bool CDefaultMatrix4x4Implementation::Parity(  Matrix4x4 &matrix )
{
	Matrix3x3 mat3 = Get3x3( matrix );
	return mat3.Parity();
}