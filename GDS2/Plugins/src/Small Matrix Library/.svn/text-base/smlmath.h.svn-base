
#ifndef _SMLMATH_H
#define _SMLMATH_H

#include <windows.h>

#include <assert.h>
#include <math.h>

#ifndef SMLLIBENTRY
#if defined(_SMLDLL_)
	#define SMLLIBENTRY __declspec(dllexport)
#else
	#if defined(_SMLSTATIC_)
		#define SMLLIBENTRY
	#else
		#define SMLLIBENTRY __declspec(dllimport)
	#endif
#endif
#endif

SMLLIBENTRY float __fastcall FastAbs(float a);
SMLLIBENTRY float __fastcall FastSin(float a);
SMLLIBENTRY float __fastcall FastCos(float a);
SMLLIBENTRY float __fastcall InverseSqrt(float a);
SMLLIBENTRY inline float FastSqrt(float a) {
	return a? a * InverseSqrt(a) : 0;
}


#pragma warning(disable: 4786)
#pragma warning(disable: 4056)

class SMLVec2f;
class SMLVec3f;
class SMLVec4f;
class SMLSphereBound;

////////Methods for Matrix3f///////////////////////////////////////////////
class SMLMatrix4f;


class SMLMatrix3f
{
public:
	SMLLIBENTRY SMLMatrix3f();
	SMLLIBENTRY SMLMatrix3f(float* src);
	SMLLIBENTRY SMLMatrix3f(const SMLMatrix3f& rhs);
	SMLLIBENTRY ~SMLMatrix3f();
	SMLLIBENTRY const SMLMatrix3f& operator=(const SMLMatrix3f &src);

	// Access elements
	SMLLIBENTRY void  Set(int i1, int i2, float);
	SMLLIBENTRY float Get(int i1, int i2) const;

	SMLLIBENTRY void TransformPoint(const SMLVec3f &src, SMLVec3f &dst, int transpose) const;
	SMLLIBENTRY void TransformPoint(const SMLVec3f &src, SMLVec3f &dst) const;
	SMLLIBENTRY void TransformPoint1(const SMLVec3f &src, SMLVec3f &dst) const;
	SMLLIBENTRY void Multiply(const SMLMatrix3f& a, const SMLMatrix3f& b, int transpose_a, int transpose_b);
	SMLLIBENTRY void Multiply(const SMLMatrix3f& a, const SMLMatrix3f& b);

	SMLLIBENTRY void Copy (const SMLMatrix3f &src);
	SMLLIBENTRY void Identity();
	SMLLIBENTRY void Zero();
    SMLLIBENTRY void Cross(const SMLVec3f &vec);
    SMLLIBENTRY void Rotate(float radians, float x, float y, float z);
    SMLLIBENTRY void Rotate(float radians, const SMLVec3f& axis);
    SMLLIBENTRY void GetMatrix4f(SMLMatrix4f &m);
    SMLLIBENTRY void SetByMatrix4f(const SMLMatrix4f &m);
    SMLLIBENTRY void Output(char *label1 = NULL, char *label2 = NULL);
	SMLLIBENTRY const float *GetData() const { return &data[0][0]; }
	SMLLIBENTRY float *GetData() { return &data[0][0]; }

//	SMLLIBENTRY void Invert(const SMLMatrix3f &src);
	SMLLIBENTRY void Transpose();
	SMLLIBENTRY	void AxisAngleMatrix(const SMLVec4f&);
	SMLLIBENTRY void XAngleMatrix (float);
	SMLLIBENTRY	void YAngleMatrix (float);
	SMLLIBENTRY	void ZAngleMatrix (float);
	SMLLIBENTRY void QuaternionMatrix(const SMLVec4f &);
	

protected:

	float data[3][3];
};
/////////////////////////////////////////////////////////////////////////////
////////Methods for Matrix4f/////////////////////////////////////////////////
class SMLMatrix4f
{
public:
	SMLLIBENTRY SMLMatrix4f();
	SMLLIBENTRY SMLMatrix4f(float*);
	SMLLIBENTRY SMLMatrix4f(const SMLMatrix4f& rhs);
	SMLLIBENTRY ~SMLMatrix4f();
	SMLLIBENTRY const SMLMatrix4f& operator=(const SMLMatrix4f &src);

	// Access elements
	SMLLIBENTRY void  Set(int i1, int i2, float);
	SMLLIBENTRY float Get(int i1, int i2) const;

