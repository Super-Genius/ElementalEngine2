/*
** Lua binding: QHStateMachineScript
** Generated automatically by toluaxx-1.2.0 on 02/02/09 21:47:59.
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
TOLUA_API int tolua_QHStateMachineScript_open (lua_State* tolua_S);

#include "StdAfx.h"
#include "../../ElementalEngine/EELuaScript/LuaHashString.h"
#pragma warning(disable : 4800)
#pragma warning(disable : 4345)
class CLuaDWORD { public: CLuaDWORD() {} CLuaDWORD(unsigned long i) { x = i; } operator DWORD() { return x; } void operator=(DWORD i) { x = i; } unsigned long x; };
#if 0 // To not include the "../"
#include "../../include/QHStateMachine/QHStateMachineDefines.h"
#endif

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_TRIGGEREVENTPARAMS (lua_State* tolua_S)
{
 TRIGGEREVENTPARAMS* self = (TRIGGEREVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_TRIGGEREVENTINMGRPARAMS (lua_State* tolua_S)
{
 TRIGGEREVENTINMGRPARAMS* self = (TRIGGEREVENTINMGRPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CLuaDWORD (lua_State* tolua_S)
{
 CLuaDWORD* self = (CLuaDWORD*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_LOADANDINITSTATEOBJECTPARAMS (lua_State* tolua_S)
{
 LOADANDINITSTATEOBJECTPARAMS* self = (LOADANDINITSTATEOBJECTPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CLuaHashString");
 tolua_usertype(tolua_S,"CLuaDWORD");
 tolua_usertype(tolua_S,"LOADANDINITSTATEOBJECTPARAMS");
 tolua_usertype(tolua_S,"IArchive");
 tolua_usertype(tolua_S,"TRIGGEREVENTPARAMS");
 tolua_usertype(tolua_S,"TRIGGEREVENTINMGRPARAMS");
}

/* get function:StateObjectName of class TRIGGEREVENTINMGRPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_TRIGGEREVENTINMGRPARAMS_StateObjectName_ptr
static int tolua_get_TRIGGEREVENTINMGRPARAMS_StateObjectName_ptr (lua_State* tolua_S) {
  TRIGGEREVENTINMGRPARAMS*self = (TRIGGEREVENTINMGRPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'StateObjectName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->StateObjectName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TRIGGEREVENTINMGRPARAMS_StateObjectName_ptr */

/* get function:StateObjectName of class TRIGGEREVENTINMGRPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_TRIGGEREVENTINMGRPARAMS_StateObjectName_ptr
static int tolua_set_TRIGGEREVENTINMGRPARAMS_StateObjectName_ptr (lua_State* tolua_S) {
  TRIGGEREVENTINMGRPARAMS*self = (TRIGGEREVENTINMGRPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'StateObjectName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->StateObjectName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TRIGGEREVENTINMGRPARAMS_StateObjectName_ptr */

/* get function:EventName of class TRIGGEREVENTINMGRPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_TRIGGEREVENTINMGRPARAMS_EventName_ptr
static int tolua_get_TRIGGEREVENTINMGRPARAMS_EventName_ptr (lua_State* tolua_S) {
  TRIGGEREVENTINMGRPARAMS*self = (TRIGGEREVENTINMGRPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->EventName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TRIGGEREVENTINMGRPARAMS_EventName_ptr */

/* get function:EventName of class TRIGGEREVENTINMGRPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_TRIGGEREVENTINMGRPARAMS_EventName_ptr
static int tolua_set_TRIGGEREVENTINMGRPARAMS_EventName_ptr (lua_State* tolua_S) {
  TRIGGEREVENTINMGRPARAMS*self = (TRIGGEREVENTINMGRPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->EventName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TRIGGEREVENTINMGRPARAMS_EventName_ptr */

/* get function:EventParamsArchive of class TRIGGEREVENTINMGRPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_TRIGGEREVENTINMGRPARAMS_EventParamsArchive_ptr
static int tolua_get_TRIGGEREVENTINMGRPARAMS_EventParamsArchive_ptr (lua_State* tolua_S) {
  TRIGGEREVENTINMGRPARAMS*self = (TRIGGEREVENTINMGRPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventParamsArchive'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->EventParamsArchive,"IArchive");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TRIGGEREVENTINMGRPARAMS_EventParamsArchive_ptr */

