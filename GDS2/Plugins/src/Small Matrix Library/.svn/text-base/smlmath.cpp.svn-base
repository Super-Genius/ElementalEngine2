#include <smlmath.h>
#include <stdio.h>
#include <memory.h>

#ifdef _PIII_CODE_
#include "smlmath_sse.inl"
#endif

#ifndef _WINDOWS_
#define OutputDebugString puts
#endif


#define ROTATE(a,i,j,k,l) g=a[i][j];h=a[k][l];a[i][j]=g-s*(h+g*tau);a[k][l]=h+s*(g-h*tau);

#define PRINT_OUTPUT
#define MATHEMATICA_SYNTAX

__declspec(naked) float __fastcall FastAbs(float a)
{
	__asm {
		fld		DWORD PTR [esp+4] 
		fabs
		ret 4
	}
}

__declspec(naked) float __fastcall FastSin(float a)
{
	__asm {
		fld		DWORD PTR [esp+4] 
		fsin
		ret 4
	}
}

__declspec(naked) float __fastcall FastCos(float a)
{
	__asm {
		fld		DWORD PTR [esp+4] 
		fcos
		ret 4
	}
}

__declspec(naked) float __fastcall InverseSqrt(float a)
{
	__asm {
		mov		eax, 0be6eb508h
		mov		DWORD PTR [esp-12],03fc00000h ;  1.5 on the stack
		sub		eax, DWORD PTR [esp+4]; a
		sub		DWORD PTR [esp+4], 800000h ; a/2 a=Y0
		shr		eax, 1     ; firs approx in eax=R0
		mov		DWORD PTR [esp-8], eax

		fld		DWORD PTR [esp-8] ;r
		fmul	st, st            ;r*r
		fld		DWORD PTR [esp-8] ;r
		fxch	st(1)
		fmul	DWORD PTR [esp+4];a ;r*r*y0
		fld		DWORD PTR [esp-12];load 1.5
		fld		st(0)
		fsub	st,st(2)			   ;r1 = 1.5 - y1
		;x1 = st(3)
		;y1 = st(2)
		;1.5 = st(1)
		;r1 = st(0)

		fld		st(1)
		fxch	st(1)
		fmul	st(3),st			; y2=y1*r1*...
		fmul	st(3),st			; y2=y1*r1*r1
		fmulp	st(4),st            ; x2=x1*r1
		fsub	st,st(2)               ; r2=1.5-y2
		;x2=st(3)
		;y2=st(2)
		;1.5=st(1)
		;r2 = st(0)

		fmul	st(2),st			;y3=y2*r2*...
		fmul	st(3),st			;x3=x2*r2
		fmulp	st(2),st			;y3=y2*r2*r2
		fxch	st(1)
		fsubp	st(1),st			;r3= 1.5 - y3
		;x3 = st(1)
		;r3 = st(0)
		fmulp	st(1), st
		ret 4
	}
}


/*
void jacobi(float a[3][3], int n, float d[], float** v, int *nrot)
{

    int j, iq, ip, i;
    float tresh, theta, tau, t, sm, s, h, g, c;
    float b[4], z[4];

    for (ip=0; ip < n; ip++) {
        for (iq=0; iq < n; iq++) {
            v[ip][iq]=0.0f;
        }
        v[ip][ip] = 1.0f;
    }

    for (ip=0; ip < n; ip++) {
        b[ip] = d[ip] = a[ip][ip];
        z[ip] = 0.0f;
    }

    *nrot = 0;

    for (i=0; i < 50; i++) {
        sm=0.0f;
        printf("i: %d\n", i);
        for (ip=0; ip < n-1; ip++) {
            for (iq=ip+1; iq < n; iq++) {
                printf("%f \t", a[ip][iq]);
                sm += FastAbs(a[ip][iq]);
            }
            printf("\n");
        }


        if (sm == 0.0f)
            return;

        if (i < 3)
            tresh = 0.2*sm/(n*n);
        else
            tresh = 0.0f;

        for (ip=0; ip < n-1; ip++) {
            for (iq=ip+1; iq < n; iq++) {
                g=100.0f*FastAbs(a[ip][iq]);

                if (i > 3 && (FastAbs(d[ip])+g) == FastAbs(d[ip]) &&
                    (FastAbs(d[iq])+g) == FastAbs(d[iq]))
                    a[ip][iq] = 0.0f;
                else if (fabs  (a[ip][iq]) > tresh) {
                    h = d[iq]-d[ip];
                    if ((FastAbs(h)+g) == FastAbs(h))
                        t=(a[ip][iq])/h;
                    else {
                        theta=0.5*h/(a[ip][iq]);
                        t=1.0f/(FastAbs(theta) + FastSqrt(1+theta*theta));
                        if (theta < 0.0f)
                            t = -t;
                    }
                    c=InverseSqrt(1+t*t);
                    s=t*c;
                    tau=s/(1.0f+c);
                    h=t*a[ip][iq];

                    z[ip] -= h;
                    z[iq] += h;
                    d[ip] -= h;
                    d[iq] += h;
                    a[ip][iq] = 0.0f;

                    for (j=0; j < ip-1; j++) {
                        ROTATE(a,j,ip,j,iq)
                            }
                    for (j=ip+1; j < iq-1; j++) {
                        ROTATE(a,ip,j,j,iq)
                            }
                    for (j=iq+1; j < n; j++) {
                        ROTATE(a,ip,j,iq,j)
                            }
                    for (j=0; j < n; j++) {
                        ROTATE(v,j,ip,j,iq);
                    }
                    ++(*nrot);
                }
            }
        }

        for (ip=0; ip < n; ip++) {

            b[ip] += z[ip];
            d[ip]  = b[ip];
            z[ip]  = 0.0f;

        }

    }

    assert(0);

}

#define TINY 1.0e-20;
void ludcmp(float **a, int n, int *indx, float *d)
{

	long i, imax, j, k;
	float big, dum, sum, temp;
	float* vv;

	vv = new float[n];
	assert( 0 != vv );

	*d = 1.0f;

	for (i=0; i < n; i++) {
        big = 0.0f;
        for (j=0; j < n; j++) {
            if ((temp = FastAbs(a[i][j])) > big)
                big = temp;
        }

        if (big == 0.0) {
            printf("Singular matrix in ludcmp!");
            assert(0);
        }

        vv[i] = 1.0f/big;
	}

	for (j=0; j < n; j++) {
        for (i=0; i < j; i++) {
            sum = a[i][j];
            for (k=0; k < i; k++)
                sum -= a[i][k]*a[k][j];
            a[i][j] = sum;
        }
        big = 0.0f;
        for (i=j; i < n; i++) {
            sum = a[i][j];
            for (k=0; k < j; k++)
                sum -= a[i][k]*a[k][j];
            a[i][j] = sum;
            if ((dum = (vv[i]*FastAbs(sum))) >= big) {
                big  = dum;
                imax = i;
            }
        }

        if (j != imax) {
            for (k=0; k < n; k++) {
                dum = a[imax][k];
                a[imax][k] = a[j][k];
                a[j][k] = dum;
            }

            *d = -(*d);
            vv[imax] = vv[j];

        }

        indx[j] = imax;
        if (a[j][j] == 0.0) {
            assert(0);
        }

        if (j < n) {
            dum = 1.0f/(a[j][j]);
            for (i=j+1;i < n; i++)
                a[i][j] *= dum;
        }

	}

	delete vv;

}

void lubksb(float **a, int n, int *indx, float b[]) 
{

  int i, ii=0, ip, j;
  float sum;

  for (i=0; i <= n-1; i++) {
	 ip = indx[i];
	 sum = b[ip];
	 b[ip] = b[i];
	 if (ii)
	   for (j=ii; j <= i-1; j++)
	      sum -= a[i][j]*b[j];
	 else if (sum != 0.0f)
		    ii=i;

	 b[i] = sum;
  }

  for (i=n-1; i >= 0; i--) {
	 sum=b[i];
	 for (j=i+1; j < n; j++)
		sum -= a[i][j]*b[j];
	 b[i] = sum/a[i][i];
  }

}
*/

