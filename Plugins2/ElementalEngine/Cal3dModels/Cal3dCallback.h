///============================================================================
/// \file	Cal3dCallback.h
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

#ifndef CAL3DCALLBACK_H
#define CAL3DCALLBACK_H

class Cal3dCallback;
class Cal3DRenderObject;

struct GETCAL3DCALLBACK
{
	CalCoreAnimation * anim;
	Cal3dCallback * instance;
	GETCAL3DCALLBACK()
	{
		instance = NULL;
	}
};

struct CallbackInfo
{
	int animid;
	float timetocall;
	bool triggeroncomplete;
	bool triggeronstop;
	CHashString	eventname;
	CHashString statename;
	IArchive * archive;
	IHashString * m_CustomEvent;
	Cal3DRenderObject * renderobject;

	CallbackInfo()
	{
		archive = NULL;
		m_CustomEvent = NULL;
		renderobject = NULL;
	}

	bool operator==(const CallbackInfo& other) const;
};

class Cal3dCallback : public CalAnimationCallback
{
private:
	static void TriggerEvent( CallbackInfo & cinfo );
public:
	Cal3dCallback()
	{
		m_bDelete = false;
	}

	bool m_bDelete;
	typedef multimap< CalModel*, CallbackInfo > ModelCallbacksMap;
	ModelCallbacksMap m_ModelMap;
	
	void AddModel( CallbackInfo &cinfo, CalModel * calmodel )
	{
		if( m_bDelete )
		{
			m_bDelete = false;
		}
		multimap< CalModel *, CallbackInfo >::iterator iter = 
			m_ModelMap.find( calmodel );
		if( iter != m_ModelMap.end() )
		{
			//we can insert since it's a multimap, just make sure the cinfo isn't the same!
			//grab range and check
			bool match = false;
			for( ; iter != m_ModelMap.end() &&
				   iter->first == calmodel; ++iter )
			{
				if( cinfo == iter->second )
				{
					match = true;				
				}
			}
			if( match == false )
			{
				m_ModelMap.insert( pair< CalModel*, CallbackInfo >( calmodel, cinfo ) );
			}
		}
		else
		{
			m_ModelMap.insert( pair< CalModel*, CallbackInfo >( calmodel, cinfo ) );
		}
	}
	void RemoveModel( CallbackInfo &cinfo, CalModel * calmodel );
	//calender objects responsible for removing themselves
	void RemoveModel( CalModel * calmodel )
	{
		if( m_bDelete )return;
		m_ModelMap.erase( calmodel );
		if( m_ModelMap.empty() )
		{
			//gotta delete this in the manager
			m_bDelete = true;
		}
	}

	virtual void AnimationUpdate( float last_time, float anim_time, CalModel *model );
    virtual void AnimationComplete( CalModel *model );
	virtual void AnimationStop( CalModel *model );
};

#endif