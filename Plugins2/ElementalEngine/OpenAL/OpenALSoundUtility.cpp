///============================================================================
/// \file		OpenALManager.cpp
/// \brief		Implementation of OpenAL manager class
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
#include "performanceprofiler.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "databasedefines.hpp"

TOLUA_API int tolua_OpenALLua_open (lua_State* tolua_S);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON(COpenALSoundUtility);

REGISTER_MESSAGE_HANDLER(AddSound, OnAddSound, COpenALSoundUtility);
REGISTER_MESSAGE_HANDLER(SetListenerVectors, OnSetListenerVectors, COpenALSoundUtility);
REGISTER_MESSAGE_HANDLER(GetSoundFileInfo, OnGetSoundFileInfo, COpenALSoundUtility);
REGISTER_MESSAGE_HANDLER(ShutDownAudioEngine, OnShutDownAudioEngine, COpenALSoundUtility);
REGISTER_MESSAGE_HANDLER(LoadSoundDatabase, OnLoadSoundDatabase, COpenALSoundUtility);
REGISTER_MESSAGE_HANDLER(SetAudioBufferLength, OnSetAudioBufferLength, COpenALSoundUtility);
REGISTER_MESSAGE_HANDLER(SetAudioBufferCount, OnSetAudioBufferCount, COpenALSoundUtility);
REGISTER_MESSAGE_HANDLER(SetSoundScale, OnSetSoundScale, COpenALSoundUtility);
REGISTER_MESSAGE_HANDLER(SetGlobalAttenuationMultiplier, OnSetGlobalAttenuationMultiplier, COpenALSoundUtility);
REGISTER_MESSAGE_HANDLER(SetGlobalDopplerFactor, OnSetGlobalDopplerFactor, COpenALSoundUtility);
REGISTER_MESSAGE_HANDLER(UpdateAudioEngine, OnUpdateAudioEngine, COpenALSoundUtility);


#define MAX_SOURCES 24
#define SPEED_OF_SOUND 343.3f

IComponent *COpenALSoundUtility::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(COpenALSoundUtility);
}

COpenALSoundUtility::COpenALSoundUtility() : 
	m_soundObjTypeName(_T("CSoundObject")),
	m_playlistTypeName(_T("CMusicPlaylist")),
	m_HashName(_T("COpenALSoundUtility"))
{
	m_ToolBox = EngineGetToolBox();
	if (m_ToolBox)
	{
		m_Timer = m_ToolBox->GetTimer();
	}

	m_hWndSound = NULL;
	m_CurrentPlaylist = NULL;
	m_fSoundScale = 1.0f;
	m_bInitialized = true;
	m_SoundSources = NULL;
	m_fBufferTimeDuration = 0.5f;
	m_iNumSoundBuffers = 3;
	m_iLoadedALMemBytes = 0;
	m_fGlobalAttenuationMultiplier = 25.0f;
	m_fGlobalDopplerFactor = 0.75f;
	m_bUpdateVelocity = true;
	InitEnumToStringMap();
	Init();
}

COpenALSoundUtility::~COpenALSoundUtility()
{
	DeInit();
}

void COpenALSoundUtility::Init()
{
	// hook into LUA
	m_LuaState = NULL;

	static DWORD msgHash_GetMasterScriptState = CHashString(_T("GetMasterScriptState")).GetUniqueID();
	if(m_ToolBox->SendMessage(msgHash_GetMasterScriptState, sizeof(lua_State *), &m_LuaState) == MSG_HANDLED)
	{
		// register our additional structures/handlers with LUA master
		tolua_OpenALLua_open(m_LuaState);
	}
	else
	{
		StdString error;
		error = _T("Error missing Master Script State Data\n");

		// log error
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
		EngineGetToolBox()->Log(LOGWARNING, error);
    }

	// this needs to be a message and stripped out of here....
#ifdef _WIN32
	// create invisible dummy sound window for DirectSound
	m_hWndSound = CreateWindowEx(0, "STATIC", "OpenALSoundWindow", 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		HWND_MESSAGE,
		(HMENU)NULL,
		NULL,
		NULL);

	if (m_hWndSound == NULL)
	{
		m_ToolBox->Log(LOGWARNING, _T("Unable to create sound window\n"));
	}
	else
	{
		SetForegroundWindow(m_hWndSound);
	}
#endif

	// Tell sound system to Start Up (default sound settings, so null is passed)
	ALboolean initResult = alutInit(NULL, NULL);

	if ( initResult == AL_FALSE )
	{
		// error initalizing OpenAL
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log(LOGWARNING, _T("Sound manager: error initalizing sound: %s\n"), alutGetErrorString(alutGetError()));
		return;
	}

	ALenum err;

	m_bInitialized = true;

	m_iMaxNumSources = MAX_SOURCES;
	bool bSourcesGenerated = false;

	do
	{
		m_SoundSources = new ALuint[m_iMaxNumSources];		
		
		alGenSources(m_iMaxNumSources, m_SoundSources);
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			delete [] m_SoundSources;
			m_SoundSources = NULL;

			// try decrementing the max number
			m_iMaxNumSources -= 4;
			if (m_iMaxNumSources <= 0)
			{
				m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
				m_ToolBox->Log( LOGERROR, _T("Sound manager error: can't generate audio sources: %s\n"), GetALErrorString(err));						
				break;
			}
		}
		else
		{
			bSourcesGenerated = true;
		}
	}while (!bSourcesGenerated);

	// load up free sources
	if (m_SoundSources != NULL)
	{
		for (UINT i=0; i<m_iMaxNumSources; i++)
		{
			m_FreeSources.push_back(m_SoundSources[i]);
		}
	}

	// ensure default position for listener
	ALfloat position[3] = { (ALfloat)0.0f,
							(ALfloat)0.0f,
							(ALfloat)0.0f };

	ALfloat ori[] = { -1.0, 0.0, 0.0,  0.0, 0.0, 1.0 };

	alListenerfv(AL_POSITION, position);
	alListenerfv(AL_ORIENTATION, ori);

	// set distance model for 3d sounds
	alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);

	// check if init of listener and distance model threw errors
	err = alGetError();
	if (err != AL_NO_ERROR) 
	{
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log( LOGWARNING, _T("Sound manager: OpenAL problem setting up OpenAL distance model: %s\n"), GetALErrorString(err));
		return;
	}

	alSpeedOfSound(343.3f * m_fSoundScale);
	err = alGetError();
	if (err != AL_NO_ERROR) 
	{
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log( LOGWARNING, _T("Sound manager: OpenAL problem setting up OpenAL speed of sound: %s\n"), GetALErrorString(err));
		return;
	}

	alDopplerFactor(m_fGlobalDopplerFactor * m_fSoundScale);
	err = alGetError();
	if (err != AL_NO_ERROR) 
	{
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log( LOGWARNING, _T("Sound manager: OpenAL problem setting up OpenAL doppler factor: %s\n"), GetALErrorString(err));
		return;
	}	
}

