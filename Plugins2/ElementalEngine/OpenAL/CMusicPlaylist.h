///============================================================================
/// \file		CMusicPlaylist.h
/// \brief		Playlist control for music files
/// \date		03/02/07
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

#ifndef _C_MUSICPLAYLIST_H_
#define _C_MUSICPLAYLIST_H_

typedef struct TRACKDATA
{
	IHashString *fileName;
	ISoundBufferStream *bufferStream;
	TRACKDATA()
	{
		fileName = NULL;
		bufferStream = NULL;
	}
}TRACKDATA;

typedef struct PLAYLISTENTRY
{
	CHashString fileName;
	UINT numLoops;
	PLAYLISTENTRY()
	{
		fileName = _T("");
		numLoops = 0;
	}
}PLAYLISTENTRY;

typedef list<PLAYLISTENTRY> ENTRYLIST;

typedef list<TRACKDATA> TRACKLIST;

class CMusicPlaylist : public CSceneObject<ISoundObject>
{
protected:
	/// ctor
	CMusicPlaylist(IHashString *parentName, IHashString *name, bool bAddToHier);
public:
	/// dtor
	virtual ~CMusicPlaylist();

	/// Creates a music playlist component 
	static IComponent *Create(int nArgs, va_list argptr);

	/// Serialize the playlist
	virtual void Serialize(IArchive &ar);

	/// Init
	virtual void Init();

	/// Deinit
	virtual void DeInit();

	/// Plays this playlist
	/// \return true if successful
	virtual bool Play();
	
	/// Stops the playlist
	/// \param fadeOutVal = amount of time in seconds to fade playlist out upon stopping
	/// \return true if successful
	virtual bool Stop(float fadeOutVal=0.0f);

	/// Pauses the playlist
	virtual bool Pause();

	/// Checks to see if the playlist is playing
	/// \return true / false playing or not
	virtual bool IsPlaying();

	/// Checks to see if the playlist was playing before we paused
	/// \return true if it was
	virtual bool WasPlaying() { return m_bWasPlaying; }

	/// Updates queue
	/// \return true if successful
	virtual bool Update();

	/// Allows the sound object to clear the buffer queue off it's source
	virtual void FreeSource();

	/// Gets the unique id of the hashstring for priority
	/// \return unique id of MUSIC_PRIORITY or SOUND_PRIORITY
	virtual DWORD GetAudioPriority() { return m_hsPriority.GetUniqueID(); }

	/// Checks to see if this sound is 2d or 3d
	/// \return true if 3d sound
	virtual bool Is3DSound() { return false; }

	/// Gets the position of a sound (if 3d)
	/// \param pos = position to set
	virtual void SetSoundPosition(Vec3 &pos){}

	/// Gets the position of a sound (if 3d)
	/// \return position
	virtual Vec3 GetSoundPosition()
	{
		Vec3 emptyVec; 
		return emptyVec;
	}

	/// Gets the volume (gain) of a sound
	/// \return volume of sound
	virtual float GetSoundVolume() { return m_fMasterVolume; }

	/// Tells the sound object to reset it's volume based on the new global volume
	/// \param fGlobalVolume = new global volume
	virtual void UpdateLocalVolume(float fGlobalVolume);

	/// Inherited fn from scene object; gets the scene sort id
	/// \return scene sort id
	virtual OBJECTSORTTYPES GetSceneSortID() const { return SOUNDOBJECT2D; }

private:

	/// Calls loadfilebyext on the playlist track files
	void CacheFiles();

	/// Preloads the first two buffers for double buffering
	void PreloadBuffers();

	/// Updates the queue for streaming
	void UpdateBufferQueue();
	
	/// Resets the buffers and tracks
	void ResetBuffers();

	/// Sets up the al information, like the buffer queue, volume, etc
	void SetUpTracks();

	/// Sets up the pertinent source properties
	void SetUpALSourceData();

	/// Resets all the track streams
	void ResetPlaylistStreams();

	/// Add to hierarchy?
	bool m_bAddToHierarchy;

	/// Check to see if it is currently paused
	bool IsPaused();

	/// Serialized entries
	ENTRYLIST m_Entries;

	/// The tracks and their filenames and loop counts
	TRACKLIST m_Tracks;

	/// The current playing track
	TRACKLIST::iterator m_CurrentTrack;

	/// Volume this playlist will play at
	float m_fMasterVolume;

	/// True if the source has been successfully loaded in openAL
	bool m_bValidSource;

	/// The sound source referenced by AL
	ALuint m_uiSoundSource;

	/// Was it playing before we paused?
	bool m_bWasPlaying;

	/// Manager
	COpenALSoundUtility *m_SoundUtility;

	/// Time keeping floats
	float m_fCurrentTimePosition;
	float m_fTotalTime;
	float m_fLastTime;
	float m_fGlobalVolume;
    
	CHashString m_hsPriority;

	bool m_bLoopWhole;

	bool m_bWaitingForBuffer;
};

#endif //_C_MUSICPLAYLIST_H_