///============================================================================
/// \file		OpenALManager.h
/// \brief		Declaration of OpenAL sound manager
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

#ifndef _OPENALSOUNDMANAGER_H_
#define _OPENALSOUNDMANAGER_H_

#ifndef	_COBJECTTEMPLATE_HPP
#include "CObjectTemplate.hpp"
#endif

class COpenALSoundObject;

using namespace std;

typedef map<DWORD, ALuint> AUDIOBUFFERMAPBYFILE;
typedef map<ALenum, DWORD> ALENUMTOSTRINGMAP;
struct BUFFERSTREAMDATA
{
	IEEStream *pStream;
	void *pData;
};
typedef map<DWORD, BUFFERSTREAMDATA> BUFFERSTREAMDATABYFILE;

typedef list<ALuint> FREESOURCELIST;
typedef map<ALuint, ISoundObject*> USEDSOURCEMAP;

typedef map<DWORD, GLOBALSOUNDDATA> SOUNDKEYMAP;
typedef map<DWORD, DWORD> FILETOKEYMAP;

// tolua_begin
class COpenALSoundUtility : public IComponent
{
// tolua_end
	SINGLETONCONSTRUCTOROVERRIDE(COpenALSoundUtility);
protected:
	/// Default ctor
	COpenALSoundUtility();
public:

	/// Default Destructor
	~COpenALSoundUtility();

	/// \brief Init Function
	virtual void Init();

	/// \brief Update function
	virtual bool Update();

	/// \brief DeInit Function
	virtual void DeInit();

	/// \brief Create an instance
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	virtual void Serialize(IArchive &ar) {}
	virtual IHashString* GetComponentType() { return &m_HashName; }
	virtual bool IsKindOf(IHashString *compType)
	{
		if ((compType) && (compType->GetUniqueID() == m_HashName.GetUniqueID()))
		{
			return true;
		}
		return false;
	}
	

	/// \brief  Creates an OpenAL buffer from a file name
	/// \return The sound buffer
	ALuint GetSoundBuffer( DWORD fileName );

	/// Gets the loaded sound stream given a hash key
	/// \param fileName = hash key for sound
	/// \return stream, if loaded / available
	IEEStream* GetSoundStreamData(DWORD fileName);

	/// Gets a sound source from the pool, if it's high enough priority
	/// NOTE: can preempt lower priority sounds
	/// \param object = object requesting sound source
	/// \return sound source if available 
	ALuint GetSoundSource(ISoundObject *object);

	/// Returns an emptied sound source to the pool
	/// \param soundSource = sound source to return
	void ReturnSoundSource(ALuint soundSource);

	/// Sets an al source property float type
	/// \param source = al source to set property of
	/// \param type = al property to set
	/// \param value = value to set
	/// \param file = calling file reference
	/// \param line = line number of fn call
	/// \return true if successful
	inline bool SetALSourceF(ALuint source, ALenum type, float value, TCHAR* file, UINT line)
	{
		ALenum err;

		alSourcef(source, type, value);
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			CHashString hsTypeName(_T("-- unregistered --"));
			ALENUMTOSTRINGMAP::iterator enumIter = m_ALEnumToStringMap.find(type);
			if (enumIter != m_ALEnumToStringMap.end())
			{
				hsTypeName = m_ToolBox->GetHashString(enumIter->second);
			}

			m_ToolBox->Log(LOGWARNING, _T("%s(%d): Could not set source param %s: %s\n"), 
				file, line, hsTypeName.GetString(), GetALErrorString(err));
			return false;
		}

