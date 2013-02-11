///============================================================================
/// \file		KnowledgeBase.h
/// \brief		Header file for KnowledgeBase
/// \date		05-25-2005
/// \author		Josh Lulewicz
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

#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#ifndef	_COBJECTTEMPLATE_HPP
#include "CObjectTemplate.hpp"
#endif

typedef struct lua_State lua_State;

typedef enum luaScriptStateTag
{
	LSS_WAITFRAME,
	LSS_WAITTIME,
	LSS_RUNNING,
	LSS_NOTLOADED,
	LSS_DONE
} LUASCRIPTSTATE;

class CLuaScript : public CObjectTemplate<IObject> 
{
private:
	CLuaScript(IHashString *parentName, IHashString *name);

public:
	/// are we waiting, not loaded yet, etc.
    LUASCRIPTSTATE  m_State;              
	/// time to wake up
	float			m_fWaitTimestamp;
	/// number of frames to wait
    int				m_iWaitFrame;
	/// current time
    float			m_fTime;               

    ~CLuaScript(void);

	/// \brief Create the a instance of IObject
	/// \param void
	/// \returns IComponent*: Pointer to This
	static IComponent* Create( va_list argptr );

	/// \brief The Init Function for the ScriptObject
	/// \param void
	/// \return void
	virtual void Init( void );

	/// \brief The DeInit Function for the ScriptObject
	/// \param void
	/// \return void
	virtual void DeInit( void );

	/// override object template override
	virtual void Serialize(IArchive &ar);

	/// Create a Script Thread
	/// \param void
	/// \return void
    void CreateThread(void);

	/// Update
	/// \param elapsedSec - seconds elapsed since last update
	/// \return void
    void Update(float elapsedSec);

	// ----------- Messages Functions ---------------//

	/// \brief Start a this script object
	/// \param size - currently size is not important
	/// \param *params - pointer to nothing
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnStartScript(DWORD size, void *params);

private:
	/// Resume Script
	/// \param param - lua stack parameter
	/// \return void
	void ResumeScript(float param);

	/// Format error message
	/// \param void
	/// \return void
    void FormatError();

	/// Output Error that occured
	/// \param strType - error type
	/// \return void
    void OutputError(char *strType);

	/// current thread state
	lua_State	*m_pThreadState;
	/// last error
    char		m_LastErrorString[256];
	/// instance of Engine Toolbox
	IToolBox	*m_EngineToolBox;

	// name of script
	StdString m_ScriptName;

	/// body of the lua script
	char		*m_ScriptBody;

	/// auto start flag
	bool	m_bAutoStart;
};

#endif //#ifndef LUASCRIPT_H
