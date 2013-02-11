#ifndef CRY_UTIL_H
#define CRY_UTIL_H

#include "BvhMVMath.h"


//Chunk types for chunk header
enum ChunkTypes
{
	ChunkType_ANY			= 0,
	ChunkType_Mesh			= 0xCCCC0000,
	ChunkType_Helper,
	ChunkType_VertAnim,
	ChunkType_BoneAnim,
	ChunkType_GeomNameList,	//Obsolute
	ChunkType_BoneNameList,
	ChunkType_MtlList,		//obsolute
	ChunkType_MRM,			//obsolute
	ChunkType_SceneProps,
	ChunkType_Light,
	ChunkType_PatchMesh,	//not implemented
	ChunkType_Node,
	ChunkType_Mtl,
	ChunkType_Controller,
	ChunkType_Timing,
	ChunkType_BoneMesh,
	ChunkType_BoneLightBinding, // describes the lights binded to bones
	ChunkType_MeshMorphTarget,  // describes a morph target of a mesh chunk
	ChunkType_BoneInitialPos,		// describes the initial position (4x3 matrix) of each bone; just an array of 4x3 matrices
	ChunkType_SourceInfo // describes the source from which the cgf was exported: source max file, machine and user
};


//========================================
//File Headers
//========================================
#define FILE_SIGNATURE	"CryTek"

enum FileTypes { FileType_Geom = 0xFFFF0000, FileType_Anim};

struct FILE_HEADER
{
	char	Signature[7];
	int		FileType;
	int		Version;
	int		ChunkTableOffset;
};

#define GeomFileVersion		0x0744
#define AnimFileVersion		0x0744

//some common cry structs:

struct CHUNK_HEADER_0744
{
	enum {VERSION = 0x0744};
	ChunkTypes	ChunkType;
	int			ChunkVersion;
	int			FileOffset;
	int			ChunkID;		//new in version 0x0744

	CHUNK_HEADER_0744()
	{
		FileOffset = -1;
	}
};

typedef CHUNK_HEADER_0744 CHUNK_HEADER;
// Keyframe and Timing Primitives __________________________________________________________________________________________________________________
struct BaseKey
{
	int time;
};

struct BaseTCB
{
	float t,c,b;
	float ein, eout;
};

struct BaseKey1 : BaseKey { float		val; };
struct BaseKey3 : BaseKey { vector3f	val; };
//struct BaseKeyQ : BaseKey { CryQuat	val; };

struct CryLin1Key : BaseKey1 {};
struct CryLin3Key : BaseKey3 {};
//struct CryLinQKey : BaseKeyQ {};
struct CryTCB1Key : BaseKey1 , BaseTCB {};
struct CryTCB3Key : BaseKey3 , BaseTCB {};
//struct CryTCBQKey : BaseKeyQ , BaseTCB {};
struct CryBez1Key : BaseKey1 { float		intan, outtan; };
struct CryBez3Key : BaseKey3 { vector3f	intan, outtan; };
//struct CryBezQKey : BaseKeyQ {};
/*struct CryBoneKey : BaseKey
{
	vector3f	abspos;
	vector3f	relpos;
	CryQuat	relquat;
};*/

struct CryKeyPQLog
{
	int nTime;
	vector3f vPos;
	vector3f vRotLog; // logarithm of the rotation

	// resets to initial position/rotation/time
	void reset ()
	{
		nTime = 0;
		vPos.vertex[0] = vPos.vertex[1] = vPos.vertex[2] = 0;
		vRotLog.vertex[0] = vRotLog.vertex[1] = vRotLog.vertex[2] = 0;
	}
};



//========================================
//Controller Chunk Header
//========================================
enum CtrlTypes 
{ 
	CTRL_NONE, 
	CTRL_CRYBONE, 
	CTRL_LINEER1,	CTRL_LINEER3,	CTRL_LINEERQ, 
	CTRL_BEZIER1,	CTRL_BEZIER3,	CTRL_BEZIERQ, 
	CTRL_TCB1,		CTRL_TCB3,		CTRL_TCBQ,
	CTRL_BSPLINE_2O, // 2-byte fixed values, open
	CTRL_BSPLINE_1O, // 1-byte fixed values, open
	CTRL_BSPLINE_2C, // 2-byte fixed values, closed
	CTRL_BSPLINE_1C,  // 1-byte fixed values, closed
	CTRL_CONST       // constant position&rotation
};

