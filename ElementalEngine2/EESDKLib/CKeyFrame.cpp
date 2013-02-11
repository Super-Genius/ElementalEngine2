///============================================================================
/// \file	CKeyFrame.cpp
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

#include "stdafx.h"
#include "CKeyFrame.h"

/// Ctor
CKeyFrameSet::CKeyFrameSet()
{
	m_Keys.clear();
	m_Events.clear();
}

/// Dtor
CKeyFrameSet::~CKeyFrameSet()
{
	m_Keys.clear();
	m_Events.clear();
}

/// Adds a keyframe to the set
/// \param time - keyframe in position in time, where 0 is start of anim
/// \param value - value displacement over time
void CKeyFrameSet::AddKeyframe( float time, float value )
{
	KEYFRAME_DATA temp;
	temp.time = time;
	temp.value = value;
	for (UINT i=0; i<m_Keys.size(); i++)
	{
		if (m_Keys[i].time > time)
		{
			m_Keys.insert(m_Keys.begin() + i, temp); 
			return;
		}
	}
	// if key should be last
	m_Keys.push_back(temp);
}

/// Gets the current keyframe index without knowing last frame
/// \param time - total time elapsed from start of animation
/// \return index of current key, or -1 if no keys or end of animation
int CKeyFrameSet::GetKeyframeIndex( float time )
{
	if (m_Keys.size() > 0)
	{
		if (m_Keys.size() == 1)
		{
			return 0;
		}
		else // psuedo binary search
		{
			UINT size, currIndex = 0, startIndex = 0, endIndex = (UINT)m_Keys.size()-1;
			size = endIndex - startIndex;
			currIndex = size / 2;
			while (size > 1)
			{
				if (time > m_Keys[currIndex].time) // go to greater half
				{
					startIndex = currIndex;
					size = endIndex - startIndex;
					currIndex += size / 2;
				}
				else // go to lesser half, even if it's equal
				{
					endIndex = currIndex;
					size = endIndex - startIndex;
					currIndex -= size / 2;
				}
			}
			if (time >= m_Keys[startIndex].time) // it's this one!
			{
				return startIndex;
			}
			else if (startIndex == m_Keys.size() -1) // currIndex is last frame in anim
			{
				return -1; // maybe should add a boolean for looping, and return 0 (first key)
			}
			else // greater than currIndex, but not the last frame
			{
				return startIndex +1;
			}
		}
	}
	return -1; // no keys
}

/// Gets the current keyframe index, given the last frame
/// \param lastKey - index of last key frame
/// \param time - total time elapsed from start of animation
/// \return index of current key, or -1 if no keys or end of animation
int CKeyFrameSet::GetKeyframeIndex( int lastKey, float time )
{
	if (lastKey >= (int)m_Keys.size()) // key too large
	{
		return -1;
	}
	if (lastKey < 0) // negative index
	{
		return -1;
	}

	if (m_Keys[lastKey].time > time)
	{
		return lastKey;
	}
	else 
	{
		if (lastKey == m_Keys.size() -1) // end of animation
		{
			return -1;
		}
		else 
		{
			unsigned int i;
			for (i=(unsigned int)lastKey; i<m_Keys.size(); i++)
			{
				if (m_Keys[i].time > time)
				{
					return i;
				}
			}
			return -1; // beyond the scope of the animation
		}
	}
}

/// Interpolates between two keys given absolute time
/// \param time - total time elapsed from start of animation
/// \param key1 - index of first key to interpolate between
/// \param key2 - index of second key to interpolate between
/// \return exact value at time - not displacement, not parameterized (0-1)
float CKeyFrameSet::Interpolate( float time, int key1, int key2 )
{
	// only linear interp for now...
	if ((key1 >= (int)m_Keys.size()) || (key2 >= (int)m_Keys.size()))
	{
		// just return last key data
		return KEYFRAME_DATA::Interpolate(time, m_Keys[m_Keys.size()-1], m_Keys[m_Keys.size()-1]);
	}
	return KEYFRAME_DATA::Interpolate(time, m_Keys[key1], m_Keys[key2]);
}

/// Adds an event to a specific position to be triggered in time
/// \param time - temporal position for the event to trigger
/// \param event - any type of data to be returned at a later check
void CKeyFrameSet::AddEvent( float time, void* eventData )
{
	KEYFRAME_EVENT temp;
	temp.event = eventData;
	temp.time = time;
	m_Events.push_back(temp);
}

/// Checks to see if we have crossed an event in time
/// \param prevTime - the last absolute time from the start of anim
/// \param currTime - the current absolute time from the start of anim
/// \return previously stored data
void* CKeyFrameSet::CheckEvent( float prevTime, float currTime )
{
	if (m_Events.size() == 0)
	{
		return NULL;
	}
	if (m_Events.size() == 1)
	{
		if ((prevTime < m_Events[0].time) && (m_Events[0].time < currTime))
		{
			return m_Events[0].event;
		}
		else 
		{
			return NULL;
		}
	}
	else
	{
		UINT size, currIndex = 0, startIndex = 0, endIndex = (UINT)m_Events.size()-1;
		size = endIndex - startIndex;
		currIndex = size / 2;
		while (size > 1)
		{
			if (prevTime > m_Events[currIndex].time) // go to greater half
			{
				startIndex = currIndex;
				size = endIndex - startIndex;
				currIndex += size / 2;
			}
			else if (currTime < m_Events[currIndex].time)// go to lesser half, even if it's equal
			{
				endIndex = currIndex;
				size = endIndex - startIndex;
				currIndex -= size / 2;
			}
			else // should be this one
			{
				return m_Events[currIndex].event;
			}
		}
		if ((prevTime < m_Events[startIndex].time) && (m_Events[startIndex].time < currTime))
		{
			return m_Events[startIndex].event;
		}
		else if ((prevTime < m_Events[endIndex].time) && (m_Events[endIndex].time < currTime))
		{
			return m_Events[endIndex].event;
		}
		else 
		{
			return NULL;
		}
	}
}

/// Checks to see if we have crossed an event in time, given last keyframe
/// \param lastKey - index of last key frame
/// \param prevTime - the last absolute time from the start of anim
/// \param currTime - the current absolute time from the start of anim
/// \return previously stored data
void* CKeyFrameSet::CheckEvent( int lastKey, float prevTime, float currTime )
{
	if (lastKey >= (int)m_Events.size()) // index out of range
	{
		return NULL;
	}
	if (lastKey < 0) // index is negative
	{
		return NULL;
	}
	unsigned int i;
	for (i=(unsigned int)lastKey; i<m_Events.size(); i++)
	{
		if (currTime < m_Events[i].time) // already passed the time
		{
			return NULL;
		}
		else if((prevTime < m_Events[i].time) && (m_Events[i].time < currTime))
		{
			return m_Events[i].event;
		}
	}
	return NULL;
}

void CKeyFrameSet::RemoveKeyframe( UINT keyInd )
{
	if (keyInd < m_Keys.size())
	{
		m_Keys.erase(m_Keys.begin() + keyInd);
	}
}

bool CKeyFrameSet::GetKeyframeValues( UINT keyInd, float *time, float *value )
{
	if (keyInd < m_Keys.size())
	{
		*time = m_Keys[keyInd].time;
		*value = m_Keys[keyInd].value;
		return true;
	}
	return false;
}
	
bool CKeyFrameSet::ChangeKeyframeValues( UINT keyInd, float time, float value )
{
	if (keyInd < m_Keys.size())
	{
		m_Keys[keyInd].time = time;
		m_Keys[keyInd].value = value;
		return true;
	}
	return false;
}