		return true;
	}	

	/// Sets an al source property float vector
	/// \param source = al source to set property of
	/// \param type = al property to set
	/// \param value = value to set
	/// \param file = calling file reference
	/// \param line = line number of fn call
	/// \return true if successful
	inline bool SetALSourceFV(ALuint source, ALenum type, float *value, TCHAR* file, UINT line)
	{
		ALenum err;

		alSourcefv(source, type, value);
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			CHashString hsTypeName(_T("-- unregistered --"));
			ALENUMTOSTRINGMAP::iterator enumIter = m_ALEnumToStringMap.find(type);
			if (enumIter != m_ALEnumToStringMap.end())
			{
				hsTypeName = m_ToolBox->GetHashString(enumIter->second);
			}

			m_ToolBox->Log(LOGWARNING, _T("%s(%d): Could not set source param %s: %s\n"), 
				file, line, hsTypeName.GetString(), GetALErrorString(err));
			return false;
		}

		return true;
	}	

	/// Sets an al source property int type
	/// \param source = al source to set property of
	/// \param type = al property to set
	/// \param value = value to set
	/// \param file = calling file reference
	/// \param line = line number of fn call
	/// \return true if successful
	inline bool SetALSourceI(ALuint source, ALenum type, int value, TCHAR* file, UINT line)
	{
		ALenum err;

		alSourcei(source, type, value);
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			CHashString hsTypeName(_T("-- unregistered --"));
			ALENUMTOSTRINGMAP::iterator enumIter = m_ALEnumToStringMap.find(type);
			if (enumIter != m_ALEnumToStringMap.end())
			{
				hsTypeName = m_ToolBox->GetHashString(enumIter->second);
			}

			m_ToolBox->Log(LOGWARNING, _T("%s(%d): Could not set source param %s: %s\n"), 
				file, line, hsTypeName.GetString(), GetALErrorString(err));
			return false;
		}

		return true;
	}	

	/// Gets an al buffer int value
	/// \param buffer = al buffer to get property of
	/// \param type = al property to get
	/// \param value = value to get
	/// \param file = calling file reference
	/// \param line = line number of fn call
	/// \return true if successful
	inline bool GetALBufferI(ALuint buffer, ALenum type, int *value, TCHAR* file, UINT line)
	{
		ALenum err;

		alGetBufferi(buffer, type, value);
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			CHashString hsTypeName(_T("-- unregistered --"));
			ALENUMTOSTRINGMAP::iterator enumIter = m_ALEnumToStringMap.find(type);
			if (enumIter != m_ALEnumToStringMap.end())
			{
				hsTypeName = m_ToolBox->GetHashString(enumIter->second);
			}

			m_ToolBox->Log(LOGWARNING, _T("%s(%d): Could not get buffer param %s: %s\n"), 
				file, line, hsTypeName.GetString(), GetALErrorString(err));
			return false;
		}

		return true;
	}

	/// Gets the global atten scale
	/// \return global attenuation
	float GetAttenuationScale() { return m_fSoundScale * m_fGlobalAttenuationMultiplier; }

	/// Gets the global sound scale
	/// \return sound scale
	float GetUnitScale() { return m_fSoundScale; }	

	/// Adds used al memory to mem counter
	/// \param bytes = bytes to add
	void AddALMemUsageBytes(UINT bytes) { m_iLoadedALMemBytes += bytes; }

	/// Removes used al memory from mem counter
	/// \param bytes = bytes to remove
	void RemoveALMemUsageBytes(UINT bytes) { m_iLoadedALMemBytes -= bytes; }

	/// Creates a new buffer stream, depending on filename
	/// \param key = key to check for file
	/// \return new buffer stream (unmanaged; to be deleted in object)
	ISoundBufferStream* CreateBufferStream(DWORD key);

	/// Gets the globally specified volume for a sound
	/// \param key = sound key
	/// \return volume of sound in db
	float GetSoundVolume(DWORD key);

	/// Forces the sound into the sound map (since playlists are specified elsewhere)
	/// \param key = key to access sound with
	/// \param fileName = filename of sound
	/// \param volume = volume to set, if necessary
	void ForceSoundIntoMap(DWORD key, DWORD fileName, float volume, bool bCacheUncompressed);

	/// Gets the globally set buffer duration
	/// \return duration
	float GetBufferDuration() { return m_fBufferTimeDuration; }

	/// Gets the number of sound buffers to use
	/// \return num buffers
	UINT GetNumSoundBuffers() { return m_iNumSoundBuffers; }

	// ** MESSAGES ** //

	/// \brief	Create a global sound
	DWORD OnAddSound(DWORD size, void *params);
	
	/// Sets the listening vectors for all sounds
	/// \param size = sizeof(SOUNDVECTORPARAMS)
	/// \param params = pointer to SOUNDVECTORPARAMS
	DWORD OnSetListenerVectors(DWORD size, void *params);

	/// Gets info pertinent to a sound
	/// \param size = sizeof(GETSOUNDFILEINFOPARAMS)
	/// \param param = GETSOUNDFILEINFOPARAMS struct
	/// \return MSG_HANDLED_STOP
	DWORD OnGetSoundFileInfo(DWORD size, void *param);	

	/// Kills al, making sure everything gets cleared properly
	/// \param size = 0
	/// \param params = NULL
	/// \return MSG_HANDLED_PROCEED
	DWORD OnShutDownAudioEngine(DWORD size, void* params);

	/// Loads the sound database, using the schema in common/databases
	/// \param size = sizeof(IHashString)
	/// \param param = IHashString file name
	/// \return MSG_HANDLED_STOP
	DWORD OnLoadSoundDatabase(DWORD size, void* param);

	/// Sets the desired length of a buffer for multibuffering
	/// \param size = sizeof(float)
	/// \param param = float length in seconds
	/// \return MSG_HANDLED_STOP
	DWORD OnSetAudioBufferLength(DWORD size, void *param);

	/// Sets the number of buffers for multibuffering
	/// \param size = sizeof(DWORD)
	/// \param param = DWORD number of buffers
	/// \return MSG_HANDLED_STOP
	DWORD OnSetAudioBufferCount(DWORD size, void *param);	

	/// Sets the global scale of the sound engine (affects speed of sound, doppler, attenuation)
	/// \param size = sizeof(float)
	/// \param param = float to set scale to
	/// \return MSG_HANDLED_STOP
	DWORD OnSetSoundScale(DWORD size, void *param);

	/// Sets the global attenuation multiplier
	/// \param size = sizeof(float)
	/// \param param = float to set multiplier to
	/// \return MSG_HANDLED_PROCEED
	DWORD OnSetGlobalAttenuationMultiplier(DWORD size, void *param);

	/// Sets the global doppler factor
	/// \param size = sizeof(float)
	/// \param param = float to set doppler to
	/// \return MSG_HANDLED_PROCEED
	DWORD OnSetGlobalDopplerFactor(DWORD size, void *param);

	/// Ticks the audio engine, updating position, orientation, and velocity of listener
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_PROCEED
	DWORD OnUpdateAudioEngine(DWORD size, void *param);

