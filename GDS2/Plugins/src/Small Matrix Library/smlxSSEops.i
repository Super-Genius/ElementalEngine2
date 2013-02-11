#include "xmmintrin.h"

// 1 pair operation
#define f2(addr1, addr2, dst, func) \
	_mm_storeh_pi((__m64*)(dst), _mm_##func##_ps(_mm_loadh_pi(m1, (__m64*)(addr1)), _mm_loadh_pi(m2, (__m64*)(addr2))))
// 2 pair operation
#define f4(addr1, addr2, dst, func) \
	/*_mm_store_ps(dst, _mm_##func##_ps(_mm_load_ps(addr1), _mm_load_ps(addr2)))*/ \
	*((__m128*)(dst)) = _mm_##func##_ps(*((__m128*)(addr1)),*((__m128*)(addr2)))
#define ff(aa, num, func) \
	f##aa##(a+aa*num, b+aa*num, dst+aa*num, func)

/*
#define load_pi(addr, reg) \
	reg = _mm_loadh_pi(_mm_loadl_pi(reg, (__m64*)(addr)), (__m64*)(addr + 2))
#define store_pi(addr, reg) \
	_mm_storeh_pi((__m64*)(addr + 2), reg); \
	_mm_storel_pi((__m64*)(addr), reg)
// 1 pair operation
#define f2(addr1, addr2, dst, func) \
	m1 = _mm_loadh_pi(m1, (__m64*)(addr1)); \
	m2 = _mm_loadh_pi(m2, (__m64*)(addr2)); \
	rez	= _mm_##func##_ps(m1, m2); \
	_mm_storeh_pi((__m64*)(dst), rez)
// 2 pair operation
#define f4(addr1, addr2, dst, func) \
	load_pi(addr1, m1); \
	load_pi(addr2, m2); \
	rez = _mm_##func##_ps(m1, m2); \
	store_pi(dst, rez)
#define ff(aa, num, func) \
	f##aa##(a+aa*num, b+aa*num, dst+aa*num, func)
*/


SMLXTmpMatrix nrxc(pool,1,1)[SMLXMatrix::poolSize];

int nrxc(tmpPoolInitialized, 1, 1) = SMLXTmpMatrix::InitTmpPool(1, 1, nrxc(pool,1,1), SMLXMatrix::poolSize);

SMLXTmpMatrix& __fastcall nrxc(TMat,1,1)() 
{
	enum {pool_sz = sizeof(nrxc(pool,1,1)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,1,1)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,1,1)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,1,1)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,1,1)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
	dst[0]=a[0]+b[0];
	return dmat;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,1,1)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,1,1)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
	dst[0]=a[0]-b[0];
	return dmat;
}

//__declspec(naked)
void __fastcall nrxc(AddTo,1,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
	dst[0]=a[0]+b[0];
	CLEAR_SUBEXPRESSION_COUNT
}

//__declspec(naked)
void __fastcall nrxc(SubFrom,1,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
	dst[0]=a[0]-b[0];
	CLEAR_SUBEXPRESSION_COUNT
}


__declspec(naked)
void __fastcall nrxc(Assign,1,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
__asm fld dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(AssignValue,1,1)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 1 + 0)], edx
	__asm ret 4
}

SMLXTmpMatrix nrxc(pool,3,3)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 3, 3) = SMLXTmpMatrix::InitTmpPool(3, 3, nrxc(pool,3,3), SMLXMatrix::poolSize);
	
SMLXTmpMatrix& __fastcall nrxc(TMat,3,3)() 
{
	enum {pool_sz = sizeof(nrxc(pool,3,3)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,3,3)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,3,3)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,3,3)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,3,3)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op add
#define size 9
#include <smlxun2.inl>
	dst[size-1]=a[size-1]+b[size-1];
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,3,3)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,3,3)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op sub
#define size 9
#include <smlxun2.inl>
	dst[size-1]=a[size-1]-b[size-1];
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
void __fastcall nrxc(AddTo,3,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op add
#define size 9
#include <smlxun2.inl>
	dst[size-1]=a[size-1]+b[size-1];
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

//__declspec(naked)
void __fastcall nrxc(SubFrom,3,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op sub
#define size 9
#include <smlxun2.inl>
	dst[size-1]=a[size-1]-b[size-1];
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,3,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
#define size 9
#include "smlxunas.inl"
#undef size

/*	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
__asm fld dword ptr [edx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 0)]
__asm fld dword ptr [edx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 1)]
__asm fld dword ptr [edx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 2)]
__asm fld dword ptr [edx + 4 * (1 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 0)]
__asm fld dword ptr [edx + 4 * (1 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 1)]
__asm fld dword ptr [edx + 4 * (1 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 2)]
__asm fld dword ptr [edx + 4 * (2 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 0)]
__asm fld dword ptr [edx + 4 * (2 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 1)]
__asm fld dword ptr [edx + 4 * (2 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 2)]
*/
	__asm ret
}

__declspec(naked)
void __fastcall nrxc(AssignValue,3,3)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 3 + 0)], edx
__asm mov dword ptr [eax + 4 * (0 * 3 + 1)], edx
__asm mov dword ptr [eax + 4 * (0 * 3 + 2)], edx
__asm mov dword ptr [eax + 4 * (1 * 3 + 0)], edx
__asm mov dword ptr [eax + 4 * (1 * 3 + 1)], edx
__asm mov dword ptr [eax + 4 * (1 * 3 + 2)], edx
__asm mov dword ptr [eax + 4 * (2 * 3 + 0)], edx
__asm mov dword ptr [eax + 4 * (2 * 3 + 1)], edx
__asm mov dword ptr [eax + 4 * (2 * 3 + 2)], edx
	__asm ret 4
}

SMLXTmpMatrix nrxc(pool,3,1)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 3, 1) = SMLXTmpMatrix::InitTmpPool(3, 1, nrxc(pool,3,1), SMLXMatrix::poolSize);
	
SMLXTmpMatrix& __fastcall nrxc(TMat,3,1)() 
{
	enum {pool_sz = sizeof(nrxc(pool,3,1)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,3,1)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,3,1)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,3,1)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,3,1)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op add
#define size 3
#include <smlxun2.inl>
	dst[size-1]=a[size-1]+b[size-1];
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,3,1)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,3,1)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op sub
#define size 3
#include <smlxun2.inl>
	dst[size-1]=a[size-1]-b[size-1];
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
void __fastcall nrxc(AddTo,3,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op add
#define size 3
#include <smlxun2.inl>
	dst[size-1]=a[size-1]+b[size-1];
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

//__declspec(naked)
void __fastcall nrxc(SubFrom,3,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op sub
#define size 3
#include <smlxun2.inl>
	dst[size-1]=a[size-1]-b[size-1];
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,3,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
#define size 3
#include "smlxunas.inl"
#undef size

/*	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
__asm fld dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]
__asm fld dword ptr [edx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 1 + 0)]
__asm fld dword ptr [edx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 1 + 0)]
*/
	__asm ret
}

__declspec(naked)
void __fastcall nrxc(AssignValue,3,1)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 1 + 0)], edx
__asm mov dword ptr [eax + 4 * (1 * 1 + 0)], edx
__asm mov dword ptr [eax + 4 * (2 * 1 + 0)], edx
	__asm ret 4
}

SMLXTmpMatrix nrxc(pool,1,3)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 1, 3) = SMLXTmpMatrix::InitTmpPool(1, 3, nrxc(pool,1,3), SMLXMatrix::poolSize);
	
SMLXTmpMatrix& __fastcall nrxc(TMat,1,3)() 
{
	enum {pool_sz = sizeof(nrxc(pool,1,3)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,1,3)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,1,3)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,1,3)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,1,3)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op add
