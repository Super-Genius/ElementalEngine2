///============================================================================
/// \file		CSoundObject.h
/// \brief		Declaration of generic sound object
/// \date		03/20/2007
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

#ifndef _C_SOUNDOBJECT_H_
#define _C_SOUNDOBJECT_H_

class CSoundObject: public CSceneObject<ISoundObject>
{
protected:
	/// Default Constructor
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CSoundObject(IHashString *parentName, IHashString *name, bool bAddToHier);
public:

	/// dtor
	virtual ~CSoundObject();

	/// \brief Create an instance
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// Inherited fn, gets the scene sort id
	/// \return scene sort id
	virtual OBJECTSORTTYPES GetSceneSortID() const { return SOUNDOBJECT3D; }

	/// Plays the sound object (MSG)
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_PROCEED
	virtual DWORD OnPlaySoundObject(DWORD size, void *param);

	/// Stops the sound object (MSG)
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_PROCEED
	virtual DWORD OnStopSoundObject(DWORD size, void *param);

	/// Plays the sound object
	/// \return true if successful
	virtual bool Play() { return false; }

	/// Stops the sound object
	/// \param fadeOutVal = amount of time to fade sound out
	/// \return true if successful
	virtual bool Stop(float fadeOutVal=0.0f) { return false; }

	/// Pauses the sound object
	/// \return true if successful
	virtual bool Pause() { return false; }

	/// Checks to see if the sound is currently playing
	/// \return true if playing
	virtual bool IsPlaying() { return false; }

	/// Checks to see if the sound was supposed to be playing
	/// \return true if it was
	virtual bool WasPlaying() { return false; }

	/// Unqueues any queued buffers and returns the source
	virtual void FreeSource(){}
	
	/// Gets the audio priority for the source pool
	/// \return priority
	virtual DWORD GetAudioPriority() { return 0; }

	/// Checks to see if this is a 3d sound
	/// \return true if 3d
	virtual bool Is3DSound() { return false; }

	/// Sets the sound position
	/// \param pos = position to set
	virtual void SetSoundPosition(Vec3 &pos){}

	/// Gets the sound position
	/// \return current position
	virtual Vec3 GetSoundPosition()
	{
		Vec3 emptyVec;
		return emptyVec;
	}
	
	/// Gets the sound volume
	/// \return sound volume
	virtual float GetSoundVolume() { return 0.0f; }

	/// Updates the local volume given a change of global volume
	/// \param fGlobalVolume = global volume update
	virtual void UpdateLocalVolume(float fGlobalVolume){}
};

#endif //_C_SOUNDOBJECT_H_