/*****************************************************************************

	bioviewer
		
    Copyright (C) 2002  Lucas Walter

  	lucasw@u.washington.edu
	http://students.washington.edu/lucasw

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


  thanks to Trent Polack (ShiningKnight)
	for the tutorial

******************************************************************************/

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

													
//#include <iostream>

#include <math.h>

#include "BvhMVMath.h"

using namespace std;

void Normal(vector3f* v1, vector3f* v2, vector3f* v3)
{
	vector3f a(0.0f, 0.0f, 0.0f);
	vector3f b(0.0f, 0.0f, 0.0f);
	vector3f result(0.0f, 0.0f, 0.0f);
	float length;

	a.vertex[0]= v1->vertex[0] - v2->vertex[0];
	a.vertex[1]= v1->vertex[1] - v2->vertex[1];
	a.vertex[2]= v1->vertex[2] - v2->vertex[2];

	b.vertex[0]= v1->vertex[0] - v3->vertex[0];
	b.vertex[1]= v1->vertex[1] - v3->vertex[1];
	b.vertex[2]= v1->vertex[2] - v3->vertex[2];

	result.vertex[0]= (a.vertex[1]*b.vertex[2]) - (b.vertex[1]*a.vertex[2]);
	result.vertex[1]= (b.vertex[0]*a.vertex[2]) - (a.vertex[0]*b.vertex[2]);
	result.vertex[2]= (a.vertex[0]*b.vertex[1]) - (b.vertex[0]*a.vertex[1]);

	// calculate the length of the normal
	length = (float)sqrt(SQUARE(result.vertex[0]) + 
						SQUARE(result.vertex[1]) + 
						SQUARE(result.vertex[2]));

	// normalize and specify the normal
	//glNormal3f(result.vertex[0]/length, result.vertex[1]/length, result.vertex[2]/length);
}


float vector3f::Length(void)
{
	/// calculate the length of the normal
	float length= (float)sqrt(SQUARE(vertex[0]) + 
					SQUARE(vertex[1]) + 
					SQUARE(vertex[2]));

	return length;
}


float vector3f::LengthSquared(void)
{
	/// calculate the length of the normal
	float length = (float)(SQUARE(vertex[0]) + 
					SQUARE(vertex[1]) + 
					SQUARE(vertex[2]));

	return length;
}


void vector3f::LoadZero(void)
{
	vertex[0] = 0;
	vertex[1] = 0;
	vertex[2] = 0;

}

float dot(const vector3f &v1,const vector3f &v2)
{
	float dot;

	dot= (v1.vertex[0]*v2.vertex[0])+
		 (v1.vertex[1]*v2.vertex[1])+
		 (v1.vertex[2]*v2.vertex[2]);

	return dot;
}


vector3f Cross(const vector3f &v1, const vector3f &v2)
{
	vector3f result(0.0f, 0.0f, 0.0f);

	result.vertex[0]= (v1.vertex[1]*v2.vertex[2]) - (v1.vertex[2]*v2.vertex[1]);
	result.vertex[1]= (v1.vertex[2]*v2.vertex[0]) - (v1.vertex[0]*v2.vertex[2]);
	result.vertex[2]= (v1.vertex[0]*v2.vertex[1]) - (v1.vertex[1]*v2.vertex[0]);

	return result;
}

vector3f vector3f::Abs(void)
{
	for (int i=0; i<3;i++)
	{
	if (this->vertex[i] < 0)
		this->vertex[i] = -this->vertex[i]; 
	}	

	return *this;
}

vector3f::vector3f(float x, float y, float z)
{
	vertex[0]=x;
	vertex[1]=y;
	vertex[2]=z;
}

vector3f::~vector3f()
{
}

void vector3f::Set(float x,float y, float z)
{
	vertex[0]=x;
	vertex[1]=y;
	vertex[2]=z;
}

/*
void vector3f::Eq(vector3f *other)
{
	vertex[0]=other->vertex[0];
	vertex[1]=other->vertex[1];
	vertex[2]=other->vertex[2];

}*/

/*vector3f operator= (const vector3f &v1, const vector3f &v2)
{

	v1.vertex[0] = v2.vertex[0];
	v1.vertex[1] = v2.vertex[1];
	v1.vertex[2] = v2.vertex[2];
}*/

vector3f& vector3f::operator=(const vector3f &v1)
{
	vertex[0] =v1.vertex[0];
	vertex[1] =v1.vertex[1];
	vertex[2] =v1.vertex[2];
	return *this;

}

bool operator== (const vector3f &v1, const vector3f &v2)
{
/*	return ((v1.vertex[0]/v2.vertex[0] > .95) && (v1.vertex[0]/v2.vertex[0] < 1.05) &&
			(v1.vertex[0]/v2.vertex[1] > .95) && (v1.vertex[0]/v2.vertex[1] < 1.05) &&
			(v1.vertex[0]/v2.vertex[2] > .95) && (v1.vertex[0]/v2.vertex[2] < 1.05)		);
*/
	return ((v1.vertex[0] == v2.vertex[0]) && 
		    (v1.vertex[1] == v2.vertex[1]) &&
		    (v1.vertex[2] == v2.vertex[2])); 
			
}

bool operator<= (const vector3f &v1, const vector3f &v2)
{
	return ((v1.vertex[0] <= v2.vertex[0]) && 
		    (v1.vertex[1] <= v2.vertex[1]) &&
		    (v1.vertex[2] <= v2.vertex[2])); 
}

