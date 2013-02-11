///============================================================================
/// \file		LuaStackCheck.h
/// \brief		Header file for Lua Stack Check
/// \date		02-9-2007
/// \author		Brian Bazyk
/// \note		Copyright (c) 2007 Signature Devices, Inc.
///============================================================================

#ifndef LUASTACKCHECK_H
#define LUASTACKCHECK_H

class CLuaStackCheck
{
public:
	CLuaStackCheck( lua_State *ls )
	{
		m_pLS = ls;
		m_iStackcheckEnter = lua_gettop( ls );
	}
	~CLuaStackCheck()
	{
		m_iStackcheckExit = lua_gettop( m_pLS);
		assert( m_iStackcheckEnter == m_iStackcheckExit );
	}

private:
	lua_State *m_pLS;
	int m_iStackcheckEnter;
	int m_iStackcheckExit;
};


#endif //#ifndef LUASTACKCHECK_H
