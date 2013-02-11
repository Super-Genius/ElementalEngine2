///=====================================================================
/// \file	CManagerListener.hpp
/// \brief	defintion of AIListener concrete class
///         This class defines a AIListener which is the listener
///			registered with the system for AI
/// \date	02/26/2005
/// \author	Kenneth Hurley
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
///=====================================================================

#ifndef _CMANAGERLISTENER__H__
#define _CMANAGERLISTENER__H__

#include "IListener.h"

namespace ElementalEngine
{

class CManager;

/// \class CManagerListener
/// \brief	This class defines the listener for Managers. 
///			Listener are really callback functions, in this case
class CManagerListener : public IListener
{	
public:
	/// constructor for CManagerListener class
	/// \param parent = CAIManager class for callbacks to recieve messages
	CManagerListener(CManager *parent);
	/// \fn virtual void Init()
	/// \brief Function call back when system is being started up
	virtual bool Init(void);
	/// \fn virtual void Update()
	/// \brief Function call back when system is being updated
	/// \param - tickCount - current tick count
	virtual void Update(DWORD tickCount);
	/// \fn virtual void DeInit()
	/// \brief Function call back when system is being shutdown
	virtual bool DeInit(void);
	/// \fn virtual IHashString* GetName();
	/// \brief Function call back to get the name of the listener
	virtual IHashString* GetName();	
private:
	CManager *m_Parent;
};

} // namespace ElementalEngine

#endif