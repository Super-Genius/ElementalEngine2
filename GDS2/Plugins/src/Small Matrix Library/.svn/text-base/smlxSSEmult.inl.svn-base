
#include "xmmintrin.h"

#undef  prologue
#define prologue \
	__asm push  ebx                   \
	__asm mov	ebx, dword ptr [ecx]  \
	__asm mov	ecx, dword ptr [edx]  \
	__asm mov   edx, eax              \
	__asm mov   eax, [edx]


#undef  epilogue
#define epilogue \
	__asm pop   ebx                   \
	__asm mov   eax, edx              \
	__asm ret


#pragma warning (disable : 592 )

_declspec(naked)
SMLXTmpMatrix& __fastcall Mult00_3x3_3x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,3,1)();

	prologue
	
	_asm {

		movss	xmm1, dword ptr [ebx]
		movhps	xmm1, qword ptr [ebx+4]
		movaps	xmm5, xmm1
		movss	xmm3, dword ptr [ebx+12]
		movhps	xmm3, qword ptr [ebx+24]
		movss	xmm4, dword ptr [ecx]
		shufps	xmm5, xmm3, 128
		movlps	xmm0, qword ptr [ebx+16]
		shufps	xmm4, xmm4, 0
		movhps	xmm0, qword ptr [ebx+28]
		shufps	xmm1, xmm0, 219
		movss	xmm2, dword ptr [ecx+4]
		movaps	xmm3, xmm1
		shufps	xmm1, xmm0, 129
		shufps	xmm2, xmm2, 0
		movss	xmm0, dword ptr [ecx+8]
		mulps	xmm4, xmm5
		mulps	xmm2, xmm1
		shufps	xmm0, xmm0, 0
		addps	xmm4, xmm2
		mulps	xmm0, xmm3
		addps	xmm4, xmm0
		movss	dword ptr [eax], xmm4
		movhps	qword ptr [eax+4], xmm4
	}

	epilogue

} // Mult00_3x3_3x1

SMLXTmpMatrix& __fastcall Mult11_3x1_3x3(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,3)();
	register float* dst = dmat.Data();
	__m128	b0, b1, b2;
	__m128	row, rslt;

	b2		= _mm_loadh_pi( _mm_load_ss( &src1[0] ), (__m64*)&src1[1] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[4] ), (__m64*)&src1[7] );
	b0		= _mm_loadh_pi( _mm_load_ss( &src1[3] ), (__m64*)&src1[6] );

	b0		= _mm_shuffle_ps( b2, b0, 0x80 );
	b2		= _mm_shuffle_ps( b2, b1, 0xDB );
	b1		= _mm_shuffle_ps( b2, b1, 0x81 );

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult11_3x1_3x3

SMLXTmpMatrix& __fastcall Mult01_1x3_3x3(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,3)();
	register float* dst = dmat.Data();
	__m128	b0, b1, b2;
	__m128	row, rslt;

	b2		= _mm_loadh_pi( _mm_load_ss( &src1[0] ), (__m64*)&src1[1] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[4] ), (__m64*)&src1[7] );
	b0		= _mm_loadh_pi( _mm_load_ss( &src1[3] ), (__m64*)&src1[6] );

	b0		= _mm_shuffle_ps( b2, b0, 0x80 );
	b2		= _mm_shuffle_ps( b2, b1, 0xDB );
	b1		= _mm_shuffle_ps( b2, b1, 0x81 );

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult01_1x3_3x3

SMLXTmpMatrix& __fastcall Mult10_3x3_3x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	b0		= _mm_loadh_pi( _mm_load_ss( &src1[0] ), (__m64*)&src1[1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src1[3] ), (__m64*)&src1[4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src1[6] ), (__m64*)&src1[7] );

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult10_3x3_3x1

SMLXTmpMatrix& __fastcall Mult00_1x3_3x3(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	b0		= _mm_loadh_pi( _mm_load_ss( &src1[0] ), (__m64*)&src1[1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src1[3] ), (__m64*)&src1[4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src1[6] ), (__m64*)&src1[7] );

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult00_1x3_3x3

SMLXTmpMatrix& __fastcall Mult10_3x1_3x3(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	b0		= _mm_loadh_pi( _mm_load_ss( &src1[0] ), (__m64*)&src1[1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src1[3] ), (__m64*)&src1[4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src1[6] ), (__m64*)&src1[7] );

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult10_3x1_3x3

SMLXTmpMatrix& __fastcall Mult11_3x3_1x3(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	b0		= _mm_loadh_pi( _mm_load_ss( &src1[0] ), (__m64*)&src1[1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src1[3] ), (__m64*)&src1[4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src1[6] ), (__m64*)&src1[7] );

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult11_3x3_1x3

SMLXTmpMatrix& __fastcall Mult01_3x3_1x3(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	b0		= _mm_loadh_pi( _mm_load_ss( &src1[0] ), (__m64*)&src1[1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src1[3] ), (__m64*)&src1[4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src1[6] ), (__m64*)&src1[7] );

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult01_3x3_1x3

_declspec(naked)
SMLXTmpMatrix& __fastcall Mult00_3x3_3x3(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,3,3)();

	prologue

	_asm {

		movss	xmm2, dword ptr [ecx+32]
		movhps	xmm2, qword ptr [ecx+24]

		movss	xmm3, dword ptr [ebx  ]
		movss	xmm4, dword ptr [ebx+4]

		movss	xmm0, dword ptr [ecx]
		movhps	xmm0, qword ptr [ecx+4]
		shufps	xmm2, xmm2, 0x36
		shufps	xmm3, xmm3, 0

		movss	xmm1, dword ptr [ecx+12]
		movhps	xmm1, qword ptr [ecx+16]

		shufps	xmm4, xmm4, 0
		mulps	xmm3, xmm0
		movss	xmm5, dword ptr [ebx+8]
		movss	xmm6, dword ptr [ebx+12]
		mulps	xmm4, xmm1
		shufps	xmm5, xmm5, 0
		mulps	xmm5, xmm2
		shufps	xmm6, xmm6, 0
		mulps	xmm6, xmm0
		addps	xmm3, xmm4


		movss	xmm7, dword ptr [ebx+16]
		movss	xmm4, dword ptr [ebx+28]

		shufps	xmm7, xmm7, 0
		addps	xmm3, xmm5
		mulps	xmm7, xmm1

		shufps	xmm4, xmm4, 0

		movss	xmm5, dword ptr [ebx+20]
		shufps	xmm5, xmm5, 0
		mulps	xmm4, xmm1

		mulps	xmm5, xmm2
		addps	xmm6, xmm7

		movss	xmm1, dword ptr [ebx+24]

		movss	dword ptr [eax  ], xmm3		
		movhps	qword ptr [eax+4], xmm3

		addps	xmm6, xmm5
		shufps	xmm1, xmm1, 0

		movss	xmm5, dword ptr [ebx+32]
		mulps	xmm1, xmm0
		shufps	xmm5, xmm5, 0
		
		mulps	xmm5, xmm2
		addps	xmm1, xmm4

		addps	xmm1, xmm5

		shufps	xmm1, xmm1, 0x8F

		movss	dword ptr [eax+12], xmm6
		movhps	qword ptr [eax+16], xmm6
		movhps	qword ptr [eax+24], xmm1
		movss	dword ptr [eax+32], xmm1

	}

	epilogue

} // Mult00_3x3_3x3


SMLXTmpMatrix& __fastcall Mult01_3x3_3x3(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	b2		= _mm_loadh_pi( _mm_load_ss( &src2[0] ), (__m64*)&src2[1] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[4] ), (__m64*)&src2[7] );
	b0		= _mm_loadh_pi( _mm_load_ss( &src2[3] ), (__m64*)&src2[6] );

	b0		= _mm_shuffle_ps( b2, b0, 0x80 );
	b2		= _mm_shuffle_ps( b2, b1, 0xDB );
	b1		= _mm_shuffle_ps( b2, b1, 0x81 );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	return dmat;

} // Mult01_3x3_3x3

SMLXTmpMatrix& __fastcall Mult10_3x3_3x3(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[0] ), (__m64*)&src2[1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src2[3] ), (__m64*)&src2[4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src2[6] ), (__m64*)&src2[7] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	return dmat;

} // Mult10_3x3_3x3

SMLXTmpMatrix& __fastcall Mult11_3x3_3x3(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	b2		= _mm_loadh_pi( _mm_load_ss( &src2[0] ), (__m64*)&src2[1] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[4] ), (__m64*)&src2[7] );
	b0		= _mm_loadh_pi( _mm_load_ss( &src2[3] ), (__m64*)&src2[6] );

	b0		= _mm_shuffle_ps( b2, b0, 0x80 );
	b2		= _mm_shuffle_ps( b2, b1, 0xDB );
	b1		= _mm_shuffle_ps( b2, b1, 0x81 );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	return dmat;

} // Mult11_3x3_3x3

_declspec(naked)
SMLXTmpMatrix& __fastcall Mult00_6x6_6x6(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,6,6)();

	prologue

	_asm {

		movlps	xmm3, qword ptr [ecx+72]

		//	Loading first 4 columns (upper 4 rows) of src2.
		movaps	xmm0, xmmword ptr [ecx]
		movlps	xmm1, qword ptr [ecx+24]
		movhps	xmm1, qword ptr [ecx+32]
		movaps	xmm2, xmmword ptr [ecx+48]
		movhps	xmm3, qword ptr [ecx+80]

		//	Calculating first 4 elements in the first row of the destination matrix.
		movss	xmm4, dword ptr [ebx]
		movss	xmm5, dword ptr [ebx+4]
		shufps	xmm4, xmm4, 0
		movss	xmm6, dword ptr [ebx+8]
		shufps	xmm5, xmm5, 0
		movss	xmm7, dword ptr [ebx+12]
		mulps	xmm4, xmm0
		shufps	xmm6, xmm6, 0
		shufps	xmm7, xmm7, 0
		mulps	xmm5, xmm1
		mulps	xmm6, xmm2
		addps	xmm5, xmm4
		mulps	xmm7, xmm3
		addps	xmm6, xmm5
		addps	xmm7, xmm6

		movaps	xmmword ptr [eax], xmm7

		//	Calculating first 4 elements in the second row of the destination matrix.
		movss	xmm4, dword ptr [ebx+24]
		movss	xmm5, dword ptr [ebx+28]
		movss	xmm6, dword ptr [ebx+32]
		movss	xmm7, dword ptr [ebx+36]

		shufps	xmm4, xmm4, 0
		shufps	xmm5, xmm5, 0
		shufps	xmm6, xmm6, 0
		shufps	xmm7, xmm7, 0

		mulps	xmm4, xmm0
		mulps	xmm5, xmm1
		mulps	xmm6, xmm2
		mulps	xmm7, xmm3

		addps	xmm7, xmm6
		addps	xmm5, xmm4
		addps	xmm7, xmm5

		//	Calculating first 4 elements in the third row of the destination matrix.
		movss	xmm4, dword ptr [ebx+48]
		movss	xmm5, dword ptr [ebx+52]

		movlps	qword ptr [eax+24], xmm7 ; save 2nd
		movhps	qword ptr [eax+32], xmm7 ; row

		movss	xmm6, dword ptr [ebx+56]
		movss	xmm7, dword ptr [ebx+60]

		shufps	xmm4, xmm4, 0
		shufps	xmm5, xmm5, 0
		shufps	xmm6, xmm6, 0
		shufps	xmm7, xmm7, 0

		mulps	xmm4, xmm0
		mulps	xmm5, xmm1
		mulps	xmm6, xmm2
		mulps	xmm7, xmm3

		addps	xmm5, xmm4
		addps	xmm7, xmm6
		addps	xmm7, xmm5

		movaps	xmmword ptr [eax+48], xmm7

		//	Calculating first 4 elements in the fourth row of the destination matrix.
		movss	xmm4, dword ptr [ebx+72]
		movss	xmm5, dword ptr [ebx+76]
		movss	xmm6, dword ptr [ebx+80]
		movss	xmm7, dword ptr [ebx+84]

		shufps	xmm4, xmm4, 0
		shufps	xmm5, xmm5, 0
		shufps	xmm6, xmm6, 0
		shufps	xmm7, xmm7, 0

		mulps	xmm4, xmm0
		mulps	xmm5, xmm1
		mulps	xmm6, xmm2
		mulps	xmm7, xmm3

		addps	xmm4, xmm5
		addps	xmm6, xmm4
		addps	xmm7, xmm6

		movlps	qword ptr [eax+72], xmm7
		movhps	qword ptr [eax+80], xmm7

		//	Calculating first 4 elements in the fifth row of the destination matrix.
		movss	xmm4, dword ptr [ebx+96]
		movss	xmm5, dword ptr [ebx+100]
		movss	xmm6, dword ptr [ebx+104]
		movss	xmm7, dword ptr [ebx+108]

		shufps	xmm4, xmm4, 0
		shufps	xmm5, xmm5, 0
		shufps	xmm6, xmm6, 0
		shufps	xmm7, xmm7, 0

		mulps	xmm4, xmm0
		mulps	xmm5, xmm1
		mulps	xmm6, xmm2
		mulps	xmm7, xmm3

		addps	xmm5, xmm4
		addps	xmm7, xmm6
		addps	xmm7, xmm5

		movaps	xmmword ptr [eax+96], xmm7

		//	Calculating first 4 elements in the sixth row of the destination matrix.
		movss	xmm4, dword ptr [ebx+120]
		movss	xmm5, dword ptr [ebx+124]
		movss	xmm6, dword ptr [ebx+128]
		movss	xmm7, dword ptr [ebx+132]

		shufps	xmm4, xmm4, 0
		shufps	xmm5, xmm5, 0
		shufps	xmm6, xmm6, 0
		shufps	xmm7, xmm7, 0

		mulps	xmm4, xmm0
		mulps	xmm5, xmm1
		mulps	xmm6, xmm2
		mulps	xmm7, xmm3

		addps	xmm4, xmm5
		addps	xmm6, xmm4
		addps	xmm7, xmm6

		movhps	qword ptr [eax+128], xmm7
		movlps	qword ptr [eax+120], xmm7

		//	Loading first 4 columns (lower 2 rows) of src2.

		movlps	xmm0, qword ptr [ecx+96]
		movhps	xmm0, qword ptr [ecx+104]
		movlps	xmm1, qword ptr [ecx+120]
		movhps	xmm1, qword ptr [ecx+128]

		//	Calculating first 4 elements in the first row of the destination matrix.
		movss	xmm2, dword ptr [ebx+16]
		shufps	xmm2, xmm2, 0
		movss	xmm4, dword ptr [ebx+40]
		movss	xmm3, dword ptr [ebx+20]
		movss	xmm5, dword ptr [ebx+44]
		movaps	xmm6, xmmword ptr [eax]
		movlps	xmm7, qword ptr [eax+24]
		shufps	xmm3, xmm3, 0
		shufps	xmm5, xmm5, 0
		movhps	xmm7, qword ptr [eax+32]

		shufps	xmm4, xmm4, 0
		mulps	xmm5, xmm1

		mulps	xmm2, xmm0
		mulps	xmm3, xmm1
		mulps	xmm4, xmm0
		addps	xmm6, xmm2

		addps	xmm7, xmm4
		addps	xmm7, xmm5
		addps	xmm6, xmm3

		movlps	qword ptr [eax+24], xmm7
		movaps	xmmword ptr [eax], xmm6
		movhps	qword ptr [eax+32], xmm7

		//	Calculating first 4 elements in the third row of the destination matrix.
		movss	xmm2, dword ptr [ebx+64]
		movss	xmm4, dword ptr [ebx+88]
		movss	xmm5, dword ptr [ebx+92]
		movss	xmm3, dword ptr [ebx+68]
		movaps	xmm6, xmmword ptr [eax+48]
		movlps	xmm7, qword ptr [eax+72]
		movhps	xmm7, qword ptr [eax+80]

		shufps	xmm2, xmm2, 0
		shufps	xmm4, xmm4, 0
		shufps	xmm5, xmm5, 0
		shufps	xmm3, xmm3, 0

		mulps	xmm2, xmm0
		mulps	xmm4, xmm0
		mulps	xmm5, xmm1
		mulps	xmm3, xmm1

		addps	xmm6, xmm2
		addps	xmm6, xmm3
		addps	xmm7, xmm4
		addps	xmm7, xmm5

		movlps	qword ptr [eax+72], xmm7
		movaps	xmmword ptr [eax+48], xmm6
		movhps	qword ptr [eax+80], xmm7

		//	Calculating first 4 elements in the fifth row of the destination matrix.
		movss	xmm2, dword ptr [ebx+112]
		movss	xmm3, dword ptr [ebx+116]
		movaps	xmm6, xmmword ptr [eax+96]

		shufps	xmm2, xmm2, 0
		shufps	xmm3, xmm3, 0

		mulps	xmm2, xmm0
		mulps	xmm3, xmm1

		addps	xmm6, xmm2
		addps	xmm6, xmm3

		movaps	xmmword ptr [eax+96], xmm6

		//	Calculating first 4 elements in the sixth row of the destination matrix.
		movss	xmm4, dword ptr [ebx+136]
		movss	xmm5, dword ptr [ebx+140]
		movhps	xmm7, qword ptr [eax+128]
		movlps	xmm7, qword ptr [eax+120]

		shufps	xmm4, xmm4, 0
		shufps	xmm5, xmm5, 0

		mulps	xmm4, xmm0
		mulps	xmm5, xmm1

		addps	xmm7, xmm4
		addps	xmm7, xmm5

		//	Calculating last 2 columns of the destination matrix.

		movlps	xmm0, qword ptr [ecx+16]
		movhps	xmm0, qword ptr [ecx+40]

		movhps	qword ptr [eax+128], xmm7
		movlps	qword ptr [eax+120], xmm7

		movlps	xmm2, qword ptr [ecx+64]
		movhps	xmm2, qword ptr [ecx+88]

		movaps	xmm3, xmm2
		shufps	xmm3, xmm3, 4Eh

		movlps	xmm4, qword ptr [ecx+112]
		movhps	xmm4, qword ptr [ecx+136]

		movaps	xmm5, xmm4
		shufps	xmm5, xmm5, 4Eh

		movlps	xmm6, qword ptr [ebx]
		movhps	xmm6, qword ptr [ebx+24]

		movaps	xmm7, xmm6
		shufps	xmm7, xmm7, 0F0h

		mulps	xmm7, xmm0

		shufps	xmm6, xmm6, 0A5h
		movaps	xmm1, xmm0
		shufps	xmm1, xmm1, 4Eh
		mulps	xmm1, xmm6
		addps	xmm7, xmm1

		movlps	xmm6, qword ptr [ebx+8]
		movhps	xmm6, qword ptr [ebx+32]

		movaps	xmm1, xmm6
		shufps	xmm1, xmm1, 0F0h
		shufps	xmm6, xmm6, 0A5h

		mulps	xmm1, xmm2
		mulps	xmm6, xmm3
		addps	xmm7, xmm1
		addps	xmm7, xmm6

		movhps	xmm6, qword ptr [ebx+40]
		movlps	xmm6, qword ptr [ebx+16]

		movaps	xmm1, xmm6
		shufps	xmm1, xmm1, 0F0h
		shufps	xmm6, xmm6, 0A5h

		mulps	xmm1, xmm4
		mulps	xmm6, xmm5
		addps	xmm7, xmm1
		addps	xmm7, xmm6

		movlps	qword ptr [eax+16], xmm7
		movhps	qword ptr [eax+40], xmm7

		movlps	xmm6, qword ptr [ebx+48]
		movhps	xmm6, qword ptr [ebx+72]

		movaps	xmm7, xmm6
		shufps	xmm7, xmm7, 0F0h

		mulps	xmm7, xmm0

		shufps	xmm6, xmm6, 0A5h
		movaps	xmm1, xmm0
		shufps	xmm1, xmm1, 4Eh
		mulps	xmm1, xmm6
		addps	xmm7, xmm1

		movhps	xmm6, qword ptr [ebx+80]
		movlps	xmm6, qword ptr [ebx+56]

		movaps	xmm1, xmm6
		shufps	xmm1, xmm1, 0F0h
		shufps	xmm6, xmm6, 0A5h

		mulps	xmm1, xmm2
		mulps	xmm6, xmm3
		addps	xmm7, xmm1
		addps	xmm7, xmm6

		movlps	xmm6, qword ptr [ebx+64]
		movhps	xmm6, qword ptr [ebx+88]

		movaps	xmm1, xmm6
		shufps	xmm1, xmm1, 0F0h
		shufps	xmm6, xmm6, 0A5h

		mulps	xmm1, xmm4
		mulps	xmm6, xmm5
		addps	xmm7, xmm1
		addps	xmm7, xmm6

		movlps	qword ptr [eax+64], xmm7
		movhps	qword ptr [eax+88], xmm7

		movlps	xmm6, qword ptr [ebx+96]
		movhps	xmm6, qword ptr [ebx+120]

		movaps	xmm7, xmm6
		shufps	xmm7, xmm7, 0F0h

		mulps	xmm7, xmm0

		shufps	xmm6, xmm6, 0A5h
		movaps	xmm1, xmm0
		shufps	xmm1, xmm1, 4Eh
		mulps	xmm1, xmm6
		addps	xmm7, xmm1

		movlps	xmm6, qword ptr [ebx+104]
		movhps	xmm6, qword ptr [ebx+128]

		movaps	xmm1, xmm6
		shufps	xmm1, xmm1, 0F0h
		shufps	xmm6, xmm6, 0A5h

		mulps	xmm1, xmm2
		mulps	xmm6, xmm3
		addps	xmm7, xmm1
		addps	xmm7, xmm6

		movlps	xmm6, qword ptr [ebx+112]
		movhps	xmm6, qword ptr [ebx+136]

		movaps	xmm1, xmm6
		shufps	xmm1, xmm1, 0F0h
		shufps	xmm6, xmm6, 0A5h

		mulps	xmm1, xmm4
		mulps	xmm6, xmm5
		addps	xmm7, xmm1
		addps	xmm7, xmm6

		movlps	qword ptr [eax+112], xmm7
		movhps	qword ptr [eax+136], xmm7

	}

	epilogue

} // Mult00_6x6_6x6

SMLXTmpMatrix& __fastcall Mult01_6x6_6x6(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading first 4 rows of src2 and transposing.

	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src2[ 0] ), (__m64*)&src2[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[12] ), (__m64*)&src2[18] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src2[ 2] ), (__m64*)&src2[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[14] ), (__m64*)&src2[20] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src2[ 4] ), (__m64*)&src2[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[16] ), (__m64*)&src2[22] );
	b4		= _mm_shuffle_ps( b5, tmp, 0x88 );
	b5		= _mm_shuffle_ps( b5, tmp, 0xDD );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating first 4 elements in the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[30] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[31] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[32] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[33] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[34] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[35] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[30], rslt );
	_mm_storeh_pi( (__m64*)&dst[32], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src2[24] ), (__m64*)&src2[30] );
	b1		= _mm_shuffle_ps( b0 , b0 , 0xDD );
	b0		= _mm_shuffle_ps( b0 , b0 , 0x88 );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2 , (__m64*)&src2[26] ), (__m64*)&src2[32] );
	b3		= _mm_shuffle_ps( b2 , b2 , 0xDD );
	b2		= _mm_shuffle_ps( b2 , b2 , 0x88 );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4 , (__m64*)&src2[28] ), (__m64*)&src2[34] );
	b5		= _mm_shuffle_ps( b4 , b4 , 0xDD );
	b4		= _mm_shuffle_ps( b4 , b4 , 0x88 );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( row, row, 0xA0 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( row, row, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );	

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 2] ), (__m64*)&src1[ 8] );
	tmp		= row;
	row		= _mm_shuffle_ps( row, row, 0xA0 );

	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	tmp		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	tmp		= _mm_shuffle_ps( row, row, 0xF5 );
	row		= _mm_shuffle_ps( row, row, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[12] ), (__m64*)&src1[18] );
	tmp		= _mm_shuffle_ps( row, row, 0xF5 );
	row		= _mm_shuffle_ps( row, row, 0xA0 );

	rslt	= _mm_add_ps( _mm_mul_ps( row, b0 ), _mm_mul_ps( tmp, b1 ) );	

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[14] ), (__m64*)&src1[20] );
	tmp		= _mm_shuffle_ps( row, row, 0xF5 );
	row		= _mm_shuffle_ps( row, row, 0xA0 );	
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );
	
	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[16] ), (__m64*)&src1[22] );
	tmp		= _mm_shuffle_ps( row, row, 0xF5 );
	row		= _mm_shuffle_ps( row, row, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[24] ), (__m64*)&src1[30] );
	tmp		= _mm_shuffle_ps( row, row, 0xF5 );
	row		= _mm_shuffle_ps( row, row, 0xA0 );
	rslt	= _mm_add_ps( _mm_mul_ps( row, b0 ), _mm_mul_ps( tmp, b1 ) );	

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[26] ), (__m64*)&src1[32] );
	tmp		= _mm_shuffle_ps( row, row, 0xF5 );
	row		= _mm_shuffle_ps( row, row, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[28] ), (__m64*)&src1[34] );
	tmp		= _mm_shuffle_ps( row, row, 0xF5 );
	row		= _mm_shuffle_ps( row, row, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );
	_mm_storeh_pi( (__m64*)&dst[34], rslt );

	return dmat;

} // Mult01_6x6_6x6

SMLXTmpMatrix& __fastcall Mult10_6x6_6x6(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading first 4 columns of src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[12] ), (__m64*)&src2[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[18] ), (__m64*)&src2[20] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[24] ), (__m64*)&src2[26] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[30] ), (__m64*)&src2[32] );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[30] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[31] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[32] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[33] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[34] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating first 4 elements in the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[35] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[30], rslt );
	_mm_storeh_pi( (__m64*)&dst[32], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src2[ 4] ), (__m64*)&src2[10] );
	b1		= _mm_shuffle_ps( b0 , b0 , 0xEE );
	b0		= _mm_shuffle_ps( b0 , b0 , 0x44 );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2 , (__m64*)&src2[16] ), (__m64*)&src2[22] );
	b3		= _mm_shuffle_ps( b2 , b2 , 0xEE );
	b2		= _mm_shuffle_ps( b2 , b2 , 0x44 );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4 , (__m64*)&src2[28] ), (__m64*)&src2[34] );
	b5		= _mm_shuffle_ps( b4 , b4 , 0xEE );
	b4		= _mm_shuffle_ps( b4 , b4 , 0x44 );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( row, row, 0x50 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( row, row, 0xFA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );	

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[12] ), (__m64*)&src1[18] );
	tmp		= row;
	row		= _mm_shuffle_ps( row, row, 0x50 );

	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	tmp		= _mm_shuffle_ps( tmp, tmp, 0xFA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[24] ), (__m64*)&src1[30] );
	tmp		= _mm_shuffle_ps( row, row, 0xFA );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 2] ), (__m64*)&src1[ 8] );
	tmp		= _mm_shuffle_ps( row, row, 0xFA );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	
	rslt	= _mm_add_ps( _mm_mul_ps( row, b0 ), _mm_mul_ps( tmp, b1 ) );	

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[14] ), (__m64*)&src1[20] );
	tmp		= _mm_shuffle_ps( row, row, 0xFA );
	row		= _mm_shuffle_ps( row, row, 0x50 );	
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );
	
	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[26] ), (__m64*)&src1[32] );
	tmp		= _mm_shuffle_ps( row, row, 0xFA );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	tmp		= _mm_shuffle_ps( row, row, 0xFA );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( _mm_mul_ps( row, b0 ), _mm_mul_ps( tmp, b1 ) );	

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[16] ), (__m64*)&src1[22] );
	tmp		= _mm_shuffle_ps( row, row, 0xFA );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[28] ), (__m64*)&src1[34] );
	tmp		= _mm_shuffle_ps( row, row, 0xFA );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );
	_mm_storeh_pi( (__m64*)&dst[34], rslt );

	return dmat;

} // Mult10_6x6_6x6

SMLXTmpMatrix& __fastcall Mult11_6x6_6x6(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading first 4 rows of src2 and transposing.

	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src2[ 0] ), (__m64*)&src2[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[12] ), (__m64*)&src2[18] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src2[ 2] ), (__m64*)&src2[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[14] ), (__m64*)&src2[20] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src2[ 4] ), (__m64*)&src2[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[16] ), (__m64*)&src2[22] );
	b4		= _mm_shuffle_ps( b5, tmp, 0x88 );
	b5		= _mm_shuffle_ps( b5, tmp, 0xDD );

		//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[30] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[31] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[32] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[33] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[34] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating first 4 elements in the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[35] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[30], rslt );
	_mm_storeh_pi( (__m64*)&dst[32], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src2[24] ), (__m64*)&src2[30] );
	b1		= _mm_shuffle_ps( b0 , b0 , 0xDD );
	b0		= _mm_shuffle_ps( b0 , b0 , 0x88 );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2 , (__m64*)&src2[26] ), (__m64*)&src2[32] );
	b3		= _mm_shuffle_ps( b2 , b2 , 0xDD );
	b2		= _mm_shuffle_ps( b2 , b2 , 0x88 );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4 , (__m64*)&src2[28] ), (__m64*)&src2[34] );
	b5		= _mm_shuffle_ps( b4 , b4 , 0xDD );
	b4		= _mm_shuffle_ps( b4 , b4 , 0x88 );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( row, row, 0xFA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );	

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[12] ), (__m64*)&src1[18] );
	tmp		= row;
	row		= _mm_shuffle_ps( row, row, 0x50 );

	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	tmp		= _mm_shuffle_ps( tmp, tmp, 0xFA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[24] ), (__m64*)&src1[30] );
	tmp		= _mm_shuffle_ps( row, row, 0xFA );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 2] ), (__m64*)&src1[ 8] );
	tmp		= _mm_shuffle_ps( row, row, 0xFA );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	
	rslt	= _mm_add_ps( _mm_mul_ps( row, b0 ), _mm_mul_ps( tmp, b1 ) );	

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[14] ), (__m64*)&src1[20] );
	tmp		= _mm_shuffle_ps( row, row, 0xFA );
	row		= _mm_shuffle_ps( row, row, 0x50 );	
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );
	
	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[26] ), (__m64*)&src1[32] );
	tmp		= _mm_shuffle_ps( row, row, 0xFA );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	tmp		= _mm_shuffle_ps( row, row, 0xFA );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( _mm_mul_ps( row, b0 ), _mm_mul_ps( tmp, b1 ) );	

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[16] ), (__m64*)&src1[22] );
	tmp		= _mm_shuffle_ps( row, row, 0xFA );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[28] ), (__m64*)&src1[34] );
	tmp		= _mm_shuffle_ps( row, row, 0xFA );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );
	_mm_storeh_pi( (__m64*)&dst[34], rslt );

	return dmat;

} // Mult11_6x6_6x6

