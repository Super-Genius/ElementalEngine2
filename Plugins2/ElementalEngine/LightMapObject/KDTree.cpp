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

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define KD_SPLIT_BUFFER_SIZE 1.f
#define KEY_EPSILON .01f
#define RAY_TRAVERSAL_EPSILON 1.f

int (*KDTree::logmsg)(const char *,...)=printf;


void OutputVector( Vec3 &vec, char * name )
{
	static char buf[1024];
	sprintf( buf, "%s: (%f,%f,%f)\n", name, vec.x, vec.y, vec.z );
	OutputDebugString( buf );
}

void OutputVector(float array[3], char * name )
{
	static char buf[1024];
	sprintf( buf, "%s: (%f,%f,%f)\n", name, array[0], array[1],array[2]);
	OutputDebugString( buf );
}

KDTree::KDTree()
{
	m_KDCellSize = 1;
	workArr = (int *) NULL;
	copyPoints = false;
	points = (float *)NULL;
	pntsInRange = (int *)0;
	m_Root = (struct KDTreeNode *)0;
	nodeMemCnt = 0;
	nodeMemAlloc = true;
	intArrMem = (int *)0;
	intArrMemCnt = 0;
	verbosity=0;
	
	//For ray traversal
	m_Mod = new int[64];
	//m_Mod = (int*)((((unsigned long)m_Mod) + 32) & (0xffffffff - 31));
	m_Mod[0] = 0, m_Mod[1] = 1, m_Mod[2] = 2, m_Mod[3] = 0, m_Mod[4] = 1;
	m_Stack = new kdstack[64];
	//m_Stack = (kdstack*)((((unsigned long)m_Stack) + 32) & (0xffffffff - 31));
	m_NodeCreationCallBack = NULL;
	return;
}				// end of constructor

KDTree::KDTree(float *setpoints, int N, int setndim)
{
	KDTree();
	create(setpoints, N, setndim);
	return;
}	 // end of constructor


// Note: this is copied almost verbatim from the heapsort 
// wiki page: http://en.wikipedia.org/wiki/Heapsort 
// 11/9/05
int KDTree::heapsort(int dim, int *idx, int len)
{
	unsigned int n = len,i=len/2,parent,child;
	int t;

	for(;;) {
		if(i>0) {
		i--;
		t = idx[i];
		}
		else {
		n--;
		if(n ==0) return 0;
		t  = idx[n];
		idx[n] = idx[0];
		}
	  
		parent = i;
		child = i*2+1;
			
		while(child < n) {
		if((child +1 < n) &&
					(points[(idx[child+1])*ndim+dim] > 
						points[idx[child]*ndim + dim])) {
					child++;
		}
		if(points[idx[child]*ndim+dim] > points[t*ndim+dim]) {
					idx[parent] = idx[child];
					parent = child;
					child = parent*2+1;
		}
		else {
					break;
		}
		}
		idx[parent] = t;
	} // end of for loop
	return 0;
} // end of heapsort

      
  
KDTree::~KDTree()
{
  if (workArr)
    delete[]workArr;
  if (copyPoints && points) {
    // Delete the 1-D array of data
    // Delete the pointer to the points
    delete[] points;
  }
	if(pntsInRange) delete[] pntsInRange;
	if(m_Root && nodeMemAlloc) delete[] m_Root;
  return;
}				// end of destructor

struct KDTreeNode *KDTree::node_alloc()
{
	nodeMemCnt++;
	if(nodeMemCnt > npoints*2)
		return (struct KDTreeNode *)0;
	return (struct KDTreeNode *)(m_Root+nodeMemCnt-1);
} // end of node_alloc

int KDTree::create(float *setpoints, int setnpoints, int setndim,
									 void *mem)
{
	char *cmem = (char *)mem;
	((int *)mem)[0] = setnpoints;
	((int *)mem)[1] = setndim;
	for(int i=0;i<setnpoints*setndim;i++)
		((float *)mem)[i+2] = setpoints[i];
	//memcpy(cmem+sizeof(int)*2,setpoints,sizeof(float)*setnpoints*setndim);
	return
		create((float *)(cmem+sizeof(int)*2),
					 setnpoints,setndim,false,
					 (struct KDTreeNode *)
					 (cmem+sizeof(int)*2+sizeof(float)*setnpoints*setndim));
} // end of create

