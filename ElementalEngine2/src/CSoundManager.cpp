///============================================================================
/// \file		CSoundManager.cpp
/// \brief		Generic sound manager implementation
/// \date		04/01/2008
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
#include "CSoundManager.h"


REGISTER_COMPONENT_SINGLETON(CSoundManager);

REGISTER_MESSAGE_HANDLER(PlaySound, OnPlaySound, CSoundManager);
REGISTER_MESSAGE_HANDLER(StopSound, OnStopSound, CSoundManager);
REGISTER_MESSAGE_HANDLER(LoadMusicPlaylist, OnLoadMusicPlaylist, CSoundManager);
REGISTER_MESSAGE_HANDLER(RunPlaylist, OnRunPlaylist, CSoundManager);
REGISTER_MESSAGE_HANDLER(StopPlaylist, OnStopPlaylist, CSoundManager);
REGISTER_MESSAGE_HANDLER(Stop, OnStop, CSoundManager);
REGISTER_MESSAGE_HANDLER(Start, OnStart, CSoundManager);
REGISTER_MESSAGE_HANDLER(LoadingUpdate, OnLoadingUpdate, CSoundManager);
REGISTER_MESSAGE_HANDLER(MuteSounds, OnMuteSounds, CSoundManager);
REGISTER_MESSAGE_HANDLER(UnMuteSounds, OnUnMuteSounds, CSoundManager);
REGISTER_MESSAGE_HANDLER(MuteMusic, OnMuteMusic, CSoundManager);
REGISTER_MESSAGE_HANDLER(UnMuteMusic, OnUnMuteMusic, CSoundManager);
REGISTER_MESSAGE_HANDLER(GetGlobalSoundVolume, OnGetGlobalSoundVolume, CSoundManager);
REGISTER_MESSAGE_HANDLER(GetGlobalMusicVolume, OnGetGlobalMusicVolume, CSoundManager);
REGISTER_MESSAGE_HANDLER(SetGlobalSoundVolume, OnSetGlobalSoundVolume, CSoundManager);
REGISTER_MESSAGE_HANDLER(SetGlobalMusicVolume, OnSetGlobalMusicVolume, CSoundManager);
REGISTER_MESSAGE_HANDLER(ShutDownAudioEngine, OnShutDownAudioEngine, CSoundManager);

IComponent *CSoundManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CSoundManager);
}

CSoundManager::CSoundManager() : 
	CManager(_T("CSoundManager"), SOUNDPRIORITY),
	m_hsSoundObjTypeName(_T("CSoundObject")),
	m_hsPlaylistTypeName(_T("CMusicPlaylist")),
	m_hsManagerTypeName(_T("CSoundManager"))
{
	Init();

	m_fGlobalSoundVolume = 1.0f;
	m_fGlobalMusicVolume = 1.0f;
	m_fLastSoundVolume = 1.0f;
	m_fLastMusicVolume = 1.0f;

	m_pCurrentPlaylist = NULL;
}

CSoundManager::~CSoundManager()
{
	DeInit();
}

bool CSoundManager::Init()
{
	return CManager::Init();
}

bool CSoundManager::DeInit()
{
	return CManager::DeInit();
}

void CSoundManager::Update(DWORD tickCount)
{
	IDTOOBJECTMAP *objMap;

	objMap = GetObjectMap(&m_hsSoundObjTypeName);
	if (objMap != NULL)
	{
		IDTOOBJECTMAP::iterator mapIter = objMap->begin();
	
		while (mapIter != objMap->end())
		{
			IObject *object = mapIter->second;
			object->Update();
			mapIter++;
		}
	}

	objMap = GetObjectMap(&m_hsPlaylistTypeName);
	if (objMap != NULL)
	{
		IDTOOBJECTMAP::iterator mapIter = objMap->begin();
		for (; mapIter != objMap->end(); mapIter++)
		{
			IObject *object = mapIter->second;
			object->Update();
		}
	}

	// make sure the audio engine updates whatever else it needs to update now
	static DWORD msgUpdateAudioEngine = CHashString(_T("UpdateAudioEngine")).GetUniqueID();
	m_ToolBox->SendMessage(msgUpdateAudioEngine, 0, NULL);
}

