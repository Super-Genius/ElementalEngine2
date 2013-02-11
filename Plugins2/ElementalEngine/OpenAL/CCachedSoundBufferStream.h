///============================================================================
/// \file		CCachedSoundBufferStream.h
/// \brief		Implementation of IBufferStream for Cached streams
/// \date		03/13/2007
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

#ifndef _C_CACHEDSOUNDBUFFERSTREAM_H_
#define _C_CACHEDSOUNDBUFFERSTREAM_H_

class COpenALSoundUtility;

typedef vector<ALuint> AUDIOBUFFERVECTOR;

class CCachedSoundBufferStream : public ISoundBufferStream
{
public:
	/// Ctor, Dtor
	CCachedSoundBufferStream(IHashString *fileName);
	~CCachedSoundBufferStream();

	/// Initializes the buffer stream
	virtual void Initialize();

	/// Gets the next AL buffer to queue in the object's buffer queue
	/// \param buffer = buffer to return, if ready
	///			NOTE: fn could return true with an invalid buffer if the stream fails somehow
	/// \return true/false if buffer is ready
	virtual bool GetBuffer(ALuint *buffer, bool bWaitForBuffer);

	/// Checks to see if the stream is over (false the first time, true every other)
	/// \return true if eos
	virtual bool IsEndOfStream() { return m_bEndOfStream; }

	/// Does nothing, since the Cacheds will stay cached
	/// \param buffer = buffer to dispose
	virtual void DisposeOldBuffer(ALuint buffer){}

	/// Resets the stream
	virtual void ResetStream()
	{ 
		m_bEndOfStream = false;
	}

	/// Gets the info for the sound
	/// \param frequency = frequency in hz
	/// \param bitDepth = bit depth in bits
	/// \param bitRate = bit rate in bits per sec
	/// \param channels = number of channels
	/// \param fileSize = file size in bytes
	/// \param duration = duration of sound in seconds
	virtual void GetSoundInfo(UINT *frequency, UINT *bitDepth, UINT *bitRate, UINT *channels, UINT *fileSize, float *duration);

private:
	bool m_bEndOfStream;
	DWORD m_dwSoundKey;
	ALuint m_Buffer;
	bool m_bCachedBuffer;
	COpenALSoundUtility *m_SoundUtility;
};

#endif //_C_CACHEDSOUNDBUFFERSTREAM_H_