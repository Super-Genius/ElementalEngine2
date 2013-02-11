#ifndef OBJMATERIAL_H
#define OBJMATERIAL_H

#include <string>
#include <vector>

using namespace std;

class triangleInd {
public:
	triangleInd();
	~triangleInd();

	int vertIndices[3];   
	vector3f normal;
	bool collision;  // temp visual debugging member
	
	triangleInd& operator= (const triangleInd &tri);
};

/// Holds all vertices and material info for CWavefrontObj entities
class material{

public:

	material();
	~material();

	material& operator= (const material &mat);

	/// material name defined in the parsed .obj files
	string name;
	//char name[128];

	//int numFaces;
	//int numUVs;

	
	vector<triangleInd> faces;
	//uv		 **uvs;

	// reflectances
	/// shininess exponent 0-128
	float Se;		
	/// Ambient
	float Ka[4];
	/// Diffuse	
	float Kd[4];	
	/// Specular
	float Ks[4];  
	/// Emission
	float Ke[4];  

	
	float opacity;
};

#endif 
