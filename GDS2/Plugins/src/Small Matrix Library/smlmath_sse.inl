// Math Inlines 

//#include <smlmath.h>
#include "xmmintrin.h"

void
SMLMatrix3f::TransformPoint(const SMLVec3f &tmp, SMLVec3f &dst) const
{
	_asm {

		mov     edx,  dword ptr this
		mov     ecx,  tmp
		movss	xmm1, dword ptr [edx]
		mov     eax,  dst
		movhps	xmm1, qword ptr [edx+4]
		movaps	xmm5, xmm1
		movss	xmm3, dword ptr [edx+12]
		movhps	xmm3, qword ptr [edx+24]
		movss	xmm4, dword ptr [ecx]
		shufps	xmm5, xmm3, 128
		movlps	xmm0, qword ptr [edx+16]
		shufps	xmm4, xmm4, 0
		movhps	xmm0, qword ptr [edx+28]
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
}

void
SMLMatrix3f::TransformPoint1(const SMLVec3f &tmp, SMLVec3f &dst) const
{
//    dst.x = data[0][0] * tmp.x + data[1][0] * tmp.y + data[2][0] * tmp.z;
//    dst.y = data[0][1] * tmp.x + data[1][1] * tmp.y + data[2][1] * tmp.z;
//    dst.z = data[0][2] * tmp.x + data[1][2] * tmp.y + data[2][2] * tmp.z;

	float	*m = (float*)this;
	float	*d = (float*)&dst;
	float	*v = (float*)&tmp;

	__m128	b0, b1, b2;
	__m128	row,  rslt;

	b0		= _mm_load_ss( &m[0] );
	b0		= _mm_loadh_pi( b0, (__m64*)&m[1] );
	b1		= _mm_load_ss( &m[3] );
	b1		= _mm_loadh_pi( b1, (__m64*)&m[4] );
	b2		= _mm_load_ss( &m[6] );
	b2		= _mm_loadh_pi( b2, (__m64*)&m[7] );

	row		= _mm_load_ss( &v[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &v[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &v[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &d[0], rslt );
	_mm_storeh_pi( (__m64*)&d[1], rslt );
}

void
SMLMatrix3f::TransformPoint(const SMLVec3f &tmp, SMLVec3f &dst, int transpose) const
{
	if (transpose)
    {
        TransformPoint1(tmp, dst);
	}
    else
    {
        TransformPoint(tmp, dst);
	}
}

void 
SMLMatrix3f::Multiply(const SMLMatrix3f &a, const SMLMatrix3f &b, int transpose_a, int transpose_b)
{
    __m128	a0, a1, a2;
	__m128	b0, b1, b2;
	__m128	c0, c1, c2;
	__m128	row, tmp, rslt;

	float	*ma = (float*)&a;
	float	*mb = (float*)&b;
	float	*mc = (float*)this;

	int		type = transpose_a + transpose_b + transpose_b;

	switch (type) {
	case 0: // A B

		b0		= _mm_load_ss( &mb[0] );
		b0		= _mm_loadh_pi( b0, (__m64*)&mb[1] );
		b1		= _mm_load_ss( &mb[3] );
		b1		= _mm_loadh_pi( b1, (__m64*)&mb[4] );
		b2		= _mm_load_ss( &mb[6] );
		b2		= _mm_loadh_pi( b2, (__m64*)&mb[7] );

		row		= _mm_load_ss( &ma[0] );
		row		= _mm_shuffle_ps( row, row, 0x00 );

		rslt	= _mm_mul_ps( row, b0 );
		row		= _mm_load_ss( &ma[1] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

		row		= _mm_load_ss( &ma[2] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

		_mm_store_ss( &mc[0], rslt );
		_mm_storeh_pi( (__m64*)&mc[1], rslt );

		row		= _mm_load_ss( &ma[3] );
		row		= _mm_shuffle_ps( row, row, 0x00 );

		rslt	= _mm_mul_ps( row, b0 );
		row		= _mm_load_ss( &ma[4] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

		row		= _mm_load_ss( &ma[5] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

		_mm_store_ss( &mc[3], rslt );
		_mm_storeh_pi( (__m64*)&mc[4], rslt );

		row		= _mm_load_ss( &ma[6] );
		row		= _mm_shuffle_ps( row, row, 0x00 );

		rslt	= _mm_mul_ps( row, b0 );
		row		= _mm_load_ss( &ma[7] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

		row		= _mm_load_ss( &ma[8] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

		_mm_store_ss( &mc[6], rslt );
		_mm_storeh_pi( (__m64*)&mc[7], rslt );

        break;

    case 1: // A^T B

		b0		= _mm_load_ss( &mb[0] );
		b0		= _mm_loadh_pi( b0, (__m64*)&mb[1] );
		b1		= _mm_load_ss( &mb[3] );
		b1		= _mm_loadh_pi( b1, (__m64*)&mb[4] );
		b2		= _mm_load_ss( &mb[6] );
		b2		= _mm_loadh_pi( b2, (__m64*)&mb[7] );

		row		= _mm_load_ss( &ma[0] );
		row		= _mm_shuffle_ps( row, row, 0x00 );

		rslt	= _mm_mul_ps( row, b0 );
		row		= _mm_load_ss( &ma[3] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

		row		= _mm_load_ss( &ma[6] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

		_mm_store_ss( &mc[0], rslt );
		_mm_storeh_pi( (__m64*)&mc[1], rslt );

		row		= _mm_load_ss( &ma[1] );
		row		= _mm_shuffle_ps( row, row, 0x00 );

		rslt	= _mm_mul_ps( row, b0 );
		row		= _mm_load_ss( &ma[4] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

		row		= _mm_load_ss( &ma[7] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

		_mm_store_ss( &mc[3], rslt );
		_mm_storeh_pi( (__m64*)&mc[4], rslt );

		row		= _mm_load_ss( &ma[2] );
		row		= _mm_shuffle_ps( row, row, 0x00 );

		rslt	= _mm_mul_ps( row, b0 );
		row		= _mm_load_ss( &ma[5] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

		row		= _mm_load_ss( &ma[8] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

		_mm_store_ss( &mc[6], rslt );
		_mm_storeh_pi( (__m64*)&mc[7], rslt );

        break;

    case 2: // A B^T

		b2		= _mm_load_ss( &mb[0] );
		b2		= _mm_loadh_pi( b2, (__m64*)&mb[1] );
		b1		= _mm_loadl_pi( b1, (__m64*)&mb[4] );
		b1		= _mm_loadh_pi( b1, (__m64*)&mb[7] );
		b0		= _mm_load_ss( &mb[3] );
		b0		= _mm_loadh_pi( b0, (__m64*)&mb[6] );

		b0		= _mm_shuffle_ps( b2, b0, 0x80 );
		b2		= _mm_shuffle_ps( b2, b1, 0xDB );
		b1		= _mm_shuffle_ps( b2, b1, 0x81 );

		row		= _mm_load_ss( &ma[0] );
		row		= _mm_shuffle_ps( row, row, 0x00 );

		rslt	= _mm_mul_ps( row, b0 );
		row		= _mm_load_ss( &ma[1] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

		row		= _mm_load_ss( &ma[2] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

		_mm_store_ss( &mc[0], rslt );
		_mm_storeh_pi( (__m64*)&mc[1], rslt );

		row		= _mm_load_ss( &ma[3] );
		row		= _mm_shuffle_ps( row, row, 0x00 );

		rslt	= _mm_mul_ps( row, b0 );
		row		= _mm_load_ss( &ma[4] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

		row		= _mm_load_ss( &ma[5] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

		_mm_store_ss( &mc[3], rslt );
		_mm_storeh_pi( (__m64*)&mc[4], rslt );

		row		= _mm_load_ss( &ma[6] );
		row		= _mm_shuffle_ps( row, row, 0x00 );

		rslt	= _mm_mul_ps( row, b0 );
		row		= _mm_load_ss( &ma[7] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

		row		= _mm_load_ss( &ma[8] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

		_mm_store_ss( &mc[6], rslt );
		_mm_storeh_pi( (__m64*)&mc[7], rslt );

        break;

    case 3: // A^T B^T

		b2		= _mm_load_ss( &mb[0] );
		b2		= _mm_loadh_pi( b2, (__m64*)&mb[1] );
		b1		= _mm_loadl_pi( b1, (__m64*)&mb[4] );
		b1		= _mm_loadh_pi( b1, (__m64*)&mb[7] );
		b0		= _mm_load_ss( &mb[3] );
		b0		= _mm_loadh_pi( b0, (__m64*)&mb[6] );

		b0		= _mm_shuffle_ps( b2, b0, 0x80 );
		b2		= _mm_shuffle_ps( b2, b1, 0xDB );
		b1		= _mm_shuffle_ps( b2, b1, 0x81 );

		row		= _mm_load_ss( &ma[0] );
		row		= _mm_shuffle_ps( row, row, 0x00 );

		rslt	= _mm_mul_ps( row, b0 );
		row		= _mm_load_ss( &ma[3] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

		row		= _mm_load_ss( &ma[6] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

		_mm_store_ss( &mc[0], rslt );
		_mm_storeh_pi( (__m64*)&mc[1], rslt );

		row		= _mm_load_ss( &ma[1] );
		row		= _mm_shuffle_ps( row, row, 0x00 );

		rslt	= _mm_mul_ps( row, b0 );
		row		= _mm_load_ss( &ma[4] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

		row		= _mm_load_ss( &ma[7] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

		_mm_store_ss( &mc[3], rslt );
		_mm_storeh_pi( (__m64*)&mc[4], rslt );

		row		= _mm_load_ss( &ma[2] );
		row		= _mm_shuffle_ps( row, row, 0x00 );

		rslt	= _mm_mul_ps( row, b0 );
		row		= _mm_load_ss( &ma[5] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

		row		= _mm_load_ss( &ma[8] );
		row		= _mm_shuffle_ps( row, row, 0x00 );
		rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

		_mm_store_ss( &mc[6], rslt );
		_mm_storeh_pi( (__m64*)&mc[7], rslt );

        break;
    }
}

void 
SMLMatrix3f::Multiply(const SMLMatrix3f &a, const SMLMatrix3f &b)
{
    __m128	row, rslt;
	__m128	b0, b1, b2;

	float	*ma = (float*)&a;
	float	*mb = (float*)&b;
	float	*mc = (float*)this;
	
	b0		= _mm_load_ss( &mb[0] );
	b0		= _mm_loadh_pi( b0, (__m64*)&mb[1] );
	b1		= _mm_load_ss( &mb[3] );
	b1		= _mm_loadh_pi( b1, (__m64*)&mb[4] );
	b2		= _mm_load_ss( &mb[6] );
	b2		= _mm_loadh_pi( b2, (__m64*)&mb[7] );

	row		= _mm_load_ss( &ma[0] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &ma[1] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &ma[2] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &mc[0], rslt );
	_mm_storeh_pi( (__m64*)&mc[1], rslt );

	row		= _mm_load_ss( &ma[3] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &ma[4] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &ma[5] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &mc[3], rslt );
	_mm_storeh_pi( (__m64*)&mc[4], rslt );

	row		= _mm_load_ss( &ma[6] );
	row		= _mm_shuffle_ps( row, row, 0x00 );

	rslt	= _mm_mul_ps( row, b0 );
	row		= _mm_load_ss( &ma[7] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b1 ) );

	row		= _mm_load_ss( &ma[8] );
	row		= _mm_shuffle_ps( row, row, 0x00 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row, b2 ) );

	_mm_store_ss( &mc[6], rslt );
	_mm_storeh_pi( (__m64*)&mc[7], rslt );
}

void 
SMLMatrix4f::Multiply(const SMLMatrix4f &a, const SMLMatrix4f &b)
{
	float	*ma = (float*)&a;
	float	*mb = (float*)&b;
	float	*mc = (float*)this;

	__m128	row0, row1, row2, row3;
	__m128	tmp1, tmp2, rslt;

	if (a.identity && b.identity)
	{
		Identity();
		return;
	}

	identity = false;

	if (a.identity)
	{
		Copy(b);
		return;
	}
	if (b.identity)
	{
		Copy(a);
		return;
	}

	_mm_prefetch( (char*)(ma+8), 1 );
	
	row0	= _mm_loadh_pi( _mm_loadl_pi( row0, (__m64*)(mb   ) ), (__m64*)(mb+2 ) );
	row1	= _mm_loadh_pi( _mm_loadl_pi( row1, (__m64*)(mb+4 ) ), (__m64*)(mb+6 ) );
	row2	= _mm_loadh_pi( _mm_loadl_pi( row2, (__m64*)(mb+8 ) ), (__m64*)(mb+10) );
	row3	= _mm_loadh_pi( _mm_loadl_pi( row3, (__m64*)(mb+12) ), (__m64*)(mb+14) );
//	-----------------------------------------------
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)(ma  ) ), (__m64*)(ma+2) );

	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0x00 );
	rslt	= _mm_mul_ps( row0, tmp1 );

	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0x55 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row1, tmp1 ) );

	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xAA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row2, tmp1 ) );

	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xFF );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row3, tmp1 ) );

	_mm_storel_pi( (__m64*)(mc  ), rslt );
	_mm_storeh_pi( (__m64*)(mc+2), rslt );
//	-----------------------------------------------
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)(ma+4) ), (__m64*)(ma+6) );

	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0x00 );
	rslt	= _mm_mul_ps( row0, tmp1 );

	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0x55 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row1, tmp1 ) );

	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xAA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row2, tmp1 ) );

	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xFF );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row3, tmp1 ) );

	_mm_storel_pi( (__m64*)(mc+4), rslt );
	_mm_storeh_pi( (__m64*)(mc+6), rslt );
//	-----------------------------------------------
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)(ma+8) ), (__m64*)(ma+10) );

	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0x00 );
	rslt	= _mm_mul_ps( row0, tmp1 );

	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0x55 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row1, tmp1 ) );

	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xAA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row2, tmp1 ) );

	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xFF );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row3, tmp1 ) );

	_mm_storel_pi( (__m64*)(mc+8 ), rslt );
	_mm_storeh_pi( (__m64*)(mc+10), rslt );
