/*
** Lua binding: TerrainScript
** Generated automatically by toluaxx-1.2.0 on 02/02/09 21:48:24.
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
TOLUA_API int tolua_TerrainScript_open (lua_State* tolua_S);

#include "StdAfx.h"
#include "../../ElementalEngine/EELuaScript/LuaHashString.h"
#pragma warning(disable : 4800)
#pragma warning(disable : 4345)
class CLuaDWORD { public: CLuaDWORD() {} CLuaDWORD(unsigned long i) { x = i; } operator DWORD() { return x; } void operator=(DWORD i) { x = i; } unsigned long x; };
#if 0 // To not include the "../"
#include "../../include/Terrain/TerrainDefines.h"
#endif

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_CLuaDWORD (lua_State* tolua_S)
{
 CLuaDWORD* self = (CLuaDWORD*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_TERRAINDECALPARAMS (lua_State* tolua_S)
{
 TERRAINDECALPARAMS* self = (TERRAINDECALPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Vec4");
 tolua_usertype(tolua_S,"CLuaDWORD");
 tolua_usertype(tolua_S,"Vec3");
 tolua_usertype(tolua_S,"TERRAINDECALPARAMS");
 tolua_usertype(tolua_S,"CLuaHashString");
}

/* get function:hsTextureName of class TERRAINDECALPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_TERRAINDECALPARAMS_hsTextureName_ptr
static int tolua_get_TERRAINDECALPARAMS_hsTextureName_ptr (lua_State* tolua_S) {
  TERRAINDECALPARAMS*self = (TERRAINDECALPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'hsTextureName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->hsTextureName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TERRAINDECALPARAMS_hsTextureName_ptr */

/* get function:hsTextureName of class TERRAINDECALPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_TERRAINDECALPARAMS_hsTextureName_ptr
static int tolua_set_TERRAINDECALPARAMS_hsTextureName_ptr (lua_State* tolua_S) {
  TERRAINDECALPARAMS*self = (TERRAINDECALPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'hsTextureName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->hsTextureName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TERRAINDECALPARAMS_hsTextureName_ptr */

/* get function:fSize of class TERRAINDECALPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_TERRAINDECALPARAMS_fSize
static int tolua_get_TERRAINDECALPARAMS_fSize (lua_State* tolua_S) {
  TERRAINDECALPARAMS*self = (TERRAINDECALPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'fSize'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->fSize);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TERRAINDECALPARAMS_fSize */

/* get function:fSize of class TERRAINDECALPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_TERRAINDECALPARAMS_fSize
static int tolua_set_TERRAINDECALPARAMS_fSize (lua_State* tolua_S) {
  TERRAINDECALPARAMS*self = (TERRAINDECALPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'fSize'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->fSize=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TERRAINDECALPARAMS_fSize */

/* get function:v4Color of class TERRAINDECALPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_TERRAINDECALPARAMS_v4Color_ptr
static int tolua_get_TERRAINDECALPARAMS_v4Color_ptr (lua_State* tolua_S) {
  TERRAINDECALPARAMS*self = (TERRAINDECALPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'v4Color'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->v4Color,"Vec4");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TERRAINDECALPARAMS_v4Color_ptr */

/* get function:v4Color of class TERRAINDECALPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_TERRAINDECALPARAMS_v4Color_ptr
static int tolua_set_TERRAINDECALPARAMS_v4Color_ptr (lua_State* tolua_S) {
  TERRAINDECALPARAMS*self = (TERRAINDECALPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'v4Color'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec4",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->v4Color=(( Vec4*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TERRAINDECALPARAMS_v4Color_ptr */

/* get function:fLingerTime of class TERRAINDECALPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_TERRAINDECALPARAMS_fLingerTime
static int tolua_get_TERRAINDECALPARAMS_fLingerTime (lua_State* tolua_S) {
  TERRAINDECALPARAMS*self = (TERRAINDECALPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'fLingerTime'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->fLingerTime);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TERRAINDECALPARAMS_fLingerTime */

/* get function:fLingerTime of class TERRAINDECALPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_TERRAINDECALPARAMS_fLingerTime
static int tolua_set_TERRAINDECALPARAMS_fLingerTime (lua_State* tolua_S) {
  TERRAINDECALPARAMS*self = (TERRAINDECALPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'fLingerTime'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->fLingerTime=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TERRAINDECALPARAMS_fLingerTime */

/* get function:fFadeTime of class TERRAINDECALPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_TERRAINDECALPARAMS_fFadeTime
static int tolua_get_TERRAINDECALPARAMS_fFadeTime (lua_State* tolua_S) {
  TERRAINDECALPARAMS*self = (TERRAINDECALPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'fFadeTime'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->fFadeTime);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TERRAINDECALPARAMS_fFadeTime */