DWORD CSoundManager::OnPlaySound(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(PLAYSOUNDPARAMS), size);
	PLAYSOUNDPARAMS psp = *(PLAYSOUNDPARAMS *)params;

	if ( psp.fileName == NULL )
	{
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log( LOGWARNING, _T("CSoundManager error: malformed PLAYSOUNDPARAMS struct (fileName is NULL)\n"));
		return MSG_NOT_HANDLED;
	}

	CHashString hsObjName;
	GENERATEUNIQUEOBJECTNAMEPARAMS guonp;
	guonp.name = psp.fileName;
	guonp.newname = &hsObjName;
	static DWORD msgGenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgGenerateUniqueObjectName, sizeof(GENERATEUNIQUEOBJECTNAMEPARAMS), &guonp) != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGERROR, _T("CSoundManager could not generate a unique object name for sound %s\n"), psp.fileName->GetString());
		return MSG_ERROR;
	}

	IComponent *soundComp = m_ToolBox->CreateComponent(&m_hsSoundObjTypeName, 3, &hsObjName, psp.hsParentName, psp.bInHierarchy);
	if (soundComp == NULL)
	{
		m_ToolBox->Log(LOGWARNING, _T("Sound manager: unable to create Sound Object %s, in file %s at line %d\n"),
			hsObjName.GetString(), __FILE__, __LINE__);
		return MSG_NOT_HANDLED;
	}

	OFACTORYADDPARAMS ofap;
	ofap.name = &hsObjName;
	ofap.component = soundComp;
	static DWORD msgHash_AddObjectToFactory = CHashString(_T("AddObjectToFactory")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddObjectToFactory, sizeof(OFACTORYADDPARAMS), &ofap);

	ISoundObject *newSoundObject = dynamic_cast<ISoundObject*>(soundComp);
	if (newSoundObject == NULL)
	{
		m_ToolBox->Log(LOGWARNING, _T("COpenALSoundManager: could not cast component to a CSoundObject!\n"));
		assert(newSoundObject);
		return MSG_NOT_HANDLED;
	}

	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL; // use internal buffer
	ca.streamSize = 0;
	CHashString memType(_T("Memory"));
	ca.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return MSG_ERROR;
	}
	IArchive *MemArchive = ca.archive;

	MemArchive->SetIsWriting(true);

	// some pre-checks
	bool autoRemove = true;
	bool is3DSound = false;

	// always have to auto remove if the remote creator isn't going to keep track of the object
	if (psp.objectName != NULL)
	{
		autoRemove = psp.autoRemove;
	}

	if (psp.is3DSound)
	{
		if (psp.soundPosition != NULL)
		{
			is3DSound = true;
		}
		else
		{
			m_ToolBox->Log(LOGWARNING, _T("Sound manager: play sound position is null for sound %s; 3D sound will play in 2D space\n"), psp.fileName->GetString());
		}
	}

	// coordinates with sound obj serialization
	MemArchive->Write(1.1f, _T("Version"));
	MemArchive->Write(psp.fileName->GetString(), _T("FileName"));
	MemArchive->Write(psp.volume, _T("MaxVolume"));
	MemArchive->Write(autoRemove, _T("AutoRemove"));
	MemArchive->Write(is3DSound, _T("Is3DSound"));	
	MemArchive->Write(psp.looping, _T("IsLooping"));
	MemArchive->Write(true, _T("AutoStart"));
	MemArchive->Write(psp.pitch, _T("PitchMultiplier"));

	MemArchive->SetIsWriting(false);

	SERIALIZEOBJECTPARAMS sop;
	sop.name = &hsObjName;
	sop.archive = MemArchive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);

	MemArchive->Close();	

	if (is3DSound == true)
	{
		newSoundObject->SetSoundPosition(*psp.soundPosition);
	}
	
	INITOBJECTPARAMS iop;
	iop.name = &hsObjName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);

	if (psp.objectName != NULL)
	{
		// copy string name into message hashstring
		*psp.objectName = hsObjName;
	}

	return MSG_HANDLED_STOP;
}