	SMLLIBENTRY void Copy (const SMLMatrix4f &src);
	SMLLIBENTRY void Identity();
	SMLLIBENTRY void Zero();
	SMLLIBENTRY void Add(const SMLMatrix4f &m1, const SMLMatrix4f &m2);
	SMLLIBENTRY void Sub(const SMLMatrix4f &m1, const SMLMatrix4f &m2);
	SMLLIBENTRY void Invert(SMLMatrix4f &src);
	SMLLIBENTRY void Transpose();
	SMLLIBENTRY void ScaleMatrix(const SMLVec3f &);
	SMLLIBENTRY void TranslationMatrix(const SMLVec3f &	);
	SMLLIBENTRY void RotationMatrix(float theta, float x, float y, float z);
	SMLLIBENTRY void RotationMatrix(const SMLVec4f &);
	SMLLIBENTRY void Multiply(const SMLMatrix4f&, const SMLMatrix4f&);
    SMLLIBENTRY void Output(char *label1 = NULL, char *label2 = NULL);
	SMLLIBENTRY const float *GetData() const { return &data[0][0]; }
	SMLLIBENTRY float *GetData() { return &data[0][0]; }

//Transformations for points, vetors, and spheres
	SMLLIBENTRY void TransformPoint(const SMLVec3f &src, SMLVec3f &dst) const;
	SMLLIBENTRY void TransformVector(const SMLVec3f &src, SMLVec3f &dst) const;
	SMLLIBENTRY void Transform(const SMLVec4f &src, SMLVec4f &dst) const;
	SMLLIBENTRY void Transform(const SMLVec3f &src, SMLVec4f &dst) const;
	SMLLIBENTRY void Transform(const SMLSphereBound &src, SMLSphereBound &dst) const;
	SMLLIBENTRY SMLVec4f GetRotationVector() const;
	SMLLIBENTRY bool IsIdentity() { return identity; }

protected:
	float data[4][4];
	float *data_elem;
	bool  identity;
};

SMLLIBENTRY void SMLAxisAngleMatrix(const SMLVec4f &, SMLMatrix4f &dest);

//////////////////////////////////////////////////////////////////////////////


float Dot2f(const float * const, const float * const);
float Dot3f(const float*, const float*);
float Dot4f(const float*, const float*);

//////////////////////Composite Types////////////////////////////////////////
class SMLVec2f
{
public:
	float x,y;

	// Access the data as an array
	SMLLIBENTRY const float *data() const { return &x; }
	SMLLIBENTRY float *data() { return &x; }

	// Rename to XY
	SMLLIBENTRY float  X() const { return x; }
	SMLLIBENTRY float  Y() const { return y; }
	SMLLIBENTRY float& X() { return x; }
	SMLLIBENTRY float& Y() { return y; }

	//Constructors
	SMLLIBENTRY SMLVec2f(float a, float b);
	SMLLIBENTRY SMLVec2f(float v[]);
	SMLLIBENTRY SMLVec2f(const SMLVec2f &v);
	SMLLIBENTRY SMLVec2f();
	SMLLIBENTRY const SMLVec2f& operator=(const SMLVec2f&);

	friend SMLLIBENTRY bool operator< (const SMLVec2f&, const SMLVec2f&);
	friend SMLLIBENTRY bool operator==(const SMLVec2f&, const SMLVec2f&);
	friend SMLLIBENTRY bool operator!=(const SMLVec2f&, const SMLVec2f&);

	//Operators
	SMLLIBENTRY SMLVec2f operator +(const SMLVec2f &) const;
	SMLLIBENTRY SMLVec2f operator -(const SMLVec2f &) const;
	SMLLIBENTRY const SMLVec2f &operator +=(const SMLVec2f &);
	SMLLIBENTRY const SMLVec2f &operator -=(const SMLVec2f &);
	SMLLIBENTRY SMLVec2f operator *(float) const;
	SMLLIBENTRY SMLVec2f operator /(float) const;
	SMLLIBENTRY const SMLVec2f &operator *=(float);
	SMLLIBENTRY const SMLVec2f &operator /=(float);
	SMLLIBENTRY SMLVec2f operator -() const;
	//	SMLLIBENTRY SMLVec2f &operator *(const SMLMatrix2f &) const;
	//	SMLLIBENTRY SMLVec2f &operator *=(const SMLMatrix2f &);
	
	//Methods
	SMLLIBENTRY void Add(const SMLVec2f &, const SMLVec2f &);
	SMLLIBENTRY void Sub(const SMLVec2f &, const SMLVec2f &);
	SMLLIBENTRY void Scale(float, const SMLVec2f &);
	