//	-----------------------------------------------
	tmp2	= _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)(ma+12) ), (__m64*)(ma+14) );

	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0x00 );
	rslt	= _mm_mul_ps( row0, tmp1 );

	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0x55 );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row1, tmp1 ) );

	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xAA );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row2, tmp1 ) );

	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xFF );
	rslt	= _mm_add_ps( rslt, _mm_mul_ps( row3, tmp1 ) );

	_mm_storel_pi( (__m64*)(mc+12), rslt );
	_mm_storeh_pi( (__m64*)(mc+14), rslt );
}

void
SMLMatrix4f::Transform(const SMLVec4f &a, SMLVec4f &c) const
{
//	c.x = Dot4f(a.data(), data[0]);
//  c.y = Dot4f(a.data(), data[1]);
//  c.z = Dot4f(a.data(), data[2]);
//	c.w = Dot4f(a.data(), data[3]);
	__m128	row0, row1, row2, row3;
	__m128	tmp1, tmp2;
	__m128	x, y, z, w;

	row0 = _mm_loadh_pi( _mm_loadl_pi( row0, (__m64*)&data[0][0] ), (__m64*)&data[1][0] );
	tmp1 = _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&data[2][0] ), (__m64*)&data[3][0] );

	row1 = _mm_shuffle_ps( row0, tmp1, 0xDD );
	row0 = _mm_shuffle_ps( row0, tmp1, 0x88 );	

	row2 = _mm_loadh_pi( _mm_loadl_pi( row2, (__m64*)&data[0][2] ), (__m64*)&data[1][2] );
	tmp1 = _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&data[2][2] ), (__m64*)&data[3][2] );

	row3 = _mm_shuffle_ps( row2, tmp1, 0xDD );
	row2 = _mm_shuffle_ps( row2, tmp1, 0x88 );	

	x = _mm_load_ss( (float*)&a.x );	x = _mm_shuffle_ps( x, x, 0x00 );
	y = _mm_load_ss( (float*)&a.y );	y = _mm_shuffle_ps( y, y, 0x00 );
	z = _mm_load_ss( (float*)&a.z );	z = _mm_shuffle_ps( z, z, 0x00 );
	w = _mm_load_ss( (float*)&a.w );	w = _mm_shuffle_ps( w, w, 0x00 );

	tmp1 = _mm_add_ps( _mm_mul_ps( row0, x ), _mm_mul_ps( row1, y ) );
	tmp2 = _mm_add_ps( _mm_mul_ps( row2, z ), _mm_mul_ps( row3, w ) );
	tmp1 = _mm_add_ps( tmp1, tmp2 );

	_mm_storel_pi( (__m64*)&c.x, tmp1 );
	_mm_storeh_pi( (__m64*)&c.z, tmp1 );
}

