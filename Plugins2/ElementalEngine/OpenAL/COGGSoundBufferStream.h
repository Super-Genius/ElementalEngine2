///============================================================================
/// \file		COGGSoundBufferStream.h
/// \brief		Implementation of IBufferStream for ogg streams
/// \date		03/14/2007
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

#ifndef _C_OGGSOUNDBUFFERSTREAM_H_
#define _C_OGGSOUNDBUFFERSTREAM_H_

class CThreadedOggDecoder;

class COGGSoundBufferStream : public ISoundBufferStream
{
public:
	/// Constructor
	/// \param fileName = hash of key for sound lookup
	/// \param stream = ee stream to stream audio from, if specified
	COGGSoundBufferStream(IHashString *fileName, IEEStream *stream=NULL);

	/// dtor
	~COGGSoundBufferStream();

	/// Initializes the buffer stream
	virtual void Initialize(){}

	/// Gets the next AL buffer to queue in the object's buffer queue
	/// \param buffer = buffer to return, if ready
	///			NOTE: fn could return true with an invalid buffer if the stream fails somehow
	/// \return true/false if buffer is ready
	virtual bool GetBuffer(ALuint *buffer, bool bWaitForBuffer);

	/// Checks to see if the stream has been exhausted
	/// \return true if eos
	virtual bool IsEndOfStream()
	{
		if (m_Decoder)
		{
			return m_Decoder->IsEndOfStream();
		}
		return true;
	}

	/// Either releases the buffer from al's internal mem (streaming) or does nothing
	/// \param buffer = al buffer to 'dispose'
	virtual void DisposeOldBuffer(ALuint buffer);

	/// Resets the stream (for looping)
	virtual void ResetStream()
	{
		if (m_Decoder)
		{
			m_Decoder->ResetStream();
		}
		// when we reset the stream, we need to start a new buffer
		m_bWaitingForBuffer = false;
	}

	/// Gets the info for the sound
	/// \param frequency = frequency in hz
	/// \param bitDepth = bit depth in bits
	/// \param bitRate = bit rate in bits per sec
	/// \param channels = number of channels
	/// \param fileSize = file size in bytes
	/// \param duration = duration of sound in seconds
	virtual void GetSoundInfo(UINT *frequency, UINT *bitDepth, UINT *bitRate, UINT *channels, UINT *fileSize, float *duration)
	{
		if (m_Decoder)
		{
			m_Decoder->GetSoundInfo(frequency, bitDepth, bitRate, channels, fileSize, duration);
		}
	}

	/// Buffers the entire sound into a single buffer
	/// \return al buffer ref
	ALuint BufferFullSound();
private:
	COpenALSoundUtility *m_SoundUtility;	

	vector<ALuint> m_FreeBuffers;

	CThreadedOggDecoder *m_Decoder;
	bool m_bWaitingForBuffer;
	ALuint m_iLoadingBuffer;
};

#endif //_C_OGGSOUNDBUFFERSTREAM_H_