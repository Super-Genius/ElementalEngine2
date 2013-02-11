#include "xmmintrin.h"

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

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,1,1)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,1,1)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		fld 	DWORD PTR [ebx+0]
		fadd 	DWORD PTR [ecx+0]
		fstp	DWORD PTR [eax+0]

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,1,1)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,1,1)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		fld 	DWORD PTR [ebx+0]
		fsub 	DWORD PTR [ecx+0]
		fstp	DWORD PTR [eax+0]

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,1,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		fld 	DWORD PTR [ecx+0]
		fadd 	DWORD PTR [edx+0]
		fstp	DWORD PTR [ecx+0]

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,1,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		fld 	DWORD PTR [ecx+0]
		fsub 	DWORD PTR [edx+0]
		fstp	DWORD PTR [ecx+0]

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,1,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		fld  dword ptr [edx+0]
		fstp dword ptr [ecx+0]

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,1,1)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		mov	edx, dword ptr [esp+4]

		mov dword ptr [eax + 0], edx

		ret 4
	}
}

SMLXTmpMatrix nrxc(pool,1,2)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 1, 2) = SMLXTmpMatrix::InitTmpPool(1, 2, nrxc(pool,1,2), SMLXMatrix::poolSize);

SMLXTmpMatrix& __fastcall nrxc(TMat,1,2)()
{
	enum {pool_sz = sizeof(nrxc(pool,1,2)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,1,2)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,1,2)[SMLXMatrix::poolSize - 1];
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
SMLXTmpMatrix& __fastcall nrxc(Add,1,2)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,1,2)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movhps xmm2, qword ptr [ebx+0]
		movhps xmm3, qword ptr [ecx+0]
		addps  xmm2, xmm3
		movhps qword ptr [eax+0], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,1,2)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,1,2)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movhps xmm2, qword ptr [ebx+0]
		movhps xmm3, qword ptr [ecx+0]
		subps  xmm2, xmm3
		movhps qword ptr [eax+0], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,1,2)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movhps xmm2, qword ptr [ecx+0]
		movhps xmm3, qword ptr [edx+0]
		addps  xmm2, xmm3
		movhps qword ptr [ecx+0], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,1,2)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movhps xmm2, qword ptr [ecx+0]
		movhps xmm3, qword ptr [edx+0]
		subps  xmm2, xmm3
		movhps qword ptr [ecx+0], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,1,2)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movhps xmm3, qword ptr [edx+0]
		movhps qword ptr [ecx+0], xmm3

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,1,2)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		mov	edx, dword ptr [esp+4]

		mov dword ptr [eax + 0], edx
		mov dword ptr [eax + 4], edx

		ret 4
	}
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

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,1,3)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,1,3)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movhps xmm2, qword ptr [ebx+0]
		movhps xmm3, qword ptr [ecx+0]
		addps  xmm2, xmm3
		movhps qword ptr [eax+0], xmm2

		fld 	DWORD PTR [ebx+8]
		fadd 	DWORD PTR [ecx+8]
		fstp	DWORD PTR [eax+8]

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,1,3)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,1,3)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movhps xmm2, qword ptr [ebx+0]
		movhps xmm3, qword ptr [ecx+0]
		subps  xmm2, xmm3
		movhps qword ptr [eax+0], xmm2

		fld 	DWORD PTR [ebx+8]
		fsub 	DWORD PTR [ecx+8]
		fstp	DWORD PTR [eax+8]

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,1,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movhps xmm2, qword ptr [ecx+0]
		movhps xmm3, qword ptr [edx+0]
		addps  xmm2, xmm3
		movhps qword ptr [ecx+0], xmm2

		fld 	DWORD PTR [ecx+8]
		fadd 	DWORD PTR [edx+8]
		fstp	DWORD PTR [ecx+8]

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,1,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movhps xmm2, qword ptr [ecx+0]
		movhps xmm3, qword ptr [edx+0]
		subps  xmm2, xmm3
		movhps qword ptr [ecx+0], xmm2

		fld 	DWORD PTR [ecx+8]
		fsub 	DWORD PTR [edx+8]
		fstp	DWORD PTR [ecx+8]

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,1,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movhps xmm3, qword ptr [edx+0]
		movhps qword ptr [ecx+0], xmm3
		fld  dword ptr [edx+8]
		fstp dword ptr [ecx+8]

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,1,3)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		mov	edx, dword ptr [esp+4]

		mov dword ptr [eax + 0], edx
		mov dword ptr [eax + 4], edx
		mov dword ptr [eax + 8], edx

		ret 4
	}
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

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,1,4)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,1,4)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+0], xmm0

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,1,4)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,1,4)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+0], xmm0

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,1,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,1,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,1,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm2, xmmword ptr [edx+0]
		movaps xmmword ptr [ecx+0], xmm2

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,1,4)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0

		ret 4
	}
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

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,1,5)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,1,5)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+0], xmm0

		fld 	DWORD PTR [ebx+16]
		fadd 	DWORD PTR [ecx+16]
		fstp	DWORD PTR [eax+16]

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,1,5)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,1,5)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+0], xmm0

		fld 	DWORD PTR [ebx+16]
		fsub 	DWORD PTR [ecx+16]
		fstp	DWORD PTR [eax+16]

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,1,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		fld 	DWORD PTR [ecx+16]
		fadd 	DWORD PTR [edx+16]
		fstp	DWORD PTR [ecx+16]

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,1,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		fld 	DWORD PTR [ecx+16]
		fsub 	DWORD PTR [edx+16]
		fstp	DWORD PTR [ecx+16]

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,1,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm2, xmmword ptr [edx+0]
		movaps xmmword ptr [ecx+0], xmm2
		fld  dword ptr [edx+16]
		fstp dword ptr [ecx+16]

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,1,5)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movss	dword ptr [eax+16], xmm0

		ret 4
	}
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

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,1,6)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,1,6)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+0], xmm0

		movhps xmm2, qword ptr [ebx+16]
		movhps xmm3, qword ptr [ecx+16]
		addps  xmm2, xmm3
		movhps qword ptr [eax+16], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,1,6)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,1,6)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+0], xmm0

		movhps xmm2, qword ptr [ebx+16]
		movhps xmm3, qword ptr [ecx+16]
		subps  xmm2, xmm3
		movhps qword ptr [eax+16], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,1,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movhps xmm2, qword ptr [ecx+16]
		movhps xmm3, qword ptr [edx+16]
		addps  xmm2, xmm3
		movhps qword ptr [ecx+16], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,1,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movhps xmm2, qword ptr [ecx+16]
		movhps xmm3, qword ptr [edx+16]
		subps  xmm2, xmm3
		movhps qword ptr [ecx+16], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,1,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm2, xmmword ptr [edx+0]
		movaps xmmword ptr [ecx+0], xmm2
		movhps xmm3, qword ptr [edx+16]
		movhps qword ptr [ecx+16], xmm3

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,1,6)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movhps	qword ptr [eax+16], xmm0

		ret 4
	}
}

