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

#include <math.h>
#include "stdafx.h"
#include "memory.h"
#include "random.h"

#define photonalloc malloc
// Precomputed conversion tables for the sin/cosine
const float		costheta[]	=	{	1.000000f,0.999925f,0.999699f,0.999322f,0.998795f,0.998118f,0.997290f,0.996313f,0.995185f,0.993907f,0.992480f,0.990903f,0.989177f,0.987301f,0.985278f,0.983105f,0.980785f,0.978317f,0.975702f,0.972940f,0.970031f,0.966976f,0.963776f,0.960431f,0.956940f,0.953306f,0.949528f,0.945607f,0.941544f,0.937339f,0.932993f,0.928506f,0.923880f,0.919114f,0.914210f,0.909168f,0.903989f,0.898674f,0.893224f,0.887640f,0.881921f,0.876070f,0.870087f,0.863973f,0.857729f,0.851355f,0.844854f,0.838225f,0.831470f,0.824589f,0.817585f,0.810457f,0.803208f,0.795837f,0.788346f,0.780737f,0.773010f,0.765167f,0.757209f,0.749136f,0.740951f,0.732654f,0.724247f,0.715731f,0.707107f,0.698376f,0.689541f,0.680601f,0.671559f,0.662416f,0.653173f,0.643832f,0.634393f,0.624859f,0.615232f,0.605511f,0.595699f,0.585798f,0.575808f,0.565732f,0.555570f,0.545325f,0.534998f,0.524590f,0.514103f,0.503538f,0.492898f,0.482184f,0.471397f,0.460539f,0.449611f,0.438616f,0.427555f,0.416430f,0.405241f,0.393992f,0.382683f,0.371317f,0.359895f,0.348419f,0.336890f,0.325310f,0.313682f,0.302006f,0.290285f,0.278520f,0.266713f,0.254866f,0.242980f,0.231058f,0.219101f,0.207111f,0.195090f,0.183040f,0.170962f,0.158858f,0.146730f,0.134581f,0.122411f,0.110222f,0.098017f,0.085797f,0.073565f,0.061321f,0.049068f,0.036807f,0.024541f,0.012272f,0.000000f,-0.012272f,-0.024541f,-0.036807f,-0.049068f,-0.061321f,-0.073565f,-0.085797f,-0.098017f,-0.110222f,-0.122411f,-0.134581f,-0.146730f,-0.158858f,-0.170962f,-0.183040f,-0.195090f,-0.207111f,-0.219101f,-0.231058f,-0.242980f,-0.254866f,-0.266713f,-0.278520f,-0.290285f,-0.302006f,-0.313682f,-0.325310f,-0.336890f,-0.348419f,-0.359895f,-0.371317f,-0.382683f,-0.393992f,-0.405241f,-0.416430f,-0.427555f,-0.438616f,-0.449611f,-0.460539f,-0.471397f,-0.482184f,-0.492898f,-0.503538f,-0.514103f,-0.524590f,-0.534998f,-0.545325f,-0.555570f,-0.565732f,-0.575808f,-0.585798f,-0.595699f,-0.605511f,-0.615232f,-0.624859f,-0.634393f,-0.643832f,-0.653173f,-0.662416f,-0.671559f,-0.680601f,-0.689541f,-0.698376f,-0.707107f,-0.715731f,-0.724247f,-0.732654f,-0.740951f,-0.749136f,-0.757209f,-0.765167f,-0.773010f,-0.780737f,-0.788346f,-0.795837f,-0.803208f,-0.810457f,-0.817585f,-0.824589f,-0.831470f,-0.838225f,-0.844854f,-0.851355f,-0.857729f,-0.863973f,-0.870087f,-0.876070f,-0.881921f,-0.887640f,-0.893224f,-0.898674f,-0.903989f,-0.909168f,-0.914210f,-0.919114f,-0.923880f,-0.928506f,-0.932993f,-0.937339f,-0.941544f,-0.945607f,-0.949528f,-0.953306f,-0.956940f,-0.960431f,-0.963776f,-0.966976f,-0.970031f,-0.972940f,-0.975702f,-0.978317f,-0.980785f,-0.983105f,-0.985278f,-0.987301f,-0.989177f,-0.990903f,-0.992480f,-0.993907f,-0.995185f,-0.996313f,-0.997290f,-0.998118f,-0.998795f,-0.999322f,-0.999699f,-0.999925f	};
const float		sintheta[]	=	{	0.000000f,0.012272f,0.024541f,0.036807f,0.049068f,0.061321f,0.073565f,0.085797f,0.098017f,0.110222f,0.122411f,0.134581f,0.146730f,0.158858f,0.170962f,0.183040f,0.195090f,0.207111f,0.219101f,0.231058f,0.242980f,0.254866f,0.266713f,0.278520f,0.290285f,0.302006f,0.313682f,0.325310f,0.336890f,0.348419f,0.359895f,0.371317f,0.382683f,0.393992f,0.405241f,0.416430f,0.427555f,0.438616f,0.449611f,0.460539f,0.471397f,0.482184f,0.492898f,0.503538f,0.514103f,0.524590f,0.534998f,0.545325f,0.555570f,0.565732f,0.575808f,0.585798f,0.595699f,0.605511f,0.615232f,0.624859f,0.634393f,0.643832f,0.653173f,0.662416f,0.671559f,0.680601f,0.689541f,0.698376f,0.707107f,0.715731f,0.724247f,0.732654f,0.740951f,0.749136f,0.757209f,0.765167f,0.773010f,0.780737f,0.788346f,0.795837f,0.803208f,0.810457f,0.817585f,0.824589f,0.831470f,0.838225f,0.844854f,0.851355f,0.857729f,0.863973f,0.870087f,0.876070f,0.881921f,0.887640f,0.893224f,0.898674f,0.903989f,0.909168f,0.914210f,0.919114f,0.923880f,0.928506f,0.932993f,0.937339f,0.941544f,0.945607f,0.949528f,0.953306f,0.956940f,0.960431f,0.963776f,0.966976f,0.970031f,0.972940f,0.975702f,0.978317f,0.980785f,0.983105f,0.985278f,0.987301f,0.989177f,0.990903f,0.992480f,0.993907f,0.995185f,0.996313f,0.997290f,0.998118f,0.998795f,0.999322f,0.999699f,0.999925f,1.000000f,0.999925f,0.999699f,0.999322f,0.998795f,0.998118f,0.997290f,0.996313f,0.995185f,0.993907f,0.992480f,0.990903f,0.989177f,0.987301f,0.985278f,0.983105f,0.980785f,0.978317f,0.975702f,0.972940f,0.970031f,0.966976f,0.963776f,0.960431f,0.956940f,0.953306f,0.949528f,0.945607f,0.941544f,0.937339f,0.932993f,0.928506f,0.923880f,0.919114f,0.914210f,0.909168f,0.903989f,0.898674f,0.893224f,0.887640f,0.881921f,0.876070f,0.870087f,0.863973f,0.857729f,0.851355f,0.844854f,0.838225f,0.831470f,0.824589f,0.817585f,0.810457f,0.803208f,0.795837f,0.788346f,0.780737f,0.773010f,0.765167f,0.757209f,0.749136f,0.740951f,0.732654f,0.724247f,0.715731f,0.707107f,0.698376f,0.689541f,0.680601f,0.671559f,0.662416f,0.653173f,0.643832f,0.634393f,0.624859f,0.615232f,0.605511f,0.595699f,0.585798f,0.575808f,0.565732f,0.555570f,0.545325f,0.534998f,0.524590f,0.514103f,0.503538f,0.492898f,0.482184f,0.471397f,0.460539f,0.449611f,0.438616f,0.427555f,0.416430f,0.405241f,0.393992f,0.382683f,0.371317f,0.359895f,0.348419f,0.336890f,0.325310f,0.313682f,0.302006f,0.290285f,0.278520f,0.266713f,0.254866f,0.242980f,0.231058f,0.219101f,0.207111f,0.195090f,0.183040f,0.170962f,0.158858f,0.146730f,0.134581f,0.122411f,0.110222f,0.098017f,0.085797f,0.073565f,0.061321f,0.049068f,0.036807f,0.024541f,0.012272f	};
const float		cosphi[]	=	{	1.000000f,0.999699f,0.998795f,0.997290f,0.995185f,0.992480f,0.989177f,0.985278f,0.980785f,0.975702f,0.970031f,0.963776f,0.956940f,0.949528f,0.941544f,0.932993f,0.923880f,0.914210f,0.903989f,0.893224f,0.881921f,0.870087f,0.857729f,0.844854f,0.831470f,0.817585f,0.803208f,0.788346f,0.773010f,0.757209f,0.740951f,0.724247f,0.707107f,0.689541f,0.671559f,0.653173f,0.634393f,0.615232f,0.595699f,0.575808f,0.555570f,0.534998f,0.514103f,0.492898f,0.471397f,0.449611f,0.427555f,0.405241f,0.382683f,0.359895f,0.336890f,0.313682f,0.290285f,0.266713f,0.242980f,0.219101f,0.195090f,0.170962f,0.146730f,0.122411f,0.098017f,0.073565f,0.049068f,0.024541f,0.000000f,-0.024541f,-0.049068f,-0.073565f,-0.098017f,-0.122411f,-0.146730f,-0.170962f,-0.195090f,-0.219101f,-0.242980f,-0.266713f,-0.290285f,-0.313682f,-0.336890f,-0.359895f,-0.382683f,-0.405241f,-0.427555f,-0.449611f,-0.471397f,-0.492898f,-0.514103f,-0.534998f,-0.555570f,-0.575808f,-0.595699f,-0.615232f,-0.634393f,-0.653173f,-0.671559f,-0.689541f,-0.707107f,-0.724247f,-0.740951f,-0.757209f,-0.773010f,-0.788346f,-0.803208f,-0.817585f,-0.831470f,-0.844854f,-0.857729f,-0.870087f,-0.881921f,-0.893224f,-0.903989f,-0.914210f,-0.923880f,-0.932993f,-0.941544f,-0.949528f,-0.956940f,-0.963776f,-0.970031f,-0.975702f,-0.980785f,-0.985278f,-0.989177f,-0.992480f,-0.995185f,-0.997290f,-0.998795f,-0.999699f,-1.000000f,-0.999699f,-0.998795f,-0.997290f,-0.995185f,-0.992480f,-0.989177f,-0.985278f,-0.980785f,-0.975702f,-0.970031f,-0.963776f,-0.956940f,-0.949528f,-0.941544f,-0.932993f,-0.923880f,-0.914210f,-0.903989f,-0.893224f,-0.881921f,-0.870087f,-0.857729f,-0.844854f,-0.831470f,-0.817585f,-0.803208f,-0.788346f,-0.773010f,-0.757209f,-0.740951f,-0.724247f,-0.707107f,-0.689541f,-0.671559f,-0.653173f,-0.634393f,-0.615232f,-0.595699f,-0.575808f,-0.555570f,-0.534998f,-0.514103f,-0.492898f,-0.471397f,-0.449611f,-0.427555f,-0.405241f,-0.382683f,-0.359895f,-0.336890f,-0.313682f,-0.290285f,-0.266713f,-0.242980f,-0.219101f,-0.195090f,-0.170962f,-0.146730f,-0.122411f,-0.098017f,-0.073565f,-0.049068f,-0.024541f,-0.000000f,0.024541f,0.049068f,0.073565f,0.098017f,0.122411f,0.146730f,0.170962f,0.195090f,0.219101f,0.242980f,0.266713f,0.290285f,0.313682f,0.336890f,0.359895f,0.382683f,0.405241f,0.427555f,0.449611f,0.471397f,0.492898f,0.514103f,0.534998f,0.555570f,0.575808f,0.595699f,0.615232f,0.634393f,0.653173f,0.671559f,0.689541f,0.707107f,0.724247f,0.740951f,0.757209f,0.773010f,0.788346f,0.803208f,0.817585f,0.831470f,0.844854f,0.857729f,0.870087f,0.881921f,0.893224f,0.903989f,0.914210f,0.923880f,0.932993f,0.941544f,0.949528f,0.956940f,0.963776f,0.970031f,0.975702f,0.980785f,0.985278f,0.989177f,0.992480f,0.995185f,0.997290f,0.998795f,0.999699f	};
const float		sinphi[]	=	{	0.000000f,0.024541f,0.049068f,0.073565f,0.098017f,0.122411f,0.146730f,0.170962f,0.195090f,0.219101f,0.242980f,0.266713f,0.290285f,0.313682f,0.336890f,0.359895f,0.382683f,0.405241f,0.427555f,0.449611f,0.471397f,0.492898f,0.514103f,0.534998f,0.555570f,0.575808f,0.595699f,0.615232f,0.634393f,0.653173f,0.671559f,0.689541f,0.707107f,0.724247f,0.740951f,0.757209f,0.773010f,0.788346f,0.803208f,0.817585f,0.831470f,0.844854f,0.857729f,0.870087f,0.881921f,0.893224f,0.903989f,0.914210f,0.923880f,0.932993f,0.941544f,0.949528f,0.956940f,0.963776f,0.970031f,0.975702f,0.980785f,0.985278f,0.989177f,0.992480f,0.995185f,0.997290f,0.998795f,0.999699f,1.000000f,0.999699f,0.998795f,0.997290f,0.995185f,0.992480f,0.989177f,0.985278f,0.980785f,0.975702f,0.970031f,0.963776f,0.956940f,0.949528f,0.941544f,0.932993f,0.923880f,0.914210f,0.903989f,0.893224f,0.881921f,0.870087f,0.857729f,0.844854f,0.831470f,0.817585f,0.803208f,0.788346f,0.773010f,0.757209f,0.740951f,0.724247f,0.707107f,0.689541f,0.671559f,0.653173f,0.634393f,0.615232f,0.595699f,0.575808f,0.555570f,0.534998f,0.514103f,0.492898f,0.471397f,0.449611f,0.427555f,0.405241f,0.382683f,0.359895f,0.336890f,0.313682f,0.290285f,0.266713f,0.242980f,0.219101f,0.195090f,0.170962f,0.146730f,0.122411f,0.098017f,0.073565f,0.049068f,0.024541f,0.000000f,-0.024541f,-0.049068f,-0.073565f,-0.098017f,-0.122411f,-0.146730f,-0.170962f,-0.195090f,-0.219101f,-0.242980f,-0.266713f,-0.290285f,-0.313682f,-0.336890f,-0.359895f,-0.382683f,-0.405241f,-0.427555f,-0.449611f,-0.471397f,-0.492898f,-0.514103f,-0.534998f,-0.555570f,-0.575808f,-0.595699f,-0.615232f,-0.634393f,-0.653173f,-0.671559f,-0.689541f,-0.707107f,-0.724247f,-0.740951f,-0.757209f,-0.773010f,-0.788346f,-0.803208f,-0.817585f,-0.831470f,-0.844854f,-0.857729f,-0.870087f,-0.881921f,-0.893224f,-0.903989f,-0.914210f,-0.923880f,-0.932993f,-0.941544f,-0.949528f,-0.956940f,-0.963776f,-0.970031f,-0.975702f,-0.980785f,-0.985278f,-0.989177f,-0.992480f,-0.995185f,-0.997290f,-0.998795f,-0.999699f,-1.000000f,-0.999699f,-0.998795f,-0.997290f,-0.995185f,-0.992480f,-0.989177f,-0.985278f,-0.980785f,-0.975702f,-0.970031f,-0.963776f,-0.956940f,-0.949528f,-0.941544f,-0.932993f,-0.923880f,-0.914210f,-0.903989f,-0.893224f,-0.881921f,-0.870087f,-0.857729f,-0.844854f,-0.831470f,-0.817585f,-0.803208f,-0.788346f,-0.773010f,-0.757209f,-0.740951f,-0.724247f,-0.707107f,-0.689541f,-0.671559f,-0.653173f,-0.634393f,-0.615232f,-0.595699f,-0.575808f,-0.555570f,-0.534998f,-0.514103f,-0.492898f,-0.471397f,-0.449611f,-0.427555f,-0.405241f,-0.382683f,-0.359895f,-0.336890f,-0.313682f,-0.290285f,-0.266713f,-0.242980f,-0.219101f,-0.195090f,-0.170962f,-0.146730f,-0.122411f,-0.098017f,-0.073565f,-0.049068f,-0.024541f	};

