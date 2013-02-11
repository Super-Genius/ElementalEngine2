#include <float.h>

#include <smlbound.h>

#define NUMDIM	3
#define RIGHT	0
#define LEFT	1
#define MIDDLE	2

static bool HitBoundingBox(float  *minB, float *maxB, float *origin, float *dir, 
						   float length, float *coord, float *closeDist)
{
	bool inside = true;
	char quadrant[NUMDIM];
	register int i;
	int whichPlane;
	float  maxT[NUMDIM];
	float candidatePlane[NUMDIM];

	/* Find candidate planes; this loop can be avoided if
   	rays cast all from the eye (assume perspective view) */
	for (i=0; i<NUMDIM; i++)
		if(origin[i] < minB[i]) {
			quadrant[i] = LEFT;
			candidatePlane[i] = minB[i];
			inside = false;
		}else if (origin[i] > maxB[i]) {
			quadrant[i] = RIGHT;
			candidatePlane[i] = maxB[i];
			inside = false;
		}else	{
			quadrant[i] = MIDDLE;
		}

	/* Ray origin inside bounding box */
	if(inside)	{
		coord = origin;
		return (true);
	}


	/* Calculate T distances to candidate planes */
	for (i = 0; i < NUMDIM; i++)
		if (quadrant[i] != MIDDLE && dir[i] !=0.)
			maxT[i] = (candidatePlane[i]-origin[i]) / dir[i];
		else
			maxT[i] = -1.;

	/* Get largest of the maxT's for final choice of intersection */
	whichPlane = 0;
	for (i = 1; i < NUMDIM; i++)
		if (maxT[whichPlane] < maxT[i])
			whichPlane = i;

	/* Check final candidate actually inside box */
	if (maxT[whichPlane] < 0.) return (false);
	for (i = 0; i < NUMDIM; i++)
		if (whichPlane != i) {
			coord[i] = origin[i] + maxT[whichPlane] *dir[i];
			if (coord[i] < minB[i] || coord[i] > maxB[i])
				return (false);
		} else {
			coord[i] = candidatePlane[i];
		}
	
	SMLVec3f *tmp  = (SMLVec3f *) coord;
	*closeDist = tmp->Distance(origin);
	if (*closeDist > length)
		return false;

	return (true);				/* ray hits box */
}	
bool 
SMLBoxBound::probe(const SMLRay &l, float *closeDist, float *p) const
{
	return true;
	SMLVec3f intersectionPoint;
	bool ret = HitBoundingBox((float *)(&min), (float *)(& max), (float *)(& l.start), (float *)(& l.direction), l.length,
		 (float *) (&intersectionPoint), closeDist);
	return ret;
}


/////////////////////////////////////////////////////////////////////////////
////////////////////// Bound Methods/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
SMLBound::SMLBound()
: center(0, 0, 0)
{
}

SMLBound::SMLBound(const SMLVec3f& rhs)
: center(rhs)
{
}

SMLBound::~SMLBound()
{
}

/////////////////////////////////////////////////////////////////////////////
////////////////Sphere Bound Methods/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
SMLSphereBound::SMLSphereBound(const SMLVec3f &c , float a)
: SMLBound(c), r(a)
{
}

SMLSphereBound::SMLSphereBound(const SMLSphereBound &rhs)
: SMLBound(rhs), r(rhs.r)
{
}

SMLSphereBound::SMLSphereBound()
: SMLBound(), r(0.0)
{
}

SMLBound* SMLSphereBound::Clone()
{
    return new SMLSphereBound(*this);
}

const SMLBound* SMLSphereBound::Clone() const
{
    return new SMLSphereBound(*this);
}

