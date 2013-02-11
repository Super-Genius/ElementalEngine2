/*
** Lua binding: FadeUtility
** Generated automatically by toluaxx-1.2.0 on 02/11/09 20:31:56.
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
TOLUA_API int tolua_FadeUtility_open (lua_State* tolua_S);

#include "StdAfx.h"
#include "../../ElementalEngine/EELuaScript/LuaHashString.h"
#pragma warning(disable : 4800)
#pragma warning(disable : 4345)
class CLuaDWORD { public: CLuaDWORD() {} CLuaDWORD(unsigned long i) { x = i; } operator DWORD() { return x; } void operator=(DWORD i) { x = i; } unsigned long x; };
#if 0 // To not include the "../"
#include "../../include/GameUtilities/FadeUtilityDefine.h"
#endif

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_CLuaDWORD (lua_State* tolua_S)
{
 CLuaDWORD* self = (CLuaDWORD*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_FADEUTILITYPARAMS (lua_State* tolua_S)
{
 FADEUTILITYPARAMS* self = (FADEUTILITYPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CLuaHashString");
 tolua_usertype(tolua_S,"CLuaDWORD");
 tolua_usertype(tolua_S,"FADEUTILITYPARAMS");
}

/* get function:ObjectName of class FADEUTILITYPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_FADEUTILITYPARAMS_ObjectName_ptr
static int tolua_get_FADEUTILITYPARAMS_ObjectName_ptr (lua_State* tolua_S) {
  FADEUTILITYPARAMS*self = (FADEUTILITYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ObjectName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->ObjectName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_FADEUTILITYPARAMS_ObjectName_ptr */

/* get function:ObjectName of class FADEUTILITYPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_FADEUTILITYPARAMS_ObjectName_ptr
static int tolua_set_FADEUTILITYPARAMS_ObjectName_ptr (lua_State* tolua_S) {
  FADEUTILITYPARAMS*self = (FADEUTILITYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ObjectName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->ObjectName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_FADEUTILITYPARAMS_ObjectName_ptr */

/* get function:CallbackEvent of class FADEUTILITYPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_FADEUTILITYPARAMS_CallbackEvent_ptr
static int tolua_get_FADEUTILITYPARAMS_CallbackEvent_ptr (lua_State* tolua_S) {
  FADEUTILITYPARAMS*self = (FADEUTILITYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'CallbackEvent'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->CallbackEvent,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_FADEUTILITYPARAMS_CallbackEvent_ptr */

/* get function:CallbackEvent of class FADEUTILITYPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_FADEUTILITYPARAMS_CallbackEvent_ptr
static int tolua_set_FADEUTILITYPARAMS_CallbackEvent_ptr (lua_State* tolua_S) {
  FADEUTILITYPARAMS*self = (FADEUTILITYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'CallbackEvent'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->CallbackEvent=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_FADEUTILITYPARAMS_CallbackEvent_ptr */

/* get function:FadeTime of class FADEUTILITYPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_FADEUTILITYPARAMS_FadeTime
static int tolua_get_FADEUTILITYPARAMS_FadeTime (lua_State* tolua_S) {
  FADEUTILITYPARAMS*self = (FADEUTILITYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'FadeTime'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->FadeTime);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_FADEUTILITYPARAMS_FadeTime */

/* get function:FadeTime of class FADEUTILITYPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_FADEUTILITYPARAMS_FadeTime
static int tolua_set_FADEUTILITYPARAMS_FadeTime (lua_State* tolua_S) {
  FADEUTILITYPARAMS*self = (FADEUTILITYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'FadeTime'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->FadeTime=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_FADEUTILITYPARAMS_FadeTime */

/* get function:StartFadeAmount of class FADEUTILITYPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_FADEUTILITYPARAMS_StartFadeAmount
static int tolua_get_FADEUTILITYPARAMS_StartFadeAmount (lua_State* tolua_S) {
  FADEUTILITYPARAMS*self = (FADEUTILITYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'StartFadeAmount'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->StartFadeAmount);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_FADEUTILITYPARAMS_StartFadeAmount */

/* get function:StartFadeAmount of class FADEUTILITYPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_FADEUTILITYPARAMS_StartFadeAmount
static int tolua_set_FADEUTILITYPARAMS_StartFadeAmount (lua_State* tolua_S) {
  FADEUTILITYPARAMS*self = (FADEUTILITYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'StartFadeAmount'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->StartFadeAmount=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_FADEUTILITYPARAMS_StartFadeAmount */

/* get function:EndFadeAmount of class FADEUTILITYPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_FADEUTILITYPARAMS_EndFadeAmount
static int tolua_get_FADEUTILITYPARAMS_EndFadeAmount (lua_State* tolua_S) {
  FADEUTILITYPARAMS*self = (FADEUTILITYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EndFadeAmount'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->EndFadeAmount);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_FADEUTILITYPARAMS_EndFadeAmount */

