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

#include <smlxmatrix.h>

#ifndef NDEBUG
#define ASSERT_SUBEXPRESSION_COUNT assert(++subExpressionsCount <= SMLXMatrix::poolSize);
#else
	#define ASSERT_SUBEXPRESSION_COUNT 
#endif

#ifndef NDEBUG
	#define CLEAR_SUBEXPRESSION_COUNT subExpressionsCount = 0;
#else
	#define CLEAR_SUBEXPRESSION_COUNT
#endif

///////////////////////////
// Family of the inversions
// without pivoting.
///////////////////////////

bool Invert1x1(SMLXMatrix& m) {
	float* mat = m.Data();
	mat[0] = 1.0f / mat[0];
	return true;
}

#ifdef _PIII_CODE_

#include "xmmintrin.h"

bool Invert3x3(SMLXMatrix& m) {

	float check;
	__asm {
		mov		  eax,  DWORD PTR m
		mov		  ecx,  [eax]
        movss     xmm0, DWORD PTR [ecx+32]                    
        movlps    xmm1, QWORD PTR [ecx+16]                    
        movhps    xmm1, QWORD PTR [ecx+24]                    
        shufps    xmm0, xmm0, 0                               
        movlps    xmm2, QWORD PTR [ecx]                    
        movhps    xmm2, QWORD PTR [ecx+8]                    
        movlps    xmm0, QWORD PTR [ecx+24]                    
        movaps    xmm3, xmm0                                  
        shufps    xmm0, xmm1, 49h                              
        shufps    xmm0, xmm0, 93h                             
        movaps    xmm4, xmm1                                  
        shufps    xmm1, xmm2, 61h                              
        movaps    xmm5, xmm1                                  
        shufps    xmm1, xmm2, 93h                             
        mulps     xmm0, xmm1                                  
        movaps    xmm1, xmm3                                  
        shufps    xmm3, xmm4, 79h                            
        mulps     xmm3, xmm5                                  
        subps     xmm3, xmm0                                  
        movaps    xmm0, xmm4                                  
        shufps    xmm4, xmm2, 0BEh
        movaps    xmm5, xmm2                                  
        shufps    xmm2, xmm4, 21h                             
        mulps     xmm2, xmm3                                  
        movaps    xmm6, xmm2                                  
        shufps    xmm2, xmm2, 78h                          
        addps     xmm6, xmm2                                  
        shufps    xmm2, xmm2, 78h                             
        addps     xmm6, xmm2                                  
        shufps	  xmm6, xmm6, 0AAh

		movss	  DWORD PTR check, xmm6
		fld		  DWORD PTR check 
		fabs
		fstp	  DWORD PTR check 
	}
	if(check > 1e-8) {
		__asm {
			rcpps	  xmm7, xmm6
			mulps	  xmm6, xmm7
			mulps	  xmm6, xmm7
			addps	  xmm7, xmm7
			subps	  xmm7, xmm6
		    mulps     xmm3, xmm7                                  
			movaps    xmm2, xmm1                                  
	        shufps    xmm3, xmm3, 93h                            
		    movhps    QWORD PTR [ecx], xmm3                       
			shufps    xmm1, xmm5, 0B8h                            
	        movss     DWORD PTR [ecx+8], xmm3                     
		    shufps    xmm2, xmm0, 69h                           
			shufps    xmm1, xmm1, 93h                          
	        movaps    xmm3, xmm0                                  
		    shufps    xmm0, xmm5, 84h                          
			mulps     xmm1, xmm0                                  
	        movaps    xmm0, xmm5                                  
		    shufps    xmm5, xmm5, 2Eh                         
			mulps     xmm2, xmm5                                  
	        subps     xmm1, xmm2                                  
		    mulps     xmm1, xmm7                                  
			shufps    xmm1, xmm1, 0E1h                            
	        movss     DWORD PTR [ecx+12], xmm1                    
		    shufps    xmm4, xmm4, 93h                            
			movhps    QWORD PTR [ecx+16], xmm1                    
	        movaps    xmm1, xmm3                                  
		    shufps    xmm3, xmm0, 41h                           
			shufps    xmm1, xmm1, 2Fh                             
	        mulps     xmm4, xmm3                                  
		    shufps    xmm0, xmm0, 1Eh                           
			mulps     xmm1, xmm0                                  
	        subps     xmm1, xmm4                                  
		    mulps     xmm7, xmm1                                  
			shufps    xmm7, xmm7, 93h                            
	        movhps    QWORD PTR [ecx+24], xmm7                    
		    movss     DWORD PTR [ecx+32], xmm7                    
		}
		return true;
	}
	return false;
}

bool Invert4x4(SMLXMatrix& src) {
	//float* mat = m.Data();
	__m128	minor0, minor1, minor2, minor3;
	__m128	row0,   row1,   row2,   row3;
	__m128	det,    tmp1,   tmp2;

	float	*m = src.Data();//(float*)&src;

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)(m  ) ), (__m64*)(m+ 4) );
	row1	= _mm_loadh_pi( _mm_loadl_pi( row1, (__m64*)(m+8) ), (__m64*)(m+12) );

	row0	= _mm_shuffle_ps( tmp1, row1, 0x88 );
	row1	= _mm_shuffle_ps( row1, tmp1, 0xDD );

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)(m+ 2) ), (__m64*)(m+ 6) );
	row3	= _mm_loadh_pi( _mm_loadl_pi( row3, (__m64*)(m+10) ), (__m64*)(m+14) );

	row2	= _mm_shuffle_ps( tmp1, row3, 0x88 );
	row3	= _mm_shuffle_ps( row3, tmp1, 0xDD );
//	-----------------------------------------------
	tmp2	= _mm_mul_ps( row2, row3 );
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1);

	minor0	= _mm_mul_ps( row1, tmp1 );
	minor1	= _mm_mul_ps( row0, tmp1 );

	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );

	minor0	= _mm_sub_ps( _mm_mul_ps( row1, tmp1 ), minor0 );
	minor1	= _mm_sub_ps( _mm_mul_ps( row0, tmp1 ), minor1 );
	minor1	= _mm_shuffle_ps( minor1, minor1, 0x4E );
//	-----------------------------------------------
	tmp2	= _mm_mul_ps( row1, row2);
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );

	minor0	= _mm_add_ps( _mm_mul_ps( row3, tmp1 ), minor0  );
	minor3	= _mm_mul_ps( row0, tmp1 );

	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );

	minor0	= _mm_sub_ps( minor0, _mm_mul_ps( row3, tmp1 ) );
	minor3	= _mm_sub_ps( _mm_mul_ps( row0, tmp1 ), minor3 );
	minor3	= _mm_shuffle_ps( minor3, minor3, 0x4E );
//	-----------------------------------------------
	tmp2	= _mm_mul_ps( _mm_shuffle_ps( row1, row1, 0x4E ), row3 );
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );
	row2	= _mm_shuffle_ps( row2, row2, 0x4E );

	minor0	= _mm_add_ps( _mm_mul_ps( row2, tmp1 ), minor0 );
	minor2	= _mm_mul_ps( row0, tmp1 );

	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );

	minor0	= _mm_sub_ps( minor0, _mm_mul_ps( row2, tmp1 ) );
	minor2	= _mm_sub_ps( _mm_mul_ps( row0, tmp1 ), minor2 );
	minor2	= _mm_shuffle_ps( minor2, minor2, 0x4E );
//	-----------------------------------------------
	tmp2	= _mm_mul_ps( row0, row1);
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );	

	minor2	= _mm_add_ps( _mm_mul_ps( row3, tmp1 ), minor2 );
	minor3	= _mm_sub_ps( _mm_mul_ps( row2, tmp1 ), minor3 );
	
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );
	
	minor2	= _mm_sub_ps( _mm_mul_ps( row3, tmp1 ), minor2 );
	minor3	= _mm_sub_ps( minor3, _mm_mul_ps( row2, tmp1 ) );
//	-----------------------------------------------
	tmp2	= _mm_mul_ps( row0, row3);
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );
	
	minor1	= _mm_sub_ps( minor1, _mm_mul_ps( row2, tmp1 ) );
	minor2	= _mm_add_ps( _mm_mul_ps( row1, tmp1 ), minor2 );	
	
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );
	
	minor1	= _mm_add_ps( _mm_mul_ps( row2, tmp1 ), minor1 );
	minor2	= _mm_sub_ps( minor2, _mm_mul_ps( row1, tmp1 ) );	
//	-----------------------------------------------
	tmp2	= _mm_mul_ps( row0, row2);
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );
	
	minor1	= _mm_add_ps( _mm_mul_ps( row3, tmp1 ), minor1 );
	minor3	= _mm_sub_ps( minor3, _mm_mul_ps( row1, tmp1 ) );	
	
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );
	
	minor1	= _mm_sub_ps( minor1, _mm_mul_ps( row3, tmp1 ) );
	minor3	= _mm_add_ps( _mm_mul_ps( row1, tmp1 ), minor3 );	
//	-----------------------------------------------
	det		= _mm_mul_ps( row0, minor0 );
	det		= _mm_add_ps( _mm_shuffle_ps( det, det, 0x4E ), det );
	det		= _mm_add_ss( _mm_shuffle_ps( det, det, 0xB1 ), det );
	tmp1	= _mm_rcp_ss( det );

	det		= _mm_sub_ss( _mm_add_ss( tmp1, tmp1 ), _mm_mul_ss( det, _mm_mul_ss( tmp1, tmp1 ) ) );
	det		= _mm_shuffle_ps( det, det, 0x00 );

	//m		= (float*)this;

	minor0	= _mm_mul_ps( det, minor0 );
	_mm_storel_pi( (__m64*)( m ), minor0 );
	_mm_storeh_pi( (__m64*)(m+2), minor0 );

	minor1	= _mm_mul_ps( det, minor1 );
	_mm_storel_pi( (__m64*)(m+4), minor1 );
	_mm_storeh_pi( (__m64*)(m+6), minor1 );

	minor2	= _mm_mul_ps( det, minor2 );
	_mm_storel_pi( (__m64*)(m+ 8), minor2 );
	_mm_storeh_pi( (__m64*)(m+10), minor2 );

	minor3	= _mm_mul_ps( det, minor3 );
	_mm_storel_pi( (__m64*)(m+12), minor3 );
	_mm_storeh_pi( (__m64*)(m+14), minor3 );

	return true;
}

#else
bool Invert3x3(SMLXMatrix& m) {
	float* mat = m.Data();
	float d, di;
	di = mat[0];
	mat[0] = d = 1.0f / di;
	mat[3] *= -d;
	mat[6] *= -d;
	mat[1] *= d;
	mat[2] *= d;
	mat[4] += mat[3] * mat[1] * di;
	mat[5] += mat[3] * mat[2] * di;
	mat[7] += mat[6] * mat[1] * di;
	mat[8] += mat[6] * mat[2] * di;
	di = mat[4];
	mat[4] = d = 1.0f / di;
	mat[1] *= -d;
	mat[7] *= -d;
	mat[3] *= d;
	mat[5] *= d;
	mat[0] += mat[1] * mat[3] * di;
	mat[2] += mat[1] * mat[5] * di;
	mat[6] += mat[7] * mat[3] * di;
	mat[8] += mat[7] * mat[5] * di;
	di = mat[8];
	mat[8] = d = 1.0f / di;
	mat[2] *= -d;
	mat[5] *= -d;
	mat[6] *= d;
	mat[7] *= d;
	mat[0] += mat[2] * mat[6] * di;
	mat[1] += mat[2] * mat[7] * di;
	mat[3] += mat[5] * mat[6] * di;
	mat[4] += mat[5] * mat[7] * di;
	return true;
}

bool Invert4x4(SMLXMatrix& m) {
	float* mat = m.Data();
	float d, di;
	di = mat[0];
	mat[0] = d = 1.0f / di;
	mat[4] *= -d;
	mat[8] *= -d;
	mat[12] *= -d;
	mat[1] *= d;
	mat[2] *= d;
	mat[3] *= d;
	mat[5] += mat[4] * mat[1] * di;
	mat[6] += mat[4] * mat[2] * di;
	mat[7] += mat[4] * mat[3] * di;
	mat[9] += mat[8] * mat[1] * di;
	mat[10] += mat[8] * mat[2] * di;
	mat[11] += mat[8] * mat[3] * di;
	mat[13] += mat[12] * mat[1] * di;
	mat[14] += mat[12] * mat[2] * di;
	mat[15] += mat[12] * mat[3] * di;
	di = mat[5];
	mat[5] = d = 1.0f / di;
	mat[1] *= -d;
	mat[9] *= -d;
	mat[13] *= -d;
	mat[4] *= d;
	mat[6] *= d;
	mat[7] *= d;
	mat[0] += mat[1] * mat[4] * di;
	mat[2] += mat[1] * mat[6] * di;
	mat[3] += mat[1] * mat[7] * di;
	mat[8] += mat[9] * mat[4] * di;
	mat[10] += mat[9] * mat[6] * di;
	mat[11] += mat[9] * mat[7] * di;
	mat[12] += mat[13] * mat[4] * di;
	mat[14] += mat[13] * mat[6] * di;
	mat[15] += mat[13] * mat[7] * di;
	di = mat[10];
	mat[10] = d = 1.0f / di;
	mat[2] *= -d;
	mat[6] *= -d;
	mat[14] *= -d;
	mat[8] *= d;
	mat[9] *= d;
	mat[11] *= d;
	mat[0] += mat[2] * mat[8] * di;
	mat[1] += mat[2] * mat[9] * di;
	mat[3] += mat[2] * mat[11] * di;
	mat[4] += mat[6] * mat[8] * di;
	mat[5] += mat[6] * mat[9] * di;
	mat[7] += mat[6] * mat[11] * di;
	mat[12] += mat[14] * mat[8] * di;
	mat[13] += mat[14] * mat[9] * di;
	mat[15] += mat[14] * mat[11] * di;
	di = mat[15];
	mat[15] = d = 1.0f / di;
	mat[3] *= -d;
	mat[7] *= -d;
	mat[11] *= -d;
	mat[12] *= d;
	mat[13] *= d;
	mat[14] *= d;
	mat[0] += mat[3] * mat[12] * di;
	mat[1] += mat[3] * mat[13] * di;
	mat[2] += mat[3] * mat[14] * di;
	mat[4] += mat[7] * mat[12] * di;
	mat[5] += mat[7] * mat[13] * di;
	mat[6] += mat[7] * mat[14] * di;
	mat[8] += mat[11] * mat[12] * di;
	mat[9] += mat[11] * mat[13] * di;
	mat[10] += mat[11] * mat[14] * di;
	return true;
}

#endif