void COpenALSoundUtility::DeInit()
{
	// iterate through sounds and playlists to make sure they dump their buffers
	USEDSOURCEMAP::iterator soundSourceIter = m_UsedSourceSoundMap.begin();
	for (; soundSourceIter != m_UsedSourceSoundMap.end(); soundSourceIter++)
	{
		if (soundSourceIter->second)
		{
			soundSourceIter->second->FreeSource();
		}		
	}

	m_UsedSourceSoundMap.clear();

	USEDSOURCEMAP::iterator musicSourceIter = m_UsedSourceMusicMap.begin();
	for (; musicSourceIter != m_UsedSourceMusicMap.end(); musicSourceIter++)
	{
		if (musicSourceIter->second)
		{
			musicSourceIter->second->FreeSource();
		}		
	}

	m_UsedSourceMusicMap.clear();
	
	// clear buffers
	AUDIOBUFFERMAPBYFILE::iterator buffIter = m_AudioBufferMap.begin();
	for (; buffIter != m_AudioBufferMap.end(); buffIter++)
	{
		ALuint buffer = buffIter->second;
		int size=0;
		if (GetALBufferI(buffer, AL_SIZE, &size, __FILE__, __LINE__))
		{
			RemoveALMemUsageBytes((UINT)size);
		}
		alDeleteBuffers(1, &buffer);
		ALenum err = alGetError();
		if (err != AL_NO_ERROR)
		{
			m_ToolBox->Log(LOGWARNING, _T("Sound manager: could not delete buffer: %s\n"), GetALErrorString(err));
		}		
	}

	m_AudioBufferMap.clear();	

	// delete sound sources
	if (m_SoundSources != NULL)
	{
		ALenum err;
		alDeleteSources(m_iMaxNumSources, m_SoundSources);
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			m_ToolBox->Log(LOGWARNING, _T("Sound manager: could not delete sources: %s\n"), 
				GetALErrorString(err));
		}
		delete [] m_SoundSources;
		m_SoundSources = NULL;
	}		

	// try to close open al
	if (m_bInitialized)
	{
		ALboolean success = alutExit();
		if (success != AL_TRUE)
		{
			ALenum err = alutGetError();
			m_ToolBox->Log(LOGERROR, _T("Sound manager: could not properly shut down alut: %s\n"), alutGetErrorString(err));
		}
		m_bInitialized = false;
	}
	
	// close buffer streams
	if (m_BufferStreamMap.size() > 0)
	{
		BUFFERSTREAMDATABYFILE::iterator streamIter = m_BufferStreamMap.begin();
		while (streamIter != m_BufferStreamMap.end())
		{
			IEEStream *stream = streamIter->second.pStream;
			delete stream;
			free( streamIter->second.pData );
			streamIter++;
		}
		m_BufferStreamMap.clear();
	}

#ifdef _WIN32
	// destroy the window
	if (m_hWndSound != NULL)
	{
		DestroyWindow(m_hWndSound);
		m_hWndSound = NULL;
	}
#endif
}

bool COpenALSoundUtility::Update()
{
	UpdateListenerVectors();

	m_LastListenerPosition = m_CurrentListenerPosition;

	static DWORD msgUpdatePerformanceProfiler = CHashString(_T("UpdatePerformanceProfiler")).GetUniqueID();
	PERFORMANCEPROFILERUPDATE ppu;
	static CHashString hsMemUsage(_T("Memory Usage"));
	static CHashString hsOpenALMemUsage(_T("Loaded OpenAL buffer mem (kb)"));
	ppu.name = &hsOpenALMemUsage;
	ppu.time = (float)m_iLoadedALMemBytes/1024.0f;
	ppu.type = &hsMemUsage;
	ppu.displayType = NAME_NUMBER;

	m_ToolBox->SendMessage(msgUpdatePerformanceProfiler, sizeof(PERFORMANCEPROFILERUPDATE), &ppu);
	
	return true;
}