DWORD CSoundManager::OnStopSound(DWORD size, void *params)
{
	DWORD retval = MSG_NOT_HANDLED;

	VERIFY_MESSAGE_SIZE(sizeof(STOPSOUNDPARAMS), size);
	STOPSOUNDPARAMS *stop_sound = (STOPSOUNDPARAMS *)params;

	IHashString *stopName = stop_sound->soundObjectName;

	if (stopName)
	{
		IDTOOBJECTMAP *objMap = GetObjectMap(&m_hsSoundObjTypeName);
		
		DWORD findKey = stopName->GetUniqueID();

		if (objMap != NULL)
		{
			IDTOOBJECTMAP::iterator mapIter = objMap->find(findKey);

			if (mapIter != objMap->end())
			{
				ISoundObject *sndObject = dynamic_cast<ISoundObject *>(mapIter->second);
				if (sndObject != NULL)
				{
					bool ret = sndObject->Stop(stop_sound->fadeOutVal);
					if (ret == true)
					{
						retval = MSG_HANDLED_STOP;
					}
				}
				else
				{
					m_ToolBox->Log(LOGWARNING, _T("Dynamic cast failed for COpenALSoundObject in %s at %d\n"),
						__FILE__, __LINE__);						
				}
			}
		}

		if (retval != MSG_HANDLED_STOP)
		{
			m_ToolBox->Log(LOGWARNING, _T("Sound manager: Could not find sound object %s to stop!\n"), stopName->GetString());
		}
	}

	return retval;
}

DWORD CSoundManager::OnLoadMusicPlaylist(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(LOADPLAYLISTPARAMS));
	LOADPLAYLISTPARAMS lpp = *(LOADPLAYLISTPARAMS*) param;

	CHashString hsFileName(lpp.fileName);
	IDTOOBJECTMAP *objMap = GetObjectMap(&m_hsPlaylistTypeName);
	if (objMap != NULL)
	{
		IDTOOBJECTMAP::iterator plIter = objMap->find(hsFileName.GetUniqueID());
		// if it's not already in the map, don't need to make another playlist object
		if (plIter == objMap->end())
		{
			IComponent *playlistComp = m_ToolBox->CreateComponent(&m_hsPlaylistTypeName, 3, &hsFileName, NULL, false);
			if (playlistComp == NULL)
			{
				m_ToolBox->Log(LOGWARNING, _T("Unable to create object %s in %s at line %d\n"),
					hsFileName.GetString(), __FILE__, __LINE__);
				return MSG_NOT_HANDLED;
			}

			OFACTORYADDPARAMS ofap;
			ofap.name = &hsFileName;
			ofap.component = playlistComp;
			static DWORD msgHash_AddObjectToFactory = CHashString(_T("AddObjectToFactory")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_AddObjectToFactory, sizeof(OFACTORYADDPARAMS), &ofap);

			SERIALIZEOBJECTPARAMS sop;
			sop.name = &hsFileName;
			sop.archive = lpp.ar;
			static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

			INITOBJECTPARAMS iop;
			iop.name = &hsFileName;
			static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
		}
	}
	// if the map is null, it could be because there are no objects in it yet
	// so let's add one
	else
	{
		IComponent *playlistComp = m_ToolBox->CreateComponent(&m_hsPlaylistTypeName, 3, &hsFileName, NULL, false);
		if (playlistComp == NULL)
		{
			m_ToolBox->Log(LOGWARNING, _T("Unable to create object %s in %s at line %d\n"),
				hsFileName.GetString(), __FILE__, __LINE__);
			return MSG_NOT_HANDLED;
		}

		OFACTORYADDPARAMS ofap;
		ofap.name = &hsFileName;
		ofap.component = playlistComp;
		static DWORD msgHash_AddObjectToFactory = CHashString(_T("AddObjectToFactory")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_AddObjectToFactory, sizeof(OFACTORYADDPARAMS), &ofap);

		SERIALIZEOBJECTPARAMS sop;
		sop.name = &hsFileName;
		sop.archive = lpp.ar;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

		INITOBJECTPARAMS iop;
		iop.name = &hsFileName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
	}

	return MSG_HANDLED_STOP;
}

