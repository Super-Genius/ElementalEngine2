// Math Inlines 

#include <float.h>

inline void
SMLMatrix3f::Identity()
{
	data[0][0] = 1.0f; data[0][1] = 0.0f; data[0][2] = 0.0f;
	data[1][0] = 0.0f; data[1][1] = 1.0f; data[1][2] = 0.0f;
	data[2][0] = 0.0f; data[2][1] = 0.0f; data[2][2] = 1.0f;
}

inline void
SMLMatrix3f::Zero()
{
	data[0][0] = 0.0f; data[0][1] = 0.0f; data[0][2] = 0.0f;
	data[1][0] = 0.0f; data[1][1] = 0.0f; data[1][2] = 0.0f;
	data[2][0] = 0.0f; data[2][1] = 0.0f; data[2][2] = 0.0f;
}

inline void
SMLMatrix3f::Cross(const SMLVec3f &V)
{	// this = [V]x		cross operator
	data[0][0] = 0.0f; data[0][1] = -V.z; data[0][2] =  V.y;
	data[1][0] =  V.z; data[1][1] = 0.0f; data[1][2] = -V.x;
	data[2][0] = -V.y; data[2][1] =  V.x; data[2][2] = 0.0f;
}

inline void 
SMLAxisAngleMatrix(const SMLVec4f &v, SMLMatrix4f &m)
{
	m.RotationMatrix(v.w, v.x, v.y, v.z);
}

inline void 
SMLMatrix3f::Set(int i1, int i2, float v)
{
	assert(i1 >=0 && i1 < 3 && i2 >=0 && i2 < 3);
	data[i1][i2] = v;
}

inline float
SMLMatrix3f::Get(int i1, int i2) const
{
	assert(i1 >=0 && i1 < 3 && i2 >=0 && i2 < 3);
	return data[i1][i2];
}

inline const SMLMatrix3f &
SMLMatrix3f::operator=(const SMLMatrix3f &src)
{
	Copy(src);
	return *this;
}

inline void SMLMatrix3f::Transpose()
{
    float tmp;
	tmp = data[0][1]; data[0][1] = data[1][0]; data[1][0] = tmp;
	tmp = data[0][2]; data[0][2] = data[2][0]; data[2][0] = tmp;
	tmp = data[1][2]; data[1][2] = data[2][1]; data[2][1] = tmp;
}

inline void SMLMatrix3f::Copy(const SMLMatrix3f &a)
{
   data[0][0] = a.data[0][0]; data[0][1] = a.data[0][1]; data[0][2] = a.data[0][2];
   data[1][0] = a.data[1][0]; data[1][1] = a.data[1][1]; data[1][2] = a.data[1][2];
   data[2][0] = a.data[2][0]; data[2][1] = a.data[2][1]; data[2][2] = a.data[2][2];
}

inline void SMLMatrix4f::Identity()
{	
	if (identity)
		return;

    identity = true;

    data[0][0] = 1.0f;
    data[0][1] = 0.0f;
    data[0][2] = 0.0f;
    data[0][3] = 0.0f;
    data[1][0] = 0.0f;
    data[1][1] = 1.0f;
    data[1][2] = 0.0f;
    data[1][3] = 0.0f;
    data[2][0] = 0.0f;
    data[2][1] = 0.0f;
    data[2][2] = 1.0f;
    data[2][3] = 0.0f;
    data[3][0] = 0.0f;
    data[3][1] = 0.0f;
    data[3][2] = 0.0f;
    data[3][3] = 1.0f;
}

inline void 
SMLMatrix4f::Copy(const SMLMatrix4f &a)
{
	if (identity == a.identity && identity)
	{
		return;
	}
	identity = a.identity;
    data[0][0]=a.data[0][0]; data[0][1]=a.data[0][1]; data[0][2]=a.data[0][2]; data[0][3]=a.data[0][3];
    data[1][0]=a.data[1][0]; data[1][1]=a.data[1][1]; data[1][2]=a.data[1][2]; data[1][3]=a.data[1][3];
    data[2][0]=a.data[2][0]; data[2][1]=a.data[2][1]; data[2][2]=a.data[2][2]; data[2][3]=a.data[2][3];
    data[3][0]=a.data[3][0]; data[3][1]=a.data[3][1]; data[3][2]=a.data[3][2]; data[3][3]=a.data[3][3];
}