#ifndef _PIII_CODE_
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
SMLMatrix3f::TransformPoint(const SMLVec3f &tmp, SMLVec3f &dst) const
{
//		dst.x = data[0][0] * tmp.x + data[0][1] * tmp.y + data[0][2] * tmp.z;
//		dst.y = data[1][0] * tmp.x + data[1][1] * tmp.y + data[1][2] * tmp.z;
//		dst.z = data[2][0] * tmp.x + data[2][1] * tmp.y + data[2][2] * tmp.z;
    __asm {
	mov	ebx, DWORD PTR this
	mov	ecx, DWORD PTR tmp
	mov	eax, DWORD PTR dst
    fld   dword ptr [ecx]
    fmul  dword ptr [ebx+24]
    fld   dword ptr [ecx]
    fmul  dword ptr [ebx+12]
    fld   dword ptr [ecx]
    fmul  dword ptr [ebx]
    fld   dword ptr [ecx+4]
    fmul  dword ptr [ebx+4]
    fld   dword ptr [ecx+4]
    fmul  dword ptr [ebx+16]
    fld   dword ptr [ecx+4]
    fmul  dword ptr [ebx+28]
    fxch  ST(2)
    faddp ST(3),ST
    faddp ST(3),ST
    faddp ST(3),ST
    fld   dword ptr [ecx+8]
    fmul  dword ptr [ebx+8]
    fld   dword ptr [ecx+8]
    fmul  dword ptr [ebx+20]
    fld   dword ptr [ecx+8]
    fmul  dword ptr [ebx+32]
    fxch  ST(2)
    faddp ST(3),ST
    faddp ST(3),ST
    faddp ST(3),ST
    fstp  dword ptr [eax]
    fstp  dword ptr [eax+4]
    fstp  dword ptr [eax+8]
    }
}

void
SMLMatrix3f::TransformPoint1(const SMLVec3f &tmp, SMLVec3f &dst) const
{
//    dst.x = data[0][0] * tmp.x + data[1][0] * tmp.y + data[2][0] * tmp.z;
//    dst.y = data[0][1] * tmp.x + data[1][1] * tmp.y + data[2][1] * tmp.z;
//    dst.z = data[0][2] * tmp.x + data[1][2] * tmp.y + data[2][2] * tmp.z;
    __asm {
	mov	ebx, DWORD PTR this
	mov	ecx, DWORD PTR tmp
	mov	eax, DWORD PTR dst
    fld   dword ptr [ecx]
    fmul  dword ptr [ebx+8]
    fld   dword ptr [ecx]
    fmul  dword ptr [ebx+4]
    fld   dword ptr [ecx]
    fmul  dword ptr [ebx]
    fld   dword ptr [ecx+4]
    fmul  dword ptr [ebx+12]
    fld   dword ptr [ecx+4]
    fmul  dword ptr [ebx+16]
    fld   dword ptr [ecx+4]
    fmul  dword ptr [ebx+20]
    fxch  ST(2)
    faddp ST(3),ST
    faddp ST(3),ST
    faddp ST(3),ST
    fld   dword ptr [ecx+8]
    fmul  dword ptr [ebx+24]
    fld   dword ptr [ecx+8]
    fmul  dword ptr [ebx+28]
    fld   dword ptr [ecx+8]
    fmul  dword ptr [ebx+32]
    fxch  ST(2)
    faddp ST(3),ST
    faddp ST(3),ST
    faddp ST(3),ST
    fstp  dword ptr [eax]
    fstp  dword ptr [eax+4]
    fstp  dword ptr [eax+8]
    }
}

