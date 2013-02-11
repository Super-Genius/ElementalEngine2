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
#include"BvhUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include <string>
#include <vector>

#include "BvhMVMath.h"
#include "BvhPart.h"

using namespace std;

BvhNode::BvhNode()
{
	//objPart = NULL;
}

CBvhObject::CBvhObject(string bvhFile)
:
CBaseObject(&GUID_BVH, (LPCSTR)bvhFile.c_str())
{
	verbose = true;
	try 
	{ 
		init(bvhFile); 
	}
	catch (fileNotFound) 
	{	
		throw fileNotFound(); 
		return; 
	}
	
	return;
}

void CBvhObject::process(string line)
{	
	//bool switched = false;	
	if (line == "OFFSET") {
		//	if (theMode != Site) {	
				vertIndex = 0;	
				theMode = OFFSET;
		//	}
	} else if (line == "ROOT") {
		root = new BvhNode;
		root->parent = NULL;
		current = root;
			
		theMode = ROOT;
	} else if (line == "{") {
			
	} else if (line == "}") {
	//	if (theMode == Site) { theMode = NONE; }
	//	else 
			if (current != root) { 
				current = current->parent;
		       		theMode = NONE;
			}
	} else if (line == "JOINT") {
		BvhNode *temp = new BvhNode;
		temp->parent = current;
		current->child.push_back(temp);
		current = temp;
		theMode = JOINT; 
	} else if (line == "CHANNELS") {
		theMode = CHANNELS;
	} else if (line == "End") {	
                theMode = End;
	} else if (line == "Site") {
		BvhNode *temp = new BvhNode;
		temp->parent = current;
		current->child.push_back(temp);
		current = temp;
		theMode = NONE; //Site;
	} else if (line == "MOTION") {
		theMode = MOTION;
	} else if (line == "Frames:") {
		theMode = Frames;
	} else if (line == "Frame") {
		theMode = Frame;
	} else if (line == "Time:") {
		theMode = Time;		
	} else {
		
		switch (theMode) {
			case (ROOT):
				root->name = line;
				
				theMode = NONE;
			break;
		
			case (JOINT):
				current->name = line;
				/*
				if (line == "LeftKnee") {
					try { current->objPart = new rigid("obj/leftforeleg.obj"); }
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "RightKnee") {
					try { current->objPart = new rigid("obj/rightforeleg.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "LeftHip") {
					try { current->objPart = new rigid("obj/leftthigh.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "RightHip") {
					try { current->objPart = new rigid("obj/rightthigh.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "LeftAnkle") {
					try { current->objPart = new rigid("obj/leftfoot.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "RightAnkle") {
					try { current->objPart = new rigid("obj/rightfoot.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "Chest") {
					//current->objPart = new objloader("obj/torso.obj",0,subdir);
				} else if (line == "Chest2") {
					try { current->objPart = new rigid("obj/torso.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "LeftShoulder") {
					try { current->objPart = new rigid("obj/leftshoulder.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "RightShoulder") {
					try { current->objPart = new rigid("obj/rightshoulder.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "LeftElbow") {
					try { current->objPart = new rigid("obj/leftforearm.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "RightElbow") {
					try { current->objPart = new rigid("obj/rightforearm.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "LeftWrist") {
					try { current->objPart = new rigid("obj/lefthand.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "RightWrist") {
					try { current->objPart = new rigid("obj/righthand.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "Neck") {
					try { current->objPart = new rigid("obj/neck.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				} else if (line == "Head") {
					try { current->objPart = new rigid("obj/head.obj");}
					catch (fileNotFound) { throw fileNotFound(); return; }
					current->objPart->location.matrix[10]=1.0f;
				}*/
				
				
				//	cout << line << "\n";
				theMode = NONE;
			break;
			
			case (OFFSET):
				current->offset.vertex[vertIndex] = float(atof(line.c_str()));
				vertIndex++;
				if (vertIndex >2) {
					vertIndex = 0;
					theMode = NONE;
				}
			break;

			case (CHANNELS):
				// assume channelsNum == 0 in the .CBvhObject .is impossible
				if (channelsNum == 0) {
					channelsNum = atoi(line.c_str());
				} else if (line == "Xposition") {		
					current->channels.push_back(BvhNode::Xpos);	
				} else if (line == "Yposition") {					
					current->channels.push_back(BvhNode::Ypos);	
				} else if (line == "Zposition") {				
					current->channels.push_back(BvhNode::Zpos);	
				} else if (line == "Zrotation") {
					current->channels.push_back(BvhNode::Zrot);				
				} else if (line == "Yrotation") {
					current->channels.push_back(BvhNode::Yrot);				
				} else if (line == "Xrotation") {
					current->channels.push_back(BvhNode::Xrot);				
				}
				
				// if there are additional channel types in error, they'll be ignored(?)
				if (current->channels.size() == channelsNum) {
					theMode = NONE;
					channelsNum = 0;
				}
				break;

			case (Frames):
				framesNum = atoi(line.c_str());
				theMode = NONE;
			
				break;

			case (Frame):
				break;

			case (Time):
				frameTime = float(atof(line.c_str()));
				theMode = MOTIONDATA;
				current = root;
				recurs(root);

				break;

			case (MOTIONDATA):
				
				data++;
				switch (bvhPartsLinear[partIndex]->channels[channelIndex]) {
					case (BvhNode::Xpos):	
						tempMotion.Translate(float(atof(line.c_str())),0,0);	
						channelIndex++;
						break;
					case (BvhNode::Ypos):
						tempMotion.Translate(0,float(atof(line.c_str())),0);
						channelIndex++;
						break;
					case (BvhNode::Zpos):
						tempMotion.Translate(0,0,float(atof(line.c_str())));
						channelIndex++;
						break;
					case (BvhNode::Zrot):
						// it seems like some limbs need a negative, and out
						// limbs don't
						tempMotionZ.RotateZ((float)-DEG_TO_RAD(atof(line.c_str())));
						tempRotateZ = (float)-DEG_TO_RAD(atof(line.c_str()));
						channelIndex++;
						break;
					case (BvhNode::Yrot):
						//if (partIndex == 3) cout << atof(line.c_str()) << "\n";
						tempMotionY.RotateY((float)-DEG_TO_RAD(atof(line.c_str())));
						tempRotateY = (float)-DEG_TO_RAD(atof(line.c_str())); 
						//tempMotion.print();
						channelIndex++;
						break;
					case (BvhNode::Xrot):
						//if (partIndex == 3) cout << atof(line.c_str()) << "\n";
						tempMotionX.RotateX((float)-DEG_TO_RAD(atof(line.c_str())));
						tempRotateX = (float)-DEG_TO_RAD(atof(line.c_str()));
						channelIndex++;
						break;
				}
				
				if (channelIndex >= bvhPartsLinear[partIndex]->channels.size()) {
					// store tempMotion and move to next part
					tempMotion = tempMotion * (tempMotionZ *tempMotionX * tempMotionY );// * tempMotionY);
					bvhPartsLinear[partIndex]->motion.push_back(tempMotion);
					vector3f rotate;
					rotate.Set(tempRotateX,tempRotateY,tempRotateZ);
					bvhPartsLinear[partIndex]->rotate.push_back(rotate);
					//tempMotion.print();
					
					tempMotion.LoadIdentity();
					tempMotionX.LoadIdentity();
					tempMotionY.LoadIdentity();
					tempMotionZ.LoadIdentity();
					tempRotateX = 0.0f;
					tempRotateZ = 0.0f;
					tempRotateY = 0.0f;
	
	/*				tempMotion.matrix[10] = -1.0f;
					tempMotionX.matrix[10] = -1.0f;
					tempMotionY.matrix[10] = -1.0f;
					tempMotionZ.matrix[10] = -1.0f;
	*/

					channelIndex = 0;
					partIndex++;
				}	
				if (partIndex >= bvhPartsLinear.size()) {
					// this should be the end of one line of motion data
					partIndex = 0;
				}	
				
			case (NONE):
			case (End):
			case (Site):
			case (MOTION):
				break;
			
		}
	}
	/*
	if (mode == 1) {
		v_ind++;
		if (v_ind == 2) v_ind = 0;
	}*/	
}

