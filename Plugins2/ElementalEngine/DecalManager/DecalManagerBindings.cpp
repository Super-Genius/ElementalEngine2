/*
** Lua binding: DecalScript
** Generated automatically by toluaxx-1.2.0 on 02/04/09 17:59:32.
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
TOLUA_API int tolua_DecalScript_open (lua_State* tolua_S);

#include "StdAfx.h"
#include "../../ElementalEngine/EELuaScript/LuaHashString.h"
#pragma warning(disable : 4800)
#pragma warning(disable : 4345)
class CLuaDWORD { public: CLuaDWORD() {} CLuaDWORD(unsigned long i) { x = i; } operator DWORD() { return x; } void operator=(DWORD i) { x = i; } unsigned long x; };
#if 0 // To not include the "../"
#include "../../include/DecalManager/DecalManagerDefine.h"
#endif

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_CLuaDWORD (lua_State* tolua_S)
{
 CLuaDWORD* self = (CLuaDWORD*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_ADDDECALSPRITEMESSAGE (lua_State* tolua_S)
{
 ADDDECALSPRITEMESSAGE* self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CLuaHashString");
 tolua_usertype(tolua_S,"ADDDECALSPRITEMESSAGE");
 tolua_usertype(tolua_S,"Vec3");
 tolua_usertype(tolua_S,"IEffect");
 tolua_usertype(tolua_S,"CLuaDWORD");
 tolua_usertype(tolua_S,"IBaseTextureObject");
}

/* get function:m_Size of class ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_get_ADDDECALSPRITEMESSAGE_m_Size
static int tolua_get_ADDDECALSPRITEMESSAGE_m_Size (lua_State* tolua_S) {
  ADDDECALSPRITEMESSAGE*self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Size'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_Size);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ADDDECALSPRITEMESSAGE_m_Size */

/* get function:m_Size of class ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_set_ADDDECALSPRITEMESSAGE_m_Size
static int tolua_set_ADDDECALSPRITEMESSAGE_m_Size (lua_State* tolua_S) {
  ADDDECALSPRITEMESSAGE*self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Size'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_Size=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ADDDECALSPRITEMESSAGE_m_Size */

/* get function:m_Position of class ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_get_ADDDECALSPRITEMESSAGE_m_Position_ptr
static int tolua_get_ADDDECALSPRITEMESSAGE_m_Position_ptr (lua_State* tolua_S) {
  ADDDECALSPRITEMESSAGE*self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Position'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_Position,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ADDDECALSPRITEMESSAGE_m_Position_ptr */

/* get function:m_Position of class ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_set_ADDDECALSPRITEMESSAGE_m_Position_ptr
static int tolua_set_ADDDECALSPRITEMESSAGE_m_Position_ptr (lua_State* tolua_S) {
  ADDDECALSPRITEMESSAGE*self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Position'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_Position=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ADDDECALSPRITEMESSAGE_m_Position_ptr */

/* get function:m_Normal of class ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_get_ADDDECALSPRITEMESSAGE_m_Normal_ptr
static int tolua_get_ADDDECALSPRITEMESSAGE_m_Normal_ptr (lua_State* tolua_S) {
  ADDDECALSPRITEMESSAGE*self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Normal'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_Normal,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ADDDECALSPRITEMESSAGE_m_Normal_ptr */

/* get function:m_Normal of class ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_set_ADDDECALSPRITEMESSAGE_m_Normal_ptr
static int tolua_set_ADDDECALSPRITEMESSAGE_m_Normal_ptr (lua_State* tolua_S) {
  ADDDECALSPRITEMESSAGE*self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Normal'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_Normal=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ADDDECALSPRITEMESSAGE_m_Normal_ptr */

/* get function:m_TimeLinger of class ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_get_ADDDECALSPRITEMESSAGE_m_TimeLinger
static int tolua_get_ADDDECALSPRITEMESSAGE_m_TimeLinger (lua_State* tolua_S) {
  ADDDECALSPRITEMESSAGE*self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_TimeLinger'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_TimeLinger);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ADDDECALSPRITEMESSAGE_m_TimeLinger */

/* get function:m_TimeLinger of class ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_set_ADDDECALSPRITEMESSAGE_m_TimeLinger
static int tolua_set_ADDDECALSPRITEMESSAGE_m_TimeLinger (lua_State* tolua_S) {
  ADDDECALSPRITEMESSAGE*self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_TimeLinger'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_TimeLinger=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ADDDECALSPRITEMESSAGE_m_TimeLinger */