enum CtrlFlags
{
	CTRL_ORT_CYCLE = 0x01,
	CTRL_ORT_LOOP = 0x02
};

// descriptor-header of the controller chunk, precedes the controller data
struct CONTROLLER_CHUNK_DESC_0826
{
	enum {VERSION=0x0826};

	CHUNK_HEADER	chdr;

	CtrlTypes		type;		//one ot the CtrlTypes values
	int				nKeys;		// # of keys this controller has; toatl # of knots (positional and orientational) in the case of B-Spline

	//unsigned short nSubCtrl;	// # of sub controllers; not used now/reserved
	unsigned int nFlags;		// Flags of controller.
	//int				nSubCtrl;	// # of sub controllers; not used now/reserved

  unsigned nControllerId; // unique generated in exporter id based on crc32 of bone name
};

// intermediate version between the CONTROLLER_BSPLINE_DATA_0826 and CONTROLLER_CHUNK_DESC_0826
// followed by CryKeyPQLog array
struct CONTROLLER_CHUNK_DESC_0827
{
	enum {VERSION = 0x0827};
	unsigned numKeys;
	unsigned nControllerId;
};

class NameList  
{
public:
	std::vector<std::string> arrNames;
	vector<BvhNode>			nodes;
	vector<matrix9f>		invTM;

	//NameList();
	//virtual ~NameList();

	int GetID(BvhNode *node, bool bAppend = true);
	void Reset();
};


class CHUNK_LIST  
{
public:
	vector<void*>	ptr_list;
	vector<CHUNK_HEADER>		ch_list;

	CHUNK_LIST()			{Reset();};
	virtual ~CHUNK_LIST()	{};

	int GetID(void *ptr,int ChunkType);
	int GetID_Xept_type(void *ptr,int Xept_ChunkType);
	int* GetPtr(int ID, int ChunkType);

	int	Append(void *ptr, CHUNK_HEADER *ch, bool assign_ID);
	void Reset();
};

struct RANGE_ENTITY
{
	char name[32];
	int start;
	int end;
};

//========================================
//Timing Chunk Header
//========================================
struct TIMING_CHUNK_DESC_0918
{
	CHUNK_HEADER	chdr;

	float			SecsPerTick;		// seconds/ticks
	int				TicksPerFrame;		// ticks/Frame

	RANGE_ENTITY	global_range;		// covers all of the time ranges
	int				nSubRanges;
};

typedef TIMING_CHUNK_DESC_0918 TIMING_CHUNK_DESC;
#define TIMING_CHUNK_DESC_VERSION 0x0918


// this structure describes the bone names
// it's followed by numEntities packed \0-terminated strings, the list terminated by double-\0
struct BONENAMELIST_CHUNK_DESC_0745
{
	enum {VERSION = 0x0745};
	int numEntities;
};

class CryCAFExporter
{
public:
	int SaveBoneAnimFile(CBvhObject * rootbone, char * filename);
private:
	bool SaveBoneController(FILE *f, BvhNode *node, float parentkeytimes);
	bool WriteChunkList(FILE *f, FILE_HEADER &header);
	bool SaveBoneNameList(FILE* f);
	bool SaveTiming(FILE* f, CBvhObject * bvhobject);

	NameList			BoneList;
	CHUNK_LIST			ChunkList;
};


//-----------------------------------------------------------------------------------------------
//
//	CrC32 Gen
//
//-----------------------------------------------------------------------------------------------
class Crc32Gen {
public:
	Crc32Gen();
	//! Creates a CRC from a text string 
	static unsigned int GetCRC32( const char *text );
	static unsigned int GetCRC32( const char *data,int size,unsigned int ulCRC );

protected:
	unsigned int crc32_table[256];  //!< Lookup table array 
	void init_CRC32_Table();  //!< Builds lookup table array 
	unsigned int reflect( unsigned int ref, char ch); //!< Reflects CRC bits in the lookup table 
	unsigned int get_CRC32( const char *data,int size,unsigned int ulCRC );
};

#endif