bool operator>= (const vector3f &v1, const vector3f &v2)
{
	return ((v1.vertex[0] >= v2.vertex[0]) && 
		    (v1.vertex[1] >= v2.vertex[1]) &&
		    (v1.vertex[2] >= v2.vertex[2])); 
}

bool operator< (const vector3f &v1, const vector3f &v2)
{
	return ((v1.vertex[0] < v2.vertex[0]) && 
		    (v1.vertex[1] < v2.vertex[1]) &&
		    (v1.vertex[2] < v2.vertex[2])); 
}

bool operator> (const vector3f &v1, const vector3f &v2)
{
	return ((v1.vertex[0] > v2.vertex[0]) && 
		    (v1.vertex[1] > v2.vertex[1]) &&
		    (v1.vertex[2] > v2.vertex[2])); 
}


vector3f operator+ (const vector3f &v1, const vector3f &v2)
{
	vector3f result(0.0f, 0.0f, 0.0f);

	result.vertex[0] = v1.vertex[0] + v2.vertex[0];
	result.vertex[1] = v1.vertex[1] + v2.vertex[1];
	result.vertex[2] = v1.vertex[2] + v2.vertex[2];

	return result;
}


vector3f operator+ (const vector3f &v1, const float scalar)
{
	vector3f result(0.0f, 0.0f, 0.0f);

	result.vertex[0] = v1.vertex[0] + scalar;
	result.vertex[1] = v1.vertex[1] + scalar;
	result.vertex[2] = v1.vertex[2] + scalar;

	return result;
}
/*
vector3f operator+= (const vector3f &v1)
{
	//vector3f result(0.0f, 0.0f, 0.0f);

	vertex[0] += v1.vertex[0];
	vertex[1] += v1.vertex[1];
	vertex[2] += v1.vertex[2];

	return result;
}

*/
vector3f operator- (const vector3f &v1, const vector3f &v2)
{
	vector3f result(0.0f, 0.0f, 0.0f);

	result.vertex[0] = v1.vertex[0] - v2.vertex[0];
	result.vertex[1] = v1.vertex[1] - v2.vertex[1];
	result.vertex[2] = v1.vertex[2] - v2.vertex[2];

	return result;
}

vector3f operator- (const vector3f &v1, const float scalar)
{
	vector3f result(0.0f, 0.0f, 0.0f);

	result.vertex[0]= v1.vertex[0] - scalar;
	result.vertex[1]= v1.vertex[1] - scalar;
	result.vertex[2]= v1.vertex[2] - scalar;

	return result;
}


vector3f operator* (const vector3f &v1, const float scalar)
{
	vector3f result(0.0f, 0.0f, 0.0f);

	result.vertex[0]= v1.vertex[0] * scalar;
	result.vertex[1]= v1.vertex[1] * scalar;
	result.vertex[2]= v1.vertex[2] * scalar;

	return result;
}


vector3f operator* (const vector3f &v1, const vector3f &v2)
{
	vector3f result(0.0f, 0.0f, 0.0f);

	result.vertex[0]= v1.vertex[0] * v2.vertex[0];
	result.vertex[1]= v1.vertex[1] * v2.vertex[1];
	result.vertex[2]= v1.vertex[2] * v2.vertex[2];

	return result;
}


vector3f operator/ (const vector3f &v1, const vector3f &v2)
{
	vector3f result(0.0f, 0.0f, 0.0f);

	result.vertex[0]= v1.vertex[0] / v2.vertex[0];
	result.vertex[1]= v1.vertex[1] / v2.vertex[1];
	result.vertex[2]= v1.vertex[2] / v2.vertex[2];

	return result;
}

vector3f operator/ (const vector3f &v1, const float scalar)
{
	vector3f result(0.0f, 0.0f, 0.0f);

	result.vertex[0]= v1.vertex[0] / scalar;
	result.vertex[1]= v1.vertex[1] / scalar;
	result.vertex[2]= v1.vertex[2] / scalar;

	return result;
}



/////////////////////////////////////////////////////////////////////////////////////////////
/*

OpengGL compatible matrix:

float[16]
[0 4 8  12]
[1 5 9  13]
[2 6 10 14]
[3 7 11 15]

[rightX upX	outX	X]
[rightY upY outY	Y]
[rightZ	upZ	outZ	Z]
[0		0	0		1]

*/
/////////////////////////////////////////////////////////////////////////////////////////////



void matrix16f::LoadZero(void)
{
	int loop;

	for(loop=0; loop<16; loop++)
		matrix[loop]=0.0f;
}

void matrix16f::LoadIdentity(void)
{
        matrix[0] =1.0f; matrix[4] =0.0f; matrix[8] =0.0f; matrix[12] =0.0f;
        matrix[1] =0.0f; matrix[5] =1.0f; matrix[9] =0.0f; matrix[13] =0.0f;
        matrix[2] =0.0f; matrix[6] =0.0f; matrix[10]=1.0f; matrix[14] =0.0f;
        matrix[3] =0.0f; matrix[7] =0.0f; matrix[11]=0.0f; matrix[15] =1.0f;
}

void matrix16f::Translate(float x, float y, float z)
{
        this->matrix[12]=(this->matrix[0]*x)+(this->matrix[4]*y)+(this->matrix[8] *z)+(this->matrix[12]);
        this->matrix[13]=(this->matrix[1]*x)+(this->matrix[5]*y)+(this->matrix[9] *z)+(this->matrix[13]);
        this->matrix[14]=(this->matrix[2]*x)+(this->matrix[6]*y)+(this->matrix[10]*z)+(this->matrix[14]);
//      this->matrix[15]=(this->matrix[3]*x)+(this->matrix[7]*y)+(this->matrix[11]*z)+(this->matrix[15]);

}