// This function creates a KD tree with the given
// points, array, and length
int KDTree::create(float *setpoints, int setnpoints, int setndim,
									 bool setCopy, struct KDTreeNode *setNodeMem)
{
  ndim = setndim;
  npoints = setnpoints;
  typedef int *intptr;
	
  // Copy the points from the original array, if necessary
  copyPoints = setCopy;
  if (copyPoints) {
    if(points) delete[] points;
    points = new float[ndim*npoints];
    memcpy(points,setpoints,sizeof(float)*ndim*npoints);
  }
  // If we are not copying, just set the pointer
  else
    points = setpoints;


  // Allocate some arrays;
  if (workArr)
    delete[]workArr;
  workArr = new int[npoints];

	if(!setNodeMem) {
		if(m_Root) delete[] m_Root;
		m_Root = new struct KDTreeNode[npoints*2+1];
		nodeMemAlloc = true;
	}
	else {
		m_Root = setNodeMem;
		nodeMemAlloc = false;
	}
	nodeMemCnt = 0;

	// Alocate array used for indexing
	if(intArrMem) delete[] intArrMem;
	intArrMem = 
		new int[(int)((float)(npoints+4)*
									ceil(log((double)npoints)/log(2.0)))];
	intArrMemCnt = 0;

  // Create the "sortidx" array by 
  // sorting the range tree points on each dimension
  int **sortidx = new intptr[ndim];
	if(verbosity>1)
		logmsg("KDTree: Sorting points\n");
	float imin[3];
	float imax[3];
	imin[0] = imin[1] = imin[2] = 999999.f;
	imax[0] = imax[1] = imax[2] = -999999.f;
  for (int i = 0; i < ndim; i++) {
    // Initialize the sortidx array for this
    // dimension
    sortidx[i] = new int[npoints];

    // Initialize the "tmp" array for the sort
    int *tmp = new int[npoints];
    for (int j = 0; j < npoints; j++)
      tmp[j] = j;

    // Sort the points on dimension i, putting
    // indexes in array "tmp"
    heapsort(i,tmp,npoints);

    // sortidx is actually the inverse of the 
    // index sorts
    for (int j = 0; j < npoints; j++)
	{
		sortidx[i][tmp[j]] = j;
		imin[i] = min( points[ j*3 + i ], imin[ i ] );
		imax[i] = max( points[ j*3 + i ], imax[ i ] );
	}
    delete[] tmp;
  }
	if(verbosity > 1)
		logmsg("KDTree: Done sorting points\n");

  // Create an initial list of points that references 
  // all the points
  int *pidx = new int[npoints];
  for (int i = 0; i < npoints; i++)
    pidx[i] = i;

  // Build a KD Tree
  AABB extents;
  Vec3 vmin(  imin[0] ,
			 imin[1],
			 imin[2]  );
  Vec3 vmax(  imax[0] ,
			 imax[1],
			 imax[2]  );
  OutputVector( vmin, "VMin");
  OutputVector( vmax, "VMax");
  extents.Set( vmin,vmax );
  m_Root->bounds.Set( vmin, vmax );
	//add objects to this node
	if( m_NodeCreationCallBack )
	{
		(*m_NodeCreationCallBack)( m_Root );
	}
  build_kdtree(sortidx,	// array of sort values
							 0,	// The current dimension
							 pidx, npoints, extents);	// The list of points
  // Delete the sort index
  for (int i = 0; i < ndim; i++)
    delete[]sortidx[i];
  delete[] sortidx;

  // Delete the initial list of points
  delete[] pidx;

	// Delete the sort arrays
	delete[] intArrMem;

	// delete the work array
  if(workArr) {
    delete[] workArr;
    workArr = (int *)NULL;
  }

	if(verbosity > 1)
		logmsg("KDTree: Done creating tree\n");
  return 0;
}				// end of create      


int *KDTree::int_alloc(int len)
{
	if(!intArrMem) return (int *)0;
	int *ret = intArrMem+intArrMemCnt;
	intArrMemCnt += len;
	return ret;
} // end of int_alloc