void
SMLMatrix4f::Transform(const SMLVec3f &a, SMLVec4f &c) const
{
//	c.x = Dot3f(a.data(), data[0]) + data[0][3];
//  c.y = Dot3f(a.data(), data[1]) + data[1][3];
//  c.z = Dot3f(a.data(), data[2]) + data[2][3];
//	c.w = Dot3f(a.data(), data[3]) + data[3][3];

	__m128	row0, row1, row2, row3;
	__m128	tmp1, tmp2;
	__m128	x, y, z;

	row0 = _mm_loadh_pi( _mm_loadl_pi( row0, (__m64*)&data[0][0] ), (__m64*)&data[1][0] );
	tmp1 = _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&data[2][0] ), (__m64*)&data[3][0] );

	row1 = _mm_shuffle_ps( row0, tmp1, 0xDD );
	row0 = _mm_shuffle_ps( row0, tmp1, 0x88 );	

	tmp1 = _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&data[0][2] ), (__m64*)&data[1][2] );
	row3 = _mm_loadh_pi( _mm_loadl_pi( row3, (__m64*)&data[2][2] ), (__m64*)&data[3][2] );

	row2 = _mm_shuffle_ps( tmp1, row3, 0x88 );
	row3 = _mm_shuffle_ps( tmp1, row3, 0xDD );

	x = _mm_load_ss( (float*)&a.x );	x = _mm_shuffle_ps( x, x, 0x00 );
	y = _mm_load_ss( (float*)&a.y );	y = _mm_shuffle_ps( y, y, 0x00 );
	z = _mm_load_ss( (float*)&a.z );	z = _mm_shuffle_ps( z, z, 0x00 );
	
	tmp1 = _mm_add_ps( _mm_mul_ps( row0, x ), _mm_mul_ps( row1, y ) );
	tmp2 = _mm_add_ps( _mm_mul_ps( row2, z ), row3 );
	tmp1 = _mm_add_ps( tmp1, tmp2 );

	_mm_storel_pi( (__m64*)&c.x, tmp1 );
	_mm_storeh_pi( (__m64*)&c.z, tmp1 );
}