#define size 3
#include <smlxun2.inl>
	dst[size-1]=a[size-1]+b[size-1];
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,1,3)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,1,3)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op sub
#define size 3
#include <smlxun2.inl>
	dst[size-1]=a[size-1]-b[size-1];
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
void __fastcall nrxc(AddTo,1,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op add
#define size 3
#include <smlxun2.inl>
	dst[size-1]=a[size-1]+b[size-1];
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

//__declspec(naked)
void __fastcall nrxc(SubFrom,1,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op sub
#define size 3
#include <smlxun2.inl>
	dst[size-1]=a[size-1]-b[size-1];
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,1,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
#define size 3
#include "smlxunas.inl"
#undef size

/*	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
__asm fld dword ptr [edx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 0)]
__asm fld dword ptr [edx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 1)]
__asm fld dword ptr [edx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 2)]
*/
	__asm ret
}

__declspec(naked)
void __fastcall nrxc(AssignValue,1,3)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 3 + 0)], edx
__asm mov dword ptr [eax + 4 * (0 * 3 + 1)], edx
__asm mov dword ptr [eax + 4 * (0 * 3 + 2)], edx
	__asm ret 4
}

SMLXTmpMatrix nrxc(pool,4,4)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 4, 4) = SMLXTmpMatrix::InitTmpPool(4, 4, nrxc(pool,4,4), SMLXMatrix::poolSize);
	
SMLXTmpMatrix& __fastcall nrxc(TMat,4,4)() 
{
	enum {pool_sz = sizeof(nrxc(pool,4,4)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,4,4)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,4,4)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,4,4)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,4,4)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op add
#define size 16
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,4,4)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,4,4)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op sub
#define size 16
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
void __fastcall nrxc(AddTo,4,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op add
#define size 16
#include <smlxun2.inl>
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

//__declspec(naked)
void __fastcall nrxc(SubFrom,4,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op sub
#define size 16
#include <smlxun2.inl>
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,4,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
#define size 16
#include "smlxunas.inl"
#undef size

/*	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
__asm fld dword ptr [edx + 4 * (0 * 4 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 0)]
__asm fld dword ptr [edx + 4 * (0 * 4 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 1)]
__asm fld dword ptr [edx + 4 * (0 * 4 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 2)]
__asm fld dword ptr [edx + 4 * (0 * 4 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 3)]
__asm fld dword ptr [edx + 4 * (1 * 4 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 4 + 0)]
__asm fld dword ptr [edx + 4 * (1 * 4 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 4 + 1)]
__asm fld dword ptr [edx + 4 * (1 * 4 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 4 + 2)]
__asm fld dword ptr [edx + 4 * (1 * 4 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 4 + 3)]
__asm fld dword ptr [edx + 4 * (2 * 4 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 4 + 0)]
__asm fld dword ptr [edx + 4 * (2 * 4 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 4 + 1)]
__asm fld dword ptr [edx + 4 * (2 * 4 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 4 + 2)]
__asm fld dword ptr [edx + 4 * (2 * 4 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 4 + 3)]
__asm fld dword ptr [edx + 4 * (3 * 4 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 4 + 0)]
__asm fld dword ptr [edx + 4 * (3 * 4 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 4 + 1)]
__asm fld dword ptr [edx + 4 * (3 * 4 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 4 + 2)]
__asm fld dword ptr [edx + 4 * (3 * 4 + 3)] __asm fstp dword ptr [ecx + 4 * (3 * 4 + 3)]
*/
	__asm ret
}

__declspec(naked)
void __fastcall nrxc(AssignValue,4,4)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 4 + 0)], edx
__asm mov dword ptr [eax + 4 * (0 * 4 + 1)], edx
__asm mov dword ptr [eax + 4 * (0 * 4 + 2)], edx
__asm mov dword ptr [eax + 4 * (0 * 4 + 3)], edx
__asm mov dword ptr [eax + 4 * (1 * 4 + 0)], edx
__asm mov dword ptr [eax + 4 * (1 * 4 + 1)], edx
__asm mov dword ptr [eax + 4 * (1 * 4 + 2)], edx
__asm mov dword ptr [eax + 4 * (1 * 4 + 3)], edx
__asm mov dword ptr [eax + 4 * (2 * 4 + 0)], edx
__asm mov dword ptr [eax + 4 * (2 * 4 + 1)], edx
__asm mov dword ptr [eax + 4 * (2 * 4 + 2)], edx
__asm mov dword ptr [eax + 4 * (2 * 4 + 3)], edx
__asm mov dword ptr [eax + 4 * (3 * 4 + 0)], edx
__asm mov dword ptr [eax + 4 * (3 * 4 + 1)], edx
__asm mov dword ptr [eax + 4 * (3 * 4 + 2)], edx
__asm mov dword ptr [eax + 4 * (3 * 4 + 3)], edx
	__asm ret 4
}

SMLXTmpMatrix nrxc(pool,4,1)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 4, 1) = SMLXTmpMatrix::InitTmpPool(4, 1, nrxc(pool,4,1), SMLXMatrix::poolSize);
	
SMLXTmpMatrix& __fastcall nrxc(TMat,4,1)() 
{
	enum {pool_sz = sizeof(nrxc(pool,4,1)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,4,1)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,4,1)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,4,1)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,4,1)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op add
#define size 4
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,4,1)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,4,1)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op sub
#define size 4
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
void __fastcall nrxc(AddTo,4,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op add
#define size 4
#include <smlxun2.inl>
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

//__declspec(naked)
void __fastcall nrxc(SubFrom,4,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op sub
#define size 4
#include <smlxun2.inl>
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,4,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
#define size 4
#include "smlxunas.inl"
#undef size

/*	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
__asm fld dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]
__asm fld dword ptr [edx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 1 + 0)]
__asm fld dword ptr [edx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 1 + 0)]
__asm fld dword ptr [edx + 4 * (3 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 1 + 0)]
*/
	__asm ret
}

__declspec(naked)
void __fastcall nrxc(AssignValue,4,1)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 1 + 0)], edx
__asm mov dword ptr [eax + 4 * (1 * 1 + 0)], edx
__asm mov dword ptr [eax + 4 * (2 * 1 + 0)], edx
__asm mov dword ptr [eax + 4 * (3 * 1 + 0)], edx
	__asm ret 4
}

SMLXTmpMatrix nrxc(pool,1,4)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 1, 4) = SMLXTmpMatrix::InitTmpPool(1, 4, nrxc(pool,1,4), SMLXMatrix::poolSize);
	
SMLXTmpMatrix& __fastcall nrxc(TMat,1,4)() 
{
	enum {pool_sz = sizeof(nrxc(pool,1,4)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,1,4)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,1,4)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,1,4)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,1,4)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op add
#define size 4
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,1,4)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,1,4)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op sub
#define size 4
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
void __fastcall nrxc(AddTo,1,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op add
#define size 4
#include <smlxun2.inl>
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

//__declspec(naked)
void __fastcall nrxc(SubFrom,1,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op sub
#define size 4
#include <smlxun2.inl>
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,1,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
#define size 4
#include "smlxunas.inl"
#undef size

/*	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
__asm fld dword ptr [edx + 4 * (0 * 4 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 0)]
__asm fld dword ptr [edx + 4 * (0 * 4 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 1)]
__asm fld dword ptr [edx + 4 * (0 * 4 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 2)]
__asm fld dword ptr [edx + 4 * (0 * 4 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 3)]
*/
	__asm ret
}

__declspec(naked)
void __fastcall nrxc(AssignValue,1,4)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 4 + 0)], edx
__asm mov dword ptr [eax + 4 * (0 * 4 + 1)], edx
__asm mov dword ptr [eax + 4 * (0 * 4 + 2)], edx
__asm mov dword ptr [eax + 4 * (0 * 4 + 3)], edx
	__asm ret 4
}

SMLXTmpMatrix nrxc(pool,5,5)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 5, 5) = SMLXTmpMatrix::InitTmpPool(5, 5, nrxc(pool,5,5), SMLXMatrix::poolSize);
	
SMLXTmpMatrix& __fastcall nrxc(TMat,5,5)() 
{
	enum {pool_sz = sizeof(nrxc(pool,5,5)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,5,5)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,5,5)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,5,5)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,5,5)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op add
