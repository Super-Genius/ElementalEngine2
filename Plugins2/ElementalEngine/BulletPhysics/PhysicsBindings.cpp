/*
** Lua binding: BulletPhysicsScript
** Generated automatically by toluaxx-1.2.0 on 02/12/09 12:42:38.
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
TOLUA_API int tolua_BulletPhysicsScript_open (lua_State* tolua_S);

#include "StdAfx.h"
#include "../../ElementalEngine/EELuaScript/LuaHashString.h"
#pragma warning(disable : 4800)
#pragma warning(disable : 4345)
class CLuaDWORD { public: CLuaDWORD() {} CLuaDWORD(unsigned long i) { x = i; } operator DWORD() { return x; } void operator=(DWORD i) { x = i; } unsigned long x; };
#if 0 // To not include the "../"
#include "../../include/Physics/PhysicsDefines.h"
#include "cluaraycollisionlist.h"
#endif

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_OBJECTCOLLISION (lua_State* tolua_S)
{
 OBJECTCOLLISION* self = (OBJECTCOLLISION*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_ACTORFORCE (lua_State* tolua_S)
{
 ACTORFORCE* self = (ACTORFORCE*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CLuaDWORD (lua_State* tolua_S)
{
 CLuaDWORD* self = (CLuaDWORD*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CONTACTPAIR (lua_State* tolua_S)
{
 CONTACTPAIR* self = (CONTACTPAIR*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_RAYCASTLISTPARAMS (lua_State* tolua_S)
{
 RAYCASTLISTPARAMS* self = (RAYCASTLISTPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_RAYCOLLISION (lua_State* tolua_S)
{
 RAYCOLLISION* self = (RAYCOLLISION*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_RAYCASTPARAMS (lua_State* tolua_S)
{
 RAYCASTPARAMS* self = (RAYCASTPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CLuaRayCollisionList (lua_State* tolua_S)
{
 CLuaRayCollisionList* self = (CLuaRayCollisionList*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_ACTORINFO (lua_State* tolua_S)
{
 ACTORINFO* self = (ACTORINFO*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_LOCKAXIES (lua_State* tolua_S)
{
 LOCKAXIES* self = (LOCKAXIES*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_TRIGGERPARAMS (lua_State* tolua_S)
{
 TRIGGERPARAMS* self = (TRIGGERPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_COLLISIONPAIR (lua_State* tolua_S)
{
 COLLISIONPAIR* self = (COLLISIONPAIR*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CLuaHashString");
 tolua_usertype(tolua_S,"ACTORFORCE");
 tolua_usertype(tolua_S,"CLuaDWORD");
 tolua_usertype(tolua_S,"list<RAYCOLLISION>");
 tolua_usertype(tolua_S,"CONTACTPAIR");
 tolua_usertype(tolua_S,"CLuaRayCollisionList");
 tolua_usertype(tolua_S,"std::list<RAYCOLLISION>");
 tolua_usertype(tolua_S,"RAYCASTLISTPARAMS");
 tolua_usertype(tolua_S,"RAYCOLLISION");
 tolua_usertype(tolua_S,"ACTORINFO");
 tolua_usertype(tolua_S,"RAYCASTPARAMS");
 tolua_usertype(tolua_S,"Vec3");
 tolua_usertype(tolua_S,"OBJECTCOLLISION");
 tolua_usertype(tolua_S,"LOCKAXIES");
 tolua_usertype(tolua_S,"TRIGGERPARAMS");
 tolua_usertype(tolua_S,"COLLISIONPAIR");
}

/* method: new of class  LOCKAXIES */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_LOCKAXIES_new00
static int tolua_BulletPhysicsScript_LOCKAXIES_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"LOCKAXIES",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  LOCKAXIES* tolua_ret = (LOCKAXIES*)  new LOCKAXIES();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"LOCKAXIES");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  LOCKAXIES */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_LOCKAXIES_new00_local
static int tolua_BulletPhysicsScript_LOCKAXIES_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"LOCKAXIES",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  LOCKAXIES* tolua_ret = (LOCKAXIES*)  new LOCKAXIES();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"LOCKAXIES");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function:xAxis of class LOCKAXIES */
#ifndef TOLUA_DISABLE_tolua_get_LOCKAXIES_xAxis
static int tolua_get_LOCKAXIES_xAxis (lua_State* tolua_S) {
  LOCKAXIES*self = (LOCKAXIES*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xAxis'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->xAxis);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_LOCKAXIES_xAxis */

/* get function:xAxis of class LOCKAXIES */
#ifndef TOLUA_DISABLE_tolua_set_LOCKAXIES_xAxis
static int tolua_set_LOCKAXIES_xAxis (lua_State* tolua_S) {
  LOCKAXIES*self = (LOCKAXIES*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xAxis'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->xAxis=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_LOCKAXIES_xAxis */

/* get function:yAxis of class LOCKAXIES */
#ifndef TOLUA_DISABLE_tolua_get_LOCKAXIES_yAxis
static int tolua_get_LOCKAXIES_yAxis (lua_State* tolua_S) {
  LOCKAXIES*self = (LOCKAXIES*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'yAxis'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->yAxis);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_LOCKAXIES_yAxis */

/* get function:yAxis of class LOCKAXIES */
#ifndef TOLUA_DISABLE_tolua_set_LOCKAXIES_yAxis
static int tolua_set_LOCKAXIES_yAxis (lua_State* tolua_S) {
  LOCKAXIES*self = (LOCKAXIES*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'yAxis'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->yAxis=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_LOCKAXIES_yAxis */

/* get function:zAxis of class LOCKAXIES */
#ifndef TOLUA_DISABLE_tolua_get_LOCKAXIES_zAxis
static int tolua_get_LOCKAXIES_zAxis (lua_State* tolua_S) {
  LOCKAXIES*self = (LOCKAXIES*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'zAxis'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->zAxis);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_LOCKAXIES_zAxis */

/* get function:zAxis of class LOCKAXIES */
#ifndef TOLUA_DISABLE_tolua_set_LOCKAXIES_zAxis
static int tolua_set_LOCKAXIES_zAxis (lua_State* tolua_S) {
  LOCKAXIES*self = (LOCKAXIES*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'zAxis'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->zAxis=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_LOCKAXIES_zAxis */

/* method: delete of class  LOCKAXIES */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_LOCKAXIES_delete00
static int tolua_BulletPhysicsScript_LOCKAXIES_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"LOCKAXIES",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  LOCKAXIES* self = (LOCKAXIES*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  LOCKAXIES */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_LOCKAXIES_SIZEOF00
static int tolua_BulletPhysicsScript_LOCKAXIES_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"LOCKAXIES",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  LOCKAXIES* self = (LOCKAXIES*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(LOCKAXIES);
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

/* get function:groupA of class COLLISIONPAIR */
#ifndef TOLUA_DISABLE_tolua_get_COLLISIONPAIR_groupA
static int tolua_get_COLLISIONPAIR_groupA (lua_State* tolua_S) {
  COLLISIONPAIR*self = (COLLISIONPAIR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'groupA'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->groupA);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_COLLISIONPAIR_groupA */

/* get function:groupA of class COLLISIONPAIR */
#ifndef TOLUA_DISABLE_tolua_set_COLLISIONPAIR_groupA
static int tolua_set_COLLISIONPAIR_groupA (lua_State* tolua_S) {
  COLLISIONPAIR*self = (COLLISIONPAIR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'groupA'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->groupA=(( COLLISIONGROUP) (int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_COLLISIONPAIR_groupA */

/* get function:groupB of class COLLISIONPAIR */
#ifndef TOLUA_DISABLE_tolua_get_COLLISIONPAIR_groupB
static int tolua_get_COLLISIONPAIR_groupB (lua_State* tolua_S) {
  COLLISIONPAIR*self = (COLLISIONPAIR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'groupB'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->groupB);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_COLLISIONPAIR_groupB */

/* get function:groupB of class COLLISIONPAIR */
#ifndef TOLUA_DISABLE_tolua_set_COLLISIONPAIR_groupB
static int tolua_set_COLLISIONPAIR_groupB (lua_State* tolua_S) {
  COLLISIONPAIR*self = (COLLISIONPAIR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'groupB'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->groupB=(( COLLISIONGROUP) (int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_COLLISIONPAIR_groupB */

/* get function:enable of class COLLISIONPAIR */
#ifndef TOLUA_DISABLE_tolua_get_COLLISIONPAIR_enable
static int tolua_get_COLLISIONPAIR_enable (lua_State* tolua_S) {
  COLLISIONPAIR*self = (COLLISIONPAIR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'enable'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->enable);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_COLLISIONPAIR_enable */

/* get function:enable of class COLLISIONPAIR */
#ifndef TOLUA_DISABLE_tolua_set_COLLISIONPAIR_enable
static int tolua_set_COLLISIONPAIR_enable (lua_State* tolua_S) {
  COLLISIONPAIR*self = (COLLISIONPAIR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'enable'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->enable=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_COLLISIONPAIR_enable */

/* method: new of class  COLLISIONPAIR */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_COLLISIONPAIR_new00
static int tolua_BulletPhysicsScript_COLLISIONPAIR_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"COLLISIONPAIR",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  COLLISIONPAIR* tolua_ret = (COLLISIONPAIR*)  new COLLISIONPAIR();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"COLLISIONPAIR");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  COLLISIONPAIR */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_COLLISIONPAIR_new00_local
static int tolua_BulletPhysicsScript_COLLISIONPAIR_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"COLLISIONPAIR",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  COLLISIONPAIR* tolua_ret = (COLLISIONPAIR*)  new COLLISIONPAIR();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"COLLISIONPAIR");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  COLLISIONPAIR */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_COLLISIONPAIR_delete00
static int tolua_BulletPhysicsScript_COLLISIONPAIR_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"COLLISIONPAIR",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  COLLISIONPAIR* self = (COLLISIONPAIR*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  COLLISIONPAIR */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_COLLISIONPAIR_SIZEOF00
static int tolua_BulletPhysicsScript_COLLISIONPAIR_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"COLLISIONPAIR",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  COLLISIONPAIR* self = (COLLISIONPAIR*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(COLLISIONPAIR);
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

/* get function:groupA of class CONTACTPAIR */
#ifndef TOLUA_DISABLE_tolua_get_CONTACTPAIR_groupA
static int tolua_get_CONTACTPAIR_groupA (lua_State* tolua_S) {
  CONTACTPAIR*self = (CONTACTPAIR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'groupA'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->groupA);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_CONTACTPAIR_groupA */

/* get function:groupA of class CONTACTPAIR */
#ifndef TOLUA_DISABLE_tolua_set_CONTACTPAIR_groupA
static int tolua_set_CONTACTPAIR_groupA (lua_State* tolua_S) {
  CONTACTPAIR*self = (CONTACTPAIR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'groupA'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->groupA=(( COLLISIONGROUP) (int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_CONTACTPAIR_groupA */

/* get function:groupB of class CONTACTPAIR */
#ifndef TOLUA_DISABLE_tolua_get_CONTACTPAIR_groupB
static int tolua_get_CONTACTPAIR_groupB (lua_State* tolua_S) {
  CONTACTPAIR*self = (CONTACTPAIR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'groupB'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->groupB);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_CONTACTPAIR_groupB */

/* get function:groupB of class CONTACTPAIR */
#ifndef TOLUA_DISABLE_tolua_set_CONTACTPAIR_groupB
static int tolua_set_CONTACTPAIR_groupB (lua_State* tolua_S) {
  CONTACTPAIR*self = (CONTACTPAIR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'groupB'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->groupB=(( COLLISIONGROUP) (int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_CONTACTPAIR_groupB */

/* get function:flags of class CONTACTPAIR */
#ifndef TOLUA_DISABLE_tolua_get_CONTACTPAIR_flags
static int tolua_get_CONTACTPAIR_flags (lua_State* tolua_S) {
  CONTACTPAIR*self = (CONTACTPAIR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'flags'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->flags);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_CONTACTPAIR_flags */

/* get function:flags of class CONTACTPAIR */
#ifndef TOLUA_DISABLE_tolua_set_CONTACTPAIR_flags
static int tolua_set_CONTACTPAIR_flags (lua_State* tolua_S) {
  CONTACTPAIR*self = (CONTACTPAIR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'flags'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->flags=(( CONTACTFLAGS) (int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_CONTACTPAIR_flags */

/* method: new of class  CONTACTPAIR */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_CONTACTPAIR_new00
static int tolua_BulletPhysicsScript_CONTACTPAIR_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CONTACTPAIR",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  CONTACTPAIR* tolua_ret = (CONTACTPAIR*)  new CONTACTPAIR();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CONTACTPAIR");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CONTACTPAIR */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_CONTACTPAIR_new00_local
static int tolua_BulletPhysicsScript_CONTACTPAIR_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CONTACTPAIR",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  CONTACTPAIR* tolua_ret = (CONTACTPAIR*)  new CONTACTPAIR();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CONTACTPAIR");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CONTACTPAIR */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_CONTACTPAIR_delete00
static int tolua_BulletPhysicsScript_CONTACTPAIR_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CONTACTPAIR",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CONTACTPAIR* self = (CONTACTPAIR*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  CONTACTPAIR */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_CONTACTPAIR_SIZEOF00
static int tolua_BulletPhysicsScript_CONTACTPAIR_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CONTACTPAIR",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CONTACTPAIR* self = (CONTACTPAIR*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(CONTACTPAIR);
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

/* method: new of class  RAYCOLLISION */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_RAYCOLLISION_new00
static int tolua_BulletPhysicsScript_RAYCOLLISION_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"RAYCOLLISION",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  RAYCOLLISION* tolua_ret = (RAYCOLLISION*)  new RAYCOLLISION();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"RAYCOLLISION");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  RAYCOLLISION */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_RAYCOLLISION_new00_local
static int tolua_BulletPhysicsScript_RAYCOLLISION_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"RAYCOLLISION",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  RAYCOLLISION* tolua_ret = (RAYCOLLISION*)  new RAYCOLLISION();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"RAYCOLLISION");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function:distance of class RAYCOLLISION */
#ifndef TOLUA_DISABLE_tolua_get_RAYCOLLISION_distance
static int tolua_get_RAYCOLLISION_distance (lua_State* tolua_S) {
  RAYCOLLISION*self = (RAYCOLLISION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'distance'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->distance);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RAYCOLLISION_distance */

/* get function:distance of class RAYCOLLISION */
#ifndef TOLUA_DISABLE_tolua_set_RAYCOLLISION_distance
static int tolua_set_RAYCOLLISION_distance (lua_State* tolua_S) {
  RAYCOLLISION*self = (RAYCOLLISION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'distance'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->distance=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RAYCOLLISION_distance */

/* get function: hitPoint of class  RAYCOLLISION */
#ifndef TOLUA_DISABLE_tolua_get_BulletPhysicsScript_RAYCOLLISION_hitPoint
static int tolua_get_BulletPhysicsScript_RAYCOLLISION_hitPoint(lua_State* tolua_S)
{
 int tolua_index;
  RAYCOLLISION* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (RAYCOLLISION*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=3)
 tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->hitPoint[tolua_index]);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: hitPoint of class  RAYCOLLISION */
#ifndef TOLUA_DISABLE_tolua_set_BulletPhysicsScript_RAYCOLLISION_hitPoint
static int tolua_set_BulletPhysicsScript_RAYCOLLISION_hitPoint(lua_State* tolua_S)
{
 int tolua_index;
  RAYCOLLISION* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (RAYCOLLISION*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=3)
 tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 self->hitPoint[tolua_index] = ((float)  tolua_tonumber(tolua_S,3,0));
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: normal of class  RAYCOLLISION */
#ifndef TOLUA_DISABLE_tolua_get_BulletPhysicsScript_RAYCOLLISION_normal
static int tolua_get_BulletPhysicsScript_RAYCOLLISION_normal(lua_State* tolua_S)
{
 int tolua_index;
  RAYCOLLISION* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (RAYCOLLISION*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=3)
 tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->normal[tolua_index]);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: normal of class  RAYCOLLISION */
#ifndef TOLUA_DISABLE_tolua_set_BulletPhysicsScript_RAYCOLLISION_normal
static int tolua_set_BulletPhysicsScript_RAYCOLLISION_normal(lua_State* tolua_S)
{
 int tolua_index;
  RAYCOLLISION* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (RAYCOLLISION*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=3)
 tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 self->normal[tolua_index] = ((float)  tolua_tonumber(tolua_S,3,0));
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function:object of class RAYCOLLISION */
#ifndef TOLUA_DISABLE_tolua_get_RAYCOLLISION_object_ptr
static int tolua_get_RAYCOLLISION_object_ptr (lua_State* tolua_S) {
  RAYCOLLISION*self = (RAYCOLLISION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'object'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->object,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RAYCOLLISION_object_ptr */

/* get function:object of class RAYCOLLISION */
#ifndef TOLUA_DISABLE_tolua_set_RAYCOLLISION_object_ptr
static int tolua_set_RAYCOLLISION_object_ptr (lua_State* tolua_S) {
  RAYCOLLISION*self = (RAYCOLLISION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'object'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->object=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RAYCOLLISION_object_ptr */

/* get function:actor of class RAYCOLLISION */
#ifndef TOLUA_DISABLE_tolua_get_RAYCOLLISION_actor_ptr
static int tolua_get_RAYCOLLISION_actor_ptr (lua_State* tolua_S) {
  RAYCOLLISION*self = (RAYCOLLISION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'actor'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->actor,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RAYCOLLISION_actor_ptr */

/* get function:actor of class RAYCOLLISION */
#ifndef TOLUA_DISABLE_tolua_set_RAYCOLLISION_actor_ptr
static int tolua_set_RAYCOLLISION_actor_ptr (lua_State* tolua_S) {
  RAYCOLLISION*self = (RAYCOLLISION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'actor'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->actor=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RAYCOLLISION_actor_ptr */

/* get function:objectType of class RAYCOLLISION */
#ifndef TOLUA_DISABLE_tolua_get_RAYCOLLISION_objectType_ptr
static int tolua_get_RAYCOLLISION_objectType_ptr (lua_State* tolua_S) {
  RAYCOLLISION*self = (RAYCOLLISION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'objectType'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->objectType,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RAYCOLLISION_objectType_ptr */

/* get function:objectType of class RAYCOLLISION */
#ifndef TOLUA_DISABLE_tolua_set_RAYCOLLISION_objectType_ptr
static int tolua_set_RAYCOLLISION_objectType_ptr (lua_State* tolua_S) {
  RAYCOLLISION*self = (RAYCOLLISION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'objectType'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->objectType=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RAYCOLLISION_objectType_ptr */

/* get function:parentobject of class RAYCOLLISION */
#ifndef TOLUA_DISABLE_tolua_get_RAYCOLLISION_parentobject_ptr
static int tolua_get_RAYCOLLISION_parentobject_ptr (lua_State* tolua_S) {
  RAYCOLLISION*self = (RAYCOLLISION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'parentobject'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->parentobject,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RAYCOLLISION_parentobject_ptr */

/* get function:parentobject of class RAYCOLLISION */
#ifndef TOLUA_DISABLE_tolua_set_RAYCOLLISION_parentobject_ptr
static int tolua_set_RAYCOLLISION_parentobject_ptr (lua_State* tolua_S) {
  RAYCOLLISION*self = (RAYCOLLISION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'parentobject'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->parentobject=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RAYCOLLISION_parentobject_ptr */

/* method: delete of class  RAYCOLLISION */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_RAYCOLLISION_delete00
static int tolua_BulletPhysicsScript_RAYCOLLISION_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"RAYCOLLISION",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  RAYCOLLISION* self = (RAYCOLLISION*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  RAYCOLLISION */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_RAYCOLLISION_SIZEOF00
static int tolua_BulletPhysicsScript_RAYCOLLISION_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"RAYCOLLISION",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  RAYCOLLISION* self = (RAYCOLLISION*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(RAYCOLLISION);
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

/* method: new of class  RAYCASTPARAMS */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_RAYCASTPARAMS_new00
static int tolua_BulletPhysicsScript_RAYCASTPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"RAYCASTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  RAYCASTPARAMS* tolua_ret = (RAYCASTPARAMS*)  new RAYCASTPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"RAYCASTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  RAYCASTPARAMS */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_RAYCASTPARAMS_new00_local
static int tolua_BulletPhysicsScript_RAYCASTPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"RAYCASTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  RAYCASTPARAMS* tolua_ret = (RAYCASTPARAMS*)  new RAYCASTPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"RAYCASTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function:group of class RAYCASTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_RAYCASTPARAMS_group
static int tolua_get_RAYCASTPARAMS_group (lua_State* tolua_S) {
  RAYCASTPARAMS*self = (RAYCASTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'group'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->group);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RAYCASTPARAMS_group */

/* get function:group of class RAYCASTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_RAYCASTPARAMS_group
static int tolua_set_RAYCASTPARAMS_group (lua_State* tolua_S) {
  RAYCASTPARAMS*self = (RAYCASTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'group'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->group=(( COLLISIONGROUP) (int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RAYCASTPARAMS_group */

/* get function:rayPos of class RAYCASTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_RAYCASTPARAMS_rayPos_ptr
static int tolua_get_RAYCASTPARAMS_rayPos_ptr (lua_State* tolua_S) {
  RAYCASTPARAMS*self = (RAYCASTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rayPos'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->rayPos,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RAYCASTPARAMS_rayPos_ptr */

/* get function:rayPos of class RAYCASTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_RAYCASTPARAMS_rayPos_ptr
static int tolua_set_RAYCASTPARAMS_rayPos_ptr (lua_State* tolua_S) {
  RAYCASTPARAMS*self = (RAYCASTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rayPos'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->rayPos=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RAYCASTPARAMS_rayPos_ptr */

/* get function:rayDir of class RAYCASTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_RAYCASTPARAMS_rayDir_ptr
static int tolua_get_RAYCASTPARAMS_rayDir_ptr (lua_State* tolua_S) {
  RAYCASTPARAMS*self = (RAYCASTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rayDir'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->rayDir,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RAYCASTPARAMS_rayDir_ptr */

/* get function:rayDir of class RAYCASTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_RAYCASTPARAMS_rayDir_ptr
static int tolua_set_RAYCASTPARAMS_rayDir_ptr (lua_State* tolua_S) {
  RAYCASTPARAMS*self = (RAYCASTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rayDir'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->rayDir=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RAYCASTPARAMS_rayDir_ptr */

/* get function:listRayCollisions of class RAYCASTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_RAYCASTPARAMS_listRayCollisions_ptr
static int tolua_get_RAYCASTPARAMS_listRayCollisions_ptr (lua_State* tolua_S) {
  RAYCASTPARAMS*self = (RAYCASTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'listRayCollisions'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->listRayCollisions,"RAYCOLLISION");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RAYCASTPARAMS_listRayCollisions_ptr */

/* get function:listRayCollisions of class RAYCASTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_RAYCASTPARAMS_listRayCollisions_ptr
static int tolua_set_RAYCASTPARAMS_listRayCollisions_ptr (lua_State* tolua_S) {
  RAYCASTPARAMS*self = (RAYCASTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'listRayCollisions'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"RAYCOLLISION",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->listRayCollisions=(( RAYCOLLISION*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RAYCASTPARAMS_listRayCollisions_ptr */

/* method: delete of class  RAYCASTPARAMS */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_RAYCASTPARAMS_delete00
static int tolua_BulletPhysicsScript_RAYCASTPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"RAYCASTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  RAYCASTPARAMS* self = (RAYCASTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  RAYCASTPARAMS */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_RAYCASTPARAMS_SIZEOF00
static int tolua_BulletPhysicsScript_RAYCASTPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"RAYCASTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  RAYCASTPARAMS* self = (RAYCASTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(RAYCASTPARAMS);
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

/* method: new of class  RAYCASTLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_RAYCASTLISTPARAMS_new00
static int tolua_BulletPhysicsScript_RAYCASTLISTPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"RAYCASTLISTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  RAYCASTLISTPARAMS* tolua_ret = (RAYCASTLISTPARAMS*)  new RAYCASTLISTPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"RAYCASTLISTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  RAYCASTLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_RAYCASTLISTPARAMS_new00_local
static int tolua_BulletPhysicsScript_RAYCASTLISTPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"RAYCASTLISTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  RAYCASTLISTPARAMS* tolua_ret = (RAYCASTLISTPARAMS*)  new RAYCASTLISTPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"RAYCASTLISTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function:group of class RAYCASTLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_RAYCASTLISTPARAMS_group
static int tolua_get_RAYCASTLISTPARAMS_group (lua_State* tolua_S) {
  RAYCASTLISTPARAMS*self = (RAYCASTLISTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'group'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->group);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RAYCASTLISTPARAMS_group */

/* get function:group of class RAYCASTLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_RAYCASTLISTPARAMS_group
static int tolua_set_RAYCASTLISTPARAMS_group (lua_State* tolua_S) {
  RAYCASTLISTPARAMS*self = (RAYCASTLISTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'group'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->group=(( COLLISIONGROUP) (int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RAYCASTLISTPARAMS_group */

/* get function:rayPos of class RAYCASTLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_RAYCASTLISTPARAMS_rayPos_ptr
static int tolua_get_RAYCASTLISTPARAMS_rayPos_ptr (lua_State* tolua_S) {
  RAYCASTLISTPARAMS*self = (RAYCASTLISTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rayPos'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->rayPos,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RAYCASTLISTPARAMS_rayPos_ptr */

/* get function:rayPos of class RAYCASTLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_RAYCASTLISTPARAMS_rayPos_ptr
static int tolua_set_RAYCASTLISTPARAMS_rayPos_ptr (lua_State* tolua_S) {
  RAYCASTLISTPARAMS*self = (RAYCASTLISTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rayPos'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->rayPos=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RAYCASTLISTPARAMS_rayPos_ptr */

/* get function:rayDir of class RAYCASTLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_RAYCASTLISTPARAMS_rayDir_ptr
static int tolua_get_RAYCASTLISTPARAMS_rayDir_ptr (lua_State* tolua_S) {
  RAYCASTLISTPARAMS*self = (RAYCASTLISTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rayDir'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->rayDir,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RAYCASTLISTPARAMS_rayDir_ptr */

/* get function:rayDir of class RAYCASTLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_RAYCASTLISTPARAMS_rayDir_ptr
static int tolua_set_RAYCASTLISTPARAMS_rayDir_ptr (lua_State* tolua_S) {
  RAYCASTLISTPARAMS*self = (RAYCASTLISTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rayDir'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->rayDir=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RAYCASTLISTPARAMS_rayDir_ptr */

/* get function:listRayCollisions of class RAYCASTLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_RAYCASTLISTPARAMS_listRayCollisions_ptr
static int tolua_get_RAYCASTLISTPARAMS_listRayCollisions_ptr (lua_State* tolua_S) {
  RAYCASTLISTPARAMS*self = (RAYCASTLISTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'listRayCollisions'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->listRayCollisions,"std::list<RAYCOLLISION>");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RAYCASTLISTPARAMS_listRayCollisions_ptr */

/* get function:listRayCollisions of class RAYCASTLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_RAYCASTLISTPARAMS_listRayCollisions_ptr
static int tolua_set_RAYCASTLISTPARAMS_listRayCollisions_ptr (lua_State* tolua_S) {
  RAYCASTLISTPARAMS*self = (RAYCASTLISTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'listRayCollisions'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"std::list<RAYCOLLISION>",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->listRayCollisions=((  std::list<RAYCOLLISION>*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RAYCASTLISTPARAMS_listRayCollisions_ptr */

/* method: delete of class  RAYCASTLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_RAYCASTLISTPARAMS_delete00
static int tolua_BulletPhysicsScript_RAYCASTLISTPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"RAYCASTLISTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  RAYCASTLISTPARAMS* self = (RAYCASTLISTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  RAYCASTLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_RAYCASTLISTPARAMS_SIZEOF00
static int tolua_BulletPhysicsScript_RAYCASTLISTPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"RAYCASTLISTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  RAYCASTLISTPARAMS* self = (RAYCASTLISTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(RAYCASTLISTPARAMS);
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

/* method: new of class  OBJECTCOLLISION */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_OBJECTCOLLISION_new00
static int tolua_BulletPhysicsScript_OBJECTCOLLISION_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"OBJECTCOLLISION",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  OBJECTCOLLISION* tolua_ret = (OBJECTCOLLISION*)  new OBJECTCOLLISION();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"OBJECTCOLLISION");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  OBJECTCOLLISION */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_OBJECTCOLLISION_new00_local
static int tolua_BulletPhysicsScript_OBJECTCOLLISION_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"OBJECTCOLLISION",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  OBJECTCOLLISION* tolua_ret = (OBJECTCOLLISION*)  new OBJECTCOLLISION();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"OBJECTCOLLISION");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  OBJECTCOLLISION */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_OBJECTCOLLISION_delete00
static int tolua_BulletPhysicsScript_OBJECTCOLLISION_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OBJECTCOLLISION",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  OBJECTCOLLISION* self = (OBJECTCOLLISION*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  OBJECTCOLLISION */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_OBJECTCOLLISION_SIZEOF00
static int tolua_BulletPhysicsScript_OBJECTCOLLISION_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OBJECTCOLLISION",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  OBJECTCOLLISION* self = (OBJECTCOLLISION*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(OBJECTCOLLISION);
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

/* method: new of class  ACTORINFO */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_ACTORINFO_new00
static int tolua_BulletPhysicsScript_ACTORINFO_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"ACTORINFO",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  ACTORINFO* tolua_ret = (ACTORINFO*)  new ACTORINFO();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"ACTORINFO");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  ACTORINFO */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_ACTORINFO_new00_local
static int tolua_BulletPhysicsScript_ACTORINFO_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"ACTORINFO",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  ACTORINFO* tolua_ret = (ACTORINFO*)  new ACTORINFO();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"ACTORINFO");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function:name of class ACTORINFO */
#ifndef TOLUA_DISABLE_tolua_get_ACTORINFO_name_ptr
static int tolua_get_ACTORINFO_name_ptr (lua_State* tolua_S) {
  ACTORINFO*self = (ACTORINFO*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->name,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ACTORINFO_name_ptr */

/* get function:name of class ACTORINFO */
#ifndef TOLUA_DISABLE_tolua_set_ACTORINFO_name_ptr
static int tolua_set_ACTORINFO_name_ptr (lua_State* tolua_S) {
  ACTORINFO*self = (ACTORINFO*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->name=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ACTORINFO_name_ptr */

/* get function:pos of class ACTORINFO */
#ifndef TOLUA_DISABLE_tolua_get_ACTORINFO_pos_ptr
static int tolua_get_ACTORINFO_pos_ptr (lua_State* tolua_S) {
  ACTORINFO*self = (ACTORINFO*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pos'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->pos,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ACTORINFO_pos_ptr */

/* get function:pos of class ACTORINFO */
#ifndef TOLUA_DISABLE_tolua_set_ACTORINFO_pos_ptr
static int tolua_set_ACTORINFO_pos_ptr (lua_State* tolua_S) {
  ACTORINFO*self = (ACTORINFO*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pos'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->pos=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ACTORINFO_pos_ptr */

/* get function:rot of class ACTORINFO */
#ifndef TOLUA_DISABLE_tolua_get_ACTORINFO_rot_ptr
static int tolua_get_ACTORINFO_rot_ptr (lua_State* tolua_S) {
  ACTORINFO*self = (ACTORINFO*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rot'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->rot,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ACTORINFO_rot_ptr */

/* get function:rot of class ACTORINFO */
#ifndef TOLUA_DISABLE_tolua_set_ACTORINFO_rot_ptr
static int tolua_set_ACTORINFO_rot_ptr (lua_State* tolua_S) {
  ACTORINFO*self = (ACTORINFO*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rot'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->rot=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ACTORINFO_rot_ptr */

/* method: delete of class  ACTORINFO */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_ACTORINFO_delete00
static int tolua_BulletPhysicsScript_ACTORINFO_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"ACTORINFO",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  ACTORINFO* self = (ACTORINFO*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  ACTORINFO */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_ACTORINFO_SIZEOF00
static int tolua_BulletPhysicsScript_ACTORINFO_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"ACTORINFO",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  ACTORINFO* self = (ACTORINFO*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(ACTORINFO);
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

/* method: new of class  ACTORFORCE */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_ACTORFORCE_new00
static int tolua_BulletPhysicsScript_ACTORFORCE_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"ACTORFORCE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  ACTORFORCE* tolua_ret = (ACTORFORCE*)  new ACTORFORCE();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"ACTORFORCE");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  ACTORFORCE */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_ACTORFORCE_new00_local
static int tolua_BulletPhysicsScript_ACTORFORCE_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"ACTORFORCE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  ACTORFORCE* tolua_ret = (ACTORFORCE*)  new ACTORFORCE();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"ACTORFORCE");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function:name of class ACTORFORCE */
#ifndef TOLUA_DISABLE_tolua_get_ACTORFORCE_name_ptr
static int tolua_get_ACTORFORCE_name_ptr (lua_State* tolua_S) {
  ACTORFORCE*self = (ACTORFORCE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->name,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ACTORFORCE_name_ptr */

/* get function:name of class ACTORFORCE */
#ifndef TOLUA_DISABLE_tolua_set_ACTORFORCE_name_ptr
static int tolua_set_ACTORFORCE_name_ptr (lua_State* tolua_S) {
  ACTORFORCE*self = (ACTORFORCE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->name=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ACTORFORCE_name_ptr */

/* get function:force of class ACTORFORCE */
#ifndef TOLUA_DISABLE_tolua_get_ACTORFORCE_force_ptr
static int tolua_get_ACTORFORCE_force_ptr (lua_State* tolua_S) {
  ACTORFORCE*self = (ACTORFORCE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'force'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->force,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ACTORFORCE_force_ptr */

/* get function:force of class ACTORFORCE */
#ifndef TOLUA_DISABLE_tolua_set_ACTORFORCE_force_ptr
static int tolua_set_ACTORFORCE_force_ptr (lua_State* tolua_S) {
  ACTORFORCE*self = (ACTORFORCE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'force'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->force=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ACTORFORCE_force_ptr */

/* method: delete of class  ACTORFORCE */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_ACTORFORCE_delete00
static int tolua_BulletPhysicsScript_ACTORFORCE_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"ACTORFORCE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  ACTORFORCE* self = (ACTORFORCE*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  ACTORFORCE */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_ACTORFORCE_SIZEOF00
static int tolua_BulletPhysicsScript_ACTORFORCE_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"ACTORFORCE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  ACTORFORCE* self = (ACTORFORCE*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(ACTORFORCE);
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

/* method: new of class  TRIGGERPARAMS */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_TRIGGERPARAMS_new00
static int tolua_BulletPhysicsScript_TRIGGERPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"TRIGGERPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  TRIGGERPARAMS* tolua_ret = (TRIGGERPARAMS*)  new TRIGGERPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"TRIGGERPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TRIGGERPARAMS */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_TRIGGERPARAMS_new00_local
static int tolua_BulletPhysicsScript_TRIGGERPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"TRIGGERPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  TRIGGERPARAMS* tolua_ret = (TRIGGERPARAMS*)  new TRIGGERPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TRIGGERPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function:triggerOnEnter of class TRIGGERPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_TRIGGERPARAMS_triggerOnEnter
static int tolua_get_TRIGGERPARAMS_triggerOnEnter (lua_State* tolua_S) {
  TRIGGERPARAMS*self = (TRIGGERPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'triggerOnEnter'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->triggerOnEnter);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TRIGGERPARAMS_triggerOnEnter */

/* get function:triggerOnEnter of class TRIGGERPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_TRIGGERPARAMS_triggerOnEnter
static int tolua_set_TRIGGERPARAMS_triggerOnEnter (lua_State* tolua_S) {
  TRIGGERPARAMS*self = (TRIGGERPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'triggerOnEnter'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->triggerOnEnter=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TRIGGERPARAMS_triggerOnEnter */

/* get function:triggerOnStay of class TRIGGERPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_TRIGGERPARAMS_triggerOnStay
static int tolua_get_TRIGGERPARAMS_triggerOnStay (lua_State* tolua_S) {
  TRIGGERPARAMS*self = (TRIGGERPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'triggerOnStay'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->triggerOnStay);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TRIGGERPARAMS_triggerOnStay */

/* get function:triggerOnStay of class TRIGGERPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_TRIGGERPARAMS_triggerOnStay
static int tolua_set_TRIGGERPARAMS_triggerOnStay (lua_State* tolua_S) {
  TRIGGERPARAMS*self = (TRIGGERPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'triggerOnStay'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->triggerOnStay=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TRIGGERPARAMS_triggerOnStay */

/* get function:triggerOnExit of class TRIGGERPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_TRIGGERPARAMS_triggerOnExit
static int tolua_get_TRIGGERPARAMS_triggerOnExit (lua_State* tolua_S) {
  TRIGGERPARAMS*self = (TRIGGERPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'triggerOnExit'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->triggerOnExit);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TRIGGERPARAMS_triggerOnExit */

/* get function:triggerOnExit of class TRIGGERPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_TRIGGERPARAMS_triggerOnExit
static int tolua_set_TRIGGERPARAMS_triggerOnExit (lua_State* tolua_S) {
  TRIGGERPARAMS*self = (TRIGGERPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'triggerOnExit'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->triggerOnExit=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TRIGGERPARAMS_triggerOnExit */

/* method: delete of class  TRIGGERPARAMS */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_TRIGGERPARAMS_delete00
static int tolua_BulletPhysicsScript_TRIGGERPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"TRIGGERPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  TRIGGERPARAMS* self = (TRIGGERPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  TRIGGERPARAMS */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_TRIGGERPARAMS_SIZEOF00
static int tolua_BulletPhysicsScript_TRIGGERPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"TRIGGERPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  TRIGGERPARAMS* self = (TRIGGERPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(TRIGGERPARAMS);
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

/* method: new of class  CLuaRayCollisionList */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_CLuaRayCollisionList_new00
static int tolua_BulletPhysicsScript_CLuaRayCollisionList_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CLuaRayCollisionList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  CLuaRayCollisionList* tolua_ret = (CLuaRayCollisionList*)  new CLuaRayCollisionList();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CLuaRayCollisionList");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CLuaRayCollisionList */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_CLuaRayCollisionList_new00_local
static int tolua_BulletPhysicsScript_CLuaRayCollisionList_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CLuaRayCollisionList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  CLuaRayCollisionList* tolua_ret = (CLuaRayCollisionList*)  new CLuaRayCollisionList();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CLuaRayCollisionList");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CLuaRayCollisionList */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_CLuaRayCollisionList_delete00
static int tolua_BulletPhysicsScript_CLuaRayCollisionList_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaRayCollisionList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaRayCollisionList* self = (CLuaRayCollisionList*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: InsertAtEnd of class  CLuaRayCollisionList */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_CLuaRayCollisionList_InsertAtEnd00
static int tolua_BulletPhysicsScript_CLuaRayCollisionList_InsertAtEnd00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaRayCollisionList",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"RAYCOLLISION",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaRayCollisionList* self = (CLuaRayCollisionList*)  tolua_tousertype(tolua_S,1,0);
  RAYCOLLISION collisionParam = *((RAYCOLLISION*)  tolua_tousertype(tolua_S,2,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'InsertAtEnd'",NULL);
 {
  self->InsertAtEnd(collisionParam);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'InsertAtEnd'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: InsertAtBegin of class  CLuaRayCollisionList */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_CLuaRayCollisionList_InsertAtBegin00
static int tolua_BulletPhysicsScript_CLuaRayCollisionList_InsertAtBegin00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaRayCollisionList",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"RAYCOLLISION",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaRayCollisionList* self = (CLuaRayCollisionList*)  tolua_tousertype(tolua_S,1,0);
  RAYCOLLISION collisionParam = *((RAYCOLLISION*)  tolua_tousertype(tolua_S,2,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'InsertAtBegin'",NULL);
 {
  self->InsertAtBegin(collisionParam);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'InsertAtBegin'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSize of class  CLuaRayCollisionList */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_CLuaRayCollisionList_GetSize00
static int tolua_BulletPhysicsScript_CLuaRayCollisionList_GetSize00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaRayCollisionList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaRayCollisionList* self = (CLuaRayCollisionList*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSize'",NULL);
 {
  int tolua_ret = (int)  self->GetSize();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSize'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFirst of class  CLuaRayCollisionList */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_CLuaRayCollisionList_GetFirst00
static int tolua_BulletPhysicsScript_CLuaRayCollisionList_GetFirst00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaRayCollisionList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaRayCollisionList* self = (CLuaRayCollisionList*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFirst'",NULL);
 {
  RAYCOLLISION* tolua_ret = (RAYCOLLISION*)  self->GetFirst();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"RAYCOLLISION");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFirst'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNext of class  CLuaRayCollisionList */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_CLuaRayCollisionList_GetNext00
static int tolua_BulletPhysicsScript_CLuaRayCollisionList_GetNext00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaRayCollisionList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaRayCollisionList* self = (CLuaRayCollisionList*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNext'",NULL);
 {
  RAYCOLLISION* tolua_ret = (RAYCOLLISION*)  self->GetNext();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"RAYCOLLISION");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNext'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  CLuaRayCollisionList */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_CLuaRayCollisionList_Clear00
static int tolua_BulletPhysicsScript_CLuaRayCollisionList_Clear00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaRayCollisionList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaRayCollisionList* self = (CLuaRayCollisionList*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Clear'",NULL);
 {
  self->Clear();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Clear'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  CLuaRayCollisionList */
#ifndef TOLUA_DISABLE_tolua_BulletPhysicsScript_CLuaRayCollisionList_SIZEOF00
static int tolua_BulletPhysicsScript_CLuaRayCollisionList_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaRayCollisionList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaRayCollisionList* self = (CLuaRayCollisionList*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(CLuaRayCollisionList);
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
TOLUA_API int tolua_BulletPhysicsScript_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_constant(tolua_S,"X",X);
 tolua_constant(tolua_S,"Y",Y);
 tolua_constant(tolua_S,"Z",Z);
 tolua_constant(tolua_S,"ALLAXIES",ALLAXIES);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"LOCKAXIES","LOCKAXIES","",tolua_collect_LOCKAXIES);
 #else
 tolua_cclass(tolua_S,"LOCKAXIES","LOCKAXIES","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"LOCKAXIES");
  tolua_function(tolua_S,"new",tolua_BulletPhysicsScript_LOCKAXIES_new00);
  tolua_function(tolua_S,"new_local",tolua_BulletPhysicsScript_LOCKAXIES_new00_local);
  tolua_function(tolua_S,".call",tolua_BulletPhysicsScript_LOCKAXIES_new00_local);
  tolua_variable(tolua_S,"xAxis",tolua_get_LOCKAXIES_xAxis,tolua_set_LOCKAXIES_xAxis);
  tolua_variable(tolua_S,"yAxis",tolua_get_LOCKAXIES_yAxis,tolua_set_LOCKAXIES_yAxis);
  tolua_variable(tolua_S,"zAxis",tolua_get_LOCKAXIES_zAxis,tolua_set_LOCKAXIES_zAxis);
  tolua_function(tolua_S,"delete",tolua_BulletPhysicsScript_LOCKAXIES_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_BulletPhysicsScript_LOCKAXIES_SIZEOF00);
 tolua_endmodule(tolua_S);
 tolua_constant(tolua_S,"ALL",ALL);
 tolua_constant(tolua_S,"OBJECTS",OBJECTS);
 tolua_constant(tolua_S,"TERRAIN",TERRAIN);
 tolua_constant(tolua_S,"CONTROL",CONTROL);
 tolua_constant(tolua_S,"NOCOLLISION",NOCOLLISION);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"COLLISIONPAIR","COLLISIONPAIR","",tolua_collect_COLLISIONPAIR);
 #else
 tolua_cclass(tolua_S,"COLLISIONPAIR","COLLISIONPAIR","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"COLLISIONPAIR");
  tolua_variable(tolua_S,"groupA",tolua_get_COLLISIONPAIR_groupA,tolua_set_COLLISIONPAIR_groupA);
  tolua_variable(tolua_S,"groupB",tolua_get_COLLISIONPAIR_groupB,tolua_set_COLLISIONPAIR_groupB);
  tolua_variable(tolua_S,"enable",tolua_get_COLLISIONPAIR_enable,tolua_set_COLLISIONPAIR_enable);
  tolua_function(tolua_S,"new",tolua_BulletPhysicsScript_COLLISIONPAIR_new00);
  tolua_function(tolua_S,"new_local",tolua_BulletPhysicsScript_COLLISIONPAIR_new00_local);
  tolua_function(tolua_S,".call",tolua_BulletPhysicsScript_COLLISIONPAIR_new00_local);
  tolua_function(tolua_S,"delete",tolua_BulletPhysicsScript_COLLISIONPAIR_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_BulletPhysicsScript_COLLISIONPAIR_SIZEOF00);
 tolua_endmodule(tolua_S);
 tolua_constant(tolua_S,"IGNORE_PAIR",IGNORE_PAIR);
 tolua_constant(tolua_S,"ON_START_TOUCH",ON_START_TOUCH);
 tolua_constant(tolua_S,"ON_END_TOUCH",ON_END_TOUCH);
 tolua_constant(tolua_S,"ON_TOUCH",ON_TOUCH);
 tolua_constant(tolua_S,"ON_IMPACT",ON_IMPACT);
 tolua_constant(tolua_S,"ON_ROLL",ON_ROLL);
 tolua_constant(tolua_S,"ON_SLIDE",ON_SLIDE);
 tolua_constant(tolua_S,"NOTIFY_FORCES",NOTIFY_FORCES);
 tolua_constant(tolua_S,"NOTIFY_ALL",NOTIFY_ALL);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"CONTACTPAIR","CONTACTPAIR","",tolua_collect_CONTACTPAIR);
 #else
 tolua_cclass(tolua_S,"CONTACTPAIR","CONTACTPAIR","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"CONTACTPAIR");
  tolua_variable(tolua_S,"groupA",tolua_get_CONTACTPAIR_groupA,tolua_set_CONTACTPAIR_groupA);
  tolua_variable(tolua_S,"groupB",tolua_get_CONTACTPAIR_groupB,tolua_set_CONTACTPAIR_groupB);
  tolua_variable(tolua_S,"flags",tolua_get_CONTACTPAIR_flags,tolua_set_CONTACTPAIR_flags);
  tolua_function(tolua_S,"new",tolua_BulletPhysicsScript_CONTACTPAIR_new00);
  tolua_function(tolua_S,"new_local",tolua_BulletPhysicsScript_CONTACTPAIR_new00_local);
  tolua_function(tolua_S,".call",tolua_BulletPhysicsScript_CONTACTPAIR_new00_local);
  tolua_function(tolua_S,"delete",tolua_BulletPhysicsScript_CONTACTPAIR_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_BulletPhysicsScript_CONTACTPAIR_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"RAYCOLLISION","RAYCOLLISION","",tolua_collect_RAYCOLLISION);
 #else
 tolua_cclass(tolua_S,"RAYCOLLISION","RAYCOLLISION","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"RAYCOLLISION");
  tolua_function(tolua_S,"new",tolua_BulletPhysicsScript_RAYCOLLISION_new00);
  tolua_function(tolua_S,"new_local",tolua_BulletPhysicsScript_RAYCOLLISION_new00_local);
  tolua_function(tolua_S,".call",tolua_BulletPhysicsScript_RAYCOLLISION_new00_local);
  tolua_variable(tolua_S,"distance",tolua_get_RAYCOLLISION_distance,tolua_set_RAYCOLLISION_distance);
  tolua_array(tolua_S,"hitPoint",tolua_get_BulletPhysicsScript_RAYCOLLISION_hitPoint,tolua_set_BulletPhysicsScript_RAYCOLLISION_hitPoint);
  tolua_array(tolua_S,"normal",tolua_get_BulletPhysicsScript_RAYCOLLISION_normal,tolua_set_BulletPhysicsScript_RAYCOLLISION_normal);
  tolua_variable(tolua_S,"object",tolua_get_RAYCOLLISION_object_ptr,tolua_set_RAYCOLLISION_object_ptr);
  tolua_variable(tolua_S,"actor",tolua_get_RAYCOLLISION_actor_ptr,tolua_set_RAYCOLLISION_actor_ptr);
  tolua_variable(tolua_S,"objectType",tolua_get_RAYCOLLISION_objectType_ptr,tolua_set_RAYCOLLISION_objectType_ptr);
  tolua_variable(tolua_S,"parentobject",tolua_get_RAYCOLLISION_parentobject_ptr,tolua_set_RAYCOLLISION_parentobject_ptr);
  tolua_function(tolua_S,"delete",tolua_BulletPhysicsScript_RAYCOLLISION_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_BulletPhysicsScript_RAYCOLLISION_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"RAYCASTPARAMS","RAYCASTPARAMS","",tolua_collect_RAYCASTPARAMS);
 #else
 tolua_cclass(tolua_S,"RAYCASTPARAMS","RAYCASTPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"RAYCASTPARAMS");
  tolua_function(tolua_S,"new",tolua_BulletPhysicsScript_RAYCASTPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_BulletPhysicsScript_RAYCASTPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_BulletPhysicsScript_RAYCASTPARAMS_new00_local);
  tolua_variable(tolua_S,"group",tolua_get_RAYCASTPARAMS_group,tolua_set_RAYCASTPARAMS_group);
  tolua_variable(tolua_S,"rayPos",tolua_get_RAYCASTPARAMS_rayPos_ptr,tolua_set_RAYCASTPARAMS_rayPos_ptr);
  tolua_variable(tolua_S,"rayDir",tolua_get_RAYCASTPARAMS_rayDir_ptr,tolua_set_RAYCASTPARAMS_rayDir_ptr);
  tolua_variable(tolua_S,"listRayCollisions",tolua_get_RAYCASTPARAMS_listRayCollisions_ptr,tolua_set_RAYCASTPARAMS_listRayCollisions_ptr);
  tolua_function(tolua_S,"delete",tolua_BulletPhysicsScript_RAYCASTPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_BulletPhysicsScript_RAYCASTPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"RAYCASTLISTPARAMS","RAYCASTLISTPARAMS","",tolua_collect_RAYCASTLISTPARAMS);
 #else
 tolua_cclass(tolua_S,"RAYCASTLISTPARAMS","RAYCASTLISTPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"RAYCASTLISTPARAMS");
  tolua_function(tolua_S,"new",tolua_BulletPhysicsScript_RAYCASTLISTPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_BulletPhysicsScript_RAYCASTLISTPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_BulletPhysicsScript_RAYCASTLISTPARAMS_new00_local);
  tolua_variable(tolua_S,"group",tolua_get_RAYCASTLISTPARAMS_group,tolua_set_RAYCASTLISTPARAMS_group);
  tolua_variable(tolua_S,"rayPos",tolua_get_RAYCASTLISTPARAMS_rayPos_ptr,tolua_set_RAYCASTLISTPARAMS_rayPos_ptr);
  tolua_variable(tolua_S,"rayDir",tolua_get_RAYCASTLISTPARAMS_rayDir_ptr,tolua_set_RAYCASTLISTPARAMS_rayDir_ptr);
  tolua_variable(tolua_S,"listRayCollisions",tolua_get_RAYCASTLISTPARAMS_listRayCollisions_ptr,tolua_set_RAYCASTLISTPARAMS_listRayCollisions_ptr);
  tolua_function(tolua_S,"delete",tolua_BulletPhysicsScript_RAYCASTLISTPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_BulletPhysicsScript_RAYCASTLISTPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"OBJECTCOLLISION","OBJECTCOLLISION","",tolua_collect_OBJECTCOLLISION);
 #else
 tolua_cclass(tolua_S,"OBJECTCOLLISION","OBJECTCOLLISION","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"OBJECTCOLLISION");
  tolua_function(tolua_S,"new",tolua_BulletPhysicsScript_OBJECTCOLLISION_new00);
  tolua_function(tolua_S,"new_local",tolua_BulletPhysicsScript_OBJECTCOLLISION_new00_local);
  tolua_function(tolua_S,".call",tolua_BulletPhysicsScript_OBJECTCOLLISION_new00_local);
  tolua_function(tolua_S,"delete",tolua_BulletPhysicsScript_OBJECTCOLLISION_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_BulletPhysicsScript_OBJECTCOLLISION_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"ACTORINFO","ACTORINFO","",tolua_collect_ACTORINFO);
 #else
 tolua_cclass(tolua_S,"ACTORINFO","ACTORINFO","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"ACTORINFO");
  tolua_function(tolua_S,"new",tolua_BulletPhysicsScript_ACTORINFO_new00);
  tolua_function(tolua_S,"new_local",tolua_BulletPhysicsScript_ACTORINFO_new00_local);
  tolua_function(tolua_S,".call",tolua_BulletPhysicsScript_ACTORINFO_new00_local);
  tolua_variable(tolua_S,"name",tolua_get_ACTORINFO_name_ptr,tolua_set_ACTORINFO_name_ptr);
  tolua_variable(tolua_S,"pos",tolua_get_ACTORINFO_pos_ptr,tolua_set_ACTORINFO_pos_ptr);
  tolua_variable(tolua_S,"rot",tolua_get_ACTORINFO_rot_ptr,tolua_set_ACTORINFO_rot_ptr);
  tolua_function(tolua_S,"delete",tolua_BulletPhysicsScript_ACTORINFO_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_BulletPhysicsScript_ACTORINFO_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"ACTORFORCE","ACTORFORCE","",tolua_collect_ACTORFORCE);
 #else
 tolua_cclass(tolua_S,"ACTORFORCE","ACTORFORCE","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"ACTORFORCE");
  tolua_function(tolua_S,"new",tolua_BulletPhysicsScript_ACTORFORCE_new00);
  tolua_function(tolua_S,"new_local",tolua_BulletPhysicsScript_ACTORFORCE_new00_local);
  tolua_function(tolua_S,".call",tolua_BulletPhysicsScript_ACTORFORCE_new00_local);
  tolua_variable(tolua_S,"name",tolua_get_ACTORFORCE_name_ptr,tolua_set_ACTORFORCE_name_ptr);
  tolua_variable(tolua_S,"force",tolua_get_ACTORFORCE_force_ptr,tolua_set_ACTORFORCE_force_ptr);
  tolua_function(tolua_S,"delete",tolua_BulletPhysicsScript_ACTORFORCE_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_BulletPhysicsScript_ACTORFORCE_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"TRIGGERPARAMS","TRIGGERPARAMS","",tolua_collect_TRIGGERPARAMS);
 #else
 tolua_cclass(tolua_S,"TRIGGERPARAMS","TRIGGERPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"TRIGGERPARAMS");
  tolua_function(tolua_S,"new",tolua_BulletPhysicsScript_TRIGGERPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_BulletPhysicsScript_TRIGGERPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_BulletPhysicsScript_TRIGGERPARAMS_new00_local);
  tolua_variable(tolua_S,"triggerOnEnter",tolua_get_TRIGGERPARAMS_triggerOnEnter,tolua_set_TRIGGERPARAMS_triggerOnEnter);
  tolua_variable(tolua_S,"triggerOnStay",tolua_get_TRIGGERPARAMS_triggerOnStay,tolua_set_TRIGGERPARAMS_triggerOnStay);
  tolua_variable(tolua_S,"triggerOnExit",tolua_get_TRIGGERPARAMS_triggerOnExit,tolua_set_TRIGGERPARAMS_triggerOnExit);
  tolua_function(tolua_S,"delete",tolua_BulletPhysicsScript_TRIGGERPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_BulletPhysicsScript_TRIGGERPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"CLuaRayCollisionList","CLuaRayCollisionList","list<RAYCOLLISION>",tolua_collect_CLuaRayCollisionList);
 #else
 tolua_cclass(tolua_S,"CLuaRayCollisionList","CLuaRayCollisionList","list<RAYCOLLISION>",NULL);
 #endif
 tolua_beginmodule(tolua_S,"CLuaRayCollisionList");
  tolua_function(tolua_S,"new",tolua_BulletPhysicsScript_CLuaRayCollisionList_new00);
  tolua_function(tolua_S,"new_local",tolua_BulletPhysicsScript_CLuaRayCollisionList_new00_local);
  tolua_function(tolua_S,".call",tolua_BulletPhysicsScript_CLuaRayCollisionList_new00_local);
  tolua_function(tolua_S,"delete",tolua_BulletPhysicsScript_CLuaRayCollisionList_delete00);
  tolua_function(tolua_S,"InsertAtEnd",tolua_BulletPhysicsScript_CLuaRayCollisionList_InsertAtEnd00);
  tolua_function(tolua_S,"InsertAtBegin",tolua_BulletPhysicsScript_CLuaRayCollisionList_InsertAtBegin00);
  tolua_function(tolua_S,"GetSize",tolua_BulletPhysicsScript_CLuaRayCollisionList_GetSize00);
  tolua_function(tolua_S,"GetFirst",tolua_BulletPhysicsScript_CLuaRayCollisionList_GetFirst00);
  tolua_function(tolua_S,"GetNext",tolua_BulletPhysicsScript_CLuaRayCollisionList_GetNext00);
  tolua_function(tolua_S,"Clear",tolua_BulletPhysicsScript_CLuaRayCollisionList_Clear00);
  tolua_function(tolua_S,"SIZEOF",tolua_BulletPhysicsScript_CLuaRayCollisionList_SIZEOF00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_BulletPhysicsScript (lua_State* tolua_S) {
 return tolua_BulletPhysicsScript_open(tolua_S);
};
#endif

