#ifndef BVH_PART_H
#define BVH_PART_H

#include <iterator>
#include <string>
#include <vector>

#include "BvhMVMath.h"
//#include "rigid.hpp"

using namespace std;

class fileNotFound {};
//class ChannelError;

// how should the animation info fit in?
// per part, or globally?
class BvhNode
{
	
public: 
	enum channelTypes {Xpos,Ypos,Zpos,Zrot,Xrot,Yrot};
	
	BvhNode();

	string name;

	vector3f offset;
	
	vector<matrix16f> motion;
	
	//objloader* objPart;
	//rigid* objPart;
	
	BvhNode* parent;
	vector<channelTypes> channels;
	vector<BvhNode*> child;
};


class CBvhObject : public CBaseObject
{
	bool verbose;

	enum mode {NONE,OFFSET,CHANNELS,JOINT,ROOT,End,Site,MOTION,Frames,Frame,Time,MOTIONDATA};


	public:
	BvhNode *root;
	float frameTime;

private:

	// mostly used just for init/processing- what to do?
	// Have another class with just essential data structures
	// that uses a processing class and then deletes it- more mem efficient
	BvhNode *current;
	vector <BvhNode*> bvhPartsLinear;
	
       mode theMode;
	int vertIndex;
	
	unsigned channelIndex;
	unsigned partIndex;

	

	int data;	
	/// the channels vector will store it's own size, so this is just for error checking.
	unsigned channelsNum;	
	
	
	matrix16f tempMotion;
	matrix16f tempMotionY;
	matrix16f tempMotionX;
	matrix16f tempMotionZ;

public:
	CBvhObject(string bvhFile);

	int framesNum;

	void recurs(BvhNode* some);
	void process(string line);
	void init(string bvhFile);
	vector<BvhNode *> * GetLinear();
 
};

#endif