bool Invert5x5(SMLXMatrix& m) {
	float* mat = m.Data();
	float d, di;
	di = mat[0];
	mat[0] = d = 1.0f / di;
	mat[5] *= -d;
	mat[10] *= -d;
	mat[15] *= -d;
	mat[20] *= -d;
	mat[1] *= d;
	mat[2] *= d;
	mat[3] *= d;
	mat[4] *= d;
	mat[6] += mat[5] * mat[1] * di;
	mat[7] += mat[5] * mat[2] * di;
	mat[8] += mat[5] * mat[3] * di;
	mat[9] += mat[5] * mat[4] * di;
	mat[11] += mat[10] * mat[1] * di;
	mat[12] += mat[10] * mat[2] * di;
	mat[13] += mat[10] * mat[3] * di;
	mat[14] += mat[10] * mat[4] * di;
	mat[16] += mat[15] * mat[1] * di;
	mat[17] += mat[15] * mat[2] * di;
	mat[18] += mat[15] * mat[3] * di;
	mat[19] += mat[15] * mat[4] * di;
	mat[21] += mat[20] * mat[1] * di;
	mat[22] += mat[20] * mat[2] * di;
	mat[23] += mat[20] * mat[3] * di;
	mat[24] += mat[20] * mat[4] * di;
	di = mat[6];
	mat[6] = d = 1.0f / di;
	mat[1] *= -d;
	mat[11] *= -d;
	mat[16] *= -d;
	mat[21] *= -d;
	mat[5] *= d;
	mat[7] *= d;
	mat[8] *= d;
	mat[9] *= d;
	mat[0] += mat[1] * mat[5] * di;
	mat[2] += mat[1] * mat[7] * di;
	mat[3] += mat[1] * mat[8] * di;
	mat[4] += mat[1] * mat[9] * di;
	mat[10] += mat[11] * mat[5] * di;
	mat[12] += mat[11] * mat[7] * di;
	mat[13] += mat[11] * mat[8] * di;
	mat[14] += mat[11] * mat[9] * di;
	mat[15] += mat[16] * mat[5] * di;
	mat[17] += mat[16] * mat[7] * di;
	mat[18] += mat[16] * mat[8] * di;
	mat[19] += mat[16] * mat[9] * di;
	mat[20] += mat[21] * mat[5] * di;
	mat[22] += mat[21] * mat[7] * di;
	mat[23] += mat[21] * mat[8] * di;
	mat[24] += mat[21] * mat[9] * di;
	di = mat[12];
	mat[12] = d = 1.0f / di;
	mat[2] *= -d;
	mat[7] *= -d;
	mat[17] *= -d;
	mat[22] *= -d;
	mat[10] *= d;
	mat[11] *= d;
	mat[13] *= d;
	mat[14] *= d;
	mat[0] += mat[2] * mat[10] * di;
	mat[1] += mat[2] * mat[11] * di;
	mat[3] += mat[2] * mat[13] * di;
	mat[4] += mat[2] * mat[14] * di;
	mat[5] += mat[7] * mat[10] * di;
	mat[6] += mat[7] * mat[11] * di;
	mat[8] += mat[7] * mat[13] * di;
	mat[9] += mat[7] * mat[14] * di;
	mat[15] += mat[17] * mat[10] * di;
	mat[16] += mat[17] * mat[11] * di;
	mat[18] += mat[17] * mat[13] * di;
	mat[19] += mat[17] * mat[14] * di;
	mat[20] += mat[22] * mat[10] * di;
	mat[21] += mat[22] * mat[11] * di;
	mat[23] += mat[22] * mat[13] * di;
	mat[24] += mat[22] * mat[14] * di;
	di = mat[18];
	mat[18] = d = 1.0f / di;
	mat[3] *= -d;
	mat[8] *= -d;
	mat[13] *= -d;
	mat[23] *= -d;
	mat[15] *= d;
	mat[16] *= d;
	mat[17] *= d;
	mat[19] *= d;
	mat[0] += mat[3] * mat[15] * di;
	mat[1] += mat[3] * mat[16] * di;
	mat[2] += mat[3] * mat[17] * di;
	mat[4] += mat[3] * mat[19] * di;
	mat[5] += mat[8] * mat[15] * di;
	mat[6] += mat[8] * mat[16] * di;
	mat[7] += mat[8] * mat[17] * di;
	mat[9] += mat[8] * mat[19] * di;
	mat[10] += mat[13] * mat[15] * di;
	mat[11] += mat[13] * mat[16] * di;
	mat[12] += mat[13] * mat[17] * di;
	mat[14] += mat[13] * mat[19] * di;
	mat[20] += mat[23] * mat[15] * di;
	mat[21] += mat[23] * mat[16] * di;
	mat[22] += mat[23] * mat[17] * di;
	mat[24] += mat[23] * mat[19] * di;
	di = mat[24];
	mat[24] = d = 1.0f / di;
	mat[4] *= -d;
	mat[9] *= -d;
	mat[14] *= -d;
	mat[19] *= -d;
	mat[20] *= d;
	mat[21] *= d;
	mat[22] *= d;
	mat[23] *= d;
	mat[0] += mat[4] * mat[20] * di;
	mat[1] += mat[4] * mat[21] * di;
	mat[2] += mat[4] * mat[22] * di;
	mat[3] += mat[4] * mat[23] * di;
	mat[5] += mat[9] * mat[20] * di;
	mat[6] += mat[9] * mat[21] * di;
	mat[7] += mat[9] * mat[22] * di;
	mat[8] += mat[9] * mat[23] * di;
	mat[10] += mat[14] * mat[20] * di;
	mat[11] += mat[14] * mat[21] * di;
	mat[12] += mat[14] * mat[22] * di;
	mat[13] += mat[14] * mat[23] * di;
	mat[15] += mat[19] * mat[20] * di;
	mat[16] += mat[19] * mat[21] * di;
	mat[17] += mat[19] * mat[22] * di;
	mat[18] += mat[19] * mat[23] * di;
	return true;
}

#ifdef _PIII_CODE_

bool Invert_6x6( float *src )
{

#define EPSILON		1e-8
#define	REAL_ZERO(x)	( fabs(x) < EPSILON ? 1:0 )

	__m128	minor0, minor1, minor2, minor3;
	__m128	det, tmp1, tmp2, tmp3, mask, index;
	__m128	b[6];
	__m128	row[6];

	static const unsigned long	minus_hex	= 0x80000000;
	static const __m128			minus		= _mm_set_ps1( *(float*)&minus_hex );
	static const __m128			e			= _mm_set_ps( 1.0f, 0.0f, 0.0f, 1.0f );
	static const __m128			epsilon		= _mm_set_ss(  EPSILON );

	float	max, f;

	int i, j, n1, n2, k, mask1, mask2, mask3;

	// Loading matrixes: 4x2 to row[0], row[1] and 4x4 to row[2]...row[5].

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)(&src[12]) ), (__m64*)(&src[18]) );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)(&src[24]) ), (__m64*)(&src[30]) );

	row[0]	= _mm_shuffle_ps( tmp1, tmp2, 0x88 );
	row[1]	= _mm_shuffle_ps( tmp1, tmp2, 0xDD );

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)(&src[14]) ), (__m64*)(&src[20]) );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)(&src[26]) ), (__m64*)(&src[32]) );

	row[2]	= _mm_shuffle_ps( tmp1, tmp2, 0x88 );
	row[3]	= _mm_shuffle_ps( tmp1, tmp2, 0xDD );

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)(&src[16]) ), (__m64*)(&src[22]) );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)(&src[28]) ), (__m64*)(&src[34]) );

	row[4]	= _mm_shuffle_ps( tmp1, tmp2, 0x88 );
	row[5]	= _mm_shuffle_ps( tmp1, tmp2, 0xDD );

	// Finding the max( |src[0]|, |src[1]|, ..., |src[5]| ).

	tmp1	= _mm_loadh_pi( _mm_load_ss( &src[2] ), (__m64*)&src[0] );
	tmp2	= _mm_loadh_pi( _mm_load_ss( &src[3] ), (__m64*)&src[4] );

	tmp1	= _mm_andnot_ps( minus, tmp1 );
	tmp2	= _mm_andnot_ps( minus, tmp2 );

	tmp3	= _mm_max_ps( tmp1, tmp2 );
	tmp3	= _mm_max_ps( tmp3, _mm_shuffle_ps( tmp3, tmp3, _MM_SHUFFLE( 3, 2, 3, 2 ) ) );
	tmp3	= _mm_max_ss( tmp3, _mm_shuffle_ps( tmp3, tmp3, _MM_SHUFFLE( 1, 1, 1, 1 ) ) );
	tmp3	= _mm_shuffle_ps( tmp3, tmp3, _MM_SHUFFLE( 0, 0, 0, 0 ) );

	mask1	= _mm_movemask_ps( _mm_cmpeq_ps( tmp1, tmp3 ) );
	mask1	|= _mm_movemask_ps( _mm_cmpeq_ps( tmp2, tmp3 ) )<<4;

	mask2	= mask1 & 0x98;
	mask2	= mask2 - (mask2 << 1);
	n1		= ( (unsigned int)mask2 ) >> 31;

	n1		|= ( (mask1 & 0x11) != 0 ) << 1;

	mask2	= mask1 & 0xC0;
	mask2	= mask2 - (mask2 << 1);
	n1		|= ( ( (unsigned int)mask2 ) >> 29 ) & 4;

	if( REAL_ZERO( src[n1] ) )
		return false;

	// The first Gauss iteration.

	tmp1	= row[n1];
	row[n1]	= row[0];
	row[0]	= tmp1;

	tmp2	= _mm_load_ss( &src[n1] );

	src[n1]	= src[0];

	f		= src[n1+6];
	src[n1+6]	= src[6];
	src[6]	= f;

	tmp1	= _mm_rcp_ss( tmp2 );
	tmp2	= _mm_sub_ss( _mm_add_ss( tmp1, tmp1 ), _mm_mul_ss( tmp2, _mm_mul_ss( tmp1, tmp1 ) ) );

	_mm_store_ss( &src[0], tmp2 );

	tmp2	= _mm_shuffle_ps( tmp2, tmp2, 0x00 );
	row[0]	= _mm_mul_ps( row[0], tmp2 );
	
	tmp1	= _mm_load_ss( &src[1] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	row[1]	= _mm_sub_ps( row[1], _mm_mul_ps( row[0], tmp1 ) );

	tmp1	= _mm_load_ss( &src[2] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	row[2]	= _mm_sub_ps( row[2], _mm_mul_ps( row[0], tmp1 ) );

	tmp1	= _mm_load_ss( &src[3] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	row[3]	= _mm_sub_ps( row[3], _mm_mul_ps( row[0], tmp1 ) );

	tmp1	= _mm_load_ss( &src[4] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	row[4]	= _mm_sub_ps( row[4], _mm_mul_ps( row[0], tmp1 ) );

	tmp1	= _mm_load_ss( &src[5] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	row[5]	= _mm_sub_ps( row[5], _mm_mul_ps( row[0], tmp1 ) );

	tmp3	= _mm_load_ss( &src[6] );
	tmp3	= _mm_mul_ss( tmp3, tmp2 );
	_mm_store_ss( &src[6], tmp3 );

	tmp1	= _mm_load_ss( &src[1] );
	tmp2	= _mm_load_ss( &src[7] );
	tmp2	= _mm_sub_ss( tmp2, _mm_mul_ss( tmp1, tmp3 ) );
	_mm_store_ss( &src[7], tmp2 );

	tmp3	= _mm_shuffle_ps( tmp3, tmp3, 0x00 );
	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src[2] ), (__m64*)&src[ 4] );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src[8] ), (__m64*)&src[10] );

	tmp2	= _mm_sub_ps( tmp2, _mm_mul_ps( tmp1, tmp3 ) );

	_mm_storel_pi( (__m64*)&src[ 8], tmp2 );
	_mm_storeh_pi( (__m64*)&src[10], tmp2 );

	// Finding the max( src[7], src[8], ..., src[11] ).

	tmp1	= _mm_loadh_pi( _mm_load_ss( &src[7] ), (__m64*)&src[10] );
	tmp2	= _mm_loadl_pi( tmp2, (__m64*)&src[8] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, _MM_SHUFFLE(0,3,2,2) );

	tmp1	= _mm_andnot_ps( minus, tmp1 );
	tmp2	= _mm_andnot_ps( minus, tmp2 );

	tmp3	= _mm_max_ps( tmp1, tmp2 );
	tmp3	= _mm_max_ps( tmp3, _mm_shuffle_ps( tmp1, tmp1, _MM_SHUFFLE(0,0,3,2) ) );
	tmp3	= _mm_max_ss( tmp3, _mm_shuffle_ps( tmp3, tmp3, _MM_SHUFFLE(1,1,1,1) ) );
	tmp3	= _mm_shuffle_ps( tmp3, tmp3, _MM_SHUFFLE(0,0,0,0) );

	mask1	= _mm_movemask_ps( _mm_cmpeq_ps( tmp2, tmp3 ) );
	mask2	= _mm_movemask_ps( _mm_cmpeq_ps( tmp1, tmp3 ) );

	n2		= (( mask1 & 3 ) | ( mask2 & 7 )) + 7;

	if( REAL_ZERO(src[n2]) )
		return false;

	// The second Gauss iteration.

	tmp2	= _mm_load_ss( &src[n2] );
	src[n2]	= src[7];

	n2		-= 6;

	tmp1	= row[n2];
	row[n2]	= row[1];
	row[1]	= tmp1;

	f		= src[n2];
	src[n2]	= src[1];
	src[1]	= f;	

	//if( n2==n1 ) n2 = 0;
	n2		*= (n1!=n2);
	
	tmp1	= _mm_rcp_ss( tmp2 );
	tmp2	= _mm_sub_ss( _mm_add_ss( tmp1, tmp1 ), _mm_mul_ss( tmp2, _mm_mul_ss( tmp1, tmp1 ) ) );

	_mm_store_ss( &src[7], tmp2 );

	tmp2	= _mm_shuffle_ps( tmp2, tmp2, 0x00 );	
	row[1]	= _mm_mul_ps( row[1], tmp2 );

	tmp1	= _mm_load_ss( &src[6] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );	
	row[0]	= _mm_sub_ps( row[0], _mm_mul_ps( row[1], tmp1 ) );

	tmp1	= _mm_load_ss( &src[8] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	row[2]	= _mm_sub_ps( row[2], _mm_mul_ps( row[1], tmp1 ) );

	tmp1	= _mm_load_ss( &src[9] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	row[3]	= _mm_sub_ps( row[3], _mm_mul_ps( row[1], tmp1 ) );

	tmp1	= _mm_load_ss( &src[10] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	row[4]	= _mm_sub_ps( row[4], _mm_mul_ps( row[1], tmp1 ) );

	tmp1	= _mm_load_ss( &src[11] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	row[5]	= _mm_sub_ps( row[5], _mm_mul_ps( row[1], tmp1 ) );

	row[0]	= _mm_xor_ps( row[0], minus );
	row[1]	= _mm_xor_ps( row[1], minus );

	// Inverting the matrix 4x4 by the Kramers method.

	row[3]	= _mm_shuffle_ps( row[3], row[3], 0x4E);
	row[5]	= _mm_shuffle_ps( row[5], row[5], 0x4E);

	tmp2	= _mm_mul_ps( row[4], row[5] );
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1);

	minor0	= _mm_mul_ps( row[3], tmp1 );
	minor1	= _mm_mul_ps( row[2], tmp1 );

	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );

	minor0	= _mm_sub_ps( _mm_mul_ps( row[3], tmp1 ), minor0 );
	minor1	= _mm_sub_ps( _mm_mul_ps( row[2], tmp1 ), minor1 );
	minor1	= _mm_shuffle_ps( minor1, minor1, 0x4E );
//	-----------------------------------------------
	tmp2	= _mm_mul_ps( row[3], row[4]);
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );

	minor0	= _mm_add_ps( _mm_mul_ps( row[5], tmp1 ), minor0  );
	minor3	= _mm_mul_ps( row[2], tmp1 );

	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );

	minor0	= _mm_sub_ps( minor0, _mm_mul_ps( row[5], tmp1 ) );
	minor3	= _mm_sub_ps( _mm_mul_ps( row[2], tmp1 ), minor3 );
	minor3	= _mm_shuffle_ps( minor3, minor3, 0x4E );
//	-----------------------------------------------
	tmp2	= _mm_mul_ps( _mm_shuffle_ps( row[3], row[3], 0x4E ), row[5] );
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );
	row[4]	= _mm_shuffle_ps( row[4], row[4], 0x4E );

	minor0	= _mm_add_ps( _mm_mul_ps( row[4], tmp1 ), minor0 );
	minor2	= _mm_mul_ps( row[2], tmp1 );

	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );

	minor0	= _mm_sub_ps( minor0, _mm_mul_ps( row[4], tmp1 ) );
	minor2	= _mm_sub_ps( _mm_mul_ps( row[2], tmp1 ), minor2 );
	minor2	= _mm_shuffle_ps( minor2, minor2, 0x4E );
