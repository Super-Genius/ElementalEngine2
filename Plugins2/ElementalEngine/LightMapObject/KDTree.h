///============================================================================
/// \note   Elemental Engine
///         Copyright (c)  2005-2008 Signature Devices, Inc.
///
///         This code is redistributable under the terms of the EE License.
///
///         This code is distributed without warranty or implied warranty of
///			merchantability or fitness for a particular purpose. See the 
///			EE License for more details.
///
///         You should have received a copy of the EE License along with this
///			code; If not, write to Signature Devices, Inc.,
///			3200 Bridge Parkway Suite 102, Redwood City, CA 94086 USA.
///============================================================================

#ifndef _KDTREE_H_
#define _KDTREE_H_

#include <map>
#include <list>
using namespace std;

typedef list< int > INDEXLIST;

#pragma pack(1)
struct KDTreeNode {
	float key;
	int pntidx;
	int axis;
	int leftIdx;
	int rightIdx;
	AABB bounds;
	INDEXLIST objectList;
	vector< int> fattyCellData;
};
#pragma pack()

struct kdstack
{
	KDTreeNode* node;
	float t;
	float pb[3];
	int prev, dummy1, dummy2;
	kdstack()
	{
		node = NULL;
	}
};

typedef bool (*NodeHandlerCallback)(KDTreeNode * node);
typedef bool (*RayIntersectCallback)( vector< KDTreeNode * > &nodes );

typedef float *Range;
typedef int IRange[2];

class KDTree 
{
public:
	KDTree(float *setpoints, int N, int setndim);
	KDTree();

	virtual ~ KDTree();

	int create(float *setpoints, int setnpoints, int setndim,
							bool setCopy = false, 
						 struct KDTreeNode *setNodeMem = (struct KDTreeNode *)0);
	int create(float *setpoints,int setnpoints,int setndim,
						 void *mem);


	int ndim;
	int npoints;
	float *points;

	// Search for the nearest neighbor to pnt and	
	// return its index
	int GetClosestPoint(float *pnt, int &idx, bool approx=false);
	int GetClosestFattyCell(float *pnt, KDTreeNode **nodeOut );
	int GetClosestPoints(float *pnt, multimap< float, int > &sortedPoints, float &distancesquared );
	int GetNumClosestPoints( float * pnt,
								   int &numpoints );
	int GetPointsInRange(Range *range);
	int nPntsInRange;
	int *pntsInRange;
	int maxPoints;
	
	int GetClosestPointsInRange(Range *range, int &numpoints );

	// Return distance squared between points
	// between points
	inline float distsq(float *pnt1, float *pnt2);

	int SetVerbosity(int v){verbosity=v;return 0;}
	int FindNearestToRay2( Ray& a_Ray, Vec3 &min, Vec3 &max, RayIntersectCallback callback, Matrix4x4 * temp );
	int FindNearestToRay( Ray& a_Ray, Vec3 &min, Vec3 &max, RayIntersectCallback callback, Matrix4x4 * temp );
	void SetNodeCreationCallback( NodeHandlerCallback callback )
	{
		m_NodeCreationCallBack = callback;
	}
	int m_KDCellSize;
protected:
	int GetClosestNumPoints( float * pnt,
								   int nodeIdx,
								   int dim,
								   int &numpoints,	
								   bool grab, int depth );
	// Do we copy the points or not
	// if we do, this is where they go
	bool copyPoints;

	int check_border_distance_sort(int nodeIdx, int dim,
				  float *pnt, float &cdistsq, multimap< float, int > &sortedList );
	int check_border_distance(int nodeIdx, int dim,
			    float *pnt, float &dist, int &idx);
	int CheckNeighborFattyCells(int nodeIdx, int dim,
			    float *pnt, float &dist, KDTreeNode ** nodeOut);
	

	int range_search(int nodeIdx, Range *range, int dim);
	int ClosestRangeSearch(int nodeIdx, Range *range, int dim, int &maxpoints );
	int heapsort(int dim, int *idx, int len);

	int build_kdtree(int **sortidx, int dim, int *pidx, int len, AABB &currentBB);
  
	int *workArr;
	int* m_Mod;
	struct KDTreeNode *m_Root;
	kdstack* m_Stack;
	int nodeMemCnt;
	bool nodeMemAlloc;
	struct KDTreeNode *node_alloc();

	int *intArrMem;
	int  intArrMemCnt;
	int *int_alloc(int len);
  
	static int (*logmsg)(const char *,...);
	int verbosity;
	NodeHandlerCallback m_NodeCreationCallBack;
  
};

#endif
