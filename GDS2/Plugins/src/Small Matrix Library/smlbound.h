#ifndef _SMLBOUND_H
#define _SMLBOUND_H

#include <assert.h>
#include <math.h>

#include <smlmath.h>

#pragma warning(disable: 4786)

class SMLBoxBound;
class SMLSphereBound;
class SMLStream;

class SMLBound
{
public:
	SMLVec3f center;
	SMLLIBENTRY SMLBound();
	SMLLIBENTRY SMLBound(const SMLVec3f&);
	SMLLIBENTRY virtual ~SMLBound();
    virtual SMLBound* Clone() = 0;
    virtual const SMLBound* Clone() const = 0;

	//Inlines
	SMLLIBENTRY void SetCenter(float x, float y, float z);
	SMLLIBENTRY void SetCenter(const SMLVec3f &);
	SMLLIBENTRY const SMLVec3f &GetCenter() const;

	virtual bool Cull(const SMLPlane *planes, int numPlane) const =0 ;

	virtual void around(const SMLVec3f *pts, int npoints)=0;
	virtual void around(const SMLSphereBound** spheres, int nspheres)=0;
	virtual void around(const SMLBoxBound** boxes, int nboxes) = 0;
	virtual void around(const SMLSphereBound *sphere) = 0;
	virtual void around(const SMLBoxBound *box) = 0;
	virtual void around(const SMLBound *bound) =0;
	virtual void around(const SMLBound **bounds, int nbounds) = 0;

	virtual void extendBy(const SMLVec3f &point) = 0;
	virtual void extendBy(const SMLBoxBound *box) = 0;
	virtual void extendBy(const SMLSphereBound *sphere) = 0;
	virtual void extendBy(const SMLBound *) = 0;

	virtual bool contains(const SMLVec3f &point) const = 0;
	virtual bool contains(const SMLBoxBound *box) const = 0;
	virtual bool contains(const SMLSphereBound *sphere) const = 0;

	virtual bool contains(const SMLBound *bound)=0;
	virtual void Transform(const SMLBound *, const SMLMatrix4f &) = 0;

	virtual bool probe(const SMLVec3f &p1, const SMLVec3f &p2,
			float *closeDist, float *farDist) const = 0;
	virtual bool probe(const SMLRay &ray, float *closeDist,
				float *farDist) const = 0;

    virtual void Save(SMLStream &) {}
    virtual void Load(SMLStream &) {}

protected:
	virtual void copyFrom(const SMLBound&) = 0;
	virtual void copySame(SMLSphereBound*) = 0;
	virtual void copySame(SMLBoxBound*) = 0;
};

class SMLSphereBound : public SMLBound
{
public:
	float r;

	SMLLIBENTRY SMLSphereBound(const SMLVec3f &, float);
	SMLLIBENTRY SMLSphereBound(const SMLSphereBound &);
	SMLLIBENTRY SMLSphereBound();
    SMLLIBENTRY virtual SMLBound* Clone();
    SMLLIBENTRY virtual const SMLBound* Clone() const;
	
	SMLLIBENTRY virtual bool Cull(const SMLPlane *, int) const;
	SMLLIBENTRY const SMLSphereBound &operator =(const SMLSphereBound &s);
	SMLLIBENTRY virtual void around(const SMLSphereBound **spheres, int nspheres);
	SMLLIBENTRY virtual void around(const SMLVec3f *pts, int npoints);
	
	SMLLIBENTRY virtual void around(const SMLBoxBound **boxes, int nboxes);
	SMLLIBENTRY virtual void around(const SMLSphereBound *sphere);
	SMLLIBENTRY virtual void around(const SMLBoxBound *box);
	SMLLIBENTRY virtual void around(const SMLBound *bound);
	SMLLIBENTRY virtual void around(const SMLBound **bounds, int nbounds);


	SMLLIBENTRY virtual void extendBy(const SMLVec3f &point);
	SMLLIBENTRY virtual void extendBy(const SMLBoxBound *box);
	SMLLIBENTRY virtual void extendBy(const SMLSphereBound *sphere);
	SMLLIBENTRY virtual void extendBy(const SMLBound *);