SMLXTmpMatrix& __fastcall Mult00_6x6_6x3(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src2[ 3] ), (__m64*)&src2[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src2[ 6] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src2[ 9] ), (__m64*)&src2[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[12] ), (__m64*)&src2[13] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src2[15] ), (__m64*)&src2[16] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[30] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[31] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[32] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[33] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[34] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[35] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[16], rslt );	

	return dmat;

} // Mult00_6x6_6x3

SMLXTmpMatrix& __fastcall Mult01_6x6_3x6(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading the src2 and transposing.

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src2[ 6] ), (__m64*)&src2[12] );
	b0		= _mm_shuffle_ps( tmp, b1, 0x80 );
	b1		= _mm_shuffle_ps( tmp, b1, 0xD5 );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src2[ 8] ), (__m64*)&src2[14] );
	b2		= _mm_shuffle_ps( tmp, b3, 0x8A );
	b3		= _mm_shuffle_ps( tmp, b3, 0xDF );

	tmp		= _mm_loadl_pi( tmp, (__m64*)&src2[4] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src2[10] ), (__m64*)&src2[16] );
	b4		= _mm_shuffle_ps( tmp, b5, 0x80 );
	b5		= _mm_shuffle_ps( tmp, b5, 0xD5 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[ 9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[30] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[31] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[32] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[33] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[34] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[35] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult01_6x6_3x6

SMLXTmpMatrix& __fastcall Mult10_6x6_6x3(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading first 4 columns of src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src2[ 3] ), (__m64*)&src2[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src2[ 6] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src2[ 9] ), (__m64*)&src2[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[12] ), (__m64*)&src2[13] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src2[15] ), (__m64*)&src2[16] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[30] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[31] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[32] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[33] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[34] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[35] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[16], rslt );
	
	return dmat;

} // Mult10_6x6_6x3

SMLXTmpMatrix& __fastcall Mult11_6x6_3x6(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading first 4 rows of src2 and transposing.

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src2[ 6] ), (__m64*)&src2[12] );
	b0		= _mm_shuffle_ps( tmp, b1, 0x80 );
	b1		= _mm_shuffle_ps( tmp, b1, 0xD5 );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src2[ 8] ), (__m64*)&src2[14] );
	b2		= _mm_shuffle_ps( tmp, b3, 0x8A );
	b3		= _mm_shuffle_ps( tmp, b3, 0xDF );

	tmp		= _mm_loadl_pi( tmp, (__m64*)&src2[4] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src2[10] ), (__m64*)&src2[16] );
	b4		= _mm_shuffle_ps( tmp, b5, 0x80 );
	b5		= _mm_shuffle_ps( tmp, b5, 0xD5 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[30] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[31] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[32] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[33] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[34] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[35] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult11_6x6_3x6

SMLXTmpMatrix& __fastcall Mult00_6x6_6x5(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[10] ), (__m64*)&src2[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[15] ), (__m64*)&src2[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[20] ), (__m64*)&src2[22] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[25] ), (__m64*)&src2[27] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[1] * src2[9];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[2] * src2[14];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[3] * src2[19];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[4] * src2[24];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[5] * src2[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;
	
	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[6] * src2[4];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[9];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[8] * src2[14];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[9] * src2[19];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[10] * src2[24];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[11] * src2[29];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[12] * src2[4];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[13] * src2[9];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[14];

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[15] * src2[19];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[16] * src2[24];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[17] * src2[29];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[18] * src2[4];

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[19] * src2[9];

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[20] * src2[14];

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[21] * src2[19];

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[22] * src2[24];

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[23] * src2[29];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[24] * src2[4];

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[25] * src2[9];

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[26] * src2[14];

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[27] * src2[19];

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[28] * src2[24];

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[29] * src2[29];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[30] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[30] * src2[4];

	row		= _mm_load_ss( &src1[31] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[31] * src2[9];

	row		= _mm_load_ss( &src1[32] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[32] * src2[14];

	row		= _mm_load_ss( &src1[33] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[33] * src2[19];

	row		= _mm_load_ss( &src1[34] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[34] * src2[24];

	row		= _mm_load_ss( &src1[35] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[35] * src2[29];

	_mm_storel_pi( (__m64*)&dst[25], rslt );
	_mm_storeh_pi( (__m64*)&dst[27], rslt );	
	dst[29]	= f;

	return dmat;

} // Mult00_6x6_6x5

SMLXTmpMatrix& __fastcall Mult01_6x6_5x6(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	float	f;

	//	Loading first 4 rows of the src2 and transposing.

	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src2[ 0] ), (__m64*)&src2[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[12] ), (__m64*)&src2[18] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src2[ 2] ), (__m64*)&src2[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[14] ), (__m64*)&src2[20] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src2[ 4] ), (__m64*)&src2[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[16] ), (__m64*)&src2[22] );
	b4		= _mm_shuffle_ps( b5, tmp, 0x88 );
	b5		= _mm_shuffle_ps( b5, tmp, 0xDD );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[24];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[1] * src2[25];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[2] * src2[26];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[3] * src2[27];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[4] * src2[28];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[5] * src2[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;
	
	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[6] * src2[24];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[25];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[8] * src2[26];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[9] * src2[27];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[10] * src2[28];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[11] * src2[29];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[12] * src2[24];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[13] * src2[25];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[26];

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[15] * src2[27];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[16] * src2[28];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[17] * src2[29];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[18] * src2[24];

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[19] * src2[25];

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[20] * src2[26];

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[21] * src2[27];

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[22] * src2[28];

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[23] * src2[29];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[24] * src2[24];

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[25] * src2[25];

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[26] * src2[26];

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[27] * src2[27];

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[28] * src2[28];

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[29] * src2[29];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[30] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[30] * src2[24];

	row		= _mm_load_ss( &src1[31] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[31] * src2[25];

	row		= _mm_load_ss( &src1[32] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[32] * src2[26];

	row		= _mm_load_ss( &src1[33] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[33] * src2[27];

	row		= _mm_load_ss( &src1[34] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[34] * src2[28];

	row		= _mm_load_ss( &src1[35] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[35] * src2[29];

	_mm_storel_pi( (__m64*)&dst[25], rslt );
	_mm_storeh_pi( (__m64*)&dst[27], rslt );	
	dst[29]	= f;

	return dmat;

} // Mult01_6x6_5x6

SMLXTmpMatrix& __fastcall Mult10_6x6_6x5(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[10] ), (__m64*)&src2[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[15] ), (__m64*)&src2[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[20] ), (__m64*)&src2[22] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[25] ), (__m64*)&src2[27] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[6] * src2[9];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[12] * src2[14];

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[18] * src2[19];

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[24] * src2[24];

	row		= _mm_load_ss( &src1[30] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[30] * src2[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[1] * src2[4];
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[9];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[13] * src2[14];

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[19] * src2[19];

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[25] * src2[24];

	row		= _mm_load_ss( &src1[31] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[31] * src2[29];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[2] * src2[4];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[8] * src2[9];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[14];

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[20] * src2[19];

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[26] * src2[24];

	row		= _mm_load_ss( &src1[32] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[32] * src2[29];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[3] * src2[4];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[9] * src2[9];

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[15] * src2[14];

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[21] * src2[19];

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[27] * src2[24];

	row		= _mm_load_ss( &src1[33] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[33] * src2[29];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[4] * src2[4];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[10] * src2[9];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[16] * src2[14];

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[22] * src2[19];

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[28] * src2[24];

	row		= _mm_load_ss( &src1[34] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[34] * src2[29];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[5] * src2[4];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[11] * src2[9];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[17] * src2[14];

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[23] * src2[19];

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[29] * src2[24];

	row		= _mm_load_ss( &src1[35] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[35] * src2[29];

	_mm_storel_pi( (__m64*)&dst[25], rslt );
	_mm_storeh_pi( (__m64*)&dst[27], rslt );
	dst[29]	= f;
	
	return dmat;

} // Mult10_6x6_6x5

SMLXTmpMatrix& __fastcall Mult11_6x6_5x6(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	float	f;

	//	Loading first 4 rows of the src2 and transposing.

	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src2[ 0] ), (__m64*)&src2[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[12] ), (__m64*)&src2[18] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src2[ 2] ), (__m64*)&src2[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[14] ), (__m64*)&src2[20] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src2[ 4] ), (__m64*)&src2[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[16] ), (__m64*)&src2[22] );
	b4		= _mm_shuffle_ps( b5, tmp, 0x88 );
	b5		= _mm_shuffle_ps( b5, tmp, 0xDD );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[24];

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[6] * src2[25];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[12] * src2[26];

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[18] * src2[27];

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[24] * src2[28];

	row		= _mm_load_ss( &src1[30] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[30] * src2[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[1] * src2[24];
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[25];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[13] * src2[26];

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[19] * src2[27];

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[25] * src2[28];

	row		= _mm_load_ss( &src1[31] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[31] * src2[29];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[2] * src2[24];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[8] * src2[25];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[26];

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[20] * src2[27];

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[26] * src2[28];

	row		= _mm_load_ss( &src1[32] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[32] * src2[29];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[3] * src2[24];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[9] * src2[25];

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[15] * src2[26];

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[21] * src2[27];

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[27] * src2[28];

	row		= _mm_load_ss( &src1[33] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[33] * src2[29];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[4] * src2[24];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[10] * src2[25];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[16] * src2[26];

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[22] * src2[27];

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[28] * src2[28];

	row		= _mm_load_ss( &src1[34] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[34] * src2[29];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[5] * src2[24];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[11] * src2[25];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[17] * src2[26];

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[23] * src2[27];

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[29] * src2[28];

	row		= _mm_load_ss( &src1[35] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[35] * src2[29];

	_mm_storel_pi( (__m64*)&dst[25], rslt );
	_mm_storeh_pi( (__m64*)&dst[27], rslt );
	dst[29]	= f;

	return dmat;

} // Mult11_6x6_5x6

SMLXTmpMatrix& __fastcall Mult00_3x6_6x6(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[12] ), (__m64*)&src2[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[18] ), (__m64*)&src2[20] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[24] ), (__m64*)&src2[26] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[30] ), (__m64*)&src2[32] );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src2[ 4] ), (__m64*)&src2[10] );
	b1		= _mm_shuffle_ps( b0 , b0 , 0x4E );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2 , (__m64*)&src2[16] ), (__m64*)&src2[22] );
	b3		= _mm_shuffle_ps( b2 , b2 , 0x4E );	
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4 , (__m64*)&src2[28] ), (__m64*)&src2[34] );
	b5		= _mm_shuffle_ps( b4 , b4 , 0x4E );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( row, row, 0xF0 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( row, row, 0xA5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );	

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 2] ), (__m64*)&src1[ 8] );
	tmp		= row;
	row		= _mm_shuffle_ps( row, row, 0xF0 );

	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	tmp		= _mm_shuffle_ps( tmp, tmp, 0xA5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	tmp		= _mm_shuffle_ps( row, row, 0xA5 );
	row		= _mm_shuffle_ps( row, row, 0xF0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );
	
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[14] );

	row		= _mm_shuffle_ps( tmp, tmp, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_shuffle_ps( tmp, tmp, 0x55 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0xAA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0xFF );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	tmp		= _mm_loadl_pi( tmp, (__m64*)&src1[16] );
	row		= _mm_shuffle_ps( tmp, tmp, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0x55 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult00_3x6_6x6

SMLXTmpMatrix& __fastcall Mult01_3x6_6x6(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading first 4 rows of the src2 and transposing.

	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src2[ 0] ), (__m64*)&src2[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[12] ), (__m64*)&src2[18] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src2[ 2] ), (__m64*)&src2[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[14] ), (__m64*)&src2[20] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src2[ 4] ), (__m64*)&src2[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[16] ), (__m64*)&src2[22] );
	b4		= _mm_shuffle_ps( b5, tmp, 0x88 );
	b5		= _mm_shuffle_ps( b5, tmp, 0xDD );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src2[24] ), (__m64*)&src2[30] );
	b1		= _mm_shuffle_ps( b0 , b0 , 0xDD );
	b0		= _mm_shuffle_ps( b0 , b0 , 0x88 );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2 , (__m64*)&src2[26] ), (__m64*)&src2[32] );
	b3		= _mm_shuffle_ps( b2 , b2 , 0xDD );
	b2		= _mm_shuffle_ps( b2 , b2 , 0x88 );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4 , (__m64*)&src2[28] ), (__m64*)&src2[34] );
	b5		= _mm_shuffle_ps( b4 , b4 , 0xDD );
	b4		= _mm_shuffle_ps( b4 , b4 , 0x88 );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( row, row, 0xA0 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( row, row, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );	

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 2] ), (__m64*)&src1[ 8] );
	tmp		= row;
	row		= _mm_shuffle_ps( row, row, 0xA0 );

	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	tmp		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	tmp		= _mm_shuffle_ps( row, row, 0xF5 );
	row		= _mm_shuffle_ps( row, row, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );
	
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[14] );

	row		= _mm_shuffle_ps( tmp, tmp, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_shuffle_ps( tmp, tmp, 0x55 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0xAA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0xFF );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	tmp		= _mm_loadl_pi( tmp, (__m64*)&src1[16] );

	row		= _mm_shuffle_ps( tmp, tmp, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0x55 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult01_3x6_6x6

SMLXTmpMatrix& __fastcall Mult10_6x3_6x6(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[12] ), (__m64*)&src2[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[18] ), (__m64*)&src2[20] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[24] ), (__m64*)&src2[26] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[30] ), (__m64*)&src2[32] );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadl_pi( b0 , (__m64*)&src2[ 4] );
	b1		= _mm_loadl_pi( b1 , (__m64*)&src2[10] );
	b2		= _mm_loadl_pi( b2 , (__m64*)&src2[16] );
	b3		= _mm_loadl_pi( b3 , (__m64*)&src2[22] );
	b4		= _mm_loadl_pi( b4 , (__m64*)&src2[28] );
	b5		= _mm_loadl_pi( b5 , (__m64*)&src2[34] );

	row		= _mm_load_ss( &src1[ 0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[ 3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[ 6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[ 9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[4], rslt );
	
	row		= _mm_load_ss( &src1[ 1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[ 4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[ 7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[10], rslt );

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[ 5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[ 8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult10_6x3_6x6

SMLXTmpMatrix& __fastcall Mult11_6x3_6x6(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading first 4 rows of the src2 and transposing.

	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src2[ 0] ), (__m64*)&src2[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[12] ), (__m64*)&src2[18] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src2[ 2] ), (__m64*)&src2[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[14] ), (__m64*)&src2[20] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src2[ 4] ), (__m64*)&src2[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[16] ), (__m64*)&src2[22] );
	b4		= _mm_shuffle_ps( b5, tmp, 0x88 );
	b5		= _mm_shuffle_ps( b5, tmp, 0xDD );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src2[24] ), (__m64*)&src2[30] );
	b1		= _mm_shuffle_ps( b0 , b0 , 0xDD );
	b0		= _mm_shuffle_ps( b0 , b0 , 0x88 );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2 , (__m64*)&src2[26] ), (__m64*)&src2[32] );
	b3		= _mm_shuffle_ps( b2 , b2 , 0xDD );
	b2		= _mm_shuffle_ps( b2 , b2 , 0x88 );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4 , (__m64*)&src2[28] ), (__m64*)&src2[34] );
	b5		= _mm_shuffle_ps( b4 , b4 , 0xDD );
	b4		= _mm_shuffle_ps( b4 , b4 , 0x88 );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( row, row, 0xFA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 3] ), (__m64*)&src1[ 9] );
	tmp		= row;
	row		= _mm_shuffle_ps( row, row, 0x50 );

	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	tmp		= _mm_shuffle_ps( tmp, tmp, 0xFA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[12] ), (__m64*)&src1[15] );
	tmp		= _mm_shuffle_ps( row, row, 0xFA );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[ 5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[ 8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult11_6x3_6x6

SMLXTmpMatrix& __fastcall Mult00_6x3_3x6(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt, tmp1, tmp2;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[12] ), (__m64*)&src2[14] );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating first 4 elements in the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[30], rslt );
	_mm_storeh_pi( (__m64*)&dst[32], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadl_pi( b0 , (__m64*)&src2[ 4] );
	b0		= _mm_shuffle_ps( b0 , b0 , 0x44 );
	b1		= _mm_loadl_pi( b1 , (__m64*)&src2[10] );
	b1		= _mm_shuffle_ps( b1 , b1 , 0x44 );
	b2		= _mm_loadl_pi( b2 , (__m64*)&src2[16] );
	b2		= _mm_shuffle_ps( b2 , b2 , 0x44 );

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	row		= _mm_shuffle_ps( tmp1, tmp1, 0xF0 );
	rslt	= _mm_mul_ps( row, b0 );

	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src1[ 4] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( tmp1, tmp2, 0x05 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_shuffle_ps( tmp1, tmp2, 0x5A );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src1[ 8] ), (__m64*)&src1[10] );

	row		= _mm_shuffle_ps( tmp2, tmp1, 0x5A );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_shuffle_ps( tmp2, tmp1, 0xAF );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_shuffle_ps( tmp1, tmp1, 0xF0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src1[12] ), (__m64*)&src1[14] );

	row		= _mm_shuffle_ps( tmp2, tmp2, 0xF0 );
	rslt	= _mm_mul_ps( row, b0 );

	tmp1	= _mm_loadl_pi( tmp1, (__m64*)&src1[16] );

	row		= _mm_shuffle_ps( tmp2, tmp1, 0x05 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_shuffle_ps( tmp2, tmp1, 0x5A );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );
	_mm_storeh_pi( (__m64*)&dst[34], rslt );

	return dmat;

} // Mult00_6x3_3x6

SMLXTmpMatrix& __fastcall Mult01_6x3_6x3(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt, tmp1, tmp2, tmp3;

	//	Loading first 2 rows of the src2 and mixing.

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	tmp3	= _mm_loadh_pi( _mm_loadl_pi( tmp3, (__m64*)&src2[ 4] ), (__m64*)&src2[10] );

	b0		= _mm_shuffle_ps( tmp1, tmp2, 0xCC );
	b1		= _mm_shuffle_ps( tmp1, tmp3, 0x49 );
	tmp1	= _mm_shuffle_ps( tmp2, tmp3, 0xE9 );
	b2		= _mm_shuffle_ps( b1  , tmp1, 0xDD );
	b1		= _mm_shuffle_ps( b1  , tmp1, 0x88 );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating first 4 elements in the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[30], rslt );
	_mm_storeh_pi( (__m64*)&dst[32], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[12] ), (__m64*)&src2[14] );
	tmp1	= _mm_loadl_pi( tmp1, (__m64*)&src2[16] );

	b2		= _mm_shuffle_ps( b0, tmp1, 0x49 );
	b0		= _mm_shuffle_ps( b0, b0, 0xCC );

	b1		= _mm_shuffle_ps( b2, b2, 0x88 );
	b2		= _mm_shuffle_ps( b2, b2, 0xDD );

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	row		= _mm_shuffle_ps( tmp1, tmp1, 0xF0 );
	rslt	= _mm_mul_ps( row, b0 );

	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src1[ 4] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( tmp1, tmp2, 0x05 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_shuffle_ps( tmp1, tmp2, 0x5A );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src1[ 8] ), (__m64*)&src1[10] );

	row		= _mm_shuffle_ps( tmp2, tmp1, 0x5A );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_shuffle_ps( tmp2, tmp1, 0xAF );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_shuffle_ps( tmp1, tmp1, 0xF0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src1[12] ), (__m64*)&src1[14] );

	row		= _mm_shuffle_ps( tmp2, tmp2, 0xF0 );
	rslt	= _mm_mul_ps( row, b0 );

	tmp1	= _mm_loadl_pi( tmp1, (__m64*)&src1[16] );

	row		= _mm_shuffle_ps( tmp2, tmp1, 0x05 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_shuffle_ps( tmp2, tmp1, 0x5A );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );
	_mm_storeh_pi( (__m64*)&dst[34], rslt );

	return dmat;

} // Mult01_6x3_6x3

SMLXTmpMatrix& __fastcall Mult10_3x6_3x6(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[12] ), (__m64*)&src2[14] );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[ 3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[ 9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[ 4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating first 4 elements in the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[ 5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[30], rslt );
	_mm_storeh_pi( (__m64*)&dst[32], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadl_pi( b0 , (__m64*)&src2[ 4] );
	b0		= _mm_shuffle_ps( b0 , b0 , 0x44 );
	b1		= _mm_loadl_pi( b1 , (__m64*)&src2[10] );
	b1		= _mm_shuffle_ps( b1 , b1 , 0x44 );
	b2		= _mm_loadl_pi( b2 , (__m64*)&src2[16] );
	b2		= _mm_shuffle_ps( b2 , b2 , 0x44 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 0] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 8] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 4] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );
	_mm_storeh_pi( (__m64*)&dst[34], rslt );

	return dmat;

} // Mult10_3x6_3x6

SMLXTmpMatrix& __fastcall Mult11_3x6_6x3(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt, tmp1, tmp2, tmp3;

	//	Loading first 2 rows of the src2 and mixing.

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	tmp3	= _mm_loadh_pi( _mm_loadl_pi( tmp3, (__m64*)&src2[ 4] ), (__m64*)&src2[10] );

	b0		= _mm_shuffle_ps( tmp1, tmp2, 0xCC );
	b1		= _mm_shuffle_ps( tmp1, tmp3, 0x49 );
	tmp1	= _mm_shuffle_ps( tmp2, tmp3, 0xE9 );
	b2		= _mm_shuffle_ps( b1  , tmp1, 0xDD );
	b1		= _mm_shuffle_ps( b1  , tmp1, 0x88 );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[ 3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[ 9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[ 4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating first 4 elements in the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[ 5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[30], rslt );
	_mm_storeh_pi( (__m64*)&dst[32], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[12] ), (__m64*)&src2[14] );
	tmp1	= _mm_loadl_pi( tmp1, (__m64*)&src2[16] );

	b2		= _mm_shuffle_ps( b0, tmp1, 0x49 );
	b0		= _mm_shuffle_ps( b0, b0, 0xCC );

	b1		= _mm_shuffle_ps( b2, b2, 0x88 );
	b2		= _mm_shuffle_ps( b2, b2, 0xDD );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 0] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 8] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 4] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );
	_mm_storeh_pi( (__m64*)&dst[34], rslt );

	return dmat;

} // Mult11_3x6_6x3

_declspec(naked)
SMLXTmpMatrix& __fastcall Mult00_6x6_6x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,6,1)();

	prologue
	
	_asm {
		
		movlps	xmm7, qword ptr [ecx]
		movlps	xmm6, qword ptr [ecx+8]

		shufps	xmm7, xmm7, 0x44
		shufps	xmm6, xmm6, 0x44
		movlps	xmm0, qword ptr [ebx    ]
		movhps	xmm0, qword ptr [ebx+ 24]

		mulps	xmm0, xmm7
		movlps	xmm3, qword ptr [ebx+  8]
		movhps	xmm3, qword ptr [ebx+ 32]

		mulps	xmm3, xmm6
		movlps	xmm1, qword ptr [ebx+ 48]
		movhps	xmm1, qword ptr [ebx+ 72]

		mulps	xmm1, xmm7
		movlps	xmm2, qword ptr [ebx+ 96]
		movhps	xmm2, qword ptr [ebx+120]

		mulps	xmm2, xmm7
		movlps	xmm4, qword ptr [ebx+ 56]
		movhps	xmm4, qword ptr [ebx+ 80]

		movlps	xmm5, qword ptr [ebx+104]
		movhps	xmm5, qword ptr [ebx+128]

		mulps	xmm4, xmm6
		movlps	xmm7, qword ptr [ecx+16]
		addps	xmm0, xmm3
		shufps	xmm7, xmm7, 0x44
		mulps	xmm5, xmm6

		addps	xmm1, xmm4
		movlps	xmm3, qword ptr [ebx+ 16]
		movhps	xmm3, qword ptr [ebx+ 40]
		
		addps	xmm2, xmm5
		movlps	xmm4, qword ptr [ebx+ 64]
		movhps	xmm4, qword ptr [ebx+ 88]

		mulps	xmm3, xmm7
		movlps	xmm5, qword ptr [ebx+112]
		movhps	xmm5, qword ptr [ebx+136]

		addps	xmm0, xmm3
		mulps	xmm4, xmm7
		mulps	xmm5, xmm7

		addps	xmm1, xmm4
		addps	xmm2, xmm5

		movaps	xmm6, xmm0
		shufps	xmm0, xmm1, 0x88
		shufps	xmm6, xmm1, 0xDD

		movaps	xmm7, xmm2
		shufps	xmm7, xmm2, 0x88
		shufps	xmm2, xmm2, 0xDD

		addps	xmm0, xmm6
		addps	xmm2, xmm7

		movaps	xmmword ptr [eax], xmm0
		movlps	qword ptr [eax+16], xmm2
	}

	epilogue

} // Mult00_6x6_6x1

SMLXTmpMatrix& __fastcall Mult01_6x6_1x6(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Calculating first 4 elements of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi(  b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[18] );	
	b1		= _mm_shuffle_ps( b0, tmp, 0xDD );
	b0		= _mm_shuffle_ps( b0, tmp, 0x88 );

	b2		= _mm_loadh_pi( _mm_loadl_pi(  b2, (__m64*)&src1[ 2] ), (__m64*)&src1[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[14] ), (__m64*)&src1[20] );	
	b3		= _mm_shuffle_ps( b2, tmp, 0xDD );
	b2		= _mm_shuffle_ps( b2, tmp, 0x88 );

	b4		= _mm_loadh_pi( _mm_loadl_pi(  b4, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[16] ), (__m64*)&src1[22] );	
	b5		= _mm_shuffle_ps( b4, tmp, 0xDD );
	b4		= _mm_shuffle_ps( b4, tmp, 0x88 );

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating last 2 elements of the destination matrix.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[0] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[2] );
	b2		= _mm_loadl_pi( b2, (__m64*)&src2[4] );

	b0		= _mm_shuffle_ps( b0, b0, 0x44 );
	b1		= _mm_shuffle_ps( b1, b1, 0x44 );
	b2		= _mm_shuffle_ps( b2, b2, 0x44 );
	
	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[24] ), (__m64*)&src1[30] );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[26] ), (__m64*)&src1[32] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[28] ), (__m64*)&src1[34] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	rslt	= _mm_add_ps( rslt, _mm_shuffle_ps( rslt, rslt, 0xB1 ) );
	rslt	= _mm_shuffle_ps( rslt, rslt, 0xCC );

	_mm_storel_pi( (__m64*)&dst[4], rslt );	

	return dmat;

} // Mult01_6x6_1x6

SMLXTmpMatrix& __fastcall Mult01_1x6_6x6(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Calculating first 4 elements of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi(  b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[18] );	
	b1		= _mm_shuffle_ps( b0, tmp, 0xDD );
	b0		= _mm_shuffle_ps( b0, tmp, 0x88 );

	b2		= _mm_loadh_pi( _mm_loadl_pi(  b2, (__m64*)&src1[ 2] ), (__m64*)&src1[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[14] ), (__m64*)&src1[20] );	
	b3		= _mm_shuffle_ps( b2, tmp, 0xDD );
	b2		= _mm_shuffle_ps( b2, tmp, 0x88 );

	b4		= _mm_loadh_pi( _mm_loadl_pi(  b4, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[16] ), (__m64*)&src1[22] );	
	b5		= _mm_shuffle_ps( b4, tmp, 0xDD );
	b4		= _mm_shuffle_ps( b4, tmp, 0x88 );

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating last 2 elements of the destination matrix.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[0] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[2] );
	b2		= _mm_loadl_pi( b2, (__m64*)&src2[4] );

	b0		= _mm_shuffle_ps( b0, b0, 0x44 );
	b1		= _mm_shuffle_ps( b1, b1, 0x44 );
	b2		= _mm_shuffle_ps( b2, b2, 0x44 );
	
	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[24] ), (__m64*)&src1[30] );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[26] ), (__m64*)&src1[32] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[28] ), (__m64*)&src1[34] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	rslt	= _mm_add_ps( rslt, _mm_shuffle_ps( rslt, rslt, 0xB1 ) );
	rslt	= _mm_shuffle_ps( rslt, rslt, 0xCC );

	_mm_storel_pi( (__m64*)&dst[4], rslt );	

	return dmat;

} // Mult01_1x6_6x6

