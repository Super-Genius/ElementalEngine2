///============================================================================
/// \file		ISoundBufferStream.h
/// \brief		Interface for al buffer 'streams'
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

#ifndef _I_SOUNDBUFFERSTREAM_H_
#define _I_SOUNDBUFFERSTREAM_H_

class ISoundBufferStream
{
public:
	/// Virtual dtor
	virtual ~ISoundBufferStream() {} ;

	/// Initializes the buffer stream
	virtual void Initialize() = 0;

	/// Gets the next AL buffer to queue in the object's buffer queue
	/// \param buffer = buffer to return, if ready
	///			NOTE: fn could return true with an invalid buffer if the stream fails somehow
	/// \return true/false if buffer is ready
	virtual bool GetBuffer(ALuint *buffer, bool bWaitForBuffer) = 0;

	/// Checks to see if the stream has been exhausted
	/// \return true if eos
	virtual bool IsEndOfStream() = 0;

	/// Either releases the buffer from al's internal mem (streaming) or does nothing
	/// \param buffer = al buffer to 'dispose'
	virtual void DisposeOldBuffer(ALuint buffer) = 0;

	/// Resets the stream (for looping)
	virtual void ResetStream() = 0;

	/// Gets the info for the sound
	/// \param frequency = frequency in hz
	/// \param bitDepth = bit depth in bits
	/// \param bitRate = bit rate in bits per sec
	/// \param channels = number of channels
	/// \param fileSize = file size in bytes
	/// \param duration = duration of sound in seconds
	virtual void GetSoundInfo(UINT *frequency, UINT *bitDepth, UINT *bitRate, UINT *channels, UINT *fileSize, float *duration) = 0;
};

#endif //_I_SOUNDBUFFERSTREAM_H_