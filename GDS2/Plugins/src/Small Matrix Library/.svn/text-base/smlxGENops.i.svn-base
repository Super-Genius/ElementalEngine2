


















































































































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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 1 + 0)]












































































































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,1,1)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 1 + 0)]












































































































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,1,1)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]















































































































	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,1,1)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]















































































































	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
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

























































	
	

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,1,1,1,6)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 6, 1, 6, 0, 0)


e1(0, 1, 6, 1, 6, 0, 0)


e1(0, 2, 6, 1, 6, 0, 0)


e1(0, 3, 6, 1, 6, 0, 0)


e1(0, 4, 6, 1, 6, 0, 0)


e1(0, 5, 6, 1, 6, 0, 0)





























































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,1,1,6,1)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 6, 1, 1, 0, 1)


e1(0, 1, 6, 1, 1, 0, 1)


e1(0, 2, 6, 1, 1, 0, 1)


e1(0, 3, 6, 1, 1, 0, 1)


e1(0, 4, 6, 1, 1, 0, 1)


e1(0, 5, 6, 1, 1, 0, 1)





























































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,1,1,1,6)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 6, 1, 6, 1, 0)


e1(0, 1, 6, 1, 6, 1, 0)


e1(0, 2, 6, 1, 6, 1, 0)


e1(0, 3, 6, 1, 6, 1, 0)


e1(0, 4, 6, 1, 6, 1, 0)


e1(0, 5, 6, 1, 6, 1, 0)





























































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,1,1,6,1)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 6, 1, 1, 1, 1)


e1(0, 1, 6, 1, 1, 1, 1)


e1(0, 2, 6, 1, 1, 1, 1)


e1(0, 3, 6, 1, 1, 1, 1)


e1(0, 4, 6, 1, 1, 1, 1)


e1(0, 5, 6, 1, 1, 1, 1)





























































































	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,1,1,1,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 1, 1, 0, 0)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,1,1,1,1)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 1, 1, 0, 1)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,1,1,1,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 1, 1, 1, 0)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,1,1,1,1)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 1, 1, 1, 1)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 3 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 3 + 1)] __asm fadd dword ptr [ecx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 3 + 2)] __asm fadd dword ptr [ecx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (1 * 3 + 0)] __asm fadd dword ptr [ecx + 4 * (1 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 3 + 1)] __asm fadd dword ptr [ecx + 4 * (1 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 3 + 2)] __asm fadd dword ptr [ecx + 4 * (1 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (2 * 3 + 0)] __asm fadd dword ptr [ecx + 4 * (2 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 3 + 1)] __asm fadd dword ptr [ecx + 4 * (2 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 3 + 2)] __asm fadd dword ptr [ecx + 4 * (2 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 3 + 2)]


































































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,3,3)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,3,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 3 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 3 + 1)] __asm fsub dword ptr [ecx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 3 + 2)] __asm fsub dword ptr [ecx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (1 * 3 + 0)] __asm fsub dword ptr [ecx + 4 * (1 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 3 + 1)] __asm fsub dword ptr [ecx + 4 * (1 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 3 + 2)] __asm fsub dword ptr [ecx + 4 * (1 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (2 * 3 + 0)] __asm fsub dword ptr [ecx + 4 * (2 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 3 + 1)] __asm fsub dword ptr [ecx + 4 * (2 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 3 + 2)] __asm fsub dword ptr [ecx + 4 * (2 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 3 + 2)]


































































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,3,3)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 3 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 3 + 1)] __asm fadd dword ptr [edx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 3 + 2)] __asm fadd dword ptr [edx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (1 * 3 + 0)] __asm fadd dword ptr [edx + 4 * (1 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 3 + 1)] __asm fadd dword ptr [edx + 4 * (1 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 3 + 2)] __asm fadd dword ptr [edx + 4 * (1 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (2 * 3 + 0)] __asm fadd dword ptr [edx + 4 * (2 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 3 + 1)] __asm fadd dword ptr [edx + 4 * (2 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 3 + 2)] __asm fadd dword ptr [edx + 4 * (2 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 2)]





































































	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,3,3)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 3 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 3 + 1)] __asm fsub dword ptr [edx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 3 + 2)] __asm fsub dword ptr [edx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (1 * 3 + 0)] __asm fsub dword ptr [edx + 4 * (1 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 3 + 1)] __asm fsub dword ptr [edx + 4 * (1 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 3 + 2)] __asm fsub dword ptr [edx + 4 * (1 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (2 * 3 + 0)] __asm fsub dword ptr [edx + 4 * (2 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 3 + 1)] __asm fsub dword ptr [edx + 4 * (2 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 3 + 2)] __asm fsub dword ptr [edx + 4 * (2 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 2)]





































































	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(Assign,3,3)(SMLXMatrix &A, const SMLXMatrix &B) 







{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax


























__asm fld dword ptr [edx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 0)]


__asm fld dword ptr [edx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 1)]


__asm fld dword ptr [edx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 2)]











__asm fld dword ptr [edx + 4 * (1 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 0)]


__asm fld dword ptr [edx + 4 * (1 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 1)]


__asm fld dword ptr [edx + 4 * (1 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 2)]











__asm fld dword ptr [edx + 4 * (2 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 0)]


__asm fld dword ptr [edx + 4 * (2 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 1)]


__asm fld dword ptr [edx + 4 * (2 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 2)]


































































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

























































	
	

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,3,3,3,6)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 6, 3, 6, 0, 0)


e3(0, 1, 6, 3, 6, 0, 0)


e3(0, 2, 6, 3, 6, 0, 0)


e3(0, 3, 6, 3, 6, 0, 0)


e3(0, 4, 6, 3, 6, 0, 0)


e3(0, 5, 6, 3, 6, 0, 0)


e3(1, 0, 6, 3, 6, 0, 0)


e3(1, 1, 6, 3, 6, 0, 0)


e3(1, 2, 6, 3, 6, 0, 0)


e3(1, 3, 6, 3, 6, 0, 0)


e3(1, 4, 6, 3, 6, 0, 0)


e3(1, 5, 6, 3, 6, 0, 0)


e3(2, 0, 6, 3, 6, 0, 0)


e3(2, 1, 6, 3, 6, 0, 0)


e3(2, 2, 6, 3, 6, 0, 0)


e3(2, 3, 6, 3, 6, 0, 0)


e3(2, 4, 6, 3, 6, 0, 0)


e3(2, 5, 6, 3, 6, 0, 0)

























































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,3,3,6,3)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 6, 3, 3, 0, 1)


e3(0, 1, 6, 3, 3, 0, 1)


e3(0, 2, 6, 3, 3, 0, 1)


e3(0, 3, 6, 3, 3, 0, 1)


e3(0, 4, 6, 3, 3, 0, 1)


e3(0, 5, 6, 3, 3, 0, 1)


e3(1, 0, 6, 3, 3, 0, 1)


e3(1, 1, 6, 3, 3, 0, 1)


e3(1, 2, 6, 3, 3, 0, 1)


e3(1, 3, 6, 3, 3, 0, 1)


e3(1, 4, 6, 3, 3, 0, 1)


e3(1, 5, 6, 3, 3, 0, 1)


e3(2, 0, 6, 3, 3, 0, 1)


e3(2, 1, 6, 3, 3, 0, 1)


e3(2, 2, 6, 3, 3, 0, 1)


e3(2, 3, 6, 3, 3, 0, 1)


e3(2, 4, 6, 3, 3, 0, 1)


e3(2, 5, 6, 3, 3, 0, 1)

























































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,3,3,3,6)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 6, 3, 6, 1, 0)


e3(0, 1, 6, 3, 6, 1, 0)


e3(0, 2, 6, 3, 6, 1, 0)


e3(0, 3, 6, 3, 6, 1, 0)


e3(0, 4, 6, 3, 6, 1, 0)


e3(0, 5, 6, 3, 6, 1, 0)


e3(1, 0, 6, 3, 6, 1, 0)


e3(1, 1, 6, 3, 6, 1, 0)


e3(1, 2, 6, 3, 6, 1, 0)


e3(1, 3, 6, 3, 6, 1, 0)


e3(1, 4, 6, 3, 6, 1, 0)


e3(1, 5, 6, 3, 6, 1, 0)


e3(2, 0, 6, 3, 6, 1, 0)


e3(2, 1, 6, 3, 6, 1, 0)


e3(2, 2, 6, 3, 6, 1, 0)


e3(2, 3, 6, 3, 6, 1, 0)


e3(2, 4, 6, 3, 6, 1, 0)


e3(2, 5, 6, 3, 6, 1, 0)

























































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,3,3,6,3)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 6, 3, 3, 1, 1)


e3(0, 1, 6, 3, 3, 1, 1)


e3(0, 2, 6, 3, 3, 1, 1)


e3(0, 3, 6, 3, 3, 1, 1)


e3(0, 4, 6, 3, 3, 1, 1)


e3(0, 5, 6, 3, 3, 1, 1)


e3(1, 0, 6, 3, 3, 1, 1)


e3(1, 1, 6, 3, 3, 1, 1)


e3(1, 2, 6, 3, 3, 1, 1)


e3(1, 3, 6, 3, 3, 1, 1)


e3(1, 4, 6, 3, 3, 1, 1)


e3(1, 5, 6, 3, 3, 1, 1)


e3(2, 0, 6, 3, 3, 1, 1)


e3(2, 1, 6, 3, 3, 1, 1)


e3(2, 2, 6, 3, 3, 1, 1)


e3(2, 3, 6, 3, 3, 1, 1)


e3(2, 4, 6, 3, 3, 1, 1)


e3(2, 5, 6, 3, 3, 1, 1)

























































	__asm mov eax, edx __asm pop ebx __asm ret
}

















































	
	


































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,3,3,3,3)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 3, 3, 3, 0, 0)


e3(0, 1, 3, 3, 3, 0, 0)


e3(0, 2, 3, 3, 3, 0, 0)











e3(1, 0, 3, 3, 3, 0, 0)


e3(1, 1, 3, 3, 3, 0, 0)


e3(1, 2, 3, 3, 3, 0, 0)











e3(2, 0, 3, 3, 3, 0, 0)


e3(2, 1, 3, 3, 3, 0, 0)


e3(2, 2, 3, 3, 3, 0, 0)


































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,3,3,3,3)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 3, 3, 3, 0, 1)


e3(0, 1, 3, 3, 3, 0, 1)


e3(0, 2, 3, 3, 3, 0, 1)











e3(1, 0, 3, 3, 3, 0, 1)


e3(1, 1, 3, 3, 3, 0, 1)


e3(1, 2, 3, 3, 3, 0, 1)











e3(2, 0, 3, 3, 3, 0, 1)


e3(2, 1, 3, 3, 3, 0, 1)


e3(2, 2, 3, 3, 3, 0, 1)


































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,3,3,3,3)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 3, 3, 3, 1, 0)


e3(0, 1, 3, 3, 3, 1, 0)


e3(0, 2, 3, 3, 3, 1, 0)











e3(1, 0, 3, 3, 3, 1, 0)


e3(1, 1, 3, 3, 3, 1, 0)


e3(1, 2, 3, 3, 3, 1, 0)











e3(2, 0, 3, 3, 3, 1, 0)


e3(2, 1, 3, 3, 3, 1, 0)


e3(2, 2, 3, 3, 3, 1, 0)


































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,3,3,3,3)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 3, 3, 3, 1, 1)


e3(0, 1, 3, 3, 3, 1, 1)


e3(0, 2, 3, 3, 3, 1, 1)











e3(1, 0, 3, 3, 3, 1, 1)


e3(1, 1, 3, 3, 3, 1, 1)


e3(1, 2, 3, 3, 3, 1, 1)











e3(2, 0, 3, 3, 3, 1, 1)


e3(2, 1, 3, 3, 3, 1, 1)


e3(2, 2, 3, 3, 3, 1, 1)


































































	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,3,3,3,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 1, 3, 1, 0, 0)

















e3(1, 0, 1, 3, 1, 0, 0)

















e3(2, 0, 1, 3, 1, 0, 0)








































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,3,3,1,3)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 1, 3, 3, 0, 1)

















e3(1, 0, 1, 3, 3, 0, 1)

















e3(2, 0, 1, 3, 3, 0, 1)








































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,3,3,3,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 1, 3, 1, 1, 0)

















e3(1, 0, 1, 3, 1, 1, 0)

















e3(2, 0, 1, 3, 1, 1, 0)








































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,3,3,1,3)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 1, 3, 3, 1, 1)

















e3(1, 0, 1, 3, 3, 1, 1)

















e3(2, 0, 1, 3, 3, 1, 1)








































































	__asm mov eax, edx __asm pop ebx __asm ret
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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (1 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (2 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 1 + 0)]








































































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,3,1)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,3,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (1 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (2 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 1 + 0)]








































































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,3,1)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (1 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (2 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 1 + 0)]











































































	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,3,1)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (1 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (2 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 1 + 0)]











































































	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(Assign,3,1)(SMLXMatrix &A, const SMLXMatrix &B) 







{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax


























__asm fld dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [edx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [edx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 1 + 0)]








































































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
















































	
	


































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,3,1,1,3)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 3, 1, 3, 0, 0)


e1(0, 1, 3, 1, 3, 0, 0)


e1(0, 2, 3, 1, 3, 0, 0)











e1(1, 0, 3, 1, 3, 0, 0)


e1(1, 1, 3, 1, 3, 0, 0)


e1(1, 2, 3, 1, 3, 0, 0)











e1(2, 0, 3, 1, 3, 0, 0)


e1(2, 1, 3, 1, 3, 0, 0)


e1(2, 2, 3, 1, 3, 0, 0)


































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,3,1,3,1)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 3, 1, 1, 0, 1)


e1(0, 1, 3, 1, 1, 0, 1)


e1(0, 2, 3, 1, 1, 0, 1)











e1(1, 0, 3, 1, 1, 0, 1)


e1(1, 1, 3, 1, 1, 0, 1)


e1(1, 2, 3, 1, 1, 0, 1)











e1(2, 0, 3, 1, 1, 0, 1)


e1(2, 1, 3, 1, 1, 0, 1)


e1(2, 2, 3, 1, 1, 0, 1)


































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,3,1,3,3)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 3, 1, 3, 1, 0)


e3(0, 1, 3, 1, 3, 1, 0)


e3(0, 2, 3, 1, 3, 1, 0)






































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,3,1,3,3)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 3, 1, 3, 1, 1)


e3(0, 1, 3, 1, 3, 1, 1)


e3(0, 2, 3, 1, 3, 1, 1)






































































































	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,3,1,1,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 1, 1, 0, 0)

















e1(1, 0, 1, 1, 1, 0, 0)

















e1(2, 0, 1, 1, 1, 0, 0)








































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,3,1,1,1)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 1, 1, 0, 1)

















e1(1, 0, 1, 1, 1, 0, 1)

















e1(2, 0, 1, 1, 1, 0, 1)








































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,3,1,3,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 1, 1, 1, 1, 0)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,3,1,1,3)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 1, 1, 3, 1, 1)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 3 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 3 + 1)] __asm fadd dword ptr [ecx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 3 + 2)] __asm fadd dword ptr [ecx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 2)]






































































































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,1,3)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,1,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 3 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 3 + 1)] __asm fsub dword ptr [ecx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 3 + 2)] __asm fsub dword ptr [ecx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 2)]






































































































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,1,3)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 3 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 3 + 1)] __asm fadd dword ptr [edx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 3 + 2)] __asm fadd dword ptr [edx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 2)]









































































































	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,1,3)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 3 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 3 + 1)] __asm fsub dword ptr [edx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 3 + 2)] __asm fsub dword ptr [edx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 2)]









































































































	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(Assign,1,3)(SMLXMatrix &A, const SMLXMatrix &B) 







{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax


























__asm fld dword ptr [edx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 0)]


__asm fld dword ptr [edx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 1)]


__asm fld dword ptr [edx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 2)]






































































































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
















































	
	


































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,1,3,3,3)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 3, 3, 3, 0, 0)


e3(0, 1, 3, 3, 3, 0, 0)


e3(0, 2, 3, 3, 3, 0, 0)






































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,1,3,3,3)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 3, 3, 3, 0, 1)


e3(0, 1, 3, 3, 3, 0, 1)


e3(0, 2, 3, 3, 3, 0, 1)






































































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,1,3,1,3)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 3, 3, 3, 1, 0)


e1(0, 1, 3, 3, 3, 1, 0)


e1(0, 2, 3, 3, 3, 1, 0)











e1(1, 0, 3, 3, 3, 1, 0)


e1(1, 1, 3, 3, 3, 1, 0)


e1(1, 2, 3, 3, 3, 1, 0)











e1(2, 0, 3, 3, 3, 1, 0)


e1(2, 1, 3, 3, 3, 1, 0)


e1(2, 2, 3, 3, 3, 1, 0)


































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,1,3,3,1)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 3, 3, 1, 1, 1)


e1(0, 1, 3, 3, 1, 1, 1)


e1(0, 2, 3, 3, 1, 1, 1)











e1(1, 0, 3, 3, 1, 1, 1)


e1(1, 1, 3, 3, 1, 1, 1)


e1(1, 2, 3, 3, 1, 1, 1)











e1(2, 0, 3, 3, 1, 1, 1)


e1(2, 1, 3, 3, 1, 1, 1)


e1(2, 2, 3, 3, 1, 1, 1)


































































	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,1,3,3,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 1, 3, 1, 0, 0)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,1,3,1,3)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 1, 3, 3, 0, 1)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,1,3,1,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 3, 1, 1, 0)

















e1(1, 0, 1, 3, 1, 1, 0)

















e1(2, 0, 1, 3, 1, 1, 0)








































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,1,3,1,1)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 3, 1, 1, 1)

















e1(1, 0, 1, 3, 1, 1, 1)

















e1(2, 0, 1, 3, 1, 1, 1)








































































	__asm mov eax, edx __asm pop ebx __asm ret
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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 4 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 4 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 4 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 4 + 1)] __asm fadd dword ptr [ecx + 4 * (0 * 4 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 4 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 4 + 2)] __asm fadd dword ptr [ecx + 4 * (0 * 4 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 4 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 4 + 3)] __asm fadd dword ptr [ecx + 4 * (0 * 4 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 4 + 3)]








__asm fld dword ptr [ebx + 4 * (1 * 4 + 0)] __asm fadd dword ptr [ecx + 4 * (1 * 4 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 4 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 4 + 1)] __asm fadd dword ptr [ecx + 4 * (1 * 4 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 4 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 4 + 2)] __asm fadd dword ptr [ecx + 4 * (1 * 4 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 4 + 2)]


__asm fld dword ptr [ebx + 4 * (1 * 4 + 3)] __asm fadd dword ptr [ecx + 4 * (1 * 4 + 3)] __asm fstp dword ptr [eax + 4 * (1 * 4 + 3)]








__asm fld dword ptr [ebx + 4 * (2 * 4 + 0)] __asm fadd dword ptr [ecx + 4 * (2 * 4 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 4 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 4 + 1)] __asm fadd dword ptr [ecx + 4 * (2 * 4 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 4 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 4 + 2)] __asm fadd dword ptr [ecx + 4 * (2 * 4 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 4 + 2)]


__asm fld dword ptr [ebx + 4 * (2 * 4 + 3)] __asm fadd dword ptr [ecx + 4 * (2 * 4 + 3)] __asm fstp dword ptr [eax + 4 * (2 * 4 + 3)]








__asm fld dword ptr [ebx + 4 * (3 * 4 + 0)] __asm fadd dword ptr [ecx + 4 * (3 * 4 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 4 + 0)]


__asm fld dword ptr [ebx + 4 * (3 * 4 + 1)] __asm fadd dword ptr [ecx + 4 * (3 * 4 + 1)] __asm fstp dword ptr [eax + 4 * (3 * 4 + 1)]


__asm fld dword ptr [ebx + 4 * (3 * 4 + 2)] __asm fadd dword ptr [ecx + 4 * (3 * 4 + 2)] __asm fstp dword ptr [eax + 4 * (3 * 4 + 2)]


__asm fld dword ptr [ebx + 4 * (3 * 4 + 3)] __asm fadd dword ptr [ecx + 4 * (3 * 4 + 3)] __asm fstp dword ptr [eax + 4 * (3 * 4 + 3)]













































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,4,4)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,4,4)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 4 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 4 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 4 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 4 + 1)] __asm fsub dword ptr [ecx + 4 * (0 * 4 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 4 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 4 + 2)] __asm fsub dword ptr [ecx + 4 * (0 * 4 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 4 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 4 + 3)] __asm fsub dword ptr [ecx + 4 * (0 * 4 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 4 + 3)]








__asm fld dword ptr [ebx + 4 * (1 * 4 + 0)] __asm fsub dword ptr [ecx + 4 * (1 * 4 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 4 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 4 + 1)] __asm fsub dword ptr [ecx + 4 * (1 * 4 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 4 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 4 + 2)] __asm fsub dword ptr [ecx + 4 * (1 * 4 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 4 + 2)]


__asm fld dword ptr [ebx + 4 * (1 * 4 + 3)] __asm fsub dword ptr [ecx + 4 * (1 * 4 + 3)] __asm fstp dword ptr [eax + 4 * (1 * 4 + 3)]








__asm fld dword ptr [ebx + 4 * (2 * 4 + 0)] __asm fsub dword ptr [ecx + 4 * (2 * 4 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 4 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 4 + 1)] __asm fsub dword ptr [ecx + 4 * (2 * 4 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 4 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 4 + 2)] __asm fsub dword ptr [ecx + 4 * (2 * 4 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 4 + 2)]


__asm fld dword ptr [ebx + 4 * (2 * 4 + 3)] __asm fsub dword ptr [ecx + 4 * (2 * 4 + 3)] __asm fstp dword ptr [eax + 4 * (2 * 4 + 3)]








__asm fld dword ptr [ebx + 4 * (3 * 4 + 0)] __asm fsub dword ptr [ecx + 4 * (3 * 4 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 4 + 0)]


__asm fld dword ptr [ebx + 4 * (3 * 4 + 1)] __asm fsub dword ptr [ecx + 4 * (3 * 4 + 1)] __asm fstp dword ptr [eax + 4 * (3 * 4 + 1)]


__asm fld dword ptr [ebx + 4 * (3 * 4 + 2)] __asm fsub dword ptr [ecx + 4 * (3 * 4 + 2)] __asm fstp dword ptr [eax + 4 * (3 * 4 + 2)]


__asm fld dword ptr [ebx + 4 * (3 * 4 + 3)] __asm fsub dword ptr [ecx + 4 * (3 * 4 + 3)] __asm fstp dword ptr [eax + 4 * (3 * 4 + 3)]













































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,4,4)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 4 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 4 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 4 + 1)] __asm fadd dword ptr [edx + 4 * (0 * 4 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 4 + 2)] __asm fadd dword ptr [edx + 4 * (0 * 4 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 4 + 3)] __asm fadd dword ptr [edx + 4 * (0 * 4 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 3)]








__asm fld dword ptr [ecx + 4 * (1 * 4 + 0)] __asm fadd dword ptr [edx + 4 * (1 * 4 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 4 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 4 + 1)] __asm fadd dword ptr [edx + 4 * (1 * 4 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 4 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 4 + 2)] __asm fadd dword ptr [edx + 4 * (1 * 4 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 4 + 2)]


__asm fld dword ptr [ecx + 4 * (1 * 4 + 3)] __asm fadd dword ptr [edx + 4 * (1 * 4 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 4 + 3)]








__asm fld dword ptr [ecx + 4 * (2 * 4 + 0)] __asm fadd dword ptr [edx + 4 * (2 * 4 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 4 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 4 + 1)] __asm fadd dword ptr [edx + 4 * (2 * 4 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 4 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 4 + 2)] __asm fadd dword ptr [edx + 4 * (2 * 4 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 4 + 2)]


__asm fld dword ptr [ecx + 4 * (2 * 4 + 3)] __asm fadd dword ptr [edx + 4 * (2 * 4 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 4 + 3)]








__asm fld dword ptr [ecx + 4 * (3 * 4 + 0)] __asm fadd dword ptr [edx + 4 * (3 * 4 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 4 + 0)]


__asm fld dword ptr [ecx + 4 * (3 * 4 + 1)] __asm fadd dword ptr [edx + 4 * (3 * 4 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 4 + 1)]


__asm fld dword ptr [ecx + 4 * (3 * 4 + 2)] __asm fadd dword ptr [edx + 4 * (3 * 4 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 4 + 2)]


__asm fld dword ptr [ecx + 4 * (3 * 4 + 3)] __asm fadd dword ptr [edx + 4 * (3 * 4 + 3)] __asm fstp dword ptr [ecx + 4 * (3 * 4 + 3)]
















































	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,4,4)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 4 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 4 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 4 + 1)] __asm fsub dword ptr [edx + 4 * (0 * 4 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 4 + 2)] __asm fsub dword ptr [edx + 4 * (0 * 4 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 4 + 3)] __asm fsub dword ptr [edx + 4 * (0 * 4 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 3)]








__asm fld dword ptr [ecx + 4 * (1 * 4 + 0)] __asm fsub dword ptr [edx + 4 * (1 * 4 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 4 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 4 + 1)] __asm fsub dword ptr [edx + 4 * (1 * 4 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 4 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 4 + 2)] __asm fsub dword ptr [edx + 4 * (1 * 4 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 4 + 2)]


__asm fld dword ptr [ecx + 4 * (1 * 4 + 3)] __asm fsub dword ptr [edx + 4 * (1 * 4 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 4 + 3)]








__asm fld dword ptr [ecx + 4 * (2 * 4 + 0)] __asm fsub dword ptr [edx + 4 * (2 * 4 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 4 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 4 + 1)] __asm fsub dword ptr [edx + 4 * (2 * 4 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 4 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 4 + 2)] __asm fsub dword ptr [edx + 4 * (2 * 4 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 4 + 2)]


__asm fld dword ptr [ecx + 4 * (2 * 4 + 3)] __asm fsub dword ptr [edx + 4 * (2 * 4 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 4 + 3)]








__asm fld dword ptr [ecx + 4 * (3 * 4 + 0)] __asm fsub dword ptr [edx + 4 * (3 * 4 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 4 + 0)]


__asm fld dword ptr [ecx + 4 * (3 * 4 + 1)] __asm fsub dword ptr [edx + 4 * (3 * 4 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 4 + 1)]


__asm fld dword ptr [ecx + 4 * (3 * 4 + 2)] __asm fsub dword ptr [edx + 4 * (3 * 4 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 4 + 2)]


__asm fld dword ptr [ecx + 4 * (3 * 4 + 3)] __asm fsub dword ptr [edx + 4 * (3 * 4 + 3)] __asm fstp dword ptr [ecx + 4 * (3 * 4 + 3)]
















































	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(Assign,4,4)(SMLXMatrix &A, const SMLXMatrix &B) 







{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax


























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



















































	
	































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,4,4,4,4)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,4,4)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e4(0, 0, 4, 4, 4, 0, 0)


e4(0, 1, 4, 4, 4, 0, 0)


e4(0, 2, 4, 4, 4, 0, 0)


e4(0, 3, 4, 4, 4, 0, 0)








e4(1, 0, 4, 4, 4, 0, 0)


e4(1, 1, 4, 4, 4, 0, 0)


e4(1, 2, 4, 4, 4, 0, 0)


e4(1, 3, 4, 4, 4, 0, 0)








e4(2, 0, 4, 4, 4, 0, 0)


e4(2, 1, 4, 4, 4, 0, 0)


e4(2, 2, 4, 4, 4, 0, 0)


e4(2, 3, 4, 4, 4, 0, 0)








e4(3, 0, 4, 4, 4, 0, 0)


e4(3, 1, 4, 4, 4, 0, 0)


e4(3, 2, 4, 4, 4, 0, 0)


e4(3, 3, 4, 4, 4, 0, 0)













































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,4,4,4,4)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,4,4)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e4(0, 0, 4, 4, 4, 0, 1)


e4(0, 1, 4, 4, 4, 0, 1)


e4(0, 2, 4, 4, 4, 0, 1)


e4(0, 3, 4, 4, 4, 0, 1)








e4(1, 0, 4, 4, 4, 0, 1)


e4(1, 1, 4, 4, 4, 0, 1)


e4(1, 2, 4, 4, 4, 0, 1)


e4(1, 3, 4, 4, 4, 0, 1)








e4(2, 0, 4, 4, 4, 0, 1)


e4(2, 1, 4, 4, 4, 0, 1)


e4(2, 2, 4, 4, 4, 0, 1)


e4(2, 3, 4, 4, 4, 0, 1)








e4(3, 0, 4, 4, 4, 0, 1)


e4(3, 1, 4, 4, 4, 0, 1)


e4(3, 2, 4, 4, 4, 0, 1)


e4(3, 3, 4, 4, 4, 0, 1)













































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,4,4,4,4)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,4,4)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e4(0, 0, 4, 4, 4, 1, 0)


e4(0, 1, 4, 4, 4, 1, 0)


e4(0, 2, 4, 4, 4, 1, 0)


e4(0, 3, 4, 4, 4, 1, 0)








e4(1, 0, 4, 4, 4, 1, 0)


e4(1, 1, 4, 4, 4, 1, 0)


e4(1, 2, 4, 4, 4, 1, 0)


e4(1, 3, 4, 4, 4, 1, 0)








e4(2, 0, 4, 4, 4, 1, 0)


e4(2, 1, 4, 4, 4, 1, 0)


e4(2, 2, 4, 4, 4, 1, 0)


e4(2, 3, 4, 4, 4, 1, 0)








e4(3, 0, 4, 4, 4, 1, 0)


e4(3, 1, 4, 4, 4, 1, 0)


e4(3, 2, 4, 4, 4, 1, 0)


e4(3, 3, 4, 4, 4, 1, 0)













































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,4,4,4,4)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,4,4)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e4(0, 0, 4, 4, 4, 1, 1)


e4(0, 1, 4, 4, 4, 1, 1)


e4(0, 2, 4, 4, 4, 1, 1)


e4(0, 3, 4, 4, 4, 1, 1)








e4(1, 0, 4, 4, 4, 1, 1)


e4(1, 1, 4, 4, 4, 1, 1)


e4(1, 2, 4, 4, 4, 1, 1)


e4(1, 3, 4, 4, 4, 1, 1)








e4(2, 0, 4, 4, 4, 1, 1)


e4(2, 1, 4, 4, 4, 1, 1)


e4(2, 2, 4, 4, 4, 1, 1)


e4(2, 3, 4, 4, 4, 1, 1)








e4(3, 0, 4, 4, 4, 1, 1)


e4(3, 1, 4, 4, 4, 1, 1)


e4(3, 2, 4, 4, 4, 1, 1)


e4(3, 3, 4, 4, 4, 1, 1)













































	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,4,4,4,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,4,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e4(0, 0, 1, 4, 1, 0, 0)

















e4(1, 0, 1, 4, 1, 0, 0)

















e4(2, 0, 1, 4, 1, 0, 0)

















e4(3, 0, 1, 4, 1, 0, 0)






















































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,4,4,1,4)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,4,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e4(0, 0, 1, 4, 4, 0, 1)

















e4(1, 0, 1, 4, 4, 0, 1)

















e4(2, 0, 1, 4, 4, 0, 1)

















e4(3, 0, 1, 4, 4, 0, 1)






















































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,4,4,4,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,4,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e4(0, 0, 1, 4, 1, 1, 0)

















e4(1, 0, 1, 4, 1, 1, 0)

















e4(2, 0, 1, 4, 1, 1, 0)

















e4(3, 0, 1, 4, 1, 1, 0)






















































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,4,4,1,4)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,4,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e4(0, 0, 1, 4, 4, 1, 1)

















e4(1, 0, 1, 4, 4, 1, 1)

















e4(2, 0, 1, 4, 4, 1, 1)

















e4(3, 0, 1, 4, 4, 1, 1)






















































	__asm mov eax, edx __asm pop ebx __asm ret
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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (1 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (2 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (3 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (3 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 1 + 0)]






















































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,4,1)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,4,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (1 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (2 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (3 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (3 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 1 + 0)]






















































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,4,1)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (1 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (2 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (3 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (3 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 1 + 0)]

























































	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,4,1)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (1 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (2 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (3 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (3 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 1 + 0)]

























































	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(Assign,4,1)(SMLXMatrix &A, const SMLXMatrix &B) 







{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax


























__asm fld dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [edx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [edx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 1 + 0)]

















__asm fld dword ptr [edx + 4 * (3 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 1 + 0)]






















































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



















































	
	































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,4,1,1,4)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,4,4)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 4, 1, 4, 0, 0)


e1(0, 1, 4, 1, 4, 0, 0)


e1(0, 2, 4, 1, 4, 0, 0)


e1(0, 3, 4, 1, 4, 0, 0)








e1(1, 0, 4, 1, 4, 0, 0)


e1(1, 1, 4, 1, 4, 0, 0)


e1(1, 2, 4, 1, 4, 0, 0)


e1(1, 3, 4, 1, 4, 0, 0)








e1(2, 0, 4, 1, 4, 0, 0)


e1(2, 1, 4, 1, 4, 0, 0)


e1(2, 2, 4, 1, 4, 0, 0)


e1(2, 3, 4, 1, 4, 0, 0)








e1(3, 0, 4, 1, 4, 0, 0)


e1(3, 1, 4, 1, 4, 0, 0)


e1(3, 2, 4, 1, 4, 0, 0)


e1(3, 3, 4, 1, 4, 0, 0)













































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,4,1,4,1)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,4,4)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 4, 1, 1, 0, 1)


e1(0, 1, 4, 1, 1, 0, 1)


e1(0, 2, 4, 1, 1, 0, 1)


e1(0, 3, 4, 1, 1, 0, 1)








e1(1, 0, 4, 1, 1, 0, 1)


e1(1, 1, 4, 1, 1, 0, 1)


e1(1, 2, 4, 1, 1, 0, 1)


e1(1, 3, 4, 1, 1, 0, 1)








e1(2, 0, 4, 1, 1, 0, 1)


e1(2, 1, 4, 1, 1, 0, 1)


e1(2, 2, 4, 1, 1, 0, 1)


e1(2, 3, 4, 1, 1, 0, 1)








e1(3, 0, 4, 1, 1, 0, 1)


e1(3, 1, 4, 1, 1, 0, 1)


e1(3, 2, 4, 1, 1, 0, 1)


e1(3, 3, 4, 1, 1, 0, 1)













































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,4,1,4,4)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,4)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e4(0, 0, 4, 1, 4, 1, 0)


e4(0, 1, 4, 1, 4, 1, 0)


e4(0, 2, 4, 1, 4, 1, 0)


e4(0, 3, 4, 1, 4, 1, 0)



































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,4,1,4,4)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,4)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e4(0, 0, 4, 1, 4, 1, 1)


e4(0, 1, 4, 1, 4, 1, 1)


e4(0, 2, 4, 1, 4, 1, 1)


e4(0, 3, 4, 1, 4, 1, 1)



































































































	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,4,1,1,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,4,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 1, 1, 0, 0)

















e1(1, 0, 1, 1, 1, 0, 0)

















e1(2, 0, 1, 1, 1, 0, 0)

















e1(3, 0, 1, 1, 1, 0, 0)






















































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,4,1,1,1)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,4,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 1, 1, 0, 1)

















e1(1, 0, 1, 1, 1, 0, 1)

















e1(2, 0, 1, 1, 1, 0, 1)

















