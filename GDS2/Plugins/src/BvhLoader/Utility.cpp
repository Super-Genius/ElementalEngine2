#include "stdafx.h"
#include <math.h>
#include <assert.h>
#include "Utility.h"


inline double DLength (const vector3f& v)
{
	return sqrt(double(v.vertex[0])*double(v.vertex[0]) + double(v.vertex[1])*double(v.vertex[1]) + double(v.vertex[2])*double(v.vertex[2]));
}

//////////////////////////////////////////////////////////////////////////
// Given the rotations in logarithmic space, adjusts the target rotation so
// that it's the same in rotational group, but the closest to the reference
// in the QLog space.
inline void AdjustRotLog (vector3f& vTgt, const vector3f& vRef)
{
	double dLenTgt = DLength(vTgt);
	const double dPi = 3.1415926535897932384626433832795;
	if (dLenTgt < 1e-4)
	{
		// the target is very small rotation, so the algorithm is to find 
		// ANY vector of length n*PI closest to the vRef point
		double dLenRef = DLength(vRef);
		if (dLenRef > dPi/2) // Otherwise the vRef vector is small enough not to make any adjustments
		{
			double f = (dPi * floor(dLenRef/dPi+0.5) / dLenRef);
			vTgt.vertex[0] = float(vRef.vertex[0] * f);
			vTgt.vertex[1] = float(vRef.vertex[1] * f);
			vTgt.vertex[2] = float(vRef.vertex[2] * f);
		}
	}
	else
	{
		// the target is big enough rotation to pick the rotation axis out

		// find the projection of the reference to the target axis
		// then find the target (projection) mod PI
		// there are basically three possibilities: the new target is in the same PI interval
		// as the reference projection, in the next or in the previous. Find the closest.
		vector3f dotvect = (vRef * vTgt);
		
		double dProjRef =  (dotvect.vertex[0] + dotvect.vertex[1] + dotvect.vertex[2])/ dLenTgt;
		double dModTgt = fmod (dLenTgt, dPi);
		if (dModTgt < 0)
			dModTgt += dPi;
		assert(dModTgt >= 0 && dModTgt < dPi);

		double dBaseTgt = dPi * floor (dProjRef/dPi + 0.5);

		double dNewTgtR = dBaseTgt + dModTgt;
		double dNewTgtL = dBaseTgt + dModTgt - dPi;
		double dNewTgt = (fabs(dNewTgtR - dProjRef) < fabs(dNewTgtL  - dProjRef))
			? dNewTgtR : dNewTgtL;

		assert (fabs(dNewTgt + dPi - dProjRef) > fabs(dNewTgt - dProjRef));
		assert (fabs(dNewTgt - dPi - dProjRef) > fabs(dNewTgt - dProjRef));

		double f = (dNewTgt / dLenTgt);
		vTgt.vertex[0] = float(vTgt.vertex[0]*f);
		vTgt.vertex[1] = float(vTgt.vertex[1]*f);
		vTgt.vertex[2] = float(vTgt.vertex[2]*f);
	}
}

// ==============================================================
void CHUNK_LIST::Reset()
{
	ch_list.clear();
	ptr_list.clear();
}

int CHUNK_LIST::GetID(void* ptr, int ChunkType)
{

	for(unsigned i=0;i<ptr_list.size();i++)
	{
		if(ptr == ptr_list[i] 
		&& ( !ChunkType || ChunkType==ch_list[i].ChunkType ))
			return ch_list[i].ChunkID;
	}

	return -1;
}

int	CHUNK_LIST::Append(void *ptr, CHUNK_HEADER *ch, bool assign_ID)
{
	//check if we already have a chunk for this pointer. If so skip this
	if(ptr && GetID(ptr,ch->ChunkType) != -1) return -1;

	if(assign_ID) ch->ChunkID = ch_list.size();

	ch_list.push_back(*ch);
	ptr_list.push_back(ptr);

	return ch_list.size()-1;
}

void NameList::Reset()
{
	arrNames.clear();
	nodes.clear();
	invTM.clear();
}

int NameList::GetID(BvhNode *node, bool bAppend)
{
	if (!node) return -1;

	for(unsigned i=0;i<nodes.size();i++)
	{
		if(node==&nodes[i]) return i;
	}

	if (!bAppend)
		return -1;

	nodes.push_back(*node);
	arrNames.push_back(node->name.c_str());

	//TimeValue time = theCSExportUtility.GetTime();
	//matrix9f invmat=node->GetNodeTM(time);
	//invmat.NoScale();
	//invmat.Invert();
	//invTM.push_back(invmat);

	return nodes.size()-1;
}


