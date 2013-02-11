///============================================================================
/// \file	CContactReport.h
/// \brief	Callback report for collisions
/// \date	10-30-2006
/// \author	Kyle Swaim
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

#ifndef CCONTACTREPORT_H
#define CCONTACTREPORT_H

#include <queue>

///============================================================================
/// \class	CContactReport
/// \brief	Callback report for collisions
///============================================================================
class CContactReport : public NxUserContactReport
{
public:
	/// Default Constructor
	CContactReport();

	/// Destructor
	virtual ~CContactReport();

	/// Execute all cached contact events.
	void ExecuteContactEventQueue();

public:
	/// Callback for object contacts.
	virtual void onContactNotify( NxContactPair& pair, NxU32 events );

private:
	IToolBox* m_ToolBox;

	struct CONTACTEVENT
	{
		IArchive *pArchive;
		CHashString szTriggerObject;
		CHashString szTriggerEventName;
	};
	typedef std::queue<CONTACTEVENT> CONTACTEVENTQUEUE;
	CONTACTEVENTQUEUE m_ContactEventQueue;

	/// Builds a contact event and queues it up.
	/// \param receivingObject = Name of the object receiving the contact.
	/// \param firstObject = Name of the first object in the collision.
	/// \param secondObject = Name of the second object in the collision.
	/// \param contactPoint = The point of contact.
	void BuildContactEvent( IHashString* receivingObject, IHashString* firstObject, IHashString* secondObject, Vec3& contactPoint );
};

#endif	// CCONTACTREPORT_H