///==========================================================================
/// \file	Win32NetworkDefines.h
/// \brief	Structs for win 2 network plugin
/// \date	06/27/2008
/// \author	Patrick Ghiocel
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

#ifndef _WIN32NETWORKDEFINES_H_
#define _WIN32NETWORKDEFINES_H_

// tolua_begin
struct CREATENETWORKPEER
{
	unsigned int iServerPort;
	unsigned int iMaxClients;
	unsigned int iThreadSleepTimer;

	CREATENETWORKPEER()
	{
		iServerPort = 50000;
		iMaxClients = 2;
		iThreadSleepTimer = 30;
	};
};

struct CONNECTPEER
{
	unsigned int iServerPort;
	StdString szAddress;
	unsigned int iPasswordSize;
	StdString szPassword;
	bool bResult;
			
	CONNECTPEER()
	{
		iServerPort = 50000;
		szAddress = "127.0.0.1";
		iPasswordSize = 0;
		szPassword = "";
		bResult = false;
	};
};

struct DISCONNECTPEER
{
	bool bResult;
	bool bNotify;
	int iChannel;

	DISCONNECTPEER()
	{
		bResult = false;
		bNotify = true;
		iChannel = 0;
	};
};
// tolua_end

#endif //_LANGUAGELOCALIZERDEFINES_H_