SMLXTmpMatrix nrxc(pool,2,1)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 2, 1) = SMLXTmpMatrix::InitTmpPool(2, 1, nrxc(pool,2,1), SMLXMatrix::poolSize);

SMLXTmpMatrix& __fastcall nrxc(TMat,2,1)()
{
	enum {pool_sz = sizeof(nrxc(pool,2,1)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,2,1)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,2,1)[SMLXMatrix::poolSize - 1];
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
SMLXTmpMatrix& __fastcall nrxc(Add,2,1)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,2,1)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movhps xmm2, qword ptr [ebx+0]
		movhps xmm3, qword ptr [ecx+0]
		addps  xmm2, xmm3
		movhps qword ptr [eax+0], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,2,1)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,2,1)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movhps xmm2, qword ptr [ebx+0]
		movhps xmm3, qword ptr [ecx+0]
		subps  xmm2, xmm3
		movhps qword ptr [eax+0], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,2,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movhps xmm2, qword ptr [ecx+0]
		movhps xmm3, qword ptr [edx+0]
		addps  xmm2, xmm3
		movhps qword ptr [ecx+0], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,2,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movhps xmm2, qword ptr [ecx+0]
		movhps xmm3, qword ptr [edx+0]
		subps  xmm2, xmm3
		movhps qword ptr [ecx+0], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,2,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movhps xmm3, qword ptr [edx+0]
		movhps qword ptr [ecx+0], xmm3

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,2,1)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		mov	edx, dword ptr [esp+4]

		mov dword ptr [eax + 0], edx
		mov dword ptr [eax + 4], edx

		ret 4
	}
}

SMLXTmpMatrix nrxc(pool,2,2)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 2, 2) = SMLXTmpMatrix::InitTmpPool(2, 2, nrxc(pool,2,2), SMLXMatrix::poolSize);

SMLXTmpMatrix& __fastcall nrxc(TMat,2,2)()
{
	enum {pool_sz = sizeof(nrxc(pool,2,2)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,2,2)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,2,2)[SMLXMatrix::poolSize - 1];
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
SMLXTmpMatrix& __fastcall nrxc(Add,2,2)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,2,2)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+0], xmm0

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,2,2)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,2,2)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+0], xmm0

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,2,2)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,2,2)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,2,2)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm2, xmmword ptr [edx+0]
		movaps xmmword ptr [ecx+0], xmm2

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,2,2)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0

		ret 4
	}
}

SMLXTmpMatrix nrxc(pool,2,3)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 2, 3) = SMLXTmpMatrix::InitTmpPool(2, 3, nrxc(pool,2,3), SMLXMatrix::poolSize);

SMLXTmpMatrix& __fastcall nrxc(TMat,2,3)()
{
	enum {pool_sz = sizeof(nrxc(pool,2,3)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,2,3)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,2,3)[SMLXMatrix::poolSize - 1];
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
SMLXTmpMatrix& __fastcall nrxc(Add,2,3)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,2,3)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+0], xmm0

		movhps xmm2, qword ptr [ebx+16]
		movhps xmm3, qword ptr [ecx+16]
		addps  xmm2, xmm3
		movhps qword ptr [eax+16], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,2,3)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,2,3)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+0], xmm0

		movhps xmm2, qword ptr [ebx+16]
		movhps xmm3, qword ptr [ecx+16]
		subps  xmm2, xmm3
		movhps qword ptr [eax+16], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,2,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movhps xmm2, qword ptr [ecx+16]
		movhps xmm3, qword ptr [edx+16]
		addps  xmm2, xmm3
		movhps qword ptr [ecx+16], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,2,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movhps xmm2, qword ptr [ecx+16]
		movhps xmm3, qword ptr [edx+16]
		subps  xmm2, xmm3
		movhps qword ptr [ecx+16], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,2,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm2, xmmword ptr [edx+0]
		movaps xmmword ptr [ecx+0], xmm2
		movhps xmm3, qword ptr [edx+16]
		movhps qword ptr [ecx+16], xmm3

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,2,3)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movhps	qword ptr [eax+16], xmm0

		ret 4
	}
}

SMLXTmpMatrix nrxc(pool,2,4)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 2, 4) = SMLXTmpMatrix::InitTmpPool(2, 4, nrxc(pool,2,4), SMLXMatrix::poolSize);

SMLXTmpMatrix& __fastcall nrxc(TMat,2,4)()
{
	enum {pool_sz = sizeof(nrxc(pool,2,4)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,2,4)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,2,4)[SMLXMatrix::poolSize - 1];
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
SMLXTmpMatrix& __fastcall nrxc(Add,2,4)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,2,4)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,2,4)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,2,4)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,2,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,2,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,2,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,2,4)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0

		ret 4
	}
}

SMLXTmpMatrix nrxc(pool,2,5)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 2, 5) = SMLXTmpMatrix::InitTmpPool(2, 5, nrxc(pool,2,5), SMLXMatrix::poolSize);

SMLXTmpMatrix& __fastcall nrxc(TMat,2,5)()
{
	enum {pool_sz = sizeof(nrxc(pool,2,5)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,2,5)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,2,5)[SMLXMatrix::poolSize - 1];
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
SMLXTmpMatrix& __fastcall nrxc(Add,2,5)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,2,5)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		movhps xmm4, qword ptr [ebx+32]
		movhps xmm5, qword ptr [ecx+32]
		addps  xmm4, xmm5
		movhps qword ptr [eax+32], xmm4

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,2,5)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,2,5)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		movhps xmm4, qword ptr [ebx+32]
		movhps xmm5, qword ptr [ecx+32]
		subps  xmm4, xmm5
		movhps qword ptr [eax+32], xmm4

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,2,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movhps xmm4, qword ptr [ecx+32]
		movhps xmm5, qword ptr [edx+32]
		addps  xmm4, xmm5
		movhps qword ptr [ecx+32], xmm4

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,2,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movhps xmm4, qword ptr [ecx+32]
		movhps xmm5, qword ptr [edx+32]
		subps  xmm4, xmm5
		movhps qword ptr [ecx+32], xmm4

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,2,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movhps xmm5, qword ptr [edx+32]
		movhps qword ptr [ecx+32], xmm5

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,2,5)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movhps	qword ptr [eax+32], xmm0

		ret 4
	}
}

SMLXTmpMatrix nrxc(pool,2,6)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 2, 6) = SMLXTmpMatrix::InitTmpPool(2, 6, nrxc(pool,2,6), SMLXMatrix::poolSize);