#define	PHOTON_CAUSTICS		1
#define	PHOTON_EXPLOSION	2
#define	PHOTON_POINT		4
#define	PHOTON_DISTANT		8
#define	PHOTON_AREA			16

///////////////////////////////////////////////////////////////////////
// Class				:	CPhotonMap
// Method				:	CPhotonMap
// Description			:	Ctor
// Return Value			:
// Comments				:
CPhotonMap::CPhotonMap() : CMapTemplate<CPhoton>(){

	#ifdef PHOTON_LOOKUP_CACHE
		root			=	NULL;
		maxDepth		=	1;
	#endif
	refCount		=	1;	// Count the fileResource reference
	modifying		=	FALSE;
	maxPower		=	0;

	from.SetIdentity();
	to.SetIdentity();

	// Make sure we have a root
	balance();
}

// Expand the box (bmin,bmax) so that point v is inside it
inline	void	addBox(Vec3 &bmin, Vec3 &bmax,const Vec3 &v) {
	int	i;

	for (i=0;i<3;i++) {
		if (v[i] < bmin[i])	bmin[i]	=	v[i];
		if (v[i] > bmax[i]) bmax[i] =	v[i];
	}
}


///////////////////////////////////////////////////////////////////////
// Class				:	CPhotonMap
// Method				:	~CPhotonMap
// Description			:	Dtor
// Return Value			:
// Comments				:
CPhotonMap::~CPhotonMap() {
	#ifdef PHOTON_LOOKUP_CACHE
		if (root != NULL) {
			CPhotonNode		**stackBase	=	(CPhotonNode **)	photonalloc(maxDepth*sizeof(CPhotonNode *)*8);
			CPhotonNode		**stack;
			CPhotonNode		*cNode;
			CPhotonSample	*cSample;
			int			i;
	
			stack		=	stackBase;
			*stack++	=	root;
			while(stack > stackBase) {
				cNode	=	*(--stack);
	
				while((cSample=cNode->samples) != NULL) {
					cNode->samples	=	cSample->next;
					delete cSample;
				}
	
				for (i=0;i<8;i++) {
					if (cNode->children[i] != NULL) *stack++	=	cNode->children[i];
				}
	
				delete cNode;
			}
		}
	#endif
}


