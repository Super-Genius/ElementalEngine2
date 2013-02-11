///==========================================================================
/// \file	DefaultRandomImpl.h
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

#ifndef DEFAULTRANDOMIMPL_H__
#define DEFAULTRANDOMIMPL_H__

#include "IRandomImplementation.h"
#include "Random.h"

namespace ElementalEngine
{

class CDefaultRandomImplementation : public IRandomImplementation
{
private:
	void Init( void );
	void CleanUp( void ) {};

	CDefaultRandomImplementation()
	{
		Init();
	}

	virtual ~CDefaultRandomImplementation()
	{
		CleanUp();
	}

public:
	static IRandomImplementation* Instance( void )
	{
		if ( s_instance == NULL )
		{
			s_instance = new CDefaultRandomImplementation();
		}
		return s_instance;
	}
	static void Destroy( void )
	{
		if ( s_instance != NULL )
		{
			delete s_instance;
		}
		s_instance = NULL;
	}

	/// Set the seed for the Random number generator to use
	/// \param seed - the seed
	virtual void SetSeed( unsigned long seed );
	/// Set the seed for the Random number generator to use
	/// \param seedArray - an array of seed values
	/// \param size - the size of seedArray
	virtual void SetSeed( const unsigned long* seedArray, int size );

	/// Get a random number
	virtual unsigned long GetRandomNumber();
	/// Get a random number between 0 and 1
	virtual double GetRandom();

private:

	// used by gen_state()
	unsigned long Twiddle(unsigned long u, unsigned long v);
	// generate new state
	void GenerateState();	

	// compile time constants
	static const int n = 624, m = 397; 

	// state vector array
	static unsigned long state[n];

	// position in state array
	static int p; 

	// true if init function is called
	static bool init; 

	// Pointer to the instance
	static IRandomImplementation* s_instance;
};

} // namespace ElementalEngine

#endif // DEFAULTRANDOMIMPL_H__