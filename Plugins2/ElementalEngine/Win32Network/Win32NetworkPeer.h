///============================================================================
/// \file		Win32NetworkPeer.h
/// \brief		Declaration of Win32 Network Peer Server or Client(using Raknet)
/// \date		06-27-2008
/// \author		Patrick Ghiocel
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

#ifndef	__WIN32NETWORKPEER_H__
#define __WIN32NETWORKPEER_H__

#include "StdAfx.h"

class CWin32NetworkPeer : public CObjectTemplate<IObject>
{
	private:
	/// Default Constructor
	CWin32NetworkPeer(IHashString *parentName, IHashString *name, bool bAddToHier);
	
public:
	/// Default Destructor
	virtual ~CWin32NetworkPeer();

	static IComponent *Create(int nArgs, va_list argptr);

	/// virtual void Init()
	/// \brief	Initialize the object
	void Init();

	/// virtual void Update()
	/// \brief	Update the object
	bool Update();

	/// Deinitialize the object
	void DeInit( void );

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );
	
	// ----------- Utility Functions ----------------//

	// ----------- Messages Functions ---------------//
	DWORD OnStartHost( DWORD size, void *params  );
	DWORD OnStartClient( DWORD size, void *params );
	
	DWORD OnConnect( DWORD size, void *params );
	DWORD OnDisconnect( DWORD size, void *params );
	DWORD OnSendNetworkPacket( DWORD size, void *params );

private:
	void OnReceivePacket( Packet *pPacket );

	/// Pointer to the EE Tool Box
	IToolBox *m_ToolBox;

	/// Pointer to Raknet Peer Interface
	RakPeerInterface *m_pPeer;

	/// Indicated type of Peer (Server=true, Client=false)
	bool m_bIsServer;

	IArchive *m_pReceivedPacketArchive;
	BYTE *m_pSendPacketBuffer;
	static const size_t SENDPACKETBUFFERSIZE = 4096;
};

#endif //__WIN32NETWORKPEER_H__

