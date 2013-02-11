///=====================================================================
/// \file	CAIManager.h
/// \brief	Definition of CAIManager Class
///         This files defines a CAIManager which is the interface
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

#ifndef _CAIMANAGER_H__
#define _CAIMANAGER_H__


namespace ElementalEngine
{
class IObject;
}

class CAIManager : public CManager
{
public:

	/// \brief The Default Destructor for CAIManager
	~CAIManager( );

	void RegisterLuaDefinitions();

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief	Initializes the AI Manager and AI System
	/// \param	void
	/// \return	bool: Success
	virtual bool Init( void );

	/// \brief	Start running the manager
	DWORD OnStart(DWORD size, void *params);

	/// \brief	Stop running the manager
	DWORD OnStop(DWORD size, void *params);

	/// \brief	Returns a the CAIObject with the specified name
	CAIObject *GetAIObject( IHashString *objectName );

private:
	SINGLETONCONSTRUCTOROVERRIDE(CAIManager);
	/// \brief The Default Constructor for CAIManager
	CAIManager();

	lua_State *m_LuaState;

	/// Boolean controling if the simulation is active or not
	bool m_IsRunning;

	CHashString m_AIObjectComponentType;
};

#endif
