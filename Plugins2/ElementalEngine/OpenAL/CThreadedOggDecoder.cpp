///============================================================================
/// \file		CThreadedOggDecoder.cpp
/// \brief		Class to wrap multithreaded decoding of ogg streams
/// \date		05/05/2008
/// \author		Nick Tourte
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
// we need platform generic versions of these
#if defined(WIN32) || defined(XBOX)
#include "process.h"

static DWORD WINAPI ThreadMain(void *param)
{
	// assume param is CThreadedOggDecoder ptr
	CThreadedOggDecoder *decoder = (CThreadedOggDecoder*)param;
	if (decoder)
	{
		decoder->DecodeBuffer();
	}

	return 0;
}

#endif

CThreadedOggDecoder::CThreadedOggDecoder(IHashString *fileName, IEEStream *stream)
{
	m_bBufferFilled = false;
	m_bThreadIdle = true;
	m_bThreadAlive = true;
	m_Thread = NULL;
	m_Buffer = 0;
	m_WakeUpEvent = NULL;
	m_BufferCompleteEvent = NULL;

	if (fileName)
	{
		m_dwFileKey = fileName->GetUniqueID();
	}
	m_bEndOfStream = false;
	m_SoundDataStream = stream;
	m_CurrentPosition = 0;
	m_iBitrate = 0;

	m_bLoadWholeBuffer = false;

	// setup callbacks
	m_VorbisCallbacks.read_func = VorbisRead;
	m_VorbisCallbacks.seek_func = VorbisSeek;
	m_VorbisCallbacks.close_func = VorbisClose;
	m_VorbisCallbacks.tell_func = VorbisTell;

	IToolBox *toolBox = EngineGetToolBox();
	static CHashString sm(_T("COpenALSoundUtility"));
	
	// Get soundManager and cast it as an OpenAL sound manager
	m_SoundUtility = dynamic_cast<COpenALSoundUtility*>(toolBox->CreateComponent(&sm, 0));
	if (m_SoundUtility == NULL)
	{
		toolBox->Log(LOGWARNING, _T("OGGSoundBuffer: Can't get sound manager!\n"));
	}

	Initialize();
}
	
CThreadedOggDecoder::~CThreadedOggDecoder()
{
	DestroyThreadAndEvent();

	int err;
	err = ov_clear(&m_OggVorbisFile);
	if (err != 0)
	{
		EngineGetToolBox()->Log(LOGWARNING, _T("Ogg sound buffer: failed to clear the stream: %s\n"), 
			GetVorbisErrorString(err));
		return;
	}
}

void CThreadedOggDecoder::Initialize()
{
	IToolBox *toolBox = EngineGetToolBox();
	if (m_SoundUtility == NULL)
	{
		toolBox->Log(LOGWARNING, _T("OGG sound buffer: cannot initialize; null manager!\n"));
		return; 
	}
	if (m_SoundDataStream == NULL)
	{
		m_SoundDataStream = m_SoundUtility->GetSoundStreamData(m_dwFileKey);
		if (m_SoundDataStream == NULL)
		{
			toolBox->Log(LOGWARNING, _T("OGG sound buffer: could not get data stream from manager!\n"));		
			return;
		}					
	}

	// dunno why the ov loader doesn't do this on open...
	m_SoundDataStream->SeekTo(0, SEEK_SET);

	int err;
	err = ov_open_callbacks((void*) m_SoundDataStream, &m_OggVorbisFile, NULL, 0, m_VorbisCallbacks);
	if (err != 0)
	{
		toolBox->Log(LOGWARNING, _T("OGG sound buffer: failed to open the stream: %s\n"), 
			GetVorbisErrorString(err));
		return;
	}

	m_FileInfo = *ov_info(&m_OggVorbisFile, -1);

	m_iBitrate = ov_bitrate(&m_OggVorbisFile, -1);

	if (m_FileInfo.channels == 1)
	{
		m_ALAudioFormat = AL_FORMAT_MONO16;
	}
	else
	{
		m_ALAudioFormat = AL_FORMAT_STEREO16;
	}

	m_bEndOfStream = false;
	m_CurrentPosition = m_SoundDataStream->Tell();

	CreateWakeUpEvent();
	CreateBufferCompleteEvent();
	CreateThread();
}
	