ALuint COpenALSoundUtility::GetSoundBuffer( DWORD fileName )
{
	if (fileName > 0)
	{
		AUDIOBUFFERMAPBYFILE::iterator soundBufferIter = m_AudioBufferMap.find(fileName);

		if ( soundBufferIter == m_AudioBufferMap.end() )
		{
			// not in buffer list, try to load

			LOADFILEEXTPARAMS lfep;
			lfep.fileName = (TCHAR*) m_ToolBox->GetHashString(fileName);
			lfep.bInternalLoad = true;
			static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);

			soundBufferIter = m_AudioBufferMap.find(fileName);

			if (soundBufferIter == m_AudioBufferMap.end())
			{
				m_ToolBox->Log(LOGWARNING, _T("Sound manager: could not get buffer for file %s\n"),
					m_ToolBox->GetHashString(fileName));
				return NULL;
			}
			else
			{
				return soundBufferIter->second;
			}
		}
		else
		{
			return soundBufferIter->second;
		}
	}
	return 0;
}

IEEStream* COpenALSoundUtility::GetSoundStreamData(DWORD fileName)
{
	if (fileName > 0)
	{
		BUFFERSTREAMDATABYFILE::iterator streamIter = m_BufferStreamMap.find(fileName);

		if (streamIter == m_BufferStreamMap.end())
		{
			// stream has not been loaded yet, try to load file by ext it
			LOADFILEEXTPARAMS lfep;
			lfep.fileName = (TCHAR*)m_ToolBox->GetHashString(fileName);
			lfep.bInternalLoad = true;
			static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
			DWORD retval = m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
			if (retval != MSG_HANDLED)
			{
				m_ToolBox->Log(LOGWARNING, _T("Sound manager: could not load stream file %s\n"),
					m_ToolBox->GetHashString(fileName));
				return NULL;
			}

			// check again
			streamIter = m_BufferStreamMap.find(fileName);
			if (streamIter == m_BufferStreamMap.end())
			{
				m_ToolBox->Log(LOGWARNING, _T("Sound manager: stream was not properly added to stream map!\n"));
				return NULL;
			}
			else
			{
				return streamIter->second.pStream;
			}
		}
		else
		{
			return streamIter->second.pStream;
		}
	}
	return NULL;
}

DWORD COpenALSoundUtility::OnAddSound(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(ADDSOUNDOBJECTPARAMS), size);
	ADDSOUNDOBJECTPARAMS *asparams = (ADDSOUNDOBJECTPARAMS *) params;

	IHashString *fileName = asparams->FileName;
	IHashString *ext = asparams->fileType;
	IEEStream *fileStream = asparams->fileStream;

	static CHashString hsWav(_T("wav"));
	static CHashString hsOgg(_T("ogg"));

	if ((fileName) && (ext) && (fileStream))
	{
		if (ext->GetUniqueID() == hsWav.GetUniqueID())
		{
			// can't stream wav files with alut function, so we have to load the whole thing
			// and add to buffer list
			AUDIOBUFFERMAPBYFILE::iterator iter = m_AudioBufferMap.find(fileName->GetUniqueID());

			if ( iter == m_AudioBufferMap.end() ) 
			{
				ALenum err;
				// now we have to load this raw buffer into OpenAL
				ALuint soundBuffer;

				void *data;
				UINT buffSize;
				data = GetStreamData(fileStream, &buffSize);

				if (data == NULL)
				{
					m_ToolBox->Log(LOGWARNING, _T("Sound manager: could not buffer data from stream for file %s\n"), 
						fileName->GetString());
					return MSG_NOT_HANDLED;
				}

				soundBuffer = alutCreateBufferFromFileImage(data, buffSize);

				free(data);
				
				err = alutGetError();
				if (err != ALUT_ERROR_NO_ERROR) 
				{
					m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
					m_ToolBox->Log( LOGWARNING, _T("Sound manager: alut couldn't buffer the data for file %s: %s\n"), fileName->GetString(), alutGetErrorString(err));
					return MSG_ERROR;
				}

				int size = 0;
				if (GetALBufferI(soundBuffer, AL_SIZE, &size, __FILE__, __LINE__))
				{
					AddALMemUsageBytes((UINT)size);
				}

				m_AudioBufferMap[fileName->GetUniqueID()] = soundBuffer;
			}			
		}
		else if (ext->GetUniqueID() == hsOgg.GetUniqueID())
		{
            // ogg files sorta have to be streamed, not really, but it'll probably help, 
			// so we have to copy the file data and make buffers at play time
			AUDIOBUFFERMAPBYFILE::iterator bufferIter = m_AudioBufferMap.find(fileName->GetUniqueID());
			BUFFERSTREAMDATABYFILE::iterator streamIter = m_BufferStreamMap.find(fileName->GetUniqueID());
			if ((streamIter == m_BufferStreamMap.end()) && (bufferIter == m_AudioBufferMap.end()))
			{
				// at some point, we may need to stream from disk, but for now, go ahead and load everything into
				// a mem stream -- the beauty of this system is that the IEEStream can be a file stream or a mem 
				// stream and the buffers will still function
				void *data;
				UINT buffSize;
				data = GetStreamData(fileStream, &buffSize);

				// NOTE: do not free this data here; it will be freed in the mem stream when it deinits

				CREATESTREAM cs;
				cs.mode = STREAM_MODE_READ;
				cs.streamData = data;
				cs.streamSize = buffSize;
				static DWORD msgHash_CreateStream_Memory = CHashString(_T("CreateStream_Memory")).GetUniqueID();
				DWORD retval = m_ToolBox->SendMessage(msgHash_CreateStream_Memory, sizeof(CREATESTREAM), &cs);
				if ((retval != MSG_HANDLED) || (cs.openStream == NULL))
				{
					m_ToolBox->Log(LOGWARNING, _T("Sound manager: could not create mem stream to store file data!\n"));
					
					free(data);
					return MSG_NOT_HANDLED;
				}
				else
				{
					// now check to see if this file is in the map, and if so, if we're caching buffers
					bool bKeepMemStream = true;
					FILETOKEYMAP::iterator fileMapIter = m_FileToKeyMap.find(fileName->GetUniqueID());
					if (fileMapIter != m_FileToKeyMap.end())
					{
						SOUNDKEYMAP::iterator mapIter = m_SoundMap.find(fileMapIter->second);
						if (mapIter != m_SoundMap.end())
						{
							if (mapIter->second.bCachedUncompressed)
							{
								bKeepMemStream = false;
								COGGSoundBufferStream tempBufferStream(fileName, cs.openStream);
								ALuint tempBuffer;
								
								tempBuffer = tempBufferStream.BufferFullSound();
								m_AudioBufferMap[fileName->GetUniqueID()] = tempBuffer;
							}
						}
					}
					
					if (bKeepMemStream)
					{
						m_BufferStreamMap[fileName->GetUniqueID()].pStream = cs.openStream;
						m_BufferStreamMap[fileName->GetUniqueID()].pData = data;
					}
					else
					{
						cs.openStream->Close();
						free(data);
					}
				}
			}			
		}

		// close and delete stream
		fileStream->Close();
		delete fileStream;
	}
	return MSG_HANDLED_STOP;
}