void CBvhObject::recurs(BvhNode* some)
{
	matrix16f motion0;
	motion0.LoadIdentity();
//	motion0.matrix[10] = -1.0f; 
	 
	motion0.Translate(some->offset.vertex[0],some->offset.vertex[1],some->offset.vertex[2]);
	some->motion.push_back(motion0);
	if (some->child.size() != 0) bvhPartsLinear.push_back(some);
//	cout << some->name << some->offset.vertex[0] << " " <<
//				some->offset.vertex[1] << " " <<
//				some->offset.vertex[2] << "\n";
	unsigned i;
	for (i = 0; i < some->child.size(); i++)
		recurs(some->child[i]);
				
}
 
void CBvhObject::init(string bvhFile)
{
	data = 0;
	partIndex = 0;
	channelIndex = 0;	

	tempMotion.LoadIdentity();
	tempMotionX.LoadIdentity();
	tempMotionY.LoadIdentity();
	tempMotionZ.LoadIdentity();
	
	HANDLE hFile = OpenFile((TCHAR*)bvhFile.c_str());
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
	
	for (unsigned i =0; i < lines.size(); i++) {
		try 
		{ 
			process(lines[i]); 
		}
		catch (fileNotFound) 
		{ 
			throw fileNotFound(); 
			return; 
		}
	}

	
	framesNum = bvhPartsLinear[0]->motion.size();
	
	//if (verbose) cout << "\n\n";
//	if (verbose) cout << "framesNum = " << framesNum << "\nframetime = " << frameTime << "\n"; 
//	if (verbose) cout << data/(framesNum*3)-1 << " bodyparts\n";

	//for (i =0; i< root->motion.size(); i++)
	//	root->child[0]->child[0]->motion[i].print();
/*
	for (i = 0; i < bvhPartsLinear[1]->motion.size(); i++) {
		bvhPartsLinear[1]->motion[i].print();		
	}
*/	
	//return;	
}


vector<BvhNode*> * CBvhObject::GetLinear()
{
	return &bvhPartsLinear;
}
