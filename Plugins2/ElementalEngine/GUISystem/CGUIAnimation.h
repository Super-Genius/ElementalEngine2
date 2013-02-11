///==========================================================================
/// \file	CGUIAnimation.h
/// \brief	Animation for gui elements
///			One animation is a collection of 'channels', which is the set of
///			all keys for one variable for one object
/// \date	11/02/2006
/// \author Nick Tourte
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

#ifndef _C_GUIANIMATION_H_
#define _C_GUIANIMATION_H_

#include "CKeyFrame.h"

typedef struct GUIKEYDATA
{
	float time;
	float value;
}GUIKEYDATA;

typedef vector<GUIKEYDATA> GUIKEYLIST;

///==========================================================================
/// \class	CGUIAnimChannel
/// \brief	Class derives CKeyFrameSet to use in gui animation
///==========================================================================
class CGUIAnimChannel : public CKeyFrameSet
{
public:

	/// Overridden add key frame method
	/// \param time = absolute time to add (from 0 point)
	/// \param value = value at time
	virtual void AddKeyframe( float time, float value )
	{
		CKeyFrameSet::AddKeyframe(time, value);

		// add to my personal list, too, so I can retrieve the data later
		GUIKEYDATA gkd;
		gkd.time = time;
		gkd.value = value;
		for (UINT i=0; i<m_Keys.size(); i++)
		{
			if (m_Keys[i].time > time)
			{
				m_Keys.insert(m_Keys.begin() + i, gkd);
				return;
			}
		}
		
		m_Keys.push_back(gkd);		
	}

	virtual bool ChangeKeyframeValues(UINT keyInd, float time, float value)
	{
		bool ret = CKeyFrameSet::ChangeKeyframeValues(keyInd, time, value);

		// mapping here should be 1-1...
		m_Keys[keyInd].time = time;
		m_Keys[keyInd].value = value;
		return ret;
	}

	virtual void RemoveKeyframe(UINT keyInd)
	{
		CKeyFrameSet::RemoveKeyframe(keyInd);
		
		m_Keys.erase(m_Keys.begin() + keyInd);
	}

	/// Gets the keys that have been added
	/// \return the key list
	GUIKEYLIST GetKeys() { return m_Keys; }
	
private:
    GUIKEYLIST m_Keys;
};

typedef struct GUIANIMCHANNEL
{
	CGUIAnimChannel m_Channel;
	int m_iLastKey;
	UINT m_iNumKeys;
	GUIANIMCHANNEL()
	{
		m_iLastKey = 0;
		m_iNumKeys = 0;
	}
}GUIANIMCHANNEL;

typedef struct GUIANIMOBJ
{
	CHashString m_szObjName;
	CHashString m_szObjType;
	GUIANIMCHANNEL m_chPosX;
	GUIANIMCHANNEL m_chPosY;
	GUIANIMCHANNEL m_chSizeX;
	GUIANIMCHANNEL m_chSizeY;
	GUIANIMCHANNEL m_chAlphaCol;
	GUIANIMCHANNEL m_chRCol;
	GUIANIMCHANNEL m_chGCol;
	GUIANIMCHANNEL m_chBCol;
	GUIANIMCHANNEL m_chRot;
}GUIANIMOBJ;

///==========================================================================
/// \class	CGUIAnimation
/// \brief	Animation class for gui animations
///==========================================================================
class CGUIAnimation : public CObjectTemplate<IObject>
{
protected:
	/// ctor	
	CGUIAnimation(IHashString *parentName, IHashString *name);

public:
	/// dtor
	~CGUIAnimation();

	/// Create a new animation object
	/// \param argptr = name of this component, name of parent
	/// \return this object
	static IComponent *Create(int nArgs, va_list argptr);	

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	/// \fn		virtual void Init()
	/// \brief	Initialize the object
	virtual void Init();

	/// \fn		virtual void Update()
	/// \brief	Update the object
	virtual bool Update();