//	-----------------------------------------------
	tmp2	= _mm_mul_ps( row[2], row[3]);
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );	

	minor2	= _mm_add_ps( _mm_mul_ps( row[5], tmp1 ), minor2 );
	minor3	= _mm_sub_ps( _mm_mul_ps( row[4], tmp1 ), minor3 );
	
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );
	
	minor2	= _mm_sub_ps( _mm_mul_ps( row[5], tmp1 ), minor2 );
	minor3	= _mm_sub_ps( minor3, _mm_mul_ps( row[4], tmp1 ) );
//	-----------------------------------------------
	tmp2	= _mm_mul_ps( row[2], row[5]);
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );
	
	minor1	= _mm_sub_ps( minor1, _mm_mul_ps( row[4], tmp1 ) );
	minor2	= _mm_add_ps( _mm_mul_ps( row[3], tmp1 ), minor2 );	
	
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );
	
	minor1	= _mm_add_ps( _mm_mul_ps( row[4], tmp1 ), minor1 );
	minor2	= _mm_sub_ps( minor2, _mm_mul_ps( row[3], tmp1 ) );	
//	-----------------------------------------------
	tmp2	= _mm_mul_ps( row[2], row[4]);
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );
	
	minor1	= _mm_add_ps( _mm_mul_ps( row[5], tmp1 ), minor1 );
	minor3	= _mm_sub_ps( minor3, _mm_mul_ps( row[3], tmp1 ) );	
	
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );
	
	minor1	= _mm_sub_ps( minor1, _mm_mul_ps( row[5], tmp1 ) );
	minor3	= _mm_add_ps( _mm_mul_ps( row[3], tmp1 ), minor3 );	
//	-----------------------------------------------
	det		= _mm_mul_ps( row[2], minor0 );
	det		= _mm_add_ps( _mm_shuffle_ps( det, det, 0x4E ), det );
	det		= _mm_add_ss( _mm_shuffle_ps( det, det, 0xB1 ), det );

	if( _mm_movemask_ps( _mm_cmplt_ss( _mm_andnot_ps( minus, det), epsilon ) ) & 1 )
		return false;

	tmp1	= _mm_rcp_ss( det );
	det		= _mm_sub_ss( _mm_add_ss( tmp1, tmp1 ), _mm_mul_ss( det, _mm_mul_ss( tmp1, tmp1 ) ) );
	det		= _mm_shuffle_ps( det, det, 0x00 );

	row[2]	= _mm_mul_ps( det, minor0 );
	row[3]	= _mm_mul_ps( det, minor1 );

	///////////////////////////////////////

	b[0]	= _mm_unpacklo_ps( row[0], row[1] );
	b[2]	= _mm_unpackhi_ps( row[0], row[1] );
	row[4]	= _mm_mul_ps( det, minor2 );
	b[1]	= _mm_shuffle_ps( b[0], b[2], 0x4E );
	row[5]	= _mm_mul_ps( det, minor3 );
	b[3]	= _mm_shuffle_ps( b[2], b[0], 0x4E );

	tmp1	= _mm_shuffle_ps( row[2], row[3], 0x50 );
	tmp2	= _mm_mul_ps( b[0], tmp1 );

	tmp1	= _mm_shuffle_ps( row[2], row[3], 0xA5 );
	tmp2	= _mm_add_ps( tmp2, _mm_mul_ps( b[1], tmp1 ) );

	tmp1	= _mm_shuffle_ps( row[2], row[3], 0xFA );
	tmp2	= _mm_add_ps( tmp2, _mm_mul_ps( b[2], tmp1 ) );

	tmp1	= _mm_shuffle_ps( row[2], row[3], 0x0F );
	row[0]	= _mm_add_ps( tmp2, _mm_mul_ps( b[3], tmp1 ) );

	tmp1	= _mm_shuffle_ps( row[4], row[5], 0x50 );
	tmp2	= _mm_mul_ps( b[0], tmp1 );

	tmp1	= _mm_shuffle_ps( row[4], row[5], 0xA5 );
	tmp2	= _mm_add_ps( tmp2, _mm_mul_ps( b[1], tmp1 ) );

	tmp1	= _mm_shuffle_ps( row[4], row[5], 0xFA );
	tmp2	= _mm_add_ps( tmp2, _mm_mul_ps( b[2], tmp1 ) );

	tmp1	= _mm_shuffle_ps( row[4], row[5], 0x0F );
	row[1]	= _mm_add_ps( tmp2, _mm_mul_ps( b[3], tmp1 ) );

	b[2]	= _mm_shuffle_ps( row[0], row[0], 0x44 );
	b[3]	= _mm_shuffle_ps( row[0], row[0], 0xEE );
	b[4]	= _mm_shuffle_ps( row[1], row[1], 0x44 );
	b[5]	= _mm_shuffle_ps( row[1], row[1], 0xEE );

	// Calculating row number n2

	tmp1	= _mm_load_ss( &src[8] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );

	b  [1]	= _mm_sub_ps( _mm_shuffle_ps( e, e, 0x4E ), _mm_mul_ps( b[2], tmp1 ) );
	row[1]	= _mm_xor_ps( _mm_mul_ps( row[2], tmp1 ), minus );

	tmp1	= _mm_load_ss( &src[9] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );

	b  [1]	= _mm_sub_ps( b  [1], _mm_mul_ps( b  [3], tmp1 ) );
	row[1]	= _mm_sub_ps( row[1], _mm_mul_ps( row[3], tmp1 ) );

	tmp1	= _mm_load_ss( &src[10] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );

	b  [1]	= _mm_sub_ps( b  [1], _mm_mul_ps( b  [4], tmp1 ) );
	row[1]	= _mm_sub_ps( row[1], _mm_mul_ps( row[4], tmp1 ) );

	tmp1	= _mm_load_ss( &src[11] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );

	b  [1]	= _mm_sub_ps( b  [1], _mm_mul_ps( b  [5], tmp1 ) );
	row[1]	= _mm_sub_ps( row[1], _mm_mul_ps( row[5], tmp1 ) );

	tmp1	= _mm_load_ss( &src[6] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );

	b  [1]	= _mm_sub_ps( b[1], _mm_mul_ps( e, tmp1 ) );

	tmp2	= _mm_load_ss( &src[7] );
	tmp2	= _mm_shuffle_ps( tmp2, tmp2, 0x00 );

	b  [1]	= _mm_mul_ps( b  [1], tmp2 );
	row[1]	= _mm_mul_ps( row[1], tmp2 );

	// Calculating row number n1

	tmp1	= _mm_load_ss( &src[1] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	
	b  [0]	= _mm_sub_ps( e, _mm_mul_ps( b[1], tmp1 ) );
	row[0]	= _mm_xor_ps( _mm_mul_ps( row[1], tmp1 ), minus );

	tmp1	= _mm_load_ss( &src[2] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );

	b  [0]	= _mm_sub_ps( b  [0], _mm_mul_ps( b  [2], tmp1 ) );
	row[0]	= _mm_sub_ps( row[0], _mm_mul_ps( row[2], tmp1 ) );

	tmp1	= _mm_load_ss( &src[3] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );

	b  [0]	= _mm_sub_ps( b  [0], _mm_mul_ps( b  [3], tmp1 ) );
	row[0]	= _mm_sub_ps( row[0], _mm_mul_ps( row[3], tmp1 ) );

	tmp1	= _mm_load_ss( &src[4] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );

	b  [0]	= _mm_sub_ps( b  [0], _mm_mul_ps( b  [4], tmp1 ) );
	row[0]	= _mm_sub_ps( row[0], _mm_mul_ps( row[4], tmp1 ) );
	
	tmp1	= _mm_load_ss( &src[5] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );

	b  [0]	= _mm_sub_ps( b  [0], _mm_mul_ps( b  [5], tmp1 ) );
	row[0]	= _mm_sub_ps( row[0], _mm_mul_ps( row[5], tmp1 ) );

	tmp2	= _mm_load_ss( &src[0] );
	tmp2	= _mm_shuffle_ps( tmp2, tmp2, 0x00 );

	b  [0]	= _mm_mul_ps( b  [0], tmp2 );
	row[0]	= _mm_mul_ps( row[0], tmp2 );

	n2		= (n2==0)*(n1-n2)+n2;

	tmp1	= row[ 1];	row[ 1]	= row[n2];	row[n2]	= tmp1;
	tmp2	= b  [ 1];	b  [ 1]	= b  [n2];	b  [n2]	= tmp2;

	tmp1	= row[ 0];	row[ 0]	= row[n1];	row[n1]	= tmp1;
	tmp2	= b  [ 0];	b  [ 0]	= b  [n1];	b  [n1]	= tmp2;	

	_mm_storel_pi( (__m64*)&src[ 0], b  [0] );
	_mm_storel_pi( (__m64*)&src[ 2], row[0] );
	_mm_storeh_pi( (__m64*)&src[ 4], row[0] );
	
	_mm_storel_pi( (__m64*)&src[ 6], b  [1] );
	_mm_storel_pi( (__m64*)&src[ 8], row[1] );
	_mm_storeh_pi( (__m64*)&src[10], row[1] );
	
	_mm_storel_pi( (__m64*)&src[12], b  [2] );
	_mm_storel_pi( (__m64*)&src[14], row[2] );
	_mm_storeh_pi( (__m64*)&src[16], row[2] );

	_mm_storel_pi( (__m64*)&src[18], b  [3] );
	_mm_storel_pi( (__m64*)&src[20], row[3] );
	_mm_storeh_pi( (__m64*)&src[22], row[3] );

	_mm_storel_pi( (__m64*)&src[24], b  [4] );
	_mm_storel_pi( (__m64*)&src[26], row[4] );
	_mm_storeh_pi( (__m64*)&src[28], row[4] );
	
	_mm_storel_pi( (__m64*)&src[30], b  [5] );
	_mm_storel_pi( (__m64*)&src[32], row[5] );
	_mm_storeh_pi( (__m64*)&src[34], row[5] );

#undef	EPSILON
#undef	REAL_ZERO

	return true;

} // Invert_6x6