SMLXTmpMatrix& __fastcall Mult11_6x1_6x6(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Calculating first 4 elements of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi(  b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[18] );	
	b1		= _mm_shuffle_ps( b0, tmp, 0xDD );
	b0		= _mm_shuffle_ps( b0, tmp, 0x88 );

	b2		= _mm_loadh_pi( _mm_loadl_pi(  b2, (__m64*)&src1[ 2] ), (__m64*)&src1[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[14] ), (__m64*)&src1[20] );	
	b3		= _mm_shuffle_ps( b2, tmp, 0xDD );
	b2		= _mm_shuffle_ps( b2, tmp, 0x88 );

	b4		= _mm_loadh_pi( _mm_loadl_pi(  b4, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[16] ), (__m64*)&src1[22] );	
	b5		= _mm_shuffle_ps( b4, tmp, 0xDD );
	b4		= _mm_shuffle_ps( b4, tmp, 0x88 );

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating last 2 elements of the destination matrix.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[0] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[2] );
	b2		= _mm_loadl_pi( b2, (__m64*)&src2[4] );

	b0		= _mm_shuffle_ps( b0, b0, 0x44 );
	b1		= _mm_shuffle_ps( b1, b1, 0x44 );
	b2		= _mm_shuffle_ps( b2, b2, 0x44 );
	
	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[24] ), (__m64*)&src1[30] );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[26] ), (__m64*)&src1[32] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[28] ), (__m64*)&src1[34] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	rslt	= _mm_add_ps( rslt, _mm_shuffle_ps( rslt, rslt, 0xB1 ) );
	rslt	= _mm_shuffle_ps( rslt, rslt, 0xCC );

	_mm_storel_pi( (__m64*)&dst[4], rslt );	

	return dmat;

} // Mult11_6x1_6x6

SMLXTmpMatrix& __fastcall Mult10_6x6_6x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	//	Calculating first 4 elements of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 6] ), (__m64*)&src1[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[12] ), (__m64*)&src1[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[18] ), (__m64*)&src1[20] );	
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src1[24] ), (__m64*)&src1[26] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src1[30] ), (__m64*)&src1[32] );	
	
	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating last 2 elements of the destination matrix.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[0] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[2] );
	b2		= _mm_loadl_pi( b2, (__m64*)&src2[4] );

	b0		= _mm_shuffle_ps( b0, b0, 0x50 );
	b1		= _mm_shuffle_ps( b1, b1, 0x50 );
	b2		= _mm_shuffle_ps( b2, b2, 0x50 );
	
	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[16] ), (__m64*)&src1[22] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[28] ), (__m64*)&src1[34] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	rslt	= _mm_add_ps( rslt, _mm_shuffle_ps( rslt, rslt, 0x4E ) );
	
	_mm_storel_pi( (__m64*)&dst[4], rslt );

	return dmat;

} // Mult10_6x6_6x1

SMLXTmpMatrix& __fastcall Mult11_6x6_1x6(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	//	Calculating first 4 elements of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 6] ), (__m64*)&src1[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[12] ), (__m64*)&src1[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[18] ), (__m64*)&src1[20] );	
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src1[24] ), (__m64*)&src1[26] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src1[30] ), (__m64*)&src1[32] );	
	
	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating last 2 elements of the destination matrix.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[0] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[2] );
	b2		= _mm_loadl_pi( b2, (__m64*)&src2[4] );

	b0		= _mm_shuffle_ps( b0, b0, 0x50 );
	b1		= _mm_shuffle_ps( b1, b1, 0x50 );
	b2		= _mm_shuffle_ps( b2, b2, 0x50 );
	
	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[16] ), (__m64*)&src1[22] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[28] ), (__m64*)&src1[34] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	rslt	= _mm_add_ps( rslt, _mm_shuffle_ps( rslt, rslt, 0x4E ) );
	
	_mm_storel_pi( (__m64*)&dst[4], rslt );

	return dmat;

} // Mult11_6x6_1x6

SMLXTmpMatrix& __fastcall Mult00_1x6_6x6(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	//	Calculating first 4 elements of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 6] ), (__m64*)&src1[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[12] ), (__m64*)&src1[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[18] ), (__m64*)&src1[20] );	
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src1[24] ), (__m64*)&src1[26] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src1[30] ), (__m64*)&src1[32] );	
	
	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating last 2 elements of the destination matrix.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[0] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[2] );
	b2		= _mm_loadl_pi( b2, (__m64*)&src2[4] );

	b0		= _mm_shuffle_ps( b0, b0, 0x50 );
	b1		= _mm_shuffle_ps( b1, b1, 0x50 );
	b2		= _mm_shuffle_ps( b2, b2, 0x50 );
	
	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[16] ), (__m64*)&src1[22] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[28] ), (__m64*)&src1[34] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	rslt	= _mm_add_ps( rslt, _mm_shuffle_ps( rslt, rslt, 0x4E ) );
	
	_mm_storel_pi( (__m64*)&dst[4], rslt );

	return dmat;

} // Mult00_1x6_6x6

SMLXTmpMatrix& __fastcall Mult10_6x1_6x6(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	//	Calculating first 4 elements of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 6] ), (__m64*)&src1[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[12] ), (__m64*)&src1[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[18] ), (__m64*)&src1[20] );	
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src1[24] ), (__m64*)&src1[26] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src1[30] ), (__m64*)&src1[32] );	
	
	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating last 2 elements of the destination matrix.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[0] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[2] );
	b2		= _mm_loadl_pi( b2, (__m64*)&src2[4] );

	b0		= _mm_shuffle_ps( b0, b0, 0x50 );
	b1		= _mm_shuffle_ps( b1, b1, 0x50 );
	b2		= _mm_shuffle_ps( b2, b2, 0x50 );
	
	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[16] ), (__m64*)&src1[22] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[28] ), (__m64*)&src1[34] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	rslt	= _mm_add_ps( rslt, _mm_shuffle_ps( rslt, rslt, 0x4E ) );
	
	_mm_storel_pi( (__m64*)&dst[4], rslt );

	return dmat;

} // Mult10_6x1_6x6

SMLXTmpMatrix& __fastcall Mult00_6x1_1x6(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1;
	__m128	row, rslt;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[ 4] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );
	_mm_storel_pi( (__m64*)&dst[4], rslt);

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[8], row );
	_mm_storel_pi( (__m64*)&dst[10], rslt);

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[14], row );
	_mm_storel_pi( (__m64*)&dst[16], rslt);

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[18], row );
	_mm_storeh_pi( (__m64*)&dst[20], row );
	_mm_storel_pi( (__m64*)&dst[22], rslt);

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[24], row );
	_mm_storeh_pi( (__m64*)&dst[26], row );
	_mm_storel_pi( (__m64*)&dst[28], rslt);

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[30], row );
	_mm_storeh_pi( (__m64*)&dst[32], row );
	_mm_storel_pi( (__m64*)&dst[34], rslt);

	return dmat;

} // Mult00_6x1_1x6

SMLXTmpMatrix& __fastcall Mult01_6x1_6x1(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1;
	__m128	row, rslt;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[ 4] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );
	_mm_storel_pi( (__m64*)&dst[4], rslt);

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[8], row );
	_mm_storel_pi( (__m64*)&dst[10], rslt);

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[14], row );
	_mm_storel_pi( (__m64*)&dst[16], rslt);

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[18], row );
	_mm_storeh_pi( (__m64*)&dst[20], row );
	_mm_storel_pi( (__m64*)&dst[22], rslt);

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[24], row );
	_mm_storeh_pi( (__m64*)&dst[26], row );
	_mm_storel_pi( (__m64*)&dst[28], rslt);

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[30], row );
	_mm_storeh_pi( (__m64*)&dst[32], row );
	_mm_storel_pi( (__m64*)&dst[34], rslt);

	return dmat;

} // Mult01_6x1_6x1

SMLXTmpMatrix& __fastcall Mult10_1x6_1x6(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1;
	__m128	row, rslt;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[ 4] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );
	_mm_storel_pi( (__m64*)&dst[4], rslt);

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[8], row );
	_mm_storel_pi( (__m64*)&dst[10], rslt);

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[14], row );
	_mm_storel_pi( (__m64*)&dst[16], rslt);

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[18], row );
	_mm_storeh_pi( (__m64*)&dst[20], row );
	_mm_storel_pi( (__m64*)&dst[22], rslt);

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[24], row );
	_mm_storeh_pi( (__m64*)&dst[26], row );
	_mm_storel_pi( (__m64*)&dst[28], rslt);

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[30], row );
	_mm_storeh_pi( (__m64*)&dst[32], row );
	_mm_storel_pi( (__m64*)&dst[34], rslt);

	return dmat;

} // Mult10_1x6_1x6

SMLXTmpMatrix& __fastcall Mult11_1x6_6x1(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1;
	__m128	row, rslt;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[ 4] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );
	_mm_storel_pi( (__m64*)&dst[4], rslt);

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[8], row );
	_mm_storel_pi( (__m64*)&dst[10], rslt);

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[14], row );
	_mm_storel_pi( (__m64*)&dst[16], rslt);

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[18], row );
	_mm_storeh_pi( (__m64*)&dst[20], row );
	_mm_storel_pi( (__m64*)&dst[22], rslt);

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[24], row );
	_mm_storeh_pi( (__m64*)&dst[26], row );
	_mm_storel_pi( (__m64*)&dst[28], rslt);

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[30], row );
	_mm_storeh_pi( (__m64*)&dst[32], row );
	_mm_storel_pi( (__m64*)&dst[34], rslt);

	return dmat;

} // Mult11_1x6_6x1

SMLXTmpMatrix& __fastcall Mult00_6x3_3x3(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src2[ 3] ), (__m64*)&src2[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src2[ 6] ), (__m64*)&src2[ 7] );
	
	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult00_6x3_3x3

SMLXTmpMatrix& __fastcall Mult01_6x3_3x3(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	//	Loading the src2 and transposing.

	b2		= _mm_loadh_pi( _mm_load_ss( &src2[0] ), (__m64*)&src2[1] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[4] ), (__m64*)&src2[7] );
	b0		= _mm_loadh_pi( _mm_load_ss( &src2[3] ), (__m64*)&src2[6] );

	b0		= _mm_shuffle_ps( b2, b0, 0x80 );
	b2		= _mm_shuffle_ps( b2, b1, 0xDB );
	b1		= _mm_shuffle_ps( b2, b1, 0x81 );
	
	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult01_6x3_3x3

SMLXTmpMatrix& __fastcall Mult10_6x3_6x3(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src2[ 3] ), (__m64*)&src2[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src2[ 6] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src2[ 9] ), (__m64*)&src2[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[12] ), (__m64*)&src2[13] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src2[15] ), (__m64*)&src2[16] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	return dmat;

} // Mult10_6x3_6x3

SMLXTmpMatrix& __fastcall Mult11_6x3_3x6(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading the src2 and transposing.

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src2[ 6] ), (__m64*)&src2[12] );
	b0		= _mm_shuffle_ps( tmp, b1, 0x80 );
	b1		= _mm_shuffle_ps( tmp, b1, 0xD5 );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src2[ 8] ), (__m64*)&src2[14] );
	b2		= _mm_shuffle_ps( tmp, b3, 0x8A );
	b3		= _mm_shuffle_ps( tmp, b3, 0xDF );

	tmp		= _mm_loadl_pi( tmp, (__m64*)&src2[4] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src2[10] ), (__m64*)&src2[16] );
	b4		= _mm_shuffle_ps( tmp, b5, 0x80 );
	b5		= _mm_shuffle_ps( tmp, b5, 0xD5 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	return dmat;

} // Mult11_6x3_3x6

SMLXTmpMatrix& __fastcall Mult00_6x3_3x5(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[10] ), (__m64*)&src2[12] );
	
	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[1] * src2[9];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[2] * src2[14];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;
	
	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[3] * src2[4];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[4] * src2[9];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[5] * src2[14];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[6] * src2[4];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[9];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[8] * src2[14];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[9] * src2[4];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[10] * src2[9];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[11] * src2[14];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[12] * src2[4];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[13] * src2[9];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[14];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[15] * src2[4];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[16] * src2[9];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[17] * src2[14];

	_mm_storel_pi( (__m64*)&dst[25], rslt );
	_mm_storeh_pi( (__m64*)&dst[27], rslt );	
	dst[29]	= f;

	return dmat;

} // Mult00_6x3_3x5

SMLXTmpMatrix& __fastcall Mult01_6x3_5x3(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt, tmp1, tmp2, tmp3;

	float	f;

	//	Loading the src2 and mixing.

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	tmp3	= _mm_loadh_pi( _mm_loadl_pi( tmp3, (__m64*)&src2[ 4] ), (__m64*)&src2[10] );

	b0		= _mm_shuffle_ps( tmp1, tmp2, 0xCC );
	b1		= _mm_shuffle_ps( tmp1, tmp3, 0x49 );
	tmp1	= _mm_shuffle_ps( tmp2, tmp3, 0xE9 );
	b2		= _mm_shuffle_ps( b1  , tmp1, 0xDD );
	b1		= _mm_shuffle_ps( b1  , tmp1, 0x88 );
	
	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[12];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[1] * src2[13];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[2] * src2[14];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;
	
	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[3] * src2[12];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[4] * src2[13];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[5] * src2[14];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[6] * src2[12];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[13];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[8] * src2[14];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[9] * src2[12];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[10] * src2[13];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[11] * src2[14];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[12] * src2[12];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[13] * src2[13];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[14];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[15] * src2[12];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[16] * src2[13];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[17] * src2[14];

	_mm_storel_pi( (__m64*)&dst[25], rslt );
	_mm_storeh_pi( (__m64*)&dst[27], rslt );	
	dst[29]	= f;

	return dmat;

} // Mult01_6x3_5x3

SMLXTmpMatrix& __fastcall Mult10_6x3_6x5(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[10] ), (__m64*)&src2[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[15] ), (__m64*)&src2[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[20] ), (__m64*)&src2[22] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[25] ), (__m64*)&src2[27] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[3] * src2[9];

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[6] * src2[14];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[9] * src2[19];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[12] * src2[24];

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[15] * src2[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[1] * src2[4];
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[4] * src2[9];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[7] * src2[14];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[10] * src2[19];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[13] * src2[24];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[16] * src2[29];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[2] * src2[4];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[5] * src2[9];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[8] * src2[14];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[11] * src2[19];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[14] * src2[24];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[17] * src2[29];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	return dmat;

} // Mult10_6x3_6x5

SMLXTmpMatrix& __fastcall Mult11_6x3_5x6(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	float	f;

	//	Loading first 4 rows of the src2 and transposing.

	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src2[ 0] ), (__m64*)&src2[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[12] ), (__m64*)&src2[18] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src2[ 2] ), (__m64*)&src2[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[14] ), (__m64*)&src2[20] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src2[ 4] ), (__m64*)&src2[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[16] ), (__m64*)&src2[22] );
	b4		= _mm_shuffle_ps( b5, tmp, 0x88 );
	b5		= _mm_shuffle_ps( b5, tmp, 0xDD );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[24];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[3] * src2[25];

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[6] * src2[26];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[9] * src2[27];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[12] * src2[28];

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[15] * src2[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[1] * src2[24];
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[4] * src2[25];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[7] * src2[26];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[10] * src2[27];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[13] * src2[28];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[16] * src2[29];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[2] * src2[24];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[5] * src2[25];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[8] * src2[26];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[11] * src2[27];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[14] * src2[28];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[17] * src2[29];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	return dmat;

} // Mult11_6x3_5x6

SMLXTmpMatrix& __fastcall Mult00_6x3_3x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt, tmp1, tmp2, tmp3;

	float	f1, f2;

	//	Loading the src1 and mixing.

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src1[ 6] ), (__m64*)&src1[ 8] );
	tmp3	= _mm_loadh_pi( _mm_loadl_pi( tmp3, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );

	b0		= _mm_shuffle_ps( tmp1, tmp2, 0xCC );
	b1		= _mm_shuffle_ps( tmp1, tmp3, 0x49 );
	tmp1	= _mm_shuffle_ps( tmp2, tmp3, 0xE9 );
	b2		= _mm_shuffle_ps( b1  , tmp1, 0xDD );
	b1		= _mm_shuffle_ps( b1  , tmp1, 0x88 );

	//	Calculating of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	f1		= src1[12] * src2[0];
	f2		= src1[15] * src2[0];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	
	f1		+= src1[13] * src2[1];
	f2		+= src1[16] * src2[1];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	
	f1		+= src1[14] * src2[2];
	f2		+= src1[17] * src2[2];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	
	dst[4]	= f1;
	dst[5]	= f2;

	return dmat;

} // Mult00_6x3_3x1

SMLXTmpMatrix& __fastcall Mult01_6x3_1x3(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt, tmp1, tmp2, tmp3;

	float	f1, f2;

	//	Loading the src1 and mixing.

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src1[ 6] ), (__m64*)&src1[ 8] );
	tmp3	= _mm_loadh_pi( _mm_loadl_pi( tmp3, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );

	b0		= _mm_shuffle_ps( tmp1, tmp2, 0xCC );
	b1		= _mm_shuffle_ps( tmp1, tmp3, 0x49 );
	tmp1	= _mm_shuffle_ps( tmp2, tmp3, 0xE9 );
	b2		= _mm_shuffle_ps( b1  , tmp1, 0xDD );
	b1		= _mm_shuffle_ps( b1  , tmp1, 0x88 );

	//	Calculating of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	f1		= src1[12] * src2[0];
	f2		= src1[15] * src2[0];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	
	f1		+= src1[13] * src2[1];
	f2		+= src1[16] * src2[1];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	
	f1		+= src1[14] * src2[2];
	f2		+= src1[17] * src2[2];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	
	dst[4]	= f1;
	dst[5]	= f2;

	return dmat;

} // Mult01_6x3_1x3

SMLXTmpMatrix& __fastcall Mult10_6x3_6x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	// Loading the src1.

	b0		= _mm_loadh_pi( _mm_load_ss( &src1[ 0] ), (__m64*)&src1[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src1[ 3] ), (__m64*)&src1[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src1[ 6] ), (__m64*)&src1[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src1[ 9] ), (__m64*)&src1[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src1[12] ), (__m64*)&src1[13] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src1[15] ), (__m64*)&src1[16] );
	
	//	Calculating of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	
	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	
	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult10_6x3_6x1

SMLXTmpMatrix& __fastcall Mult00_1x6_6x3(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	// Loading the src1.

	b0		= _mm_loadh_pi( _mm_load_ss( &src1[ 0] ), (__m64*)&src1[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src1[ 3] ), (__m64*)&src1[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src1[ 6] ), (__m64*)&src1[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src1[ 9] ), (__m64*)&src1[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src1[12] ), (__m64*)&src1[13] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src1[15] ), (__m64*)&src1[16] );
	
	//	Calculating of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	
	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	
	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult00_1x6_6x3

SMLXTmpMatrix& __fastcall Mult11_6x3_1x6(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	// Loading the src1.

	b0		= _mm_loadh_pi( _mm_load_ss( &src1[ 0] ), (__m64*)&src1[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src1[ 3] ), (__m64*)&src1[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src1[ 6] ), (__m64*)&src1[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src1[ 9] ), (__m64*)&src1[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src1[12] ), (__m64*)&src1[13] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src1[15] ), (__m64*)&src1[16] );
	
	//	Calculating of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	
	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	
	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult11_6x3_1x6

SMLXTmpMatrix& __fastcall Mult10_6x1_6x3(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	// Loading the src1.

	b0		= _mm_loadh_pi( _mm_load_ss( &src1[ 0] ), (__m64*)&src1[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src1[ 3] ), (__m64*)&src1[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src1[ 6] ), (__m64*)&src1[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src1[ 9] ), (__m64*)&src1[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src1[12] ), (__m64*)&src1[13] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src1[15] ), (__m64*)&src1[16] );
	
	//	Calculating of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	
	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	
	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult10_6x1_6x3

SMLXTmpMatrix& __fastcall Mult00_3x6_6x5(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[10] ), (__m64*)&src2[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[15] ), (__m64*)&src2[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[20] ), (__m64*)&src2[22] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[25] ), (__m64*)&src2[27] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[1] * src2[9];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[2] * src2[14];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[3] * src2[19];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[4] * src2[24];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[5] * src2[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;
	
	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[6] * src2[4];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[9];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[8] * src2[14];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[9] * src2[19];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[10] * src2[24];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[11] * src2[29];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[12] * src2[4];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[13] * src2[9];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[14];

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[15] * src2[19];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[16] * src2[24];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[17] * src2[29];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	return dmat;

} // Mult00_3x6_6x5

SMLXTmpMatrix& __fastcall Mult01_3x6_5x6(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	float	f;

	//	Loading first 4 rows of the src2 and transposing.

	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src2[ 0] ), (__m64*)&src2[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[12] ), (__m64*)&src2[18] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src2[ 2] ), (__m64*)&src2[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[14] ), (__m64*)&src2[20] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src2[ 4] ), (__m64*)&src2[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[16] ), (__m64*)&src2[22] );
	b4		= _mm_shuffle_ps( b5, tmp, 0x88 );
	b5		= _mm_shuffle_ps( b5, tmp, 0xDD );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[24];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[1] * src2[25];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[2] * src2[26];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[3] * src2[27];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[4] * src2[28];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[5] * src2[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;
	
	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[6] * src2[24];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[25];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[8] * src2[26];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[9] * src2[27];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[10] * src2[28];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[11] * src2[29];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[12] * src2[24];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[13] * src2[25];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[26];

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[15] * src2[27];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[16] * src2[28];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[17] * src2[29];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	return dmat;

} // Mult01_3x6_5x6

SMLXTmpMatrix& __fastcall Mult10_3x6_3x5(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[10] ), (__m64*)&src2[12] );
	
	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[6] * src2[9];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[12] * src2[14];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;
	
	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[1] * src2[4];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[9];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[13] * src2[14];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[2] * src2[4];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[8] * src2[9];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[14];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[3] * src2[4];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[9] * src2[9];

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[15] * src2[14];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[4] * src2[4];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[10] * src2[9];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[16] * src2[14];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[5] * src2[4];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[11] * src2[9];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[17] * src2[14];

	_mm_storel_pi( (__m64*)&dst[25], rslt );
	_mm_storeh_pi( (__m64*)&dst[27], rslt );	
	dst[29]	= f;

	return dmat;

} // Mult10_3x6_3x5

SMLXTmpMatrix& __fastcall Mult11_3x6_5x3(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt, tmp1, tmp2, tmp3;

	float	f;

	//	Loading the src2 and mixing.

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	tmp3	= _mm_loadh_pi( _mm_loadl_pi( tmp3, (__m64*)&src2[ 4] ), (__m64*)&src2[10] );

	b0		= _mm_shuffle_ps( tmp1, tmp2, 0xCC );
	b1		= _mm_shuffle_ps( tmp1, tmp3, 0x49 );
	tmp1	= _mm_shuffle_ps( tmp2, tmp3, 0xE9 );
	b2		= _mm_shuffle_ps( b1  , tmp1, 0xDD );
	b1		= _mm_shuffle_ps( b1  , tmp1, 0x88 );
	
	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[12];

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[6] * src2[13];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[12] * src2[14];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;
	
	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[1] * src2[12];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[13];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[13] * src2[14];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[2] * src2[12];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[8] * src2[13];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[14];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[3] * src2[12];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[9] * src2[13];

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[15] * src2[14];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[4] * src2[12];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[10] * src2[13];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[16] * src2[14];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[5] * src2[12];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[11] * src2[13];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[17] * src2[14];

	_mm_storel_pi( (__m64*)&dst[25], rslt );
	_mm_storeh_pi( (__m64*)&dst[27], rslt );	
	dst[29]	= f;

	return dmat;

} // Mult11_3x6_5x3

SMLXTmpMatrix& __fastcall Mult00_3x6_6x3(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src2[ 3] ), (__m64*)&src2[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src2[ 6] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src2[ 9] ), (__m64*)&src2[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[12] ), (__m64*)&src2[13] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src2[15] ), (__m64*)&src2[16] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	return dmat;

} // Mult00_3x6_6x3

SMLXTmpMatrix& __fastcall Mult01_3x6_3x6(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading the src2 and transposing.

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src2[ 6] ), (__m64*)&src2[12] );
	b0		= _mm_shuffle_ps( tmp, b1, 0x80 );
	b1		= _mm_shuffle_ps( tmp, b1, 0xD5 );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src2[ 8] ), (__m64*)&src2[14] );
	b2		= _mm_shuffle_ps( tmp, b3, 0x8A );
	b3		= _mm_shuffle_ps( tmp, b3, 0xDF );

	tmp		= _mm_loadl_pi( tmp, (__m64*)&src2[4] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src2[10] ), (__m64*)&src2[16] );
	b4		= _mm_shuffle_ps( tmp, b5, 0x80 );
	b5		= _mm_shuffle_ps( tmp, b5, 0xD5 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	return dmat;

} // Mult01_3x6_3x6

SMLXTmpMatrix& __fastcall Mult10_3x6_3x3(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src2[ 3] ), (__m64*)&src2[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src2[ 6] ), (__m64*)&src2[ 7] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult10_3x6_3x3

SMLXTmpMatrix& __fastcall Mult11_3x6_3x3(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	//	Loading the src2.

	b2		= _mm_loadh_pi( _mm_load_ss( &src2[0] ), (__m64*)&src2[1] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[4] ), (__m64*)&src2[7] );
	b0		= _mm_loadh_pi( _mm_load_ss( &src2[3] ), (__m64*)&src2[6] );

	b0		= _mm_shuffle_ps( b2, b0, 0x80 );
	b2		= _mm_shuffle_ps( b2, b1, 0xDB );
	b1		= _mm_shuffle_ps( b2, b1, 0x81 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult11_3x6_3x3

SMLXTmpMatrix& __fastcall Mult00_3x6_6x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	// Loading the src1 and transposing.

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 6] ), (__m64*)&src1[12] );
	b0		= _mm_shuffle_ps( b3, b1, 0x80 );
	b1		= _mm_shuffle_ps( b3, b1, 0xD5 );

	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[ 8] ), (__m64*)&src1[14] );
	b3		= _mm_shuffle_ps( b3, b2, 0xDB );
	b2		= _mm_shuffle_ps( b3, b2, 0x85 );

	b5		= _mm_loadl_pi( b5, &src1[ 4] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src1[10] ), (__m64*)&src1[16] );
	b5		= _mm_shuffle_ps( b5, b4, 0xD1 );
	b4		= _mm_shuffle_ps( b5, b4, 0x85 );

	//	Calculating the destination matrix.
	
	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult00_3x6_6x1

SMLXTmpMatrix& __fastcall Mult01_3x6_1x6(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	// Loading the src1 and transposing.

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 6] ), (__m64*)&src1[12] );
	b0		= _mm_shuffle_ps( b3, b1, 0x80 );
	b1		= _mm_shuffle_ps( b3, b1, 0xD5 );

	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[ 8] ), (__m64*)&src1[14] );
	b3		= _mm_shuffle_ps( b3, b2, 0xDB );
	b2		= _mm_shuffle_ps( b3, b2, 0x85 );

	b5		= _mm_loadl_pi( b5, &src1[ 4] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src1[10] ), (__m64*)&src1[16] );
	b5		= _mm_shuffle_ps( b5, b4, 0xD1 );
	b4		= _mm_shuffle_ps( b5, b4, 0x85 );

	//	Calculating the destination matrix.
	
	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult01_3x6_1x6

SMLXTmpMatrix& __fastcall Mult01_1x6_3x6(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	// Loading the src1 and transposing.

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 6] ), (__m64*)&src1[12] );
	b0		= _mm_shuffle_ps( b3, b1, 0x80 );
	b1		= _mm_shuffle_ps( b3, b1, 0xD5 );

	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[ 8] ), (__m64*)&src1[14] );
	b3		= _mm_shuffle_ps( b3, b2, 0xDB );
	b2		= _mm_shuffle_ps( b3, b2, 0x85 );

	b5		= _mm_loadl_pi( b5, &src1[ 4] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src1[10] ), (__m64*)&src1[16] );
	b5		= _mm_shuffle_ps( b5, b4, 0xD1 );
	b4		= _mm_shuffle_ps( b5, b4, 0x85 );

	//	Calculating the destination matrix.
	
	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult01_1x6_3x6

SMLXTmpMatrix& __fastcall Mult11_6x1_3x6(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	// Loading the src1 and transposing.

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 6] ), (__m64*)&src1[12] );
	b0		= _mm_shuffle_ps( b3, b1, 0x80 );
	b1		= _mm_shuffle_ps( b3, b1, 0xD5 );

	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[ 8] ), (__m64*)&src1[14] );
	b3		= _mm_shuffle_ps( b3, b2, 0xDB );
	b2		= _mm_shuffle_ps( b3, b2, 0x85 );

	b5		= _mm_loadl_pi( b5, &src1[ 4] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src1[10] ), (__m64*)&src1[16] );
	b5		= _mm_shuffle_ps( b5, b4, 0xD1 );
	b4		= _mm_shuffle_ps( b5, b4, 0x85 );

	//	Calculating the destination matrix.
	
	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult11_6x1_3x6