	SMLLIBENTRY float Dot(const SMLVec2f &) const;
	SMLLIBENTRY float Dot(const float*) const;
	SMLLIBENTRY const SMLVec2f &Negate();
		
	SMLLIBENTRY float Length() const;
	SMLLIBENTRY float LengthSquared() const;
	SMLLIBENTRY float Normalize();
	SMLLIBENTRY float Distance(const SMLVec2f &) const;
	SMLLIBENTRY float DistanceSquared(const SMLVec2f &v) const;

	//Fast methods
};

class SMLVec3f
{
//Data
public:
	float x, y, z;
	
	// Access the data as an array.
	SMLLIBENTRY const float *data() const { return &x; }
	SMLLIBENTRY float* data() { return &x; }

	// Rename to XYZ
	SMLLIBENTRY float  X() const { return x; }
	SMLLIBENTRY float  Y() const { return y; }
	SMLLIBENTRY float  Z() const { return z; }
	SMLLIBENTRY float& X() { return x; }
	SMLLIBENTRY float& Y() { return y; }
	SMLLIBENTRY float& Z() { return z; }

	// Rename to RGB
	SMLLIBENTRY float  Red()   const { return x; }
	SMLLIBENTRY float  Green() const { return y; }
	SMLLIBENTRY float  Blue()  const { return z; }
	SMLLIBENTRY float& Red()   { return x; }
	SMLLIBENTRY float& Green() { return y; }
	SMLLIBENTRY float& Blue()  { return z; }

//Constructors
	SMLLIBENTRY SMLVec3f(float a, float b, float c);
	SMLLIBENTRY SMLVec3f(float  v[]);
	SMLLIBENTRY SMLVec3f(const SMLVec3f &v);
	SMLLIBENTRY SMLVec3f();
	SMLLIBENTRY const SMLVec3f& operator=(const SMLVec3f&);

	friend SMLLIBENTRY bool operator< (const SMLVec3f&, const SMLVec3f&);
	friend SMLLIBENTRY bool operator==(const SMLVec3f&, const SMLVec3f&);
	friend SMLLIBENTRY bool operator!=(const SMLVec3f&, const SMLVec3f&);

//Operators
	SMLLIBENTRY SMLVec3f operator +(const SMLVec3f &) const;
	SMLLIBENTRY SMLVec3f operator -(const SMLVec3f &) const;
	SMLLIBENTRY SMLVec3f operator *(float) const;
	SMLLIBENTRY SMLVec3f operator /(float) const;

	SMLLIBENTRY const SMLVec3f &operator +=(const SMLVec3f &);
	SMLLIBENTRY const SMLVec3f &operator -=(const SMLVec3f &);
	SMLLIBENTRY const SMLVec3f &operator *=(float);
	SMLLIBENTRY const SMLVec3f &operator /=(float);
	SMLLIBENTRY SMLVec3f operator -() const;

	SMLLIBENTRY float& operator[](int index);
	SMLLIBENTRY float  operator[](int index) const;

	
//Methods
	SMLLIBENTRY void  Set(const SMLVec3f &);
	SMLLIBENTRY void  Set(float, float, float);
	SMLLIBENTRY void  Add(const SMLVec3f &, const SMLVec3f &);
	SMLLIBENTRY void  Sub(const SMLVec3f &, const SMLVec3f &);
	SMLLIBENTRY void  Scale(float,  const SMLVec3f &);
	SMLLIBENTRY void  ScaleAdd(float, const SMLVec3f &, const SMLVec3f &);
	SMLLIBENTRY float Dot(const SMLVec3f &) const;
	SMLLIBENTRY float Dot(const float * const) const;
	SMLLIBENTRY const SMLVec3f &Negate();
	SMLLIBENTRY float Length() const;
	SMLLIBENTRY float LengthSquared() const;
	SMLLIBENTRY float Distance(const SMLVec3f &) const;
	SMLLIBENTRY float DistanceSquared(const SMLVec3f &v) const;
				
//Methods
	SMLLIBENTRY SMLVec3f Cross(const SMLVec3f &) const;
	SMLLIBENTRY float operator*(const SMLVec3f &) const;
	SMLLIBENTRY float Normalize();
	SMLLIBENTRY void  TransformPoint(const SMLMatrix4f &, const SMLVec3f &);
	SMLLIBENTRY void  TransformVector(const SMLMatrix4f &, const SMLVec3f &);
};

class SMLVec4f
{
//Data
public:
	float x, y, z, w;

	// Access the data as an array
	SMLLIBENTRY const float *data() const { return &x; }
	SMLLIBENTRY float *data() { return &x; }
		