void matrix16f::Scale(float x, float y, float z)
{
	matrix[0] *= x;
		matrix[5] *= y;
			matrix[10] *= z;
}
				 


// current uses only 3x3 orientation portion, not location triple
vector3f matrix16f::Transform(vector3f point)
{

	vector3f result;

	result.vertex[0] = (this->matrix[0]*point.vertex[0])+(this->matrix[4]*point.vertex[1])+(this->matrix[8] *point.vertex[2]) +(this->matrix[12]);
	result.vertex[1] = (this->matrix[1]*point.vertex[0])+(this->matrix[5]*point.vertex[1])+(this->matrix[9] *point.vertex[2]) +(this->matrix[13]);
	result.vertex[2] = (this->matrix[2]*point.vertex[0])+(this->matrix[6]*point.vertex[1])+(this->matrix[10]*point.vertex[2]) +(this->matrix[14]);

	return result;
}

matrix16f matrix16f::FlipOrientation(void)
{
	matrix16f result;

	result.matrix[0] = matrix[0]; result.matrix[4] = matrix[1]; result.matrix[8] = matrix[2]; result.matrix[12] = matrix[12];
	result.matrix[1] = matrix[4]; result.matrix[5] = matrix[5]; result.matrix[9] = matrix[6]; result.matrix[13] = matrix[13];
	result.matrix[2] = matrix[8]; result.matrix[6] = matrix[9]; result.matrix[10]= matrix[10];result.matrix[14] = matrix[14];
	result.matrix[3] = matrix[3]; result.matrix[7] = matrix[7]; result.matrix[11]= matrix[11];result.matrix[15] = matrix[15];

	return result;
}


void matrix16f::Normalize(void)
{
	vector3f right(	matrix[0],matrix[1],matrix[2]);
	vector3f up(	matrix[4],matrix[5],matrix[6]);
	vector3f out(	matrix[8],matrix[9],matrix[10]);

	// make all axes orthogonal
	out   = Cross(up,right);
	right = Cross(out,up);

	// normalize
	right = right/right.Length();
	up	  = up/up.Length();
	out   = out/out.Length();

	matrix[0] = right.vertex[0]; matrix[4] = up.vertex[0]; matrix[8] = out.vertex[0]; 
	matrix[1] = right.vertex[1]; matrix[5] = up.vertex[1]; matrix[9] = out.vertex[1]; 
	matrix[2] = right.vertex[2]; matrix[6] = up.vertex[2]; matrix[10]= out.vertex[2];
}

void matrix16f::print()
{
	/*std::cout    
		<< matrix[0] << "\t" << matrix[4] << "\t" << matrix[8] << "\t" << matrix[12] << "\n"
		<< matrix[1] << "\t" << matrix[5] << "\t" << matrix[9] << "\t" << matrix[13] << "\n" 
		<< matrix[2] << "\t" << matrix[6] << "\t" << matrix[10]<< "\t" << matrix[14] << "\n\n";*/
}

// 1	0		 0		0 
// 0	cos(x) -sin(x)	0  
// 0	sin(x)  cos(x)	0  
// 0	0		 0		1 
void matrix16f::RotateX(int deg)
{
	float tempX = this->matrix[12];
	float tempY = this->matrix[13];
	float tempZ = this->matrix[14];

	this->matrix[12] = 0;
	this->matrix[13] = 0;
	this->matrix[14] = 0;

	matrix16f temp;
	temp.LoadIdentity();
	temp.matrix[5] = (float)cos(DEG_TO_RAD(deg));  temp.matrix[9]  = (float) sin(DEG_TO_RAD(deg));
	temp.matrix[6] = (float)-sin(DEG_TO_RAD(deg)); temp.matrix[10] = (float) cos(DEG_TO_RAD(deg));

	*this = (*this)*temp;

	// restore x,y,z so rotation won't affect them - doesn't appear to work
	this->matrix[12] = tempX;
	this->matrix[13] = tempY;
	this->matrix[14] = tempZ;
} 



// rotate about y-axis about center of this, not origin
// cos(x)	0	sin(x)	0 
// 0		1	 0		0
//-sin(x)	0	cos(x)	0  
// 0		0	 0		1 
void matrix16f::RotateY(int deg)
{
	float tempX = this->matrix[12];
	float tempY = this->matrix[13];
	float tempZ = this->matrix[14];

	this->matrix[12] = 0;
	this->matrix[13] = 0;
	this->matrix[14] = 0;

	matrix16f temp;

	temp.LoadIdentity();
	temp.matrix[0] = (float) cos(DEG_TO_RAD(deg)); temp.matrix[8]  = (float) -sin(DEG_TO_RAD(deg));
	temp.matrix[2] = (float) sin(DEG_TO_RAD(deg)); temp.matrix[10] = (float) cos(DEG_TO_RAD(deg));

	*this = (*this)*temp;

	// restore x,y,z so rotation won't affect them
	this->matrix[12] = tempX;
	this->matrix[13] = tempY;
	this->matrix[14] = tempZ;
} 


