///============================================================================
/// \file		CMusicPlaylist.cpp
/// \brief		Playlist control for music files
/// \date		03/02/07
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

#ifdef XBOX
#include "alconsole.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CMusicPlaylist);

CMusicPlaylist::CMusicPlaylist(IHashString *parentName, IHashString *name, bool bAddToHier) : 
	SCENEOBJECTTEMPLATE(CSoundManager, CMusicPlaylist, ISoundObject, parentName, name),
	m_hsPriority(_T("MUSIC_PRIORITY"))
{
	m_bAddToHierarchy = bAddToHier;
	if( m_bAddToHierarchy )
	{
		AddToHierarchy();
	}
	m_fMasterVolume = 1.0f;
	m_Tracks.clear();
	m_bValidSource = false;
	m_bWasPlaying = false;
	m_fCurrentTimePosition = 0.0f;
	m_fTotalTime = 0.0f;
	m_fLastTime = 0.0f;
	m_fGlobalVolume = 0.0f;
	
	static CHashString sm(_T("COpenALSoundUtility"));	
	// Get soundManager and cast it as an OpenAL sound manager
	m_SoundUtility = dynamic_cast<COpenALSoundUtility*>(m_ToolBox->CreateComponent(&sm, 0));
	if(m_SoundUtility == NULL)
	{
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log(LOGERROR, _T("Music playlist: COpenALSoundUtility is NULL\n")); 		
	}

	m_bLoopWhole = false;
	m_bWaitingForBuffer = false;
}

CMusicPlaylist::~CMusicPlaylist()
{
	if( m_bAddToHierarchy )
	{
		RemoveFromHierarchy();
	}

	DeInit();
}

IComponent *CMusicPlaylist::Create(int nArgs, va_list argptr)
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
			// default to false
			self = new CMusicPlaylist(parentName, name, false);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CMusicPlaylist(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CMusicPlaylist::Init()
{
	SetUpTracks();	
	static DWORD msgGetGlobalMusicVolume = CHashString(_T("GetGlobalMusicVolume")).GetUniqueID();
	m_ToolBox->SendMessage(msgGetGlobalMusicVolume, sizeof(float), &m_fGlobalVolume);

	if (!m_bValidSource)
	{
		if (m_SoundUtility == NULL)
		{
			m_ToolBox->Log(LOGWARNING, _T("Music Playlist: manager is null in init\n"));
			return;
		}

		m_uiSoundSource = m_SoundUtility->GetSoundSource(this);

		if (alIsSource(m_uiSoundSource) == AL_FALSE)
		{
			return;
		}

		m_bValidSource = true;

		SetUpALSourceData();
	}

	PreloadBuffers();
}
	
void CMusicPlaylist::DeInit()
{
	if (m_bValidSource)
	{
		FreeSource();
		if (m_SoundUtility != NULL)
		{
			m_SoundUtility->ReturnSoundSource(m_uiSoundSource);
		}
	}

	TRACKLIST::iterator trackIter = m_Tracks.begin();
	for (; trackIter != m_Tracks.end(); trackIter++)
	{
		if (trackIter->bufferStream != NULL)
		{
			delete trackIter->bufferStream;
			trackIter->bufferStream = NULL;
		}
	}

	m_Tracks.clear();
	m_CurrentTrack = m_Tracks.end();
}

void CMusicPlaylist::Serialize(IArchive &ar)
{
	if (ar.IsReading())
	{
		UINT iNumTracks;
		float fVersion;
		ar.Read(fVersion);
		ar.Read(iNumTracks);
		ar.Read(m_fMasterVolume);

		if (fVersion >= 1.1f)
		{
			ar.Read(m_bLoopWhole);
		}

		for (UINT i=0; i<iNumTracks; i++)
		{
			PLAYLISTENTRY entry;

			StdString fileName;
			ar.Read(fileName);
			fileName.tolower();
			entry.fileName.Init(fileName);

			ar.Read(entry.numLoops);
			
			m_Entries.push_back(entry);
		}
		
		CacheFiles();
	}
	else
	{
		static float saveVersion = 1.1f;
		ar.Write(saveVersion, _T("Version"));
		ar.Write((int)m_Tracks.size(), _T("NumTracks"));
		ar.Write(m_fMasterVolume, _T("MasterVolume"));
		ar.Write(m_bLoopWhole, _T("LoopWholeList"));

		ar.StartClass(_T("Track"));

		ENTRYLIST::iterator entryIter = m_Entries.begin();
		for (; entryIter != m_Entries.end(); entryIter++)
		{
			IHashString *fileName = &entryIter->fileName;
			ar.Write(fileName->GetString(), _T("FileName"));
			ar.Write(entryIter->numLoops, _T("NumLoops"));
		}

		ar.EndClass();
	}
}

void CMusicPlaylist::CacheFiles()
{
	ENTRYLIST::iterator entryIter = m_Entries.begin();
	for (; entryIter != m_Entries.end(); entryIter++)
	{
		IHashString *hszFile = &entryIter->fileName;
		
		LOADFILEEXTPARAMS lfep;
		lfep.fileName = (TCHAR*) hszFile->GetString();
		lfep.bInternalLoad = true;
		static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);

		if (m_SoundUtility)
		{
			m_SoundUtility->ForceSoundIntoMap(hszFile->GetUniqueID(), hszFile->GetUniqueID(), 1.0f, false);
		}
	}	
}