#define size 25
#include <smlxun2.inl>
	dst[size-1]=a[size-1]+b[size-1];
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,5,5)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,5,5)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op sub
#define size 25
#include <smlxun2.inl>
	dst[size-1]=a[size-1]-b[size-1];
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
void __fastcall nrxc(AddTo,5,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op add
#define size 25
#include <smlxun2.inl>
	dst[size-1]=a[size-1]+b[size-1];
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

//__declspec(naked)
void __fastcall nrxc(SubFrom,5,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op sub
#define size 25
#include <smlxun2.inl>
	dst[size-1]=a[size-1]-b[size-1];
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,5,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
#define size 25
#include "smlxunas.inl"
#undef size
/*
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
__asm fld dword ptr [edx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 0)]
__asm fld dword ptr [edx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 1)]
__asm fld dword ptr [edx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 2)]
__asm fld dword ptr [edx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 3)]
__asm fld dword ptr [edx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 4)]
__asm fld dword ptr [edx + 4 * (1 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 0)]
__asm fld dword ptr [edx + 4 * (1 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 1)]
__asm fld dword ptr [edx + 4 * (1 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 2)]
__asm fld dword ptr [edx + 4 * (1 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 3)]
__asm fld dword ptr [edx + 4 * (1 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 4)]
__asm fld dword ptr [edx + 4 * (2 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 0)]
__asm fld dword ptr [edx + 4 * (2 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 1)]
__asm fld dword ptr [edx + 4 * (2 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 2)]
__asm fld dword ptr [edx + 4 * (2 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 3)]
__asm fld dword ptr [edx + 4 * (2 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 4)]
__asm fld dword ptr [edx + 4 * (3 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 0)]
__asm fld dword ptr [edx + 4 * (3 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 1)]
__asm fld dword ptr [edx + 4 * (3 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 2)]
__asm fld dword ptr [edx + 4 * (3 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 3)]
__asm fld dword ptr [edx + 4 * (3 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 4)]
__asm fld dword ptr [edx + 4 * (4 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 0)]
__asm fld dword ptr [edx + 4 * (4 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 1)]
__asm fld dword ptr [edx + 4 * (4 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 2)]
__asm fld dword ptr [edx + 4 * (4 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 3)]
__asm fld dword ptr [edx + 4 * (4 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 4)]
*/

	__asm ret
}

__declspec(naked)
void __fastcall nrxc(AssignValue,5,5)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 5 + 0)], edx
__asm mov dword ptr [eax + 4 * (0 * 5 + 1)], edx
__asm mov dword ptr [eax + 4 * (0 * 5 + 2)], edx
__asm mov dword ptr [eax + 4 * (0 * 5 + 3)], edx
__asm mov dword ptr [eax + 4 * (0 * 5 + 4)], edx
__asm mov dword ptr [eax + 4 * (1 * 5 + 0)], edx
__asm mov dword ptr [eax + 4 * (1 * 5 + 1)], edx
__asm mov dword ptr [eax + 4 * (1 * 5 + 2)], edx
__asm mov dword ptr [eax + 4 * (1 * 5 + 3)], edx
__asm mov dword ptr [eax + 4 * (1 * 5 + 4)], edx
__asm mov dword ptr [eax + 4 * (2 * 5 + 0)], edx
__asm mov dword ptr [eax + 4 * (2 * 5 + 1)], edx
__asm mov dword ptr [eax + 4 * (2 * 5 + 2)], edx
__asm mov dword ptr [eax + 4 * (2 * 5 + 3)], edx
__asm mov dword ptr [eax + 4 * (2 * 5 + 4)], edx
__asm mov dword ptr [eax + 4 * (3 * 5 + 0)], edx
__asm mov dword ptr [eax + 4 * (3 * 5 + 1)], edx
__asm mov dword ptr [eax + 4 * (3 * 5 + 2)], edx
__asm mov dword ptr [eax + 4 * (3 * 5 + 3)], edx
__asm mov dword ptr [eax + 4 * (3 * 5 + 4)], edx
__asm mov dword ptr [eax + 4 * (4 * 5 + 0)], edx
__asm mov dword ptr [eax + 4 * (4 * 5 + 1)], edx
__asm mov dword ptr [eax + 4 * (4 * 5 + 2)], edx
__asm mov dword ptr [eax + 4 * (4 * 5 + 3)], edx
__asm mov dword ptr [eax + 4 * (4 * 5 + 4)], edx
	__asm ret 4
}

SMLXTmpMatrix nrxc(pool,5,1)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 5, 1) = SMLXTmpMatrix::InitTmpPool(5, 1, nrxc(pool,5,1), SMLXMatrix::poolSize);
	
SMLXTmpMatrix& __fastcall nrxc(TMat,5,1)() 
{
	enum {pool_sz = sizeof(nrxc(pool,5,1)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,5,1)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,5,1)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,5,1)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,5,1)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op add
#define size 5
#include <smlxun2.inl>
	dst[size-1]=a[size-1]+b[size-1];
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,5,1)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,5,1)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op sub
#define size 5
#include <smlxun2.inl>
	dst[size-1]=a[size-1]-b[size-1];
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
void __fastcall nrxc(AddTo,5,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op add
#define size 5
#include <smlxun2.inl>
	dst[size-1]=a[size-1]+b[size-1];
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

//__declspec(naked)
void __fastcall nrxc(SubFrom,5,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op sub
#define size 5
#include <smlxun2.inl>
	dst[size-1]=a[size-1]-b[size-1];
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,5,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
#define size 5
#include "smlxunas.inl"
#undef size

/*	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
__asm fld dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]
__asm fld dword ptr [edx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 1 + 0)]
__asm fld dword ptr [edx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 1 + 0)]
__asm fld dword ptr [edx + 4 * (3 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 1 + 0)]
__asm fld dword ptr [edx + 4 * (4 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 1 + 0)]
*/
	__asm ret
}

__declspec(naked)
void __fastcall nrxc(AssignValue,5,1)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 1 + 0)], edx
__asm mov dword ptr [eax + 4 * (1 * 1 + 0)], edx
__asm mov dword ptr [eax + 4 * (2 * 1 + 0)], edx
__asm mov dword ptr [eax + 4 * (3 * 1 + 0)], edx
__asm mov dword ptr [eax + 4 * (4 * 1 + 0)], edx
	__asm ret 4
}

SMLXTmpMatrix nrxc(pool,1,5)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 1, 5) = SMLXTmpMatrix::InitTmpPool(1, 5, nrxc(pool,1,5), SMLXMatrix::poolSize);
	
SMLXTmpMatrix& __fastcall nrxc(TMat,1,5)() 
{
	enum {pool_sz = sizeof(nrxc(pool,1,5)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,1,5)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,1,5)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,1,5)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,1,5)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op add
#define size 5
#include <smlxun2.inl>
	dst[size-1]=a[size-1]+b[size-1];
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,1,5)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,1,5)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op sub
#define size 5
#include <smlxun2.inl>
	dst[size-1]=a[size-1]-b[size-1];
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
void __fastcall nrxc(AddTo,1,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op add
#define size 5
#include <smlxun2.inl>
	dst[size-1]=a[size-1]+b[size-1];
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

//__declspec(naked)
void __fastcall nrxc(SubFrom,1,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op sub
#define size 5
#include <smlxun2.inl>
	dst[size-1]=a[size-1]-b[size-1];
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,1,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
#define size 5
#include "smlxunas.inl"
#undef size

/*	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
__asm fld dword ptr [edx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 0)]
__asm fld dword ptr [edx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 1)]
__asm fld dword ptr [edx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 2)]
__asm fld dword ptr [edx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 3)]
__asm fld dword ptr [edx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 4)]
*/
	__asm ret
}

__declspec(naked)
void __fastcall nrxc(AssignValue,1,5)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 5 + 0)], edx
__asm mov dword ptr [eax + 4 * (0 * 5 + 1)], edx
__asm mov dword ptr [eax + 4 * (0 * 5 + 2)], edx
__asm mov dword ptr [eax + 4 * (0 * 5 + 3)], edx
__asm mov dword ptr [eax + 4 * (0 * 5 + 4)], edx
	__asm ret 4
}

