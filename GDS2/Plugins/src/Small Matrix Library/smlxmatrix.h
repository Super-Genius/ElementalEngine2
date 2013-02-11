///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                INTEL CORPORATION PROPRIETARY INFORMATION                  //
//                                                                           //
//  This program has been developed by Intel Corporation.                    //
//  You have Intel's permission to incorporate this code into your product,  //
//  royalty free.  Intel has various intellectual property rights            //
//  which it may assert under certain circumstances.                         //            
//                                                                           //
//  Intel specifically disclaims all warranties, express or implied,         //
//  and all liability, including consequential and other indirect damages,   //
//  for the use of this code, including liability for infringement           //
//  of any proprietary rights, and including the warranties                  //
//  of merchantability and fitness for a particular purpose.                 //
//  Intel does not assume any responsibility for any errors which may        //
//  appear in this code nor any responsibility to update it.                 //
//                                                                           //
//         Copyright (c) 1998 Intel Corporation. All Rights Reserved.        //
//                                                                           //
//  Purpose:                                                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#ifndef __SMLXMATRIX_H__
#define __SMLXMATRIX_H__

//#include <smlxopt.h>
#include <smlmath.h>

class  SMLXMatrix;
class  SMLXTmpMatrix;
class  SMLXTransposedMatrix;
class  SMLXSpatialVector;
struct InvertInstantiation;

// This class defines resizable matrices.
// It is optimized for sizes {1 | 3 | 4 | 6} X {1 | 3 | 4 | 6}.
// +,-,= will work for other sizes up to 100 X 100, 
// but operator* will generate asssertion in _DEBUG mode 
// (Mult() will work fine).

class SMLXMatrix {

public:

#ifdef COUNT_OPERATIONS
	SMLLIBENTRY static int mCount[7][7][8]; // = (0); =v (1); += (2); -= (3); + (4); - (5); * (6), *= v (7)
#endif

	////////
	// Ctors
	////////

	SMLLIBENTRY SMLXMatrix();
	SMLLIBENTRY SMLXMatrix(const SMLXMatrix& B);
	SMLLIBENTRY SMLXMatrix(short nrow, short ncol);
	SMLLIBENTRY SMLXMatrix(short nrow, short ncol, const float* initialValues);
	SMLLIBENTRY explicit SMLXMatrix(const SMLMatrix3f& m33); // SMLMatrix3f in smlmath.h
	SMLLIBENTRY explicit SMLXMatrix(const SMLMatrix4f& m44); // SMLMatrix4f in smlmath.h
	SMLLIBENTRY ~SMLXMatrix();

	/////////
	// Access
	/////////

	SMLLIBENTRY float                   Get(int i, int j) const;
	SMLLIBENTRY float                   MaxAbs() const;
	SMLLIBENTRY SMLXTmpMatrix&          TMat() const;
	SMLLIBENTRY const float*            Data() const;
	SMLLIBENTRY const float*            operator[](int i) const;
	SMLLIBENTRY int                     Index(int i, int j) const;
	SMLLIBENTRY int                     Size() const { return nc * nr; }
	SMLLIBENTRY operator const          SMLMatrix3f&() const; // cast
	SMLLIBENTRY operator const          SMLMatrix4f&() const; // cast
	SMLLIBENTRY short                   ncols() const { return nc; }
	SMLLIBENTRY short                   nrows() const { return nr; }
	SMLLIBENTRY void                    Export(float* dst) const;

	SMLLIBENTRY float*                  Data();
	SMLLIBENTRY float*                  operator[](int i);
	SMLLIBENTRY static SMLXTmpMatrix&   TMat(const SMLXMatrix& A);
	SMLLIBENTRY static SMLXTmpMatrix&   TMat(const SMLXMatrix& A, short nrow, short ncol);
	SMLLIBENTRY static SMLXTmpMatrix& __fastcall TMat(short nrow, short ncol);


	////////////
	// Modifiers
	////////////

