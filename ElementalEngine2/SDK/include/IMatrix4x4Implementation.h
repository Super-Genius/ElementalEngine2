///==========================================================================
/// \file	IMatrix4x4Implementation.h
/// \brief	4x4 Matrix math function implementation interface
/// \date	3\16\2005
/// \author	Kyle Swaim
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

#ifndef	IMATRIX4X4IMPL_H__
#define	IMATRIX4X4IMPL_H__

#include "Matrix4x4.h"

namespace ElementalEngine
{
///==========================================================================
/// \class	IMatrix4x4Implementation
/// \brief	
///==========================================================================
class __declspec(novtable) IMatrix4x4Implementation
{
public:
	virtual void Set( Matrix4x4& matrix, float m00, float m01, float m02, float m03,
										 float m10, float m11, float m12, float m23,
										 float m20, float m21, float m22, float m13,
										 float m30, float m31, float m32, float m33 ) = 0;
	/// Sets the matrix given forward, up, right, and translation vectors
	/// NOTE: the direction vectors need to be orthagonal
	/// \param matrix = matrix to set
	/// \param forwardVec = forward vector
	/// \param upVec = up vector
	/// \param right vec = right vector
	/// \param translation = translation of matrix
	virtual void SetFromVectors(Matrix4x4 &matrix, const Vec3 &forwardVec, const Vec3 &upVec, 
								const Vec3 &rightVec, const Vec3 &translation) = 0;
	virtual void SetFrom3x3( Matrix4x4& matrix, float* newM ) = 0;
	virtual void SetFrom4x4( Matrix4x4& matrix, float* newM ) = 0;
	/// Returns true if two matrices are equal
	/// \param a - The first matrix to test
	/// \param b - The second matrix to test
	virtual bool Equals(const Matrix4x4& a, const Matrix4x4& b) const = 0;
	/// Returns true if two matrices are not equal
	/// \param a - The first matrix to test
	/// \param b - The second matrix to test
	virtual bool NotEquals(const Matrix4x4& a, const Matrix4x4& b) const = 0;
	/// Multiply two matrices
	/// \param a - The first matrix to miltiply
	/// \param b - The second matrix to multiply
	/// \return The resultant matrix
	virtual Matrix4x4 Times(const Matrix4x4& a, const Matrix4x4& b) const = 0;
	/// Multiply two matrices and store the results in this
	/// \param a - The first matrix to miltiply
	/// \param b - The second matrix to multiply
	/// \return The resultant matrix
	virtual const Matrix4x4& TimesEquals(Matrix4x4& a, const Matrix4x4& b) const = 0;
	/// Returns true if the matrix is the identity matrix
	/// \param matrix - The matrix to test
	/// \return True if the matrix is the identity matrix, false if not
	virtual bool IsIdentity(Matrix4x4& matrix) = 0;
	/// Set the translation from a Vector
	/// \param matrix - The matrix to translate
	/// \param trans - The new translation
	virtual void SetTranslation(Matrix4x4& matrix, const Vec3& trans) = 0;
	/// Set the rotation from euler angles of degrees
	/// \param matrix - The matrix to rotate
	/// \param euler - The new rotation in euler angles of degrees
	virtual void SetRotation(Matrix4x4& matrix, const EulerAngle& euler) = 0;
	/// Set Scale (resets, then sets new scale)
	/// This fn does not scale translation
	/// \param matrix - The matrix to scale
	/// \param scale - The new scale
	virtual void SetScale(Matrix4x4& matrix, const Vec3& scale) = 0;
	/// Sets the scale of the matrix and scales the translation
	/// Useful if you want to scale from the origin
	/// \param matrix - The matrix to scale
	/// \param scale - The new scale
	virtual void SetScaleWithTranslation(Matrix4x4 &matrix, const Vec3 &scale) = 0;
	/// Resets the scale of the matrix (scaled to 1)
	/// \param matrix = matrix to reset scale of
	virtual void ResetScale(Matrix4x4 &matrix) = 0;
	/// Applies scale value to current scaled matrix
	/// Does NOT reset the matrix before applying the scale
	/// \param matrix - The matrix to scale
	/// \param scale - scale to multiply thru
	virtual void ApplyScale(Matrix4x4& matrix, const Vec3& scale) = 0;

	virtual Vec3 GetAxisX(Matrix4x4& matrix) = 0;
	virtual Vec3 GetAxisY(Matrix4x4& matrix) = 0;
	virtual Vec3 GetAxisZ(Matrix4x4& matrix) = 0;
	virtual Vec3 GetTranslation(Matrix4x4& matrix) = 0;
	virtual EulerAngle GetRotation(Matrix4x4& matrix) = 0;
	virtual Vec3 GetScale(Matrix4x4& matrix) = 0;
	virtual Vec4 GetRow( Matrix4x4& matrix, int row ) = 0;

	/// Calculates inverse of matrix. Slow.
	/// \return Returns false if there is no inverse matrix.
	virtual bool SetInverse(Matrix4x4& matrix) = 0;
	//! Returns the inversed matrix
	/// \parma in - Source matrix to invert
	//! \param out - Where result matrix is written to.
	//! \return Return false if there is no inverse matrix.
	virtual bool GetInverse(Matrix4x4& in, Matrix4x4& out) = 0;
	/// Transposes a matrix
	virtual void Transpose( Matrix4x4& inout ) = 0;
	/// Transforms the input vector based on the matrix
	/// \param matrix = matrix to transform by
	/// \param vec = vector to transform
	/// \return transformed vector
	virtual Vec3 TransformVector(const Matrix4x4 &matrix, const Vec3 &vec) = 0;
	/// Transforms the input vector based on the matrix
	/// \param matrix = matrix to transform by
	/// \param vec = vector to transform
	/// \return transformed vector
	virtual Vec4 TransformVector(const Matrix4x4 &matrix, const Vec4 &vec) = 0;
	/// Transforms the input vector based on the matrix disregarding translation
	/// \param vec = vector to transform
	/// \return transformed vector
	virtual Vec3 TransformDirection(const Matrix4x4 &matrix, const Vec3 &vec) = 0;
	/// Gets the 3x3 rotational matrix from a full 4x4 matrix
	/// \param matrix = 4x4 matrix to get smaller matrix of
	/// \return 3x3 rotational matrix
	virtual Matrix3x3 Get3x3(const Matrix4x4 &matrix) = 0;
	/// Checks the parity of a matrix
	/// \return false if even, true if odd parity
	virtual bool Parity(  Matrix4x4 &matrix ) = 0;
};

} //namespace ElementalEngine

#endif // IMATRIX4X4IMPL_H__