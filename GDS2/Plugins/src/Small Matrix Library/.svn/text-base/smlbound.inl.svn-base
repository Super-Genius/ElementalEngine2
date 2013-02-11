inline void SMLSphereBound::Transform(const SMLSphereBound *src, const SMLMatrix4f &mat)
{
	if (src)
		mat.Transform(*src, *this);
}

inline void 
SMLBound::SetCenter(float x, float y, float z)
{
	center.x = x;
	center.y = y;
	center.z = z;
}

inline void 
SMLBound::SetCenter(const SMLVec3f &c)
{
	center = c;
}

inline const SMLVec3f &
SMLBound::GetCenter() const
{
	return center;
}

//////////Sphere Bound Inlines////////////////////////////////////////////

inline void 
SMLSphereBound::SetRadius(float a)
{
	r = a;
}

inline float 
SMLSphereBound::GetRadius() const
{
	return r;
}

inline const SMLSphereBound &
SMLSphereBound::operator =(const SMLSphereBound &s)
{
	center = s.center;
	r = s.r;
	return *this;
}

inline void
SMLSphereBound::copyFrom(const SMLBound &rhs)
{
	// No checking for rhs, if it really is a SphereBound.
	center = ((SMLSphereBound&) rhs).center;
	r      = ((SMLSphereBound&) rhs).r;
	
	// VC5.0 still can't figure out `this' is SMLSphereBound* !
	//rhs.copySame( this );
}

inline void
SMLSphereBound::copySame(SMLSphereBound* rhs)
{
	center = rhs -> center;
	r = rhs -> r;
}

//////////Box Bound Inlines///////////////////////////////////////////////

inline const SMLBoxBound &
SMLBoxBound::operator =(const SMLBoxBound &rhs)
{
	if( this != &rhs ) {
		center = rhs.center;
		side   = rhs.side;
	}
	return *this;
}
inline void 
SMLBoxBound::SetSide(const SMLVec3f &rhs)
{
	side = rhs;
}

inline const SMLVec3f &
SMLBoxBound::GetSide() const
{
	return side;
}

inline void
SMLBoxBound::copyFrom(const SMLBound& rhs)
{
	// No checking for rhs, if it really a BoxBound.
	center = rhs.center;
	side   = ((SMLBoxBound&) rhs).side;

	//rhs.copySame( this );
}

inline void
SMLBoxBound::copySame(SMLBoxBound* rhs)
{
	center = rhs -> center;
	side   = rhs -> side;
}