	SMLLIBENTRY bool GenericInvert();   // with pivoting
	SMLLIBENTRY bool Invert();          // non-pivoting !!!
	SMLLIBENTRY void AdjustSize(short nrow, short ncol);
	SMLLIBENTRY void ChangeNumberOfRows(short nrow) { nr = nrow; }
	SMLLIBENTRY void Identity();
	SMLLIBENTRY void Identity(short nrow);
	SMLLIBENTRY void Identity(short nrow, short ncol);
	SMLLIBENTRY void IdentityMinus();
	SMLLIBENTRY void Negate();
	SMLLIBENTRY void Resize(const SMLXMatrix& A);
	SMLLIBENTRY void Resize(short nrow, short ncol);
	SMLLIBENTRY void Set(float initialValue);
	SMLLIBENTRY void Swap(const SMLXMatrix& B);
	SMLLIBENTRY void Zero();
	SMLLIBENTRY void Transpose();
	SMLLIBENTRY void Transpose(const SMLXMatrix& m);

	///////////////////
	// const Operations
	///////////////////

	SMLLIBENTRY SMLXTmpMatrix& operator*(float scale)  const;
	SMLLIBENTRY SMLXTmpMatrix& operator+(SMLXTmpMatrix& B) const;
	SMLLIBENTRY SMLXTmpMatrix& operator-(SMLXTmpMatrix& B) const;
	SMLLIBENTRY void Transform(SMLXSpatialVector const& A, SMLXSpatialVector& B) const;
	SMLLIBENTRY void TransformTransposed(SMLXSpatialVector const& A, SMLXSpatialVector& B) const;

	////////////////////////
	// modifiable Operations
	////////////////////////

	SMLLIBENTRY void Add(const SMLXMatrix& B);
	SMLLIBENTRY void Cross(const SMLVec3f& V);
	SMLLIBENTRY void Scale(float scale);
	SMLLIBENTRY void operator*=(float scale);
	SMLLIBENTRY void operator+=(const SMLXMatrix& B);
	SMLLIBENTRY void operator-=(const SMLXMatrix& B);
	SMLLIBENTRY void operator=(const SMLXMatrix& B);
	SMLLIBENTRY void operator=(const SMLXTmpMatrix& B);

	SMLLIBENTRY void Mult(const SMLXMatrix& A, const SMLXMatrix& B, 
						  short Atranspose   , short Btranspose);
	SMLLIBENTRY void NSub(const SMLXMatrix& B);
	SMLLIBENTRY void Sub (const SMLXMatrix& B);

	//////////////////
	// Transformations
	//////////////////

	SMLLIBENTRY float Pythag(double a, double b) const;
	SMLLIBENTRY bool  LUDecomposition(SMLXMatrix& indx, double& d);
	SMLLIBENTRY bool  SVDecomposition(SMLXMatrix& W, SMLXMatrix& V, SMLXMatrix& tmp);
	SMLLIBENTRY void  Jacobi(SMLXMatrix& eigval, SMLXMatrix& eigvec, int& nrot);
	SMLLIBENTRY void  LUBackSubstitution(SMLXMatrix& indx, SMLXMatrix& b) const;
	SMLLIBENTRY void  SVDBackSubstitution(const SMLXMatrix& U, const SMLXMatrix& W,
 										  const SMLXMatrix& V, const SMLXMatrix& b, SMLXMatrix& tmp);
	SMLLIBENTRY void  SVDInvert(const SMLXMatrix& U, const SMLXMatrix& W, SMLXMatrix& V);

	////////////
	// Predicats
	////////////

	SMLLIBENTRY bool IsDefined() const  { return flags != 0; }
	SMLLIBENTRY void MarkAsDefined()    { flags = 1;         }
	SMLLIBENTRY void MarkAsUndefined()  { flags = 0;         }

	SMLLIBENTRY void  Output(const char* caption = NULL) const;

	enum {poolSize = 8};

protected:

