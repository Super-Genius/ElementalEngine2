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

#ifndef CMapTemplateTEMPLATE_H
#define CMapTemplateTEMPLATE_H

// Some extern variables defined in photon.cpp
extern	const float				costheta[];
extern	const float				sintheta[];
extern	const float				cosphi[];
extern	const float				sinphi[];
#define	C_INFINITY							1e30f

///////////////////////////////////////////////////////////////////////
// Class				:	CTon
// Description			:	Holds a generic particle
// Comments				:
class	CTon {
public:
	Vec3			P,N;		// Position and normal
	short			flags;		// Photon flags
};

extern void	addBox(Vec3 &bmin, Vec3 &bmax,const Vec3 &v) ;
///////////////////////////////////////////////////////////////////////
// Class				:	CMapTemplate
// Description			:	Holds a generic map
// Comments				:
template <class T> class	CMapTemplate {
public:

	CMapTemplate() 
	{
		numPhotons	=	0;
		maxPhotons	=	0;
		photons		=	NULL;
		stepSize	=	10000;
		bmin = Vec3(C_INFINITY,C_INFINITY,C_INFINITY);
		bmax = Vec3(-C_INFINITY,-C_INFINITY,-C_INFINITY);
	}
	virtual	~CMapTemplate() 
	{
		if (photons != NULL)	delete [] photons;
	}
	void	reset() 
	{
		if (photons != NULL)	delete photons;

		numPhotons	=	0;
		maxPhotons	=	0;
		photons		=	NULL;
		bmin = Vec3( C_INFINITY,C_INFINITY,C_INFINITY);
		bmax = Vec3(-C_INFINITY,-C_INFINITY,-C_INFINITY);
	}
	T	*store(const Vec3 &P,const Vec3 &N) 
	{
		if (numPhotons < maxPhotons) {
			T	*photon	=	&photons[++numPhotons];

			photon->P = P;
			photon->N = N;
			addBox(bmin,bmax,P);
			photon->flags	=	0;

			return	photon;
		} else {
			T	*newPhotons;

			maxPhotons	+=	stepSize;
			stepSize	*=	2;
			newPhotons	=	new T[maxPhotons+1];
			if (numPhotons > 0)		{
				memcpy(newPhotons,photons,(numPhotons+1)*sizeof(T));
				delete [] photons;
			}
			photons		=	newPhotons;

			T	*photon	=	&photons[++numPhotons];

			photon->P = P;
			photon->N = N;
			addBox(bmin,bmax,P);
			photon->flags	=	0;

			return	photon;
		}
	}

	T		*store(const T *item) 
	{
		if (numPhotons < maxPhotons) {
			T	*photon	=	&photons[++numPhotons];

			photon[0]	=	item[0];
			addBox(bmin,bmax,item->P);

			return	photon;
		} else {
			T	*newPhotons;

			maxPhotons	+=	stepSize;
			stepSize	*=	2;
			newPhotons	=	new T[maxPhotons+1];
			if (numPhotons > 0)		{
				memcpy(newPhotons,photons,(numPhotons+1)*sizeof(T));
				delete [] photons;
			}
			photons		=	newPhotons;

			T	*photon	=	&photons[++numPhotons];

			photon[0]	=	item[0];
			addBox(bmin,bmax,item->P);

			return	photon;
		}
	}

	virtual	void	balance() 
	{
		if (numPhotons == 0)	return;

		T	**ar1	=	new T*[numPhotons+1];
		T	**ar2	=	new T*[numPhotons+1];
		int	i;
		T	*finalPhotons;

		for (i=0;i<=numPhotons;i++) {
			ar2[i]	=	&photons[i];
		}

		balance(ar1,ar2,1,1,numPhotons);

		delete [] ar2;

		finalPhotons		=	new T[numPhotons+1];
		for (i=1;i<=numPhotons;i++) {
			finalPhotons[i]	=	ar1[i][0];
		}

		delete [] ar1;
		delete [] photons;
		maxPhotons			=	numPhotons;
		photons				=	finalPhotons;
		numPhotonsh			=	numPhotons >> 1;
	}

	Vec3	bmin,bmax;
protected:

	class	CLookup {
			public:
				int			maxFound;
				int			numFound;
				int			gotHeap;
				Vec3		P,N;
				float		*distances;
				const T		**indices;
			};

	void	balance(T **ar1,T **ar2,int index,int start,int end)
	{
		int	median	=	1;

		while((4*median) <= (end-start+1))	median	+=	median;

		if ((3*median) <= (end-start+1)) {
			median	+=	median;
			median	+=	start-1;
		} else
			median	=	end-median+1;

		int	axis	=	2;
		if (((bmax[0] - bmin[0]) > (bmax[1] - bmin[1]))  && ((bmax[0] - bmin[0]) > (bmax[2] - bmin[2])))
			axis	=	0;
		else if ((bmax[1] - bmin[1]) > (bmax[2] - bmin[2]))
			axis	=	1;

				{
#define	swap(item1,item2)	{			\
	T		*tmp	=	ar2[item1];		\
	ar2[item1]		=	ar2[item2];		\
	ar2[item2]		=	tmp;			\
}
					int	left	=	start;
					int	right	=	end;

					while(right > left) {
						const	float	v	=	ar2[right]->P[axis];
						int				i	=	left-1;
						int				j	=	right;

						for(;;) {
							while(ar2[++i]->P[axis] < v);
							while((ar2[--j]->P[axis] > v) && (j>left));

							if (i >= j)	break;

							swap(i,j);
						}

						swap(i,right);
						if (i >= median)	right	=	i-1;
						if (i <= median)	left	=	i+1;
					}


#ifdef PHOTON_DEBUG
					// FIXME: Remove
					int	i;
					for (i=start;i<median;i++) {
						assert(ar2[i]->P[axis] <= ar2[median]->P[axis]);
					}

					for (i=median+1;i<=end;i++) {
						assert(ar2[i]->P[axis] >= ar2[median]->P[axis]);
					}
					// FIXME: Remove
#endif

#undef swap
				}

				ar1[index]			=	ar2[median];
				ar1[index]->flags	=	(short) axis;

				if (median > start) {
					if (start < (median-1)) {
						const	float	tmp	=	bmax[axis];
						bmax[axis]			=	ar1[index]->P[axis];
						balance(ar1,ar2,2*index,start,median-1);
						bmax[axis]			=	tmp;
					} else {
						ar1[2*index]		=	ar2[start];
					}
				}

				if (median < end) {
					if ((median+1) < end) {
						const	float	tmp	=	bmin[axis];
						bmin[axis]			=	ar1[index]->P[axis];
						balance(ar1,ar2,2*index+1,median+1,end);
						bmin[axis]			=	tmp;
					} else {
						ar1[2*index+1]		=	ar2[end];
					}
				}
			}

