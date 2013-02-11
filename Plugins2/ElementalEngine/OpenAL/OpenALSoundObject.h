///============================================================================
/// \file		OpenALSoundObject.h
/// \brief		Declaration of OpenAL sound
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

#ifndef _OPENALSOUNDOBJECT_H_
#define _OPENALSOUNDOBJECT_H_

using namespace std;

// higher this number, more obvious a doppler effect
#define DOPPLER_FACTOR 0.2f

class COpenALSoundObject : public CSoundObject
{
protected:
	/// Default Constructor
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	COpenALSoundObject(IHashString *parentName, IHashString *name, bool bAddToHier);
public:

	/// Default Destructor
	virtual ~COpenALSoundObject();

	/// \brief Create an instance
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief Init Function
	virtual void Init();
	
	/// \brief DeInit Function
	virtual void DeInit();	

	/// \brief serialize
	/// \param ar - The Archive
	virtual void Serialize(IArchive &ar);

	/// \brief Update
	/// \return true if successful
	virtual bool Update();

	/// Gets the sort class for this object
	/// \return component type
	virtual IHashString * GetBaseSortClass() { return GetComponentType(); }

	/// Checks to see if this is a specified type
	/// \param compType = type to check against
	/// \return t/f is a 
	virtual bool IsKindOf( IHashString * compType )
	{
		if (compType->GetUniqueID() == m_hsBaseType.GetUniqueID())
		{
			return true;
		}

		return CSoundObject::IsKindOf(compType);
	}

	/// Plays the sound object
	/// \return true if successful
	virtual bool Play();
	
	/// Stops the sound object
	/// \param fadeOutVal = amount of time to fade sound out
	/// \return true if successful
	virtual bool Stop(float fadeOutVal=0.0f);

	/// Pauses the sound object
	/// \return true if successful
	virtual bool Pause();

	/// Checks to see if the sound is currently playing
	/// \return true if playing
	virtual bool IsPlaying() { return m_bIsPlaying; }

	/// Checks to see if the sound was supposed to be playing
	/// \return true if it was
	virtual bool WasPlaying() { return m_bWasPlaying; }

	/// Sets the GLOBAL position of the sound (sound recalcs local pos relative to parent pos)
	void SetSoundPosition(const Vec3 &pos);
	
	// ** MESSAGES ** //

	/// Plays the sound object (MSG)
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_PROCEED
	virtual DWORD OnPlaySoundObject(DWORD size, void *params);

	/// Stops the sound object (MSG)
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_PROCEED
	virtual DWORD OnStopSoundObject(DWORD size, void *params);

	/// Allows the sound object to clear the buffer queue off it's source
	virtual void FreeSource();

	/// Gets the unique id of the hashstring for priority
	/// \return unique id of MUSIC_PRIORITY or SOUND_PRIORITY
	virtual DWORD GetAudioPriority() { return m_hsPriority.GetUniqueID(); }

	/// Checks to see if this sound is 2d or 3d
	/// \return true if 3d sound
	virtual bool Is3DSound() { return m_b3DSound; }

	/// Gets the position of a sound (if 3d)
	/// \return position
	virtual Vec3 GetSoundPosition() { return m_GlobalPosition; }

	/// Gets the volume (gain) of a sound
	/// \return volume of sound
	virtual float GetSoundVolume() { return m_fVolume; }

	/// Tells the sound object to reset it's volume based on the new global volume
	/// \param fGlobalVolume = new global volume
	virtual void UpdateLocalVolume(float fGlobalVolume);

private:

	COpenALSoundUtility *m_SoundUtility;

	/// Updates position and velocity against moving entity parent
	void ResetPosition();

	/// Preloads buffers into the queue
	void PreloadBufferQueue();

	/// Updates the buffer queue to make sure we don't starve the queue
	void UpdateBufferQueue();

	/// Sets up the pertinent source properties
	void SetUpSourceData();

	/// Class Type (COpenALSoundObject)
	CHashString m_hszNameType;
	/// Alternate for kind of (CSoundObject)
	CHashString m_hsBaseType;

	CHashString m_hsPriority;

	// Key for sound
	DWORD m_dwSoundKey;

	/// Looping
	bool m_bLooping;

	/// 3D or 2D Sound
	/// true = 3D, false 2D
	bool m_b3DSound;

	/// Position
	Vec3 m_GlobalPosition;
	Vec3 m_LocalPosition;

	float m_fAttenuationScale;

	/// Velocity
	Vec3 m_Velocity;

	/// Self Removal
	///	If true the object will remove itself once it is done playing
	bool m_bAutoRemove;

	/// Add this to the hierarchy
	/// If true the object will be added to the hierarchy
	bool m_bAddToHierarchy;

	/// Streaming audio file
	/// if this is true the file is streamed from disk
	bool m_bStream;

	/// Volume
	float m_fVolume;

	// whether openAL SoundSource has been initialized correctly or not
	bool m_bValidSource;
	// OpenAL SPECIFIC DATA
	ALuint m_uiSoundSource;

	float m_fTimeOfPositionUpdate;
	bool m_bAutoStart;
	bool m_bIsPlaying;
	bool m_bWasPlaying;

	bool m_bWasPreempted;

	ISoundBufferStream *m_BufferStream;

	float m_fGlobalVolume;

	bool m_bJustInitialized;

	float m_fFadeOutTime;
	float m_fFadeOutStartTime;	
	float m_fFadeStartVolume;
	bool m_bFadeOutSound;
	float m_fPitch;

	bool m_bWaitingForBuffer;
	bool m_bUsingSingleBuffer;
};

#endif //_OPENALSOUNDOBJECT_H_
 