void
SMLMatrix4f::Add(const SMLMatrix4f &m1, const SMLMatrix4f &m2)
{
	__m128	a, b;

	identity = false;

	a = _mm_loadh_pi( _mm_loadl_pi( a, (__m64*)&m1 ), (__m64*)((float*)&m1+2) );
	b = _mm_loadh_pi( _mm_loadl_pi( b, (__m64*)&m2 ), (__m64*)((float*)&m2+2) );

	a = _mm_add_ps( b, a );

	_mm_storel_pi( (__m64*)this, a );
	_mm_storeh_pi( (__m64*)((float*)this+2), a );

	a = _mm_loadh_pi( _mm_loadl_pi( a, (__m64*)((float*)&m1+4) ), (__m64*)((float*)&m1+6) );
	b = _mm_loadh_pi( _mm_loadl_pi( b, (__m64*)((float*)&m2+4) ), (__m64*)((float*)&m2+6) );

	a = _mm_add_ps( b, a );

	_mm_storel_pi( (__m64*)((float*)this+4), a );
	_mm_storeh_pi( (__m64*)((float*)this+6), a );

	a = _mm_loadh_pi( _mm_loadl_pi( a, (__m64*)((float*)&m1+ 8) ), (__m64*)((float*)&m1+10) );
	b = _mm_loadh_pi( _mm_loadl_pi( b, (__m64*)((float*)&m2+ 8) ), (__m64*)((float*)&m2+10) );

	a = _mm_add_ps( b, a );

	_mm_storel_pi( (__m64*)((float*)this+ 8), a );
	_mm_storeh_pi( (__m64*)((float*)this+10), a );

	a = _mm_loadh_pi( _mm_loadl_pi( a, (__m64*)((float*)&m1+12) ), (__m64*)((float*)&m1+14) );
	b = _mm_loadh_pi( _mm_loadl_pi( b, (__m64*)((float*)&m2+12) ), (__m64*)((float*)&m2+14) );

	a = _mm_add_ps( b, a );

	_mm_storel_pi( (__m64*)((float*)this+12), a );
	_mm_storeh_pi( (__m64*)((float*)this+14), a );
}