public:


	void	lookupWithN(CLookup *l,int index) {
				const T		*photon	=	&photons[index];
				float		d,t;
				Vec3		D;
				int			axis	=	photon->flags;

				if (index < numPhotonsh) {
					d	=	l->P[axis] - photon->P[axis];

					if (d > 0) {
						lookupWithN(l,2*index+1);

						if (d*d < l->distances[0]) {
							lookupWithN(l,2*index);
						}

					} else {
						lookupWithN(l,2*index);

						if (d*d < l->distances[0]) {
							lookupWithN(l,2*index+1);
						}
					}
				}

				D = photon->P - l->P;
				d	=	D.Dot(D);
				t	=	D.Dot(l->N);
				d	+=	t*t*16;

				if (d < l->distances[0]) {

					if (l->numFound < l->maxFound) {


						l->numFound++;
						l->distances[l->numFound]	=	d;
						l->indices[l->numFound]		=	photon;
					} else {
						int	j,parent;


						if (l->gotHeap == FALSE) {
							int		halfPhotons	=	l->numFound >> 1;
							int		k;
							float	dtmp;
							const T	*ptmp;

							for (k=halfPhotons;k>=1;k--) {
								parent	=	k;
								ptmp	=	l->indices[k];
								dtmp	=	l->distances[k];

								while(parent <= halfPhotons) {
									j	=	parent + parent;
									if ((j < l->numFound) && (l->distances[j] < l->distances[j+1])) j++;
									if (dtmp >= l->distances[j])	break;

									l->distances[parent]	=	l->distances[j];
									l->indices[parent]		=	l->indices[j];
									parent					=	j;
								}

								l->distances[parent]		=	dtmp;
								l->indices[parent]			=	ptmp;
							}

							l->gotHeap	=	TRUE;
						}


						for (parent=1,j=2;j<=l->numFound;) {
							if ((j < l->numFound) && (l->distances[j] < l->distances[j+1]))
								j++;

							if (d > l->distances[j]) break;

							l->distances[parent]	=	l->distances[j];
							l->indices[parent]		=	l->indices[j];
							parent	=	j;
							j		+=	j;
						}

						l->distances[parent]	=	d;
						l->indices[parent]		=	photon;
						assert(l->distances[1] <= l->distances[0]);
						l->distances[0]			=	l->distances[1];
					}
				}
			}

	void	lookup(CLookup *l,int index) {
				const T		*photon	=	&photons[index];
				float		d;
				Vec3		D;
				int			axis	=	photon->flags;

				if (index < numPhotonsh) {
					d	=	l->P[axis] - photon->P[axis];

					if (d > 0) {
						lookup(l,2*index+1);

						if (d*d < l->distances[0]) {
							lookup(l,2*index);
						}

					} else {
						lookup(l,2*index);

						if (d*d < l->distances[0]) {
							lookup(l,2*index+1);
						}
					}
				}

				D = photon->P - l->P;
				d	=	D.Dot( D );

				if (d < l->distances[0]) {

					if (l->numFound < l->maxFound) {


						l->numFound++;
						l->distances[l->numFound]	=	d;
						l->indices[l->numFound]		=	photon;
					} else {
						int	j,parent;


						if (l->gotHeap == FALSE) {
							int		halfPhotons	=	l->numFound >> 1;
							int		k;
							float	dtmp;
							const T	*ptmp;

							for (k=halfPhotons;k>=1;k--) {
								parent	=	k;
								ptmp	=	l->indices[k];
								dtmp	=	l->distances[k];

								while(parent <= halfPhotons) {
									j	=	parent + parent;
									if ((j < l->numFound) && (l->distances[j] < l->distances[j+1])) j++;
									if (dtmp >= l->distances[j])	break;

									l->distances[parent]	=	l->distances[j];
									l->indices[parent]		=	l->indices[j];
									parent					=	j;
								}

								l->distances[parent]		=	dtmp;
								l->indices[parent]			=	ptmp;
							}

							l->gotHeap	=	TRUE;
						}


						for (parent=1,j=2;j<=l->numFound;) {
							if ((j < l->numFound) && (l->distances[j] < l->distances[j+1]))
								j++;

							if (d > l->distances[j]) break;

							l->distances[parent]	=	l->distances[j];
							l->indices[parent]		=	l->indices[j];
							parent	=	j;
							j		+=	j;
						}

						l->distances[parent]	=	d;
						l->indices[parent]		=	photon;
						assert(l->distances[1] <= l->distances[0]);
						l->distances[0]			=	l->distances[1];
					}
				}
			}

		T				*photons;
		int				numPhotons;
		int				maxPhotons;
		int				numPhotonsh;
		int				stepSize;
};

#endif