///==========================================================================
/// \file	CKeyFrame.h
/// \brief	Simple keyframe implementation
/// \date	08/31/06
/// \author	Nick Tourte
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
///==========================================================================

#ifndef _C_KEYFRAME_H_
#define _C_KEYFRAME_H_

namespace ElementalEngine
{

struct KEYFRAME_DATA
{
	// total amount of elapsed time from start of animation
	float time;
	// total displacement of any value
	float value;
	//for speedy interpolation
	static __forceinline float Interpolate( float &curtime, KEYFRAME_DATA &a, KEYFRAME_DATA &b )
	{
		float v;
		float distance = b.time - a.time;
		if( distance > 0 )
		{
			v = (curtime - a.time )*b.value + (b.time - curtime)*a.value;
			v /= distance;
			return v;
		}
		else
		{
			v = b.value;
			return v;
		}
	}
};

/// KEYFRAME_DATA_SET
struct KEYFRAME_DATA_SET
{
	KEYFRAME_DATA *			m_Keys;
	int						m_Size;
	float					m_ValueMin;
	float					m_ValueMax;
	float					m_TimeMin;
	float					m_TimeMax;
	KEYFRAME_DATA_SET(){ m_Keys = NULL; m_Size = 0; }
	~KEYFRAME_DATA_SET()
	{ 
		if( m_Keys )
		{ 
			delete[] m_Keys;
			m_Keys = NULL;
		}
	}

};

struct KEYFRAME_EVENT
{
	// elapsed time from start of anim
	float time;
	// event data
	void *event;
};


///==========================================================================
/// \class	IKeyFrameSet
/// \brief	KeyFrame interface
/// A keyframe set is a list of keys for one variable over time
/// It should be used as a reference by specific objects, I.E. particles
/// should reference to this as a pattern, so not every particle has
/// a keyframe set for every var
///==========================================================================
class CKeyFrameSet
{
public:
	/// Ctor
	CKeyFrameSet();
	/// Dtor
	~CKeyFrameSet();
	/// Adds a keyframe to the set
	/// \param time - keyframe in position in time, where 0 is start of anim
	/// \param value - value displacement over time
	virtual void AddKeyframe( float time, float value );
	/// Removes a keyframe from the set
	/// \param keyInd - index of key to remove
	virtual void RemoveKeyframe( UINT keyInd );
	/// Returns values set for key at given index, if the key exists
	/// \param keyInd - index of key to get data of
	/// \param time - time value returned
	/// \param value - value of key returned
	/// \return true if successful
	virtual bool GetKeyframeValues( UINT keyInd, float *time, float *value );
	/// Changes key data at given key index, if exists
	/// \param keyInd - index of key to change values of
	/// \param time - time to change to
	/// \param value - value to change to
	/// \return true if successful
	virtual bool ChangeKeyframeValues( UINT keyInd, float time, float value );
	/// Gets the current keyframe index without knowing last frame
	/// \param time - total time elapsed from start of animation
	/// \return index of current key, or -1 if no keys or end of animation
	virtual int GetKeyframeIndex( float time );
	/// Gets the current keyframe index, given the last frame
	/// \param lastKey - index of last key frame
	/// \param time - total time elapsed from start of animation
	/// \return index of current key, or -1 if no keys or end of animation
	virtual int GetKeyframeIndex( int lastKey, float time );
	/// Interpolates between two keys given absolute time
	/// \param time - total time elapsed from start of animation
	/// \param key1 - index of first key to interpolate between
	/// \param key2 - index of second key to interpolate between
	/// \return exact value at time - not displacement, not parameterized (0-1)
	virtual float Interpolate( float time, int key1, int key2 );
	/// Adds an event to a specific position to be triggered in time
	/// \param time - temporal position for the event to trigger
	/// \param event - any type of data to be returned at a later check
	virtual void AddEvent( float time, void* eventData );
	/// Checks to see if we have crossed an event in time
	/// \param prevTime - the last absolute time from the start of anim
	/// \param currTime - the current absolute time from the start of anim
	/// \return previously stored data
	virtual void* CheckEvent( float prevTime, float currTime );
	/// Checks to see if we have crossed an event in time, given last keyframe
	/// \param lastKey - index of last key frame
	/// \param prevTime - the last absolute time from the start of anim
	/// \param currTime - the current absolute time from the start of anim
	/// \return previously stored data
	virtual void* CheckEvent( int lastKey, float prevTime, float currTime );
	/// Gets the last key index of the set
	/// \return last key index
	virtual int GetLastKeyIndex()
	{
		return (int)m_Keys.size() -1; 
	}

private:

	/// array of keyframes
	vector<KEYFRAME_DATA> m_Keys;
	/// array of events
	vector<KEYFRAME_EVENT> m_Events;
};
} // namespace ElementalEngine

#endif