void 
SMLMatrix3f::Multiply(const SMLMatrix3f &a, const SMLMatrix3f &b, int transpose_a, int transpose_b)
{

    // Mike's implementation:
//	int i, j, trsw;
//	trsw = transpose_a;
//	if (transpose_b)
//		trsw |= 2;
//	for (i=0; i<3; i++) {
//		for (j=0; j<3; j++) {
//			switch (trsw) {
//			case 0:
//				data[i][j] = a.data[i][0] * b.data[0][j] + a.data[i][1] * b.data[1][j] + a.data[i][2] * b.data[2][j];
//				break;
//			case 1:
//				data[i][j] = a.data[0][i] * b.data[0][j] + a.data[1][i] * b.data[1][j] + a.data[2][i] * b.data[2][j];
//				break;
//			case 2:
//				data[i][j] = a.data[i][0] * b.data[j][0] + a.data[i][1] * b.data[j][1] + a.data[i][2] * b.data[j][2];
//				break;
//			default:
//				data[i][j] = a.data[0][i] * b.data[j][0] + a.data[1][i] * b.data[j][1] + a.data[2][i] * b.data[j][2];
//				break;
//			}
//		}
//	}
//	return;

    int type = transpose_a + transpose_b + transpose_b;
    switch (type) {
    case 0: // A B
        data[0][0] = a.data[0][0]*b.data[0][0] + a.data[0][1]*b.data[1][0] + a.data[0][2]*b.data[2][0];
        data[1][0] = a.data[1][0]*b.data[0][0] + a.data[1][1]*b.data[1][0] + a.data[1][2]*b.data[2][0];
        data[2][0] = a.data[2][0]*b.data[0][0] + a.data[2][1]*b.data[1][0] + a.data[2][2]*b.data[2][0];
        data[0][1] = a.data[0][0]*b.data[0][1] + a.data[0][1]*b.data[1][1] + a.data[0][2]*b.data[2][1];
        data[1][1] = a.data[1][0]*b.data[0][1] + a.data[1][1]*b.data[1][1] + a.data[1][2]*b.data[2][1];
        data[2][1] = a.data[2][0]*b.data[0][1] + a.data[2][1]*b.data[1][1] + a.data[2][2]*b.data[2][1];
        data[0][2] = a.data[0][0]*b.data[0][2] + a.data[0][1]*b.data[1][2] + a.data[0][2]*b.data[2][2];
        data[1][2] = a.data[1][0]*b.data[0][2] + a.data[1][1]*b.data[1][2] + a.data[1][2]*b.data[2][2];
        data[2][2] = a.data[2][0]*b.data[0][2] + a.data[2][1]*b.data[1][2] + a.data[2][2]*b.data[2][2];
        break;
    case 1: // A^T B
        data[0][0] = a.data[0][0]*b.data[0][0] + a.data[1][0]*b.data[1][0] + a.data[2][0]*b.data[2][0];
        data[0][1] = a.data[0][0]*b.data[0][1] + a.data[1][0]*b.data[1][1] + a.data[2][0]*b.data[2][1];
        data[0][2] = a.data[0][0]*b.data[0][2] + a.data[1][0]*b.data[1][2] + a.data[2][0]*b.data[2][2];
        data[1][0] = a.data[0][1]*b.data[0][0] + a.data[1][1]*b.data[1][0] + a.data[2][1]*b.data[2][0];
        data[1][1] = a.data[0][1]*b.data[0][1] + a.data[1][1]*b.data[1][1] + a.data[2][1]*b.data[2][1];
        data[1][2] = a.data[0][1]*b.data[0][2] + a.data[1][1]*b.data[1][2] + a.data[2][1]*b.data[2][2];
        data[2][0] = a.data[0][2]*b.data[0][0] + a.data[1][2]*b.data[1][0] + a.data[2][2]*b.data[2][0];
        data[2][1] = a.data[0][2]*b.data[0][1] + a.data[1][2]*b.data[1][1] + a.data[2][2]*b.data[2][1];
        data[2][2] = a.data[0][2]*b.data[0][2] + a.data[1][2]*b.data[1][2] + a.data[2][2]*b.data[2][2];
        break;
    case 2: // A B^T
        data[0][0] = a.data[0][0]*b.data[0][0] + a.data[0][1]*b.data[0][1] + a.data[0][2]*b.data[0][2];
        data[0][1] = a.data[0][0]*b.data[1][0] + a.data[0][1]*b.data[1][1] + a.data[0][2]*b.data[1][2];
        data[0][2] = a.data[0][0]*b.data[2][0] + a.data[0][1]*b.data[2][1] + a.data[0][2]*b.data[2][2];
        data[1][0] = a.data[1][0]*b.data[0][0] + a.data[1][1]*b.data[0][1] + a.data[1][2]*b.data[0][2];
        data[1][1] = a.data[1][0]*b.data[1][0] + a.data[1][1]*b.data[1][1] + a.data[1][2]*b.data[1][2];
        data[1][2] = a.data[1][0]*b.data[2][0] + a.data[1][1]*b.data[2][1] + a.data[1][2]*b.data[2][2];
        data[2][0] = a.data[2][0]*b.data[0][0] + a.data[2][1]*b.data[0][1] + a.data[2][2]*b.data[0][2];
        data[2][1] = a.data[2][0]*b.data[1][0] + a.data[2][1]*b.data[1][1] + a.data[2][2]*b.data[1][2];
        data[2][2] = a.data[2][0]*b.data[2][0] + a.data[2][1]*b.data[2][1] + a.data[2][2]*b.data[2][2];
        break;
    case 3: // A^T B^T
        data[0][0] = a.data[0][0]*b.data[0][0] + a.data[1][0]*b.data[0][1] + a.data[2][0]*b.data[0][2];
        data[0][1] = a.data[0][0]*b.data[1][0] + a.data[1][0]*b.data[1][1] + a.data[2][0]*b.data[1][2];
        data[0][2] = a.data[0][0]*b.data[2][0] + a.data[1][0]*b.data[2][1] + a.data[2][0]*b.data[2][2];
        data[1][0] = a.data[0][1]*b.data[0][0] + a.data[1][1]*b.data[0][1] + a.data[2][1]*b.data[0][2];
        data[1][1] = a.data[0][1]*b.data[1][0] + a.data[1][1]*b.data[1][1] + a.data[2][1]*b.data[1][2];
        data[1][2] = a.data[0][1]*b.data[2][0] + a.data[1][1]*b.data[2][1] + a.data[2][1]*b.data[2][2];
        data[2][0] = a.data[0][2]*b.data[0][0] + a.data[1][2]*b.data[0][1] + a.data[2][2]*b.data[0][2];
        data[2][1] = a.data[0][2]*b.data[1][0] + a.data[1][2]*b.data[1][1] + a.data[2][2]*b.data[1][2];
        data[2][2] = a.data[0][2]*b.data[2][0] + a.data[1][2]*b.data[2][1] + a.data[2][2]*b.data[2][2];
        break;
    }
}

