///============================================================================
/// \file		OpenAL.cpp
/// \brief		Implementation of COpenALSoundObject Class
/// \date		10-01-2006
/// \author		Jonathan Burgess
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_AS(COpenALSoundObject, CSoundObject);

IComponent *COpenALSoundObject::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool bAddToHier;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new COpenALSoundObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new COpenALSoundObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}


	return self;
}

COpenALSoundObject::COpenALSoundObject(IHashString *parentName, IHashString *name, bool bAddToHier) : 
	CSoundObject(parentName, name, bAddToHier),
	m_hszNameType( _T("COpenALSoundObject") ),
	m_hsPriority(_T("SOUND_PRIORITY"))
{

	m_ToolBox = EngineGetToolBox();

	// default is to be added
	m_bAddToHierarchy = bAddToHier;
	if( m_bAddToHierarchy )
	{
		AddToHierarchy();
	}

	m_fTimeOfPositionUpdate = 0;
	m_bAutoStart = false;
	m_bValidSource = false;
	m_bIsPlaying = false;
	m_bWasPlaying = false;
	m_bWasPreempted = false;
	m_BufferStream = NULL;
	m_fGlobalVolume = 0.0f;
	m_bJustInitialized = false;
	m_dwSoundKey = 0;
	m_fFadeOutTime = 0.0f;
	m_fFadeOutStartTime = 0.0f;	
	m_bFadeOutSound = false;
	m_fFadeStartVolume = 0.0f;
	m_fPitch = 1.0f;
	m_bUsingSingleBuffer = false;

	static CHashString sm(_T("COpenALSoundUtility"));
	
	// Get soundManager and cast it as an OpenAL sound manager
	m_SoundUtility = dynamic_cast<COpenALSoundUtility*>(m_ToolBox->CreateComponent(&sm, 0));
	if(m_SoundUtility == NULL)
	{
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log(LOGERROR, _T("OpenALSoundObject error: COpenALSoundUtility is NULL\n")); 		
	}

	m_bWaitingForBuffer = false;
}