void
SMLMatrix4f::Sub(const SMLMatrix4f &m1, const SMLMatrix4f &m2)
{
	__m128	a, b;

	identity = false;

	a = _mm_loadh_pi( _mm_loadl_pi( a, (__m64*)&m1 ), (__m64*)((float*)&m1+2) );
	b = _mm_loadh_pi( _mm_loadl_pi( b, (__m64*)&m2 ), (__m64*)((float*)&m2+2) );

	a = _mm_sub_ps( a, b );

	_mm_storel_pi( (__m64*)this, a );
	_mm_storeh_pi( (__m64*)((float*)this+2), a );

	a = _mm_loadh_pi( _mm_loadl_pi( a, (__m64*)((float*)&m1+4) ), (__m64*)((float*)&m1+6) );
	b = _mm_loadh_pi( _mm_loadl_pi( b, (__m64*)((float*)&m2+4) ), (__m64*)((float*)&m2+6) );

	a = _mm_sub_ps( a, b );

	_mm_storel_pi( (__m64*)((float*)this+4), a );
	_mm_storeh_pi( (__m64*)((float*)this+6), a );

	a = _mm_loadh_pi( _mm_loadl_pi( a, (__m64*)((float*)&m1+ 8) ), (__m64*)((float*)&m1+10) );
	b = _mm_loadh_pi( _mm_loadl_pi( b, (__m64*)((float*)&m2+ 8) ), (__m64*)((float*)&m2+10) );

	a = _mm_sub_ps( a, b );

	_mm_storel_pi( (__m64*)((float*)this+ 8), a );
	_mm_storeh_pi( (__m64*)((float*)this+10), a );

	a = _mm_loadh_pi( _mm_loadl_pi( a, (__m64*)((float*)&m1+12) ), (__m64*)((float*)&m1+14) );
	b = _mm_loadh_pi( _mm_loadl_pi( b, (__m64*)((float*)&m2+12) ), (__m64*)((float*)&m2+14) );

	a = _mm_sub_ps( a, b );

	_mm_storel_pi( (__m64*)((float*)this+12), a );
	_mm_storeh_pi( (__m64*)((float*)this+14), a );
}

void SMLMatrix4f::Invert(SMLMatrix4f &src)
{
	if (src.identity)
	{
		Identity();
		return;
	}

	__m128	minor0, minor1, minor2, minor3;
	__m128	row0,   row1,   row2,   row3;
	__m128	det,    tmp1,   tmp2;

	float	*m = (float*)&src;

	identity = false;

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)(m  ) ), (__m64*)(m+ 4) );
	row1	= _mm_loadh_pi( _mm_loadl_pi( row1, (__m64*)(m+8) ), (__m64*)(m+12) );

	row0	= _mm_shuffle_ps( tmp1, row1, 0x88 );
	row1	= _mm_shuffle_ps( row1, tmp1, 0xDD );

	tmp1	= _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)(m+ 2) ), (__m64*)(m+ 6) );
	row3	= _mm_loadh_pi( _mm_loadl_pi( row3, (__m64*)(m+10) ), (__m64*)(m+14) );

	row2	= _mm_shuffle_ps( tmp1, row3, 0x88 );
	row3	= _mm_shuffle_ps( row3, tmp1, 0xDD );
//	-----------------------------------------------
	tmp2	= _mm_mul_ps( row2, row3 );
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1);

	minor0	= _mm_mul_ps( row1, tmp1 );
	minor1	= _mm_mul_ps( row0, tmp1 );

	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );

	minor0	= _mm_sub_ps( _mm_mul_ps( row1, tmp1 ), minor0 );
	minor1	= _mm_sub_ps( _mm_mul_ps( row0, tmp1 ), minor1 );
	minor1	= _mm_shuffle_ps( minor1, minor1, 0x4E );
//	-----------------------------------------------
	tmp2	= _mm_mul_ps( row1, row2);
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );

	minor0	= _mm_add_ps( _mm_mul_ps( row3, tmp1 ), minor0  );
	minor3	= _mm_mul_ps( row0, tmp1 );

	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );

	minor0	= _mm_sub_ps( minor0, _mm_mul_ps( row3, tmp1 ) );
	minor3	= _mm_sub_ps( _mm_mul_ps( row0, tmp1 ), minor3 );
	minor3	= _mm_shuffle_ps( minor3, minor3, 0x4E );
