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

#ifndef _AN
#define _AN ((_CS/_NN)%10)
#endif

#if _AN == 1
	#undef _AN
	#define _AN 1
#elif _AN == 2
	#undef _AN
	#define _AN 2
#elif _AN == 3
	#undef _AN
	#define _AN 3
#elif _AN == 4
	#undef _AN
	#define _AN 4
#elif _AN == 5
	#undef _AN
	#define _AN 5
#elif _AN == 6
	#undef _AN
	#define _AN 6
#endif


#if _AN > 0

#if _AC == 1
#define e_x e1
#elif _AC == 2
#define e_x e2
#elif _AC == 3
#define e_x e3
#elif _AC == 4
#define e_x e4
#elif _AC == 5
#define e_x e5
#elif _AC == 6
#define e_x e6
#endif

#define iir _AR
#define iic _AN


// 00
#ifdef  _MMBODY
__declspec(naked)
#endif
SMLXTmpMatrix/*<iir, iic>*/& __fastcall n_rxc_rxc(Mult00,_AR,_AC,_AC,_AN)(const SMLXMatrix/*<_AR, _AC>*/ &A, const SMLXMatrix/*<_AC, _AN>*/ &B)
#ifndef _MMBODY
;
#else
#undef  fpOP
#define fpOP(r, c) e_x(r, c, _AN, _AC, _AN, 0, 0)
{
	nrxc(TMat,iir,iic)();//SMLTMatrix<iir, iic>::TMat();
	prologue
#include <smlxunroll.inl>
	epilogue
}
#endif

// 01
#ifdef  _MMBODY
__declspec(naked)
#endif
SMLXTmpMatrix/*<iir, iic>*/& __fastcall n_rxc_rxc(Mult01,_AR,_AC,_AN,_AC)(const SMLXMatrix/*<_AR, _AC>*/ &A, const SMLXTransposedMatrix/*<_AN, _AC>*/ &B)
#ifndef _MMBODY
;
#else
#undef  fpOP
#define fpOP(r, c) e_x(r, c, _AN, _AC, _AC, 0, 1)
{
	nrxc(TMat,iir,iic)();//SMLTMatrix<iir, iic>::TMat();
	prologue
#include <smlxunroll.inl>
	epilogue
}
#endif

#undef  iir
#undef  iic

#undef  e_x
#if _AR == 1
#define e_x e1
#elif _AR == 2
#define e_x e2
#elif _AR == 3
#define e_x e3
#elif _AR == 4
#define e_x e4
#elif _AR == 5
#define e_x e5
#elif _AR == 6
#define e_x e6
#endif

#define iir _AC
#define iic _AN

// 10
#ifdef  _MMBODY
__declspec(naked)
#endif
SMLXTmpMatrix/*<iir, iic>*/& __fastcall n_rxc_rxc(Mult10,_AR,_AC,_AR,_AN)(const SMLXTransposedMatrix/*<_AR, _AC>*/ &A, const SMLXMatrix/*<_AR, _AN>*/ &B)
#ifndef _MMBODY
;
#else
#undef  fpOP
#define fpOP(r, c) e_x(r, c, _AN, _AC, _AN, 1, 0)
{
	nrxc(TMat,iir,iic)();//SMLTMatrix<iir, iic>::TMat();
	prologue
#include <smlxunroll.inl>
	epilogue
}
#endif

// 11
#ifdef  _MMBODY
__declspec(naked)
#endif
SMLXTmpMatrix/*<iir, iic>*/& __fastcall n_rxc_rxc(Mult11,_AR,_AC,_AN,_AR)(const SMLXTransposedMatrix/*<_AR, _AC>*/ &A, const SMLXTransposedMatrix/*<_AN, _AR>*/ &B)
#ifndef _MMBODY
;
#else
#undef  fpOP
#define fpOP(r, c) e_x(r, c, _AN, _AC, _AR, 1, 1)
{
	nrxc(TMat,iir,iic)();//SMLTMatrix<iir, iic>::TMat();
	prologue
#include <smlxunroll.inl>
	epilogue
}
#endif

#undef  fpOP
#undef  iir
#undef  iic
#undef  e_x

#endif

#undef  _NN
#undef  _AN