// This function build a node of the kdtree with the
// points indexed by pidx with length "len"
// sortidx is a pre-computed array using the heapsort
// algorithm above
int KDTree::build_kdtree(int **sortidx, int dim,
                                   int *pidx, int len, AABB &currentBB)
{
	static const Vec3 BufferOffset( KD_SPLIT_BUFFER_SIZE, KD_SPLIT_BUFFER_SIZE, KD_SPLIT_BUFFER_SIZE );
	AABB b1, b2;
	int ncnt = nodeMemCnt;
	struct KDTreeNode *node = node_alloc();
	node->bounds.Set( currentBB.m_Bounds[0] - BufferOffset, currentBB.m_Bounds[1] + BufferOffset );
		
	node->axis = dim;
	if (len <= m_KDCellSize &&
		m_KDCellSize == 1 ) 
	{
		node->leftIdx = -1;
		node->rightIdx = -1;
		node->pntidx = pidx[0];
		node->key = 0;
		node->bounds.Set( currentBB.m_Bounds[0] - BufferOffset, currentBB.m_Bounds[1] + BufferOffset );
		//add objects to this node
		if( m_NodeCreationCallBack )
		{
			(*m_NodeCreationCallBack)( node );
		}
		return ncnt;
	}
  
	 // If not, we must make a node
	int pivot = -1;
	int lcnt = 0, rcnt = 0;
	int *larray, *rarray;

	// Find the pivot (index of median point of available
	// points on current dimension).

	// If heapsorting the current list of points is quicker than
	// iterating through all the points, just do that instead 
	// (heapsort if of order n*log(n)
	// This test could probably use some fine tuning
	if((double)len*log((double)len) < npoints) {
		heapsort(dim,pidx,len);
		larray = pidx;
		rarray = pidx+len/2;
		pivot = pidx[len/2];
		lcnt = len/2;
		rcnt = len/2 + (len%2==0 ? 0 : 1);
	}
	else 
	{
		// Use the previously calculated sort index
		// to make this a process linear in npoints
		// This gets a little confusing, but it works.
		// Sortidx:: sortidx[dim][idx] = val 
		// idx = the index to the point
		// val = the order in the array
		int *parray = workArr;
		
		// Setting parray to -1 indicates we are not using 
		// the point
		for (int i = 0; i < npoints; i++)
			parray[i] = -1;
		// Populate "parray" with the points that we
		// are using, indexed in the order they occur
		// on the current dimension
		for (int i = 0; i < len; i++)
			parray[sortidx[dim][pidx[i]]] = pidx[i];
		int cnt = 0;
		larray = int_alloc(len/2+1);
		rarray = int_alloc(len/2+1);
		
		// The middle valid value of parray is the pivot,
		// the left go to a node on the left, the right
		// go to a node on the right.
		for (int i = 0; i < npoints; i++) {
			if (parray[i] == -1)
				continue;
			if (cnt == len / 2) {
				pivot = parray[i];
				rarray[rcnt++] = parray[i];
			} else if (cnt > len / 2)
				rarray[rcnt++] = parray[i];
			else
				larray[lcnt++] = parray[i];
			cnt++;
			if(cnt>len)
				break;
		}
	}

	if (len <= m_KDCellSize &&
		m_KDCellSize > 1 ) 
	{
		node->leftIdx = -1;
		node->rightIdx = -1;
		if( m_KDCellSize > 1 )
		{
			for( int i = 0; i < len; i++ )
			{
				node->fattyCellData.push_back( pidx[ i ] );
			}
		}
		node->pntidx = pivot;
		node->key = 0;
		node->bounds.Set( currentBB.m_Bounds[0] - BufferOffset, currentBB.m_Bounds[1] + BufferOffset );
		//add objects to this node
		if( m_NodeCreationCallBack )
		{
			(*m_NodeCreationCallBack)( node );
		}
		return ncnt;
	}

	// Create the node
	node->pntidx = -1;
	node->key = points[pivot*ndim+dim] + KEY_EPSILON;
	//calculate bounding boxes
	if( m_NodeCreationCallBack )
	{
		b1 = b2 = currentBB; //b1 is for left, b2 for right
		//split on the x axis
		int realDim = dim%ndim;
		if( realDim == 0 ){
			b1.m_Bounds[1].x = node->key; //set tmax bounds of left bb to be new split point
			b2.m_Bounds[0].x = node->key; //set tmin bounds of right bb to be new split point 
		}else if( realDim == 1 )
		{
			b1.m_Bounds[1].y = node->key; //set tmax bounds of left bb to be new split point
			b2.m_Bounds[0].y = node->key; //set tmin bounds of right bb to be new split point 
		}else if( realDim == 2 )
		{
			b1.m_Bounds[1].z = node->key; //set tmax bounds of left bb to be new split point
			b2.m_Bounds[0].z = node->key; //set tmin bounds of right bb to be new split point 
		}
	}
	// Create nodes to the left
	node->leftIdx = 
		build_kdtree(sortidx, (dim + 1) % ndim, larray, lcnt, b1);
	
	// Create nodes to the right
	node->rightIdx = 
		build_kdtree(sortidx, (dim + 1) % ndim, rarray, rcnt, b2);

  return ncnt;
}				// end of build_kdtree


// This function operates a search on a node for points within
// the specified range.
// It assumes the current node is at a depth corresponding to 
// dimension "dim"
int KDTree::range_search(int nodeIdx, Range * range, int dim)
{
  if (nodeIdx < 0 || nPntsInRange > maxPoints )
    return 0;
	
	struct KDTreeNode *node = m_Root+nodeIdx;
  // If this is a leaf node, check to see if the 
  // data is in range.  If so, operate on it.
  if (node->pntidx>=0) {
    // Return if not in range
    for (int i = 0; i < ndim; i++) {
			int idx = node->pntidx*ndim + i;
      if (points[idx] < range[i][0] || points[idx] > range[i][1])
				return 0;
    }
		pntsInRange[nPntsInRange++] = node->pntidx;
    return 0;
  }
  // Search left, if necessary
  if( rand()%2 == 0 )
  {
	if (node->key >= range[dim][0])
		range_search(node->leftIdx, range, (dim + 1) % ndim);
	// Search right,if necessary
	if (node->key <= range[dim][1])
		range_search(node->rightIdx, range, (dim + 1) % ndim);
  }else //reverse search
  {
	// Search right,if necessary
	if (node->key <= range[dim][1])
		range_search(node->rightIdx, range, (dim + 1) % ndim);
	if (node->key >= range[dim][0])
		range_search(node->leftIdx, range, (dim + 1) % ndim);
  }
  return 0;
}				// end of range_search