e1(3, 0, 1, 1, 1, 0, 1)






















































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,4,1,4,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e4(0, 0, 1, 1, 1, 1, 0)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,4,1,1,4)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e4(0, 0, 1, 1, 4, 1, 1)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 4 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 4 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 4 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 4 + 1)] __asm fadd dword ptr [ecx + 4 * (0 * 4 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 4 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 4 + 2)] __asm fadd dword ptr [ecx + 4 * (0 * 4 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 4 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 4 + 3)] __asm fadd dword ptr [ecx + 4 * (0 * 4 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 4 + 3)]



































































































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,1,4)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,1,4)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 4 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 4 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 4 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 4 + 1)] __asm fsub dword ptr [ecx + 4 * (0 * 4 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 4 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 4 + 2)] __asm fsub dword ptr [ecx + 4 * (0 * 4 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 4 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 4 + 3)] __asm fsub dword ptr [ecx + 4 * (0 * 4 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 4 + 3)]



































































































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,1,4)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 4 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 4 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 4 + 1)] __asm fadd dword ptr [edx + 4 * (0 * 4 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 4 + 2)] __asm fadd dword ptr [edx + 4 * (0 * 4 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 4 + 3)] __asm fadd dword ptr [edx + 4 * (0 * 4 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 3)]






































































































	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,1,4)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 4 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 4 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 4 + 1)] __asm fsub dword ptr [edx + 4 * (0 * 4 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 4 + 2)] __asm fsub dword ptr [edx + 4 * (0 * 4 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 4 + 3)] __asm fsub dword ptr [edx + 4 * (0 * 4 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 3)]






































































































	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(Assign,1,4)(SMLXMatrix &A, const SMLXMatrix &B) 







{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax


























__asm fld dword ptr [edx + 4 * (0 * 4 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 0)]


__asm fld dword ptr [edx + 4 * (0 * 4 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 1)]


__asm fld dword ptr [edx + 4 * (0 * 4 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 2)]


__asm fld dword ptr [edx + 4 * (0 * 4 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 4 + 3)]



































































































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



















































	
	































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,1,4,4,4)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,4)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e4(0, 0, 4, 4, 4, 0, 0)


e4(0, 1, 4, 4, 4, 0, 0)


e4(0, 2, 4, 4, 4, 0, 0)


e4(0, 3, 4, 4, 4, 0, 0)



































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,1,4,4,4)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,4)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e4(0, 0, 4, 4, 4, 0, 1)


e4(0, 1, 4, 4, 4, 0, 1)


e4(0, 2, 4, 4, 4, 0, 1)


e4(0, 3, 4, 4, 4, 0, 1)



































































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,1,4,1,4)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,4,4)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 4, 4, 4, 1, 0)


e1(0, 1, 4, 4, 4, 1, 0)


e1(0, 2, 4, 4, 4, 1, 0)


e1(0, 3, 4, 4, 4, 1, 0)








e1(1, 0, 4, 4, 4, 1, 0)


e1(1, 1, 4, 4, 4, 1, 0)


e1(1, 2, 4, 4, 4, 1, 0)


e1(1, 3, 4, 4, 4, 1, 0)








e1(2, 0, 4, 4, 4, 1, 0)


e1(2, 1, 4, 4, 4, 1, 0)


e1(2, 2, 4, 4, 4, 1, 0)


e1(2, 3, 4, 4, 4, 1, 0)








e1(3, 0, 4, 4, 4, 1, 0)


e1(3, 1, 4, 4, 4, 1, 0)


e1(3, 2, 4, 4, 4, 1, 0)


e1(3, 3, 4, 4, 4, 1, 0)













































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,1,4,4,1)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,4,4)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 4, 4, 1, 1, 1)


e1(0, 1, 4, 4, 1, 1, 1)


e1(0, 2, 4, 4, 1, 1, 1)


e1(0, 3, 4, 4, 1, 1, 1)








e1(1, 0, 4, 4, 1, 1, 1)


e1(1, 1, 4, 4, 1, 1, 1)


e1(1, 2, 4, 4, 1, 1, 1)


e1(1, 3, 4, 4, 1, 1, 1)








e1(2, 0, 4, 4, 1, 1, 1)


e1(2, 1, 4, 4, 1, 1, 1)


e1(2, 2, 4, 4, 1, 1, 1)


e1(2, 3, 4, 4, 1, 1, 1)








e1(3, 0, 4, 4, 1, 1, 1)


e1(3, 1, 4, 4, 1, 1, 1)


e1(3, 2, 4, 4, 1, 1, 1)


e1(3, 3, 4, 4, 1, 1, 1)













































	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,1,4,4,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e4(0, 0, 1, 4, 1, 0, 0)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,1,4,1,4)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e4(0, 0, 1, 4, 4, 0, 1)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,1,4,1,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,4,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 4, 1, 1, 0)

















e1(1, 0, 1, 4, 1, 1, 0)

















e1(2, 0, 1, 4, 1, 1, 0)

















e1(3, 0, 1, 4, 1, 1, 0)






















































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,1,4,1,1)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,4,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 4, 1, 1, 1)

















e1(1, 0, 1, 4, 1, 1, 1)

















e1(2, 0, 1, 4, 1, 1, 1)

















e1(3, 0, 1, 4, 1, 1, 1)






















































	__asm mov eax, edx __asm pop ebx __asm ret
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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 5 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 1)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 2)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 3)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 4)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (1 * 5 + 0)] __asm fadd dword ptr [ecx + 4 * (1 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 1)] __asm fadd dword ptr [ecx + 4 * (1 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 2)] __asm fadd dword ptr [ecx + 4 * (1 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 3)] __asm fadd dword ptr [ecx + 4 * (1 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 4)] __asm fadd dword ptr [ecx + 4 * (1 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (2 * 5 + 0)] __asm fadd dword ptr [ecx + 4 * (2 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 1)] __asm fadd dword ptr [ecx + 4 * (2 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 2)] __asm fadd dword ptr [ecx + 4 * (2 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 3)] __asm fadd dword ptr [ecx + 4 * (2 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 4)] __asm fadd dword ptr [ecx + 4 * (2 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (3 * 5 + 0)] __asm fadd dword ptr [ecx + 4 * (3 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (3 * 5 + 1)] __asm fadd dword ptr [ecx + 4 * (3 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (3 * 5 + 2)] __asm fadd dword ptr [ecx + 4 * (3 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (3 * 5 + 3)] __asm fadd dword ptr [ecx + 4 * (3 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (3 * 5 + 4)] __asm fadd dword ptr [ecx + 4 * (3 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (4 * 5 + 0)] __asm fadd dword ptr [ecx + 4 * (4 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (4 * 5 + 1)] __asm fadd dword ptr [ecx + 4 * (4 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (4 * 5 + 2)] __asm fadd dword ptr [ecx + 4 * (4 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (4 * 5 + 3)] __asm fadd dword ptr [ecx + 4 * (4 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (4 * 5 + 4)] __asm fadd dword ptr [ecx + 4 * (4 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 4)]
























	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,5,5)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,5,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 5 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 1)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 2)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 3)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 4)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (1 * 5 + 0)] __asm fsub dword ptr [ecx + 4 * (1 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 1)] __asm fsub dword ptr [ecx + 4 * (1 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 2)] __asm fsub dword ptr [ecx + 4 * (1 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 3)] __asm fsub dword ptr [ecx + 4 * (1 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 4)] __asm fsub dword ptr [ecx + 4 * (1 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (2 * 5 + 0)] __asm fsub dword ptr [ecx + 4 * (2 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 1)] __asm fsub dword ptr [ecx + 4 * (2 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 2)] __asm fsub dword ptr [ecx + 4 * (2 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 3)] __asm fsub dword ptr [ecx + 4 * (2 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 4)] __asm fsub dword ptr [ecx + 4 * (2 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (3 * 5 + 0)] __asm fsub dword ptr [ecx + 4 * (3 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (3 * 5 + 1)] __asm fsub dword ptr [ecx + 4 * (3 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (3 * 5 + 2)] __asm fsub dword ptr [ecx + 4 * (3 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (3 * 5 + 3)] __asm fsub dword ptr [ecx + 4 * (3 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (3 * 5 + 4)] __asm fsub dword ptr [ecx + 4 * (3 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (4 * 5 + 0)] __asm fsub dword ptr [ecx + 4 * (4 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (4 * 5 + 1)] __asm fsub dword ptr [ecx + 4 * (4 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (4 * 5 + 2)] __asm fsub dword ptr [ecx + 4 * (4 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (4 * 5 + 3)] __asm fsub dword ptr [ecx + 4 * (4 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (4 * 5 + 4)] __asm fsub dword ptr [ecx + 4 * (4 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 4)]
























	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,5,5)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 5 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 1)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 2)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 3)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 4)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (1 * 5 + 0)] __asm fadd dword ptr [edx + 4 * (1 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 1)] __asm fadd dword ptr [edx + 4 * (1 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 2)] __asm fadd dword ptr [edx + 4 * (1 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 3)] __asm fadd dword ptr [edx + 4 * (1 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 4)] __asm fadd dword ptr [edx + 4 * (1 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (2 * 5 + 0)] __asm fadd dword ptr [edx + 4 * (2 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 1)] __asm fadd dword ptr [edx + 4 * (2 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 2)] __asm fadd dword ptr [edx + 4 * (2 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 3)] __asm fadd dword ptr [edx + 4 * (2 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 4)] __asm fadd dword ptr [edx + 4 * (2 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (3 * 5 + 0)] __asm fadd dword ptr [edx + 4 * (3 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (3 * 5 + 1)] __asm fadd dword ptr [edx + 4 * (3 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (3 * 5 + 2)] __asm fadd dword ptr [edx + 4 * (3 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (3 * 5 + 3)] __asm fadd dword ptr [edx + 4 * (3 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (3 * 5 + 4)] __asm fadd dword ptr [edx + 4 * (3 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (4 * 5 + 0)] __asm fadd dword ptr [edx + 4 * (4 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (4 * 5 + 1)] __asm fadd dword ptr [edx + 4 * (4 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (4 * 5 + 2)] __asm fadd dword ptr [edx + 4 * (4 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (4 * 5 + 3)] __asm fadd dword ptr [edx + 4 * (4 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (4 * 5 + 4)] __asm fadd dword ptr [edx + 4 * (4 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 4)]



























	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,5,5)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 5 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 1)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 2)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 3)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 4)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (1 * 5 + 0)] __asm fsub dword ptr [edx + 4 * (1 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 1)] __asm fsub dword ptr [edx + 4 * (1 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 2)] __asm fsub dword ptr [edx + 4 * (1 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 3)] __asm fsub dword ptr [edx + 4 * (1 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 4)] __asm fsub dword ptr [edx + 4 * (1 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (2 * 5 + 0)] __asm fsub dword ptr [edx + 4 * (2 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 1)] __asm fsub dword ptr [edx + 4 * (2 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 2)] __asm fsub dword ptr [edx + 4 * (2 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 3)] __asm fsub dword ptr [edx + 4 * (2 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 4)] __asm fsub dword ptr [edx + 4 * (2 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (3 * 5 + 0)] __asm fsub dword ptr [edx + 4 * (3 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (3 * 5 + 1)] __asm fsub dword ptr [edx + 4 * (3 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (3 * 5 + 2)] __asm fsub dword ptr [edx + 4 * (3 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (3 * 5 + 3)] __asm fsub dword ptr [edx + 4 * (3 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (3 * 5 + 4)] __asm fsub dword ptr [edx + 4 * (3 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (4 * 5 + 0)] __asm fsub dword ptr [edx + 4 * (4 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (4 * 5 + 1)] __asm fsub dword ptr [edx + 4 * (4 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (4 * 5 + 2)] __asm fsub dword ptr [edx + 4 * (4 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (4 * 5 + 3)] __asm fsub dword ptr [edx + 4 * (4 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (4 * 5 + 4)] __asm fsub dword ptr [edx + 4 * (4 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 4)]



























	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(Assign,5,5)(SMLXMatrix &A, const SMLXMatrix &B) 







{
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

























































	
	

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,5,5,5,6)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 6, 5, 6, 0, 0)


e5(0, 1, 6, 5, 6, 0, 0)


e5(0, 2, 6, 5, 6, 0, 0)


e5(0, 3, 6, 5, 6, 0, 0)


e5(0, 4, 6, 5, 6, 0, 0)


e5(0, 5, 6, 5, 6, 0, 0)


e5(1, 0, 6, 5, 6, 0, 0)


e5(1, 1, 6, 5, 6, 0, 0)


e5(1, 2, 6, 5, 6, 0, 0)


e5(1, 3, 6, 5, 6, 0, 0)


e5(1, 4, 6, 5, 6, 0, 0)


e5(1, 5, 6, 5, 6, 0, 0)


e5(2, 0, 6, 5, 6, 0, 0)


e5(2, 1, 6, 5, 6, 0, 0)


e5(2, 2, 6, 5, 6, 0, 0)


e5(2, 3, 6, 5, 6, 0, 0)


e5(2, 4, 6, 5, 6, 0, 0)


e5(2, 5, 6, 5, 6, 0, 0)


e5(3, 0, 6, 5, 6, 0, 0)


e5(3, 1, 6, 5, 6, 0, 0)


e5(3, 2, 6, 5, 6, 0, 0)


e5(3, 3, 6, 5, 6, 0, 0)


e5(3, 4, 6, 5, 6, 0, 0)


e5(3, 5, 6, 5, 6, 0, 0)


e5(4, 0, 6, 5, 6, 0, 0)


e5(4, 1, 6, 5, 6, 0, 0)


e5(4, 2, 6, 5, 6, 0, 0)


e5(4, 3, 6, 5, 6, 0, 0)


e5(4, 4, 6, 5, 6, 0, 0)


e5(4, 5, 6, 5, 6, 0, 0)





















	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,5,5,6,5)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 6, 5, 5, 0, 1)


e5(0, 1, 6, 5, 5, 0, 1)


e5(0, 2, 6, 5, 5, 0, 1)


e5(0, 3, 6, 5, 5, 0, 1)


e5(0, 4, 6, 5, 5, 0, 1)


e5(0, 5, 6, 5, 5, 0, 1)


e5(1, 0, 6, 5, 5, 0, 1)


e5(1, 1, 6, 5, 5, 0, 1)


e5(1, 2, 6, 5, 5, 0, 1)


e5(1, 3, 6, 5, 5, 0, 1)


e5(1, 4, 6, 5, 5, 0, 1)


e5(1, 5, 6, 5, 5, 0, 1)


e5(2, 0, 6, 5, 5, 0, 1)


e5(2, 1, 6, 5, 5, 0, 1)


e5(2, 2, 6, 5, 5, 0, 1)


e5(2, 3, 6, 5, 5, 0, 1)


e5(2, 4, 6, 5, 5, 0, 1)


e5(2, 5, 6, 5, 5, 0, 1)


e5(3, 0, 6, 5, 5, 0, 1)


e5(3, 1, 6, 5, 5, 0, 1)


e5(3, 2, 6, 5, 5, 0, 1)


e5(3, 3, 6, 5, 5, 0, 1)


e5(3, 4, 6, 5, 5, 0, 1)


e5(3, 5, 6, 5, 5, 0, 1)


e5(4, 0, 6, 5, 5, 0, 1)


e5(4, 1, 6, 5, 5, 0, 1)


e5(4, 2, 6, 5, 5, 0, 1)


e5(4, 3, 6, 5, 5, 0, 1)


e5(4, 4, 6, 5, 5, 0, 1)


e5(4, 5, 6, 5, 5, 0, 1)





















	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,5,5,5,6)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 6, 5, 6, 1, 0)


e5(0, 1, 6, 5, 6, 1, 0)


e5(0, 2, 6, 5, 6, 1, 0)


e5(0, 3, 6, 5, 6, 1, 0)


e5(0, 4, 6, 5, 6, 1, 0)


e5(0, 5, 6, 5, 6, 1, 0)


e5(1, 0, 6, 5, 6, 1, 0)


e5(1, 1, 6, 5, 6, 1, 0)


e5(1, 2, 6, 5, 6, 1, 0)


e5(1, 3, 6, 5, 6, 1, 0)


e5(1, 4, 6, 5, 6, 1, 0)


e5(1, 5, 6, 5, 6, 1, 0)


e5(2, 0, 6, 5, 6, 1, 0)


e5(2, 1, 6, 5, 6, 1, 0)


e5(2, 2, 6, 5, 6, 1, 0)


e5(2, 3, 6, 5, 6, 1, 0)


e5(2, 4, 6, 5, 6, 1, 0)


e5(2, 5, 6, 5, 6, 1, 0)


e5(3, 0, 6, 5, 6, 1, 0)


e5(3, 1, 6, 5, 6, 1, 0)


e5(3, 2, 6, 5, 6, 1, 0)


e5(3, 3, 6, 5, 6, 1, 0)


e5(3, 4, 6, 5, 6, 1, 0)


e5(3, 5, 6, 5, 6, 1, 0)


e5(4, 0, 6, 5, 6, 1, 0)


e5(4, 1, 6, 5, 6, 1, 0)


e5(4, 2, 6, 5, 6, 1, 0)


e5(4, 3, 6, 5, 6, 1, 0)


e5(4, 4, 6, 5, 6, 1, 0)


e5(4, 5, 6, 5, 6, 1, 0)





















	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,5,5,6,5)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 6, 5, 5, 1, 1)


e5(0, 1, 6, 5, 5, 1, 1)


e5(0, 2, 6, 5, 5, 1, 1)


e5(0, 3, 6, 5, 5, 1, 1)


e5(0, 4, 6, 5, 5, 1, 1)


e5(0, 5, 6, 5, 5, 1, 1)


e5(1, 0, 6, 5, 5, 1, 1)


e5(1, 1, 6, 5, 5, 1, 1)


e5(1, 2, 6, 5, 5, 1, 1)


e5(1, 3, 6, 5, 5, 1, 1)


e5(1, 4, 6, 5, 5, 1, 1)


e5(1, 5, 6, 5, 5, 1, 1)


e5(2, 0, 6, 5, 5, 1, 1)


e5(2, 1, 6, 5, 5, 1, 1)


e5(2, 2, 6, 5, 5, 1, 1)


e5(2, 3, 6, 5, 5, 1, 1)


e5(2, 4, 6, 5, 5, 1, 1)


e5(2, 5, 6, 5, 5, 1, 1)


e5(3, 0, 6, 5, 5, 1, 1)


e5(3, 1, 6, 5, 5, 1, 1)


e5(3, 2, 6, 5, 5, 1, 1)


e5(3, 3, 6, 5, 5, 1, 1)


e5(3, 4, 6, 5, 5, 1, 1)


e5(3, 5, 6, 5, 5, 1, 1)


e5(4, 0, 6, 5, 5, 1, 1)


e5(4, 1, 6, 5, 5, 1, 1)


e5(4, 2, 6, 5, 5, 1, 1)


e5(4, 3, 6, 5, 5, 1, 1)


e5(4, 4, 6, 5, 5, 1, 1)


e5(4, 5, 6, 5, 5, 1, 1)





















	__asm mov eax, edx __asm pop ebx __asm ret
}























































	
	




























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,5,5,5,5)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 5, 5, 5, 0, 0)


e5(0, 1, 5, 5, 5, 0, 0)


e5(0, 2, 5, 5, 5, 0, 0)


e5(0, 3, 5, 5, 5, 0, 0)


e5(0, 4, 5, 5, 5, 0, 0)





e5(1, 0, 5, 5, 5, 0, 0)


e5(1, 1, 5, 5, 5, 0, 0)


e5(1, 2, 5, 5, 5, 0, 0)


e5(1, 3, 5, 5, 5, 0, 0)


e5(1, 4, 5, 5, 5, 0, 0)





e5(2, 0, 5, 5, 5, 0, 0)


e5(2, 1, 5, 5, 5, 0, 0)


e5(2, 2, 5, 5, 5, 0, 0)


e5(2, 3, 5, 5, 5, 0, 0)


e5(2, 4, 5, 5, 5, 0, 0)





e5(3, 0, 5, 5, 5, 0, 0)


e5(3, 1, 5, 5, 5, 0, 0)


e5(3, 2, 5, 5, 5, 0, 0)


e5(3, 3, 5, 5, 5, 0, 0)


e5(3, 4, 5, 5, 5, 0, 0)





e5(4, 0, 5, 5, 5, 0, 0)


e5(4, 1, 5, 5, 5, 0, 0)


e5(4, 2, 5, 5, 5, 0, 0)


e5(4, 3, 5, 5, 5, 0, 0)


e5(4, 4, 5, 5, 5, 0, 0)
























	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,5,5,5,5)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 5, 5, 5, 0, 1)


e5(0, 1, 5, 5, 5, 0, 1)


e5(0, 2, 5, 5, 5, 0, 1)


e5(0, 3, 5, 5, 5, 0, 1)


e5(0, 4, 5, 5, 5, 0, 1)





e5(1, 0, 5, 5, 5, 0, 1)


e5(1, 1, 5, 5, 5, 0, 1)


e5(1, 2, 5, 5, 5, 0, 1)


e5(1, 3, 5, 5, 5, 0, 1)


e5(1, 4, 5, 5, 5, 0, 1)





e5(2, 0, 5, 5, 5, 0, 1)


e5(2, 1, 5, 5, 5, 0, 1)


e5(2, 2, 5, 5, 5, 0, 1)


e5(2, 3, 5, 5, 5, 0, 1)


e5(2, 4, 5, 5, 5, 0, 1)





e5(3, 0, 5, 5, 5, 0, 1)


e5(3, 1, 5, 5, 5, 0, 1)


e5(3, 2, 5, 5, 5, 0, 1)


e5(3, 3, 5, 5, 5, 0, 1)


e5(3, 4, 5, 5, 5, 0, 1)





e5(4, 0, 5, 5, 5, 0, 1)


e5(4, 1, 5, 5, 5, 0, 1)


e5(4, 2, 5, 5, 5, 0, 1)


e5(4, 3, 5, 5, 5, 0, 1)


e5(4, 4, 5, 5, 5, 0, 1)
























	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,5,5,5,5)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 5, 5, 5, 1, 0)


e5(0, 1, 5, 5, 5, 1, 0)


e5(0, 2, 5, 5, 5, 1, 0)


e5(0, 3, 5, 5, 5, 1, 0)


e5(0, 4, 5, 5, 5, 1, 0)





e5(1, 0, 5, 5, 5, 1, 0)


e5(1, 1, 5, 5, 5, 1, 0)


e5(1, 2, 5, 5, 5, 1, 0)


e5(1, 3, 5, 5, 5, 1, 0)


e5(1, 4, 5, 5, 5, 1, 0)





e5(2, 0, 5, 5, 5, 1, 0)


e5(2, 1, 5, 5, 5, 1, 0)


e5(2, 2, 5, 5, 5, 1, 0)


e5(2, 3, 5, 5, 5, 1, 0)


e5(2, 4, 5, 5, 5, 1, 0)





e5(3, 0, 5, 5, 5, 1, 0)


e5(3, 1, 5, 5, 5, 1, 0)


e5(3, 2, 5, 5, 5, 1, 0)


e5(3, 3, 5, 5, 5, 1, 0)


e5(3, 4, 5, 5, 5, 1, 0)





e5(4, 0, 5, 5, 5, 1, 0)


e5(4, 1, 5, 5, 5, 1, 0)


e5(4, 2, 5, 5, 5, 1, 0)


e5(4, 3, 5, 5, 5, 1, 0)


e5(4, 4, 5, 5, 5, 1, 0)
























	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,5,5,5,5)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 5, 5, 5, 1, 1)


e5(0, 1, 5, 5, 5, 1, 1)


e5(0, 2, 5, 5, 5, 1, 1)


e5(0, 3, 5, 5, 5, 1, 1)


e5(0, 4, 5, 5, 5, 1, 1)





e5(1, 0, 5, 5, 5, 1, 1)


e5(1, 1, 5, 5, 5, 1, 1)


e5(1, 2, 5, 5, 5, 1, 1)


e5(1, 3, 5, 5, 5, 1, 1)


e5(1, 4, 5, 5, 5, 1, 1)





e5(2, 0, 5, 5, 5, 1, 1)


e5(2, 1, 5, 5, 5, 1, 1)


e5(2, 2, 5, 5, 5, 1, 1)


e5(2, 3, 5, 5, 5, 1, 1)


e5(2, 4, 5, 5, 5, 1, 1)





e5(3, 0, 5, 5, 5, 1, 1)


e5(3, 1, 5, 5, 5, 1, 1)


e5(3, 2, 5, 5, 5, 1, 1)


e5(3, 3, 5, 5, 5, 1, 1)


e5(3, 4, 5, 5, 5, 1, 1)





e5(4, 0, 5, 5, 5, 1, 1)


e5(4, 1, 5, 5, 5, 1, 1)


e5(4, 2, 5, 5, 5, 1, 1)


e5(4, 3, 5, 5, 5, 1, 1)


e5(4, 4, 5, 5, 5, 1, 1)
























	__asm mov eax, edx __asm pop ebx __asm ret
}

















































	
	


































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,5,5,5,3)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 3, 5, 3, 0, 0)


e5(0, 1, 3, 5, 3, 0, 0)


e5(0, 2, 3, 5, 3, 0, 0)











e5(1, 0, 3, 5, 3, 0, 0)


e5(1, 1, 3, 5, 3, 0, 0)


e5(1, 2, 3, 5, 3, 0, 0)











e5(2, 0, 3, 5, 3, 0, 0)


e5(2, 1, 3, 5, 3, 0, 0)


e5(2, 2, 3, 5, 3, 0, 0)











e5(3, 0, 3, 5, 3, 0, 0)


e5(3, 1, 3, 5, 3, 0, 0)


e5(3, 2, 3, 5, 3, 0, 0)











e5(4, 0, 3, 5, 3, 0, 0)


e5(4, 1, 3, 5, 3, 0, 0)


e5(4, 2, 3, 5, 3, 0, 0)






























	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,5,5,3,5)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 3, 5, 5, 0, 1)


e5(0, 1, 3, 5, 5, 0, 1)


e5(0, 2, 3, 5, 5, 0, 1)











e5(1, 0, 3, 5, 5, 0, 1)


e5(1, 1, 3, 5, 5, 0, 1)


e5(1, 2, 3, 5, 5, 0, 1)











e5(2, 0, 3, 5, 5, 0, 1)


e5(2, 1, 3, 5, 5, 0, 1)


e5(2, 2, 3, 5, 5, 0, 1)











e5(3, 0, 3, 5, 5, 0, 1)


e5(3, 1, 3, 5, 5, 0, 1)


e5(3, 2, 3, 5, 5, 0, 1)











e5(4, 0, 3, 5, 5, 0, 1)


e5(4, 1, 3, 5, 5, 0, 1)


e5(4, 2, 3, 5, 5, 0, 1)






























	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,5,5,5,3)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 3, 5, 3, 1, 0)


e5(0, 1, 3, 5, 3, 1, 0)


e5(0, 2, 3, 5, 3, 1, 0)











e5(1, 0, 3, 5, 3, 1, 0)


e5(1, 1, 3, 5, 3, 1, 0)


e5(1, 2, 3, 5, 3, 1, 0)











e5(2, 0, 3, 5, 3, 1, 0)


e5(2, 1, 3, 5, 3, 1, 0)


e5(2, 2, 3, 5, 3, 1, 0)











e5(3, 0, 3, 5, 3, 1, 0)


e5(3, 1, 3, 5, 3, 1, 0)


e5(3, 2, 3, 5, 3, 1, 0)











e5(4, 0, 3, 5, 3, 1, 0)


e5(4, 1, 3, 5, 3, 1, 0)


e5(4, 2, 3, 5, 3, 1, 0)






























	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,5,5,3,5)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 3, 5, 5, 1, 1)


e5(0, 1, 3, 5, 5, 1, 1)


e5(0, 2, 3, 5, 5, 1, 1)











e5(1, 0, 3, 5, 5, 1, 1)


e5(1, 1, 3, 5, 5, 1, 1)


e5(1, 2, 3, 5, 5, 1, 1)











e5(2, 0, 3, 5, 5, 1, 1)


e5(2, 1, 3, 5, 5, 1, 1)


e5(2, 2, 3, 5, 5, 1, 1)











e5(3, 0, 3, 5, 5, 1, 1)


e5(3, 1, 3, 5, 5, 1, 1)


e5(3, 2, 3, 5, 5, 1, 1)











e5(4, 0, 3, 5, 5, 1, 1)


e5(4, 1, 3, 5, 5, 1, 1)


e5(4, 2, 3, 5, 5, 1, 1)






























	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,5,5,5,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 1, 5, 1, 0, 0)

















e5(1, 0, 1, 5, 1, 0, 0)

















e5(2, 0, 1, 5, 1, 0, 0)

















e5(3, 0, 1, 5, 1, 0, 0)

















e5(4, 0, 1, 5, 1, 0, 0)




































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,5,5,1,5)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 1, 5, 5, 0, 1)

















e5(1, 0, 1, 5, 5, 0, 1)

















e5(2, 0, 1, 5, 5, 0, 1)

















e5(3, 0, 1, 5, 5, 0, 1)

















e5(4, 0, 1, 5, 5, 0, 1)




































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,5,5,5,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 1, 5, 1, 1, 0)

















e5(1, 0, 1, 5, 1, 1, 0)

















e5(2, 0, 1, 5, 1, 1, 0)

















e5(3, 0, 1, 5, 1, 1, 0)

















e5(4, 0, 1, 5, 1, 1, 0)




































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,5,5,1,5)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 1, 5, 5, 1, 1)

















e5(1, 0, 1, 5, 5, 1, 1)

















e5(2, 0, 1, 5, 5, 1, 1)

















e5(3, 0, 1, 5, 5, 1, 1)

















e5(4, 0, 1, 5, 5, 1, 1)




































	__asm mov eax, edx __asm pop ebx __asm ret
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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (1 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (2 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (3 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (3 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (4 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (4 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (4 * 1 + 0)]




































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,5,1)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,5,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (1 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (2 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (3 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (3 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (4 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (4 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (4 * 1 + 0)]




































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,5,1)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (1 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (2 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (3 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (3 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (4 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (4 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 1 + 0)]







































	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,5,1)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (1 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (2 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (3 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (3 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (4 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (4 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 1 + 0)]







































	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(Assign,5,1)(SMLXMatrix &A, const SMLXMatrix &B) 







{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax


























__asm fld dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [edx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [edx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 1 + 0)]

















__asm fld dword ptr [edx + 4 * (3 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 1 + 0)]

















__asm fld dword ptr [edx + 4 * (4 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 1 + 0)]




































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

























































	
	

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,5,1,1,6)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 6, 1, 6, 0, 0)


e1(0, 1, 6, 1, 6, 0, 0)


e1(0, 2, 6, 1, 6, 0, 0)


e1(0, 3, 6, 1, 6, 0, 0)


e1(0, 4, 6, 1, 6, 0, 0)


e1(0, 5, 6, 1, 6, 0, 0)


e1(1, 0, 6, 1, 6, 0, 0)


e1(1, 1, 6, 1, 6, 0, 0)


e1(1, 2, 6, 1, 6, 0, 0)


e1(1, 3, 6, 1, 6, 0, 0)


e1(1, 4, 6, 1, 6, 0, 0)


e1(1, 5, 6, 1, 6, 0, 0)


e1(2, 0, 6, 1, 6, 0, 0)


e1(2, 1, 6, 1, 6, 0, 0)


e1(2, 2, 6, 1, 6, 0, 0)


e1(2, 3, 6, 1, 6, 0, 0)


e1(2, 4, 6, 1, 6, 0, 0)


e1(2, 5, 6, 1, 6, 0, 0)


e1(3, 0, 6, 1, 6, 0, 0)


e1(3, 1, 6, 1, 6, 0, 0)


e1(3, 2, 6, 1, 6, 0, 0)


e1(3, 3, 6, 1, 6, 0, 0)


e1(3, 4, 6, 1, 6, 0, 0)


e1(3, 5, 6, 1, 6, 0, 0)


e1(4, 0, 6, 1, 6, 0, 0)


e1(4, 1, 6, 1, 6, 0, 0)


e1(4, 2, 6, 1, 6, 0, 0)


e1(4, 3, 6, 1, 6, 0, 0)


e1(4, 4, 6, 1, 6, 0, 0)


e1(4, 5, 6, 1, 6, 0, 0)





















	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,5,1,6,1)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 6, 1, 1, 0, 1)


e1(0, 1, 6, 1, 1, 0, 1)


e1(0, 2, 6, 1, 1, 0, 1)


e1(0, 3, 6, 1, 1, 0, 1)


e1(0, 4, 6, 1, 1, 0, 1)


e1(0, 5, 6, 1, 1, 0, 1)


e1(1, 0, 6, 1, 1, 0, 1)


e1(1, 1, 6, 1, 1, 0, 1)


e1(1, 2, 6, 1, 1, 0, 1)


e1(1, 3, 6, 1, 1, 0, 1)


e1(1, 4, 6, 1, 1, 0, 1)


e1(1, 5, 6, 1, 1, 0, 1)


e1(2, 0, 6, 1, 1, 0, 1)


e1(2, 1, 6, 1, 1, 0, 1)


e1(2, 2, 6, 1, 1, 0, 1)


e1(2, 3, 6, 1, 1, 0, 1)


e1(2, 4, 6, 1, 1, 0, 1)


e1(2, 5, 6, 1, 1, 0, 1)


e1(3, 0, 6, 1, 1, 0, 1)


e1(3, 1, 6, 1, 1, 0, 1)


e1(3, 2, 6, 1, 1, 0, 1)


e1(3, 3, 6, 1, 1, 0, 1)


e1(3, 4, 6, 1, 1, 0, 1)


e1(3, 5, 6, 1, 1, 0, 1)


e1(4, 0, 6, 1, 1, 0, 1)


e1(4, 1, 6, 1, 1, 0, 1)


e1(4, 2, 6, 1, 1, 0, 1)


e1(4, 3, 6, 1, 1, 0, 1)


e1(4, 4, 6, 1, 1, 0, 1)


e1(4, 5, 6, 1, 1, 0, 1)





















	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,5,1,5,6)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 6, 1, 6, 1, 0)


e5(0, 1, 6, 1, 6, 1, 0)


e5(0, 2, 6, 1, 6, 1, 0)


e5(0, 3, 6, 1, 6, 1, 0)


e5(0, 4, 6, 1, 6, 1, 0)


e5(0, 5, 6, 1, 6, 1, 0)





























































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,5,1,6,5)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 6, 1, 5, 1, 1)


e5(0, 1, 6, 1, 5, 1, 1)


e5(0, 2, 6, 1, 5, 1, 1)


e5(0, 3, 6, 1, 5, 1, 1)


e5(0, 4, 6, 1, 5, 1, 1)


e5(0, 5, 6, 1, 5, 1, 1)





























































































	__asm mov eax, edx __asm pop ebx __asm ret
}























































	
	




























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,5,1,1,5)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 5, 1, 5, 0, 0)


e1(0, 1, 5, 1, 5, 0, 0)


e1(0, 2, 5, 1, 5, 0, 0)


e1(0, 3, 5, 1, 5, 0, 0)


e1(0, 4, 5, 1, 5, 0, 0)





e1(1, 0, 5, 1, 5, 0, 0)


e1(1, 1, 5, 1, 5, 0, 0)


e1(1, 2, 5, 1, 5, 0, 0)


e1(1, 3, 5, 1, 5, 0, 0)


e1(1, 4, 5, 1, 5, 0, 0)





e1(2, 0, 5, 1, 5, 0, 0)


e1(2, 1, 5, 1, 5, 0, 0)


e1(2, 2, 5, 1, 5, 0, 0)


e1(2, 3, 5, 1, 5, 0, 0)


e1(2, 4, 5, 1, 5, 0, 0)





e1(3, 0, 5, 1, 5, 0, 0)


e1(3, 1, 5, 1, 5, 0, 0)


e1(3, 2, 5, 1, 5, 0, 0)


e1(3, 3, 5, 1, 5, 0, 0)


e1(3, 4, 5, 1, 5, 0, 0)





e1(4, 0, 5, 1, 5, 0, 0)


e1(4, 1, 5, 1, 5, 0, 0)


e1(4, 2, 5, 1, 5, 0, 0)


e1(4, 3, 5, 1, 5, 0, 0)


e1(4, 4, 5, 1, 5, 0, 0)
























	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,5,1,5,1)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 5, 1, 1, 0, 1)


e1(0, 1, 5, 1, 1, 0, 1)


e1(0, 2, 5, 1, 1, 0, 1)


e1(0, 3, 5, 1, 1, 0, 1)


e1(0, 4, 5, 1, 1, 0, 1)





e1(1, 0, 5, 1, 1, 0, 1)


e1(1, 1, 5, 1, 1, 0, 1)


e1(1, 2, 5, 1, 1, 0, 1)


e1(1, 3, 5, 1, 1, 0, 1)


e1(1, 4, 5, 1, 1, 0, 1)





e1(2, 0, 5, 1, 1, 0, 1)


e1(2, 1, 5, 1, 1, 0, 1)


e1(2, 2, 5, 1, 1, 0, 1)


e1(2, 3, 5, 1, 1, 0, 1)


e1(2, 4, 5, 1, 1, 0, 1)





e1(3, 0, 5, 1, 1, 0, 1)


e1(3, 1, 5, 1, 1, 0, 1)


e1(3, 2, 5, 1, 1, 0, 1)


e1(3, 3, 5, 1, 1, 0, 1)


e1(3, 4, 5, 1, 1, 0, 1)





e1(4, 0, 5, 1, 1, 0, 1)


e1(4, 1, 5, 1, 1, 0, 1)


e1(4, 2, 5, 1, 1, 0, 1)


e1(4, 3, 5, 1, 1, 0, 1)


e1(4, 4, 5, 1, 1, 0, 1)
























	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,5,1,5,5)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 5, 1, 5, 1, 0)


e5(0, 1, 5, 1, 5, 1, 0)


e5(0, 2, 5, 1, 5, 1, 0)


e5(0, 3, 5, 1, 5, 1, 0)


e5(0, 4, 5, 1, 5, 1, 0)
































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,5,1,5,5)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 5, 1, 5, 1, 1)