SMLXTmpMatrix& __fastcall nrxc(TMat,2,6)()
{
	enum {pool_sz = sizeof(nrxc(pool,2,6)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,2,6)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,2,6)[SMLXMatrix::poolSize - 1];
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
SMLXTmpMatrix& __fastcall nrxc(Add,2,6)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,2,6)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,2,6)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,2,6)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,2,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,2,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,2,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movaps xmm4, xmmword ptr [edx+32]
		movaps xmmword ptr [ecx+32], xmm4

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,2,6)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movaps	xmmword ptr [eax+32], xmm0

		ret 4
	}
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

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,3,1)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,3,1)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movhps xmm2, qword ptr [ebx+0]
		movhps xmm3, qword ptr [ecx+0]
		addps  xmm2, xmm3
		movhps qword ptr [eax+0], xmm2

		fld 	DWORD PTR [ebx+8]
		fadd 	DWORD PTR [ecx+8]
		fstp	DWORD PTR [eax+8]

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,3,1)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,3,1)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movhps xmm2, qword ptr [ebx+0]
		movhps xmm3, qword ptr [ecx+0]
		subps  xmm2, xmm3
		movhps qword ptr [eax+0], xmm2

		fld 	DWORD PTR [ebx+8]
		fsub 	DWORD PTR [ecx+8]
		fstp	DWORD PTR [eax+8]

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,3,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movhps xmm2, qword ptr [ecx+0]
		movhps xmm3, qword ptr [edx+0]
		addps  xmm2, xmm3
		movhps qword ptr [ecx+0], xmm2

		fld 	DWORD PTR [ecx+8]
		fadd 	DWORD PTR [edx+8]
		fstp	DWORD PTR [ecx+8]

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,3,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movhps xmm2, qword ptr [ecx+0]
		movhps xmm3, qword ptr [edx+0]
		subps  xmm2, xmm3
		movhps qword ptr [ecx+0], xmm2

		fld 	DWORD PTR [ecx+8]
		fsub 	DWORD PTR [edx+8]
		fstp	DWORD PTR [ecx+8]

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,3,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movhps xmm3, qword ptr [edx+0]
		movhps qword ptr [ecx+0], xmm3
		fld  dword ptr [edx+8]
		fstp dword ptr [ecx+8]

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,3,1)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		mov	edx, dword ptr [esp+4]

		mov dword ptr [eax + 0], edx
		mov dword ptr [eax + 4], edx
		mov dword ptr [eax + 8], edx

		ret 4
	}
}

SMLXTmpMatrix nrxc(pool,3,2)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 3, 2) = SMLXTmpMatrix::InitTmpPool(3, 2, nrxc(pool,3,2), SMLXMatrix::poolSize);

SMLXTmpMatrix& __fastcall nrxc(TMat,3,2)()
{
	enum {pool_sz = sizeof(nrxc(pool,3,2)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,3,2)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,3,2)[SMLXMatrix::poolSize - 1];
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
SMLXTmpMatrix& __fastcall nrxc(Add,3,2)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,3,2)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+0], xmm0

		movhps xmm2, qword ptr [ebx+16]
		movhps xmm3, qword ptr [ecx+16]
		addps  xmm2, xmm3
		movhps qword ptr [eax+16], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,3,2)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,3,2)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+0], xmm0

		movhps xmm2, qword ptr [ebx+16]
		movhps xmm3, qword ptr [ecx+16]
		subps  xmm2, xmm3
		movhps qword ptr [eax+16], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,3,2)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movhps xmm2, qword ptr [ecx+16]
		movhps xmm3, qword ptr [edx+16]
		addps  xmm2, xmm3
		movhps qword ptr [ecx+16], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,3,2)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movhps xmm2, qword ptr [ecx+16]
		movhps xmm3, qword ptr [edx+16]
		subps  xmm2, xmm3
		movhps qword ptr [ecx+16], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,3,2)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm2, xmmword ptr [edx+0]
		movaps xmmword ptr [ecx+0], xmm2
		movhps xmm3, qword ptr [edx+16]
		movhps qword ptr [ecx+16], xmm3

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,3,2)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movhps	qword ptr [eax+16], xmm0

		ret 4
	}
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

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,3,3)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,3,3)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		fld 	DWORD PTR [ebx+32]
		fadd 	DWORD PTR [ecx+32]
		fstp	DWORD PTR [eax+32]

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,3,3)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,3,3)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		fld 	DWORD PTR [ebx+32]
		fsub 	DWORD PTR [ecx+32]
		fstp	DWORD PTR [eax+32]

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,3,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		fld 	DWORD PTR [ecx+32]
		fadd 	DWORD PTR [edx+32]
		fstp	DWORD PTR [ecx+32]

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,3,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		fld 	DWORD PTR [ecx+32]
		fsub 	DWORD PTR [edx+32]
		fstp	DWORD PTR [ecx+32]

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,3,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		fld  dword ptr [edx+32]
		fstp dword ptr [ecx+32]

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,3,3)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movss	dword ptr [eax+32], xmm0

		ret 4
	}
}

SMLXTmpMatrix nrxc(pool,3,4)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 3, 4) = SMLXTmpMatrix::InitTmpPool(3, 4, nrxc(pool,3,4), SMLXMatrix::poolSize);

