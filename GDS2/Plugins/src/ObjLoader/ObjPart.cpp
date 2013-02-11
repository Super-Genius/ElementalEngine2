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

******************************************************************************/

#include "stdafx.h"
#include"ObjUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include <string>
#include <vector>

#include "ObjMVMath.h"
#include "ObjPart.h"
#include "ObjMaterials.h"

//#include <SDL/SDL.h>

#include <cmath>

#ifdef WIN32
#define M_PI PI
#endif 

using namespace std;

CWavefrontObj::CWavefrontObj()
{	
/*
	counter = 0;

	numVertices = 0;
	numMtl		= 0;

	
	fprintf(stderr,"spaceship.obj init...");

	load("spaceship.obj");

	fprintf(stderr,"finished\n");*/
}


CWavefrontObj::CWavefrontObj(string objFile)
{	
	counter = 0;

	verbose = false;
	
	translucent = false;
	
	//drawBB = true;
	
	
	/// Strip directory info out of objFile
	string::size_type pos = objFile.find_last_of("/");
	this->subdir = objFile.substr(0,pos+1);
	if(this->subdir == "")
	{
		//try slash
		pos = objFile.find_last_of("\\");
		this->subdir = objFile.substr(0,pos+1);	
	}
	
	objFile = objFile.substr(pos+1,objFile.size());

	try { load(objFile.c_str()); }
	catch (fileNotFound) { throw fileNotFound(); return;}
	
	//mass = 1;


	//makeList();
/*
	// GLG toggles //////////////////////
	surfBox = new CheckBox(0,"Surface");
	surfBoxListener = new CheckBoxListener(&drawSurface);
	surfBox->addMessageListener(surfBoxListener);

	movablePanel->addChildWindow(surfBox);
	
	normBox = new CheckBox(0,"Normalize");
	normBoxListener = new CheckBoxListener(&normalize);
	normBox->addMessageListener(normBoxListener);

	movablePanel->addChildWindow(normBox);

	movablePanel->arrangeChildren();
*/
}

CWavefrontObj::~CWavefrontObj()
{

}

CWavefrontObj& CWavefrontObj::operator= (const CWavefrontObj &obj1)
{

	return *this;
}


bool CWavefrontObj::load(LPCSTR objFile)
{
	/*
	// find the last forward slash in the string
	string::size_type lastSlashPosition;
	while (lastSlashPosition = objFile.find("/")) {}
	subdir = objFile.substr(0,lastSlashPosition);
	cout << "subdir = " << subdir << "\n";
	*/
	string objFileStr = "";
	objFileStr += subdir;
	objFileStr += objFile;
	
	
	HANDLE hFile = OpenFile((TCHAR*)objFileStr.c_str());
	if(hFile == INVALID_HANDLE_VALUE)
	{
		throw fileNotFound();
		return false;
	}
	// now write the actual line
	vector<string> lines;
	while(hFile != INVALID_HANDLE_VALUE)
	{
		string newstr="";
		char cread = '?';
		while(cread != '\0')
		{
			DWORD numbytesread;
			ReadFile(hFile, &cread, sizeof(cread), &numbytesread, NULL);
			if(numbytesread == 0)
			{
				DWORD dwError = GetLastError();
				//oddly, when numbytesread is 0, and it's supposed to be eof,
				//last error returns success
				if(dwError == ERROR_HANDLE_EOF || dwError == 0)
				{
					CloseHandle( hFile );
					hFile = INVALID_HANDLE_VALUE;
					cread = '\0';
				}else if(dwError == ERROR_IO_PENDING)
				{
					continue;
				}
			}else
			{	
				if(cread != '\n' && cread != '\r' && cread != '\t' && cread != ' ' && cread != '\0')
				{
                    newstr += cread;
				}else if(newstr != "")
				{
					//add line to vector
					lines.push_back(newstr);				
					newstr.clear();
				}
			}
		}
		
	}

	theMode = NONE;
	
	//	for_each(lines.begin(),lines.end(),&CBvhObject::process);
	
	unsigned i;
	for (i =0; i< lines.size(); i++) {
		try { process(lines[i]); }
		catch (fileNotFound) { throw fileNotFound(); return false; }
	}
	
	/// find face normals for collision detection and other uses
	vector3f normal;
	for (i = 0; i < mtls.size(); i++)
	{				
		for (unsigned j = 0; j < mtls[i]->faces.size(); j++)
		{
			int ind0 = mtls[i]->faces[j].vertIndices[0]-1;
			int ind1 = mtls[i]->faces[j].vertIndices[1]-1;
			int ind2 = mtls[i]->faces[j].vertIndices[2]-1;
				
			normal = Cross( vertices[ind1] - vertices[ind0],
				 		vertices[ind2] - vertices[ind0] );
			
			normal = normal/(normal.Length());

			mtls[i]->faces[j].normal = normal;

		}
	}
	
	return true;
}
	
