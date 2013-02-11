///=====================================================================
/// \file	IListener.h
/// \brief	Definition of IListener Interface
///         This class defines a IListener which is the generic
///			interface outside listener systems
/// \date	02/16/2005
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

#ifndef _ILISTENER__H__
#define _ILISTENER__H__

namespace ElementalEngine
{
/// \class IListener
/// \brief	This class defines interfaces for listeners. Listener are
//			really callback functions, in this case
class IListener
{	
public:
	virtual ~IListener() {};
	/// \fn virtual void Init()
	/// \brief Function call back when system is being started up
	virtual bool Init(void) = 0;
	/// \fn virtual void Update()
	/// \brief Function call back when system is being updated
	/// \param tickCount - Current tick count
	virtual void Update(DWORD tickCount) = 0;
	/// \fn virtual void DeInit()
	/// \brief Function call back when system is being shutdown
	virtual bool DeInit(void) = 0;
	/// \fn virtual IHashString* GetName();
	/// \brief Function call back to get the name of the listener
	virtual IHashString* GetName() = 0;
};
}	//namespace ElementalEngine

#endif