void 
SMLMatrix3f::Multiply(const SMLMatrix3f &a, const SMLMatrix3f &b)
{
    data[0][0] = a.data[0][0]*b.data[0][0] + a.data[0][1]*b.data[1][0] + a.data[0][2]*b.data[2][0];
    data[1][0] = a.data[1][0]*b.data[0][0] + a.data[1][1]*b.data[1][0] + a.data[1][2]*b.data[2][0];
    data[2][0] = a.data[2][0]*b.data[0][0] + a.data[2][1]*b.data[1][0] + a.data[2][2]*b.data[2][0];
    data[0][1] = a.data[0][0]*b.data[0][1] + a.data[0][1]*b.data[1][1] + a.data[0][2]*b.data[2][1];
    data[1][1] = a.data[1][0]*b.data[0][1] + a.data[1][1]*b.data[1][1] + a.data[1][2]*b.data[2][1];
    data[2][1] = a.data[2][0]*b.data[0][1] + a.data[2][1]*b.data[1][1] + a.data[2][2]*b.data[2][1];
    data[0][2] = a.data[0][0]*b.data[0][2] + a.data[0][1]*b.data[1][2] + a.data[0][2]*b.data[2][2];
    data[1][2] = a.data[1][0]*b.data[0][2] + a.data[1][1]*b.data[1][2] + a.data[1][2]*b.data[2][2];
    data[2][2] = a.data[2][0]*b.data[0][2] + a.data[2][1]*b.data[1][2] + a.data[2][2]*b.data[2][2];
}
#endif

#ifndef _PIII_CODE_
void 
SMLMatrix4f::Multiply(const SMLMatrix4f &a, const SMLMatrix4f &b)
{

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

//  data[0][0]=a.data[0][0]*b.data[0][0]+a.data[0][1]*b.data[1][0]+a.data[0][2]*b.data[2][0]+a.data[0][3]*b.data[3][0];
//  data[1][0]=a.data[1][0]*b.data[0][0]+a.data[1][1]*b.data[1][0]+a.data[1][2]*b.data[2][0]+a.data[1][3]*b.data[3][0];
//  data[2][0]=a.data[2][0]*b.data[0][0]+a.data[2][1]*b.data[1][0]+a.data[2][2]*b.data[2][0]+a.data[2][3]*b.data[3][0];
//  data[3][0]=a.data[3][0]*b.data[0][0]+a.data[3][1]*b.data[1][0]+a.data[3][2]*b.data[2][0]+a.data[3][3]*b.data[3][0];
//  data[0][1]=a.data[0][0]*b.data[0][1]+a.data[0][1]*b.data[1][1]+a.data[0][2]*b.data[2][1]+a.data[0][3]*b.data[3][1];
//  data[1][1]=a.data[1][0]*b.data[0][1]+a.data[1][1]*b.data[1][1]+a.data[1][2]*b.data[2][1]+a.data[1][3]*b.data[3][1];
//  data[2][1]=a.data[2][0]*b.data[0][1]+a.data[2][1]*b.data[1][1]+a.data[2][2]*b.data[2][1]+a.data[2][3]*b.data[3][1];
//  data[3][1]=a.data[3][0]*b.data[0][1]+a.data[3][1]*b.data[1][1]+a.data[3][2]*b.data[2][1]+a.data[3][3]*b.data[3][1];
//  data[0][2]=a.data[0][0]*b.data[0][2]+a.data[0][1]*b.data[1][2]+a.data[0][2]*b.data[2][2]+a.data[0][3]*b.data[3][2];
//  data[1][2]=a.data[1][0]*b.data[0][2]+a.data[1][1]*b.data[1][2]+a.data[1][2]*b.data[2][2]+a.data[1][3]*b.data[3][2];
//  data[2][2]=a.data[2][0]*b.data[0][2]+a.data[2][1]*b.data[1][2]+a.data[2][2]*b.data[2][2]+a.data[2][3]*b.data[3][2];
//  data[3][2]=a.data[3][0]*b.data[0][2]+a.data[3][1]*b.data[1][2]+a.data[3][2]*b.data[2][2]+a.data[3][3]*b.data[3][2];
//  data[0][3]=a.data[0][0]*b.data[0][3]+a.data[0][1]*b.data[1][3]+a.data[0][2]*b.data[2][3]+a.data[0][3]*b.data[3][3];
//  data[1][3]=a.data[1][0]*b.data[0][3]+a.data[1][1]*b.data[1][3]+a.data[1][2]*b.data[2][3]+a.data[1][3]*b.data[3][3];
//  data[2][3]=a.data[2][0]*b.data[0][3]+a.data[2][1]*b.data[1][3]+a.data[2][2]*b.data[2][3]+a.data[2][3]*b.data[3][3];
//  data[3][3]=a.data[3][0]*b.data[0][3]+a.data[3][1]*b.data[1][3]+a.data[3][2]*b.data[2][3]+a.data[3][3]*b.data[3][3];

	__asm mov	eax, DWORD PTR a
	__asm mov	ebx, DWORD PTR b 
	__asm mov	ecx, DWORD PTR this

    _E4(0, 0) _E4(0, 1) _E4(0, 2) _E4(0, 3) 
    _E4(1, 0) _E4(1, 1) _E4(1, 2) _E4(1, 3) 
    _E4(2, 0) _E4(2, 1) _E4(2, 2) _E4(2, 3) 
    _E4(3, 0) _E4(3, 1) _E4(3, 2) _E4(3, 3) 
}
#endif