// rotate about x-axis about center of this, not origin
// cos(x) -sin(x)	0	0 
// sin(x)  cos(x)	0	0  
// 0		0		1	0
// 0		0		0	1 
void matrix16f::RotateZ(int deg)
{
	float tempX = this->matrix[12];
	float tempY = this->matrix[13];
	float tempZ = this->matrix[14];

	this->matrix[12] = 0;
	this->matrix[13] = 0;
	this->matrix[14] = 0;

	matrix16f temp;
	temp.LoadIdentity();
	temp.matrix[0] = (float)cos(DEG_TO_RAD(deg)); temp.matrix[4] = (float)sin(DEG_TO_RAD(deg));
	temp.matrix[1] = (float)-sin(DEG_TO_RAD(deg)); temp.matrix[5] = (float) cos(DEG_TO_RAD(deg));

	*this = (*this)*temp;

	// restore x,y,z so rotation won't affect them
	this->matrix[12] = tempX;
	this->matrix[13] = tempY;
	this->matrix[14] = tempZ;
} 




// 1	0		 0		0 
// 0	cos(x) -sin(x)	0  
// 0	sin(x)  cos(x)	0  
// 0	0		 0		1 
void matrix16f::RotateX(float rad)
{
	float tempX = this->matrix[12];
	float tempY = this->matrix[13];
	float tempZ = this->matrix[14];

	this->matrix[12] = 0;
	this->matrix[13] = 0;
	this->matrix[14] = 0;

	matrix16f temp;
	temp.LoadIdentity();
	temp.matrix[5] = (float)cos(rad); temp.matrix[9]  = (float)sin(rad);
	temp.matrix[6] = (float)-sin(rad); temp.matrix[10] = (float) cos(rad);

	*this = (*this)*temp;

	// restore x,y,z so rotation won't affect them - doesn't appear to work
	this->matrix[12] = tempX;
	this->matrix[13] = tempY;
	this->matrix[14] = tempZ;
} 


// rotate about y-axis about center of this, not origin
// cos(x)	0	sin(x)	0 
// 0		1	 0		0
//-sin(x)	0	cos(x)	0  
// 0		0	 0		1 
void matrix16f::RotateY(float rad)
{
	float tempX = this->matrix[12];
	float tempY = this->matrix[13];
	float tempZ = this->matrix[14];

	this->matrix[12] = 0;
	this->matrix[13] = 0;
	this->matrix[14] = 0;

	matrix16f temp;

	temp.LoadIdentity();
	temp.matrix[0] = (float) cos(rad); temp.matrix[8]  = (float)-sin(rad);
	temp.matrix[2] = (float) sin(rad); temp.matrix[10] = (float) cos(rad);

	*this = (*this)*temp;

	// restore x,y,z so rotation won't affect them
	this->matrix[12] = tempX;
	this->matrix[13] = tempY;
	this->matrix[14] = tempZ;
} 


// rotate about x-axis about center of this, not origin
// cos(x) -sin(x)	0	0 
// sin(x)  cos(x)	0	0  
// 0		0		1	0
// 0		0		0	1 
void matrix16f::RotateZ(float rad)
{
	float tempX = this->matrix[12];
	float tempY = this->matrix[13];
	float tempZ = this->matrix[14];

	this->matrix[12] = 0;
	this->matrix[13] = 0;
	this->matrix[14] = 0;

	matrix16f temp;
	temp.LoadIdentity();
	temp.matrix[0] = (float)cos(rad);  temp.matrix[4] = (float)sin(rad);
	temp.matrix[1] = (float)-sin(rad); temp.matrix[5] = (float)cos(rad);

	*this = (*this)*temp;

	// restore x,y,z so rotation won't affect them
	this->matrix[12] = tempX;
	this->matrix[13] = tempY;
	this->matrix[14] = tempZ;
} 

void matrix16f::RotateXYZ(float x, float y, float z)
{
	matrix16f temp;
/*	
	temp.matrix[0] = (float)(cos(x)*cos(y)*cos(z)-sin(x)*sin(z));
	temp.matrix[1] = (float)(-cos(z)*sin(x)-cos(x)*cos(y)*sin(z));
	temp.matrix[2] = (float)(cos(x)*sin(y));
	
	temp.matrix[4] = (float)(cos(y)*cos(z)*sin(x)+cos(x)*sin(z));
	temp.matrix[5] =
	temp.matrix[6] =

	temp.matrix[8] =
	temp.matrix[9] =
	temp.matrix[10]=
	
	temp.matrix[15]=1;
*/
	*this = (*this)*temp;	
}