//	-----------------------------------------------
	tmp2	= _mm_mul_ps( _mm_shuffle_ps( row1, row1, 0x4E ), row3 );
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );
	row2	= _mm_shuffle_ps( row2, row2, 0x4E );

	minor0	= _mm_add_ps( _mm_mul_ps( row2, tmp1 ), minor0 );
	minor2	= _mm_mul_ps( row0, tmp1 );

	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );

	minor0	= _mm_sub_ps( minor0, _mm_mul_ps( row2, tmp1 ) );
	minor2	= _mm_sub_ps( _mm_mul_ps( row0, tmp1 ), minor2 );
	minor2	= _mm_shuffle_ps( minor2, minor2, 0x4E );
//	-----------------------------------------------
	tmp2	= _mm_mul_ps( row0, row1);
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );	

	minor2	= _mm_add_ps( _mm_mul_ps( row3, tmp1 ), minor2 );
	minor3	= _mm_sub_ps( _mm_mul_ps( row2, tmp1 ), minor3 );
	
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );
	
	minor2	= _mm_sub_ps( _mm_mul_ps( row3, tmp1 ), minor2 );
	minor3	= _mm_sub_ps( minor3, _mm_mul_ps( row2, tmp1 ) );
//	-----------------------------------------------
	tmp2	= _mm_mul_ps( row0, row3);
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );
	
	minor1	= _mm_sub_ps( minor1, _mm_mul_ps( row2, tmp1 ) );
	minor2	= _mm_add_ps( _mm_mul_ps( row1, tmp1 ), minor2 );	
	
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );
	
	minor1	= _mm_add_ps( _mm_mul_ps( row2, tmp1 ), minor1 );
	minor2	= _mm_sub_ps( minor2, _mm_mul_ps( row1, tmp1 ) );	
//	-----------------------------------------------
	tmp2	= _mm_mul_ps( row0, row2);
	tmp1	= _mm_shuffle_ps( tmp2, tmp2, 0xB1 );
	
	minor1	= _mm_add_ps( _mm_mul_ps( row3, tmp1 ), minor1 );
	minor3	= _mm_sub_ps( minor3, _mm_mul_ps( row1, tmp1 ) );	
	
	tmp1	= _mm_shuffle_ps( tmp1, tmp1, 0x4E );
	
	minor1	= _mm_sub_ps( minor1, _mm_mul_ps( row3, tmp1 ) );
	minor3	= _mm_add_ps( _mm_mul_ps( row1, tmp1 ), minor3 );	
//	-----------------------------------------------
	det		= _mm_mul_ps( row0, minor0 );
	det		= _mm_add_ps( _mm_shuffle_ps( det, det, 0x4E ), det );
	det		= _mm_add_ss( _mm_shuffle_ps( det, det, 0xB1 ), det );
	tmp1	= _mm_rcp_ss( det );

	det		= _mm_sub_ss( _mm_add_ss( tmp1, tmp1 ), _mm_mul_ss( det, _mm_mul_ss( tmp1, tmp1 ) ) );
	det		= _mm_shuffle_ps( det, det, 0x00 );

	m		= (float*)this;

	minor0	= _mm_mul_ps( det, minor0 );
	_mm_storel_pi( (__m64*)( m ), minor0 );
	_mm_storeh_pi( (__m64*)(m+2), minor0 );

	minor1	= _mm_mul_ps( det, minor1 );
	_mm_storel_pi( (__m64*)(m+4), minor1 );
	_mm_storeh_pi( (__m64*)(m+6), minor1 );

	minor2	= _mm_mul_ps( det, minor2 );
	_mm_storel_pi( (__m64*)(m+ 8), minor2 );
	_mm_storeh_pi( (__m64*)(m+10), minor2 );

	minor3	= _mm_mul_ps( det, minor3 );
	_mm_storel_pi( (__m64*)(m+12), minor3 );
	_mm_storeh_pi( (__m64*)(m+14), minor3 );
}

