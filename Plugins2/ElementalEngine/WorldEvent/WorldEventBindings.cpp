/*
** Lua binding: WorldEventScript
** Generated automatically by toluaxx-1.2.0 on 02/02/09 21:47:23.
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
TOLUA_API int tolua_WorldEventScript_open (lua_State* tolua_S);

#include "StdAfx.h"
#include "../../ElementalEngine/EELuaScript/LuaHashString.h"
#pragma warning(disable : 4800)
#pragma warning(disable : 4345)
class CLuaDWORD { public: CLuaDWORD() {} CLuaDWORD(unsigned long i) { x = i; } operator DWORD() { return x; } void operator=(DWORD i) { x = i; } unsigned long x; };
#if 0 // To not include the "../"
#include "../../include/WorldEvent/WorldEventDefines.h"
#endif

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_BROADCASTWORLDEVENTPARAMS (lua_State* tolua_S)
{
 BROADCASTWORLDEVENTPARAMS* self = (BROADCASTWORLDEVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CLuaDWORD (lua_State* tolua_S)
{
 CLuaDWORD* self = (CLuaDWORD*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_TRIGGERWORLDEVENTPARAMS (lua_State* tolua_S)
{
 TRIGGERWORLDEVENTPARAMS* self = (TRIGGERWORLDEVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CLuaHashString");
 tolua_usertype(tolua_S,"BROADCASTWORLDEVENTPARAMS");
 tolua_usertype(tolua_S,"CLuaDWORD");
 tolua_usertype(tolua_S,"TRIGGERWORLDEVENTPARAMS");
}

/* get function:pEntityName of class BROADCASTWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_BROADCASTWORLDEVENTPARAMS_pEntityName_ptr
static int tolua_get_BROADCASTWORLDEVENTPARAMS_pEntityName_ptr (lua_State* tolua_S) {
  BROADCASTWORLDEVENTPARAMS*self = (BROADCASTWORLDEVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pEntityName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->pEntityName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_BROADCASTWORLDEVENTPARAMS_pEntityName_ptr */

/* get function:pEntityName of class BROADCASTWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_BROADCASTWORLDEVENTPARAMS_pEntityName_ptr
static int tolua_set_BROADCASTWORLDEVENTPARAMS_pEntityName_ptr (lua_State* tolua_S) {
  BROADCASTWORLDEVENTPARAMS*self = (BROADCASTWORLDEVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pEntityName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->pEntityName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_BROADCASTWORLDEVENTPARAMS_pEntityName_ptr */

/* get function:pEntityType of class BROADCASTWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_BROADCASTWORLDEVENTPARAMS_pEntityType_ptr
static int tolua_get_BROADCASTWORLDEVENTPARAMS_pEntityType_ptr (lua_State* tolua_S) {
  BROADCASTWORLDEVENTPARAMS*self = (BROADCASTWORLDEVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pEntityType'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->pEntityType,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_BROADCASTWORLDEVENTPARAMS_pEntityType_ptr */

/* get function:pEntityType of class BROADCASTWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_BROADCASTWORLDEVENTPARAMS_pEntityType_ptr
static int tolua_set_BROADCASTWORLDEVENTPARAMS_pEntityType_ptr (lua_State* tolua_S) {
  BROADCASTWORLDEVENTPARAMS*self = (BROADCASTWORLDEVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pEntityType'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->pEntityType=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_BROADCASTWORLDEVENTPARAMS_pEntityType_ptr */

/* get function:pEventName of class BROADCASTWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_BROADCASTWORLDEVENTPARAMS_pEventName_ptr
static int tolua_get_BROADCASTWORLDEVENTPARAMS_pEventName_ptr (lua_State* tolua_S) {
  BROADCASTWORLDEVENTPARAMS*self = (BROADCASTWORLDEVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pEventName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->pEventName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_BROADCASTWORLDEVENTPARAMS_pEventName_ptr */

/* get function:pEventName of class BROADCASTWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_BROADCASTWORLDEVENTPARAMS_pEventName_ptr
static int tolua_set_BROADCASTWORLDEVENTPARAMS_pEventName_ptr (lua_State* tolua_S) {
  BROADCASTWORLDEVENTPARAMS*self = (BROADCASTWORLDEVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pEventName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->pEventName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_BROADCASTWORLDEVENTPARAMS_pEventName_ptr */