void CMusicPlaylist::SetUpTracks()
{
	if (m_SoundUtility == NULL)
	{
		m_ToolBox->Log(LOGWARNING, _T("CMusicPlaylist: sound manager is null in set up al data!\n"));
		return;
	}

	// convert entry data into individual tracks
	ENTRYLIST::iterator entryIter = m_Entries.begin();
	for (; entryIter != m_Entries.end(); entryIter++)
	{
		for (UINT i=0; i<entryIter->numLoops; i++)
		{
			TRACKDATA data;
			data.fileName = &entryIter->fileName;
			data.bufferStream = m_SoundUtility->CreateBufferStream(data.fileName->GetUniqueID());

			if (data.bufferStream == NULL)
			{
				m_ToolBox->Log(LOGWARNING, _T("CMusicPlaylist: buffer stream is null in set up al data for file %s\n"), 
					data.fileName->GetString());
				continue;
			}	

			UINT frequency, bitDepth, bitRate, channels, fileSize;
			float duration;

			data.bufferStream->GetSoundInfo(&frequency, &bitDepth, &bitRate, &channels, &fileSize, &duration);
			m_fTotalTime += duration;

			m_Tracks.push_back(data);
		}
	}	
	m_CurrentTrack = m_Tracks.begin();
}

void CMusicPlaylist::SetUpALSourceData()
{
	if (m_bValidSource)
	{
		m_SoundUtility->SetALSourceF(m_uiSoundSource, AL_MAX_GAIN, 1.0f, __FILE__, __LINE__);
		m_SoundUtility->SetALSourceF(m_uiSoundSource, AL_ROLLOFF_FACTOR, 0.0f, __FILE__, __LINE__);
		m_SoundUtility->SetALSourceI(m_uiSoundSource, AL_LOOPING, AL_FALSE, __FILE__, __LINE__);
		m_SoundUtility->SetALSourceF(m_uiSoundSource, AL_GAIN, (m_fMasterVolume * m_fGlobalVolume), __FILE__, __LINE__);
		m_SoundUtility->SetALSourceF(m_uiSoundSource, AL_PITCH, 1.0f, __FILE__, __LINE__);
		m_SoundUtility->SetALSourceI(m_uiSoundSource, AL_BUFFER, 0, __FILE__, __LINE__);		
#ifdef XBOX
		alSourceSetTypeMusic(m_uiSoundSource, AL_TRUE);
#endif
	}
}