	/// \fn		virtual void DeInit()
	/// \brief	Deinitialize the object
	virtual void DeInit();

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType()
	{
		static CHashString CGUIAnimationCT(_T("CGUIAnimation"));
		return &CGUIAnimationCT;
	}

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType)
	{
		return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
	}

	/// Sets the position of the object(stub)
	/// \param position IVec3 class reference to fill in
	virtual void SetPosition( const Vec3 &position ){}

	/// Gets the position of the object(stub)
	/// \return position IVec3 class reference
	virtual Vec3 &GetPosition()
	{
		static Vec3 emptyVec(0.f, 0.f, 0.f);
		return emptyVec;
	}
	
	/// Gets the bounding sphere of the object (stub)
	virtual void GetBoundingSphere( Vec3 &position, float &radius ){}

	//// Messages ////

	/// Starts the animation
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_STOP
	DWORD OnStartGUIAnimation(DWORD size, void *param);

	/// Stops the animation
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_STOP
	DWORD OnStopGUIAnimation(DWORD size, void *param);

	/// Checks to see if the animation is still running
	/// \param size = sizeof(bool)
	/// \param param = bool return val
	/// \return MSG_HANDLED_STOP
	DWORD OnGetGUIAnimationIsRunning(DWORD size, void *param);

	/// Gets the data loaded from a file
	/// \param size = sizeof(GUIANIMDATA)
	/// \param param = GUIANIMDATA filled with data
	/// \return MSG_HANDLED_STOP
	DWORD OnGetAnimationData(DWORD size, void *param);

	/// Scrubs the animation (sets it to freeze at a specific time)
	/// \param size = sizeof(float)
	/// \param param = float of time (in sec) to show anim
	/// \return MSG_HANDLED_STOP
	DWORD OnScrubGUIAnimation(DWORD size, void *param);

	/// Adds keys to the animation to the appropriate object
	/// \param size = sizeof(ADDMULTICHANNELKEYTOGUIANIM)
	/// \param param = ADDMULTICHANNELKEYTOGUIANIM with object data and the various keys set
	/// \return MSG_HANDLED_STOP
	DWORD OnAddKeysToAnimation(DWORD size, void *param);

	/// Adds a single key to one channel of a gui object
	/// \param size = sizeof(ADDSINGLECHANNELKEYTOGUIANIM)
	/// \param param = ADDSINGLECHANNELKEYTOGUIANIM with key data
	/// \return MSG_HANDLED_STOP
	DWORD OnAddOneKey(DWORD size, void *param);

	/// Deletes one key from a channel set from an object
	/// \param size = sizeof(GUIDELETEONEKEY)
	/// \param param = GUIDELETEONEKEY with object name, key index, and channel
	/// \return MSG_HANDLED_STOP
	DWORD OnDeleteOneKey(DWORD size, void *param);

	/// Adds a gui anim callback to be fired when the animation completes
	/// NOTE: event gets queued onto internal gui stack, so it goes directly to
	///		the specified gui state object
	/// \param size = sizeof(GUIEVENT)
	/// \param param = GUIEVENT struct with event name and archive
	/// \return MSG_HANDLED_STOP
	DWORD OnAddGUIAnimationCallback(DWORD size, void *param);

	/// Changes an anim key
	/// \param size = sizeof(ADDSINGLECHANNELKEYTOGUIANIM) 
	/// \param param = ADDSINGLECHANNELKEYTOGUIANIM with key data
	/// \return MSG_HANDLED_STOP
	DWORD OnChangeGUIAnimKey(DWORD size, void *param);
	
	/// Checks to see if the anim is set to use relative positions
	/// \param size = sizeof(bool)
	/// \param param = bool retval for using relative pos or not
	/// \return MSG_HANDLED_STOP
	DWORD OnIsGUIAnimationRelativePositions(DWORD size, void *param);

private:
	/// Recalculates the length of the anim
	void RecalcTime();

	/// Converts the position keys between relative and absolute position
	void ConvertPositions();

	/// list of objects
	vector <GUIANIMOBJ> m_AnimObjects;

	/// number of objects
	UINT m_iObjectNum;

	/// total time of anim
	float m_fTotalTime;

	/// current time of anim
	float m_fCurrentTime;

	/// starting time of anim
	float m_fStartTime;

	/// anim is running
	bool m_bIsRunning;

	/// toolbox
	IToolBox *m_ToolBox;

	/// frames per second for the anim
	UINT m_iFramesPerSec;

	/// looping?
	bool m_bIsLooping;

	/// are we scrubbing, or running?
	bool m_bScrub;
	
	/// vector of anim callbacks
	vector<GUIEVENT> m_AnimCallbacks;

	float m_fSaveVersion;
	bool m_bRelativePositions;
	bool m_bInitialSerialize;
};


#endif