bool Invert_6x6_fast( float *src )
{

#define EPSILON		1e-8
#define	REAL_ZERO(x)	( fabs(x) < EPSILON ? 1:0 )

	__m128	minor0, minor1, minor2, minor3;
	__m128	det, tmp1, tmp2;
	__m128	b0, b1, b2, b3;
	__m128	row[6];

	static const unsigned long	minus_hex	= 0x80000000;
	static const __m128	minus	= _mm_set_ps1( *(float*)&minus_hex );
	static const __m128	zero	= _mm_setzero_ps( );
	static const __m128	e	= _mm_set_ps( 1.0f, 0.0f, 0.0f, 1.0f );
	static const __m128	epsilon		= _mm_set_ss(  EPSILON );
	static const __m128	epsilon1	= _mm_set_ss( -EPSILON );

	// Loading matrixes: 4x2 to row[0], row[1] and 4x4 to row[2]...row[5].

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)(&src[12]) ), (__m64*)(&src[18]) );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)(&src[24]) ), (__m64*)(&src[30]) );

	row[0]	= _mm_shuffle_ps( tmp1, tmp2, 0x88 );
	row[1]	= _mm_shuffle_ps( tmp1, tmp2, 0xDD );

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)(&src[14]) ), (__m64*)(&src[20]) );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)(&src[26]) ), (__m64*)(&src[32]) );

	row[2]	= _mm_shuffle_ps( tmp1, tmp2, 0x88 );
	row[3]	= _mm_shuffle_ps( tmp1, tmp2, 0xDD );

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)(&src[16]) ), (__m64*)(&src[22]) );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)(&src[28]) ), (__m64*)(&src[34]) );

	row[4]	= _mm_shuffle_ps( tmp1, tmp2, 0x88 );
	row[5]	= _mm_shuffle_ps( tmp1, tmp2, 0xDD );

	// The first Gauss iteration.

	tmp2	= _mm_load_ss( &src[0] );
	tmp1	= _mm_rcp_ss( tmp2 );
	tmp2	= _mm_sub_ss( _mm_add_ss( tmp1, tmp1 ), _mm_mul_ss( tmp2, _mm_mul_ss( tmp1, tmp1 ) ) );

	_mm_store_ss( &src[0], tmp2 );

	tmp2	= _mm_shuffle_ps( tmp2, tmp2, 0x00 );
	row[0]	= _mm_mul_ps( row[0], tmp2 );
	
	tmp1	= _mm_load_ss( &src[1] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	row[1]	= _mm_sub_ps( row[1], _mm_mul_ps( row[0], tmp1 ) );

	tmp1	= _mm_load_ss( &src[2] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	row[2]	= _mm_sub_ps( row[2], _mm_mul_ps( row[0], tmp1 ) );

	tmp1	= _mm_load_ss( &src[3] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	row[3]	= _mm_sub_ps( row[3], _mm_mul_ps( row[0], tmp1 ) );

	tmp1	= _mm_load_ss( &src[4] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	row[4]	= _mm_sub_ps( row[4], _mm_mul_ps( row[0], tmp1 ) );

	tmp1	= _mm_load_ss( &src[5] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	row[5]	= _mm_sub_ps( row[5], _mm_mul_ps( row[0], tmp1 ) );

	b0		= _mm_load_ss( &src[6] );
	b0		= _mm_mul_ss( b0, tmp2 );
	_mm_store_ss( &src[6], b0 );

	tmp1	= _mm_load_ss( &src[1] );
	tmp2	= _mm_load_ss( &src[7] );
	tmp2	= _mm_sub_ss( tmp2, _mm_mul_ss( tmp1, b0 ) );
	_mm_store_ss( &src[7], tmp2 );

	b0		= _mm_shuffle_ps( b0, b0, 0x00 );
	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src[2] ), (__m64*)&src[ 4] );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src[8] ), (__m64*)&src[10] );

	tmp2	= _mm_sub_ps( tmp2, _mm_mul_ps( tmp1, b0 ) );

	_mm_storel_pi( (__m64*)&src[ 8], tmp2 );
	_mm_storeh_pi( (__m64*)&src[10], tmp2 );

	// The second Gauss iteration.

	tmp2	= _mm_load_ss( &src[7] );
	tmp1	= _mm_rcp_ss( tmp2 );
	tmp2	= _mm_sub_ss( _mm_add_ss( tmp1, tmp1 ), _mm_mul_ss( tmp2, _mm_mul_ss( tmp1, tmp1 ) ) );

	_mm_store_ss( &src[7], tmp2 );

	tmp2	= _mm_shuffle_ps( tmp2, tmp2, 0x00 );	
	row[1]	= _mm_mul_ps( row[1], tmp2 );
	
	row[0]	= _mm_sub_ps( row[0], _mm_mul_ps( row[1], b0 ) );

	tmp1	= _mm_load_ss( &src[8] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	row[2]	= _mm_sub_ps( row[2], _mm_mul_ps( row[1], tmp1 ) );

	tmp1	= _mm_load_ss( &src[9] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	row[3]	= _mm_sub_ps( row[3], _mm_mul_ps( row[1], tmp1 ) );

	tmp1	= _mm_load_ss( &src[10] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	row[4]	= _mm_sub_ps( row[4], _mm_mul_ps( row[1], tmp1 ) );

	tmp1	= _mm_load_ss( &src[11] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	row[5]	= _mm_sub_ps( row[5], _mm_mul_ps( row[1], tmp1 ) );

	row[0]	= _mm_xor_ps( row[0], minus );
	row[1]	= _mm_xor_ps( row[1], minus );

	row[3]	= _mm_shuffle_ps( row[3], row[3], 0x4E );
	row[5]	= _mm_shuffle_ps( row[5], row[5], 0x4E );

	// Inverting the matrix 4x4 by the Kramers method.

	tmp2	= _mm_mul_ps( row[4], row[5] );
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1);

	minor0	= _mm_mul_ps( row[3], tmp1 );
	minor1	= _mm_mul_ps( row[2], tmp1 );

	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );

	minor0	= _mm_sub_ps( _mm_mul_ps( row[3], tmp1 ), minor0 );
	minor1	= _mm_sub_ps( _mm_mul_ps( row[2], tmp1 ), minor1 );
	minor1	= _mm_shuffle_ps( minor1, minor1, 0x4E );
	// -----------------------------------------------
	tmp2	= _mm_mul_ps( row[3], row[4]);
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );

	minor0	= _mm_add_ps( _mm_mul_ps( row[5], tmp1 ), minor0  );
	minor3	= _mm_mul_ps( row[2], tmp1 );

	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );

	minor0	= _mm_sub_ps( minor0, _mm_mul_ps( row[5], tmp1 ) );
	minor3	= _mm_sub_ps( _mm_mul_ps( row[2], tmp1 ), minor3 );
	minor3	= _mm_shuffle_ps( minor3, minor3, 0x4E );
	// -----------------------------------------------
	tmp2	= _mm_mul_ps( _mm_shuffle_ps( row[3], row[3], 0x4E ), row[5] );
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );
	row[4]	= _mm_shuffle_ps( row[4], row[4], 0x4E );

	minor0	= _mm_add_ps( _mm_mul_ps( row[4], tmp1 ), minor0 );
	minor2	= _mm_mul_ps( row[2], tmp1 );

	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );

	minor0	= _mm_sub_ps( minor0, _mm_mul_ps( row[4], tmp1 ) );
	minor2	= _mm_sub_ps( _mm_mul_ps( row[2], tmp1 ), minor2 );
	minor2	= _mm_shuffle_ps( minor2, minor2, 0x4E );
	// -----------------------------------------------
	tmp2	= _mm_mul_ps( row[2], row[3]);
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );	

	minor2	= _mm_add_ps( _mm_mul_ps( row[5], tmp1 ), minor2 );
	minor3	= _mm_sub_ps( _mm_mul_ps( row[4], tmp1 ), minor3 );
	
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );
	
	minor2	= _mm_sub_ps( _mm_mul_ps( row[5], tmp1 ), minor2 );
	minor3	= _mm_sub_ps( minor3, _mm_mul_ps( row[4], tmp1 ) );
	// -----------------------------------------------
	tmp2	= _mm_mul_ps( row[2], row[5]);
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );
	
	minor1	= _mm_sub_ps( minor1, _mm_mul_ps( row[4], tmp1 ) );
	minor2	= _mm_add_ps( _mm_mul_ps( row[3], tmp1 ), minor2 );	
	
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );
	
	minor1	= _mm_add_ps( _mm_mul_ps( row[4], tmp1 ), minor1 );
	minor2	= _mm_sub_ps( minor2, _mm_mul_ps( row[3], tmp1 ) );	
	// -----------------------------------------------
	tmp2	= _mm_mul_ps( row[2], row[4]);
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );
	
	minor1	= _mm_add_ps( _mm_mul_ps( row[5], tmp1 ), minor1 );
	minor3	= _mm_sub_ps( minor3, _mm_mul_ps( row[3], tmp1 ) );	
	
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );
	
	minor1	= _mm_sub_ps( minor1, _mm_mul_ps( row[5], tmp1 ) );
	minor3	= _mm_add_ps( _mm_mul_ps( row[3], tmp1 ), minor3 );	
	// -----------------------------------------------
	det		= _mm_mul_ps( row[2], minor0 );
	det		= _mm_add_ps( _mm_shuffle_ps( det, det, 0x4E ), det );
	det		= _mm_add_ss( _mm_shuffle_ps( det, det, 0xB1 ), det );

	if( _mm_movemask_ps( _mm_and_ps( _mm_cmplt_ss( det, epsilon ), _mm_cmpgt_ss( det, epsilon1 ) ) ) & 1 )
		return false;

	tmp1	= _mm_rcp_ss( det );
	det		= _mm_sub_ss( _mm_add_ss( tmp1, tmp1 ), _mm_mul_ss( det, _mm_mul_ss( tmp1, tmp1 ) ) );
	det		= _mm_shuffle_ps( det, det, 0x00 );

	row[2]	= _mm_mul_ps( det, minor0 );
	row[3]	= _mm_mul_ps( det, minor1 );
	row[4]	= _mm_mul_ps( det, minor2 );
	row[5]	= _mm_mul_ps( det, minor3 );

	b0		= _mm_unpacklo_ps( row[0], row[1] );
	b2		= _mm_unpackhi_ps( row[0], row[1] );
	b1		= _mm_shuffle_ps( b0, b2, 0x4E );
	b3		= _mm_shuffle_ps( b2, b0, 0x4E );

	tmp1	= _mm_shuffle_ps( row[2], row[3], 0x50 );
	tmp2	= _mm_mul_ps( b0, tmp1 );

	tmp1	= _mm_shuffle_ps( row[2], row[3], 0xA5 );
	tmp2	= _mm_add_ps( tmp2, _mm_mul_ps( b1, tmp1 ) );

	tmp1	= _mm_shuffle_ps( row[2], row[3], 0xFA );
	tmp2	= _mm_add_ps( tmp2, _mm_mul_ps( b2, tmp1 ) );

	tmp1	= _mm_shuffle_ps( row[2], row[3], 0x0F );
	row[0]	= _mm_add_ps( tmp2, _mm_mul_ps( b3, tmp1 ) );
	
	tmp1	= _mm_shuffle_ps( row[4], row[5], 0x50 );
	tmp2	= _mm_mul_ps( b0, tmp1 );
	
	tmp1	= _mm_shuffle_ps( row[4], row[5], 0xA5 );
	tmp2	= _mm_add_ps( tmp2, _mm_mul_ps( b1, tmp1 ) );
	
	tmp1	= _mm_shuffle_ps( row[4], row[5], 0xFA );
	tmp2	= _mm_add_ps( tmp2, _mm_mul_ps( b2, tmp1 ) );
	
	tmp1	= _mm_shuffle_ps( row[4], row[5], 0x0F );
	row[1]	= _mm_add_ps( tmp2, _mm_mul_ps( b3, tmp1 ) );
	
	// Calculating row number 1
	
	b0		= e;
	tmp1	= _mm_load_ss( &src[8] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	
	b0		= _mm_sub_ps( b0, _mm_mul_ps( _mm_shuffle_ps( row[0], row[0], 0x4E ), tmp1 ) );
	b1		= _mm_xor_ps( _mm_mul_ps( row[2], tmp1 ), minus );
	
	tmp1	= _mm_load_ss( &src[9] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	
	b0		= _mm_sub_ps( b0, _mm_mul_ps( row[0], tmp1 ) );
	b1		= _mm_sub_ps( b1, _mm_mul_ps( row[3], tmp1 ) );

	tmp1	= _mm_load_ss( &src[10] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );

	b0		= _mm_sub_ps( b0, _mm_mul_ps( _mm_shuffle_ps( row[1], row[1], 0x4E ), tmp1 ) );
	b1		= _mm_sub_ps( b1, _mm_mul_ps( row[4], tmp1 ) );

	tmp1	= _mm_load_ss( &src[11] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );

	b0		= _mm_sub_ps( b0, _mm_mul_ps( row[1], tmp1 ) );
	b1		= _mm_sub_ps( b1, _mm_mul_ps( row[5], tmp1 ) );

	tmp1	= _mm_load_ss( &src[6] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );

	b0		= _mm_sub_ps( b0, _mm_mul_ps( _mm_shuffle_ps( e, e, 0x4E ), tmp1 ) );

	tmp2	= _mm_load_ss( &src[7] );
	tmp2	= _mm_shuffle_ps( tmp2, tmp2, 0x00 );

	b0		= _mm_mul_ps( b0, tmp2 );
	b1		= _mm_mul_ps( b1, tmp2 );

	_mm_storeh_pi( (__m64*)&src[ 6], b0 );
	_mm_storel_pi( (__m64*)&src[ 8], b1 );
	_mm_storeh_pi( (__m64*)&src[10], b1 );

	// Calculating row number 0

	tmp1	= _mm_load_ss( &src[1] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );
	
	b0		= _mm_sub_ps( e, _mm_mul_ps( _mm_shuffle_ps( b0, b0, 0x4E ), tmp1 ) );
	b1		= _mm_xor_ps( _mm_mul_ps( b1, tmp1 ), minus );

	tmp1	= _mm_load_ss( &src[2] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );

	b0		= _mm_sub_ps( b0, _mm_mul_ps( row[0], tmp1 ) );
	b1		= _mm_sub_ps( b1, _mm_mul_ps( row[2], tmp1 ) );

	tmp1	= _mm_load_ss( &src[3] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );

	b0		= _mm_sub_ps( b0, _mm_mul_ps( _mm_shuffle_ps( row[0], row[0], 0x4E ), tmp1 ) );
	b1		= _mm_sub_ps( b1, _mm_mul_ps( row[3], tmp1 ) );

	tmp1	= _mm_load_ss( &src[4] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );

	b0		= _mm_sub_ps( b0, _mm_mul_ps( row[1], tmp1 ) );
	b1		= _mm_sub_ps( b1, _mm_mul_ps( row[4], tmp1 ) );
	
	tmp1	= _mm_load_ss( &src[5] );
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x00 );

	b0		= _mm_sub_ps( b0, _mm_mul_ps( _mm_shuffle_ps( row[1], row[1], 0x4E ), tmp1 ) );
	b1		= _mm_sub_ps( b1, _mm_mul_ps( row[5], tmp1 ) );

	tmp2	= _mm_load_ss( &src[0] );
	tmp2	= _mm_shuffle_ps( tmp2, tmp2, 0x00 );

	b0		= _mm_mul_ps( b0, tmp2 );
	b1		= _mm_mul_ps( b1, tmp2 );

	_mm_storel_pi( (__m64*)&src[0], b0 );
	_mm_storel_pi( (__m64*)&src[2], b1 );
	_mm_storeh_pi( (__m64*)&src[4], b1 );

	_mm_storel_pi( (__m64*)&src[12], row[0] );
	_mm_storel_pi( (__m64*)&src[14], row[2] );
	_mm_storeh_pi( (__m64*)&src[16], row[2] );

	_mm_storeh_pi( (__m64*)&src[18], row[0] );
	_mm_storel_pi( (__m64*)&src[20], row[3] );
	_mm_storeh_pi( (__m64*)&src[22], row[3] );

	_mm_storel_pi( (__m64*)&src[24], row[1] );
	_mm_storel_pi( (__m64*)&src[26], row[4] );
	_mm_storeh_pi( (__m64*)&src[28], row[4] );
	
	_mm_storeh_pi( (__m64*)&src[30], row[1] );
	_mm_storel_pi( (__m64*)&src[32], row[5] );
	_mm_storeh_pi( (__m64*)&src[34], row[5] );

	return true;

#undef	EPSILON
#undef	REAL_ZERO

} // Invert_6x6_fast

bool Invert6x6(SMLXMatrix& m) {

#define	Epsilon		1e-6
#define RealZero(x)	( fabs(x)<Epsilon ? 1:0 )

	float* mat = m.Data();

	if( !RealZero(mat[0]) && !RealZero(mat[0]*mat[7]-mat[1]*mat[6]) )
		return Invert_6x6_fast( mat );

	return Invert_6x6( mat );

#define	Epsilon	
#define RealZero
}

#else // _PIII_CODE_