	SMLLIBENTRY virtual bool contains(const SMLVec3f &point) const;
	SMLLIBENTRY virtual bool contains(const SMLBoxBound *box) const;
	SMLLIBENTRY virtual bool contains(const SMLSphereBound *sphere) const;
	
	SMLLIBENTRY virtual bool contains(const SMLBound *bound);
	SMLLIBENTRY virtual void Transform(const SMLBound *, const SMLMatrix4f &);

	SMLLIBENTRY virtual bool probe(const SMLVec3f &p1, const SMLVec3f &p2,
 			    float *closeDist, float *farDist) const;
	SMLLIBENTRY virtual bool probe(const SMLRay &ray, float *closeDist,
				float *farDist) const;
	SMLLIBENTRY void  Transform(const SMLSphereBound *src, const SMLMatrix4f &);
	SMLLIBENTRY void SetRadius(float);
	SMLLIBENTRY float GetRadius() const;

protected:
	virtual void copyFrom(const SMLBound&);
	virtual void copySame(SMLSphereBound*);
	virtual void copySame(SMLBoxBound*) { assert(0); }

private:
    SMLLIBENTRY virtual bool containsRelativeCoordinates(float px, float py, float pz) const;
	
};

class SMLBoxBound : public SMLBound
{
public:
	SMLVec3f side;//, min, max;

	SMLVec3f min, max;
	SMLVec3f corners[8];
	 
	SMLLIBENTRY SMLBoxBound(const SMLVec3f &center, const SMLVec3f &side);
	SMLLIBENTRY SMLBoxBound(const SMLBoxBound &);
	SMLLIBENTRY SMLBoxBound();
    SMLLIBENTRY virtual SMLBound* Clone();
    SMLLIBENTRY virtual const SMLBound* Clone() const;

	SMLLIBENTRY void ComputeMinMax();

	SMLLIBENTRY virtual bool Cull(const SMLPlane *, int) const;
	SMLLIBENTRY const SMLBoxBound &operator =(const SMLBoxBound &);
	SMLLIBENTRY virtual void around(const SMLVec3f *pts, int npoints);
	SMLLIBENTRY virtual void around(const SMLSphereBound** spheres, int nspheres);
	SMLLIBENTRY virtual void around(const SMLBoxBound** boxes, int nboxes);
	SMLLIBENTRY virtual void around(const SMLSphereBound *sphere);
	SMLLIBENTRY virtual void around(const SMLBoxBound *box);
	
	SMLLIBENTRY virtual void around(const SMLBound *bound);
	SMLLIBENTRY virtual void around(const SMLBound **bounds, int nbounds);

	SMLLIBENTRY virtual void extendBy(const SMLVec3f &point);
	SMLLIBENTRY virtual void extendBy(const SMLBoxBound *box);
	SMLLIBENTRY virtual void extendBy(const SMLSphereBound *sphere);
	SMLLIBENTRY virtual void extendBy(const SMLBound *);
	
	SMLLIBENTRY virtual bool contains(const SMLVec3f &point) const;
	SMLLIBENTRY virtual bool contains(const SMLBoxBound *box) const;
	SMLLIBENTRY virtual bool contains(const SMLSphereBound *sphere) const;
	SMLLIBENTRY virtual bool contains(const SMLBound *bound);
	
	SMLLIBENTRY virtual void Transform(const SMLBound *, const SMLMatrix4f &);
	SMLLIBENTRY virtual bool probe(const SMLVec3f &p1, const SMLVec3f &p2,
			float *closeDist, float *farDist) const;
	SMLLIBENTRY virtual bool probe(const SMLRay &ray,float *closeDist,
				float *farDist) const;
	SMLLIBENTRY void  Transform(const SMLBoxBound *src, const SMLMatrix4f &);
	
	//Inlines
	SMLLIBENTRY void SetSide(const SMLVec3f &);
	SMLLIBENTRY const SMLVec3f &GetSide() const;

protected:
	virtual void copyFrom(const SMLBound&);
	virtual void copySame(SMLBoxBound*);
	virtual void copySame(SMLSphereBound*) { assert(0); }
};

#include <smlbound.inl>

#endif