#ifndef _PIII_CODE_
void
SMLMatrix4f::Transform(const SMLVec4f &a, SMLVec4f &c) const
{
//	c.x = Dot4f(a.data(), data[0]);
//  c.y = Dot4f(a.data(), data[1]);
//  c.z = Dot4f(a.data(), data[2]);
//	c.w = Dot4f(a.data(), data[3]);
	__asm mov	eax, DWORD PTR this
	__asm mov	ebx, DWORD PTR a 
	__asm mov	ecx, DWORD PTR c

    _E41(0) _E41(1) _E41(2) _E41(3) 
}

void
SMLMatrix4f::Transform(const SMLVec3f &a, SMLVec4f &c) const
{
//	c.x = Dot3f(a.data(), data[0]) + data[0][3];
//  c.y = Dot3f(a.data(), data[1]) + data[1][3];
//  c.z = Dot3f(a.data(), data[2]) + data[2][3];
//	c.w = Dot3f(a.data(), data[3]) + data[3][3];
	__asm mov	eax, DWORD PTR this
	__asm mov	ebx, DWORD PTR a 
	__asm mov	ecx, DWORD PTR c

    __asm fld   DWORD PTR [ebx]      
    __asm fmul  DWORD PTR [eax+32]  
    __asm fld   DWORD PTR [ebx]      
    __asm fmul  DWORD PTR [eax+16]   
    __asm fld   DWORD PTR [ebx]      
    __asm fmul  DWORD PTR [eax]     
    __asm fld   DWORD PTR [ebx+4]    
    __asm fmul  DWORD PTR [eax+4]  
    __asm fld   DWORD PTR [ebx+4]    
    __asm fmul  DWORD PTR [eax+20]  
    __asm fld   DWORD PTR [ebx+4]    
    __asm fmul  DWORD PTR [eax+36]  
    __asm fxch  ST(2)           
    __asm faddp ST(3),ST       
    __asm faddp ST(3),ST       
    __asm faddp ST(3),ST       
    __asm fld   DWORD PTR [ebx+8]   
    __asm fmul  DWORD PTR [eax+8]  
    __asm fld   DWORD PTR [ebx+8]   
    __asm fmul  DWORD PTR [eax+24]  
    __asm fld   DWORD PTR [ebx+8]   
    __asm fmul  DWORD PTR [eax+40]  
    __asm fxch  ST(2)           
    __asm faddp ST(3),ST       
    __asm faddp ST(3),ST       
    __asm faddp ST(3),ST       
    __asm fadd  DWORD PTR [eax+12]  
    __asm fstp  DWORD PTR [ecx]     
    __asm fadd  DWORD PTR [eax+28]  
    __asm fstp  DWORD PTR [ecx+4]   
    __asm fadd  DWORD PTR [eax+44]  
    __asm fstp  DWORD PTR [ecx+8]  

    __asm fld   DWORD PTR [ebx]   
    __asm fmul  DWORD PTR [eax+48]  
    __asm fld   DWORD PTR [ebx+4]   
    __asm fmul  DWORD PTR [eax+52]  
    __asm fld   DWORD PTR [ebx+8]   
    __asm fmul  DWORD PTR [eax+56]  
    __asm fxch  ST(1)           
    __asm fadd  DWORD PTR [eax+60]   
    __asm fxch  ST(1)           
    __asm faddp ST(2),ST       
    __asm faddp ST(1),ST
    __asm fstp  DWORD PTR [ecx+12]  
}
#endif

#ifndef _PIII_CODE_
void
SMLMatrix4f::Add(const SMLMatrix4f &m1, const SMLMatrix4f &m2)
{
	for(int i=0; i<16; i++)
		data_elem[i] = m1.data_elem[i] + m2.data_elem[i];
	identity = false;
}

void
SMLMatrix4f::Sub(const SMLMatrix4f &m1, const SMLMatrix4f &m2)
{
	for(int i=0; i<16; i++)
		data_elem[i] = m1.data_elem[i] - m2.data_elem[i];
	identity = false;
}
#endif



void SMLMatrix3f::Output( char* l1, char* l2)
{
#ifdef PRINT_OUTPUT
    char str[256];
#ifdef MATHEMATICA_SYNTAX
	sprintf( str, "%s %s = {\n{", l1? l1:"", l2? l2:"");
	OutputDebugString( str);
	for ( int i=0; i<3; i++) {
		for ( int j=0; j<3; j++) {
			sprintf( str, "%f%s", data[i][j], j == 2? (i == 2? "}};\n" : "},\n{") : ", ");
		    OutputDebugString( str);
		}
	}
#else
	sprintf( str, "Matrix3f %s %s \n", l1? l1:"", l2? l2:"");
	OutputDebugString( str);
	for ( int i=0; i<3; i++) {
		for ( int j=0; j<3; j++) {
			sprintf( str, "%f  ", data[i][j]);
		    OutputDebugString( str);
		}
		OutputDebugString( "\n");
	}
#endif
#endif
}