SMLXTmpMatrix& __fastcall Mult10_3x6_3x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	// Loading first 4 rows of the src1.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 6] ), (__m64*)&src1[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[12] ), (__m64*)&src1[14] );
	b3		= _mm_loadl_pi( b3, &src1[ 4] );
	b4		= _mm_loadl_pi( b4, &src1[10] );
	b5		= _mm_loadl_pi( b5, &src1[16] );
	
	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	b3		= _mm_mul_ps( b3, row );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	b3		= _mm_add_ps( b3, _mm_mul_ps( b4, row ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	b3		= _mm_add_ps( b3, _mm_mul_ps( b5, row ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	_mm_storel_pi( (__m64*)&dst[4], b3   );

	return dmat;

} // Mult10_3x6_3x1

SMLXTmpMatrix& __fastcall Mult11_3x6_1x3(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	// Loading first 4 rows of the src1.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 6] ), (__m64*)&src1[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[12] ), (__m64*)&src1[14] );
	b3		= _mm_loadl_pi( b3, &src1[ 4] );
	b4		= _mm_loadl_pi( b4, &src1[10] );
	b5		= _mm_loadl_pi( b5, &src1[16] );
	
	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	b3		= _mm_mul_ps( b3, row );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	b3		= _mm_add_ps( b3, _mm_mul_ps( b4, row ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	b3		= _mm_add_ps( b3, _mm_mul_ps( b5, row ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	_mm_storel_pi( (__m64*)&dst[4], b3   );

	return dmat;

} // Mult11_3x6_1x3

SMLXTmpMatrix& __fastcall Mult00_5x6_6x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	float	f;

	//	Loading first 4 rows of the src1 and transposing.

	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[18] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src1[ 2] ), (__m64*)&src1[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[14] ), (__m64*)&src1[20] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[16] ), (__m64*)&src1[22] );
	b4		= _mm_shuffle_ps( b5, tmp, 0x88 );
	b5		= _mm_shuffle_ps( b5, tmp, 0xDD );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src2[0] * src1[24];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[25];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[26];

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src2[3] * src1[27];

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src2[4] * src1[28];

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src2[5] * src1[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult00_5x6_6x1

SMLXTmpMatrix& __fastcall Mult01_5x6_1x6(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	float	f;

	//	Loading first 4 rows of the src1 and transposing.

	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[18] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src1[ 2] ), (__m64*)&src1[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[14] ), (__m64*)&src1[20] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[16] ), (__m64*)&src1[22] );
	b4		= _mm_shuffle_ps( b5, tmp, 0x88 );
	b5		= _mm_shuffle_ps( b5, tmp, 0xDD );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src2[0] * src1[24];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[25];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[26];

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src2[3] * src1[27];

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src2[4] * src1[28];

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src2[5] * src1[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult01_5x6_1x6

SMLXTmpMatrix& __fastcall Mult01_1x6_5x6(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	float	f;

	//	Loading first 4 rows of the src1 and transposing.

	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[18] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src1[ 2] ), (__m64*)&src1[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[14] ), (__m64*)&src1[20] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[16] ), (__m64*)&src1[22] );
	b4		= _mm_shuffle_ps( b5, tmp, 0x88 );
	b5		= _mm_shuffle_ps( b5, tmp, 0xDD );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src2[0] * src1[24];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[25];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[26];

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src2[3] * src1[27];

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src2[4] * src1[28];

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src2[5] * src1[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult01_1x6_5x6

SMLXTmpMatrix& __fastcall Mult11_6x1_5x6(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	float	f;

	//	Loading first 4 rows of the src1 and transposing.

	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[18] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src1[ 2] ), (__m64*)&src1[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[14] ), (__m64*)&src1[20] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[16] ), (__m64*)&src1[22] );
	b4		= _mm_shuffle_ps( b5, tmp, 0x88 );
	b5		= _mm_shuffle_ps( b5, tmp, 0xDD );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src2[0] * src1[24];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[25];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[26];

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src2[3] * src1[27];

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src2[4] * src1[28];

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src2[5] * src1[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult11_6x1_5x6

SMLXTmpMatrix& __fastcall Mult10_6x5_6x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 rows of the src1 and transposing.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src1[ 5] ), (__m64*)&src1[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2 , (__m64*)&src1[10] ), (__m64*)&src1[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src1[15] ), (__m64*)&src1[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4 , (__m64*)&src1[20] ), (__m64*)&src1[22] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src1[25] ), (__m64*)&src1[27] );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src2[0] * src1[4];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[9];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[14];

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src2[3] * src1[19];

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src2[4] * src1[24];

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src2[5] * src1[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult10_6x5_6x1

SMLXTmpMatrix& __fastcall Mult11_6x5_1x6(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 rows of the src1 and transposing.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src1[ 5] ), (__m64*)&src1[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2 , (__m64*)&src1[10] ), (__m64*)&src1[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src1[15] ), (__m64*)&src1[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4 , (__m64*)&src1[20] ), (__m64*)&src1[22] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src1[25] ), (__m64*)&src1[27] );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src2[0] * src1[4];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[9];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[14];

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src2[3] * src1[19];

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src2[4] * src1[24];

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src2[5] * src1[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult11_6x5_1x6

SMLXTmpMatrix& __fastcall Mult00_1x6_6x5(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 rows of the src1 and transposing.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src1[ 5] ), (__m64*)&src1[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2 , (__m64*)&src1[10] ), (__m64*)&src1[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src1[15] ), (__m64*)&src1[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4 , (__m64*)&src1[20] ), (__m64*)&src1[22] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src1[25] ), (__m64*)&src1[27] );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src2[0] * src1[4];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[9];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[14];

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src2[3] * src1[19];

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src2[4] * src1[24];

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src2[5] * src1[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult00_1x6_6x5

SMLXTmpMatrix& __fastcall Mult10_6x1_6x5(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 rows of the src1 and transposing.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src1[ 5] ), (__m64*)&src1[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2 , (__m64*)&src1[10] ), (__m64*)&src1[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src1[15] ), (__m64*)&src1[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4 , (__m64*)&src1[20] ), (__m64*)&src1[22] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src1[25] ), (__m64*)&src1[27] );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src2[0] * src1[4];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[9];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[14];

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src2[3] * src1[19];

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src2[4] * src1[24];

	row		= _mm_load_ss( &src2[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src2[5] * src1[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult10_6x1_6x5

SMLXTmpMatrix& __fastcall Mult00_6x1_1x5(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,5)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );

	dst[4]	= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[5], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );

	dst[9]	= src1[1] * src2[4];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[10], row );
	_mm_storeh_pi( (__m64*)&dst[12], row );

	dst[14]	= src1[2] * src2[4];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[15], row );
	_mm_storeh_pi( (__m64*)&dst[17], row );

	dst[19]	= src1[3] * src2[4];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[20], row );
	_mm_storeh_pi( (__m64*)&dst[22], row );

	dst[24]	= src1[4] * src2[4];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[25], row );
	_mm_storeh_pi( (__m64*)&dst[27], row );

	dst[29]	= src1[5] * src2[4];

	return dmat;

} // Mult00_6x1_1x5

SMLXTmpMatrix& __fastcall Mult01_6x1_5x1(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,5)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );

	dst[4]	= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[5], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );

	dst[9]	= src1[1] * src2[4];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[10], row );
	_mm_storeh_pi( (__m64*)&dst[12], row );

	dst[14]	= src1[2] * src2[4];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[15], row );
	_mm_storeh_pi( (__m64*)&dst[17], row );

	dst[19]	= src1[3] * src2[4];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[20], row );
	_mm_storeh_pi( (__m64*)&dst[22], row );

	dst[24]	= src1[4] * src2[4];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[25], row );
	_mm_storeh_pi( (__m64*)&dst[27], row );

	dst[29]	= src1[5] * src2[4];

	return dmat;

} // Mult01_6x1_5x1

SMLXTmpMatrix& __fastcall Mult10_1x6_1x5(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,5)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );

	dst[4]	= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[5], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );

	dst[9]	= src1[1] * src2[4];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[10], row );
	_mm_storeh_pi( (__m64*)&dst[12], row );

	dst[14]	= src1[2] * src2[4];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[15], row );
	_mm_storeh_pi( (__m64*)&dst[17], row );

	dst[19]	= src1[3] * src2[4];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[20], row );
	_mm_storeh_pi( (__m64*)&dst[22], row );

	dst[24]	= src1[4] * src2[4];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[25], row );
	_mm_storeh_pi( (__m64*)&dst[27], row );

	dst[29]	= src1[5] * src2[4];

	return dmat;

} // Mult10_1x6_1x5

SMLXTmpMatrix& __fastcall Mult11_1x6_5x1(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,5)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );

	dst[4]	= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[5], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );

	dst[9]	= src1[1] * src2[4];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[10], row );
	_mm_storeh_pi( (__m64*)&dst[12], row );

	dst[14]	= src1[2] * src2[4];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[15], row );
	_mm_storeh_pi( (__m64*)&dst[17], row );

	dst[19]	= src1[3] * src2[4];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[20], row );
	_mm_storeh_pi( (__m64*)&dst[22], row );

	dst[24]	= src1[4] * src2[4];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[25], row );
	_mm_storeh_pi( (__m64*)&dst[27], row );

	dst[29]	= src1[5] * src2[4];

	return dmat;

} // Mult11_1x6_5x1

SMLXTmpMatrix& __fastcall Mult00_6x1_1x3(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,3)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[1], row );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[3], row );
	_mm_storeh_pi( (__m64*)&dst[4], row );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[9], row );
	_mm_storeh_pi( (__m64*)&dst[10], row );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[13], row );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[15], row );
	_mm_storeh_pi( (__m64*)&dst[16], row );

	return dmat;

} // Mult00_6x1_1x3

SMLXTmpMatrix& __fastcall Mult11_1x6_3x1(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,3)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[1], row );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[3], row );
	_mm_storeh_pi( (__m64*)&dst[4], row );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[9], row );
	_mm_storeh_pi( (__m64*)&dst[10], row );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[13], row );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[15], row );
	_mm_storeh_pi( (__m64*)&dst[16], row );

	return dmat;

} // Mult11_1x6_3x1

SMLXTmpMatrix& __fastcall Mult10_1x6_1x3(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,3)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[1], row );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[3], row );
	_mm_storeh_pi( (__m64*)&dst[4], row );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[9], row );
	_mm_storeh_pi( (__m64*)&dst[10], row );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[13], row );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[15], row );
	_mm_storeh_pi( (__m64*)&dst[16], row );

	return dmat;

} // Mult10_1x6_1x3

SMLXTmpMatrix& __fastcall Mult01_6x1_3x1(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,3)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[1], row );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[3], row );
	_mm_storeh_pi( (__m64*)&dst[4], row );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[9], row );
	_mm_storeh_pi( (__m64*)&dst[10], row );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[13], row );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[15], row );
	_mm_storeh_pi( (__m64*)&dst[16], row );

	return dmat;

} // Mult01_6x1_3x1

SMLXTmpMatrix& __fastcall Mult00_1x6_6x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	dst[0] =	src1[0]*src2[0] + src1[1]*src2[1] + src1[2]*src2[2] +
				src1[3]*src2[3] + src1[4]*src2[4] + src1[5]*src2[5];

	return dmat;

} // Mult00_1x6_6x1

SMLXTmpMatrix& __fastcall Mult01_1x6_1x6(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	dst[0] =	src1[0]*src2[0] + src1[1]*src2[1] + src1[2]*src2[2] +
				src1[3]*src2[3] + src1[4]*src2[4] + src1[5]*src2[5];

	return dmat;

} // Mult01_1x6_1x6

SMLXTmpMatrix& __fastcall Mult10_6x1_6x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	dst[0] =	src1[0]*src2[0] + src1[1]*src2[1] + src1[2]*src2[2] +
				src1[3]*src2[3] + src1[4]*src2[4] + src1[5]*src2[5];

	return dmat;

} // Mult10_6x1_6x1

SMLXTmpMatrix& __fastcall Mult11_6x1_1x6(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	dst[0] =	src1[0]*src2[0] + src1[1]*src2[1] + src1[2]*src2[2] +
				src1[3]*src2[3] + src1[4]*src2[4] + src1[5]*src2[5];

	return dmat;

} // Mult11_6x1_1x6

SMLXTmpMatrix& __fastcall Mult00_6x1_1x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,1)();
	register float* dst = dmat.Data();

	dst[0] =	src1[0]*src2[0];
	dst[1] =	src1[1]*src2[0];
	dst[2] =	src1[2]*src2[0];
	dst[3] =	src1[3]*src2[0];
	dst[4] =	src1[4]*src2[0];
	dst[5] =	src1[5]*src2[0];

	return dmat;

} // Mult00_6x1_1x1

SMLXTmpMatrix& __fastcall Mult01_6x1_1x1(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,1)();
	register float* dst = dmat.Data();

	dst[0] =	src1[0]*src2[0];
	dst[1] =	src1[1]*src2[0];
	dst[2] =	src1[2]*src2[0];
	dst[3] =	src1[3]*src2[0];
	dst[4] =	src1[4]*src2[0];
	dst[5] =	src1[5]*src2[0];

	return dmat;

} // Mult01_6x1_1x1

SMLXTmpMatrix& __fastcall Mult10_1x6_1x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,1)();
	register float* dst = dmat.Data();

	dst[0] =	src1[0]*src2[0];
	dst[1] =	src1[1]*src2[0];
	dst[2] =	src1[2]*src2[0];
	dst[3] =	src1[3]*src2[0];
	dst[4] =	src1[4]*src2[0];
	dst[5] =	src1[5]*src2[0];

	return dmat;

} // Mult10_1x6_1x1

SMLXTmpMatrix& __fastcall Mult11_1x6_1x1(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,1)();
	register float* dst = dmat.Data();

	dst[0] =	src1[0]*src2[0];
	dst[1] =	src1[1]*src2[0];
	dst[2] =	src1[2]*src2[0];
	dst[3] =	src1[3]*src2[0];
	dst[4] =	src1[4]*src2[0];
	dst[5] =	src1[5]*src2[0];

	return dmat;

} // Mult11_1x6_1x1

SMLXTmpMatrix& __fastcall Mult00_6x5_5x6(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[12] ), (__m64*)&src2[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[18] ), (__m64*)&src2[20] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[24] ), (__m64*)&src2[26] );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating first 4 elements in the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[30], rslt );
	_mm_storeh_pi( (__m64*)&dst[32], rslt );

	//	Calculating last 2 columns of the destination matrix.

	tmp		= _mm_loadl_pi( tmp, (__m64*)&src2[ 4] );
	b1		= _mm_loadl_pi(  b1, (__m64*)&src2[10] );
	b2		= _mm_loadl_pi(  b2, (__m64*)&src2[16] );
	b3		= _mm_loadl_pi(  b3, (__m64*)&src2[22] );
	b4		= _mm_loadl_pi(  b4, (__m64*)&src2[28] );

	b0		= _mm_shuffle_ps( tmp, b1, 0x44 );
	b1		= _mm_shuffle_ps( b1,  b2, 0x44 );
	b2		= _mm_shuffle_ps( b2,  b3, 0x44 );
	b3		= _mm_shuffle_ps( b3,  b4, 0x44 );
	b4		= _mm_shuffle_ps( b4, tmp, 0x44 );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 2] ), (__m64*)&src1[ 8] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 4] );

	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[10] ), (__m64*)&src1[16] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[18] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[14] );

	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[20] ), (__m64*)&src1[26] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[22] ), (__m64*)&src1[28] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[24] );

	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );
	_mm_storeh_pi( (__m64*)&dst[34], rslt );

	return dmat;

} // Mult00_6x5_5x6

SMLXTmpMatrix& __fastcall Mult01_6x5_6x5(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading first 4 columns of the src2.

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 0] ), (__m64*)&src2[ 5] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[10] ), (__m64*)&src2[15] );	
	b0		= _mm_shuffle_ps( b5, b1, 0x88 );
	b1		= _mm_shuffle_ps( b5, b1, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 2] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[12] ), (__m64*)&src2[17] );
	b2		= _mm_shuffle_ps( b5, b3, 0x88 );
	b3		= _mm_shuffle_ps( b5, b3, 0xDD );

	b5		= _mm_loadh_pi( _mm_load_ss( &src2[ 4] ), (__m64*)&src2[ 9] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[14] ), (__m64*)&src2[19] );
	b4		= _mm_shuffle_ps( b5, b4, 0x88 );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating first 4 elements in the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[30], rslt );
	_mm_storeh_pi( (__m64*)&dst[32], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[20] ), (__m64*)&src2[25] );
	b1		= _mm_shuffle_ps( b0,  b0, 0xDD );
	b0		= _mm_shuffle_ps( b0,  b0, 0x88 );

	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[22] ), (__m64*)&src2[27] );
	b3		= _mm_shuffle_ps( b2,  b2, 0xDD );
	b2		= _mm_shuffle_ps( b2,  b2, 0x88 );

	b4		= _mm_loadh_pi( _mm_load_ss( &src2[29] ), (__m64*)&src2[24] );
	b4		= _mm_shuffle_ps( b4, b4, 0x22 );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 0] ), (__m64*)&src1[ 5] );
	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 2] ), (__m64*)&src1[ 7] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadh_pi( _mm_load_ss( &src1[9] ), (__m64*)&src1[4] );

	row		= _mm_shuffle_ps( row, row, 0x0A );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[10] ), (__m64*)&src1[15] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[17] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadh_pi( _mm_load_ss( &src1[19] ), (__m64*)&src1[14] );

	row		= _mm_shuffle_ps( row, row, 0x0A );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[20] ), (__m64*)&src1[25] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[22] ), (__m64*)&src1[27] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadh_pi( _mm_load_ss( &src1[29] ), (__m64*)&src1[24] );

	row		= _mm_shuffle_ps( row, row, 0x0A );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );
	_mm_storeh_pi( (__m64*)&dst[34], rslt );

	return dmat;

} // Mult01_6x5_6x5

SMLXTmpMatrix& __fastcall Mult10_6x5_6x6(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading first 4 columns of src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[12] ), (__m64*)&src2[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[18] ), (__m64*)&src2[20] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[24] ), (__m64*)&src2[26] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[30] ), (__m64*)&src2[32] );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src2[ 4] ), (__m64*)&src2[10] );
	b1		= _mm_shuffle_ps( b0 , b0 , 0xEE );
	b0		= _mm_shuffle_ps( b0 , b0 , 0x44 );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2 , (__m64*)&src2[16] ), (__m64*)&src2[22] );
	b3		= _mm_shuffle_ps( b2 , b2 , 0xEE );
	b2		= _mm_shuffle_ps( b2 , b2 , 0x44 );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4 , (__m64*)&src2[28] ), (__m64*)&src2[34] );
	b5		= _mm_shuffle_ps( b4 , b4 , 0xEE );
	b4		= _mm_shuffle_ps( b4 , b4 , 0x44 );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 0] ), (__m64*)&src1[ 5] );
	row		= _mm_shuffle_ps( tmp, tmp, 0x50 );
	rslt	= _mm_mul_ps( row, b0 );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xFA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b1 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[10] ), (__m64*)&src1[15] );
	row		= _mm_shuffle_ps( tmp, tmp, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xFA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[20] ), (__m64*)&src1[25] );
	row		= _mm_shuffle_ps( tmp, tmp, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xFA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 2] ), (__m64*)&src1[ 7] );
	row		= _mm_shuffle_ps( tmp, tmp, 0x50 );
	tmp		= _mm_shuffle_ps( tmp, tmp, 0xFA );
	rslt	= _mm_add_ps( _mm_mul_ps( row, b0 ), _mm_mul_ps( tmp, b1 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[17] );
	row		= _mm_shuffle_ps( tmp, tmp, 0x50 );	
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xFA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );
	
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[22] ), (__m64*)&src1[27] );
	row		= _mm_shuffle_ps( tmp, tmp, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xFA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	row		= _mm_load_ss( &src1[ 4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[ 9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );
	
	return dmat;

} // Mult10_6x5_6x6

SMLXTmpMatrix& __fastcall Mult11_6x5_6x6(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading first 4 rows of src2 and transposing.

	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src2[ 0] ), (__m64*)&src2[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[12] ), (__m64*)&src2[18] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src2[ 2] ), (__m64*)&src2[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[14] ), (__m64*)&src2[20] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src2[ 4] ), (__m64*)&src2[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[16] ), (__m64*)&src2[22] );
	b4		= _mm_shuffle_ps( b5, tmp, 0x88 );
	b5		= _mm_shuffle_ps( b5, tmp, 0xDD );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src2[24] ), (__m64*)&src2[30] );
	b1		= _mm_shuffle_ps( b0 , b0 , 0xDD );
	b0		= _mm_shuffle_ps( b0 , b0 , 0x88 );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2 , (__m64*)&src2[26] ), (__m64*)&src2[32] );
	b3		= _mm_shuffle_ps( b2 , b2 , 0xDD );
	b2		= _mm_shuffle_ps( b2 , b2 , 0x88 );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4 , (__m64*)&src2[28] ), (__m64*)&src2[34] );
	b5		= _mm_shuffle_ps( b4 , b4 , 0xDD );
	b4		= _mm_shuffle_ps( b4 , b4 , 0x88 );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 0] ), (__m64*)&src1[ 5] );
	row		= _mm_shuffle_ps( tmp, tmp, 0x50 );
	rslt	= _mm_mul_ps( row, b0 );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xFA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b1 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[10] ), (__m64*)&src1[15] );
	row		= _mm_shuffle_ps( tmp, tmp, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xFA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[20] ), (__m64*)&src1[25] );
	row		= _mm_shuffle_ps( tmp, tmp, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xFA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 2] ), (__m64*)&src1[ 7] );
	row		= _mm_shuffle_ps( tmp, tmp, 0x50 );
	tmp		= _mm_shuffle_ps( tmp, tmp, 0xFA );
	rslt	= _mm_add_ps( _mm_mul_ps( row, b0 ), _mm_mul_ps( tmp, b1 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[17] );
	row		= _mm_shuffle_ps( tmp, tmp, 0x50 );	
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xFA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );
	
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[22] ), (__m64*)&src1[27] );
	row		= _mm_shuffle_ps( tmp, tmp, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xFA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	row		= _mm_load_ss( &src1[ 4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[ 9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );

	return dmat;

} // Mult11_6x5_6x6

SMLXTmpMatrix& __fastcall Mult00_6x5_5x5(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[10] ), (__m64*)&src2[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[15] ), (__m64*)&src2[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[20] ), (__m64*)&src2[22] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );	
	f		= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[1] * src2[9];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[2] * src2[14];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[3] * src2[19];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[4] * src2[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[5] * src2[4];
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[6] * src2[9];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[7] * src2[14];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[8] * src2[19];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[9] * src2[24];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[10] * src2[4];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[11] * src2[9];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[12] * src2[14];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[13] * src2[19];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[14] * src2[24];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[15] * src2[4];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[16] * src2[9];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[17] * src2[14];

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[18] * src2[19];

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[19] * src2[24];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[20] * src2[4];

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[21] * src2[9];

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[22] * src2[14];

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[23] * src2[19];

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[24] * src2[24];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[25] * src2[4];

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[26] * src2[9];

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[27] * src2[14];

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[28] * src2[19];

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[29] * src2[24];

	_mm_storel_pi( (__m64*)&dst[25], rslt );
	_mm_storeh_pi( (__m64*)&dst[27], rslt );
	dst[29]	= f;

	return dmat;

} // Mult00_6x5_5x5

SMLXTmpMatrix& __fastcall Mult01_6x5_5x5(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	float	f;

	//	Loading first 4 columns of the src2.

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 0] ), (__m64*)&src2[ 5] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[10] ), (__m64*)&src2[15] );	
	b0		= _mm_shuffle_ps( b5, b1, 0x88 );
	b1		= _mm_shuffle_ps( b5, b1, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 2] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[12] ), (__m64*)&src2[17] );
	b2		= _mm_shuffle_ps( b5, b3, 0x88 );
	b3		= _mm_shuffle_ps( b5, b3, 0xDD );

	b5		= _mm_loadh_pi( _mm_load_ss( &src2[ 4] ), (__m64*)&src2[ 9] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[14] ), (__m64*)&src2[19] );
	b4		= _mm_shuffle_ps( b5, b4, 0x88 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[20];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[1] * src2[21];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[2] * src2[22];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[3] * src2[23];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[4] * src2[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[5] * src2[20];
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[6] * src2[21];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[7] * src2[22];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[8] * src2[23];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[9] * src2[24];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[10] * src2[20];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[11] * src2[21];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[12] * src2[22];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[13] * src2[23];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[14] * src2[24];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[15] * src2[20];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[16] * src2[21];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[17] * src2[22];

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[18] * src2[23];

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[19] * src2[24];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[20] * src2[20];

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[21] * src2[21];

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[22] * src2[22];

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[23] * src2[23];

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[24] * src2[24];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[25] * src2[20];

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[26] * src2[21];

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[27] * src2[22];

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[28] * src2[23];

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[29] * src2[24];

	_mm_storel_pi( (__m64*)&dst[25], rslt );
	_mm_storeh_pi( (__m64*)&dst[27], rslt );
	dst[29]	= f;

	return dmat;

} // Mult01_6x5_5x5

SMLXTmpMatrix& __fastcall Mult10_6x5_6x5(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[10] ), (__m64*)&src2[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[15] ), (__m64*)&src2[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[20] ), (__m64*)&src2[22] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[25] ), (__m64*)&src2[27] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[5] * src2[9];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[10] * src2[14];

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[15] * src2[19];

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[20] * src2[24];

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[25] * src2[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[1] * src2[4];
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[6] * src2[9];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[11] * src2[14];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[16] * src2[19];

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[21] * src2[24];

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[26] * src2[29];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[2] * src2[4];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[9];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[12] * src2[14];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[17] * src2[19];

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[22] * src2[24];

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[27] * src2[29];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[3] * src2[4];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[8] * src2[9];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[13] * src2[14];

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[18] * src2[19];

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[23] * src2[24];

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[28] * src2[29];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[4] * src2[4];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[9] * src2[9];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[14];

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[19] * src2[19];

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[24] * src2[24];

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[29] * src2[29];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	return dmat;

} // Mult10_6x5_6x5

SMLXTmpMatrix& __fastcall Mult11_6x5_5x6(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	float	f;

	//	Loading first 4 rows of the src2 and transposing.

	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src2[ 0] ), (__m64*)&src2[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[12] ), (__m64*)&src2[18] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src2[ 2] ), (__m64*)&src2[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[14] ), (__m64*)&src2[20] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src2[ 4] ), (__m64*)&src2[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[16] ), (__m64*)&src2[22] );
	b4		= _mm_shuffle_ps( b5, tmp, 0x88 );
	b5		= _mm_shuffle_ps( b5, tmp, 0xDD );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[24];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[5] * src2[25];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[10] * src2[26];

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[15] * src2[27];

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[20] * src2[28];

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[25] * src2[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[1] * src2[24];
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[6] * src2[25];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[11] * src2[26];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[16] * src2[27];

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[21] * src2[28];

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[26] * src2[29];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[2] * src2[24];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[25];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[12] * src2[26];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[17] * src2[27];

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[22] * src2[28];

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[27] * src2[29];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[3] * src2[24];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[8] * src2[25];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[13] * src2[26];

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[18] * src2[27];

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[23] * src2[28];

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[28] * src2[29];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[4] * src2[24];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[9] * src2[25];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[26];

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[19] * src2[27];

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[24] * src2[28];

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[29] * src2[29];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	return dmat;

} // Mult11_6x5_5x6

SMLXTmpMatrix& __fastcall Mult00_6x5_5x3(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src2[ 3] ), (__m64*)&src2[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src2[ 6] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src2[ 9] ), (__m64*)&src2[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[12] ), (__m64*)&src2[13] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult00_6x5_5x3

SMLXTmpMatrix& __fastcall Mult01_6x5_3x5(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[ 0] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[10] );	
	b0		= _mm_shuffle_ps( b0, b1, 0x84 );
	b1		= _mm_shuffle_ps( b0, b1, 0xD5 );

	b2		= _mm_loadl_pi( b2, (__m64*)&src2[ 2] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[7] ), (__m64*)&src2[12] );
	b2		= _mm_shuffle_ps( b2, b3, 0x84 );
	b3		= _mm_shuffle_ps( b2, b3, 0xD5 );

	b4		= _mm_load_ss( &src2[ 4] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src2[9] ), (__m64*)&src2[14] );
	b4		= _mm_shuffle_ps( b4, b5, 0x80 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult01_6x5_3x5

SMLXTmpMatrix& __fastcall Mult10_6x5_6x3(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	//	Loading first 4 columns of src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src2[ 3] ), (__m64*)&src2[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src2[ 6] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src2[ 9] ), (__m64*)&src2[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[12] ), (__m64*)&src2[13] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src2[15] ), (__m64*)&src2[16] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	
	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	return dmat;

} // Mult10_6x5_6x3

SMLXTmpMatrix& __fastcall Mult11_6x5_3x6(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading the src2 and transposing.

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src2[ 6] ), (__m64*)&src2[12] );
	b0		= _mm_shuffle_ps( tmp, b1, 0x80 );
	b1		= _mm_shuffle_ps( tmp, b1, 0xD5 );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src2[ 8] ), (__m64*)&src2[14] );
	b2		= _mm_shuffle_ps( tmp, b3, 0x8A );
	b3		= _mm_shuffle_ps( tmp, b3, 0xDF );

	tmp		= _mm_loadl_pi( tmp, (__m64*)&src2[4] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src2[10] ), (__m64*)&src2[16] );
	b4		= _mm_shuffle_ps( tmp, b5, 0x80 );
	b5		= _mm_shuffle_ps( tmp, b5, 0xD5 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	
	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	return dmat;

} // Mult11_6x5_3x6