e5(0, 1, 5, 1, 5, 1, 1)


e5(0, 2, 5, 1, 5, 1, 1)


e5(0, 3, 5, 1, 5, 1, 1)


e5(0, 4, 5, 1, 5, 1, 1)
































































































	__asm mov eax, edx __asm pop ebx __asm ret
}

















































	
	


































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,5,1,1,3)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 3, 1, 3, 0, 0)


e1(0, 1, 3, 1, 3, 0, 0)


e1(0, 2, 3, 1, 3, 0, 0)











e1(1, 0, 3, 1, 3, 0, 0)


e1(1, 1, 3, 1, 3, 0, 0)


e1(1, 2, 3, 1, 3, 0, 0)











e1(2, 0, 3, 1, 3, 0, 0)


e1(2, 1, 3, 1, 3, 0, 0)


e1(2, 2, 3, 1, 3, 0, 0)











e1(3, 0, 3, 1, 3, 0, 0)


e1(3, 1, 3, 1, 3, 0, 0)


e1(3, 2, 3, 1, 3, 0, 0)











e1(4, 0, 3, 1, 3, 0, 0)


e1(4, 1, 3, 1, 3, 0, 0)


e1(4, 2, 3, 1, 3, 0, 0)






























	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,5,1,3,1)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 3, 1, 1, 0, 1)


e1(0, 1, 3, 1, 1, 0, 1)


e1(0, 2, 3, 1, 1, 0, 1)











e1(1, 0, 3, 1, 1, 0, 1)


e1(1, 1, 3, 1, 1, 0, 1)


e1(1, 2, 3, 1, 1, 0, 1)











e1(2, 0, 3, 1, 1, 0, 1)


e1(2, 1, 3, 1, 1, 0, 1)


e1(2, 2, 3, 1, 1, 0, 1)











e1(3, 0, 3, 1, 1, 0, 1)


e1(3, 1, 3, 1, 1, 0, 1)


e1(3, 2, 3, 1, 1, 0, 1)











e1(4, 0, 3, 1, 1, 0, 1)


e1(4, 1, 3, 1, 1, 0, 1)


e1(4, 2, 3, 1, 1, 0, 1)






























	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,5,1,5,3)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 3, 1, 3, 1, 0)


e5(0, 1, 3, 1, 3, 1, 0)


e5(0, 2, 3, 1, 3, 1, 0)






































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,5,1,3,5)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 3, 1, 5, 1, 1)


e5(0, 1, 3, 1, 5, 1, 1)


e5(0, 2, 3, 1, 5, 1, 1)






































































































	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,5,1,1,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 1, 1, 0, 0)

















e1(1, 0, 1, 1, 1, 0, 0)

















e1(2, 0, 1, 1, 1, 0, 0)

















e1(3, 0, 1, 1, 1, 0, 0)

















e1(4, 0, 1, 1, 1, 0, 0)




































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,5,1,1,1)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 1, 1, 0, 1)

















e1(1, 0, 1, 1, 1, 0, 1)

















e1(2, 0, 1, 1, 1, 0, 1)

















e1(3, 0, 1, 1, 1, 0, 1)

















e1(4, 0, 1, 1, 1, 0, 1)




































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,5,1,5,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 1, 1, 1, 1, 0)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,5,1,1,5)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 1, 1, 5, 1, 1)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 5 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 1)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 2)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 3)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 4)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 4)]
































































































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,1,5)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,1,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 5 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 1)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 2)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 3)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 4)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 4)]
































































































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,1,5)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 5 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 1)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 2)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 3)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 4)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 4)]



































































































	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,1,5)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 5 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 1)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 2)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 3)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 4)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 4)]



































































































	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(Assign,1,5)(SMLXMatrix &A, const SMLXMatrix &B) 







{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax


























__asm fld dword ptr [edx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 0)]


__asm fld dword ptr [edx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 1)]


__asm fld dword ptr [edx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 2)]


__asm fld dword ptr [edx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 3)]


__asm fld dword ptr [edx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 4)]
































































































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

























































	
	

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,1,5,5,6)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 6, 5, 6, 0, 0)


e5(0, 1, 6, 5, 6, 0, 0)


e5(0, 2, 6, 5, 6, 0, 0)


e5(0, 3, 6, 5, 6, 0, 0)


e5(0, 4, 6, 5, 6, 0, 0)


e5(0, 5, 6, 5, 6, 0, 0)





























































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,1,5,6,5)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 6, 5, 5, 0, 1)


e5(0, 1, 6, 5, 5, 0, 1)


e5(0, 2, 6, 5, 5, 0, 1)


e5(0, 3, 6, 5, 5, 0, 1)


e5(0, 4, 6, 5, 5, 0, 1)


e5(0, 5, 6, 5, 5, 0, 1)





























































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,1,5,1,6)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 6, 5, 6, 1, 0)


e1(0, 1, 6, 5, 6, 1, 0)


e1(0, 2, 6, 5, 6, 1, 0)


e1(0, 3, 6, 5, 6, 1, 0)


e1(0, 4, 6, 5, 6, 1, 0)


e1(0, 5, 6, 5, 6, 1, 0)


e1(1, 0, 6, 5, 6, 1, 0)


e1(1, 1, 6, 5, 6, 1, 0)


e1(1, 2, 6, 5, 6, 1, 0)


e1(1, 3, 6, 5, 6, 1, 0)


e1(1, 4, 6, 5, 6, 1, 0)


e1(1, 5, 6, 5, 6, 1, 0)


e1(2, 0, 6, 5, 6, 1, 0)


e1(2, 1, 6, 5, 6, 1, 0)


e1(2, 2, 6, 5, 6, 1, 0)


e1(2, 3, 6, 5, 6, 1, 0)


e1(2, 4, 6, 5, 6, 1, 0)


e1(2, 5, 6, 5, 6, 1, 0)


e1(3, 0, 6, 5, 6, 1, 0)


e1(3, 1, 6, 5, 6, 1, 0)


e1(3, 2, 6, 5, 6, 1, 0)


e1(3, 3, 6, 5, 6, 1, 0)


e1(3, 4, 6, 5, 6, 1, 0)


e1(3, 5, 6, 5, 6, 1, 0)


e1(4, 0, 6, 5, 6, 1, 0)


e1(4, 1, 6, 5, 6, 1, 0)


e1(4, 2, 6, 5, 6, 1, 0)


e1(4, 3, 6, 5, 6, 1, 0)


e1(4, 4, 6, 5, 6, 1, 0)


e1(4, 5, 6, 5, 6, 1, 0)





















	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,1,5,6,1)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 6, 5, 1, 1, 1)


e1(0, 1, 6, 5, 1, 1, 1)


e1(0, 2, 6, 5, 1, 1, 1)


e1(0, 3, 6, 5, 1, 1, 1)


e1(0, 4, 6, 5, 1, 1, 1)


e1(0, 5, 6, 5, 1, 1, 1)


e1(1, 0, 6, 5, 1, 1, 1)


e1(1, 1, 6, 5, 1, 1, 1)


e1(1, 2, 6, 5, 1, 1, 1)


e1(1, 3, 6, 5, 1, 1, 1)


e1(1, 4, 6, 5, 1, 1, 1)


e1(1, 5, 6, 5, 1, 1, 1)


e1(2, 0, 6, 5, 1, 1, 1)


e1(2, 1, 6, 5, 1, 1, 1)


e1(2, 2, 6, 5, 1, 1, 1)


e1(2, 3, 6, 5, 1, 1, 1)


e1(2, 4, 6, 5, 1, 1, 1)


e1(2, 5, 6, 5, 1, 1, 1)


e1(3, 0, 6, 5, 1, 1, 1)


e1(3, 1, 6, 5, 1, 1, 1)


e1(3, 2, 6, 5, 1, 1, 1)


e1(3, 3, 6, 5, 1, 1, 1)


e1(3, 4, 6, 5, 1, 1, 1)


e1(3, 5, 6, 5, 1, 1, 1)


e1(4, 0, 6, 5, 1, 1, 1)


e1(4, 1, 6, 5, 1, 1, 1)


e1(4, 2, 6, 5, 1, 1, 1)


e1(4, 3, 6, 5, 1, 1, 1)


e1(4, 4, 6, 5, 1, 1, 1)


e1(4, 5, 6, 5, 1, 1, 1)





















	__asm mov eax, edx __asm pop ebx __asm ret
}























































	
	




























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,1,5,5,5)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 5, 5, 5, 0, 0)


e5(0, 1, 5, 5, 5, 0, 0)


e5(0, 2, 5, 5, 5, 0, 0)


e5(0, 3, 5, 5, 5, 0, 0)


e5(0, 4, 5, 5, 5, 0, 0)
































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,1,5,5,5)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 5, 5, 5, 0, 1)


e5(0, 1, 5, 5, 5, 0, 1)


e5(0, 2, 5, 5, 5, 0, 1)


e5(0, 3, 5, 5, 5, 0, 1)


e5(0, 4, 5, 5, 5, 0, 1)
































































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,1,5,1,5)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 5, 5, 5, 1, 0)


e1(0, 1, 5, 5, 5, 1, 0)


e1(0, 2, 5, 5, 5, 1, 0)


e1(0, 3, 5, 5, 5, 1, 0)


e1(0, 4, 5, 5, 5, 1, 0)





e1(1, 0, 5, 5, 5, 1, 0)


e1(1, 1, 5, 5, 5, 1, 0)


e1(1, 2, 5, 5, 5, 1, 0)


e1(1, 3, 5, 5, 5, 1, 0)


e1(1, 4, 5, 5, 5, 1, 0)





e1(2, 0, 5, 5, 5, 1, 0)


e1(2, 1, 5, 5, 5, 1, 0)


e1(2, 2, 5, 5, 5, 1, 0)


e1(2, 3, 5, 5, 5, 1, 0)


e1(2, 4, 5, 5, 5, 1, 0)





e1(3, 0, 5, 5, 5, 1, 0)


e1(3, 1, 5, 5, 5, 1, 0)


e1(3, 2, 5, 5, 5, 1, 0)


e1(3, 3, 5, 5, 5, 1, 0)


e1(3, 4, 5, 5, 5, 1, 0)





e1(4, 0, 5, 5, 5, 1, 0)


e1(4, 1, 5, 5, 5, 1, 0)


e1(4, 2, 5, 5, 5, 1, 0)


e1(4, 3, 5, 5, 5, 1, 0)


e1(4, 4, 5, 5, 5, 1, 0)
























	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,1,5,5,1)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 5, 5, 1, 1, 1)


e1(0, 1, 5, 5, 1, 1, 1)


e1(0, 2, 5, 5, 1, 1, 1)


e1(0, 3, 5, 5, 1, 1, 1)


e1(0, 4, 5, 5, 1, 1, 1)





e1(1, 0, 5, 5, 1, 1, 1)


e1(1, 1, 5, 5, 1, 1, 1)


e1(1, 2, 5, 5, 1, 1, 1)


e1(1, 3, 5, 5, 1, 1, 1)


e1(1, 4, 5, 5, 1, 1, 1)





e1(2, 0, 5, 5, 1, 1, 1)


e1(2, 1, 5, 5, 1, 1, 1)


e1(2, 2, 5, 5, 1, 1, 1)


e1(2, 3, 5, 5, 1, 1, 1)


e1(2, 4, 5, 5, 1, 1, 1)





e1(3, 0, 5, 5, 1, 1, 1)


e1(3, 1, 5, 5, 1, 1, 1)


e1(3, 2, 5, 5, 1, 1, 1)


e1(3, 3, 5, 5, 1, 1, 1)


e1(3, 4, 5, 5, 1, 1, 1)





e1(4, 0, 5, 5, 1, 1, 1)


e1(4, 1, 5, 5, 1, 1, 1)


e1(4, 2, 5, 5, 1, 1, 1)


e1(4, 3, 5, 5, 1, 1, 1)


e1(4, 4, 5, 5, 1, 1, 1)
























	__asm mov eax, edx __asm pop ebx __asm ret
}

















































	
	


































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,1,5,5,3)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 3, 5, 3, 0, 0)


e5(0, 1, 3, 5, 3, 0, 0)


e5(0, 2, 3, 5, 3, 0, 0)






































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,1,5,3,5)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 3, 5, 5, 0, 1)


e5(0, 1, 3, 5, 5, 0, 1)


e5(0, 2, 3, 5, 5, 0, 1)






































































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,1,5,1,3)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 3, 5, 3, 1, 0)


e1(0, 1, 3, 5, 3, 1, 0)


e1(0, 2, 3, 5, 3, 1, 0)











e1(1, 0, 3, 5, 3, 1, 0)


e1(1, 1, 3, 5, 3, 1, 0)


e1(1, 2, 3, 5, 3, 1, 0)











e1(2, 0, 3, 5, 3, 1, 0)


e1(2, 1, 3, 5, 3, 1, 0)


e1(2, 2, 3, 5, 3, 1, 0)











e1(3, 0, 3, 5, 3, 1, 0)


e1(3, 1, 3, 5, 3, 1, 0)


e1(3, 2, 3, 5, 3, 1, 0)











e1(4, 0, 3, 5, 3, 1, 0)


e1(4, 1, 3, 5, 3, 1, 0)


e1(4, 2, 3, 5, 3, 1, 0)






























	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,1,5,3,1)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 3, 5, 1, 1, 1)


e1(0, 1, 3, 5, 1, 1, 1)


e1(0, 2, 3, 5, 1, 1, 1)











e1(1, 0, 3, 5, 1, 1, 1)


e1(1, 1, 3, 5, 1, 1, 1)


e1(1, 2, 3, 5, 1, 1, 1)











e1(2, 0, 3, 5, 1, 1, 1)


e1(2, 1, 3, 5, 1, 1, 1)


e1(2, 2, 3, 5, 1, 1, 1)











e1(3, 0, 3, 5, 1, 1, 1)


e1(3, 1, 3, 5, 1, 1, 1)


e1(3, 2, 3, 5, 1, 1, 1)











e1(4, 0, 3, 5, 1, 1, 1)


e1(4, 1, 3, 5, 1, 1, 1)


e1(4, 2, 3, 5, 1, 1, 1)






























	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,1,5,5,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 1, 5, 1, 0, 0)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,1,5,1,5)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 1, 5, 5, 0, 1)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,1,5,1,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 5, 1, 1, 0)

















e1(1, 0, 1, 5, 1, 1, 0)

















e1(2, 0, 1, 5, 1, 1, 0)

















e1(3, 0, 1, 5, 1, 1, 0)

















e1(4, 0, 1, 5, 1, 1, 0)




































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,1,5,1,1)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 5, 1, 1, 1)

















e1(1, 0, 1, 5, 1, 1, 1)

















e1(2, 0, 1, 5, 1, 1, 1)

















e1(3, 0, 1, 5, 1, 1, 1)

















e1(4, 0, 1, 5, 1, 1, 1)




































	__asm mov eax, edx __asm pop ebx __asm ret
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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 3 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 3 + 1)] __asm fadd dword ptr [ecx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 3 + 2)] __asm fadd dword ptr [ecx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (1 * 3 + 0)] __asm fadd dword ptr [ecx + 4 * (1 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 3 + 1)] __asm fadd dword ptr [ecx + 4 * (1 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 3 + 2)] __asm fadd dword ptr [ecx + 4 * (1 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (2 * 3 + 0)] __asm fadd dword ptr [ecx + 4 * (2 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 3 + 1)] __asm fadd dword ptr [ecx + 4 * (2 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 3 + 2)] __asm fadd dword ptr [ecx + 4 * (2 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (3 * 3 + 0)] __asm fadd dword ptr [ecx + 4 * (3 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (3 * 3 + 1)] __asm fadd dword ptr [ecx + 4 * (3 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (3 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (3 * 3 + 2)] __asm fadd dword ptr [ecx + 4 * (3 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (3 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (4 * 3 + 0)] __asm fadd dword ptr [ecx + 4 * (4 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (4 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (4 * 3 + 1)] __asm fadd dword ptr [ecx + 4 * (4 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (4 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (4 * 3 + 2)] __asm fadd dword ptr [ecx + 4 * (4 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (4 * 3 + 2)]






























	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,5,3)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,5,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 3 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 3 + 1)] __asm fsub dword ptr [ecx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 3 + 2)] __asm fsub dword ptr [ecx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (1 * 3 + 0)] __asm fsub dword ptr [ecx + 4 * (1 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 3 + 1)] __asm fsub dword ptr [ecx + 4 * (1 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 3 + 2)] __asm fsub dword ptr [ecx + 4 * (1 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (2 * 3 + 0)] __asm fsub dword ptr [ecx + 4 * (2 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 3 + 1)] __asm fsub dword ptr [ecx + 4 * (2 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 3 + 2)] __asm fsub dword ptr [ecx + 4 * (2 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (3 * 3 + 0)] __asm fsub dword ptr [ecx + 4 * (3 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (3 * 3 + 1)] __asm fsub dword ptr [ecx + 4 * (3 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (3 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (3 * 3 + 2)] __asm fsub dword ptr [ecx + 4 * (3 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (3 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (4 * 3 + 0)] __asm fsub dword ptr [ecx + 4 * (4 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (4 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (4 * 3 + 1)] __asm fsub dword ptr [ecx + 4 * (4 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (4 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (4 * 3 + 2)] __asm fsub dword ptr [ecx + 4 * (4 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (4 * 3 + 2)]






























	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,5,3)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 3 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 3 + 1)] __asm fadd dword ptr [edx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 3 + 2)] __asm fadd dword ptr [edx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (1 * 3 + 0)] __asm fadd dword ptr [edx + 4 * (1 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 3 + 1)] __asm fadd dword ptr [edx + 4 * (1 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 3 + 2)] __asm fadd dword ptr [edx + 4 * (1 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (2 * 3 + 0)] __asm fadd dword ptr [edx + 4 * (2 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 3 + 1)] __asm fadd dword ptr [edx + 4 * (2 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 3 + 2)] __asm fadd dword ptr [edx + 4 * (2 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (3 * 3 + 0)] __asm fadd dword ptr [edx + 4 * (3 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (3 * 3 + 1)] __asm fadd dword ptr [edx + 4 * (3 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (3 * 3 + 2)] __asm fadd dword ptr [edx + 4 * (3 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (4 * 3 + 0)] __asm fadd dword ptr [edx + 4 * (4 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (4 * 3 + 1)] __asm fadd dword ptr [edx + 4 * (4 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (4 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (4 * 3 + 2)] __asm fadd dword ptr [edx + 4 * (4 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (4 * 3 + 2)]

































	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,5,3)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 3 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 3 + 1)] __asm fsub dword ptr [edx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 3 + 2)] __asm fsub dword ptr [edx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (1 * 3 + 0)] __asm fsub dword ptr [edx + 4 * (1 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 3 + 1)] __asm fsub dword ptr [edx + 4 * (1 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 3 + 2)] __asm fsub dword ptr [edx + 4 * (1 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (2 * 3 + 0)] __asm fsub dword ptr [edx + 4 * (2 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 3 + 1)] __asm fsub dword ptr [edx + 4 * (2 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 3 + 2)] __asm fsub dword ptr [edx + 4 * (2 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (3 * 3 + 0)] __asm fsub dword ptr [edx + 4 * (3 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (3 * 3 + 1)] __asm fsub dword ptr [edx + 4 * (3 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (3 * 3 + 2)] __asm fsub dword ptr [edx + 4 * (3 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (4 * 3 + 0)] __asm fsub dword ptr [edx + 4 * (4 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (4 * 3 + 1)] __asm fsub dword ptr [edx + 4 * (4 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (4 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (4 * 3 + 2)] __asm fsub dword ptr [edx + 4 * (4 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (4 * 3 + 2)]

































	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(Assign,5,3)(SMLXMatrix &A, const SMLXMatrix &B) 







{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax


























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

























































	
	

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,5,3,3,6)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 6, 3, 6, 0, 0)


e3(0, 1, 6, 3, 6, 0, 0)


e3(0, 2, 6, 3, 6, 0, 0)


e3(0, 3, 6, 3, 6, 0, 0)


e3(0, 4, 6, 3, 6, 0, 0)


e3(0, 5, 6, 3, 6, 0, 0)


e3(1, 0, 6, 3, 6, 0, 0)


e3(1, 1, 6, 3, 6, 0, 0)


e3(1, 2, 6, 3, 6, 0, 0)


e3(1, 3, 6, 3, 6, 0, 0)


e3(1, 4, 6, 3, 6, 0, 0)


e3(1, 5, 6, 3, 6, 0, 0)


e3(2, 0, 6, 3, 6, 0, 0)


e3(2, 1, 6, 3, 6, 0, 0)


e3(2, 2, 6, 3, 6, 0, 0)


e3(2, 3, 6, 3, 6, 0, 0)


e3(2, 4, 6, 3, 6, 0, 0)


e3(2, 5, 6, 3, 6, 0, 0)


e3(3, 0, 6, 3, 6, 0, 0)


e3(3, 1, 6, 3, 6, 0, 0)


e3(3, 2, 6, 3, 6, 0, 0)


e3(3, 3, 6, 3, 6, 0, 0)


e3(3, 4, 6, 3, 6, 0, 0)


e3(3, 5, 6, 3, 6, 0, 0)


e3(4, 0, 6, 3, 6, 0, 0)


e3(4, 1, 6, 3, 6, 0, 0)


e3(4, 2, 6, 3, 6, 0, 0)


e3(4, 3, 6, 3, 6, 0, 0)


e3(4, 4, 6, 3, 6, 0, 0)


e3(4, 5, 6, 3, 6, 0, 0)





















	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,5,3,6,3)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 6, 3, 3, 0, 1)


e3(0, 1, 6, 3, 3, 0, 1)


e3(0, 2, 6, 3, 3, 0, 1)


e3(0, 3, 6, 3, 3, 0, 1)


e3(0, 4, 6, 3, 3, 0, 1)


e3(0, 5, 6, 3, 3, 0, 1)


e3(1, 0, 6, 3, 3, 0, 1)


e3(1, 1, 6, 3, 3, 0, 1)


e3(1, 2, 6, 3, 3, 0, 1)


e3(1, 3, 6, 3, 3, 0, 1)


e3(1, 4, 6, 3, 3, 0, 1)


e3(1, 5, 6, 3, 3, 0, 1)


e3(2, 0, 6, 3, 3, 0, 1)


e3(2, 1, 6, 3, 3, 0, 1)


e3(2, 2, 6, 3, 3, 0, 1)


e3(2, 3, 6, 3, 3, 0, 1)


e3(2, 4, 6, 3, 3, 0, 1)


e3(2, 5, 6, 3, 3, 0, 1)


e3(3, 0, 6, 3, 3, 0, 1)


e3(3, 1, 6, 3, 3, 0, 1)


e3(3, 2, 6, 3, 3, 0, 1)


e3(3, 3, 6, 3, 3, 0, 1)


e3(3, 4, 6, 3, 3, 0, 1)


e3(3, 5, 6, 3, 3, 0, 1)


e3(4, 0, 6, 3, 3, 0, 1)


e3(4, 1, 6, 3, 3, 0, 1)


e3(4, 2, 6, 3, 3, 0, 1)


e3(4, 3, 6, 3, 3, 0, 1)


e3(4, 4, 6, 3, 3, 0, 1)


e3(4, 5, 6, 3, 3, 0, 1)





















	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,5,3,5,6)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 6, 3, 6, 1, 0)


e5(0, 1, 6, 3, 6, 1, 0)


e5(0, 2, 6, 3, 6, 1, 0)


e5(0, 3, 6, 3, 6, 1, 0)


e5(0, 4, 6, 3, 6, 1, 0)


e5(0, 5, 6, 3, 6, 1, 0)


e5(1, 0, 6, 3, 6, 1, 0)


e5(1, 1, 6, 3, 6, 1, 0)


e5(1, 2, 6, 3, 6, 1, 0)


e5(1, 3, 6, 3, 6, 1, 0)


e5(1, 4, 6, 3, 6, 1, 0)


e5(1, 5, 6, 3, 6, 1, 0)


e5(2, 0, 6, 3, 6, 1, 0)


e5(2, 1, 6, 3, 6, 1, 0)


e5(2, 2, 6, 3, 6, 1, 0)


e5(2, 3, 6, 3, 6, 1, 0)


e5(2, 4, 6, 3, 6, 1, 0)


e5(2, 5, 6, 3, 6, 1, 0)

























































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,5,3,6,5)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 6, 3, 5, 1, 1)


e5(0, 1, 6, 3, 5, 1, 1)


e5(0, 2, 6, 3, 5, 1, 1)


e5(0, 3, 6, 3, 5, 1, 1)


e5(0, 4, 6, 3, 5, 1, 1)


e5(0, 5, 6, 3, 5, 1, 1)


e5(1, 0, 6, 3, 5, 1, 1)


e5(1, 1, 6, 3, 5, 1, 1)


e5(1, 2, 6, 3, 5, 1, 1)


e5(1, 3, 6, 3, 5, 1, 1)


e5(1, 4, 6, 3, 5, 1, 1)


e5(1, 5, 6, 3, 5, 1, 1)


e5(2, 0, 6, 3, 5, 1, 1)


e5(2, 1, 6, 3, 5, 1, 1)


e5(2, 2, 6, 3, 5, 1, 1)


e5(2, 3, 6, 3, 5, 1, 1)


e5(2, 4, 6, 3, 5, 1, 1)


e5(2, 5, 6, 3, 5, 1, 1)

























































	__asm mov eax, edx __asm pop ebx __asm ret
}























































	
	




























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,5,3,3,5)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 5, 3, 5, 0, 0)


e3(0, 1, 5, 3, 5, 0, 0)


e3(0, 2, 5, 3, 5, 0, 0)


e3(0, 3, 5, 3, 5, 0, 0)


e3(0, 4, 5, 3, 5, 0, 0)





e3(1, 0, 5, 3, 5, 0, 0)


e3(1, 1, 5, 3, 5, 0, 0)


e3(1, 2, 5, 3, 5, 0, 0)


e3(1, 3, 5, 3, 5, 0, 0)


e3(1, 4, 5, 3, 5, 0, 0)





e3(2, 0, 5, 3, 5, 0, 0)


e3(2, 1, 5, 3, 5, 0, 0)


e3(2, 2, 5, 3, 5, 0, 0)


e3(2, 3, 5, 3, 5, 0, 0)


e3(2, 4, 5, 3, 5, 0, 0)





e3(3, 0, 5, 3, 5, 0, 0)


e3(3, 1, 5, 3, 5, 0, 0)


e3(3, 2, 5, 3, 5, 0, 0)


e3(3, 3, 5, 3, 5, 0, 0)


e3(3, 4, 5, 3, 5, 0, 0)





e3(4, 0, 5, 3, 5, 0, 0)


e3(4, 1, 5, 3, 5, 0, 0)


e3(4, 2, 5, 3, 5, 0, 0)


e3(4, 3, 5, 3, 5, 0, 0)


e3(4, 4, 5, 3, 5, 0, 0)
























	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,5,3,5,3)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 5, 3, 3, 0, 1)


e3(0, 1, 5, 3, 3, 0, 1)


e3(0, 2, 5, 3, 3, 0, 1)


e3(0, 3, 5, 3, 3, 0, 1)


e3(0, 4, 5, 3, 3, 0, 1)





e3(1, 0, 5, 3, 3, 0, 1)


e3(1, 1, 5, 3, 3, 0, 1)


e3(1, 2, 5, 3, 3, 0, 1)


e3(1, 3, 5, 3, 3, 0, 1)


e3(1, 4, 5, 3, 3, 0, 1)





e3(2, 0, 5, 3, 3, 0, 1)


e3(2, 1, 5, 3, 3, 0, 1)


e3(2, 2, 5, 3, 3, 0, 1)


e3(2, 3, 5, 3, 3, 0, 1)


e3(2, 4, 5, 3, 3, 0, 1)





e3(3, 0, 5, 3, 3, 0, 1)


e3(3, 1, 5, 3, 3, 0, 1)


e3(3, 2, 5, 3, 3, 0, 1)


e3(3, 3, 5, 3, 3, 0, 1)


e3(3, 4, 5, 3, 3, 0, 1)





e3(4, 0, 5, 3, 3, 0, 1)


e3(4, 1, 5, 3, 3, 0, 1)


e3(4, 2, 5, 3, 3, 0, 1)


e3(4, 3, 5, 3, 3, 0, 1)


e3(4, 4, 5, 3, 3, 0, 1)
























	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,5,3,5,5)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 5, 3, 5, 1, 0)


e5(0, 1, 5, 3, 5, 1, 0)


e5(0, 2, 5, 3, 5, 1, 0)


e5(0, 3, 5, 3, 5, 1, 0)


e5(0, 4, 5, 3, 5, 1, 0)





e5(1, 0, 5, 3, 5, 1, 0)


e5(1, 1, 5, 3, 5, 1, 0)


e5(1, 2, 5, 3, 5, 1, 0)


e5(1, 3, 5, 3, 5, 1, 0)


e5(1, 4, 5, 3, 5, 1, 0)





e5(2, 0, 5, 3, 5, 1, 0)


e5(2, 1, 5, 3, 5, 1, 0)


e5(2, 2, 5, 3, 5, 1, 0)


e5(2, 3, 5, 3, 5, 1, 0)


e5(2, 4, 5, 3, 5, 1, 0)




























































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,5,3,5,5)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 5, 3, 5, 1, 1)


e5(0, 1, 5, 3, 5, 1, 1)


e5(0, 2, 5, 3, 5, 1, 1)


e5(0, 3, 5, 3, 5, 1, 1)


e5(0, 4, 5, 3, 5, 1, 1)





e5(1, 0, 5, 3, 5, 1, 1)


e5(1, 1, 5, 3, 5, 1, 1)


e5(1, 2, 5, 3, 5, 1, 1)


e5(1, 3, 5, 3, 5, 1, 1)


e5(1, 4, 5, 3, 5, 1, 1)





e5(2, 0, 5, 3, 5, 1, 1)


e5(2, 1, 5, 3, 5, 1, 1)


e5(2, 2, 5, 3, 5, 1, 1)


e5(2, 3, 5, 3, 5, 1, 1)


e5(2, 4, 5, 3, 5, 1, 1)




























































	__asm mov eax, edx __asm pop ebx __asm ret
}

















































	
	


































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,5,3,3,3)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 3, 3, 3, 0, 0)


e3(0, 1, 3, 3, 3, 0, 0)


e3(0, 2, 3, 3, 3, 0, 0)











e3(1, 0, 3, 3, 3, 0, 0)


e3(1, 1, 3, 3, 3, 0, 0)


e3(1, 2, 3, 3, 3, 0, 0)











e3(2, 0, 3, 3, 3, 0, 0)


e3(2, 1, 3, 3, 3, 0, 0)


e3(2, 2, 3, 3, 3, 0, 0)











e3(3, 0, 3, 3, 3, 0, 0)


e3(3, 1, 3, 3, 3, 0, 0)


e3(3, 2, 3, 3, 3, 0, 0)











e3(4, 0, 3, 3, 3, 0, 0)


e3(4, 1, 3, 3, 3, 0, 0)


e3(4, 2, 3, 3, 3, 0, 0)






























	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,5,3,3,3)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 3, 3, 3, 0, 1)


e3(0, 1, 3, 3, 3, 0, 1)


e3(0, 2, 3, 3, 3, 0, 1)











e3(1, 0, 3, 3, 3, 0, 1)


e3(1, 1, 3, 3, 3, 0, 1)


e3(1, 2, 3, 3, 3, 0, 1)











e3(2, 0, 3, 3, 3, 0, 1)


e3(2, 1, 3, 3, 3, 0, 1)


e3(2, 2, 3, 3, 3, 0, 1)











e3(3, 0, 3, 3, 3, 0, 1)


e3(3, 1, 3, 3, 3, 0, 1)


e3(3, 2, 3, 3, 3, 0, 1)











e3(4, 0, 3, 3, 3, 0, 1)


e3(4, 1, 3, 3, 3, 0, 1)


e3(4, 2, 3, 3, 3, 0, 1)






























	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,5,3,5,3)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 3, 3, 3, 1, 0)


e5(0, 1, 3, 3, 3, 1, 0)


e5(0, 2, 3, 3, 3, 1, 0)











e5(1, 0, 3, 3, 3, 1, 0)


e5(1, 1, 3, 3, 3, 1, 0)


e5(1, 2, 3, 3, 3, 1, 0)











e5(2, 0, 3, 3, 3, 1, 0)


e5(2, 1, 3, 3, 3, 1, 0)


e5(2, 2, 3, 3, 3, 1, 0)


































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,5,3,3,5)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 3, 3, 5, 1, 1)


e5(0, 1, 3, 3, 5, 1, 1)


e5(0, 2, 3, 3, 5, 1, 1)











e5(1, 0, 3, 3, 5, 1, 1)


e5(1, 1, 3, 3, 5, 1, 1)


e5(1, 2, 3, 3, 5, 1, 1)











e5(2, 0, 3, 3, 5, 1, 1)


e5(2, 1, 3, 3, 5, 1, 1)


e5(2, 2, 3, 3, 5, 1, 1)


































































	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,5,3,3,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 1, 3, 1, 0, 0)

















e3(1, 0, 1, 3, 1, 0, 0)

















e3(2, 0, 1, 3, 1, 0, 0)

















e3(3, 0, 1, 3, 1, 0, 0)

















e3(4, 0, 1, 3, 1, 0, 0)




































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,5,3,1,3)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 1, 3, 3, 0, 1)

















e3(1, 0, 1, 3, 3, 0, 1)

















e3(2, 0, 1, 3, 3, 0, 1)

















e3(3, 0, 1, 3, 3, 0, 1)

















e3(4, 0, 1, 3, 3, 0, 1)




































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,5,3,5,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 1, 3, 1, 1, 0)

















e5(1, 0, 1, 3, 1, 1, 0)

















e5(2, 0, 1, 3, 1, 1, 0)








































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,5,3,1,5)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 1, 3, 5, 1, 1)

















