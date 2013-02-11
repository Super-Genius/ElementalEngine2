///============================================================================
/// \file		LuaRandUtil.h
/// \brief		Header file for CLuaRandUtil.h
/// \date		09-21-2005
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

#ifndef _LUARANDUTIL_H_
#define _LUARANDUTIL_H_

// tolua_begin
class CLuaRandUtil
{
public:
	/// Default Constructor
	CLuaRandUtil();
	/// Constructor
	/// \param seed - Seed value to generate random numbers
	CLuaRandUtil(unsigned long seed);
	/// Destructor
	~CLuaRandUtil();

	/// Set the seed for the Random number generator to use
	/// \param seed - The seed tp generate random numbers
	void SetSeed(unsigned long seed);

	/// Generate 32 bit random integer
	/// \return unsigned long - generated number
	unsigned long GetRandomNumber();
	/// Generates a random number between 0 and 1
	/// \return double - generated number between 0 and 1
	double GetRandom();

// tolua_end
private:
	Random m_Random;
	/// double floating point random number in the half-open interval [0, 1]
	//MTRand	m_MTRand;
	/// double floating point random number in the closed interval [0, 1]
	//MTRand_closed m_MTRandClosed;
	/// double floating point random number in the open interval [0, 1]
	//MTRand_open m_MTRandOpen;
	/// 53 bit resolution double random number in the half-open interval	[0, 1]
	//MTRand53 m_MTRand53;
	/// Mersenne Twister random number generator
	//MTRand_int32 m_MTRandInt32;
// tolua_begin
};
// tolua_end

#endif // #ifndef _LUARANDUTIL_H_