SMLXTmpMatrix nrxc(pool,5,3)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 5, 3) = SMLXTmpMatrix::InitTmpPool(5, 3, nrxc(pool,5,3), SMLXMatrix::poolSize);
	
SMLXTmpMatrix& __fastcall nrxc(TMat,5,3)() 
{
	enum {pool_sz = sizeof(nrxc(pool,5,3)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,5,3)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,5,3)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,5,3)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,5,3)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op add
#define size 15
#include <smlxun2.inl>
	dst[size-1]=a[size-1]+b[size-1];
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,5,3)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,5,3)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op sub
#define size 15
#include <smlxun2.inl>
	dst[size-1]=a[size-1]-b[size-1];
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
void __fastcall nrxc(AddTo,5,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op add
#define size 15
#include <smlxun2.inl>
	dst[size-1]=a[size-1]+b[size-1];
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

//__declspec(naked)
void __fastcall nrxc(SubFrom,5,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op sub
#define size 15
#include <smlxun2.inl>
	dst[size-1]=a[size-1]-b[size-1];
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,5,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
#define size 15
#include "smlxunas.inl"
#undef size

/*	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
__asm fld dword ptr [edx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 0)]
__asm fld dword ptr [edx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 1)]
__asm fld dword ptr [edx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 2)]
__asm fld dword ptr [edx + 4 * (1 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 0)]
__asm fld dword ptr [edx + 4 * (1 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 1)]
__asm fld dword ptr [edx + 4 * (1 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 2)]
__asm fld dword ptr [edx + 4 * (2 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 0)]
__asm fld dword ptr [edx + 4 * (2 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 1)]
__asm fld dword ptr [edx + 4 * (2 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 2)]
__asm fld dword ptr [edx + 4 * (3 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 3 + 0)]
__asm fld dword ptr [edx + 4 * (3 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 3 + 1)]
__asm fld dword ptr [edx + 4 * (3 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 3 + 2)]
__asm fld dword ptr [edx + 4 * (4 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 3 + 0)]
__asm fld dword ptr [edx + 4 * (4 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (4 * 3 + 1)]
__asm fld dword ptr [edx + 4 * (4 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (4 * 3 + 2)]
*/
	__asm ret
}

__declspec(naked)
void __fastcall nrxc(AssignValue,5,3)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 3 + 0)], edx
__asm mov dword ptr [eax + 4 * (0 * 3 + 1)], edx
__asm mov dword ptr [eax + 4 * (0 * 3 + 2)], edx
__asm mov dword ptr [eax + 4 * (1 * 3 + 0)], edx
__asm mov dword ptr [eax + 4 * (1 * 3 + 1)], edx
__asm mov dword ptr [eax + 4 * (1 * 3 + 2)], edx
__asm mov dword ptr [eax + 4 * (2 * 3 + 0)], edx
__asm mov dword ptr [eax + 4 * (2 * 3 + 1)], edx
__asm mov dword ptr [eax + 4 * (2 * 3 + 2)], edx
__asm mov dword ptr [eax + 4 * (3 * 3 + 0)], edx
__asm mov dword ptr [eax + 4 * (3 * 3 + 1)], edx
__asm mov dword ptr [eax + 4 * (3 * 3 + 2)], edx
__asm mov dword ptr [eax + 4 * (4 * 3 + 0)], edx
__asm mov dword ptr [eax + 4 * (4 * 3 + 1)], edx
__asm mov dword ptr [eax + 4 * (4 * 3 + 2)], edx
	__asm ret 4
}

SMLXTmpMatrix nrxc(pool,3,5)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 3, 5) = SMLXTmpMatrix::InitTmpPool(3, 5, nrxc(pool,3,5), SMLXMatrix::poolSize);
	
SMLXTmpMatrix& __fastcall nrxc(TMat,3,5)() 
{
	enum {pool_sz = sizeof(nrxc(pool,3,5)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,3,5)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,3,5)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,3,5)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,3,5)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op add
#define size 15
#include <smlxun2.inl>
	dst[size-1]=a[size-1]+b[size-1];
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,3,5)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,3,5)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op sub
#define size 15
#include <smlxun2.inl>
	dst[size-1]=a[size-1]-b[size-1];
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
void __fastcall nrxc(AddTo,3,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op add
#define size 15
#include <smlxun2.inl>
	dst[size-1]=a[size-1]+b[size-1];
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

//__declspec(naked)
void __fastcall nrxc(SubFrom,3,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op sub
#define size 15
#include <smlxun2.inl>
	dst[size-1]=a[size-1]-b[size-1];
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,3,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
#define size 15
#include "smlxunas.inl"
#undef size

/*	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
__asm fld dword ptr [edx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 0)]
__asm fld dword ptr [edx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 1)]
__asm fld dword ptr [edx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 2)]
__asm fld dword ptr [edx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 3)]
__asm fld dword ptr [edx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 4)]
__asm fld dword ptr [edx + 4 * (1 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 0)]
__asm fld dword ptr [edx + 4 * (1 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 1)]
__asm fld dword ptr [edx + 4 * (1 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 2)]
__asm fld dword ptr [edx + 4 * (1 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 3)]
__asm fld dword ptr [edx + 4 * (1 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 4)]
__asm fld dword ptr [edx + 4 * (2 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 0)]
__asm fld dword ptr [edx + 4 * (2 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 1)]
__asm fld dword ptr [edx + 4 * (2 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 2)]
__asm fld dword ptr [edx + 4 * (2 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 3)]
__asm fld dword ptr [edx + 4 * (2 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 4)]
*/
	__asm ret
}

__declspec(naked)
void __fastcall nrxc(AssignValue,3,5)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 5 + 0)], edx
__asm mov dword ptr [eax + 4 * (0 * 5 + 1)], edx
__asm mov dword ptr [eax + 4 * (0 * 5 + 2)], edx
__asm mov dword ptr [eax + 4 * (0 * 5 + 3)], edx
__asm mov dword ptr [eax + 4 * (0 * 5 + 4)], edx
__asm mov dword ptr [eax + 4 * (1 * 5 + 0)], edx
__asm mov dword ptr [eax + 4 * (1 * 5 + 1)], edx
__asm mov dword ptr [eax + 4 * (1 * 5 + 2)], edx
__asm mov dword ptr [eax + 4 * (1 * 5 + 3)], edx
__asm mov dword ptr [eax + 4 * (1 * 5 + 4)], edx
__asm mov dword ptr [eax + 4 * (2 * 5 + 0)], edx
__asm mov dword ptr [eax + 4 * (2 * 5 + 1)], edx
__asm mov dword ptr [eax + 4 * (2 * 5 + 2)], edx
__asm mov dword ptr [eax + 4 * (2 * 5 + 3)], edx
__asm mov dword ptr [eax + 4 * (2 * 5 + 4)], edx
	__asm ret 4
}

SMLXTmpMatrix nrxc(pool,5,6)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 5, 6) = SMLXTmpMatrix::InitTmpPool(5, 6, nrxc(pool,5,6), SMLXMatrix::poolSize);
	
SMLXTmpMatrix& __fastcall nrxc(TMat,5,6)() 
{
	enum {pool_sz = sizeof(nrxc(pool,5,6)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,5,6)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,5,6)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,5,6)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,5,6)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op add
