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

#ifndef __SMLTMATRIX_H__
#define __SMLTMATRIX_H__

//#pragma message("INCLUDE smlxopt.h")

#include <stdio.h>
#include <time.h>
#include <stddef.h>

#include <smlmath.h>

#ifndef NDEBUG
SMLLIBENTRY extern int subExpressionsCount;
#endif

//    SMLLIBENTRY void AllignAllocate() {
//        mat = new float[nr * nc + 8];
//        long address = (long) mat;
//        if (address%32 != 0) address = 32 + 32 * (address/32);
//        mat = (float*)address;
//    }


// Objects of type SMLXTmpMatrix are used in the matrix expressions.
// Memory for this is allocated from some pool and is overwritten
// when pointers are overwrapped. It is assumed, that any expression
// requires no more then MaxNumberOfSubexpressions temporary entities.

class SMLMemoryPool {
public:
	static float* GetFloat(int n);

private:
	//!! Reserve memory for some predefined sizes (no 2X matrices now).
	enum {
		MemorySize = 16+SMLXMatrix::poolSize * ((1+3+4+5+6) * (1+3+4+5+6)+(1+1+1+1+1) * (16+16+16+16+16))
	};
	static int index;
	static float pool[MemorySize];
	static unsigned int poolStart;
};

// General commutativity, may be not appropriate for everything...
template <class T> inline
T& operator*(float scale, T& A) { T& An = (T&)A.TMat(); An *= scale; return An; }


// Some additional function declarations.
#include <smlxopt.inl>


// Time measurements.

/*
static int _StartHi;
static int _StartLo;
inline void StartTimer() {
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	__asm _emit 0x0F __asm _emit 0x31   // Read clock ticks into edx:eax
	__asm mov _StartLo, eax             // Save the result
	__asm mov _StartHi, edx             // Save the result
}
inline int* Timer() {
   static int FinishLo, FinishHi;
	__asm _emit 0x0F __asm _emit 0x31   // Read clock ticks into edx:eax
	__asm mov FinishLo, eax             // Save the result
	__asm mov FinishHi, edx             // Save the result
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
   FinishHi -= _StartHi;
   FinishLo -= _StartLo;
   return (int*)&FinishLo;
 }
*/

#endif
