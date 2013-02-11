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


// Define the Transpose() function.
//DefTranspose(_AR, _AC);
//#pragma message("INCLUDE smlxfun.inl")

#ifdef _MMBODY
SMLXTmpMatrix/*<_AR, _AC>*/ /*SMLTMatrix<_AR, _AC>::*/nrxc(pool,_AR,_AC)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, _AR, _AC) = SMLXTmpMatrix::InitTmpPool(_AR, _AC, nrxc(pool,_AR,_AC), SMLXMatrix::poolSize);

	#define DECLARE __declspec(naked)
#else
	#define DECLARE SMLLIBENTRY
#endif

#ifndef _MMBODY
DECLARE
SMLXTmpMatrix& __fastcall nrxc(TMat,_AR,_AC)(); 
#else
//DECLARE
SMLXTmpMatrix& __fastcall nrxc(TMat,_AR,_AC)() 
{
	enum {pool_sz = sizeof(nrxc(pool,_AR,_AC)), class_sz = sizeof(SMLXTmpMatrix)};
//  if (poolPointer == poolEnd) poolPointer -= pool_sz;
//  poolPointer += class_sz;
//	return *(SMLTTmpMatrix<nr, nc>*)poolPointer;
	static char* poolPointer = (char*)&nrxc(pool,_AR,_AC)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,_AR,_AC)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		// Conservative assertion.
/*#ifndef NDEBUG
		assert(++subExpressionsCount <= SMLXMatrix::poolSize);
#endif
*/
	ASSERT_SUBEXPRESSION_COUNT

	return *(SMLXTmpMatrix*)poolPointer;
}
#endif

#undef  fpOP
#define fpOP(n1, n2) fpOP1(4 * (n1 * _AC + n2))

#undef  DereferenceFastCallParameters
#define DereferenceFastCallParameters \
	__asm mov	eax, dword ptr [edx]  \
	__asm mov	edx, eax              \
	__asm mov	eax, dword ptr [ecx]  \
	__asm mov	ecx, eax


// In this file macro fpOP is redefined to specify different operations
// to be performed. These operations are unrolled in file smltunroll.inl.

#define iir _AR
#define iic _AC

DECLARE
SMLXTmpMatrix& __fastcall nrxc(Add,_AR,_AC)(const SMLXMatrix &A, const SMLXMatrix &B) 
#ifndef _MMBODY
;
#else
#undef  fpOP1
#define fpOP1(n) \
	__asm fld	dword ptr [ebx + n] \
	__asm fadd	dword ptr [ecx + n] \
	__asm fstp	dword ptr [eax + n]
{
	nrxc(TMat,_AR,_AC)();//A.TMat();
	prologue
#include <smlxunroll.inl>
	epilogue
}
#endif

DECLARE
SMLXTmpMatrix& __fastcall nrxc(Sub,_AR,_AC)(const SMLXMatrix &A, const SMLXMatrix &B) 
#ifndef _MMBODY
;
#else
#undef  fpOP1
#define fpOP1(n) \
	__asm fld	dword ptr [ebx + n] \
	__asm fsub	dword ptr [ecx + n] \
	__asm fstp	dword ptr [eax + n]
{
	nrxc(TMat,_AR,_AC)();//A.TMat();
	prologue
#include <smlxunroll.inl>
	epilogue
}
#endif

DECLARE
void __fastcall nrxc(AddTo,_AR,_AC)(SMLXMatrix &A, const SMLXMatrix &B) 
#ifndef _MMBODY
;
#else
#undef  fpOP1
#define fpOP1(n) \
	__asm fld	dword ptr [ecx + n] \
	__asm fadd	dword ptr [edx + n] \
	__asm fstp	dword ptr [ecx + n]
{
	DereferenceFastCallParameters
#include <smlxunroll.inl>
/*#ifndef NDEBUG
	subExpressionsCount = 0;
#endif*/
	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}
#endif

DECLARE
void __fastcall nrxc(SubFrom,_AR,_AC)(SMLXMatrix &A, const SMLXMatrix &B) 
#ifndef _MMBODY
;
#else
#undef  fpOP1
#define fpOP1(n) \
	__asm fld	dword ptr [ecx + n] \
	__asm fsub	dword ptr [edx + n] \
	__asm fstp	dword ptr [ecx + n]
{
	DereferenceFastCallParameters
#include <smlxunroll.inl>
/*#ifndef NDEBUG
	subExpressionsCount = 0;
#endif*/
	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}
#endif

DECLARE
void __fastcall nrxc(Assign,_AR,_AC)(SMLXMatrix &A, const SMLXMatrix &B) 
#ifndef _MMBODY
;
#else
#undef  fpOP1
#define fpOP1(n) \
	__asm fld	dword ptr [edx + n] \
	__asm fstp	dword ptr [ecx + n]
{
	DereferenceFastCallParameters

#include <smlxunroll.inl>
	__asm ret
}
#endif

DECLARE
void __fastcall nrxc(AssignValue,_AR,_AC)(SMLXMatrix &A, float initialValue) 
#ifndef _MMBODY
;
#else
#undef  fpOP1
#define fpOP1(n) \
	__asm mov 	dword ptr [eax + n], edx
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
#include <smlxunroll.inl>
	__asm ret 4
}
#endif


//////////////////
// Multiplication.
//////////////////

#undef  fpOP
#undef  fpOP1
#undef  iir
#undef  iic

#define _NN 1
#include <smlxmult.inl>

#define _NN 10
#include <smlxmult.inl>

#define _NN 100
#include <smlxmult.inl>

#define _NN 1000
#include <smlxmult.inl>

#define _NN 10000
#include <smlxmult.inl>

#define _NN 100000
#include <smlxmult.inl>

#undef  _AR
#undef  _AC
#undef  _CS