#define size 30
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,5,6)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,5,6)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op sub
#define size 30
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
void __fastcall nrxc(AddTo,5,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op add
#define size 30
#include <smlxun2.inl>
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

//__declspec(naked)
void __fastcall nrxc(SubFrom,5,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op sub
#define size 30
#include <smlxun2.inl>
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,5,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
#define size 30
#include "smlxunas.inl"
#undef size

/*
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
__asm fld dword ptr [edx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 0)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 1)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 2)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 3)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 4)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 5)]
__asm fld dword ptr [edx + 4 * (1 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 0)]
__asm fld dword ptr [edx + 4 * (1 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 1)]
__asm fld dword ptr [edx + 4 * (1 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 2)]
__asm fld dword ptr [edx + 4 * (1 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 3)]
__asm fld dword ptr [edx + 4 * (1 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 4)]
__asm fld dword ptr [edx + 4 * (1 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 5)]
__asm fld dword ptr [edx + 4 * (2 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 0)]
__asm fld dword ptr [edx + 4 * (2 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 1)]
__asm fld dword ptr [edx + 4 * (2 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 2)]
__asm fld dword ptr [edx + 4 * (2 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 3)]
__asm fld dword ptr [edx + 4 * (2 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 4)]
__asm fld dword ptr [edx + 4 * (2 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 5)]
__asm fld dword ptr [edx + 4 * (3 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 0)]
__asm fld dword ptr [edx + 4 * (3 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 1)]
__asm fld dword ptr [edx + 4 * (3 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 2)]
__asm fld dword ptr [edx + 4 * (3 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 3)]
__asm fld dword ptr [edx + 4 * (3 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 4)]
__asm fld dword ptr [edx + 4 * (3 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 5)]
__asm fld dword ptr [edx + 4 * (4 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 0)]
__asm fld dword ptr [edx + 4 * (4 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 1)]
__asm fld dword ptr [edx + 4 * (4 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 2)]
__asm fld dword ptr [edx + 4 * (4 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 3)]
__asm fld dword ptr [edx + 4 * (4 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 4)]
__asm fld dword ptr [edx + 4 * (4 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 5)]
*/
	__asm ret
}

__declspec(naked)
void __fastcall nrxc(AssignValue,5,6)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 6 + 0)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 1)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 2)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 3)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 4)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 5)], edx
__asm mov dword ptr [eax + 4 * (1 * 6 + 0)], edx
__asm mov dword ptr [eax + 4 * (1 * 6 + 1)], edx
__asm mov dword ptr [eax + 4 * (1 * 6 + 2)], edx
__asm mov dword ptr [eax + 4 * (1 * 6 + 3)], edx
__asm mov dword ptr [eax + 4 * (1 * 6 + 4)], edx
__asm mov dword ptr [eax + 4 * (1 * 6 + 5)], edx
__asm mov dword ptr [eax + 4 * (2 * 6 + 0)], edx
__asm mov dword ptr [eax + 4 * (2 * 6 + 1)], edx
__asm mov dword ptr [eax + 4 * (2 * 6 + 2)], edx
__asm mov dword ptr [eax + 4 * (2 * 6 + 3)], edx
__asm mov dword ptr [eax + 4 * (2 * 6 + 4)], edx
__asm mov dword ptr [eax + 4 * (2 * 6 + 5)], edx
__asm mov dword ptr [eax + 4 * (3 * 6 + 0)], edx
__asm mov dword ptr [eax + 4 * (3 * 6 + 1)], edx
__asm mov dword ptr [eax + 4 * (3 * 6 + 2)], edx
__asm mov dword ptr [eax + 4 * (3 * 6 + 3)], edx
__asm mov dword ptr [eax + 4 * (3 * 6 + 4)], edx
__asm mov dword ptr [eax + 4 * (3 * 6 + 5)], edx
__asm mov dword ptr [eax + 4 * (4 * 6 + 0)], edx
__asm mov dword ptr [eax + 4 * (4 * 6 + 1)], edx
__asm mov dword ptr [eax + 4 * (4 * 6 + 2)], edx
__asm mov dword ptr [eax + 4 * (4 * 6 + 3)], edx
__asm mov dword ptr [eax + 4 * (4 * 6 + 4)], edx
__asm mov dword ptr [eax + 4 * (4 * 6 + 5)], edx
	__asm ret 4
}

SMLXTmpMatrix nrxc(pool,6,5)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 6, 5) = SMLXTmpMatrix::InitTmpPool(6, 5, nrxc(pool,6,5), SMLXMatrix::poolSize);
	
SMLXTmpMatrix& __fastcall nrxc(TMat,6,5)() 
{
	enum {pool_sz = sizeof(nrxc(pool,6,5)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,6,5)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,6,5)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,6,5)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,6,5)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op add
#define size 30
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,6,5)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,6,5)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op sub
#define size 30
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
void __fastcall nrxc(AddTo,6,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op add
#define size 30
#include <smlxun2.inl>
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

//__declspec(naked)
void __fastcall nrxc(SubFrom,6,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op sub
#define size 30
#include <smlxun2.inl>
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,6,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
#define size 30
#include "smlxunas.inl"
#undef size

/*	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
__asm fld dword ptr [edx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 0)]
__asm fld dword ptr [edx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 1)]
__asm fld dword ptr [edx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 2)]
__asm fld dword ptr [edx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 3)]
__asm fld dword ptr [edx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 4)]
__asm fld dword ptr [edx + 4 * (1 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 0)]
__asm fld dword ptr [edx + 4 * (1 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 1)]
__asm fld dword ptr [edx + 4 * (1 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 2)]
__asm fld dword ptr [edx + 4 * (1 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 3)]
__asm fld dword ptr [edx + 4 * (1 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 4)]
__asm fld dword ptr [edx + 4 * (2 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 0)]
__asm fld dword ptr [edx + 4 * (2 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 1)]
__asm fld dword ptr [edx + 4 * (2 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 2)]
__asm fld dword ptr [edx + 4 * (2 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 3)]
__asm fld dword ptr [edx + 4 * (2 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 4)]
__asm fld dword ptr [edx + 4 * (3 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 0)]
__asm fld dword ptr [edx + 4 * (3 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 1)]
__asm fld dword ptr [edx + 4 * (3 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 2)]
__asm fld dword ptr [edx + 4 * (3 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 3)]
__asm fld dword ptr [edx + 4 * (3 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 4)]
__asm fld dword ptr [edx + 4 * (4 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 0)]
__asm fld dword ptr [edx + 4 * (4 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 1)]
__asm fld dword ptr [edx + 4 * (4 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 2)]
__asm fld dword ptr [edx + 4 * (4 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 3)]
__asm fld dword ptr [edx + 4 * (4 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 4)]
__asm fld dword ptr [edx + 4 * (5 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 0)]
__asm fld dword ptr [edx + 4 * (5 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 1)]
__asm fld dword ptr [edx + 4 * (5 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 2)]
__asm fld dword ptr [edx + 4 * (5 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 3)]
__asm fld dword ptr [edx + 4 * (5 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 4)]
*/

	__asm ret
}

__declspec(naked)
void __fastcall nrxc(AssignValue,6,5)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 5 + 0)], edx
__asm mov dword ptr [eax + 4 * (0 * 5 + 1)], edx
__asm mov dword ptr [eax + 4 * (0 * 5 + 2)], edx
__asm mov dword ptr [eax + 4 * (0 * 5 + 3)], edx
__asm mov dword ptr [eax + 4 * (0 * 5 + 4)], edx
__asm mov dword ptr [eax + 4 * (1 * 5 + 0)], edx
__asm mov dword ptr [eax + 4 * (1 * 5 + 1)], edx
__asm mov dword ptr [eax + 4 * (1 * 5 + 2)], edx
__asm mov dword ptr [eax + 4 * (1 * 5 + 3)], edx
__asm mov dword ptr [eax + 4 * (1 * 5 + 4)], edx
__asm mov dword ptr [eax + 4 * (2 * 5 + 0)], edx
__asm mov dword ptr [eax + 4 * (2 * 5 + 1)], edx
__asm mov dword ptr [eax + 4 * (2 * 5 + 2)], edx
__asm mov dword ptr [eax + 4 * (2 * 5 + 3)], edx
__asm mov dword ptr [eax + 4 * (2 * 5 + 4)], edx
__asm mov dword ptr [eax + 4 * (3 * 5 + 0)], edx
__asm mov dword ptr [eax + 4 * (3 * 5 + 1)], edx
__asm mov dword ptr [eax + 4 * (3 * 5 + 2)], edx
__asm mov dword ptr [eax + 4 * (3 * 5 + 3)], edx
__asm mov dword ptr [eax + 4 * (3 * 5 + 4)], edx
__asm mov dword ptr [eax + 4 * (4 * 5 + 0)], edx
__asm mov dword ptr [eax + 4 * (4 * 5 + 1)], edx
__asm mov dword ptr [eax + 4 * (4 * 5 + 2)], edx
__asm mov dword ptr [eax + 4 * (4 * 5 + 3)], edx
__asm mov dword ptr [eax + 4 * (4 * 5 + 4)], edx
__asm mov dword ptr [eax + 4 * (5 * 5 + 0)], edx
__asm mov dword ptr [eax + 4 * (5 * 5 + 1)], edx
__asm mov dword ptr [eax + 4 * (5 * 5 + 2)], edx
__asm mov dword ptr [eax + 4 * (5 * 5 + 3)], edx
__asm mov dword ptr [eax + 4 * (5 * 5 + 4)], edx
	__asm ret 4
}