/* get function:EventParamsArchive of class TRIGGEREVENTINMGRPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_TRIGGEREVENTINMGRPARAMS_EventParamsArchive_ptr
static int tolua_set_TRIGGEREVENTINMGRPARAMS_EventParamsArchive_ptr (lua_State* tolua_S) {
  TRIGGEREVENTINMGRPARAMS*self = (TRIGGEREVENTINMGRPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventParamsArchive'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"IArchive",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->EventParamsArchive=(( IArchive*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TRIGGEREVENTINMGRPARAMS_EventParamsArchive_ptr */

/* method: new of class  TRIGGEREVENTINMGRPARAMS */
#ifndef TOLUA_DISABLE_tolua_QHStateMachineScript_TRIGGEREVENTINMGRPARAMS_new00
static int tolua_QHStateMachineScript_TRIGGEREVENTINMGRPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"TRIGGEREVENTINMGRPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  TRIGGEREVENTINMGRPARAMS* tolua_ret = (TRIGGEREVENTINMGRPARAMS*)  new TRIGGEREVENTINMGRPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"TRIGGEREVENTINMGRPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TRIGGEREVENTINMGRPARAMS */
#ifndef TOLUA_DISABLE_tolua_QHStateMachineScript_TRIGGEREVENTINMGRPARAMS_new00_local
static int tolua_QHStateMachineScript_TRIGGEREVENTINMGRPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"TRIGGEREVENTINMGRPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  TRIGGEREVENTINMGRPARAMS* tolua_ret = (TRIGGEREVENTINMGRPARAMS*)  new TRIGGEREVENTINMGRPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TRIGGEREVENTINMGRPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  TRIGGEREVENTINMGRPARAMS */
#ifndef TOLUA_DISABLE_tolua_QHStateMachineScript_TRIGGEREVENTINMGRPARAMS_delete00
static int tolua_QHStateMachineScript_TRIGGEREVENTINMGRPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"TRIGGEREVENTINMGRPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  TRIGGEREVENTINMGRPARAMS* self = (TRIGGEREVENTINMGRPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  TRIGGEREVENTINMGRPARAMS */
#ifndef TOLUA_DISABLE_tolua_QHStateMachineScript_TRIGGEREVENTINMGRPARAMS_SIZEOF00
static int tolua_QHStateMachineScript_TRIGGEREVENTINMGRPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"TRIGGEREVENTINMGRPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  TRIGGEREVENTINMGRPARAMS* self = (TRIGGEREVENTINMGRPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(TRIGGEREVENTINMGRPARAMS);
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

/* get function:EventName of class TRIGGEREVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_TRIGGEREVENTPARAMS_EventName_ptr
static int tolua_get_TRIGGEREVENTPARAMS_EventName_ptr (lua_State* tolua_S) {
  TRIGGEREVENTPARAMS*self = (TRIGGEREVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->EventName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TRIGGEREVENTPARAMS_EventName_ptr */

/* get function:EventName of class TRIGGEREVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_TRIGGEREVENTPARAMS_EventName_ptr
static int tolua_set_TRIGGEREVENTPARAMS_EventName_ptr (lua_State* tolua_S) {
  TRIGGEREVENTPARAMS*self = (TRIGGEREVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->EventName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TRIGGEREVENTPARAMS_EventName_ptr */

/* get function:EventParamsArchive of class TRIGGEREVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_TRIGGEREVENTPARAMS_EventParamsArchive_ptr
static int tolua_get_TRIGGEREVENTPARAMS_EventParamsArchive_ptr (lua_State* tolua_S) {
  TRIGGEREVENTPARAMS*self = (TRIGGEREVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventParamsArchive'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->EventParamsArchive,"IArchive");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TRIGGEREVENTPARAMS_EventParamsArchive_ptr */

/* get function:EventParamsArchive of class TRIGGEREVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_TRIGGEREVENTPARAMS_EventParamsArchive_ptr
static int tolua_set_TRIGGEREVENTPARAMS_EventParamsArchive_ptr (lua_State* tolua_S) {
  TRIGGEREVENTPARAMS*self = (TRIGGEREVENTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventParamsArchive'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"IArchive",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->EventParamsArchive=(( IArchive*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TRIGGEREVENTPARAMS_EventParamsArchive_ptr */