DWORD CSoundManager::OnRunPlaylist(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));

	IHashString *fileName = (IHashString*)param;
	IDTOOBJECTMAP::iterator plIter;	

	if (fileName)
	{
		// convert to lowercase hashstring
		StdString szFileName = fileName->GetString();
		szFileName.tolower();
		CHashString tempFileName(szFileName);

		if (m_pCurrentPlaylist)
		{
			m_pCurrentPlaylist->Stop();
		}

		IDTOOBJECTMAP *objMap = GetObjectMap(&m_hsPlaylistTypeName);
		bool found = false;
		if (objMap != NULL)
		{
			plIter = objMap->find(tempFileName.GetUniqueID());

			if (plIter != objMap->end())
			{
				found = true;
			}
		}

		if (!found)
		{

			// not in map... try to load and check again
			LOADFILEEXTPARAMS lfep;
			lfep.fileName = (TCHAR*) tempFileName.GetString();
			lfep.bInternalLoad = true;
			static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
			
			objMap = GetObjectMap(&m_hsPlaylistTypeName);
			if (objMap != NULL)
			{
				plIter = objMap->find(tempFileName.GetUniqueID());
				if (plIter != objMap->end())
				{
					found = true;
				}
			}

			if (!found)
			{
				m_ToolBox->Log(LOGWARNING, _T("Sound manager: could not open playlist file %s\n"), tempFileName.GetString());
				return MSG_NOT_HANDLED;
			}
		}
		
		m_pCurrentPlaylist = dynamic_cast<ISoundObject *>(plIter->second);
		if (m_pCurrentPlaylist != NULL)
		{
			m_pCurrentPlaylist->Play();
		}
		else
		{
			m_ToolBox->Log(LOGWARNING, _T("Dynamic cast failed for CMusicPlayList in %s at line %d\n"),
				__FILE__, __LINE__);
			return MSG_NOT_HANDLED;
		}		
	}

	return MSG_HANDLED_STOP;
}

DWORD CSoundManager::OnStopPlaylist(DWORD size, void *param)
{
	if ((m_pCurrentPlaylist) && (m_pCurrentPlaylist->IsPlaying()))
	{
		m_pCurrentPlaylist->Stop();
	}

	return MSG_HANDLED_STOP;
}