bool Invert6x6(SMLXMatrix& m) {
	float* mat = m.Data();
	float d, di;
	di = mat[0];
	mat[0] = d = 1.0f / di;
	mat[6] *= -d;
	mat[12] *= -d;
	mat[18] *= -d;
	mat[24] *= -d;
	mat[30] *= -d;
	mat[1] *= d;
	mat[2] *= d;
	mat[3] *= d;
	mat[4] *= d;
	mat[5] *= d;
	mat[7] += mat[6] * mat[1] * di;
	mat[8] += mat[6] * mat[2] * di;
	mat[9] += mat[6] * mat[3] * di;
	mat[10] += mat[6] * mat[4] * di;
	mat[11] += mat[6] * mat[5] * di;
	mat[13] += mat[12] * mat[1] * di;
	mat[14] += mat[12] * mat[2] * di;
	mat[15] += mat[12] * mat[3] * di;
	mat[16] += mat[12] * mat[4] * di;
	mat[17] += mat[12] * mat[5] * di;
	mat[19] += mat[18] * mat[1] * di;
	mat[20] += mat[18] * mat[2] * di;
	mat[21] += mat[18] * mat[3] * di;
	mat[22] += mat[18] * mat[4] * di;
	mat[23] += mat[18] * mat[5] * di;
	mat[25] += mat[24] * mat[1] * di;
	mat[26] += mat[24] * mat[2] * di;
	mat[27] += mat[24] * mat[3] * di;
	mat[28] += mat[24] * mat[4] * di;
	mat[29] += mat[24] * mat[5] * di;
	mat[31] += mat[30] * mat[1] * di;
	mat[32] += mat[30] * mat[2] * di;
	mat[33] += mat[30] * mat[3] * di;
	mat[34] += mat[30] * mat[4] * di;
	mat[35] += mat[30] * mat[5] * di;
	di = mat[7];
	mat[7] = d = 1.0f / di;
	mat[1] *= -d;
	mat[13] *= -d;
	mat[19] *= -d;
	mat[25] *= -d;
	mat[31] *= -d;
	mat[6] *= d;
	mat[8] *= d;
	mat[9] *= d;
	mat[10] *= d;
	mat[11] *= d;
	mat[0] += mat[1] * mat[6] * di;
	mat[2] += mat[1] * mat[8] * di;
	mat[3] += mat[1] * mat[9] * di;
	mat[4] += mat[1] * mat[10] * di;
	mat[5] += mat[1] * mat[11] * di;
	mat[12] += mat[13] * mat[6] * di;
	mat[14] += mat[13] * mat[8] * di;
	mat[15] += mat[13] * mat[9] * di;
	mat[16] += mat[13] * mat[10] * di;
	mat[17] += mat[13] * mat[11] * di;
	mat[18] += mat[19] * mat[6] * di;
	mat[20] += mat[19] * mat[8] * di;
	mat[21] += mat[19] * mat[9] * di;
	mat[22] += mat[19] * mat[10] * di;
	mat[23] += mat[19] * mat[11] * di;
	mat[24] += mat[25] * mat[6] * di;
	mat[26] += mat[25] * mat[8] * di;
	mat[27] += mat[25] * mat[9] * di;
	mat[28] += mat[25] * mat[10] * di;
	mat[29] += mat[25] * mat[11] * di;
	mat[30] += mat[31] * mat[6] * di;
	mat[32] += mat[31] * mat[8] * di;
	mat[33] += mat[31] * mat[9] * di;
	mat[34] += mat[31] * mat[10] * di;
	mat[35] += mat[31] * mat[11] * di;
	di = mat[14];
	mat[14] = d = 1.0f / di;
	mat[2] *= -d;
	mat[8] *= -d;
	mat[20] *= -d;
	mat[26] *= -d;
	mat[32] *= -d;
	mat[12] *= d;
	mat[13] *= d;
	mat[15] *= d;
	mat[16] *= d;
	mat[17] *= d;
	mat[0] += mat[2] * mat[12] * di;
	mat[1] += mat[2] * mat[13] * di;
	mat[3] += mat[2] * mat[15] * di;
	mat[4] += mat[2] * mat[16] * di;
	mat[5] += mat[2] * mat[17] * di;
	mat[6] += mat[8] * mat[12] * di;
	mat[7] += mat[8] * mat[13] * di;
	mat[9] += mat[8] * mat[15] * di;
	mat[10] += mat[8] * mat[16] * di;
	mat[11] += mat[8] * mat[17] * di;
	mat[18] += mat[20] * mat[12] * di;
	mat[19] += mat[20] * mat[13] * di;
	mat[21] += mat[20] * mat[15] * di;
	mat[22] += mat[20] * mat[16] * di;
	mat[23] += mat[20] * mat[17] * di;
	mat[24] += mat[26] * mat[12] * di;
	mat[25] += mat[26] * mat[13] * di;
	mat[27] += mat[26] * mat[15] * di;
	mat[28] += mat[26] * mat[16] * di;
	mat[29] += mat[26] * mat[17] * di;
	mat[30] += mat[32] * mat[12] * di;
	mat[31] += mat[32] * mat[13] * di;
	mat[33] += mat[32] * mat[15] * di;
	mat[34] += mat[32] * mat[16] * di;
	mat[35] += mat[32] * mat[17] * di;
	di = mat[21];
	mat[21] = d = 1.0f / di;
	mat[3] *= -d;
	mat[9] *= -d;
	mat[15] *= -d;
	mat[27] *= -d;
	mat[33] *= -d;
	mat[18] *= d;
	mat[19] *= d;
	mat[20] *= d;
	mat[22] *= d;
	mat[23] *= d;
	mat[0] += mat[3] * mat[18] * di;
	mat[1] += mat[3] * mat[19] * di;
	mat[2] += mat[3] * mat[20] * di;
	mat[4] += mat[3] * mat[22] * di;
	mat[5] += mat[3] * mat[23] * di;
	mat[6] += mat[9] * mat[18] * di;
	mat[7] += mat[9] * mat[19] * di;
	mat[8] += mat[9] * mat[20] * di;
	mat[10] += mat[9] * mat[22] * di;
	mat[11] += mat[9] * mat[23] * di;
	mat[12] += mat[15] * mat[18] * di;
	mat[13] += mat[15] * mat[19] * di;
	mat[14] += mat[15] * mat[20] * di;
	mat[16] += mat[15] * mat[22] * di;
	mat[17] += mat[15] * mat[23] * di;
	mat[24] += mat[27] * mat[18] * di;
	mat[25] += mat[27] * mat[19] * di;
	mat[26] += mat[27] * mat[20] * di;
	mat[28] += mat[27] * mat[22] * di;
	mat[29] += mat[27] * mat[23] * di;
	mat[30] += mat[33] * mat[18] * di;
	mat[31] += mat[33] * mat[19] * di;
	mat[32] += mat[33] * mat[20] * di;
	mat[34] += mat[33] * mat[22] * di;
	mat[35] += mat[33] * mat[23] * di;
	di = mat[28];
	mat[28] = d = 1.0f / di;
	mat[4] *= -d;
	mat[10] *= -d;
	mat[16] *= -d;
	mat[22] *= -d;
	mat[34] *= -d;
	mat[24] *= d;
	mat[25] *= d;
	mat[26] *= d;
	mat[27] *= d;
	mat[29] *= d;
	mat[0] += mat[4] * mat[24] * di;
	mat[1] += mat[4] * mat[25] * di;
	mat[2] += mat[4] * mat[26] * di;
	mat[3] += mat[4] * mat[27] * di;
	mat[5] += mat[4] * mat[29] * di;
	mat[6] += mat[10] * mat[24] * di;
	mat[7] += mat[10] * mat[25] * di;
	mat[8] += mat[10] * mat[26] * di;
	mat[9] += mat[10] * mat[27] * di;
	mat[11] += mat[10] * mat[29] * di;
	mat[12] += mat[16] * mat[24] * di;
	mat[13] += mat[16] * mat[25] * di;
	mat[14] += mat[16] * mat[26] * di;
	mat[15] += mat[16] * mat[27] * di;
	mat[17] += mat[16] * mat[29] * di;
	mat[18] += mat[22] * mat[24] * di;
	mat[19] += mat[22] * mat[25] * di;
	mat[20] += mat[22] * mat[26] * di;
	mat[21] += mat[22] * mat[27] * di;
	mat[23] += mat[22] * mat[29] * di;
	mat[30] += mat[34] * mat[24] * di;
	mat[31] += mat[34] * mat[25] * di;
	mat[32] += mat[34] * mat[26] * di;
	mat[33] += mat[34] * mat[27] * di;
	mat[35] += mat[34] * mat[29] * di;
	di = mat[35];
	mat[35] = d = 1.0f / di;
	mat[5] *= -d;
	mat[11] *= -d;
	mat[17] *= -d;
	mat[23] *= -d;
	mat[29] *= -d;
	mat[30] *= d;
	mat[31] *= d;
	mat[32] *= d;
	mat[33] *= d;
	mat[34] *= d;
	mat[0] += mat[5] * mat[30] * di;
	mat[1] += mat[5] * mat[31] * di;
	mat[2] += mat[5] * mat[32] * di;
	mat[3] += mat[5] * mat[33] * di;
	mat[4] += mat[5] * mat[34] * di;
	mat[6] += mat[11] * mat[30] * di;
	mat[7] += mat[11] * mat[31] * di;
	mat[8] += mat[11] * mat[32] * di;
	mat[9] += mat[11] * mat[33] * di;
	mat[10] += mat[11] * mat[34] * di;
	mat[12] += mat[17] * mat[30] * di;
	mat[13] += mat[17] * mat[31] * di;
	mat[14] += mat[17] * mat[32] * di;
	mat[15] += mat[17] * mat[33] * di;
	mat[16] += mat[17] * mat[34] * di;
	mat[18] += mat[23] * mat[30] * di;
	mat[19] += mat[23] * mat[31] * di;
	mat[20] += mat[23] * mat[32] * di;
	mat[21] += mat[23] * mat[33] * di;
	mat[22] += mat[23] * mat[34] * di;
	mat[24] += mat[29] * mat[30] * di;
	mat[25] += mat[29] * mat[31] * di;
	mat[26] += mat[29] * mat[32] * di;
	mat[27] += mat[29] * mat[33] * di;
	mat[28] += mat[29] * mat[34] * di;
	return true;
}

#endif // _PIII_CODE_



#define ff2(addr1, addr2, reg1, reg2, dst, rez, op) \
	reg1 = _mm_loadh_pi(reg1, (__m64*)(addr1)); \
	reg2 = _mm_loadh_pi(reg2, (__m64*)(addr2)); \
	rez	= _mm_##op##_ps(reg1, reg2); \
	_mm_storeh_pi((__m64*)(dst), rez)

#ifdef _PIII_CODE_
void SMLXMatrix::Add(const SMLXMatrix& B) 
{
	assert (nr == B.nr && nc == B.nc);

	// matrixes must been 4-byte aligned
	//assert(((unsigned int) b & 0x03) == 0);
	//assert(((unsigned int) a & 0x03) == 0);

	register float *a	= mat;
	register float *b	= B.Data();

	int last = msize & 3;
	int end = msize - last;
	
	__m128	m, m2, sum;

	for (int i = 0; i < end; i += 4) {
		m	= _mm_load_ps(a + i);
		m2	= _mm_load_ps(b + i);
		sum	= _mm_add_ps(m, m2);
		_mm_store_ps(a + i, sum);
	}
	
	if(last)
	{
		float *tmp1, *tmp2;
		switch (last)
		{
			case 2:
				tmp1 = a + msize - 2;
				tmp2 = b + msize - 2;
				ff2(tmp1, tmp2, m, m2, tmp1, sum, add);
				break;
			case 1:
				a[end] += b[end];
				break;
			case 3:
				a[end] += b[end];
				tmp1 = a + msize - 2;
				tmp2 = b + msize - 2;
				ff2(tmp1, tmp2, m, m2, tmp1, sum, add);
		}
	}
}

#else

void SMLXMatrix::Add(const SMLXMatrix& B) 
{
	// Not optimized.
	assert (nr == B.nr && nc == B.nc);
	for (short i = 0; i < msize; i++)
		mat[i] += B.Data()[i];
}
#endif

#ifdef _PIII_CODE_

void SMLXMatrix::Sub(const SMLXMatrix& B) 
{
#ifdef COUNT_OPERATIONS
	++mCount[nr][nc][5];
#endif
	assert (nr == B.nr && nc == B.nc);
		
	register float *a	= mat;
	register float *b	= B.Data();

	__m128	m, m2, dif;

	int last = msize & 3;
	int end = msize - last;

	for (int i = 0; i < end; i += 4) {
		m	= _mm_load_ps(a + i);
		m2	= _mm_load_ps(b + i);
		dif	= _mm_sub_ps(m, m2);
		_mm_store_ps(a + i, dif);
	}

	if(last)
	{
		switch (last)
		{
			float *tmp1, *tmp2;
			case 2:
				tmp1 = a + msize - 2;
				tmp2 = b + msize - 2;
				ff2(tmp1, tmp2, m, m2, tmp1, dif, sub);
				break;
			case 1:
				a[end] -= b[end];
				break;
			case 3:
				a[end] -= b[end];
				tmp1 = a + msize - 2;
				tmp2 = b + msize - 2;
				ff2(tmp1, tmp2, m, m2, tmp1, dif, sub);
		}
	}
}

#else

void SMLXMatrix::Sub(const SMLXMatrix& B) 
{
#ifdef COUNT_OPERATIONS
	++mCount[nr][nc][5];
#endif
  // Not optimized.
	assert (nr == B.nr && nc == B.nc);
	for (short i = 0; i < msize; i++)
		mat[i] -= B.Data()[i];
}
#endif

#ifdef _PIII_CODE_
void SMLXMatrix::NSub(const SMLXMatrix& B) 
{
#ifdef COUNT_OPERATIONS
	++mCount[nr][nc][5];
#endif
	assert (nr == B.nr && nc == B.nc);

	register float *a	= mat;
	register float *b	= B.Data();

	__m128	m, m2, dif;

	int last = msize & 0x3;
	int end = msize - last;

	for (int i = 0; i < end; i += 4) {
		m	= _mm_load_ps(b + i);
		m2	= _mm_load_ps(a + i);
		dif	= _mm_sub_ps(m, m2);
		_mm_store_ps(a + i, dif);
	}

	if(last)
	{
		float *tmp1, *tmp2;
		switch (last)
		{
			case 2:
				tmp1 = a + msize - 2;
				tmp2 = b + msize - 2;
				ff2(tmp2, tmp1, m, m2, tmp1, dif, sub);
				break;
			case 1:
				a[end] = b[end] - a[end];
				break;
			case 3:
				a[end] = b[end] - a[end];
				tmp1 = a + msize - 2;
				tmp2 = b + msize - 2;
				ff2(tmp2, tmp1, m, m2, tmp1, dif, sub);
		}
	}
}

#else

void SMLXMatrix::NSub(const SMLXMatrix& B) 
{
#ifdef COUNT_OPERATIONS
	++mCount[nr][nc][5];
#endif
	// Not optimized.
	assert (nr == B.nr && nc == B.nc);
	for (short i = 0; i < msize; i++)
		mat[i] = B.Data()[i] - mat[i];
}
#endif

#ifdef _PIII_CODE_
void SMLXMatrix::Scale(float scale) 
{
#ifdef COUNT_OPERATIONS
	++mCount[nr][nc][7];
#endif

	register float *a	= mat;

	__m128	m;
	__m128	sc		= _mm_set_ps1(scale);
	int		last	= msize & 3;
	int end = msize - last;

	for (int i = 0; i < end; i += 4) {
		m	= _mm_mul_ps(sc, _mm_load_ps(a + i));
		_mm_store_ps(a + i, m);
	}

	if(last)
	{
		float *tmp1;
		switch (last)
		{
			case 2:
				tmp1	= a + msize - 2;
				m		= _mm_mul_ps(sc, _mm_loadh_pi(m, (__m64*)tmp1));
				_mm_storeh_pi((__m64*)tmp1, m);
				break;
			case 1:
				a[end] *= scale;
				break;
			case 3:
				a[end] *= scale;
				tmp1	= a + msize - 2;
				m		= _mm_mul_ps(sc, _mm_loadh_pi(m, (__m64*)tmp1));
				_mm_storeh_pi((__m64*)tmp1, m);
		}
	}
}

#else

void SMLXMatrix::Scale(float scale) 
{
#ifdef COUNT_OPERATIONS
	++mCount[nr][nc][7];
#endif
	// Not optimized.
	for (short i = 0; i < msize; i++)
		mat[i] *= scale;
}
#endif

