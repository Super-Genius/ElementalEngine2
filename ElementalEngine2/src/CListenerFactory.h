///=====================================================================
/// \file	CListenerFactory.h
/// \brief	Definition of CListenerFactory Class
///         This files defines a CListenerFactory which is the interface
///			between AI Module and Red Phoenix.
///			Inherits from IManager
/// \date	02/15/2005
/// \author	Patrick Ghiocel
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

#ifndef _CLISTENERFACTORY_H__
#define _CLISTENERFACTORY_H__

#include "CManager.h"

typedef multimap<DWORD, IListener *> LISTENERMMAP;
typedef map<IListener *, DWORD> LISTENERKEYMAP;

class CListenerFactory : public IListenerFactory
{
	SINGLETON(CListenerFactory);

public:

	/// \brief The Default Destructor for CListenerFactory
	/// \return void
	~CListenerFactory( );

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create(int nArgs, va_list argptr);

	/// \brief	Initializes the AI Manager and AI System
	/// \param	void
	/// \return	bool: Success
	virtual bool Init( void );

	/// \brief	Update function for the AI Manager and AI System
	/// \param tickCount - current tick count
	/// \return	void
	virtual void Update(DWORD tickCount);
	
	/// \brief	DeInitializes the AI Manager and AI System
	/// \return	bool: Success
	virtual bool DeInit( void );

	/// Register a IListener with the system
	/// \param sourceListener - Listener that gets callbacks from system
	/// \param priority - priority listeners are called.
	virtual void Register(IListener *sourceListener, DWORD priority);
	/// UnRegister a IListener with the system
	/// \param sourceListener - Listener that gets callbacks from system
	virtual bool UnRegister(IListener *sourceListener);

	virtual void Serialize(IArchive &ar);

	/// return a string of the component type
	/// \return IHashString * pointer to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);


	/// Message for Global Ticks (Updates)
	DWORD GlobalTickMessage(DWORD size, void *param);

private:
	LISTENERMMAP m_RegisteredListeners;
	LISTENERKEYMAP m_ListenerKeys;
};

#endif