static void Full_Inverse_Xform3(float b[][4], float a[][4])
{
    long indxc[4], indxr[4], ipiv[4];
    long i, icol, irow, j, ir, ic;
    float big, dum, pivinv, temp, bb;
    ipiv[0] = -1;
    ipiv[1] = -1;
    ipiv[2] = -1;
    ipiv[3] = -1;
    a[0][0] = b[0][0];
    a[1][0] = b[1][0];
    a[2][0] = b[2][0];
    a[3][0] = b[3][0];
    a[0][1] = b[0][1];
    a[1][1] = b[1][1];
    a[2][1] = b[2][1];
    a[3][1] = b[3][1];
    a[0][2] = b[0][2];
    a[1][2] = b[1][2];
    a[2][2] = b[2][2];
    a[3][2] = b[3][2];
    a[0][3] = b[0][3];
    a[1][3] = b[1][3];
    a[2][3] = b[2][3];
    a[3][3] = b[3][3];
    for (i = 0; i < 4; i++) {
        big = 0.0f;
        for (j = 0; j < 4; j++) {
            if (ipiv[j] != 0) {
                if (ipiv[0] == -1) {
                    if ((bb = FastAbs(a[j][0])) > big) {
                        big = bb;
                        irow = j;
                        icol = 0;
                    }
                } else if (ipiv[0] > 0) {
                    return;
                }
                if (ipiv[1] == -1) {
                    if ((bb = FastAbs(a[j][1])) > big) {
                        big = bb;
                        irow = j;
                        icol = 1;
                    }
                } else if (ipiv[1] > 0) {
                    return;
                }
                if (ipiv[2] == -1) {
                    if ((bb = FastAbs(a[j][2])) > big) {
                        big = bb;
                        irow = j;
                        icol = 2;
                    }
                } else if (ipiv[2] > 0) {
                    return;
                }
                if (ipiv[3] == -1) {
                    if ((bb = FastAbs(a[j][3])) > big) {
                        big = bb;
                        irow = j;
                        icol = 3;
                    }
                } else if (ipiv[3] > 0) {
                    return;
                }
            }
        }
        ++(ipiv[icol]);
        if (irow != icol) {
            temp = a[irow][0];
            a[irow][0] = a[icol][0];
            a[icol][0] = temp;
            temp = a[irow][1];
            a[irow][1] = a[icol][1];
            a[icol][1] = temp;
            temp = a[irow][2];
            a[irow][2] = a[icol][2];
            a[icol][2] = temp;
            temp = a[irow][3];
            a[irow][3] = a[icol][3];
            a[icol][3] = temp;
        }
        indxr[i] = irow;
        indxc[i] = icol;
        if (a[icol][icol] == 0.0) {
            return;
        }
        pivinv = 1.0f / a[icol][icol];
        a[icol][icol] = 1.0f;
        a[icol][0] *= pivinv;
        a[icol][1] *= pivinv;
        a[icol][2] *= pivinv;
        a[icol][3] *= pivinv;
        if (icol != 0) {
            dum = a[0][icol];
            a[0][icol] = 0.0f;
            a[0][0] -= a[icol][0] * dum;
            a[0][1] -= a[icol][1] * dum;
            a[0][2] -= a[icol][2] * dum;
            a[0][3] -= a[icol][3] * dum;
        }
        if (icol != 1) {
            dum = a[1][icol];
            a[1][icol] = 0.0f;
            a[1][0] -= a[icol][0] * dum;
            a[1][1] -= a[icol][1] * dum;
            a[1][2] -= a[icol][2] * dum;
            a[1][3] -= a[icol][3] * dum;
        }
        if (icol != 2) {
            dum = a[2][icol];
            a[2][icol] = 0.0f;
            a[2][0] -= a[icol][0] * dum;
            a[2][1] -= a[icol][1] * dum;
            a[2][2] -= a[icol][2] * dum;
            a[2][3] -= a[icol][3] * dum;
        }
        if (icol != 3) {
            dum = a[3][icol];
            a[3][icol] = 0.0f;
            a[3][0] -= a[icol][0] * dum;
            a[3][1] -= a[icol][1] * dum;
            a[3][2] -= a[icol][2] * dum;
            a[3][3] -= a[icol][3] * dum;
        }
    }
    if (indxr[3] != indxc[3]) {
        ir = indxr[3];
        ic = indxc[3];
        temp = a[0][ir];
        a[0][ir] = a[0][ic];
        a[0][ic] = temp;
        temp = a[1][ir];
        a[1][ir] = a[1][ic];
        a[1][ic] = temp;
        temp = a[2][ir];
        a[2][ir] = a[2][ic];
        a[2][ic] = temp;
        temp = a[3][ir];
        a[3][ir] = a[3][ic];
        a[3][ic] = temp;
    }
    if (indxr[2] != indxc[2]) {
        ir = indxr[2];
        ic = indxc[2];
        temp = a[0][ir];
        a[0][ir] = a[0][ic];
        a[0][ic] = temp;
        temp = a[1][ir];
        a[1][ir] = a[1][ic];
        a[1][ic] = temp;
        temp = a[2][ir];
        a[2][ir] = a[2][ic];
        a[2][ic] = temp;
        temp = a[3][ir];
        a[3][ir] = a[3][ic];
        a[3][ic] = temp;
    }
    if (indxr[1] != indxc[1]) {
        ir = indxr[1];
        ic = indxc[1];
        temp = a[0][ir];
        a[0][ir] = a[0][ic];
        a[0][ic] = temp;
        temp = a[1][ir];
        a[1][ir] = a[1][ic];
        a[1][ic] = temp;
        temp = a[2][ir];
        a[2][ir] = a[2][ic];
        a[2][ic] = temp;
        temp = a[3][ir];
        a[3][ir] = a[3][ic];
        a[3][ic] = temp;
    }
    if (indxr[0] != indxc[0]) {
        ir = indxr[0];
        ic = indxc[0];
        temp = a[0][ir];
        a[0][ir] = a[0][ic];
        a[0][ic] = temp;
        temp = a[1][ir];
        a[1][ir] = a[1][ic];
        a[1][ic] = temp;
        temp = a[2][ir];
        a[2][ir] = a[2][ic];
        a[2][ic] = temp;
        temp = a[3][ir];
        a[3][ir] = a[3][ic];
        a[3][ic] = temp;
    }
}