// This function operates a search on a node for points within
// the specified range.
// It assumes the current node is at a depth corresponding to 
// dimension "dim"
int KDTree::ClosestRangeSearch(int nodeIdx, Range * range, int dim, int &maxpoints )
{
	//Stop search if we're closing in on Max points
	if (nodeIdx < 0 ||
		nPntsInRange >= maxpoints )
	return 0;

	struct KDTreeNode *node = m_Root+nodeIdx;
	// If this is a leaf node, check to see if the 
	// data is in range.  If so, operate on it.
	if (node->pntidx>=0) 
	{
		// Return if not in range
		for (int i = 0; i < ndim; i++)
		{
			int idx = node->pntidx*ndim + i;
			if (points[idx] < range[i][0] || points[idx] > range[i][1])
					return 0;
		}
		pntsInRange[nPntsInRange++] = node->pntidx;
		return 0;
	}
	// Search left, if necessary
	if (node->key >= range[dim][0])
	range_search(node->leftIdx, range, (dim + 1) % ndim);
	// Search right,if necessary
	if (node->key <= range[dim][1])
	range_search(node->rightIdx, range, (dim + 1) % ndim);
	return 0;
}				// end of range_search

// This is the public function that will call the
// function "opFunction" on all points in the array
// within "range"
int KDTree::GetPointsInRange(Range * range)
{
	if(!pntsInRange) 
		pntsInRange = new int[npoints];
	nPntsInRange = 0;
	range_search(0, range, 0);
	return 0;
}				// end of operate_on_points

int KDTree::GetNumClosestPoints( float * pnt,
								   int &numpoints )
{
	if(!pntsInRange) 
		pntsInRange = new int[npoints];
	nPntsInRange = 0;
	GetClosestNumPoints( pnt, 0, 0, numpoints, false, 0 );
	return 0;
}	

int KDTree::GetClosestPointsInRange(Range *range, int &numpoints )
{
	if(!pntsInRange) 
		pntsInRange = new int[npoints];
	nPntsInRange = 0;

	int dim = 0;

	// First, iterate along the path to the point, 
	// and find the one associated with this point
	// on the line
	struct KDTreeNode *node = m_Root;
	int idx = -1;
	/*for (;;)
	{
		// Is this a leaf node
		if (node->pntidx >= 0) 
		{
			idx = node->pntidx;
			break;
		}
		if (node->key > pnt[dim]) 
			node = m_Root + node->leftIdx;
		else 
			node = m_Root + node->rightIdx;;
		dim = (dim + 1) % ndim;
	}*/
	// Are we getting an approximate value?
	int nodeIndex = node - m_Root;
	ClosestRangeSearch( nodeIndex, range, 0, numpoints);
	return 0;
}

int KDTree::GetClosestFattyCell(float *pnt, KDTreeNode **nodeOut )
{
	 int dim = 0;
	// First, iterate along the path to the point, 
	// and find the one associated with this point
	// on the line
		struct KDTreeNode *n = m_Root;
	for (;;) {
		// Is this a leaf node
		if (n->pntidx >= 0) {
			*nodeOut = n;
			break;
		}
		if (n->key > pnt[dim]) 
				n = m_Root + n->leftIdx;
			else 
				n = m_Root + n->rightIdx;;
		dim = (dim + 1) % ndim;
	}
	int idx = (int)(n-m_Root);
	float ndistsq = distsq( pnt, points + idx*ndim );
		
	// Search for possible other nearest neighbors
	// by examining adjoining nodes whos children may
	// be closer
	CheckNeighborFattyCells(0, 0, pnt, ndistsq, nodeOut);
	return 0;
}

int KDTree::GetClosestPoint(float *pnt, int &idx, bool approx)
{
  int dim = 0;

  // First, iterate along the path to the point, 
  // and find the one associated with this point
  // on the line
	struct KDTreeNode *n = m_Root;
  idx = -1;
  for (;;) {
    // Is this a leaf node
    if (n->pntidx >= 0) {
      idx = n->pntidx;
      break;
    }
    if (n->key > pnt[dim]) 
			n = m_Root + n->leftIdx;
		else 
				n = m_Root + n->rightIdx;;
    dim = (dim + 1) % ndim;
  }
  // Are we getting an approximate value?
  if(approx == true) return 0;

  float ndistsq = distsq(pnt,points+idx*ndim);
	
  // Search for possible other nearest neighbors
  // by examining adjoining nodes whos children may
  // be closer
  check_border_distance(0, 0, pnt, ndistsq, idx);

  return 0;
}				// end of GetClosestPoint