void CThreadedOggDecoder::DecodeBuffer()
{
	IToolBox *toolBox = EngineGetToolBox();
	m_bThreadIdle = true;
#if defined(WIN32) || defined(XBOX)
	WaitForSingleObject(m_WakeUpEvent, INFINITE);
#endif

	while (m_bThreadAlive)
	{
		bool bTripEndOfStream = false;
		
		// need to seek to and reset everything because more than one streaming sound can use the same ogg stream
		m_SoundDataStream->SeekTo(m_CurrentPosition);
		
		UINT size = 0;
		int bytesRead, section;
		
		UINT maxBufSize = 0;

		if (m_bLoadWholeBuffer)
		{
			ogg_int64_t sampleCount = ov_pcm_total(&m_OggVorbisFile, -1);
			// oggs are pretty much always 16 bit, so 16/8 = 2
			sampleCount *= 2 * m_FileInfo.channels;
			maxBufSize = (UINT)sampleCount;
		}
		else
		{
			// buffer size in bytes = duration * bitrate * channels / 8 (bits to bytes)
			float bufferLength = m_SoundUtility->GetBufferDuration();
			maxBufSize = (UINT)((bufferLength * (float)m_FileInfo.rate * 16.0f * (float)m_FileInfo.channels) / 8.0f);
		}
		char *data = new char[maxBufSize];

		bool bHasData = true;

		// read into buffer
		while (size < maxBufSize)
		{
#ifdef XBOX
			// big endian for xbox
			bytesRead = ov_read(&m_OggVorbisFile, data + size, maxBufSize - size, 1, 2, 1, &section);
#else
			bytesRead = ov_read(&m_OggVorbisFile, data + size, maxBufSize - size, 0, 2, 1, &section);
#endif

			if (bytesRead > 0)
			{
				size += bytesRead;
			}
			else if (bytesRead < 0)
			{
				toolBox->Log(LOGWARNING, _T("OGG sound buffer: failed to read from stream: %s\n"),
					GetVorbisErrorString(bytesRead));
				m_CurrentPosition = m_SoundDataStream->Tell();
				bHasData = false;
			}
			else // 0 bytes read, is 0
			{
				bTripEndOfStream = true;
				break;
			}
		}	

		if (size == 0)
		{
			m_CurrentPosition = m_SoundDataStream->Tell();
			bHasData = false; // out of data
		}
			 
		if (bHasData)
		{
			ALenum err;

			alBufferData(m_Buffer, m_ALAudioFormat, data, size, m_FileInfo.rate);

			err = alGetError();
			if (err != AL_NO_ERROR)
			{
				toolBox->Log(LOGWARNING, _T("OGG sound buffer: could not load streamed data into al buffer: %s \n\tbuffer: %d\n"),
					GetALErrorString(err), m_Buffer);
				m_CurrentPosition = m_SoundDataStream->Tell();
			}

			m_SoundUtility->AddALMemUsageBytes(size);

			m_CurrentPosition = m_SoundDataStream->Tell();
			m_bBufferFilled = true;
		}
		else
		{
			m_bBufferFilled = false;
		}
		
		delete [] data;
		m_bThreadIdle = true;
		m_bEndOfStream = bTripEndOfStream;

#if defined(WIN32) || defined(XBOX)
		SetEvent(m_BufferCompleteEvent);
		WaitForSingleObject(m_WakeUpEvent, INFINITE);
#endif
	}
}

void CThreadedOggDecoder::CreateWakeUpEvent()
{
#if defined(WIN32) || defined(XBOX)
	m_WakeUpEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
#endif
	if (m_WakeUpEvent == NULL)
	{
#if defined(WIN32) || defined(XBOX)
		DWORD err = GetLastError();
		EngineGetToolBox()->Log(LOGERROR, _T("CThreadedOggDecoder: unable to create wake up event! Error: %d\n"), err);
#endif
	}

#if defined(WIN32) || defined(XBOX)
	ResetEvent(m_WakeUpEvent);
#endif
}

void CThreadedOggDecoder::CreateBufferCompleteEvent()
{
#if defined(WIN32) || defined(XBOX)
	m_BufferCompleteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (m_BufferCompleteEvent == NULL)
	{
		DWORD err = GetLastError();
		EngineGetToolBox()->Log(LOGERROR, _T("CThreadedOggDecoder: unable to create buffer complete event! Error: %d\n"), err);
	}

	ResetEvent(m_BufferCompleteEvent);
#endif
}
	
void CThreadedOggDecoder::CreateThread()
{
#if defined(WIN32) || defined(XBOX)
	m_Thread = (HANDLE)::CreateThread(NULL, 0, ThreadMain, this, CREATE_SUSPENDED, NULL);
	if (m_Thread)
	{
#ifdef XBOX
		EEXBOX_SETTHREADCORE( m_Thread, 1 );
#endif
		ResumeThread( m_Thread );
	}
	else
	{
		EngineGetToolBox()->Log(LOGERROR, _T("CThreadedOggDecoder: unable to create thread! Error: %s\n"), 
			strerror(errno));
	}
#endif
}