SMLXTmpMatrix& __fastcall nrxc(TMat,3,4)()
{
	enum {pool_sz = sizeof(nrxc(pool,3,4)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,3,4)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,3,4)[SMLXMatrix::poolSize - 1];
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
SMLXTmpMatrix& __fastcall nrxc(Add,3,4)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,3,4)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,3,4)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,3,4)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,3,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,3,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,3,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movaps xmm4, xmmword ptr [edx+32]
		movaps xmmword ptr [ecx+32], xmm4

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,3,4)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movaps	xmmword ptr [eax+32], xmm0

		ret 4
	}
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

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,3,5)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,3,5)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4

		movhps xmm6, qword ptr [ebx+48]
		movhps xmm7, qword ptr [ecx+48]
		addps  xmm6, xmm7
		movhps qword ptr [eax+48], xmm6

		fld 	DWORD PTR [ebx+56]
		fadd 	DWORD PTR [ecx+56]
		fstp	DWORD PTR [eax+56]

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,3,5)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,3,5)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4

		movhps xmm6, qword ptr [ebx+48]
		movhps xmm7, qword ptr [ecx+48]
		subps  xmm6, xmm7
		movhps qword ptr [eax+48], xmm6

		fld 	DWORD PTR [ebx+56]
		fsub 	DWORD PTR [ecx+56]
		fstp	DWORD PTR [eax+56]

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,3,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movhps xmm6, qword ptr [ecx+48]
		movhps xmm7, qword ptr [edx+48]
		addps  xmm6, xmm7
		movhps qword ptr [ecx+48], xmm6

		fld 	DWORD PTR [ecx+56]
		fadd 	DWORD PTR [edx+56]
		fstp	DWORD PTR [ecx+56]

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,3,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movhps xmm6, qword ptr [ecx+48]
		movhps xmm7, qword ptr [edx+48]
		subps  xmm6, xmm7
		movhps qword ptr [ecx+48], xmm6

		fld 	DWORD PTR [ecx+56]
		fsub 	DWORD PTR [edx+56]
		fstp	DWORD PTR [ecx+56]

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,3,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movaps xmm4, xmmword ptr [edx+32]
		movaps xmmword ptr [ecx+32], xmm4
		movhps xmm5, qword ptr [edx+48]
		movhps qword ptr [ecx+48], xmm5
		fld  dword ptr [edx+56]
		fstp dword ptr [ecx+56]

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,3,5)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movaps	xmmword ptr [eax+32], xmm0
		movhps	qword ptr [eax+48], xmm0
		movss	dword ptr [eax+56], xmm0

		ret 4
	}
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

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,3,6)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,3,6)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ebx+48]
		movaps	xmm7,	XMMWORD PTR [ecx+48]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4

		addps   xmm6,	xmm7
		movaps  XMMWORD PTR [eax+48], xmm6

		movhps xmm0, qword ptr [ebx+64]
		movhps xmm1, qword ptr [ecx+64]
		addps  xmm0, xmm1
		movhps qword ptr [eax+64], xmm0

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,3,6)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,3,6)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ebx+48]
		movaps	xmm7,	XMMWORD PTR [ecx+48]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4

		subps   xmm6,	xmm7
		movaps  XMMWORD PTR [eax+48], xmm6

		movhps xmm0, qword ptr [ebx+64]
		movhps xmm1, qword ptr [ecx+64]
		subps  xmm0, xmm1
		movhps qword ptr [eax+64], xmm0

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,3,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		addps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		movhps xmm0, qword ptr [ecx+64]
		movhps xmm1, qword ptr [edx+64]
		addps  xmm0, xmm1
		movhps qword ptr [ecx+64], xmm0

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,3,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		subps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		movhps xmm0, qword ptr [ecx+64]
		movhps xmm1, qword ptr [edx+64]
		subps  xmm0, xmm1
		movhps qword ptr [ecx+64], xmm0

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,3,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movaps xmm2, xmmword ptr [edx+32]
		movaps xmm3, xmmword ptr [edx+48]
		movaps xmmword ptr [ecx+32], xmm2
		movaps xmmword ptr [ecx+48], xmm3
		movhps xmm7, qword ptr [edx+64]
		movhps qword ptr [ecx+64], xmm7

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,3,6)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movaps	xmmword ptr [eax+32], xmm0
		movaps	xmmword ptr [eax+48], xmm0
		movhps	qword ptr [eax+64], xmm0

		ret 4
	}
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

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,4,1)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,4,1)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+0], xmm0

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,4,1)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,4,1)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+0], xmm0

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,4,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,4,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,4,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm2, xmmword ptr [edx+0]
		movaps xmmword ptr [ecx+0], xmm2

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,4,1)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0

		ret 4
	}
}

SMLXTmpMatrix nrxc(pool,4,2)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 4, 2) = SMLXTmpMatrix::InitTmpPool(4, 2, nrxc(pool,4,2), SMLXMatrix::poolSize);

SMLXTmpMatrix& __fastcall nrxc(TMat,4,2)()
{
	enum {pool_sz = sizeof(nrxc(pool,4,2)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,4,2)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,4,2)[SMLXMatrix::poolSize - 1];
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
SMLXTmpMatrix& __fastcall nrxc(Add,4,2)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,4,2)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,4,2)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,4,2)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,4,2)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,4,2)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,4,2)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,4,2)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0

		ret 4
	}
}

SMLXTmpMatrix nrxc(pool,4,3)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 4, 3) = SMLXTmpMatrix::InitTmpPool(4, 3, nrxc(pool,4,3), SMLXMatrix::poolSize);

SMLXTmpMatrix& __fastcall nrxc(TMat,4,3)()
{
	enum {pool_sz = sizeof(nrxc(pool,4,3)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,4,3)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,4,3)[SMLXMatrix::poolSize - 1];
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
SMLXTmpMatrix& __fastcall nrxc(Add,4,3)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,4,3)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,4,3)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,4,3)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,4,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,4,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,4,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movaps xmm4, xmmword ptr [edx+32]
		movaps xmmword ptr [ecx+32], xmm4

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,4,3)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movaps	xmmword ptr [eax+32], xmm0

		ret 4
	}
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

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,4,4)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,4,4)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ebx+48]
		movaps	xmm7,	XMMWORD PTR [ecx+48]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4

		addps   xmm6,	xmm7
		movaps  XMMWORD PTR [eax+48], xmm6

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,4,4)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,4,4)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ebx+48]
		movaps	xmm7,	XMMWORD PTR [ecx+48]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4

		subps   xmm6,	xmm7
		movaps  XMMWORD PTR [eax+48], xmm6

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,4,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		addps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,4,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		subps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,4,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movaps xmm2, xmmword ptr [edx+32]
		movaps xmm3, xmmword ptr [edx+48]
		movaps xmmword ptr [ecx+32], xmm2
		movaps xmmword ptr [ecx+48], xmm3

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,4,4)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movaps	xmmword ptr [eax+32], xmm0
		movaps	xmmword ptr [eax+48], xmm0

		ret 4
	}
}

SMLXTmpMatrix nrxc(pool,4,5)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 4, 5) = SMLXTmpMatrix::InitTmpPool(4, 5, nrxc(pool,4,5), SMLXMatrix::poolSize);

SMLXTmpMatrix& __fastcall nrxc(TMat,4,5)()
{
	enum {pool_sz = sizeof(nrxc(pool,4,5)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,4,5)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,4,5)[SMLXMatrix::poolSize - 1];
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
SMLXTmpMatrix& __fastcall nrxc(Add,4,5)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,4,5)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

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

		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+64], xmm0

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,4,5)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,4,5)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

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

		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+64], xmm0

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,4,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movaps	xmm0,	XMMWORD PTR [ecx+64]
		movaps	xmm1,	XMMWORD PTR [edx+64]
		addps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+64], xmm0

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,4,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movaps	xmm0,	XMMWORD PTR [ecx+64]
		movaps	xmm1,	XMMWORD PTR [edx+64]
		subps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+64], xmm0

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,4,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movaps xmm2, xmmword ptr [edx+32]
		movaps xmm3, xmmword ptr [edx+48]
		movaps xmmword ptr [ecx+32], xmm2
		movaps xmmword ptr [ecx+48], xmm3
		movaps xmm6, xmmword ptr [edx+64]
		movaps xmmword ptr [ecx+64], xmm6

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,4,5)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movaps	xmmword ptr [eax+32], xmm0
		movaps	xmmword ptr [eax+48], xmm0
		movaps	xmmword ptr [eax+64], xmm0

		ret 4
	}
}

