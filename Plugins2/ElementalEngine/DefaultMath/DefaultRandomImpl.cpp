///==========================================================================
/// \file	DefaultRandomImpl.cpp
/// \brief	
/// \date	09/01/2006
/// \author	Kyle Swaim
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

///==========================================================================

#include "stdafx.h"
#include "DefaultRandomImpl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IRandomImplementation *CDefaultRandomImplementation::s_instance = NULL;

unsigned long CDefaultRandomImplementation::state[n] = {0x0UL};
int CDefaultRandomImplementation::p = 0;
bool CDefaultRandomImplementation::init = false;

void CDefaultRandomImplementation::Init( void )
{
	UINT64 time;
	QueryPerformanceCounter((LARGE_INTEGER *)&time);
	if (time > 0)
	{
		SetSeed((unsigned long) time);
	}
	else
	{
		EngineGetToolBox()->Log(LOGFATALERROR, _T("Random: could not set the seed!\n"));
		assert(0);
	}
}

void CDefaultRandomImplementation::SetSeed( unsigned long seed )
{
	state[0] = seed & 0xFFFFFFFFUL; // for > 32 bit machines
	for( int i = 1; i < n; ++i )
	{
		state[i] = 1812433253UL * (state[i - 1] ^ (state[i - 1] >> 30)) + i;
		// see Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier
		// in the previous versions, MSBs of the seed affect only MSBs of the array state
		// 2002/01/09 modified by Makoto Matsumoto
		state[i] &= 0xFFFFFFFFUL; // for > 32 bit machines
	}
	p = n; // force gen_state() to be called for next random number	
}

void CDefaultRandomImplementation::SetSeed( const unsigned long* seedArray, int size )
{
	SetSeed(19650218UL);
	int i = 1, j = 0;
	for (int k = ((n > size) ? n : size); k; --k)
	{
		state[i] = (state[i] ^ ((state[i - 1] ^ (state[i - 1] >> 30)) * 1664525UL)) + seedArray[j] + j; // non linear
		state[i] &= 0xFFFFFFFFUL; // for > 32 bit machines
		++j; j %= size;
		if ((++i) == n) { state[0] = state[n - 1]; i = 1; }
	}
	for (int k = n - 1; k; --k)
	{
		state[i] = (state[i] ^ ((state[i - 1] ^ (state[i - 1] >> 30)) * 1566083941UL)) - i;
		state[i] &= 0xFFFFFFFFUL; // for > 32 bit machines
		if ((++i) == n) { state[0] = state[n - 1]; i = 1; }
	}
	state[0] = 0x80000000UL; // MSB is 1; assuring non-zero initial array
	p = n; // force gen_state() to be called for next random number
}

unsigned long CDefaultRandomImplementation::GetRandomNumber()
{
	if( p == n )
		GenerateState(); // new state vector needed
	// gen_state() is split off to be non-inline, because it is only called once
	// in every 624 calls and otherwise irand() would become too big to get inlined
	unsigned long x = state[p++];
	x ^= (x >> 11);
	x ^= (x << 7) & 0x9D2C5680UL;
	x ^= (x << 15) & 0xEFC60000UL;
	return x ^ (x >> 18);
}

double CDefaultRandomImplementation::GetRandom()
{
	return static_cast<double>(GetRandomNumber()) * (1. / 4294967296.); 
}

unsigned long CDefaultRandomImplementation::Twiddle(unsigned long u, unsigned long v)
{
	return (((u & 0x80000000UL) | (v & 0x7FFFFFFFUL)) >> 1)^ ((v & 1UL) ? 0x9908B0DFUL : 0x0UL);
}

void CDefaultRandomImplementation::GenerateState()
{
	for( int i = 0; i < (n - m); ++i )
	{
		state[i] = state[i + m] ^ Twiddle(state[i], state[i + 1]);
	}
	for( int i = n - m; i < (n - 1); ++i )
	{
		state[i] = state[i + m - n] ^ Twiddle(state[i], state[i + 1]);
	}
	state[n - 1] = state[m - 1] ^ Twiddle(state[n - 1], state[0]);
	p = 0; // reset position
}
