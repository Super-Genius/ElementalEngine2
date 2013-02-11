#include "stdafx.h"
#include "ObjMaterials.h"

triangleInd::triangleInd() 
{
	
}

triangleInd::~triangleInd() 
{
	
}

triangleInd& triangleInd::operator= (const triangleInd &tri)
{
	for (int i =0; i < 3; i++) { vertIndices[i] = tri.vertIndices[i]; }   
	
	normal = tri.normal;
	collision = tri.collision;  

	return *this;	
}

material::material()
{
}

material::~material()
{

}

material& material::operator= (const material &mat)
{
	name = mat.name;
	faces = mat.faces;
	
	Se = mat.Se;
	
	for (int i=0; i <4; i++)
	{	
		Ka[i] = mat.Ka[i];
		Ks[i] = mat.Ks[i];
		Kd[i] = mat.Kd[i];
		
		Ke[i] = mat.Ke[i];	
	}
	
	opacity = mat.opacity;

	return *this;
}