// Rotate in place- don't change location
void matrix16f::Rotate(matrix16f m2)
{
	matrix16f temp = *this;
	
	// 
	// row 0
       temp.matrix[0] = (this->matrix[0]*m2.matrix[0]) + (this->matrix[4]*m2.matrix[1]) +(this->matrix[8]*m2.matrix[2]);
        temp.matrix[4] = (this->matrix[0]*m2.matrix[4]) + (this->matrix[4]*m2.matrix[5]) +(this->matrix[8]*m2.matrix[6]);
        temp.matrix[8] = (this->matrix[0]*m2.matrix[8]) + (this->matrix[4]*m2.matrix[9]) +(this->matrix[8]*m2.matrix[10]);
        
	// row 1
        temp.matrix[1] = (this->matrix[1]*m2.matrix[0]) +(this->matrix[5]*m2.matrix[1]) +(this->matrix[9]*m2.matrix[2]);
        temp.matrix[5] = (this->matrix[1]*m2.matrix[4]) +(this->matrix[5]*m2.matrix[5]) +(this->matrix[9]*m2.matrix[6]);
        temp.matrix[9] = (this->matrix[1]*m2.matrix[8]) +(this->matrix[5]*m2.matrix[9]) +(this->matrix[9]*m2.matrix[10]);
        
	// row 2
        temp.matrix[2] = (this->matrix[2]*m2.matrix[0]) +(this->matrix[6]*m2.matrix[1]) +(this->matrix[10]*m2.matrix[2]);
        temp.matrix[6] = (this->matrix[2]*m2.matrix[4]) +(this->matrix[6]*m2.matrix[5]) +(this->matrix[10]*m2.matrix[6]);
        temp.matrix[10]= (this->matrix[2]*m2.matrix[8]) +(this->matrix[6]*m2.matrix[9]) +(this->matrix[10]*m2.matrix[10]);
	
	// row 3
        temp.matrix[3] = (this->matrix[3]*m2.matrix[0]) +(this->matrix[7]*m2.matrix[1]) +(this->matrix[11]*m2.matrix[2]);
        temp.matrix[7] = (this->matrix[3]*m2.matrix[4]) +(this->matrix[7]*m2.matrix[5]) +(this->matrix[11]*m2.matrix[6]);
        temp.matrix[11]= (this->matrix[3]*m2.matrix[8]) +(this->matrix[7]*m2.matrix[9]) +(this->matrix[11]*m2.matrix[10]);
	
	*this  = temp;
}


void matrix16f::Set(vector3f right, vector3f up, vector3f out)
{
	
	// row 0
	this->matrix[0] = right.vertex[0];
	this->matrix[1] = right.vertex[1];
	this->matrix[2] = right.vertex[2];
	
	// row 1
	this->matrix[4] = up.vertex[0];
	this->matrix[5] = up.vertex[1];
	this->matrix[6] = up.vertex[2];
	
	// row 2
	this->matrix[8] = out.vertex[0];
	this->matrix[9] = out.vertex[1];
	this->matrix[10]= out.vertex[2];
	
}

//matrix16f operator= (const float m[16]);
//void matrix16f::Eq(float m[16])
matrix16f& matrix16f::operator= (const matrix16f &m1)
{
	for (int i = 0; i<16; i++)	
			matrix[i]= m1.matrix[i];

	return *this;
}


matrix16f operator+ (const matrix16f &m1, const matrix16f &m2)
{
	matrix16f result;

	for (int i = 0; i<16; i++)
		result.matrix[i]= m1.matrix[i]+m2.matrix[i];

	return result;
}


// for adding location vectors to orientation-location matrices
matrix16f operator+ (const matrix16f &m1, const vector3f  &v2)
{
	matrix16f result;

	result = m1;

	result.matrix[12] += v2.vertex[0];
	result.matrix[13] += v2.vertex[1];
	result.matrix[14] += v2.vertex[2];

	return result;
}

matrix16f operator- (const matrix16f &m1, const matrix16f &m2)
{
	matrix16f result;

	for (int i = 0; i<16; i++)
		result.matrix[i]= m1.matrix[i]-m2.matrix[i];

	return result;
}


matrix16f operator* (const matrix16f &m1, const float scalar)
{
	matrix16f result;

	for (int i = 0; i<16; i++)
		result.matrix[i]= m1.matrix[i]*scalar;


	return result;
}


matrix16f operator* (const matrix16f &m1, const matrix16f &m2)
{
	matrix16f result;

	// row 0
	result.matrix[0] = (m1.matrix[0]*m2.matrix[0]) + (m1.matrix[4]*m2.matrix[1]) +(m1.matrix[8]*m2.matrix[2]) +(m1.matrix[12]*m2.matrix[3]);
	result.matrix[4] = (m1.matrix[0]*m2.matrix[4]) + (m1.matrix[4]*m2.matrix[5]) +(m1.matrix[8]*m2.matrix[6]) +(m1.matrix[12]*m2.matrix[7]);
	result.matrix[8] = (m1.matrix[0]*m2.matrix[8]) + (m1.matrix[4]*m2.matrix[9]) +(m1.matrix[8]*m2.matrix[10])+(m1.matrix[12]*m2.matrix[11]);
	result.matrix[12]= (m1.matrix[0]*m2.matrix[12])+ (m1.matrix[4]*m2.matrix[13])+(m1.matrix[8]*m2.matrix[14])+(m1.matrix[12]*m2.matrix[15]);
	
	// row 1
	result.matrix[1] = (m1.matrix[1]*m2.matrix[0]) +(m1.matrix[5]*m2.matrix[1]) +(m1.matrix[9]*m2.matrix[2]) +(m1.matrix[13]*m2.matrix[3]);
	result.matrix[5] = (m1.matrix[1]*m2.matrix[4]) +(m1.matrix[5]*m2.matrix[5]) +(m1.matrix[9]*m2.matrix[6]) +(m1.matrix[13]*m2.matrix[7]);
	result.matrix[9] = (m1.matrix[1]*m2.matrix[8]) +(m1.matrix[5]*m2.matrix[9]) +(m1.matrix[9]*m2.matrix[10])+(m1.matrix[13]*m2.matrix[11]);
	result.matrix[13]= (m1.matrix[1]*m2.matrix[12])+(m1.matrix[5]*m2.matrix[13])+(m1.matrix[9]*m2.matrix[14])+(m1.matrix[13]*m2.matrix[15]);

	// row 2
	result.matrix[2] = (m1.matrix[2]*m2.matrix[0]) +(m1.matrix[6]*m2.matrix[1]) +(m1.matrix[10]*m2.matrix[2]) +(m1.matrix[14]*m2.matrix[3]);
	result.matrix[6] = (m1.matrix[2]*m2.matrix[4]) +(m1.matrix[6]*m2.matrix[5]) +(m1.matrix[10]*m2.matrix[6]) +(m1.matrix[14]*m2.matrix[7]);
	result.matrix[10]= (m1.matrix[2]*m2.matrix[8]) +(m1.matrix[6]*m2.matrix[9]) +(m1.matrix[10]*m2.matrix[10])+(m1.matrix[14]*m2.matrix[11]);
	result.matrix[14]= (m1.matrix[2]*m2.matrix[12])+(m1.matrix[6]*m2.matrix[13])+(m1.matrix[10]*m2.matrix[14])+(m1.matrix[14]*m2.matrix[15]);

	// row 3
	result.matrix[3] = (m1.matrix[3]*m2.matrix[0]) +(m1.matrix[7]*m2.matrix[1]) +(m1.matrix[11]*m2.matrix[2]) +(m1.matrix[15]*m2.matrix[3]);
	result.matrix[7] = (m1.matrix[3]*m2.matrix[4]) +(m1.matrix[7]*m2.matrix[5]) +(m1.matrix[11]*m2.matrix[6]) +(m1.matrix[15]*m2.matrix[7]);
	result.matrix[11]= (m1.matrix[3]*m2.matrix[8]) +(m1.matrix[7]*m2.matrix[9]) +(m1.matrix[11]*m2.matrix[10])+(m1.matrix[15]*m2.matrix[11]);
	result.matrix[15]= (m1.matrix[3]*m2.matrix[12])+(m1.matrix[7]*m2.matrix[13])+(m1.matrix[11]*m2.matrix[14])+(m1.matrix[15]*m2.matrix[15]);

	return result;
}

