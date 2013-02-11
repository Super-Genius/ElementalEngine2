///============================================================================
/// \file		MemoryStream.h
/// \brief		Implementations of Novodex's NxStream interface
///				Only for use with physics
/// \date		7-7-2005
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

#ifndef PHYSSTREAM_H
#define PHYSSTREAM_H

#include "NxStream.h"

///============================================================================
/// \class	MemoryReadStream
/// \brief	Reads data from a NxStream. This class is only for internal
/// physics plugin use.
///============================================================================
class MemoryReadStream : public NxStream
{
public:
	/// Constructor
	MemoryReadStream(const NxU8* data);

	/// Destructor
	virtual ~MemoryReadStream();

	/// Read a byte.
	virtual	NxU8 readByte() const;

	/// Read a word.
	virtual	NxU16 readWord() const;

	/// Read a dword.
	virtual	NxU32 readDword() const;

	/// Read a float.
	virtual	float readFloat() const;

	/// Read a double.
	virtual	double readDouble() const;

	/// Read a buffer.
	virtual	void readBuffer(void* buffer, NxU32 size) const;

	/// Store a byte.
	virtual	NxStream& storeByte(NxU8 b) { NX_ASSERT(0); return *this; }

	/// Store a word.
	virtual	NxStream& storeWord(NxU16 w) { NX_ASSERT(0); return *this; }

	/// Store a dword.
	virtual	NxStream& storeDword(NxU32 d) { NX_ASSERT(0); return *this; }
	
	/// Store a float.
	virtual	NxStream& storeFloat(NxReal f) { NX_ASSERT(0); return *this; }

	/// Store a double.
	virtual	NxStream& storeDouble(NxF64 f) { NX_ASSERT(0); return *this; }

	/// Store a buffer.
	virtual	NxStream& storeBuffer(const void* buffer, NxU32 size) { NX_ASSERT(0); return *this; }

	mutable	const NxU8*	buffer;
};

///============================================================================
/// \class	MemoryWriteStream
/// \brief	Writes data to a NxStream. This class is only for internal
/// physics plugin use.
///============================================================================
class MemoryWriteStream : public NxStream
{
public:
	/// Constructor
	MemoryWriteStream();

	/// Destructor
	virtual ~MemoryWriteStream();

	/// Read a byte
	virtual	NxU8 readByte() const { NX_ASSERT(0); return 0; }

	/// Read a word
	virtual	NxU16 readWord() const { NX_ASSERT(0); return 0; }

	/// Read a dword
	virtual	NxU32 readDword() const { NX_ASSERT(0); return 0; }

	/// Read a float
	virtual	float readFloat() const { NX_ASSERT(0); return 0.0f; }

	/// Read a double
	virtual	double readDouble() const { NX_ASSERT(0); return 0.0; }

	/// Read a buffer
	virtual	void readBuffer(void* buffer, NxU32 size) const { NX_ASSERT(0); }

	/// Store a byte
	virtual	NxStream& storeByte(NxU8 b);

	/// Store a word
	virtual	NxStream& storeWord(NxU16 w);

	/// Store a dword
	virtual	NxStream& storeDword(NxU32 d);

	/// Store a float
	virtual	NxStream& storeFloat(NxReal f);

	/// Store a double
	virtual	NxStream& storeDouble(NxF64 f);

	/// Store a buffer
	virtual	NxStream& storeBuffer(const void* buffer, NxU32 size);

	NxU32 currentSize;
	NxU32 maxSize;
	NxU8* data;
};

#endif	// PHYSSTREAM_H