e5(1, 0, 1, 3, 5, 1, 1)

















e5(2, 0, 1, 3, 5, 1, 1)








































































	__asm mov eax, edx __asm pop ebx __asm ret
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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 5 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 1)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 2)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 3)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 4)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (1 * 5 + 0)] __asm fadd dword ptr [ecx + 4 * (1 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 1)] __asm fadd dword ptr [ecx + 4 * (1 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 2)] __asm fadd dword ptr [ecx + 4 * (1 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 3)] __asm fadd dword ptr [ecx + 4 * (1 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 4)] __asm fadd dword ptr [ecx + 4 * (1 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (2 * 5 + 0)] __asm fadd dword ptr [ecx + 4 * (2 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 1)] __asm fadd dword ptr [ecx + 4 * (2 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 2)] __asm fadd dword ptr [ecx + 4 * (2 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 3)] __asm fadd dword ptr [ecx + 4 * (2 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 4)] __asm fadd dword ptr [ecx + 4 * (2 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 4)]




























































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,3,5)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,3,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 5 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 1)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 2)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 3)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 4)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (1 * 5 + 0)] __asm fsub dword ptr [ecx + 4 * (1 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 1)] __asm fsub dword ptr [ecx + 4 * (1 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 2)] __asm fsub dword ptr [ecx + 4 * (1 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 3)] __asm fsub dword ptr [ecx + 4 * (1 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 4)] __asm fsub dword ptr [ecx + 4 * (1 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (2 * 5 + 0)] __asm fsub dword ptr [ecx + 4 * (2 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 1)] __asm fsub dword ptr [ecx + 4 * (2 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 2)] __asm fsub dword ptr [ecx + 4 * (2 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 3)] __asm fsub dword ptr [ecx + 4 * (2 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 4)] __asm fsub dword ptr [ecx + 4 * (2 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 4)]




























































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,3,5)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 5 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 1)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 2)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 3)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 4)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (1 * 5 + 0)] __asm fadd dword ptr [edx + 4 * (1 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 1)] __asm fadd dword ptr [edx + 4 * (1 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 2)] __asm fadd dword ptr [edx + 4 * (1 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 3)] __asm fadd dword ptr [edx + 4 * (1 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 4)] __asm fadd dword ptr [edx + 4 * (1 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (2 * 5 + 0)] __asm fadd dword ptr [edx + 4 * (2 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 1)] __asm fadd dword ptr [edx + 4 * (2 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 2)] __asm fadd dword ptr [edx + 4 * (2 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 3)] __asm fadd dword ptr [edx + 4 * (2 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 4)] __asm fadd dword ptr [edx + 4 * (2 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 4)]































































	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,3,5)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 5 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 1)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 2)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 3)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 4)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (1 * 5 + 0)] __asm fsub dword ptr [edx + 4 * (1 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 1)] __asm fsub dword ptr [edx + 4 * (1 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 2)] __asm fsub dword ptr [edx + 4 * (1 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 3)] __asm fsub dword ptr [edx + 4 * (1 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 4)] __asm fsub dword ptr [edx + 4 * (1 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (2 * 5 + 0)] __asm fsub dword ptr [edx + 4 * (2 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 1)] __asm fsub dword ptr [edx + 4 * (2 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 2)] __asm fsub dword ptr [edx + 4 * (2 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 3)] __asm fsub dword ptr [edx + 4 * (2 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 4)] __asm fsub dword ptr [edx + 4 * (2 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 4)]































































	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(Assign,3,5)(SMLXMatrix &A, const SMLXMatrix &B) 







{
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

























































	
	

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,3,5,5,6)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 6, 5, 6, 0, 0)


e5(0, 1, 6, 5, 6, 0, 0)


e5(0, 2, 6, 5, 6, 0, 0)


e5(0, 3, 6, 5, 6, 0, 0)


e5(0, 4, 6, 5, 6, 0, 0)


e5(0, 5, 6, 5, 6, 0, 0)


e5(1, 0, 6, 5, 6, 0, 0)


e5(1, 1, 6, 5, 6, 0, 0)


e5(1, 2, 6, 5, 6, 0, 0)


e5(1, 3, 6, 5, 6, 0, 0)


e5(1, 4, 6, 5, 6, 0, 0)


e5(1, 5, 6, 5, 6, 0, 0)


e5(2, 0, 6, 5, 6, 0, 0)


e5(2, 1, 6, 5, 6, 0, 0)


e5(2, 2, 6, 5, 6, 0, 0)


e5(2, 3, 6, 5, 6, 0, 0)


e5(2, 4, 6, 5, 6, 0, 0)


e5(2, 5, 6, 5, 6, 0, 0)

























































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,3,5,6,5)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 6, 5, 5, 0, 1)


e5(0, 1, 6, 5, 5, 0, 1)


e5(0, 2, 6, 5, 5, 0, 1)


e5(0, 3, 6, 5, 5, 0, 1)


e5(0, 4, 6, 5, 5, 0, 1)


e5(0, 5, 6, 5, 5, 0, 1)


e5(1, 0, 6, 5, 5, 0, 1)


e5(1, 1, 6, 5, 5, 0, 1)


e5(1, 2, 6, 5, 5, 0, 1)


e5(1, 3, 6, 5, 5, 0, 1)


e5(1, 4, 6, 5, 5, 0, 1)


e5(1, 5, 6, 5, 5, 0, 1)


e5(2, 0, 6, 5, 5, 0, 1)


e5(2, 1, 6, 5, 5, 0, 1)


e5(2, 2, 6, 5, 5, 0, 1)


e5(2, 3, 6, 5, 5, 0, 1)


e5(2, 4, 6, 5, 5, 0, 1)


e5(2, 5, 6, 5, 5, 0, 1)

























































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,3,5,3,6)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 6, 5, 6, 1, 0)


e3(0, 1, 6, 5, 6, 1, 0)


e3(0, 2, 6, 5, 6, 1, 0)


e3(0, 3, 6, 5, 6, 1, 0)


e3(0, 4, 6, 5, 6, 1, 0)


e3(0, 5, 6, 5, 6, 1, 0)


e3(1, 0, 6, 5, 6, 1, 0)


e3(1, 1, 6, 5, 6, 1, 0)


e3(1, 2, 6, 5, 6, 1, 0)


e3(1, 3, 6, 5, 6, 1, 0)


e3(1, 4, 6, 5, 6, 1, 0)


e3(1, 5, 6, 5, 6, 1, 0)


e3(2, 0, 6, 5, 6, 1, 0)


e3(2, 1, 6, 5, 6, 1, 0)


e3(2, 2, 6, 5, 6, 1, 0)


e3(2, 3, 6, 5, 6, 1, 0)


e3(2, 4, 6, 5, 6, 1, 0)


e3(2, 5, 6, 5, 6, 1, 0)


e3(3, 0, 6, 5, 6, 1, 0)


e3(3, 1, 6, 5, 6, 1, 0)


e3(3, 2, 6, 5, 6, 1, 0)


e3(3, 3, 6, 5, 6, 1, 0)


e3(3, 4, 6, 5, 6, 1, 0)


e3(3, 5, 6, 5, 6, 1, 0)


e3(4, 0, 6, 5, 6, 1, 0)


e3(4, 1, 6, 5, 6, 1, 0)


e3(4, 2, 6, 5, 6, 1, 0)


e3(4, 3, 6, 5, 6, 1, 0)


e3(4, 4, 6, 5, 6, 1, 0)


e3(4, 5, 6, 5, 6, 1, 0)





















	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,3,5,6,3)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 6, 5, 3, 1, 1)


e3(0, 1, 6, 5, 3, 1, 1)


e3(0, 2, 6, 5, 3, 1, 1)


e3(0, 3, 6, 5, 3, 1, 1)


e3(0, 4, 6, 5, 3, 1, 1)


e3(0, 5, 6, 5, 3, 1, 1)


e3(1, 0, 6, 5, 3, 1, 1)


e3(1, 1, 6, 5, 3, 1, 1)


e3(1, 2, 6, 5, 3, 1, 1)


e3(1, 3, 6, 5, 3, 1, 1)


e3(1, 4, 6, 5, 3, 1, 1)


e3(1, 5, 6, 5, 3, 1, 1)


e3(2, 0, 6, 5, 3, 1, 1)


e3(2, 1, 6, 5, 3, 1, 1)


e3(2, 2, 6, 5, 3, 1, 1)


e3(2, 3, 6, 5, 3, 1, 1)


e3(2, 4, 6, 5, 3, 1, 1)


e3(2, 5, 6, 5, 3, 1, 1)


e3(3, 0, 6, 5, 3, 1, 1)


e3(3, 1, 6, 5, 3, 1, 1)


e3(3, 2, 6, 5, 3, 1, 1)


e3(3, 3, 6, 5, 3, 1, 1)


e3(3, 4, 6, 5, 3, 1, 1)


e3(3, 5, 6, 5, 3, 1, 1)


e3(4, 0, 6, 5, 3, 1, 1)


e3(4, 1, 6, 5, 3, 1, 1)


e3(4, 2, 6, 5, 3, 1, 1)


e3(4, 3, 6, 5, 3, 1, 1)


e3(4, 4, 6, 5, 3, 1, 1)


e3(4, 5, 6, 5, 3, 1, 1)





















	__asm mov eax, edx __asm pop ebx __asm ret
}























































	
	




























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,3,5,5,5)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 5, 5, 5, 0, 0)


e5(0, 1, 5, 5, 5, 0, 0)


e5(0, 2, 5, 5, 5, 0, 0)


e5(0, 3, 5, 5, 5, 0, 0)


e5(0, 4, 5, 5, 5, 0, 0)





e5(1, 0, 5, 5, 5, 0, 0)


e5(1, 1, 5, 5, 5, 0, 0)


e5(1, 2, 5, 5, 5, 0, 0)


e5(1, 3, 5, 5, 5, 0, 0)


e5(1, 4, 5, 5, 5, 0, 0)





e5(2, 0, 5, 5, 5, 0, 0)


e5(2, 1, 5, 5, 5, 0, 0)


e5(2, 2, 5, 5, 5, 0, 0)


e5(2, 3, 5, 5, 5, 0, 0)


e5(2, 4, 5, 5, 5, 0, 0)




























































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,3,5,5,5)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 5, 5, 5, 0, 1)


e5(0, 1, 5, 5, 5, 0, 1)


e5(0, 2, 5, 5, 5, 0, 1)


e5(0, 3, 5, 5, 5, 0, 1)


e5(0, 4, 5, 5, 5, 0, 1)





e5(1, 0, 5, 5, 5, 0, 1)


e5(1, 1, 5, 5, 5, 0, 1)


e5(1, 2, 5, 5, 5, 0, 1)


e5(1, 3, 5, 5, 5, 0, 1)


e5(1, 4, 5, 5, 5, 0, 1)





e5(2, 0, 5, 5, 5, 0, 1)


e5(2, 1, 5, 5, 5, 0, 1)


e5(2, 2, 5, 5, 5, 0, 1)


e5(2, 3, 5, 5, 5, 0, 1)


e5(2, 4, 5, 5, 5, 0, 1)




























































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,3,5,3,5)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 5, 5, 5, 1, 0)


e3(0, 1, 5, 5, 5, 1, 0)


e3(0, 2, 5, 5, 5, 1, 0)


e3(0, 3, 5, 5, 5, 1, 0)


e3(0, 4, 5, 5, 5, 1, 0)





e3(1, 0, 5, 5, 5, 1, 0)


e3(1, 1, 5, 5, 5, 1, 0)


e3(1, 2, 5, 5, 5, 1, 0)


e3(1, 3, 5, 5, 5, 1, 0)


e3(1, 4, 5, 5, 5, 1, 0)





e3(2, 0, 5, 5, 5, 1, 0)


e3(2, 1, 5, 5, 5, 1, 0)


e3(2, 2, 5, 5, 5, 1, 0)


e3(2, 3, 5, 5, 5, 1, 0)


e3(2, 4, 5, 5, 5, 1, 0)





e3(3, 0, 5, 5, 5, 1, 0)


e3(3, 1, 5, 5, 5, 1, 0)


e3(3, 2, 5, 5, 5, 1, 0)


e3(3, 3, 5, 5, 5, 1, 0)


e3(3, 4, 5, 5, 5, 1, 0)





e3(4, 0, 5, 5, 5, 1, 0)


e3(4, 1, 5, 5, 5, 1, 0)


e3(4, 2, 5, 5, 5, 1, 0)


e3(4, 3, 5, 5, 5, 1, 0)


e3(4, 4, 5, 5, 5, 1, 0)
























	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,3,5,5,3)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 5, 5, 3, 1, 1)


e3(0, 1, 5, 5, 3, 1, 1)


e3(0, 2, 5, 5, 3, 1, 1)


e3(0, 3, 5, 5, 3, 1, 1)


e3(0, 4, 5, 5, 3, 1, 1)





e3(1, 0, 5, 5, 3, 1, 1)


e3(1, 1, 5, 5, 3, 1, 1)


e3(1, 2, 5, 5, 3, 1, 1)


e3(1, 3, 5, 5, 3, 1, 1)


e3(1, 4, 5, 5, 3, 1, 1)





e3(2, 0, 5, 5, 3, 1, 1)


e3(2, 1, 5, 5, 3, 1, 1)


e3(2, 2, 5, 5, 3, 1, 1)


e3(2, 3, 5, 5, 3, 1, 1)


e3(2, 4, 5, 5, 3, 1, 1)





e3(3, 0, 5, 5, 3, 1, 1)


e3(3, 1, 5, 5, 3, 1, 1)


e3(3, 2, 5, 5, 3, 1, 1)


e3(3, 3, 5, 5, 3, 1, 1)


e3(3, 4, 5, 5, 3, 1, 1)





e3(4, 0, 5, 5, 3, 1, 1)


e3(4, 1, 5, 5, 3, 1, 1)


e3(4, 2, 5, 5, 3, 1, 1)


e3(4, 3, 5, 5, 3, 1, 1)


e3(4, 4, 5, 5, 3, 1, 1)
























	__asm mov eax, edx __asm pop ebx __asm ret
}

















































	
	


































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,3,5,5,3)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 3, 5, 3, 0, 0)


e5(0, 1, 3, 5, 3, 0, 0)


e5(0, 2, 3, 5, 3, 0, 0)











e5(1, 0, 3, 5, 3, 0, 0)


e5(1, 1, 3, 5, 3, 0, 0)


e5(1, 2, 3, 5, 3, 0, 0)











e5(2, 0, 3, 5, 3, 0, 0)


e5(2, 1, 3, 5, 3, 0, 0)


e5(2, 2, 3, 5, 3, 0, 0)


































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,3,5,3,5)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 3, 5, 5, 0, 1)


e5(0, 1, 3, 5, 5, 0, 1)


e5(0, 2, 3, 5, 5, 0, 1)











e5(1, 0, 3, 5, 5, 0, 1)


e5(1, 1, 3, 5, 5, 0, 1)


e5(1, 2, 3, 5, 5, 0, 1)











e5(2, 0, 3, 5, 5, 0, 1)


e5(2, 1, 3, 5, 5, 0, 1)


e5(2, 2, 3, 5, 5, 0, 1)


































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,3,5,3,3)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 3, 5, 3, 1, 0)


e3(0, 1, 3, 5, 3, 1, 0)


e3(0, 2, 3, 5, 3, 1, 0)











e3(1, 0, 3, 5, 3, 1, 0)


e3(1, 1, 3, 5, 3, 1, 0)


e3(1, 2, 3, 5, 3, 1, 0)











e3(2, 0, 3, 5, 3, 1, 0)


e3(2, 1, 3, 5, 3, 1, 0)


e3(2, 2, 3, 5, 3, 1, 0)











e3(3, 0, 3, 5, 3, 1, 0)


e3(3, 1, 3, 5, 3, 1, 0)


e3(3, 2, 3, 5, 3, 1, 0)











e3(4, 0, 3, 5, 3, 1, 0)


e3(4, 1, 3, 5, 3, 1, 0)


e3(4, 2, 3, 5, 3, 1, 0)






























	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,3,5,3,3)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 3, 5, 3, 1, 1)


e3(0, 1, 3, 5, 3, 1, 1)


e3(0, 2, 3, 5, 3, 1, 1)











e3(1, 0, 3, 5, 3, 1, 1)


e3(1, 1, 3, 5, 3, 1, 1)


e3(1, 2, 3, 5, 3, 1, 1)











e3(2, 0, 3, 5, 3, 1, 1)


e3(2, 1, 3, 5, 3, 1, 1)


e3(2, 2, 3, 5, 3, 1, 1)











e3(3, 0, 3, 5, 3, 1, 1)


e3(3, 1, 3, 5, 3, 1, 1)


e3(3, 2, 3, 5, 3, 1, 1)











e3(4, 0, 3, 5, 3, 1, 1)


e3(4, 1, 3, 5, 3, 1, 1)


e3(4, 2, 3, 5, 3, 1, 1)






























	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,3,5,5,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 1, 5, 1, 0, 0)

















e5(1, 0, 1, 5, 1, 0, 0)

















e5(2, 0, 1, 5, 1, 0, 0)








































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,3,5,1,5)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 1, 5, 5, 0, 1)

















e5(1, 0, 1, 5, 5, 0, 1)

















e5(2, 0, 1, 5, 5, 0, 1)








































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,3,5,3,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 1, 5, 1, 1, 0)

















e3(1, 0, 1, 5, 1, 1, 0)

















e3(2, 0, 1, 5, 1, 1, 0)

















e3(3, 0, 1, 5, 1, 1, 0)

















e3(4, 0, 1, 5, 1, 1, 0)




































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,3,5,1,3)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 1, 5, 3, 1, 1)

















e3(1, 0, 1, 5, 3, 1, 1)

















e3(2, 0, 1, 5, 3, 1, 1)

















e3(3, 0, 1, 5, 3, 1, 1)

















e3(4, 0, 1, 5, 3, 1, 1)




































	__asm mov eax, edx __asm pop ebx __asm ret
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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 6 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 1)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 2)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 3)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 4)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 5)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 0)] __asm fadd dword ptr [ecx + 4 * (1 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 1)] __asm fadd dword ptr [ecx + 4 * (1 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 2)] __asm fadd dword ptr [ecx + 4 * (1 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 3)] __asm fadd dword ptr [ecx + 4 * (1 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 4)] __asm fadd dword ptr [ecx + 4 * (1 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 5)] __asm fadd dword ptr [ecx + 4 * (1 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 0)] __asm fadd dword ptr [ecx + 4 * (2 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 1)] __asm fadd dword ptr [ecx + 4 * (2 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 2)] __asm fadd dword ptr [ecx + 4 * (2 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 3)] __asm fadd dword ptr [ecx + 4 * (2 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 4)] __asm fadd dword ptr [ecx + 4 * (2 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 5)] __asm fadd dword ptr [ecx + 4 * (2 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 0)] __asm fadd dword ptr [ecx + 4 * (3 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 1)] __asm fadd dword ptr [ecx + 4 * (3 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 2)] __asm fadd dword ptr [ecx + 4 * (3 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 3)] __asm fadd dword ptr [ecx + 4 * (3 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 4)] __asm fadd dword ptr [ecx + 4 * (3 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 5)] __asm fadd dword ptr [ecx + 4 * (3 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 0)] __asm fadd dword ptr [ecx + 4 * (4 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 1)] __asm fadd dword ptr [ecx + 4 * (4 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 2)] __asm fadd dword ptr [ecx + 4 * (4 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 3)] __asm fadd dword ptr [ecx + 4 * (4 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 4)] __asm fadd dword ptr [ecx + 4 * (4 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 5)] __asm fadd dword ptr [ecx + 4 * (4 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 5)]





















	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,5,6)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,5,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 6 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 1)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 2)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 3)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 4)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 5)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 0)] __asm fsub dword ptr [ecx + 4 * (1 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 1)] __asm fsub dword ptr [ecx + 4 * (1 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 2)] __asm fsub dword ptr [ecx + 4 * (1 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 3)] __asm fsub dword ptr [ecx + 4 * (1 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 4)] __asm fsub dword ptr [ecx + 4 * (1 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 5)] __asm fsub dword ptr [ecx + 4 * (1 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 0)] __asm fsub dword ptr [ecx + 4 * (2 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 1)] __asm fsub dword ptr [ecx + 4 * (2 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 2)] __asm fsub dword ptr [ecx + 4 * (2 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 3)] __asm fsub dword ptr [ecx + 4 * (2 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 4)] __asm fsub dword ptr [ecx + 4 * (2 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 5)] __asm fsub dword ptr [ecx + 4 * (2 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 0)] __asm fsub dword ptr [ecx + 4 * (3 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 1)] __asm fsub dword ptr [ecx + 4 * (3 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 2)] __asm fsub dword ptr [ecx + 4 * (3 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 3)] __asm fsub dword ptr [ecx + 4 * (3 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 4)] __asm fsub dword ptr [ecx + 4 * (3 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 5)] __asm fsub dword ptr [ecx + 4 * (3 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 0)] __asm fsub dword ptr [ecx + 4 * (4 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 1)] __asm fsub dword ptr [ecx + 4 * (4 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 2)] __asm fsub dword ptr [ecx + 4 * (4 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 3)] __asm fsub dword ptr [ecx + 4 * (4 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 4)] __asm fsub dword ptr [ecx + 4 * (4 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 5)] __asm fsub dword ptr [ecx + 4 * (4 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 5)]





















	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,5,6)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 6 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 1)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 2)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 3)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 4)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 5)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 0)] __asm fadd dword ptr [edx + 4 * (1 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 1)] __asm fadd dword ptr [edx + 4 * (1 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 2)] __asm fadd dword ptr [edx + 4 * (1 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 3)] __asm fadd dword ptr [edx + 4 * (1 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 4)] __asm fadd dword ptr [edx + 4 * (1 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 5)] __asm fadd dword ptr [edx + 4 * (1 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 0)] __asm fadd dword ptr [edx + 4 * (2 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 1)] __asm fadd dword ptr [edx + 4 * (2 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 2)] __asm fadd dword ptr [edx + 4 * (2 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 3)] __asm fadd dword ptr [edx + 4 * (2 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 4)] __asm fadd dword ptr [edx + 4 * (2 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 5)] __asm fadd dword ptr [edx + 4 * (2 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 0)] __asm fadd dword ptr [edx + 4 * (3 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 1)] __asm fadd dword ptr [edx + 4 * (3 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 2)] __asm fadd dword ptr [edx + 4 * (3 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 3)] __asm fadd dword ptr [edx + 4 * (3 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 4)] __asm fadd dword ptr [edx + 4 * (3 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 5)] __asm fadd dword ptr [edx + 4 * (3 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 0)] __asm fadd dword ptr [edx + 4 * (4 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 1)] __asm fadd dword ptr [edx + 4 * (4 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 2)] __asm fadd dword ptr [edx + 4 * (4 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 3)] __asm fadd dword ptr [edx + 4 * (4 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 4)] __asm fadd dword ptr [edx + 4 * (4 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 5)] __asm fadd dword ptr [edx + 4 * (4 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 5)]
























	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,5,6)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 6 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 1)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 2)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 3)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 4)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 5)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 0)] __asm fsub dword ptr [edx + 4 * (1 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 1)] __asm fsub dword ptr [edx + 4 * (1 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 2)] __asm fsub dword ptr [edx + 4 * (1 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 3)] __asm fsub dword ptr [edx + 4 * (1 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 4)] __asm fsub dword ptr [edx + 4 * (1 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 5)] __asm fsub dword ptr [edx + 4 * (1 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 0)] __asm fsub dword ptr [edx + 4 * (2 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 1)] __asm fsub dword ptr [edx + 4 * (2 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 2)] __asm fsub dword ptr [edx + 4 * (2 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 3)] __asm fsub dword ptr [edx + 4 * (2 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 4)] __asm fsub dword ptr [edx + 4 * (2 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 5)] __asm fsub dword ptr [edx + 4 * (2 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 0)] __asm fsub dword ptr [edx + 4 * (3 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 1)] __asm fsub dword ptr [edx + 4 * (3 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 2)] __asm fsub dword ptr [edx + 4 * (3 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 3)] __asm fsub dword ptr [edx + 4 * (3 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 4)] __asm fsub dword ptr [edx + 4 * (3 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 5)] __asm fsub dword ptr [edx + 4 * (3 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 0)] __asm fsub dword ptr [edx + 4 * (4 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 1)] __asm fsub dword ptr [edx + 4 * (4 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 2)] __asm fsub dword ptr [edx + 4 * (4 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 3)] __asm fsub dword ptr [edx + 4 * (4 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 4)] __asm fsub dword ptr [edx + 4 * (4 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 5)] __asm fsub dword ptr [edx + 4 * (4 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 5)]
























	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(Assign,5,6)(SMLXMatrix &A, const SMLXMatrix &B) 







{
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

























































	
	

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,5,6,6,6)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 6, 6, 6, 0, 0)


e6(0, 1, 6, 6, 6, 0, 0)


e6(0, 2, 6, 6, 6, 0, 0)


e6(0, 3, 6, 6, 6, 0, 0)


e6(0, 4, 6, 6, 6, 0, 0)


e6(0, 5, 6, 6, 6, 0, 0)


e6(1, 0, 6, 6, 6, 0, 0)


e6(1, 1, 6, 6, 6, 0, 0)


e6(1, 2, 6, 6, 6, 0, 0)


e6(1, 3, 6, 6, 6, 0, 0)


e6(1, 4, 6, 6, 6, 0, 0)


e6(1, 5, 6, 6, 6, 0, 0)


e6(2, 0, 6, 6, 6, 0, 0)


e6(2, 1, 6, 6, 6, 0, 0)


e6(2, 2, 6, 6, 6, 0, 0)


e6(2, 3, 6, 6, 6, 0, 0)


e6(2, 4, 6, 6, 6, 0, 0)


e6(2, 5, 6, 6, 6, 0, 0)


e6(3, 0, 6, 6, 6, 0, 0)


e6(3, 1, 6, 6, 6, 0, 0)


e6(3, 2, 6, 6, 6, 0, 0)


e6(3, 3, 6, 6, 6, 0, 0)


e6(3, 4, 6, 6, 6, 0, 0)


e6(3, 5, 6, 6, 6, 0, 0)


e6(4, 0, 6, 6, 6, 0, 0)


e6(4, 1, 6, 6, 6, 0, 0)


e6(4, 2, 6, 6, 6, 0, 0)


e6(4, 3, 6, 6, 6, 0, 0)


e6(4, 4, 6, 6, 6, 0, 0)


e6(4, 5, 6, 6, 6, 0, 0)





















	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,5,6,6,6)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 6, 6, 6, 0, 1)


e6(0, 1, 6, 6, 6, 0, 1)


e6(0, 2, 6, 6, 6, 0, 1)


e6(0, 3, 6, 6, 6, 0, 1)


e6(0, 4, 6, 6, 6, 0, 1)


e6(0, 5, 6, 6, 6, 0, 1)


e6(1, 0, 6, 6, 6, 0, 1)


e6(1, 1, 6, 6, 6, 0, 1)


e6(1, 2, 6, 6, 6, 0, 1)


e6(1, 3, 6, 6, 6, 0, 1)


e6(1, 4, 6, 6, 6, 0, 1)


e6(1, 5, 6, 6, 6, 0, 1)


e6(2, 0, 6, 6, 6, 0, 1)


e6(2, 1, 6, 6, 6, 0, 1)


e6(2, 2, 6, 6, 6, 0, 1)


e6(2, 3, 6, 6, 6, 0, 1)


e6(2, 4, 6, 6, 6, 0, 1)


e6(2, 5, 6, 6, 6, 0, 1)


e6(3, 0, 6, 6, 6, 0, 1)


e6(3, 1, 6, 6, 6, 0, 1)


e6(3, 2, 6, 6, 6, 0, 1)


e6(3, 3, 6, 6, 6, 0, 1)


e6(3, 4, 6, 6, 6, 0, 1)


e6(3, 5, 6, 6, 6, 0, 1)


e6(4, 0, 6, 6, 6, 0, 1)


e6(4, 1, 6, 6, 6, 0, 1)


e6(4, 2, 6, 6, 6, 0, 1)


e6(4, 3, 6, 6, 6, 0, 1)


e6(4, 4, 6, 6, 6, 0, 1)


e6(4, 5, 6, 6, 6, 0, 1)





















	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,5,6,5,6)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 6, 6, 6, 1, 0)


e5(0, 1, 6, 6, 6, 1, 0)


e5(0, 2, 6, 6, 6, 1, 0)


e5(0, 3, 6, 6, 6, 1, 0)


e5(0, 4, 6, 6, 6, 1, 0)


e5(0, 5, 6, 6, 6, 1, 0)


e5(1, 0, 6, 6, 6, 1, 0)


e5(1, 1, 6, 6, 6, 1, 0)


e5(1, 2, 6, 6, 6, 1, 0)


e5(1, 3, 6, 6, 6, 1, 0)


e5(1, 4, 6, 6, 6, 1, 0)


e5(1, 5, 6, 6, 6, 1, 0)


e5(2, 0, 6, 6, 6, 1, 0)


e5(2, 1, 6, 6, 6, 1, 0)


e5(2, 2, 6, 6, 6, 1, 0)


e5(2, 3, 6, 6, 6, 1, 0)


e5(2, 4, 6, 6, 6, 1, 0)


e5(2, 5, 6, 6, 6, 1, 0)


e5(3, 0, 6, 6, 6, 1, 0)


e5(3, 1, 6, 6, 6, 1, 0)


e5(3, 2, 6, 6, 6, 1, 0)


e5(3, 3, 6, 6, 6, 1, 0)


e5(3, 4, 6, 6, 6, 1, 0)


e5(3, 5, 6, 6, 6, 1, 0)


e5(4, 0, 6, 6, 6, 1, 0)


e5(4, 1, 6, 6, 6, 1, 0)


e5(4, 2, 6, 6, 6, 1, 0)


e5(4, 3, 6, 6, 6, 1, 0)


e5(4, 4, 6, 6, 6, 1, 0)


e5(4, 5, 6, 6, 6, 1, 0)


e5(5, 0, 6, 6, 6, 1, 0)


e5(5, 1, 6, 6, 6, 1, 0)


e5(5, 2, 6, 6, 6, 1, 0)


e5(5, 3, 6, 6, 6, 1, 0)


e5(5, 4, 6, 6, 6, 1, 0)


e5(5, 5, 6, 6, 6, 1, 0)



	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,5,6,6,5)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 6, 6, 5, 1, 1)


e5(0, 1, 6, 6, 5, 1, 1)


e5(0, 2, 6, 6, 5, 1, 1)


e5(0, 3, 6, 6, 5, 1, 1)


e5(0, 4, 6, 6, 5, 1, 1)


e5(0, 5, 6, 6, 5, 1, 1)


e5(1, 0, 6, 6, 5, 1, 1)


e5(1, 1, 6, 6, 5, 1, 1)


e5(1, 2, 6, 6, 5, 1, 1)


e5(1, 3, 6, 6, 5, 1, 1)


e5(1, 4, 6, 6, 5, 1, 1)


e5(1, 5, 6, 6, 5, 1, 1)


e5(2, 0, 6, 6, 5, 1, 1)


e5(2, 1, 6, 6, 5, 1, 1)


e5(2, 2, 6, 6, 5, 1, 1)


e5(2, 3, 6, 6, 5, 1, 1)


e5(2, 4, 6, 6, 5, 1, 1)


e5(2, 5, 6, 6, 5, 1, 1)


e5(3, 0, 6, 6, 5, 1, 1)


e5(3, 1, 6, 6, 5, 1, 1)


e5(3, 2, 6, 6, 5, 1, 1)


e5(3, 3, 6, 6, 5, 1, 1)


e5(3, 4, 6, 6, 5, 1, 1)


e5(3, 5, 6, 6, 5, 1, 1)


e5(4, 0, 6, 6, 5, 1, 1)


e5(4, 1, 6, 6, 5, 1, 1)


e5(4, 2, 6, 6, 5, 1, 1)


e5(4, 3, 6, 6, 5, 1, 1)


e5(4, 4, 6, 6, 5, 1, 1)


e5(4, 5, 6, 6, 5, 1, 1)


e5(5, 0, 6, 6, 5, 1, 1)


e5(5, 1, 6, 6, 5, 1, 1)


e5(5, 2, 6, 6, 5, 1, 1)


e5(5, 3, 6, 6, 5, 1, 1)


e5(5, 4, 6, 6, 5, 1, 1)


e5(5, 5, 6, 6, 5, 1, 1)



	__asm mov eax, edx __asm pop ebx __asm ret
}























































	
	




























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,5,6,6,5)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 5, 6, 5, 0, 0)


e6(0, 1, 5, 6, 5, 0, 0)


e6(0, 2, 5, 6, 5, 0, 0)


e6(0, 3, 5, 6, 5, 0, 0)


e6(0, 4, 5, 6, 5, 0, 0)





e6(1, 0, 5, 6, 5, 0, 0)


e6(1, 1, 5, 6, 5, 0, 0)


e6(1, 2, 5, 6, 5, 0, 0)


e6(1, 3, 5, 6, 5, 0, 0)


e6(1, 4, 5, 6, 5, 0, 0)





e6(2, 0, 5, 6, 5, 0, 0)


e6(2, 1, 5, 6, 5, 0, 0)


e6(2, 2, 5, 6, 5, 0, 0)


e6(2, 3, 5, 6, 5, 0, 0)


e6(2, 4, 5, 6, 5, 0, 0)





e6(3, 0, 5, 6, 5, 0, 0)


e6(3, 1, 5, 6, 5, 0, 0)


e6(3, 2, 5, 6, 5, 0, 0)


e6(3, 3, 5, 6, 5, 0, 0)


e6(3, 4, 5, 6, 5, 0, 0)





e6(4, 0, 5, 6, 5, 0, 0)


e6(4, 1, 5, 6, 5, 0, 0)


e6(4, 2, 5, 6, 5, 0, 0)


e6(4, 3, 5, 6, 5, 0, 0)


e6(4, 4, 5, 6, 5, 0, 0)
























	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,5,6,5,6)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 5, 6, 6, 0, 1)


e6(0, 1, 5, 6, 6, 0, 1)


e6(0, 2, 5, 6, 6, 0, 1)


e6(0, 3, 5, 6, 6, 0, 1)


e6(0, 4, 5, 6, 6, 0, 1)





e6(1, 0, 5, 6, 6, 0, 1)


e6(1, 1, 5, 6, 6, 0, 1)


e6(1, 2, 5, 6, 6, 0, 1)


e6(1, 3, 5, 6, 6, 0, 1)


e6(1, 4, 5, 6, 6, 0, 1)





e6(2, 0, 5, 6, 6, 0, 1)


e6(2, 1, 5, 6, 6, 0, 1)


e6(2, 2, 5, 6, 6, 0, 1)


e6(2, 3, 5, 6, 6, 0, 1)


e6(2, 4, 5, 6, 6, 0, 1)





e6(3, 0, 5, 6, 6, 0, 1)


e6(3, 1, 5, 6, 6, 0, 1)


e6(3, 2, 5, 6, 6, 0, 1)


e6(3, 3, 5, 6, 6, 0, 1)


e6(3, 4, 5, 6, 6, 0, 1)





e6(4, 0, 5, 6, 6, 0, 1)


e6(4, 1, 5, 6, 6, 0, 1)


e6(4, 2, 5, 6, 6, 0, 1)


e6(4, 3, 5, 6, 6, 0, 1)


e6(4, 4, 5, 6, 6, 0, 1)
























	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,5,6,5,5)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 5, 6, 5, 1, 0)


e5(0, 1, 5, 6, 5, 1, 0)


