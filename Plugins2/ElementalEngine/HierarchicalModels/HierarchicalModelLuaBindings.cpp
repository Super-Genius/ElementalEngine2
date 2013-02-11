/*
** Lua binding: HierarchicalModelsLua
** Generated automatically by toluaxx-1.2.0 on 02/02/09 21:47:49.
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
TOLUA_API int tolua_HierarchicalModelsLua_open (lua_State* tolua_S);

#include "StdAfx.h"
#include "../../ElementalEngine/EELuaScript/LuaHashString.h"
#pragma warning(disable : 4800)
#pragma warning(disable : 4345)
class CLuaDWORD { public: CLuaDWORD() {} CLuaDWORD(unsigned long i) { x = i; } operator DWORD() { return x; } void operator=(DWORD i) { x = i; } unsigned long x; };
#if 0	// dummy include with relative path for tolua .pkg file
#include "..\..\include\HierarchicalModels\HierarchicalModelsDefine.h"
#endif

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_HIERARCHICALOFFSETPARAM (lua_State* tolua_S)
{
 HIERARCHICALOFFSETPARAM* self = (HIERARCHICALOFFSETPARAM*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_HIERARCHICALPOSITIONMATRIX (lua_State* tolua_S)
{
 HIERARCHICALPOSITIONMATRIX* self = (HIERARCHICALPOSITIONMATRIX*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CLuaDWORD (lua_State* tolua_S)
{
 CLuaDWORD* self = (CLuaDWORD*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_HIERARCHICALANIMATION (lua_State* tolua_S)
{
 HIERARCHICALANIMATION* self = (HIERARCHICALANIMATION*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_HRCMATERIALOVERRIDEPARAMS (lua_State* tolua_S)
{
 HRCMATERIALOVERRIDEPARAMS* self = (HRCMATERIALOVERRIDEPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_HIERARCHICALMODELGETANIMLENGTH (lua_State* tolua_S)
{
 HIERARCHICALMODELGETANIMLENGTH* self = (HIERARCHICALMODELGETANIMLENGTH*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_HIERARCHICALMODELGETCHILDID (lua_State* tolua_S)
{
 HIERARCHICALMODELGETCHILDID* self = (HIERARCHICALMODELGETCHILDID*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CLuaHashString");
 tolua_usertype(tolua_S,"HIERARCHICALPOSITIONMATRIX");
 tolua_usertype(tolua_S,"CLuaDWORD");
 tolua_usertype(tolua_S,"HRCMATERIALOVERRIDEPARAMS");
 tolua_usertype(tolua_S,"EulerAngle");
 tolua_usertype(tolua_S,"HIERARCHICALMODELGETCHILDID");
 tolua_usertype(tolua_S,"HIERARCHICALMODELGETANIMLENGTH");
 tolua_usertype(tolua_S,"HIERARCHICALANIMATION");
 tolua_usertype(tolua_S,"HIERARCHICALOFFSETPARAM");
 tolua_usertype(tolua_S,"Matrix3x3");
 tolua_usertype(tolua_S,"MATERIALOVERRIDEPARAMS");
 tolua_usertype(tolua_S,"Vec3");
}

/* get function:m_Offset of class HIERARCHICALOFFSETPARAM */
#ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALOFFSETPARAM_m_Offset_ptr
static int tolua_get_HIERARCHICALOFFSETPARAM_m_Offset_ptr (lua_State* tolua_S) {
  HIERARCHICALOFFSETPARAM*self = (HIERARCHICALOFFSETPARAM*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Offset'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_Offset,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALOFFSETPARAM_m_Offset_ptr */

/* get function:m_Offset of class HIERARCHICALOFFSETPARAM */
#ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALOFFSETPARAM_m_Offset_ptr
static int tolua_set_HIERARCHICALOFFSETPARAM_m_Offset_ptr (lua_State* tolua_S) {
  HIERARCHICALOFFSETPARAM*self = (HIERARCHICALOFFSETPARAM*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Offset'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_Offset=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALOFFSETPARAM_m_Offset_ptr */

/* get function:m_RotationOffset of class HIERARCHICALOFFSETPARAM */
#ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALOFFSETPARAM_m_RotationOffset_ptr
static int tolua_get_HIERARCHICALOFFSETPARAM_m_RotationOffset_ptr (lua_State* tolua_S) {
  HIERARCHICALOFFSETPARAM*self = (HIERARCHICALOFFSETPARAM*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_RotationOffset'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_RotationOffset,"EulerAngle");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALOFFSETPARAM_m_RotationOffset_ptr */

/* get function:m_RotationOffset of class HIERARCHICALOFFSETPARAM */
#ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALOFFSETPARAM_m_RotationOffset_ptr
static int tolua_set_HIERARCHICALOFFSETPARAM_m_RotationOffset_ptr (lua_State* tolua_S) {
  HIERARCHICALOFFSETPARAM*self = (HIERARCHICALOFFSETPARAM*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_RotationOffset'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"EulerAngle",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_RotationOffset=(( EulerAngle*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALOFFSETPARAM_m_RotationOffset_ptr */

/* get function:m_Child of class HIERARCHICALOFFSETPARAM */
#ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALOFFSETPARAM_m_Child
static int tolua_get_HIERARCHICALOFFSETPARAM_m_Child (lua_State* tolua_S) {
  HIERARCHICALOFFSETPARAM*self = (HIERARCHICALOFFSETPARAM*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Child'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_Child);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALOFFSETPARAM_m_Child */

/* get function:m_Child of class HIERARCHICALOFFSETPARAM */
#ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALOFFSETPARAM_m_Child
static int tolua_set_HIERARCHICALOFFSETPARAM_m_Child (lua_State* tolua_S) {
  HIERARCHICALOFFSETPARAM*self = (HIERARCHICALOFFSETPARAM*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Child'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_Child=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALOFFSETPARAM_m_Child */

/* method: new of class  HIERARCHICALOFFSETPARAM */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALOFFSETPARAM_new00
static int tolua_HierarchicalModelsLua_HIERARCHICALOFFSETPARAM_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"HIERARCHICALOFFSETPARAM",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  HIERARCHICALOFFSETPARAM* tolua_ret = (HIERARCHICALOFFSETPARAM*)  new HIERARCHICALOFFSETPARAM();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"HIERARCHICALOFFSETPARAM");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  HIERARCHICALOFFSETPARAM */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALOFFSETPARAM_new00_local
static int tolua_HierarchicalModelsLua_HIERARCHICALOFFSETPARAM_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"HIERARCHICALOFFSETPARAM",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  HIERARCHICALOFFSETPARAM* tolua_ret = (HIERARCHICALOFFSETPARAM*)  new HIERARCHICALOFFSETPARAM();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"HIERARCHICALOFFSETPARAM");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  HIERARCHICALOFFSETPARAM */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALOFFSETPARAM_delete00
static int tolua_HierarchicalModelsLua_HIERARCHICALOFFSETPARAM_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"HIERARCHICALOFFSETPARAM",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  HIERARCHICALOFFSETPARAM* self = (HIERARCHICALOFFSETPARAM*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  HIERARCHICALOFFSETPARAM */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALOFFSETPARAM_SIZEOF00
static int tolua_HierarchicalModelsLua_HIERARCHICALOFFSETPARAM_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"HIERARCHICALOFFSETPARAM",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  HIERARCHICALOFFSETPARAM* self = (HIERARCHICALOFFSETPARAM*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(HIERARCHICALOFFSETPARAM);
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

/* get function:m_OutMatrix of class HIERARCHICALPOSITIONMATRIX */
#ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALPOSITIONMATRIX_m_OutMatrix_ptr
static int tolua_get_HIERARCHICALPOSITIONMATRIX_m_OutMatrix_ptr (lua_State* tolua_S) {
  HIERARCHICALPOSITIONMATRIX*self = (HIERARCHICALPOSITIONMATRIX*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_OutMatrix'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_OutMatrix,"Matrix3x3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALPOSITIONMATRIX_m_OutMatrix_ptr */

/* get function:m_OutMatrix of class HIERARCHICALPOSITIONMATRIX */
#ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALPOSITIONMATRIX_m_OutMatrix_ptr
static int tolua_set_HIERARCHICALPOSITIONMATRIX_m_OutMatrix_ptr (lua_State* tolua_S) {
  HIERARCHICALPOSITIONMATRIX*self = (HIERARCHICALPOSITIONMATRIX*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_OutMatrix'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Matrix3x3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_OutMatrix=(( Matrix3x3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALPOSITIONMATRIX_m_OutMatrix_ptr */

/* get function:m_Child of class HIERARCHICALPOSITIONMATRIX */
#ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALPOSITIONMATRIX_m_Child
static int tolua_get_HIERARCHICALPOSITIONMATRIX_m_Child (lua_State* tolua_S) {
  HIERARCHICALPOSITIONMATRIX*self = (HIERARCHICALPOSITIONMATRIX*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Child'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_Child);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALPOSITIONMATRIX_m_Child */

/* get function:m_Child of class HIERARCHICALPOSITIONMATRIX */
#ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALPOSITIONMATRIX_m_Child
static int tolua_set_HIERARCHICALPOSITIONMATRIX_m_Child (lua_State* tolua_S) {
  HIERARCHICALPOSITIONMATRIX*self = (HIERARCHICALPOSITIONMATRIX*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Child'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_Child=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALPOSITIONMATRIX_m_Child */

/* method: new of class  HIERARCHICALPOSITIONMATRIX */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALPOSITIONMATRIX_new00
static int tolua_HierarchicalModelsLua_HIERARCHICALPOSITIONMATRIX_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"HIERARCHICALPOSITIONMATRIX",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  HIERARCHICALPOSITIONMATRIX* tolua_ret = (HIERARCHICALPOSITIONMATRIX*)  new HIERARCHICALPOSITIONMATRIX();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"HIERARCHICALPOSITIONMATRIX");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  HIERARCHICALPOSITIONMATRIX */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALPOSITIONMATRIX_new00_local
static int tolua_HierarchicalModelsLua_HIERARCHICALPOSITIONMATRIX_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"HIERARCHICALPOSITIONMATRIX",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  HIERARCHICALPOSITIONMATRIX* tolua_ret = (HIERARCHICALPOSITIONMATRIX*)  new HIERARCHICALPOSITIONMATRIX();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"HIERARCHICALPOSITIONMATRIX");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  HIERARCHICALPOSITIONMATRIX */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALPOSITIONMATRIX_delete00
static int tolua_HierarchicalModelsLua_HIERARCHICALPOSITIONMATRIX_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"HIERARCHICALPOSITIONMATRIX",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  HIERARCHICALPOSITIONMATRIX* self = (HIERARCHICALPOSITIONMATRIX*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  HIERARCHICALPOSITIONMATRIX */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALPOSITIONMATRIX_SIZEOF00
static int tolua_HierarchicalModelsLua_HIERARCHICALPOSITIONMATRIX_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"HIERARCHICALPOSITIONMATRIX",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  HIERARCHICALPOSITIONMATRIX* self = (HIERARCHICALPOSITIONMATRIX*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(HIERARCHICALPOSITIONMATRIX);
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

/* get function:animtemplate of class HIERARCHICALANIMATION */
#ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALANIMATION_animtemplate_ptr
static int tolua_get_HIERARCHICALANIMATION_animtemplate_ptr (lua_State* tolua_S) {
  HIERARCHICALANIMATION*self = (HIERARCHICALANIMATION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'animtemplate'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->animtemplate,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALANIMATION_animtemplate_ptr */

/* get function:animtemplate of class HIERARCHICALANIMATION */
#ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALANIMATION_animtemplate_ptr
static int tolua_set_HIERARCHICALANIMATION_animtemplate_ptr (lua_State* tolua_S) {
  HIERARCHICALANIMATION*self = (HIERARCHICALANIMATION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'animtemplate'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->animtemplate=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALANIMATION_animtemplate_ptr */

/* get function:speed of class HIERARCHICALANIMATION */
#ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALANIMATION_speed
static int tolua_get_HIERARCHICALANIMATION_speed (lua_State* tolua_S) {
  HIERARCHICALANIMATION*self = (HIERARCHICALANIMATION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'speed'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->speed);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALANIMATION_speed */

/* get function:speed of class HIERARCHICALANIMATION */
#ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALANIMATION_speed
static int tolua_set_HIERARCHICALANIMATION_speed (lua_State* tolua_S) {
  HIERARCHICALANIMATION*self = (HIERARCHICALANIMATION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'speed'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->speed=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALANIMATION_speed */

/* get function:loop of class HIERARCHICALANIMATION */
#ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALANIMATION_loop
static int tolua_get_HIERARCHICALANIMATION_loop (lua_State* tolua_S) {
  HIERARCHICALANIMATION*self = (HIERARCHICALANIMATION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'loop'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->loop);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALANIMATION_loop */

/* get function:loop of class HIERARCHICALANIMATION */
#ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALANIMATION_loop
static int tolua_set_HIERARCHICALANIMATION_loop (lua_State* tolua_S) {
  HIERARCHICALANIMATION*self = (HIERARCHICALANIMATION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'loop'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->loop=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALANIMATION_loop */

/* get function:freezeOnLastFrame of class HIERARCHICALANIMATION */
#ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALANIMATION_freezeOnLastFrame
static int tolua_get_HIERARCHICALANIMATION_freezeOnLastFrame (lua_State* tolua_S) {
  HIERARCHICALANIMATION*self = (HIERARCHICALANIMATION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'freezeOnLastFrame'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->freezeOnLastFrame);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALANIMATION_freezeOnLastFrame */

/* get function:freezeOnLastFrame of class HIERARCHICALANIMATION */
#ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALANIMATION_freezeOnLastFrame
static int tolua_set_HIERARCHICALANIMATION_freezeOnLastFrame (lua_State* tolua_S) {
  HIERARCHICALANIMATION*self = (HIERARCHICALANIMATION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'freezeOnLastFrame'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->freezeOnLastFrame=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALANIMATION_freezeOnLastFrame */

/* method: new of class  HIERARCHICALANIMATION */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALANIMATION_new00
static int tolua_HierarchicalModelsLua_HIERARCHICALANIMATION_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"HIERARCHICALANIMATION",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  HIERARCHICALANIMATION* tolua_ret = (HIERARCHICALANIMATION*)  new HIERARCHICALANIMATION();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"HIERARCHICALANIMATION");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  HIERARCHICALANIMATION */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALANIMATION_new00_local
static int tolua_HierarchicalModelsLua_HIERARCHICALANIMATION_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"HIERARCHICALANIMATION",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  HIERARCHICALANIMATION* tolua_ret = (HIERARCHICALANIMATION*)  new HIERARCHICALANIMATION();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"HIERARCHICALANIMATION");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  HIERARCHICALANIMATION */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALANIMATION_delete00
static int tolua_HierarchicalModelsLua_HIERARCHICALANIMATION_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"HIERARCHICALANIMATION",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  HIERARCHICALANIMATION* self = (HIERARCHICALANIMATION*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  HIERARCHICALANIMATION */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALANIMATION_SIZEOF00
static int tolua_HierarchicalModelsLua_HIERARCHICALANIMATION_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"HIERARCHICALANIMATION",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  HIERARCHICALANIMATION* self = (HIERARCHICALANIMATION*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(HIERARCHICALANIMATION);
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

/* get function:m_Childname of class HIERARCHICALMODELGETCHILDID */
#ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALMODELGETCHILDID_m_Childname_ptr
static int tolua_get_HIERARCHICALMODELGETCHILDID_m_Childname_ptr (lua_State* tolua_S) {
  HIERARCHICALMODELGETCHILDID*self = (HIERARCHICALMODELGETCHILDID*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Childname'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_Childname,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALMODELGETCHILDID_m_Childname_ptr */

/* get function:m_Childname of class HIERARCHICALMODELGETCHILDID */
#ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALMODELGETCHILDID_m_Childname_ptr
static int tolua_set_HIERARCHICALMODELGETCHILDID_m_Childname_ptr (lua_State* tolua_S) {
  HIERARCHICALMODELGETCHILDID*self = (HIERARCHICALMODELGETCHILDID*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Childname'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_Childname=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALMODELGETCHILDID_m_Childname_ptr */

/* get function:m_RetID of class HIERARCHICALMODELGETCHILDID */
#ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALMODELGETCHILDID_m_RetID
static int tolua_get_HIERARCHICALMODELGETCHILDID_m_RetID (lua_State* tolua_S) {
  HIERARCHICALMODELGETCHILDID*self = (HIERARCHICALMODELGETCHILDID*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_RetID'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_RetID);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALMODELGETCHILDID_m_RetID */

/* get function:m_RetID of class HIERARCHICALMODELGETCHILDID */
#ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALMODELGETCHILDID_m_RetID
static int tolua_set_HIERARCHICALMODELGETCHILDID_m_RetID (lua_State* tolua_S) {
  HIERARCHICALMODELGETCHILDID*self = (HIERARCHICALMODELGETCHILDID*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_RetID'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_RetID=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALMODELGETCHILDID_m_RetID */

/* method: new of class  HIERARCHICALMODELGETCHILDID */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALMODELGETCHILDID_new00
static int tolua_HierarchicalModelsLua_HIERARCHICALMODELGETCHILDID_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"HIERARCHICALMODELGETCHILDID",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  HIERARCHICALMODELGETCHILDID* tolua_ret = (HIERARCHICALMODELGETCHILDID*)  new HIERARCHICALMODELGETCHILDID();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"HIERARCHICALMODELGETCHILDID");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  HIERARCHICALMODELGETCHILDID */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALMODELGETCHILDID_new00_local
static int tolua_HierarchicalModelsLua_HIERARCHICALMODELGETCHILDID_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"HIERARCHICALMODELGETCHILDID",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  HIERARCHICALMODELGETCHILDID* tolua_ret = (HIERARCHICALMODELGETCHILDID*)  new HIERARCHICALMODELGETCHILDID();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"HIERARCHICALMODELGETCHILDID");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  HIERARCHICALMODELGETCHILDID */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALMODELGETCHILDID_delete00
static int tolua_HierarchicalModelsLua_HIERARCHICALMODELGETCHILDID_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"HIERARCHICALMODELGETCHILDID",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  HIERARCHICALMODELGETCHILDID* self = (HIERARCHICALMODELGETCHILDID*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  HIERARCHICALMODELGETCHILDID */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALMODELGETCHILDID_SIZEOF00
static int tolua_HierarchicalModelsLua_HIERARCHICALMODELGETCHILDID_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"HIERARCHICALMODELGETCHILDID",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  HIERARCHICALMODELGETCHILDID* self = (HIERARCHICALMODELGETCHILDID*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(HIERARCHICALMODELGETCHILDID);
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

/* get function:length of class HIERARCHICALMODELGETANIMLENGTH */
#ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALMODELGETANIMLENGTH_length
static int tolua_get_HIERARCHICALMODELGETANIMLENGTH_length (lua_State* tolua_S) {
  HIERARCHICALMODELGETANIMLENGTH*self = (HIERARCHICALMODELGETANIMLENGTH*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'length'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->length);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_HIERARCHICALMODELGETANIMLENGTH_length */

/* get function:length of class HIERARCHICALMODELGETANIMLENGTH */
#ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALMODELGETANIMLENGTH_length
static int tolua_set_HIERARCHICALMODELGETANIMLENGTH_length (lua_State* tolua_S) {
  HIERARCHICALMODELGETANIMLENGTH*self = (HIERARCHICALMODELGETANIMLENGTH*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'length'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->length=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_HIERARCHICALMODELGETANIMLENGTH_length */

/* method: new of class  HIERARCHICALMODELGETANIMLENGTH */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALMODELGETANIMLENGTH_new00
static int tolua_HierarchicalModelsLua_HIERARCHICALMODELGETANIMLENGTH_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"HIERARCHICALMODELGETANIMLENGTH",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  HIERARCHICALMODELGETANIMLENGTH* tolua_ret = (HIERARCHICALMODELGETANIMLENGTH*)  new HIERARCHICALMODELGETANIMLENGTH();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"HIERARCHICALMODELGETANIMLENGTH");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  HIERARCHICALMODELGETANIMLENGTH */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALMODELGETANIMLENGTH_new00_local
static int tolua_HierarchicalModelsLua_HIERARCHICALMODELGETANIMLENGTH_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"HIERARCHICALMODELGETANIMLENGTH",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  HIERARCHICALMODELGETANIMLENGTH* tolua_ret = (HIERARCHICALMODELGETANIMLENGTH*)  new HIERARCHICALMODELGETANIMLENGTH();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"HIERARCHICALMODELGETANIMLENGTH");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  HIERARCHICALMODELGETANIMLENGTH */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALMODELGETANIMLENGTH_delete00
static int tolua_HierarchicalModelsLua_HIERARCHICALMODELGETANIMLENGTH_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"HIERARCHICALMODELGETANIMLENGTH",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  HIERARCHICALMODELGETANIMLENGTH* self = (HIERARCHICALMODELGETANIMLENGTH*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  HIERARCHICALMODELGETANIMLENGTH */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HIERARCHICALMODELGETANIMLENGTH_SIZEOF00
static int tolua_HierarchicalModelsLua_HIERARCHICALMODELGETANIMLENGTH_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"HIERARCHICALMODELGETANIMLENGTH",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  HIERARCHICALMODELGETANIMLENGTH* self = (HIERARCHICALMODELGETANIMLENGTH*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(HIERARCHICALMODELGETANIMLENGTH);
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

/* get function:hsNodeName of class HRCMATERIALOVERRIDEPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_HRCMATERIALOVERRIDEPARAMS_hsNodeName_ptr
static int tolua_get_HRCMATERIALOVERRIDEPARAMS_hsNodeName_ptr (lua_State* tolua_S) {
  HRCMATERIALOVERRIDEPARAMS*self = (HRCMATERIALOVERRIDEPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'hsNodeName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->hsNodeName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_HRCMATERIALOVERRIDEPARAMS_hsNodeName_ptr */

/* get function:hsNodeName of class HRCMATERIALOVERRIDEPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_HRCMATERIALOVERRIDEPARAMS_hsNodeName_ptr
static int tolua_set_HRCMATERIALOVERRIDEPARAMS_hsNodeName_ptr (lua_State* tolua_S) {
  HRCMATERIALOVERRIDEPARAMS*self = (HRCMATERIALOVERRIDEPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'hsNodeName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->hsNodeName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_HRCMATERIALOVERRIDEPARAMS_hsNodeName_ptr */

/* get function:overrideParams of class HRCMATERIALOVERRIDEPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_HRCMATERIALOVERRIDEPARAMS_overrideParams
static int tolua_get_HRCMATERIALOVERRIDEPARAMS_overrideParams (lua_State* tolua_S) {
  HRCMATERIALOVERRIDEPARAMS*self = (HRCMATERIALOVERRIDEPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'overrideParams'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->overrideParams,"MATERIALOVERRIDEPARAMS");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_HRCMATERIALOVERRIDEPARAMS_overrideParams */

/* get function:overrideParams of class HRCMATERIALOVERRIDEPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_HRCMATERIALOVERRIDEPARAMS_overrideParams
static int tolua_set_HRCMATERIALOVERRIDEPARAMS_overrideParams (lua_State* tolua_S) {
  HRCMATERIALOVERRIDEPARAMS*self = (HRCMATERIALOVERRIDEPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'overrideParams'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"MATERIALOVERRIDEPARAMS",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->overrideParams=*(( MATERIALOVERRIDEPARAMS*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_HRCMATERIALOVERRIDEPARAMS_overrideParams */

/* method: new of class  HRCMATERIALOVERRIDEPARAMS */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HRCMATERIALOVERRIDEPARAMS_new00
static int tolua_HierarchicalModelsLua_HRCMATERIALOVERRIDEPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"HRCMATERIALOVERRIDEPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  HRCMATERIALOVERRIDEPARAMS* tolua_ret = (HRCMATERIALOVERRIDEPARAMS*)  new HRCMATERIALOVERRIDEPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"HRCMATERIALOVERRIDEPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  HRCMATERIALOVERRIDEPARAMS */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HRCMATERIALOVERRIDEPARAMS_new00_local
static int tolua_HierarchicalModelsLua_HRCMATERIALOVERRIDEPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"HRCMATERIALOVERRIDEPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  HRCMATERIALOVERRIDEPARAMS* tolua_ret = (HRCMATERIALOVERRIDEPARAMS*)  new HRCMATERIALOVERRIDEPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"HRCMATERIALOVERRIDEPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  HRCMATERIALOVERRIDEPARAMS */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HRCMATERIALOVERRIDEPARAMS_delete00
static int tolua_HierarchicalModelsLua_HRCMATERIALOVERRIDEPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"HRCMATERIALOVERRIDEPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  HRCMATERIALOVERRIDEPARAMS* self = (HRCMATERIALOVERRIDEPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  HRCMATERIALOVERRIDEPARAMS */
#ifndef TOLUA_DISABLE_tolua_HierarchicalModelsLua_HRCMATERIALOVERRIDEPARAMS_SIZEOF00
static int tolua_HierarchicalModelsLua_HRCMATERIALOVERRIDEPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"HRCMATERIALOVERRIDEPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  HRCMATERIALOVERRIDEPARAMS* self = (HRCMATERIALOVERRIDEPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(HRCMATERIALOVERRIDEPARAMS);
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
TOLUA_API int tolua_HierarchicalModelsLua_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"HIERARCHICALOFFSETPARAM","HIERARCHICALOFFSETPARAM","",tolua_collect_HIERARCHICALOFFSETPARAM);
 #else
 tolua_cclass(tolua_S,"HIERARCHICALOFFSETPARAM","HIERARCHICALOFFSETPARAM","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"HIERARCHICALOFFSETPARAM");
  tolua_variable(tolua_S,"m_Offset",tolua_get_HIERARCHICALOFFSETPARAM_m_Offset_ptr,tolua_set_HIERARCHICALOFFSETPARAM_m_Offset_ptr);
  tolua_variable(tolua_S,"m_RotationOffset",tolua_get_HIERARCHICALOFFSETPARAM_m_RotationOffset_ptr,tolua_set_HIERARCHICALOFFSETPARAM_m_RotationOffset_ptr);
  tolua_variable(tolua_S,"m_Child",tolua_get_HIERARCHICALOFFSETPARAM_m_Child,tolua_set_HIERARCHICALOFFSETPARAM_m_Child);
  tolua_function(tolua_S,"new",tolua_HierarchicalModelsLua_HIERARCHICALOFFSETPARAM_new00);
  tolua_function(tolua_S,"new_local",tolua_HierarchicalModelsLua_HIERARCHICALOFFSETPARAM_new00_local);
  tolua_function(tolua_S,".call",tolua_HierarchicalModelsLua_HIERARCHICALOFFSETPARAM_new00_local);
  tolua_function(tolua_S,"delete",tolua_HierarchicalModelsLua_HIERARCHICALOFFSETPARAM_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_HierarchicalModelsLua_HIERARCHICALOFFSETPARAM_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"HIERARCHICALPOSITIONMATRIX","HIERARCHICALPOSITIONMATRIX","",tolua_collect_HIERARCHICALPOSITIONMATRIX);
 #else
 tolua_cclass(tolua_S,"HIERARCHICALPOSITIONMATRIX","HIERARCHICALPOSITIONMATRIX","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"HIERARCHICALPOSITIONMATRIX");
  tolua_variable(tolua_S,"m_OutMatrix",tolua_get_HIERARCHICALPOSITIONMATRIX_m_OutMatrix_ptr,tolua_set_HIERARCHICALPOSITIONMATRIX_m_OutMatrix_ptr);
  tolua_variable(tolua_S,"m_Child",tolua_get_HIERARCHICALPOSITIONMATRIX_m_Child,tolua_set_HIERARCHICALPOSITIONMATRIX_m_Child);
  tolua_function(tolua_S,"new",tolua_HierarchicalModelsLua_HIERARCHICALPOSITIONMATRIX_new00);
  tolua_function(tolua_S,"new_local",tolua_HierarchicalModelsLua_HIERARCHICALPOSITIONMATRIX_new00_local);
  tolua_function(tolua_S,".call",tolua_HierarchicalModelsLua_HIERARCHICALPOSITIONMATRIX_new00_local);
  tolua_function(tolua_S,"delete",tolua_HierarchicalModelsLua_HIERARCHICALPOSITIONMATRIX_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_HierarchicalModelsLua_HIERARCHICALPOSITIONMATRIX_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"HIERARCHICALANIMATION","HIERARCHICALANIMATION","",tolua_collect_HIERARCHICALANIMATION);
 #else
 tolua_cclass(tolua_S,"HIERARCHICALANIMATION","HIERARCHICALANIMATION","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"HIERARCHICALANIMATION");
  tolua_variable(tolua_S,"animtemplate",tolua_get_HIERARCHICALANIMATION_animtemplate_ptr,tolua_set_HIERARCHICALANIMATION_animtemplate_ptr);
  tolua_variable(tolua_S,"speed",tolua_get_HIERARCHICALANIMATION_speed,tolua_set_HIERARCHICALANIMATION_speed);
  tolua_variable(tolua_S,"loop",tolua_get_HIERARCHICALANIMATION_loop,tolua_set_HIERARCHICALANIMATION_loop);
  tolua_variable(tolua_S,"freezeOnLastFrame",tolua_get_HIERARCHICALANIMATION_freezeOnLastFrame,tolua_set_HIERARCHICALANIMATION_freezeOnLastFrame);
  tolua_function(tolua_S,"new",tolua_HierarchicalModelsLua_HIERARCHICALANIMATION_new00);
  tolua_function(tolua_S,"new_local",tolua_HierarchicalModelsLua_HIERARCHICALANIMATION_new00_local);
  tolua_function(tolua_S,".call",tolua_HierarchicalModelsLua_HIERARCHICALANIMATION_new00_local);
  tolua_function(tolua_S,"delete",tolua_HierarchicalModelsLua_HIERARCHICALANIMATION_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_HierarchicalModelsLua_HIERARCHICALANIMATION_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"HIERARCHICALMODELGETCHILDID","HIERARCHICALMODELGETCHILDID","",tolua_collect_HIERARCHICALMODELGETCHILDID);
 #else
 tolua_cclass(tolua_S,"HIERARCHICALMODELGETCHILDID","HIERARCHICALMODELGETCHILDID","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"HIERARCHICALMODELGETCHILDID");
  tolua_variable(tolua_S,"m_Childname",tolua_get_HIERARCHICALMODELGETCHILDID_m_Childname_ptr,tolua_set_HIERARCHICALMODELGETCHILDID_m_Childname_ptr);
  tolua_variable(tolua_S,"m_RetID",tolua_get_HIERARCHICALMODELGETCHILDID_m_RetID,tolua_set_HIERARCHICALMODELGETCHILDID_m_RetID);
  tolua_function(tolua_S,"new",tolua_HierarchicalModelsLua_HIERARCHICALMODELGETCHILDID_new00);
  tolua_function(tolua_S,"new_local",tolua_HierarchicalModelsLua_HIERARCHICALMODELGETCHILDID_new00_local);
  tolua_function(tolua_S,".call",tolua_HierarchicalModelsLua_HIERARCHICALMODELGETCHILDID_new00_local);
  tolua_function(tolua_S,"delete",tolua_HierarchicalModelsLua_HIERARCHICALMODELGETCHILDID_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_HierarchicalModelsLua_HIERARCHICALMODELGETCHILDID_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"HIERARCHICALMODELGETANIMLENGTH","HIERARCHICALMODELGETANIMLENGTH","",tolua_collect_HIERARCHICALMODELGETANIMLENGTH);
 #else
 tolua_cclass(tolua_S,"HIERARCHICALMODELGETANIMLENGTH","HIERARCHICALMODELGETANIMLENGTH","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"HIERARCHICALMODELGETANIMLENGTH");
  tolua_variable(tolua_S,"length",tolua_get_HIERARCHICALMODELGETANIMLENGTH_length,tolua_set_HIERARCHICALMODELGETANIMLENGTH_length);
  tolua_function(tolua_S,"new",tolua_HierarchicalModelsLua_HIERARCHICALMODELGETANIMLENGTH_new00);
  tolua_function(tolua_S,"new_local",tolua_HierarchicalModelsLua_HIERARCHICALMODELGETANIMLENGTH_new00_local);
  tolua_function(tolua_S,".call",tolua_HierarchicalModelsLua_HIERARCHICALMODELGETANIMLENGTH_new00_local);
  tolua_function(tolua_S,"delete",tolua_HierarchicalModelsLua_HIERARCHICALMODELGETANIMLENGTH_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_HierarchicalModelsLua_HIERARCHICALMODELGETANIMLENGTH_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"HRCMATERIALOVERRIDEPARAMS","HRCMATERIALOVERRIDEPARAMS","",tolua_collect_HRCMATERIALOVERRIDEPARAMS);
 #else
 tolua_cclass(tolua_S,"HRCMATERIALOVERRIDEPARAMS","HRCMATERIALOVERRIDEPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"HRCMATERIALOVERRIDEPARAMS");
  tolua_variable(tolua_S,"hsNodeName",tolua_get_HRCMATERIALOVERRIDEPARAMS_hsNodeName_ptr,tolua_set_HRCMATERIALOVERRIDEPARAMS_hsNodeName_ptr);
  tolua_variable(tolua_S,"overrideParams",tolua_get_HRCMATERIALOVERRIDEPARAMS_overrideParams,tolua_set_HRCMATERIALOVERRIDEPARAMS_overrideParams);
  tolua_function(tolua_S,"new",tolua_HierarchicalModelsLua_HRCMATERIALOVERRIDEPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_HierarchicalModelsLua_HRCMATERIALOVERRIDEPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_HierarchicalModelsLua_HRCMATERIALOVERRIDEPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_HierarchicalModelsLua_HRCMATERIALOVERRIDEPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_HierarchicalModelsLua_HRCMATERIALOVERRIDEPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_HierarchicalModelsLua (lua_State* tolua_S) {
 return tolua_HierarchicalModelsLua_open(tolua_S);
};
#endif