// WriteChunkList ______________________________________________________________________________________
bool CryCAFExporter::WriteChunkList(FILE *f, FILE_HEADER &header)
{
	//get chunk list pos
	fpos_t chunklistpos;
	fgetpos(f,&chunklistpos);

	//remove the chunks that are not saved
	vector<CHUNK_HEADER> tempv;
	vector<void *> tempp;
	for(unsigned i=0; i < ChunkList.ch_list.size();i++) 
	{
		if(ChunkList.ch_list[i].FileOffset == -1)
		{
			//ChunkList.ch_list.Delete(i,1);
			//ChunkList.ptr_list.Delete(i,1);
		}else
		{
			tempv.push_back(ChunkList.ch_list[i]);
			tempp.push_back(ChunkList.ptr_list[i]);
		}
	}
	ChunkList.ch_list.clear();
	ChunkList.ptr_list.clear();
	for(unsigned i=0; i < ChunkList.ch_list.size();i++) 
	{
		ChunkList.ch_list.push_back(tempv[i]);
		ChunkList.ptr_list.push_back(tempp[i]);

	}

	//=======================
	//Write # of Chunks
	//=======================
	int nch=ChunkList.ch_list.size();
	int res=fwrite(&nch,sizeof(nch),1,f);
	if(res!=1) return true;

	//=======================
	//Write Chunk List
	//=======================
	for(int i=0;i<nch;i++)
	{
		CHUNK_HEADER ch_ent=ChunkList.ch_list[i];
		int res=fwrite(&ch_ent,sizeof(ch_ent),1,f);
		if(res!=1) return true;
	}

	//update Header for chunk list offset
	header.ChunkTableOffset		= int(chunklistpos);
	res=fseek(f,0,SEEK_SET);
	if(res) return true;
	res=fwrite(&header,sizeof(header),1,f);
	if(res!=1) return true;

	return false;
}
// SaveBoneAnimFile ______________________________________________________________________________________
int CryCAFExporter::SaveBoneAnimFile(CBvhObject * rootbone, char * filename)
{
/*
	#ifdef PROTECTION_ENABLED
		#pragma message ("===========Protection in SaveBoneAnimFile Enabled ===========")
		if(!CheckIntegrity())
		{
			ip->RegisterCommandModeChangedCallback((CommandModeChangedCallback *)0x13051972);
		}
	#endif
*/
	#define HALT { fclose(f); DeleteFile(filename); return IDNO; }
//	TimeValue time;//=GetTime();
	//===================================
	//check if there are bones to export
	//===================================
	//int nobjects=pb_bones->Count(PB_BONES);

	FILE *f = 0;

	//for(int k=0;k<nobjects;k++)
	{
		//INode *rootbone=pb_bones->GetINode(PB_BONES,0,k);
		if(rootbone)
		{
	
		//=================
		//create anim file
		//=================
		char def_fname[512];
		char *max_name=filename;
		if(strlen(max_name)>=4)
		{
			strcpy(def_fname,max_name);
			def_fname[strlen(def_fname)-4]='_';
			def_fname[strlen(def_fname)-3]=0;
			strcat(def_fname,rootbone->root->name.c_str());
		}
		else strcpy(def_fname,rootbone->root->name.c_str());

		int	 res=0;
		char filename[1024];
		//char filenameOrg[1024];
		//char str[512];
		//sprintf(str,"Save skeleton starting at bone \"%s\" As...",rootbone->GetName());
		//int bSave = SaveFileDlg(FileType_Anim,filename, str, def_fname);
		//if(!bSave) return IDCANCEL;
		strcpy(filename, def_fname);

    //char tempPath[1024];
		//strcpy( filenameOrg,filename );
    //GetTempPath( sizeof(tempPath),tempPath );
    //strcpy( filename,tempPath );
    //strcat( filename,"CryExport.caf" );
		f = fopen( filename,"wb" );
		if (!f) return IDNO; // error: cannot open the file
		
		ChunkList.Reset();

		//==================
		//Write File Header
		//==================
		FILE_HEADER header;
		header.FileType				= FileType_Anim;
		header.ChunkTableOffset		= -1;
		header.Version				= AnimFileVersion;

		strcpy(header.Signature,FILE_SIGNATURE);
		res=fwrite(&header,sizeof(header),1,f);
		res=res!=1;
		if(res) HALT;

		CHUNK_HEADER ch_ent;
		ChunkList.Reset();
		BoneList.Reset();
		ch_ent.FileOffset		= -1;
		//=======================
		//Prepare SceneProperties
		//=======================
		//int numProps = 0;//ip->GetNumProperties(PROPSET_USERDEFINED);
		//if(numProps)
		//{
		//	ch_ent.ChunkType		= ChunkType_SceneProps;
		//	ch_ent.ChunkVersion		= SCENEPROPS_CHUNK_DESC_VERSION;
		//	ChunkList.Append(NULL,&ch_ent,true);
		//}
		//=======================
		//Prepare Timing
		//=======================
		ch_ent.ChunkType		= ChunkType_Timing;
		ch_ent.ChunkVersion		= TIMING_CHUNK_DESC_VERSION;
		ChunkList.Append(NULL,&ch_ent,true);

		//---------------------------------------------------
		//Get all the bones and prepare the controller chunks
		//---------------------------------------------------
		//INodeTab allnodes;
		//allnodes.Append(1,&rootbone);
		//if (GetValidChildrenList(rootbone, allnodes, true))
		//	return true;
		//go through all children
		//add controller chunks
		for(unsigned i=0;i<(rootbone->GetLinear())->size();i++)
		{
			BvhNode *node = (*rootbone->GetLinear())[i];

			//============================
			//Prepare Controller chunk(s)
			//============================
			ch_ent.ChunkType		= ChunkType_Controller;
			ch_ent.ChunkVersion		= CONTROLLER_CHUNK_DESC_0827::VERSION;
			ChunkList.Append(node,&ch_ent,true);

			//also fill the Bonelist
			BoneList.GetID(node);
		}
		
		/*
		//============================
		//Prepare bone hierarchy chunk
		//============================
		ch_ent.ChunkType		= ChunkType_BoneAnim;
		ch_ent.ChunkVersion		= BONEANIM_CHUNK_DESC_VERSION;
		ChunkList.Append(rootbone,&ch_ent,true);
		*/
				
		//===========================
		//prepare bone namelist chunk
		//===========================
		ch_ent.ChunkType		= ChunkType_BoneNameList;
		ch_ent.ChunkVersion		= BONENAMELIST_CHUNK_DESC_0745::VERSION;
		ChunkList.Append(NULL,&ch_ent,true);

		//---------------------------------------------------
		//Every chunk is prepared.. Now write them
		//---------------------------------------------------
		//==================================================
		//Treverse the Bone Tree and save Controller Chunks
		//==================================================
		res=SaveBoneController(f, rootbone->root, rootbone->frameTime);
		if(res) HALT;

		//==================
		//Save other chunks
		//==================
		for(i=0;i<ChunkList.ch_list.size();i++)
		{
			int	ChunkType = ChunkList.ch_list[i].ChunkType;
			//int* ChunkPtr = (int*)ChunkList.ptr_list[i];
			int res=0;

			switch(ChunkType)
			{
			//case ChunkType_Controller:
				//skip: we expoerted hierarchically with SaveBoneController
			//	break;

			case ChunkType_BoneNameList:
				res=SaveBoneNameList(f);
				break;

			case ChunkType_Timing:
				res=SaveTiming(f,rootbone);
				break;

			//case ChunkType_SceneProps:
			//	res=SaveSceneProps(f);
			//	break;

			case ChunkType_BoneMesh:
				//res=SaveMeshObject(f,(INode*)ChunkPtr,true,false);
				break;

			default:
				break;
			//		CryMessageBox(hwnd_nodes, "unsuported chunk type encountered", "Internal Error", MB_OK | MB_ICONINFORMATION);
			}

			if(res) HALT;
		}
		
		//=======================
		//Write Chunk List
		//=======================
		res=WriteChunkList(f, header);
		if(res) HALT;

		//==========================================
		//CLOSE File and Call DLL Callback function
		//==========================================
		fclose(f);
		//CallExtensionDLLs(filename, DLL_FLAG_ANIM);

		}
	}

#undef HALT	
	return IDYES;
}

