///============================================================================
/// \file	Cal3dCallback.cpp
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

bool CallbackInfo::operator==(const CallbackInfo &other) const
{
	return (this->animid == other.animid &&
			this->archive == other.archive &&
			this->eventname.GetUniqueID() == other.eventname.GetUniqueID() &&
			this->m_CustomEvent == other.m_CustomEvent &&
			this->renderobject == other.renderobject &&
			this->statename.GetUniqueID() == other.statename.GetUniqueID() &&
			this->timetocall == other.timetocall &&
			this->triggeroncomplete == other.triggeroncomplete &&
			this->triggeronstop == other.triggeronstop);
}

void Cal3dCallback::TriggerEvent( CallbackInfo & cinfo )
{
	IToolBox * toolbox = EngineGetToolBox();
	TRIGGEREVENTPARAMS tevent;
	tevent.EventName = &cinfo.eventname;
	tevent.EventParamsArchive = cinfo.archive;
	static DWORD msgHash_TriggerEvent = CHashString(_T("TriggerEvent")).GetUniqueID();
	DWORD msg = cinfo.m_CustomEvent != NULL ? cinfo.m_CustomEvent->GetUniqueID() : msgHash_TriggerEvent;
	toolbox->SendMessage( msgHash_TriggerEvent,sizeof(tevent),&tevent, &cinfo.statename );
}

//calender objects responsible for removing themselves
void Cal3dCallback::RemoveModel( CallbackInfo &cinfo, CalModel * calmodel )
{
	if( m_bDelete )
	{
		return;
	}
	pair<ModelCallbacksMap::iterator, ModelCallbacksMap::iterator> range;
	range = m_ModelMap.equal_range( calmodel );
	for ( ; range.first != range.second; ++range.first )
	{
		if( range.first->second == cinfo )
		{
			m_ModelMap.erase( range.first );
			break;
		}
	}
	if( m_ModelMap.empty() )
	{
		//gotta delete this in the manager
		m_bDelete = true;
	}
}

void Cal3dCallback::AnimationUpdate( float last_time, float anim_time, CalModel *model )
{
	if( m_bDelete )
	{
		return;
	}
	// list with callback infos for firing event
	vector<CallbackInfo> callbacks;
	pair<ModelCallbacksMap::iterator, ModelCallbacksMap::iterator> range;
	range = m_ModelMap.equal_range( model );
	for( ; range.first != range.second; ++range.first)
	{
		CallbackInfo &cinfo = range.first->second;
		//call this back if it hits time
		if( cinfo.timetocall > 0 &&
			last_time <= cinfo.timetocall &&
			cinfo.timetocall <= anim_time )
		{
			// saves to list. Trigger event may modify map
			callbacks.push_back( cinfo );
		}
	}
	// now time for firing event. model map may be modified freely now
	for_each( callbacks.begin(), callbacks.end(), Cal3dCallback::TriggerEvent);
}

void Cal3dCallback::AnimationComplete( CalModel *model )
{
	if( m_bDelete )
	{
		return;
	}
	// list with callback infos for firing event
	vector<CallbackInfo> callbacks;
	pair<ModelCallbacksMap::iterator, ModelCallbacksMap::iterator> range;
	range = m_ModelMap.equal_range( model );
	for( ; range.first != range.second; ++range.first)
	{
		CallbackInfo &cinfo = range.first->second;
		//call this back if it hits time
		if( cinfo.triggeroncomplete )
		{
			// saves to list. Trigger event may modify map
			callbacks.push_back( cinfo );
		}
	}
	// now time for firing event. model map may be modified freely now
	for_each( callbacks.begin(), callbacks.end(), Cal3dCallback::TriggerEvent);

	Cal3DRenderObject *pRenderModel = (Cal3DRenderObject*)model->getUserData();
	if (pRenderModel)
		pRenderModel->ResetRenderOffsetDelta();
}

void Cal3dCallback::AnimationStop( CalModel *model )
{
	if( m_bDelete )
	{
		return;
	}
	// list with callback infos for firing event
	vector<CallbackInfo> callbacks;
	pair<ModelCallbacksMap::iterator, ModelCallbacksMap::iterator> range;
	range = m_ModelMap.equal_range( model );
	for( ; range.first != range.second; ++range.first)
	{
		CallbackInfo &cinfo = range.first->second;
		//call this back if it hits time
		if( cinfo.triggeronstop )
		{
			// saves to list. Trigger event may modify map
			callbacks.push_back( cinfo );
		}
	}
	// now time for firing event. model map may be modified freely now
	for_each( callbacks.begin(), callbacks.end(), Cal3dCallback::TriggerEvent);

	Cal3DRenderObject *pRenderModel = (Cal3DRenderObject*)model->getUserData();
	if (pRenderModel)
		pRenderModel->ResetRenderOffsetDelta();
}