void SMLMatrix4f::RotationMatrix(float theta, float a, float b, float c)
{
	identity = false;

    float	cost, sint;
	__m128	s, rc, tmp;

	static __m128	mm_3	= _mm_set_ss( 3.0f );
	static __m128	mm_05	= _mm_set_ss( 0.5f );
    
	data[0][3] = 0.0f;
	data[1][3] = 0.0f;
	data[2][3] = 0.0f;
	data[3][3] = 1.0f;
    
	data[3][0] = 0.0f; data[3][1] = 0.0f; data[3][2] = 0.0f;
	
    cost = FastCos(theta);
    sint = FastSin(theta);

	if( a + b + c == 1 ) {

		if ( 1 == a && 0 == b && 0 == c) {

			data[0][0] = 1.0f; data[0][1] = 0.0f; data[0][2] = 0.0f;
			data[1][0] = 0.0f; data[1][1] = cost; data[1][2] =-sint;
			data[2][0] = 0.0f; data[2][1] = sint; data[2][2] = cost;
			return;

		} else if (0 == a  && 1 == b && 0 == c) {
        
			data[0][0] = cost; data[0][1] = 0.0f; data[0][2] = sint;
			data[1][0] = 0.0f; data[1][1] = 1.0f; data[1][2] = 0.0f;
			data[2][0] =-sint; data[2][1] = 0.0f; data[2][2] = cost;
			return;

		} else if (0 == a && 0 == b && 1 == c) {

			data[0][0] = cost; data[0][1] =-sint; data[0][2] = 0.0f;
			data[1][0] = sint; data[1][1] = cost; data[1][2] = 0.0f;
			data[2][0] = 0.0f; data[2][1] = 0.0f; data[2][2] = 1.0f;
			return;
		}
	}

	//	we need scale a,b,c to unit length.
	float	a2, b2, c2;
	float	abm, acm, bcm;
	float	asin, bsin, csin;
	float	mcos = 1.0f - cost;
	float	scale;

	s	= _mm_set_ss( a*a + b*b + c*c );
	rc	= _mm_rsqrt_ss( s );
	tmp	= _mm_mul_ss( _mm_mul_ss( s, rc ), rc );
	tmp	= _mm_sub_ss( mm_3, tmp );
	s	= _mm_mul_ss( mm_05, _mm_mul_ss( tmp, rc ) );

	_mm_store_ss( &scale, s );

	a			*= scale;
	b			*= scale;
	c			*= scale;

	a2			= a*a;
	b2			= b*b;
	c2			= c*c;

	abm			= a * b * mcos;
	acm			= a * c * mcos;
	bcm			= b * c * mcos;

    asin		= a * sint;
	bsin		= b * sint;
	csin		= c * sint;

	data[0][0]	= a2 * mcos + cost;
    data[0][1]	= abm - csin;
    data[0][2]	= acm + bsin;
    
	data[1][0]	= abm + csin;
    data[1][1]	= b2 * mcos + cost;
    data[1][2]	= bcm - asin;
    
	data[2][0]	= acm - bsin;
    data[2][1]	= bcm + asin;
    data[2][2]	= c2 * mcos + cost;   
}