//---------------------------------------------------------------------
// Function:	SaveBoneController
// Description: Recurses through bone hierarchy and saves them
// Parameters:	f = In File pointer, node = bone node, parentkeytimes = 
// Returns:		bool = true or false
//---------------------------------------------------------------------
bool CryCAFExporter::SaveBoneController (FILE *f, BvhNode *node, float parentkeytimes)
{
	//TimeValue time;//=GetTime();
	char *name = (char*)node->name.c_str();//node->GetName();
	

	//---------------------
	// Determine the flags
	//---------------------
	//stabilize
	//	bool stabilize_leg=false;
	//if(pb_bones->GetInt(PB_BONE_ANIM_ATKEYS) && pb_bones->GetInt(PB_STABILIZE_FEET) && IsLegBone(node)) 
	//	stabilize_leg=true;
	//use parent times
	//bool use_parent_time = stabilize_leg || (pb_bones->GetInt(PB_BONE_ANIM_ATKEYS) && pb_bones->GetInt(PB_PARENT_TIMES));
	//use child times
	//bool use_child_time = pb_bones->GetInt(PB_BONE_ANIM_ATKEYS) && pb_bones->GetInt(PB_CHILD_TIMES);

	//----------------
	//Get Own Key times
	//----------------
	//Key times are all the same incrementally!

	//------------
	//Get the keys
	//------------
	//Tab<CryBoneKey> keys;
	//GetBoneNodeKeys (node, combined_keytimes, keys);

	//------------------------
	//Collect all of it's children inside a node
	//------------------------
	//vector<BvhNode> children;
	//children.ZeroCount();
	//if (GetValidChildrenList(node, children, false))
	//	return true;

	//==================
	//Write chunk header
	//==================
	//UpdateProgress(100,"\"%s\" Writing chunk header", name);
	fpos_t fpos;
	fgetpos(f,&fpos);
	int chunk_id=ChunkList.GetID(node,ChunkType_Controller);
	assert(chunk_id != -1);
	CHUNK_HEADER &ch_ent = ChunkList.ch_list[chunk_id];//*node;//*(ChunkList.ch_list.Addr(chunk_id));
	ch_ent.FileOffset	 = int(fpos);
	
	//DebugPrint(" (ChunkType_Controller at pos %d)",ch_ent.FileOffset);
	CONTROLLER_CHUNK_DESC_0827 chunk;
	chunk.numKeys				= node->motion.size();
	chunk.nControllerId = Crc32Gen::GetCRC32( name );
	int	res=fwrite(&chunk,sizeof(chunk),1,f);
	if(res!=1) return true;	
	/*
	//=======================
	//Write the bone entity
	//=======================
	UpdateProgress(100,"\"%s\" Writing bone ", node->GetName());
	TSTR PropStr; 
	node->GetUserPropBuffer(PropStr);
	BONE_ENTITY bone;
	bone.ParentID	= BoneList.GetID(node->GetParentNode());
	bone.BoneID		= BoneList.GetID(node);
	bone.nChildren	= children.Count();
	bone.nKeys		= keys.Count();
	strncpy(bone.prop,PropStr,32);
	int res=fwrite(&bone,sizeof(bone),1,f);
	if(res!=1) return true;
	*/

	//==============
	//Write the keys
	//==============
	CryKeyPQLog keyLast;
	keyLast.nTime = 0;
	keyLast.vRotLog.Set(0,0,0);// = Vec3d(0,0,0);
	keyLast.vPos.Set(0,0,0);

	for(unsigned i=0;i<node->motion.size();i++)
	{
		//DebugPrint(" [%d]", keys[i].time/GetTicksPerFrame());		

		CryKeyPQLog key;
		key.nTime = int(parentkeytimes*i);//keys[i].time; //parentkeytimes is per frame, i frames
		key.vPos = node->offset;

		key.vRotLog = node->rotate[i];//log (qLast).v;
		AdjustRotLog (key.vRotLog, keyLast.vRotLog);

		keyLast = key;

		int res=fwrite(&key,sizeof(key),1,f);
		if(res!=1) return true;
	}
	
	//-------
	//recurse
	//-------
	for(i=0;i<node->child.size();i++)
	{
		int res;
		if(node->child[i]->name.length() <= 0)
			continue;
		//if(stabilize_leg && IsLegBone(node->child[i]))
			res=SaveBoneController(f,node->child[i],parentkeytimes);
		//else
		//	res=SaveBoneController(f,node->child[i],);
		
		if(res) return res;
	}
	return false;
}