SMLXTmpMatrix& __fastcall Mult00_6x5_5x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Calculating first 4 elements of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi(  b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 5] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[10] ), (__m64*)&src1[15] );
	b1		= _mm_shuffle_ps( b0, tmp, 0xDD );
	b0		= _mm_shuffle_ps( b0, tmp, 0x88 );

	b2		= _mm_loadh_pi( _mm_loadl_pi(  b2, (__m64*)&src1[ 2] ), (__m64*)&src1[ 7] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[17] );
	b3		= _mm_shuffle_ps( b2, tmp, 0xDD );
	b2		= _mm_shuffle_ps( b2, tmp, 0x88 );

	b4		= _mm_loadh_pi( _mm_load_ss( &src1[ 4] ), (__m64*)&src1[ 9] );
	tmp		= _mm_loadh_pi( _mm_load_ss( &src1[14] ), (__m64*)&src1[19] );
	b4		= _mm_shuffle_ps( b4, tmp, 0x88 );

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating last 2 elements of the destination matrix.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[0] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[2] );
	b2		= _mm_load_ss( &src2[4] );

	b0		= _mm_shuffle_ps( b0, b0, 0x44 );
	b1		= _mm_shuffle_ps( b1, b1, 0x44 );
	b2		= _mm_shuffle_ps( b2, b2, 0x44 );
	
	row		= _mm_loadl_pi( _mm_loadh_pi( row, (__m64*)&src1[20] ), (__m64*)&src1[25] );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( _mm_loadh_pi( row, (__m64*)&src1[22] ), (__m64*)&src1[27] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadh_pi( _mm_load_ss( &src1[29] ), (__m64*)&src1[24] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	rslt	= _mm_add_ps( rslt, _mm_shuffle_ps( rslt, rslt, 0xB1 ) );
	rslt	= _mm_shuffle_ps( rslt, rslt, 0x33 );

	_mm_storel_pi( (__m64*)&dst[4], rslt );

	return dmat;

} // Mult00_6x5_5x1

SMLXTmpMatrix& __fastcall Mult01_6x5_1x5(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Calculating first 4 elements of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi(  b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 5] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[10] ), (__m64*)&src1[15] );
	b1		= _mm_shuffle_ps( b0, tmp, 0xDD );
	b0		= _mm_shuffle_ps( b0, tmp, 0x88 );

	b2		= _mm_loadh_pi( _mm_loadl_pi(  b2, (__m64*)&src1[ 2] ), (__m64*)&src1[ 7] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[17] );
	b3		= _mm_shuffle_ps( b2, tmp, 0xDD );
	b2		= _mm_shuffle_ps( b2, tmp, 0x88 );

	b4		= _mm_loadh_pi( _mm_load_ss( &src1[ 4] ), (__m64*)&src1[ 9] );
	tmp		= _mm_loadh_pi( _mm_load_ss( &src1[14] ), (__m64*)&src1[19] );
	b4		= _mm_shuffle_ps( b4, tmp, 0x88 );

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating last 2 elements of the destination matrix.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[0] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[2] );
	b2		= _mm_load_ss( &src2[4] );

	b0		= _mm_shuffle_ps( b0, b0, 0x44 );
	b1		= _mm_shuffle_ps( b1, b1, 0x44 );
	b2		= _mm_shuffle_ps( b2, b2, 0x44 );
	
	row		= _mm_loadl_pi( _mm_loadh_pi( row, (__m64*)&src1[20] ), (__m64*)&src1[25] );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( _mm_loadh_pi( row, (__m64*)&src1[22] ), (__m64*)&src1[27] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadh_pi( _mm_load_ss( &src1[29] ), (__m64*)&src1[24] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	rslt	= _mm_add_ps( rslt, _mm_shuffle_ps( rslt, rslt, 0xB1 ) );
	rslt	= _mm_shuffle_ps( rslt, rslt, 0x33 );

	_mm_storel_pi( (__m64*)&dst[4], rslt );

	return dmat;

} // Mult01_6x5_1x5

SMLXTmpMatrix& __fastcall Mult01_1x5_6x5(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Calculating first 4 elements of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi(  b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 5] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[10] ), (__m64*)&src1[15] );
	b1		= _mm_shuffle_ps( b0, tmp, 0xDD );
	b0		= _mm_shuffle_ps( b0, tmp, 0x88 );

	b2		= _mm_loadh_pi( _mm_loadl_pi(  b2, (__m64*)&src1[ 2] ), (__m64*)&src1[ 7] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[17] );
	b3		= _mm_shuffle_ps( b2, tmp, 0xDD );
	b2		= _mm_shuffle_ps( b2, tmp, 0x88 );

	b4		= _mm_loadh_pi( _mm_load_ss( &src1[ 4] ), (__m64*)&src1[ 9] );
	tmp		= _mm_loadh_pi( _mm_load_ss( &src1[14] ), (__m64*)&src1[19] );
	b4		= _mm_shuffle_ps( b4, tmp, 0x88 );

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating last 2 elements of the destination matrix.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[0] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[2] );
	b2		= _mm_load_ss( &src2[4] );

	b0		= _mm_shuffle_ps( b0, b0, 0x44 );
	b1		= _mm_shuffle_ps( b1, b1, 0x44 );
	b2		= _mm_shuffle_ps( b2, b2, 0x44 );
	
	row		= _mm_loadl_pi( _mm_loadh_pi( row, (__m64*)&src1[20] ), (__m64*)&src1[25] );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( _mm_loadh_pi( row, (__m64*)&src1[22] ), (__m64*)&src1[27] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadh_pi( _mm_load_ss( &src1[29] ), (__m64*)&src1[24] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	rslt	= _mm_add_ps( rslt, _mm_shuffle_ps( rslt, rslt, 0xB1 ) );
	rslt	= _mm_shuffle_ps( rslt, rslt, 0x33 );

	_mm_storel_pi( (__m64*)&dst[4], rslt );

	return dmat;

} // Mult01_1x5_6x5

SMLXTmpMatrix& __fastcall Mult11_5x1_6x5(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Calculating first 4 elements of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi(  b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 5] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[10] ), (__m64*)&src1[15] );
	b1		= _mm_shuffle_ps( b0, tmp, 0xDD );
	b0		= _mm_shuffle_ps( b0, tmp, 0x88 );

	b2		= _mm_loadh_pi( _mm_loadl_pi(  b2, (__m64*)&src1[ 2] ), (__m64*)&src1[ 7] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[17] );
	b3		= _mm_shuffle_ps( b2, tmp, 0xDD );
	b2		= _mm_shuffle_ps( b2, tmp, 0x88 );

	b4		= _mm_loadh_pi( _mm_load_ss( &src1[ 4] ), (__m64*)&src1[ 9] );
	tmp		= _mm_loadh_pi( _mm_load_ss( &src1[14] ), (__m64*)&src1[19] );
	b4		= _mm_shuffle_ps( b4, tmp, 0x88 );

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating last 2 elements of the destination matrix.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[0] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[2] );
	b2		= _mm_load_ss( &src2[4] );

	b0		= _mm_shuffle_ps( b0, b0, 0x44 );
	b1		= _mm_shuffle_ps( b1, b1, 0x44 );
	b2		= _mm_shuffle_ps( b2, b2, 0x44 );
	
	row		= _mm_loadl_pi( _mm_loadh_pi( row, (__m64*)&src1[20] ), (__m64*)&src1[25] );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( _mm_loadh_pi( row, (__m64*)&src1[22] ), (__m64*)&src1[27] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadh_pi( _mm_load_ss( &src1[29] ), (__m64*)&src1[24] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	rslt	= _mm_add_ps( rslt, _mm_shuffle_ps( rslt, rslt, 0xB1 ) );
	rslt	= _mm_shuffle_ps( rslt, rslt, 0x33 );

	_mm_storel_pi( (__m64*)&dst[4], rslt );

	return dmat;

} // Mult11_5x1_6x5

SMLXTmpMatrix& __fastcall Mult00_5x6_6x6(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading first 4 columns of src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[12] ), (__m64*)&src2[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[18] ), (__m64*)&src2[20] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[24] ), (__m64*)&src2[26] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[30] ), (__m64*)&src2[32] );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src2[ 4] ), (__m64*)&src2[10] );
	b1		= _mm_shuffle_ps( b0 , b0 , 0x4E );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2 , (__m64*)&src2[16] ), (__m64*)&src2[22] );
	b3		= _mm_shuffle_ps( b2 , b2 , 0x4E );	
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4 , (__m64*)&src2[28] ), (__m64*)&src2[34] );
	b5		= _mm_shuffle_ps( b4 , b4 , 0x4E );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( tmp, tmp, 0xF0 );
	rslt	= _mm_mul_ps( row, b0 );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xA5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b1 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 2] ), (__m64*)&src1[ 8] );
	row		= _mm_shuffle_ps( tmp, tmp, 0xF0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xA5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	row		= _mm_shuffle_ps( tmp, tmp, 0xF0 );	
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xA5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );
	
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[18] );
	row		= _mm_shuffle_ps( tmp, tmp, 0xF0 );
	rslt	= _mm_mul_ps( row, b0 );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xA5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b1 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[14] ), (__m64*)&src1[20] );
	row		= _mm_shuffle_ps( tmp, tmp, 0xF0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xA5 );	
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );
	
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[16] ), (__m64*)&src1[22] );
	row		= _mm_shuffle_ps( tmp, tmp, 0xF0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xA5 );	
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[24] ), (__m64*)&src1[26] );
	row		= _mm_shuffle_ps( tmp, tmp, 0x50 );
	rslt	= _mm_mul_ps( row, b0 );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xFA );	
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b2 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	rslt	= _mm_add_ps( rslt, _mm_shuffle_ps( rslt, rslt, 0x5E ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );

	return dmat;

} // Mult00_5x6_6x6

SMLXTmpMatrix& __fastcall Mult01_5x6_6x6(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading first 4 rows of src2 and transposing.

	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src2[ 0] ), (__m64*)&src2[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[12] ), (__m64*)&src2[18] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src2[ 2] ), (__m64*)&src2[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[14] ), (__m64*)&src2[20] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src2[ 4] ), (__m64*)&src2[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[16] ), (__m64*)&src2[22] );
	b4		= _mm_shuffle_ps( b5, tmp, 0x88 );
	b5		= _mm_shuffle_ps( b5, tmp, 0xDD );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src2[24] ), (__m64*)&src2[30] );
	b1		= _mm_shuffle_ps( b0 , b0 , 0xDD );
	b0		= _mm_shuffle_ps( b0 , b0 , 0x88 );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2 , (__m64*)&src2[26] ), (__m64*)&src2[32] );
	b3		= _mm_shuffle_ps( b2 , b2 , 0xDD );
	b2		= _mm_shuffle_ps( b2 , b2 , 0x88 );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4 , (__m64*)&src2[28] ), (__m64*)&src2[34] );
	b5		= _mm_shuffle_ps( b4 , b4 , 0xDD );
	b4		= _mm_shuffle_ps( b4 , b4 , 0x88 );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_mul_ps( row, b0 );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b1 ) );	

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 2] ), (__m64*)&src1[ 8] );
	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[18] );
	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_mul_ps( row, b0 );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b1 ) );	

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[14] ), (__m64*)&src1[20] );
	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );	
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b3 ) );
	
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[16] ), (__m64*)&src1[22] );	
	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	tmp		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( tmp, b5 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[24] ), (__m64*)&src1[26] );
	row		= _mm_shuffle_ps( tmp, tmp, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_shuffle_ps( tmp, tmp, 0x55 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0xAA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0xFF );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_shuffle_ps( row, row, 0xFF );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );

	return dmat;

} // Mult01_5x6_6x6

SMLXTmpMatrix& __fastcall Mult10_5x6_5x6(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[12] ), (__m64*)&src2[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[18] ), (__m64*)&src2[20] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[24] ), (__m64*)&src2[26] );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating first 4 elements in the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[30], rslt );
	_mm_storeh_pi( (__m64*)&dst[32], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[ 4] );
	b0		= _mm_shuffle_ps( b0, b0, 0x44 );

	b1		= _mm_loadl_pi( b1, (__m64*)&src2[10] );
	b1		= _mm_shuffle_ps( b1, b1, 0x44 );

	b2		= _mm_loadl_pi( b2, (__m64*)&src2[16] );
	b2		= _mm_shuffle_ps( b2, b2, 0x44 );

	b3		= _mm_loadl_pi( b3, (__m64*)&src2[22] );
	b3		= _mm_shuffle_ps( b3, b3, 0x44 );

	b4		= _mm_loadl_pi( b4, (__m64*)&src2[28] );
	b4		= _mm_shuffle_ps( b4, b4, 0x44 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 0] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 6] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[12] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[18] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[24] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 2] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 8] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[14] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[20] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[26] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 4] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[10] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[16] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[22] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[28] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );
	_mm_storeh_pi( (__m64*)&dst[34], rslt );

	return dmat;

} // Mult10_5x6_5x6

SMLXTmpMatrix& __fastcall Mult11_5x6_6x5(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	//	Loading first 4 columns of the src2.

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 0] ), (__m64*)&src2[ 5] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[10] ), (__m64*)&src2[15] );	
	b0		= _mm_shuffle_ps( b5, b1, 0x88 );
	b1		= _mm_shuffle_ps( b5, b1, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 2] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[12] ), (__m64*)&src2[17] );
	b2		= _mm_shuffle_ps( b5, b3, 0x88 );
	b3		= _mm_shuffle_ps( b5, b3, 0xDD );

	b5		= _mm_loadh_pi( _mm_load_ss( &src2[ 4] ), (__m64*)&src2[ 9] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[14] ), (__m64*)&src2[19] );
	b4		= _mm_shuffle_ps( b5, b4, 0x88 );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating first 4 elements in the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[30], rslt );
	_mm_storeh_pi( (__m64*)&dst[32], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src2[20] ), (__m64*)&src2[25] );
	b1		= _mm_shuffle_ps( b0, b0, 0xDD );
	b0		= _mm_shuffle_ps( b0, b0, 0x88 );

	b2		= _mm_loadh_pi( _mm_loadl_pi( b2 , (__m64*)&src2[22] ), (__m64*)&src2[27] );
	b3		= _mm_shuffle_ps( b2, b2, 0xDD );
	b2		= _mm_shuffle_ps( b2, b2, 0x88 );

	b4		= _mm_loadh_pi( _mm_load_ss( &src2[29] ), (__m64*)&src2[24] );
	b4		= _mm_shuffle_ps( b4, b4, 0x22 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 0] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 6] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[12] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[18] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[24] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 2] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 8] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[14] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[20] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[26] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 4] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[10] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[16] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[22] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[28] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );
	_mm_storeh_pi( (__m64*)&dst[34], rslt );

	return dmat;

} // Mult11_5x6_6x5

SMLXTmpMatrix& __fastcall Mult00_5x6_6x5(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[10] ), (__m64*)&src2[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[15] ), (__m64*)&src2[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[20] ), (__m64*)&src2[22] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[25] ), (__m64*)&src2[27] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[1] * src2[9];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[2] * src2[14];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[3] * src2[19];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[4] * src2[24];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[5] * src2[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;
	
	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[6] * src2[4];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[9];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[8] * src2[14];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[9] * src2[19];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[10] * src2[24];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[11] * src2[29];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[12] * src2[4];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[13] * src2[9];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[14];

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[15] * src2[19];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[16] * src2[24];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[17] * src2[29];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[18] * src2[4];

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[19] * src2[9];

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[20] * src2[14];

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[21] * src2[19];

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[22] * src2[24];

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[23] * src2[29];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[24] * src2[4];

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[25] * src2[9];

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[26] * src2[14];

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[27] * src2[19];

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[28] * src2[24];

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[29] * src2[29];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	return dmat;

} // Mult00_5x6_6x5

SMLXTmpMatrix& __fastcall Mult01_5x6_5x6(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	float	f;

	//	Loading first 4 rows of the src2 and transposing.

	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src2[ 0] ), (__m64*)&src2[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[12] ), (__m64*)&src2[18] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src2[ 2] ), (__m64*)&src2[ 8] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[14] ), (__m64*)&src2[20] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src2[ 4] ), (__m64*)&src2[10] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[16] ), (__m64*)&src2[22] );
	b4		= _mm_shuffle_ps( b5, tmp, 0x88 );
	b5		= _mm_shuffle_ps( b5, tmp, 0xDD );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[24];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[1] * src2[25];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[2] * src2[26];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[3] * src2[27];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[4] * src2[28];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[5] * src2[29];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;
	
	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[6] * src2[24];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[25];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[8] * src2[26];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[9] * src2[27];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[10] * src2[28];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[11] * src2[29];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[12] * src2[24];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[13] * src2[25];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[26];

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[15] * src2[27];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[16] * src2[28];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[17] * src2[29];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[18] * src2[24];

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[19] * src2[25];

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[20] * src2[26];

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[21] * src2[27];

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[22] * src2[28];

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[23] * src2[29];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[24] * src2[24];

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[25] * src2[25];

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[26] * src2[26];

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[27] * src2[27];

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[28] * src2[28];

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );
	f		+= src1[29] * src2[29];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	return dmat;

} // Mult01_5x6_5x6

SMLXTmpMatrix& __fastcall Mult10_5x6_5x5(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[10] ), (__m64*)&src2[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[15] ), (__m64*)&src2[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[20] ), (__m64*)&src2[22] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[6] * src2[9];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[12] * src2[14];

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[18] * src2[19];

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[24] * src2[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[1] * src2[4];
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[9];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[13] * src2[14];

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[19] * src2[19];

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[25] * src2[24];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[2] * src2[4];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[8] * src2[9];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[14];

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[20] * src2[19];

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[26] * src2[24];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[3] * src2[4];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[9] * src2[9];

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[15] * src2[14];

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[21] * src2[19];

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[27] * src2[24];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[4] * src2[4];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[10] * src2[9];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[16] * src2[14];

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[22] * src2[19];

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[28] * src2[24];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[5] * src2[4];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[11] * src2[9];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[17] * src2[14];

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[23] * src2[19];

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[29] * src2[24];

	_mm_storel_pi( (__m64*)&dst[25], rslt );
	_mm_storeh_pi( (__m64*)&dst[27], rslt );
	dst[29]	= f;

	return dmat;

} // Mult10_5x6_5x5

SMLXTmpMatrix& __fastcall Mult11_5x6_5x5(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	float	f;

	//	Loading first 4 columns of the src2.

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 0] ), (__m64*)&src2[ 5] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[10] ), (__m64*)&src2[15] );	
	b0		= _mm_shuffle_ps( b5, b1, 0x88 );
	b1		= _mm_shuffle_ps( b5, b1, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 2] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[12] ), (__m64*)&src2[17] );
	b2		= _mm_shuffle_ps( b5, b3, 0x88 );
	b3		= _mm_shuffle_ps( b5, b3, 0xDD );

	b5		= _mm_loadh_pi( _mm_load_ss( &src2[ 4] ), (__m64*)&src2[ 9] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[14] ), (__m64*)&src2[19] );
	b4		= _mm_shuffle_ps( b5, b4, 0x88 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[20];

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[6] * src2[21];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[12] * src2[22];

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[18] * src2[23];

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[24] * src2[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[1] * src2[20];
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[21];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[13] * src2[22];

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[19] * src2[23];

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[25] * src2[24];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[2] * src2[20];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[8] * src2[21];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[22];

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[20] * src2[23];

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[26] * src2[24];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[3] * src2[20];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[9] * src2[21];

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[15] * src2[22];

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[21] * src2[23];

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[27] * src2[24];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[4] * src2[20];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[10] * src2[21];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[16] * src2[22];

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[22] * src2[23];

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[28] * src2[24];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[5] * src2[20];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[11] * src2[21];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[17] * src2[22];

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[23] * src2[23];

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[29] * src2[24];

	_mm_storel_pi( (__m64*)&dst[25], rslt );
	_mm_storeh_pi( (__m64*)&dst[27], rslt );
	dst[29]	= f;

	return dmat;

} // Mult11_5x6_5x5

SMLXTmpMatrix& __fastcall Mult00_5x6_6x3(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src2[ 3] ), (__m64*)&src2[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src2[ 6] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src2[ 9] ), (__m64*)&src2[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[12] ), (__m64*)&src2[13] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src2[15] ), (__m64*)&src2[16] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	return dmat;

} // Mult00_5x6_6x3

SMLXTmpMatrix& __fastcall Mult01_5x6_3x6(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading the src2 and transposing.

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1 , (__m64*)&src2[ 6] ), (__m64*)&src2[12] );
	b0		= _mm_shuffle_ps( tmp, b1, 0x80 );
	b1		= _mm_shuffle_ps( tmp, b1, 0xD5 );

	b3		= _mm_loadh_pi( _mm_loadl_pi( b3 , (__m64*)&src2[ 8] ), (__m64*)&src2[14] );
	b2		= _mm_shuffle_ps( tmp, b3, 0x8A );
	b3		= _mm_shuffle_ps( tmp, b3, 0xDF );

	tmp		= _mm_loadl_pi( tmp, (__m64*)&src2[4] );
	b5		= _mm_loadh_pi( _mm_loadl_pi( b5 , (__m64*)&src2[10] ), (__m64*)&src2[16] );
	b4		= _mm_shuffle_ps( tmp, b5, 0x80 );
	b5		= _mm_shuffle_ps( tmp, b5, 0xD5 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[ 9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b5 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	return dmat;

} // Mult01_5x6_3x6

SMLXTmpMatrix& __fastcall Mult10_5x6_5x3(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src2[ 3] ), (__m64*)&src2[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src2[ 6] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src2[ 9] ), (__m64*)&src2[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[12] ), (__m64*)&src2[13] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult10_5x6_5x3

SMLXTmpMatrix& __fastcall Mult11_5x6_3x5(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[ 0] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[10] );	
	b0		= _mm_shuffle_ps( b0, b1, 0x84 );
	b1		= _mm_shuffle_ps( b0, b1, 0xD5 );

	b2		= _mm_loadl_pi( b2, (__m64*)&src2[ 2] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[7] ), (__m64*)&src2[12] );
	b2		= _mm_shuffle_ps( b2, b3, 0x84 );
	b3		= _mm_shuffle_ps( b2, b3, 0xD5 );

	b4		= _mm_load_ss( &src2[ 4] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src2[9] ), (__m64*)&src2[14] );
	b4		= _mm_shuffle_ps( b4, b5, 0x80 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[25] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[26] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[27] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[28] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	//	Calculating the sixth row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[29] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult11_5x6_3x5

SMLXTmpMatrix& __fastcall Mult10_5x6_5x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	//	Calculating first 4 elements of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 6] ), (__m64*)&src1[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[12] ), (__m64*)&src1[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[18] ), (__m64*)&src1[20] );	
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src1[24] ), (__m64*)&src1[26] );
	
	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating last 2 elements of the destination matrix.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[0] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[2] );
	b2		= _mm_load_ss( &src2[4] );

	b0		= _mm_shuffle_ps( b0, b0, 0x50 );
	b1		= _mm_shuffle_ps( b1, b1, 0x50 );
	b2		= _mm_shuffle_ps( b2, b2, 0x50 );
	
	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[16] ), (__m64*)&src1[22] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[28] ), (__m64*)&src1[34] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	rslt	= _mm_add_ps( rslt, _mm_shuffle_ps( rslt, rslt, 0x4E ) );
	
	_mm_storel_pi( (__m64*)&dst[4], rslt );

	return dmat;

} // Mult10_5x6_5x1

SMLXTmpMatrix& __fastcall Mult00_1x5_5x6(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	//	Calculating first 4 elements of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 6] ), (__m64*)&src1[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[12] ), (__m64*)&src1[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[18] ), (__m64*)&src1[20] );	
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src1[24] ), (__m64*)&src1[26] );
	
	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating last 2 elements of the destination matrix.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[0] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[2] );
	b2		= _mm_load_ss( &src2[4] );

	b0		= _mm_shuffle_ps( b0, b0, 0x50 );
	b1		= _mm_shuffle_ps( b1, b1, 0x50 );
	b2		= _mm_shuffle_ps( b2, b2, 0x50 );
	
	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[16] ), (__m64*)&src1[22] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[28] ), (__m64*)&src1[34] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	rslt	= _mm_add_ps( rslt, _mm_shuffle_ps( rslt, rslt, 0x4E ) );
	
	_mm_storel_pi( (__m64*)&dst[4], rslt );

	return dmat;

} // Mult00_1x5_5x6

SMLXTmpMatrix& __fastcall Mult11_5x6_1x5(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	//	Calculating first 4 elements of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 6] ), (__m64*)&src1[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[12] ), (__m64*)&src1[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[18] ), (__m64*)&src1[20] );	
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src1[24] ), (__m64*)&src1[26] );
	
	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating last 2 elements of the destination matrix.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[0] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[2] );
	b2		= _mm_load_ss( &src2[4] );

	b0		= _mm_shuffle_ps( b0, b0, 0x50 );
	b1		= _mm_shuffle_ps( b1, b1, 0x50 );
	b2		= _mm_shuffle_ps( b2, b2, 0x50 );
	
	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[16] ), (__m64*)&src1[22] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[28] ), (__m64*)&src1[34] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	rslt	= _mm_add_ps( rslt, _mm_shuffle_ps( rslt, rslt, 0x4E ) );
	
	_mm_storel_pi( (__m64*)&dst[4], rslt );

	return dmat;

} // Mult11_5x6_1x5

SMLXTmpMatrix& __fastcall Mult10_5x1_5x6(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	//	Calculating first 4 elements of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 6] ), (__m64*)&src1[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[12] ), (__m64*)&src1[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[18] ), (__m64*)&src1[20] );	
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src1[24] ), (__m64*)&src1[26] );
	
	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating last 2 elements of the destination matrix.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[0] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[2] );
	b2		= _mm_load_ss( &src2[4] );

	b0		= _mm_shuffle_ps( b0, b0, 0x50 );
	b1		= _mm_shuffle_ps( b1, b1, 0x50 );
	b2		= _mm_shuffle_ps( b2, b2, 0x50 );
	
	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[16] ), (__m64*)&src1[22] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadh_pi( _mm_loadl_pi( row, (__m64*)&src1[28] ), (__m64*)&src1[34] );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	rslt	= _mm_add_ps( rslt, _mm_shuffle_ps( rslt, rslt, 0x4E ) );
	
	_mm_storel_pi( (__m64*)&dst[4], rslt );

	return dmat;

} // Mult10_5x1_5x6

SMLXTmpMatrix& __fastcall Mult00_3x5_5x6(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt, tmp;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[12] ), (__m64*)&src2[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[18] ), (__m64*)&src2[20] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[24] ), (__m64*)&src2[26] );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating last 2 columns of the destination matrix.

	tmp		= _mm_loadl_pi( tmp, (__m64*)&src2[ 4] );
	b1		= _mm_loadl_pi(  b1, (__m64*)&src2[10] );
	b2		= _mm_loadl_pi(  b2, (__m64*)&src2[16] );
	b3		= _mm_loadl_pi(  b3, (__m64*)&src2[22] );
	b4		= _mm_loadl_pi(  b4, (__m64*)&src2[28] );

	b0		= _mm_shuffle_ps( tmp, b1, 0x44 );
	b1		= _mm_shuffle_ps( b1,  b2, 0x44 );
	b2		= _mm_shuffle_ps( b2,  b3, 0x44 );
	b3		= _mm_shuffle_ps( b3,  b4, 0x44 );
	b4		= _mm_shuffle_ps( b4, tmp, 0x44 );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 2] ), (__m64*)&src1[ 8] );
	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 4] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_loadl_pi( row, (__m64*)&src1[10] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_loadl_pi( row, (__m64*)&src1[12] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	rslt	= _mm_add_ps( rslt, _mm_shuffle_ps( rslt, rslt, 0x4E ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	
	return dmat;

} // Mult00_3x5_5x6

SMLXTmpMatrix& __fastcall Mult01_3x5_6x5(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading first 4 columns of the src2.

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 0] ), (__m64*)&src2[ 5] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[10] ), (__m64*)&src2[15] );	
	b0		= _mm_shuffle_ps( b5, b1, 0x88 );
	b1		= _mm_shuffle_ps( b5, b1, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 2] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[12] ), (__m64*)&src2[17] );
	b2		= _mm_shuffle_ps( b5, b3, 0x88 );
	b3		= _mm_shuffle_ps( b5, b3, 0xDD );

	b5		= _mm_loadh_pi( _mm_load_ss( &src2[ 4] ), (__m64*)&src2[ 9] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[14] ), (__m64*)&src2[19] );
	b4		= _mm_shuffle_ps( b5, b4, 0x88 );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[20] ), (__m64*)&src2[25] );
	b1		= _mm_shuffle_ps( b0,  b0, 0xDD );
	b0		= _mm_shuffle_ps( b0,  b0, 0x88 );

	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[22] ), (__m64*)&src2[27] );
	b3		= _mm_shuffle_ps( b2,  b2, 0xDD );
	b2		= _mm_shuffle_ps( b2,  b2, 0x88 );

	b4		= _mm_loadh_pi( _mm_load_ss( &src2[29] ), (__m64*)&src2[24] );
	b4		= _mm_shuffle_ps( b4, b4, 0x22 );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 0] ), (__m64*)&src1[ 5] );
	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 2] ), (__m64*)&src1[ 7] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadh_pi( _mm_load_ss( &src1[9] ), (__m64*)&src1[4] );

	row		= _mm_shuffle_ps( row, row, 0x0A );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[11] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult01_3x5_6x5