bool CMusicPlaylist::Update()
{
	// if it was playing, but isn't...
	bool isPlaying = IsPlaying();
	if ((m_bWasPlaying) && (!isPlaying))
	{
		// if it isn't paused...
		if (!IsPaused())
		{
			// if the time is short...
			if (m_fCurrentTimePosition < m_fTotalTime)
			{
				// we must have dropped a buffer in the queue - try to play again
				Play();
			}
			else
			{
				if (m_bValidSource)
				{
					FreeSource();
					if (m_SoundUtility != NULL)
					{
						m_SoundUtility->ReturnSoundSource(m_uiSoundSource);
					}
				}
			}
		}
	}
	// if stop had been called
	else if (!isPlaying)
	{
		if (m_bValidSource)
		{
			FreeSource();
			if (m_SoundUtility != NULL)
			{
				m_SoundUtility->ReturnSoundSource(m_uiSoundSource);
			}
		}
	}
	else if (isPlaying)
	{
		// update timer

		// using GetTime versus GetTimeDelta, because as of right now the music still plays when the game 
		// pauses, so if the time delta stops ticking (which pauses the scripts), the music will still 
		// play and my current time timer will be out of sync
		ITimer* timer = m_ToolBox->GetTimer();
		float currentTime = timer->GetTime();
		float deltaT = currentTime - m_fLastTime;
		m_fLastTime = currentTime;

		m_fCurrentTimePosition += deltaT;

		if ((m_bLoopWhole) && (m_fCurrentTimePosition > m_fTotalTime))
		{
			// make sure the current position is correct if it's looping
			m_fCurrentTimePosition -= m_fTotalTime;
		}
	}
	if (m_bValidSource)
	{
		UpdateBufferQueue();
	}
	return true;
}

bool CMusicPlaylist::Play()
{
	if (!m_bValidSource)
	{
		if (m_SoundUtility == NULL)
		{
			m_ToolBox->Log(LOGWARNING, _T("Music Playlist: manager is null in play\n"));
			return false;
		}

		m_uiSoundSource = m_SoundUtility->GetSoundSource(this);

		if (alIsSource(m_uiSoundSource) == AL_FALSE)
		{
			return false;
		}

		m_bValidSource = true;

		SetUpALSourceData();
	}

	ALenum err;

	UpdateBufferQueue();

	alSourcePlay(m_uiSoundSource);

	err = alGetError();

	if (err != AL_NO_ERROR)
	{
		m_ToolBox->Log(LOGWARNING, _T("OpenAL could not start playlist queue: %s\n"), GetALErrorString(err));			
		return false;
	}

	m_bWasPlaying = true;

	ITimer* timer = m_ToolBox->GetTimer();
	m_fLastTime = timer->GetTime();

	return true;	
}
	
