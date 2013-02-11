///============================================================================
/// \file		IActionHandler.h
/// \brief		Header file for the Action Handler Interface
/// \date		07-01-2005
/// \author		Josh Lulewicz
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

#ifndef IACTIONHANDLER_H
#define IACTIONHANDLER_H

namespace ElementalEngine
{

/// \interface IActionHandler
/// \brief Action Handler Interface
class IActionHandler : public IComponent
{
public:
	/// Destructor
	virtual ~IActionHandler() {}

	/// Action Handler callback
	/// \param parms - data for action function
	/// \param name - name of action function
	typedef void (IActionHandler::*ACTIONCALLBACK)(IArchive *parms, IHashString *name, IHashString *parentName);
	typedef map<DWORD, ACTIONCALLBACK> ACTIONCALLBACKMAP;

	/// Get action handler(s) implemented in a derivation of this class.
	/// \return - Map of action names to call back member functions
	virtual ACTIONCALLBACKMAP *GetActionHandlers() = 0;

    /// Get the namespace that protects actions from name clashes.  Compliant Lua scripts specify
    /// this name when loaded, whereas C++ actions supply this name merely by using the
    /// name of the derived class (derived from IActionHandler).
    virtual IHashString* GetNameSpace() = 0;
};

}		//namespace ElementalEngine

#endif // IACTIONHANDLER_H