SMLXTmpMatrix& __fastcall Mult10_3x5_3x6(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[12] ), (__m64*)&src2[14] );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[ 3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[ 8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[ 4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[ 9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadl_pi( b0 , (__m64*)&src2[ 4] );
	b0		= _mm_shuffle_ps( b0 , b0 , 0x44 );
	b1		= _mm_loadl_pi( b1 , (__m64*)&src2[10] );
	b1		= _mm_shuffle_ps( b1 , b1 , 0x44 );
	b2		= _mm_loadl_pi( b2 , (__m64*)&src2[16] );
	b2		= _mm_shuffle_ps( b2 , b2 , 0x44 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 0] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 5] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 7] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	row		= _mm_load_ss( &src1[ 4] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[ 9] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );	

	return dmat;

} // Mult10_3x5_3x6

SMLXTmpMatrix& __fastcall Mult11_3x5_6x3(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt, tmp1, tmp2, tmp3;

	//	Loading first 2 rows of the src2 and mixing.

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	tmp3	= _mm_loadh_pi( _mm_loadl_pi( tmp3, (__m64*)&src2[ 4] ), (__m64*)&src2[10] );

	b0		= _mm_shuffle_ps( tmp1, tmp2, 0xCC );
	b1		= _mm_shuffle_ps( tmp1, tmp3, 0x49 );
	tmp1	= _mm_shuffle_ps( tmp2, tmp3, 0xE9 );
	b2		= _mm_shuffle_ps( b1  , tmp1, 0xDD );
	b1		= _mm_shuffle_ps( b1  , tmp1, 0x88 );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[ 3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[ 8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[ 4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[ 9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[12] ), (__m64*)&src2[14] );
	tmp1	= _mm_loadl_pi( tmp1, (__m64*)&src2[16] );

	b2		= _mm_shuffle_ps( b0, tmp1, 0x49 );
	b0		= _mm_shuffle_ps( b0, b0, 0xCC );

	b1		= _mm_shuffle_ps( b2, b2, 0x88 );
	b2		= _mm_shuffle_ps( b2, b2, 0xDD );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 0] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 5] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 7] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	row		= _mm_load_ss( &src1[ 4] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[ 9] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );

	return dmat;

} // Mult11_3x5_6x3

SMLXTmpMatrix& __fastcall Mult00_3x5_5x5(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[10] ), (__m64*)&src2[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[15] ), (__m64*)&src2[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[20] ), (__m64*)&src2[22] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[1] * src2[9];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[2] * src2[14];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[3] * src2[19];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[4] * src2[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;
	
	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[5] * src2[4];

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[6] * src2[9];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[7] * src2[14];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[8] * src2[19];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[9] * src2[24];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[10] * src2[4];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[11] * src2[9];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[12] * src2[14];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[13] * src2[19];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[14] * src2[24];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	return dmat;

} // Mult00_3x5_5x5

SMLXTmpMatrix& __fastcall Mult01_3x5_5x5(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 0] ), (__m64*)&src2[ 5] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[10] ), (__m64*)&src2[15] );	
	b0		= _mm_shuffle_ps( b5, b1, 0x88 );
	b1		= _mm_shuffle_ps( b5, b1, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 2] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[12] ), (__m64*)&src2[17] );
	b2		= _mm_shuffle_ps( b5, b3, 0x88 );
	b3		= _mm_shuffle_ps( b5, b3, 0xDD );

	b5		= _mm_loadh_pi( _mm_load_ss( &src2[ 4] ), (__m64*)&src2[ 9] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[14] ), (__m64*)&src2[19] );
	b4		= _mm_shuffle_ps( b5, b4, 0x88 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[20];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[1] * src2[21];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[2] * src2[22];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[3] * src2[23];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[4] * src2[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;
	
	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[5] * src2[20];

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[6] * src2[21];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[7] * src2[22];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[8] * src2[23];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[9] * src2[24];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[10] * src2[20];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[11] * src2[21];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[12] * src2[22];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[13] * src2[23];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[14] * src2[24];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	return dmat;

} // Mult01_3x5_5x5

SMLXTmpMatrix& __fastcall Mult10_3x5_3x5(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[10] ), (__m64*)&src2[12] );
	
	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[5] * src2[9];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[10] * src2[14];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;
	
	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[1] * src2[4];

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[6] * src2[9];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[11] * src2[14];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[2] * src2[4];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[9];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[12] * src2[14];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[3] * src2[4];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[8] * src2[9];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[13] * src2[14];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[4] * src2[4];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[9] * src2[9];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[14];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	return dmat;

} // Mult10_3x5_3x5

SMLXTmpMatrix& __fastcall Mult11_3x5_5x3(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt, tmp1, tmp2, tmp3;

	float	f;

	//	Loading the src2 and mixing.

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	tmp3	= _mm_loadh_pi( _mm_loadl_pi( tmp3, (__m64*)&src2[ 4] ), (__m64*)&src2[10] );

	b0		= _mm_shuffle_ps( tmp1, tmp2, 0xCC );
	b1		= _mm_shuffle_ps( tmp1, tmp3, 0x49 );
	tmp1	= _mm_shuffle_ps( tmp2, tmp3, 0xE9 );
	b2		= _mm_shuffle_ps( b1  , tmp1, 0xDD );
	b1		= _mm_shuffle_ps( b1  , tmp1, 0x88 );
	
	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[12];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[5] * src2[13];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[10] * src2[14];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;
	
	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[1] * src2[12];

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[6] * src2[13];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[11] * src2[14];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[2] * src2[12];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[13];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[12] * src2[14];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[3] * src2[12];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[8] * src2[13];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[13] * src2[14];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[4] * src2[12];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[9] * src2[13];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[14];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	return dmat;

} // Mult11_3x5_5x3

SMLXTmpMatrix& __fastcall Mult00_3x5_5x3(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src2[ 3] ), (__m64*)&src2[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src2[ 6] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src2[ 9] ), (__m64*)&src2[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[12] ), (__m64*)&src2[13] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	return dmat;

} // Mult00_3x5_5x3

SMLXTmpMatrix& __fastcall Mult01_3x5_3x5(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[ 0] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[10] );	
	b0		= _mm_shuffle_ps( b0, b1, 0x84 );
	b1		= _mm_shuffle_ps( b0, b1, 0xD5 );

	b2		= _mm_loadl_pi( b2, (__m64*)&src2[ 2] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[7] ), (__m64*)&src2[12] );
	b2		= _mm_shuffle_ps( b2, b3, 0x84 );
	b3		= _mm_shuffle_ps( b2, b3, 0xD5 );

	b4		= _mm_load_ss( &src2[ 4] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src2[9] ), (__m64*)&src2[14] );
	b4		= _mm_shuffle_ps( b4, b5, 0x80 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	return dmat;

} // Mult01_3x5_3x5

SMLXTmpMatrix& __fastcall Mult10_3x5_3x3(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src2[ 3] ), (__m64*)&src2[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src2[ 6] ), (__m64*)&src2[ 7] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	return dmat;

} // Mult10_3x5_3x3

SMLXTmpMatrix& __fastcall Mult11_3x5_3x3(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	//	Loading the src2.

	b2		= _mm_loadh_pi( _mm_load_ss( &src2[0] ), (__m64*)&src2[1] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[4] ), (__m64*)&src2[7] );
	b0		= _mm_loadh_pi( _mm_load_ss( &src2[3] ), (__m64*)&src2[6] );

	b0		= _mm_shuffle_ps( b2, b0, 0x80 );
	b2		= _mm_shuffle_ps( b2, b1, 0xDB );
	b1		= _mm_shuffle_ps( b2, b1, 0x81 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	return dmat;

} // Mult11_3x5_3x3

SMLXTmpMatrix& __fastcall Mult00_3x5_5x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	// Loading the src1 and transposing.

	b0		= _mm_loadl_pi( b0, (__m64*)&src1[ 0] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 5] ), (__m64*)&src1[10] );	
	b0		= _mm_shuffle_ps( b0, b1, 0x84 );
	b1		= _mm_shuffle_ps( b0, b1, 0xD5 );

	b2		= _mm_loadl_pi( b2, (__m64*)&src1[ 2] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[7] ), (__m64*)&src1[12] );
	b2		= _mm_shuffle_ps( b2, b3, 0x84 );
	b3		= _mm_shuffle_ps( b2, b3, 0xD5 );

	b4		= _mm_load_ss( &src1[ 4] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src1[9] ), (__m64*)&src1[14] );
	b4		= _mm_shuffle_ps( b4, b5, 0x80 );

	//	Calculating the destination matrix.
	
	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult00_3x5_5x1

SMLXTmpMatrix& __fastcall Mult01_1x5_3x5(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	// Loading the src1 and transposing.

	b0		= _mm_loadl_pi( b0, (__m64*)&src1[ 0] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 5] ), (__m64*)&src1[10] );	
	b0		= _mm_shuffle_ps( b0, b1, 0x84 );
	b1		= _mm_shuffle_ps( b0, b1, 0xD5 );

	b2		= _mm_loadl_pi( b2, (__m64*)&src1[ 2] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[7] ), (__m64*)&src1[12] );
	b2		= _mm_shuffle_ps( b2, b3, 0x84 );
	b3		= _mm_shuffle_ps( b2, b3, 0xD5 );

	b4		= _mm_load_ss( &src1[ 4] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src1[9] ), (__m64*)&src1[14] );
	b4		= _mm_shuffle_ps( b4, b5, 0x80 );

	//	Calculating the destination matrix.
	
	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult01_1x5_3x5

SMLXTmpMatrix& __fastcall Mult01_3x5_1x5(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	// Loading the src1 and transposing.

	b0		= _mm_loadl_pi( b0, (__m64*)&src1[ 0] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 5] ), (__m64*)&src1[10] );	
	b0		= _mm_shuffle_ps( b0, b1, 0x84 );
	b1		= _mm_shuffle_ps( b0, b1, 0xD5 );

	b2		= _mm_loadl_pi( b2, (__m64*)&src1[ 2] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[7] ), (__m64*)&src1[12] );
	b2		= _mm_shuffle_ps( b2, b3, 0x84 );
	b3		= _mm_shuffle_ps( b2, b3, 0xD5 );

	b4		= _mm_load_ss( &src1[ 4] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src1[9] ), (__m64*)&src1[14] );
	b4		= _mm_shuffle_ps( b4, b5, 0x80 );

	//	Calculating the destination matrix.
	
	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult01_3x5_1x5

SMLXTmpMatrix& __fastcall Mult11_5x1_3x5(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	// Loading the src1 and transposing.

	b0		= _mm_loadl_pi( b0, (__m64*)&src1[ 0] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 5] ), (__m64*)&src1[10] );	
	b0		= _mm_shuffle_ps( b0, b1, 0x84 );
	b1		= _mm_shuffle_ps( b0, b1, 0xD5 );

	b2		= _mm_loadl_pi( b2, (__m64*)&src1[ 2] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[7] ), (__m64*)&src1[12] );
	b2		= _mm_shuffle_ps( b2, b3, 0x84 );
	b3		= _mm_shuffle_ps( b2, b3, 0xD5 );

	b4		= _mm_load_ss( &src1[ 4] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src1[9] ), (__m64*)&src1[14] );
	b4		= _mm_shuffle_ps( b4, b5, 0x80 );

	//	Calculating the destination matrix.
	
	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult11_5x1_3x5

SMLXTmpMatrix& __fastcall Mult10_3x5_3x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 5] ), (__m64*)&src1[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[10] ), (__m64*)&src1[12] );
	
	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src2[0] * src1[4];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[9];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[14];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult10_3x5_3x1

SMLXTmpMatrix& __fastcall Mult11_3x5_1x3(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 5] ), (__m64*)&src1[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[10] ), (__m64*)&src1[12] );
	
	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src2[0] * src1[4];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[9];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[14];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult11_3x5_1x3

SMLXTmpMatrix& __fastcall Mult00_5x3_3x6(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt, tmp1, tmp2;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[12] ), (__m64*)&src2[14] );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadl_pi( b0 , (__m64*)&src2[ 4] );
	b0		= _mm_shuffle_ps( b0 , b0 , 0x44 );
	b1		= _mm_loadl_pi( b1 , (__m64*)&src2[10] );
	b1		= _mm_shuffle_ps( b1 , b1 , 0x44 );
	b2		= _mm_loadl_pi( b2 , (__m64*)&src2[16] );
	b2		= _mm_shuffle_ps( b2 , b2 , 0x44 );

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	row		= _mm_shuffle_ps( tmp1, tmp1, 0xF0 );
	rslt	= _mm_mul_ps( row, b0 );

	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src1[ 4] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( tmp1, tmp2, 0x05 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_shuffle_ps( tmp1, tmp2, 0x5A );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src1[ 8] ), (__m64*)&src1[10] );

	row		= _mm_shuffle_ps( tmp2, tmp1, 0x5A );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_shuffle_ps( tmp2, tmp1, 0xAF );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_shuffle_ps( tmp1, tmp1, 0xF0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );

	return dmat;

} // Mult00_5x3_3x6

SMLXTmpMatrix& __fastcall Mult01_5x3_6x3(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt, tmp1, tmp2, tmp3;

	//	Loading first 2 rows of the src2 and mixing.

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	tmp3	= _mm_loadh_pi( _mm_loadl_pi( tmp3, (__m64*)&src2[ 4] ), (__m64*)&src2[10] );

	b0		= _mm_shuffle_ps( tmp1, tmp2, 0xCC );
	b1		= _mm_shuffle_ps( tmp1, tmp3, 0x49 );
	tmp1	= _mm_shuffle_ps( tmp2, tmp3, 0xE9 );
	b2		= _mm_shuffle_ps( b1  , tmp1, 0xDD );
	b1		= _mm_shuffle_ps( b1  , tmp1, 0x88 );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[12] ), (__m64*)&src2[14] );
	tmp1	= _mm_loadl_pi( tmp1, (__m64*)&src2[16] );

	b2		= _mm_shuffle_ps( b0, tmp1, 0x49 );
	b0		= _mm_shuffle_ps( b0, b0, 0xCC );

	b1		= _mm_shuffle_ps( b2, b2, 0x88 );
	b2		= _mm_shuffle_ps( b2, b2, 0xDD );

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	row		= _mm_shuffle_ps( tmp1, tmp1, 0xF0 );
	rslt	= _mm_mul_ps( row, b0 );

	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src1[ 4] ), (__m64*)&src1[ 6] );
	row		= _mm_shuffle_ps( tmp1, tmp2, 0x05 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_shuffle_ps( tmp1, tmp2, 0x5A );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src1[ 8] ), (__m64*)&src1[10] );

	row		= _mm_shuffle_ps( tmp2, tmp1, 0x5A );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_shuffle_ps( tmp2, tmp1, 0xAF );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_shuffle_ps( tmp1, tmp1, 0xF0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );

	return dmat;

} // Mult01_5x3_6x3

SMLXTmpMatrix& __fastcall Mult10_5x3_5x6(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[12] ), (__m64*)&src2[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[18] ), (__m64*)&src2[20] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[24] ), (__m64*)&src2[26] );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[ 4] );
	b0		= _mm_shuffle_ps( b0, b0, 0x44 );

	b1		= _mm_loadl_pi( b1, (__m64*)&src2[10] );
	b1		= _mm_shuffle_ps( b1, b1, 0x44 );

	b2		= _mm_loadl_pi( b2, (__m64*)&src2[16] );
	b2		= _mm_shuffle_ps( b2, b2, 0x44 );

	b3		= _mm_loadl_pi( b3, (__m64*)&src2[22] );
	b3		= _mm_shuffle_ps( b3, b3, 0x44 );

	b4		= _mm_loadl_pi( b4, (__m64*)&src2[28] );
	b4		= _mm_shuffle_ps( b4, b4, 0x44 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 0] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 3] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 6] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 9] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[12] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[ 5] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[ 8] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult10_5x3_5x6

SMLXTmpMatrix& __fastcall Mult11_5x3_6x5(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	//	Loading first 4 columns of the src2.

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 0] ), (__m64*)&src2[ 5] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[10] ), (__m64*)&src2[15] );	
	b0		= _mm_shuffle_ps( b5, b1, 0x88 );
	b1		= _mm_shuffle_ps( b5, b1, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 2] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[12] ), (__m64*)&src2[17] );
	b2		= _mm_shuffle_ps( b5, b3, 0x88 );
	b3		= _mm_shuffle_ps( b5, b3, 0xDD );

	b5		= _mm_loadh_pi( _mm_load_ss( &src2[ 4] ), (__m64*)&src2[ 9] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[14] ), (__m64*)&src2[19] );
	b4		= _mm_shuffle_ps( b5, b4, 0x88 );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src2[20] ), (__m64*)&src2[25] );
	b1		= _mm_shuffle_ps( b0, b0, 0xDD );
	b0		= _mm_shuffle_ps( b0, b0, 0x88 );

	b2		= _mm_loadh_pi( _mm_loadl_pi( b2 , (__m64*)&src2[22] ), (__m64*)&src2[27] );
	b3		= _mm_shuffle_ps( b2, b2, 0xDD );
	b2		= _mm_shuffle_ps( b2, b2, 0x88 );

	b4		= _mm_loadh_pi( _mm_load_ss( &src2[29] ), (__m64*)&src2[24] );
	b4		= _mm_shuffle_ps( b4, b4, 0x22 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[0] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[3] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[6] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[9] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[12] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult11_5x3_6x5

SMLXTmpMatrix& __fastcall Mult00_5x3_3x5(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[10] ), (__m64*)&src2[12] );
	
	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[1] * src2[9];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[2] * src2[14];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;
	
	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[3] * src2[4];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[4] * src2[9];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[5] * src2[14];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[6] * src2[4];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[9];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[8] * src2[14];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[9] * src2[4];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[10] * src2[9];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[11] * src2[14];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[12] * src2[4];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[13] * src2[9];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[14];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	return dmat;

} // Mult00_5x3_3x5

SMLXTmpMatrix& __fastcall Mult01_5x3_5x3(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt, tmp1, tmp2, tmp3;

	float	f;

	//	Loading the src2 and mixing.

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	tmp3	= _mm_loadh_pi( _mm_loadl_pi( tmp3, (__m64*)&src2[ 4] ), (__m64*)&src2[10] );

	b0		= _mm_shuffle_ps( tmp1, tmp2, 0xCC );
	b1		= _mm_shuffle_ps( tmp1, tmp3, 0x49 );
	tmp1	= _mm_shuffle_ps( tmp2, tmp3, 0xE9 );
	b2		= _mm_shuffle_ps( b1  , tmp1, 0xDD );
	b1		= _mm_shuffle_ps( b1  , tmp1, 0x88 );
	
	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[12];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[1] * src2[13];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[2] * src2[14];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;
	
	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[3] * src2[12];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[4] * src2[13];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[5] * src2[14];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[6] * src2[12];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[13];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[8] * src2[14];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[9] * src2[12];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[10] * src2[13];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[11] * src2[14];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[12] * src2[12];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[13] * src2[13];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[14];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	return dmat;

} // Mult01_5x3_5x3

SMLXTmpMatrix& __fastcall Mult10_5x3_5x5(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[10] ), (__m64*)&src2[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[15] ), (__m64*)&src2[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[20] ), (__m64*)&src2[22] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[3] * src2[9];

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[6] * src2[14];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[9] * src2[19];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[12] * src2[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[1] * src2[4];
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[4] * src2[9];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[7] * src2[14];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[10] * src2[19];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[13] * src2[24];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[2] * src2[4];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[5] * src2[9];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[8] * src2[14];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[11] * src2[19];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[14] * src2[24];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	return dmat;

} // Mult10_5x3_5x5

SMLXTmpMatrix& __fastcall Mult11_5x3_5x5(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	float	f;

	//	Loading first 4 columns of the src2.

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 0] ), (__m64*)&src2[ 5] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[10] ), (__m64*)&src2[15] );	
	b0		= _mm_shuffle_ps( b5, b1, 0x88 );
	b1		= _mm_shuffle_ps( b5, b1, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 2] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[12] ), (__m64*)&src2[17] );
	b2		= _mm_shuffle_ps( b5, b3, 0x88 );
	b3		= _mm_shuffle_ps( b5, b3, 0xDD );

	b5		= _mm_loadh_pi( _mm_load_ss( &src2[ 4] ), (__m64*)&src2[ 9] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[14] ), (__m64*)&src2[19] );
	b4		= _mm_shuffle_ps( b5, b4, 0x88 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[20];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[3] * src2[21];

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[6] * src2[22];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[9] * src2[23];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[12] * src2[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[1] * src2[20];
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[4] * src2[21];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[7] * src2[22];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[10] * src2[23];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[13] * src2[24];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[2] * src2[20];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[5] * src2[21];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[8] * src2[22];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[11] * src2[23];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[14] * src2[24];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	return dmat;

} // Mult11_5x3_5x5

SMLXTmpMatrix& __fastcall Mult00_5x3_3x3(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src2[ 3] ), (__m64*)&src2[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src2[ 6] ), (__m64*)&src2[ 7] );
	
	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	return dmat;

} // Mult00_5x3_3x3

SMLXTmpMatrix& __fastcall Mult01_5x3_3x3(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	//	Loading the src2 and transposing.

	b2		= _mm_loadh_pi( _mm_load_ss( &src2[0] ), (__m64*)&src2[1] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[4] ), (__m64*)&src2[7] );
	b0		= _mm_loadh_pi( _mm_load_ss( &src2[3] ), (__m64*)&src2[6] );

	b0		= _mm_shuffle_ps( b2, b0, 0x80 );
	b2		= _mm_shuffle_ps( b2, b1, 0xDB );
	b1		= _mm_shuffle_ps( b2, b1, 0x81 );
	
	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	return dmat;

} // Mult01_5x3_3x3

SMLXTmpMatrix& __fastcall Mult10_5x3_5x3(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src2[ 3] ), (__m64*)&src2[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src2[ 6] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src2[ 9] ), (__m64*)&src2[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[12] ), (__m64*)&src2[13] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	return dmat;

} // Mult10_5x3_5x3

SMLXTmpMatrix& __fastcall Mult11_5x3_3x5(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[ 0] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[10] );	
	b0		= _mm_shuffle_ps( b0, b1, 0x84 );
	b1		= _mm_shuffle_ps( b0, b1, 0xD5 );

	b2		= _mm_loadl_pi( b2, (__m64*)&src2[ 2] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[7] ), (__m64*)&src2[12] );
	b2		= _mm_shuffle_ps( b2, b3, 0x84 );
	b3		= _mm_shuffle_ps( b2, b3, 0xD5 );

	b4		= _mm_load_ss( &src2[ 4] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src2[9] ), (__m64*)&src2[14] );
	b4		= _mm_shuffle_ps( b4, b5, 0x80 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	return dmat;

} // Mult11_5x3_3x5

SMLXTmpMatrix& __fastcall Mult00_5x3_3x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt, tmp1, tmp2, tmp3;

	float	f;

	//	Loading the src2 and mixing.

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src1[ 6] ), (__m64*)&src1[ 8] );
	tmp3	= _mm_loadh_pi( _mm_loadl_pi( tmp3, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );

	b0		= _mm_shuffle_ps( tmp1, tmp2, 0xCC );
	b1		= _mm_shuffle_ps( tmp1, tmp3, 0x49 );
	tmp1	= _mm_shuffle_ps( tmp2, tmp3, 0xE9 );
	b2		= _mm_shuffle_ps( b1  , tmp1, 0xDD );
	b1		= _mm_shuffle_ps( b1  , tmp1, 0x88 );
	
	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src2[0] * src1[12];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[13];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[14];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult00_5x3_3x1

SMLXTmpMatrix& __fastcall Mult01_5x3_1x3(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt, tmp1, tmp2, tmp3;

	float	f;

	//	Loading the src2 and mixing.

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src1[ 6] ), (__m64*)&src1[ 8] );
	tmp3	= _mm_loadh_pi( _mm_loadl_pi( tmp3, (__m64*)&src1[ 4] ), (__m64*)&src1[10] );

	b0		= _mm_shuffle_ps( tmp1, tmp2, 0xCC );
	b1		= _mm_shuffle_ps( tmp1, tmp3, 0x49 );
	tmp1	= _mm_shuffle_ps( tmp2, tmp3, 0xE9 );
	b2		= _mm_shuffle_ps( b1  , tmp1, 0xDD );
	b1		= _mm_shuffle_ps( b1  , tmp1, 0x88 );
	
	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src2[0] * src1[12];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[13];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[14];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult01_5x3_1x3

SMLXTmpMatrix& __fastcall Mult10_5x3_5x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src1[ 0] ), (__m64*)&src1[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src1[ 3] ), (__m64*)&src1[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src1[ 6] ), (__m64*)&src1[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src1[ 9] ), (__m64*)&src1[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src1[12] ), (__m64*)&src1[13] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult10_5x3_5x1

SMLXTmpMatrix& __fastcall Mult00_1x5_5x3(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src1[ 0] ), (__m64*)&src1[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src1[ 3] ), (__m64*)&src1[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src1[ 6] ), (__m64*)&src1[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src1[ 9] ), (__m64*)&src1[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src1[12] ), (__m64*)&src1[13] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult00_1x5_5x3

SMLXTmpMatrix& __fastcall Mult11_5x3_1x5(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src1[ 0] ), (__m64*)&src1[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src1[ 3] ), (__m64*)&src1[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src1[ 6] ), (__m64*)&src1[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src1[ 9] ), (__m64*)&src1[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src1[12] ), (__m64*)&src1[13] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult11_5x3_1x5

SMLXTmpMatrix& __fastcall Mult10_5x1_5x3(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src1[ 0] ), (__m64*)&src1[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src1[ 3] ), (__m64*)&src1[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src1[ 6] ), (__m64*)&src1[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src1[ 9] ), (__m64*)&src1[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src1[12] ), (__m64*)&src1[13] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	return dmat;

} // Mult10_5x1_5x3


SMLXTmpMatrix& __fastcall Mult00_5x5_5x6(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[12] ), (__m64*)&src2[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[18] ), (__m64*)&src2[20] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[24] ), (__m64*)&src2[26] );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating last 2 columns of the destination matrix.

	tmp		= _mm_loadl_pi( tmp, (__m64*)&src2[ 4] );
	b1		= _mm_loadl_pi(  b1, (__m64*)&src2[10] );
	b2		= _mm_loadl_pi(  b2, (__m64*)&src2[16] );
	b3		= _mm_loadl_pi(  b3, (__m64*)&src2[22] );
	b4		= _mm_loadl_pi(  b4, (__m64*)&src2[28] );

	b0		= _mm_shuffle_ps( tmp, b1, 0x44 );
	b1		= _mm_shuffle_ps( b1,  b2, 0x44 );
	b2		= _mm_shuffle_ps( b2,  b3, 0x44 );
	b3		= _mm_shuffle_ps( b3,  b4, 0x44 );
	b4		= _mm_shuffle_ps( b4, tmp, 0x44 );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 0] ), (__m64*)&src1[ 6] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 2] ), (__m64*)&src1[ 8] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 4] );

	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[10] ), (__m64*)&src1[16] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[18] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[14] );

	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	
	row		= _mm_loadl_pi( row, (__m64*)&src1[20] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_loadl_pi( row, (__m64*)&src1[22] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	rslt	= _mm_add_ps( rslt, _mm_shuffle_ps( rslt, rslt, 0x4E ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );

	return dmat;

} // Mult00_5x5_5x6

SMLXTmpMatrix& __fastcall Mult01_5x5_6x5(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	//	Loading first 4 columns of the src2.

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 0] ), (__m64*)&src2[ 5] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[10] ), (__m64*)&src2[15] );	
	b0		= _mm_shuffle_ps( b5, b1, 0x88 );
	b1		= _mm_shuffle_ps( b5, b1, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 2] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[12] ), (__m64*)&src2[17] );
	b2		= _mm_shuffle_ps( b5, b3, 0x88 );
	b3		= _mm_shuffle_ps( b5, b3, 0xDD );

	b5		= _mm_loadh_pi( _mm_load_ss( &src2[ 4] ), (__m64*)&src2[ 9] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[14] ), (__m64*)&src2[19] );
	b4		= _mm_shuffle_ps( b5, b4, 0x88 );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[20] ), (__m64*)&src2[25] );
	b1		= _mm_shuffle_ps( b0,  b0, 0xDD );
	b0		= _mm_shuffle_ps( b0,  b0, 0x88 );

	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[22] ), (__m64*)&src2[27] );
	b3		= _mm_shuffle_ps( b2,  b2, 0xDD );
	b2		= _mm_shuffle_ps( b2,  b2, 0x88 );

	b4		= _mm_loadh_pi( _mm_load_ss( &src2[29] ), (__m64*)&src2[24] );
	b4		= _mm_shuffle_ps( b4, b4, 0x22 );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 0] ), (__m64*)&src1[ 5] );
	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 2] ), (__m64*)&src1[ 7] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadh_pi( _mm_load_ss( &src1[9] ), (__m64*)&src1[4] );

	row		= _mm_shuffle_ps( row, row, 0x0A );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[10] ), (__m64*)&src1[15] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[12] ), (__m64*)&src1[17] );

	row		= _mm_shuffle_ps( tmp, tmp, 0xA0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_shuffle_ps( tmp, tmp, 0xF5 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadh_pi( _mm_load_ss( &src1[19] ), (__m64*)&src1[14] );

	row		= _mm_shuffle_ps( row, row, 0x0A );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	
	row		= _mm_load_ss( &src1[20] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[21] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );

	return dmat;

} // Mult01_5x5_6x5

SMLXTmpMatrix& __fastcall Mult10_5x5_5x6(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[12] ), (__m64*)&src2[14] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[18] ), (__m64*)&src2[20] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[24] ), (__m64*)&src2[26] );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[ 4] );
	b0		= _mm_shuffle_ps( b0, b0, 0x44 );

	b1		= _mm_loadl_pi( b1, (__m64*)&src2[10] );
	b1		= _mm_shuffle_ps( b1, b1, 0x44 );

	b2		= _mm_loadl_pi( b2, (__m64*)&src2[16] );
	b2		= _mm_shuffle_ps( b2, b2, 0x44 );

	b3		= _mm_loadl_pi( b3, (__m64*)&src2[22] );
	b3		= _mm_shuffle_ps( b3, b3, 0x44 );

	b4		= _mm_loadl_pi( b4, (__m64*)&src2[28] );
	b4		= _mm_shuffle_ps( b4, b4, 0x44 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 0] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 5] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[10] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[15] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[20] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 2] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 7] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[12] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[17] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[22] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	row		= _mm_load_ss( &src1[ 4] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[ 9] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );

	return dmat;

} // Mult10_5x5_5x6

