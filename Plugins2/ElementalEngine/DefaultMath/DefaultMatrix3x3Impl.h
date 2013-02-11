///==========================================================================
/// \file	DefaultMatrix3x3Impl.h
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

#ifndef DEFAULTMATRIX3X3IMPL_H__
#define DEFAULTMATRIX3X3IMPL_H__

#include "IMatrix3x3Implementation.h"
#include "Matrix3x3.h"

namespace ElementalEngine
{

class CDefaultMatrix3x3Implementation : public IMatrix3x3Implementation
{
private:
	void Init( void );
	void CleanUp( void );

	CDefaultMatrix3x3Implementation()
	{
		Init();
	}

	virtual ~CDefaultMatrix3x3Implementation()
	{
		CleanUp();
	}

public:
	static IMatrix3x3Implementation* Instance( void )
	{
		if ( s_instance == NULL )
		{
			s_instance = new CDefaultMatrix3x3Implementation();
		}
		return s_instance;
	}
	static void Destroy( void )
	{
		if ( s_instance != NULL )
		{
			delete s_instance;
		}
		s_instance = NULL;
	}

	virtual void Set( Matrix3x3& matrix, 
					  float m00, float m01, float m02, 
					  float m10, float m11, float m12, 
					  float m20, float m21, float m22 );
	/// Set the Matrix3x3 with the values from a 3x3 matrix
	virtual void SetFrom3x3( Matrix3x3& matrix, float* newM );
	/// Set the Matrix3x3 with the values from a 4x4 matrix
	virtual void SetFrom4x4( Matrix3x3& matrix, float* newM );
	/// Set the Matrix3x3 from axis angles
	virtual void SetFromAxisAngle( Matrix3x3& matrix, const Vec3& axis, float angle );
	/// GetAxisX
	/// \param matrix - The matrix to extract axis vector from
	/// \return	The axis
	virtual Vec3 GetAxisX(const Matrix3x3& matrix) const;
	/// GetAxisY
	/// \param matrix - The matrix to extract axis vector from
	/// \return	The axis
	virtual Vec3 GetAxisY(const Matrix3x3& matrix) const;
	/// GetAxisZ
	/// \param matrix - The matrix to extract axis vector from
	/// \return	The axis
	virtual Vec3 GetAxisZ(const Matrix3x3& matrix) const;
	/// Returns true if two matrices are equal
	/// \param a - The first matrix to test
	/// \param b - The second matrix to test
	virtual bool Equals(const Matrix3x3& a, const Matrix3x3& b) const ;
	/// Returns true if two matrices are not equal
	/// \param a - The first matrix to test
	/// \param b - The second matrix to test
	virtual bool NotEquals(const Matrix3x3& a, const Matrix3x3& b) const ;
	/// Multiply two matrices
	/// \param a - The first matrix to miltiply
	/// \param b - The second matrix to multiply
	/// \return The resultant matrix
	virtual Matrix3x3 Times(const Matrix3x3& a, const Matrix3x3& b) const ;
	/// Multiply two matrices and store the results in this
	/// \param a - The first matrix to miltiply
	/// \param b - The second matrix to multiply
	/// \return The resultant matrix
	virtual Matrix3x3& TimesEquals(Matrix3x3& a, const Matrix3x3& b) const ;
	/// Returns true if the matrix is the identity matrix
	/// \param matrix - The matrix to test
	/// \return True if the matrix is the identity matrix, false if not
	virtual bool IsIdentity(Matrix3x3& matrix) ;
	/// Set the rotation from euler angles of degrees
	/// \param matrix - The matrix to rotate
	/// \param euler - The new rotation in euler angles of degrees
	virtual void SetRotation(Matrix3x3& matrix, const EulerAngle& euler) ;
	/// Calculates inverse of matrix. Slow.
	/// \param matrix - The matrix to invert
	/// \return Returns false if there is no inverse matrix.
	virtual bool SetInverse(Matrix3x3& matrix) ;
	//! Returns the inversed matrix
	/// \parma in - Source matrix to invert
	//! \param out - Where result matrix is written to.
	//! \return Return false if there is no inverse matrix.
	virtual bool GetInverse(Matrix3x3& in, Matrix3x3& out) ;
	/// Normalizes the matrix
	/// \param matrix - The matrix to normalize
//	virtual void Normalize(Matrix3x3& matrix) ;
	/// Transposes the matrix
	/// \param matrix - The matrix to transpose
	virtual void Transpose(Matrix3x3& matrix);
	/// Transforms the input vector based on the matrix
	/// \param vec = vector to transform
	/// \return transformed vector
	virtual Vec3 TransformVector(const Matrix3x3 &matrix, const Vec3 &vec);
	/// Checks the parity of a matrix
	/// \return false if even, true if odd parity
	virtual bool Parity(  Matrix3x3 &matrix );
	/// GetAxisX
	/// \param matrix - The matrix to extract axis vector from
	/// \return	The axis
	virtual Vec3 GetAxisX(Matrix3x3& matrix);
	/// GetAxisY
	/// \param matrix - The matrix to extract axis vector from
	/// \return	The axis
	virtual Vec3 GetAxisY(Matrix3x3& matrix);
	/// GetAxisZ
	/// \param matrix - The matrix to extract axis vector from
	/// \return	The axis
	virtual Vec3 GetAxisZ(Matrix3x3& matrix);
	

private:
	static IMatrix3x3Implementation* s_instance;
};

} // namespace ElementalEngine

#endif // DEFAULTMATRIX3X3IMPL_H__