bool CMusicPlaylist::IsPlaying()
{
	if (!m_bValidSource)
	{
		return false;
	}
	else
	{
		ALenum err;
		ALint sourceState;
		alGetSourcei(m_uiSoundSource, AL_SOURCE_STATE, &sourceState);

		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			m_ToolBox->Log(LOGWARNING, _T("Music playlist: is playing-> OpenAL can't get source state: %s\n"), GetALErrorString(err));
			return false;
		}
		
		if ((sourceState == AL_PLAYING) || (sourceState == AL_INITIAL))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool CMusicPlaylist::Stop(float fadeOutVal)
{
	if (fadeOutVal > 0.0f)
	{
		// TODO
		return true;
	}
	else
	{
		if (m_bValidSource)
		{
			ALenum err;

			alSourceStop(m_uiSoundSource);

			err = alGetError();

			if (err != AL_NO_ERROR)
			{
				m_ToolBox->Log(LOGWARNING, _T("OpenAL could not stop playlist: %s\n"), GetALErrorString(err));
				return false;
			}
			
			m_bWasPlaying = false;

			ResetBuffers();		

			return true;
		}	
		return false;
	}
}

bool CMusicPlaylist::Pause()
{
	if (m_bValidSource)
	{
		ALenum err;

		alSourcePause(m_uiSoundSource);

		err = alGetError();

		if (err != AL_NO_ERROR)
		{
			m_ToolBox->Log(LOGWARNING, _T("OpenAL could not pause playlist: %s\n"), GetALErrorString(err));
			return false;
		}
		
        m_bWasPlaying = false;
		return true;
	}	
	return false;
}

bool CMusicPlaylist::IsPaused()
{
	if (!m_bValidSource)
	{
		return false;
	}
	else
	{
		ALenum err;
		ALint sourceState;
		alGetSourcei(m_uiSoundSource, AL_SOURCE_STATE, &sourceState);

		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			m_ToolBox->Log(LOGWARNING, _T("Music playlist: is paused-> OpenAL can't get source state: %s\n"), GetALErrorString(err));
			return false;
		}
		
		if (sourceState == AL_PAUSED)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

void CMusicPlaylist::PreloadBuffers()
{
	ALenum err;

	ALint queuedBuffers;
	alGetSourcei(m_uiSoundSource, AL_BUFFERS_QUEUED, &queuedBuffers);
	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log( LOGWARNING, _T("Music playlist: OpenAL can't get number of buffers queued: %s\n"), GetALErrorString(err));		

		return;
	}

	UINT numBuffers = m_SoundUtility->GetNumSoundBuffers();

	for (UINT i=(UINT)queuedBuffers; i<numBuffers; i++)
	{
		if (m_CurrentTrack != m_Tracks.end())
		{
			ISoundBufferStream *currentStream = m_CurrentTrack->bufferStream;
			if (currentStream == NULL)
			{
				m_ToolBox->Log(LOGWARNING, _T("CMusicPlaylist: current stream in preload is NULL!\n"));
				return;
			}

			if (currentStream->IsEndOfStream())
			{
				m_CurrentTrack++;
				if (m_CurrentTrack == m_Tracks.end())
				{
					if (m_bLoopWhole)
					{
						// loop, so start over
						ResetPlaylistStreams();
						m_CurrentTrack = m_Tracks.begin();
					}
					else
					{
						// out of tracks
						return;
					}
				}

				currentStream = m_CurrentTrack->bufferStream;
				if (currentStream == NULL)
				{
					m_ToolBox->Log(LOGWARNING, _T("CMusicPlaylist: current stream in preload is NULL!\n"));
					return;
				}
			}

			ALuint buffer;
			bool ret = currentStream->GetBuffer(&buffer, true);
			if (alIsBuffer(buffer) == AL_TRUE)
			{
				alSourceQueueBuffers(m_uiSoundSource, 1, &buffer);
				err = alGetError();
				if (err != AL_NO_ERROR)
				{
					m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
					m_ToolBox->Log( LOGWARNING, _T("Music playlist: OpenAL could not queue buffer: %s\n\tNOTE: all the PCM data in the playlist must be the same format\n"), GetALErrorString(err));

					return;
				}
			}
		}
	}
}

void CMusicPlaylist::UpdateBufferQueue()
{
	ALenum err;
	ALint processedBuffers;

	// get the number of buffers in queue that have been processed
	alGetSourcei(m_uiSoundSource, AL_BUFFERS_PROCESSED, &processedBuffers);
	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log( LOGWARNING, _T("Music playlist: OpenAL can't get number of buffers processed: %s\n"), GetALErrorString(err));

		return;
	}

	UINT i;
	ALuint buffer;

	// check if m_CurrentTrack is valid - returning if not. (perhaps other actions can be done as well)
	if (m_CurrentTrack == m_Tracks.end())
	{
		return;
	}

	ISoundBufferStream *bufferStream = m_CurrentTrack->bufferStream;
	if (bufferStream == NULL)
	{
		m_ToolBox->Log(LOGWARNING, _T("CMusicPlaylist: buffer stream is null in update buffer queue\n"));
		return;
	}

	for (i=0; i<(UINT)processedBuffers; i++)
	{
		alSourceUnqueueBuffers(m_uiSoundSource, 1, &buffer);		
	
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
			m_ToolBox->Log( LOGWARNING, _T("Music playlist: OpenAL could not unqueue buffer: %s\n"), GetALErrorString(err));

			return;
		}
		
		if (m_CurrentTrack != m_Tracks.end())
		{			

			bufferStream->DisposeOldBuffer(buffer);

			if (bufferStream->IsEndOfStream())
			{
				m_CurrentTrack++;
				if (m_CurrentTrack == m_Tracks.end())
				{
					if (m_bLoopWhole)
					{
						// loop, so start over
						ResetPlaylistStreams();
						m_CurrentTrack = m_Tracks.begin();
					}
					else
					{
						// out of tracks
						return;
					}
				}

				bufferStream = m_CurrentTrack->bufferStream;
				
				if (bufferStream == NULL)
				{
					m_ToolBox->Log(LOGWARNING, _T("CMusicPlaylist: buffer stream is null in update buffer queue\n"));
					return;
				}	
			}
		}
	}

	// double check...
	ALint queuedBuffers;
	alGetSourcei(m_uiSoundSource, AL_BUFFERS_QUEUED, &queuedBuffers);
	if (queuedBuffers < (int)m_SoundUtility->GetNumSoundBuffers())
	{
		m_bWaitingForBuffer = true;
	}

	if (m_bWaitingForBuffer)
	{
		bool ret = bufferStream->GetBuffer(&buffer, false);
		if (!ret)
		{
			m_bWaitingForBuffer = true;
		}
		else
		{
			m_bWaitingForBuffer = false;			
			if (alIsBuffer(buffer) == AL_TRUE)
			{
				alSourceQueueBuffers(m_uiSoundSource, 1, &buffer);
				err = alGetError();
				if (err != AL_NO_ERROR)
				{
					m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
					m_ToolBox->Log( LOGWARNING, _T("Music playlist: OpenAL could not queue buffer: %s\n\tNOTE: all the PCM data in the playlist must be the same format\n"), GetALErrorString(err));

					return;
				}
			}
		}
	}
}

