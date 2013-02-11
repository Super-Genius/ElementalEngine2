///============================================================================
/// \file	EEStdStreamBufProxy.cpp
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

#include "StdAfx.h"
#include "EEStdStreamBufProxy.h"
#include "IEEStream.h"

CEEStdStreamBufProxy::CEEStdStreamBufProxy(IEEStream *sourceStream, 
											unsigned int bufSize,
											unsigned int reserved)
{
	unsigned totalBufSize;

	// prevent zero sized buffer
	if (bufSize == 0)
	{
		bufSize = 1;
	}

	m_Reserved = reserved;
	m_EEStream = sourceStream;
	m_ToolBox = EngineGetToolBox();

	totalBufSize = (bufSize > reserved) ? bufSize : reserved + 1;

	m_BufSize = totalBufSize - reserved;
	try
	{
		m_Buffer = new TCHAR[totalBufSize];
	}
	catch (...)
	{
		m_ToolBox->Log(LOGFATALERROR, 
			_T("Unable to allocate %d bytes in CEEStdStreamBufProxy constructor\n"),
			totalBufSize);
	}

	m_Base = m_Buffer + m_Reserved;
	// set the read buffer
	setg(m_Base, m_Base, m_Base);
	// set the write buffer
	setp(m_Buffer, m_Buffer + m_BufSize);

}

CEEStdStreamBufProxy::~CEEStdStreamBufProxy(void)
{
	if (m_EEStream != NULL)
	{
		sync();
		delete m_EEStream;
		m_EEStream = NULL;
	}
	// kill off the buffer
	delete [] m_Buffer;
	m_BufSize = 0;
	m_Reserved = 0;
}

// ---------
int CEEStdStreamBufProxy::underflow()
{
	unsigned int used, moveCnt;
	if (gptr() < egptr())
		return *gptr();

	used = (unsigned int)(gptr() - eback());
	moveCnt = std::min(used, m_Reserved);
	if (moveCnt > 0)
	{
		memcpy(m_Base - moveCnt, egptr() - moveCnt, moveCnt);
	}

	if (!m_EEStream->IsReading())
	{
		m_ToolBox->Log(LOGERROR, _T("Attempting to read from write-only stream in %s\n"),
			__FILE__);
		return EOF;
	}

	// get how much is left in the buffer so we don't assert that we over read
	int iCur = m_EEStream->Tell();
	m_EEStream->SeekTo( 0, SEEK_END);
	int iEnd = m_EEStream->Tell();
	m_EEStream->SeekTo( iCur, SEEK_SET);
	unsigned int iReadSize = (m_BufSize < (unsigned int)(iEnd-iCur)) ? m_BufSize : (unsigned int)(iEnd-iCur);
	size_t nRead = m_EEStream->Read(m_Base, iReadSize);

	if (nRead <= 0)
	{
		return EOF;
	}

	setg(m_Base - moveCnt, m_Base, m_Base + nRead);

	return *gptr();
}

// ---------
streamsize CEEStdStreamBufProxy::xsgetn(TCHAR *dest, streamsize sSize)
{
	int nRead = 0;
	int avail;

	while (sSize)
	{
		// get number of character available
		avail = in_avail();
		if (!avail)
		{
			if (underflow() == EOF)
				break;
		}

		if (avail > sSize)
		{
			avail = sSize;
		}

		memcpy(dest + nRead, gptr(), avail);
		gbump(avail);

		nRead += avail;
		sSize -= avail;
	}

	return nRead;
}

// ---------
pos_type CEEStdStreamBufProxy::seekoff(off_type offset, seekdir dir, openmode mode)
{
	// sync in case writing to stream
	sync();

    pos_type pos = 
		m_EEStream->SeekTo(offset, 
            (dir ==  ios::beg) ? SEEK_SET :
            (dir ==  ios::cur) ? SEEK_CUR :
                                        SEEK_END
        );

    if (pos < 0)
        return -1;

    setg(m_Buffer, m_Buffer + 1, m_Buffer + 1);
    return pos;
}
    
pos_type CEEStdStreamBufProxy::seekpos(pos_type offset, openmode mode)
{
    return seekoff(offset, ios::beg, mode);
}

int CEEStdStreamBufProxy::sync()
{
	if (m_EEStream->IsWriting())
	{
		if (pptr() > pbase())
		{
			size_t writeSize;
			writeSize = pptr() - pbase();
			if (m_EEStream->Write(m_Buffer, writeSize) != writeSize)
			{
				m_ToolBox->Log(LOGWARNING, _T("Unable to write %d bytes to file\n"),
					writeSize);
				return 1;
			}
			setp(m_Buffer, m_Buffer + m_BufSize);
		}
	}

    return 0;
}


int CEEStdStreamBufProxy::overflow(int c)
{
    sync();
    if (c != EOF)
    {
        *pptr() = c;
        pbump(1);
    }
    return c;
}

