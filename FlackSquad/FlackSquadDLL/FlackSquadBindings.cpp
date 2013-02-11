/*
** Lua binding: FlackSquadScript
** Generated automatically by toluaxx-1.2.0 on 06/04/08 16:57:11.
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
TOLUA_API int tolua_FlackSquadScript_open (lua_State* tolua_S);

#include "StdAfx.h"
#include "../../Plugins2/ElementalEngine/EELuaScript/LuaHashString.h"
#pragma warning(disable : 4800)
#pragma warning(disable : 4345)
class CLuaDWORD { public: CLuaDWORD() {} CLuaDWORD(unsigned long i) { x = i; } operator DWORD() { return x; } void operator=(DWORD i) { x = i; } unsigned long x; };
#if 0 // To not include the "../"
#include "FlackSquadNetwork.h"
#endif
#pragma warning(disable : 4800)

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_NETWORKMSG_DESTROYOBJECTPARAMS (lua_State* tolua_S)
{
 NETWORKMSG_DESTROYOBJECTPARAMS* self = (NETWORKMSG_DESTROYOBJECTPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_NETWORKMSG_TURRETSTARTFIREPARAMS (lua_State* tolua_S)
{
 NETWORKMSG_TURRETSTARTFIREPARAMS* self = (NETWORKMSG_TURRETSTARTFIREPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_NETWORKMSG_SPAWNOBJECTPARAMS (lua_State* tolua_S)
{
 NETWORKMSG_SPAWNOBJECTPARAMS* self = (NETWORKMSG_SPAWNOBJECTPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CLuaDWORD (lua_State* tolua_S)
{
 CLuaDWORD* self = (CLuaDWORD*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"NETWORKMSG_DESTROYOBJECTPARAMS");
 tolua_usertype(tolua_S,"NETWORKMSG_TURRETSTARTFIREPARAMS");
 tolua_usertype(tolua_S,"CLuaDWORD");
 tolua_usertype(tolua_S,"NETWORKMSG_SPAWNOBJECTPARAMS");
}

/* method: new of class  NETWORKMSG_SPAWNOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_FlackSquadScript_NETWORKMSG_SPAWNOBJECTPARAMS_new00
static int tolua_FlackSquadScript_NETWORKMSG_SPAWNOBJECTPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"NETWORKMSG_SPAWNOBJECTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  NETWORKMSG_SPAWNOBJECTPARAMS* tolua_ret = (NETWORKMSG_SPAWNOBJECTPARAMS*)  new NETWORKMSG_SPAWNOBJECTPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"NETWORKMSG_SPAWNOBJECTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  NETWORKMSG_SPAWNOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_FlackSquadScript_NETWORKMSG_SPAWNOBJECTPARAMS_new00_local
static int tolua_FlackSquadScript_NETWORKMSG_SPAWNOBJECTPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"NETWORKMSG_SPAWNOBJECTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  NETWORKMSG_SPAWNOBJECTPARAMS* tolua_ret = (NETWORKMSG_SPAWNOBJECTPARAMS*)  new NETWORKMSG_SPAWNOBJECTPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"NETWORKMSG_SPAWNOBJECTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  NETWORKMSG_SPAWNOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_FlackSquadScript_NETWORKMSG_SPAWNOBJECTPARAMS_delete00
static int tolua_FlackSquadScript_NETWORKMSG_SPAWNOBJECTPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"NETWORKMSG_SPAWNOBJECTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  NETWORKMSG_SPAWNOBJECTPARAMS* self = (NETWORKMSG_SPAWNOBJECTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  NETWORKMSG_SPAWNOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_FlackSquadScript_NETWORKMSG_SPAWNOBJECTPARAMS_SIZEOF00
static int tolua_FlackSquadScript_NETWORKMSG_SPAWNOBJECTPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"NETWORKMSG_SPAWNOBJECTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  NETWORKMSG_SPAWNOBJECTPARAMS* self = (NETWORKMSG_SPAWNOBJECTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(NETWORKMSG_SPAWNOBJECTPARAMS);
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

/* method: new of class  NETWORKMSG_DESTROYOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_FlackSquadScript_NETWORKMSG_DESTROYOBJECTPARAMS_new00
static int tolua_FlackSquadScript_NETWORKMSG_DESTROYOBJECTPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"NETWORKMSG_DESTROYOBJECTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  NETWORKMSG_DESTROYOBJECTPARAMS* tolua_ret = (NETWORKMSG_DESTROYOBJECTPARAMS*)  new NETWORKMSG_DESTROYOBJECTPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"NETWORKMSG_DESTROYOBJECTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  NETWORKMSG_DESTROYOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_FlackSquadScript_NETWORKMSG_DESTROYOBJECTPARAMS_new00_local
static int tolua_FlackSquadScript_NETWORKMSG_DESTROYOBJECTPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"NETWORKMSG_DESTROYOBJECTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  NETWORKMSG_DESTROYOBJECTPARAMS* tolua_ret = (NETWORKMSG_DESTROYOBJECTPARAMS*)  new NETWORKMSG_DESTROYOBJECTPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"NETWORKMSG_DESTROYOBJECTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  NETWORKMSG_DESTROYOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_FlackSquadScript_NETWORKMSG_DESTROYOBJECTPARAMS_delete00
static int tolua_FlackSquadScript_NETWORKMSG_DESTROYOBJECTPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"NETWORKMSG_DESTROYOBJECTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  NETWORKMSG_DESTROYOBJECTPARAMS* self = (NETWORKMSG_DESTROYOBJECTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  NETWORKMSG_DESTROYOBJECTPARAMS */
#ifndef TOLUA_DISABLE_tolua_FlackSquadScript_NETWORKMSG_DESTROYOBJECTPARAMS_SIZEOF00
static int tolua_FlackSquadScript_NETWORKMSG_DESTROYOBJECTPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"NETWORKMSG_DESTROYOBJECTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  NETWORKMSG_DESTROYOBJECTPARAMS* self = (NETWORKMSG_DESTROYOBJECTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(NETWORKMSG_DESTROYOBJECTPARAMS);
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