void CThreadedOggDecoder::RunThread(bool bWaitForCompletion, bool bLoadWholeBuffer)
{
#if defined(WIN32) || defined(XBOX)
	m_bThreadIdle = false;
	m_bBufferFilled = false;
	m_bLoadWholeBuffer = bLoadWholeBuffer;
	ResetEvent(m_BufferCompleteEvent);
	SetEvent(m_WakeUpEvent);
	if (bWaitForCompletion)
	{
		WaitForSingleObject(m_BufferCompleteEvent, INFINITE);
	}
#endif
}

void CThreadedOggDecoder::WaitForRunningThread()
{
#if defined(WIN32) || defined(XBOX)
	WaitForSingleObject(m_BufferCompleteEvent, INFINITE);
#endif
}

void CThreadedOggDecoder::DestroyThreadAndEvent()
{
#if defined(WIN32) || defined(XBOX)
	if (m_WakeUpEvent && m_Thread && m_BufferCompleteEvent)
	{
		if (!m_bThreadIdle)
		{
			WaitForSingleObject(m_BufferCompleteEvent, INFINITE);
		}

		m_bThreadAlive = false;
		SetEvent(m_WakeUpEvent);

		WaitForSingleObject(m_Thread, INFINITE);

		CloseHandle(m_WakeUpEvent);
		CloseHandle(m_BufferCompleteEvent);
		CloseHandle(m_Thread);
		m_WakeUpEvent = NULL;
		m_BufferCompleteEvent = NULL;
		m_Thread = NULL;
	}
#endif
}

void CThreadedOggDecoder::ResetStream()
{
#if defined(WIN32) || defined(XBOX)
	if (m_SoundDataStream)
	{
		if (!IsThreadIdle())
		{
			// just in case
			WaitForRunningThread();
		}
		IToolBox *toolBox = EngineGetToolBox();
		
		int err;
		err = ov_time_seek(&m_OggVorbisFile, 0.0);
		if (err != 0)
		{
			toolBox->Log(LOGWARNING, _T("Ogg sound buffer: failed to reset the stream: %s\n"), 
				GetVorbisErrorString(err));
			return;
		}

		m_CurrentPosition = m_SoundDataStream->Tell();

		m_bEndOfStream = false;
		m_bBufferFilled = false;
		ResetEvent(m_BufferCompleteEvent);
	}
#endif
}

void CThreadedOggDecoder::GetSoundInfo(UINT *frequency, UINT *bitDepth, UINT *bitRate, UINT *channels, UINT *fileSize, float *duration)
{
	*frequency = (UINT) m_FileInfo.rate;
	*channels = (UINT) m_FileInfo.channels;
	*duration = (float) ov_time_total(&m_OggVorbisFile, -1);
	*bitRate = (UINT) m_iBitrate;
}

TCHAR* CThreadedOggDecoder::GetVorbisErrorString(int err)
{
	switch(err)
	{
		case OV_EREAD:
		{
			return _T("Read error occured");			
		}
		case OV_ENOTVORBIS:
		{
			return _T("Not Vorbis data");
		}
		case OV_EVERSION:
		{
			return _T("Bad Vorbis version");
		}
		case OV_EBADHEADER:
		{
			return _T("Bad Vorbis header");
		}
		case OV_EFAULT:
		{
			return _T("Internal fault (heap or stack corruption)");
		}
		default:
		{
			return _T("Some unknown error has occured");
		}
	}
}

size_t CThreadedOggDecoder::VorbisRead(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	IEEStream *stream = static_cast<IEEStream*>(datasource);
	if (stream == NULL)
	{
		EngineGetToolBox()->Log(LOGWARNING, _T("OGG buffer stream: failed to cast to IEEStream!\n"));
		return 0;
	}
	
	size_t ret = 0;

	UINT totalSize = size * nmemb;
	ret += stream->Read(ptr, totalSize);
	return ret;
}

int CThreadedOggDecoder::VorbisSeek(void *datasource, ogg_int64_t offset, int whence)
{
	IEEStream *stream = static_cast<IEEStream*>(datasource);
	if (stream == NULL)
	{
		EngineGetToolBox()->Log(LOGWARNING, _T("OGG buffer stream: failed to cast to IEEStream!\n"));
		return 0;
	}

	return (int)stream->SeekTo((DWORD)offset, whence);	
}

int CThreadedOggDecoder::VorbisClose(void *datasource)
{
	// don't let vorbis close the stream
	return 0;
}

long CThreadedOggDecoder::VorbisTell(void *datasource)
{
	IEEStream *stream = static_cast<IEEStream*>(datasource);
	if (stream == NULL)
	{
		EngineGetToolBox()->Log(LOGWARNING, _T("OGG buffer stream: failed to cast to IEEStream!\n"));
		return 0;
	}

	return stream->Tell();
}