matrix16f operator/ (const matrix16f &m1, const float scalar)
{	
	matrix16f result;

	for (int i = 0; i<16; i++)
		result.matrix[i]= m1.matrix[i]/scalar;


	return result;
}


bool operator== (const matrix16f &m1, const matrix16f &m2)
{
	bool result = true;
	for (int i = 0; i< 16; i++)
		if (m1.matrix[i] != m2.matrix[i]) result = false;

	return result;
}

// | f0 f2 |
// | f1 f3 |
float det(float f0, float f1, float f2, float f3)
{
	return (f0*f3-f1*f2);
}


// fill matrix with zeros
void matrix9f::LoadZero(void)
{
	int loop;

	for(loop=0; loop<9; loop++)
		matrix[loop]=0.0f;
}
 


matrix9f matrix9f::Inverse(void)
{
	matrix9f result;

	float detThis =	this->matrix[0]*det(this->matrix[4],this->matrix[5],this->matrix[7],this->matrix[8]) -
					this->matrix[3]*det(this->matrix[1],this->matrix[2],this->matrix[7],this->matrix[8]) +
					this->matrix[6]*det(this->matrix[1],this->matrix[2],this->matrix[4],this->matrix[5]);
		    
	result.matrix[0] =  det(this->matrix[4],this->matrix[5],this->matrix[7],this->matrix[8]); 
	result.matrix[3] = -det(this->matrix[1],this->matrix[2],this->matrix[7],this->matrix[8]); 
	result.matrix[6] =  det(this->matrix[1],this->matrix[2],this->matrix[4],this->matrix[5]); 

	result.matrix[1] = -det(this->matrix[3],this->matrix[5],this->matrix[6],this->matrix[8]); 
	result.matrix[4] =  det(this->matrix[0],this->matrix[2],this->matrix[6],this->matrix[8]); 
	result.matrix[7] = -det(this->matrix[1],this->matrix[2],this->matrix[4],this->matrix[5]);
			
	result.matrix[2] =  det(this->matrix[3],this->matrix[4],this->matrix[6],this->matrix[7]); 
	result.matrix[5] = -det(this->matrix[0],this->matrix[1],this->matrix[6],this->matrix[7]);
	result.matrix[8] =  det(this->matrix[0],this->matrix[1],this->matrix[3],this->matrix[4]);

	result = result.Transpose();

	result = result/detThis;

	// check
	//matrix9f check = (*this) * result;


	return result;
}


void matrix9f::LoadIdentity(void)
{
	matrix[0] = 1.0f; matrix[3] = 0.0f; matrix[6] = 0.0f; 
	matrix[1] = 0.0f; matrix[4] = 1.0f; matrix[7] = 0.0f; 
	matrix[2] = 0.0f; matrix[5] = 0.0f; matrix[8] = 1.0f; 

}

matrix9f matrix9f::Transpose(void)
{
	matrix9f result;

	result.matrix[0] = matrix[0]; result.matrix[3] = matrix[1]; result.matrix[6] = matrix[2]; 
	result.matrix[1] = matrix[3]; result.matrix[4] = matrix[4]; result.matrix[7] = matrix[5]; 
	result.matrix[2] = matrix[6]; result.matrix[5] = matrix[7]; result.matrix[8] = matrix[8];

	return result;
}


matrix9f Star(vector3f a)
{
	matrix9f result;

	result.matrix[0] =  0;			 result.matrix[3] = -a.vertex[2]; result.matrix[6] =  a.vertex[1]; 
	result.matrix[1] =  a.vertex[2]; result.matrix[4] =  0;			  result.matrix[7] = -a.vertex[0]; 
	result.matrix[2] = -a.vertex[1]; result.matrix[5] =  a.vertex[0]; result.matrix[8] =  0;

	return result;
}