int KDTree::GetClosestPoints(float *pnt, multimap< float, int > &sortedPoints, float &distancesquared )
{  int dim = 0;

  // First, iterate along the path to the point, 
  // and find the one associated with this point
  // on the line
	struct KDTreeNode *n = m_Root;
  int idx = -1;
  for (;;) {
    // Is this a leaf node
    if (n->pntidx >= 0) {
      idx = n->pntidx;
      break;
    }
    if (n->key > pnt[dim]) 
			n = m_Root + n->leftIdx;
		else 
			n = m_Root + n->rightIdx;;
    dim = (dim + 1) % ndim;
  }

  float ndistsq = distsq(pnt,points+idx*ndim);
  sortedPoints.insert( pair< float, int >( ndistsq, idx ) );
  // Search for possible other nearest neighbors
  // by examining adjoining nodes whos children may
  // be closer
  check_border_distance_sort(0, 0, pnt,  distancesquared, sortedPoints);

  return 0;
}			


int KDTree::check_border_distance_sort(int nodeIdx, int dim,
				  float *pnt, float &cdistsq, multimap< float, int > &sortedList )
{
	if(nodeIdx < 0) return 0;

  // Are we at a closer leaf node?  
  // If so, check the distance
	struct KDTreeNode *node = m_Root+nodeIdx;
  if (node->pntidx >= 0) {
    float dsq = distsq(pnt, points+node->pntidx*ndim);
    if (dsq < cdistsq) {
	  sortedList.insert( pair< float, int >( dsq, node->pntidx ) );
    }
    return 0;
  }

  // The distance squared along the current dimension between the
  // point and the key
  float ndistsq = 
    (node->key - pnt[dim])*(node->key - pnt[dim]);

  // If the distance squared from the key to the current value is 
  // greater than the nearest distance, we need only look
  // in one direction.
  if (ndistsq > cdistsq) {
    if (node->key > pnt[dim])
      check_border_distance_sort(node->leftIdx, (dim + 1) % ndim, pnt, cdistsq,  sortedList);
    else
      check_border_distance_sort(node->rightIdx, (dim + 1) % ndim, pnt, cdistsq,  sortedList);
  }
  // If the distance from the key to the current value is 
  // less than the nearest distance, we still need to look
  // in both directions.
  else {
    check_border_distance_sort(node->leftIdx, (dim + 1) % ndim, pnt, cdistsq, sortedList);
    check_border_distance_sort(node->rightIdx, (dim + 1) % ndim, pnt, cdistsq,  sortedList);
  }
  return 0;
} // end of check_border_distance

int KDTree::CheckNeighborFattyCells( int nodeIdx, int dim,
				  float *pnt, float &cdistsq, KDTreeNode ** nodeOut )
{
	if(!nodeOut ||
		!(*nodeOut) )
		return 0;

	// Are we at a closer leaf node?  
	// If so, check the distance
	struct KDTreeNode *node = *nodeOut;
	if (node->pntidx >= 0) {
		float dsq = distsq(pnt, points+node->pntidx*ndim);
			if (dsq < cdistsq) {
			cdistsq = dsq;
			*nodeOut = node;
		}
		return 0;
	}

	// The distance squared along the current dimension between the
	// point and the key
	float ndistsq = 
		(node->key - pnt[dim])*(node->key - pnt[dim]);

	// If the distance squared from the key to the current value is 
	// greater than the nearest distance, we need only look
	// in one direction.
	if (ndistsq > cdistsq) {
		if (node->key > pnt[dim])
		CheckNeighborFattyCells(node->leftIdx, (dim + 1) % ndim, pnt, cdistsq, &node);
		else
		CheckNeighborFattyCells(node->rightIdx, (dim + 1) % ndim, pnt, cdistsq, &node);
	}
	// If the distance from the key to the current value is 
	// less than the nearest distance, we still need to look
	// in both directions.
	else {
		CheckNeighborFattyCells(node->leftIdx, (dim + 1) % ndim, pnt, cdistsq, &node);
		CheckNeighborFattyCells(node->rightIdx, (dim + 1) % ndim, pnt, cdistsq, &node);
	}
	return 0;

}
int KDTree::check_border_distance(int nodeIdx, int dim,
				  float *pnt, float &cdistsq, int &idx)
{
	if(nodeIdx < 0) return 0;

  // Are we at a closer leaf node?  
  // If so, check the distance
	struct KDTreeNode *node = m_Root+nodeIdx;
  if (node->pntidx >= 0) {
    float dsq = distsq(pnt, points+node->pntidx*ndim);
    if (dsq < cdistsq) {
      cdistsq = dsq;
      idx = node->pntidx;
    }
    return 0;
  }

  // The distance squared along the current dimension between the
  // point and the key
  float ndistsq = 
    (node->key - pnt[dim])*(node->key - pnt[dim]);

  // If the distance squared from the key to the current value is 
  // greater than the nearest distance, we need only look
  // in one direction.
  if (ndistsq > cdistsq) {
    if (node->key > pnt[dim])
      check_border_distance(node->leftIdx, (dim + 1) % ndim, pnt, cdistsq, idx);
    else
      check_border_distance(node->rightIdx, (dim + 1) % ndim, pnt, cdistsq, idx);
  }
  // If the distance from the key to the current value is 
  // less than the nearest distance, we still need to look
  // in both directions.
  else {
    check_border_distance(node->leftIdx, (dim + 1) % ndim, pnt, cdistsq, idx);
    check_border_distance(node->rightIdx, (dim + 1) % ndim, pnt, cdistsq, idx);
  }
  return 0;
} // end of check_border_distance