///////////////////////////////////////////////////////////////////////
// Class				:	CPhotonMap
// Method				:	reset
// Description			:	Reset the photonmap
// Return Value			:
// Comments				:
void	CPhotonMap::reset() {
	CMapTemplate<CPhoton>::reset();
}

#ifdef PHOTON_LOOKUP_CACHE

///////////////////////////////////////////////////////////////////////
// Class				:	CPhotonMap
// Method				:	lookup
// Description			:	Locate the nearest maxFoundPhoton photons
// Return Value			:
// Comments				:	
int		CPhotonMap::probe(Vec3 &C,const Vec3 &P,const Vec3 &N) {
	CPhotonNode			*cNode;
	CPhotonNode			**stackBase	=	(CPhotonNode **)	photonalloc(maxDepth*sizeof(CPhotonNode *)*8);
	CPhotonNode			**stack;
	CPhotonSample		*cSample;
	float				totalWeight	=	0;
	int					i;
	
	// Note: if word-stores are atomic, we don't need to lock when doing this
	
	if (root == NULL) return FALSE;

	stack			=	stackBase;
	*stack++		=	root;
	C = Vec3( 0,0,0 );
	while(stack > stackBase) {
		cNode	=	*(--stack);

		// Iterate over the samples
		for (cSample=cNode->samples;cSample!=NULL;cSample=cSample->next) {
			Vec3	D;
			float	d;
			
			D = cSample->P - P;

			d	=	D.Dot( D );

			if (d < (cSample->dP*cSample->dP)) {
				d	=	sqrtf(d);
				d	+=	fabs( D.Dot( cSample->N ) )*16;

				if ( cSample->N.Dot( N ) < 0) {
					// if N=0, this should not operate anything out.  verify
					d = C_INFINITY;
				}
			
				if (d < cSample->dP) {
					float	weight	=	1 - d / cSample->dP;

					totalWeight		+=	weight;
					C[0]			+=	cSample->C[0]*weight;
					C[1]			+=	cSample->C[1]*weight;
					C[2]			+=	cSample->C[2]*weight;
				}
			}
		}

		// Check the children
		for (i=0;i<8;i++) {
			CPhotonNode	*tNode	=	cNode->children[i];

			if (tNode != NULL) {
				const	float	tSide	=	tNode->side;

				if (	((tNode->center[0] + tSide) > P[0])	&&
						((tNode->center[1] + tSide) > P[1])	&&
						((tNode->center[2] + tSide) > P[2])	&&
						((tNode->center[0] - tSide) < P[0])	&&
						((tNode->center[1] - tSide) < P[1])	&&
						((tNode->center[2] - tSide) < P[2])) {
					*stack++	=	tNode;
				}
			}
		}
	}

	if (totalWeight > 0) {
		C = C*( 1 / totalWeight);
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////
// Class				:	CPhotonMap
// Method				:	insert
// Description			:	Insert a sample
// Return Value			:
// Comments				:	
void	CPhotonMap::insert(const Vec3 &C,const Vec3 &P,const Vec3 &N,float dP) {
	CPhotonSample	*cSample	=	new CPhotonSample;
	CPhotonNode		*cNode		=	root;
	int				depth		=	0;
	int				i,j;

	// lock the mutex so we're thread safe
	
	cSample->C = C;
	cSample->P = P;
	cSample->N = N;
	cSample->dP	=	dP;

	while(cNode->side > (2*dP)) {
		depth++;

		for (j=0,i=0;i<3;i++) {
			if (P[i] > cNode->center[i]) {
				j			|=	1 << i;
			}
		}

		if (cNode->children[j] == NULL)	{
			CPhotonNode	*nNode	=	(CPhotonNode *) new CPhotonNode;

			for (i=0;i<3;i++) {
				if (P[i] > cNode->center[i]) {
					nNode->center[i]	=	cNode->center[i] + cNode->side / (float) 4;
				} else {
					nNode->center[i]	=	cNode->center[i] - cNode->side / (float) 4;
				}
			}

			cNode->children[j]	=	nNode;
			nNode->side			=	cNode->side / (float) 2;
			nNode->samples		=	NULL;
			for (i=0;i<8;i++)	nNode->children[i]	=	NULL;
		}

		cNode			=	cNode->children[j];
	}

	cSample->next	=	cNode->samples;
	cNode->samples	=	cSample;
	maxDepth		=	max(maxDepth,depth);
	
}

#endif

///////////////////////////////////////////////////////////////////////
// Class				:	CPhotonMap
// Method				:	lookup
// Description			:	Locate the nearest maxFoundPhoton photons
// Return Value			:
// Comments				:	Nl	must be normalized
//							Il	must be normalized
void	CPhotonMap::lookup(Vec3 &Cl,const Vec3 &Pl,const Vec3 &Nl,int maxFound) {
	int				numFound;
	const CPhoton	**indices	=	(const CPhoton **)	photonalloc((maxFound+1)*sizeof(CPhoton *)); 
	float			*distances	=	(float	*)			photonalloc((maxFound+1)*sizeof(float)); 
	CLookup			l;

	searchRadius		=	(sqrtf(maxFound*maxPower / 0.05f) / (float) PI)*0.5f;

	// Lookup the photonmap at the location of the photon
	distances[0]		=	searchRadius*searchRadius;
	l.maxFound			=	maxFound;
	l.numFound			=	0;
	l.P = Pl;
	l.N = Nl;
	l.gotHeap			=	FALSE;
	l.indices			=	indices;
	l.distances			=	distances;

	#ifdef PHOTON_LOOKUP_CACHE
	if (!probe(Cl,l.P,l.N)) {
	#endif
		CMapTemplate<CPhoton>::lookupWithN(&l,1);
	
		Cl = Vec3(0,0,0);
	
		if ((numFound = l.numFound) < 2)	return;
	
		// Accumulate the irradiance
		for (int i=1;i<=numFound;i++) {
			const	CPhoton	*p	=	indices[i];
			Vec3	I;
	
			assert(distances[i] <= distances[0]);
	
			photonToDir(I,p->theta,p->phi);
	
			if ( I.Dot( l.N ) < 0) {
				Cl = Cl + p->C;
			}
		}
	
		// Normalize the result
		Cl = Cl * (float) (1.0 / (PI*distances[0]));
	
	#ifdef PHOTON_LOOKUP_CACHE
		// Insert it into the probe 
		insert(Cl,l.P,l.N,sqrtf(distances[0])*(float) 0.2);
	}
	#endif
}

///////////////////////////////////////////////////////////////////////
// Class				:	CPhotonMap
// Method				:	lookup
// Description			:	Locate the nearest maxFoundPhoton photons
// Return Value			:
// Comments				:	Nl	must be normalized
//							Il	must be normalized
void	CPhotonMap::lookup(Vec3 &Cl,const Vec3 &Pl,int maxFound) {
	int				numFound	=	0;
	const CPhoton	**indices	=	(const CPhoton **)	photonalloc((maxFound+1)*sizeof(CPhoton *)); 
	float			*distances	=	(float	*)			photonalloc((maxFound+1)*sizeof(float)); 
	CLookup			l;

	searchRadius		=	(sqrtf(maxFound*maxPower / 0.05f) / (float) PI)*0.5f;

	distances[0]		=	searchRadius*searchRadius;

	l.maxFound			=	maxFound;
	l.numFound			=	0;
	l.P = Pl;
	l.N = Vec3( 0,0,0);
	l.gotHeap			=	FALSE;
	l.indices			=	indices;
	l.distances			=	distances;

	#ifdef PHOTON_LOOKUP_CACHE
	if (!probe(Cl,l.P,l.N)) {
	#endif
		CMapTemplate<CPhoton>::lookup(&l,1);
	
		Cl = Vec3( 0,0,0 );
	
		if (l.numFound < 2)	return;
	
		numFound	=	l.numFound;
	
		for (int i=1;i<=numFound;i++) {
			const	CPhoton	*p	=	indices[i];
	
			assert(distances[i] <= distances[0]);
	
			Cl = Cl + p->C;
		}
		
		// Normalize the result
		Cl = Cl*(float) (1.0 / (PI*distances[0]));
	
	#ifdef PHOTON_LOOKUP_CACHE
		// Insert it into the probe 
		insert(Cl,l.P,l.N,sqrtf(distances[0])*(float) 0.2);
	}
	#endif
}

///////////////////////////////////////////////////////////////////////
// Class				:	CPhotonMap
// Method				:	balance
// Description			:	Balance the map
// Return Value			:
// Comments				:
void	CPhotonMap::balance() {
	// If we have no photons in the map, add a dummy one to avoid an if statement during the lookup
	if (numPhotons == 0) {
		Vec3	P( 0,0,0 );
		Vec3	I( 0,0,1 );
		CPhoton	*photon	=	CMapTemplate<CPhoton>::store(P,I);
		photon->C = Vec3( 0,0,0 );
	}

	CMapTemplate<CPhoton>::balance();
}



///////////////////////////////////////////////////////////////////////
// Class				:	CPhotonMap
// Method				:	store
// Description			:	Store a photon
// Return Value			:
// Comments				:
void	CPhotonMap::store(const Vec3 &P,const Vec3 &N,const Vec3 &I,const Vec3 &C) {

	CPhoton	*ton	=	CMapTemplate<CPhoton>::store(P,N);
	dirToPhoton(ton->theta,ton->phi,I);
	ton->C =C;
	maxPower	=	max(maxPower, C.Dot( C ) );
}


///////////////////////////////////////////////////////////////////////
// Class				:	CPhotonMap
// Method				:	bound
// Description			:	Bound the data
// Return Value			:
// Comments				:
void	CPhotonMap::bound(Vec3 &bmin,Vec3 &bmax) {
	int	i;

	bmin = Vec3(C_INFINITY, C_INFINITY, C_INFINITY);
	bmax = Vec3(-C_INFINITY, -C_INFINITY, -C_INFINITY);

	for (i=1;i<numPhotons;i++)	
	{
		addBox(bmin,bmax,photons[i].P );
	}
}