SMLXTmpMatrix nrxc(pool,4,6)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 4, 6) = SMLXTmpMatrix::InitTmpPool(4, 6, nrxc(pool,4,6), SMLXMatrix::poolSize);

SMLXTmpMatrix& __fastcall nrxc(TMat,4,6)()
{
	enum {pool_sz = sizeof(nrxc(pool,4,6)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,4,6)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,4,6)[SMLXMatrix::poolSize - 1];
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
SMLXTmpMatrix& __fastcall nrxc(Add,4,6)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,4,6)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

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

		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+80], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,4,6)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,4,6)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

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

		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+80], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,4,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movaps	xmm0,	XMMWORD PTR [ecx+64]
		movaps	xmm1,	XMMWORD PTR [edx+64]
		addps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		movaps	xmm2,	XMMWORD PTR [ecx+80]
		movaps	xmm3,	XMMWORD PTR [edx+80]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+64], xmm0

		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+80], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,4,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movaps	xmm0,	XMMWORD PTR [ecx+64]
		movaps	xmm1,	XMMWORD PTR [edx+64]
		subps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		movaps	xmm2,	XMMWORD PTR [ecx+80]
		movaps	xmm3,	XMMWORD PTR [edx+80]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+64], xmm0

		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+80], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,4,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movaps xmm2, xmmword ptr [edx+32]
		movaps xmm3, xmmword ptr [edx+48]
		movaps xmmword ptr [ecx+32], xmm2
		movaps xmmword ptr [ecx+48], xmm3
		movaps xmm4, xmmword ptr [edx+64]
		movaps xmm5, xmmword ptr [edx+80]
		movaps xmmword ptr [ecx+64], xmm4
		movaps xmmword ptr [ecx+80], xmm5

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,4,6)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movaps	xmmword ptr [eax+32], xmm0
		movaps	xmmword ptr [eax+48], xmm0
		movaps	xmmword ptr [eax+64], xmm0
		movaps	xmmword ptr [eax+80], xmm0

		ret 4
	}
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

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,5,1)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,5,1)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+0], xmm0

		fld 	DWORD PTR [ebx+16]
		fadd 	DWORD PTR [ecx+16]
		fstp	DWORD PTR [eax+16]

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,5,1)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,5,1)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+0], xmm0

		fld 	DWORD PTR [ebx+16]
		fsub 	DWORD PTR [ecx+16]
		fstp	DWORD PTR [eax+16]

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,5,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		fld 	DWORD PTR [ecx+16]
		fadd 	DWORD PTR [edx+16]
		fstp	DWORD PTR [ecx+16]

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,5,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		fld 	DWORD PTR [ecx+16]
		fsub 	DWORD PTR [edx+16]
		fstp	DWORD PTR [ecx+16]

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,5,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm2, xmmword ptr [edx+0]
		movaps xmmword ptr [ecx+0], xmm2
		fld  dword ptr [edx+16]
		fstp dword ptr [ecx+16]

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,5,1)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movss	dword ptr [eax+16], xmm0

		ret 4
	}
}

SMLXTmpMatrix nrxc(pool,5,2)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 5, 2) = SMLXTmpMatrix::InitTmpPool(5, 2, nrxc(pool,5,2), SMLXMatrix::poolSize);

SMLXTmpMatrix& __fastcall nrxc(TMat,5,2)()
{
	enum {pool_sz = sizeof(nrxc(pool,5,2)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,5,2)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,5,2)[SMLXMatrix::poolSize - 1];
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
SMLXTmpMatrix& __fastcall nrxc(Add,5,2)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,5,2)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		movhps xmm4, qword ptr [ebx+32]
		movhps xmm5, qword ptr [ecx+32]
		addps  xmm4, xmm5
		movhps qword ptr [eax+32], xmm4

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,5,2)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,5,2)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		movhps xmm4, qword ptr [ebx+32]
		movhps xmm5, qword ptr [ecx+32]
		subps  xmm4, xmm5
		movhps qword ptr [eax+32], xmm4

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,5,2)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movhps xmm4, qword ptr [ecx+32]
		movhps xmm5, qword ptr [edx+32]
		addps  xmm4, xmm5
		movhps qword ptr [ecx+32], xmm4

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,5,2)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movhps xmm4, qword ptr [ecx+32]
		movhps xmm5, qword ptr [edx+32]
		subps  xmm4, xmm5
		movhps qword ptr [ecx+32], xmm4

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,5,2)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movhps xmm5, qword ptr [edx+32]
		movhps qword ptr [ecx+32], xmm5

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,5,2)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movhps	qword ptr [eax+32], xmm0

		ret 4
	}
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

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,5,3)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,5,3)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4

		movhps xmm6, qword ptr [ebx+48]
		movhps xmm7, qword ptr [ecx+48]
		addps  xmm6, xmm7
		movhps qword ptr [eax+48], xmm6

		fld 	DWORD PTR [ebx+56]
		fadd 	DWORD PTR [ecx+56]
		fstp	DWORD PTR [eax+56]

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,5,3)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,5,3)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4

		movhps xmm6, qword ptr [ebx+48]
		movhps xmm7, qword ptr [ecx+48]
		subps  xmm6, xmm7
		movhps qword ptr [eax+48], xmm6

		fld 	DWORD PTR [ebx+56]
		fsub 	DWORD PTR [ecx+56]
		fstp	DWORD PTR [eax+56]

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,5,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movhps xmm6, qword ptr [ecx+48]
		movhps xmm7, qword ptr [edx+48]
		addps  xmm6, xmm7
		movhps qword ptr [ecx+48], xmm6

		fld 	DWORD PTR [ecx+56]
		fadd 	DWORD PTR [edx+56]
		fstp	DWORD PTR [ecx+56]

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,5,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movhps xmm6, qword ptr [ecx+48]
		movhps xmm7, qword ptr [edx+48]
		subps  xmm6, xmm7
		movhps qword ptr [ecx+48], xmm6

		fld 	DWORD PTR [ecx+56]
		fsub 	DWORD PTR [edx+56]
		fstp	DWORD PTR [ecx+56]

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,5,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movaps xmm4, xmmword ptr [edx+32]
		movaps xmmword ptr [ecx+32], xmm4
		movhps xmm5, qword ptr [edx+48]
		movhps qword ptr [ecx+48], xmm5
		fld  dword ptr [edx+56]
		fstp dword ptr [ecx+56]

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,5,3)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movaps	xmmword ptr [eax+32], xmm0
		movhps	qword ptr [eax+48], xmm0
		movss	dword ptr [eax+56], xmm0

		ret 4
	}
}

SMLXTmpMatrix nrxc(pool,5,4)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 5, 4) = SMLXTmpMatrix::InitTmpPool(5, 4, nrxc(pool,5,4), SMLXMatrix::poolSize);