void SMLSphereBound::around(const SMLVec3f *pts, int npts)
{
	if (npts) {

		SMLBoxBound box;
		box.center = pts[0];
		box.side   = SMLVec3f(0,0,0);

		int i;
		float r1 = 0.0f, r2 = 0.0f;

		for (i = 1; i < npts; i++)
			box.extendBy(pts[i]);

		center = box.center;

		r2 = 0.0f;
		for (i = 0; i < npts; i++) {
			r1 = pts[i].DistanceSquared(center);
			if (r1 > r2)
				r2 = r1;
		}
		r = FastSqrt(r2);
	}
}
void SMLSphereBound::around(const SMLSphereBound** spheres, int nspheres)
{
	if (nspheres)
	{
		int i, j;
		for (i = 0; i<nspheres; i++)
		{
			if (spheres[i]->r > FLT_EPSILON) 
			{
				center= spheres[i]->center;
				r = spheres[i]->r;
				break;
			}
		}
		for (j = i; j <nspheres; j++)
		{
			if (spheres[j]->r > FLT_EPSILON)
			{
				SMLVec3f dCenter = center - spheres[j]->center;
				float disc = dCenter.Length();
				if (disc + r < spheres[j]->r)
				{
					center = spheres[j]->center;
					r = spheres[j]->r;
				}
				else if (disc + spheres[j]->r > r)
				{
					if (disc > FLT_EPSILON) {        // new center
						float alpha = (r - spheres[j]->r + disc) / (2.0f * disc);
						center.ScaleAdd(alpha, dCenter, spheres[j]->center);
					}
					r = 0.5f * (spheres[j]->r + disc + r);
				}
			}
		}
	}
}

void SMLSphereBound::around(const SMLBoxBound** boxes, int nboxes)
{
	if (nboxes) {
		// NOT COMPLETE !!!// 
		// Need check invalid box, later... Youda
		SMLBoxBound box;
		box.center = boxes[0]->center;
		box.side   = boxes[0]->side;

		for (int i = 1; i<nboxes; i++) {
			box.extendBy( boxes[i] );
		}
		center = box.center;
		r      = box.side.Length();
	} 

}

void SMLSphereBound::around(const SMLSphereBound *sphere)
{
	if (sphere)
	{
		center = sphere->center;
		r      = sphere->r;
	}
}
void SMLSphereBound::around(const SMLBoxBound *box)
{
	if (box) {
		center = box->center;
		r      = box->side.Length();
	}
}

void SMLSphereBound::around(const SMLBound *b)
{
	const SMLSphereBound *sb = dynamic_cast<const SMLSphereBound *> (b);
	if (sb)
	{
		around(sb);
		return;
	}
	const SMLBoxBound *bb = dynamic_cast<const SMLBoxBound *>( b);
	if (bb)
	{
		around(bb);
		return;
	}
}

void SMLSphereBound::around(const SMLBound **bounds, int nbounds)
{
	if (nbounds > 0)
	{
		around(bounds[0]);
	}
	for (int i = 1; i<nbounds; i++)
	{
		extendBy(bounds[i]);
	}
}
void SMLSphereBound::extendBy(const SMLBound *b)
{
	const SMLSphereBound *sb = dynamic_cast<const SMLSphereBound *> (b);
	if (sb)
	{
		extendBy(sb);
		return;
	}
	const SMLBoxBound *bb = dynamic_cast<const SMLBoxBound *>( b);
	if (bb)
	{
		extendBy(bb);
		return;
	}
}

