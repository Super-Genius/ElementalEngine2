///============================================================================
/// \file		ILuaScript.h
/// \brief		Header file for ILuaScript Interface
/// \date		08-22-2005
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

#ifndef _ILUASCRIPT_H_
#define _ILUASCRIPT_H_

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "lstate.h"
#include "lfunc.h"
#include "ldo.h"


class ILuaScript : public IObject
{
public:
    /// Used by "EE to lua script" callback system to wrap parameters going to lua
    virtual void Call(IArchive *ar, IHashString *name)=0;
	virtual int WaitSec(lua_State *l)=0;
	virtual int WaitTime(lua_State *l)=0;
	virtual int WaitFrame(lua_State *l)=0;
	virtual int LuaBeep(lua_State *l)=0;
	virtual IHashString* GetPropertyTableName()=0;
	virtual void RegisterActionHandler(char *name, int funcRef) = 0;
};

#define GetScriptObject(s, l)					\
	lua_pushlightuserdata(l, l);				\
    lua_gettable(l, LUA_GLOBALSINDEX );			\
    s = (ILuaScript *)lua_touserdata(l, -1);	\
	lua_pop(l, 1);							

#endif // #ifndef _ILUASCRIPT_H_