void CWavefrontObj::process(string line)
{	
	int index; 
	string::size_type slashes;
	
	//cout << "Mode = " << theMode << "\n";
	if ((line.size()) && (line.substr(0,1) != "#"))
	{
		if (line == "mtllib") {
			theMode = MTLLIB;
		} else if (line == "v") {
			theMode = VERTEX;
			tempVectorIndex = 0;
		} else if (line == "vn") {
			theMode = NORMAL; 
			tempVectorIndex = 0;
		} else if (line == "f") {
			theMode = FACE;
			tempVectorIndex = 0;
		} else switch (theMode) {
		
			case (MTLLIB):
				mtlFile = line;
				
				if (!matchMtl(mtlIndex, mtlFile)) {
					try { loadMtl(mtlFile); }
					catch (fileNotFound) { throw fileNotFound(); return; }
						
					matchMtl(mtlIndex, mtlFile);
				}
				theMode = NONE;
			break;
			
			case (VERTEX):
				
				tempVector.vertex[tempVectorIndex] = float(atof(line.c_str()));
				//cout << tempVector.vertex[tempVectorIndex] << "\n";
				tempVectorIndex++;
				if (tempVectorIndex >=3) {
					theMode = NONE;
					vertices.push_back(tempVector);	
				}
				
			break;
			
			case (NORMAL):
				tempVector.vertex[tempVectorIndex] = float(atof(line.c_str()));
				tempVectorIndex++;
				if (tempVectorIndex >=3) {
					theMode = NONE;
					normals.push_back(tempVector);
				}
			
			break;
			
			case (FACE):
				/// face indices are denoted index//index (redundantly) in the .obj format
				slashes = line.find("//");
				index = atoi(line.substr(0,slashes).c_str());
				//cout << slashes << " " << index << "\n";
				tempTriangle.vertIndices[tempVectorIndex] = index;
				tempVectorIndex++;
				if (tempVectorIndex >= 3) {
					theMode = NONE;
					mtls[mtlIndex]->faces.push_back(tempTriangle);	
				}
				
			break;
			
			case (NONE):
				break;
		}
		
	}
	
		
}

void CWavefrontObj::processMtl(string line, material *mtl)
{	
	if ((line.size()) && (line.substr(0,1) != "#"))
	{
		if (line == "newmtl") {
			theMtlMode = NEWMTL;
		} else if (line == "Ns") {
			theMtlMode = NS;
		} else	if (line == "d") {
			theMtlMode = D;
		} else	if (line == "illum") {
			theMtlMode = ILLUM;
		} else	if (line == "Kd") {
			theMtlMode = KD;
			kIndex = 0;
		} else	if (line == "Ka") {
			theMtlMode = KA;
			kIndex = 0;
		} else	if (line == "Ks") {
			theMtlMode = KS;
			kIndex = 0;
		} else switch (theMtlMode) {
		
			case KD:
				mtl->Kd[kIndex] = float(atof(line.c_str()));
				kIndex++;
				if (kIndex >= 3) {
					mtl->Kd[3] = 1.0f;
					/*cout << mtl->name << " " << mtl->Kd[0] << " " << 
						mtl->Kd[1] << " " << mtl->Kd[2] << " " <<
						mtl->Kd[3] << "\n";*/
					theMtlMode = NONEM;
				}
				break;
				
			case KA:
				mtl->Ka[kIndex] = float(atof(line.c_str()));
				kIndex++;
				if (kIndex >= 3) {
					mtl->Ka[3] = 1.0f;
					theMtlMode = NONEM;
				}
				break;
				
			case KS:
				mtl->Ks[kIndex] = float(atof(line.c_str()));
				kIndex++;
				if (kIndex >= 3) {
					mtl->Ks[3] = 1.0f;
					theMtlMode = NONEM;
				}
				break;
				
			case (NONEM):
			case (NEWMTL):
			case (NS):
			case (D):
			case (ILLUM):
				break;
				
		}
	}
}

