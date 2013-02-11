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

#ifndef PHOTONMAP_H
#define PHOTONMAP_H


// Macros to pack/unpack directions
#define	dirToPhoton(theta__,phi__,D__)	{						\
	int		t__,p__;											\
	t__	=	(int) (acos(D__[2])*(256.0 / PI));				\
	p__	=	(int) (atan2(D__[1],D__[0])*(256.0 / (2.0*PI)));	\
	if (t__ > 255)												\
		theta__	=	(unsigned char) 255;						\
	else														\
		theta__	=	(unsigned char) t__;						\
																\
	if (p__ > 255)												\
		phi__		=	(unsigned char) 255;					\
	else if (p__ < 0)											\
		phi__		=	(unsigned char) (p__ + 256);			\
	else														\
		phi__		=	(unsigned char) p__;					\
}

#define	photonToDir(D__,theta__,phi__)	{						\
	D__[0]	=	sintheta[theta__]*cosphi[phi__];				\
	D__[1]	=	sintheta[theta__]*sinphi[phi__];				\
	D__[2]	=	costheta[theta__];								\
}


// Debug and build options
//#define PHOTON_DEBUG
//#define PHOTON_LOOKUP_CACHE

///////////////////////////////////////////////////////////////////////
// Class				:	CPhoton
// Description			:	A Photon
// Comments				:
class	CPhoton : public CTon {
public:
	Vec3			C;				// The intensity
	unsigned char	theta,phi;		// Photon direction
};


///////////////////////////////////////////////////////////////////////
// Class				:	CPhotonRay
// Description			:	A Photon
// Comments				:
class	CPhotonRay {
public:
	CPhotonRay( Ray &ray )
	{
		m_Ray = ray;
	}
	Ray				m_Ray;
	Vec3			intensity;		// The intensity
	float			factor;			// The product of all the factors (used for routian roulette)
};

///////////////////////////////////////////////////////////////////////
// Class				:	CPhotonMap
// Description			:	A Photon map
// Comments				:
class	CPhotonMap : public CMapTemplate<CPhoton>  {
	
	#ifdef PHOTON_LOOKUP_CACHE
		class	CPhotonSample {
		public:
			Vec3			C,P,N;
			float			dP;
			CPhotonSample	*next;
		};

		class	CPhotonNode {
		public:
			Vec3			center;
			float			side;
			CPhotonSample	*samples;
			CPhotonNode		*children[8];
		};
	#endif
	
public:
				CPhotonMap();
				~CPhotonMap();

	void		attach()	{	refCount++;	}
	void		detach()	{	refCount--; if (refCount == 0) delete this; }
	void		check()		{	if (refCount == 0)	delete this;			}

	void		reset();

	void		lookup(Vec3 &,const Vec3 &,int);
	void		lookup(Vec3 &,const Vec3 &,const Vec3 &,int);
	void		balance();

	void		store(const Vec3 &,const Vec3 &,const Vec3 &,const Vec3 &);

	void		bound(Vec3 &bmin,Vec3 &bmax);

	#ifdef PHOTON_LOOKUP_CACHE
		int			probe(Vec3 &,const Vec3 &,const Vec3 &);
		void		insert(const Vec3 &,const Vec3 &,const Vec3 &,float);

		CPhotonNode	*root;
		int			maxDepth;			// The maximum depth of the hierarchy
	#endif

	int			refCount;
	int			modifying;
	Matrix4x4		from,to;
	float		maxPower;			// The maximum photon power
	float		searchRadius;
};


#endif // PHOTONMAP_H