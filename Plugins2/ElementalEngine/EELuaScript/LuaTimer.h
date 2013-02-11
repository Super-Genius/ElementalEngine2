///============================================================================
/// \file		LuaTimer.h
/// \brief		Lua wrapper for CPerfTimer
/// \date		12-19-2007
/// \author		Kyle Swaim
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

#ifndef _LUATIMER_H
#define _LUATIMER_H

//class CPerfTimer;

///============================================================================
/// \class	CLuaTimer
///============================================================================
// tolua_begin
class CLuaTimer 
{
public:
	/// Default Constructor
	CLuaTimer();
	/// Destructor
	virtual ~CLuaTimer();

	/// Gets the current game time.
	/// This is the time in game and can be
	/// paused, unpaused and scaled.
	/// \return	The current game time
	float GetTime();
	/// Gets the current real time.
	/// This time is the actual system time
	/// and cannot be modified.
	/// \return	The current game time
	float GetRealTime();
	/// Gets the Frames Per Second.
	/// \return	The Frames Per Second
	float GetFPS();
	/// Gets the amount of time passed since the.
	/// last frame.
	/// \return The amount of time since the last frame
	float GetTimeDelta(IHashString* hsTimerName = NULL);

	int GetCurrentFrame();

	void ResetTime();
	void PauseTime(IHashString* hsTimerName = NULL);
	void UnpauseTime(IHashString* hsTimerName = NULL);

	void SetTimeScale(float scale, IHashString* hsTimerName = NULL);
	void SetMaxTick(float maxTick);
// tolua_end

private:
	ITimer* m_Timer;

// tolua_begin
};
// tolua_end

#endif // _LUATIMER_H