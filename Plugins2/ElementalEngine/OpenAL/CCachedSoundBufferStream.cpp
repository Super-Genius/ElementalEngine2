///============================================================================
/// \file		CCachedSoundBufferStream.cpp
/// \brief		Implementation of IBufferStream for Cached streams
/// \date		03/13/2007
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCachedSoundBufferStream::CCachedSoundBufferStream(IHashString *fileName)
{
	if (fileName)
	{
		m_dwSoundKey = fileName->GetUniqueID();
	}
	m_bEndOfStream = false;
	m_bCachedBuffer = false;
	
	IToolBox *toolBox = EngineGetToolBox();	
	static CHashString sm(_T("COpenALSoundUtility"));
	
	// Get soundManager and cast it as an OpenAL sound manager
	m_SoundUtility = dynamic_cast<COpenALSoundUtility*>(toolBox->CreateComponent(&sm, 0));
	if (m_SoundUtility == NULL)
	{
		toolBox->Log(LOGWARNING, _T("CachedSoundBuffer: Can't get sound manager!\n"));		
	}
	m_Buffer = 0;	

	Initialize();
}

CCachedSoundBufferStream::~CCachedSoundBufferStream()
{}

void CCachedSoundBufferStream::Initialize()
{
	if (m_SoundUtility)
	{
		m_Buffer = m_SoundUtility->GetSoundBuffer(m_dwSoundKey);
		m_bCachedBuffer = true;
	}
}

bool CCachedSoundBufferStream::GetBuffer(ALuint *buffer, bool bWaitForBuffer)
{
	if ((m_bCachedBuffer == false) && (m_SoundUtility))
	{
		m_Buffer = m_SoundUtility->GetSoundBuffer(m_dwSoundKey);
		m_bCachedBuffer = true;
	}
	
	if (m_Buffer)
	{
		ALuint retBuffer = m_Buffer;
		m_bEndOfStream = true;
		*buffer = retBuffer;
		return true;
	}
	return false;
}

void CCachedSoundBufferStream::GetSoundInfo(UINT *frequency, UINT *bitDepth, UINT *bitRate, UINT *channels, UINT *fileSize, float *duration)
{
	*frequency = 0;
	*bitDepth = 0;
	*bitRate = 0;
	*channels = 0;
	*fileSize = 0;
	*duration = 0.0f;
	if ((m_SoundUtility) && (m_Buffer != 0))
	{
		if (alIsBuffer(m_Buffer) == AL_TRUE)
		{
			m_SoundUtility->GetALBufferI(m_Buffer, AL_FREQUENCY, (int*)frequency, __FILE__, __LINE__);
			
			m_SoundUtility->GetALBufferI(m_Buffer, AL_BITS, (int*)bitDepth, __FILE__, __LINE__);
				            
			*bitRate = (*frequency) * (*bitDepth); // this is in bits per sec

			m_SoundUtility->GetALBufferI(m_Buffer, AL_CHANNELS, (int*)channels, __FILE__, __LINE__);
			
			m_SoundUtility->GetALBufferI(m_Buffer, AL_SIZE, (int*)fileSize, __FILE__, __LINE__);
			
			if ((*bitRate > 0) && (*fileSize > 0))
			{
				*duration = (((float)*fileSize) * 8.0f) / (((float)*bitRate) * ((float)*channels));
			}
		}
	}
}
	