SMLXTmpMatrix& __fastcall nrxc(TMat,5,4)()
{
	enum {pool_sz = sizeof(nrxc(pool,5,4)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,5,4)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,5,4)[SMLXMatrix::poolSize - 1];
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
SMLXTmpMatrix& __fastcall nrxc(Add,5,4)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,5,4)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

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

		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+64], xmm0

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,5,4)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,5,4)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

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

		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+64], xmm0

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,5,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movaps	xmm0,	XMMWORD PTR [ecx+64]
		movaps	xmm1,	XMMWORD PTR [edx+64]
		addps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+64], xmm0

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,5,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movaps	xmm0,	XMMWORD PTR [ecx+64]
		movaps	xmm1,	XMMWORD PTR [edx+64]
		subps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+64], xmm0

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,5,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movaps xmm2, xmmword ptr [edx+32]
		movaps xmm3, xmmword ptr [edx+48]
		movaps xmmword ptr [ecx+32], xmm2
		movaps xmmword ptr [ecx+48], xmm3
		movaps xmm6, xmmword ptr [edx+64]
		movaps xmmword ptr [ecx+64], xmm6

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,5,4)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movaps	xmmword ptr [eax+32], xmm0
		movaps	xmmword ptr [eax+48], xmm0
		movaps	xmmword ptr [eax+64], xmm0

		ret 4
	}
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

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,5,5)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,5,5)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

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

		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+80], xmm2

		fld 	DWORD PTR [ebx+96]
		fadd 	DWORD PTR [ecx+96]
		fstp	DWORD PTR [eax+96]

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,5,5)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,5,5)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

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

		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+80], xmm2

		fld 	DWORD PTR [ebx+96]
		fsub 	DWORD PTR [ecx+96]
		fstp	DWORD PTR [eax+96]

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,5,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movaps	xmm0,	XMMWORD PTR [ecx+64]
		movaps	xmm1,	XMMWORD PTR [edx+64]
		addps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		movaps	xmm2,	XMMWORD PTR [ecx+80]
		movaps	xmm3,	XMMWORD PTR [edx+80]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+64], xmm0

		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+80], xmm2

		fld 	DWORD PTR [ecx+96]
		fadd 	DWORD PTR [edx+96]
		fstp	DWORD PTR [ecx+96]

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,5,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movaps	xmm0,	XMMWORD PTR [ecx+64]
		movaps	xmm1,	XMMWORD PTR [edx+64]
		subps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		movaps	xmm2,	XMMWORD PTR [ecx+80]
		movaps	xmm3,	XMMWORD PTR [edx+80]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+64], xmm0

		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+80], xmm2

		fld 	DWORD PTR [ecx+96]
		fsub 	DWORD PTR [edx+96]
		fstp	DWORD PTR [ecx+96]

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,5,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movaps xmm2, xmmword ptr [edx+32]
		movaps xmm3, xmmword ptr [edx+48]
		movaps xmmword ptr [ecx+32], xmm2
		movaps xmmword ptr [ecx+48], xmm3
		movaps xmm4, xmmword ptr [edx+64]
		movaps xmm5, xmmword ptr [edx+80]
		movaps xmmword ptr [ecx+64], xmm4
		movaps xmmword ptr [ecx+80], xmm5
		fld  dword ptr [edx+96]
		fstp dword ptr [ecx+96]

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,5,5)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movaps	xmmword ptr [eax+32], xmm0
		movaps	xmmword ptr [eax+48], xmm0
		movaps	xmmword ptr [eax+64], xmm0
		movaps	xmmword ptr [eax+80], xmm0
		movss	dword ptr [eax+96], xmm0

		ret 4
	}
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

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,5,6)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,5,6)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

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

		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+96], xmm4

		movhps xmm6, qword ptr [ebx+112]
		movhps xmm7, qword ptr [ecx+112]
		addps  xmm6, xmm7
		movhps qword ptr [eax+112], xmm6

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,5,6)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,5,6)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

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

		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+96], xmm4

		movhps xmm6, qword ptr [ebx+112]
		movhps xmm7, qword ptr [ecx+112]
		subps  xmm6, xmm7
		movhps qword ptr [eax+112], xmm6

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,5,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movaps	xmm0,	XMMWORD PTR [ecx+64]
		movaps	xmm1,	XMMWORD PTR [edx+64]
		addps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		movaps	xmm2,	XMMWORD PTR [ecx+80]
		movaps	xmm3,	XMMWORD PTR [edx+80]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+64], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+96]
		movaps	xmm5,	XMMWORD PTR [edx+96]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+80], xmm2

		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+96], xmm4

		movhps xmm6, qword ptr [ecx+112]
		movhps xmm7, qword ptr [edx+112]
		addps  xmm6, xmm7
		movhps qword ptr [ecx+112], xmm6

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,5,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movaps	xmm0,	XMMWORD PTR [ecx+64]
		movaps	xmm1,	XMMWORD PTR [edx+64]
		subps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		movaps	xmm2,	XMMWORD PTR [ecx+80]
		movaps	xmm3,	XMMWORD PTR [edx+80]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+64], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+96]
		movaps	xmm5,	XMMWORD PTR [edx+96]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+80], xmm2

		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+96], xmm4

		movhps xmm6, qword ptr [ecx+112]
		movhps xmm7, qword ptr [edx+112]
		subps  xmm6, xmm7
		movhps qword ptr [ecx+112], xmm6

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,5,6)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movaps xmm2, xmmword ptr [edx+32]
		movaps xmm3, xmmword ptr [edx+48]
		movaps xmmword ptr [ecx+32], xmm2
		movaps xmmword ptr [ecx+48], xmm3
		movaps xmm4, xmmword ptr [edx+64]
		movaps xmm5, xmmword ptr [edx+80]
		movaps xmmword ptr [ecx+64], xmm4
		movaps xmmword ptr [ecx+80], xmm5
		movaps xmm0, xmmword ptr [edx+96]
		movaps xmmword ptr [ecx+96], xmm0
		movhps xmm1, qword ptr [edx+112]
		movhps qword ptr [ecx+112], xmm1

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,5,6)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movaps	xmmword ptr [eax+32], xmm0
		movaps	xmmword ptr [eax+48], xmm0
		movaps	xmmword ptr [eax+64], xmm0
		movaps	xmmword ptr [eax+80], xmm0
		movaps	xmmword ptr [eax+96], xmm0
		movhps	qword ptr [eax+112], xmm0

		ret 4
	}
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

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,6,1)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,6,1)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+0], xmm0

		movhps xmm2, qword ptr [ebx+16]
		movhps xmm3, qword ptr [ecx+16]
		addps  xmm2, xmm3
		movhps qword ptr [eax+16], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,6,1)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,6,1)();
	__asm {
		push ebx
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [edx]
		mov edx, eax
		mov eax, [edx]

		movaps	xmm0,	XMMWORD PTR [ebx]
		movaps	xmm1,	XMMWORD PTR [ecx]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [eax+0], xmm0

		movhps xmm2, qword ptr [ebx+16]
		movhps xmm3, qword ptr [ecx+16]
		subps  xmm2, xmm3
		movhps qword ptr [eax+16], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,6,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movhps xmm2, qword ptr [ecx+16]
		movhps xmm3, qword ptr [edx+16]
		addps  xmm2, xmm3
		movhps qword ptr [ecx+16], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,6,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movhps xmm2, qword ptr [ecx+16]
		movhps xmm3, qword ptr [edx+16]
		subps  xmm2, xmm3
		movhps qword ptr [ecx+16], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,6,1)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm2, xmmword ptr [edx+0]
		movaps xmmword ptr [ecx+0], xmm2
		movhps xmm3, qword ptr [edx+16]
		movhps qword ptr [ecx+16], xmm3

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,6,1)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movhps	qword ptr [eax+16], xmm0

		ret 4
	}
}

