///============================================================================
/// \file		LuaEuler.h
/// \brief		Header file for CLuaEulerAngle
/// \date		12-18-2006
/// \author		kyle Swaim
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

#ifndef	_CLUAEULERANGLE_H_
#define	_CLUAEULERANGLE_H_

// tolua_begin
class CLuaEulerAngle : public EulerAngle
{
public:
	CLuaEulerAngle()			
	{
		roll = 0.0f;
		pitch = 0.0f;
		yaw = 0.0f;
	}
	CLuaEulerAngle( float aroll, float apitch, float ayaw )
	{
		roll = aroll;
		pitch = apitch;
		yaw = ayaw;
	}
	CLuaEulerAngle( const EulerAngle& a )			
	{
		roll = a.roll;
		pitch = a.pitch;
		yaw = a.yaw;
	}

	inline bool Equals( const EulerAngle& b )
	{
		return m_EulerImplementation->Equals( *this, b );
	}

	inline EulerAngle Add( const EulerAngle& b )
	{
		return m_EulerImplementation->Add( *this, b );
	}

	inline EulerAngle Times( float f )
	{
		return m_EulerImplementation->Times( *this, f );
	}

	inline void TimesEquals( float f )
	{
		m_EulerImplementation->TimesEquals( *this, f );
	}
// tolua_end

	/// Setup the implemenation for this Euler lib
	static void InitImplementation();

private:

	/// Euler implementation
	static IEulerImplementation *m_EulerImplementation;

// tolua_begin
};
// tolua_end

#endif	// _CLUAEULERANGLE_H_