///============================================================================
/// \file		COGGSoundBufferStream.cpp
/// \brief		Implementation of IBufferStream for ogg streams
/// \date		03/14/2007
/// \author		Nick Tourte
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

#include "stdafx.h"

COGGSoundBufferStream::COGGSoundBufferStream(IHashString *fileName, IEEStream *stream)
{
	IToolBox *toolBox = EngineGetToolBox();	
	static CHashString sm(_T("COpenALSoundUtility"));

	m_Decoder = new CThreadedOggDecoder(fileName, stream);
	
	// Get soundManager and cast it as an OpenAL sound manager
	m_SoundUtility = dynamic_cast<COpenALSoundUtility*>(toolBox->CreateComponent(&sm, 0));
	if (m_SoundUtility == NULL)
	{
		toolBox->Log(LOGWARNING, _T("OGGSoundBuffer: Can't get sound manager!\n"));		
	}	

	m_bWaitingForBuffer = false;
	m_iLoadingBuffer = 0;	
}

COGGSoundBufferStream::~COGGSoundBufferStream()
{
	// delete buffers now!
	for (UINT i=0; i<m_FreeBuffers.size(); i++)
	{
		alDeleteBuffers(1, &m_FreeBuffers[i]);
	
		ALenum err = alGetError();
		if (err != AL_NO_ERROR)
		{
			EngineGetToolBox()->Log(LOGWARNING, _T("OGG sound buffer: could not free al buffer: %s\n"), 
				GetALErrorString(err));
		}		
	}

	m_FreeBuffers.clear();	

	if (m_Decoder)
	{
		delete m_Decoder;
		m_Decoder = NULL;
	}
}

bool COGGSoundBufferStream::GetBuffer(ALuint *buffer, bool bWaitForBuffer)
{
	bool ret = true;
	if (m_Decoder)
	{
		if (m_bWaitingForBuffer)
		{
			if (!bWaitForBuffer)
			{
				if (!m_Decoder->IsThreadIdle())
				{
					// not done yet
					ret = false;
				}
				else if (m_Decoder->IsBufferFilled())
				{
					// good to go
					*buffer = m_iLoadingBuffer;
					m_bWaitingForBuffer = false;
					ret = true;
				}
				else if (IsEndOfStream())
				{
					m_bWaitingForBuffer = false;
					ret = false;
				}
			}
			else
			{
				if (!m_Decoder->IsThreadIdle())
				{
					m_Decoder->WaitForRunningThread();

					// NOW should be done
					if (m_Decoder->IsBufferFilled())
					{
						*buffer = m_iLoadingBuffer;
						m_bWaitingForBuffer = false;
						ret = true;
					}
					else
					{
						// should never end up here...
						m_bWaitingForBuffer = false;
						ret = false;
					}
				}
				else if (m_Decoder->IsBufferFilled())
				{
					// good to go
					*buffer = m_iLoadingBuffer;
					m_bWaitingForBuffer = false;
					ret = true;
				}
			}
		}
		else
		{
			if (m_FreeBuffers.size() > 0)
			{
				m_iLoadingBuffer = m_FreeBuffers[0];
				m_FreeBuffers.erase(m_FreeBuffers.begin());
			}
			else
			{
				alGenBuffers(1, &m_iLoadingBuffer);
			}

			IToolBox *toolBox = EngineGetToolBox();

			m_Decoder->SetBuffer(m_iLoadingBuffer);
			m_Decoder->RunThread(bWaitForBuffer, false);

			if (bWaitForBuffer)
			{
				if (m_Decoder->IsBufferFilled())
				{
					*buffer = m_iLoadingBuffer;
					ret = true;
				}
				else
				{
					// should never end up here...
					ret = false;
				}
			}
			else
			{
				// PROBABLY not ready yet, come back tomorrow
				m_bWaitingForBuffer = true;
				ret = false;
			}
		}
	}
	else
	{
		// no decoder for some reason... return bad buffer
		*buffer = 0;
	}

	// now pre-emptively buffer the next one, if we're not waiting for it...
	if (!m_bWaitingForBuffer)
	{
		if (!IsEndOfStream())
		{
			GetBuffer(&m_iLoadingBuffer, false);
		}
	}
	return ret;
}

void COGGSoundBufferStream::DisposeOldBuffer(ALuint buffer)
{
	// TECHNICALLY this buffer is still in memory now... maybe should reevaluate logic below
	int size=0;
	if (m_SoundUtility->GetALBufferI(buffer, AL_SIZE, &size, __FILE__, __LINE__))
	{
		m_SoundUtility->RemoveALMemUsageBytes((UINT)size);
	}

	m_FreeBuffers.push_back(buffer);
}

ALuint COGGSoundBufferStream::BufferFullSound()
{
	if (m_Decoder)
	{
		if (m_FreeBuffers.size() > 0)
		{
			m_iLoadingBuffer = m_FreeBuffers[0];
			m_FreeBuffers.erase(m_FreeBuffers.begin());
		}
		else
		{
			alGenBuffers(1, &m_iLoadingBuffer);
		}

		IToolBox *toolBox = EngineGetToolBox();

		m_Decoder->SetBuffer(m_iLoadingBuffer);
		m_Decoder->RunThread(true, true);
	}
	return m_iLoadingBuffer;
}