inline float KDTree::distsq(float *pnt1, float *pnt2)
{
  float d = 0.0;
  for (int i = 0; i < ndim; i++)
    d += (pnt1[i] - pnt2[i]) * (pnt1[i] - pnt2[i]);
  return d;
}				// end if distsq

void VecToFloatArray( Vec3& v, float array[] )
{
	array[0] = v.x;
	array[1] = v.y;
	array[2] = v.z;
}

int KDTree::GetClosestNumPoints( float * pnt,
								   int nodeIdx,
								   int dim,
								   int &numpoints,
								   bool grab,
								   int depth )
{
	struct KDTreeNode *node = m_Root+nodeIdx;
	//figure out based on depthz
	// Is this a leaf node
    if (node->pntidx >= 0)
	{
		pntsInRange[ nPntsInRange++ ] = node->pntidx;
		return 0;//this is the node index
    }
    if( !grab )
	{
		bool right = false;
		if (node->key > pnt[dim]) 
		{
			GetClosestNumPoints( pnt, node->leftIdx, (dim + 1) % ndim, numpoints, false, depth + 1 );
		}
		else 
		{
			right = true;
			GetClosestNumPoints( pnt, node->rightIdx, (dim + 1) % ndim, numpoints, false, depth + 1 );
		}
		if( right )
		{
			//go left now
			GetClosestNumPoints( pnt, node->leftIdx, (dim + 1)%ndim, numpoints, true, depth + 1 );
		}else
		{
			//go right
			GetClosestNumPoints( pnt, node->rightIdx, (dim + 1)%ndim, numpoints, true, depth + 1 );
		}
	}else //grabbing just goes down and grabs all leaves below, if grabbing, means we never recursed these nodes
	{
		if ( nPntsInRange < numpoints &&
			node->key > pnt[dim]) 
		{
			//left is closer, grab left first
			GetClosestNumPoints( pnt, node->leftIdx, (dim + 1)%ndim, numpoints, true, depth + 1 );
		}
		else if( nPntsInRange < numpoints )
		{
			GetClosestNumPoints( pnt, node->rightIdx, (dim + 1)%ndim, numpoints, true, depth + 1 );
		}
	}
	return 0;
}

vector< ADDAXISALIGNEDBOXPARAMS > g_Boxes;
void DrawBox( Vec3 &min, Vec3 &max, Matrix4x4 * temp )
{	
	static CHashString name( _T("none"));
	ADDAXISALIGNEDBOXPARAMS boxrender;
	boxrender.name = &name;
	boxrender.min = (*temp)*min;
	boxrender.max = (*temp)*max;
	
	boxrender.alpha = 255;
	boxrender.green = 0;
	boxrender.blue = 0;
	boxrender.red = 255;
	g_Boxes.push_back( boxrender );
	
}

struct RayFindStruct
{
	KDTreeNode * node;
	float tmin;
	float tmax;
	RayFindStruct( KDTreeNode * n, float tn, float tm )
	{
		node = n;
		tmin = tn;
		tmax = tm;
	}
};