void SMLSphereBound::Transform(const SMLBound *b, const SMLMatrix4f &m)
{
	const SMLSphereBound *sb = dynamic_cast<const SMLSphereBound *> (b);
	if (sb)
	{
		Transform(sb, m);
		return;
	}
	const SMLBoxBound *bb = dynamic_cast<const SMLBoxBound *> (b);
	if (bb)
	{
		SMLSphereBound sb;
		sb.around(bb);
		Transform(&sb, m);
		return;
	}
}
void SMLSphereBound::extendBy(const SMLVec3f &point)
{
	
	SMLVec3f toPt;
	toPt.Sub(point,center);
	float dist = toPt.LengthSquared();
	if (dist > r * r)
	{
		dist = FastSqrt(dist);
		float diff = (dist - r) /(2.0f*dist);
		toPt *= diff;
		center += toPt;
		r += (dist - r) * 0.5f;
	}
}
void SMLSphereBound::extendBy(const SMLBoxBound *box)
{
	// To be optimized.
    if (r == 0.0f)
    {
        around(box);
        return;
    }
    extendBy(SMLVec3f(box->center.x + box->side.x, box->center.y + box->side.y, box->center.z + box->side.z));
    extendBy(SMLVec3f(box->center.x + box->side.x, box->center.y + box->side.y, box->center.z - box->side.z));
    extendBy(SMLVec3f(box->center.x + box->side.x, box->center.y - box->side.y, box->center.z + box->side.z));
    extendBy(SMLVec3f(box->center.x + box->side.x, box->center.y - box->side.y, box->center.z - box->side.z));
    extendBy(SMLVec3f(box->center.x - box->side.x, box->center.y + box->side.y, box->center.z + box->side.z));
    extendBy(SMLVec3f(box->center.x - box->side.x, box->center.y + box->side.y, box->center.z - box->side.z));
    extendBy(SMLVec3f(box->center.x - box->side.x, box->center.y - box->side.y, box->center.z + box->side.z));
    extendBy(SMLVec3f(box->center.x - box->side.x, box->center.y - box->side.y, box->center.z - box->side.z));
}

void SMLSphereBound::extendBy(const SMLSphereBound *sphere)
{
	if (sphere)
	{
		if (sphere->r > FLT_EPSILON)
		{
			if (r == 0.0f)
			{
				around(sphere);
				return;
			}
			SMLVec3f dCenter = center - sphere->center;
			float disc = dCenter.Length();
			if (disc + r < sphere->r)
			{
				center = sphere->center;
				r = sphere->r;
			}
			else if (disc + sphere->r > r)
			{
				if (disc > FLT_EPSILON) {        // new center
					float alpha = (r - sphere->r + disc) / (2.0f * disc);
					center.ScaleAdd(alpha, dCenter, sphere->center);
				}
				r = 0.5f * (sphere->r + disc + r);
			}
		}
	}

}

bool 
SMLSphereBound::containsRelativeCoordinates(float px, float py, float pz) const
{
    float r2;
    float dist;
    return FastAbs(px) <= r && FastAbs(py) <= r && FastAbs(pz) <= r &&
           ((dist  = px * px) <= (r2 = r * r)) &&
           ((dist += py * py) <= r2) &&
           ((dist +  pz * pz) <= r2);
}

bool 
SMLSphereBound::contains(const SMLVec3f &point) const
{
    return containsRelativeCoordinates(point.x - center.x,
                                       point.y - center.y,
                                       point.z - center.z);
}

bool 
SMLSphereBound::contains(const SMLBoxBound *box) const
{
	if (box) {
		if( box->side.x < 0 ) {
			// invalid box, what should we return?
			return true;
		}
		SMLVec3f min=box->center-box->side;
		SMLVec3f max=box->center+box->side;

        float minx = FastAbs(min.x - center.x);
        float maxx = FastAbs(max.x - center.x);
        float miny = FastAbs(min.y - center.y);
        float maxy = FastAbs(max.y - center.y);
        float minz = FastAbs(min.z - center.z);
        float maxz = FastAbs(max.z - center.z);
        return containsRelativeCoordinates(max(minx, maxx), max(miny, maxy), max(minz, maxz));
	} else
		return false;
}

bool 
SMLSphereBound::contains(const SMLSphereBound *sphere) const
{
	if (sphere) {
		SMLVec3f cenDiff;
		cenDiff.Sub(sphere->center,center);
		float dist = cenDiff.Length();
		return (dist + sphere->r <= r);
	
	} else
		return false;
}

bool 
SMLSphereBound::probe(const SMLVec3f &start, const SMLVec3f &end,
			float *closeDist, float *farDist) const
{	
	SMLRay ray(start, end);
	return probe(ray, closeDist, farDist);
}

