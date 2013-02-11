///============================================================================
/// \file	ISoundObject.hpp
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

#ifndef _ISOUNDOBJECT_H
#define _ISOUNDOBJECT_H

namespace ElementalEngine
{

class ISoundObject : public ISceneObject
{
public:
	virtual ~ISoundObject() { };

	/// Plays the sound, after it's been loaded and initialized
	/// Will also continue sound if paused
	/// \return true if successful
	virtual bool Play() = 0;

	/// Stops the sound if it's playing
	/// \param fadeOutTime = seconds to fade the sound out, if specified
	/// \return true if successful
	virtual bool Stop(float fadeOutTime=0.0f) = 0; 

	/// Pauses the sound where it is
	/// \return true if successful
	virtual bool Pause() = 0;

	/// Checks to see if the sound is currently playing
	/// \return true if playing
	virtual bool IsPlaying() = 0;

	/// If the sound was playing before being paused, returns true
	/// \return true if last state = playing
	virtual bool WasPlaying() = 0;

	/// Allows the sound object to clear the buffer queue off it's source
	virtual void FreeSource() = 0;

	/// Gets the unique id of the hashstring for priority
	/// \return unique id of MUSIC_PRIORITY or SOUND_PRIORITY
	virtual DWORD GetAudioPriority() = 0;

	/// Checks to see if this sound is 2d or 3d
	/// \return true if 3d sound
	virtual bool Is3DSound() = 0;

	/// Gets the position of a sound (if 3d)
	/// \param pos = position to set
	virtual void SetSoundPosition(Vec3 &pos) = 0;

	/// Gets the position of a sound (if 3d)
	/// \return position
	virtual Vec3 GetSoundPosition() = 0;

	/// Gets the volume (gain) of a sound
	/// \return volume of sound
	virtual float GetSoundVolume() = 0; 

	/// Tells the sound object to reset it's volume based on the new global volume
	/// \param fGlobalVolume = new global volume
	virtual void UpdateLocalVolume(float fGlobalVolume) = 0;

};

}	//namespace ElementalEngine

#endif