SMLXTmpMatrix& __fastcall Mult11_5x5_6x5(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	//	Loading first 4 columns of the src2.

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 0] ), (__m64*)&src2[ 5] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[10] ), (__m64*)&src2[15] );	
	b0		= _mm_shuffle_ps( b5, b1, 0x88 );
	b1		= _mm_shuffle_ps( b5, b1, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 2] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[12] ), (__m64*)&src2[17] );
	b2		= _mm_shuffle_ps( b5, b3, 0x88 );
	b3		= _mm_shuffle_ps( b5, b3, 0xDD );

	b5		= _mm_loadh_pi( _mm_load_ss( &src2[ 4] ), (__m64*)&src2[ 9] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[14] ), (__m64*)&src2[19] );
	b4		= _mm_shuffle_ps( b5, b4, 0x88 );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating first 4 elements in the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[18], rslt );
	_mm_storeh_pi( (__m64*)&dst[20], rslt );

	//	Calculating first 4 elements in the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[24], rslt );
	_mm_storeh_pi( (__m64*)&dst[26], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0 , (__m64*)&src2[20] ), (__m64*)&src2[25] );
	b1		= _mm_shuffle_ps( b0, b0, 0xDD );
	b0		= _mm_shuffle_ps( b0, b0, 0x88 );

	b2		= _mm_loadh_pi( _mm_loadl_pi( b2 , (__m64*)&src2[22] ), (__m64*)&src2[27] );
	b3		= _mm_shuffle_ps( b2, b2, 0xDD );
	b2		= _mm_shuffle_ps( b2, b2, 0x88 );

	b4		= _mm_loadh_pi( _mm_load_ss( &src2[29] ), (__m64*)&src2[24] );
	b4		= _mm_shuffle_ps( b4, b4, 0x22 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 0] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 5] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[10] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[15] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[20] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 2] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[ 7] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[12] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[17] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[22] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );

	row		= _mm_load_ss( &src1[ 4] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[ 9] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_unpacklo_ps( row, row );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_storel_pi( (__m64*)&dst[28], rslt );

	return dmat;

} // Mult11_5x5_6x5

SMLXTmpMatrix& __fastcall Mult00_5x5_5x5(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[10] ), (__m64*)&src2[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[15] ), (__m64*)&src2[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[20] ), (__m64*)&src2[22] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );	
	f		= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[1] * src2[9];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[2] * src2[14];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[3] * src2[19];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[4] * src2[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[5] * src2[4];
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[6] * src2[9];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[7] * src2[14];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[8] * src2[19];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[9] * src2[24];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[10] * src2[4];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[11] * src2[9];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[12] * src2[14];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[13] * src2[19];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[14] * src2[24];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[15] * src2[4];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[16] * src2[9];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[17] * src2[14];

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[18] * src2[19];

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[19] * src2[24];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[20] * src2[4];

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[21] * src2[9];

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[22] * src2[14];

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[23] * src2[19];

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[24] * src2[24];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	return dmat;

} // Mult00_5x5_5x5

SMLXTmpMatrix& __fastcall Mult01_5x5_5x5(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	float	f;

	//	Loading first 4 columns of the src2.

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 0] ), (__m64*)&src2[ 5] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[10] ), (__m64*)&src2[15] );	
	b0		= _mm_shuffle_ps( b5, b1, 0x88 );
	b1		= _mm_shuffle_ps( b5, b1, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 2] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[12] ), (__m64*)&src2[17] );
	b2		= _mm_shuffle_ps( b5, b3, 0x88 );
	b3		= _mm_shuffle_ps( b5, b3, 0xDD );

	b5		= _mm_loadh_pi( _mm_load_ss( &src2[ 4] ), (__m64*)&src2[ 9] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[14] ), (__m64*)&src2[19] );
	b4		= _mm_shuffle_ps( b5, b4, 0x88 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[20];

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[1] * src2[21];

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[2] * src2[22];

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[3] * src2[23];

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[4] * src2[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[5] * src2[20];
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[6] * src2[21];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[7] * src2[22];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[8] * src2[23];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[9] * src2[24];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[10] * src2[20];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[11] * src2[21];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[12] * src2[22];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[13] * src2[23];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[14] * src2[24];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[15] * src2[20];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[16] * src2[21];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[17] * src2[22];

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[18] * src2[23];

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[19] * src2[24];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[20] * src2[20];

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[21] * src2[21];

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[22] * src2[22];

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[23] * src2[23];

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[24] * src2[24];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	return dmat;

} // Mult01_5x5_5x5

SMLXTmpMatrix& __fastcall Mult10_5x5_5x5(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[10] ), (__m64*)&src2[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[15] ), (__m64*)&src2[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src2[20] ), (__m64*)&src2[22] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[4];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[5] * src2[9];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[10] * src2[14];

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[15] * src2[19];

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[20] * src2[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[1] * src2[4];
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[6] * src2[9];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[11] * src2[14];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[16] * src2[19];

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[21] * src2[24];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[2] * src2[4];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[9];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[12] * src2[14];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[17] * src2[19];

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[22] * src2[24];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[3] * src2[4];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[8] * src2[9];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[13] * src2[14];

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[18] * src2[19];

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[23] * src2[24];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[4] * src2[4];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[9] * src2[9];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[14];

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[19] * src2[19];

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[24] * src2[24];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	return dmat;

} // Mult10_5x5_5x5

SMLXTmpMatrix& __fastcall Mult11_5x5_5x5(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt, tmp;

	float	f;

	//	Loading first 4 columns of the src2.

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 0] ), (__m64*)&src2[ 5] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[10] ), (__m64*)&src2[15] );	
	b0		= _mm_shuffle_ps( b5, b1, 0x88 );
	b1		= _mm_shuffle_ps( b5, b1, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src2[ 2] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[12] ), (__m64*)&src2[17] );
	b2		= _mm_shuffle_ps( b5, b3, 0x88 );
	b3		= _mm_shuffle_ps( b5, b3, 0xDD );

	b5		= _mm_loadh_pi( _mm_load_ss( &src2[ 4] ), (__m64*)&src2[ 9] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[14] ), (__m64*)&src2[19] );
	b4		= _mm_shuffle_ps( b5, b4, 0x88 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[0] * src2[20];

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[5] * src2[21];

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[10] * src2[22];

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[15] * src2[23];

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[20] * src2[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[1] * src2[20];
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[6] * src2[21];

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[11] * src2[22];

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[16] * src2[23];

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[21] * src2[24];

	_mm_storel_pi( (__m64*)&dst[5], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );
	dst[9]	= f;

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[2] * src2[20];

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[7] * src2[21];

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[12] * src2[22];

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[17] * src2[23];

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[22] * src2[24];

	_mm_storel_pi( (__m64*)&dst[10], rslt );
	_mm_storeh_pi( (__m64*)&dst[12], rslt );
	dst[14]	= f;

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[3] * src2[20];

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[8] * src2[21];

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[13] * src2[22];

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[18] * src2[23];

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[23] * src2[24];

	_mm_storel_pi( (__m64*)&dst[15], rslt );
	_mm_storeh_pi( (__m64*)&dst[17], rslt );
	dst[19]	= f;

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src1[4] * src2[20];

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src1[9] * src2[21];

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src1[14] * src2[22];

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src1[19] * src2[23];

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src1[24] * src2[24];

	_mm_storel_pi( (__m64*)&dst[20], rslt );
	_mm_storeh_pi( (__m64*)&dst[22], rslt );
	dst[24]	= f;

	return dmat;

} // Mult11_5x5_5x5

SMLXTmpMatrix& __fastcall Mult00_5x5_5x3(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src2[ 3] ), (__m64*)&src2[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src2[ 6] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src2[ 9] ), (__m64*)&src2[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[12] ), (__m64*)&src2[13] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	return dmat;

} // Mult00_5x5_5x3

SMLXTmpMatrix& __fastcall Mult01_5x5_3x5(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[ 0] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[10] );	
	b0		= _mm_shuffle_ps( b0, b1, 0x84 );
	b1		= _mm_shuffle_ps( b0, b1, 0xD5 );

	b2		= _mm_loadl_pi( b2, (__m64*)&src2[ 2] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[7] ), (__m64*)&src2[12] );
	b2		= _mm_shuffle_ps( b2, b3, 0x84 );
	b3		= _mm_shuffle_ps( b2, b3, 0xD5 );

	b4		= _mm_load_ss( &src2[ 4] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src2[9] ), (__m64*)&src2[14] );
	b4		= _mm_shuffle_ps( b4, b5, 0x80 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	return dmat;

} // Mult01_5x5_3x5

SMLXTmpMatrix& __fastcall Mult10_5x5_5x3(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );
	b1		= _mm_loadh_pi( _mm_load_ss( &src2[ 3] ), (__m64*)&src2[ 4] );
	b2		= _mm_loadh_pi( _mm_load_ss( &src2[ 6] ), (__m64*)&src2[ 7] );
	b3		= _mm_loadh_pi( _mm_load_ss( &src2[ 9] ), (__m64*)&src2[10] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src2[12] ), (__m64*)&src2[13] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	return dmat;

} // Mult10_5x5_5x3

SMLXTmpMatrix& __fastcall Mult11_5x5_3x5(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,3)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	//	Loading the src2.

	b0		= _mm_loadl_pi( b0, (__m64*)&src2[ 0] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 5] ), (__m64*)&src2[10] );	
	b0		= _mm_shuffle_ps( b0, b1, 0x84 );
	b1		= _mm_shuffle_ps( b0, b1, 0xD5 );

	b2		= _mm_loadl_pi( b2, (__m64*)&src2[ 2] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[7] ), (__m64*)&src2[12] );
	b2		= _mm_shuffle_ps( b2, b3, 0x84 );
	b3		= _mm_shuffle_ps( b2, b3, 0xD5 );

	b4		= _mm_load_ss( &src2[ 4] );
	b5		= _mm_loadh_pi( _mm_load_ss( &src2[9] ), (__m64*)&src2[14] );
	b4		= _mm_shuffle_ps( b4, b5, 0x80 );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[20] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[1], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[16] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[21] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[3], rslt );
	_mm_storeh_pi( (__m64*)&dst[4], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[17] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[22] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[7], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[18] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[23] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[9], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[19] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	row		= _mm_load_ss( &src1[24] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );

	_mm_store_ss( &dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[13], rslt );

	return dmat;

} // Mult11_5x5_3x5

SMLXTmpMatrix& __fastcall Mult00_5x5_5x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src1[ 0] ), (__m64*)&src1[ 5] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[10] ), (__m64*)&src1[15] );	
	b0		= _mm_shuffle_ps( b5, b1, 0x88 );
	b1		= _mm_shuffle_ps( b5, b1, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src1[ 2] ), (__m64*)&src1[ 7] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[12] ), (__m64*)&src1[17] );
	b2		= _mm_shuffle_ps( b5, b3, 0x88 );
	b3		= _mm_shuffle_ps( b5, b3, 0xDD );

	b5		= _mm_loadh_pi( _mm_load_ss( &src1[ 4] ), (__m64*)&src1[ 9] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src1[14] ), (__m64*)&src1[19] );
	b4		= _mm_shuffle_ps( b5, b4, 0x88 );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src2[0] * src1[20];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[21];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[22];

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src2[3] * src1[23];

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src2[4] * src1[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult00_5x5_5x1

SMLXTmpMatrix& __fastcall Mult11_5x1_5x5(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src1[ 0] ), (__m64*)&src1[ 5] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[10] ), (__m64*)&src1[15] );	
	b0		= _mm_shuffle_ps( b5, b1, 0x88 );
	b1		= _mm_shuffle_ps( b5, b1, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src1[ 2] ), (__m64*)&src1[ 7] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[12] ), (__m64*)&src1[17] );
	b2		= _mm_shuffle_ps( b5, b3, 0x88 );
	b3		= _mm_shuffle_ps( b5, b3, 0xDD );

	b5		= _mm_loadh_pi( _mm_load_ss( &src1[ 4] ), (__m64*)&src1[ 9] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src1[14] ), (__m64*)&src1[19] );
	b4		= _mm_shuffle_ps( b5, b4, 0x88 );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src2[0] * src1[20];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[21];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[22];

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src2[3] * src1[23];

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src2[4] * src1[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult11_5x1_5x5

SMLXTmpMatrix& __fastcall Mult01_5x5_1x5(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src1[ 0] ), (__m64*)&src1[ 5] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[10] ), (__m64*)&src1[15] );	
	b0		= _mm_shuffle_ps( b5, b1, 0x88 );
	b1		= _mm_shuffle_ps( b5, b1, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src1[ 2] ), (__m64*)&src1[ 7] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[12] ), (__m64*)&src1[17] );
	b2		= _mm_shuffle_ps( b5, b3, 0x88 );
	b3		= _mm_shuffle_ps( b5, b3, 0xDD );

	b5		= _mm_loadh_pi( _mm_load_ss( &src1[ 4] ), (__m64*)&src1[ 9] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src1[14] ), (__m64*)&src1[19] );
	b4		= _mm_shuffle_ps( b5, b4, 0x88 );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src2[0] * src1[20];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[21];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[22];

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src2[3] * src1[23];

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src2[4] * src1[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult01_5x5_1x5

SMLXTmpMatrix& __fastcall Mult01_1x5_5x5(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4, b5;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src1[ 0] ), (__m64*)&src1[ 5] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[10] ), (__m64*)&src1[15] );	
	b0		= _mm_shuffle_ps( b5, b1, 0x88 );
	b1		= _mm_shuffle_ps( b5, b1, 0xDD );

	b5		= _mm_loadh_pi( _mm_loadl_pi( b5, (__m64*)&src1[ 2] ), (__m64*)&src1[ 7] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[12] ), (__m64*)&src1[17] );
	b2		= _mm_shuffle_ps( b5, b3, 0x88 );
	b3		= _mm_shuffle_ps( b5, b3, 0xDD );

	b5		= _mm_loadh_pi( _mm_load_ss( &src1[ 4] ), (__m64*)&src1[ 9] );
	b4		= _mm_loadh_pi( _mm_load_ss( &src1[14] ), (__m64*)&src1[19] );
	b4		= _mm_shuffle_ps( b5, b4, 0x88 );

	//	Calculating the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	f		= src2[0] * src1[20];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[21];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[22];

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src2[3] * src1[23];

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src2[4] * src1[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult01_1x5_5x5

SMLXTmpMatrix& __fastcall Mult10_5x5_5x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 5] ), (__m64*)&src1[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[10] ), (__m64*)&src1[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[15] ), (__m64*)&src1[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src1[20] ), (__m64*)&src1[22] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );	
	f		= src2[0] * src1[4];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[9];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[14];

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src2[3] * src1[19];

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src2[4] * src1[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult10_5x5_5x1

SMLXTmpMatrix& __fastcall Mult11_5x5_1x5(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 5] ), (__m64*)&src1[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[10] ), (__m64*)&src1[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[15] ), (__m64*)&src1[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src1[20] ), (__m64*)&src1[22] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );	
	f		= src2[0] * src1[4];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[9];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[14];

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src2[3] * src1[19];

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src2[4] * src1[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult11_5x5_1x5

SMLXTmpMatrix& __fastcall Mult00_1x5_5x5(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 5] ), (__m64*)&src1[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[10] ), (__m64*)&src1[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[15] ), (__m64*)&src1[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src1[20] ), (__m64*)&src1[22] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );	
	f		= src2[0] * src1[4];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[9];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[14];

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src2[3] * src1[19];

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src2[4] * src1[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult00_1x5_5x5

SMLXTmpMatrix& __fastcall Mult11_5x5_1x5(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 5] ), (__m64*)&src1[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[10] ), (__m64*)&src1[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[15] ), (__m64*)&src1[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src1[20] ), (__m64*)&src1[22] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );	
	f		= src2[0] * src1[4];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[9];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[14];

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src2[3] * src1[19];

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src2[4] * src1[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult11_5x5_1x5

SMLXTmpMatrix& __fastcall Mult10_5x1_5x5(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3, b4;
	__m128	row, rslt;

	float	f;

	//	Loading first 4 columns of the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 5] ), (__m64*)&src1[ 7] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[10] ), (__m64*)&src1[12] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[15] ), (__m64*)&src1[17] );
	b4		= _mm_loadh_pi( _mm_loadl_pi( b4, (__m64*)&src1[20] ), (__m64*)&src1[22] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );	
	f		= src2[0] * src1[4];

	row		= _mm_load_ss( &src2[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );
	f		+= src2[1] * src1[9];

	row		= _mm_load_ss( &src2[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );
	f		+= src2[2] * src1[14];

	row		= _mm_load_ss( &src2[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );
	f		+= src2[3] * src1[19];

	row		= _mm_load_ss( &src2[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b4 ) );
	f		+= src2[4] * src1[24];

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );
	dst[4]	= f;

	return dmat;

} // Mult10_5x1_5x5

SMLXTmpMatrix& __fastcall Mult00_5x1_1x6(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1;
	__m128	row, rslt;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[ 4] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );
	_mm_storel_pi( (__m64*)&dst[4], rslt);

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[8], row );
	_mm_storel_pi( (__m64*)&dst[10], rslt);

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[14], row );
	_mm_storel_pi( (__m64*)&dst[16], rslt);

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[18], row );
	_mm_storeh_pi( (__m64*)&dst[20], row );
	_mm_storel_pi( (__m64*)&dst[22], rslt);

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[24], row );
	_mm_storeh_pi( (__m64*)&dst[26], row );
	_mm_storel_pi( (__m64*)&dst[28], rslt);

	return dmat;

} // Mult00_5x1_1x6

SMLXTmpMatrix& __fastcall Mult01_5x1_6x1(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1;
	__m128	row, rslt;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[ 4] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );
	_mm_storel_pi( (__m64*)&dst[4], rslt);

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[8], row );
	_mm_storel_pi( (__m64*)&dst[10], rslt);

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[14], row );
	_mm_storel_pi( (__m64*)&dst[16], rslt);

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[18], row );
	_mm_storeh_pi( (__m64*)&dst[20], row );
	_mm_storel_pi( (__m64*)&dst[22], rslt);

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[24], row );
	_mm_storeh_pi( (__m64*)&dst[26], row );
	_mm_storel_pi( (__m64*)&dst[28], rslt);

	return dmat;

} // Mult01_5x1_6x1

SMLXTmpMatrix& __fastcall Mult10_1x5_1x6(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1;
	__m128	row, rslt;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[ 4] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );
	_mm_storel_pi( (__m64*)&dst[4], rslt);

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[8], row );
	_mm_storel_pi( (__m64*)&dst[10], rslt);

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[14], row );
	_mm_storel_pi( (__m64*)&dst[16], rslt);

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[18], row );
	_mm_storeh_pi( (__m64*)&dst[20], row );
	_mm_storel_pi( (__m64*)&dst[22], rslt);

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[24], row );
	_mm_storeh_pi( (__m64*)&dst[26], row );
	_mm_storel_pi( (__m64*)&dst[28], rslt);

	return dmat;

} // Mult10_1x5_1x6

SMLXTmpMatrix& __fastcall Mult11_1x5_6x1(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1;
	__m128	row, rslt;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadl_pi( b1, (__m64*)&src2[ 4] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );
	_mm_storel_pi( (__m64*)&dst[4], rslt);

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[8], row );
	_mm_storel_pi( (__m64*)&dst[10], rslt);

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[14], row );
	_mm_storel_pi( (__m64*)&dst[16], rslt);

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[18], row );
	_mm_storeh_pi( (__m64*)&dst[20], row );
	_mm_storel_pi( (__m64*)&dst[22], rslt);

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[24], row );
	_mm_storeh_pi( (__m64*)&dst[26], row );
	_mm_storel_pi( (__m64*)&dst[28], rslt);

	return dmat;

} // Mult11_1x5_6x1

SMLXTmpMatrix& __fastcall Mult00_5x1_1x5(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1;
	__m128	row, rslt;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_load_ss( &src2[ 4] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );
	_mm_store_ss( &dst[4], rslt);

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[5], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );
	_mm_store_ss( &dst[9], rslt);

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[10], row );
	_mm_storeh_pi( (__m64*)&dst[12], row );
	_mm_store_ss( &dst[14], rslt);

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[15], row );
	_mm_storeh_pi( (__m64*)&dst[17], row );
	_mm_store_ss( &dst[19], rslt);

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[20], row );
	_mm_storeh_pi( (__m64*)&dst[22], row );
	_mm_store_ss( &dst[24], rslt);

	return dmat;

} // Mult00_5x1_1x5

SMLXTmpMatrix& __fastcall Mult01_5x1_5x1(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1;
	__m128	row, rslt;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_load_ss( &src2[ 4] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );
	_mm_store_ss( &dst[4], rslt);

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[5], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );
	_mm_store_ss( &dst[9], rslt);

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[10], row );
	_mm_storeh_pi( (__m64*)&dst[12], row );
	_mm_store_ss( &dst[14], rslt);

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[15], row );
	_mm_storeh_pi( (__m64*)&dst[17], row );
	_mm_store_ss( &dst[19], rslt);

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[20], row );
	_mm_storeh_pi( (__m64*)&dst[22], row );
	_mm_store_ss( &dst[24], rslt);

	return dmat;

} // Mult01_5x1_5x1

SMLXTmpMatrix& __fastcall Mult10_1x5_1x5(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1;
	__m128	row, rslt;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_load_ss( &src2[ 4] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );
	_mm_store_ss( &dst[4], rslt);

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[5], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );
	_mm_store_ss( &dst[9], rslt);

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[10], row );
	_mm_storeh_pi( (__m64*)&dst[12], row );
	_mm_store_ss( &dst[14], rslt);

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[15], row );
	_mm_storeh_pi( (__m64*)&dst[17], row );
	_mm_store_ss( &dst[19], rslt);

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[20], row );
	_mm_storeh_pi( (__m64*)&dst[22], row );
	_mm_store_ss( &dst[24], rslt);

	return dmat;

} // Mult10_1x5_1x5

SMLXTmpMatrix& __fastcall Mult11_1x5_5x1(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,5)();
	register float* dst = dmat.Data();

	__m128	b0, b1;
	__m128	row, rslt;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_load_ss( &src2[ 4] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );
	_mm_store_ss( &dst[4], rslt);

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[5], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );
	_mm_store_ss( &dst[9], rslt);

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[10], row );
	_mm_storeh_pi( (__m64*)&dst[12], row );
	_mm_store_ss( &dst[14], rslt);

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[15], row );
	_mm_storeh_pi( (__m64*)&dst[17], row );
	_mm_store_ss( &dst[19], rslt);

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[20], row );
	_mm_storeh_pi( (__m64*)&dst[22], row );
	_mm_store_ss( &dst[24], rslt);

	return dmat;

} // Mult11_1x5_5x1

SMLXTmpMatrix& __fastcall Mult00_5x1_1x3(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,3)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );
	
	_mm_store_ss( &dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[1], row );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[3], row );
	_mm_storeh_pi( (__m64*)&dst[4], row );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[9], row );
	_mm_storeh_pi( (__m64*)&dst[10], row );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[13], row );

	return dmat;

} // Mult00_5x1_1x3

SMLXTmpMatrix& __fastcall Mult01_5x1_3x1(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,3)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );
	
	_mm_store_ss( &dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[1], row );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[3], row );
	_mm_storeh_pi( (__m64*)&dst[4], row );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[9], row );
	_mm_storeh_pi( (__m64*)&dst[10], row );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[13], row );

	return dmat;

} // Mult01_5x1_3x1

SMLXTmpMatrix& __fastcall Mult10_1x5_1x3(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,3)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );
	
	_mm_store_ss( &dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[1], row );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[3], row );
	_mm_storeh_pi( (__m64*)&dst[4], row );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[9], row );
	_mm_storeh_pi( (__m64*)&dst[10], row );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[13], row );

	return dmat;

} // Mult10_1x5_1x3

SMLXTmpMatrix& __fastcall Mult11_1x5_3x1(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,3)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[ 0] ), (__m64*)&src2[ 1] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );
	
	_mm_store_ss( &dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[1], row );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[3], row );
	_mm_storeh_pi( (__m64*)&dst[4], row );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[9], row );
	_mm_storeh_pi( (__m64*)&dst[10], row );

	//	Calculating the fifth row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[13], row );

	return dmat;

} // Mult11_1x5_3x1

SMLXTmpMatrix& __fastcall Mult00_5x1_1x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1;
	__m128	row, rslt;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_load_ss( &src1[ 4] );

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );
	_mm_store_ss( &dst[4], rslt);

	return dmat;

} // Mult00_5x1_1x1

SMLXTmpMatrix& __fastcall Mult01_5x1_1x1(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1;
	__m128	row, rslt;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_load_ss( &src1[ 4] );

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );
	_mm_store_ss( &dst[4], rslt);

	return dmat;

} // Mult01_5x1_1x1

SMLXTmpMatrix& __fastcall Mult10_1x5_1x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1;
	__m128	row, rslt;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_load_ss( &src1[ 4] );

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );
	_mm_store_ss( &dst[4], rslt);

	return dmat;

} // Mult10_1x5_1x1

SMLXTmpMatrix& __fastcall Mult11_1x5_1x1(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,5,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1;
	__m128	row, rslt;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_load_ss( &src1[ 4] );

	row		= _mm_load_ss( &src2[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ss( row, b1 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );
	_mm_store_ss( &dst[4], rslt);

	return dmat;

} // Mult11_1x5_1x1

SMLXTmpMatrix& __fastcall Mult00_1x5_5x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	dst[0] = src1[0]*src2[0] + src1[1]*src2[1] + src1[2]*src2[2] + src1[3]*src2[3] + src1[4]*src2[4];

	return dmat;

} // Mult00_1x5_5x1

SMLXTmpMatrix& __fastcall Mult01_1x5_1x5(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	dst[0] = src1[0]*src2[0] + src1[1]*src2[1] + src1[2]*src2[2] + src1[3]*src2[3] + src1[4]*src2[4];

	return dmat;

} // Mult01_1x5_1x5

SMLXTmpMatrix& __fastcall Mult10_5x1_5x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	dst[0] = src1[0]*src2[0] + src1[1]*src2[1] + src1[2]*src2[2] + src1[3]*src2[3] + src1[4]*src2[4];

	return dmat;

} // Mult10_5x1_5x1

SMLXTmpMatrix& __fastcall Mult11_5x1_1x5(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	dst[0] = src1[0]*src2[0] + src1[1]*src2[1] + src1[2]*src2[2] + src1[3]*src2[3] + src1[4]*src2[4];

	return dmat;

} // Mult11_5x1_1x5

SMLXTmpMatrix& __fastcall Mult00_4x1_1x4(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,4,4)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[4], row );
	_mm_storeh_pi( (__m64*)&dst[6], row );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[8], row );
	_mm_storeh_pi( (__m64*)&dst[10], row );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[14], row );

	return dmat;

} // Mult00_4x1_1x4

SMLXTmpMatrix& __fastcall Mult11_1x4_4x1(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,4,4)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[4], row );
	_mm_storeh_pi( (__m64*)&dst[6], row );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[8], row );
	_mm_storeh_pi( (__m64*)&dst[10], row );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[14], row );

	return dmat;

} // Mult11_1x4_4x1

SMLXTmpMatrix& __fastcall Mult10_1x4_1x4(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,4,4)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[4], row );
	_mm_storeh_pi( (__m64*)&dst[6], row );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[8], row );
	_mm_storeh_pi( (__m64*)&dst[10], row );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[14], row );

	return dmat;

} // Mult10_1x4_1x4

SMLXTmpMatrix& __fastcall Mult01_4x1_4x1(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,4,4)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[4], row );
	_mm_storeh_pi( (__m64*)&dst[6], row );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[8], row );
	_mm_storeh_pi( (__m64*)&dst[10], row );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[12], row );
	_mm_storeh_pi( (__m64*)&dst[14], row );

	return dmat;

} // Mult01_4x1_4x1