/* method: new of class  TRIGGEREVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_QHStateMachineScript_TRIGGEREVENTPARAMS_new00
static int tolua_QHStateMachineScript_TRIGGEREVENTPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"TRIGGEREVENTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  TRIGGEREVENTPARAMS* tolua_ret = (TRIGGEREVENTPARAMS*)  new TRIGGEREVENTPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"TRIGGEREVENTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TRIGGEREVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_QHStateMachineScript_TRIGGEREVENTPARAMS_new00_local
static int tolua_QHStateMachineScript_TRIGGEREVENTPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"TRIGGEREVENTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  TRIGGEREVENTPARAMS* tolua_ret = (TRIGGEREVENTPARAMS*)  new TRIGGEREVENTPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TRIGGEREVENTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  TRIGGEREVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_QHStateMachineScript_TRIGGEREVENTPARAMS_delete00
static int tolua_QHStateMachineScript_TRIGGEREVENTPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"TRIGGEREVENTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  TRIGGEREVENTPARAMS* self = (TRIGGEREVENTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  TRIGGEREVENTPARAMS */
#ifndef TOLUA_DISABLE_tolua_QHStateMachineScript_TRIGGEREVENTPARAMS_SIZEOF00
static int tolua_QHStateMachineScript_TRIGGEREVENTPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"TRIGGEREVENTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  TRIGGEREVENTPARAMS* self = (TRIGGEREVENTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(TRIGGEREVENTPARAMS);
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

/* get function:StateObjectName of class LOADANDINITSTATEOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateObjectName_ptr
static int tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateObjectName_ptr (lua_State* tolua_S) {
  LOADANDINITSTATEOBJECTPARAMS*self = (LOADANDINITSTATEOBJECTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'StateObjectName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->StateObjectName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateObjectName_ptr */

/* get function:StateObjectName of class LOADANDINITSTATEOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateObjectName_ptr
static int tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateObjectName_ptr (lua_State* tolua_S) {
  LOADANDINITSTATEOBJECTPARAMS*self = (LOADANDINITSTATEOBJECTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'StateObjectName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->StateObjectName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateObjectName_ptr */

/* get function:StateObjectParentName of class LOADANDINITSTATEOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateObjectParentName_ptr
static int tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateObjectParentName_ptr (lua_State* tolua_S) {
  LOADANDINITSTATEOBJECTPARAMS*self = (LOADANDINITSTATEOBJECTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'StateObjectParentName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->StateObjectParentName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateObjectParentName_ptr */

/* get function:StateObjectParentName of class LOADANDINITSTATEOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateObjectParentName_ptr
static int tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateObjectParentName_ptr (lua_State* tolua_S) {
  LOADANDINITSTATEOBJECTPARAMS*self = (LOADANDINITSTATEOBJECTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'StateObjectParentName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->StateObjectParentName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateObjectParentName_ptr */

/* get function:StateMachineName of class LOADANDINITSTATEOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateMachineName_ptr
static int tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateMachineName_ptr (lua_State* tolua_S) {
  LOADANDINITSTATEOBJECTPARAMS*self = (LOADANDINITSTATEOBJECTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'StateMachineName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->StateMachineName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateMachineName_ptr */

/* get function:StateMachineName of class LOADANDINITSTATEOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateMachineName_ptr
static int tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateMachineName_ptr (lua_State* tolua_S) {
  LOADANDINITSTATEOBJECTPARAMS*self = (LOADANDINITSTATEOBJECTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'StateMachineName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->StateMachineName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateMachineName_ptr */

/* get function:StateMachineFileName of class LOADANDINITSTATEOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateMachineFileName_ptr
static int tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateMachineFileName_ptr (lua_State* tolua_S) {
  LOADANDINITSTATEOBJECTPARAMS*self = (LOADANDINITSTATEOBJECTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'StateMachineFileName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->StateMachineFileName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateMachineFileName_ptr */

/* get function:StateMachineFileName of class LOADANDINITSTATEOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateMachineFileName_ptr
static int tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateMachineFileName_ptr (lua_State* tolua_S) {
  LOADANDINITSTATEOBJECTPARAMS*self = (LOADANDINITSTATEOBJECTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'StateMachineFileName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->StateMachineFileName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateMachineFileName_ptr */

/* get function:StateMachineStartState of class LOADANDINITSTATEOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateMachineStartState_ptr
static int tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateMachineStartState_ptr (lua_State* tolua_S) {
  LOADANDINITSTATEOBJECTPARAMS*self = (LOADANDINITSTATEOBJECTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'StateMachineStartState'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->StateMachineStartState,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateMachineStartState_ptr */

/* get function:StateMachineStartState of class LOADANDINITSTATEOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateMachineStartState_ptr
static int tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateMachineStartState_ptr (lua_State* tolua_S) {
  LOADANDINITSTATEOBJECTPARAMS*self = (LOADANDINITSTATEOBJECTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'StateMachineStartState'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->StateMachineStartState=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateMachineStartState_ptr */