bool CryCAFExporter::SaveBoneNameList(FILE* f)
{
	//==================
	//Write chunk header
	//==================
	fpos_t fpos;
	fgetpos(f,&fpos);
	//append to chunk list
	int chunk_id=ChunkList.GetID(NULL,ChunkType_BoneNameList);
	assert(chunk_id != -1);
	CHUNK_HEADER &ch_ent = ChunkList.ch_list[chunk_id];
	ch_ent.FileOffset	 = int(fpos);
//	DebugPrint("\n(ChunkType_BoneNameList at pos %d)",ch_ent.FileOffset);


	//UpdateProgress(100,"Writing bone name list");

	BONENAMELIST_CHUNK_DESC_0745 chunk;
	chunk.numEntities = BoneList.arrNames.size();
	int res=fwrite(&chunk,sizeof(chunk),1,f);
	if(res!=1) return true;

	//==================
	//Write bone names
	//==================
	for (int i = 0; i < chunk.numEntities; ++i)
	{
		const std::string& strName = BoneList.arrNames[i];
		int res = fwrite (strName.c_str(),strName.length()+1,1,f);
		if(res!=1) return true;
	}
	// write the terminating double-\0
	res = fwrite ("\0", 1, 1, f);
	if (res != 1) return true;

	// move the file pointer position so that it's 4-byte aligned
	fgetpos(f,&fpos);
	unsigned long nPad = 0; // pad bytes

	if (fpos & 3)
	{
		// unaligned pos
		fwrite (&nPad, 1, size_t(4- (fpos&3)), f);
	}

	return false;
}


