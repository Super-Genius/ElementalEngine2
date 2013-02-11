///============================================================================
/// \file		CTimer.h
/// \brief		Default timer implementation
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

#ifndef _CTIMER_H
#define _CTIMER_H

#include "ITimer.h"

struct DELTATIMERSCALE
{
	float m_fTimeScale;
	bool m_bPaused;
	DELTATIMERSCALE()
	{
		m_fTimeScale = 1.0f;
		m_bPaused = false;
	}
};

typedef map<DWORD, DELTATIMERSCALE> DELTATIMEMAP;

///============================================================================
/// \class	CTimer
/// \brief	Default timer implementation
///============================================================================
class CTimer : public ITimer
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CTimer);

	/// Default Constructor
	///	Since CTimer is implemented as a singleton the
	///	constructor is private.
	CTimer();

public:
	/// Destructor
	virtual ~CTimer();

	/// Function to get this component.
	/// \return	The singleton instance in this case
	static IComponent* Create(int nArgs, va_list argptr);

	virtual void Serialize(IArchive &ar);

	/// return a string of the component type
	/// \return IHashString * pointer to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	/// Gets the current game time.
	/// This is the time in game and can be
	/// paused, unpaused and scaled.
	/// \return	The current game time
	virtual float GetTime();
	/// Gets the current real time.
	/// This time is the actual system time
	/// and cannot be modified.
	/// \return	The current game time
	virtual float GetRealTime();
	/// Gets the Frames Per Second.
	/// \return	The Frames Per Second
	virtual float GetFPS();
	/// Gets the amount of time passed since the.
	/// last frame.
	/// \return The amount of time since the last frame
	virtual float GetTimeDelta(IHashString* hsTimerName = NULL);

	virtual int GetCurrentFrame();

	virtual void ResetTime();
	virtual void PauseTime(IHashString* hsTimerName = NULL);
	virtual void UnpauseTime(IHashString* hsTimerName = NULL);

	virtual void SetTimeScale(float scale, IHashString* hsTimerName = NULL);
	virtual void SetMaxTick(float maxTick);

	/// Message for Global Ticks (Updates)
	DWORD GlobalTickMessage(DWORD size, void *param);

protected:
	virtual void Update();

private:
	// Called once at the start of the game
	__forceinline void InitGameTime()
	{
		// We need to know how often the clock is updated
		if( !QueryPerformanceFrequency((LARGE_INTEGER *)&_ticksPerSecond) )
			_ticksPerSecond = 1000000;

		// If timeAtGameStart is 0 then we get the time since
		// the start of the computer when we call GetGameTime()
		_timeAtGameStart = 0;
		_timeAtGameStart = GetGameTime();
	}

	// Called every time you need the current game time
	__forceinline float GetGameTime()
	{
		float time = GetRealTime();
		// Subtract the time at game start to get
		// the time since the game started
		time -= _timeAtGameStart;
		_curGameTime = time;
		_curTimeDelta = _curGameTime - _lastTime;
        _lastTime = _curGameTime;
		return time;
	}

	// Called once for every frame
	__forceinline void UpdateFPS()
	{
		_numFrames++;
		float currentUpdate = GetTime();
		if( currentUpdate - _lastUpdate > _fpsUpdateInterval )
		{
			_fps = _numFrames / (currentUpdate - _lastUpdate);
			_lastUpdate = currentUpdate;
			_numFrames = 0;
		}
	}

	__forceinline float CalcTimeDelta()
	{
		if (m_fMaxTick < _curTimeDelta)
		{
			return m_fMaxTick;
		}

		return _curTimeDelta;
	}

	float  _timeAtGameStart;
	UINT64 _ticksPerSecond;

	// Global variables for measuring fps
	float _lastUpdate;
	float _fpsUpdateInterval;
	unsigned int  _numFrames;
	float _fps;
	float _curGameTime;
	float _curTimeDelta;
	float _lastTime;
    
	float m_fMaxTick;

	DELTATIMEMAP m_mDeltaTimers;
};

#endif // _CTIMER_H