/* get function:bIsInHierarchy of class LOADANDINITSTATEOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_LOADANDINITSTATEOBJECTPARAMS_bIsInHierarchy
static int tolua_get_LOADANDINITSTATEOBJECTPARAMS_bIsInHierarchy (lua_State* tolua_S) {
  LOADANDINITSTATEOBJECTPARAMS*self = (LOADANDINITSTATEOBJECTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bIsInHierarchy'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->bIsInHierarchy);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_LOADANDINITSTATEOBJECTPARAMS_bIsInHierarchy */

/* get function:bIsInHierarchy of class LOADANDINITSTATEOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_LOADANDINITSTATEOBJECTPARAMS_bIsInHierarchy
static int tolua_set_LOADANDINITSTATEOBJECTPARAMS_bIsInHierarchy (lua_State* tolua_S) {
  LOADANDINITSTATEOBJECTPARAMS*self = (LOADANDINITSTATEOBJECTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bIsInHierarchy'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->bIsInHierarchy=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_LOADANDINITSTATEOBJECTPARAMS_bIsInHierarchy */

/* get function:iUpdatePriority of class LOADANDINITSTATEOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_LOADANDINITSTATEOBJECTPARAMS_iUpdatePriority
static int tolua_get_LOADANDINITSTATEOBJECTPARAMS_iUpdatePriority (lua_State* tolua_S) {
  LOADANDINITSTATEOBJECTPARAMS*self = (LOADANDINITSTATEOBJECTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'iUpdatePriority'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->iUpdatePriority);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_LOADANDINITSTATEOBJECTPARAMS_iUpdatePriority */

/* get function:iUpdatePriority of class LOADANDINITSTATEOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_LOADANDINITSTATEOBJECTPARAMS_iUpdatePriority
static int tolua_set_LOADANDINITSTATEOBJECTPARAMS_iUpdatePriority (lua_State* tolua_S) {
  LOADANDINITSTATEOBJECTPARAMS*self = (LOADANDINITSTATEOBJECTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'iUpdatePriority'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->iUpdatePriority=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_LOADANDINITSTATEOBJECTPARAMS_iUpdatePriority */