DWORD CSoundManager::OnStop(DWORD size, void *param)
{
	IDTOOBJECTMAP *soundObjMap = GetObjectMap(&m_hsSoundObjTypeName);
	if (soundObjMap != NULL)
	{
		IDTOOBJECTMAP::iterator somIter = soundObjMap->begin();
		for (; somIter != soundObjMap->end(); somIter++)
		{
			ISoundObject *soundObject = dynamic_cast<ISoundObject*>(somIter->second);
			if (soundObject == NULL)
			{
				m_ToolBox->Log(LOGWARNING, _T("Sound manager: could not dynamic cast to ISoundObject in stop!\n"));
			}
			else
			{
				bool ret = soundObject->Pause();
				if (ret == false)
				{
					IHashString *objName = soundObject->GetName();
					m_ToolBox->Log(LOGWARNING, _T("Sound manager: could not pause sound %s!\n"), objName->GetString());
				}
			}
		}
	}

	if (m_pCurrentPlaylist != NULL)
	{
		if (m_pCurrentPlaylist->IsPlaying())
		{
			bool ret = m_pCurrentPlaylist->Pause();
			if (ret == false)
			{
				m_ToolBox->Log(LOGWARNING, _T("Sound manager: could not pause current playlist!\n"));
			}
		}
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CSoundManager::OnStart(DWORD size, void *param)
{
	IDTOOBJECTMAP *soundObjMap = GetObjectMap(&m_hsSoundObjTypeName);
	if (soundObjMap != NULL)
	{
		IDTOOBJECTMAP::iterator somIter = soundObjMap->begin();
		for (; somIter != soundObjMap->end(); somIter++)
		{
			ISoundObject *soundObject = dynamic_cast<ISoundObject*>(somIter->second);
			if (soundObject == NULL)
			{
				m_ToolBox->Log(LOGWARNING, _T("Sound manager: could not dynamic cast to ISoundObject in stop!\n"));
			}
			else
			{
				if (soundObject->WasPlaying())
				{
					bool ret = soundObject->Play();
					if (ret == false)
					{
						IHashString *objName = soundObject->GetName();
						m_ToolBox->Log(LOGWARNING, _T("Sound manager: could not play sound %s!\n"), objName->GetString());
					}
				}
			}
		}
	}

	if (m_pCurrentPlaylist != NULL)
	{
		if (m_pCurrentPlaylist->WasPlaying())
		{
			bool ret = m_pCurrentPlaylist->Play();
			if (ret == false)
			{
				m_ToolBox->Log(LOGWARNING, _T("Sound manager: could not pause current playlist!\n"));
			}
		}
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CSoundManager::OnLoadingUpdate(DWORD size, void *param)
{
	// loading a worldfile, go ahead and tick
	Update(0);
	return MSG_HANDLED_PROCEED;
}

void CSoundManager::SetGlobalSoundVolume(float volume)
{
	m_fGlobalSoundVolume = volume;

	if (m_fGlobalSoundVolume > 1.0f)
	{
		m_fGlobalSoundVolume = 1.0f;
	}

	if (m_fGlobalSoundVolume < 0.0f)
	{
		m_fGlobalSoundVolume = 0.0f;
	}

	IDTOOBJECTMAP *soundObjMap = GetObjectMap(&m_hsSoundObjTypeName);
	if (soundObjMap != NULL)
	{
		IDTOOBJECTMAP::iterator mapIter = soundObjMap->begin();
		for (; mapIter != soundObjMap->end(); mapIter++)
		{
			ISoundObject *soundObj = dynamic_cast<ISoundObject*>(mapIter->second);
			if (soundObj == NULL)
			{
				m_ToolBox->Log(LOGWARNING, _T("Sound manager: could not cast to isoundobject in set global sound volume!\n"));
				continue;
			}
			soundObj->UpdateLocalVolume(m_fGlobalSoundVolume);
		}
	}	
}
	
void CSoundManager::SetGlobalMusicVolume(float volume)
{
	m_fGlobalMusicVolume = volume;

	if (m_fGlobalMusicVolume > 1.0f)
	{
		m_fGlobalMusicVolume = 1.0f;
	}

	if (m_fGlobalMusicVolume < 0.0f)
	{
		m_fGlobalMusicVolume = 0.0f;
	}

	IDTOOBJECTMAP *playlistObjMap = GetObjectMap(&m_hsPlaylistTypeName);
	if (playlistObjMap != NULL)
	{
		IDTOOBJECTMAP::iterator mapIter = playlistObjMap->begin();
		for (; mapIter != playlistObjMap->end(); mapIter++)
		{
			ISoundObject *playlistObj = dynamic_cast<ISoundObject*>(mapIter->second);
			if (playlistObj == NULL)
			{
				m_ToolBox->Log(LOGWARNING, _T("Sound manager: could not cast to isoundobject in set global music volume!\n"));
				continue;
			}
			playlistObj->UpdateLocalVolume(m_fGlobalMusicVolume);
		}
	}
}

DWORD CSoundManager::OnMuteSounds(DWORD size, void *param)
{
	if (m_fGlobalSoundVolume > 0.0f)
	{
		m_fLastSoundVolume = m_fGlobalSoundVolume;
		SetGlobalSoundVolume(0.0f);		
	}
	return MSG_HANDLED_STOP;
}
	
DWORD CSoundManager::OnUnMuteSounds(DWORD size, void *param)
{
	if (m_fGlobalSoundVolume == 0.0f)
	{
		SetGlobalSoundVolume(m_fLastSoundVolume);
	}
	return MSG_HANDLED_STOP;
}

DWORD CSoundManager::OnMuteMusic(DWORD size, void *param)
{
	if (m_fGlobalMusicVolume > 0.0f)
	{
		m_fLastMusicVolume = m_fGlobalMusicVolume;
		SetGlobalMusicVolume(0.0f);
	}
	return MSG_HANDLED_STOP;
}

DWORD CSoundManager::OnUnMuteMusic(DWORD size, void *param)
{
	if (m_fGlobalMusicVolume == 0.0f)
	{
		SetGlobalMusicVolume(m_fLastMusicVolume);
	}
	return MSG_HANDLED_STOP;
}

DWORD CSoundManager::OnGetGlobalSoundVolume(DWORD size, void* param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float *outVol = (float*)param;
	*outVol = m_fGlobalSoundVolume;
	return MSG_HANDLED_STOP;
}
	
DWORD CSoundManager::OnGetGlobalMusicVolume(DWORD size, void* param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float *outVol = (float*)param;
	*outVol = m_fGlobalMusicVolume;
	return MSG_HANDLED_STOP;
}

DWORD CSoundManager::OnSetGlobalSoundVolume(DWORD size, void* param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float inVolume = *(float*)param;

	SetGlobalSoundVolume(inVolume);

	// save off explicitly set volume
	m_fLastSoundVolume = m_fGlobalSoundVolume;

	return MSG_HANDLED_STOP;
}

DWORD CSoundManager::OnSetGlobalMusicVolume(DWORD size, void* param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float inVolume = *(float*)param;
	
	SetGlobalMusicVolume(inVolume);

	// save off explicitly set volume
	m_fLastMusicVolume = m_fGlobalMusicVolume;

	return MSG_HANDLED_STOP;
}

DWORD CSoundManager::OnShutDownAudioEngine(DWORD size, void* params)
{
	DeInit();
	return MSG_HANDLED_PROCEED;
}