inline const 
SMLMatrix4f&
SMLMatrix4f::operator=(const SMLMatrix4f &src)
{
	Copy(src);
	return *this;
}

inline void 
SMLMatrix4f::RotationMatrix(const SMLVec4f &v)
{
	RotationMatrix(v.w, v.x, v.y, v.z);
}
/*inline SMLMatrix4f &operator =(SMLMatrix4f &dst, const SMLMatrix4f &src)
{
	SMLMatrixCopy(src, dst);
	return dst;
}
*/
inline void
SMLMatrix4f::Set(int i1, int i2, float v)
{
	assert(i1 >=0 && i1 < 4 && i2 >=0 && i2 < 4);
	data[i1][i2] = v;
	identity = false;
}
inline float
SMLMatrix4f::Get(int i1, int i2) const
{
	assert(i1 >=0 && i1 < 4 && i2 >=0 && i2 < 4);
	return data[i1][i2];
}

// Offset for data[i][j]
#undef  _I
#define _I(i, j)  4 * (i * 4 + j)

// Load & multiply.
#undef  _LM
#define _LM(k, i, j) \
	__asm fld	DWORD PTR [eax + _I(i, k)] \
	__asm fmul	DWORD PTR [ebx + _I(k, j)]

#undef  _E4
#define _E4(i, j) \
    _LM(0, i, j)  \
    _LM(1, i, j)  \
    _LM(2, i, j)  \
    _LM(3, i, j)  \
    __asm faddp st(1), st(0) \
    __asm fxch  st(2)        \
    __asm faddp st(1), st(0) \
    __asm faddp st(1), st(0) \
	__asm fstp	DWORD PTR [ecx + _I(i, j)] 



inline void
SMLMatrix4f::TransformPoint(const SMLVec3f &a, SMLVec3f &c) const
{
//	c.x = Dot3f(a.data(), data[0]) + data[0][3];
//  c.y = Dot3f(a.data(), data[1]) + data[1][3];
//  c.z = Dot3f(a.data(), data[2]) + data[2][3];

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
}

inline void
SMLMatrix4f::TransformVector(const SMLVec3f &a, SMLVec3f &c) const
{
//	c.x = Dot3f(a.data(), data[0]);
//  c.y = Dot3f(a.data(), data[1]);
//  c.z = Dot3f(a.data(), data[2]);

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
    __asm fstp  DWORD PTR [ecx]     
    __asm fstp  DWORD PTR [ecx+4]   
    __asm fstp  DWORD PTR [ecx+8]  
}


// Load & multiply.
#undef  _LM1
#define _LM1(i, j) \
	__asm fld	DWORD PTR [eax + _I(i, j)] \
	__asm fmul	DWORD PTR [ebx + _I(0, j)]

#undef  _E41
#define _E41(i) \
    _LM1(i, 0)  \
    _LM1(i, 1)  \
    _LM1(i, 2)  \
    _LM1(i, 3)  \
    __asm faddp st(1), st(0) \
    __asm fxch  st(2)        \
    __asm faddp st(1), st(0) \
    __asm faddp st(1), st(0) \
	__asm fstp	DWORD PTR [ecx + _I(0, i)] 


inline float 
Dot2f(const float * const d1, const float * const d2)
{
	return d1[0]*d2[0]+d1[1]*d2[1];
}

inline float 
Dot3f(const float *d1, const float *d2)
{
	return d1[0]*d2[0]+d1[1]*d2[1]+d1[2]*d2[2];
}

inline float 
Dot4f(const float *d1, const float *d2)
{
	return d1[0]*d2[0]+d1[1]*d2[1]+d1[2]*d2[2]+d1[3]*d2[3];
}

///////////Inlines for Vec2f//////////////////////////////////////
inline SMLVec2f::SMLVec2f(float d0, float d1)
: x(d0), y(d1)
{
}

inline SMLVec2f::SMLVec2f(float v[]) 
{
	x = v[0];
	y = v[1];
}

inline SMLVec2f::SMLVec2f(const SMLVec2f &v)
{
	x = v.x;
	y = v.y;
}
inline SMLVec2f::SMLVec2f ()
{
	x = y= 0.0f;
}