SMLXTmpMatrix& __fastcall Mult10_4x4_4x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,4,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3;
	__m128	row, rslt;

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 4] ), (__m64*)&src1[ 6] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[ 8] ), (__m64*)&src1[10] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[12] ), (__m64*)&src1[14] );

	row		= _mm_load_ss( &src2[0]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	return dmat;

} // Mult10_4x4_4x1

SMLXTmpMatrix& __fastcall Mult00_1x4_4x4(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,4)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3;
	__m128	row, rslt;

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 4] ), (__m64*)&src1[ 6] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[ 8] ), (__m64*)&src1[10] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[12] ), (__m64*)&src1[14] );

	row		= _mm_load_ss( &src2[0]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	return dmat;

} // Mult00_1x4_4x4

SMLXTmpMatrix& __fastcall Mult11_4x4_1x4(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,4,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3;
	__m128	row, rslt;

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 4] ), (__m64*)&src1[ 6] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[ 8] ), (__m64*)&src1[10] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[12] ), (__m64*)&src1[14] );

	row		= _mm_load_ss( &src2[0]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	return dmat;

} // Mult11_4x4_1x4

SMLXTmpMatrix& __fastcall Mult10_4x1_4x4(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,4)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3;
	__m128	row, rslt;

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src1[ 4] ), (__m64*)&src1[ 6] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src1[ 8] ), (__m64*)&src1[10] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src1[12] ), (__m64*)&src1[14] );

	row		= _mm_load_ss( &src2[0]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	return dmat;

} // Mult10_4x1_4x4

_declspec(naked)
SMLXTmpMatrix& __fastcall Mult00_4x4_4x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,4,1)();

	prologue
	
	__asm {

		movlps	xmm6, qword ptr [ecx   ]
		movlps	xmm0, qword ptr [ebx   ]

		shufps	xmm6, xmm6, 0x44
		movhps	xmm0, qword ptr [ebx+16]
		mulps	xmm0, xmm6
		movlps	xmm7, qword ptr [ecx+ 8]

		movlps	xmm2, qword ptr [ebx+ 8]
		shufps	xmm7, xmm7, 0x44
		movhps	xmm2, qword ptr [ebx+24]

		mulps	xmm2, xmm7
		movlps	xmm1, qword ptr [ebx+32]
		movhps	xmm1, qword ptr [ebx+48]
		
		mulps	xmm1, xmm6
		movlps	xmm3, qword ptr [ebx+40]
		addps	xmm0, xmm2
		movhps	xmm3, qword ptr [ebx+56]

		mulps	xmm3, xmm7

		movaps	xmm4, xmm0
		addps	xmm1, xmm3

		shufps	xmm4, xmm1, 0x88
		shufps	xmm0, xmm1, 0xDD

		addps	xmm0, xmm4

		movaps	xmmword ptr [eax], xmm0
	}

	epilogue

} // Mult00_4x4_4x1

SMLXTmpMatrix& __fastcall Mult11_4x1_4x4(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,4)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3;
	__m128	row, rslt, tmp;

	b1		= _mm_loadh_pi( _mm_loadl_pi(  b1, (__m64*)&src1[ 0] ), (__m64*)&src1[ 4] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 8] ), (__m64*)&src1[12] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi(  b3, (__m64*)&src1[ 2] ), (__m64*)&src1[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[10] ), (__m64*)&src1[14] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	row		= _mm_load_ss( &src2[0]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	return dmat;

} // Mult11_4x1_4x4

SMLXTmpMatrix& __fastcall Mult01_4x4_1x4(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,4,1)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3;
	__m128	row, rslt, tmp;

	b1		= _mm_loadh_pi( _mm_loadl_pi(  b1, (__m64*)&src1[ 0] ), (__m64*)&src1[ 4] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 8] ), (__m64*)&src1[12] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi(  b3, (__m64*)&src1[ 2] ), (__m64*)&src1[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[10] ), (__m64*)&src1[14] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	row		= _mm_load_ss( &src2[0]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	return dmat;

} // Mult01_4x4_1x4

SMLXTmpMatrix& __fastcall Mult01_1x4_4x4(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = B.Data();
	register float* src2 = A.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,4)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3;
	__m128	row, rslt, tmp;

	b1		= _mm_loadh_pi( _mm_loadl_pi(  b1, (__m64*)&src1[ 0] ), (__m64*)&src1[ 4] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[ 8] ), (__m64*)&src1[12] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi(  b3, (__m64*)&src1[ 2] ), (__m64*)&src1[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src1[10] ), (__m64*)&src1[14] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	row		= _mm_load_ss( &src2[0]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src2[1]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src2[2]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src2[3]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	return dmat;

} // Mult01_1x4_4x4

SMLXTmpMatrix& __fastcall Mult00_4x1_1x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,4,1)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	b0		= _mm_loadh_pi( _mm_loadl_pi(  b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	
	row		= _mm_load_ss( &src2[0]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );

	return dmat;

} // Mult00_4x1_1x1

SMLXTmpMatrix& __fastcall Mult10_1x4_1x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,4,1)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	b0		= _mm_loadh_pi( _mm_loadl_pi(  b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	
	row		= _mm_load_ss( &src2[0]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );

	return dmat;

} // Mult10_1x4_1x1

SMLXTmpMatrix& __fastcall Mult11_1x4_1x1(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,4,1)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	b0		= _mm_loadh_pi( _mm_loadl_pi(  b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	
	row		= _mm_load_ss( &src2[0]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );

	return dmat;

} // Mult11_1x4_1x1

SMLXTmpMatrix& __fastcall Mult01_4x1_1x1(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,4,1)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	b0		= _mm_loadh_pi( _mm_loadl_pi(  b0, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	
	row		= _mm_load_ss( &src2[0]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_storel_pi( (__m64*)&dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[2], row );

	return dmat;

} // Mult01_4x1_1x1

SMLXTmpMatrix& __fastcall Mult00_1x4_4x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	dst[0] = src1[0]*src2[0] + src1[1]*src2[1] + src1[2]*src2[2] + src1[3]*src2[3];

	return dmat;

} // Mult00_1x4_4x1

SMLXTmpMatrix& __fastcall Mult01_1x4_1x4(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	dst[0] = src1[0]*src2[0] + src1[1]*src2[1] + src1[2]*src2[2] + src1[3]*src2[3];

	return dmat;

} // Mult01_1x4_1x4

SMLXTmpMatrix& __fastcall Mult10_4x1_4x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	dst[0] = src1[0]*src2[0] + src1[1]*src2[1] + src1[2]*src2[2] + src1[3]*src2[3];

	return dmat;

} // Mult10_4x1_4x1

SMLXTmpMatrix& __fastcall Mult11_4x1_1x4(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	dst[0] = src1[0]*src2[0] + src1[1]*src2[1] + src1[2]*src2[2] + src1[3]*src2[3];

	return dmat;

} // Mult11_4x1_1x4

SMLXTmpMatrix& __fastcall Mult00_3x1_1x3(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,3)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[0] ), (__m64*)&src2[1] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[1], row );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[3], row );
	_mm_storeh_pi( (__m64*)&dst[4], row );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );

	return dmat;

} // Mult00_3x1_1x3

SMLXTmpMatrix& __fastcall Mult01_3x1_3x1(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,3)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[0] ), (__m64*)&src2[1] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[1], row );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[3], row );
	_mm_storeh_pi( (__m64*)&dst[4], row );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );

	return dmat;

} // Mult01_3x1_3x1

SMLXTmpMatrix& __fastcall Mult10_1x3_1x3(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,3)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[0] ), (__m64*)&src2[1] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[1], row );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[3], row );
	_mm_storeh_pi( (__m64*)&dst[4], row );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );

	return dmat;

} // Mult10_1x3_1x3

SMLXTmpMatrix& __fastcall Mult11_1x3_3x1(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,3)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	//	Calculating the first row of the destination matrix.

	b0		= _mm_loadh_pi( _mm_load_ss( &src2[0] ), (__m64*)&src2[1] );

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[1], row );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[3], row );
	_mm_storeh_pi( (__m64*)&dst[4], row );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[ 2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[6], row );
	_mm_storeh_pi( (__m64*)&dst[7], row );

	return dmat;

} // Mult11_1x3_3x1

SMLXTmpMatrix& __fastcall Mult00_3x1_1x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,1)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	b0		= _mm_loadh_pi( _mm_load_ss(  &src1[ 0] ), (__m64*)&src1[ 1] );
	
	row		= _mm_load_ss( &src2[0]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[1], row );

	return dmat;

} // Mult00_3x1_1x1

SMLXTmpMatrix& __fastcall Mult01_3x1_1x1(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,1)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	b0		= _mm_loadh_pi( _mm_load_ss(  &src1[ 0] ), (__m64*)&src1[ 1] );
	
	row		= _mm_load_ss( &src2[0]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[1], row );

	return dmat;

} // Mult01_3x1_1x1

SMLXTmpMatrix& __fastcall Mult10_1x3_1x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,1)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	b0		= _mm_loadh_pi( _mm_load_ss(  &src1[ 0] ), (__m64*)&src1[ 1] );
	
	row		= _mm_load_ss( &src2[0]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[1], row );

	return dmat;

} // Mult10_1x3_1x1

SMLXTmpMatrix& __fastcall Mult11_1x3_1x1(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,1)();
	register float* dst = dmat.Data();

	__m128	b0;
	__m128	row;

	b0		= _mm_loadh_pi( _mm_load_ss(  &src1[ 0] ), (__m64*)&src1[ 1] );
	
	row		= _mm_load_ss( &src2[0]);
	row		= _mm_shuffle_ps( row, row, 0x00 );
	row		= _mm_mul_ps( row, b0 );

	_mm_store_ss( &dst[0], row );
	_mm_storeh_pi( (__m64*)&dst[1], row );

	return dmat;

} // Mult11_1x3_1x1



SMLXTmpMatrix& __fastcall Mult00_1x3_3x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	dst[0] = src1[0]*src2[0] + src1[1]*src2[1] + src1[2]*src2[2];

	return dmat;

} // Mult00_1x3_3x1

SMLXTmpMatrix& __fastcall Mult11_3x1_1x3(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	dst[0] = src1[0]*src2[0] + src1[1]*src2[1] + src1[2]*src2[2];

	return dmat;

} // Mult11_3x1_1x3


SMLXTmpMatrix& __fastcall Mult01_1x3_1x3(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	dst[0] = src1[0]*src2[0] + src1[1]*src2[1] + src1[2]*src2[2];

	return dmat;

} // Mult01_1x3_1x3

SMLXTmpMatrix& __fastcall Mult10_3x1_3x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	dst[0] = src1[0]*src2[0] + src1[1]*src2[1] + src1[2]*src2[2];

	return dmat;

} // Mult10_3x1_3x1

__declspec(naked)
SMLXTmpMatrix& __fastcall Mult00_4x4_4x4(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,4,4)();

	prologue

	__asm {
		movss       xmm0,dword ptr [ebx]
		movaps      xmm1,xmmword ptr [ecx]
		shufps      xmm0,xmm0,0
		movss       xmm2,dword ptr [ebx+4]
		mulps       xmm0,xmm1
		shufps      xmm2,xmm2,0
		movaps      xmm3,xmmword ptr [ecx+10h]
		movss       xmm7,dword ptr [ebx+8]
		mulps       xmm2,xmm3
		shufps      xmm7,xmm7,0
		addps       xmm0,xmm2
		movaps      xmm4,xmmword ptr [ecx+20h]
		movss       xmm2,dword ptr [ebx+0Ch]
		mulps       xmm7,xmm4
		shufps      xmm2,xmm2,0
		addps       xmm0,xmm7
		movaps      xmm5,xmmword ptr [ecx+30h]
		movss       xmm6,dword ptr [ebx+10h]
		mulps       xmm2,xmm5
		movss       xmm7,dword ptr [ebx+14h]
		shufps      xmm6,xmm6,0
		addps       xmm0,xmm2
		shufps      xmm7,xmm7,0
		movlps      qword ptr [eax],xmm0
		movhps      qword ptr [eax+8],xmm0
		mulps       xmm7,xmm3
		movss       xmm0,dword ptr [ebx+18h]
		mulps       xmm6,xmm1
		shufps      xmm0,xmm0,0
		addps       xmm6,xmm7
		mulps       xmm0,xmm4
		movss       xmm2,dword ptr [ebx+24h]
		addps       xmm6,xmm0
		movss       xmm0,dword ptr [ebx+1Ch]
		movss       xmm7,dword ptr [ebx+20h]
		shufps      xmm0,xmm0,0
		shufps      xmm7,xmm7,0
		mulps       xmm0,xmm5
		mulps       xmm7,xmm1
		addps       xmm6,xmm0
		shufps      xmm2,xmm2,0
		movlps      qword ptr [eax+10h],xmm6
		movhps      qword ptr [eax+18h],xmm6
		mulps       xmm2,xmm3
		movss       xmm6,dword ptr [ebx+28h]
		addps       xmm7,xmm2
		shufps      xmm6,xmm6,0
		movss       xmm2,dword ptr [ebx+2Ch]
		mulps       xmm6,xmm4
		shufps      xmm2,xmm2,0
		addps       xmm7,xmm6
		mulps       xmm2,xmm5
		movss       xmm0,dword ptr [ebx+34h]
		addps       xmm7,xmm2
		shufps      xmm0,xmm0,0
		movlps      qword ptr [eax+20h],xmm7
		movss       xmm2,dword ptr [ebx+30h]
		movhps      qword ptr [eax+28h],xmm7
		mulps       xmm0,xmm3
		shufps      xmm2,xmm2,0
		movss       xmm6,dword ptr [ebx+38h]
		mulps       xmm2,xmm1
		shufps      xmm6,xmm6,0
		addps       xmm2,xmm0
		mulps       xmm6,xmm4
		movss       xmm7,dword ptr [ebx+3Ch]
		shufps      xmm7,xmm7,0
		addps       xmm2,xmm6
		mulps       xmm7,xmm5
		addps       xmm2,xmm7
		movaps      xmmword ptr [eax+30h],xmm2
	}

	epilogue

} // Mult00_4x4_4x4

SMLXTmpMatrix& __fastcall Mult01_4x4_4x4(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,4,4)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3;
	__m128	row, rslt, tmp;

	// Loading the src2.

	b1		= _mm_loadh_pi( _mm_loadl_pi(  b1, (__m64*)&src2[ 0] ), (__m64*)&src2[ 4] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[ 8] ), (__m64*)&src2[12] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi(  b3, (__m64*)&src2[ 2] ), (__m64*)&src2[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[10] ), (__m64*)&src2[14] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[4], rslt );
	_mm_storeh_pi( (__m64*)&dst[6], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[ 8], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	return dmat;

} // Mult01_4x4_4x4

SMLXTmpMatrix& __fastcall Mult10_4x4_4x4(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,4,4)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3;
	__m128	row, rslt;

	// Loading the src2.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 4] ), (__m64*)&src2[ 6] );
	b2		= _mm_loadh_pi( _mm_loadl_pi( b2, (__m64*)&src2[ 8] ), (__m64*)&src2[10] );
	b3		= _mm_loadh_pi( _mm_loadl_pi( b3, (__m64*)&src2[12] ), (__m64*)&src2[14] );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[4], rslt );
	_mm_storeh_pi( (__m64*)&dst[6], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[ 8], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	return dmat;

} // Mult10_4x4_4x4

SMLXTmpMatrix& __fastcall Mult11_4x4_4x4(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,4,4)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2, b3;
	__m128	row, rslt, tmp;

	// Loading the src2.

	b1		= _mm_loadh_pi( _mm_loadl_pi(  b1, (__m64*)&src2[ 0] ), (__m64*)&src2[ 4] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[ 8] ), (__m64*)&src2[12] );
	b0		= _mm_shuffle_ps( b1, tmp, 0x88 );
	b1		= _mm_shuffle_ps( b1, tmp, 0xDD );

	b3		= _mm_loadh_pi( _mm_loadl_pi(  b3, (__m64*)&src2[ 2] ), (__m64*)&src2[ 6] );
	tmp		= _mm_loadh_pi( _mm_loadl_pi( tmp, (__m64*)&src2[10] ), (__m64*)&src2[14] );
	b2		= _mm_shuffle_ps( b3, tmp, 0x88 );
	b3		= _mm_shuffle_ps( b3, tmp, 0xDD );

	//	Calculating the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[12] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[9] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[13] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[4], rslt );
	_mm_storeh_pi( (__m64*)&dst[6], rslt );

	//	Calculating the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[10] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[14] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[ 8], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	//	Calculating the fourth row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[11] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	row		= _mm_load_ss( &src1[15] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b3 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	return dmat;

} // Mult11_4x4_4x4

SMLXTmpMatrix& __fastcall Mult00_1x1_1x1(const SMLXMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	_mm_store_ss( &dst[0], _mm_mul_ss( _mm_load_ss( &src1[0] ), _mm_load_ss( &src2[0] ) ) );

	return dmat;

} // Mult00_1x1_1x1

SMLXTmpMatrix& __fastcall Mult01_1x1_1x1(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	_mm_store_ss( &dst[0], _mm_mul_ss( _mm_load_ss( &src1[0] ), _mm_load_ss( &src2[0] ) ) );

	return dmat;

} // Mult01_1x1_1x1

SMLXTmpMatrix& __fastcall Mult10_1x1_1x1(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	_mm_store_ss( &dst[0], _mm_mul_ss( _mm_load_ss( &src1[0] ), _mm_load_ss( &src2[0] ) ) );

	return dmat;

} // Mult10_1x1_1x1

SMLXTmpMatrix& __fastcall Mult11_1x1_1x1(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,1,1)();
	register float* dst = dmat.Data();

	_mm_store_ss( &dst[0], _mm_mul_ss( _mm_load_ss( &src1[0] ), _mm_load_ss( &src2[0] ) ) );

	return dmat;

} // Mult11_1x1_1x1

__declspec(naked)
SMLXTmpMatrix& __fastcall Mult00_1x1_1x6(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,1,6)();

	prologue

	__asm {

		movaps	xmm0, xmmword ptr [ecx]
		movlps	xmm1, qword ptr [ecx+16]
		movss	xmm2, dword ptr [ebx]
		
		shufps	xmm2, xmm2, 0

		mulps	xmm0, xmm2
		mulps	xmm1, xmm2

		movaps	xmmword ptr [eax], xmm0
		movlps	qword ptr [eax+16], xmm1
	}

	epilogue

} // Mult00_1x1_1x6

__declspec(naked)
SMLXTmpMatrix& __fastcall Mult01_1x1_6x1(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	nrxc(TMat,1,6)();

	prologue

	__asm {

		movaps	xmm0, xmmword ptr [ecx]
		movlps	xmm1, qword ptr [ecx+16]
		movss	xmm2, dword ptr [ebx]
		
		shufps	xmm2, xmm2, 0

		mulps	xmm0, xmm2
		mulps	xmm1, xmm2

		movaps	xmmword ptr [eax], xmm0
		movlps	qword ptr [eax+16], xmm1
	}

	epilogue

} // Mult01_1x1_6x1

__declspec(naked)
SMLXTmpMatrix& __fastcall Mult10_1x1_1x6(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,1,6)();

	prologue

	__asm {

		movaps	xmm0, xmmword ptr [ecx]
		movlps	xmm1, qword ptr [ecx+16]
		movss	xmm2, dword ptr [ebx]
		
		shufps	xmm2, xmm2, 0

		mulps	xmm0, xmm2
		mulps	xmm1, xmm2

		movaps	xmmword ptr [eax], xmm0
		movlps	qword ptr [eax+16], xmm1
	}

	epilogue

} // Mult10_1x1_1x6

__declspec(naked)
SMLXTmpMatrix& __fastcall Mult11_1x1_6x1(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	nrxc(TMat,1,6)();

	prologue

	__asm {

		movaps	xmm0, xmmword ptr [ecx]
		movlps	xmm1, qword ptr [ecx+16]
		movss	xmm2, dword ptr [ebx]
		
		shufps	xmm2, xmm2, 0

		mulps	xmm0, xmm2
		mulps	xmm1, xmm2

		movaps	xmmword ptr [eax], xmm0
		movlps	qword ptr [eax+16], xmm1
	}

	epilogue

} // Mult11_1x1_6x1

__declspec(naked)
SMLXTmpMatrix& __fastcall Mult00_3x3_3x6(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,3,6)();

	prologue

	__asm {

		movaps	xmm5, xmmword ptr [ecx]
		movlps	xmm6,   qword ptr [ecx+24]
		movhps	xmm6,   qword ptr [ecx+32]
		movaps	xmm7, xmmword ptr [ecx+48]

		movss	xmm0, dword ptr [ebx]
		shufps	xmm0, xmm0, 0
		mulps	xmm0, xmm5

		movss	xmm1, dword ptr [ebx+4]
		shufps	xmm1, xmm1, 0
		mulps	xmm1, xmm6

		movss	xmm2, dword ptr [ebx+8]
		shufps	xmm2, xmm2, 0
		mulps	xmm2, xmm7

		addps	xmm0, xmm1
		addps	xmm0, xmm2

		movaps	xmmword ptr [eax], xmm0

		movss	xmm3, dword ptr [ebx+12]
		shufps	xmm3, xmm3, 0
		mulps	xmm3, xmm5

		movss	xmm4, dword ptr [ebx+16]
		shufps	xmm4, xmm4, 0
		mulps	xmm4, xmm6

		movss	xmm0, dword ptr [ebx+20]
		shufps	xmm0, xmm0, 0
		mulps	xmm0, xmm7

		addps	xmm3, xmm4
		addps	xmm0, xmm3

		movlps	qword ptr [eax+24], xmm0
		movhps	qword ptr [eax+32], xmm0

		movss	xmm1, dword ptr [ebx+24]
		shufps	xmm1, xmm1, 0
		mulps	xmm1, xmm5

		movss	xmm2, dword ptr [ebx+28]
		shufps	xmm2, xmm2, 0
		mulps	xmm2, xmm6

		movss	xmm3, dword ptr [ebx+32]
		shufps	xmm3, xmm3, 0
		mulps	xmm3, xmm7

		addps	xmm1, xmm2
		addps	xmm1, xmm3

		movaps	xmmword ptr [eax+48], xmm1

		movlps	xmm5, qword ptr [ecx+16]
		movlps	xmm6, qword ptr [ecx+40]
		movlps	xmm7, qword ptr [ecx+64]

		shufps	xmm5, xmm5, 0x44
		shufps	xmm6, xmm6, 0x44
		shufps	xmm7, xmm7, 0x44

		movaps	xmm3, xmmword ptr [ebx]
		movlps	xmm4, qword ptr [ebx+16]

		movaps	xmm0, xmm3
		shufps	xmm0, xmm0, 0xF0
		mulps	xmm0, xmm5

		movaps	xmm1, xmm3
		shufps	xmm1, xmm4, 0x05
		mulps	xmm1, xmm6

		shufps	xmm3, xmm4, 0x5A
		mulps	xmm3, xmm7

		addps	xmm1, xmm0
		addps	xmm1, xmm3

		movlps	qword ptr [eax+16], xmm1
		movhps	qword ptr [eax+40], xmm1

		movss	xmm0, dword ptr [ebx+24]
		shufps	xmm0, xmm0, 0
		mulps	xmm0, xmm5

		movss	xmm2, dword ptr [ebx+28]
		shufps	xmm2, xmm2, 0
		mulps	xmm2, xmm6

		movss	xmm4, dword ptr [ebx+32]
		shufps	xmm4, xmm4, 0
		mulps	xmm4, xmm7

		addps	xmm0, xmm2
		addps	xmm0, xmm4

		movlps	qword ptr [eax+64], xmm0
	}

	epilogue
		
} // Mult00_3x3_3x6

SMLXTmpMatrix& __fastcall Mult01_3x3_6x3(const SMLXMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,6,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt, tmp1, tmp2, tmp3;

	//	Loading first 2 rows of the src2 and mixing.

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	tmp3	= _mm_loadh_pi( _mm_loadl_pi( tmp3, (__m64*)&src2[ 4] ), (__m64*)&src2[10] );

	b0		= _mm_shuffle_ps( tmp1, tmp2, 0xCC );
	b1		= _mm_shuffle_ps( tmp1, tmp3, 0x49 );
	tmp1	= _mm_shuffle_ps( tmp2, tmp3, 0xE9 );
	b2		= _mm_shuffle_ps( b1  , tmp1, 0xDD );
	b1		= _mm_shuffle_ps( b1  , tmp1, 0x88 );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[12] ), (__m64*)&src2[14] );
	tmp1	= _mm_loadl_pi( tmp1, (__m64*)&src2[16] );

	b2		= _mm_shuffle_ps( b0, tmp1, 0x49 );
	b0		= _mm_shuffle_ps( b0, b0, 0xCC );

	b1		= _mm_shuffle_ps( b2, b2, 0x88 );
	b2		= _mm_shuffle_ps( b2, b2, 0xDD );

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src1[ 0] ), (__m64*)&src1[ 2] );
	row		= _mm_shuffle_ps( tmp1, tmp1, 0xF0 );
	rslt	= _mm_mul_ps( row, b0 );

	tmp2	= _mm_loadl_pi( tmp2, (__m64*)&src1[ 4] );
	row		= _mm_shuffle_ps( tmp1, tmp2, 0x05 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_shuffle_ps( tmp1, tmp2, 0x5A );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult01_3x3_6x3

SMLXTmpMatrix& __fastcall Mult10_3x3_3x6(const SMLXTransposedMatrix &A, const SMLXMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt;

	//	Loading first 4 columns of the src2.

	b0		= _mm_load_ps( &src2[0] );
	b1		= _mm_loadh_pi( _mm_loadl_pi( b1, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	b2		= _mm_load_ps( &src2[12] );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadl_pi( b0 , (__m64*)&src2[ 4] );
	b0		= _mm_shuffle_ps( b0 , b0 , 0x44 );
	b1		= _mm_loadl_pi( b1 , (__m64*)&src2[10] );
	b1		= _mm_shuffle_ps( b1 , b1 , 0x44 );
	b2		= _mm_loadl_pi( b2 , (__m64*)&src2[16] );
	b2		= _mm_shuffle_ps( b2 , b2 , 0x44 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );
	
	return dmat;

} // Mult10_3x3_3x6

SMLXTmpMatrix& __fastcall Mult11_3x3_6x3(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)
{
	register float* src1 = A.Data();
	register float* src2 = B.Data();
	register SMLXTmpMatrix& dmat= nrxc(TMat,3,6)();
	register float* dst = dmat.Data();

	__m128	b0, b1, b2;
	__m128	row, rslt, tmp1, tmp2, tmp3;

	//	Loading first 2 rows of the src2 and mixing.

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&src2[ 0] ), (__m64*)&src2[ 2] );
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&src2[ 6] ), (__m64*)&src2[ 8] );
	tmp3	= _mm_loadh_pi( _mm_loadl_pi( tmp3, (__m64*)&src2[ 4] ), (__m64*)&src2[10] );

	b0		= _mm_shuffle_ps( tmp1, tmp2, 0xCC );
	b1		= _mm_shuffle_ps( tmp1, tmp3, 0x49 );
	tmp1	= _mm_shuffle_ps( tmp2, tmp3, 0xE9 );
	b2		= _mm_shuffle_ps( b1  , tmp1, 0xDD );
	b1		= _mm_shuffle_ps( b1  , tmp1, 0x88 );

	//	Calculating first 4 elements in the first row of the destination matrix.

	row		= _mm_load_ss( &src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[0], rslt );
	_mm_storeh_pi( (__m64*)&dst[2], rslt );

	//	Calculating first 4 elements in the second row of the destination matrix.

	row		= _mm_load_ss( &src1[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );
	
	row		= _mm_load_ss( &src1[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[6], rslt );
	_mm_storeh_pi( (__m64*)&dst[8], rslt );

	//	Calculating first 4 elements in the third row of the destination matrix.

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[12], rslt );
	_mm_storeh_pi( (__m64*)&dst[14], rslt );

	//	Calculating last 2 columns of the destination matrix.

	b0		= _mm_loadh_pi( _mm_loadl_pi( b0, (__m64*)&src2[12] ), (__m64*)&src2[14] );
	tmp1	= _mm_loadl_pi( tmp1, (__m64*)&src2[16] );

	b2		= _mm_shuffle_ps( b0, tmp1, 0x49 );
	b0		= _mm_shuffle_ps( b0, b0, 0xCC );

	b1		= _mm_shuffle_ps( b2, b2, 0x88 );
	b2		= _mm_shuffle_ps( b2, b2, 0xDD );

	row		= _mm_loadl_pi( row, (__m64*)&src1[0] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_loadl_pi( row, (__m64*)&src1[3] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_loadl_pi( row, (__m64*)&src1[6] );
	row		= _mm_shuffle_ps( row, row, 0x50 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[ 4], rslt );
	_mm_storeh_pi( (__m64*)&dst[10], rslt );

	row		= _mm_load_ss( &src1[2] );
	row		= _mm_shuffle_ps( row, row, 0 );
	rslt	= _mm_mul_ps( row, b0 );

	row		= _mm_load_ss( &src1[5] );
	row		= _mm_shuffle_ps( row, row, 0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &src1[8] );
	row		= _mm_shuffle_ps( row, row, 0 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_storel_pi( (__m64*)&dst[16], rslt );

	return dmat;

} // Mult11_3x3_6x3