DWORD COpenALSoundUtility::OnSetListenerVectors(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(LISTENERVECTORPARAMS), size);
	LISTENERVECTORPARAMS lvp = *(LISTENERVECTORPARAMS *)params;

	if ( lvp.position != NULL )
	{
		m_CurrentListenerPosition.Set(*lvp.position);
	}

	if (lvp.orientation != NULL)
	{
		m_CurrentListenerOrientation.SetRotation(*lvp.orientation);
	}

	if (!lvp.bUpdateVelocity)
	{
		m_bUpdateVelocity = false;
	}

	return MSG_HANDLED_STOP;
}

void COpenALSoundUtility::UpdateListenerVectors()
{
	Vec3 diffVec = m_CurrentListenerPosition - m_LastListenerPosition;

	static CHashString hsGameTime(GAMEDELTATIMER);
	float deltaT = m_Timer->GetTimeDelta( &hsGameTime );

	ALenum err;

	// set position
	ALfloat position[3]= { (ALfloat)m_CurrentListenerPosition.x ,
						   (ALfloat)m_CurrentListenerPosition.y ,
						   (ALfloat)m_CurrentListenerPosition.z  };

	alListenerfv(AL_POSITION, position);

	// set orientation
	
	Vec3 forwardVec, upVec;
	// UP IS FLIPPED BECAUSE AL, FOR WHATEVER STUPID REASON, DOES NOT ALLOW YOU TO 
	// SPECIFY A 'RIGHT' VECTOR, AND OUR SYSTEM IS OPPOSITE THEIR INTERNAL COORDINATE SYSTEM
	upVec = -m_CurrentListenerOrientation.GetAxisY();
	forwardVec = m_CurrentListenerOrientation.GetAxisZ();
	
	float orientation[6] = { forwardVec.x, forwardVec.y, forwardVec.z,
								upVec.x, upVec.y, upVec.z};

	alListenerfv(AL_ORIENTATION, orientation);
	
	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log( LOGWARNING, _T("Sound manager: OpenAL problem setting listener orientation: %s\n"), GetALErrorString(err));
	}

	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log( LOGWARNING, _T("Sound manager: OpenAL problem setting listener position: %s\n"), GetALErrorString(err));
	}

	if (m_bUpdateVelocity)
	{
		// this denominator is time * scale, which will translate the displacement vector into meters/sec
		float denom = deltaT * m_fSoundScale;
		if (denom > 0)
		{
			diffVec /= denom;
		}
		else
		{
			diffVec.Set(0.0f, 0.0f, 0.0f);
		}
	}
	else
	{
		m_bUpdateVelocity = true; // update next frame
		diffVec.Set(0.0f, 0.0f, 0.0f);
	}

	float alVeloc[3] = {diffVec.x, diffVec.y, diffVec.z};	

	alListenerfv(AL_VELOCITY, alVeloc);

	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log( LOGWARNING, _T("Sound manager: OpenAL problem setting listener velocity: %s\n"), GetALErrorString(err));
	}
}