SMLXTmpMatrix nrxc(pool,6,6)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 6, 6) = SMLXTmpMatrix::InitTmpPool(6, 6, nrxc(pool,6,6), SMLXMatrix::poolSize);
	
SMLXTmpMatrix& __fastcall nrxc(TMat,6,6)() 
{
	enum {pool_sz = sizeof(nrxc(pool,6,6)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,6,6)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,6,6)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,6,6)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
/*	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,6,6)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op add
#define size 36
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;*/
	nrxc(TMat,6,6)();
	__asm {
		push ebx 
		mov ebx, dword ptr [ecx] 
		mov ecx, dword ptr [edx] 
		mov edx, eax 
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		movaps	xmm2,	XMMWORD PTR [ebx+16]
		movaps	xmm3,	XMMWORD PTR [ecx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax], xmm0
		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2
		movaps	xmm6,	XMMWORD PTR [ebx+48]
		movaps	xmm7,	XMMWORD PTR [ecx+48]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4
		movaps	xmm0,	XMMWORD PTR [ebx+64]
		movaps	xmm1,	XMMWORD PTR [ecx+64]
		addps   xmm6,	xmm7
		movaps  XMMWORD PTR [eax+48], xmm6
		movaps	xmm2,	XMMWORD PTR [ebx+80]
		movaps	xmm3,	XMMWORD PTR [ecx+80]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+64], xmm0
		movaps	xmm4,	XMMWORD PTR [ebx+96]
		movaps	xmm5,	XMMWORD PTR [ecx+96]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+80], xmm2
		movaps	xmm6,	XMMWORD PTR [ebx+112]
		movaps	xmm7,	XMMWORD PTR [ecx+112]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+96], xmm4
		movaps	xmm0,	XMMWORD PTR [ebx+128]
		movaps	xmm1,	XMMWORD PTR [ecx+128]
		addps   xmm6,	xmm7
		movaps  XMMWORD PTR [eax+112], xmm6
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+128], xmm0

		mov eax, edx 
		pop ebx 
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,6,6)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
/*	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,6,6)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op sub
#define size 36
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;*/

	nrxc(TMat,6,6)();
	__asm {
		push ebx 
		mov ebx, dword ptr [ecx] 
		mov ecx, dword ptr [edx] 
		mov edx, eax 
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		movaps	xmm2,	XMMWORD PTR [ebx+16]
		movaps	xmm3,	XMMWORD PTR [ecx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax], xmm0
		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2
		movaps	xmm6,	XMMWORD PTR [ebx+48]
		movaps	xmm7,	XMMWORD PTR [ecx+48]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4
		movaps	xmm0,	XMMWORD PTR [ebx+64]
		movaps	xmm1,	XMMWORD PTR [ecx+64]
		subps   xmm6,	xmm7
		movaps  XMMWORD PTR [eax+48], xmm6
		movaps	xmm2,	XMMWORD PTR [ebx+80]
		movaps	xmm3,	XMMWORD PTR [ecx+80]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+64], xmm0
		movaps	xmm4,	XMMWORD PTR [ebx+96]
		movaps	xmm5,	XMMWORD PTR [ecx+96]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+80], xmm2
		movaps	xmm6,	XMMWORD PTR [ebx+112]
		movaps	xmm7,	XMMWORD PTR [ecx+112]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+96], xmm4
		movaps	xmm0,	XMMWORD PTR [ebx+128]
		movaps	xmm1,	XMMWORD PTR [ecx+128]
		subps   xmm6,	xmm7
		movaps  XMMWORD PTR [eax+112], xmm6
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+128], xmm0

		mov eax, edx 
		pop ebx 
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,6,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
/*	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op add
#define size 36
#include <smlxun2.inl>
#undef size
#undef op*/
	__asm
	{
		mov eax, dword ptr [edx] 
		mov edx, eax 
		mov eax, dword ptr [ecx] 
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [edx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		movaps	xmm2,	XMMWORD PTR [edx+16]
		movaps	xmm3,	XMMWORD PTR [ecx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx], xmm0
		movaps	xmm4,	XMMWORD PTR [edx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2
		movaps	xmm6,	XMMWORD PTR [edx+48]
		movaps	xmm7,	XMMWORD PTR [ecx+48]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4
		movaps	xmm0,	XMMWORD PTR [edx+64]
		movaps	xmm1,	XMMWORD PTR [ecx+64]
		addps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6
		movaps	xmm2,	XMMWORD PTR [edx+80]
		movaps	xmm3,	XMMWORD PTR [ecx+80]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+64], xmm0
		movaps	xmm4,	XMMWORD PTR [edx+96]
		movaps	xmm5,	XMMWORD PTR [ecx+96]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+80], xmm2
		movaps	xmm6,	XMMWORD PTR [edx+112]
		movaps	xmm7,	XMMWORD PTR [ecx+112]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+96], xmm4
		movaps	xmm0,	XMMWORD PTR [edx+128]
		movaps	xmm1,	XMMWORD PTR [ecx+128]
		addps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+112], xmm6
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+128], xmm0

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,6,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
/*	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op sub
#define size 36
#include <smlxun2.inl>
#undef size
#undef op*/
	__asm
	{
		mov eax, dword ptr [edx] 
		mov edx, eax 
		mov eax, dword ptr [ecx] 
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [edx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		movaps	xmm2,	XMMWORD PTR [edx+16]
		movaps	xmm3,	XMMWORD PTR [ecx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx], xmm0
		movaps	xmm4,	XMMWORD PTR [edx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2
		movaps	xmm6,	XMMWORD PTR [edx+48]
		movaps	xmm7,	XMMWORD PTR [ecx+48]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4
		movaps	xmm0,	XMMWORD PTR [edx+64]
		movaps	xmm1,	XMMWORD PTR [ecx+64]
		subps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6
		movaps	xmm2,	XMMWORD PTR [edx+80]
		movaps	xmm3,	XMMWORD PTR [ecx+80]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+64], xmm0
		movaps	xmm4,	XMMWORD PTR [edx+96]
		movaps	xmm5,	XMMWORD PTR [ecx+96]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+80], xmm2
		movaps	xmm6,	XMMWORD PTR [edx+112]
		movaps	xmm7,	XMMWORD PTR [ecx+112]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+96], xmm4
		movaps	xmm0,	XMMWORD PTR [edx+128]
		movaps	xmm1,	XMMWORD PTR [ecx+128]
		subps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+112], xmm6
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+128], xmm0

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,6,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
#define size 36
#include "smlxunas.inl"
#undef size
/*	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
	__asm movaps xmm0, xmmword ptr [edx]
	__asm movaps xmm1, xmmword ptr [edx+16]
	__asm movaps xmmword ptr [ecx], xmm0
	__asm movaps xmmword ptr [ecx+16], xmm1
	__asm movaps xmm2, xmmword ptr [edx+32]
	__asm movaps xmm3, xmmword ptr [edx+48]
	__asm movaps xmmword ptr [ecx+32], xmm2
	__asm movaps xmmword ptr [ecx+48], xmm3
	__asm movaps xmm4, xmmword ptr [edx+64]
	__asm movaps xmm5, xmmword ptr [edx+80]
	__asm movaps xmmword ptr [ecx+64], xmm4
	__asm movaps xmmword ptr [ecx+80], xmm5
	__asm movaps xmm6, xmmword ptr [edx+96]
	__asm movaps xmm7, xmmword ptr [edx+112]
	__asm movaps xmmword ptr [ecx+96], xmm6
	__asm movaps xmmword ptr [ecx+112], xmm7
	__asm movaps xmm0, xmmword ptr [edx+128]
	__asm movaps xmmword ptr [ecx+128], xmm0
*/
/*__asm fld dword ptr [edx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 0)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 1)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 2)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 3)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 4)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 5)]
__asm fld dword ptr [edx + 4 * (1 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 0)]
__asm fld dword ptr [edx + 4 * (1 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 1)]
__asm fld dword ptr [edx + 4 * (1 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 2)]
__asm fld dword ptr [edx + 4 * (1 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 3)]
__asm fld dword ptr [edx + 4 * (1 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 4)]
__asm fld dword ptr [edx + 4 * (1 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 5)]
__asm fld dword ptr [edx + 4 * (2 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 0)]
__asm fld dword ptr [edx + 4 * (2 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 1)]
__asm fld dword ptr [edx + 4 * (2 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 2)]
__asm fld dword ptr [edx + 4 * (2 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 3)]
__asm fld dword ptr [edx + 4 * (2 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 4)]
__asm fld dword ptr [edx + 4 * (2 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 5)]
__asm fld dword ptr [edx + 4 * (3 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 0)]
__asm fld dword ptr [edx + 4 * (3 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 1)]
__asm fld dword ptr [edx + 4 * (3 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 2)]
__asm fld dword ptr [edx + 4 * (3 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 3)]
__asm fld dword ptr [edx + 4 * (3 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 4)]
__asm fld dword ptr [edx + 4 * (3 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 5)]
__asm fld dword ptr [edx + 4 * (4 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 0)]
__asm fld dword ptr [edx + 4 * (4 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 1)]
__asm fld dword ptr [edx + 4 * (4 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 2)]
__asm fld dword ptr [edx + 4 * (4 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 3)]
__asm fld dword ptr [edx + 4 * (4 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 4)]
__asm fld dword ptr [edx + 4 * (4 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 5)]
__asm fld dword ptr [edx + 4 * (5 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 0)]
__asm fld dword ptr [edx + 4 * (5 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 1)]
__asm fld dword ptr [edx + 4 * (5 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 2)]
__asm fld dword ptr [edx + 4 * (5 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 3)]
__asm fld dword ptr [edx + 4 * (5 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 4)]
__asm fld dword ptr [edx + 4 * (5 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 5)]
*/
	__asm ret
}