	// Rename to XYZW
	SMLLIBENTRY float X() const { return x; }
	SMLLIBENTRY float Y() const { return y; }
	SMLLIBENTRY float Z() const { return z; }
	SMLLIBENTRY float W() const { return w; }
		
	SMLLIBENTRY float& X() { return x; }
	SMLLIBENTRY float& Y() { return y; }
	SMLLIBENTRY float& Z() { return z; }
	SMLLIBENTRY float& W() { return w; }

	// Rename to RGBA
	SMLLIBENTRY float Red()   const { return x; }
	SMLLIBENTRY float Green() const { return y; }
	SMLLIBENTRY float Blue()  const { return z; }
	SMLLIBENTRY float Alpha() const { return w; }
		
	SMLLIBENTRY float& Red()   { return x; }
	SMLLIBENTRY float& Green() { return y; }
	SMLLIBENTRY float& Blue()  { return z; }
	SMLLIBENTRY float& Alpha() { return w; }

	//Constructors
	SMLLIBENTRY SMLVec4f(float a, float b, float c, float d);
	SMLLIBENTRY SMLVec4f(float  v[]);
	SMLLIBENTRY SMLVec4f(const SMLVec4f &v);
	SMLLIBENTRY SMLVec4f();
	SMLLIBENTRY const SMLVec4f &operator =(const SMLVec4f &);

	friend SMLLIBENTRY bool operator==(const SMLVec4f&, const SMLVec4f&);
	friend SMLLIBENTRY bool operator!=(const SMLVec4f&, const SMLVec4f&);
	friend SMLLIBENTRY bool operator < (const SMLVec4f &, const SMLVec4f &);
	//Operators
	SMLLIBENTRY SMLVec4f operator +(const SMLVec4f &) const;
	SMLLIBENTRY SMLVec4f operator -(const SMLVec4f &) const;
	SMLLIBENTRY const SMLVec4f & operator +=(const SMLVec4f &) ;
	SMLLIBENTRY const SMLVec4f &operator -=(const SMLVec4f &);
	SMLLIBENTRY SMLVec4f operator *(float) const;
	SMLLIBENTRY SMLVec4f operator /(float) const;
	SMLLIBENTRY const SMLVec4f &operator *=(float);
	SMLLIBENTRY const SMLVec4f &operator /=(float);
	SMLLIBENTRY SMLVec4f operator -() const;
	
//Methods
	SMLLIBENTRY void  Set(float, float, float, float);
	SMLLIBENTRY void  Transform(const SMLMatrix4f &, const SMLVec4f &);
	SMLLIBENTRY void  Transform(const SMLMatrix4f &, const SMLVec3f &);
	SMLLIBENTRY float Dot(const SMLVec4f &) const;
	SMLLIBENTRY const SMLVec4f &Negate();
	SMLLIBENTRY void  Add(const SMLVec4f &, const SMLVec4f &);
	SMLLIBENTRY void  Sub(const SMLVec4f &, const SMLVec4f &);
	SMLLIBENTRY void  Scale(float, const SMLVec4f &);
		
	SMLLIBENTRY float Length() const;
	SMLLIBENTRY float LengthSquared() const;
	SMLLIBENTRY float Normalize();
	SMLLIBENTRY float Distance(const SMLVec4f &) const;
	SMLLIBENTRY float DistanceSquared(const SMLVec4f &v) const;
};

class SMLLine
{
public:
	SMLVec3f start, end; 
	SMLLIBENTRY SMLLine(const SMLVec3f &start, const SMLVec3f &end);
	SMLLIBENTRY SMLLine();
};

class SMLRay : public SMLLine
{
public:
	SMLVec3f direction;
	float length, a;
	SMLLIBENTRY SMLRay(const SMLVec3f &start, const SMLVec3f &direction, float length);
	SMLLIBENTRY SMLRay(const SMLVec3f &start, const SMLVec3f &end);
	SMLLIBENTRY SMLRay();
	SMLLIBENTRY SMLRay &operator =(SMLRay &ray);
};

class SMLPlane: public SMLVec4f
{
public:
    // This class has the same ctors as SMLVec4f and also Distance() member.
	SMLLIBENTRY SMLPlane(float a, float b, float c, float d);
	SMLLIBENTRY SMLPlane(float  v[]);
	SMLLIBENTRY SMLPlane(const SMLPlane &v);
	SMLLIBENTRY SMLPlane();
	SMLLIBENTRY const SMLPlane &operator =(const SMLPlane &);
	SMLLIBENTRY float Distance(const SMLVec3f &) const;
};

#include "smlmath.inl"	
#include <smlbound.h>

#endif
