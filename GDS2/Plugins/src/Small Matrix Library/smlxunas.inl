	__asm mov eax, dword ptr [edx] __asm mov edx, eax __asm mov eax, dword ptr [ecx] __asm mov ecx, eax

#define __count8 size/8
#define __end8 size%8
#define __count4 __end8 / 4
#define __end4 __end8%4
#define unop8(dst, src, m0, m1) \
	__asm movaps xmm ## m0, xmmword ptr [edx+src] \
	__asm movaps xmm ## m1, xmmword ptr [edx+src+16] \
	__asm movaps xmmword ptr [ecx+dst], xmm ## m0 \
	__asm movaps xmmword ptr [ecx+dst+16], xmm ## m1

#define unop4(dst, src, m0) \
	__asm movaps xmm ## m0, xmmword ptr [edx+src] \
	__asm movaps xmmword ptr [ecx+dst], xmm ## m0 

#define unop2(dst, src, m0) \
	__asm movhps xmm ## m0, qword ptr [edx+src] \
	__asm movhps qword ptr [ecx+dst], xmm ## m0 

#define unop1(dst, src) \
	__asm fld dword ptr [edx+src] \
	__asm fstp dword ptr [ecx+dst] 

#if __count8 > 0
unop8(0,0,0,1);
#endif
#if __count8 > 1
unop8(32,32,2,3);
#endif
#if __count8 > 2
unop8(64,64,4,5);
#endif
#if __count8 > 3
unop8(96,96,6,7);
#endif
#if __count4 > 0
unop4(((size - __end8)*4),((size - __end8)*4),0);
#endif

#if __end4 > 1
unop2(((size - __end4)*4),((size - __end4)*4), 1);
#endif

#if ((__end4 % 2)>0)
unop1(((size - 1)*4),((size - 1)*4));
#endif

#undef unop1
#undef unop2
#undef unop4
#undef __count8
#undef __end8
#undef __count4
#undef __end4