/* method: new of class  LOADANDINITSTATEOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_QHStateMachineScript_LOADANDINITSTATEOBJECTPARAMS_new00
static int tolua_QHStateMachineScript_LOADANDINITSTATEOBJECTPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"LOADANDINITSTATEOBJECTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  LOADANDINITSTATEOBJECTPARAMS* tolua_ret = (LOADANDINITSTATEOBJECTPARAMS*)  new LOADANDINITSTATEOBJECTPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"LOADANDINITSTATEOBJECTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  LOADANDINITSTATEOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_QHStateMachineScript_LOADANDINITSTATEOBJECTPARAMS_new00_local
static int tolua_QHStateMachineScript_LOADANDINITSTATEOBJECTPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"LOADANDINITSTATEOBJECTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  LOADANDINITSTATEOBJECTPARAMS* tolua_ret = (LOADANDINITSTATEOBJECTPARAMS*)  new LOADANDINITSTATEOBJECTPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"LOADANDINITSTATEOBJECTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  LOADANDINITSTATEOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_QHStateMachineScript_LOADANDINITSTATEOBJECTPARAMS_delete00
static int tolua_QHStateMachineScript_LOADANDINITSTATEOBJECTPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"LOADANDINITSTATEOBJECTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  LOADANDINITSTATEOBJECTPARAMS* self = (LOADANDINITSTATEOBJECTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  LOADANDINITSTATEOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_QHStateMachineScript_LOADANDINITSTATEOBJECTPARAMS_SIZEOF00
static int tolua_QHStateMachineScript_LOADANDINITSTATEOBJECTPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"LOADANDINITSTATEOBJECTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  LOADANDINITSTATEOBJECTPARAMS* self = (LOADANDINITSTATEOBJECTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(LOADANDINITSTATEOBJECTPARAMS);
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
TOLUA_API int tolua_QHStateMachineScript_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"TRIGGEREVENTINMGRPARAMS","TRIGGEREVENTINMGRPARAMS","",tolua_collect_TRIGGEREVENTINMGRPARAMS);
 #else
 tolua_cclass(tolua_S,"TRIGGEREVENTINMGRPARAMS","TRIGGEREVENTINMGRPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"TRIGGEREVENTINMGRPARAMS");
  tolua_variable(tolua_S,"StateObjectName",tolua_get_TRIGGEREVENTINMGRPARAMS_StateObjectName_ptr,tolua_set_TRIGGEREVENTINMGRPARAMS_StateObjectName_ptr);
  tolua_variable(tolua_S,"EventName",tolua_get_TRIGGEREVENTINMGRPARAMS_EventName_ptr,tolua_set_TRIGGEREVENTINMGRPARAMS_EventName_ptr);
  tolua_variable(tolua_S,"EventParamsArchive",tolua_get_TRIGGEREVENTINMGRPARAMS_EventParamsArchive_ptr,tolua_set_TRIGGEREVENTINMGRPARAMS_EventParamsArchive_ptr);
  tolua_function(tolua_S,"new",tolua_QHStateMachineScript_TRIGGEREVENTINMGRPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_QHStateMachineScript_TRIGGEREVENTINMGRPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_QHStateMachineScript_TRIGGEREVENTINMGRPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_QHStateMachineScript_TRIGGEREVENTINMGRPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_QHStateMachineScript_TRIGGEREVENTINMGRPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"TRIGGEREVENTPARAMS","TRIGGEREVENTPARAMS","",tolua_collect_TRIGGEREVENTPARAMS);
 #else
 tolua_cclass(tolua_S,"TRIGGEREVENTPARAMS","TRIGGEREVENTPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"TRIGGEREVENTPARAMS");
  tolua_variable(tolua_S,"EventName",tolua_get_TRIGGEREVENTPARAMS_EventName_ptr,tolua_set_TRIGGEREVENTPARAMS_EventName_ptr);
  tolua_variable(tolua_S,"EventParamsArchive",tolua_get_TRIGGEREVENTPARAMS_EventParamsArchive_ptr,tolua_set_TRIGGEREVENTPARAMS_EventParamsArchive_ptr);
  tolua_function(tolua_S,"new",tolua_QHStateMachineScript_TRIGGEREVENTPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_QHStateMachineScript_TRIGGEREVENTPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_QHStateMachineScript_TRIGGEREVENTPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_QHStateMachineScript_TRIGGEREVENTPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_QHStateMachineScript_TRIGGEREVENTPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"LOADANDINITSTATEOBJECTPARAMS","LOADANDINITSTATEOBJECTPARAMS","",tolua_collect_LOADANDINITSTATEOBJECTPARAMS);
 #else
 tolua_cclass(tolua_S,"LOADANDINITSTATEOBJECTPARAMS","LOADANDINITSTATEOBJECTPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"LOADANDINITSTATEOBJECTPARAMS");
  tolua_variable(tolua_S,"StateObjectName",tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateObjectName_ptr,tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateObjectName_ptr);
  tolua_variable(tolua_S,"StateObjectParentName",tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateObjectParentName_ptr,tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateObjectParentName_ptr);
  tolua_variable(tolua_S,"StateMachineName",tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateMachineName_ptr,tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateMachineName_ptr);
  tolua_variable(tolua_S,"StateMachineFileName",tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateMachineFileName_ptr,tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateMachineFileName_ptr);
  tolua_variable(tolua_S,"StateMachineStartState",tolua_get_LOADANDINITSTATEOBJECTPARAMS_StateMachineStartState_ptr,tolua_set_LOADANDINITSTATEOBJECTPARAMS_StateMachineStartState_ptr);
  tolua_variable(tolua_S,"bIsInHierarchy",tolua_get_LOADANDINITSTATEOBJECTPARAMS_bIsInHierarchy,tolua_set_LOADANDINITSTATEOBJECTPARAMS_bIsInHierarchy);
  tolua_variable(tolua_S,"iUpdatePriority",tolua_get_LOADANDINITSTATEOBJECTPARAMS_iUpdatePriority,tolua_set_LOADANDINITSTATEOBJECTPARAMS_iUpdatePriority);
  tolua_function(tolua_S,"new",tolua_QHStateMachineScript_LOADANDINITSTATEOBJECTPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_QHStateMachineScript_LOADANDINITSTATEOBJECTPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_QHStateMachineScript_LOADANDINITSTATEOBJECTPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_QHStateMachineScript_LOADANDINITSTATEOBJECTPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_QHStateMachineScript_LOADANDINITSTATEOBJECTPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_QHStateMachineScript (lua_State* tolua_S) {
 return tolua_QHStateMachineScript_open(tolua_S);
};
#endif