bool CryCAFExporter::SaveTiming(FILE* f, CBvhObject * bvhobject)
{
	//=======================
	//Write TimeRanges
	//=======================
	int anim_start;
	int anim_end;
	//int	manualrange = pb_timing->GetInt(PB_MAN_RANGE);

	//if(manualrange)
	{
		anim_start	= 0;// pb_timing->GetInt(PB_RANGE_START);
		anim_end	= bvhobject->framesNum;//*bvhobject->frameTime;//pb_timing->GetInt(PB_RANGE_END);
	}
	//else
	//{
	//	Interval ii =ip->GetAnimRange();
	//	anim_start	=ii.Start() / GetTicksPerFrame();
	//	anim_end	=ii.End()   / GetTicksPerFrame();
	//}

	//==================
	//Write chunk header
	//==================
	fpos_t fpos;
	fgetpos(f,&fpos);
	//append to chunk list
	int chunk_id=ChunkList.GetID(NULL,ChunkType_Timing);
	assert(chunk_id != -1);
	CHUNK_HEADER &ch_ent = ChunkList.ch_list[chunk_id];
	ch_ent.FileOffset	 = int(fpos);
	//DebugPrint("\n(ChunkType_Timing at pos %d)",ch_ent.FileOffset);

	TIMING_CHUNK_DESC chunk;
	chunk.chdr					= ch_ent;
	chunk.global_range.start	= anim_start;
	chunk.global_range.end		= anim_end;
	chunk.SecsPerTick			= 1/1000;//TicksToSec(1);
	chunk.TicksPerFrame			= bvhobject->frameTime;//GetTicksPerFrame();
	strcpy(chunk.global_range.name,"GlobalRange");
	chunk.nSubRanges = 0;

	//chunk.nSubRanges			= Ranges.Count();
	chunk.nSubRanges			= 0;

//	if (animationName.Length() > 0)
//		chunk.nSubRanges = 1;

	int res=fwrite(&chunk,sizeof(chunk),1,f);
	if(res!=1) return true;

//	if (animationName.Length() > 0)
//	{
//		RANGE_ENTITY ent;
//		strcpy( ent.name,animationName );
//		ent.start = anim_start;
//		ent.end = anim_end;
//		int res=fwrite(&ent,sizeof(ent),1,f);
//		if(res!=1) return true;
//	}

	/*
	for(int i=0;i<chunk.nSubRanges;i++)
	{
		RANGE_ENTITY ent=Ranges[i];
		int res=fwrite(&ent,sizeof(ent),1,f);
		if(res!=1) return true;
	}
	*/

	return false;
}