e5(0, 2, 5, 6, 5, 1, 0)


e5(0, 3, 5, 6, 5, 1, 0)


e5(0, 4, 5, 6, 5, 1, 0)





e5(1, 0, 5, 6, 5, 1, 0)


e5(1, 1, 5, 6, 5, 1, 0)


e5(1, 2, 5, 6, 5, 1, 0)


e5(1, 3, 5, 6, 5, 1, 0)


e5(1, 4, 5, 6, 5, 1, 0)





e5(2, 0, 5, 6, 5, 1, 0)


e5(2, 1, 5, 6, 5, 1, 0)


e5(2, 2, 5, 6, 5, 1, 0)


e5(2, 3, 5, 6, 5, 1, 0)


e5(2, 4, 5, 6, 5, 1, 0)





e5(3, 0, 5, 6, 5, 1, 0)


e5(3, 1, 5, 6, 5, 1, 0)


e5(3, 2, 5, 6, 5, 1, 0)


e5(3, 3, 5, 6, 5, 1, 0)


e5(3, 4, 5, 6, 5, 1, 0)





e5(4, 0, 5, 6, 5, 1, 0)


e5(4, 1, 5, 6, 5, 1, 0)


e5(4, 2, 5, 6, 5, 1, 0)


e5(4, 3, 5, 6, 5, 1, 0)


e5(4, 4, 5, 6, 5, 1, 0)





e5(5, 0, 5, 6, 5, 1, 0)


e5(5, 1, 5, 6, 5, 1, 0)


e5(5, 2, 5, 6, 5, 1, 0)


e5(5, 3, 5, 6, 5, 1, 0)


e5(5, 4, 5, 6, 5, 1, 0)






	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,5,6,5,5)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 5, 6, 5, 1, 1)


e5(0, 1, 5, 6, 5, 1, 1)


e5(0, 2, 5, 6, 5, 1, 1)


e5(0, 3, 5, 6, 5, 1, 1)


e5(0, 4, 5, 6, 5, 1, 1)





e5(1, 0, 5, 6, 5, 1, 1)


e5(1, 1, 5, 6, 5, 1, 1)


e5(1, 2, 5, 6, 5, 1, 1)


e5(1, 3, 5, 6, 5, 1, 1)


e5(1, 4, 5, 6, 5, 1, 1)





e5(2, 0, 5, 6, 5, 1, 1)


e5(2, 1, 5, 6, 5, 1, 1)


e5(2, 2, 5, 6, 5, 1, 1)


e5(2, 3, 5, 6, 5, 1, 1)


e5(2, 4, 5, 6, 5, 1, 1)





e5(3, 0, 5, 6, 5, 1, 1)


e5(3, 1, 5, 6, 5, 1, 1)


e5(3, 2, 5, 6, 5, 1, 1)


e5(3, 3, 5, 6, 5, 1, 1)


e5(3, 4, 5, 6, 5, 1, 1)





e5(4, 0, 5, 6, 5, 1, 1)


e5(4, 1, 5, 6, 5, 1, 1)


e5(4, 2, 5, 6, 5, 1, 1)


e5(4, 3, 5, 6, 5, 1, 1)


e5(4, 4, 5, 6, 5, 1, 1)





e5(5, 0, 5, 6, 5, 1, 1)


e5(5, 1, 5, 6, 5, 1, 1)


e5(5, 2, 5, 6, 5, 1, 1)


e5(5, 3, 5, 6, 5, 1, 1)


e5(5, 4, 5, 6, 5, 1, 1)






	__asm mov eax, edx __asm pop ebx __asm ret
}

















































	
	


































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,5,6,6,3)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 3, 6, 3, 0, 0)


e6(0, 1, 3, 6, 3, 0, 0)


e6(0, 2, 3, 6, 3, 0, 0)











e6(1, 0, 3, 6, 3, 0, 0)


e6(1, 1, 3, 6, 3, 0, 0)


e6(1, 2, 3, 6, 3, 0, 0)











e6(2, 0, 3, 6, 3, 0, 0)


e6(2, 1, 3, 6, 3, 0, 0)


e6(2, 2, 3, 6, 3, 0, 0)











e6(3, 0, 3, 6, 3, 0, 0)


e6(3, 1, 3, 6, 3, 0, 0)


e6(3, 2, 3, 6, 3, 0, 0)











e6(4, 0, 3, 6, 3, 0, 0)


e6(4, 1, 3, 6, 3, 0, 0)


e6(4, 2, 3, 6, 3, 0, 0)






























	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,5,6,3,6)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 3, 6, 6, 0, 1)


e6(0, 1, 3, 6, 6, 0, 1)


e6(0, 2, 3, 6, 6, 0, 1)











e6(1, 0, 3, 6, 6, 0, 1)


e6(1, 1, 3, 6, 6, 0, 1)


e6(1, 2, 3, 6, 6, 0, 1)











e6(2, 0, 3, 6, 6, 0, 1)


e6(2, 1, 3, 6, 6, 0, 1)


e6(2, 2, 3, 6, 6, 0, 1)











e6(3, 0, 3, 6, 6, 0, 1)


e6(3, 1, 3, 6, 6, 0, 1)


e6(3, 2, 3, 6, 6, 0, 1)











e6(4, 0, 3, 6, 6, 0, 1)


e6(4, 1, 3, 6, 6, 0, 1)


e6(4, 2, 3, 6, 6, 0, 1)






























	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,5,6,5,3)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 3, 6, 3, 1, 0)


e5(0, 1, 3, 6, 3, 1, 0)


e5(0, 2, 3, 6, 3, 1, 0)











e5(1, 0, 3, 6, 3, 1, 0)


e5(1, 1, 3, 6, 3, 1, 0)


e5(1, 2, 3, 6, 3, 1, 0)











e5(2, 0, 3, 6, 3, 1, 0)


e5(2, 1, 3, 6, 3, 1, 0)


e5(2, 2, 3, 6, 3, 1, 0)











e5(3, 0, 3, 6, 3, 1, 0)


e5(3, 1, 3, 6, 3, 1, 0)


e5(3, 2, 3, 6, 3, 1, 0)











e5(4, 0, 3, 6, 3, 1, 0)


e5(4, 1, 3, 6, 3, 1, 0)


e5(4, 2, 3, 6, 3, 1, 0)











e5(5, 0, 3, 6, 3, 1, 0)


e5(5, 1, 3, 6, 3, 1, 0)


e5(5, 2, 3, 6, 3, 1, 0)












	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,5,6,3,5)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 3, 6, 5, 1, 1)


e5(0, 1, 3, 6, 5, 1, 1)


e5(0, 2, 3, 6, 5, 1, 1)











e5(1, 0, 3, 6, 5, 1, 1)


e5(1, 1, 3, 6, 5, 1, 1)


e5(1, 2, 3, 6, 5, 1, 1)











e5(2, 0, 3, 6, 5, 1, 1)


e5(2, 1, 3, 6, 5, 1, 1)


e5(2, 2, 3, 6, 5, 1, 1)











e5(3, 0, 3, 6, 5, 1, 1)


e5(3, 1, 3, 6, 5, 1, 1)


e5(3, 2, 3, 6, 5, 1, 1)











e5(4, 0, 3, 6, 5, 1, 1)


e5(4, 1, 3, 6, 5, 1, 1)


e5(4, 2, 3, 6, 5, 1, 1)











e5(5, 0, 3, 6, 5, 1, 1)


e5(5, 1, 3, 6, 5, 1, 1)


e5(5, 2, 3, 6, 5, 1, 1)












	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,5,6,6,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 1, 6, 1, 0, 0)

















e6(1, 0, 1, 6, 1, 0, 0)

















e6(2, 0, 1, 6, 1, 0, 0)

















e6(3, 0, 1, 6, 1, 0, 0)

















e6(4, 0, 1, 6, 1, 0, 0)




































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,5,6,1,6)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 1, 6, 6, 0, 1)

















e6(1, 0, 1, 6, 6, 0, 1)

















e6(2, 0, 1, 6, 6, 0, 1)

















e6(3, 0, 1, 6, 6, 0, 1)

















e6(4, 0, 1, 6, 6, 0, 1)




































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,5,6,5,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 1, 6, 1, 1, 0)

















e5(1, 0, 1, 6, 1, 1, 0)

















e5(2, 0, 1, 6, 1, 1, 0)

















e5(3, 0, 1, 6, 1, 1, 0)

















e5(4, 0, 1, 6, 1, 1, 0)

















e5(5, 0, 1, 6, 1, 1, 0)


















	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,5,6,1,5)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 1, 6, 5, 1, 1)

















e5(1, 0, 1, 6, 5, 1, 1)

















e5(2, 0, 1, 6, 5, 1, 1)

















e5(3, 0, 1, 6, 5, 1, 1)

















e5(4, 0, 1, 6, 5, 1, 1)

















