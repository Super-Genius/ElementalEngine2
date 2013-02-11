///============================================================================
/// \file		ITimer.h
/// \brief		Timer interface
/// \date		12-19-2007
/// \author		Kyle Swaim
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

#ifndef _ITIMER_H
#define _ITIMER_H

namespace ElementalEngine
{

///============================================================================
/// \class	ITimer
/// \brief	Timer interface
///============================================================================
class ITimer : public IComponent
{
public:
	/// Destructor
	virtual ~ITimer() {};

	/// Gets the current game time.
	/// This is the time in game and can be
	/// paused, unpaused and scaled.
	/// \return	The current game time
	virtual float GetTime() = 0;
	/// Gets the current real time.
	/// This time is the actual system time
	/// and cannot be modified.
	/// \return	The current game time
	virtual float GetRealTime() = 0;
	/// Gets the Frames Per Second.
	/// \return	The Frames Per Second
	virtual float GetFPS() = 0;
	/// Gets the amount of time passed since the.
	/// last frame.
	/// \return The amount of time since the last frame
	virtual float GetTimeDelta(IHashString* hsTimerName = NULL) = 0;

	virtual int GetCurrentFrame() = 0;

	virtual void ResetTime() = 0;
	virtual void PauseTime(IHashString* hsTimerName = NULL) = 0;
	virtual void UnpauseTime(IHashString* hsTimerName = NULL) = 0;

	virtual void SetTimeScale(float scale, IHashString* hsTimerName = NULL) = 0;
	virtual void SetMaxTick(float maxTick) = 0;

protected:
	virtual void Update() = 0;
};

} // namespace ElementalEngine

#endif // _ITIMER_H