inline const SMLVec2f&
SMLVec2f::operator=(const SMLVec2f& rhs)
{
	x = rhs.x;
	y = rhs.y;
	return *this;
}

inline void 
SMLVec2f::Add(const SMLVec2f &v1, const SMLVec2f &v2)
{
	x = v1.x + v2.x;
	y = v1.y + v2.y;
}

inline void 
SMLVec2f::Sub(const SMLVec2f &v1, const SMLVec2f &v2)
{
	x = v1.x - v2.x;
	y = v1.y - v2.y;
}

inline void 
SMLVec2f::Scale(float a,  const SMLVec2f &v)
{
	x = a*v.x;
	y = a*v.y;
}

inline SMLVec2f
SMLVec2f::operator -() const
{
	return SMLVec2f(-x, -y);
}

inline bool 
operator< (SMLVec4f& lhs, SMLVec4f& rhs)
{
	return lhs.LengthSquared() < rhs.LengthSquared();
}

inline bool 
operator< (SMLVec3f& lhs, SMLVec3f& rhs)
{
	return lhs.LengthSquared() < rhs.LengthSquared();
}

inline bool 
operator< (SMLVec2f& lhs, SMLVec2f& rhs)
{
	return lhs.LengthSquared() < rhs.LengthSquared();
}

inline bool
operator==(const SMLVec2f& lhs, const SMLVec2f& rhs)
{
	return (FastAbs(lhs.x - rhs.x) < FLT_EPSILON &&
			 FastAbs(lhs.y - rhs.y) < FLT_EPSILON);
}

inline bool
operator!=(const SMLVec2f& lhs, const SMLVec2f& rhs)
{
	return !(lhs == rhs);
}

inline SMLVec2f
SMLVec2f::operator +(const SMLVec2f &v) const
{
	return SMLVec2f(x+v.x, y+v.y);
}

inline  SMLVec2f
SMLVec2f::operator -(const SMLVec2f &v) const
{
	return SMLVec2f(x-v.x, y-v.y);
}

inline const SMLVec2f& 
SMLVec2f::operator +=(const SMLVec2f &v)
{
	x += v.x;
	y += v.y;
	return *this;
}