// -----------------------------------------------------------
// Find nearest ray 2
// -----------------------------------------------------------
int KDTree::FindNearestToRay2( Ray& vRay, Vec3 &tmin, Vec3 &tmax, RayIntersectCallback callback, Matrix4x4 * temp )
{
	static bool bOutput = false;
	float tnear = 0;
	float tfar = ( tmax - tmin ).Length();
	int retval = 0;
	float D[3];
	float O[3];
	VecToFloatArray( vRay.m_Direction, D );
	VecToFloatArray( vRay.m_Origin, O );
	// We assume ray fits in extents
	// clip ray segment to box
	static AABB bbox;
	static int aa = 0;
	bbox.Set( tmin, tmax );
	if( !bbox.IntersectRay( vRay, tnear, tfar ) )
		return 0;
	if( tnear < 0 || 
		tfar < 0 || 
		tnear > tfar )
		return 0;
	stack< RayFindStruct > nodeStack;
	vector< KDTreeNode * > nodesToCheck;
	g_Boxes.clear();
	nodeStack.push( RayFindStruct( m_Root, tnear, tfar ) );
	while( nodeStack.size() > 0 )
	{
		RayFindStruct curSearch = nodeStack.top();
		nodeStack.pop();
		DrawBox( curSearch.node->bounds.m_Bounds[0], curSearch.node->bounds.m_Bounds[1], temp );
		
		while( curSearch.node->pntidx < 0 )//is not a leaf
		{
			int dim = curSearch.node->axis;
			float tSplit = (curSearch.node->key - O[dim]) / D[dim];
			KDTreeNode * first = m_Root + curSearch.node->leftIdx;
			KDTreeNode * second = m_Root + curSearch.node->rightIdx;
			//check dimension
			if( D[dim] < 0 )
			{
				//swap
				KDTreeNode * temp;
				temp = first;
				first = second;
				second = temp;
			}
			if( tSplit >= curSearch.tmax 
				//|| tSplit < 0 
				)
			{
				curSearch.node = first;
			}
			else 
				if( tSplit <= curSearch.tmin )
			{
				curSearch.node = second;
			}
			else
			{
				nodeStack.push( RayFindStruct( second, tSplit, curSearch.tmax ) );
				curSearch.node = first;
				curSearch.tmax = tSplit;
			}
			DrawBox( curSearch.node->bounds.m_Bounds[0], curSearch.node->bounds.m_Bounds[1], temp );
		}
		assert( curSearch.node != NULL );
		//check triangles
		nodesToCheck.push_back( curSearch.node );
		bool bresult = (*callback)( nodesToCheck );
		if( bresult )
		{
			return 1;
		}
		nodesToCheck.clear();
	}
	return 0;
	if( tnear == tfar )
	{
		return 0;
	}
	nodesToCheck.clear();
	nodesToCheck.push_back( m_Root );
	bool bresult = (*callback)( nodesToCheck );
	if( bresult )
	{
		if( g_Boxes.size() > 0 && aa < 5 
			&&
			vRay.m_Origin.z > 1000 )
		{
			OutputDebugString("RAYDEBUG START----------------------\n");
			for( int i = 0; i < (int)g_Boxes.size(); i++ )
			{
				static DWORD msgHash_AddAxisAlignedBox = CHashString(_T("AddAxisAlignedBox")).GetUniqueID();
				EngineGetToolBox()->SendMessage(msgHash_AddAxisAlignedBox,sizeof(g_Boxes[i]), &g_Boxes[i] );
				OutputDebugString("Box:\t");
				OutputVector(	g_Boxes[i].min, "Min" );
				OutputVector(	g_Boxes[i].max, "Max" );
			}
			g_Boxes.clear();
			aa++;
			static CHashString h(_T("none"));
			ADDLINEPARAMS LineParam;
			LineParam.name = &h;
			LineParam.start = (*temp)*vRay.m_Origin;
			LineParam.end =  vRay.m_Origin + vRay.m_Direction*10000;
			LineParam.end = (*temp)*LineParam.end;
			LineParam.red = 0;
			LineParam.blue = 0;
			LineParam.green = 255;
			static DWORD msgHash_AddLine = CHashString(_T("AddLine")).GetUniqueID();
			EngineGetToolBox()->SendMessage(msgHash_AddLine,sizeof(LineParam), &LineParam );
		}
        return 1;
	}
	return 0;
}