bool 
SMLSphereBound::probe(const SMLRay &l, float *closeDist, float *farDist) const
{
//	return true;
	float t = FLT_MAX;
	SMLVec3f diff = center - l.start;

	float rsqur = r * r;
	float c = diff.LengthSquared() - rsqur;
	if (c < 0.0) return true;

	float b = -2.0f * (l.direction.Dot(diff));
	float delta = b*b -4 * c;
	
	if (delta < 0.0)
	  return false;

	if (delta < FLT_EPSILON) {
		if (b < 0) 
			return false;

		t =  -b/(2.0f );
		return t<l.length;

	} else  {
		float u = FastSqrt(delta);
		float d1 = (-b - u) /(2.0f);
	   
		if (d1 < 0 )  {
			return 0;
		} 
		else 
			return d1<l.length;
	}
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////Box Bound Methods////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
SMLBoxBound::SMLBoxBound(const SMLVec3f &center, const SMLVec3f &s)
: SMLBound( center), side(s)
{
}

SMLBoxBound::SMLBoxBound(const SMLBoxBound &rhs)
: SMLBound( rhs.center), side(rhs.side)
{
}

SMLBoxBound::SMLBoxBound()
: SMLBound(), side(0.0,0.0,0.0)
{

}

SMLBound* SMLBoxBound::Clone()
{
    return new SMLBoxBound(*this);
}

const SMLBound* SMLBoxBound::Clone() const
{
    return new SMLBoxBound(*this);
}

void SMLBoxBound::around(const SMLBound *b)
{	
	const SMLSphereBound *sb = dynamic_cast<const SMLSphereBound *> (b);
	if (sb)
	{
		around(sb);
		return;
	}
	const SMLBoxBound *bb = dynamic_cast<const SMLBoxBound *>( b);
	if (bb)
	{
		around(bb);
		return;
	}

}

void SMLBoxBound::around(const SMLBound **bounds, int nbounds)
{
	if (nbounds > 0)
	{
		around(bounds[0]);
	}
	for (int i = 1; i<nbounds; i++)
	{
		extendBy(bounds[i]);
	}

}
void SMLBoxBound::extendBy(const SMLBound *b)
{
	const SMLSphereBound *sb = dynamic_cast<const SMLSphereBound *> (b);
	if (sb)
	{
		extendBy(sb);
		return;
	}
	const SMLBoxBound *bb = dynamic_cast<const SMLBoxBound *>( b);
	if (bb)
	{
		extendBy(bb);
		return;
	}
}

void SMLBoxBound::Transform(const SMLBound *b, const SMLMatrix4f &m)
{
	const SMLBoxBound *sb = dynamic_cast<const SMLBoxBound *>(b);
	if (sb)
	{
		Transform(sb, m);
		return;
	}
}
void SMLBoxBound::around(const SMLVec3f *pts, int npts)
{
	center = pts[0];
    side   = SMLVec3f(0,0,0);

    for (int i = 1; i < npts; i++)
		extendBy( pts[i] );

}

void SMLBoxBound::around(const SMLSphereBound** spheres, int nspheres)
{
	SMLSphereBound sb;
	sb.around(spheres, nspheres);
	around(&sb);

}

void 
SMLBoxBound::around(const SMLBoxBound** boxes, int nboxes)
{
	if (nboxes) 
    {
		center = boxes[0]->center;
		side   = boxes[0]->side;
		for (int i = 1; i<nboxes; i++)
			extendBy(boxes[i]); 
	}

}

void SMLBoxBound::around(const SMLSphereBound *sphere)
{	
	if (sphere) 
    {
		float r = sphere->r;
		center  = sphere->center;
		side    = SMLVec3f(r,r,r);
	}

}

void 
SMLBoxBound::around(const SMLBoxBound *box)
{
	if (box) {
		center = box->center;
		side   = box->side;
	}

}

void 
SMLBoxBound::extendBy(const SMLVec3f &point)
{
	SMLVec3f min=center-side;
	SMLVec3f max=center+side;

	if (min.x > point.x)
		min.x = point.x;
	if (min.y > point.y)
		min.y = point.y;
	if (min.z > point.z)
		min.z = point.z;
	if (max.x < point.x)
		max.x = point.x;
	if (max.y < point.y)
		max.y = point.y;
	if (max.z < point.z)
		max.z = point.z;
	
	center = (min+max)*0.5;
	side   = center - min;

}

void SMLBoxBound::extendBy(const SMLBoxBound *box)
{
    if (box)
    {
        if (box->side.x >= 0.0f && box->side.y >= 0.0f && box->side.z >= 0.0f &&
            box->side.x + box->side.y + box->side.z > FLT_EPSILON)
        {
            if (side.x <= 0.0f && side.y <= 0.0f && side.z <= 0.0f)
            {
                around(box);
                return;
            }

            SMLVec3f min=center-side;
            SMLVec3f max=center+side;
            SMLVec3f bmin=box->center-box->side;
            SMLVec3f bmax=box->center+box->side;

            if (bmin.x < min.x) 
                min.x = bmin.x;
            if (bmin.y < min.y)
                min.y = bmin.y;
            if (bmin.z < min.z) 
                min.z = bmin.z;
            if (bmax.x > max.x)
                max.x = bmax.x;
            if (bmax.y > max.y)
                max.y = bmax.y;
            if (bmax.z > max.z)
                max.z = bmax.z;

            center = (min+max)*0.5;
            side   = center - min;
        }
    }
}

void SMLBoxBound::extendBy(const SMLSphereBound *sphere)
{
	if (sphere)
	{
		if (sphere->r > FLT_EPSILON)
		{
            if (side.x <= 0.0f && side.y <= 0.0f && side.z <= 0.0f)
            {
                around(sphere);
                return;
            }

            SMLBoxBound sphereInTheBox;
            sphereInTheBox.around(sphere);
            extendBy(&sphereInTheBox);
        }
    }
}

bool 
SMLBoxBound::contains(const SMLVec3f &point) const
{
	return ( FastAbs(center.x-point.x)<=side.x && 
			 FastAbs(center.y-point.y)<=side.y &&
			 FastAbs(center.z-point.z)<=side.z );
}

bool SMLBoxBound::contains(const SMLBoxBound *box) const
{	
	if (box)
		return contains(box->center+box->side) &&
			   contains(box->center-box->side);
	else
		return false;
}

bool SMLBoxBound::contains(const SMLSphereBound *sphere) const
{ 
	if (sphere) {
		float r =  sphere->r;
		return	contains( sphere->center + SMLVec3f(r,r,r) ) &&
				contains( sphere->center - SMLVec3f(r,r,r) );
	}
	else
		return false;
}

bool SMLBoxBound::probe(const SMLVec3f &p1, const SMLVec3f &p2,
			float *closeDist, float *farDist) const
{
	SMLRay ray(p1, p2);
	return probe(ray, closeDist, farDist);

}



void 
SMLBoxBound::Transform(const SMLBoxBound *src, const SMLMatrix4f &m)
{
    const SMLVec3f& c = src->center;
    const SMLVec3f& s = src->side;
    SMLVec3f dst;
    side.x = side.y = side.z = 0.0f;
    m.TransformPoint(SMLVec3f(c.x + s.x, c.y + s.y, c.z + s.z), center);
    m.TransformPoint(SMLVec3f(c.x + s.x, c.y + s.y, c.z - s.z), dst); extendBy(dst);
    m.TransformPoint(SMLVec3f(c.x + s.x, c.y - s.y, c.z + s.z), dst); extendBy(dst);
    m.TransformPoint(SMLVec3f(c.x + s.x, c.y - s.y, c.z - s.z), dst); extendBy(dst);
    m.TransformPoint(SMLVec3f(c.x - s.x, c.y + s.y, c.z + s.z), dst); extendBy(dst); 
    m.TransformPoint(SMLVec3f(c.x - s.x, c.y + s.y, c.z - s.z), dst); extendBy(dst);  
    m.TransformPoint(SMLVec3f(c.x - s.x, c.y - s.y, c.z + s.z), dst); extendBy(dst);  
    m.TransformPoint(SMLVec3f(c.x - s.x, c.y - s.y, c.z - s.z), dst); extendBy(dst);  
}
bool SMLSphereBound::contains(const SMLBound *b)
{
	const SMLSphereBound *sb = dynamic_cast<const SMLSphereBound *> (b);
	if (sb)
	{
		return contains(sb);
	}
	const SMLBoxBound *bb = dynamic_cast<const SMLBoxBound *> (b);
	if (bb) return contains(bb);
	return false;
}
bool SMLBoxBound::contains(const SMLBound *b)
{
	const SMLSphereBound *sb = dynamic_cast<const SMLSphereBound *> (b);
	if (sb)
	{
		return contains(sb);
	}
	const SMLBoxBound *bb = dynamic_cast<const SMLBoxBound *> (b);
	if (bb) return contains(bb);
	return false;
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////Culling Methods//////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
bool SMLSphereBound::Cull(const SMLPlane *planes, int nPlanes) const
{
//	return false;
	if (r == 0.0) 
		return true;
    for (int i = 0; i < nPlanes; i++)
	{
		if (planes[i].Distance(center) >= r)
			return true;        // if sphere is outside of this plane
	}
	return false;               // if sphere is inside all planes
} 

// long total = 0;
// long culled[6] = {0,0,0,0,0,0};

bool SMLBoxBound::Cull(const SMLPlane *planes, int nPlanes) const
{
    // total++;
//return false;
    if (side.x < 0.0f)
    {
        return true;
    }

    for (int i = 0; i < nPlanes; i++)
	{
        // if (i == 1) continue;   // Don't bother with far plane.
        float px = planes[i].x;
        float py = planes[i].y;
        float pz = planes[i].z;
        float pw = planes[i].w;
        // If point c is outside the plane, then all box is outside.
        float cx = center.x + ((px > 0.0f)? -side.x : side.x);
        float cy = center.y + ((py > 0.0f)? -side.y : side.y);
        float cz = center.z + ((pz > 0.0f)? -side.z : side.z);
        if (cx * px + cy * py + cz * pz + pw > 0.0f) 
        {
            // ++culled[i];
            return true;
        }
	}
    // Edge to edge exclusion is not considered.
	return false;
}

void SMLBoxBound::ComputeMinMax()
{
	min = center - side;
	max = center + side;

	corners[0] = min;
	corners[1] = min + SMLVec3f(0, 0, 2.0f*side.z);
	corners[2] = min + SMLVec3f(0, 2.0f*side.y, 2.0f*side.z);
	corners[3] = min + SMLVec3f(2.0f*side.x, 0, 2.0f*side.z);
	corners[4] = min + SMLVec3f(2.0f*side.x, 0, 0);
	corners[5] = min + SMLVec3f (0, 2.0f*side.y, 0);
	corners[6] = min + SMLVec3f(2.0f*side.x, 2.0f*side.y, 0);
	corners[7] = max;
}
	
void
SMLMatrix4f::Transform(const SMLSphereBound &src, SMLSphereBound &dst) const
{
	TransformPoint(src.center, dst.center);
	float rx, tmp;
	rx = FastAbs(data[0][0]);
	tmp = FastAbs(data[0][1]);
	if (tmp > rx) rx = tmp;
		tmp = FastAbs(data[0][2]);
	if (tmp > rx) rx = tmp;
		tmp = FastAbs(data[1][0]);
	if (tmp > rx) rx = tmp;
		tmp = FastAbs(data[1][1]);
	if (tmp > rx) rx = tmp;
		tmp = FastAbs(data[1][2]);
	if (tmp > rx) rx = tmp;
		tmp = FastAbs(data[2][0]);
	if (tmp > rx) rx = tmp;
		tmp = FastAbs(data[2][1]);
	if (tmp > rx) rx = tmp;
		tmp = FastAbs(data[2][2]);
	if (tmp > rx) rx = tmp;
		dst.r = src.r * (float) rx;
	
}