private:
	
	/// Reads in all the data from a stream and loads it into ram
	/// \param stream = stream to load data from
	/// \param bufferSize = output size of resultant buffer
	/// \return mem buffer filled with data
	void* GetStreamData(IEEStream *stream, UINT *bufferSize);

	/// Updates listener vectors in oal
	void UpdateListenerVectors();

	/// Calculates the sound object priority based on position and volume
	/// \param object = sound object to calc priority for
	/// \return priority value
	float CalculateSoundObjPriority(ISoundObject *object);

	/// map of buffers by file name
	AUDIOBUFFERMAPBYFILE m_AudioBufferMap;

	/// enum string stuffs
	void InitEnumToStringMap();
	ALENUMTOSTRINGMAP m_ALEnumToStringMap;

	lua_State *m_LuaState;

	CMusicPlaylist *m_CurrentPlaylist;

	CHashString m_HashName;
	CHashString m_soundObjTypeName;
	CHashString m_playlistTypeName;

	BUFFERSTREAMDATABYFILE m_BufferStreamMap;

	Vec3 m_CurrentListenerPosition;
	Vec3 m_LastListenerPosition;
	Matrix4x4 m_CurrentListenerOrientation;

	float m_fSoundScale;
	// dummy sound window
	HWND m_hWndSound;

	bool m_bInitialized;

	// sound source stuffs
	ALuint *m_SoundSources;
	UINT m_iMaxNumSources;
	FREESOURCELIST m_FreeSources;
	USEDSOURCEMAP m_UsedSourceSoundMap;
	USEDSOURCEMAP m_UsedSourceMusicMap;

	SOUNDKEYMAP m_SoundMap;
	FILETOKEYMAP m_FileToKeyMap;

	float m_fBufferTimeDuration;
	UINT m_iNumSoundBuffers;

	UINT m_iLoadedALMemBytes;

	IToolBox *m_ToolBox;
	ITimer *m_Timer;

	float m_fGlobalAttenuationMultiplier;
	float m_fGlobalDopplerFactor;

	bool m_bUpdateVelocity;
};

#endif //_OPENALSOUNDMANAGER_H_
 