__declspec(naked)
void __fastcall nrxc(AssignValue,6,6)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 6 + 0)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 1)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 2)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 3)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 4)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 5)], edx
__asm mov dword ptr [eax + 4 * (1 * 6 + 0)], edx
__asm mov dword ptr [eax + 4 * (1 * 6 + 1)], edx
__asm mov dword ptr [eax + 4 * (1 * 6 + 2)], edx
__asm mov dword ptr [eax + 4 * (1 * 6 + 3)], edx
__asm mov dword ptr [eax + 4 * (1 * 6 + 4)], edx
__asm mov dword ptr [eax + 4 * (1 * 6 + 5)], edx
__asm mov dword ptr [eax + 4 * (2 * 6 + 0)], edx
__asm mov dword ptr [eax + 4 * (2 * 6 + 1)], edx
__asm mov dword ptr [eax + 4 * (2 * 6 + 2)], edx
__asm mov dword ptr [eax + 4 * (2 * 6 + 3)], edx
__asm mov dword ptr [eax + 4 * (2 * 6 + 4)], edx
__asm mov dword ptr [eax + 4 * (2 * 6 + 5)], edx
__asm mov dword ptr [eax + 4 * (3 * 6 + 0)], edx
__asm mov dword ptr [eax + 4 * (3 * 6 + 1)], edx
__asm mov dword ptr [eax + 4 * (3 * 6 + 2)], edx
__asm mov dword ptr [eax + 4 * (3 * 6 + 3)], edx
__asm mov dword ptr [eax + 4 * (3 * 6 + 4)], edx
__asm mov dword ptr [eax + 4 * (3 * 6 + 5)], edx
__asm mov dword ptr [eax + 4 * (4 * 6 + 0)], edx
__asm mov dword ptr [eax + 4 * (4 * 6 + 1)], edx
__asm mov dword ptr [eax + 4 * (4 * 6 + 2)], edx
__asm mov dword ptr [eax + 4 * (4 * 6 + 3)], edx
__asm mov dword ptr [eax + 4 * (4 * 6 + 4)], edx
__asm mov dword ptr [eax + 4 * (4 * 6 + 5)], edx
__asm mov dword ptr [eax + 4 * (5 * 6 + 0)], edx
__asm mov dword ptr [eax + 4 * (5 * 6 + 1)], edx
__asm mov dword ptr [eax + 4 * (5 * 6 + 2)], edx
__asm mov dword ptr [eax + 4 * (5 * 6 + 3)], edx
__asm mov dword ptr [eax + 4 * (5 * 6 + 4)], edx
__asm mov dword ptr [eax + 4 * (5 * 6 + 5)], edx
	__asm ret 4
}

SMLXTmpMatrix nrxc(pool,6,1)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 6, 1) = SMLXTmpMatrix::InitTmpPool(6, 1, nrxc(pool,6,1), SMLXMatrix::poolSize);
	
SMLXTmpMatrix& __fastcall nrxc(TMat,6,1)() 
{
	enum {pool_sz = sizeof(nrxc(pool,6,1)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,6,1)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,6,1)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,6,1)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,6,1)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op add
#define size 6
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,6,1)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,6,1)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op sub
#define size 6
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
void __fastcall nrxc(AddTo,6,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op add
#define size 6
#include <smlxun2.inl>
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

//__declspec(naked)
void __fastcall nrxc(SubFrom,6,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op sub
#define size 6
#include <smlxun2.inl>
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,6,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
#define size 6
#include "smlxunas.inl"
#undef size

/*	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
__asm fld dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]
__asm fld dword ptr [edx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 1 + 0)]
__asm fld dword ptr [edx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 1 + 0)]
__asm fld dword ptr [edx + 4 * (3 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 1 + 0)]
__asm fld dword ptr [edx + 4 * (4 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 1 + 0)]
__asm fld dword ptr [edx + 4 * (5 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (5 * 1 + 0)]
*/
	__asm ret
}

__declspec(naked)
void __fastcall nrxc(AssignValue,6,1)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 1 + 0)], edx
__asm mov dword ptr [eax + 4 * (1 * 1 + 0)], edx
__asm mov dword ptr [eax + 4 * (2 * 1 + 0)], edx
__asm mov dword ptr [eax + 4 * (3 * 1 + 0)], edx
__asm mov dword ptr [eax + 4 * (4 * 1 + 0)], edx
__asm mov dword ptr [eax + 4 * (5 * 1 + 0)], edx
	__asm ret 4
}

SMLXTmpMatrix nrxc(pool,1,6)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 1, 6) = SMLXTmpMatrix::InitTmpPool(1, 6, nrxc(pool,1,6), SMLXMatrix::poolSize);
	
SMLXTmpMatrix& __fastcall nrxc(TMat,1,6)() 
{
	enum {pool_sz = sizeof(nrxc(pool,1,6)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,1,6)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,1,6)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,1,6)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,1,6)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op add
#define size 6
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,1,6)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,1,6)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op sub
#define size 6
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
void __fastcall nrxc(AddTo,1,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op add
#define size 6
#include <smlxun2.inl>
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

//__declspec(naked)
void __fastcall nrxc(SubFrom,1,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op sub
#define size 6
#include <smlxun2.inl>
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,1,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
#define size 6
#include "smlxunas.inl"
#undef size

/*	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
__asm fld dword ptr [edx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 0)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 1)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 2)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 3)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 4)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 5)]
*/

	__asm ret
}

__declspec(naked)
void __fastcall nrxc(AssignValue,1,6)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 6 + 0)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 1)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 2)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 3)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 4)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 5)], edx
	__asm ret 4
}