/* get function:EndFadeAmount of class FADEUTILITYPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_FADEUTILITYPARAMS_EndFadeAmount
static int tolua_set_FADEUTILITYPARAMS_EndFadeAmount (lua_State* tolua_S) {
  FADEUTILITYPARAMS*self = (FADEUTILITYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EndFadeAmount'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->EndFadeAmount=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_FADEUTILITYPARAMS_EndFadeAmount */

/* get function:bClearOverride of class FADEUTILITYPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_FADEUTILITYPARAMS_bClearOverride
static int tolua_get_FADEUTILITYPARAMS_bClearOverride (lua_State* tolua_S) {
  FADEUTILITYPARAMS*self = (FADEUTILITYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bClearOverride'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->bClearOverride);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_FADEUTILITYPARAMS_bClearOverride */

/* get function:bClearOverride of class FADEUTILITYPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_FADEUTILITYPARAMS_bClearOverride
static int tolua_set_FADEUTILITYPARAMS_bClearOverride (lua_State* tolua_S) {
  FADEUTILITYPARAMS*self = (FADEUTILITYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bClearOverride'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->bClearOverride=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_FADEUTILITYPARAMS_bClearOverride */

/* method: new of class  FADEUTILITYPARAMS */
#ifndef TOLUA_DISABLE_tolua_FadeUtility_FADEUTILITYPARAMS_new00
static int tolua_FadeUtility_FADEUTILITYPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"FADEUTILITYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  FADEUTILITYPARAMS* tolua_ret = (FADEUTILITYPARAMS*)  new FADEUTILITYPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"FADEUTILITYPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  FADEUTILITYPARAMS */
#ifndef TOLUA_DISABLE_tolua_FadeUtility_FADEUTILITYPARAMS_new00_local
static int tolua_FadeUtility_FADEUTILITYPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"FADEUTILITYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  FADEUTILITYPARAMS* tolua_ret = (FADEUTILITYPARAMS*)  new FADEUTILITYPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"FADEUTILITYPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  FADEUTILITYPARAMS */
#ifndef TOLUA_DISABLE_tolua_FadeUtility_FADEUTILITYPARAMS_delete00
static int tolua_FadeUtility_FADEUTILITYPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"FADEUTILITYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  FADEUTILITYPARAMS* self = (FADEUTILITYPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  FADEUTILITYPARAMS */
#ifndef TOLUA_DISABLE_tolua_FadeUtility_FADEUTILITYPARAMS_SIZEOF00
static int tolua_FadeUtility_FADEUTILITYPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"FADEUTILITYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  FADEUTILITYPARAMS* self = (FADEUTILITYPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(FADEUTILITYPARAMS);
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
TOLUA_API int tolua_FadeUtility_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"FADEUTILITYPARAMS","FADEUTILITYPARAMS","",tolua_collect_FADEUTILITYPARAMS);
 #else
 tolua_cclass(tolua_S,"FADEUTILITYPARAMS","FADEUTILITYPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"FADEUTILITYPARAMS");
  tolua_variable(tolua_S,"ObjectName",tolua_get_FADEUTILITYPARAMS_ObjectName_ptr,tolua_set_FADEUTILITYPARAMS_ObjectName_ptr);
  tolua_variable(tolua_S,"CallbackEvent",tolua_get_FADEUTILITYPARAMS_CallbackEvent_ptr,tolua_set_FADEUTILITYPARAMS_CallbackEvent_ptr);
  tolua_variable(tolua_S,"FadeTime",tolua_get_FADEUTILITYPARAMS_FadeTime,tolua_set_FADEUTILITYPARAMS_FadeTime);
  tolua_variable(tolua_S,"StartFadeAmount",tolua_get_FADEUTILITYPARAMS_StartFadeAmount,tolua_set_FADEUTILITYPARAMS_StartFadeAmount);
  tolua_variable(tolua_S,"EndFadeAmount",tolua_get_FADEUTILITYPARAMS_EndFadeAmount,tolua_set_FADEUTILITYPARAMS_EndFadeAmount);
  tolua_variable(tolua_S,"bClearOverride",tolua_get_FADEUTILITYPARAMS_bClearOverride,tolua_set_FADEUTILITYPARAMS_bClearOverride);
  tolua_function(tolua_S,"new",tolua_FadeUtility_FADEUTILITYPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_FadeUtility_FADEUTILITYPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_FadeUtility_FADEUTILITYPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_FadeUtility_FADEUTILITYPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_FadeUtility_FADEUTILITYPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_FadeUtility (lua_State* tolua_S) {
 return tolua_FadeUtility_open(tolua_S);
};
#endif

