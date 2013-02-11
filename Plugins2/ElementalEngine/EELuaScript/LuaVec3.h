///============================================================================
/// \file		LuaVec3.h
/// \brief		Header file for CLuaVec3
/// \date		08-21-2005
/// \author		Josh Lulewicz
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

#ifndef	_CLUAVEC3_H_
#define	_CLUAVEC3_H_

// tolua_begin
class CLuaVec3 : public Vec3
{
public:
	CLuaVec3()			
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
	CLuaVec3( float ax, float ay, float az )
	{
		x = ax;
		y = ay;
		z = az;	
	}
	CLuaVec3( const Vec3& a )			
	{
		x = a.x;
		y = a.y;
		z = a.z;	
	}

	inline bool OpEquals( const Vec3& b ) const
	{
		return m_Vec3Implementation->Equals( *this, b );
	}
	inline bool OpNotEquals( const Vec3& b ) const
	{
		return m_Vec3Implementation->NotEquals( *this, b );
	}
	inline bool OpLessThan( const Vec3& b ) const
	{
		return m_Vec3Implementation->LessThan( *this, b );
	}
	inline bool OpGreaterThan( const Vec3& b ) const
	{
		return m_Vec3Implementation->GreaterThan( *this, b );
	}

	inline Vec3 OpAdd( const Vec3& b ) const
	{
		return m_Vec3Implementation->Add( *this, b );
	}
	inline Vec3& OpAddEquals( const Vec3& b )
	{
		m_Vec3Implementation->AddEquals( *this, b );
		return *this;
	}
	inline Vec3 OpSubtract( const Vec3& b ) const
	{
		return m_Vec3Implementation->Subtract( *this, b );
	}
	inline Vec3& OpSubtractEquals( const Vec3& b )
	{
		m_Vec3Implementation->SubtractEquals( *this, b );
		return *this;
	}
	inline Vec3 OpTimes( const Vec3& b ) const
	{
		return m_Vec3Implementation->Times( *this, b);
	}
	inline Vec3& OpTimesEquals( const Vec3& b )
	{
		m_Vec3Implementation->TimesEquals( *this, b );
		return *this;
	}
	inline Vec3 OpTimes( const Matrix3x3& m ) const
	{
		return m_Vec3Implementation->Times( *this, m);
	}
	inline Vec3& OpTimesEquals( const Matrix3x3& m )
	{
		m_Vec3Implementation->TimesEquals( *this, m );
		return *this;
	}
	inline Vec3 OpTimes( float f ) const
	{
		return m_Vec3Implementation->Times( *this, f);
	}
	inline Vec3& OpTimesEquals( float f )
	{
		m_Vec3Implementation->TimesEquals( *this, f );
		return *this;
	}
	inline Vec3 OpDivide( const Vec3& b ) const
	{
		return m_Vec3Implementation->Divide( *this, b );
	}
	inline Vec3& OpDivideEquals( const Vec3& b )
	{
		m_Vec3Implementation->DivideEquals( *this, b );
		return *this;
	}
	inline Vec3 OpDivide( float f ) const
	{
		return m_Vec3Implementation->Divide( *this, f );
	}
	inline Vec3& OpDivideEquals( float f )
	{
		m_Vec3Implementation->DivideEquals( *this, f );
		return *this;
	}
	inline Vec3 OpNegate() const
	{
		return Vec3(-x, -y, -z);
	}
// tolua_end

	/// Setup the implemenation for this vec3 lib
	static void InitImplementation();

private:

	/// vec3 implementation
	static IVec3Implementation *m_Vec3Implementation;

// tolua_begin
};
// tolua_end

#endif	// _CLUAVEC3_H_