#ifdef _PIII_CODE_
SMLXTmpMatrix& SMLXMatrix::operator*(float scale) const {
#ifdef COUNT_OPERATIONS
	++mCount[nr][nc][7];
#endif
	SMLXTmpMatrix& tm = TMat(nr, nc);
	register float *a	= mat;
	register float *b	= tm.Data();

	__m128	m;
	__m128	sc		= _mm_set_ps1(scale);
	int		last	= msize & 3;
	int end = msize - last;

	for (int i = 0; i < end; i += 4) {
		m	= _mm_mul_ps(sc, _mm_load_ps(a + i));
		_mm_store_ps(b + i, m);
	}

	if(last)
	{
		switch (last)
		{
			case 2:
				m		= _mm_mul_ps(sc, _mm_loadh_pi(m, (__m64*)(a + msize - 2)));
				_mm_storeh_pi((__m64*)(b + msize - 2), m);
				break;
			case 1:
				a[end] *= scale;
				break;
			case 3:
				a[end] *= scale;
				m		= _mm_mul_ps(sc, _mm_loadh_pi(m, (__m64*)(a + msize - 2)));
				_mm_storeh_pi((__m64*)(b + msize - 2), m);
		}
	}
	return tm;
}

#else


SMLXTmpMatrix& SMLXMatrix::operator*(float scale) const {
#ifdef COUNT_OPERATIONS
	++mCount[nr][nc][7];
#endif
	SMLXTmpMatrix& tm = TMat(nr, nc);
	for (short i = 0; i < msize; i++)
		tm.mat[i] = mat[i] * scale;
	return tm;
}
#endif





//including optimized functions definitions
#ifdef _PIII_CODE_
//	#include <smlxSSEops.i>
	#include <smlxSSEops2.i>
#else
	#include <smlxGENops.i>
#endif

#ifdef COUNT_OPERATIONS
SMLLIBENTRY int SMLXMatrix::mCount[7][7][8] = {0};
#endif
#ifndef NDEBUG
SMLLIBENTRY int subExpressionsCount = 0;
#endif

int SMLMemoryPool::index = 0;
float SMLMemoryPool::pool[MemorySize];
unsigned int SMLMemoryPool::poolStart = 0xffffffff;


// conversion from 'double' to 'float', possible loss of data
#pragma warning(disable: 4244)

// Searching for hash function for s:
/*
 s = {{1,1,1},{1,1,3},{1,1,6},
	  {1,3,1},{1,3,3},{1,3,6},
	  {1,6,1},{1,6,3},{1,6,6},
	  {3,1,1},{3,1,3},{3,1,6},
	  {3,3,1},{3,3,3},{3,3,6},
	  {3,6,1},{3,6,3},{3,6,6},
	  {4,1,4},{4,4,1},{4,4,4},
	  {1,4,4},{6,1,1},{6,1,6},
	  {6,3,1},{6,3,3},{6,3,6},
	  {6,6,1},{6,6,3},{6,6,6}};
 hi[n1_, n2_, n3_] := Map[n1 #[[1]] + n2 #[[2]] + n3 #[[3]]&, s];
 goodhi[n1_, n2_, n3_] := Module[{fs = hi[n1, n2, n3]},Length[Union[fs]] === Length[fs]]
 m = 8; ht = Sort[DeleteCases[Flatten[Table[
		   If[goodhi[i,j,k], {{i, j, k}, Max[hi[i, j, k]] - Min[hi[i, j, k]]}], 
			   {i, -m, m},{j, -m, m},{k, -m, m}] , 2], Null], Last[#1] < Last[#2]&]

 hash3 = ht[[1, 1]];
 sorthash3  = Sort[Thread[{hi@@hash3, s}]];
 arrayhash3 = Map[Function[i, Select[sorthash3, #[[1]] == i&]], Table[i, {i, 0, First[Last[sorthash3]]}]];
 Map[Print[If[# === {}, "0,", "fmm("<>ToString[#[[1,2,1]]]<>","<>ToString[#[[1,2,2]]]<>","<>ToString[#[[1,2,3]]]
	 <>"), // "<>ToString[#[[1,1]]]]]&, arrayhash3];
*/


#ifndef NDEBUG
void VerifyXMatrix(const SMLXMatrix& A)
{
	// Check for NANs.
	for (int i = 0; i < A.nrows(); i++)
		for (int j = 0; j < A.ncols(); j++)
			assert(A[i][j] > -1000000000000);
}
#endif

SMLXTmpMatrix& __fastcall Mult00(const SMLXMatrix& A, const SMLXMatrix& B) {
		SMLXTmpMatrix& tm = SMLXMatrix::TMat(A.nrows(), B.ncols());
		tm.Mult(A, B, 0, 0);
		return tm;
}
SMLXTmpMatrix& __fastcall Mult01(const SMLXMatrix& A, const SMLXMatrix& B) {
		SMLXTmpMatrix& tm = SMLXMatrix::TMat(A.nrows(), B.nrows());
		tm.Mult(A, B, 0, 1);
		return tm;
}
SMLXTmpMatrix& __fastcall Mult10(const SMLXMatrix& A, const SMLXMatrix& B) {
	SMLXTmpMatrix& tm = SMLXMatrix::TMat(A.ncols(), B.ncols());
	tm.Mult(A, B, 1, 0);
	return tm;
}
SMLXTmpMatrix& __fastcall Mult11(const SMLXMatrix& A, const SMLXMatrix& B) {
	SMLXTmpMatrix& tm = SMLXMatrix::TMat(A.ncols(), B.nrows());
	tm.Mult(A, B, 1, 1);
	return tm;
}

// Map[Print["fmm(",#[[1]],",",#[[2]],",",#[[3]], "),"]&, Flatten[Table[{i,j,k},{i,1,6},{j,1,6},{k,1,6}], 2]];

#define gmm Mult00
#define fmm(AR, AC, RN) (SMLXTmpMatrix& (__fastcall *) \
	(const SMLXMatrix&, const SMLXMatrix&)) Mult00_ ## AR ## x ## AC ## _ ## AC ## x ## RN /*operator**/
void* ftable_mm00[] = {
#include <smlxmmm.inl>
};

#define gmm Mult01
#define fmm(AR, AC, RN) (SMLXTmpMatrix& (__fastcall *) \
	(const SMLXMatrix&, const SMLXTransposedMatrix&)) Mult01_ ## AR ## x ## AC ## _ ## RN ## x ## AC /*operator**/
void* ftable_mm01[] = {
#include <smlxmmm.inl>
};

#define gmm Mult10
#define fmm(AR, AC, RN) (SMLXTmpMatrix& (__fastcall *) \
	(const SMLXTransposedMatrix&, const SMLXMatrix&)) Mult10_ ## AR ## x ## AC ## _ ## AR ## x ## RN /*operator**/
void* ftable_mm10[] = {
#include <smlxmmm.inl>
};

#define gmm Mult11
#define fmm(AR, AC, RN) (SMLXTmpMatrix& (__fastcall *) \
	(const SMLXTransposedMatrix&, const SMLXTransposedMatrix&)) Mult11_ ## AR ## x ## AC ## _ ## RN ## x ## AR/*operator**/
void* ftable_mm11[] = {
#include <smlxmmm.inl>
};