SMLXTmpMatrix nrxc(pool,6,2)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 6, 2) = SMLXTmpMatrix::InitTmpPool(6, 2, nrxc(pool,6,2), SMLXMatrix::poolSize);

SMLXTmpMatrix& __fastcall nrxc(TMat,6,2)()
{
	enum {pool_sz = sizeof(nrxc(pool,6,2)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,6,2)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,6,2)[SMLXMatrix::poolSize - 1];
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
SMLXTmpMatrix& __fastcall nrxc(Add,6,2)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,6,2)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,6,2)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,6,2)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,6,2)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,6,2)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,6,2)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movaps xmm4, xmmword ptr [edx+32]
		movaps xmmword ptr [ecx+32], xmm4

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,6,2)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movaps	xmmword ptr [eax+32], xmm0

		ret 4
	}
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

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,6,3)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,6,3)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ebx+48]
		movaps	xmm7,	XMMWORD PTR [ecx+48]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4

		addps   xmm6,	xmm7
		movaps  XMMWORD PTR [eax+48], xmm6

		movhps xmm0, qword ptr [ebx+64]
		movhps xmm1, qword ptr [ecx+64]
		addps  xmm0, xmm1
		movhps qword ptr [eax+64], xmm0

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,6,3)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,6,3)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ebx+32]
		movaps	xmm5,	XMMWORD PTR [ecx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ebx+48]
		movaps	xmm7,	XMMWORD PTR [ecx+48]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+32], xmm4

		subps   xmm6,	xmm7
		movaps  XMMWORD PTR [eax+48], xmm6

		movhps xmm0, qword ptr [ebx+64]
		movhps xmm1, qword ptr [ecx+64]
		subps  xmm0, xmm1
		movhps qword ptr [eax+64], xmm0

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,6,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		addps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		movhps xmm0, qword ptr [ecx+64]
		movhps xmm1, qword ptr [edx+64]
		addps  xmm0, xmm1
		movhps qword ptr [ecx+64], xmm0

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,6,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		subps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		movhps xmm0, qword ptr [ecx+64]
		movhps xmm1, qword ptr [edx+64]
		subps  xmm0, xmm1
		movhps qword ptr [ecx+64], xmm0

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,6,3)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movaps xmm2, xmmword ptr [edx+32]
		movaps xmm3, xmmword ptr [edx+48]
		movaps xmmword ptr [ecx+32], xmm2
		movaps xmmword ptr [ecx+48], xmm3
		movhps xmm7, qword ptr [edx+64]
		movhps qword ptr [ecx+64], xmm7

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,6,3)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movaps	xmmword ptr [eax+32], xmm0
		movaps	xmmword ptr [eax+48], xmm0
		movhps	qword ptr [eax+64], xmm0

		ret 4
	}
}

SMLXTmpMatrix nrxc(pool,6,4)[SMLXMatrix::poolSize];
int nrxc(tmpPoolInitialized, 6, 4) = SMLXTmpMatrix::InitTmpPool(6, 4, nrxc(pool,6,4), SMLXMatrix::poolSize);

SMLXTmpMatrix& __fastcall nrxc(TMat,6,4)()
{
	enum {pool_sz = sizeof(nrxc(pool,6,4)), class_sz = sizeof(SMLXTmpMatrix)};
	static char* poolPointer = (char*)&nrxc(pool,6,4)[-1];
	static char* poolEnd     = (char*)&nrxc(pool,6,4)[SMLXMatrix::poolSize - 1];
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
SMLXTmpMatrix& __fastcall nrxc(Add,6,4)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,6,4)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

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

		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+80], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,6,4)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,6,4)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

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

		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [eax+80], xmm2

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,6,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movaps	xmm0,	XMMWORD PTR [ecx+64]
		movaps	xmm1,	XMMWORD PTR [edx+64]
		addps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		movaps	xmm2,	XMMWORD PTR [ecx+80]
		movaps	xmm3,	XMMWORD PTR [edx+80]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+64], xmm0

		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+80], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,6,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movaps	xmm0,	XMMWORD PTR [ecx+64]
		movaps	xmm1,	XMMWORD PTR [edx+64]
		subps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		movaps	xmm2,	XMMWORD PTR [ecx+80]
		movaps	xmm3,	XMMWORD PTR [edx+80]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+64], xmm0

		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+80], xmm2

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,6,4)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movaps xmm2, xmmword ptr [edx+32]
		movaps xmm3, xmmword ptr [edx+48]
		movaps xmmword ptr [ecx+32], xmm2
		movaps xmmword ptr [ecx+48], xmm3
		movaps xmm4, xmmword ptr [edx+64]
		movaps xmm5, xmmword ptr [edx+80]
		movaps xmmword ptr [ecx+64], xmm4
		movaps xmmword ptr [ecx+80], xmm5

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,6,4)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movaps	xmmword ptr [eax+32], xmm0
		movaps	xmmword ptr [eax+48], xmm0
		movaps	xmmword ptr [eax+64], xmm0
		movaps	xmmword ptr [eax+80], xmm0

		ret 4
	}
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

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Add,6,5)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,6,5)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

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

		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+96], xmm4

		movhps xmm6, qword ptr [ebx+112]
		movhps xmm7, qword ptr [ecx+112]
		addps  xmm6, xmm7
		movhps qword ptr [eax+112], xmm6

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,6,5)(const SMLXMatrix &A, const SMLXMatrix &B)
{
	nrxc(TMat,6,5)();
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
		movaps  XMMWORD PTR [eax+0], xmm0

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

		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [eax+96], xmm4

		movhps xmm6, qword ptr [ebx+112]
		movhps xmm7, qword ptr [ecx+112]
		subps  xmm6, xmm7
		movhps qword ptr [eax+112], xmm6

		mov eax, edx
		pop ebx
		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AddTo,6,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movaps	xmm0,	XMMWORD PTR [ecx+64]
		movaps	xmm1,	XMMWORD PTR [edx+64]
		addps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		movaps	xmm2,	XMMWORD PTR [ecx+80]
		movaps	xmm3,	XMMWORD PTR [edx+80]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+64], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+96]
		movaps	xmm5,	XMMWORD PTR [edx+96]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+80], xmm2

		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+96], xmm4

		movhps xmm6, qword ptr [ecx+112]
		movhps xmm7, qword ptr [edx+112]
		addps  xmm6, xmm7
		movhps qword ptr [ecx+112], xmm6

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(SubFrom,6,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movaps	xmm0,	XMMWORD PTR [ecx+64]
		movaps	xmm1,	XMMWORD PTR [edx+64]
		subps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		movaps	xmm2,	XMMWORD PTR [ecx+80]
		movaps	xmm3,	XMMWORD PTR [edx+80]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+64], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+96]
		movaps	xmm5,	XMMWORD PTR [edx+96]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+80], xmm2

		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+96], xmm4

		movhps xmm6, qword ptr [ecx+112]
		movhps xmm7, qword ptr [edx+112]
		subps  xmm6, xmm7
		movhps qword ptr [ecx+112], xmm6

		ret
	}
	CLEAR_SUBEXPRESSION_COUNT
}

