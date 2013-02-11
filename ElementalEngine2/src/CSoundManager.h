///============================================================================
/// \file		CSoundManager.h
/// \brief		Generic sound manager header
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

#ifndef _C_SOUNDMANAGER_H_
#define _C_SOUNDMANAGER_H_

#include "ISoundObject.hpp"

class CSoundManager: public CManager
{
	SINGLETONCONSTRUCTOROVERRIDE(CSoundManager);
protected:
	/// Default ctor
	CSoundManager();
public:
	/// Default dtor
	~CSoundManager();

	/// \brief Init Function
	virtual bool Init();

	/// \brief Update function
	virtual void Update(DWORD tickCount);

	/// \brief DeInit Function
	virtual bool DeInit();

	/// \brief Create an instance
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// MESSAGES ///

	/// Creates a sound object and plays it
	/// \param size = sizeof(PLAYSOUNDPARAMS)
	/// \param params = PLAYSOUNDPARAMS struct
	/// \return MSG_HANDLED_STOP
	DWORD OnPlaySound(DWORD size, void *params);

	/// Stops a global sound that is playing 
	/// \param size = sizeof(STOPSOUNDPARAMS)
	/// \param params = STOPSOUNDPARAMS struct
	/// \return MSG_HANDLED_STOP
	DWORD OnStopSound(DWORD size, void *params);

	/// Creates a music playlist object given a loaded file
	/// \param size = sizeof(LOADPLAYLISTPARAMS)
	/// \param param = LOADPLAYLISTPARAMS struct
	/// \return	MSG_HANDLED_STOP
	DWORD OnLoadMusicPlaylist(DWORD size, void *param);
	
	/// Sets the active running playlist, starts the playlist
	/// \param size = sizeof(IHashString*)
	/// \param param = IHashString* name of playlist (filename)
	/// \return MSG_HANDLED_STOP
	DWORD OnRunPlaylist(DWORD size, void *param);

	/// Stops the current running playlist
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_STOP
	DWORD OnStopPlaylist(DWORD size, void *param);

	/// Stops the audio by pausing all the sources
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_PROCEED
	DWORD OnStop(DWORD size, void *param);

	/// Unpauses any paused sounds that were stopped
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_PROCEED
	DWORD OnStart(DWORD size, void *param);

	/// Ticks the sound manager between loads to update sound objects to update the buffer queues
	/// \param size = sizeof(LOADINGUPDATEPARAMS)
	/// \param param = LOADINGUPDATEPARAMS struct
	/// \return MSG_HANDLED_PROCEED
	DWORD OnLoadingUpdate(DWORD size, void *param);

	/// Mutes all sounds (not music)
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_STOP
	DWORD OnMuteSounds(DWORD size, void *param);

	/// Unmutes all sounds by setting global sound volume to setting before being muted
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_STOP
	DWORD OnUnMuteSounds(DWORD size, void *param);

	/// Mutes music
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_STOP
	DWORD OnMuteMusic(DWORD size, void *param);

	/// Unmutes music by setting global music volume to prior setting
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_STOP
	DWORD OnUnMuteMusic(DWORD size, void *param);

	/// Gets the global sound volume
	/// \param size = sizeof(float)
	/// \param param = float to store value into
	/// \return MSG_HANDLED_STOP
	DWORD OnGetGlobalSoundVolume(DWORD size, void* param);

	/// Gets the global music volume
	/// \param size = sizeof(float)
	/// \param param = float to store value into
	/// \return MSG_HANDLED_STOP
	DWORD OnGetGlobalMusicVolume(DWORD size, void* param);

	/// Sets the global sound volume
	/// \param size = sizeof(float)
	/// \param param = float to set value from
	/// \return MSG_HANDLED_STOP
	DWORD OnSetGlobalSoundVolume(DWORD size, void* param);

	/// Sets the global music volume
	/// \param size = sizeof(float)
	/// \param param = float to set value from
	/// \return MSG_HANDLED_STOP
	DWORD OnSetGlobalMusicVolume(DWORD size, void* param);

	/// Deinitializes the manager
	/// \param size = 0
	/// \param params = NULL
	/// \return MSG_HANDLED_PROCEED
	DWORD OnShutDownAudioEngine(DWORD size, void* params);

private:	

	void SetGlobalSoundVolume(float volume);
	void SetGlobalMusicVolume(float volume);

	/// some types
	CHashString m_hsSoundObjTypeName;
	CHashString m_hsPlaylistTypeName;
	CHashString m_hsManagerTypeName;

	/// current playlist 
	/// TODO: support a/b and crossfade?
	ISoundObject *m_pCurrentPlaylist;

	float m_fGlobalSoundVolume;
	float m_fGlobalMusicVolume;

	float m_fLastSoundVolume;
	float m_fLastMusicVolume;
};

#endif //_C_SOUNDMANAGER_H_