COpenALSoundObject::~COpenALSoundObject()
{
	DeInit();
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

void COpenALSoundObject::Init()
{
	if (m_SoundUtility == NULL)
	{
		m_ToolBox->Log(LOGWARNING, _T("COpenALSoundObject: cannot init; manager is null\n"));
		return;
	}

	m_bJustInitialized = true;

//	CHashString zFileName((const TCHAR *)m_szSoundFile);	

	m_BufferStream = m_SoundUtility->CreateBufferStream(m_dwSoundKey);

	static DWORD msgGetGlobalSoundVolume = CHashString(_T("GetGlobalSoundVolume")).GetUniqueID();
	m_ToolBox->SendMessage(msgGetGlobalSoundVolume, sizeof(float), &m_fGlobalVolume);	
	
	if (m_bAutoStart)
	{
		if (!m_bValidSource)
		{
			if (m_SoundUtility == NULL)
			{
				m_ToolBox->Log( LOGWARNING, _T("OpenALSoundObject error: play -> manager is null\n"));
				return;
			}

			m_uiSoundSource = m_SoundUtility->GetSoundSource(this);
			if (alIsSource(m_uiSoundSource) == AL_FALSE)
			{
		//			m_ToolBox->Log( LOGWARNING, _T("OpenALSoundObject error: play -> could not get valid sound source\n"));
				return;
			}

			m_bValidSource = true;
			SetUpSourceData();
		}
		PreloadBufferQueue();
		Play();
	}	
}

void COpenALSoundObject::DeInit()
{
	if (m_bValidSource)
	{
		FreeSource();
		if (m_SoundUtility != NULL)
		{
			m_SoundUtility->ReturnSoundSource(m_uiSoundSource);
		}
	}
	if (m_BufferStream)
	{
		delete m_BufferStream;
		m_BufferStream = NULL;
	}
}

void COpenALSoundObject::Serialize(IArchive &ar)
{
	float fSaveVersion = 1.1f;
	if( ar.IsReading() )
	{
		float fVersion = 0.0f;
		StdString soundFile;
		ar.Read(fVersion);
		ar.Read(soundFile);
		ar.Read(m_fVolume);
		ar.Read(m_bAutoRemove);
		ar.Read(m_b3DSound);
		ar.Read(m_bLooping);
		ar.Read(m_bAutoStart);

		if (fVersion >= 1.1f)
		{
			ar.Read(m_fPitch);
		}

		CHashString hsFile(soundFile);
		m_dwSoundKey = hsFile.GetUniqueID();
	}
	else
	{
		StdString soundFile = m_ToolBox->GetHashString(m_dwSoundKey);
		ar.Write(fSaveVersion, _T("Version"));
		ar.Write(soundFile, _T("FileName"));
		ar.Write(m_fVolume, _T("MaxVolume"));
		ar.Write(m_bAutoRemove, _T("AutoRemove"));
		ar.Write(m_b3DSound, _T("Is3DSound"));
		ar.Write(m_bLooping, _T("IsLooping"));
		ar.Write(m_bAutoStart, _T("AutoStart"));
		ar.Write(m_fPitch, _T("PitchMultiplier"));
	}
}

bool COpenALSoundObject::Update()
{
	if (m_SoundUtility == NULL)
	{
		m_ToolBox->Log(LOGWARNING, _T("Sound object: Manager is null in update\n"));
		return false;
	}

	if ((m_b3DSound) && (m_bTransformIsDirty))
	{
		m_bTransformIsDirty = false;
		ResetPosition();
	}

	if (m_bValidSource)
	{
		if (!m_b3DSound)
		{
			float velocity[3] = {0.0f, 0.0f, 0.0f};
			m_SoundUtility->SetALSourceFV(m_uiSoundSource, AL_VELOCITY, velocity, __FILE__, __LINE__);		
		}

		ALenum err;
	
		if (!m_bUsingSingleBuffer)
		{
			// check to make sure the queue is updating
			UpdateBufferQueue();
		}

		// check to see if we're playing	
		ALint playing_state;
		alGetSourcei(m_uiSoundSource, AL_SOURCE_STATE, &playing_state);
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
			m_ToolBox->Log( LOGWARNING, _T("Sound object: OpenAL can't get audio sources state: %s\n"), GetALErrorString(err));

			// schedule for delete
			DELETEOBJECTPARAMS dop;
			dop.name = GetName();
			static DWORD msgHash_DelayedDeleteObject = CHashString(_T("DelayedDeleteObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_DelayedDeleteObject, sizeof(DELETEOBJECTPARAMS), &dop);

			return false;
		}

		if( playing_state == AL_PLAYING )
		{
			// still playing
			m_bIsPlaying = true;

			if (m_bFadeOutSound)
			{
				float curTime = 0.0f;
				static DWORD msgGetTime = CHashString(_T("GetTime")).GetUniqueID();
				m_ToolBox->SendMessage(msgGetTime, sizeof(float), &curTime);

				float deltaT = curTime - m_fFadeOutStartTime;
				float fadeVal = (1.0f - (deltaT / m_fFadeOutTime)) * m_fFadeStartVolume;
				if (fadeVal > 0.0f)
				{
					m_fVolume = fadeVal;
					UpdateLocalVolume(m_fGlobalVolume);
				}
				else
				{
					Stop();
				}
			}

			return true;
		}
		else if (playing_state == AL_STOPPED)
		{
			m_bIsPlaying = false;
			if (m_bLooping && m_bWasPlaying)
			{
				// try to play again
				Play();
			}	
			
			else if ( m_bAutoRemove )
			{
				// it's not playing and should be removed

				// schedule for delete
				DELETEOBJECTPARAMS dop;
				dop.name = GetName();
				static DWORD msgHash_DelayedDeleteObject = CHashString(_T("DelayedDeleteObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_DelayedDeleteObject, sizeof(DELETEOBJECTPARAMS), &dop);

				FreeSource();

				m_SoundUtility->ReturnSoundSource(m_uiSoundSource);

				return false;
			}
			else
			{
				return true;
			}
		}
	}
	else if (m_bWasPreempted)
	{
		if (m_bLooping && m_bWasPlaying)
		{
			// try to play again
			Play();
		}
        else
		{
			// if not looping, may as well kill self
			DELETEOBJECTPARAMS dop;
			dop.name = GetName();
			static DWORD msgHash_DelayedDeleteObject = CHashString(_T("DelayedDeleteObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_DelayedDeleteObject, sizeof(DELETEOBJECTPARAMS), &dop);
		}
	}
	return true;
}

void COpenALSoundObject::PreloadBufferQueue()
{
	ALenum err;
	if (m_BufferStream == NULL)
	{
		m_ToolBox->Log(LOGWARNING, _T("Sound object: buffer stream is null in preload buffer queue!\n"));
		return;
	}
	
	// get the number of queued buffers first; want to keep this full if possible
	ALint queuedBuffers;
	alGetSourcei(m_uiSoundSource, AL_BUFFERS_QUEUED, &queuedBuffers);
	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log( LOGWARNING, _T("Sound object: OpenAL can't get number of buffers queued: %s\n"), GetALErrorString(err));		

		return;
	}

	UINT numBuffers = m_SoundUtility->GetNumSoundBuffers();
	for (UINT i=queuedBuffers; i<numBuffers; i++)
	{
		if (m_bLooping && m_BufferStream->IsEndOfStream())
		{
			m_BufferStream->ResetStream();
		}
		
		if (m_BufferStream->IsEndOfStream() == false)
		{
			ALuint buffer;
			bool ret = m_BufferStream->GetBuffer(&buffer, true);

			if (alIsBuffer(buffer) == AL_TRUE)
			{
				if ((i == 0) && (m_BufferStream->IsEndOfStream()))
				{
					// single buffer!
					m_bUsingSingleBuffer = true;
					m_SoundUtility->SetALSourceI(m_uiSoundSource, AL_BUFFER, buffer, __FILE__, __LINE__);
					// handle looping here
					if (m_bLooping)
					{
						m_SoundUtility->SetALSourceI(m_uiSoundSource, AL_LOOPING, AL_TRUE, __FILE__, __LINE__);
					}
					break;
				}
				else
				{
					alSourceQueueBuffers(m_uiSoundSource, 1, &buffer);
					err = alGetError();
					if (err != AL_NO_ERROR)
					{
						m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
						m_ToolBox->Log( LOGWARNING, _T("Sound object: OpenAL could not queue buffer: %s\n"), GetALErrorString(err));

						return;
					}
				}
			}
		}
	}
}

void COpenALSoundObject::UpdateBufferQueue()
{
	ALenum err;
	ALint processedBuffers;

	// get the number of buffers in queue that have been processed
	alGetSourcei(m_uiSoundSource, AL_BUFFERS_PROCESSED, &processedBuffers);
	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log( LOGWARNING, _T("Sound object: OpenAL can't get number of buffers processed: %s\n"), GetALErrorString(err));

		return;
	}

	UINT i;
	ALuint buffer;

	// remove processed buffers and replace with newly loaded ones, if possible
	for (i=0; i<(UINT)processedBuffers; i++)
	{
		if (m_bLooping && m_BufferStream->IsEndOfStream())
		{
			m_BufferStream->ResetStream();
		}

		if (m_BufferStream->IsEndOfStream() == false)
		{			
			alSourceUnqueueBuffers(m_uiSoundSource, 1, &buffer);			
		
			// if we're streaming, we need to allow the buffer stream to deallocate the space from al's internals
			m_BufferStream->DisposeOldBuffer(buffer);

			err = alGetError();
			if (err != AL_NO_ERROR)
			{
				m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
				m_ToolBox->Log( LOGWARNING, _T("Sound object: OpenAL could not unqueue buffer: %s\n"), GetALErrorString(err));

				return;
			}
		}
	}

	// double check...
	ALint queuedBuffers;
	alGetSourcei(m_uiSoundSource, AL_BUFFERS_QUEUED, &queuedBuffers);
	if (queuedBuffers < (int)m_SoundUtility->GetNumSoundBuffers())
	{
		if ((m_BufferStream) && (!m_BufferStream->IsEndOfStream()))
		{
			m_bWaitingForBuffer = true;
		}
	}

	if (m_bWaitingForBuffer)
	{
		if (m_BufferStream)
		{
			bool ret = m_BufferStream->GetBuffer(&buffer, false);
			if (!ret)
			{
				m_bWaitingForBuffer = true;
			}
			else
			{
				alSourceQueueBuffers(m_uiSoundSource, 1, &buffer);
				m_bWaitingForBuffer = false;
				
				err = alGetError();
				if (err != AL_NO_ERROR)
				{
					m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
					m_ToolBox->Log( LOGWARNING, _T("Sound object: OpenAL could not queue buffer: %s\n"), GetALErrorString(err));

					return;
				}
			}
		}
	}
}

bool COpenALSoundObject::Play()
{
	if (!m_bValidSource)
	{
		if (m_SoundUtility == NULL)
		{
			m_ToolBox->Log( LOGWARNING, _T("OpenALSoundObject error: play -> manager is null\n"));
			return false;
		}

		m_uiSoundSource = m_SoundUtility->GetSoundSource(this);
		if (alIsSource(m_uiSoundSource) == AL_FALSE)
		{
//			m_ToolBox->Log( LOGWARNING, _T("OpenALSoundObject error: play -> could not get valid sound source\n"));
			return false;
		}

		m_bValidSource = true;
		SetUpSourceData();
	}
	UpdateBufferQueue();
	
	ALenum err;

	alSourcePlay(m_uiSoundSource);
	err = alGetError();

	if (err != AL_NO_ERROR)
	{
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log( LOGERROR, _T("OpenALSoundObject error: can't play sound: %s\n"), GetALErrorString(err));
		return false;
	}
	m_bIsPlaying = true;
	m_bWasPlaying = true;
	return true;
}

bool COpenALSoundObject::Stop(float fadeOutVal)
{
	if (fadeOutVal > 0.0f)
	{
		m_fFadeOutTime = fadeOutVal;
		
		static DWORD msgGetTime = CHashString(_T("GetTime")).GetUniqueID();
		m_ToolBox->SendMessage(msgGetTime, sizeof(float), &m_fFadeOutStartTime);

		m_bFadeOutSound = true;

		m_fFadeStartVolume = m_fVolume;
		return true;
	}
	else
	{
		if (m_bValidSource)
		{
			alSourceStop(m_uiSoundSource);
			ALenum err = alGetError();
			if (err != AL_NO_ERROR)
			{
				m_ToolBox->Log(LOGWARNING, _T("Sound object %s: could not stop sound: %s\n"), GetName()->GetString(), GetALErrorString(err));
				return false;
			}

			if (m_SoundUtility != NULL)
			{
				m_SoundUtility->SetALSourceF(m_uiSoundSource, AL_SEC_OFFSET, 0.0f, __FILE__, __LINE__);
			}	
		}

		m_bIsPlaying = false;
		m_bWasPlaying = false;
		return true;
	}
}

bool COpenALSoundObject::Pause()
{
	if (!m_bValidSource)
	{
		ALenum err;

		alSourcePause(m_uiSoundSource);
		err = alGetError();

		if (err != AL_NO_ERROR)
		{
			m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
			m_ToolBox->Log( LOGERROR, _T("OpenALSoundObject error: can't pause sound: %s\n"), GetALErrorString(err));
			return false;
		}
	}
	m_bIsPlaying = false;
	return true;
}

DWORD COpenALSoundObject::OnPlaySoundObject(DWORD size, void *params)
{
	if (Play() == true)
	{
		return MSG_HANDLED_PROCEED;
	}
	return MSG_NOT_HANDLED;
}

DWORD COpenALSoundObject::OnStopSoundObject(DWORD size, void *params)
{
	if (Stop() == true)
	{
		return MSG_HANDLED_PROCEED;
	}
	return MSG_NOT_HANDLED;
}

void COpenALSoundObject::SetSoundPosition(const Vec3 &pos)
{
	m_GlobalPosition.Set(pos);
	
	// recalc local pos, in case parent moves again
	IHashString *parentName = GetParentName();
	if (parentName)
	{
		Vec3 parentPos;
		static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &parentPos, GetParentName());

		m_LocalPosition.Set(pos - parentPos);
	}
}

void COpenALSoundObject::ResetPosition()
{
	Vec3 lastPosition(m_GlobalPosition);
	IHashString *parentName	= GetParentName();
	if (parentName)
	{
		Vec3 parentPos;
		static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &parentPos, parentName);

		m_GlobalPosition.Set(m_LocalPosition + parentPos);		
	}

	if (m_bValidSource)
	{
		float position[3] = { m_GlobalPosition.x, m_GlobalPosition.y, m_GlobalPosition.z };
		m_SoundUtility->SetALSourceFV(m_uiSoundSource, AL_POSITION, position, __FILE__, __LINE__);

		if (m_bJustInitialized)
		{
			// if it has just been initialized, don't calculate doppler till next tick
			m_bJustInitialized = false;
			return;
		}

		// calculate doppler
		Vec3 movingVector = m_GlobalPosition - lastPosition;
		
		float unitScale = m_SoundUtility->GetUnitScale();

		static CHashString hsGameTime(GAMEDELTATIMER);
		float deltaT = m_Timer->GetTimeDelta( &hsGameTime );

		// velocity is probably in meters per sec
		float denom = (unitScale * deltaT);
		if (denom > 0.0f)
		{
			movingVector /= denom;		
		}
		else
		{
			movingVector.Set(0.0f, 0.0f, 0.0f);
		}

		float alVeloc[3] = {movingVector.x, movingVector.y, movingVector.z};
		m_SoundUtility->SetALSourceFV(m_uiSoundSource, AL_VELOCITY, alVeloc, __FILE__, __LINE__);
	}
}

void COpenALSoundObject::FreeSource()
{
	ALenum err;
	if (m_bValidSource)
	{
		bool bIsPlaying, bWasPlaying;
		bIsPlaying = IsPlaying();
		bWasPlaying = WasPlaying();

		Stop();
		
		if (bIsPlaying || bWasPlaying)
		{			
			// reset was playing flag
			m_bWasPlaying = true;
			m_bWasPreempted = true;
		}

		// unqueue and clear all buffers
		ALint queuedBuffers;
		UINT i;
		alGetSourcei(m_uiSoundSource, AL_BUFFERS_QUEUED, &queuedBuffers);
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
			m_ToolBox->Log( LOGWARNING, _T("Sound object: OpenAL can't get number of buffers queued: %s\n"), GetALErrorString(err));		

			return;
		}

		// clear buffer queue
		for (i=0; i<(UINT)queuedBuffers; i++)
		{
			ALuint buffer;
			alSourceUnqueueBuffers(m_uiSoundSource, 1, &buffer);
			err = alGetError();
			if (err != AL_NO_ERROR)
			{
				m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
				m_ToolBox->Log( LOGWARNING, _T("Sound object: OpenAL could not unqueue buffer: %s\n"), GetALErrorString(err));
				return;
			}
			if (m_BufferStream)
			{
				m_BufferStream->DisposeOldBuffer(buffer);
			}
		}
		m_bValidSource = false;
	}
}

void COpenALSoundObject::SetUpSourceData()
{
	if (m_bValidSource)
	{
		if( m_b3DSound )
		{
			ResetPosition();
			m_SoundUtility->SetALSourceF(m_uiSoundSource, AL_ROLLOFF_FACTOR, 1.0f, __FILE__, __LINE__);
			m_SoundUtility->SetALSourceI(m_uiSoundSource, AL_SOURCE_RELATIVE, AL_FALSE, __FILE__, __LINE__);
		}
		else
		{
			m_SoundUtility->SetALSourceF(m_uiSoundSource, AL_ROLLOFF_FACTOR, 0.0f, __FILE__, __LINE__);
			m_SoundUtility->SetALSourceI(m_uiSoundSource, AL_SOURCE_RELATIVE, AL_TRUE, __FILE__, __LINE__);

			float position[3] = {0.0f, 0.0f, 0.0f};
			m_SoundUtility->SetALSourceFV(m_uiSoundSource, AL_POSITION, position, __FILE__, __LINE__);
			float alVeloc[3] = {0.0f, 0.0f, 0.0f};
			m_SoundUtility->SetALSourceFV(m_uiSoundSource, AL_VELOCITY, alVeloc, __FILE__, __LINE__);
		}

		if (m_fVolume == -1.0f)
		{
			m_fVolume = m_SoundUtility->GetSoundVolume(m_dwSoundKey);
		}
		m_SoundUtility->SetALSourceF(m_uiSoundSource, AL_GAIN, (m_fVolume * m_fGlobalVolume), __FILE__, __LINE__);

		float attenScale = m_SoundUtility->GetAttenuationScale();
		m_SoundUtility->SetALSourceF(m_uiSoundSource, AL_REFERENCE_DISTANCE, attenScale, __FILE__, __LINE__);

		m_SoundUtility->SetALSourceF(m_uiSoundSource, AL_PITCH, m_fPitch, __FILE__, __LINE__);
		m_SoundUtility->SetALSourceI(m_uiSoundSource, AL_BUFFER, 0, __FILE__, __LINE__);
		m_SoundUtility->SetALSourceI(m_uiSoundSource, AL_LOOPING, AL_FALSE, __FILE__, __LINE__);
	}
}

void COpenALSoundObject::UpdateLocalVolume(float fGlobalVolume)
{
	m_fGlobalVolume = fGlobalVolume;
	if (m_bValidSource)
	{
		if (m_SoundUtility != NULL)
		{
			m_SoundUtility->SetALSourceF(m_uiSoundSource, AL_GAIN, (m_fVolume * m_fGlobalVolume), __FILE__, __LINE__);	
		}
	}
}