e5(5, 0, 1, 6, 5, 1, 1)


















	__asm mov eax, edx __asm pop ebx __asm ret
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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 5 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 1)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 2)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 3)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 4)] __asm fadd dword ptr [ecx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (1 * 5 + 0)] __asm fadd dword ptr [ecx + 4 * (1 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 1)] __asm fadd dword ptr [ecx + 4 * (1 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 2)] __asm fadd dword ptr [ecx + 4 * (1 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 3)] __asm fadd dword ptr [ecx + 4 * (1 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 4)] __asm fadd dword ptr [ecx + 4 * (1 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (2 * 5 + 0)] __asm fadd dword ptr [ecx + 4 * (2 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 1)] __asm fadd dword ptr [ecx + 4 * (2 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 2)] __asm fadd dword ptr [ecx + 4 * (2 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 3)] __asm fadd dword ptr [ecx + 4 * (2 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 4)] __asm fadd dword ptr [ecx + 4 * (2 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (3 * 5 + 0)] __asm fadd dword ptr [ecx + 4 * (3 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (3 * 5 + 1)] __asm fadd dword ptr [ecx + 4 * (3 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (3 * 5 + 2)] __asm fadd dword ptr [ecx + 4 * (3 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (3 * 5 + 3)] __asm fadd dword ptr [ecx + 4 * (3 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (3 * 5 + 4)] __asm fadd dword ptr [ecx + 4 * (3 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (4 * 5 + 0)] __asm fadd dword ptr [ecx + 4 * (4 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (4 * 5 + 1)] __asm fadd dword ptr [ecx + 4 * (4 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (4 * 5 + 2)] __asm fadd dword ptr [ecx + 4 * (4 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (4 * 5 + 3)] __asm fadd dword ptr [ecx + 4 * (4 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (4 * 5 + 4)] __asm fadd dword ptr [ecx + 4 * (4 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (5 * 5 + 0)] __asm fadd dword ptr [ecx + 4 * (5 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (5 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (5 * 5 + 1)] __asm fadd dword ptr [ecx + 4 * (5 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (5 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (5 * 5 + 2)] __asm fadd dword ptr [ecx + 4 * (5 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (5 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (5 * 5 + 3)] __asm fadd dword ptr [ecx + 4 * (5 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (5 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (5 * 5 + 4)] __asm fadd dword ptr [ecx + 4 * (5 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (5 * 5 + 4)]






	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,6,5)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,6,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 5 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 1)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 2)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 3)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (0 * 5 + 4)] __asm fsub dword ptr [ecx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (0 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (1 * 5 + 0)] __asm fsub dword ptr [ecx + 4 * (1 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 1)] __asm fsub dword ptr [ecx + 4 * (1 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 2)] __asm fsub dword ptr [ecx + 4 * (1 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 3)] __asm fsub dword ptr [ecx + 4 * (1 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (1 * 5 + 4)] __asm fsub dword ptr [ecx + 4 * (1 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (1 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (2 * 5 + 0)] __asm fsub dword ptr [ecx + 4 * (2 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 1)] __asm fsub dword ptr [ecx + 4 * (2 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 2)] __asm fsub dword ptr [ecx + 4 * (2 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 3)] __asm fsub dword ptr [ecx + 4 * (2 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (2 * 5 + 4)] __asm fsub dword ptr [ecx + 4 * (2 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (2 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (3 * 5 + 0)] __asm fsub dword ptr [ecx + 4 * (3 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (3 * 5 + 1)] __asm fsub dword ptr [ecx + 4 * (3 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (3 * 5 + 2)] __asm fsub dword ptr [ecx + 4 * (3 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (3 * 5 + 3)] __asm fsub dword ptr [ecx + 4 * (3 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (3 * 5 + 4)] __asm fsub dword ptr [ecx + 4 * (3 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (3 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (4 * 5 + 0)] __asm fsub dword ptr [ecx + 4 * (4 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (4 * 5 + 1)] __asm fsub dword ptr [ecx + 4 * (4 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (4 * 5 + 2)] __asm fsub dword ptr [ecx + 4 * (4 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (4 * 5 + 3)] __asm fsub dword ptr [ecx + 4 * (4 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (4 * 5 + 4)] __asm fsub dword ptr [ecx + 4 * (4 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (4 * 5 + 4)]





__asm fld dword ptr [ebx + 4 * (5 * 5 + 0)] __asm fsub dword ptr [ecx + 4 * (5 * 5 + 0)] __asm fstp dword ptr [eax + 4 * (5 * 5 + 0)]


__asm fld dword ptr [ebx + 4 * (5 * 5 + 1)] __asm fsub dword ptr [ecx + 4 * (5 * 5 + 1)] __asm fstp dword ptr [eax + 4 * (5 * 5 + 1)]


__asm fld dword ptr [ebx + 4 * (5 * 5 + 2)] __asm fsub dword ptr [ecx + 4 * (5 * 5 + 2)] __asm fstp dword ptr [eax + 4 * (5 * 5 + 2)]


__asm fld dword ptr [ebx + 4 * (5 * 5 + 3)] __asm fsub dword ptr [ecx + 4 * (5 * 5 + 3)] __asm fstp dword ptr [eax + 4 * (5 * 5 + 3)]


__asm fld dword ptr [ebx + 4 * (5 * 5 + 4)] __asm fsub dword ptr [ecx + 4 * (5 * 5 + 4)] __asm fstp dword ptr [eax + 4 * (5 * 5 + 4)]






	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,6,5)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 5 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 1)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 2)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 3)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 4)] __asm fadd dword ptr [edx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (1 * 5 + 0)] __asm fadd dword ptr [edx + 4 * (1 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 1)] __asm fadd dword ptr [edx + 4 * (1 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 2)] __asm fadd dword ptr [edx + 4 * (1 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 3)] __asm fadd dword ptr [edx + 4 * (1 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 4)] __asm fadd dword ptr [edx + 4 * (1 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (2 * 5 + 0)] __asm fadd dword ptr [edx + 4 * (2 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 1)] __asm fadd dword ptr [edx + 4 * (2 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 2)] __asm fadd dword ptr [edx + 4 * (2 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 3)] __asm fadd dword ptr [edx + 4 * (2 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 4)] __asm fadd dword ptr [edx + 4 * (2 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (3 * 5 + 0)] __asm fadd dword ptr [edx + 4 * (3 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (3 * 5 + 1)] __asm fadd dword ptr [edx + 4 * (3 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (3 * 5 + 2)] __asm fadd dword ptr [edx + 4 * (3 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (3 * 5 + 3)] __asm fadd dword ptr [edx + 4 * (3 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (3 * 5 + 4)] __asm fadd dword ptr [edx + 4 * (3 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (4 * 5 + 0)] __asm fadd dword ptr [edx + 4 * (4 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (4 * 5 + 1)] __asm fadd dword ptr [edx + 4 * (4 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (4 * 5 + 2)] __asm fadd dword ptr [edx + 4 * (4 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (4 * 5 + 3)] __asm fadd dword ptr [edx + 4 * (4 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (4 * 5 + 4)] __asm fadd dword ptr [edx + 4 * (4 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (5 * 5 + 0)] __asm fadd dword ptr [edx + 4 * (5 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (5 * 5 + 1)] __asm fadd dword ptr [edx + 4 * (5 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (5 * 5 + 2)] __asm fadd dword ptr [edx + 4 * (5 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (5 * 5 + 3)] __asm fadd dword ptr [edx + 4 * (5 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (5 * 5 + 4)] __asm fadd dword ptr [edx + 4 * (5 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 4)]









	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,6,5)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 5 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 1)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 2)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 3)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (0 * 5 + 4)] __asm fsub dword ptr [edx + 4 * (0 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (1 * 5 + 0)] __asm fsub dword ptr [edx + 4 * (1 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 1)] __asm fsub dword ptr [edx + 4 * (1 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 2)] __asm fsub dword ptr [edx + 4 * (1 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 3)] __asm fsub dword ptr [edx + 4 * (1 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (1 * 5 + 4)] __asm fsub dword ptr [edx + 4 * (1 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (1 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (2 * 5 + 0)] __asm fsub dword ptr [edx + 4 * (2 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 1)] __asm fsub dword ptr [edx + 4 * (2 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 2)] __asm fsub dword ptr [edx + 4 * (2 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 3)] __asm fsub dword ptr [edx + 4 * (2 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (2 * 5 + 4)] __asm fsub dword ptr [edx + 4 * (2 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (2 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (3 * 5 + 0)] __asm fsub dword ptr [edx + 4 * (3 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (3 * 5 + 1)] __asm fsub dword ptr [edx + 4 * (3 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (3 * 5 + 2)] __asm fsub dword ptr [edx + 4 * (3 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (3 * 5 + 3)] __asm fsub dword ptr [edx + 4 * (3 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (3 * 5 + 4)] __asm fsub dword ptr [edx + 4 * (3 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (3 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (4 * 5 + 0)] __asm fsub dword ptr [edx + 4 * (4 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (4 * 5 + 1)] __asm fsub dword ptr [edx + 4 * (4 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (4 * 5 + 2)] __asm fsub dword ptr [edx + 4 * (4 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (4 * 5 + 3)] __asm fsub dword ptr [edx + 4 * (4 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (4 * 5 + 4)] __asm fsub dword ptr [edx + 4 * (4 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (4 * 5 + 4)]





__asm fld dword ptr [ecx + 4 * (5 * 5 + 0)] __asm fsub dword ptr [edx + 4 * (5 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 0)]


__asm fld dword ptr [ecx + 4 * (5 * 5 + 1)] __asm fsub dword ptr [edx + 4 * (5 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 1)]


__asm fld dword ptr [ecx + 4 * (5 * 5 + 2)] __asm fsub dword ptr [edx + 4 * (5 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 2)]


__asm fld dword ptr [ecx + 4 * (5 * 5 + 3)] __asm fsub dword ptr [edx + 4 * (5 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 3)]


__asm fld dword ptr [ecx + 4 * (5 * 5 + 4)] __asm fsub dword ptr [edx + 4 * (5 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 4)]









	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(Assign,6,5)(SMLXMatrix &A, const SMLXMatrix &B) 







{
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





__asm fld dword ptr [edx + 4 * (5 * 5 + 0)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 0)]


__asm fld dword ptr [edx + 4 * (5 * 5 + 1)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 1)]


__asm fld dword ptr [edx + 4 * (5 * 5 + 2)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 2)]


__asm fld dword ptr [edx + 4 * (5 * 5 + 3)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 3)]


__asm fld dword ptr [edx + 4 * (5 * 5 + 4)] __asm fstp dword ptr [ecx + 4 * (5 * 5 + 4)]






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

























































	
	

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,6,5,5,6)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 6, 5, 6, 0, 0)


e5(0, 1, 6, 5, 6, 0, 0)


e5(0, 2, 6, 5, 6, 0, 0)


e5(0, 3, 6, 5, 6, 0, 0)


e5(0, 4, 6, 5, 6, 0, 0)


e5(0, 5, 6, 5, 6, 0, 0)


e5(1, 0, 6, 5, 6, 0, 0)


e5(1, 1, 6, 5, 6, 0, 0)


e5(1, 2, 6, 5, 6, 0, 0)


e5(1, 3, 6, 5, 6, 0, 0)


e5(1, 4, 6, 5, 6, 0, 0)


e5(1, 5, 6, 5, 6, 0, 0)


e5(2, 0, 6, 5, 6, 0, 0)


e5(2, 1, 6, 5, 6, 0, 0)


e5(2, 2, 6, 5, 6, 0, 0)


e5(2, 3, 6, 5, 6, 0, 0)


e5(2, 4, 6, 5, 6, 0, 0)


e5(2, 5, 6, 5, 6, 0, 0)


e5(3, 0, 6, 5, 6, 0, 0)


e5(3, 1, 6, 5, 6, 0, 0)


e5(3, 2, 6, 5, 6, 0, 0)


e5(3, 3, 6, 5, 6, 0, 0)


e5(3, 4, 6, 5, 6, 0, 0)


e5(3, 5, 6, 5, 6, 0, 0)


e5(4, 0, 6, 5, 6, 0, 0)


e5(4, 1, 6, 5, 6, 0, 0)


e5(4, 2, 6, 5, 6, 0, 0)


e5(4, 3, 6, 5, 6, 0, 0)


e5(4, 4, 6, 5, 6, 0, 0)


e5(4, 5, 6, 5, 6, 0, 0)


e5(5, 0, 6, 5, 6, 0, 0)


e5(5, 1, 6, 5, 6, 0, 0)


e5(5, 2, 6, 5, 6, 0, 0)


e5(5, 3, 6, 5, 6, 0, 0)


e5(5, 4, 6, 5, 6, 0, 0)


e5(5, 5, 6, 5, 6, 0, 0)



	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,6,5,6,5)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 6, 5, 5, 0, 1)


e5(0, 1, 6, 5, 5, 0, 1)


e5(0, 2, 6, 5, 5, 0, 1)


e5(0, 3, 6, 5, 5, 0, 1)


e5(0, 4, 6, 5, 5, 0, 1)


e5(0, 5, 6, 5, 5, 0, 1)


e5(1, 0, 6, 5, 5, 0, 1)


e5(1, 1, 6, 5, 5, 0, 1)


e5(1, 2, 6, 5, 5, 0, 1)


e5(1, 3, 6, 5, 5, 0, 1)


e5(1, 4, 6, 5, 5, 0, 1)


e5(1, 5, 6, 5, 5, 0, 1)


e5(2, 0, 6, 5, 5, 0, 1)


e5(2, 1, 6, 5, 5, 0, 1)


e5(2, 2, 6, 5, 5, 0, 1)


e5(2, 3, 6, 5, 5, 0, 1)


e5(2, 4, 6, 5, 5, 0, 1)


e5(2, 5, 6, 5, 5, 0, 1)


e5(3, 0, 6, 5, 5, 0, 1)


e5(3, 1, 6, 5, 5, 0, 1)


e5(3, 2, 6, 5, 5, 0, 1)


e5(3, 3, 6, 5, 5, 0, 1)


e5(3, 4, 6, 5, 5, 0, 1)


e5(3, 5, 6, 5, 5, 0, 1)


e5(4, 0, 6, 5, 5, 0, 1)


e5(4, 1, 6, 5, 5, 0, 1)


e5(4, 2, 6, 5, 5, 0, 1)


e5(4, 3, 6, 5, 5, 0, 1)


e5(4, 4, 6, 5, 5, 0, 1)


e5(4, 5, 6, 5, 5, 0, 1)


e5(5, 0, 6, 5, 5, 0, 1)


e5(5, 1, 6, 5, 5, 0, 1)


e5(5, 2, 6, 5, 5, 0, 1)


e5(5, 3, 6, 5, 5, 0, 1)


e5(5, 4, 6, 5, 5, 0, 1)


e5(5, 5, 6, 5, 5, 0, 1)



	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,6,5,6,6)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 6, 5, 6, 1, 0)


e6(0, 1, 6, 5, 6, 1, 0)


e6(0, 2, 6, 5, 6, 1, 0)


e6(0, 3, 6, 5, 6, 1, 0)


e6(0, 4, 6, 5, 6, 1, 0)


e6(0, 5, 6, 5, 6, 1, 0)


e6(1, 0, 6, 5, 6, 1, 0)


e6(1, 1, 6, 5, 6, 1, 0)


e6(1, 2, 6, 5, 6, 1, 0)


e6(1, 3, 6, 5, 6, 1, 0)


e6(1, 4, 6, 5, 6, 1, 0)


e6(1, 5, 6, 5, 6, 1, 0)


e6(2, 0, 6, 5, 6, 1, 0)


e6(2, 1, 6, 5, 6, 1, 0)


e6(2, 2, 6, 5, 6, 1, 0)


e6(2, 3, 6, 5, 6, 1, 0)


e6(2, 4, 6, 5, 6, 1, 0)


e6(2, 5, 6, 5, 6, 1, 0)


e6(3, 0, 6, 5, 6, 1, 0)


e6(3, 1, 6, 5, 6, 1, 0)


e6(3, 2, 6, 5, 6, 1, 0)


e6(3, 3, 6, 5, 6, 1, 0)


e6(3, 4, 6, 5, 6, 1, 0)


e6(3, 5, 6, 5, 6, 1, 0)


e6(4, 0, 6, 5, 6, 1, 0)


e6(4, 1, 6, 5, 6, 1, 0)


e6(4, 2, 6, 5, 6, 1, 0)


e6(4, 3, 6, 5, 6, 1, 0)


e6(4, 4, 6, 5, 6, 1, 0)


e6(4, 5, 6, 5, 6, 1, 0)





















	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,6,5,6,6)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 6, 5, 6, 1, 1)


e6(0, 1, 6, 5, 6, 1, 1)


e6(0, 2, 6, 5, 6, 1, 1)


e6(0, 3, 6, 5, 6, 1, 1)


e6(0, 4, 6, 5, 6, 1, 1)


e6(0, 5, 6, 5, 6, 1, 1)


e6(1, 0, 6, 5, 6, 1, 1)


e6(1, 1, 6, 5, 6, 1, 1)


e6(1, 2, 6, 5, 6, 1, 1)


e6(1, 3, 6, 5, 6, 1, 1)


e6(1, 4, 6, 5, 6, 1, 1)


e6(1, 5, 6, 5, 6, 1, 1)


e6(2, 0, 6, 5, 6, 1, 1)


e6(2, 1, 6, 5, 6, 1, 1)


e6(2, 2, 6, 5, 6, 1, 1)


e6(2, 3, 6, 5, 6, 1, 1)


e6(2, 4, 6, 5, 6, 1, 1)


e6(2, 5, 6, 5, 6, 1, 1)


e6(3, 0, 6, 5, 6, 1, 1)


e6(3, 1, 6, 5, 6, 1, 1)


e6(3, 2, 6, 5, 6, 1, 1)


e6(3, 3, 6, 5, 6, 1, 1)


e6(3, 4, 6, 5, 6, 1, 1)


e6(3, 5, 6, 5, 6, 1, 1)


e6(4, 0, 6, 5, 6, 1, 1)


e6(4, 1, 6, 5, 6, 1, 1)


e6(4, 2, 6, 5, 6, 1, 1)


e6(4, 3, 6, 5, 6, 1, 1)


e6(4, 4, 6, 5, 6, 1, 1)


e6(4, 5, 6, 5, 6, 1, 1)





















	__asm mov eax, edx __asm pop ebx __asm ret
}























































	
	




























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,6,5,5,5)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 5, 5, 5, 0, 0)


e5(0, 1, 5, 5, 5, 0, 0)


e5(0, 2, 5, 5, 5, 0, 0)


e5(0, 3, 5, 5, 5, 0, 0)


e5(0, 4, 5, 5, 5, 0, 0)





e5(1, 0, 5, 5, 5, 0, 0)


e5(1, 1, 5, 5, 5, 0, 0)


e5(1, 2, 5, 5, 5, 0, 0)


e5(1, 3, 5, 5, 5, 0, 0)


e5(1, 4, 5, 5, 5, 0, 0)





e5(2, 0, 5, 5, 5, 0, 0)


e5(2, 1, 5, 5, 5, 0, 0)


e5(2, 2, 5, 5, 5, 0, 0)


e5(2, 3, 5, 5, 5, 0, 0)


e5(2, 4, 5, 5, 5, 0, 0)





e5(3, 0, 5, 5, 5, 0, 0)


e5(3, 1, 5, 5, 5, 0, 0)


e5(3, 2, 5, 5, 5, 0, 0)


e5(3, 3, 5, 5, 5, 0, 0)


e5(3, 4, 5, 5, 5, 0, 0)





e5(4, 0, 5, 5, 5, 0, 0)


e5(4, 1, 5, 5, 5, 0, 0)


e5(4, 2, 5, 5, 5, 0, 0)


e5(4, 3, 5, 5, 5, 0, 0)


e5(4, 4, 5, 5, 5, 0, 0)





e5(5, 0, 5, 5, 5, 0, 0)


e5(5, 1, 5, 5, 5, 0, 0)


e5(5, 2, 5, 5, 5, 0, 0)


e5(5, 3, 5, 5, 5, 0, 0)


e5(5, 4, 5, 5, 5, 0, 0)






	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,6,5,5,5)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 5, 5, 5, 0, 1)


e5(0, 1, 5, 5, 5, 0, 1)


e5(0, 2, 5, 5, 5, 0, 1)


e5(0, 3, 5, 5, 5, 0, 1)


e5(0, 4, 5, 5, 5, 0, 1)





e5(1, 0, 5, 5, 5, 0, 1)


e5(1, 1, 5, 5, 5, 0, 1)


e5(1, 2, 5, 5, 5, 0, 1)


e5(1, 3, 5, 5, 5, 0, 1)


e5(1, 4, 5, 5, 5, 0, 1)





e5(2, 0, 5, 5, 5, 0, 1)


e5(2, 1, 5, 5, 5, 0, 1)


e5(2, 2, 5, 5, 5, 0, 1)


e5(2, 3, 5, 5, 5, 0, 1)


e5(2, 4, 5, 5, 5, 0, 1)





e5(3, 0, 5, 5, 5, 0, 1)


e5(3, 1, 5, 5, 5, 0, 1)


e5(3, 2, 5, 5, 5, 0, 1)


e5(3, 3, 5, 5, 5, 0, 1)


e5(3, 4, 5, 5, 5, 0, 1)





e5(4, 0, 5, 5, 5, 0, 1)


e5(4, 1, 5, 5, 5, 0, 1)


e5(4, 2, 5, 5, 5, 0, 1)


e5(4, 3, 5, 5, 5, 0, 1)


e5(4, 4, 5, 5, 5, 0, 1)





e5(5, 0, 5, 5, 5, 0, 1)


e5(5, 1, 5, 5, 5, 0, 1)


e5(5, 2, 5, 5, 5, 0, 1)


e5(5, 3, 5, 5, 5, 0, 1)


e5(5, 4, 5, 5, 5, 0, 1)






	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,6,5,6,5)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 5, 5, 5, 1, 0)


e6(0, 1, 5, 5, 5, 1, 0)


e6(0, 2, 5, 5, 5, 1, 0)


e6(0, 3, 5, 5, 5, 1, 0)


e6(0, 4, 5, 5, 5, 1, 0)





e6(1, 0, 5, 5, 5, 1, 0)


e6(1, 1, 5, 5, 5, 1, 0)


e6(1, 2, 5, 5, 5, 1, 0)


e6(1, 3, 5, 5, 5, 1, 0)


e6(1, 4, 5, 5, 5, 1, 0)





e6(2, 0, 5, 5, 5, 1, 0)


e6(2, 1, 5, 5, 5, 1, 0)


e6(2, 2, 5, 5, 5, 1, 0)


e6(2, 3, 5, 5, 5, 1, 0)


e6(2, 4, 5, 5, 5, 1, 0)





e6(3, 0, 5, 5, 5, 1, 0)


e6(3, 1, 5, 5, 5, 1, 0)


e6(3, 2, 5, 5, 5, 1, 0)


e6(3, 3, 5, 5, 5, 1, 0)


e6(3, 4, 5, 5, 5, 1, 0)





e6(4, 0, 5, 5, 5, 1, 0)


e6(4, 1, 5, 5, 5, 1, 0)


e6(4, 2, 5, 5, 5, 1, 0)


e6(4, 3, 5, 5, 5, 1, 0)


e6(4, 4, 5, 5, 5, 1, 0)
























	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,6,5,5,6)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 5, 5, 6, 1, 1)


e6(0, 1, 5, 5, 6, 1, 1)


e6(0, 2, 5, 5, 6, 1, 1)


e6(0, 3, 5, 5, 6, 1, 1)


e6(0, 4, 5, 5, 6, 1, 1)





e6(1, 0, 5, 5, 6, 1, 1)


e6(1, 1, 5, 5, 6, 1, 1)


e6(1, 2, 5, 5, 6, 1, 1)


e6(1, 3, 5, 5, 6, 1, 1)


e6(1, 4, 5, 5, 6, 1, 1)





e6(2, 0, 5, 5, 6, 1, 1)


e6(2, 1, 5, 5, 6, 1, 1)


e6(2, 2, 5, 5, 6, 1, 1)


e6(2, 3, 5, 5, 6, 1, 1)


e6(2, 4, 5, 5, 6, 1, 1)





e6(3, 0, 5, 5, 6, 1, 1)


e6(3, 1, 5, 5, 6, 1, 1)


e6(3, 2, 5, 5, 6, 1, 1)


e6(3, 3, 5, 5, 6, 1, 1)


e6(3, 4, 5, 5, 6, 1, 1)





e6(4, 0, 5, 5, 6, 1, 1)


e6(4, 1, 5, 5, 6, 1, 1)


e6(4, 2, 5, 5, 6, 1, 1)


e6(4, 3, 5, 5, 6, 1, 1)


e6(4, 4, 5, 5, 6, 1, 1)
























	__asm mov eax, edx __asm pop ebx __asm ret
}

















































	
	


































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,6,5,5,3)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 3, 5, 3, 0, 0)


e5(0, 1, 3, 5, 3, 0, 0)


e5(0, 2, 3, 5, 3, 0, 0)











e5(1, 0, 3, 5, 3, 0, 0)


e5(1, 1, 3, 5, 3, 0, 0)


e5(1, 2, 3, 5, 3, 0, 0)











e5(2, 0, 3, 5, 3, 0, 0)


e5(2, 1, 3, 5, 3, 0, 0)


e5(2, 2, 3, 5, 3, 0, 0)











e5(3, 0, 3, 5, 3, 0, 0)


e5(3, 1, 3, 5, 3, 0, 0)


e5(3, 2, 3, 5, 3, 0, 0)











e5(4, 0, 3, 5, 3, 0, 0)


e5(4, 1, 3, 5, 3, 0, 0)


e5(4, 2, 3, 5, 3, 0, 0)











e5(5, 0, 3, 5, 3, 0, 0)


e5(5, 1, 3, 5, 3, 0, 0)


e5(5, 2, 3, 5, 3, 0, 0)












	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,6,5,3,5)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 3, 5, 5, 0, 1)


e5(0, 1, 3, 5, 5, 0, 1)


e5(0, 2, 3, 5, 5, 0, 1)











e5(1, 0, 3, 5, 5, 0, 1)


e5(1, 1, 3, 5, 5, 0, 1)


e5(1, 2, 3, 5, 5, 0, 1)











e5(2, 0, 3, 5, 5, 0, 1)


e5(2, 1, 3, 5, 5, 0, 1)


e5(2, 2, 3, 5, 5, 0, 1)











e5(3, 0, 3, 5, 5, 0, 1)


e5(3, 1, 3, 5, 5, 0, 1)


e5(3, 2, 3, 5, 5, 0, 1)











e5(4, 0, 3, 5, 5, 0, 1)


e5(4, 1, 3, 5, 5, 0, 1)


e5(4, 2, 3, 5, 5, 0, 1)











e5(5, 0, 3, 5, 5, 0, 1)


e5(5, 1, 3, 5, 5, 0, 1)


e5(5, 2, 3, 5, 5, 0, 1)












	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,6,5,6,3)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 3, 5, 3, 1, 0)


e6(0, 1, 3, 5, 3, 1, 0)


e6(0, 2, 3, 5, 3, 1, 0)











e6(1, 0, 3, 5, 3, 1, 0)


e6(1, 1, 3, 5, 3, 1, 0)


e6(1, 2, 3, 5, 3, 1, 0)











e6(2, 0, 3, 5, 3, 1, 0)


e6(2, 1, 3, 5, 3, 1, 0)


e6(2, 2, 3, 5, 3, 1, 0)











e6(3, 0, 3, 5, 3, 1, 0)


e6(3, 1, 3, 5, 3, 1, 0)


e6(3, 2, 3, 5, 3, 1, 0)











e6(4, 0, 3, 5, 3, 1, 0)


e6(4, 1, 3, 5, 3, 1, 0)


e6(4, 2, 3, 5, 3, 1, 0)






























	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,6,5,3,6)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 3, 5, 6, 1, 1)


e6(0, 1, 3, 5, 6, 1, 1)


e6(0, 2, 3, 5, 6, 1, 1)











e6(1, 0, 3, 5, 6, 1, 1)


e6(1, 1, 3, 5, 6, 1, 1)


e6(1, 2, 3, 5, 6, 1, 1)











e6(2, 0, 3, 5, 6, 1, 1)


e6(2, 1, 3, 5, 6, 1, 1)


e6(2, 2, 3, 5, 6, 1, 1)











e6(3, 0, 3, 5, 6, 1, 1)


e6(3, 1, 3, 5, 6, 1, 1)


e6(3, 2, 3, 5, 6, 1, 1)











e6(4, 0, 3, 5, 6, 1, 1)


e6(4, 1, 3, 5, 6, 1, 1)


e6(4, 2, 3, 5, 6, 1, 1)






























	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,6,5,5,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 1, 5, 1, 0, 0)

















e5(1, 0, 1, 5, 1, 0, 0)

















e5(2, 0, 1, 5, 1, 0, 0)

















e5(3, 0, 1, 5, 1, 0, 0)

















e5(4, 0, 1, 5, 1, 0, 0)

















e5(5, 0, 1, 5, 1, 0, 0)


















	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,6,5,1,5)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e5(0, 0, 1, 5, 5, 0, 1)

















e5(1, 0, 1, 5, 5, 0, 1)

















e5(2, 0, 1, 5, 5, 0, 1)

















e5(3, 0, 1, 5, 5, 0, 1)

















e5(4, 0, 1, 5, 5, 0, 1)

















e5(5, 0, 1, 5, 5, 0, 1)


















	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,6,5,6,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,5,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 1, 5, 1, 1, 0)

















e6(1, 0, 1, 5, 1, 1, 0)

















e6(2, 0, 1, 5, 1, 1, 0)

















e6(3, 0, 1, 5, 1, 1, 0)

















e6(4, 0, 1, 5, 1, 1, 0)




































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,6,5,1,6)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,5,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 1, 5, 6, 1, 1)

















e6(1, 0, 1, 5, 6, 1, 1)

















e6(2, 0, 1, 5, 6, 1, 1)

















e6(3, 0, 1, 5, 6, 1, 1)

















e6(4, 0, 1, 5, 6, 1, 1)




































	__asm mov eax, edx __asm pop ebx __asm ret
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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 6 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 1)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 2)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 3)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 4)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 5)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 0)] __asm fadd dword ptr [ecx + 4 * (1 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 1)] __asm fadd dword ptr [ecx + 4 * (1 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 2)] __asm fadd dword ptr [ecx + 4 * (1 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 3)] __asm fadd dword ptr [ecx + 4 * (1 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 4)] __asm fadd dword ptr [ecx + 4 * (1 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 5)] __asm fadd dword ptr [ecx + 4 * (1 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 0)] __asm fadd dword ptr [ecx + 4 * (2 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 1)] __asm fadd dword ptr [ecx + 4 * (2 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 2)] __asm fadd dword ptr [ecx + 4 * (2 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 3)] __asm fadd dword ptr [ecx + 4 * (2 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 4)] __asm fadd dword ptr [ecx + 4 * (2 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 5)] __asm fadd dword ptr [ecx + 4 * (2 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 0)] __asm fadd dword ptr [ecx + 4 * (3 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 1)] __asm fadd dword ptr [ecx + 4 * (3 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 2)] __asm fadd dword ptr [ecx + 4 * (3 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 3)] __asm fadd dword ptr [ecx + 4 * (3 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 4)] __asm fadd dword ptr [ecx + 4 * (3 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 5)] __asm fadd dword ptr [ecx + 4 * (3 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 0)] __asm fadd dword ptr [ecx + 4 * (4 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 1)] __asm fadd dword ptr [ecx + 4 * (4 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 2)] __asm fadd dword ptr [ecx + 4 * (4 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 3)] __asm fadd dword ptr [ecx + 4 * (4 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 4)] __asm fadd dword ptr [ecx + 4 * (4 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 5)] __asm fadd dword ptr [ecx + 4 * (4 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (5 * 6 + 0)] __asm fadd dword ptr [ecx + 4 * (5 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (5 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (5 * 6 + 1)] __asm fadd dword ptr [ecx + 4 * (5 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (5 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (5 * 6 + 2)] __asm fadd dword ptr [ecx + 4 * (5 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (5 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (5 * 6 + 3)] __asm fadd dword ptr [ecx + 4 * (5 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (5 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (5 * 6 + 4)] __asm fadd dword ptr [ecx + 4 * (5 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (5 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (5 * 6 + 5)] __asm fadd dword ptr [ecx + 4 * (5 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (5 * 6 + 5)]



	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,6,6)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,6,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 6 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 1)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 2)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 3)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 4)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 5)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 0)] __asm fsub dword ptr [ecx + 4 * (1 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 1)] __asm fsub dword ptr [ecx + 4 * (1 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 2)] __asm fsub dword ptr [ecx + 4 * (1 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 3)] __asm fsub dword ptr [ecx + 4 * (1 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 4)] __asm fsub dword ptr [ecx + 4 * (1 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 5)] __asm fsub dword ptr [ecx + 4 * (1 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 0)] __asm fsub dword ptr [ecx + 4 * (2 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 1)] __asm fsub dword ptr [ecx + 4 * (2 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 2)] __asm fsub dword ptr [ecx + 4 * (2 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 3)] __asm fsub dword ptr [ecx + 4 * (2 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 4)] __asm fsub dword ptr [ecx + 4 * (2 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 5)] __asm fsub dword ptr [ecx + 4 * (2 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 0)] __asm fsub dword ptr [ecx + 4 * (3 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 1)] __asm fsub dword ptr [ecx + 4 * (3 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 2)] __asm fsub dword ptr [ecx + 4 * (3 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 3)] __asm fsub dword ptr [ecx + 4 * (3 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 4)] __asm fsub dword ptr [ecx + 4 * (3 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (3 * 6 + 5)] __asm fsub dword ptr [ecx + 4 * (3 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (3 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 0)] __asm fsub dword ptr [ecx + 4 * (4 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 1)] __asm fsub dword ptr [ecx + 4 * (4 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 2)] __asm fsub dword ptr [ecx + 4 * (4 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 3)] __asm fsub dword ptr [ecx + 4 * (4 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 4)] __asm fsub dword ptr [ecx + 4 * (4 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (4 * 6 + 5)] __asm fsub dword ptr [ecx + 4 * (4 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (4 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (5 * 6 + 0)] __asm fsub dword ptr [ecx + 4 * (5 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (5 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (5 * 6 + 1)] __asm fsub dword ptr [ecx + 4 * (5 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (5 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (5 * 6 + 2)] __asm fsub dword ptr [ecx + 4 * (5 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (5 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (5 * 6 + 3)] __asm fsub dword ptr [ecx + 4 * (5 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (5 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (5 * 6 + 4)] __asm fsub dword ptr [ecx + 4 * (5 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (5 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (5 * 6 + 5)] __asm fsub dword ptr [ecx + 4 * (5 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (5 * 6 + 5)]



	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,6,6)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 6 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 1)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 2)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 3)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 4)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 5)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 0)] __asm fadd dword ptr [edx + 4 * (1 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 1)] __asm fadd dword ptr [edx + 4 * (1 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 2)] __asm fadd dword ptr [edx + 4 * (1 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 3)] __asm fadd dword ptr [edx + 4 * (1 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 4)] __asm fadd dword ptr [edx + 4 * (1 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 5)] __asm fadd dword ptr [edx + 4 * (1 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 0)] __asm fadd dword ptr [edx + 4 * (2 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 1)] __asm fadd dword ptr [edx + 4 * (2 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 2)] __asm fadd dword ptr [edx + 4 * (2 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 3)] __asm fadd dword ptr [edx + 4 * (2 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 4)] __asm fadd dword ptr [edx + 4 * (2 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 5)] __asm fadd dword ptr [edx + 4 * (2 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 0)] __asm fadd dword ptr [edx + 4 * (3 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 1)] __asm fadd dword ptr [edx + 4 * (3 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 2)] __asm fadd dword ptr [edx + 4 * (3 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 3)] __asm fadd dword ptr [edx + 4 * (3 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 4)] __asm fadd dword ptr [edx + 4 * (3 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 5)] __asm fadd dword ptr [edx + 4 * (3 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 0)] __asm fadd dword ptr [edx + 4 * (4 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 1)] __asm fadd dword ptr [edx + 4 * (4 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 2)] __asm fadd dword ptr [edx + 4 * (4 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 3)] __asm fadd dword ptr [edx + 4 * (4 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 4)] __asm fadd dword ptr [edx + 4 * (4 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 5)] __asm fadd dword ptr [edx + 4 * (4 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (5 * 6 + 0)] __asm fadd dword ptr [edx + 4 * (5 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (5 * 6 + 1)] __asm fadd dword ptr [edx + 4 * (5 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (5 * 6 + 2)] __asm fadd dword ptr [edx + 4 * (5 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (5 * 6 + 3)] __asm fadd dword ptr [edx + 4 * (5 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (5 * 6 + 4)] __asm fadd dword ptr [edx + 4 * (5 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (5 * 6 + 5)] __asm fadd dword ptr [edx + 4 * (5 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 5)]






	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,6,6)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 6 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 1)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 2)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 3)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 4)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 5)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 0)] __asm fsub dword ptr [edx + 4 * (1 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 1)] __asm fsub dword ptr [edx + 4 * (1 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 2)] __asm fsub dword ptr [edx + 4 * (1 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 3)] __asm fsub dword ptr [edx + 4 * (1 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 4)] __asm fsub dword ptr [edx + 4 * (1 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 5)] __asm fsub dword ptr [edx + 4 * (1 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 0)] __asm fsub dword ptr [edx + 4 * (2 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 1)] __asm fsub dword ptr [edx + 4 * (2 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 2)] __asm fsub dword ptr [edx + 4 * (2 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 3)] __asm fsub dword ptr [edx + 4 * (2 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 4)] __asm fsub dword ptr [edx + 4 * (2 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 5)] __asm fsub dword ptr [edx + 4 * (2 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 0)] __asm fsub dword ptr [edx + 4 * (3 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 1)] __asm fsub dword ptr [edx + 4 * (3 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 2)] __asm fsub dword ptr [edx + 4 * (3 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 3)] __asm fsub dword ptr [edx + 4 * (3 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 4)] __asm fsub dword ptr [edx + 4 * (3 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (3 * 6 + 5)] __asm fsub dword ptr [edx + 4 * (3 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (3 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 0)] __asm fsub dword ptr [edx + 4 * (4 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 1)] __asm fsub dword ptr [edx + 4 * (4 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 2)] __asm fsub dword ptr [edx + 4 * (4 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 3)] __asm fsub dword ptr [edx + 4 * (4 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 4)] __asm fsub dword ptr [edx + 4 * (4 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (4 * 6 + 5)] __asm fsub dword ptr [edx + 4 * (4 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (4 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (5 * 6 + 0)] __asm fsub dword ptr [edx + 4 * (5 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (5 * 6 + 1)] __asm fsub dword ptr [edx + 4 * (5 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (5 * 6 + 2)] __asm fsub dword ptr [edx + 4 * (5 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (5 * 6 + 3)] __asm fsub dword ptr [edx + 4 * (5 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (5 * 6 + 4)] __asm fsub dword ptr [edx + 4 * (5 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (5 * 6 + 5)] __asm fsub dword ptr [edx + 4 * (5 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 5)]






	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(Assign,6,6)(SMLXMatrix &A, const SMLXMatrix &B) 







{
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


__asm fld dword ptr [edx + 4 * (5 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 0)]


__asm fld dword ptr [edx + 4 * (5 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 1)]


__asm fld dword ptr [edx + 4 * (5 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 2)]


__asm fld dword ptr [edx + 4 * (5 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 3)]


__asm fld dword ptr [edx + 4 * (5 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 4)]


__asm fld dword ptr [edx + 4 * (5 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (5 * 6 + 5)]



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

























































	
	

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,6,6,6,6)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 6, 6, 6, 0, 0)


e6(0, 1, 6, 6, 6, 0, 0)


e6(0, 2, 6, 6, 6, 0, 0)


e6(0, 3, 6, 6, 6, 0, 0)


e6(0, 4, 6, 6, 6, 0, 0)


e6(0, 5, 6, 6, 6, 0, 0)


e6(1, 0, 6, 6, 6, 0, 0)


e6(1, 1, 6, 6, 6, 0, 0)


e6(1, 2, 6, 6, 6, 0, 0)


e6(1, 3, 6, 6, 6, 0, 0)


e6(1, 4, 6, 6, 6, 0, 0)


e6(1, 5, 6, 6, 6, 0, 0)


e6(2, 0, 6, 6, 6, 0, 0)


e6(2, 1, 6, 6, 6, 0, 0)


e6(2, 2, 6, 6, 6, 0, 0)


e6(2, 3, 6, 6, 6, 0, 0)


e6(2, 4, 6, 6, 6, 0, 0)


e6(2, 5, 6, 6, 6, 0, 0)


e6(3, 0, 6, 6, 6, 0, 0)


e6(3, 1, 6, 6, 6, 0, 0)


e6(3, 2, 6, 6, 6, 0, 0)


e6(3, 3, 6, 6, 6, 0, 0)


e6(3, 4, 6, 6, 6, 0, 0)


e6(3, 5, 6, 6, 6, 0, 0)


e6(4, 0, 6, 6, 6, 0, 0)


e6(4, 1, 6, 6, 6, 0, 0)


e6(4, 2, 6, 6, 6, 0, 0)


e6(4, 3, 6, 6, 6, 0, 0)


e6(4, 4, 6, 6, 6, 0, 0)


e6(4, 5, 6, 6, 6, 0, 0)


e6(5, 0, 6, 6, 6, 0, 0)


e6(5, 1, 6, 6, 6, 0, 0)


e6(5, 2, 6, 6, 6, 0, 0)


e6(5, 3, 6, 6, 6, 0, 0)


e6(5, 4, 6, 6, 6, 0, 0)


e6(5, 5, 6, 6, 6, 0, 0)



	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,6,6,6,6)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 6, 6, 6, 0, 1)


e6(0, 1, 6, 6, 6, 0, 1)


e6(0, 2, 6, 6, 6, 0, 1)


e6(0, 3, 6, 6, 6, 0, 1)


e6(0, 4, 6, 6, 6, 0, 1)


e6(0, 5, 6, 6, 6, 0, 1)


e6(1, 0, 6, 6, 6, 0, 1)


e6(1, 1, 6, 6, 6, 0, 1)


e6(1, 2, 6, 6, 6, 0, 1)


e6(1, 3, 6, 6, 6, 0, 1)


e6(1, 4, 6, 6, 6, 0, 1)


e6(1, 5, 6, 6, 6, 0, 1)


e6(2, 0, 6, 6, 6, 0, 1)


e6(2, 1, 6, 6, 6, 0, 1)


e6(2, 2, 6, 6, 6, 0, 1)


e6(2, 3, 6, 6, 6, 0, 1)


e6(2, 4, 6, 6, 6, 0, 1)


e6(2, 5, 6, 6, 6, 0, 1)


e6(3, 0, 6, 6, 6, 0, 1)


e6(3, 1, 6, 6, 6, 0, 1)


e6(3, 2, 6, 6, 6, 0, 1)


e6(3, 3, 6, 6, 6, 0, 1)


e6(3, 4, 6, 6, 6, 0, 1)


e6(3, 5, 6, 6, 6, 0, 1)


e6(4, 0, 6, 6, 6, 0, 1)


e6(4, 1, 6, 6, 6, 0, 1)


e6(4, 2, 6, 6, 6, 0, 1)


e6(4, 3, 6, 6, 6, 0, 1)


e6(4, 4, 6, 6, 6, 0, 1)


e6(4, 5, 6, 6, 6, 0, 1)


e6(5, 0, 6, 6, 6, 0, 1)


e6(5, 1, 6, 6, 6, 0, 1)


e6(5, 2, 6, 6, 6, 0, 1)


e6(5, 3, 6, 6, 6, 0, 1)


e6(5, 4, 6, 6, 6, 0, 1)


e6(5, 5, 6, 6, 6, 0, 1)



	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,6,6,6,6)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 6, 6, 6, 1, 0)


e6(0, 1, 6, 6, 6, 1, 0)


e6(0, 2, 6, 6, 6, 1, 0)


e6(0, 3, 6, 6, 6, 1, 0)


e6(0, 4, 6, 6, 6, 1, 0)


e6(0, 5, 6, 6, 6, 1, 0)


e6(1, 0, 6, 6, 6, 1, 0)


e6(1, 1, 6, 6, 6, 1, 0)


e6(1, 2, 6, 6, 6, 1, 0)


e6(1, 3, 6, 6, 6, 1, 0)


e6(1, 4, 6, 6, 6, 1, 0)


e6(1, 5, 6, 6, 6, 1, 0)


e6(2, 0, 6, 6, 6, 1, 0)


e6(2, 1, 6, 6, 6, 1, 0)


e6(2, 2, 6, 6, 6, 1, 0)


e6(2, 3, 6, 6, 6, 1, 0)


e6(2, 4, 6, 6, 6, 1, 0)


e6(2, 5, 6, 6, 6, 1, 0)


e6(3, 0, 6, 6, 6, 1, 0)


e6(3, 1, 6, 6, 6, 1, 0)


e6(3, 2, 6, 6, 6, 1, 0)


e6(3, 3, 6, 6, 6, 1, 0)


e6(3, 4, 6, 6, 6, 1, 0)


e6(3, 5, 6, 6, 6, 1, 0)


e6(4, 0, 6, 6, 6, 1, 0)


e6(4, 1, 6, 6, 6, 1, 0)


e6(4, 2, 6, 6, 6, 1, 0)


e6(4, 3, 6, 6, 6, 1, 0)


e6(4, 4, 6, 6, 6, 1, 0)


e6(4, 5, 6, 6, 6, 1, 0)


e6(5, 0, 6, 6, 6, 1, 0)


e6(5, 1, 6, 6, 6, 1, 0)


e6(5, 2, 6, 6, 6, 1, 0)


e6(5, 3, 6, 6, 6, 1, 0)


e6(5, 4, 6, 6, 6, 1, 0)


e6(5, 5, 6, 6, 6, 1, 0)



	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,6,6,6,6)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 6, 6, 6, 1, 1)


e6(0, 1, 6, 6, 6, 1, 1)


e6(0, 2, 6, 6, 6, 1, 1)


e6(0, 3, 6, 6, 6, 1, 1)


e6(0, 4, 6, 6, 6, 1, 1)


e6(0, 5, 6, 6, 6, 1, 1)


e6(1, 0, 6, 6, 6, 1, 1)


e6(1, 1, 6, 6, 6, 1, 1)


e6(1, 2, 6, 6, 6, 1, 1)


e6(1, 3, 6, 6, 6, 1, 1)


e6(1, 4, 6, 6, 6, 1, 1)


e6(1, 5, 6, 6, 6, 1, 1)


e6(2, 0, 6, 6, 6, 1, 1)


e6(2, 1, 6, 6, 6, 1, 1)


e6(2, 2, 6, 6, 6, 1, 1)


e6(2, 3, 6, 6, 6, 1, 1)


e6(2, 4, 6, 6, 6, 1, 1)


e6(2, 5, 6, 6, 6, 1, 1)


e6(3, 0, 6, 6, 6, 1, 1)


e6(3, 1, 6, 6, 6, 1, 1)


e6(3, 2, 6, 6, 6, 1, 1)


e6(3, 3, 6, 6, 6, 1, 1)


e6(3, 4, 6, 6, 6, 1, 1)


e6(3, 5, 6, 6, 6, 1, 1)


e6(4, 0, 6, 6, 6, 1, 1)


e6(4, 1, 6, 6, 6, 1, 1)


e6(4, 2, 6, 6, 6, 1, 1)


e6(4, 3, 6, 6, 6, 1, 1)


e6(4, 4, 6, 6, 6, 1, 1)


e6(4, 5, 6, 6, 6, 1, 1)


e6(5, 0, 6, 6, 6, 1, 1)


e6(5, 1, 6, 6, 6, 1, 1)


e6(5, 2, 6, 6, 6, 1, 1)


e6(5, 3, 6, 6, 6, 1, 1)


e6(5, 4, 6, 6, 6, 1, 1)


e6(5, 5, 6, 6, 6, 1, 1)



	__asm mov eax, edx __asm pop ebx __asm ret
}























































	
	




























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,6,6,6,5)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 5, 6, 5, 0, 0)


e6(0, 1, 5, 6, 5, 0, 0)


e6(0, 2, 5, 6, 5, 0, 0)


e6(0, 3, 5, 6, 5, 0, 0)


e6(0, 4, 5, 6, 5, 0, 0)





e6(1, 0, 5, 6, 5, 0, 0)


e6(1, 1, 5, 6, 5, 0, 0)


e6(1, 2, 5, 6, 5, 0, 0)


e6(1, 3, 5, 6, 5, 0, 0)


e6(1, 4, 5, 6, 5, 0, 0)





e6(2, 0, 5, 6, 5, 0, 0)


e6(2, 1, 5, 6, 5, 0, 0)


e6(2, 2, 5, 6, 5, 0, 0)


e6(2, 3, 5, 6, 5, 0, 0)


e6(2, 4, 5, 6, 5, 0, 0)





e6(3, 0, 5, 6, 5, 0, 0)


e6(3, 1, 5, 6, 5, 0, 0)


e6(3, 2, 5, 6, 5, 0, 0)


e6(3, 3, 5, 6, 5, 0, 0)


e6(3, 4, 5, 6, 5, 0, 0)





e6(4, 0, 5, 6, 5, 0, 0)


e6(4, 1, 5, 6, 5, 0, 0)


e6(4, 2, 5, 6, 5, 0, 0)


e6(4, 3, 5, 6, 5, 0, 0)


e6(4, 4, 5, 6, 5, 0, 0)





e6(5, 0, 5, 6, 5, 0, 0)


e6(5, 1, 5, 6, 5, 0, 0)


e6(5, 2, 5, 6, 5, 0, 0)


e6(5, 3, 5, 6, 5, 0, 0)


e6(5, 4, 5, 6, 5, 0, 0)






	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,6,6,5,6)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 5, 6, 6, 0, 1)


e6(0, 1, 5, 6, 6, 0, 1)


e6(0, 2, 5, 6, 6, 0, 1)


e6(0, 3, 5, 6, 6, 0, 1)


e6(0, 4, 5, 6, 6, 0, 1)





e6(1, 0, 5, 6, 6, 0, 1)


e6(1, 1, 5, 6, 6, 0, 1)


e6(1, 2, 5, 6, 6, 0, 1)


e6(1, 3, 5, 6, 6, 0, 1)


e6(1, 4, 5, 6, 6, 0, 1)





e6(2, 0, 5, 6, 6, 0, 1)


e6(2, 1, 5, 6, 6, 0, 1)


e6(2, 2, 5, 6, 6, 0, 1)


e6(2, 3, 5, 6, 6, 0, 1)


e6(2, 4, 5, 6, 6, 0, 1)





e6(3, 0, 5, 6, 6, 0, 1)


e6(3, 1, 5, 6, 6, 0, 1)


e6(3, 2, 5, 6, 6, 0, 1)


e6(3, 3, 5, 6, 6, 0, 1)


e6(3, 4, 5, 6, 6, 0, 1)





e6(4, 0, 5, 6, 6, 0, 1)


e6(4, 1, 5, 6, 6, 0, 1)


e6(4, 2, 5, 6, 6, 0, 1)


e6(4, 3, 5, 6, 6, 0, 1)


e6(4, 4, 5, 6, 6, 0, 1)





e6(5, 0, 5, 6, 6, 0, 1)


e6(5, 1, 5, 6, 6, 0, 1)


e6(5, 2, 5, 6, 6, 0, 1)


e6(5, 3, 5, 6, 6, 0, 1)


e6(5, 4, 5, 6, 6, 0, 1)






	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,6,6,6,5)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 5, 6, 5, 1, 0)


e6(0, 1, 5, 6, 5, 1, 0)


e6(0, 2, 5, 6, 5, 1, 0)


e6(0, 3, 5, 6, 5, 1, 0)


e6(0, 4, 5, 6, 5, 1, 0)





e6(1, 0, 5, 6, 5, 1, 0)


e6(1, 1, 5, 6, 5, 1, 0)


e6(1, 2, 5, 6, 5, 1, 0)


e6(1, 3, 5, 6, 5, 1, 0)


e6(1, 4, 5, 6, 5, 1, 0)





e6(2, 0, 5, 6, 5, 1, 0)


e6(2, 1, 5, 6, 5, 1, 0)


e6(2, 2, 5, 6, 5, 1, 0)


e6(2, 3, 5, 6, 5, 1, 0)


e6(2, 4, 5, 6, 5, 1, 0)





e6(3, 0, 5, 6, 5, 1, 0)


e6(3, 1, 5, 6, 5, 1, 0)


e6(3, 2, 5, 6, 5, 1, 0)


e6(3, 3, 5, 6, 5, 1, 0)


e6(3, 4, 5, 6, 5, 1, 0)





e6(4, 0, 5, 6, 5, 1, 0)


e6(4, 1, 5, 6, 5, 1, 0)


e6(4, 2, 5, 6, 5, 1, 0)


e6(4, 3, 5, 6, 5, 1, 0)


e6(4, 4, 5, 6, 5, 1, 0)





e6(5, 0, 5, 6, 5, 1, 0)


e6(5, 1, 5, 6, 5, 1, 0)


e6(5, 2, 5, 6, 5, 1, 0)


e6(5, 3, 5, 6, 5, 1, 0)


e6(5, 4, 5, 6, 5, 1, 0)






	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,6,6,5,6)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 5, 6, 6, 1, 1)


e6(0, 1, 5, 6, 6, 1, 1)


e6(0, 2, 5, 6, 6, 1, 1)


e6(0, 3, 5, 6, 6, 1, 1)


e6(0, 4, 5, 6, 6, 1, 1)





e6(1, 0, 5, 6, 6, 1, 1)


e6(1, 1, 5, 6, 6, 1, 1)


e6(1, 2, 5, 6, 6, 1, 1)


e6(1, 3, 5, 6, 6, 1, 1)


e6(1, 4, 5, 6, 6, 1, 1)





e6(2, 0, 5, 6, 6, 1, 1)


e6(2, 1, 5, 6, 6, 1, 1)


e6(2, 2, 5, 6, 6, 1, 1)


e6(2, 3, 5, 6, 6, 1, 1)


e6(2, 4, 5, 6, 6, 1, 1)





e6(3, 0, 5, 6, 6, 1, 1)


e6(3, 1, 5, 6, 6, 1, 1)


e6(3, 2, 5, 6, 6, 1, 1)


e6(3, 3, 5, 6, 6, 1, 1)


e6(3, 4, 5, 6, 6, 1, 1)





e6(4, 0, 5, 6, 6, 1, 1)


e6(4, 1, 5, 6, 6, 1, 1)


e6(4, 2, 5, 6, 6, 1, 1)


e6(4, 3, 5, 6, 6, 1, 1)


e6(4, 4, 5, 6, 6, 1, 1)





e6(5, 0, 5, 6, 6, 1, 1)


e6(5, 1, 5, 6, 6, 1, 1)


e6(5, 2, 5, 6, 6, 1, 1)


e6(5, 3, 5, 6, 6, 1, 1)


e6(5, 4, 5, 6, 6, 1, 1)






	__asm mov eax, edx __asm pop ebx __asm ret
}

















































	
	


































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,6,6,6,3)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 3, 6, 3, 0, 0)


e6(0, 1, 3, 6, 3, 0, 0)


e6(0, 2, 3, 6, 3, 0, 0)











e6(1, 0, 3, 6, 3, 0, 0)


e6(1, 1, 3, 6, 3, 0, 0)


e6(1, 2, 3, 6, 3, 0, 0)











e6(2, 0, 3, 6, 3, 0, 0)


e6(2, 1, 3, 6, 3, 0, 0)


e6(2, 2, 3, 6, 3, 0, 0)











e6(3, 0, 3, 6, 3, 0, 0)


e6(3, 1, 3, 6, 3, 0, 0)


e6(3, 2, 3, 6, 3, 0, 0)











e6(4, 0, 3, 6, 3, 0, 0)


e6(4, 1, 3, 6, 3, 0, 0)


e6(4, 2, 3, 6, 3, 0, 0)











e6(5, 0, 3, 6, 3, 0, 0)


e6(5, 1, 3, 6, 3, 0, 0)


e6(5, 2, 3, 6, 3, 0, 0)












	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,6,6,3,6)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 3, 6, 6, 0, 1)


e6(0, 1, 3, 6, 6, 0, 1)


e6(0, 2, 3, 6, 6, 0, 1)











e6(1, 0, 3, 6, 6, 0, 1)


e6(1, 1, 3, 6, 6, 0, 1)


e6(1, 2, 3, 6, 6, 0, 1)











e6(2, 0, 3, 6, 6, 0, 1)


e6(2, 1, 3, 6, 6, 0, 1)


e6(2, 2, 3, 6, 6, 0, 1)











e6(3, 0, 3, 6, 6, 0, 1)


e6(3, 1, 3, 6, 6, 0, 1)


e6(3, 2, 3, 6, 6, 0, 1)











e6(4, 0, 3, 6, 6, 0, 1)


e6(4, 1, 3, 6, 6, 0, 1)


e6(4, 2, 3, 6, 6, 0, 1)











e6(5, 0, 3, 6, 6, 0, 1)


e6(5, 1, 3, 6, 6, 0, 1)


e6(5, 2, 3, 6, 6, 0, 1)












	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,6,6,6,3)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 3, 6, 3, 1, 0)


e6(0, 1, 3, 6, 3, 1, 0)


e6(0, 2, 3, 6, 3, 1, 0)











e6(1, 0, 3, 6, 3, 1, 0)


e6(1, 1, 3, 6, 3, 1, 0)


e6(1, 2, 3, 6, 3, 1, 0)











e6(2, 0, 3, 6, 3, 1, 0)


e6(2, 1, 3, 6, 3, 1, 0)


e6(2, 2, 3, 6, 3, 1, 0)











e6(3, 0, 3, 6, 3, 1, 0)


e6(3, 1, 3, 6, 3, 1, 0)


e6(3, 2, 3, 6, 3, 1, 0)











e6(4, 0, 3, 6, 3, 1, 0)


e6(4, 1, 3, 6, 3, 1, 0)


e6(4, 2, 3, 6, 3, 1, 0)











e6(5, 0, 3, 6, 3, 1, 0)


e6(5, 1, 3, 6, 3, 1, 0)


e6(5, 2, 3, 6, 3, 1, 0)












	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,6,6,3,6)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 3, 6, 6, 1, 1)


e6(0, 1, 3, 6, 6, 1, 1)


e6(0, 2, 3, 6, 6, 1, 1)











e6(1, 0, 3, 6, 6, 1, 1)


e6(1, 1, 3, 6, 6, 1, 1)


e6(1, 2, 3, 6, 6, 1, 1)











e6(2, 0, 3, 6, 6, 1, 1)


e6(2, 1, 3, 6, 6, 1, 1)


e6(2, 2, 3, 6, 6, 1, 1)











e6(3, 0, 3, 6, 6, 1, 1)


e6(3, 1, 3, 6, 6, 1, 1)


e6(3, 2, 3, 6, 6, 1, 1)











e6(4, 0, 3, 6, 6, 1, 1)


e6(4, 1, 3, 6, 6, 1, 1)


e6(4, 2, 3, 6, 6, 1, 1)











e6(5, 0, 3, 6, 6, 1, 1)


e6(5, 1, 3, 6, 6, 1, 1)


e6(5, 2, 3, 6, 6, 1, 1)












	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,6,6,6,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 1, 6, 1, 0, 0)

















e6(1, 0, 1, 6, 1, 0, 0)

















e6(2, 0, 1, 6, 1, 0, 0)

















e6(3, 0, 1, 6, 1, 0, 0)

















e6(4, 0, 1, 6, 1, 0, 0)

















e6(5, 0, 1, 6, 1, 0, 0)


















	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,6,6,1,6)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 1, 6, 6, 0, 1)

















e6(1, 0, 1, 6, 6, 0, 1)

















e6(2, 0, 1, 6, 6, 0, 1)

















e6(3, 0, 1, 6, 6, 0, 1)

















e6(4, 0, 1, 6, 6, 0, 1)

















e6(5, 0, 1, 6, 6, 0, 1)


















	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,6,6,6,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 1, 6, 1, 1, 0)

















e6(1, 0, 1, 6, 1, 1, 0)

















e6(2, 0, 1, 6, 1, 1, 0)

















e6(3, 0, 1, 6, 1, 1, 0)

















e6(4, 0, 1, 6, 1, 1, 0)

















e6(5, 0, 1, 6, 1, 1, 0)


















	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,6,6,1,6)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 1, 6, 6, 1, 1)

















e6(1, 0, 1, 6, 6, 1, 1)

















e6(2, 0, 1, 6, 6, 1, 1)

















e6(3, 0, 1, 6, 6, 1, 1)

















e6(4, 0, 1, 6, 6, 1, 1)

















e6(5, 0, 1, 6, 6, 1, 1)


















	__asm mov eax, edx __asm pop ebx __asm ret
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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (1 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (2 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (3 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (3 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (4 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (4 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (4 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (5 * 1 + 0)] __asm fadd dword ptr [ecx + 4 * (5 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (5 * 1 + 0)]


















	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,6,1)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,6,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (1 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (2 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (3 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (3 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (4 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (4 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (4 * 1 + 0)]

















__asm fld dword ptr [ebx + 4 * (5 * 1 + 0)] __asm fsub dword ptr [ecx + 4 * (5 * 1 + 0)] __asm fstp dword ptr [eax + 4 * (5 * 1 + 0)]


















	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,6,1)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (1 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (2 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (3 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (3 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (4 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (4 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (5 * 1 + 0)] __asm fadd dword ptr [edx + 4 * (5 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (5 * 1 + 0)]





















	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,6,1)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (1 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (2 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (3 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (3 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (4 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (4 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 1 + 0)]

















__asm fld dword ptr [ecx + 4 * (5 * 1 + 0)] __asm fsub dword ptr [edx + 4 * (5 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (5 * 1 + 0)]





















	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(Assign,6,1)(SMLXMatrix &A, const SMLXMatrix &B) 







{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax


























__asm fld dword ptr [edx + 4 * (0 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 1 + 0)]

















__asm fld dword ptr [edx + 4 * (1 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 1 + 0)]

















__asm fld dword ptr [edx + 4 * (2 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 1 + 0)]

















__asm fld dword ptr [edx + 4 * (3 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 1 + 0)]

















__asm fld dword ptr [edx + 4 * (4 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 1 + 0)]

















__asm fld dword ptr [edx + 4 * (5 * 1 + 0)] __asm fstp dword ptr [ecx + 4 * (5 * 1 + 0)]


















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

























































	
	

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,6,1,1,6)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 6, 1, 6, 0, 0)


e1(0, 1, 6, 1, 6, 0, 0)


e1(0, 2, 6, 1, 6, 0, 0)


e1(0, 3, 6, 1, 6, 0, 0)


e1(0, 4, 6, 1, 6, 0, 0)


e1(0, 5, 6, 1, 6, 0, 0)


e1(1, 0, 6, 1, 6, 0, 0)


e1(1, 1, 6, 1, 6, 0, 0)


e1(1, 2, 6, 1, 6, 0, 0)


e1(1, 3, 6, 1, 6, 0, 0)


e1(1, 4, 6, 1, 6, 0, 0)


e1(1, 5, 6, 1, 6, 0, 0)


e1(2, 0, 6, 1, 6, 0, 0)


e1(2, 1, 6, 1, 6, 0, 0)


e1(2, 2, 6, 1, 6, 0, 0)


e1(2, 3, 6, 1, 6, 0, 0)


e1(2, 4, 6, 1, 6, 0, 0)


e1(2, 5, 6, 1, 6, 0, 0)


e1(3, 0, 6, 1, 6, 0, 0)


e1(3, 1, 6, 1, 6, 0, 0)


e1(3, 2, 6, 1, 6, 0, 0)


e1(3, 3, 6, 1, 6, 0, 0)


e1(3, 4, 6, 1, 6, 0, 0)


e1(3, 5, 6, 1, 6, 0, 0)


e1(4, 0, 6, 1, 6, 0, 0)


e1(4, 1, 6, 1, 6, 0, 0)


e1(4, 2, 6, 1, 6, 0, 0)


e1(4, 3, 6, 1, 6, 0, 0)


e1(4, 4, 6, 1, 6, 0, 0)


e1(4, 5, 6, 1, 6, 0, 0)


e1(5, 0, 6, 1, 6, 0, 0)


e1(5, 1, 6, 1, 6, 0, 0)


e1(5, 2, 6, 1, 6, 0, 0)


e1(5, 3, 6, 1, 6, 0, 0)


e1(5, 4, 6, 1, 6, 0, 0)


e1(5, 5, 6, 1, 6, 0, 0)



	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,6,1,6,1)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 6, 1, 1, 0, 1)


e1(0, 1, 6, 1, 1, 0, 1)


e1(0, 2, 6, 1, 1, 0, 1)


e1(0, 3, 6, 1, 1, 0, 1)


e1(0, 4, 6, 1, 1, 0, 1)


e1(0, 5, 6, 1, 1, 0, 1)


e1(1, 0, 6, 1, 1, 0, 1)


e1(1, 1, 6, 1, 1, 0, 1)


e1(1, 2, 6, 1, 1, 0, 1)


e1(1, 3, 6, 1, 1, 0, 1)


e1(1, 4, 6, 1, 1, 0, 1)


e1(1, 5, 6, 1, 1, 0, 1)


e1(2, 0, 6, 1, 1, 0, 1)


e1(2, 1, 6, 1, 1, 0, 1)


e1(2, 2, 6, 1, 1, 0, 1)


e1(2, 3, 6, 1, 1, 0, 1)


e1(2, 4, 6, 1, 1, 0, 1)


e1(2, 5, 6, 1, 1, 0, 1)


e1(3, 0, 6, 1, 1, 0, 1)


e1(3, 1, 6, 1, 1, 0, 1)


e1(3, 2, 6, 1, 1, 0, 1)


e1(3, 3, 6, 1, 1, 0, 1)


e1(3, 4, 6, 1, 1, 0, 1)


e1(3, 5, 6, 1, 1, 0, 1)


e1(4, 0, 6, 1, 1, 0, 1)


e1(4, 1, 6, 1, 1, 0, 1)


e1(4, 2, 6, 1, 1, 0, 1)


e1(4, 3, 6, 1, 1, 0, 1)


e1(4, 4, 6, 1, 1, 0, 1)


e1(4, 5, 6, 1, 1, 0, 1)


e1(5, 0, 6, 1, 1, 0, 1)


e1(5, 1, 6, 1, 1, 0, 1)


e1(5, 2, 6, 1, 1, 0, 1)


e1(5, 3, 6, 1, 1, 0, 1)


e1(5, 4, 6, 1, 1, 0, 1)


e1(5, 5, 6, 1, 1, 0, 1)



	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,6,1,6,6)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 6, 1, 6, 1, 0)


e6(0, 1, 6, 1, 6, 1, 0)


e6(0, 2, 6, 1, 6, 1, 0)


e6(0, 3, 6, 1, 6, 1, 0)


e6(0, 4, 6, 1, 6, 1, 0)


e6(0, 5, 6, 1, 6, 1, 0)





























































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,6,1,6,6)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 6, 1, 6, 1, 1)


e6(0, 1, 6, 1, 6, 1, 1)


e6(0, 2, 6, 1, 6, 1, 1)


e6(0, 3, 6, 1, 6, 1, 1)


e6(0, 4, 6, 1, 6, 1, 1)


e6(0, 5, 6, 1, 6, 1, 1)





























































































	__asm mov eax, edx __asm pop ebx __asm ret
}























































	
	




























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,6,1,1,5)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 5, 1, 5, 0, 0)


e1(0, 1, 5, 1, 5, 0, 0)


e1(0, 2, 5, 1, 5, 0, 0)


e1(0, 3, 5, 1, 5, 0, 0)


e1(0, 4, 5, 1, 5, 0, 0)





e1(1, 0, 5, 1, 5, 0, 0)


e1(1, 1, 5, 1, 5, 0, 0)


e1(1, 2, 5, 1, 5, 0, 0)


e1(1, 3, 5, 1, 5, 0, 0)


e1(1, 4, 5, 1, 5, 0, 0)





e1(2, 0, 5, 1, 5, 0, 0)


e1(2, 1, 5, 1, 5, 0, 0)


e1(2, 2, 5, 1, 5, 0, 0)


e1(2, 3, 5, 1, 5, 0, 0)


e1(2, 4, 5, 1, 5, 0, 0)





e1(3, 0, 5, 1, 5, 0, 0)


e1(3, 1, 5, 1, 5, 0, 0)


e1(3, 2, 5, 1, 5, 0, 0)


e1(3, 3, 5, 1, 5, 0, 0)


e1(3, 4, 5, 1, 5, 0, 0)





e1(4, 0, 5, 1, 5, 0, 0)


e1(4, 1, 5, 1, 5, 0, 0)


e1(4, 2, 5, 1, 5, 0, 0)


e1(4, 3, 5, 1, 5, 0, 0)


e1(4, 4, 5, 1, 5, 0, 0)





e1(5, 0, 5, 1, 5, 0, 0)


e1(5, 1, 5, 1, 5, 0, 0)


e1(5, 2, 5, 1, 5, 0, 0)


e1(5, 3, 5, 1, 5, 0, 0)


e1(5, 4, 5, 1, 5, 0, 0)






	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,6,1,5,1)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 5, 1, 1, 0, 1)


e1(0, 1, 5, 1, 1, 0, 1)


e1(0, 2, 5, 1, 1, 0, 1)


e1(0, 3, 5, 1, 1, 0, 1)


e1(0, 4, 5, 1, 1, 0, 1)





e1(1, 0, 5, 1, 1, 0, 1)


e1(1, 1, 5, 1, 1, 0, 1)


e1(1, 2, 5, 1, 1, 0, 1)


e1(1, 3, 5, 1, 1, 0, 1)


e1(1, 4, 5, 1, 1, 0, 1)





e1(2, 0, 5, 1, 1, 0, 1)


e1(2, 1, 5, 1, 1, 0, 1)


e1(2, 2, 5, 1, 1, 0, 1)


e1(2, 3, 5, 1, 1, 0, 1)


e1(2, 4, 5, 1, 1, 0, 1)





e1(3, 0, 5, 1, 1, 0, 1)


e1(3, 1, 5, 1, 1, 0, 1)


e1(3, 2, 5, 1, 1, 0, 1)


e1(3, 3, 5, 1, 1, 0, 1)


e1(3, 4, 5, 1, 1, 0, 1)





e1(4, 0, 5, 1, 1, 0, 1)


e1(4, 1, 5, 1, 1, 0, 1)


e1(4, 2, 5, 1, 1, 0, 1)


e1(4, 3, 5, 1, 1, 0, 1)


e1(4, 4, 5, 1, 1, 0, 1)





e1(5, 0, 5, 1, 1, 0, 1)


e1(5, 1, 5, 1, 1, 0, 1)


e1(5, 2, 5, 1, 1, 0, 1)


e1(5, 3, 5, 1, 1, 0, 1)


e1(5, 4, 5, 1, 1, 0, 1)






	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,6,1,6,5)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 5, 1, 5, 1, 0)


e6(0, 1, 5, 1, 5, 1, 0)


e6(0, 2, 5, 1, 5, 1, 0)


e6(0, 3, 5, 1, 5, 1, 0)


e6(0, 4, 5, 1, 5, 1, 0)
































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,6,1,5,6)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 5, 1, 6, 1, 1)


e6(0, 1, 5, 1, 6, 1, 1)


e6(0, 2, 5, 1, 6, 1, 1)


e6(0, 3, 5, 1, 6, 1, 1)


e6(0, 4, 5, 1, 6, 1, 1)
































































































	__asm mov eax, edx __asm pop ebx __asm ret
}

















































	
	


































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,6,1,1,3)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 3, 1, 3, 0, 0)


e1(0, 1, 3, 1, 3, 0, 0)


e1(0, 2, 3, 1, 3, 0, 0)











e1(1, 0, 3, 1, 3, 0, 0)


e1(1, 1, 3, 1, 3, 0, 0)


e1(1, 2, 3, 1, 3, 0, 0)











e1(2, 0, 3, 1, 3, 0, 0)


e1(2, 1, 3, 1, 3, 0, 0)


e1(2, 2, 3, 1, 3, 0, 0)











e1(3, 0, 3, 1, 3, 0, 0)


e1(3, 1, 3, 1, 3, 0, 0)


e1(3, 2, 3, 1, 3, 0, 0)











e1(4, 0, 3, 1, 3, 0, 0)


e1(4, 1, 3, 1, 3, 0, 0)


e1(4, 2, 3, 1, 3, 0, 0)











e1(5, 0, 3, 1, 3, 0, 0)


e1(5, 1, 3, 1, 3, 0, 0)


e1(5, 2, 3, 1, 3, 0, 0)












	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,6,1,3,1)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 3, 1, 1, 0, 1)


e1(0, 1, 3, 1, 1, 0, 1)


e1(0, 2, 3, 1, 1, 0, 1)











e1(1, 0, 3, 1, 1, 0, 1)


e1(1, 1, 3, 1, 1, 0, 1)


e1(1, 2, 3, 1, 1, 0, 1)











e1(2, 0, 3, 1, 1, 0, 1)


e1(2, 1, 3, 1, 1, 0, 1)


e1(2, 2, 3, 1, 1, 0, 1)











e1(3, 0, 3, 1, 1, 0, 1)


e1(3, 1, 3, 1, 1, 0, 1)


e1(3, 2, 3, 1, 1, 0, 1)











e1(4, 0, 3, 1, 1, 0, 1)


e1(4, 1, 3, 1, 1, 0, 1)


e1(4, 2, 3, 1, 1, 0, 1)











e1(5, 0, 3, 1, 1, 0, 1)


e1(5, 1, 3, 1, 1, 0, 1)


e1(5, 2, 3, 1, 1, 0, 1)












	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,6,1,6,3)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 3, 1, 3, 1, 0)


e6(0, 1, 3, 1, 3, 1, 0)


e6(0, 2, 3, 1, 3, 1, 0)






































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,6,1,3,6)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 3, 1, 6, 1, 1)


e6(0, 1, 3, 1, 6, 1, 1)


e6(0, 2, 3, 1, 6, 1, 1)






































































































	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,6,1,1,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 1, 1, 0, 0)

















