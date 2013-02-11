///============================================================================
/// \file		CErrorStream.cpp
/// \brief		Implementations of Novodex's NxUserOutputStream interface
/// \date		12-08-2006
/// \author		Kyle Swaim
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

#ifndef CERRORSTREAM_H
#define CERRORSTREAM_H

#include "NxUserOutputStream.h"

///============================================================================
/// \class	CErrorStream
/// \brief	Handles PhysX error messages.
///============================================================================
class CErrorStream : public NxUserOutputStream
{
public:
	/// Default Constructor.
	CErrorStream();

	/// Destructor
	virtual ~CErrorStream();

public:
	/// Report a PhysX error through EE's error system.
	/// \param code = Error code.
	/// \param message = Error message.
	/// \param file = File error occured in.
	/// \param line = Line error occured in.
	virtual void reportError( NxErrorCode code, const char* message, const char* file, int line );

	/// Report a PhysX assert through EE's error system.
	/// \param code = Error code.
	/// \param message = Error message.
	/// \param file = File error occured in.
	/// \param line = Line error occured in.
	virtual NxAssertResponse reportAssertViolation( const char* message, const char* file, int line );
	
	/// Print a message.
	virtual void print( const char* message ) {};

private:
	/// \brief	Pointer to the tool box
	IToolBox* m_ToolBox;
};

#endif	// CERRORSTREAM_H