SMLXTmpMatrix nrxc(pool,6,3)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 6, 3) = SMLXTmpMatrix::InitTmpPool(6, 3, nrxc(pool,6,3), SMLXMatrix::poolSize);
	
SMLXTmpMatrix& __fastcall nrxc(TMat,6,3)() 
{
	enum {pool_sz = sizeof(nrxc(pool,6,3)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,6,3)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,6,3)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,6,3)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	nrxc(TMat,6,3)();
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,6,3)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op add
#define size 18
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,6,3)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,6,3)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op sub
#define size 18
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
void __fastcall nrxc(AddTo,6,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op add
#define size 18
#include <smlxun2.inl>
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

//__declspec(naked)
void __fastcall nrxc(SubFrom,6,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op sub
#define size 18
#include <smlxun2.inl>
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,6,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
#define size 18
#include "smlxunas.inl"
#undef size

/*	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
__asm fld dword ptr [edx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 0)]
__asm fld dword ptr [edx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 1)]
__asm fld dword ptr [edx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 2)]
__asm fld dword ptr [edx + 4 * (1 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 0)]
__asm fld dword ptr [edx + 4 * (1 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 1)]
__asm fld dword ptr [edx + 4 * (1 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 2)]
__asm fld dword ptr [edx + 4 * (2 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 0)]
__asm fld dword ptr [edx + 4 * (2 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 1)]
__asm fld dword ptr [edx + 4 * (2 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 2)]
__asm fld dword ptr [edx + 4 * (3 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 3 + 0)]
__asm fld dword ptr [edx + 4 * (3 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 3 + 1)]
__asm fld dword ptr [edx + 4 * (3 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 3 + 2)]
__asm fld dword ptr [edx + 4 * (4 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 3 + 0)]
__asm fld dword ptr [edx + 4 * (4 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (4 * 3 + 1)]
__asm fld dword ptr [edx + 4 * (4 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (4 * 3 + 2)]
__asm fld dword ptr [edx + 4 * (5 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (5 * 3 + 0)]
__asm fld dword ptr [edx + 4 * (5 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (5 * 3 + 1)]
__asm fld dword ptr [edx + 4 * (5 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (5 * 3 + 2)]
*/

	__asm ret
}

__declspec(naked)
void __fastcall nrxc(AssignValue,6,3)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 3 + 0)], edx
__asm mov dword ptr [eax + 4 * (0 * 3 + 1)], edx
__asm mov dword ptr [eax + 4 * (0 * 3 + 2)], edx
__asm mov dword ptr [eax + 4 * (1 * 3 + 0)], edx
__asm mov dword ptr [eax + 4 * (1 * 3 + 1)], edx
__asm mov dword ptr [eax + 4 * (1 * 3 + 2)], edx
__asm mov dword ptr [eax + 4 * (2 * 3 + 0)], edx
__asm mov dword ptr [eax + 4 * (2 * 3 + 1)], edx
__asm mov dword ptr [eax + 4 * (2 * 3 + 2)], edx
__asm mov dword ptr [eax + 4 * (3 * 3 + 0)], edx
__asm mov dword ptr [eax + 4 * (3 * 3 + 1)], edx
__asm mov dword ptr [eax + 4 * (3 * 3 + 2)], edx
__asm mov dword ptr [eax + 4 * (4 * 3 + 0)], edx
__asm mov dword ptr [eax + 4 * (4 * 3 + 1)], edx
__asm mov dword ptr [eax + 4 * (4 * 3 + 2)], edx
__asm mov dword ptr [eax + 4 * (5 * 3 + 0)], edx
__asm mov dword ptr [eax + 4 * (5 * 3 + 1)], edx
__asm mov dword ptr [eax + 4 * (5 * 3 + 2)], edx
	__asm ret 4
}

SMLXTmpMatrix nrxc(pool,3,6)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 3, 6) = SMLXTmpMatrix::InitTmpPool(3, 6, nrxc(pool,3,6), SMLXMatrix::poolSize);
	
SMLXTmpMatrix& __fastcall nrxc(TMat,3,6)() 
{
	enum {pool_sz = sizeof(nrxc(pool,3,6)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,3,6)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,3,6)[SMLXMatrix::poolSize - 1];
	__asm mov   eax, dword ptr [poolPointer]
		__asm cmp   eax, dword ptr [poolEnd]
		__asm je    adjust
		__asm jmp   incpp
		adjust:
	__asm sub   eax, pool_sz
		incpp:
	__asm add   eax, class_sz
		__asm mov   dword ptr [poolPointer], eax
		
	ASSERT_SUBEXPRESSION_COUNT
	return *(SMLXTmpMatrix*)poolPointer;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,3,6)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,3,6)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op add
#define size 18
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,3,6)(const SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	SMLXTmpMatrix& dmat = nrxc(TMat,3,6)();
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= dmat.Data();
#define op sub
#define size 18
#include <smlxun2.inl>
#undef size
#undef op
	return dmat;
}

//__declspec(naked)
void __fastcall nrxc(AddTo,3,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op add
#define size 18
#include <smlxun2.inl>
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

//__declspec(naked)
void __fastcall nrxc(SubFrom,3,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__m128	m1, m2, rez;
	float *a			= A.Data();
	float *b			= B.Data();
	float *dst			= a;
#define op sub
#define size 18
#include <smlxun2.inl>
#undef size
#undef op
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,3,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
#define size 18
#include "smlxunas.inl"
#undef size

/*	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax
__asm fld dword ptr [edx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 0)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 1)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 2)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 3)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 4)]
__asm fld dword ptr [edx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 5)]
__asm fld dword ptr [edx + 4 * (1 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 0)]
__asm fld dword ptr [edx + 4 * (1 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 1)]
__asm fld dword ptr [edx + 4 * (1 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 2)]
__asm fld dword ptr [edx + 4 * (1 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 3)]
__asm fld dword ptr [edx + 4 * (1 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 4)]
__asm fld dword ptr [edx + 4 * (1 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 5)]
__asm fld dword ptr [edx + 4 * (2 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 0)]
__asm fld dword ptr [edx + 4 * (2 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 1)]
__asm fld dword ptr [edx + 4 * (2 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 2)]
__asm fld dword ptr [edx + 4 * (2 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 3)]
__asm fld dword ptr [edx + 4 * (2 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 4)]
__asm fld dword ptr [edx + 4 * (2 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 5)]
*/

	__asm ret
}

__declspec(naked)
void __fastcall nrxc(AssignValue,3,6)(SMLXMatrix &A, float initialValue) 
{
	__asm mov	eax, dword ptr [ecx]
	__asm mov   edx, dword ptr [esp+4]
__asm mov dword ptr [eax + 4 * (0 * 6 + 0)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 1)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 2)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 3)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 4)], edx
__asm mov dword ptr [eax + 4 * (0 * 6 + 5)], edx
__asm mov dword ptr [eax + 4 * (1 * 6 + 0)], edx
__asm mov dword ptr [eax + 4 * (1 * 6 + 1)], edx
__asm mov dword ptr [eax + 4 * (1 * 6 + 2)], edx
__asm mov dword ptr [eax + 4 * (1 * 6 + 3)], edx
__asm mov dword ptr [eax + 4 * (1 * 6 + 4)], edx
__asm mov dword ptr [eax + 4 * (1 * 6 + 5)], edx
__asm mov dword ptr [eax + 4 * (2 * 6 + 0)], edx
__asm mov dword ptr [eax + 4 * (2 * 6 + 1)], edx
__asm mov dword ptr [eax + 4 * (2 * 6 + 2)], edx
__asm mov dword ptr [eax + 4 * (2 * 6 + 3)], edx
__asm mov dword ptr [eax + 4 * (2 * 6 + 4)], edx
__asm mov dword ptr [eax + 4 * (2 * 6 + 5)], edx
	__asm ret 4
}




//////////////////////////////////////////////////////////////
//
// Optimized Multiplications for small dimensions
//
/////////////////////////////////////////////////////////////
#include "smlxSSEmult.inl"

//////////////////////////////////////////////////////////////
//
// That's all, folks
//
/////////////////////////////////////////////////////////////