// sizeof(lutSizes) = maximum matrix dimension,
// -1 values will trigger generic (unoptimized) calls.
SMLXTmpMatrix SMLXMatrix::pool[poolSize];
#ifdef _PIII_CODE_
/*const*/ int SMLXMatrix::lutSizes[] = {
-1,  0,  1,  2,  3,  4,  5, -1, -1, -1, 
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
#else
/*const*/ int SMLXMatrix::lutSizes[] = {
-1,  0, -1,  2,  3,  4,  5, -1, -1, -1, 
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
#endif

void __fastcall GenericAssign(SMLXMatrix& A, const SMLXMatrix& B) {
	for (int i = 0; i < A.msize; i++)
		A.mat[i] = B.mat[i];
}

void __fastcall GenericAssignValue(SMLXMatrix& A, float v) {
	for (int i = 0; i < A.msize; i++)
		A.mat[i] = v;
}

void __fastcall GenericAddTo(SMLXMatrix& A, const SMLXMatrix& B) {
#ifdef _PIII_CODE_
	__m128 dummy;
#endif
	A.Add(B);
}

void __fastcall GenericSubFrom(SMLXMatrix& A, const SMLXMatrix& B) {
#ifdef _PIII_CODE_
	__m128 dummy;
#endif
	A.Sub(B);
}

SMLXTmpMatrix& __fastcall GenericAdd(SMLXMatrix& A, const SMLXMatrix& B) {
#ifdef _PIII_CODE_
	__m128 dummy;
#endif
	//SMLXTmpMatrix& tm = SMLXMatrix::TMat(A.nrows(), A.ncols());
	SMLXTmpMatrix& tm = GenericTMat(A.nrows(), A.ncols());
	GenericAssign(tm, A);
	GenericAddTo(tm, B);
	return tm;
}

SMLXTmpMatrix& __fastcall GenericSub(SMLXMatrix& A, const SMLXMatrix& B) {
#ifdef _PIII_CODE_
	__m128 dummy;
#endif
	//SMLXTmpMatrix& tm = SMLXMatrix::TMat(A.nrows(), A.ncols());
	SMLXTmpMatrix& tm = GenericTMat(A.nrows(), A.ncols());
	GenericAssign(tm, A);
	GenericSubFrom(tm, B);
	return tm;
}

bool SMLXMatrix::GenericInvert() {
	assert (nr == nc);
	const float epsilon = 1.0e-15f;
	for (int k = 0; k < nr; k++) {

		if (FastAbs((*this)[k][k]) < epsilon) {
			return false;

		} else {

			float di = (*this)[k][k];
			float d = 1.0f / di;
			(*this)[k][k] = d;

			for (int i = 0; i < k; i++) 
				(*this)[i][k] = -(*this)[i][k] * d; 

			for (i++; i < nr; i++) 
				(*this)[i][k] = -(*this)[i][k] * d; 

			for (int j = 0; j < k; j++)
				(*this)[k][j] = (*this)[k][j] * d; 

			for (j++; j < nr; j++)
				(*this)[k][j] = (*this)[k][j] * d; 

			for (i = 0; i < nr; i++) {
				if (i != k) {
					for (int j = 0; j < k; j++)
						(*this)[i][j] = (*this)[i][j] + (*this)[i][k] * (*this)[k][j] * di;
					for (j++; j < nr; j++)
						(*this)[i][j] = (*this)[i][j] + (*this)[i][k] * (*this)[k][j] * di;
				} 
			}
		}
	}
	return true;
}

void* SMLXMatrix::Inverts[7];
void SMLXMatrix::SetInvertPointers() {
	void* pInverts = &Inverts[0];
	bool (SMLXMatrix::*pmiss)  (void) = SMLXMatrix::GenericInvert;
	bool (*p1)(SMLXMatrix&) = Invert1x1;
	bool (*p3)(SMLXMatrix&) = Invert3x3;
	bool (*p4)(SMLXMatrix&) = Invert4x4;
	bool (*p5)(SMLXMatrix&) = Invert5x5;
	bool (*p6)(SMLXMatrix&) = Invert6x6;
	__asm mov   edx, pInverts;
	__asm mov   eax, pmiss
	__asm mov   dword ptr [edx], eax
	__asm mov   eax, p1
	__asm mov   dword ptr [edx+4], eax
	__asm mov   eax, pmiss
	__asm mov   dword ptr [edx+8], eax
	__asm mov   eax, p3
	__asm mov   dword ptr [edx+12], eax
	__asm mov   eax, p4
	__asm mov   dword ptr [edx+16], eax
	__asm mov   eax, p5
	__asm mov   dword ptr [edx+20], eax
	__asm mov   eax, p6
	__asm mov   dword ptr [edx+24], eax
}

struct InvertInstantiation {
	InvertInstantiation() {
		SMLXMatrix::SetInvertPointers();
	}
} _This_Class_Is_Here_Only_To_Call_SetInvertPointers;

bool SMLXMatrix::Invert() {
	assert (nr == nc);
	void* pf = Inverts[SMLXMatrix::lutSizes[nr] + 1];
	bool result;
	__asm mov   ecx, [this]
	__asm push	ecx
	__asm call  [pf]
	__asm pop	ecx
	__asm mov   [result], al
	return result;
}

void SMLXMatrix::Output(const char* caption) const {
//#nifdef NDEBUG
	static char str[200];
	if (caption)
		sprintf(str, "%s = {\n{", caption);
	else
		sprintf(str, "	{\n{");
	OutputDebugString(str);
	for (int i = 0; i < nr; i++) {
		for (int j = 0; j < nc; j++) {
			assert(strlen(str) < sizeof(str) - 15);
			sprintf(str, "%13.9f%s", (*this)[i][j], j == nc-1? (i == nr-1? "}}\n" : "},\n{") : ", ");
			OutputDebugString(str);
		}
	}
//#endif
}

void SMLXMatrix::Mult(const SMLXMatrix& A, const SMLXMatrix& B, short Atranspose, short Btranspose) {
	// Not optimized.
	// this = A * B, with optional transposition
	assert(this != &A && this != &B);
	float sum;
	float* d;
	short i, j, k;
	switch (Atranspose + Btranspose + Btranspose) {
	case 0: // 00
		assert(A.nc <= B.nr);
		Resize(A.nr, B.nc);
		d = Data();
		for (i = 0; i < A.nr; i++)
			for (j = 0; j < B.nc; j++) {
				for (sum = 0, k = 0; k < A.nc; k++)
					sum += A[i][k] * B[k][j];
				*d++ = sum;
			}
		break;
	case 1: // 10
		assert(A.nr <= B.nr);
		Resize(A.nc, B.nc);
		d = Data();
		for (i = 0; i < A.nc; i++)
			for (j = 0; j < B.nc; j++) {
				for (sum = 0, k = 0; k < A.nr; k++)
					sum += A[k][i] * B[k][j];
				*d++ = sum;
			}
		break;
	case 2: // 01
		assert(A.nc <= B.nc);
		Resize(A.nr, B.nr);
		d = Data();
		for (i = 0; i < A.nr; i++)
			for (j = 0; j < B.nr; j++) {
				for (sum = 0, k = 0; k < A.nc; k++)
					sum += A[i][k] * B[j][k];
				*d++ = sum;
			}
		break;
	default: // 11
		assert(A.nr <= B.nc);
		Resize(A.nc, B.nr);
		d = Data();
		for (i = 0; i < A.nc; i++)
			for (j = 0; j < B.nr; j++) {
				for (sum = 0, k = 0; k < A.nr; k++)
					sum += A[k][i] * B[j][k];
				*d++ = sum;
			}
	}
	MarkAsDefined();
}

#ifdef _PIII_CODE_

bool  SMLXMatrix::LUDecomposition(SMLXMatrix &idxm, double &det)
{
	// Factors "this" matrix into A=LU where L is lower triangular and U is upper
	// triangular.  The matrix is overwritten by LU with the diagonal elements
	// of L (which are unity) not stored.  This must be a square n x n matrix.
	// idxm is an output 3 x n vector which is used by LUBackSubstitution.
	// d is returned +-1 indicating that the
	// number of row interchanges was even or odd respectively.
	//
	// Note:
	// Calling of this function is to be followed by (some) LUBackSubstitutions.

	assert(nrows() == ncols());
	assert(sizeof(int) <= sizeof(float));

	int		n = nrows();
	float	*pdata = mat;

	int i, j, k;

	int		size, last4, end4, pe4;
	int		last8, pe8;
	float	frcp, tmp, pivel;
	register float *tmpptr;
	float	*ptr2, *ptr;

	idxm.Resize(3, ncols());
	int* ri		= (int*)idxm[0];
	int* irow	= (int*)idxm[1];

	det = 1.0;

	// Initialize the pointer vector.
	for (i = 0; i < n-1; i += 2) {
		ri[i]	  = i;
		irow[i]   = i * n;
		ri[i+1]	  = i + 1;
		irow[i+1] = irow[i] + n;
	}
	if (n & 1) {
		ri[n-1]	  = n - 1;
		irow[n-1] = (n - 1) * n;
	}

	// LU factorization.
	for (int p = 1; p <= n - 1; p++) {
		// Find pivot element.
		for (i = p + 1; i <= n; i++) {
			if (FastAbs((pdata + irow[i-1])[p-1]) > FastAbs((pdata + irow[p-1])[p-1])) {
				// Switch the index for the p-1 pivot row if necessary.
				int t	= ri[p-1];
				ri[p-1] = ri[i-1];
				ri[i-1] = t;
				int s	  = irow[p-1];
				irow[p-1] = irow[i-1];
				irow[i-1] = s;
				det = -det;
			}
		}

		pivel = *(pdata + irow[p-1] + p - 1);

		if (pivel == 0) {
			// The matrix is singular.
			return false;
		}

		// Multiply the diagonal elements.
		det *= pivel;

		// Form multiplier.
		__asm {
			movss	xmm0, DWORD PTR pivel
			rcpss	xmm1, xmm0
			mulss	xmm0, xmm1
			mulss	xmm0, xmm1
			addss	xmm1, xmm1
			subps	xmm1, xmm0
			movss	DWORD PTR frcp, xmm1
		} // calculates 1/pivel using reciprocal division

		//1. A[p+1:n][n] = A[p+1:n][n] / A[p][p]
		size	= n - p;
		last8	= size & 7;
		pe8		= n - last8;
		last4	= last8 & 3;
		end4	= size - last4;
		pe4		= n - last4;

		for (i = p + 1; i <= pe4; i += 4) {
			*(pdata + irow[i-1]+p-1) *= frcp;
			*(pdata + irow[i]+p-1)	 *= frcp;
			*(pdata + irow[i+1]+p-1) *= frcp;
			*(pdata + irow[i+2]+p-1) *= frcp;
		}
		if(last4)
		{
			for (i = p + 1 + end4; i <= n; i++) {
				*(pdata + irow[i-1]+p-1) *= frcp;
			}
		}// end 1 and form multiplier

		
		//2. A[p+1:n][p+1:n] = A[p+1:n][p+1:n] - A[p+1:n][p] * A[p][p+1:n] 
		ptr2 = pdata + irow[p-1] - 1;
		for (j = p + 1; j < pe8; j += 8) { //loop for 8 columns
			tmpptr = ptr2 + j;
			__asm {
				mov		eax,  DWORD PTR [tmpptr]
				movups	xmm0, XMMWORD PTR[eax]
				movlps	xmm7, QWORD PTR[eax+16]
				movhps	xmm7, QWORD PTR[eax+24]
			}
			for (i = p + 1; i < pe4; i += 4) { //loop for 4 rows 
				//--------------------------------------- 1 - 8
				ptr		= pdata + irow[i-1];
				tmpptr	= ptr+p-1;
				__asm {
					mov		eax,  DWORD PTR [tmpptr]
				    movss	xmm1, DWORD PTR[eax]
				    shufps	xmm1, xmm1, 0
					movlhps	xmm3, xmm1
					movhlps	xmm3, xmm1
				    mulps	xmm1, xmm0
				}
				tmpptr	= ptr+j-1;
				__asm {
					mov		ecx,  DWORD PTR [tmpptr]
					movlps	xmm2, QWORD PTR[ecx]
				    mulps	xmm3, xmm7
					movhps	xmm2, QWORD PTR[ecx+8]
				    subps	xmm2, xmm1
					movlps	xmm4, QWORD PTR[ecx+16]
					movhps	xmm4, QWORD PTR[ecx+24]
				    subps	xmm4, xmm3
					movups	XMMWORD PTR[ecx], xmm2
				    movups	XMMWORD PTR[ecx+16], xmm4
				}
				ptr		= pdata + irow[i];
				tmpptr	= ptr+p-1;
				//--------------------------------------- 2 - 8
				__asm {
					mov		eax,  DWORD PTR [tmpptr]
				    movss	xmm1, DWORD PTR[eax]
				    shufps	xmm1, xmm1, 0
					movlhps	xmm3, xmm1
					movhlps	xmm3, xmm1					
					mulps	xmm1, xmm0
				}
				tmpptr	= ptr+j-1;
				__asm {
					mov		ecx,  DWORD PTR [tmpptr]
					movlps	xmm2, QWORD PTR[ecx]
				    mulps	xmm3, xmm7
					movhps	xmm2, QWORD PTR[ecx+8]
				    subps	xmm2, xmm1
					movlps	xmm4, QWORD PTR[ecx+16]
					movhps	xmm4, QWORD PTR[ecx+24]
				    subps	xmm4, xmm3
				    movups	XMMWORD PTR[ecx], xmm2
				    movups	XMMWORD PTR[ecx+16], xmm4
				}
				ptr		= pdata + irow[i+1];
				tmpptr	= ptr+p-1;
				//--------------------------------------- 3 - 8
				__asm {
					mov		eax,  DWORD PTR [tmpptr]
				    movss	xmm1, DWORD PTR[eax]
				    shufps	xmm1, xmm1, 0
					movlhps	xmm3, xmm1
					movhlps	xmm3, xmm1					
				    mulps	xmm1, xmm0
				}
				tmpptr	= ptr+j-1;
				__asm {
					mov		ecx,  DWORD PTR [tmpptr]
					movlps	xmm2, QWORD PTR[ecx]
				    mulps	xmm3, xmm7
					movhps	xmm2, QWORD PTR[ecx+8]
				    subps	xmm2, xmm1
					movlps	xmm4, QWORD PTR[ecx+16]
					movhps	xmm4, QWORD PTR[ecx+24]
				    subps	xmm4, xmm3
				    movups	XMMWORD PTR[ecx], xmm2
				    movups	XMMWORD PTR[ecx+16], xmm4
				}
				ptr		= pdata + irow[i+2];
				tmpptr	= ptr+p-1;
				//--------------------------------------- 4 - 8
				__asm {
					mov		eax,  DWORD PTR [tmpptr]
				    movss	xmm1, DWORD PTR[eax]
				    shufps	xmm1, xmm1, 0
					movlhps	xmm3, xmm1
					movhlps	xmm3, xmm1					
				    mulps	xmm1, xmm0
				}
				tmpptr	= ptr+j-1;
				__asm {
					mov		ecx,  DWORD PTR [tmpptr]
					movlps	xmm2, QWORD PTR[ecx]
				    mulps	xmm3, xmm7
					movhps	xmm2, QWORD PTR[ecx+8]
				    subps	xmm2, xmm1
				    movups	XMMWORD PTR[ecx], xmm2
					movlps	xmm4, QWORD PTR[ecx+16]
					movhps	xmm4, QWORD PTR[ecx+24]
				    subps	xmm4, xmm3
				    movups	XMMWORD PTR[ecx+16], xmm4
				}
			}
			if(last4)
			{ 
				for (i = p + 1 + end4; i <= n; i++) {// calculates last rows
					ptr		= pdata + irow[i-1];
					tmpptr	= ptr+p-1;
					__asm {
						mov		eax,  DWORD PTR [tmpptr]
					    movss	xmm1, DWORD PTR[eax]
					    shufps	xmm1, xmm1, 0
					    movaps	xmm3, xmm1
					    mulps	xmm1, xmm0
					}
					tmpptr	= ptr+j-1;
					__asm {
						mov		eax,  DWORD PTR [tmpptr]
					    movups	xmm2, XMMWORD PTR[eax]
					    mulps	xmm3, xmm7
						subps	xmm2, xmm1
					    movups	xmm4, XMMWORD PTR[eax+16]
					    subps	xmm4, xmm3
					    movups	XMMWORD PTR[eax], xmm2
					    movups	XMMWORD PTR[eax+16], xmm4
					}
				}
			}// end loop for rows 
		}
		if(last8 > 3)
		{
			tmpptr = ptr2 + p + 1 + size - last8;
			__asm {
				mov		eax,  DWORD PTR [tmpptr]
				movups	xmm0, XMMWORD PTR[eax]
			}
			for (i = p + 1; i < pe4; i += 4) { //loop for rows 
				ptr		= pdata + irow[i-1];
				tmpptr	= ptr+p-1;
				__asm {
					mov		eax, DWORD PTR [tmpptr]
				    movss	xmm1, DWORD PTR[eax]
				    shufps	xmm1, xmm1, 0
				    mulps	xmm1, xmm0
				}
				tmpptr	= ptr+pe8;
				__asm {
					mov		eax, DWORD PTR [tmpptr]
				    movups	xmm2, XMMWORD PTR[eax]
				    subps	xmm2, xmm1
				    movups	XMMWORD PTR[eax], xmm2
				}
				ptr		= pdata + irow[i];
				tmpptr	= ptr+p-1;
				__asm {
					mov		eax, DWORD PTR [tmpptr]
				    movss	xmm3, DWORD PTR[eax]
				    shufps	xmm3, xmm3, 0
				    mulps	xmm3, xmm0
				}
				tmpptr	= ptr+pe8;
				__asm {
					mov		eax, DWORD PTR [tmpptr]
				    movups	xmm4, XMMWORD PTR[eax]
				    subps	xmm4, xmm3
				    movups	XMMWORD PTR[eax], xmm4
				}
				ptr		= pdata + irow[i+1];
				tmpptr	= ptr+p-1;
				__asm {
					mov		eax, DWORD PTR [tmpptr]
				    movss	xmm3, DWORD PTR[eax]
				    shufps	xmm3, xmm3, 0
				    mulps	xmm3, xmm0
				}
				tmpptr	= ptr+pe8;
				__asm {
					mov		eax, DWORD PTR [tmpptr]
				    movups	xmm4, XMMWORD PTR[eax]
				    subps	xmm4, xmm3
				    movups	XMMWORD PTR[eax], xmm4
				}
				ptr		= pdata + irow[i+2];
				tmpptr	= ptr+p-1;
				__asm {
					mov		eax, DWORD PTR [tmpptr]
				    movss	xmm3, DWORD PTR[eax]
				    shufps	xmm3, xmm3, 0
				    mulps	xmm3, xmm0
				}
				tmpptr	= ptr+pe8;
				__asm {
					mov		eax, DWORD PTR [tmpptr]
				    movups	xmm4, XMMWORD PTR[eax]
				    subps	xmm4, xmm3
				    movups	XMMWORD PTR[eax], xmm4
				}
			}
			if(last4)
			{ 
				for (i = p + 1 + end4; i <= n; i++) {// calculates last rows
					ptr		= pdata + irow[i-1];
					tmpptr	= ptr+p-1;
					__asm {
						mov		eax, DWORD PTR [tmpptr]
					    movss	xmm1, DWORD PTR[eax]
						shufps	xmm1, xmm1, 0
					    mulps	xmm1, xmm0
					}
					tmpptr	= ptr+pe8;
					__asm {
						mov		eax, DWORD PTR [tmpptr]
						movups	xmm2, XMMWORD PTR[eax]
					    subps	xmm2, xmm1
					    movups	XMMWORD PTR[eax], xmm2
					}
				}
			}// end loop for rows 
		}// end loop for columns
		switch(last4) {// calculates last columns
			case 0: break;
			case 3: 
				ptr2 = pdata + irow[p-1] + p + end4;
				for (i = p + 1; i <= n; i++) {
					ptr		= pdata + irow[i-1] + p;
					tmp		= *(ptr - 1);
					ptr		+= end4;
					*ptr	-= tmp * (*ptr2);
					*(ptr+1) -= tmp * (*(ptr2+1));
					*(ptr+2) -= tmp * (*(ptr2+2));
				}
				break;
			case 2: 
				ptr2 = pdata + irow[p-1] + p + end4;
				for (i = p + 1; i <= n; i++) {
					ptr		= pdata + irow[i-1] + p;
					tmp		= *(ptr - 1);
					ptr		+= end4;
					*ptr	-= tmp * (*ptr2);
					*(ptr+1) -= tmp * (*(ptr2+1));
				}
				break;
			case 1: 
				ptr2 = pdata + irow[p-1] + p + end4;
				for (i = p + 1; i <= n; i++) {
					ptr		= pdata + irow[i-1] + p;
					tmp		= *(ptr - 1);
					ptr		+= end4;
					*ptr	-= tmp * (*ptr2);
				}
				break;
		}// end 2 and calculates last columns
	}
	det *= *(pdata + irow[n-1]+n-1);
	return det != 0.0;
}

void  SMLXMatrix::LUBackSubstitution(SMLXMatrix &idxm, SMLXMatrix &bx) const
{
	// Uses the LU decomposition stored in "this" to solve the n linear equations
	// Ax=b.  
	// The idxm 3xn array from LUDecomposition is used.  
	// The bx 1 x n matrix is the input vector b and is 
	// overwritten by the solution vector x.

	assert(nrows() == ncols());
	assert(idxm.ncols() == ncols() && idxm.nrows() == 3);
	assert(bx.nrows()   == nrows() && bx.ncols()   == 1);

	const int n		= nrows();
	float *pdata	= mat; //!!!
	float *bdata	= bx.Data(); //!!!

	const int* bri	= (int*)idxm[0];
	const int* ri	= (int*)idxm[1];

	
	float* y		= idxm[2];

	int i, j;

	float sum, div;

	// Forward substitution.
	y[0] = (bdata + bri[0])[0];
	for (i = 2; i <= n; i++) {

		for (sum = 0, j = 1; j <= i - 1; j++) {
			sum += (pdata + ri[i-1])[j-1] * y[j-1];
		}
		y[i-1] = (bdata + bri[i-1])[0] - sum;
	}


	// Back substitution.
	*(bdata + n-1) = y[n-1] / (pdata + ri[n-1])[n-1];
	for (i = n - 1; i >= 1; i--) {

		sum = 0;
		for (j = i + 1; j <= n; j++) {
			sum += (pdata + ri[i-1])[j-1] * (bdata + j-1)[0];
		}
		*(bdata + i-1) = (y[i-1] - sum) / (pdata + ri[i-1])[i-1];
	}
}

#else

bool  SMLXMatrix::LUDecomposition(SMLXMatrix &idxm, double &det) {
	// Factors "this" matrix into A=LU where L is lower triangular and U is upper
	// triangular.  The matrix is overwritten by LU with the diagonal elements
	// of L (which are unity) not stored.  This must be a square n x n matrix.
	// idxm is an output 2 x n vector which is used by LUBackSubstitution.
	// d is returned +-1 indicating that the
	// number of row interchanges was even or odd respectively.
	//
	// Note:
	// Calling of this function is to be followed by (some) LUBackSubstitutions.

	assert(nrows() == ncols());
	assert(sizeof(short) <= sizeof(float));
	const short n = nrows();

	int i;
	idxm.Resize(2, ncols());
	short* ri = (short*)idxm[0];

	det = 1.0;

	// Initialize the pointer vector.

	for (i = 0; i < n; i++)
		ri[i] = i;

	// LU factorization.

	for (int p = 1; p <= n - 1; p++) {

		// Find pivot element.
		for (i = p + 1; i <= n; i++) {
			if (FastAbs((*this)[ri[i-1]][p-1]) > FastAbs((*this)[ri[p-1]][p-1])) {
				// Switch the index for the p-1 pivot row if necessary.
				int t = ri[p-1];
				ri[p-1] = ri[i-1];
				ri[i-1] = t;
				det = -det;
			}
		}

		if ((*this)[ri[p-1]][p-1] == 0) {
			// The matrix is singular.
			return false;
		}

		// Multiply the diagonal elements.
		det = det * (*this)[ri[p-1]][p-1];

		// Form multiplier.
		for (i = p + 1; i <= n; i++) {
			(*this)[ri[i-1]][p-1] /= (*this)[ri[p-1]][p-1];

			// Eliminate [p-1].
			for (int j = p + 1; j <= n; j++)
				(*this)[ri[i-1]][j-1] -= (*this)[ri[i-1]][p-1] * (*this)[ri[p-1]][j-1];
		}

	}

	det = det * (*this)[ri[n-1]][n-1];
	return det != 0.0;
}

void  SMLXMatrix::LUBackSubstitution(SMLXMatrix &idxm, SMLXMatrix &bx) const {
	// Uses the LU decomposition stored in "this" to solve the n linear equations
	// Ax=b.  
	// The idxm 2xn array from LUDecomposition is used.  
	// The bx 1 x n matrix is the input vector b and is 
	// overwritten by the solution vector x.

	assert(nrows() == ncols());
	assert(idxm.ncols() == ncols() && idxm.nrows() == 2);
	assert(bx.nrows()   == nrows() && bx.ncols()   == 1);
	const short n = nrows();

	const short* ri = (short*)idxm[0];
	float* y = idxm[1];

	int i, j;

	float sum;

	// Forward substitution.
	y[0] = bx[ri[0]][0];
	for (i = 2; i <= n; i++) {

		for (sum = 0, j = 1; j <= i - 1; j++)
			sum += (*this)[ri[i-1]][j-1] * y[j-1];

		y[i-1] = bx[ri[i-1]][0] - sum;
	}


	// Back substitution.
	bx[n-1][0] = y[n-1] / (*this)[ri[n-1]][n-1];
	for (i = n - 1; i >= 1; i--) {

		sum = 0;
		for (j = i + 1; j <= n; j++)
			sum += (*this)[ri[i-1]][j-1] * bx[j-1][0];

		bx[i-1][0] = (y[i-1] - sum) / (*this)[ri[i-1]][i-1];

	}
}

#endif

bool SMLXMatrix::SVDecomposition(SMLXMatrix &w,	SMLXMatrix &v, SMLXMatrix &rv1) {
	// Singular value decomposition.  A = [a][w][vT].
	// Input A matrix is this.  Output orthogonal a matrix overwrites this. 
	// Output n x n w matrix is diagonal.  Output n x n v matrix is orthogonal.
	// Ainverse = [v][diag(1/w)][aT].  Inverses of a and v equal their transposes.
	// Scratch 1 x n rv1 vector is used by this function.
	// w~=0 elements may have 1/w set to 0 to solve singular system.
	short  m = nrows(), n = ncols();
	short  flag, i, its, j, jj, k, l, nm;
	double c, f, h, s, x, y, z;
	double anorm=0.0, g=0.0, r;

	rv1.Resize(1,n);
	rv1.Zero();
	if (m < n)
		return false;			// a error: rows < cols

	w.Resize(n, n);
	w.Zero();
	v.Resize(n, n);
	v.Zero();

	SVDBiDiag(w, rv1, anorm);
	SVDInitialWV(w, v, rv1);

	for (k=(n-1); k>=0; k--) {
		for (its = 1; its <= 30; its++) {
			flag = 1;
			for (l=k; l>=0; l--) {
				nm = l - 1;
				if ((FastAbs(rv1[0][l]) + anorm) == anorm) {
					flag = 0;
					break;
				}
				if ((FastAbs(w[nm][nm]) + anorm) == anorm)
					break;
			}
			if (flag) {
				c = 0.0;
				s = 1.0;
				for (i=l; i<=k; i++) {
					f = s * rv1[0][i];
					//rv1[0][i] *= c;
					if ((FastAbs(f) + anorm) != anorm) {
						g = w[i][i];
						h = Pythag(f,g);
						w[i][i] = h;
						h = 1.0 / h;
						c = g * h;
						s = (-f * h);
						for (j=0; j<m; j++) {
							y = (*this)[j][nm];
							z = (*this)[j][i];
							(*this)[j][nm] = y * c + z * s;
							(*this)[j][i] = z * c - y * s;
						}
					}
				}
			}
			z = w[k][k];
			if (l == k) {
				if (z < 0.0) {
					w[k][k] = -z;
					for (j=0; j<n; j++)
						v[j][k] = (-v[j][k]);
				}
				break;
			}
			if (its == 30) {
				return false;		// error: no convergence
			}
			x = w[l][l];
			nm = k - 1;
			y = w[nm][nm];
			g = rv1[0][nm];
			h = rv1[0][k];
			f = ((y-z)*(y+z) + (g-h)*(g+h)) / (2.0f * h * y);
			g = Pythag(f,1.0);
			r = (f >= 0.0 ? g : - g);
			f= ((x-z)*(x+z) + h * ((y / (f + r)) - h)) / x;
			c = s = 1.0;
			for (j=l; j<=nm; j++) {
				i = j + 1;
				g = rv1[0][i];
				y = w[i][i];
				h = s * g;
				g = c * g;
				z = Pythag(f,h);
				rv1[0][j] = z;
				c = f / z;
				s = h / z;
				f = x * c + g * s;
				g = g * c - x * s;
				h = y * s;
				y = y * c;
				for (jj=0; jj<n; jj++) {
					x = v[jj][j];
					z = v[jj][i];
					v[jj][j] = x * c + z * s;
					v[jj][i] = z * c - x * s;
				}
				z = Pythag(f,h);
				w[j][j] = z;
				if (z) {
					z = 1.0 / z;
					c = f * z;
					s = h * z;
				}
				f = (c * g) + (s * y);
				x = (c * y) - (s * g);
				for (jj=0; jj<m; jj++) {
					y = (*this)[jj][j];
					z = (*this)[jj][i];
					(*this)[jj][j] = y * c + z * s;
					(*this)[jj][i] = z * c - y * s;
				}
			}
			rv1[0][l] = 0.0;
			rv1[0][k] = f;
			w[k][k] = x;
		}
	}
	return true;
}

void  SMLXMatrix::SVDBackSubstitution(const SMLXMatrix& U, const SMLXMatrix& W, 
											   const SMLXMatrix& V, const SMLXMatrix& b, SMLXMatrix &tmp) {
	// Assumes U[0..m-1][0..n-1], W[0..n-1][0..n-1] and V[0..n-1][0..n-1] are 
	// svd decomposition of A. 
	// Solves Ax=b[0..m-1,0] for x.  The result x[0..n-1,0] is written to this.  
	// Uses tmp[0..n-1,0] as a temporary vector.
	// Ignores components with zero singular values.
	// Client must set w to zero if w < wmax * 1.0e-12 or appropriate wmin.
	short jj, j, i, ncol = U.ncols(), nrow = U.nrows();
	double s;

	assert(ncol == W.nrows() && ncol == W.ncols() && ncol == V.nrows() && ncol == V.ncols() && nrow == b.nrows());

	tmp.Resize(1, ncol);
	Resize(ncol, 1);
	for (j=0; j<ncol; j++) {
		s = 0.0;
		if (W[j][j]) {			// if wj is 0, result is 0
			for (i=0; i<nrow; i++)
				s += U[i][j] * b[i][0];
			s /= W[j][j];
		}
		tmp[0][j] = s;
	}
	for (j=0; j<ncol; j++) {
		s = 0.0;
		for (jj=0; jj<ncol; jj++)
			s += V[j][jj] * tmp[0][jj];
		(*this)[0][j] = s;
	}
}

void  SMLXMatrix::SVDInvert(const SMLXMatrix& U, const SMLXMatrix& W, SMLXMatrix &V) {
	// this = [V][diag(1/w)][U]'    U, W are unchanged and 
	// V is changed to [V][diag(1/w)]
	// if w is < FLT_EPSILON then 1/w is set to zero.
	double wi, wmax = 0.0, wmin;
	short i, j;
	assert(U.ncols() == W.ncols() && W.nrows() == W.ncols() && W.ncols() == V.nrows() && W.ncols() == V.ncols());
	Resize(U.ncols(), U.nrows());

		// fix w values
	for (j=0; j<W.ncols(); j++) if (W[j][j] > wmax) wmax = W[j][j];
	wmin = FLT_EPSILON;

	for (j=0; j<W.ncols(); j++) {
		wi = W[j][j];				// fix singularities in 1/W
		wi = (wi < wmin) ? 0.0 : 1.0 / wi;
		for (i=0; i<W.ncols(); i++) {		// [V] = [V][diag(1/w)]
			V[i][j] *= wi;
		}
	}
	this->Mult(V, U, 0,1);				// this = [V][diag(1/w)][U]'
}

void  SMLXMatrix::Jacobi(SMLXMatrix &eigval, SMLXMatrix &eigvec, int &nrot) {
	// Computes all eigenvalues and eigenvectors of "this" real symmetric n x n matrix.
	// eigval, a 1 x n vector returns the resulting eigenvalues.
	// eigvec, an n x n matrix whose columns return the normalized eigenvectors.
	// nrot returns the number of Jacobi rotations required.
	// Elements of this above the diagonal are destroyed in the process.
	assert(0);
}


void  SMLXMatrix::SVDBiDiag(SMLXMatrix &w, SMLXMatrix &rv1, double &anorm) {
	double f, g = 0.0, h, r, s = 0.0, scale = 0.0;
	short m = nrows(), n = ncols(), i, j, k, l ;
	anorm = 0.0;
	for (i=0; i<n; i++) {
		l = i+1;
		rv1[0][i] = scale * g;
		g = s = scale = 0.0;
		if (i < m) {
			for (k=i; k<m; k++)
				scale += FastAbs((*this)[k][i]);
			if (scale) {
				for (k=i; k<m; k++) {
					(*this)[k][i] /= scale;
					s += (*this)[k][i] * (*this)[k][i];
				}
				f = (*this)[i][i];
				g = FastSqrt(s);
				if (f >= 0.0)
					g = - g;
				h = f * g - s;
				(*this)[i][i] = f - g;
				if (i != (n-1)) {
					for (j=l; j<n; j++) {
						for (s = 0.0, k=i; k<m; k++)
							s += (*this)[k][i] * (*this)[k][j];
						f = s / h;
						for (k=i; k<m; k++)
							(*this)[k][j] += f * (*this)[k][i];
					}
				}
				for (k=i; k<m; k++)
					(*this)[k][i] *= scale;
			}
		}
		w[i][i] = scale * g;
		g = s = scale = 0.0;
		if (i<m && i!=(n-1)) {
			for (k=l; k<n; k++)
				scale += FastAbs((*this)[i][k]);
			if (scale) {
				for (k=l; k<n; k++) {
					(*this)[i][k] /= scale;
					s += (*this)[i][k] * (*this)[i][k];
				}
				f = (*this)[i][l];
				g = FastSqrt(s);
				if (f >= 0.0)
					g = -g;
				h = 1.0f / (f * g - s);
				(*this)[i][l] = f - g;
				for (k=l; k<n; k++)
					rv1[0][k] = (*this)[i][k] * h;
				if (i != (m-1)) {
					for (j=l; j<m; j++) {
						for (s = 0.0, k=l; k<n; k++)
							s += (*this)[j][k] * (*this)[i][k];
						for (k=l; k<n; k++)
							(*this)[j][k] += s * rv1[0][k];
					}
				}
				for (k=l; k<n; k++)
					(*this)[i][k] *= scale;
			}
		}
		r = (FastAbs(w[i][i]) + FastAbs(rv1[0][i]));
		if (r > anorm)
			anorm = r;
	}
}

void  SMLXMatrix::SVDInitialWV(SMLXMatrix &w, SMLXMatrix &v, SMLXMatrix &rv1) {
	short m = nrows(), n = ncols(), i, j, k, l;
	double f, g = 0.0, s;

	for (i=(n-1); i>=0; i--) {
		l = i + 1;
		if (i < (n-1)) {
			if (g) {
				for (j=l; j<n; j++)
					v[j][i] = ((*this)[i][j] / (*this)[i][l]) / g;
				// double division to reduce underflow
				for (j=l; j<n; j++) {
					for (s = 0.0, k=l; k<n; k++)
						s += (*this)[i][k] * v[k][j];
					for (k=l; k<n; k++)
						v[k][j] += s * v[k][i];
				}
			}
			for (j=l; j<n; j++)
				v[i][j] = v[j][i] = 0.0;
		}
		v[i][i] = 1.0;
		g = rv1[0][i];
	}
	for (i=(n-1); i>=0; i--) {
		l = i + 1 ;
		g = w[i][i];
		if (i < (n-1)) {
			for (j=l; j<n; j++)
				(*this)[i][j]=0.0;
		}
		if (g) {
			g = 1.0 / g;
			if (i != (n-1)) {
				for (j=l; j<n; j++) {
					for (s = 0.0, k=l; k<m; k++)
						s += (*this)[k][i] * (*this)[k][j];
					f = (s / (*this)[i][i]) * g;
					for (k=i; k<m; k++)
						(*this)[k][j] += f * (*this)[k][i];
				}
			}
			for (j=i; j<m; j++)
				(*this)[j][i] *= g;
		} else {
			for (j=i; j<m; j++)
				(*this)[j][i] = 0.0;
		}
		(*this)[i][i] += 1.0;
	}
}
 
float SMLXMatrix::Pythag(double a, double b) const {
	// computes (a^2 + b^2)^1/2 without destructive underflow or overflow
	double at, bt, ct;
	at = FastAbs(a);
	bt = FastAbs(b);
	if (at > bt) {
		ct = bt/at;
		return at * FastSqrt(1.0 + ct * ct);
	} else {
		if (bt) {
			ct = at/bt;
			return bt * FastSqrt(1.0 + ct * ct);
		} else {
			return 0.0;
		}
	}
}

void SMLXTmpMatrix::PoolElement(short nr, short nc)
{
	msize = nr*nc;
	maxsize = nr*nc;
	owned = 0;
	flags = 1;
	mat = (float*)SMLMemoryPool::GetFloat(nr*nc);
}

int SMLXTmpMatrix::InitTmpPool(short nr, short nc, SMLXTmpMatrix* p, int pSize)
{
	for(int i = 0; i<pSize; i++)
		p[i].PoolElement(nr, nc);
	return 1;
}