void SMLMatrix4f::Output( char* l1, char* l2)
{
#ifdef PRINT_OUTPUT
    char str[256];
#ifdef MATHEMATICA_SYNTAX
	sprintf( str, "%s %s ={{\n", l1? l1:"", l2? l2:"");
	OutputDebugString( str);
	for ( int i=0; i<4; i++) {
		for ( int j=0; j<4; j++) {
			sprintf( str, "%f%s", data[i][j], j == 3? (i == 3? "}};\n" : "},\n{") : ", ");
		    OutputDebugString( str);
		}
	}
#else
	sprintf( str, "Matrix4f %s %s \n", l1? l1:"", l2? l2:"");
	OutputDebugString( str);
	for ( int i=0; i<4; i++) {
		for ( int j=0; j<4; j++) {
			sprintf( str, "%f  ", data[i][j]);
		    OutputDebugString( str);
		}
		OutputDebugString( "\n");
	}
#endif
#endif
}

static float det3x3(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3)
{
        return (a1 * (b2 * c3 - b3 * c2) - b1 * (a2 * c3 - a3 * c2) + c1 * (a2 * b3 - a3 * b2));
}

static void Down_triangle_Inverse_Xform(float src[][4], float dst[][4])
{
    float det1, o33;
    float a1, a2, a3, b1, b2, b3, c1, c2, c3;
    a1 = src[0][0];
    b1 = src[0][1];
    c1 = src[0][2];
    a2 = src[1][0];
    b2 = src[1][1];
    c2 = src[1][2];
    a3 = src[2][0];
    b3 = src[2][1];
    c3 = src[2][2];
    det1 = det3x3(a1, a2, a3, b1, b2, b3, c1, c2, c3);
    if (FastAbs(det1) < FLT_EPSILON) {
        return;
    } else {
        det1 = 1.0f / det1;
        o33 = 1.0f / src[3][3];
        dst[0][0] =  (b2 * c3 - c2 * b3) * det1;
        dst[1][0] = -(a2 * c3 - c2 * a3) * det1;
        dst[2][0] =  (a2 * b3 - b2 * a3) * det1;
        dst[0][1] = -(b1 * c3 - c1 * b3) * det1;
        dst[1][1] =  (a1 * c3 - c1 * a3) * det1;
        dst[2][1] = -(a1 * b3 - b1 * a3) * det1;
        dst[0][2] =  (b1 * c2 - c1 * b2) * det1;
        dst[1][2] = -(a1 * c2 - c1 * a2) * det1;
        dst[2][2] =  (a1 * b2 - b1 * a2) * det1;
        dst[3][0] = -(src[3][0] * dst[0][0] + src[3][1] * dst[1][0] + src[3][2] * dst[2][0]) * o33;
        dst[3][1] = -(src[3][0] * dst[0][1] + src[3][1] * dst[1][1] + src[3][2] * dst[2][1]) * o33;
        dst[3][2] = -(src[3][0] * dst[0][2] + src[3][1] * dst[1][2] + src[3][2] * dst[2][2]) * o33;
        dst[0][3] = 0.0f;
        dst[1][3] = 0.0f;
        dst[2][3] = 0.0f;
        dst[3][3] = o33;
    }
}

static void Up_triangle_Inverse_Xform(float src[][4], float dst[][4])
{
    float det1, o33;
    float a1, a2, a3, b1, b2, b3, c1, c2, c3;
    a1 = src[0][0];
    b1 = src[0][1];
    c1 = src[0][2];
    a2 = src[1][0];
    b2 = src[1][1];
    c2 = src[1][2];
    a3 = src[2][0];
    b3 = src[2][1];
    c3 = src[2][2];
    det1 = det3x3(a1, a2, a3, b1, b2, b3, c1, c2, c3);
    if (FastAbs(det1) < FLT_EPSILON) {
        return;
    } else {
        det1 = (1.0f / det1);
        o33 = 1.0f / src[3][3];
        dst[0][0] =  (b2 * c3 - c2 * b3) * det1;
        dst[1][0] = -(a2 * c3 - c2 * a3) * det1;
        dst[2][0] =  (a2 * b3 - b2 * a3) * det1;
        dst[0][1] = -(b1 * c3 - c1 * b3) * det1;
        dst[1][1] =  (a1 * c3 - c1 * a3) * det1;
        dst[2][1] = -(a1 * b3 - b1 * a3) * det1;
        dst[0][2] =  (b1 * c2 - c1 * b2) * det1;
        dst[1][2] = -(a1 * c2 - c1 * a2) * det1;
        dst[2][2] =  (a1 * b2 - b1 * a2) * det1;
        dst[0][3] = -(src[0][3] * dst[0][0] + src[1][3] * dst[0][1] + src[2][3] * dst[0][2]) * o33;
        dst[1][3] = -(src[0][3] * dst[1][0] + src[1][3] * dst[1][1] + src[2][3] * dst[1][2]) * o33;
        dst[2][3] = -(src[0][3] * dst[2][0] + src[1][3] * dst[2][1] + src[2][3] * dst[2][2]) * o33;
        dst[3][0] = 0.0f;
        dst[3][1] = 0.0f;
        dst[3][2] = 0.0f;
        dst[3][3] = o33;
    }
}