DWORD COpenALSoundUtility::OnGetSoundFileInfo(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETSOUNDFILEINFOPARAMS));

	DWORD retval = MSG_NOT_HANDLED;

	GETSOUNDFILEINFOPARAMS *gsfip = (GETSOUNDFILEINFOPARAMS*)param;
	IHashString *fileName = gsfip->fileName;

	if (fileName)
	{
		AUDIOBUFFERMAPBYFILE::iterator mapIter = m_AudioBufferMap.find(fileName->GetUniqueID());

		if (mapIter != m_AudioBufferMap.end())
		{
			ALuint buffer = mapIter->second;
			ALint freq, bits, channels, size;
			
			GetALBufferI(buffer, AL_FREQUENCY, &freq, __FILE__, __LINE__);
			gsfip->frequency = (UINT)freq;

			GetALBufferI(buffer, AL_BITS, &bits, __FILE__, __LINE__);
			gsfip->bitDepth = (UINT)bits;
            
			gsfip->bitRate = (UINT)(freq * bits); // this is in bits per sec

			GetALBufferI(buffer, AL_CHANNELS, &channels, __FILE__, __LINE__);
			gsfip->channels = (UINT)channels;

			GetALBufferI(buffer, AL_SIZE, &size, __FILE__, __LINE__);
			gsfip->fileSize = (UINT)size;

			if ((gsfip->bitRate > 0) && (gsfip->fileSize > 0))
			{
				gsfip->duration = ((float)gsfip->fileSize * 8.0f) / ((float)gsfip->bitRate * (float)gsfip->channels);
			}

			retval = MSG_HANDLED_STOP;
		}
		else
		{
			m_ToolBox->Log(LOGWARNING, _T("SoundManager: could not find file %s to get info of\n\tNOTE: if sound is streaming, the manager does not cache the al buffer\n"), 
				fileName->GetString());
		}
	}
	return retval;
}

ISoundBufferStream* COpenALSoundUtility::CreateBufferStream(DWORD key)
{
	if (key)
	{
		SOUNDKEYMAP::iterator soundIter = m_SoundMap.find(key);
		if (soundIter == m_SoundMap.end())
		{
			m_ToolBox->Log(LOGWARNING, _T("Sound manager: no file loaded for key %s; could not get buffer stream\n"),
				m_ToolBox->GetHashString(key));
			return NULL;
		}

		static CHashString hsWav(_T("wav"));
		static CHashString hsOgg(_T("ogg"));

		StdString current, extension, stdFileName;
		stdFileName = m_ToolBox->GetHashString((soundIter->second).dwFileName);

		CHashString hsFileName(stdFileName);

		stdFileName.GetToken(_T("."), current);
		while (_tcscmp(current, _T("")) != 0)
		{
			extension = current;
			stdFileName.GetToken(_T("."), current);
		}

		bool bCached = (soundIter->second).bCachedUncompressed;

		CHashString hsExt(extension);
		if ((hsExt.GetUniqueID() == hsWav.GetUniqueID()) || (bCached))
		{
			CCachedSoundBufferStream *ret = new CCachedSoundBufferStream(&hsFileName);
			return ret;
		}		
		else if (hsExt.GetUniqueID() == hsOgg.GetUniqueID())
		{
			COGGSoundBufferStream *ret = new COGGSoundBufferStream(&hsFileName);
			return ret;
		}

		return NULL;
	}
	else
	{
		return NULL;
	}
}

void* COpenALSoundUtility::GetStreamData(IEEStream *stream, UINT *bufferSize)
{
	if (stream)
	{
		void* data;
		
		// determine how much is in the stream, and malloc the data
		stream->SeekTo(0, SEEK_END);
		*bufferSize = (UINT)stream->Tell();
		data = malloc(*bufferSize);
		
		// read the data from the stream
		stream->SeekTo(0, SEEK_SET);
		stream->Read(data, *bufferSize);

		stream->Close();
		
		return data;
	}
	else
	{
		*bufferSize = 0;
		return NULL;
	}
}