	SMLLIBENTRY void  SVDBiDiag(SMLXMatrix& w, SMLXMatrix& rv1, double& anorm);
	SMLLIBENTRY void  SVDInitialWV(SMLXMatrix& w, SMLXMatrix& v, SMLXMatrix& rv1);

#if _MSC_VER == 1200
// work around some bug in VC 6.0.
public:
#endif
						// Offset
	mutable float* mat; // 0
	mutable short msize;// 4
	short   maxsize;    // 6
	short   flags;      // 8
	short   owned;      // 10
	short   nr;         // 12
	short   nc;         // 14
						// 16

	static void* Inverts[7];
	static void SetInvertPointers();

	SMLLIBENTRY static SMLXTmpMatrix pool[poolSize];

	friend SMLLIBENTRY SMLXTmpMatrix&   __fastcall GenericAdd(SMLXMatrix& A, const SMLXMatrix& B);
	friend SMLLIBENTRY SMLXTmpMatrix&   __fastcall GenericSub(SMLXMatrix& A, const SMLXMatrix& B);
	friend SMLLIBENTRY void             __fastcall GenericAddTo(SMLXMatrix& A, const SMLXMatrix& B);
	friend SMLLIBENTRY void             __fastcall GenericAssign(SMLXMatrix& A, const SMLXMatrix& B);
	friend SMLLIBENTRY void             __fastcall GenericAssignValue(SMLXMatrix& A, float v);
	friend SMLLIBENTRY void             __fastcall GenericSubFrom(SMLXMatrix& A, const SMLXMatrix& B);
	friend inline SMLXTmpMatrix&        __fastcall GenericTMat(short nrow, short ncol);
	friend inline SMLXTmpMatrix&        __fastcall operator*(const SMLXMatrix& A, const SMLXMatrix& B);
	friend inline SMLXTmpMatrix&        __fastcall operator*(const SMLXMatrix& A, const SMLXTransposedMatrix& B);
	friend inline SMLXTmpMatrix&        __fastcall operator*(const SMLXTransposedMatrix& A, const SMLXMatrix& B);
	friend inline SMLXTmpMatrix&        __fastcall operator*(const SMLXTransposedMatrix& A, 
															 const SMLXTransposedMatrix& B);
	friend inline SMLXTmpMatrix&        __fastcall operator+(const SMLXMatrix& A, const SMLXMatrix& B);
	friend inline SMLXTmpMatrix&        __fastcall operator-(const SMLXMatrix& A, const SMLXMatrix& B);
	friend inline void                  Assign(SMLXMatrix& A, float value);
	friend inline void                  Assign(SMLXMatrix& A, const SMLXMatrix& B);
	friend struct InvertInstantiation;

	friend SMLLIBENTRY SMLXTmpMatrix& WorldSpaceRevolute (const SMLXSpatialVector &A);
	friend SMLLIBENTRY SMLXTmpMatrix& WorldSpaceSpherical(const SMLXSpatialVector &A);
	friend SMLLIBENTRY SMLXTmpMatrix& WorldSpaceSixDOF   (const SMLXSpatialVector &A);
	friend SMLLIBENTRY SMLXTmpMatrix& WorldSpacePrismatic(const SMLXSpatialVector &A);

public:

	SMLLIBENTRY static /*const*/ int lutSizes[100];

};


inline float Scalar(const SMLXMatrix& A, const SMLXMatrix& B);

// 1 in XX (MultXX) means transposed matrix.
SMLLIBENTRY SMLXTmpMatrix& __fastcall Mult00(const SMLXMatrix& A, const SMLXMatrix& B);
SMLLIBENTRY SMLXTmpMatrix& __fastcall Mult01(const SMLXMatrix& A, const SMLXMatrix& B);
SMLLIBENTRY SMLXTmpMatrix& __fastcall Mult10(const SMLXMatrix& A, const SMLXMatrix& B);
SMLLIBENTRY SMLXTmpMatrix& __fastcall Mult11(const SMLXMatrix& A, const SMLXMatrix& B);