#ifndef _PIII_CODE_
void SMLMatrix4f::Invert(SMLMatrix4f &src)
{
	if (src.identity)
	{
		Identity();
		return;
	}

	identity = false;	
    if ((src.data[0][3] == 0) && (src.data[1][3] == 0) && (src.data[2][3] == 0)) {
        Down_triangle_Inverse_Xform(src.data, data);
        return;
    } else if ((src.data[3][0] == 0) && (src.data[3][1] == 0) && (src.data[3][2] == 0)) {
        Up_triangle_Inverse_Xform(src.data, data);
        return;
    } else {
        Full_Inverse_Xform3(src.data, data);
        return;
    }

}
#endif

SMLVec4f SMLMatrix4f::GetRotationVector() const
{

	SMLVec3f V;
	float theta;
	float factor;
	// convert from cosine matrix to axis-angle
	theta  = (float)acos((Get(0,0) + Get(1,1) + Get(2,2) - 1.0f) * 0.5f);
	factor = FastSin(theta);
	if (FastAbs( theta) < 0.0003) {	// theta < 1 minute of arc
		V.x = 0.0f;	V.y = 0.0f;	V.z = 0.0f;
	} else {
		factor = 0.5f / factor;
		V.x = (factor * (Get(2,1) - Get(1,2)));
		V.y = (factor * (Get(0,2) - Get(2,0)));
		V.z = (factor * (Get(1,0) - Get(0,1)));
	}
	return SMLVec4f(V.x, V.y, V.z, theta);
}

void SMLMatrix4f::Transpose()
{

    float val;
    val = data[0][1];
    data[0][1] = data[1][0];
    data[1][0] = val;
    val = data[0][2];
    data[0][2] = data[2][0];
    data[2][0] = val;
    val = data[0][3];
    data[0][3] = data[3][0];
    data[3][0] = val;
    val = data[1][2];
    data[1][2] = data[2][1];
    data[2][1] = val;
    val = data[1][3];
    data[1][3] = data[3][1];
    data[3][1] = val;
    val = data[2][3];
    data[2][3] = data[3][2];
    data[3][2] = val;
}

void SMLMatrix4f::TranslationMatrix(const SMLVec3f &v)
{
	identity = false;
    data[0][0] = 1.0f;
    data[0][1] = 0.0f;
    data[0][2] = 0.0f;
    data[0][3] = v.x;
    data[1][0] = 0.0f;
    data[1][1] = 1.0f;
    data[1][2] = 0.0f;
    data[1][3] = v.y;
    data[2][0] = 0.0f;
    data[2][1] = 0.0f;
    data[2][2] = 1.0f;
    data[2][3] = v.z;
    data[3][0] = 0.0f;
    data[3][1] = 0.0f;
    data[3][2] = 0.0f;
    data[3][3] = 1.0f;
}

void SMLMatrix4f::ScaleMatrix(const SMLVec3f &s)
{
	identity = false;
    data[0][0] = s.x;
    data[0][1] = 0.0f;
    data[0][2] = 0.0f;
    data[0][3] = 0.0f;
    data[1][0] = 0.0f;
    data[1][1] = s.y;
    data[1][2] = 0.0f;
    data[1][3] = 0.0f;
    data[2][0] = 0.0f;
    data[2][1] = 0.0f;
    data[2][2] = s.z;
    data[2][3] = 0.0f;
    data[3][0] = 0.0f;
    data[3][1] = 0.0f;
    data[3][2] = 0.0f;
    data[3][3] = 1.0f;
}


#ifndef _PIII_CODE_
void SMLMatrix4f::RotationMatrix(float theta, float a, float b, float c)
{

	identity = false;

    float cost, sint;
    data[0][3] = 0.0f;
    data[1][3] = 0.0f;
    data[2][3] = 0.0f;
    data[3][3] = 1.0f;
    data[3][0] = 0.0f;
    data[3][1] = 0.0f;
    data[3][2] = 0.0f;
    cost = FastCos(theta);
    sint = FastSin(theta);
    if ( 1 == a && 0 == b && 0 == c) {
        data[0][0] = 1.0f;
        data[0][1] = 0.0f;
        data[0][2] = 0.0f;
        data[1][0] = 0.0f;
        data[1][1] = cost;
        data[1][2] = -sint;
        data[2][0] = 0.0f;
        data[2][1] = sint;
        data[2][2] = cost;
    } else if (0 == a  && 1 == b && 0 == c) {
        data[0][0] = cost;
        data[0][1] = 0.0f;
        data[0][2] = sint;
        data[1][0] = 0.0f;
        data[1][1] = 1.0f;
        data[1][2] = 0.0f;
        data[2][0] = -sint;
        data[2][1] = 0.0f;
        data[2][2] = cost;
    } else if (0 == a && 0 == b && 1 == c) {
        data[0][0] = cost;
        data[0][1] = -sint;
        data[0][2] = 0.0f;
        data[1][0] = sint;
        data[1][1] = cost;
        data[1][2] = 0.0f;
        data[2][0] = 0.0f;
        data[2][1] = 0.0f;
        data[2][2] = 1.0f;
    } else {
		//we need scale a,b,c to unit length.
		float scale = FastSqrt(a*a+b*b+c*c);
		a/=scale;
		b/=scale;
		c/=scale;

        float a2, b2, c2, abm, acm, bcm;
        float mcos, asin, bsin, csin;
        mcos = 1.0f - cost;
        a2 = a * a;
        b2 = b * b;
        c2 = c * c;
        abm = a * b * mcos;
        acm = a * c * mcos;
        bcm = b * c * mcos;
        asin = a * sint;
        bsin = b * sint;
        csin = c * sint;
        data[0][0] = a2 * mcos + cost;
        data[0][1] = abm - csin;
        data[0][2] = acm + bsin;
        data[1][0] = abm + csin;
        data[1][1] = b2 * mcos + cost;
        data[1][2] = bcm - asin;
        data[2][0] = acm - bsin;
        data[2][1] = bcm + asin;
        data[2][2] = c2 * mcos + cost;
    }
}
#endif
