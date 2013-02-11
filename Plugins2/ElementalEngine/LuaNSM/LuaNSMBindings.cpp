/*
** Lua binding: LuaNSM
** Generated automatically by toluaxx-1.2.0 on 02/11/09 21:23:20.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "toluaxx.h"

#ifdef __cplusplus
#include<string>
 inline const char* tolua_tocppstring(lua_State* L, int narg, std::string def){return tolua_tocppstring(L,narg,def.c_str());}
 inline const char* tolua_tofieldcppstring(lua_State* L, int lo, int index, std::string def){return tolua_tofieldcppstring(L,lo,index,def.c_str());}
#endif
/* Exported function */
TOLUA_API int tolua_LuaNSM_open (lua_State* tolua_S);

#include "StdAfx.h"
#include "../../ElementalEngine/EELuaScript/LuaHashString.h"
#pragma warning(disable : 4800)
#pragma warning(disable : 4345)
class CLuaDWORD { public: CLuaDWORD() {} CLuaDWORD(unsigned long i) { x = i; } operator DWORD() { return x; } void operator=(DWORD i) { x = i; } unsigned long x; };
#if 0
#include "DNSMUtility.h"
#include "LuaStateHandler.h"
#endif

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_CLuaDWORD (lua_State* tolua_S)
{
 CLuaDWORD* self = (CLuaDWORD*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_ACTIONHANDLERLIST (lua_State* tolua_S)
{
 ACTIONHANDLERLIST* self = (ACTIONHANDLERLIST*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CLuaDWORD");
 tolua_usertype(tolua_S,"ACTIONHANDLERLIST");
 tolua_usertype(tolua_S,"EVENT");
}

/* function: CreateEvent */
#ifndef TOLUA_DISABLE_tolua_LuaNSM_CreateEvent00
static int tolua_LuaNSM_CreateEvent00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isstring(tolua_S,1,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  char* evtName = ((char*)  tolua_tostring(tolua_S,1,0));
  unsigned uSize = ((unsigned)  tolua_tonumber(tolua_S,2,0));
 {
  EVENT* tolua_ret = (EVENT*)  CreateEvent(evtName,uSize);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"EVENT");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CreateEvent'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: add of class  ACTIONHANDLERLIST */
#ifndef TOLUA_DISABLE_tolua_LuaNSM_ACTIONHANDLERLIST_add00
static int tolua_LuaNSM_ACTIONHANDLERLIST_add00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"ACTIONHANDLERLIST",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  ACTIONHANDLERLIST* self = (ACTIONHANDLERLIST*)  tolua_tousertype(tolua_S,1,0);
  lua_State* luaState =  tolua_S;
  char* funcName = ((char*)  tolua_tostring(tolua_S,2,0));
  lua_Function funcIdx = ((lua_Function)  tolua_tovalue(tolua_S,3,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add'",NULL);
 {
  self->add(luaState,funcName,funcIdx);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  ACTIONHANDLERLIST */
#ifndef TOLUA_DISABLE_tolua_LuaNSM_ACTIONHANDLERLIST_new00
static int tolua_LuaNSM_ACTIONHANDLERLIST_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"ACTIONHANDLERLIST",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  lua_State* l =  tolua_S;
 {
  ACTIONHANDLERLIST* tolua_ret = (ACTIONHANDLERLIST*)  new ACTIONHANDLERLIST(l);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"ACTIONHANDLERLIST");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  ACTIONHANDLERLIST */
#ifndef TOLUA_DISABLE_tolua_LuaNSM_ACTIONHANDLERLIST_new00_local
static int tolua_LuaNSM_ACTIONHANDLERLIST_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"ACTIONHANDLERLIST",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  lua_State* l =  tolua_S;
 {
  ACTIONHANDLERLIST* tolua_ret = (ACTIONHANDLERLIST*)  new ACTIONHANDLERLIST(l);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"ACTIONHANDLERLIST");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  ACTIONHANDLERLIST */
#ifndef TOLUA_DISABLE_tolua_LuaNSM_ACTIONHANDLERLIST_delete00
static int tolua_LuaNSM_ACTIONHANDLERLIST_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"ACTIONHANDLERLIST",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  ACTIONHANDLERLIST* self = (ACTIONHANDLERLIST*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  ACTIONHANDLERLIST */
#ifndef TOLUA_DISABLE_tolua_LuaNSM_ACTIONHANDLERLIST_SIZEOF00
static int tolua_LuaNSM_ACTIONHANDLERLIST_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"ACTIONHANDLERLIST",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  ACTIONHANDLERLIST* self = (ACTIONHANDLERLIST*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(ACTIONHANDLERLIST);
 {
#ifdef __cplusplus
 void* tolua_obj = new CLuaDWORD(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CLuaDWORD");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CLuaDWORD));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CLuaDWORD");
#endif
 }
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SIZEOF'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_LuaNSM_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_function(tolua_S,"CreateEvent",tolua_LuaNSM_CreateEvent00);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"ACTIONHANDLERLIST","ACTIONHANDLERLIST","",tolua_collect_ACTIONHANDLERLIST);
 #else
 tolua_cclass(tolua_S,"ACTIONHANDLERLIST","ACTIONHANDLERLIST","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"ACTIONHANDLERLIST");
  tolua_function(tolua_S,"add",tolua_LuaNSM_ACTIONHANDLERLIST_add00);
  tolua_function(tolua_S,"new",tolua_LuaNSM_ACTIONHANDLERLIST_new00);
  tolua_function(tolua_S,"new_local",tolua_LuaNSM_ACTIONHANDLERLIST_new00_local);
  tolua_function(tolua_S,".call",tolua_LuaNSM_ACTIONHANDLERLIST_new00_local);
  tolua_function(tolua_S,"delete",tolua_LuaNSM_ACTIONHANDLERLIST_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_LuaNSM_ACTIONHANDLERLIST_SIZEOF00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_LuaNSM (lua_State* tolua_S) {
 return tolua_LuaNSM_open(tolua_S);
};
#endif

