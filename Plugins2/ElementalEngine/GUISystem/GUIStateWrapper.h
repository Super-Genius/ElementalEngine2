///============================================================================
/// \file		GUIStateWrapper.h
/// \brief		Because the state machine implementation is not uber generic (as in 
///				I have to specify state objects when triggering events), a wrapper
///				class is necessary for minimal contact with the machine
/// \date		10/30/06
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

#ifndef _C_GUISTATEWRAPPER_H_
#define _C_GUISTATEWRAPPER_H_

class CGUIStateWrapper
{
public:
	/// ctor, dtor
	CGUIStateWrapper();
	~CGUIStateWrapper();

	/// Sets the state object name of the gui state machine
	/// \param name = name of state machine
	void SetStateObjectName(IHashString *name)
	{
		m_szStateObjName = name;
	}

	/// Triggers event from event queue, if any
	void Update();

	/// Adds an event to the queue
	/// \param event - event to add to queue
	void AddToQueue(GUIEVENT event);	

	/// Clears the queue
	void ClearQueue()
	{
		m_vEvents.clear();
	}

private:

	/// vector of events to trigger
	vector<GUIEVENT> m_vEvents;

	/// name of state object
	CHashString m_szStateObjName;
};

#endif