/* get function:m_FadeTime of class ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_get_ADDDECALSPRITEMESSAGE_m_FadeTime
static int tolua_get_ADDDECALSPRITEMESSAGE_m_FadeTime (lua_State* tolua_S) {
  ADDDECALSPRITEMESSAGE*self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_FadeTime'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_FadeTime);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ADDDECALSPRITEMESSAGE_m_FadeTime */

/* get function:m_FadeTime of class ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_set_ADDDECALSPRITEMESSAGE_m_FadeTime
static int tolua_set_ADDDECALSPRITEMESSAGE_m_FadeTime (lua_State* tolua_S) {
  ADDDECALSPRITEMESSAGE*self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_FadeTime'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_FadeTime=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ADDDECALSPRITEMESSAGE_m_FadeTime */

/* get function:m_Color of class ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_get_ADDDECALSPRITEMESSAGE_m_Color
static int tolua_get_ADDDECALSPRITEMESSAGE_m_Color (lua_State* tolua_S) {
  ADDDECALSPRITEMESSAGE*self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Color'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->m_Color,"CLuaDWORD");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ADDDECALSPRITEMESSAGE_m_Color */

/* get function:m_Color of class ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_set_ADDDECALSPRITEMESSAGE_m_Color
static int tolua_set_ADDDECALSPRITEMESSAGE_m_Color (lua_State* tolua_S) {
  ADDDECALSPRITEMESSAGE*self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Color'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaDWORD",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_Color=*((  CLuaDWORD*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ADDDECALSPRITEMESSAGE_m_Color */

/* get function:m_Texture of class ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_get_ADDDECALSPRITEMESSAGE_m_Texture_ptr
static int tolua_get_ADDDECALSPRITEMESSAGE_m_Texture_ptr (lua_State* tolua_S) {
  ADDDECALSPRITEMESSAGE*self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Texture'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_Texture,"IBaseTextureObject");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ADDDECALSPRITEMESSAGE_m_Texture_ptr */

/* get function:m_Texture of class ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_set_ADDDECALSPRITEMESSAGE_m_Texture_ptr
static int tolua_set_ADDDECALSPRITEMESSAGE_m_Texture_ptr (lua_State* tolua_S) {
  ADDDECALSPRITEMESSAGE*self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Texture'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"IBaseTextureObject",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_Texture=(( IBaseTextureObject*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ADDDECALSPRITEMESSAGE_m_Texture_ptr */

/* get function:m_Effect of class ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_get_ADDDECALSPRITEMESSAGE_m_Effect_ptr
static int tolua_get_ADDDECALSPRITEMESSAGE_m_Effect_ptr (lua_State* tolua_S) {
  ADDDECALSPRITEMESSAGE*self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Effect'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_Effect,"IEffect");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ADDDECALSPRITEMESSAGE_m_Effect_ptr */

/* get function:m_Effect of class ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_set_ADDDECALSPRITEMESSAGE_m_Effect_ptr
static int tolua_set_ADDDECALSPRITEMESSAGE_m_Effect_ptr (lua_State* tolua_S) {
  ADDDECALSPRITEMESSAGE*self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Effect'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"IEffect",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_Effect=(( IEffect*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ADDDECALSPRITEMESSAGE_m_Effect_ptr */

/* get function:m_hsTextureName of class ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_get_ADDDECALSPRITEMESSAGE_m_hsTextureName_ptr
static int tolua_get_ADDDECALSPRITEMESSAGE_m_hsTextureName_ptr (lua_State* tolua_S) {
  ADDDECALSPRITEMESSAGE*self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_hsTextureName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_hsTextureName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ADDDECALSPRITEMESSAGE_m_hsTextureName_ptr */

/* get function:m_hsTextureName of class ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_set_ADDDECALSPRITEMESSAGE_m_hsTextureName_ptr
static int tolua_set_ADDDECALSPRITEMESSAGE_m_hsTextureName_ptr (lua_State* tolua_S) {
  ADDDECALSPRITEMESSAGE*self = (ADDDECALSPRITEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_hsTextureName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_hsTextureName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ADDDECALSPRITEMESSAGE_m_hsTextureName_ptr */

