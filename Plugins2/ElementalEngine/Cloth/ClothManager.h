///============================================================================
/// \file		ClothManager.h
/// \brief		Header file for a Cloth Manager
/// \date		7-23-2007
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

#ifndef	CLOTHMANAGER_H
#define	CLOTHMANAGER_H

#include "CManager.h"
#include "lua.h"

class CClothManager : public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CClothManager);
	/// \brief The Default Constructor
	/// \return void
	CClothManager();
public:
	/// \brief The Default Deconstructor
	/// \return void
	virtual ~CClothManager();

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent *Create(int nArgs, va_list argptr);

	virtual bool Init();
	virtual bool DeInit();
	virtual void Update(DWORD tickCount);

	///	\brief	Start the manager
	///	\param	size = 0
	///	\param	data = void pointer to NULL
	///	\return DWROD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED,
	///						error
	DWORD OnStart(DWORD size, void *data);

	///	\brief	Stop the manager
	///	\param	size = 0
	///	\param	data = void pointer to NULL
	///	\return DWROD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED,
	///						error
	DWORD OnStop(DWORD size, void *data);

	void RegisterLuaDefinitions();
	// ------------------ Messages ------------------ //

private:
	bool m_bIsRunning;
	lua_State *m_LuaState;
	CHashString m_hszClothType;
};

#endif //CLOTHMANAGER_H