__declspec(naked)
void __fastcall nrxc(Assign,6,5)(SMLXMatrix &A, const SMLXMatrix &B) 
{
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movaps xmm2, xmmword ptr [edx+32]
		movaps xmm3, xmmword ptr [edx+48]
		movaps xmmword ptr [ecx+32], xmm2
		movaps xmmword ptr [ecx+48], xmm3
		movaps xmm4, xmmword ptr [edx+64]
		movaps xmm5, xmmword ptr [edx+80]
		movaps xmmword ptr [ecx+64], xmm4
		movaps xmmword ptr [ecx+80], xmm5
		movaps xmm0, xmmword ptr [edx+96]
		movaps xmmword ptr [ecx+96], xmm0
		movhps xmm1, qword ptr [edx+112]
		movhps qword ptr [ecx+112], xmm1

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,6,5)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movaps	xmmword ptr [eax+32], xmm0
		movaps	xmmword ptr [eax+48], xmm0
		movaps	xmmword ptr [eax+64], xmm0
		movaps	xmmword ptr [eax+80], xmm0
		movaps	xmmword ptr [eax+96], xmm0
		movhps	qword ptr [eax+112], xmm0

		ret 4
	}
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
		movaps  XMMWORD PTR [eax+0], xmm0

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
		movaps  XMMWORD PTR [eax+0], xmm0

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
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movaps	xmm0,	XMMWORD PTR [ecx+64]
		movaps	xmm1,	XMMWORD PTR [edx+64]
		addps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		movaps	xmm2,	XMMWORD PTR [ecx+80]
		movaps	xmm3,	XMMWORD PTR [edx+80]
		addps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+64], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+96]
		movaps	xmm5,	XMMWORD PTR [edx+96]
		addps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+80], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+112]
		movaps	xmm7,	XMMWORD PTR [edx+112]
		addps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+96], xmm4

		movaps	xmm0,	XMMWORD PTR [ecx+128]
		movaps	xmm1,	XMMWORD PTR [edx+128]
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
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps	xmm0,	XMMWORD PTR [ecx]
		movaps	xmm1,	XMMWORD PTR [edx]
		movaps	xmm2,	XMMWORD PTR [ecx+16]
		movaps	xmm3,	XMMWORD PTR [edx+16]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+0], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+32]
		movaps	xmm5,	XMMWORD PTR [edx+32]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+16], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+48]
		movaps	xmm7,	XMMWORD PTR [edx+48]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+32], xmm4

		movaps	xmm0,	XMMWORD PTR [ecx+64]
		movaps	xmm1,	XMMWORD PTR [edx+64]
		subps   xmm6,	xmm7
		movaps  XMMWORD PTR [ecx+48], xmm6

		movaps	xmm2,	XMMWORD PTR [ecx+80]
		movaps	xmm3,	XMMWORD PTR [edx+80]
		subps   xmm0,	xmm1
		movaps  XMMWORD PTR [ecx+64], xmm0

		movaps	xmm4,	XMMWORD PTR [ecx+96]
		movaps	xmm5,	XMMWORD PTR [edx+96]
		subps   xmm2,	xmm3
		movaps  XMMWORD PTR [ecx+80], xmm2

		movaps	xmm6,	XMMWORD PTR [ecx+112]
		movaps	xmm7,	XMMWORD PTR [edx+112]
		subps   xmm4,	xmm5
		movaps  XMMWORD PTR [ecx+96], xmm4

		movaps	xmm0,	XMMWORD PTR [ecx+128]
		movaps	xmm1,	XMMWORD PTR [edx+128]
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
	__asm {
		mov eax, dword ptr [edx]
		mov edx, eax
		mov eax, dword ptr [ecx]
		mov ecx, eax

		movaps xmm0, xmmword ptr [edx+0]
		movaps xmm1, xmmword ptr [edx+16]
		movaps xmmword ptr [ecx+0], xmm0
		movaps xmmword ptr [ecx+16], xmm1
		movaps xmm2, xmmword ptr [edx+32]
		movaps xmm3, xmmword ptr [edx+48]
		movaps xmmword ptr [ecx+32], xmm2
		movaps xmmword ptr [ecx+48], xmm3
		movaps xmm4, xmmword ptr [edx+64]
		movaps xmm5, xmmword ptr [edx+80]
		movaps xmmword ptr [ecx+64], xmm4
		movaps xmmword ptr [ecx+80], xmm5
		movaps xmm6, xmmword ptr [edx+96]
		movaps xmm7, xmmword ptr [edx+112]
		movaps xmmword ptr [ecx+96], xmm6
		movaps xmmword ptr [ecx+112], xmm7
		movaps xmm2, xmmword ptr [edx+128]
		movaps xmmword ptr [ecx+128], xmm2

		ret
	}
}

__declspec(naked)
void __fastcall nrxc(AssignValue,6,6)(SMLXMatrix &A, float initialValue) 
{
	__asm {
		mov	eax, dword ptr [ecx]
		movss	xmm0, dword ptr [esp+4]
		shufps	xmm0, xmm0, 0

		movaps	xmmword ptr [eax+0], xmm0
		movaps	xmmword ptr [eax+16], xmm0
		movaps	xmmword ptr [eax+32], xmm0
		movaps	xmmword ptr [eax+48], xmm0
		movaps	xmmword ptr [eax+64], xmm0
		movaps	xmmword ptr [eax+80], xmm0
		movaps	xmmword ptr [eax+96], xmm0
		movaps	xmmword ptr [eax+112], xmm0
		movaps	xmmword ptr [eax+128], xmm0

		ret 4
	}
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