void CWavefrontObj::loadMtl(string mtlFile)
{	
	
	HANDLE hFile = OpenFile((TCHAR*)(subdir + mtlFile).c_str());
	if(hFile == INVALID_HANDLE_VALUE)
	{
		throw fileNotFound();
		return;
	}
	// now write the actual line
	vector<string> lines;
	while(hFile != INVALID_HANDLE_VALUE)
	{
		string newstr="";
		char cread = '?';
		while(cread != '\0')
		{
			DWORD numbytesread;
			ReadFile(hFile, &cread, sizeof(cread), &numbytesread, NULL);
			if(numbytesread == 0)
			{
				DWORD dwError = GetLastError();
				//oddly, when numbytesread is 0, and it's supposed to be eof,
				//last error returns success
				if(dwError == ERROR_HANDLE_EOF || dwError == 0)
				{
					CloseHandle( hFile );
					hFile = INVALID_HANDLE_VALUE;
					cread = '\0';
				}else if(dwError == ERROR_IO_PENDING)
				{
					continue;
				}
			}else
			{	
				if(cread != '\n' && cread != '\r' && cread != '\t' && cread != ' ' && cread != '\0')
				{
                    newstr += cread;
				}else if(newstr != "")
				{
					//add line to vector
					lines.push_back(newstr);				
					newstr.clear();
				}
			}
		}
		
	}
	theMtlMode = NONEM;
	
	material* newMtl = new material;
	newMtl->name = mtlFile;
	
	newMtl->Ke[0] = 0;
	newMtl->Ke[1] = 0;
	newMtl->Ke[2] = 0;
	newMtl->Ke[3] = 1.0f;
	
	unsigned i;
	for (i = 0; i< lines.size(); i++) {
		processMtl(lines[i],newMtl);
	}
	
	mtls.push_back(newMtl);
}

bool CWavefrontObj::matchMtl(unsigned &index, string name)
{
	unsigned m;
	
	for (m = 0; m < (mtls.size()); m++) {
		
		if (name == mtls[m]->name) {
			index = m;
			return true;
		}
	}
	//cout << (int)mtls.size() <<" mtlssize \n";
	
	return false;
}


void CWavefrontObj::setMass(float newMass)
{
	/*if (step != 0)
	{
		Ibody = (Ibody*newMass)/mass;
		IbodyInv = Ibody.Inverse();
	}

	mass = newMass;*/

}

/*void CWavefrontObj::InitTexture()
{
//	image = auxDIBImageLoad("spaceshipTex2.bmp");
//LoadBMP("spaceshipTex2.bmp");

//	GLfloat env_color[4];
//	GLfloat border_color[4];
    
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, image->sizeX, image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, env_color);

//	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->sizeX, image->sizeX, GL_RGB, GL_UNSIGNED_BYTE, image->data);

}
*/

void CWavefrontObj::update(float seconds)
{
	//getAABB();

	/*
	if (counter <90)
		location.RotateX(1);
	else if (counter <180)
		location.RotateY(1);
	else if (counter <300)
		location.Translate(0,0,0.2f);
		//location.RotateY(1);
		//location.RotateZ(1);
	else if (counter < 290)
		location.RotateX(1);
	else if (counter < 400)
		location.Translate(0,0,0.2f);
	else
		counter = -1;

	counter++;*/


}

void CWavefrontObj::draw(void)
{
	return;
}