void CMusicPlaylist::ResetBuffers()
{
	ALenum err;
	ALint queuedBuffers;

	alGetSourcei(m_uiSoundSource, AL_BUFFERS_QUEUED, &queuedBuffers);
	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		m_ToolBox->Log(LOGWARNING, _T("Music playlist: could not get buffers queued in clear queue: %s\n"), 
			GetALErrorString(err));
		return;
	}

	for (int i=0; i<queuedBuffers; i++)
	{
		ALuint buffer;
		alSourceUnqueueBuffers(m_uiSoundSource, 1, &buffer);
	
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
			m_ToolBox->Log( LOGWARNING, _T("Music playlist: OpenAL could not unqueue buffer: %s\n"), GetALErrorString(err));

			return;
		}		

		if (m_CurrentTrack == m_Tracks.end())
		{
			m_CurrentTrack--;
		}

		ISoundBufferStream *bufferStream = m_CurrentTrack->bufferStream;
		if (bufferStream == NULL)
		{
			m_ToolBox->Log(LOGWARNING, _T("CMusicPlaylist: buffer stream is null in update buffer queue\n"));
			return;
		}

		bufferStream->DisposeOldBuffer(buffer);
	}

	TRACKLIST::iterator trackIter = m_Tracks.begin();
	for (; trackIter != m_Tracks.end(); trackIter++)
	{
		ISoundBufferStream *bufferStream = trackIter->bufferStream;
		if (bufferStream == NULL)
		{
			m_ToolBox->Log(LOGWARNING, _T("CMusicPlaylist: buffer stream is null in update buffer queue\n"));
			return;
		}

		bufferStream->ResetStream();
	}

	m_CurrentTrack = m_Tracks.begin();

	m_fCurrentTimePosition = 0.0f;
}

void CMusicPlaylist::FreeSource()
{
	ALenum err;
	if (m_bValidSource)
	{
		Stop();			

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
			ISoundBufferStream *bufferStream = m_CurrentTrack->bufferStream;
			if (bufferStream)
			{
				bufferStream->DisposeOldBuffer(buffer);
			}
		}

#ifdef XBOX
		alSourceSetTypeMusic(m_uiSoundSource, AL_FALSE);
#endif
		m_bValidSource = false;
	}
}

void CMusicPlaylist::UpdateLocalVolume(float fGlobalVolume)
{
	m_fGlobalVolume = fGlobalVolume;
	if (m_bValidSource)
	{
		if (m_SoundUtility != NULL)
		{
			m_SoundUtility->SetALSourceF(m_uiSoundSource, AL_GAIN, (m_fMasterVolume * m_fGlobalVolume), __FILE__, __LINE__);	
		}
	}
}

void CMusicPlaylist::ResetPlaylistStreams()
{
	TRACKLIST::iterator iter = m_Tracks.begin();
	for (; iter != m_Tracks.end(); iter++)
	{
		ISoundBufferStream *bufferStream = iter->bufferStream;
		if (bufferStream)
		{
			bufferStream->ResetStream();
		}
	}
}