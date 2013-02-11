///============================================================================
/// \file		CThreadedOggDecoder.h
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

#ifndef _C_THREADEDOGGDECODER_H_
#define _C_THREADEDOGGDECODER_H_

class CThreadedOggDecoder
{
public:
	/// ctor & dtor
	CThreadedOggDecoder(IHashString *fileName, IEEStream *stream);
	~CThreadedOggDecoder();

	/// Initializes the buffer stream
	void Initialize();
	
	/// Checks to see if the thread is idle
	/// \return t/f thread idle
	bool IsThreadIdle() { return m_bThreadIdle; }

	/// Checks to see if the buffer is done loading
	/// \return t/f done loading
	bool IsBufferFilled() { return m_bBufferFilled; }

	/// Checks to see if we've hit the end of the stream
	/// return t/f end of stream
	bool IsEndOfStream() 
	{ 
		if (m_bBufferFilled)
		{
			return false;
		}
		return m_bEndOfStream;
	}

	/// Sets the al buffer to fill
	/// \param buffer = buffer to fill
	void SetBuffer(ALuint buffer) { m_Buffer = buffer; }

	/// Wakes up the sleeping thread to start decoding into the buffer
	/// \param bWaitForCompletion = t/f lock main thread until the buffer is loaded
	/// \param bLoadWholeBuffer = t/f buffer entire ogg stream, or buffer only specified buffer size
	void RunThread(bool bWaitForCompletion, bool bLoadWholeBuffer);

	/// Sleeps the main thread if the decoding thread is running and we want to wait for it
	void WaitForRunningThread();

	/// Actual multithreaded decoding fn
	void DecodeBuffer();

	/// Resets the stream (for looping)
	void ResetStream();

	/// Gets the info for the sound
	/// \param frequency = frequency in hz
	/// \param bitDepth = bit depth in bits
	/// \param bitRate = bit rate in bits per sec
	/// \param channels = number of channels
	/// \param fileSize = file size in bytes
	/// \param duration = duration of sound in seconds
	void GetSoundInfo(UINT *frequency, UINT *bitDepth, UINT *bitRate, UINT *channels, UINT *fileSize, float *duration);

private:

	/// Creates an event to wake up the sleeping decoder thread
	void CreateWakeUpEvent();

	/// Creates an event to wake up the main thread, if set to lock
	void CreateBufferCompleteEvent();

	/// Creates the thread to run the decoder on
	void CreateThread();

	/// Destroys and cleans up thread and events
	void DestroyThreadAndEvent();

	COpenALSoundUtility *m_SoundUtility;

	bool m_bBufferFilled;
	bool m_bThreadIdle;
	bool m_bThreadAlive;
	HANDLE m_Thread;
	HANDLE m_WakeUpEvent;
	HANDLE m_BufferCompleteEvent;
	ALuint m_Buffer;

	// vorbis data
	OggVorbis_File m_OggVorbisFile;
	vorbis_info m_FileInfo;
	ov_callbacks m_VorbisCallbacks;

	long m_iBitrate;

	ALenum m_ALAudioFormat;

	TCHAR* GetVorbisErrorString(int err);

	// vorbis callback overrides
	static size_t VorbisRead(void *ptr, size_t size, size_t nmemb, void *datasource);
	static int VorbisSeek(void *datasource, ogg_int64_t offset, int whence);
	static int VorbisClose(void *datasource);
	static long VorbisTell(void *datasource);

	long m_CurrentPosition;
	DWORD m_dwFileKey;
	bool m_bEndOfStream;
	IEEStream *m_SoundDataStream;
	bool m_bLoadWholeBuffer;
};

#endif //_C_THREADEDOGGDECODER_H_