// This class is used in the intermediate evaluations.
class SMLXTmpMatrix : public SMLXMatrix {

public:

	SMLXTmpMatrix() {
		// It is just an intermediate class, no memory of its own.
		owned = 0;
		maxsize = 0;
		msize = 0;
		mat = NULL;
	}

	// Memory-saving versions.

	SMLLIBENTRY SMLXTmpMatrix& operator+(const SMLXMatrix& A);
	SMLLIBENTRY SMLXTmpMatrix& operator-(const SMLXMatrix& A);
	SMLLIBENTRY SMLXTmpMatrix& operator*(float scale);

	//Not exported
	//initializing pools for temporary matrix
	static int InitTmpPool(short nr, short nc, SMLXTmpMatrix*, int);

	//initialize pool element, called by InitTmpPools()
	//user shouldn't call it
private:
	void PoolElement(short nr, short nc);
};


class SMLXTransposedMatrix : public SMLXMatrix {
};
inline SMLXTransposedMatrix& Transpose(const SMLXMatrix& m);


// Vector with sizes from 1x1 to 6x1.
class SMLXSpatialVector: public SMLXMatrix {

public:
	
	////////
	// Ctors
	////////

	SMLLIBENTRY SMLXSpatialVector();
	SMLLIBENTRY SMLXSpatialVector(const SMLVec3f& A);
	SMLLIBENTRY SMLXSpatialVector(const SMLVec4f& A);
	SMLLIBENTRY SMLXSpatialVector(const SMLXMatrix& A);
	SMLLIBENTRY SMLXSpatialVector(const SMLXSpatialVector& A);
	SMLLIBENTRY explicit SMLXSpatialVector(const float* initialValues);
	SMLLIBENTRY explicit SMLXSpatialVector(float);
	SMLLIBENTRY          SMLXSpatialVector(float, float);
	SMLLIBENTRY          SMLXSpatialVector(float, float, float);
	SMLLIBENTRY          SMLXSpatialVector(float, float, float, float);
	SMLLIBENTRY          SMLXSpatialVector(float, float, float, float, float);
	SMLLIBENTRY          SMLXSpatialVector(float, float, float, float, float, float);

	/////////
	// Access
	/////////

	SMLLIBENTRY int   Size() const { return nr; }
	SMLLIBENTRY float LengthSquared()  const { return Scalar(*this, *this); }
	SMLLIBENTRY float Length()  const { return FastSqrt(LengthSquared()); }

	// Casting to angular/linear components.
	SMLLIBENTRY const SMLVec3f& Angular() const { assert(nr == 6); return (SMLVec3f&)vec[0]; }
	SMLLIBENTRY const SMLVec3f& Linear()  const { assert(nr == 6); return (SMLVec3f&)vec[3]; }
	SMLLIBENTRY       SMLVec3f& Angular()       { assert(nr == 6); return (SMLVec3f&)vec[0]; }
	SMLLIBENTRY       SMLVec3f& Linear()        { assert(nr == 6); return (SMLVec3f&)vec[3]; }

	//////////////
	// Operations.
	//////////////

	SMLLIBENTRY void AdjustSize(short nrow);
	SMLLIBENTRY void Copy(const SMLXMatrix& A, int row = 0);
	SMLLIBENTRY void operator*=(float scale);
	SMLLIBENTRY void operator=(const SMLXMatrix& A);
	SMLLIBENTRY void operator=(const SMLXSpatialVector& A);

	// Directly accessible by an application.
	float* vec;
	float  m_vecData[6+4];

private:
	// Not allowed, non-accessible.
	SMLLIBENTRY void Resize(const SMLXMatrix& A)               { assert(0); }
	SMLLIBENTRY void __fastcall Resize(short nrow, short ncol) { assert(0); }

};

#define _MMBODY
#include <smlxopt.h>

#include <smlxmatrix.inl>

#endif
