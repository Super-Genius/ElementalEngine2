///=====================================================================
/// \file	ProxyManager.h
/// \brief	Manager for proxy objects 
///			Very barebones implementation, since the CManager has
///			everything we need, just basically needed to make this guy
///			to house the proxied objects and a pump
/// \date	03/20/2007
/// \author	Nick Tourte
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

#ifndef _C_PROXYMANAGER_H_
#define _C_PROXYMANAGER_H_

class CProxyManager : public CManager
{
protected:
	SINGLETONCONSTRUCTOROVERRIDE(CProxyManager);
	/// ctor
	CProxyManager();
public:
	/// dtor
	virtual ~CProxyManager();

	/// Create func
	/// \param nArgs = 0 (singleton)
	/// \param argptr = NULL
	/// \return succesfully created manager component
	static IComponent* Create( int nArgs, va_list argptr );

	/// Pump Hanlder to route messages to objects we manage. 
	/// Called through C++ callback
	/// \param msg = the ID of the message to send
	/// \param size = the size of the data associated with *data
	/// \param data = void pointer to data specific to message
	///	\param name = the name of the object to send the message to,
	///					if NULL, then compType cannot be null and all
	///					components of type compType will be called if they
	///					have message handler registered
	///	\param compType = name of component type NULL - means any type
	/// \return DWORD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED, 
	///						error
	virtual DWORD PumpHandler(DWORD size, void *data, 
		IHashString *name, IHashString *compType, MSGFUNCTION msgFunction);

private:

	/// Set to true when the pump gets registered
	bool m_bPumpRegistered;
};

#endif //_C_PROXYMANAGER_H_