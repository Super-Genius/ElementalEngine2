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

#include <smlxspace.h>

// Some highly specialized operations for
// recursive dynamics.

// Transpose(Phi) * Matrix6X6
SMLXTmpMatrix& MotionSpaceRevolute(SMLXTmpMatrix &A) 
{
	/// SMLXTmpMatrix A0 = A;
	float* s;
	float* d;
	s = A[2];
	d = A[0];
	*d++ = *s++;
	*d++ = *s++;
	*d++ = *s++;
	*d++ = *s++;
	*d++ = *s++;
	*d   = *s  ;
	A.nr = 1;
	return A;
}

SMLXTmpMatrix& MotionSpaceSpherical(SMLXTmpMatrix &A) 
{
	A.nr = 3;
	return A;
}

SMLXTmpMatrix& MotionSpaceSixDOF(SMLXTmpMatrix &A) 
{
	return A;
}

SMLXTmpMatrix& MotionSpacePrismatic(SMLXTmpMatrix &A) 
{
	/// SMLXTmpMatrix A0 = A;
	float* s;
	float* d;
	s = A[5];
	d = A[0];
	*d++ = *s++;
	*d++ = *s++;
	*d++ = *s++;
	*d++ = *s++;
	*d++ = *s++;
	*d   = *s  ;
	A.nr = 1;
	return A;
}


// Phi * Vector
SMLXTmpMatrix& WorldSpaceRevolute(const SMLXSpatialVector &A) 
{
	// Should not use vec[] to enable upcast from SMLXMatrix.
	SMLXTmpMatrix& R = TMat6x1();//(SMLXTmpMatrix&)SMLTMatrix<6, 1>::TMat();
	R.AdjustSize(6, 1);
	float* s = A.mat;
	float* d = R.mat;
	*d++ = 0.0f;
	*d++ = 0.0f;
	*d++ = s[0];
	*d++ = 0.0f;
	*d++ = 0.0f;
	*d   = 0.0f;
	return R;
}

SMLXTmpMatrix& WorldSpaceSpherical(const SMLXSpatialVector &A) 
{
	// Should not use vec[] to enable upcast from SMLXMatrix.
	SMLXTmpMatrix& R = TMat6x1();//(SMLXTmpMatrix&)SMLTMatrix<6, 1>::TMat();
	R.AdjustSize(6, 1);
	float* s = A.mat;
	float* d = R.mat;
	*d++ = s[0];
	*d++ = s[1];
	*d++ = s[2];
	*d++ = 0.0f;
	*d++ = 0.0f;
	*d   = 0.0f;
	return R;
}

SMLXTmpMatrix& WorldSpaceSixDOF(const SMLXSpatialVector &A) 
{
	// Should not use vec[] to enable upcast from SMLXMatrix.
	SMLXTmpMatrix& R = TMat6x1();//(SMLXTmpMatrix&)SMLTMatrix<6, 1>::TMat();
	R.AdjustSize(6, 1);
	float* s = A.mat;
	float* d = R.mat;
	*d++ = *s++;
	*d++ = *s++;
	*d++ = *s++;
	*d++ = *s++;
	*d++ = *s++;
	*d   = *s  ;
	return R;
}

SMLXTmpMatrix& WorldSpacePrismatic(const SMLXSpatialVector &A) 
{
	// Should not use vec[] to enable upcast from SMLXMatrix.
	SMLXTmpMatrix& R = TMat6x1();//(SMLXTmpMatrix&)SMLTMatrix<6, 1>::TMat();
	R.AdjustSize(6, 1);
	float* s = A.mat;
	float* d = R.mat;
	*d++ = 0.0f;
	*d++ = 0.0f;
	*d++ = 0.0f;
	*d++ = 0.0f;
	*d++ = 0.0f;
	*d   = s[0];
	return R;
}

// Phi * Matrix
SMLXTmpMatrix& WorldSpaceRevolute(SMLXMatrix &A) 
{
	if (A.nc == 1)
		return WorldSpaceRevolute((const SMLXSpatialVector&)A);

	assert(A.nr == 1);
	const float* s = A.Data();
	short ancol = A.ncols();
	SMLXTmpMatrix& R = SMLXMatrix::TMat(6, ancol);
	R.Zero();
	float* r2 = R[2];
	for (short j = 0; j < ancol; j++) {
		r2[j] = s[j];
	}
	return R;
}

SMLXTmpMatrix& WorldSpaceSpherical(SMLXMatrix &A) 
{
	if (A.nc == 1)
		return WorldSpaceSpherical((const SMLXSpatialVector&)A);

	assert(A.nr == 3);
	short ancol = A.ncols();
	SMLXTmpMatrix& R = SMLXMatrix::TMat(6, ancol);
	R.Zero();
	float* r0 = R[0];
	float* r1 = R[1];
	float* r2 = R[2];
	const float* s0 = A.Data();
	const float* s1 = A[1];
	const float* s2 = A[2];
	for (short j = 0; j < ancol; j++) {
		r0[j] = s0[j];
		r1[j] = s1[j];
		r2[j] = s2[j];
	}
	return R;
}

SMLXTmpMatrix& WorldSpaceSixDOF(SMLXMatrix &A) 
{
	if (A.nc == 1)
		return WorldSpaceSixDOF((const SMLXSpatialVector&)A);

	assert(A.nr == 6);
	return A.TMat();
}

SMLXTmpMatrix& WorldSpacePrismatic(SMLXMatrix &A) 
{
	if (A.nc == 1)
		return WorldSpacePrismatic((const SMLXSpatialVector&)A);

	assert(A.nr == 1);
	const float* s = A.Data();
	short ancol = A.ncols();
	SMLXTmpMatrix& R = SMLXMatrix::TMat(6, ancol);
	R.Zero();
	float* r5 = R[5];
	for (short j = 0; j < ancol; j++) {
		r5[j] = s[j];
	}
	return R;
}