e1(1, 0, 1, 1, 1, 0, 0)

















e1(2, 0, 1, 1, 1, 0, 0)

















e1(3, 0, 1, 1, 1, 0, 0)

















e1(4, 0, 1, 1, 1, 0, 0)

















e1(5, 0, 1, 1, 1, 0, 0)


















	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,6,1,1,1)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 1, 1, 0, 1)

















e1(1, 0, 1, 1, 1, 0, 1)

















e1(2, 0, 1, 1, 1, 0, 1)

















e1(3, 0, 1, 1, 1, 0, 1)

















e1(4, 0, 1, 1, 1, 0, 1)

















e1(5, 0, 1, 1, 1, 0, 1)


















	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,6,1,6,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 1, 1, 1, 1, 0)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,6,1,1,6)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 1, 1, 6, 1, 1)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 6 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 1)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 2)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 3)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 4)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 5)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 5)]





























































































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,1,6)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,1,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 6 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 1)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 2)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 3)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 4)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 5)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 5)]





























































































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,1,6)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 6 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 1)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 2)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 3)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 4)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 5)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 5)]
































































































	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,1,6)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 6 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 1)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 2)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 3)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 4)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 5)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 5)]
































































































	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(Assign,1,6)(SMLXMatrix &A, const SMLXMatrix &B) 







{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax


























__asm fld dword ptr [edx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 0)]


__asm fld dword ptr [edx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 1)]


__asm fld dword ptr [edx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 2)]


__asm fld dword ptr [edx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 3)]


__asm fld dword ptr [edx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 4)]


__asm fld dword ptr [edx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 5)]





























































































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

























































	
	

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,1,6,6,6)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 6, 6, 6, 0, 0)


e6(0, 1, 6, 6, 6, 0, 0)


e6(0, 2, 6, 6, 6, 0, 0)


e6(0, 3, 6, 6, 6, 0, 0)


e6(0, 4, 6, 6, 6, 0, 0)


e6(0, 5, 6, 6, 6, 0, 0)





























































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,1,6,6,6)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 6, 6, 6, 0, 1)


e6(0, 1, 6, 6, 6, 0, 1)


e6(0, 2, 6, 6, 6, 0, 1)


e6(0, 3, 6, 6, 6, 0, 1)


e6(0, 4, 6, 6, 6, 0, 1)


e6(0, 5, 6, 6, 6, 0, 1)





























































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,1,6,1,6)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 6, 6, 6, 1, 0)


e1(0, 1, 6, 6, 6, 1, 0)


e1(0, 2, 6, 6, 6, 1, 0)


e1(0, 3, 6, 6, 6, 1, 0)


e1(0, 4, 6, 6, 6, 1, 0)


e1(0, 5, 6, 6, 6, 1, 0)


e1(1, 0, 6, 6, 6, 1, 0)


e1(1, 1, 6, 6, 6, 1, 0)


e1(1, 2, 6, 6, 6, 1, 0)


e1(1, 3, 6, 6, 6, 1, 0)


e1(1, 4, 6, 6, 6, 1, 0)


e1(1, 5, 6, 6, 6, 1, 0)


e1(2, 0, 6, 6, 6, 1, 0)


e1(2, 1, 6, 6, 6, 1, 0)


e1(2, 2, 6, 6, 6, 1, 0)


e1(2, 3, 6, 6, 6, 1, 0)


e1(2, 4, 6, 6, 6, 1, 0)


e1(2, 5, 6, 6, 6, 1, 0)


e1(3, 0, 6, 6, 6, 1, 0)


e1(3, 1, 6, 6, 6, 1, 0)


e1(3, 2, 6, 6, 6, 1, 0)


e1(3, 3, 6, 6, 6, 1, 0)


e1(3, 4, 6, 6, 6, 1, 0)


e1(3, 5, 6, 6, 6, 1, 0)


e1(4, 0, 6, 6, 6, 1, 0)


e1(4, 1, 6, 6, 6, 1, 0)


e1(4, 2, 6, 6, 6, 1, 0)


e1(4, 3, 6, 6, 6, 1, 0)


e1(4, 4, 6, 6, 6, 1, 0)


e1(4, 5, 6, 6, 6, 1, 0)


e1(5, 0, 6, 6, 6, 1, 0)


e1(5, 1, 6, 6, 6, 1, 0)


e1(5, 2, 6, 6, 6, 1, 0)


e1(5, 3, 6, 6, 6, 1, 0)


e1(5, 4, 6, 6, 6, 1, 0)


e1(5, 5, 6, 6, 6, 1, 0)



	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,1,6,6,1)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 6, 6, 1, 1, 1)


e1(0, 1, 6, 6, 1, 1, 1)


e1(0, 2, 6, 6, 1, 1, 1)


e1(0, 3, 6, 6, 1, 1, 1)


e1(0, 4, 6, 6, 1, 1, 1)


e1(0, 5, 6, 6, 1, 1, 1)


e1(1, 0, 6, 6, 1, 1, 1)


e1(1, 1, 6, 6, 1, 1, 1)


e1(1, 2, 6, 6, 1, 1, 1)


e1(1, 3, 6, 6, 1, 1, 1)


e1(1, 4, 6, 6, 1, 1, 1)


e1(1, 5, 6, 6, 1, 1, 1)


e1(2, 0, 6, 6, 1, 1, 1)


e1(2, 1, 6, 6, 1, 1, 1)


e1(2, 2, 6, 6, 1, 1, 1)


e1(2, 3, 6, 6, 1, 1, 1)


e1(2, 4, 6, 6, 1, 1, 1)


e1(2, 5, 6, 6, 1, 1, 1)


e1(3, 0, 6, 6, 1, 1, 1)


e1(3, 1, 6, 6, 1, 1, 1)


e1(3, 2, 6, 6, 1, 1, 1)


e1(3, 3, 6, 6, 1, 1, 1)


e1(3, 4, 6, 6, 1, 1, 1)


e1(3, 5, 6, 6, 1, 1, 1)


e1(4, 0, 6, 6, 1, 1, 1)


e1(4, 1, 6, 6, 1, 1, 1)


e1(4, 2, 6, 6, 1, 1, 1)


e1(4, 3, 6, 6, 1, 1, 1)


e1(4, 4, 6, 6, 1, 1, 1)


e1(4, 5, 6, 6, 1, 1, 1)


e1(5, 0, 6, 6, 1, 1, 1)


e1(5, 1, 6, 6, 1, 1, 1)


e1(5, 2, 6, 6, 1, 1, 1)


e1(5, 3, 6, 6, 1, 1, 1)


e1(5, 4, 6, 6, 1, 1, 1)


e1(5, 5, 6, 6, 1, 1, 1)



	__asm mov eax, edx __asm pop ebx __asm ret
}























































	
	




























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,1,6,6,5)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 5, 6, 5, 0, 0)


e6(0, 1, 5, 6, 5, 0, 0)


e6(0, 2, 5, 6, 5, 0, 0)


e6(0, 3, 5, 6, 5, 0, 0)


e6(0, 4, 5, 6, 5, 0, 0)
































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,1,6,5,6)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 5, 6, 6, 0, 1)


e6(0, 1, 5, 6, 6, 0, 1)


e6(0, 2, 5, 6, 6, 0, 1)


e6(0, 3, 5, 6, 6, 0, 1)


e6(0, 4, 5, 6, 6, 0, 1)
































































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,1,6,1,5)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 5, 6, 5, 1, 0)


e1(0, 1, 5, 6, 5, 1, 0)


e1(0, 2, 5, 6, 5, 1, 0)


e1(0, 3, 5, 6, 5, 1, 0)


e1(0, 4, 5, 6, 5, 1, 0)





e1(1, 0, 5, 6, 5, 1, 0)


e1(1, 1, 5, 6, 5, 1, 0)


e1(1, 2, 5, 6, 5, 1, 0)


e1(1, 3, 5, 6, 5, 1, 0)


e1(1, 4, 5, 6, 5, 1, 0)





e1(2, 0, 5, 6, 5, 1, 0)


e1(2, 1, 5, 6, 5, 1, 0)


e1(2, 2, 5, 6, 5, 1, 0)


e1(2, 3, 5, 6, 5, 1, 0)


e1(2, 4, 5, 6, 5, 1, 0)





e1(3, 0, 5, 6, 5, 1, 0)


e1(3, 1, 5, 6, 5, 1, 0)


e1(3, 2, 5, 6, 5, 1, 0)


e1(3, 3, 5, 6, 5, 1, 0)


e1(3, 4, 5, 6, 5, 1, 0)





e1(4, 0, 5, 6, 5, 1, 0)


e1(4, 1, 5, 6, 5, 1, 0)


e1(4, 2, 5, 6, 5, 1, 0)


e1(4, 3, 5, 6, 5, 1, 0)


e1(4, 4, 5, 6, 5, 1, 0)





e1(5, 0, 5, 6, 5, 1, 0)


e1(5, 1, 5, 6, 5, 1, 0)


e1(5, 2, 5, 6, 5, 1, 0)


e1(5, 3, 5, 6, 5, 1, 0)


e1(5, 4, 5, 6, 5, 1, 0)






	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,1,6,5,1)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 5, 6, 1, 1, 1)


e1(0, 1, 5, 6, 1, 1, 1)


e1(0, 2, 5, 6, 1, 1, 1)


e1(0, 3, 5, 6, 1, 1, 1)


e1(0, 4, 5, 6, 1, 1, 1)





e1(1, 0, 5, 6, 1, 1, 1)


e1(1, 1, 5, 6, 1, 1, 1)


e1(1, 2, 5, 6, 1, 1, 1)


e1(1, 3, 5, 6, 1, 1, 1)


e1(1, 4, 5, 6, 1, 1, 1)





e1(2, 0, 5, 6, 1, 1, 1)


e1(2, 1, 5, 6, 1, 1, 1)


e1(2, 2, 5, 6, 1, 1, 1)


e1(2, 3, 5, 6, 1, 1, 1)


e1(2, 4, 5, 6, 1, 1, 1)





e1(3, 0, 5, 6, 1, 1, 1)


e1(3, 1, 5, 6, 1, 1, 1)


e1(3, 2, 5, 6, 1, 1, 1)


e1(3, 3, 5, 6, 1, 1, 1)


e1(3, 4, 5, 6, 1, 1, 1)





e1(4, 0, 5, 6, 1, 1, 1)


e1(4, 1, 5, 6, 1, 1, 1)


e1(4, 2, 5, 6, 1, 1, 1)


e1(4, 3, 5, 6, 1, 1, 1)


e1(4, 4, 5, 6, 1, 1, 1)





e1(5, 0, 5, 6, 1, 1, 1)


e1(5, 1, 5, 6, 1, 1, 1)


e1(5, 2, 5, 6, 1, 1, 1)


e1(5, 3, 5, 6, 1, 1, 1)


e1(5, 4, 5, 6, 1, 1, 1)






	__asm mov eax, edx __asm pop ebx __asm ret
}

















































	
	


































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,1,6,6,3)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 3, 6, 3, 0, 0)


e6(0, 1, 3, 6, 3, 0, 0)


e6(0, 2, 3, 6, 3, 0, 0)






































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,1,6,3,6)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 3, 6, 6, 0, 1)


e6(0, 1, 3, 6, 6, 0, 1)


e6(0, 2, 3, 6, 6, 0, 1)






































































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,1,6,1,3)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 3, 6, 3, 1, 0)


e1(0, 1, 3, 6, 3, 1, 0)


e1(0, 2, 3, 6, 3, 1, 0)











e1(1, 0, 3, 6, 3, 1, 0)


e1(1, 1, 3, 6, 3, 1, 0)


e1(1, 2, 3, 6, 3, 1, 0)











e1(2, 0, 3, 6, 3, 1, 0)


e1(2, 1, 3, 6, 3, 1, 0)


e1(2, 2, 3, 6, 3, 1, 0)











e1(3, 0, 3, 6, 3, 1, 0)


e1(3, 1, 3, 6, 3, 1, 0)


e1(3, 2, 3, 6, 3, 1, 0)











e1(4, 0, 3, 6, 3, 1, 0)


e1(4, 1, 3, 6, 3, 1, 0)


e1(4, 2, 3, 6, 3, 1, 0)











e1(5, 0, 3, 6, 3, 1, 0)


e1(5, 1, 3, 6, 3, 1, 0)


e1(5, 2, 3, 6, 3, 1, 0)












	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,1,6,3,1)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 3, 6, 1, 1, 1)


e1(0, 1, 3, 6, 1, 1, 1)


e1(0, 2, 3, 6, 1, 1, 1)











e1(1, 0, 3, 6, 1, 1, 1)


e1(1, 1, 3, 6, 1, 1, 1)


e1(1, 2, 3, 6, 1, 1, 1)











e1(2, 0, 3, 6, 1, 1, 1)


e1(2, 1, 3, 6, 1, 1, 1)


e1(2, 2, 3, 6, 1, 1, 1)











e1(3, 0, 3, 6, 1, 1, 1)


e1(3, 1, 3, 6, 1, 1, 1)


e1(3, 2, 3, 6, 1, 1, 1)











e1(4, 0, 3, 6, 1, 1, 1)


e1(4, 1, 3, 6, 1, 1, 1)


e1(4, 2, 3, 6, 1, 1, 1)











e1(5, 0, 3, 6, 1, 1, 1)


e1(5, 1, 3, 6, 1, 1, 1)


e1(5, 2, 3, 6, 1, 1, 1)












	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,1,6,6,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 1, 6, 1, 0, 0)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,1,6,1,6)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,1,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 1, 6, 6, 0, 1)












































































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,1,6,1,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 6, 1, 1, 0)

















e1(1, 0, 1, 6, 1, 1, 0)

















e1(2, 0, 1, 6, 1, 1, 0)

















e1(3, 0, 1, 6, 1, 1, 0)

















e1(4, 0, 1, 6, 1, 1, 0)

















e1(5, 0, 1, 6, 1, 1, 0)


















	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,1,6,1,1)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e1(0, 0, 1, 6, 1, 1, 1)

















e1(1, 0, 1, 6, 1, 1, 1)

















e1(2, 0, 1, 6, 1, 1, 1)

















e1(3, 0, 1, 6, 1, 1, 1)

















e1(4, 0, 1, 6, 1, 1, 1)

