/* method: new of class  ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_DecalScript_ADDDECALSPRITEMESSAGE_new00
static int tolua_DecalScript_ADDDECALSPRITEMESSAGE_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"ADDDECALSPRITEMESSAGE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  ADDDECALSPRITEMESSAGE* tolua_ret = (ADDDECALSPRITEMESSAGE*)  new ADDDECALSPRITEMESSAGE();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"ADDDECALSPRITEMESSAGE");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_DecalScript_ADDDECALSPRITEMESSAGE_new00_local
static int tolua_DecalScript_ADDDECALSPRITEMESSAGE_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"ADDDECALSPRITEMESSAGE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  ADDDECALSPRITEMESSAGE* tolua_ret = (ADDDECALSPRITEMESSAGE*)  new ADDDECALSPRITEMESSAGE();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"ADDDECALSPRITEMESSAGE");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_DecalScript_ADDDECALSPRITEMESSAGE_delete00
static int tolua_DecalScript_ADDDECALSPRITEMESSAGE_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"ADDDECALSPRITEMESSAGE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  ADDDECALSPRITEMESSAGE* self = (ADDDECALSPRITEMESSAGE*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  ADDDECALSPRITEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_DecalScript_ADDDECALSPRITEMESSAGE_SIZEOF00
static int tolua_DecalScript_ADDDECALSPRITEMESSAGE_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"ADDDECALSPRITEMESSAGE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  ADDDECALSPRITEMESSAGE* self = (ADDDECALSPRITEMESSAGE*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(ADDDECALSPRITEMESSAGE);
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
TOLUA_API int tolua_DecalScript_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"ADDDECALSPRITEMESSAGE","ADDDECALSPRITEMESSAGE","",tolua_collect_ADDDECALSPRITEMESSAGE);
 #else
 tolua_cclass(tolua_S,"ADDDECALSPRITEMESSAGE","ADDDECALSPRITEMESSAGE","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"ADDDECALSPRITEMESSAGE");
  tolua_variable(tolua_S,"m_Size",tolua_get_ADDDECALSPRITEMESSAGE_m_Size,tolua_set_ADDDECALSPRITEMESSAGE_m_Size);
  tolua_variable(tolua_S,"m_Position",tolua_get_ADDDECALSPRITEMESSAGE_m_Position_ptr,tolua_set_ADDDECALSPRITEMESSAGE_m_Position_ptr);
  tolua_variable(tolua_S,"m_Normal",tolua_get_ADDDECALSPRITEMESSAGE_m_Normal_ptr,tolua_set_ADDDECALSPRITEMESSAGE_m_Normal_ptr);
  tolua_variable(tolua_S,"m_TimeLinger",tolua_get_ADDDECALSPRITEMESSAGE_m_TimeLinger,tolua_set_ADDDECALSPRITEMESSAGE_m_TimeLinger);
  tolua_variable(tolua_S,"m_FadeTime",tolua_get_ADDDECALSPRITEMESSAGE_m_FadeTime,tolua_set_ADDDECALSPRITEMESSAGE_m_FadeTime);
  tolua_variable(tolua_S,"m_Color",tolua_get_ADDDECALSPRITEMESSAGE_m_Color,tolua_set_ADDDECALSPRITEMESSAGE_m_Color);
  tolua_variable(tolua_S,"m_Texture",tolua_get_ADDDECALSPRITEMESSAGE_m_Texture_ptr,tolua_set_ADDDECALSPRITEMESSAGE_m_Texture_ptr);
  tolua_variable(tolua_S,"m_Effect",tolua_get_ADDDECALSPRITEMESSAGE_m_Effect_ptr,tolua_set_ADDDECALSPRITEMESSAGE_m_Effect_ptr);
  tolua_variable(tolua_S,"m_hsTextureName",tolua_get_ADDDECALSPRITEMESSAGE_m_hsTextureName_ptr,tolua_set_ADDDECALSPRITEMESSAGE_m_hsTextureName_ptr);
  tolua_function(tolua_S,"new",tolua_DecalScript_ADDDECALSPRITEMESSAGE_new00);
  tolua_function(tolua_S,"new_local",tolua_DecalScript_ADDDECALSPRITEMESSAGE_new00_local);
  tolua_function(tolua_S,".call",tolua_DecalScript_ADDDECALSPRITEMESSAGE_new00_local);
  tolua_function(tolua_S,"delete",tolua_DecalScript_ADDDECALSPRITEMESSAGE_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_DecalScript_ADDDECALSPRITEMESSAGE_SIZEOF00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_DecalScript (lua_State* tolua_S) {
 return tolua_DecalScript_open(tolua_S);
};
#endif