/* method: new of class  BROADCASTWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_WorldEventScript_BROADCASTWORLDEVENTPARAMS_new00
static int tolua_WorldEventScript_BROADCASTWORLDEVENTPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"BROADCASTWORLDEVENTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  BROADCASTWORLDEVENTPARAMS* tolua_ret = (BROADCASTWORLDEVENTPARAMS*)  new BROADCASTWORLDEVENTPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"BROADCASTWORLDEVENTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  BROADCASTWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_WorldEventScript_BROADCASTWORLDEVENTPARAMS_new00_local
static int tolua_WorldEventScript_BROADCASTWORLDEVENTPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"BROADCASTWORLDEVENTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  BROADCASTWORLDEVENTPARAMS* tolua_ret = (BROADCASTWORLDEVENTPARAMS*)  new BROADCASTWORLDEVENTPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"BROADCASTWORLDEVENTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  BROADCASTWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_WorldEventScript_BROADCASTWORLDEVENTPARAMS_delete00
static int tolua_WorldEventScript_BROADCASTWORLDEVENTPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"BROADCASTWORLDEVENTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  BROADCASTWORLDEVENTPARAMS* self = (BROADCASTWORLDEVENTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  BROADCASTWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_WorldEventScript_BROADCASTWORLDEVENTPARAMS_SIZEOF00
static int tolua_WorldEventScript_BROADCASTWORLDEVENTPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"BROADCASTWORLDEVENTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  BROADCASTWORLDEVENTPARAMS* self = (BROADCASTWORLDEVENTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(BROADCASTWORLDEVENTPARAMS);
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

/* get function:pEntityName of class TRIGGERWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_TRIGGERWORLDEVENTPARAMS_pEntityName_ptr
static int tolua_get_TRIGGERWORLDEVENTPARAMS_pEntityName_ptr (lua_State* tolua_S) {
  TRIGGERWORLDEVENTPARAMS*self = (TRIGGERWORLDEVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pEntityName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->pEntityName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TRIGGERWORLDEVENTPARAMS_pEntityName_ptr */

/* get function:pEntityName of class TRIGGERWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_TRIGGERWORLDEVENTPARAMS_pEntityName_ptr
static int tolua_set_TRIGGERWORLDEVENTPARAMS_pEntityName_ptr (lua_State* tolua_S) {
  TRIGGERWORLDEVENTPARAMS*self = (TRIGGERWORLDEVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pEntityName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->pEntityName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TRIGGERWORLDEVENTPARAMS_pEntityName_ptr */

/* get function:pEntityType of class TRIGGERWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_TRIGGERWORLDEVENTPARAMS_pEntityType_ptr
static int tolua_get_TRIGGERWORLDEVENTPARAMS_pEntityType_ptr (lua_State* tolua_S) {
  TRIGGERWORLDEVENTPARAMS*self = (TRIGGERWORLDEVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pEntityType'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->pEntityType,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TRIGGERWORLDEVENTPARAMS_pEntityType_ptr */

/* get function:pEntityType of class TRIGGERWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_TRIGGERWORLDEVENTPARAMS_pEntityType_ptr
static int tolua_set_TRIGGERWORLDEVENTPARAMS_pEntityType_ptr (lua_State* tolua_S) {
  TRIGGERWORLDEVENTPARAMS*self = (TRIGGERWORLDEVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pEntityType'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->pEntityType=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TRIGGERWORLDEVENTPARAMS_pEntityType_ptr */

/* get function:pEventName of class TRIGGERWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_TRIGGERWORLDEVENTPARAMS_pEventName_ptr
static int tolua_get_TRIGGERWORLDEVENTPARAMS_pEventName_ptr (lua_State* tolua_S) {
  TRIGGERWORLDEVENTPARAMS*self = (TRIGGERWORLDEVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pEventName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->pEventName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TRIGGERWORLDEVENTPARAMS_pEventName_ptr */

/* get function:pEventName of class TRIGGERWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_TRIGGERWORLDEVENTPARAMS_pEventName_ptr
static int tolua_set_TRIGGERWORLDEVENTPARAMS_pEventName_ptr (lua_State* tolua_S) {
  TRIGGERWORLDEVENTPARAMS*self = (TRIGGERWORLDEVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pEventName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->pEventName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TRIGGERWORLDEVENTPARAMS_pEventName_ptr */