ALuint COpenALSoundUtility::GetSoundSource(ISoundObject *object)
{
	ALuint retSource = 0;

	if (object == NULL)
	{
		return retSource;
	}
	static CHashString hsSoundPrior(_T("SOUND_PRIORITY"));
	static CHashString hsMusicPrior(_T("MUSIC_PRIORITY"));
	
	DWORD objectPrior = object->GetAudioPriority();
	
	// grab a free source, put it on the used map
	if (m_FreeSources.size() > 0)
	{
		FREESOURCELIST::iterator freeIter = m_FreeSources.begin();
		
		if (objectPrior == hsSoundPrior.GetUniqueID())
		{
			retSource = (*freeIter);
			m_FreeSources.erase(freeIter);

			m_UsedSourceSoundMap.insert(pair<ALuint, ISoundObject*>(retSource, object));
		}
		else if (objectPrior == hsMusicPrior.GetUniqueID())
		{
			retSource = (*freeIter);
			m_FreeSources.erase(freeIter);

			m_UsedSourceMusicMap.insert(pair<ALuint, ISoundObject*>(retSource, object));
		}		
	}

	// no free sources, must prioritize and kick someone out
	else
	{
		// priority based on apparent volume, so it should NEVER be this high
		float lowestPriority = 50.0f;
		USEDSOURCEMAP::iterator lowestIter;

		float requestPriority = CalculateSoundObjPriority(object);

		// iterate through sound map first
		USEDSOURCEMAP::iterator usedSoundIter = m_UsedSourceSoundMap.begin();
		for (; usedSoundIter != m_UsedSourceSoundMap.end(); usedSoundIter++)
		{
			ISoundObject *curObject = usedSoundIter->second;
			if (curObject == NULL)
			{
				continue;
			}

			float curPriority = CalculateSoundObjPriority(curObject);

			if (curPriority < lowestPriority)
			{
				lowestPriority = curPriority;
				lowestIter = usedSoundIter;
			}
		}

		if (lowestPriority < requestPriority)
		{
			ISoundObject *kickOutObject = lowestIter->second;
			kickOutObject->FreeSource();
			retSource = lowestIter->first;
			m_UsedSourceSoundMap.erase(lowestIter);
			
			if (objectPrior == hsSoundPrior.GetUniqueID())
			{
				m_UsedSourceSoundMap.insert(pair<ALuint, ISoundObject*>(retSource, object));
			}
			else if (objectPrior == hsMusicPrior.GetUniqueID())
			{
				m_UsedSourceMusicMap.insert(pair<ALuint, ISoundObject*>(retSource, object));
			}
		}

		else if (lowestPriority == 50.0f)
		{
			// hasn't changed, maybe there were no sounds in used sound map (all music???)
			// try kicking a music one
			USEDSOURCEMAP::iterator usedMusicIter = m_UsedSourceMusicMap.begin();
			for (; usedMusicIter != m_UsedSourceMusicMap.end(); usedMusicIter++)
			{
				ISoundObject *curObject = usedMusicIter->second;
				if (curObject == NULL)
				{
					continue;
				}

				float curPriority = CalculateSoundObjPriority(curObject);

				if (curPriority < lowestPriority)
				{
					lowestPriority = curPriority;
					lowestIter = usedMusicIter;
				}
			}

			if (lowestPriority < requestPriority)
			{
				ISoundObject *kickOutObject = lowestIter->second;
				kickOutObject->FreeSource();
				retSource = lowestIter->first;
				m_UsedSourceMusicMap.erase(lowestIter);
				
				if (objectPrior == hsSoundPrior.GetUniqueID())
				{
					m_UsedSourceSoundMap.insert(pair<ALuint, ISoundObject*>(retSource, object));
				}
				else if (objectPrior == hsMusicPrior.GetUniqueID())
				{
					m_UsedSourceMusicMap.insert(pair<ALuint, ISoundObject*>(retSource, object));
				}
			}
		}
	}	
	return retSource;
}

float COpenALSoundUtility::CalculateSoundObjPriority(ISoundObject *object)
{
	float volume = object->GetSoundVolume();
	float priority = volume;

	if (object->Is3DSound())
	{
		Vec3 objectPos = object->GetSoundPosition();
		Vec3 displacementVec = m_CurrentListenerPosition - objectPos;
		
		float distance = displacementVec.Length();
		float refDist = GetAttenuationScale();

		float gainFactor = (refDist) / (refDist + (distance - refDist));
		if (gainFactor > 1.0f)
		{
			gainFactor = 1.0f; // clamped
		}

		priority = volume * gainFactor;
	}
	
	static CHashString hsMusicPrior(_T("MUSIC_PRIORITY"));
	if (object->GetAudioPriority() == hsMusicPrior.GetUniqueID())
	{
		// artificially inflate music priority
		priority *= 5.0f;
	}
	return priority;
}

void COpenALSoundUtility::ReturnSoundSource(ALuint soundSource)
{

	USEDSOURCEMAP::iterator usedSoundIter = m_UsedSourceSoundMap.find(soundSource);
	if (usedSoundIter != m_UsedSourceSoundMap.end())
	{
		m_FreeSources.push_back(soundSource);
		m_UsedSourceSoundMap.erase(usedSoundIter);
	}
	else
	{
		USEDSOURCEMAP::iterator usedMusicIter = m_UsedSourceMusicMap.find(soundSource);
		if (usedMusicIter != m_UsedSourceMusicMap.end())
		{
			m_FreeSources.push_back(soundSource);
			m_UsedSourceMusicMap.erase(usedMusicIter);
		}	
	}
}

DWORD COpenALSoundUtility::OnShutDownAudioEngine(DWORD size, void* params)
{
	DeInit();
	return MSG_HANDLED_PROCEED;
}