// rotate about x-axis about center of this, not origin
// 1	0		 0		0 
// 0	cos(x) -sin(x)	0  
// 0	sin(x)  cos(x)	0  
// 0	0		 0		1 
void matrix9f::RotateX(int deg)
{
	matrix9f temp;
	temp.LoadIdentity();
	temp.matrix[4] = (float)cos(DEG_TO_RAD(deg));  temp.matrix[7]  = (float) sin(DEG_TO_RAD(deg));
	temp.matrix[5] = (float)-sin(DEG_TO_RAD(deg)); temp.matrix[8] = (float) cos(DEG_TO_RAD(deg));

	*this = (*this)*temp;
} 


// rotate about y-axis about center of this, not origin
// cos(x)	0	sin(x)	0 
// 0		1	 0		0
//-sin(x)	0	cos(x)	0  
// 0		0	 0		1 
void matrix9f::RotateY(int deg)
{
	matrix9f temp;

	temp.LoadIdentity();
	temp.matrix[0] = (float) cos(DEG_TO_RAD(deg)); temp.matrix[6]  = (float) -sin(DEG_TO_RAD(deg));
	temp.matrix[2] = (float) sin(DEG_TO_RAD(deg)); temp.matrix[8] = (float) cos(DEG_TO_RAD(deg));

	*this = (*this)*temp;
} 


// rotate about x-axis about center of this, not origin
// cos(x) -sin(x)	0	0 
// sin(x)  cos(x)	0	0  
// 0		0		1	0
// 0		0		0	1 
void matrix9f::RotateZ(int deg)
{
	matrix9f temp;
	temp.LoadIdentity();
	temp.matrix[0] = (float)cos(DEG_TO_RAD(deg));  temp.matrix[3] = (float)sin(DEG_TO_RAD(deg));
	temp.matrix[1] = (float)-sin(DEG_TO_RAD(deg)); temp.matrix[4] = (float) cos(DEG_TO_RAD(deg));

	*this = (*this)*temp;
} 


//matrix9f operator= (const float m[9]);
//void matrix9f::Eq(float m[9])
matrix9f& matrix9f::operator= (const matrix9f &m1)
{
	for (int i = 0; i<9; i++)	
			matrix[i]= m1.matrix[i];

	return *this;
}


matrix9f operator+ (const matrix9f &m1, const matrix9f &m2)
{
	matrix9f result;

	for (int i = 0; i<9; i++)
		result.matrix[i]= m1.matrix[i]+m2.matrix[i];

	return result;
}

matrix16f operator+ (const matrix16f &m1, const matrix9f &m2)
{
	matrix16f result;

	result = m1;

	// row 0
	result.matrix[0] += m2.matrix[0];
	result.matrix[4] += m2.matrix[3];
	result.matrix[8] += m2.matrix[6];
	
	// row 1
	result.matrix[1] += m2.matrix[1];
	result.matrix[5] += m2.matrix[4];
	result.matrix[9] += m2.matrix[7];

	// row 2
	result.matrix[2] += m2.matrix[2];
	result.matrix[6] += m2.matrix[5];
	result.matrix[10] += m2.matrix[8];

	return result;
}

matrix9f operator- (const matrix9f &m1, const matrix9f &m2)
{
	matrix9f result;

	for (int i = 0; i<9; i++)
		result.matrix[i]= m1.matrix[i]-m2.matrix[i];

	return result;
}


matrix9f operator* (const matrix9f &m1, const float scalar)
{
	matrix9f result;

	for (int i = 0; i<9; i++)
		result.matrix[i]= m1.matrix[i]*scalar;


	return result;
}


vector3f operator* (const matrix9f &m1, const vector3f &v2)
{
	vector3f result;

	result.vertex[0] = (m1.matrix[0]*v2.vertex[0]) + (m1.matrix[3]*v2.vertex[1]) + (m1.matrix[6]*v2.vertex[2]);
	result.vertex[1] = (m1.matrix[1]*v2.vertex[0]) + (m1.matrix[4]*v2.vertex[1]) + (m1.matrix[7]*v2.vertex[2]);
	result.vertex[2] = (m1.matrix[2]*v2.vertex[0]) + (m1.matrix[5]*v2.vertex[1]) + (m1.matrix[8]*v2.vertex[2]);

	return result;
}


matrix9f operator* (const matrix9f &m1, const matrix9f &m2)
{
	matrix9f result;

	// row 0
	result.matrix[0] = (m1.matrix[0]*m2.matrix[0]) + (m1.matrix[3]*m2.matrix[1]) + (m1.matrix[6]*m2.matrix[2]);
	result.matrix[3] = (m1.matrix[0]*m2.matrix[3]) + (m1.matrix[3]*m2.matrix[4]) + (m1.matrix[6]*m2.matrix[5]);
	result.matrix[6] = (m1.matrix[0]*m2.matrix[6]) + (m1.matrix[3]*m2.matrix[7]) + (m1.matrix[6]*m2.matrix[8]);
	
	// row 1
	result.matrix[1] = (m1.matrix[1]*m2.matrix[0]) + (m1.matrix[4]*m2.matrix[1]) + (m1.matrix[7]*m2.matrix[2]);
	result.matrix[4] = (m1.matrix[1]*m2.matrix[3]) + (m1.matrix[4]*m2.matrix[4]) + (m1.matrix[7]*m2.matrix[5]);
	result.matrix[7] = (m1.matrix[1]*m2.matrix[6]) + (m1.matrix[4]*m2.matrix[7]) + (m1.matrix[7]*m2.matrix[8]);

	// row 2
	result.matrix[2] = (m1.matrix[2]*m2.matrix[0]) + (m1.matrix[5]*m2.matrix[1]) + (m1.matrix[8]*m2.matrix[2]);
	result.matrix[5] = (m1.matrix[2]*m2.matrix[3]) + (m1.matrix[5]*m2.matrix[4]) + (m1.matrix[8]*m2.matrix[5]);
	result.matrix[8] = (m1.matrix[2]*m2.matrix[6]) + (m1.matrix[5]*m2.matrix[7]) + (m1.matrix[8]*m2.matrix[8]);


	return result;
}