/*
void SMLMatrix4f::Transform(SMLVec3f *V, int num) const
{
	//	V[i].x = Dot3f(V[i].data(), data[0]) + data[0][3];
	//	V[i].y = Dot3f(V[i].data(), data[1]) + data[1][3];
	//	V[i].z = Dot3f(V[i].data(), data[2]) + data[2][3];

	__m128	row0, row1, row2, row3;
	__m128	tmp1, tmp2;
	__m128	x, y, z;
	int		i;

	row0 = _mm_loadh_pi( _mm_loadl_pi( row0, (__m64*)&data[0][0] ), (__m64*)&data[0][2] );
	tmp2 = _mm_loadh_pi( _mm_loadl_pi( tmp2, (__m64*)&data[1][2] ), (__m64*)&data[2][2] );
	tmp1 = _mm_loadh_pi( _mm_loadl_pi( tmp1, (__m64*)&data[1][0] ), (__m64*)&data[2][0] );

	row2 = _mm_shuffle_ps( row0, tmp2, 0x82 );
	row3 = _mm_shuffle_ps( row0, tmp2, 0xD3 );
	row1 = _mm_shuffle_ps( row0, tmp1, 0xD1 );
	row0 = _mm_shuffle_ps( row0, tmp1, 0x80 );

	for( i=0; i<num; i++ ) {

		x = _mm_load_ss( (float*)&V[i].x );
		y = _mm_load_ss( (float*)&V[i].y );
		z = _mm_load_ss( (float*)&V[i].z );

		_mm_prefetch( (char*)&V[i+4].x, 1 );

		x = _mm_shuffle_ps( x, x, 0x00 );
		y = _mm_shuffle_ps( y, y, 0x00 );
		z = _mm_shuffle_ps( z, z, 0x00 );

		tmp1 = _mm_add_ps( _mm_mul_ps( row1, y ), _mm_mul_ps( row0, x ) );
		tmp2 = _mm_add_ps( row3, _mm_mul_ps( row2, z ) );

		tmp1 = _mm_add_ps( tmp2, tmp1 );

		_mm_store_ss( (float*)&V[i].x, tmp1 );
		_mm_storeh_pi( (__m64*)&V[i].y, tmp1 );
	}
}
void SMLMatrix4f::Transform(float *Vx, float *Vy, float *Vz, int num) const
{
	//	Vx[i] = Vx[i]*data[0][0] + Vy[i]*data[0][1] + Vz[i]*data[0][2] + data[0][3];
	//	Vy[i] = Vx[i]*data[1][0] + Vy[i]*data[1][1] + Vz[i]*data[1][2] + data[1][3];
	//	Vz[i] = Vx[i]*data[2][0] + Vy[i]*data[2][1] + Vz[i]*data[2][2] + data[2][3];

	__m128	a00, a01, a02, a03;
	__m128	a10, a11, a12, a13;
	__m128	a20, a21, a22, a23;
	__m128	tmp1, tmp2;
	__m128	x, y, z;

	int		i, num4 = num & 0xFFFFFFFC;

	a00 = _mm_load_ss( (float*)&data[0][0] );	a00 = _mm_shuffle_ps( a00, a00, 0x00 );
	a10 = _mm_load_ss( (float*)&data[1][0] );	a10 = _mm_shuffle_ps( a10, a10, 0x00 );
	a20 = _mm_load_ss( (float*)&data[2][0] );	a20 = _mm_shuffle_ps( a20, a20, 0x00 );
	
	a01 = _mm_load_ss( (float*)&data[0][1] );	a01 = _mm_shuffle_ps( a01, a01, 0x00 );
	a11 = _mm_load_ss( (float*)&data[1][1] );	a11 = _mm_shuffle_ps( a11, a11, 0x00 );
	a21 = _mm_load_ss( (float*)&data[2][1] );	a21 = _mm_shuffle_ps( a21, a21, 0x00 );
		
	a02 = _mm_load_ss( (float*)&data[0][2] );	a02 = _mm_shuffle_ps( a02, a02, 0x00 );
	a12 = _mm_load_ss( (float*)&data[1][2] );	a12 = _mm_shuffle_ps( a12, a12, 0x00 );
	a22 = _mm_load_ss( (float*)&data[2][2] );	a22 = _mm_shuffle_ps( a22, a22, 0x00 );
		
	a03 = _mm_load_ss( (float*)&data[0][3] );	a03 = _mm_shuffle_ps( a03, a03, 0x00 );
	a13 = _mm_load_ss( (float*)&data[1][3] );	a13 = _mm_shuffle_ps( a13, a13, 0x00 );
	a23 = _mm_load_ss( (float*)&data[2][3] );	a23 = _mm_shuffle_ps( a23, a23, 0x00 );
	
	for( i=0; i<num4; i+=4 ) {

		x = _mm_loadh_pi( _mm_loadl_pi( x, (__m64*)&Vx[i] ), (__m64*)&Vx[i+2] );
		y = _mm_loadh_pi( _mm_loadl_pi( y, (__m64*)&Vy[i] ), (__m64*)&Vy[i+2] );
		z = _mm_loadh_pi( _mm_loadl_pi( z, (__m64*)&Vz[i] ), (__m64*)&Vz[i+2] );

		tmp1 = _mm_add_ps( _mm_mul_ps( a00, x ), _mm_mul_ps( a01, y ) );
		tmp2 = _mm_add_ps( _mm_mul_ps( a02, z ), a03 );
		tmp1 = _mm_add_ps( tmp1, tmp2 );

		_mm_storel_pi( (__m64*)&Vx[ i ], tmp1 );
		_mm_storeh_pi( (__m64*)&Vx[i+2], tmp1 );

		tmp1 = _mm_add_ps( _mm_mul_ps( a11, y ), _mm_mul_ps( a10, x ) );
		tmp2 = _mm_add_ps( _mm_mul_ps( a12, z ), a13 );
		tmp1 = _mm_add_ps( tmp1, tmp2 );

		_mm_storel_pi( (__m64*)&Vy[ i ], tmp1 );
		_mm_storeh_pi( (__m64*)&Vy[i+2], tmp1 );
		
		tmp1 = _mm_add_ps( _mm_mul_ps( a21, y ), _mm_mul_ps( a20, x ) );
		tmp2 = _mm_add_ps( _mm_mul_ps( a22, z ), a23 );
		tmp1 = _mm_add_ps( tmp1, tmp2 );

		_mm_storel_pi( (__m64*)&Vz[ i ], tmp1 );
		_mm_storeh_pi( (__m64*)&Vz[i+2], tmp1 );
	}

	for( ; i<num; i++ ) {

		x = _mm_load_ss( (float*)&Vx[i] );
		y = _mm_load_ss( (float*)&Vy[i] );
		z = _mm_load_ss( (float*)&Vz[i] );
		
		tmp1 = _mm_add_ss( _mm_mul_ss( a00, x ), _mm_mul_ss( a01, y ) );
		tmp2 = _mm_add_ss( _mm_mul_ss( a02, z ), a03 );
		tmp1 = _mm_add_ss( tmp1, tmp2 );

		_mm_store_ss( (float*)&Vx[i], tmp1 );		

		tmp1 = _mm_add_ss( _mm_mul_ss( a10, x ), _mm_mul_ss( a11, y ) );
		tmp2 = _mm_add_ss( _mm_mul_ss( a12, z ), a13 );
		tmp1 = _mm_add_ss( tmp1, tmp2 );

		_mm_store_ss( (float*)&Vy[i], tmp1 );
		
		tmp1 = _mm_add_ss( _mm_mul_ss( a20, x ), _mm_mul_ss( a21, y ) );
		tmp2 = _mm_add_ss( _mm_mul_ss( a22, z ), a23 );
		tmp1 = _mm_add_ss( tmp1, tmp2 );

		_mm_store_ss( (float*)&Vz[i], tmp1 );
	}
}
*/