DWORD COpenALSoundUtility::OnLoadSoundDatabase(DWORD size, void* param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));
	IHashString *dbFileName = (IHashString*) param;

	if ((dbFileName != NULL) && (_tcscmp(dbFileName->GetString(), _T("")) != 0))
	{
		TCHAR *fileName = _tcsdup(dbFileName->GetString());
		LOADFILEEXTPARAMS lfep;
		lfep.fileName = fileName;
		lfep.bInternalLoad = true;

		static DWORD msgLoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgLoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
		free( fileName );
		if (retval == MSG_HANDLED)
		{
			// clear prior sound list - not additive, exclusive!
			m_SoundMap.clear();
			
			CHashString hsSoundDBObject;
			DATABASEINFO dbi;
			dbi.m_FileName = dbFileName;
			static DWORD msgGetDatabase = CHashString(_T("GetDatabase")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgGetDatabase, sizeof(DATABASEINFO), &dbi);
			if (retval == MSG_HANDLED)
			{
				hsSoundDBObject.Init(dbi.m_DatabaseName->GetString());
				DATABASEATTRIBUTEPARAMS dap;
				CREATEARCHIVE ca;
				static CHashString memType(_T("Memory"));

				ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
				ca.streamData = NULL;
				ca.streamSize = 0;
				ca.streamType = &memType;
				static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);

				IArchive *attribArchive = ca.archive;
				if (attribArchive == NULL)
				{
					m_ToolBox->Log(LOGERROR, _T("Sound manager: archive is null; could not get sound attributes!\n\tNo sounds loaded!\n"));
					return MSG_ERROR;
				}

				dap.m_AttributeArchive = attribArchive;

				static DWORD msgGetAttribute = CHashString(_T("GetAttribute")).GetUniqueID();
				static CHashString hsKeyString(_T("KeyString"));
				static CHashString hsFileName(_T("FileName"));
				static CHashString hsGlobalVolume(_T("GlobalVolume"));
				static CHashString hsCachedUncompressed(_T("CacheUncompressed"));

				static CHashString hsDBType(_T("CDefinedDatabase"));
				for (UINT i=0; i<(UINT)dbi.m_nItems; i++)
				{
					// get key string
					attribArchive->SeekTo(0, SEEK_SET);
					dap.m_Index = i;
					dap.m_AttributeName = &hsKeyString;
					retval = m_ToolBox->SendMessage(msgGetAttribute, sizeof(DATABASEATTRIBUTEPARAMS), &dap, &hsSoundDBObject, &hsDBType);
					if (retval != MSG_HANDLED)
					{
						m_ToolBox->Log(LOGERROR, _T("Sound manager: could not get key attribute for row %d; sounds not fully loaded!\n"), i);
						attribArchive->Close();
						return MSG_ERROR;
					}

					CHashString hsKey;
					StdString szKey;
					attribArchive->SetIsWriting(false);
					attribArchive->Read(szKey);
					hsKey = szKey;

					// get filename
					attribArchive->SeekTo(0, SEEK_SET);
					dap.m_AttributeName = &hsFileName;
					retval = m_ToolBox->SendMessage(msgGetAttribute, sizeof(DATABASEATTRIBUTEPARAMS), &dap, &hsSoundDBObject, &hsDBType);
					if (retval != MSG_HANDLED)
					{
						m_ToolBox->Log(LOGERROR, _T("Sound manager: could not get file attribute for row %d; sounds not fully loaded!\n"), i);
						attribArchive->Close();
						return MSG_ERROR;
					}

					CHashString hsFile;
					StdString szFile;
					attribArchive->SetIsWriting(false);
					attribArchive->Read(szFile);
					szFile.tolower();
					hsFile = szFile;

					// get volume
					attribArchive->SeekTo(0, SEEK_SET);
					dap.m_AttributeName = &hsGlobalVolume;
					retval = m_ToolBox->SendMessage(msgGetAttribute, sizeof(DATABASEATTRIBUTEPARAMS), &dap, &hsSoundDBObject, &hsDBType);
					if (retval != MSG_HANDLED)
					{
						m_ToolBox->Log(LOGERROR, _T("Sound manager: could not get volume attribute for row %d; sounds not fully loaded!\n"), i);
						attribArchive->Close();
						return MSG_ERROR;
					}

					float fVolume;
					attribArchive->SetIsWriting(false);
					attribArchive->Read(fVolume);

					// get cached attribute
					attribArchive->SeekTo(0, SEEK_SET);
					dap.m_AttributeName = &hsCachedUncompressed;
					retval = m_ToolBox->SendMessage(msgGetAttribute, sizeof(DATABASEATTRIBUTEPARAMS), &dap, &hsSoundDBObject, &hsDBType);
					if (retval != MSG_HANDLED)
					{
						m_ToolBox->Log(LOGERROR, _T("Sound manager: could not get volume attribute for row %d; sounds not fully loaded!\n"), i);
						attribArchive->Close();
						return MSG_ERROR;
					}

					bool bCachedUncompressed;
					attribArchive->SetIsWriting(false);
					attribArchive->Read(bCachedUncompressed);

					if (_tcscmp(szKey, _T("GlobalSoundVolume")) == 0)
					{
						static DWORD msgSetGlobalSoundVolume = CHashString(_T("SetGlobalSoundVolume")).GetUniqueID();
						m_ToolBox->SendMessage(msgSetGlobalSoundVolume, sizeof(float), &fVolume);						
					}
					else if(_tcscmp(szKey, _T("GlobalMusicVolume")) == 0)
					{
						static DWORD msgSetGlobalMusicVolume = CHashString(_T("SetGlobalMusicVolume")).GetUniqueID();
						m_ToolBox->SendMessage(msgSetGlobalMusicVolume, sizeof(float), &fVolume);
					}
					else
					{					
						// insert into map
						GLOBALSOUNDDATA gsd;
						gsd.dwFileName = hsFile.GetUniqueID();
						gsd.fVolume = fVolume;
						gsd.bCachedUncompressed = bCachedUncompressed;

						m_SoundMap[hsKey.GetUniqueID()] = gsd;
						m_FileToKeyMap[hsFile.GetUniqueID()] = hsKey.GetUniqueID();
					}
				}

				attribArchive->Close();
			}
			else
			{
				m_ToolBox->Log(LOGERROR, _T("Sound manager: could not get database info!\n\tNo sounds loaded!\n"));
				return MSG_ERROR;
			}
		}
		else
		{
			m_ToolBox->Log(LOGERROR, _T("Sound manager: sound database failed to load!\n\tNo sounds loaded!\n"));
			return MSG_ERROR;
		}
	}
	return MSG_HANDLED_STOP;
}