/* method: new of class  TRIGGERWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_WorldEventScript_TRIGGERWORLDEVENTPARAMS_new00
static int tolua_WorldEventScript_TRIGGERWORLDEVENTPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"TRIGGERWORLDEVENTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  TRIGGERWORLDEVENTPARAMS* tolua_ret = (TRIGGERWORLDEVENTPARAMS*)  new TRIGGERWORLDEVENTPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"TRIGGERWORLDEVENTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TRIGGERWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_WorldEventScript_TRIGGERWORLDEVENTPARAMS_new00_local
static int tolua_WorldEventScript_TRIGGERWORLDEVENTPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"TRIGGERWORLDEVENTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  TRIGGERWORLDEVENTPARAMS* tolua_ret = (TRIGGERWORLDEVENTPARAMS*)  new TRIGGERWORLDEVENTPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TRIGGERWORLDEVENTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  TRIGGERWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_WorldEventScript_TRIGGERWORLDEVENTPARAMS_delete00
static int tolua_WorldEventScript_TRIGGERWORLDEVENTPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"TRIGGERWORLDEVENTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  TRIGGERWORLDEVENTPARAMS* self = (TRIGGERWORLDEVENTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  TRIGGERWORLDEVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_WorldEventScript_TRIGGERWORLDEVENTPARAMS_SIZEOF00
static int tolua_WorldEventScript_TRIGGERWORLDEVENTPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"TRIGGERWORLDEVENTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  TRIGGERWORLDEVENTPARAMS* self = (TRIGGERWORLDEVENTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(TRIGGERWORLDEVENTPARAMS);
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
TOLUA_API int tolua_WorldEventScript_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"BROADCASTWORLDEVENTPARAMS","BROADCASTWORLDEVENTPARAMS","",tolua_collect_BROADCASTWORLDEVENTPARAMS);
 #else
 tolua_cclass(tolua_S,"BROADCASTWORLDEVENTPARAMS","BROADCASTWORLDEVENTPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"BROADCASTWORLDEVENTPARAMS");
  tolua_variable(tolua_S,"pEntityName",tolua_get_BROADCASTWORLDEVENTPARAMS_pEntityName_ptr,tolua_set_BROADCASTWORLDEVENTPARAMS_pEntityName_ptr);
  tolua_variable(tolua_S,"pEntityType",tolua_get_BROADCASTWORLDEVENTPARAMS_pEntityType_ptr,tolua_set_BROADCASTWORLDEVENTPARAMS_pEntityType_ptr);
  tolua_variable(tolua_S,"pEventName",tolua_get_BROADCASTWORLDEVENTPARAMS_pEventName_ptr,tolua_set_BROADCASTWORLDEVENTPARAMS_pEventName_ptr);
  tolua_function(tolua_S,"new",tolua_WorldEventScript_BROADCASTWORLDEVENTPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_WorldEventScript_BROADCASTWORLDEVENTPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_WorldEventScript_BROADCASTWORLDEVENTPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_WorldEventScript_BROADCASTWORLDEVENTPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_WorldEventScript_BROADCASTWORLDEVENTPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"TRIGGERWORLDEVENTPARAMS","TRIGGERWORLDEVENTPARAMS","",tolua_collect_TRIGGERWORLDEVENTPARAMS);
 #else
 tolua_cclass(tolua_S,"TRIGGERWORLDEVENTPARAMS","TRIGGERWORLDEVENTPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"TRIGGERWORLDEVENTPARAMS");
  tolua_variable(tolua_S,"pEntityName",tolua_get_TRIGGERWORLDEVENTPARAMS_pEntityName_ptr,tolua_set_TRIGGERWORLDEVENTPARAMS_pEntityName_ptr);
  tolua_variable(tolua_S,"pEntityType",tolua_get_TRIGGERWORLDEVENTPARAMS_pEntityType_ptr,tolua_set_TRIGGERWORLDEVENTPARAMS_pEntityType_ptr);
  tolua_variable(tolua_S,"pEventName",tolua_get_TRIGGERWORLDEVENTPARAMS_pEventName_ptr,tolua_set_TRIGGERWORLDEVENTPARAMS_pEventName_ptr);
  tolua_function(tolua_S,"new",tolua_WorldEventScript_TRIGGERWORLDEVENTPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_WorldEventScript_TRIGGERWORLDEVENTPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_WorldEventScript_TRIGGERWORLDEVENTPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_WorldEventScript_TRIGGERWORLDEVENTPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_WorldEventScript_TRIGGERWORLDEVENTPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_WorldEventScript (lua_State* tolua_S) {
 return tolua_WorldEventScript_open(tolua_S);
};
#endif