/* get function:iWeapon of class NETWORKMSG_TURRETSTARTFIREPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_NETWORKMSG_TURRETSTARTFIREPARAMS_unsigned_iWeapon
static int tolua_get_NETWORKMSG_TURRETSTARTFIREPARAMS_unsigned_iWeapon (lua_State* tolua_S) {
  NETWORKMSG_TURRETSTARTFIREPARAMS*self = (NETWORKMSG_TURRETSTARTFIREPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'iWeapon'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->iWeapon);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_NETWORKMSG_TURRETSTARTFIREPARAMS_unsigned_iWeapon */

/* get function:iWeapon of class NETWORKMSG_TURRETSTARTFIREPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_NETWORKMSG_TURRETSTARTFIREPARAMS_unsigned_iWeapon
static int tolua_set_NETWORKMSG_TURRETSTARTFIREPARAMS_unsigned_iWeapon (lua_State* tolua_S) {
  NETWORKMSG_TURRETSTARTFIREPARAMS*self = (NETWORKMSG_TURRETSTARTFIREPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'iWeapon'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->iWeapon=(( unsigned int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_NETWORKMSG_TURRETSTARTFIREPARAMS_unsigned_iWeapon */

/* method: new of class  NETWORKMSG_TURRETSTARTFIREPARAMS */
#ifndef TOLUA_DISABLE_tolua_FlackSquadScript_NETWORKMSG_TURRETSTARTFIREPARAMS_new00
static int tolua_FlackSquadScript_NETWORKMSG_TURRETSTARTFIREPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"NETWORKMSG_TURRETSTARTFIREPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  NETWORKMSG_TURRETSTARTFIREPARAMS* tolua_ret = (NETWORKMSG_TURRETSTARTFIREPARAMS*)  new NETWORKMSG_TURRETSTARTFIREPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"NETWORKMSG_TURRETSTARTFIREPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  NETWORKMSG_TURRETSTARTFIREPARAMS */
#ifndef TOLUA_DISABLE_tolua_FlackSquadScript_NETWORKMSG_TURRETSTARTFIREPARAMS_new00_local
static int tolua_FlackSquadScript_NETWORKMSG_TURRETSTARTFIREPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"NETWORKMSG_TURRETSTARTFIREPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  NETWORKMSG_TURRETSTARTFIREPARAMS* tolua_ret = (NETWORKMSG_TURRETSTARTFIREPARAMS*)  new NETWORKMSG_TURRETSTARTFIREPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"NETWORKMSG_TURRETSTARTFIREPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  NETWORKMSG_TURRETSTARTFIREPARAMS */
#ifndef TOLUA_DISABLE_tolua_FlackSquadScript_NETWORKMSG_TURRETSTARTFIREPARAMS_delete00
static int tolua_FlackSquadScript_NETWORKMSG_TURRETSTARTFIREPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"NETWORKMSG_TURRETSTARTFIREPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  NETWORKMSG_TURRETSTARTFIREPARAMS* self = (NETWORKMSG_TURRETSTARTFIREPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  NETWORKMSG_TURRETSTARTFIREPARAMS */
#ifndef TOLUA_DISABLE_tolua_FlackSquadScript_NETWORKMSG_TURRETSTARTFIREPARAMS_SIZEOF00
static int tolua_FlackSquadScript_NETWORKMSG_TURRETSTARTFIREPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"NETWORKMSG_TURRETSTARTFIREPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  NETWORKMSG_TURRETSTARTFIREPARAMS* self = (NETWORKMSG_TURRETSTARTFIREPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(NETWORKMSG_TURRETSTARTFIREPARAMS);
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
TOLUA_API int tolua_FlackSquadScript_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_constant(tolua_S,"NETWORKMSG_SPAWNOBJECT",NETWORKMSG_SPAWNOBJECT);
 tolua_constant(tolua_S,"NETWORKMSG_DESTROYOBJECT",NETWORKMSG_DESTROYOBJECT);
 tolua_constant(tolua_S,"NETWORKMSG_OBJECTSELECTTARGET",NETWORKMSG_OBJECTSELECTTARGET);
 tolua_constant(tolua_S,"NETWORKMSG_TURRETANGLES",NETWORKMSG_TURRETANGLES);
 tolua_constant(tolua_S,"NETWORKMSG_TURRETSTARTFIRE",NETWORKMSG_TURRETSTARTFIRE);
 tolua_constant(tolua_S,"NETWORKMSG_TURRETSTOPFIRE",NETWORKMSG_TURRETSTOPFIRE);
 tolua_constant(tolua_S,"NETWORKMSG_TURRETMISSILEFIRE",NETWORKMSG_TURRETMISSILEFIRE);
 tolua_constant(tolua_S,"NETWORKMSG_PLAYERDIED",NETWORKMSG_PLAYERDIED);
 tolua_constant(tolua_S,"NETWORKMSG_STARTMISSION",NETWORKMSG_STARTMISSION);
 tolua_constant(tolua_S,"NETWORKMSG_STOPMISSION",NETWORKMSG_STOPMISSION);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"NETWORKMSG_SPAWNOBJECTPARAMS","NETWORKMSG_SPAWNOBJECTPARAMS","",tolua_collect_NETWORKMSG_SPAWNOBJECTPARAMS);
 #else
 tolua_cclass(tolua_S,"NETWORKMSG_SPAWNOBJECTPARAMS","NETWORKMSG_SPAWNOBJECTPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"NETWORKMSG_SPAWNOBJECTPARAMS");
  tolua_function(tolua_S,"new",tolua_FlackSquadScript_NETWORKMSG_SPAWNOBJECTPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_FlackSquadScript_NETWORKMSG_SPAWNOBJECTPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_FlackSquadScript_NETWORKMSG_SPAWNOBJECTPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_FlackSquadScript_NETWORKMSG_SPAWNOBJECTPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_FlackSquadScript_NETWORKMSG_SPAWNOBJECTPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"NETWORKMSG_DESTROYOBJECTPARAMS","NETWORKMSG_DESTROYOBJECTPARAMS","",tolua_collect_NETWORKMSG_DESTROYOBJECTPARAMS);
 #else
 tolua_cclass(tolua_S,"NETWORKMSG_DESTROYOBJECTPARAMS","NETWORKMSG_DESTROYOBJECTPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"NETWORKMSG_DESTROYOBJECTPARAMS");
  tolua_function(tolua_S,"new",tolua_FlackSquadScript_NETWORKMSG_DESTROYOBJECTPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_FlackSquadScript_NETWORKMSG_DESTROYOBJECTPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_FlackSquadScript_NETWORKMSG_DESTROYOBJECTPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_FlackSquadScript_NETWORKMSG_DESTROYOBJECTPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_FlackSquadScript_NETWORKMSG_DESTROYOBJECTPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"NETWORKMSG_TURRETSTARTFIREPARAMS","NETWORKMSG_TURRETSTARTFIREPARAMS","",tolua_collect_NETWORKMSG_TURRETSTARTFIREPARAMS);
 #else
 tolua_cclass(tolua_S,"NETWORKMSG_TURRETSTARTFIREPARAMS","NETWORKMSG_TURRETSTARTFIREPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"NETWORKMSG_TURRETSTARTFIREPARAMS");
  tolua_variable(tolua_S,"iWeapon",tolua_get_NETWORKMSG_TURRETSTARTFIREPARAMS_unsigned_iWeapon,tolua_set_NETWORKMSG_TURRETSTARTFIREPARAMS_unsigned_iWeapon);
  tolua_function(tolua_S,"new",tolua_FlackSquadScript_NETWORKMSG_TURRETSTARTFIREPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_FlackSquadScript_NETWORKMSG_TURRETSTARTFIREPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_FlackSquadScript_NETWORKMSG_TURRETSTARTFIREPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_FlackSquadScript_NETWORKMSG_TURRETSTARTFIREPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_FlackSquadScript_NETWORKMSG_TURRETSTARTFIREPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_FlackSquadScript (lua_State* tolua_S) {
 return tolua_FlackSquadScript_open(tolua_S);
};
#endif