// -----------------------------------------------------------
// Engine::FindNearest
// Finds the nearest intersection in a KdTree for a ray
// -----------------------------------------------------------
int KDTree::FindNearestToRay( Ray& vRay, Vec3 &tmin, Vec3 &tmax, RayIntersectCallback callback, Matrix4x4 * temp )
{
	static bool bOutput = false;
	float tnear = 0;
	float tfar = ( tmax - tmin ).Length();
	float t;
	int retval = 0;
	float D[3];
	float O[3];
	VecToFloatArray( vRay.m_Direction, D );
	VecToFloatArray( vRay.m_Origin, O );
	// We assume ray fits in extents
	// clip ray segment to box
	static AABB bbox;
	bbox.Set( tmin, tmax );
	if( !bbox.IntersectRay( vRay, tnear, tfar ) )
		return 0;
	if( tnear < 0 || 
		tfar < 0 || 
		tnear > tfar )
		return 0;
	// init stack
	int entrypoint = 0, exitpoint = 1;
	// init traversal
	KDTreeNode* farchild, *currnode;
	farchild = 0;
	currnode = m_Root;
	m_Stack[entrypoint].t = tnear;
	if (tnear > 0.0f) 
	{
		m_Stack[entrypoint].pb[0] = O[0] + D[0]*tnear;
		m_Stack[entrypoint].pb[1] = O[1] + D[1]*tnear;
		m_Stack[entrypoint].pb[2] = O[2] + D[2]*tnear;
	}
	else
	{
		m_Stack[entrypoint].pb[0] = O[0];
		m_Stack[entrypoint].pb[1] = O[1];
		m_Stack[entrypoint].pb[2] = O[2];
	}
	m_Stack[exitpoint].t = tfar;
	m_Stack[exitpoint].pb[0] = O[0] + D[0]*tfar;
	m_Stack[exitpoint].pb[1] = O[1] + D[1]*tfar;
	m_Stack[exitpoint].pb[2]= O[2] + D[2]*tfar;
	m_Stack[exitpoint].node = 0;
	int dim = -1;
	// traverse kd-tree
	char buf[1024];

	static int aa = 0;
	if( bOutput )
	{
		sprintf( buf, "tfar: %f, tnear: %f\n", tfar, tnear );
		OutputDebugString( buf );
		OutputVector( vRay.m_Origin, "Ray origin" );
		OutputVector( vRay.m_Direction, "Ray direction" );
		OutputVector( O, "Ray origin2" );
		OutputVector( D, "Ray direction2" );
		OutputVector( m_Stack[ entrypoint ].pb, "entry" );
		sprintf( buf, "exit far: %f\n", m_Stack[exitpoint].t );
		OutputDebugString( buf );
		OutputVector( m_Stack[ exitpoint ].pb, "exit" );
	}
	KDTreeNode * lastNode = 0;
	g_Boxes.clear();
	vector< KDTreeNode * > nodesToCheck;
	while(currnode)
	{
		if( aa < 1 )
			DrawBox( currnode->bounds.m_Bounds[0], currnode->bounds.m_Bounds[1], temp );
		//Is not a leaf?
		while( currnode->pntidx < 0 )
		{		
			lastNode = currnode;
			dim = (dim + 1) % ndim;
			if( m_Stack[entrypoint].pb[dim] <= currnode->key )
			{
				if( m_Stack[exitpoint].pb[dim] <= currnode->key )
				{
					currnode = m_Root + currnode->leftIdx;
					continue;
				}
				farchild = m_Root + currnode->rightIdx; // GetRight();
				currnode = m_Root + currnode->leftIdx;
			}
			else 
			{
				if (m_Stack[exitpoint].pb[dim] > currnode->key)
				{
					currnode = m_Root + currnode->rightIdx;
					continue;
				}
				farchild = m_Root + currnode->leftIdx;
				currnode = m_Root + currnode->rightIdx; // GetRight();
			}
			
			t = (currnode->key - O[dim]) / D[dim];
			int tmp = exitpoint++;
			if (exitpoint == entrypoint) 
				exitpoint++;
			m_Stack[exitpoint].prev = tmp;
			m_Stack[exitpoint].t = t;
			m_Stack[exitpoint].node = farchild;
			m_Stack[exitpoint].pb[dim] = currnode->key;
			int nextaxis = (dim + 1) % ndim;
			int prevaxis = (dim + 2) % ndim;
			m_Stack[exitpoint].pb[nextaxis] = O[nextaxis] + t * D[nextaxis];
			m_Stack[exitpoint].pb[prevaxis] = O[prevaxis] + t * D[prevaxis];
		}
		if( aa < 1 )
				DrawBox( currnode->bounds.m_Bounds[0], currnode->bounds.m_Bounds[1], temp );			
			
		float dist = m_Stack[exitpoint].t;
		nodesToCheck.push_back( currnode );
		entrypoint = exitpoint;
		currnode = m_Stack[exitpoint].node;
		exitpoint = m_Stack[entrypoint].prev;
	}
	bool bfound = (*callback)( nodesToCheck );
	if( bfound )
	{
		return 1;
	}
//	return 0;
	//check by comparing all nodes
	nodesToCheck.clear();
	nodesToCheck.push_back( m_Root );
	bool bresult = (*callback)( nodesToCheck );
	if( bresult )
	{
		if( g_Boxes.size() > 0 && aa < 1 )
		{
			for( int i = 0; i < (int)g_Boxes.size(); i++ )
			{
				static DWORD msgHash_AddAxisAlignedBox = CHashString(_T("AddAxisAlignedBox")).GetUniqueID();
				EngineGetToolBox()->SendMessage(msgHash_AddAxisAlignedBox,sizeof(g_Boxes[i]), &g_Boxes[i] );
				OutputDebugString("Box:\t");
				OutputVector(	g_Boxes[i].min, "Min" );
				OutputVector(	g_Boxes[i].max, "Max" );
			}
			g_Boxes.clear();
			aa++;
			static CHashString h(_T("none"));
			ADDLINEPARAMS LineParam;
			LineParam.name = &h;
			LineParam.start = (*temp)*vRay.m_Origin;
			LineParam.end =  vRay.m_Origin + vRay.m_Direction*10000;
			LineParam.end = (*temp)*LineParam.end;
			LineParam.red = 0;
			LineParam.blue = 0;
			LineParam.green = 255;
			static DWORD msgHash_AddLine = CHashString(_T("AddLine")).GetUniqueID();
			EngineGetToolBox()->SendMessage(msgHash_AddLine,sizeof(LineParam), &LineParam );

		}
        return 1;
	}
	return 0;
}