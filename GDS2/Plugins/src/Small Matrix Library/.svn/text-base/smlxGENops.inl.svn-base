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
//#pragma message("INCLUDE smlxops.inl")

///////////////////////////////////////
// Multiplication and other operations.
///////////////////////////////////////

// Visual C++ is not ISO C++ yet. It can not deduce nr, nc in
// template<short nr, short nc> f(SMLTMatrix<nr, nc>) {...}
// from f(M). So, we declare some classes with these macros.

#undef  DefTranspose
#undef  DECLARE

/*
#ifdef  _MMBODY
#define DefTranspose(anr, anc)                 \
SMLTTransposedMatrix<anr, anc>&                \
Transpose(const SMLTMatrix<anr, anc>& m) {     \
	return (SMLTTransposedMatrix<anr, anc>&)m; \
}                                              \
SMLTTransposedMatrix<anr, anc>&                \
Transpose(SMLTTmpMatrix<anr, anc>& m) {        \
	return (SMLTTransposedMatrix<anr, anc>&)m; \
}
#else
#define DefTranspose(anr, anc)                 \
SMLTTransposedMatrix<anr, anc>&                \
Transpose(const SMLTMatrix<anr, anc>& m);      \
SMLTTransposedMatrix<anr, anc>&                \
Transpose(SMLTTmpMatrix<anr, anc>& m);
#endif
*/

// Load addresses:
// ebx = [ecx] ; 1st argument with __fastcall
// ecx = [edx] ; 2st argument with __fastcall
// eax - return value from TMat(), saved in edx
// eax = TMat().mat
#undef  prologue
#define prologue \
	__asm push  ebx                   \
	__asm mov	ebx, dword ptr [ecx]  \
	__asm mov	ecx, dword ptr [edx]  \
	__asm mov   edx, eax              \
	__asm mov   eax, [edx]
#undef  epilogue
#define epilogue \
	__asm mov   eax, edx              \
	__asm pop   ebx                   \
	__asm ret

/////////////////////////
//                     //
// Now the real stuff. //
//                     //
/////////////////////////


#define _AR 1
#define _AC 1
#define _CS 16
#include <smlxfun.inl>

#define _AR 3
#define _AC 3
#define _CS 136
#include <smlxfun.inl>

#define _AR 3
#define _AC 1
#define _CS 13
#include <smlxfun.inl>

#define _AR 1
#define _AC 3
#define _CS 13
#include <smlxfun.inl>

#define _AR 4
#define _AC 4
#define _CS 14
#include <smlxfun.inl>

#define _AR 4
#define _AC 1
#define _CS 14
#include <smlxfun.inl>

#define _AR 1
#define _AC 4
#define _CS 14
#include <smlxfun.inl>

#define _AR 5
#define _AC 5
#define _CS 1356
#include <smlxfun.inl>

#define _AR 5
#define _AC 1
#define _CS 1356
#include <smlxfun.inl>

#define _AR 1
#define _AC 5
#define _CS 1356
#include <smlxfun.inl>

#define _AR 5
#define _AC 3
#define _CS 1356
#include <smlxfun.inl>

#define _AR 3
#define _AC 5
#define _CS 1356
#include <smlxfun.inl>

#define _AR 5
#define _AC 6
#define _CS 1356
#include <smlxfun.inl>

#define _AR 6
#define _AC 5
#define _CS 1356
#include <smlxfun.inl>

#define _AR 6
#define _AC 6
#define _CS 1356
#include <smlxfun.inl>

#define _AR 6
#define _AC 1
#define _CS 1356
#include <smlxfun.inl>

#define _AR 1
#define _AC 6
#define _CS 1356
#include <smlxfun.inl>

#define _AR 6
#define _AC 3
#define _CS 1356
#include <smlxfun.inl>

#define _AR 3
#define _AC 6
#define _CS 1356
#include <smlxfun.inl>