inline const SMLVec2f& 
SMLVec2f::operator -=(const SMLVec2f &v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

inline  SMLVec2f 
SMLVec2f::operator *(float f) const
{
	return SMLVec2f(x*f, y*f);
}

inline  SMLVec2f 
SMLVec2f::operator /(float f) const
{
	assert(0 != f);
	return SMLVec2f(x/f, y/f);
}

inline const SMLVec2f&
SMLVec2f::operator *=(float f)
{
	x *= f;
	y *= f;
	return *this;
}

inline const SMLVec2f& 
SMLVec2f::operator /=(float f)
{
	assert(0 != f);
	return operator *=(1.0f/f);
}

inline float 
SMLVec2f::Dot(const SMLVec2f &v) const
{
	return Dot2f(data(), v.data());
}

inline float 
SMLVec2f::Dot(const float *v) const
{
	return Dot2f(data(), v);
}

inline float 
SMLVec2f::Length() const
{
	return FastSqrt(LengthSquared());
}

inline float 
SMLVec2f::LengthSquared() const
{
	return Dot2f(data(), data());
}

inline float 
SMLVec2f::Normalize()
{
	assert(LengthSquared()>0.0f);
	float li = InverseSqrt(LengthSquared());
	if (li < 1.0f/FLT_EPSILON) {
		x*=li;
		y*=li; 
	} else {
		x = 0.0f;
		y = 1.0f;
	}
	return li;
}

inline float 
SMLVec2f::DistanceSquared(const SMLVec2f &rhs) const
{
	return (*this-rhs).LengthSquared();
}

inline float 
SMLVec2f::Distance(const SMLVec2f &rhs) const
{
	return (*this-rhs).Length();
}

inline const SMLVec2f& SMLVec2f::Negate()
{
	x = -x;
	y = -y;
	return *this;
}

///////////Inlines for Vec3f//////////////////////////////////////
inline SMLVec3f::SMLVec3f(float a, float b, float c) 
{
	x = a;
	y = b;
	z = c;
}

inline SMLVec3f::SMLVec3f(float v[]) 
: x(v[0]), y(v[1]), z(v[2])
{
}

inline SMLVec3f::SMLVec3f(const SMLVec3f &v)
: x(v.x), y(v.y), z(v.z)
{
}

inline SMLVec3f::SMLVec3f()
: x(0), y(0), z(0)
{
}

inline const SMLVec3f&
SMLVec3f::operator=(const SMLVec3f& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	return *this;
}

inline void 
SMLVec3f::Set(const SMLVec3f &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
}

inline void 
SMLVec3f::Set(float xx, float yy, float zz)
{
	x = xx;
	y = yy;
	z = zz;
}

inline void 
SMLVec3f::Add(const SMLVec3f &v1, const SMLVec3f &v2)
{
	x = v1.x + v2.x;
	y = v1.y + v2.y;
	z = v1.z + v2.z;
}

inline void 
SMLVec3f::Sub(const SMLVec3f &v1, const SMLVec3f &v2)
{
	x = v1.x - v2.x;
	y = v1.y - v2.y;
	z = v1.z - v2.z;
}

inline void 
SMLVec3f::Scale(float a,  const SMLVec3f &v)
{
	x = a*v.x;
	y = a*v.y;
	z = a*v.z;
}

inline void 
SMLVec3f::ScaleAdd(float a, const SMLVec3f &v1, const SMLVec3f &v2)
{
	x = a*v1.x + v2.x;
	y = a*v1.y + v2.y;
	z = a*v1.z + v2.z;
}

inline SMLVec3f 
SMLVec3f::operator +(const SMLVec3f &v) const
{
	return SMLVec3f(x+v.x, y+v.y, z+v.z);
}

inline SMLVec3f 
SMLVec3f::operator -(const SMLVec3f &v) const
{
	return SMLVec3f(x-v.x, y-v.y, z-v.z);
}

inline const SMLVec3f& 
SMLVec3f::operator +=(const SMLVec3f &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

inline const SMLVec3f& 
SMLVec3f::operator -=(const SMLVec3f &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

inline SMLVec3f 
SMLVec3f::operator *(float f) const
{
	return SMLVec3f(x*f, y*f, z*f);
}

inline SMLVec3f 
SMLVec3f::operator /(float f) const
{
	assert(0 != f);
	return SMLVec3f(x/f, y/f, z/f);
}

inline const SMLVec3f&
SMLVec3f::operator *=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

inline const SMLVec3f& 
SMLVec3f::operator /=(float f)
{
	assert(0 != f);
	return operator*=(1.0f/f);
}

inline SMLVec3f 
SMLVec3f::operator -() const
{
	return SMLVec3f(-x, -y, -z);
}

inline float&
SMLVec3f::operator [](int index)
{
	assert(index >=0 && index <= 2);
	return data()[index];
}

inline float
SMLVec3f::operator [](int index) const
{
	assert(index >=0 && index <= 2);
	return data()[index];
}

inline float 
SMLVec3f::Dot(const SMLVec3f &v) const
{
	return Dot3f(data(), v.data());
}

inline float 
SMLVec3f::Dot(const float *v) const
{
	return Dot3f(data(), v);
}

inline float 
SMLVec3f::Length() const
{
	return FastSqrt(LengthSquared());
}

inline float 
SMLVec3f::LengthSquared() const
{
	return Dot3f(data(), data());
}

inline float SMLVec3f::Distance(const SMLVec3f &v) const
{
	return ((*this) - v).Length();
}

inline float 
SMLVec3f:: DistanceSquared(const SMLVec3f &v) const
{
	return ((*this) - v).LengthSquared();
}

inline SMLVec3f
SMLVec3f::Cross(const SMLVec3f &v) const
{
	return SMLVec3f(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}

inline float
SMLVec3f::operator*(const SMLVec3f &v) const
{
    return this->Dot(v);
}

inline const SMLVec3f&
 SMLVec3f::Negate()
{
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

inline void  
SMLVec3f::TransformPoint(const SMLMatrix4f &mat, const SMLVec3f &tmp) 
{
	mat.TransformPoint(tmp, *this);
}

inline void  
SMLVec3f::TransformVector(const SMLMatrix4f &mat, const SMLVec3f &tmp) 
{
	mat.TransformVector(tmp, *this);
}

inline void SMLVec4f::Transform(const SMLMatrix4f &mat, const SMLVec4f &tmp)
{
	mat.Transform(tmp, *this);
}

inline void SMLVec4f::Transform(const SMLMatrix4f &mat, const SMLVec3f &tmp)
{
	mat.Transform(tmp, *this);
}

///////////Inlines for Vec4f//////////////////////////////////////

inline const SMLVec4f&
SMLVec4f::operator =(const SMLVec4f &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
	return *this;
}

inline SMLVec4f 
SMLVec4f::operator +(const SMLVec4f &v) const
{
	return SMLVec4f(x+v.x, y+v.y, z+v.z, w+v.w);
}

inline SMLVec4f 
SMLVec4f::operator -(const SMLVec4f &v) const
{
	return SMLVec4f(x-v.x, y-v.y, z-v.z, w-v.w);
}

inline const SMLVec4f& 
SMLVec4f::operator +=(const SMLVec4f &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

inline const SMLVec4f& 
SMLVec4f::operator -=(const SMLVec4f &v)
{
	x -= v.x;
	y -= v.y; 
	z -= v.z;
	w -= v.w;
	return *this;
}

inline SMLVec4f 
SMLVec4f::operator *(float f) const
{
	// Should we leave w alone?
	return SMLVec4f(x*f, y*f, z*f, w*f);
}

inline SMLVec4f 
SMLVec4f::operator /(float f) const
{
	assert(0 != f);
	return operator*(1.0f/f);
}

inline const SMLVec4f& 
SMLVec4f::operator *=(float f)
{
	// Whould we leave w alone?
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

inline const SMLVec4f& 
SMLVec4f::operator /=(float f)
{
	assert(0 != f);
	return operator*=(1.0f/f);
}

inline float 
SMLVec4f::Dot(const SMLVec4f &v) const
{
	return Dot4f(data(), v.data());
}

inline float 
SMLVec4f::Length() const
{
	return FastSqrt(LengthSquared());
}

inline float 
SMLVec4f::LengthSquared() const
{
	return Dot4f(data(), data());
}

inline float 
SMLVec4f::Distance(const SMLVec4f &v) const
{
	return (*this - v).Length();
}

inline float 
SMLVec4f::DistanceSquared(const SMLVec4f &v) const
{
	return (*this - v).LengthSquared();
}

inline const SMLVec4f& SMLVec4f::Negate()
{
	x = -x;
	y = -y;
	z = -z;
	w = -w;
	return *this;
}

inline void 
SMLVec4f::Set(float xx, float yy, float zz, float ww)
{
	x = xx;
	y = yy;
	z = zz;
	w = ww;
}

inline void 
SMLVec4f::Add(const SMLVec4f &v1, const SMLVec4f &v2)
{
	x = v1.x + v2.x;
	y = v1.y + v2.y;
	z = v1.z + v2.z;
	w = v1.w + v2.w;
}

inline void 
SMLVec4f::Sub(const SMLVec4f &v1, const SMLVec4f &v2)
{
	x = v1.x - v2.x;
	y = v1.y - v2.y;
	z = v1.z - v2.z;
	w = v1.w - v2.w;
}

inline void 
SMLVec4f::Scale(float a,  const SMLVec4f &v)
{
	x = a*v.x;
	y = a*v.y;
	z = a*v.z;
	w = a*v.w;
}

inline SMLVec4f 
SMLVec4f::operator -() const
{
	return SMLVec4f(-x, -y, -z, -w);
}

// SMLPlane is just SMLVec4f with a Distance.
inline
SMLPlane::SMLPlane(float a, float b, float c, float d) : SMLVec4f(a, b, c, d) {}
inline
SMLPlane::SMLPlane(float  v[]) : SMLVec4f(v) {}
inline
SMLPlane::SMLPlane(const SMLPlane &v) : SMLVec4f(v) {}
inline
SMLPlane::SMLPlane()
{
}
inline const SMLPlane& 
SMLPlane::operator=(const SMLPlane &plane) 
{ 
	SMLVec4f::operator=(plane); 
	return *this;
}

inline float 
SMLPlane::Distance(const SMLVec3f &v) const
{
	return Dot3f(data(), v.data()) + w;
}

inline bool
operator==(const SMLVec4f &lhs, const SMLVec4f &rhs)
{
	return(FastAbs(lhs.x-rhs.x) < FLT_EPSILON &&
		   FastAbs(lhs.y-rhs.y) < FLT_EPSILON &&
		   FastAbs(lhs.z-rhs.z) < FLT_EPSILON &&
		   FastAbs(lhs.w-rhs.w) < FLT_EPSILON);
}

inline bool
operator!=(const SMLVec4f &lhs, const SMLVec4f &rhs)
{
	return !(lhs == rhs);
}

inline bool
operator==(const SMLVec3f& lhs, const SMLVec3f &rhs)
{
	return(FastAbs(lhs.x-rhs.x) < FLT_EPSILON &&
		   FastAbs(lhs.y-rhs.y) < FLT_EPSILON &&
		   FastAbs(lhs.z-rhs.z) < FLT_EPSILON);
}

inline bool
operator!=(const SMLVec3f& lhs, const SMLVec3f &rhs)
{
	return !(lhs == rhs);
}

inline SMLLine::SMLLine()
{
}

inline SMLLine::SMLLine(const SMLVec3f &s, const SMLVec3f &e)
: start(s), end(e)
{
}

inline SMLRay::SMLRay()
{
	length = a = 0;
}

inline SMLRay::SMLRay(const SMLVec3f &s, const SMLVec3f &e)
: SMLLine(s, e)
{
// Since we already paid the price of finding direction length.
// we can normalize the direction vector.
	direction = end-start;
	length = direction.Length();
	assert(length > FLT_EPSILON);
	direction *= 1.0f/length;
//	a = direction.Dot(direction);
}	

inline SMLRay::SMLRay(const SMLVec3f &s, const SMLVec3f &d, float l)
: direction(d), length(l)
{
// We assume user supply a normalized vector d.
	assert(FastAbs(d.LengthSquared()-1.0f) < FLT_EPSILON);
	start = s;
	end = start + direction * l;
//	a = direction.Dot(direction);
}

inline void 
SMLMatrix3f::GetMatrix4f(SMLMatrix4f &m)
{
	m.Set(0,0, (float)data[0][0]); m.Set(0,1, (float)data[0][1]); m.Set(0,2, (float)data[0][2]); m.Set(0,3, 0.0f);
	m.Set(1,0, (float)data[1][0]); m.Set(1,1, (float)data[1][1]); m.Set(1,2, (float)data[1][2]); m.Set(1,3, 0.0f);
	m.Set(2,0, (float)data[2][0]); m.Set(2,1, (float)data[2][1]); m.Set(2,2, (float)data[2][2]); m.Set(2,3, 0.0f);
	m.Set(3,0, 0.0f);              m.Set(3,1, 0.0f);              m.Set(3,2, 0.0f);              m.Set(3,3, 1.0f);
}

inline void 
SMLMatrix3f::SetByMatrix4f(const SMLMatrix4f &m)
{
	data[0][0] = m.Get(0,0); data[0][1] = m.Get(0,1); data[0][2] = m.Get(0,2);
	data[1][0] = m.Get(1,0); data[1][1] = m.Get(1,1); data[1][2] = m.Get(1,2);
	data[2][0] = m.Get(2,0); data[2][1] = m.Get(2,1); data[2][2] = m.Get(2,2);
}

inline float 
SMLVec3f::Normalize()
{
	assert(LengthSquared()>0.0f);
	float li = InverseSqrt(LengthSquared());
	if (li < 1.0f/FLT_EPSILON) {
		operator*=(li);
	} else {
		x = 0.0f;
		y = 1.0f;
		z = 0.0f;
	}
	return li;
}

inline float 
SMLVec4f::Normalize()
{
	assert(LengthSquared()>0.0f);
	float li = InverseSqrt(LengthSquared());
	if (li < 1.0f/FLT_EPSILON) {
		operator*=(li);
	} else {
		x = 0.0f;
		y = 1.0f;
		z = 0.0f;
		w = 0.0f;
	}
	return li;
}

inline
SMLVec4f::SMLVec4f(float d0, float d1, float d2, float d3)
: x(d0), y(d1), z(d2), w(d3)
{
}

inline
SMLVec4f::SMLVec4f(float v[])
: x(v[0]), y(v[1]), z(v[2]), w(v[3])
{
}

inline
SMLVec4f::SMLVec4f(const SMLVec4f &v)
: x(v.x), y(v.y), z(v.z), w(v.w)
{
}

inline
SMLVec4f::SMLVec4f()
: x(0), y(0), z(0), w(0)
{
}

inline
SMLMatrix3f::SMLMatrix3f()
{
	Identity();
}

inline
SMLMatrix3f::SMLMatrix3f(const SMLMatrix3f& rhs)
{
    Copy(rhs);
}

inline
SMLMatrix3f::SMLMatrix3f(float* s)
{
    data[0][0]=*s++; data[0][1]=*s++; data[0][2]=*s++;
    data[1][0]=*s++; data[1][1]=*s++; data[1][2]=*s++;
    data[2][0]=*s++; data[2][1]=*s++; data[2][2]=*s;
}

inline void
SMLMatrix3f::Rotate(float rads, float x, float y, float z)
{
	SMLMatrix4f m;
	m.RotationMatrix(rads, x, y, z);
	data[0][0] = m.Get(0,0);  data[0][1] = m.Get(0,1);  data[0][2] = m.Get(0,2);
	data[1][0] = m.Get(1,0);  data[1][1] = m.Get(1,1);  data[1][2] = m.Get(1,2);
	data[2][0] = m.Get(2,0);  data[2][1] = m.Get(2,1);  data[2][2] = m.Get(2,2);
}

inline void
SMLMatrix3f::Rotate(float radians, const SMLVec3f& axis)
{
	Rotate(radians, axis.x, axis.y, axis.z);
}

inline void
SMLMatrix3f::QuaternionMatrix(const SMLVec4f &q) {
	float q11,q12,q13,q14,q22,q23,q24,q33,q34,q44;
	q11 = q.x * q.x; q12 = q.x * q.y; q13 = q.x * q.z; q14 = q.x * q.w;
	q22 = q.y * q.y; q23 = q.y * q.z; q24 = q.y * q.w;
	q33 = q.z * q.z; q34 = q.z * q.w;
	q44 = q.w * q.w;
	Set(0, 0, (q11-q22-q33+q44));
	Set(0, 1, 2*(q12+q34));
	Set(0, 2, 2*(q13-q24));
	Set(1, 0, 2*(q12-q34));
	Set(1, 1, (q22-q33-q11+q44));
	Set(1, 2, 2*(q23+q14));
	Set(2, 0, 2*(q13+q24));
	Set(2, 1, 2*(q23-q14));
	Set(2, 2, (q33-q11-q22+q44));
}

inline
SMLMatrix3f::~SMLMatrix3f()
{
}

inline

SMLMatrix4f::SMLMatrix4f():identity(false)
{
	data_elem = &data[0][0];
	Identity();
}

inline
SMLMatrix4f::SMLMatrix4f(const SMLMatrix4f& rhs)
{
	data_elem = &data[0][0];
	Copy(rhs);
	identity = rhs.identity;
}

inline
SMLMatrix4f::SMLMatrix4f(float *s) : identity(false)
{
    data_elem = &data[0][0];
    data[0][0]=*s++; data[0][1]=*s++; data[0][2]=*s++; data[0][3]=*s++;
    data[1][0]=*s++; data[1][1]=*s++; data[1][2]=*s++; data[1][3]=*s++;
    data[2][0]=*s++; data[2][1]=*s++; data[2][2]=*s++; data[2][3]=*s++;
    data[3][0]=*s++; data[3][1]=*s++; data[3][2]=*s++; data[3][3]=*s;
}

inline
SMLMatrix4f::~SMLMatrix4f()
{
}

inline void
SMLMatrix4f::Zero()
{
    data[0][0] = 0.0f;
    data[0][1] = 0.0f;
    data[0][2] = 0.0f;
    data[0][3] = 0.0f;
    data[1][0] = 0.0f;
    data[1][1] = 0.0f;
    data[1][2] = 0.0f;
    data[1][3] = 0.0f;
    data[2][0] = 0.0f;
    data[2][1] = 0.0f;
    data[2][2] = 0.0f;
    data[2][3] = 0.0f;
    data[3][0] = 0.0f;
    data[3][1] = 0.0f;
    data[3][2] = 0.0f;
    data[3][3] = 0.0f;
	identity = false;
}

inline
SMLRay &SMLRay::operator=(SMLRay& r)
{
	start = r.start;
	end  = r.end;
	direction = r.direction;
	length = r.length;
	a = r.a;
	return *this;
}