float COpenALSoundUtility::GetSoundVolume(DWORD key)
{
	SOUNDKEYMAP::iterator sound = m_SoundMap.find(key);
	if (sound != m_SoundMap.end())
	{
		return (sound->second).fVolume;
	}
	else
	{
		m_ToolBox->Log(LOGWARNING, _T("Sound manager: no key entry for %s in get sound volume; sound will not play\n"),
			m_ToolBox->GetHashString(key));
		return 0.0f;
	}
}

void COpenALSoundUtility::ForceSoundIntoMap(DWORD key, DWORD fileName, float volume, bool bCacheUncompressed)
{
	GLOBALSOUNDDATA gsd;
	gsd.dwFileName = fileName;
	gsd.fVolume = volume;
	gsd.bCachedUncompressed = bCacheUncompressed;
	m_SoundMap[key] = gsd;
}

DWORD COpenALSoundUtility::OnSetAudioBufferLength(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float bufLen = *(float*)param;
	m_fBufferTimeDuration = bufLen;
	return MSG_HANDLED_STOP;
}

DWORD COpenALSoundUtility::OnSetAudioBufferCount(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(DWORD));
	DWORD numBufs = *(DWORD*)param;
	m_iNumSoundBuffers = numBufs;
	return MSG_HANDLED_STOP;
}

DWORD COpenALSoundUtility::OnSetSoundScale(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float soundScale = *(float*)param;

	m_fSoundScale = soundScale;

	alSpeedOfSound(SPEED_OF_SOUND * m_fSoundScale);
	ALenum err = alGetError();
	if (err != AL_NO_ERROR) 
	{
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log( LOGWARNING, _T("Sound manager: OpenAL problem setting up OpenAL speed of sound: %s\n"), GetALErrorString(err));
		return false;
	}

	alDopplerFactor(m_fGlobalDopplerFactor * m_fSoundScale);
	err = alGetError();
	if (err != AL_NO_ERROR) 
	{
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log( LOGWARNING, _T("Sound manager: OpenAL problem setting up OpenAL doppler factor: %s\n"), GetALErrorString(err));
		return false;
	}

	return MSG_HANDLED_STOP;
}

void COpenALSoundUtility::InitEnumToStringMap()
{
	CHashString hsALGain(_T("AL_GAIN"));
	CHashString hsALBuffer(_T("AL_BUFFER"));
	CHashString hsALMaxGain(_T("AL_MAX_GAIN"));
	CHashString hsALPosition(_T("AL_POSITION"));
	CHashString hsALRolloffFactor(_T("AL_ROLLOFF_FACTOR"));
	CHashString hsALLooping(_T("AL_LOOPING"));
	CHashString hsALReferenceDistance(_T("AL_REFERENCE_DISTANCE"));

	// TODO: add everything not here

	m_ALEnumToStringMap.insert(pair<ALenum, DWORD>(AL_GAIN, hsALGain.GetUniqueID()));
	m_ALEnumToStringMap.insert(pair<ALenum, DWORD>(AL_BUFFER, hsALBuffer.GetUniqueID()));
	m_ALEnumToStringMap.insert(pair<ALenum, DWORD>(AL_MAX_GAIN, hsALMaxGain.GetUniqueID()));
	m_ALEnumToStringMap.insert(pair<ALenum, DWORD>(AL_POSITION, hsALPosition.GetUniqueID()));
	m_ALEnumToStringMap.insert(pair<ALenum, DWORD>(AL_ROLLOFF_FACTOR, hsALRolloffFactor.GetUniqueID()));
	m_ALEnumToStringMap.insert(pair<ALenum, DWORD>(AL_LOOPING, hsALLooping.GetUniqueID()));
	m_ALEnumToStringMap.insert(pair<ALenum, DWORD>(AL_REFERENCE_DISTANCE, hsALReferenceDistance.GetUniqueID()));
}

DWORD COpenALSoundUtility::OnSetGlobalAttenuationMultiplier(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	
	float attenMult = *(float*)param;
	m_fGlobalAttenuationMultiplier = attenMult;

	return MSG_HANDLED_PROCEED;
}
	
DWORD COpenALSoundUtility::OnSetGlobalDopplerFactor(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	
	float dopplerFact = *(float*)param;
	m_fGlobalDopplerFactor = dopplerFact;

	return MSG_HANDLED_PROCEED;
}

DWORD COpenALSoundUtility::OnUpdateAudioEngine(DWORD size, void *param)
{
	Update();
	return MSG_HANDLED_PROCEED;
}