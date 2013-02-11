///============================================================================
/// \file		EEStdStreamBufProxy.h
/// \brief		Header file for wrapping std::streambuf to map to EE Streams
/// \date		11-16-2006
/// \author		Kenneth Hurley
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
///============================================================================

#ifndef _EESTDSTREAMBUFPROXY_H_
#define _EESTDSTREAMBUFPROXY_H_

#include <streambuf>
#pragma warning(disable : 4996) // deprecated function
using namespace std;

typedef streambuf::pos_type        pos_type;
typedef streambuf::off_type        off_type;
typedef ios::seekdir               seekdir;
typedef ios::openmode              openmode;

namespace ElementalEngine
{
class IEEStream;
class IToolBox;

class CEEStdStreamBufProxy : public streambuf
{
private:
	/// the EE stream to send/get data from
	IEEStream *m_EEStream;
	/// the buffer size to buffer TCHARS
	unsigned int m_BufSize;
	/// the buffer size to reserve for ungets
	unsigned int m_Reserved;
	/// the actual allocated buffer
	TCHAR *m_Buffer;
	/// pointer for convenience for toolbox
	IToolBox *m_ToolBox;
	/// the base pointer to unreserved data
	TCHAR *m_Base;

public:
	/// constructor for Proxy Buffer
	/// \param eeSteam = the EE stream to proxy through std:streambuf
	/// \param bufSize = the buffer size to allocate
	/// \param reserved = the number of ungets possible
	CEEStdStreamBufProxy(IEEStream *eeStream, unsigned int bufsize = 256, unsigned int reservered = 128);
	/// standard virtual destructor for Proxy Buffer
	~CEEStdStreamBufProxy(void);

protected:
	/// overflow is output function for streambuf
	/// \param c = character to output
	/// \return EOF if EOF, else return c
	virtual int overflow(int c);

	/// underflow is the input function for streambuf
	/// \return EOF, or character read
	virtual int underflow();

	/// xsgetn grams a number of characters from the stream
	/// \param dest = destination of characters to output
	/// \param sSize = size to read from the stream
	/// \return size read as a streamsize type
	virtual streamsize xsgetn(TCHAR *dest, streamsize sSize);

	/// seekpos seeks to an offset from the beginning of the file
	/// \param offset = offset inside the stream
	/// \param mode = write and/or read mode allowed for 
	///				specified for stream
	/// \return = resulting stream position
	virtual pos_type seekpos(pos_type offset, openmode mode);

	/// seekoff seeks to an offset based on direction
	/// \param offset = offset from dir
	/// \param dir = from beginning, current or end of stream
	/// \param mode = write and/or read mode allowed for 
	///				specified for stream
	/// \return = resulting stream position
	virtual pos_type seekoff(off_type offset, seekdir dir, openmode mode);

	/// sync makes sure internal buffers are synced with external file
	/// \return int = 0 for success
	int sync();

};

} // namespace ElementalEngine

#endif // #ifndef _EESTDSTREAMBUFPROXY_H_
