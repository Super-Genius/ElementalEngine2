///============================================================================
/// \file		MemoryStream.cpp
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

#include "StdAfx.h"

#include "NxPhysics.h"
#include "MemoryStream.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

MemoryWriteStream::MemoryWriteStream() : currentSize(0), maxSize(0), data(NULL)
{
}

MemoryWriteStream::~MemoryWriteStream()
{
	NX_FREE(data);
}

NxStream& MemoryWriteStream::storeByte(NxU8 b)
{
	storeBuffer(&b, sizeof(NxU8));
	return *this;
}
NxStream& MemoryWriteStream::storeWord(NxU16 w)
{
	storeBuffer(&w, sizeof(NxU16));
	return *this;
}
NxStream& MemoryWriteStream::storeDword(NxU32 d)
{
	storeBuffer(&d, sizeof(NxU32));
	return *this;
}
NxStream& MemoryWriteStream::storeFloat(NxReal f)
{
	storeBuffer(&f, sizeof(NxReal));
	return *this;
}
NxStream& MemoryWriteStream::storeDouble(NxF64 f)
{
	storeBuffer(&f, sizeof(NxF64));
	return *this;
}
NxStream& MemoryWriteStream::storeBuffer(const void* buffer, NxU32 size)
{
	NxU32 expectedSize = currentSize + size;
	if(expectedSize > maxSize)
	{
		maxSize = expectedSize + 4096;

		NxU8* newData = (NxU8*)NX_ALLOC(maxSize);
		if(data)
		{
			memcpy(newData, data, currentSize);
			NX_FREE(data);
		}
		data = newData;
	}
	memcpy(data+currentSize, buffer, size);
	currentSize += size;
	return *this;
}


MemoryReadStream::MemoryReadStream(const NxU8* data) : buffer(data)
{
}

MemoryReadStream::~MemoryReadStream()
{
	// We don't own the data => no delete
}

NxU8 MemoryReadStream::readByte() const
{
	NxU8 b;
	memcpy(&b, buffer, sizeof(NxU8));
	buffer += sizeof(NxU8);
	return b;
}

NxU16 MemoryReadStream::readWord() const
{
	NxU16 w;
	memcpy(&w, buffer, sizeof(NxU16));
	buffer += sizeof(NxU16);
	return w;
}

NxU32 MemoryReadStream::readDword() const
{
	NxU32 d;
	memcpy(&d, buffer, sizeof(NxU32));
	buffer += sizeof(NxU32);
	return d;
}

float MemoryReadStream::readFloat() const
{
	float f;
	memcpy(&f, buffer, sizeof(float));
	buffer += sizeof(float);
	return f;
}

double MemoryReadStream::readDouble() const
{
	double f;
	memcpy(&f, buffer, sizeof(double));
	buffer += sizeof(double);
	return f;
}

void MemoryReadStream::readBuffer(void* dest, NxU32 size) const
{
	memcpy(dest, buffer, size);
	buffer += size;
}