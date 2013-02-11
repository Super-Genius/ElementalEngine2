///==========================================================================
/// \file	IRandImplementation.h
/// \brief	Random math function implementation interface
/// \date	9\01\2006
/// \author	Kyle Swaim
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
///==========================================================================

#ifndef	IRANDOMIMPLEMENTATION_H
#define	IRANDOMIMPLEMENTATION_H

namespace ElementalEngine
{

class Random;

///==========================================================================
/// \class	IRandomImplementation
/// \brief	Interface for different Random math implementations
///==========================================================================
class __declspec(novtable) IRandomImplementation
{
public:
	/// Set the seed for the Random number generator to use
	/// \param seed - the seed
	virtual void SetSeed(unsigned long seed) = 0;
	/// Set the seed for the Random number generator to use
	/// \param seedArray - an array of seed values
	/// \param size - the size of seedArray
	virtual void SetSeed(const unsigned long* seedArray, int size) = 0;

	/// Generate 32 bit random integer
	virtual unsigned long GetRandomNumber() = 0;
	/// Get a random number between 0 and 1
	virtual double GetRandom() = 0;
};

} //namespace ElementalEngine

#endif // IRANDOMIMPLEMENTATION_H