///============================================================================
/// \file		LuaVec4.h
/// \brief		Header file for the CLuaVec4 Class
/// \date		08/09/2007
/// \author		Nick Tourte
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

#ifndef _C_LUAVEC4_H_
#define _C_LUAVEC4_H_


// tolua_begin
class CLuaVec4 : public Vec4
{
public:
	/// ctors
	CLuaVec4()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	}

	CLuaVec4(float inX, float inY, float inZ, float inW)
	{
		x = inX;
		y = inY;
		z = inZ;
		w = inW;
	}

	CLuaVec4(const Vec4 &in)
	{
		x = in.x;
		y = in.y;
		z = in.z;
		w = in.w;
	}

	~CLuaVec4() {}

	/// The only set fn in the impl
	inline void Set(float ax, float ay, float az, float aw)
	{
		x = ax;
		y = ay;
		z = az;
		w = aw;
	}

	/// Individual sets
	inline void SetX(float newX)
	{
		x = newX;
	}

	inline void SetY(float newY)
	{
		y = newY;
	}

	inline void SetZ(float newZ)
	{
		z = newZ;
	}

	inline void SetW(float newW)
	{
		w = newW;
	}

	/// Individual gets
	inline float GetX()
	{
		return x;
	}

	inline float GetY()
	{
		return y;
	}

	inline float GetZ()
	{
		return z;
	}

	inline float GetW()
	{
		return w;
	}

// tolua_end

	/// Init the implementation
	static void InitImplementation();	

private:

	/// The implementation
	static IVec4Implementation *m_Vec4Impl;

// tolua_begin
};

// tolua_end

#endif //_C_LUAVEC4_H_