e1(5, 0, 1, 6, 1, 1, 1)


















	__asm mov eax, edx __asm pop ebx __asm ret
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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 3 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 3 + 1)] __asm fadd dword ptr [ecx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 3 + 2)] __asm fadd dword ptr [ecx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (1 * 3 + 0)] __asm fadd dword ptr [ecx + 4 * (1 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 3 + 1)] __asm fadd dword ptr [ecx + 4 * (1 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 3 + 2)] __asm fadd dword ptr [ecx + 4 * (1 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (2 * 3 + 0)] __asm fadd dword ptr [ecx + 4 * (2 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 3 + 1)] __asm fadd dword ptr [ecx + 4 * (2 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 3 + 2)] __asm fadd dword ptr [ecx + 4 * (2 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (3 * 3 + 0)] __asm fadd dword ptr [ecx + 4 * (3 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (3 * 3 + 1)] __asm fadd dword ptr [ecx + 4 * (3 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (3 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (3 * 3 + 2)] __asm fadd dword ptr [ecx + 4 * (3 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (3 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (4 * 3 + 0)] __asm fadd dword ptr [ecx + 4 * (4 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (4 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (4 * 3 + 1)] __asm fadd dword ptr [ecx + 4 * (4 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (4 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (4 * 3 + 2)] __asm fadd dword ptr [ecx + 4 * (4 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (4 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (5 * 3 + 0)] __asm fadd dword ptr [ecx + 4 * (5 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (5 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (5 * 3 + 1)] __asm fadd dword ptr [ecx + 4 * (5 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (5 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (5 * 3 + 2)] __asm fadd dword ptr [ecx + 4 * (5 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (5 * 3 + 2)]












	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,6,3)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,6,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 3 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 3 + 1)] __asm fsub dword ptr [ecx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 3 + 2)] __asm fsub dword ptr [ecx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (1 * 3 + 0)] __asm fsub dword ptr [ecx + 4 * (1 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 3 + 1)] __asm fsub dword ptr [ecx + 4 * (1 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 3 + 2)] __asm fsub dword ptr [ecx + 4 * (1 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (2 * 3 + 0)] __asm fsub dword ptr [ecx + 4 * (2 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 3 + 1)] __asm fsub dword ptr [ecx + 4 * (2 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 3 + 2)] __asm fsub dword ptr [ecx + 4 * (2 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (3 * 3 + 0)] __asm fsub dword ptr [ecx + 4 * (3 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (3 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (3 * 3 + 1)] __asm fsub dword ptr [ecx + 4 * (3 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (3 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (3 * 3 + 2)] __asm fsub dword ptr [ecx + 4 * (3 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (3 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (4 * 3 + 0)] __asm fsub dword ptr [ecx + 4 * (4 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (4 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (4 * 3 + 1)] __asm fsub dword ptr [ecx + 4 * (4 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (4 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (4 * 3 + 2)] __asm fsub dword ptr [ecx + 4 * (4 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (4 * 3 + 2)]











__asm fld dword ptr [ebx + 4 * (5 * 3 + 0)] __asm fsub dword ptr [ecx + 4 * (5 * 3 + 0)] __asm fstp dword ptr [eax + 4 * (5 * 3 + 0)]


__asm fld dword ptr [ebx + 4 * (5 * 3 + 1)] __asm fsub dword ptr [ecx + 4 * (5 * 3 + 1)] __asm fstp dword ptr [eax + 4 * (5 * 3 + 1)]


__asm fld dword ptr [ebx + 4 * (5 * 3 + 2)] __asm fsub dword ptr [ecx + 4 * (5 * 3 + 2)] __asm fstp dword ptr [eax + 4 * (5 * 3 + 2)]












	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,6,3)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 3 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 3 + 1)] __asm fadd dword ptr [edx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 3 + 2)] __asm fadd dword ptr [edx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (1 * 3 + 0)] __asm fadd dword ptr [edx + 4 * (1 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 3 + 1)] __asm fadd dword ptr [edx + 4 * (1 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 3 + 2)] __asm fadd dword ptr [edx + 4 * (1 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (2 * 3 + 0)] __asm fadd dword ptr [edx + 4 * (2 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 3 + 1)] __asm fadd dword ptr [edx + 4 * (2 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 3 + 2)] __asm fadd dword ptr [edx + 4 * (2 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (3 * 3 + 0)] __asm fadd dword ptr [edx + 4 * (3 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (3 * 3 + 1)] __asm fadd dword ptr [edx + 4 * (3 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (3 * 3 + 2)] __asm fadd dword ptr [edx + 4 * (3 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (4 * 3 + 0)] __asm fadd dword ptr [edx + 4 * (4 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (4 * 3 + 1)] __asm fadd dword ptr [edx + 4 * (4 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (4 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (4 * 3 + 2)] __asm fadd dword ptr [edx + 4 * (4 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (4 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (5 * 3 + 0)] __asm fadd dword ptr [edx + 4 * (5 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (5 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (5 * 3 + 1)] __asm fadd dword ptr [edx + 4 * (5 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (5 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (5 * 3 + 2)] __asm fadd dword ptr [edx + 4 * (5 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (5 * 3 + 2)]















	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,6,3)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 3 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 3 + 1)] __asm fsub dword ptr [edx + 4 * (0 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 3 + 2)] __asm fsub dword ptr [edx + 4 * (0 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (1 * 3 + 0)] __asm fsub dword ptr [edx + 4 * (1 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 3 + 1)] __asm fsub dword ptr [edx + 4 * (1 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 3 + 2)] __asm fsub dword ptr [edx + 4 * (1 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (2 * 3 + 0)] __asm fsub dword ptr [edx + 4 * (2 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 3 + 1)] __asm fsub dword ptr [edx + 4 * (2 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 3 + 2)] __asm fsub dword ptr [edx + 4 * (2 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (3 * 3 + 0)] __asm fsub dword ptr [edx + 4 * (3 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (3 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (3 * 3 + 1)] __asm fsub dword ptr [edx + 4 * (3 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (3 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (3 * 3 + 2)] __asm fsub dword ptr [edx + 4 * (3 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (3 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (4 * 3 + 0)] __asm fsub dword ptr [edx + 4 * (4 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (4 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (4 * 3 + 1)] __asm fsub dword ptr [edx + 4 * (4 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (4 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (4 * 3 + 2)] __asm fsub dword ptr [edx + 4 * (4 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (4 * 3 + 2)]











__asm fld dword ptr [ecx + 4 * (5 * 3 + 0)] __asm fsub dword ptr [edx + 4 * (5 * 3 + 0)] __asm fstp dword ptr [ecx + 4 * (5 * 3 + 0)]


__asm fld dword ptr [ecx + 4 * (5 * 3 + 1)] __asm fsub dword ptr [edx + 4 * (5 * 3 + 1)] __asm fstp dword ptr [ecx + 4 * (5 * 3 + 1)]


__asm fld dword ptr [ecx + 4 * (5 * 3 + 2)] __asm fsub dword ptr [edx + 4 * (5 * 3 + 2)] __asm fstp dword ptr [ecx + 4 * (5 * 3 + 2)]















	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(Assign,6,3)(SMLXMatrix &A, const SMLXMatrix &B) 







{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax


























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

























































	
	

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,6,3,3,6)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 6, 3, 6, 0, 0)


e3(0, 1, 6, 3, 6, 0, 0)


e3(0, 2, 6, 3, 6, 0, 0)


e3(0, 3, 6, 3, 6, 0, 0)


e3(0, 4, 6, 3, 6, 0, 0)


e3(0, 5, 6, 3, 6, 0, 0)


e3(1, 0, 6, 3, 6, 0, 0)


e3(1, 1, 6, 3, 6, 0, 0)


e3(1, 2, 6, 3, 6, 0, 0)


e3(1, 3, 6, 3, 6, 0, 0)


e3(1, 4, 6, 3, 6, 0, 0)


e3(1, 5, 6, 3, 6, 0, 0)


e3(2, 0, 6, 3, 6, 0, 0)


e3(2, 1, 6, 3, 6, 0, 0)


e3(2, 2, 6, 3, 6, 0, 0)


e3(2, 3, 6, 3, 6, 0, 0)


e3(2, 4, 6, 3, 6, 0, 0)


e3(2, 5, 6, 3, 6, 0, 0)


e3(3, 0, 6, 3, 6, 0, 0)


e3(3, 1, 6, 3, 6, 0, 0)


e3(3, 2, 6, 3, 6, 0, 0)


e3(3, 3, 6, 3, 6, 0, 0)


e3(3, 4, 6, 3, 6, 0, 0)


e3(3, 5, 6, 3, 6, 0, 0)


e3(4, 0, 6, 3, 6, 0, 0)


e3(4, 1, 6, 3, 6, 0, 0)


e3(4, 2, 6, 3, 6, 0, 0)


e3(4, 3, 6, 3, 6, 0, 0)


e3(4, 4, 6, 3, 6, 0, 0)


e3(4, 5, 6, 3, 6, 0, 0)


e3(5, 0, 6, 3, 6, 0, 0)


e3(5, 1, 6, 3, 6, 0, 0)


e3(5, 2, 6, 3, 6, 0, 0)


e3(5, 3, 6, 3, 6, 0, 0)


e3(5, 4, 6, 3, 6, 0, 0)


e3(5, 5, 6, 3, 6, 0, 0)



	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,6,3,6,3)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 6, 3, 3, 0, 1)


e3(0, 1, 6, 3, 3, 0, 1)


e3(0, 2, 6, 3, 3, 0, 1)


e3(0, 3, 6, 3, 3, 0, 1)


e3(0, 4, 6, 3, 3, 0, 1)


e3(0, 5, 6, 3, 3, 0, 1)


e3(1, 0, 6, 3, 3, 0, 1)


e3(1, 1, 6, 3, 3, 0, 1)


e3(1, 2, 6, 3, 3, 0, 1)


e3(1, 3, 6, 3, 3, 0, 1)


e3(1, 4, 6, 3, 3, 0, 1)


e3(1, 5, 6, 3, 3, 0, 1)


e3(2, 0, 6, 3, 3, 0, 1)


e3(2, 1, 6, 3, 3, 0, 1)


e3(2, 2, 6, 3, 3, 0, 1)


e3(2, 3, 6, 3, 3, 0, 1)


e3(2, 4, 6, 3, 3, 0, 1)


e3(2, 5, 6, 3, 3, 0, 1)


e3(3, 0, 6, 3, 3, 0, 1)


e3(3, 1, 6, 3, 3, 0, 1)


e3(3, 2, 6, 3, 3, 0, 1)


e3(3, 3, 6, 3, 3, 0, 1)


e3(3, 4, 6, 3, 3, 0, 1)


e3(3, 5, 6, 3, 3, 0, 1)


e3(4, 0, 6, 3, 3, 0, 1)


e3(4, 1, 6, 3, 3, 0, 1)


e3(4, 2, 6, 3, 3, 0, 1)


e3(4, 3, 6, 3, 3, 0, 1)


e3(4, 4, 6, 3, 3, 0, 1)


e3(4, 5, 6, 3, 3, 0, 1)


e3(5, 0, 6, 3, 3, 0, 1)


e3(5, 1, 6, 3, 3, 0, 1)


e3(5, 2, 6, 3, 3, 0, 1)


e3(5, 3, 6, 3, 3, 0, 1)


e3(5, 4, 6, 3, 3, 0, 1)


e3(5, 5, 6, 3, 3, 0, 1)



	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,6,3,6,6)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 6, 3, 6, 1, 0)


e6(0, 1, 6, 3, 6, 1, 0)


e6(0, 2, 6, 3, 6, 1, 0)


e6(0, 3, 6, 3, 6, 1, 0)


e6(0, 4, 6, 3, 6, 1, 0)


e6(0, 5, 6, 3, 6, 1, 0)


e6(1, 0, 6, 3, 6, 1, 0)


e6(1, 1, 6, 3, 6, 1, 0)


e6(1, 2, 6, 3, 6, 1, 0)


e6(1, 3, 6, 3, 6, 1, 0)


e6(1, 4, 6, 3, 6, 1, 0)


e6(1, 5, 6, 3, 6, 1, 0)


e6(2, 0, 6, 3, 6, 1, 0)


e6(2, 1, 6, 3, 6, 1, 0)


e6(2, 2, 6, 3, 6, 1, 0)


e6(2, 3, 6, 3, 6, 1, 0)


e6(2, 4, 6, 3, 6, 1, 0)


e6(2, 5, 6, 3, 6, 1, 0)

























































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,6,3,6,6)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 6, 3, 6, 1, 1)


e6(0, 1, 6, 3, 6, 1, 1)


e6(0, 2, 6, 3, 6, 1, 1)


e6(0, 3, 6, 3, 6, 1, 1)


e6(0, 4, 6, 3, 6, 1, 1)


e6(0, 5, 6, 3, 6, 1, 1)


e6(1, 0, 6, 3, 6, 1, 1)


e6(1, 1, 6, 3, 6, 1, 1)


e6(1, 2, 6, 3, 6, 1, 1)


e6(1, 3, 6, 3, 6, 1, 1)


e6(1, 4, 6, 3, 6, 1, 1)


e6(1, 5, 6, 3, 6, 1, 1)


e6(2, 0, 6, 3, 6, 1, 1)


e6(2, 1, 6, 3, 6, 1, 1)


e6(2, 2, 6, 3, 6, 1, 1)


e6(2, 3, 6, 3, 6, 1, 1)


e6(2, 4, 6, 3, 6, 1, 1)


e6(2, 5, 6, 3, 6, 1, 1)

























































	__asm mov eax, edx __asm pop ebx __asm ret
}























































	
	




























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,6,3,3,5)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 5, 3, 5, 0, 0)


e3(0, 1, 5, 3, 5, 0, 0)


e3(0, 2, 5, 3, 5, 0, 0)


e3(0, 3, 5, 3, 5, 0, 0)


e3(0, 4, 5, 3, 5, 0, 0)





e3(1, 0, 5, 3, 5, 0, 0)


e3(1, 1, 5, 3, 5, 0, 0)


e3(1, 2, 5, 3, 5, 0, 0)


e3(1, 3, 5, 3, 5, 0, 0)


e3(1, 4, 5, 3, 5, 0, 0)





e3(2, 0, 5, 3, 5, 0, 0)


e3(2, 1, 5, 3, 5, 0, 0)


e3(2, 2, 5, 3, 5, 0, 0)


e3(2, 3, 5, 3, 5, 0, 0)


e3(2, 4, 5, 3, 5, 0, 0)





e3(3, 0, 5, 3, 5, 0, 0)


e3(3, 1, 5, 3, 5, 0, 0)


e3(3, 2, 5, 3, 5, 0, 0)


e3(3, 3, 5, 3, 5, 0, 0)


e3(3, 4, 5, 3, 5, 0, 0)





e3(4, 0, 5, 3, 5, 0, 0)


e3(4, 1, 5, 3, 5, 0, 0)


e3(4, 2, 5, 3, 5, 0, 0)


e3(4, 3, 5, 3, 5, 0, 0)


e3(4, 4, 5, 3, 5, 0, 0)





e3(5, 0, 5, 3, 5, 0, 0)


e3(5, 1, 5, 3, 5, 0, 0)


e3(5, 2, 5, 3, 5, 0, 0)


e3(5, 3, 5, 3, 5, 0, 0)


e3(5, 4, 5, 3, 5, 0, 0)






	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,6,3,5,3)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 5, 3, 3, 0, 1)


e3(0, 1, 5, 3, 3, 0, 1)


e3(0, 2, 5, 3, 3, 0, 1)


e3(0, 3, 5, 3, 3, 0, 1)


e3(0, 4, 5, 3, 3, 0, 1)





e3(1, 0, 5, 3, 3, 0, 1)


e3(1, 1, 5, 3, 3, 0, 1)


e3(1, 2, 5, 3, 3, 0, 1)


e3(1, 3, 5, 3, 3, 0, 1)


e3(1, 4, 5, 3, 3, 0, 1)





e3(2, 0, 5, 3, 3, 0, 1)


e3(2, 1, 5, 3, 3, 0, 1)


e3(2, 2, 5, 3, 3, 0, 1)


e3(2, 3, 5, 3, 3, 0, 1)


e3(2, 4, 5, 3, 3, 0, 1)





e3(3, 0, 5, 3, 3, 0, 1)


e3(3, 1, 5, 3, 3, 0, 1)


e3(3, 2, 5, 3, 3, 0, 1)


e3(3, 3, 5, 3, 3, 0, 1)


e3(3, 4, 5, 3, 3, 0, 1)





e3(4, 0, 5, 3, 3, 0, 1)


e3(4, 1, 5, 3, 3, 0, 1)


e3(4, 2, 5, 3, 3, 0, 1)


e3(4, 3, 5, 3, 3, 0, 1)


e3(4, 4, 5, 3, 3, 0, 1)





e3(5, 0, 5, 3, 3, 0, 1)


e3(5, 1, 5, 3, 3, 0, 1)


e3(5, 2, 5, 3, 3, 0, 1)


e3(5, 3, 5, 3, 3, 0, 1)


e3(5, 4, 5, 3, 3, 0, 1)






	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,6,3,6,5)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 5, 3, 5, 1, 0)


e6(0, 1, 5, 3, 5, 1, 0)


e6(0, 2, 5, 3, 5, 1, 0)


e6(0, 3, 5, 3, 5, 1, 0)


e6(0, 4, 5, 3, 5, 1, 0)





e6(1, 0, 5, 3, 5, 1, 0)


e6(1, 1, 5, 3, 5, 1, 0)


e6(1, 2, 5, 3, 5, 1, 0)


e6(1, 3, 5, 3, 5, 1, 0)


e6(1, 4, 5, 3, 5, 1, 0)





e6(2, 0, 5, 3, 5, 1, 0)


e6(2, 1, 5, 3, 5, 1, 0)


e6(2, 2, 5, 3, 5, 1, 0)


e6(2, 3, 5, 3, 5, 1, 0)


e6(2, 4, 5, 3, 5, 1, 0)




























































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,6,3,5,6)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 5, 3, 6, 1, 1)


e6(0, 1, 5, 3, 6, 1, 1)


e6(0, 2, 5, 3, 6, 1, 1)


e6(0, 3, 5, 3, 6, 1, 1)


e6(0, 4, 5, 3, 6, 1, 1)





e6(1, 0, 5, 3, 6, 1, 1)


e6(1, 1, 5, 3, 6, 1, 1)


e6(1, 2, 5, 3, 6, 1, 1)


e6(1, 3, 5, 3, 6, 1, 1)


e6(1, 4, 5, 3, 6, 1, 1)





e6(2, 0, 5, 3, 6, 1, 1)


e6(2, 1, 5, 3, 6, 1, 1)


e6(2, 2, 5, 3, 6, 1, 1)


e6(2, 3, 5, 3, 6, 1, 1)


e6(2, 4, 5, 3, 6, 1, 1)




























































	__asm mov eax, edx __asm pop ebx __asm ret
}

















































	
	


































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,6,3,3,3)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 3, 3, 3, 0, 0)


e3(0, 1, 3, 3, 3, 0, 0)


e3(0, 2, 3, 3, 3, 0, 0)











e3(1, 0, 3, 3, 3, 0, 0)


e3(1, 1, 3, 3, 3, 0, 0)


e3(1, 2, 3, 3, 3, 0, 0)











e3(2, 0, 3, 3, 3, 0, 0)


e3(2, 1, 3, 3, 3, 0, 0)


e3(2, 2, 3, 3, 3, 0, 0)











e3(3, 0, 3, 3, 3, 0, 0)


e3(3, 1, 3, 3, 3, 0, 0)


e3(3, 2, 3, 3, 3, 0, 0)











e3(4, 0, 3, 3, 3, 0, 0)


e3(4, 1, 3, 3, 3, 0, 0)


e3(4, 2, 3, 3, 3, 0, 0)











e3(5, 0, 3, 3, 3, 0, 0)


e3(5, 1, 3, 3, 3, 0, 0)


e3(5, 2, 3, 3, 3, 0, 0)












	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,6,3,3,3)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 3, 3, 3, 0, 1)


e3(0, 1, 3, 3, 3, 0, 1)


e3(0, 2, 3, 3, 3, 0, 1)











e3(1, 0, 3, 3, 3, 0, 1)


e3(1, 1, 3, 3, 3, 0, 1)


e3(1, 2, 3, 3, 3, 0, 1)











e3(2, 0, 3, 3, 3, 0, 1)


e3(2, 1, 3, 3, 3, 0, 1)


e3(2, 2, 3, 3, 3, 0, 1)











e3(3, 0, 3, 3, 3, 0, 1)


e3(3, 1, 3, 3, 3, 0, 1)


e3(3, 2, 3, 3, 3, 0, 1)











e3(4, 0, 3, 3, 3, 0, 1)


e3(4, 1, 3, 3, 3, 0, 1)


e3(4, 2, 3, 3, 3, 0, 1)











e3(5, 0, 3, 3, 3, 0, 1)


e3(5, 1, 3, 3, 3, 0, 1)


e3(5, 2, 3, 3, 3, 0, 1)












	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,6,3,6,3)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 3, 3, 3, 1, 0)


e6(0, 1, 3, 3, 3, 1, 0)


e6(0, 2, 3, 3, 3, 1, 0)











e6(1, 0, 3, 3, 3, 1, 0)


e6(1, 1, 3, 3, 3, 1, 0)


e6(1, 2, 3, 3, 3, 1, 0)











e6(2, 0, 3, 3, 3, 1, 0)


e6(2, 1, 3, 3, 3, 1, 0)


e6(2, 2, 3, 3, 3, 1, 0)


































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,6,3,3,6)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 3, 3, 6, 1, 1)


e6(0, 1, 3, 3, 6, 1, 1)


e6(0, 2, 3, 3, 6, 1, 1)











e6(1, 0, 3, 3, 6, 1, 1)


e6(1, 1, 3, 3, 6, 1, 1)


e6(1, 2, 3, 3, 6, 1, 1)











e6(2, 0, 3, 3, 6, 1, 1)


e6(2, 1, 3, 3, 6, 1, 1)


e6(2, 2, 3, 3, 6, 1, 1)


































































	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,6,3,3,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 1, 3, 1, 0, 0)

















e3(1, 0, 1, 3, 1, 0, 0)

















e3(2, 0, 1, 3, 1, 0, 0)

















e3(3, 0, 1, 3, 1, 0, 0)

















e3(4, 0, 1, 3, 1, 0, 0)

















e3(5, 0, 1, 3, 1, 0, 0)


















	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,6,3,1,3)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 1, 3, 3, 0, 1)

















e3(1, 0, 1, 3, 3, 0, 1)

















e3(2, 0, 1, 3, 3, 0, 1)

















e3(3, 0, 1, 3, 3, 0, 1)

















e3(4, 0, 1, 3, 3, 0, 1)

















e3(5, 0, 1, 3, 3, 0, 1)


















	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,6,3,6,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 1, 3, 1, 1, 0)

















e6(1, 0, 1, 3, 1, 1, 0)

















e6(2, 0, 1, 3, 1, 1, 0)








































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,6,3,1,6)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 1, 3, 6, 1, 1)

















e6(1, 0, 1, 3, 6, 1, 1)

















e6(2, 0, 1, 3, 6, 1, 1)








































































	__asm mov eax, edx __asm pop ebx __asm ret
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
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 6 + 0)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 1)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 2)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 3)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 4)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 5)] __asm fadd dword ptr [ecx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 0)] __asm fadd dword ptr [ecx + 4 * (1 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 1)] __asm fadd dword ptr [ecx + 4 * (1 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 2)] __asm fadd dword ptr [ecx + 4 * (1 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 3)] __asm fadd dword ptr [ecx + 4 * (1 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 4)] __asm fadd dword ptr [ecx + 4 * (1 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 5)] __asm fadd dword ptr [ecx + 4 * (1 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 0)] __asm fadd dword ptr [ecx + 4 * (2 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 1)] __asm fadd dword ptr [ecx + 4 * (2 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 2)] __asm fadd dword ptr [ecx + 4 * (2 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 3)] __asm fadd dword ptr [ecx + 4 * (2 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 4)] __asm fadd dword ptr [ecx + 4 * (2 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 5)] __asm fadd dword ptr [ecx + 4 * (2 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 5)]

























































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
SMLXTmpMatrix& __fastcall nrxc(Sub,3,6)(const SMLXMatrix &A, const SMLXMatrix &B) 








{
	nrxc(TMat,3,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























__asm fld dword ptr [ebx + 4 * (0 * 6 + 0)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 1)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 2)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 3)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 4)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (0 * 6 + 5)] __asm fsub dword ptr [ecx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (0 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 0)] __asm fsub dword ptr [ecx + 4 * (1 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 1)] __asm fsub dword ptr [ecx + 4 * (1 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 2)] __asm fsub dword ptr [ecx + 4 * (1 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 3)] __asm fsub dword ptr [ecx + 4 * (1 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 4)] __asm fsub dword ptr [ecx + 4 * (1 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (1 * 6 + 5)] __asm fsub dword ptr [ecx + 4 * (1 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (1 * 6 + 5)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 0)] __asm fsub dword ptr [ecx + 4 * (2 * 6 + 0)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 0)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 1)] __asm fsub dword ptr [ecx + 4 * (2 * 6 + 1)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 1)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 2)] __asm fsub dword ptr [ecx + 4 * (2 * 6 + 2)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 2)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 3)] __asm fsub dword ptr [ecx + 4 * (2 * 6 + 3)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 3)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 4)] __asm fsub dword ptr [ecx + 4 * (2 * 6 + 4)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 4)]


__asm fld dword ptr [ebx + 4 * (2 * 6 + 5)] __asm fsub dword ptr [ecx + 4 * (2 * 6 + 5)] __asm fstp dword ptr [eax + 4 * (2 * 6 + 5)]

























































	__asm mov eax, edx __asm pop ebx __asm ret
}


__declspec(naked)
void __fastcall nrxc(AddTo,3,6)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 6 + 0)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 1)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 2)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 3)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 4)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 5)] __asm fadd dword ptr [edx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 0)] __asm fadd dword ptr [edx + 4 * (1 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 1)] __asm fadd dword ptr [edx + 4 * (1 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 2)] __asm fadd dword ptr [edx + 4 * (1 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 3)] __asm fadd dword ptr [edx + 4 * (1 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 4)] __asm fadd dword ptr [edx + 4 * (1 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 5)] __asm fadd dword ptr [edx + 4 * (1 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 0)] __asm fadd dword ptr [edx + 4 * (2 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 1)] __asm fadd dword ptr [edx + 4 * (2 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 2)] __asm fadd dword ptr [edx + 4 * (2 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 3)] __asm fadd dword ptr [edx + 4 * (2 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 4)] __asm fadd dword ptr [edx + 4 * (2 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 5)] __asm fadd dword ptr [edx + 4 * (2 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 5)]




























































	CLEAR_SUBEXPRESSION_COUNT

	__asm ret
}


__declspec(naked)
void __fastcall nrxc(SubFrom,3,6)(SMLXMatrix &A, const SMLXMatrix &B) 








{
	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

























__asm fld dword ptr [ecx + 4 * (0 * 6 + 0)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 1)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 2)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 3)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 4)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (0 * 6 + 5)] __asm fsub dword ptr [edx + 4 * (0 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (0 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 0)] __asm fsub dword ptr [edx + 4 * (1 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 1)] __asm fsub dword ptr [edx + 4 * (1 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 2)] __asm fsub dword ptr [edx + 4 * (1 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 3)] __asm fsub dword ptr [edx + 4 * (1 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 4)] __asm fsub dword ptr [edx + 4 * (1 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (1 * 6 + 5)] __asm fsub dword ptr [edx + 4 * (1 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (1 * 6 + 5)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 0)] __asm fsub dword ptr [edx + 4 * (2 * 6 + 0)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 0)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 1)] __asm fsub dword ptr [edx + 4 * (2 * 6 + 1)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 1)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 2)] __asm fsub dword ptr [edx + 4 * (2 * 6 + 2)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 2)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 3)] __asm fsub dword ptr [edx + 4 * (2 * 6 + 3)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 3)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 4)] __asm fsub dword ptr [edx + 4 * (2 * 6 + 4)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 4)]


__asm fld dword ptr [ecx + 4 * (2 * 6 + 5)] __asm fsub dword ptr [edx + 4 * (2 * 6 + 5)] __asm fstp dword ptr [ecx + 4 * (2 * 6 + 5)]




























































	CLEAR_SUBEXPRESSION_COUNT
	__asm ret
}


__declspec(naked)
void __fastcall nrxc(Assign,3,6)(SMLXMatrix &A, const SMLXMatrix &B) 







{
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

























































	
	

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,3,6,6,6)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 6, 6, 6, 0, 0)


e6(0, 1, 6, 6, 6, 0, 0)


e6(0, 2, 6, 6, 6, 0, 0)


e6(0, 3, 6, 6, 6, 0, 0)


e6(0, 4, 6, 6, 6, 0, 0)


e6(0, 5, 6, 6, 6, 0, 0)


e6(1, 0, 6, 6, 6, 0, 0)


e6(1, 1, 6, 6, 6, 0, 0)


e6(1, 2, 6, 6, 6, 0, 0)


e6(1, 3, 6, 6, 6, 0, 0)


e6(1, 4, 6, 6, 6, 0, 0)


e6(1, 5, 6, 6, 6, 0, 0)


e6(2, 0, 6, 6, 6, 0, 0)


e6(2, 1, 6, 6, 6, 0, 0)


e6(2, 2, 6, 6, 6, 0, 0)


e6(2, 3, 6, 6, 6, 0, 0)


e6(2, 4, 6, 6, 6, 0, 0)


e6(2, 5, 6, 6, 6, 0, 0)

























































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,3,6,6,6)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 6, 6, 6, 0, 1)


e6(0, 1, 6, 6, 6, 0, 1)


e6(0, 2, 6, 6, 6, 0, 1)


e6(0, 3, 6, 6, 6, 0, 1)


e6(0, 4, 6, 6, 6, 0, 1)


e6(0, 5, 6, 6, 6, 0, 1)


e6(1, 0, 6, 6, 6, 0, 1)


e6(1, 1, 6, 6, 6, 0, 1)


e6(1, 2, 6, 6, 6, 0, 1)


e6(1, 3, 6, 6, 6, 0, 1)


e6(1, 4, 6, 6, 6, 0, 1)


e6(1, 5, 6, 6, 6, 0, 1)


e6(2, 0, 6, 6, 6, 0, 1)


e6(2, 1, 6, 6, 6, 0, 1)


e6(2, 2, 6, 6, 6, 0, 1)


e6(2, 3, 6, 6, 6, 0, 1)


e6(2, 4, 6, 6, 6, 0, 1)


e6(2, 5, 6, 6, 6, 0, 1)

























































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,3,6,3,6)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 6, 6, 6, 1, 0)


e3(0, 1, 6, 6, 6, 1, 0)


e3(0, 2, 6, 6, 6, 1, 0)


e3(0, 3, 6, 6, 6, 1, 0)


e3(0, 4, 6, 6, 6, 1, 0)


e3(0, 5, 6, 6, 6, 1, 0)


e3(1, 0, 6, 6, 6, 1, 0)


e3(1, 1, 6, 6, 6, 1, 0)


e3(1, 2, 6, 6, 6, 1, 0)


e3(1, 3, 6, 6, 6, 1, 0)


e3(1, 4, 6, 6, 6, 1, 0)


e3(1, 5, 6, 6, 6, 1, 0)


e3(2, 0, 6, 6, 6, 1, 0)


e3(2, 1, 6, 6, 6, 1, 0)


e3(2, 2, 6, 6, 6, 1, 0)


e3(2, 3, 6, 6, 6, 1, 0)


e3(2, 4, 6, 6, 6, 1, 0)


e3(2, 5, 6, 6, 6, 1, 0)


e3(3, 0, 6, 6, 6, 1, 0)


e3(3, 1, 6, 6, 6, 1, 0)


e3(3, 2, 6, 6, 6, 1, 0)


e3(3, 3, 6, 6, 6, 1, 0)


e3(3, 4, 6, 6, 6, 1, 0)


e3(3, 5, 6, 6, 6, 1, 0)


e3(4, 0, 6, 6, 6, 1, 0)


e3(4, 1, 6, 6, 6, 1, 0)


e3(4, 2, 6, 6, 6, 1, 0)


e3(4, 3, 6, 6, 6, 1, 0)


e3(4, 4, 6, 6, 6, 1, 0)


e3(4, 5, 6, 6, 6, 1, 0)


e3(5, 0, 6, 6, 6, 1, 0)


e3(5, 1, 6, 6, 6, 1, 0)


e3(5, 2, 6, 6, 6, 1, 0)


e3(5, 3, 6, 6, 6, 1, 0)


e3(5, 4, 6, 6, 6, 1, 0)


e3(5, 5, 6, 6, 6, 1, 0)



	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,3,6,6,3)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,6)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 6, 6, 3, 1, 1)


e3(0, 1, 6, 6, 3, 1, 1)


e3(0, 2, 6, 6, 3, 1, 1)


e3(0, 3, 6, 6, 3, 1, 1)


e3(0, 4, 6, 6, 3, 1, 1)


e3(0, 5, 6, 6, 3, 1, 1)


e3(1, 0, 6, 6, 3, 1, 1)


e3(1, 1, 6, 6, 3, 1, 1)


e3(1, 2, 6, 6, 3, 1, 1)


e3(1, 3, 6, 6, 3, 1, 1)


e3(1, 4, 6, 6, 3, 1, 1)


e3(1, 5, 6, 6, 3, 1, 1)


e3(2, 0, 6, 6, 3, 1, 1)


e3(2, 1, 6, 6, 3, 1, 1)


e3(2, 2, 6, 6, 3, 1, 1)


e3(2, 3, 6, 6, 3, 1, 1)


e3(2, 4, 6, 6, 3, 1, 1)


e3(2, 5, 6, 6, 3, 1, 1)


e3(3, 0, 6, 6, 3, 1, 1)


e3(3, 1, 6, 6, 3, 1, 1)


e3(3, 2, 6, 6, 3, 1, 1)


e3(3, 3, 6, 6, 3, 1, 1)


e3(3, 4, 6, 6, 3, 1, 1)


e3(3, 5, 6, 6, 3, 1, 1)


e3(4, 0, 6, 6, 3, 1, 1)


e3(4, 1, 6, 6, 3, 1, 1)


e3(4, 2, 6, 6, 3, 1, 1)


e3(4, 3, 6, 6, 3, 1, 1)


e3(4, 4, 6, 6, 3, 1, 1)


e3(4, 5, 6, 6, 3, 1, 1)


e3(5, 0, 6, 6, 3, 1, 1)


e3(5, 1, 6, 6, 3, 1, 1)


e3(5, 2, 6, 6, 3, 1, 1)


e3(5, 3, 6, 6, 3, 1, 1)


e3(5, 4, 6, 6, 3, 1, 1)


e3(5, 5, 6, 6, 3, 1, 1)



	__asm mov eax, edx __asm pop ebx __asm ret
}























































	
	




























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,3,6,6,5)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 5, 6, 5, 0, 0)


e6(0, 1, 5, 6, 5, 0, 0)


e6(0, 2, 5, 6, 5, 0, 0)


e6(0, 3, 5, 6, 5, 0, 0)


e6(0, 4, 5, 6, 5, 0, 0)





e6(1, 0, 5, 6, 5, 0, 0)


e6(1, 1, 5, 6, 5, 0, 0)


e6(1, 2, 5, 6, 5, 0, 0)


e6(1, 3, 5, 6, 5, 0, 0)


e6(1, 4, 5, 6, 5, 0, 0)





e6(2, 0, 5, 6, 5, 0, 0)


e6(2, 1, 5, 6, 5, 0, 0)


e6(2, 2, 5, 6, 5, 0, 0)


e6(2, 3, 5, 6, 5, 0, 0)


e6(2, 4, 5, 6, 5, 0, 0)




























































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,3,6,5,6)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 5, 6, 6, 0, 1)


e6(0, 1, 5, 6, 6, 0, 1)


e6(0, 2, 5, 6, 6, 0, 1)


e6(0, 3, 5, 6, 6, 0, 1)


e6(0, 4, 5, 6, 6, 0, 1)





e6(1, 0, 5, 6, 6, 0, 1)


e6(1, 1, 5, 6, 6, 0, 1)


e6(1, 2, 5, 6, 6, 0, 1)


e6(1, 3, 5, 6, 6, 0, 1)


e6(1, 4, 5, 6, 6, 0, 1)





e6(2, 0, 5, 6, 6, 0, 1)


e6(2, 1, 5, 6, 6, 0, 1)


e6(2, 2, 5, 6, 6, 0, 1)


e6(2, 3, 5, 6, 6, 0, 1)


e6(2, 4, 5, 6, 6, 0, 1)




























































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,3,6,3,5)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 5, 6, 5, 1, 0)


e3(0, 1, 5, 6, 5, 1, 0)


e3(0, 2, 5, 6, 5, 1, 0)


e3(0, 3, 5, 6, 5, 1, 0)


e3(0, 4, 5, 6, 5, 1, 0)





e3(1, 0, 5, 6, 5, 1, 0)


e3(1, 1, 5, 6, 5, 1, 0)


e3(1, 2, 5, 6, 5, 1, 0)


e3(1, 3, 5, 6, 5, 1, 0)


e3(1, 4, 5, 6, 5, 1, 0)





e3(2, 0, 5, 6, 5, 1, 0)


e3(2, 1, 5, 6, 5, 1, 0)


e3(2, 2, 5, 6, 5, 1, 0)


e3(2, 3, 5, 6, 5, 1, 0)


e3(2, 4, 5, 6, 5, 1, 0)





e3(3, 0, 5, 6, 5, 1, 0)


e3(3, 1, 5, 6, 5, 1, 0)


e3(3, 2, 5, 6, 5, 1, 0)


e3(3, 3, 5, 6, 5, 1, 0)


e3(3, 4, 5, 6, 5, 1, 0)





e3(4, 0, 5, 6, 5, 1, 0)


e3(4, 1, 5, 6, 5, 1, 0)


e3(4, 2, 5, 6, 5, 1, 0)


e3(4, 3, 5, 6, 5, 1, 0)


e3(4, 4, 5, 6, 5, 1, 0)





e3(5, 0, 5, 6, 5, 1, 0)


e3(5, 1, 5, 6, 5, 1, 0)


e3(5, 2, 5, 6, 5, 1, 0)


e3(5, 3, 5, 6, 5, 1, 0)


e3(5, 4, 5, 6, 5, 1, 0)






	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,3,6,5,3)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,5)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 5, 6, 3, 1, 1)


e3(0, 1, 5, 6, 3, 1, 1)


e3(0, 2, 5, 6, 3, 1, 1)


e3(0, 3, 5, 6, 3, 1, 1)


e3(0, 4, 5, 6, 3, 1, 1)





e3(1, 0, 5, 6, 3, 1, 1)


e3(1, 1, 5, 6, 3, 1, 1)


e3(1, 2, 5, 6, 3, 1, 1)


e3(1, 3, 5, 6, 3, 1, 1)


e3(1, 4, 5, 6, 3, 1, 1)





e3(2, 0, 5, 6, 3, 1, 1)


e3(2, 1, 5, 6, 3, 1, 1)


e3(2, 2, 5, 6, 3, 1, 1)


e3(2, 3, 5, 6, 3, 1, 1)


e3(2, 4, 5, 6, 3, 1, 1)





e3(3, 0, 5, 6, 3, 1, 1)


e3(3, 1, 5, 6, 3, 1, 1)


e3(3, 2, 5, 6, 3, 1, 1)


e3(3, 3, 5, 6, 3, 1, 1)


e3(3, 4, 5, 6, 3, 1, 1)





e3(4, 0, 5, 6, 3, 1, 1)


e3(4, 1, 5, 6, 3, 1, 1)


e3(4, 2, 5, 6, 3, 1, 1)


e3(4, 3, 5, 6, 3, 1, 1)


e3(4, 4, 5, 6, 3, 1, 1)





e3(5, 0, 5, 6, 3, 1, 1)


e3(5, 1, 5, 6, 3, 1, 1)


e3(5, 2, 5, 6, 3, 1, 1)


e3(5, 3, 5, 6, 3, 1, 1)


e3(5, 4, 5, 6, 3, 1, 1)






	__asm mov eax, edx __asm pop ebx __asm ret
}

















































	
	


































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,3,6,6,3)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 3, 6, 3, 0, 0)


e6(0, 1, 3, 6, 3, 0, 0)


e6(0, 2, 3, 6, 3, 0, 0)











e6(1, 0, 3, 6, 3, 0, 0)


e6(1, 1, 3, 6, 3, 0, 0)


e6(1, 2, 3, 6, 3, 0, 0)











e6(2, 0, 3, 6, 3, 0, 0)


e6(2, 1, 3, 6, 3, 0, 0)


e6(2, 2, 3, 6, 3, 0, 0)


































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,3,6,3,6)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 3, 6, 6, 0, 1)


e6(0, 1, 3, 6, 6, 0, 1)


e6(0, 2, 3, 6, 6, 0, 1)











e6(1, 0, 3, 6, 6, 0, 1)


e6(1, 1, 3, 6, 6, 0, 1)


e6(1, 2, 3, 6, 6, 0, 1)











e6(2, 0, 3, 6, 6, 0, 1)


e6(2, 1, 3, 6, 6, 0, 1)


e6(2, 2, 3, 6, 6, 0, 1)


































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,3,6,3,3)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 3, 6, 3, 1, 0)


e3(0, 1, 3, 6, 3, 1, 0)


e3(0, 2, 3, 6, 3, 1, 0)











e3(1, 0, 3, 6, 3, 1, 0)


e3(1, 1, 3, 6, 3, 1, 0)


e3(1, 2, 3, 6, 3, 1, 0)











e3(2, 0, 3, 6, 3, 1, 0)


e3(2, 1, 3, 6, 3, 1, 0)


e3(2, 2, 3, 6, 3, 1, 0)











e3(3, 0, 3, 6, 3, 1, 0)


e3(3, 1, 3, 6, 3, 1, 0)


e3(3, 2, 3, 6, 3, 1, 0)











e3(4, 0, 3, 6, 3, 1, 0)


e3(4, 1, 3, 6, 3, 1, 0)


e3(4, 2, 3, 6, 3, 1, 0)











e3(5, 0, 3, 6, 3, 1, 0)


e3(5, 1, 3, 6, 3, 1, 0)


e3(5, 2, 3, 6, 3, 1, 0)












	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,3,6,3,3)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,3)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 3, 6, 3, 1, 1)


e3(0, 1, 3, 6, 3, 1, 1)


e3(0, 2, 3, 6, 3, 1, 1)











e3(1, 0, 3, 6, 3, 1, 1)


e3(1, 1, 3, 6, 3, 1, 1)


e3(1, 2, 3, 6, 3, 1, 1)











e3(2, 0, 3, 6, 3, 1, 1)


e3(2, 1, 3, 6, 3, 1, 1)


e3(2, 2, 3, 6, 3, 1, 1)











e3(3, 0, 3, 6, 3, 1, 1)


e3(3, 1, 3, 6, 3, 1, 1)


e3(3, 2, 3, 6, 3, 1, 1)











e3(4, 0, 3, 6, 3, 1, 1)


e3(4, 1, 3, 6, 3, 1, 1)


e3(4, 2, 3, 6, 3, 1, 1)











e3(5, 0, 3, 6, 3, 1, 1)


e3(5, 1, 3, 6, 3, 1, 1)


e3(5, 2, 3, 6, 3, 1, 1)












	__asm mov eax, edx __asm pop ebx __asm ret
}











































	
	








































__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult00,3,6,6,1)(const SMLXMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,3,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 1, 6, 1, 0, 0)

















e6(1, 0, 1, 6, 1, 0, 0)

















e6(2, 0, 1, 6, 1, 0, 0)








































































	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult01,3,6,1,6)(const SMLXMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,3,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e6(0, 0, 1, 6, 6, 0, 1)

















e6(1, 0, 1, 6, 6, 0, 1)

















e6(2, 0, 1, 6, 6, 0, 1)








































































	__asm mov eax, edx __asm pop ebx __asm ret
}

























__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult10,3,6,3,1)(const SMLXTransposedMatrix &A, const SMLXMatrix &B)





{
	nrxc(TMat,6,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 1, 6, 1, 1, 0)

















e3(1, 0, 1, 6, 1, 1, 0)

















e3(2, 0, 1, 6, 1, 1, 0)

















e3(3, 0, 1, 6, 1, 1, 0)

















e3(4, 0, 1, 6, 1, 1, 0)

















e3(5, 0, 1, 6, 1, 1, 0)


















	__asm mov eax, edx __asm pop ebx __asm ret
}




__declspec(naked)

SMLXTmpMatrix& __fastcall n_rxc_rxc(Mult11,3,6,1,3)(const SMLXTransposedMatrix &A, const SMLXTransposedMatrix &B)





{
	nrxc(TMat,6,1)();
	__asm push ebx __asm mov ebx, dword ptr [ecx] __asm mov ecx, dword ptr [edx] __asm mov edx, eax __asm mov eax, [edx]

























e3(0, 0, 1, 6, 3, 1, 1)

















e3(1, 0, 1, 6, 3, 1, 1)

















e3(2, 0, 1, 6, 3, 1, 1)

















e3(3, 0, 1, 6, 3, 1, 1)

















e3(4, 0, 1, 6, 3, 1, 1)

















e3(5, 0, 1, 6, 3, 1, 1)


















	__asm mov eax, edx __asm pop ebx __asm ret
}

















































































































































































































































































































































