/* get function:fFadeTime of class TERRAINDECALPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_TERRAINDECALPARAMS_fFadeTime
static int tolua_set_TERRAINDECALPARAMS_fFadeTime (lua_State* tolua_S) {
  TERRAINDECALPARAMS*self = (TERRAINDECALPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'fFadeTime'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->fFadeTime=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TERRAINDECALPARAMS_fFadeTime */

/* get function:vPosition of class TERRAINDECALPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_TERRAINDECALPARAMS_vPosition_ptr
static int tolua_get_TERRAINDECALPARAMS_vPosition_ptr (lua_State* tolua_S) {
  TERRAINDECALPARAMS*self = (TERRAINDECALPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vPosition'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->vPosition,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TERRAINDECALPARAMS_vPosition_ptr */

/* get function:vPosition of class TERRAINDECALPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_TERRAINDECALPARAMS_vPosition_ptr
static int tolua_set_TERRAINDECALPARAMS_vPosition_ptr (lua_State* tolua_S) {
  TERRAINDECALPARAMS*self = (TERRAINDECALPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vPosition'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->vPosition=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TERRAINDECALPARAMS_vPosition_ptr */

/* method: new of class  TERRAINDECALPARAMS */
#ifndef TOLUA_DISABLE_tolua_TerrainScript_TERRAINDECALPARAMS_new00
static int tolua_TerrainScript_TERRAINDECALPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"TERRAINDECALPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  TERRAINDECALPARAMS* tolua_ret = (TERRAINDECALPARAMS*)  new TERRAINDECALPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"TERRAINDECALPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TERRAINDECALPARAMS */
#ifndef TOLUA_DISABLE_tolua_TerrainScript_TERRAINDECALPARAMS_new00_local
static int tolua_TerrainScript_TERRAINDECALPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"TERRAINDECALPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  TERRAINDECALPARAMS* tolua_ret = (TERRAINDECALPARAMS*)  new TERRAINDECALPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TERRAINDECALPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  TERRAINDECALPARAMS */
#ifndef TOLUA_DISABLE_tolua_TerrainScript_TERRAINDECALPARAMS_delete00
static int tolua_TerrainScript_TERRAINDECALPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"TERRAINDECALPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  TERRAINDECALPARAMS* self = (TERRAINDECALPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  TERRAINDECALPARAMS */
#ifndef TOLUA_DISABLE_tolua_TerrainScript_TERRAINDECALPARAMS_SIZEOF00
static int tolua_TerrainScript_TERRAINDECALPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"TERRAINDECALPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  TERRAINDECALPARAMS* self = (TERRAINDECALPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(TERRAINDECALPARAMS);
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
TOLUA_API int tolua_TerrainScript_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"TERRAINDECALPARAMS","TERRAINDECALPARAMS","",tolua_collect_TERRAINDECALPARAMS);
 #else
 tolua_cclass(tolua_S,"TERRAINDECALPARAMS","TERRAINDECALPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"TERRAINDECALPARAMS");
  tolua_variable(tolua_S,"hsTextureName",tolua_get_TERRAINDECALPARAMS_hsTextureName_ptr,tolua_set_TERRAINDECALPARAMS_hsTextureName_ptr);
  tolua_variable(tolua_S,"fSize",tolua_get_TERRAINDECALPARAMS_fSize,tolua_set_TERRAINDECALPARAMS_fSize);
  tolua_variable(tolua_S,"v4Color",tolua_get_TERRAINDECALPARAMS_v4Color_ptr,tolua_set_TERRAINDECALPARAMS_v4Color_ptr);
  tolua_variable(tolua_S,"fLingerTime",tolua_get_TERRAINDECALPARAMS_fLingerTime,tolua_set_TERRAINDECALPARAMS_fLingerTime);
  tolua_variable(tolua_S,"fFadeTime",tolua_get_TERRAINDECALPARAMS_fFadeTime,tolua_set_TERRAINDECALPARAMS_fFadeTime);
  tolua_variable(tolua_S,"vPosition",tolua_get_TERRAINDECALPARAMS_vPosition_ptr,tolua_set_TERRAINDECALPARAMS_vPosition_ptr);
  tolua_function(tolua_S,"new",tolua_TerrainScript_TERRAINDECALPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_TerrainScript_TERRAINDECALPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_TerrainScript_TERRAINDECALPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_TerrainScript_TERRAINDECALPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_TerrainScript_TERRAINDECALPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_TerrainScript (lua_State* tolua_S) {
 return tolua_TerrainScript_open(tolua_S);
};
#endif

