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

// Include some additional function declarations and inlined
// functions.
// If _MMBODY is defined, it will have definitions as well.


#define swapmem(n) \
	__asm mov ecx, dword ptr [eax + n] \
	__asm mov edx, dword ptr [ebx + n] \
	__asm mov dword ptr [eax + n], edx \
	__asm mov dword ptr [ebx + n], ecx


// Using internal memory allocation.
inline float* SMLMemoryPool::GetFloat(int n) {
	if(poolStart == 0xffffffff)
	{
		poolStart =((((unsigned int)pool)+0xf) & 0xfffffff0);
	}
	float* cp = (float*)poolStart + index;
	index += (n+3) & 0xfffffffc;
	assert(index <= (MemorySize-16));
	return cp;
}

///////////////////////////
// Family of the inversions
// without pivoting.
///////////////////////////
//prototypes only
//bodies gone to smlxmatrix.cpp
//
SMLLIBENTRY
bool Invert1x1(SMLXMatrix& m);

SMLLIBENTRY
bool Invert3x3(SMLXMatrix& m);

SMLLIBENTRY
bool Invert4x4(SMLXMatrix& m);

SMLLIBENTRY
bool Invert5x5(SMLXMatrix& m);

SMLLIBENTRY
bool Invert6x6(SMLXMatrix& m);

// Optimized multiplications & other operations.
#undef nrxc
#define nrxc(name, r, c) name ## r ## x ## c
#undef n_rxc_rxc
#define n_rxc_rxc(name,r1,c1,r2,c2) name ## _ ## r1 ## x ## c1 ## _ ## r2 ## x ## c2 

// Parameters for the macros:
// w - width (# of columns) for the particular matrix
// t - 1 for transpose, 0 for not (aka a, b for 1st and 2nd matrices)
// i - statement(s) for M[i][j]
// j -    will be generated
// k - kind of third index in the multiplication
// l - width of the result
// m - width of the first matrix
// n - width of the second matrix
// a - 1 for transpose, 0 for not (matrix A)
// b - 1 for transpose, 0 for not (matrix B)


// Offset for mat[i][j], w is an row width, t == 1 for transposed access.
#define mi(w, t, i, j)  4 * ((i * w + j) * (1-t) + (j * w + i) * t)

// Load & multiply.
#define flm(k, i, j, m, n, a, b) \
	__asm fld	dword ptr [ebx + mi(m, a, i, k)] \
	__asm fmul	dword ptr [ecx + mi(n, b, k, j)]

// Load, multiply & add.
#define flma(k, i, j, m, n, a, b) flm(k, i, j, m, n, a, b) __asm faddp ST(1), ST(0)


////////////////////////////
// e<x> for different sizes.
////////////////////////////

#define e1(i, j, l, m, n, a, b) \
	flm(0, i, j, m, n, a, b)    \
	__asm fstp	dword ptr [eax + mi(l, 0, i, j)]

#define e2(i, j, l, m, n, a, b) \
	flm (0, i, j, m, n, a, b)   \
	flma(1, i, j, m, n, a, b)   \
	__asm fstp	dword ptr [eax + mi(l, 0, i, j)]

#define e3(i, j, l, m, n, a, b) \
	flm (0, i, j, m, n, a, b)   \
	flma(1, i, j, m, n, a, b)   \
	flma(2, i, j, m, n, a, b)   \
	__asm fstp	dword ptr [eax + mi(l, 0, i, j)]

// e4 is optimized.
#define e4(i, j, l, m, n, a, b) \
	flm(0, i, j, m, n, a, b)    \
	flm(1, i, j, m, n, a, b)    \
	flm(2, i, j, m, n, a, b)    \
	flm(3, i, j, m, n, a, b)    \
	__asm faddp st(1), st(0)    \
	__asm fxch  st(2)           \
	__asm faddp st(1), st(0)    \
	__asm faddp st(1), st(0)    \
	__asm fstp	dword ptr [eax + mi(l, 0, i, j)]

#define e5(i, j, l, m, n, a, b) \
	flm (0, i, j, m, n, a, b)   \
	flma(1, i, j, m, n, a, b)   \
	flma(2, i, j, m, n, a, b)   \
	flma(3, i, j, m, n, a, b)   \
	flma(4, i, j, m, n, a, b)   \
	__asm fstp	dword ptr [eax + mi(l, 0, i, j)]

// e6 is optimized.
#define e6(i, j, l, m, n, a, b) \
	flm(0, i, j, m, n, a, b)    \
	flm(1, i, j, m, n, a, b)    \
	flm(2, i, j, m, n, a, b)    \
	flm(3, i, j, m, n, a, b)    \
	flm(4, i, j, m, n, a, b)    \
	flm(5, i, j, m, n, a, b)    \
	__asm faddp st(1), st(0)    \
	__asm fxch  st(2)           \
	__asm faddp st(1), st(0)    \
	__asm faddp st(1), st(0)    \
	__asm fxch  st(2)           \
	__asm faddp st(1), st(0)    \
	__asm faddp st(1), st(0)    \
	__asm fstp	dword ptr [eax + mi(l, 0, i, j)]

//including optimized functions declarations
//functions definitions are included in smlxmatrix.cpp

#ifdef _PIII_CODE_
	#include <smlxSSEops.h>
#else
	#include <smlxGENops.h>
#endif


//#include <smlxGENops.h>

// General assignement (cld before movsd?)
// It should be invoked for sizes different, that specified above.
#define genass(nr, nc) \
/*void   force_instantiation ## nr ## nc(SMLTMatrix<nr, nc>);*/ \
inline void Assign ## nr ## nc ##(SMLXMatrix &A, const SMLXMatrix &B) { \
	enum {sz = nr * nc};             \
	__asm push  edi                  \
	__asm push  esi                  \
	__asm mov	edx, dword ptr A.mat \
	__asm mov	edi, dword ptr [edx] \
	__asm mov	edx, dword ptr B.mat \
	__asm mov	esi, dword ptr [edx] \
	__asm mov	ecx, sz              \
	__asm rep   movsd                \
	__asm pop   edi                  \
	__asm pop   esi                  \
} \
inline void Assign ## nr ## nc ## (SMLXMatrix &A, float initialValue) { \
	enum {sz = nr * nc};             \
	__asm push  edi                  \
	__asm mov	edx, dword ptr A.mat \
	__asm mov	edi, dword ptr [edx] \
	__asm mov	eax, initialValue    \
	__asm mov	ecx, sz              \
	__asm rep   stosd                \
	__asm pop   edi                  \
}