matrix9f operator* (const matrix9f &m1, const matrix16f &m2)
{
		matrix9f result;

	// row 0
	result.matrix[0] = (m1.matrix[0]*m2.matrix[0]) + (m1.matrix[3]*m2.matrix[1]) + (m1.matrix[6]*m2.matrix[2]);
	result.matrix[3] = (m1.matrix[0]*m2.matrix[4]) + (m1.matrix[3]*m2.matrix[5]) + (m1.matrix[6]*m2.matrix[6]);
	result.matrix[6] = (m1.matrix[0]*m2.matrix[8]) + (m1.matrix[3]*m2.matrix[9]) + (m1.matrix[6]*m2.matrix[10]);
	
	// row 1
	result.matrix[1] = (m1.matrix[1]*m2.matrix[0]) + (m1.matrix[4]*m2.matrix[1]) + (m1.matrix[7]*m2.matrix[2]);
	result.matrix[4] = (m1.matrix[1]*m2.matrix[4]) + (m1.matrix[4]*m2.matrix[5]) + (m1.matrix[7]*m2.matrix[6]);
	result.matrix[7] = (m1.matrix[1]*m2.matrix[8]) + (m1.matrix[4]*m2.matrix[9]) + (m1.matrix[7]*m2.matrix[10]);

	// row 2
	result.matrix[2] = (m1.matrix[2]*m2.matrix[0]) + (m1.matrix[5]*m2.matrix[1]) + (m1.matrix[8]*m2.matrix[2]);
	result.matrix[5] = (m1.matrix[2]*m2.matrix[4]) + (m1.matrix[5]*m2.matrix[5]) + (m1.matrix[8]*m2.matrix[6]);
	result.matrix[8] = (m1.matrix[2]*m2.matrix[8]) + (m1.matrix[5]*m2.matrix[9]) + (m1.matrix[8]*m2.matrix[10]);


	return result;

}

matrix9f operator* (const matrix16f &m1, const matrix9f &m2)
{
		matrix9f result;

	// row 0
	result.matrix[0] = (m1.matrix[0]*m2.matrix[0]) + (m1.matrix[4]*m2.matrix[1]) + (m1.matrix[8]*m2.matrix[2]);
	result.matrix[3] = (m1.matrix[0]*m2.matrix[3]) + (m1.matrix[4]*m2.matrix[4]) + (m1.matrix[8]*m2.matrix[5]);
	result.matrix[6] = (m1.matrix[0]*m2.matrix[6]) + (m1.matrix[4]*m2.matrix[7]) + (m1.matrix[8]*m2.matrix[8]);
	
	// row 1
	result.matrix[1] = (m1.matrix[1]*m2.matrix[0]) + (m1.matrix[5]*m2.matrix[1]) + (m1.matrix[9]*m2.matrix[2]);
	result.matrix[4] = (m1.matrix[1]*m2.matrix[3]) + (m1.matrix[5]*m2.matrix[4]) + (m1.matrix[9]*m2.matrix[5]);
	result.matrix[7] = (m1.matrix[1]*m2.matrix[6]) + (m1.matrix[5]*m2.matrix[7]) + (m1.matrix[9]*m2.matrix[8]);

	// row 2
	result.matrix[2] = (m1.matrix[2]*m2.matrix[0]) + (m1.matrix[6]*m2.matrix[1]) + (m1.matrix[10]*m2.matrix[2]);
	result.matrix[5] = (m1.matrix[2]*m2.matrix[3]) + (m1.matrix[6]*m2.matrix[4]) + (m1.matrix[10]*m2.matrix[5]);
	result.matrix[8] = (m1.matrix[2]*m2.matrix[6]) + (m1.matrix[6]*m2.matrix[7]) + (m1.matrix[10]*m2.matrix[8]);


	return result;
	
}

matrix9f operator/ (const matrix9f &m1, const float scalar)
{	
	matrix9f result;

	for (int i = 0; i<9; i++)
		result.matrix[i]= m1.matrix[i]/scalar;


	return result;
}


bool operator== (const matrix9f &m1, const matrix9f &m2)
{
	bool result = true;
	for (int i = 0; i< 9; i++)
		if (m1.matrix[i] != m2.matrix[i]) result = false;

	return result;

}

//////////////////////////////////////////////////////////////////



// This makes a normal based on the surface normals of all triangles adjacent
// to the point, regardless ('irregardless' as Homer would say) of the size of the
// angle that triangle makes with the point
vector3f findNorm(vector3f &matchVertex, int numSurfTriangles, triangleV **surfTriangles)
{
	vector3f norm;
	//int numAdjacent;

	for (int i = 0; i < numSurfTriangles; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (surfTriangles[i]->vertice[j] == &matchVertex)
			{	
				//numAdjacent++;
				norm = norm + surfTriangles[i]->norm;
			}
		}
	}
	norm = norm/norm.Length();
	

	return norm;

}
