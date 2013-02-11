/*
** Lua binding: EntityQueryLua
** Generated automatically by toluaxx-1.2.0 on 02/11/09 20:30:18.
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
TOLUA_API int tolua_EntityQueryLua_open (lua_State* tolua_S);

#include "StdAfx.h"
#include "../../ElementalEngine/EELuaScript/LuaHashString.h"
#pragma warning(disable : 4800)
#pragma warning(disable : 4345)
class CLuaDWORD { public: CLuaDWORD() {} CLuaDWORD(unsigned long i) { x = i; } operator DWORD() { return x; } void operator=(DWORD i) { x = i; } unsigned long x; };
#if 0	// dummy include with relative path for tolua .pkg file
#include "..\..\include\EntityQuery\EntityQueryDefs.h"
#include "LuaObjDistQueryList.h"
#include "LuaObjectTypeQueryList.h"
#endif

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_GETOBJECTTYPEQUERYPARAMS (lua_State* tolua_S)
{
 GETOBJECTTYPEQUERYPARAMS* self = (GETOBJECTTYPEQUERYPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CLuaDWORD (lua_State* tolua_S)
{
 CLuaDWORD* self = (CLuaDWORD*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_OBJECTDISTANCEQUERY (lua_State* tolua_S)
{
 OBJECTDISTANCEQUERY* self = (OBJECTDISTANCEQUERY*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CLuaObjDistQueryList (lua_State* tolua_S)
{
 CLuaObjDistQueryList* self = (CLuaObjDistQueryList*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_GETOBJECTQUERYPARAMS (lua_State* tolua_S)
{
 GETOBJECTQUERYPARAMS* self = (GETOBJECTQUERYPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CLuaObjectTypeQueryList (lua_State* tolua_S)
{
 CLuaObjectTypeQueryList* self = (CLuaObjectTypeQueryList*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"GETOBJECTTYPEQUERYPARAMS");
 tolua_usertype(tolua_S,"list<OBJECTDISTANCEQUERY>");
 tolua_usertype(tolua_S,"CLuaObjDistQueryList");
 tolua_usertype(tolua_S,"GETOBJECTQUERYPARAMS");
 tolua_usertype(tolua_S,"CLuaObjectTypeQueryList");
 tolua_usertype(tolua_S,"Vec3");
 tolua_usertype(tolua_S,"OBJECTDISTANCEQUERY");
 tolua_usertype(tolua_S,"CLuaHashString");
 tolua_usertype(tolua_S,"list<IHashString*>");
 tolua_usertype(tolua_S,"CLuaDWORD");
}

/* get function:name of class OBJECTDISTANCEQUERY */
#ifndef TOLUA_DISABLE_tolua_get_OBJECTDISTANCEQUERY_name_ptr
static int tolua_get_OBJECTDISTANCEQUERY_name_ptr (lua_State* tolua_S) {
  OBJECTDISTANCEQUERY*self = (OBJECTDISTANCEQUERY*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->name,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_OBJECTDISTANCEQUERY_name_ptr */

/* get function:name of class OBJECTDISTANCEQUERY */
#ifndef TOLUA_DISABLE_tolua_set_OBJECTDISTANCEQUERY_name_ptr
static int tolua_set_OBJECTDISTANCEQUERY_name_ptr (lua_State* tolua_S) {
  OBJECTDISTANCEQUERY*self = (OBJECTDISTANCEQUERY*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->name=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_OBJECTDISTANCEQUERY_name_ptr */

/* get function:distance of class OBJECTDISTANCEQUERY */
#ifndef TOLUA_DISABLE_tolua_get_OBJECTDISTANCEQUERY_distance
static int tolua_get_OBJECTDISTANCEQUERY_distance (lua_State* tolua_S) {
  OBJECTDISTANCEQUERY*self = (OBJECTDISTANCEQUERY*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'distance'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->distance);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_OBJECTDISTANCEQUERY_distance */

/* get function:distance of class OBJECTDISTANCEQUERY */
#ifndef TOLUA_DISABLE_tolua_set_OBJECTDISTANCEQUERY_distance
static int tolua_set_OBJECTDISTANCEQUERY_distance (lua_State* tolua_S) {
  OBJECTDISTANCEQUERY*self = (OBJECTDISTANCEQUERY*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'distance'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->distance=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_OBJECTDISTANCEQUERY_distance */

/* method: new of class  OBJECTDISTANCEQUERY */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_OBJECTDISTANCEQUERY_new00
static int tolua_EntityQueryLua_OBJECTDISTANCEQUERY_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"OBJECTDISTANCEQUERY",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  OBJECTDISTANCEQUERY* tolua_ret = (OBJECTDISTANCEQUERY*)  new OBJECTDISTANCEQUERY();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"OBJECTDISTANCEQUERY");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  OBJECTDISTANCEQUERY */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_OBJECTDISTANCEQUERY_new00_local
static int tolua_EntityQueryLua_OBJECTDISTANCEQUERY_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"OBJECTDISTANCEQUERY",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  OBJECTDISTANCEQUERY* tolua_ret = (OBJECTDISTANCEQUERY*)  new OBJECTDISTANCEQUERY();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"OBJECTDISTANCEQUERY");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  OBJECTDISTANCEQUERY */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_OBJECTDISTANCEQUERY_delete00
static int tolua_EntityQueryLua_OBJECTDISTANCEQUERY_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OBJECTDISTANCEQUERY",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  OBJECTDISTANCEQUERY* self = (OBJECTDISTANCEQUERY*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  OBJECTDISTANCEQUERY */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_OBJECTDISTANCEQUERY_SIZEOF00
static int tolua_EntityQueryLua_OBJECTDISTANCEQUERY_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OBJECTDISTANCEQUERY",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  OBJECTDISTANCEQUERY* self = (OBJECTDISTANCEQUERY*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(OBJECTDISTANCEQUERY);
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

/* get function:vPosition of class GETOBJECTQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_GETOBJECTQUERYPARAMS_vPosition
static int tolua_get_GETOBJECTQUERYPARAMS_vPosition (lua_State* tolua_S) {
  GETOBJECTQUERYPARAMS*self = (GETOBJECTQUERYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vPosition'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->vPosition,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GETOBJECTQUERYPARAMS_vPosition */

/* get function:vPosition of class GETOBJECTQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_GETOBJECTQUERYPARAMS_vPosition
static int tolua_set_GETOBJECTQUERYPARAMS_vPosition (lua_State* tolua_S) {
  GETOBJECTQUERYPARAMS*self = (GETOBJECTQUERYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vPosition'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->vPosition=*(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GETOBJECTQUERYPARAMS_vPosition */

/* get function:fRadius of class GETOBJECTQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_GETOBJECTQUERYPARAMS_fRadius
static int tolua_get_GETOBJECTQUERYPARAMS_fRadius (lua_State* tolua_S) {
  GETOBJECTQUERYPARAMS*self = (GETOBJECTQUERYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'fRadius'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->fRadius);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GETOBJECTQUERYPARAMS_fRadius */

/* get function:fRadius of class GETOBJECTQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_GETOBJECTQUERYPARAMS_fRadius
static int tolua_set_GETOBJECTQUERYPARAMS_fRadius (lua_State* tolua_S) {
  GETOBJECTQUERYPARAMS*self = (GETOBJECTQUERYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'fRadius'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->fRadius=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GETOBJECTQUERYPARAMS_fRadius */

/* get function:hszType of class GETOBJECTQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_GETOBJECTQUERYPARAMS_hszType_ptr
static int tolua_get_GETOBJECTQUERYPARAMS_hszType_ptr (lua_State* tolua_S) {
  GETOBJECTQUERYPARAMS*self = (GETOBJECTQUERYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'hszType'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->hszType,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GETOBJECTQUERYPARAMS_hszType_ptr */

/* get function:hszType of class GETOBJECTQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_GETOBJECTQUERYPARAMS_hszType_ptr
static int tolua_set_GETOBJECTQUERYPARAMS_hszType_ptr (lua_State* tolua_S) {
  GETOBJECTQUERYPARAMS*self = (GETOBJECTQUERYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'hszType'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->hszType=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GETOBJECTQUERYPARAMS_hszType_ptr */

/* get function:pObjectsList of class GETOBJECTQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_GETOBJECTQUERYPARAMS_pObjectsList_ptr
static int tolua_get_GETOBJECTQUERYPARAMS_pObjectsList_ptr (lua_State* tolua_S) {
  GETOBJECTQUERYPARAMS*self = (GETOBJECTQUERYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pObjectsList'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->pObjectsList,"list<OBJECTDISTANCEQUERY>");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GETOBJECTQUERYPARAMS_pObjectsList_ptr */

/* get function:pObjectsList of class GETOBJECTQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_GETOBJECTQUERYPARAMS_pObjectsList_ptr
static int tolua_set_GETOBJECTQUERYPARAMS_pObjectsList_ptr (lua_State* tolua_S) {
  GETOBJECTQUERYPARAMS*self = (GETOBJECTQUERYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pObjectsList'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"list<OBJECTDISTANCEQUERY>",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->pObjectsList=((  list<OBJECTDISTANCEQUERY>*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GETOBJECTQUERYPARAMS_pObjectsList_ptr */

/* method: new of class  GETOBJECTQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_GETOBJECTQUERYPARAMS_new00
static int tolua_EntityQueryLua_GETOBJECTQUERYPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GETOBJECTQUERYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GETOBJECTQUERYPARAMS* tolua_ret = (GETOBJECTQUERYPARAMS*)  new GETOBJECTQUERYPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"GETOBJECTQUERYPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GETOBJECTQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_GETOBJECTQUERYPARAMS_new00_local
static int tolua_EntityQueryLua_GETOBJECTQUERYPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GETOBJECTQUERYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GETOBJECTQUERYPARAMS* tolua_ret = (GETOBJECTQUERYPARAMS*)  new GETOBJECTQUERYPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GETOBJECTQUERYPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  GETOBJECTQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_GETOBJECTQUERYPARAMS_delete00
static int tolua_EntityQueryLua_GETOBJECTQUERYPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GETOBJECTQUERYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GETOBJECTQUERYPARAMS* self = (GETOBJECTQUERYPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  GETOBJECTQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_GETOBJECTQUERYPARAMS_SIZEOF00
static int tolua_EntityQueryLua_GETOBJECTQUERYPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GETOBJECTQUERYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GETOBJECTQUERYPARAMS* self = (GETOBJECTQUERYPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(GETOBJECTQUERYPARAMS);
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

/* get function:type of class GETOBJECTTYPEQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_GETOBJECTTYPEQUERYPARAMS_type_ptr
static int tolua_get_GETOBJECTTYPEQUERYPARAMS_type_ptr (lua_State* tolua_S) {
  GETOBJECTTYPEQUERYPARAMS*self = (GETOBJECTTYPEQUERYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'type'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->type,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GETOBJECTTYPEQUERYPARAMS_type_ptr */

/* get function:type of class GETOBJECTTYPEQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_GETOBJECTTYPEQUERYPARAMS_type_ptr
static int tolua_set_GETOBJECTTYPEQUERYPARAMS_type_ptr (lua_State* tolua_S) {
  GETOBJECTTYPEQUERYPARAMS*self = (GETOBJECTTYPEQUERYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'type'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->type=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GETOBJECTTYPEQUERYPARAMS_type_ptr */

/* get function:pObjectsList of class GETOBJECTTYPEQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_GETOBJECTTYPEQUERYPARAMS_pObjectsList_ptr
static int tolua_get_GETOBJECTTYPEQUERYPARAMS_pObjectsList_ptr (lua_State* tolua_S) {
  GETOBJECTTYPEQUERYPARAMS*self = (GETOBJECTTYPEQUERYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pObjectsList'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->pObjectsList,"list<IHashString*>");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GETOBJECTTYPEQUERYPARAMS_pObjectsList_ptr */

/* get function:pObjectsList of class GETOBJECTTYPEQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_GETOBJECTTYPEQUERYPARAMS_pObjectsList_ptr
static int tolua_set_GETOBJECTTYPEQUERYPARAMS_pObjectsList_ptr (lua_State* tolua_S) {
  GETOBJECTTYPEQUERYPARAMS*self = (GETOBJECTTYPEQUERYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pObjectsList'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"list<IHashString*>",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->pObjectsList=((  list<IHashString*>*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GETOBJECTTYPEQUERYPARAMS_pObjectsList_ptr */

/* method: new of class  GETOBJECTTYPEQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_GETOBJECTTYPEQUERYPARAMS_new00
static int tolua_EntityQueryLua_GETOBJECTTYPEQUERYPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GETOBJECTTYPEQUERYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GETOBJECTTYPEQUERYPARAMS* tolua_ret = (GETOBJECTTYPEQUERYPARAMS*)  new GETOBJECTTYPEQUERYPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"GETOBJECTTYPEQUERYPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GETOBJECTTYPEQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_GETOBJECTTYPEQUERYPARAMS_new00_local
static int tolua_EntityQueryLua_GETOBJECTTYPEQUERYPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GETOBJECTTYPEQUERYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GETOBJECTTYPEQUERYPARAMS* tolua_ret = (GETOBJECTTYPEQUERYPARAMS*)  new GETOBJECTTYPEQUERYPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GETOBJECTTYPEQUERYPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  GETOBJECTTYPEQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_GETOBJECTTYPEQUERYPARAMS_delete00
static int tolua_EntityQueryLua_GETOBJECTTYPEQUERYPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GETOBJECTTYPEQUERYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GETOBJECTTYPEQUERYPARAMS* self = (GETOBJECTTYPEQUERYPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  GETOBJECTTYPEQUERYPARAMS */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_GETOBJECTTYPEQUERYPARAMS_SIZEOF00
static int tolua_EntityQueryLua_GETOBJECTTYPEQUERYPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GETOBJECTTYPEQUERYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GETOBJECTTYPEQUERYPARAMS* self = (GETOBJECTTYPEQUERYPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(GETOBJECTTYPEQUERYPARAMS);
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

/* method: new of class  CLuaObjDistQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjDistQueryList_new00
static int tolua_EntityQueryLua_CLuaObjDistQueryList_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CLuaObjDistQueryList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  CLuaObjDistQueryList* tolua_ret = (CLuaObjDistQueryList*)  new CLuaObjDistQueryList();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CLuaObjDistQueryList");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CLuaObjDistQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjDistQueryList_new00_local
static int tolua_EntityQueryLua_CLuaObjDistQueryList_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CLuaObjDistQueryList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  CLuaObjDistQueryList* tolua_ret = (CLuaObjDistQueryList*)  new CLuaObjDistQueryList();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CLuaObjDistQueryList");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CLuaObjDistQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjDistQueryList_delete00
static int tolua_EntityQueryLua_CLuaObjDistQueryList_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaObjDistQueryList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaObjDistQueryList* self = (CLuaObjDistQueryList*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: Add of class  CLuaObjDistQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjDistQueryList_Add00
static int tolua_EntityQueryLua_CLuaObjDistQueryList_Add00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaObjDistQueryList",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaObjDistQueryList* self = (CLuaObjDistQueryList*)  tolua_tousertype(tolua_S,1,0);
   CLuaHashString* name = ((  CLuaHashString*)  tolua_tousertype(tolua_S,2,0));
  float distance = ((float)  tolua_tonumber(tolua_S,3,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Add'",NULL);
 {
  self->Add(name,distance);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Add'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: Remove of class  CLuaObjDistQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjDistQueryList_Remove00
static int tolua_EntityQueryLua_CLuaObjDistQueryList_Remove00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaObjDistQueryList",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaObjDistQueryList* self = (CLuaObjDistQueryList*)  tolua_tousertype(tolua_S,1,0);
   CLuaHashString* name = ((  CLuaHashString*)  tolua_tousertype(tolua_S,2,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Remove'",NULL);
 {
  self->Remove(name);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Remove'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFirst of class  CLuaObjDistQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjDistQueryList_GetFirst00
static int tolua_EntityQueryLua_CLuaObjDistQueryList_GetFirst00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaObjDistQueryList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaObjDistQueryList* self = (CLuaObjDistQueryList*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFirst'",NULL);
 {
  OBJECTDISTANCEQUERY* tolua_ret = (OBJECTDISTANCEQUERY*)  self->GetFirst();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"OBJECTDISTANCEQUERY");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFirst'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNext of class  CLuaObjDistQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjDistQueryList_GetNext00
static int tolua_EntityQueryLua_CLuaObjDistQueryList_GetNext00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaObjDistQueryList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaObjDistQueryList* self = (CLuaObjDistQueryList*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNext'",NULL);
 {
  OBJECTDISTANCEQUERY* tolua_ret = (OBJECTDISTANCEQUERY*)  self->GetNext();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"OBJECTDISTANCEQUERY");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNext'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSize of class  CLuaObjDistQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjDistQueryList_GetSize00
static int tolua_EntityQueryLua_CLuaObjDistQueryList_GetSize00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaObjDistQueryList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaObjDistQueryList* self = (CLuaObjDistQueryList*)  tolua_tousertype(tolua_S,1,0);
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

/* method: Clear of class  CLuaObjDistQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjDistQueryList_Clear00
static int tolua_EntityQueryLua_CLuaObjDistQueryList_Clear00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaObjDistQueryList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaObjDistQueryList* self = (CLuaObjDistQueryList*)  tolua_tousertype(tolua_S,1,0);
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

/* method: SIZEOF of class  CLuaObjDistQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjDistQueryList_SIZEOF00
static int tolua_EntityQueryLua_CLuaObjDistQueryList_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaObjDistQueryList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaObjDistQueryList* self = (CLuaObjDistQueryList*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(CLuaObjDistQueryList);
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

/* method: new of class  CLuaObjectTypeQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjectTypeQueryList_new00
static int tolua_EntityQueryLua_CLuaObjectTypeQueryList_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CLuaObjectTypeQueryList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  CLuaObjectTypeQueryList* tolua_ret = (CLuaObjectTypeQueryList*)  new CLuaObjectTypeQueryList();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CLuaObjectTypeQueryList");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CLuaObjectTypeQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjectTypeQueryList_new00_local
static int tolua_EntityQueryLua_CLuaObjectTypeQueryList_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CLuaObjectTypeQueryList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  CLuaObjectTypeQueryList* tolua_ret = (CLuaObjectTypeQueryList*)  new CLuaObjectTypeQueryList();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CLuaObjectTypeQueryList");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CLuaObjectTypeQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjectTypeQueryList_delete00
static int tolua_EntityQueryLua_CLuaObjectTypeQueryList_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaObjectTypeQueryList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaObjectTypeQueryList* self = (CLuaObjectTypeQueryList*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: Add of class  CLuaObjectTypeQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjectTypeQueryList_Add00
static int tolua_EntityQueryLua_CLuaObjectTypeQueryList_Add00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaObjectTypeQueryList",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaObjectTypeQueryList* self = (CLuaObjectTypeQueryList*)  tolua_tousertype(tolua_S,1,0);
   CLuaHashString* name = ((  CLuaHashString*)  tolua_tousertype(tolua_S,2,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Add'",NULL);
 {
  self->Add(name);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Add'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: Remove of class  CLuaObjectTypeQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjectTypeQueryList_Remove00
static int tolua_EntityQueryLua_CLuaObjectTypeQueryList_Remove00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaObjectTypeQueryList",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaObjectTypeQueryList* self = (CLuaObjectTypeQueryList*)  tolua_tousertype(tolua_S,1,0);
   CLuaHashString* name = ((  CLuaHashString*)  tolua_tousertype(tolua_S,2,0));
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Remove'",NULL);
 {
  self->Remove(name);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Remove'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFirst of class  CLuaObjectTypeQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjectTypeQueryList_GetFirst00
static int tolua_EntityQueryLua_CLuaObjectTypeQueryList_GetFirst00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaObjectTypeQueryList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaObjectTypeQueryList* self = (CLuaObjectTypeQueryList*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFirst'",NULL);
 {
   CLuaHashString* tolua_ret = (  CLuaHashString*)  self->GetFirst();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CLuaHashString");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFirst'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNext of class  CLuaObjectTypeQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjectTypeQueryList_GetNext00
static int tolua_EntityQueryLua_CLuaObjectTypeQueryList_GetNext00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaObjectTypeQueryList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaObjectTypeQueryList* self = (CLuaObjectTypeQueryList*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNext'",NULL);
 {
   CLuaHashString* tolua_ret = (  CLuaHashString*)  self->GetNext();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CLuaHashString");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNext'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSize of class  CLuaObjectTypeQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjectTypeQueryList_GetSize00
static int tolua_EntityQueryLua_CLuaObjectTypeQueryList_GetSize00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaObjectTypeQueryList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaObjectTypeQueryList* self = (CLuaObjectTypeQueryList*)  tolua_tousertype(tolua_S,1,0);
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

/* method: Clear of class  CLuaObjectTypeQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjectTypeQueryList_Clear00
static int tolua_EntityQueryLua_CLuaObjectTypeQueryList_Clear00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaObjectTypeQueryList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaObjectTypeQueryList* self = (CLuaObjectTypeQueryList*)  tolua_tousertype(tolua_S,1,0);
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

/* method: SIZEOF of class  CLuaObjectTypeQueryList */
#ifndef TOLUA_DISABLE_tolua_EntityQueryLua_CLuaObjectTypeQueryList_SIZEOF00
static int tolua_EntityQueryLua_CLuaObjectTypeQueryList_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CLuaObjectTypeQueryList",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CLuaObjectTypeQueryList* self = (CLuaObjectTypeQueryList*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(CLuaObjectTypeQueryList);
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
TOLUA_API int tolua_EntityQueryLua_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"OBJECTDISTANCEQUERY","OBJECTDISTANCEQUERY","",tolua_collect_OBJECTDISTANCEQUERY);
 #else
 tolua_cclass(tolua_S,"OBJECTDISTANCEQUERY","OBJECTDISTANCEQUERY","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"OBJECTDISTANCEQUERY");
  tolua_variable(tolua_S,"name",tolua_get_OBJECTDISTANCEQUERY_name_ptr,tolua_set_OBJECTDISTANCEQUERY_name_ptr);
  tolua_variable(tolua_S,"distance",tolua_get_OBJECTDISTANCEQUERY_distance,tolua_set_OBJECTDISTANCEQUERY_distance);
  tolua_function(tolua_S,"new",tolua_EntityQueryLua_OBJECTDISTANCEQUERY_new00);
  tolua_function(tolua_S,"new_local",tolua_EntityQueryLua_OBJECTDISTANCEQUERY_new00_local);
  tolua_function(tolua_S,".call",tolua_EntityQueryLua_OBJECTDISTANCEQUERY_new00_local);
  tolua_function(tolua_S,"delete",tolua_EntityQueryLua_OBJECTDISTANCEQUERY_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_EntityQueryLua_OBJECTDISTANCEQUERY_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"GETOBJECTQUERYPARAMS","GETOBJECTQUERYPARAMS","",tolua_collect_GETOBJECTQUERYPARAMS);
 #else
 tolua_cclass(tolua_S,"GETOBJECTQUERYPARAMS","GETOBJECTQUERYPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"GETOBJECTQUERYPARAMS");
  tolua_variable(tolua_S,"vPosition",tolua_get_GETOBJECTQUERYPARAMS_vPosition,tolua_set_GETOBJECTQUERYPARAMS_vPosition);
  tolua_variable(tolua_S,"fRadius",tolua_get_GETOBJECTQUERYPARAMS_fRadius,tolua_set_GETOBJECTQUERYPARAMS_fRadius);
  tolua_variable(tolua_S,"hszType",tolua_get_GETOBJECTQUERYPARAMS_hszType_ptr,tolua_set_GETOBJECTQUERYPARAMS_hszType_ptr);
  tolua_variable(tolua_S,"pObjectsList",tolua_get_GETOBJECTQUERYPARAMS_pObjectsList_ptr,tolua_set_GETOBJECTQUERYPARAMS_pObjectsList_ptr);
  tolua_function(tolua_S,"new",tolua_EntityQueryLua_GETOBJECTQUERYPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_EntityQueryLua_GETOBJECTQUERYPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_EntityQueryLua_GETOBJECTQUERYPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_EntityQueryLua_GETOBJECTQUERYPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_EntityQueryLua_GETOBJECTQUERYPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"GETOBJECTTYPEQUERYPARAMS","GETOBJECTTYPEQUERYPARAMS","",tolua_collect_GETOBJECTTYPEQUERYPARAMS);
 #else
 tolua_cclass(tolua_S,"GETOBJECTTYPEQUERYPARAMS","GETOBJECTTYPEQUERYPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"GETOBJECTTYPEQUERYPARAMS");
  tolua_variable(tolua_S,"type",tolua_get_GETOBJECTTYPEQUERYPARAMS_type_ptr,tolua_set_GETOBJECTTYPEQUERYPARAMS_type_ptr);
  tolua_variable(tolua_S,"pObjectsList",tolua_get_GETOBJECTTYPEQUERYPARAMS_pObjectsList_ptr,tolua_set_GETOBJECTTYPEQUERYPARAMS_pObjectsList_ptr);
  tolua_function(tolua_S,"new",tolua_EntityQueryLua_GETOBJECTTYPEQUERYPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_EntityQueryLua_GETOBJECTTYPEQUERYPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_EntityQueryLua_GETOBJECTTYPEQUERYPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_EntityQueryLua_GETOBJECTTYPEQUERYPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_EntityQueryLua_GETOBJECTTYPEQUERYPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"CLuaObjDistQueryList","CLuaObjDistQueryList","ODQLIST",tolua_collect_CLuaObjDistQueryList);
 #else
 tolua_cclass(tolua_S,"CLuaObjDistQueryList","CLuaObjDistQueryList","ODQLIST",NULL);
 #endif
 tolua_beginmodule(tolua_S,"CLuaObjDistQueryList");
  tolua_function(tolua_S,"new",tolua_EntityQueryLua_CLuaObjDistQueryList_new00);
  tolua_function(tolua_S,"new_local",tolua_EntityQueryLua_CLuaObjDistQueryList_new00_local);
  tolua_function(tolua_S,".call",tolua_EntityQueryLua_CLuaObjDistQueryList_new00_local);
  tolua_function(tolua_S,"delete",tolua_EntityQueryLua_CLuaObjDistQueryList_delete00);
  tolua_function(tolua_S,"Add",tolua_EntityQueryLua_CLuaObjDistQueryList_Add00);
  tolua_function(tolua_S,"Remove",tolua_EntityQueryLua_CLuaObjDistQueryList_Remove00);
  tolua_function(tolua_S,"GetFirst",tolua_EntityQueryLua_CLuaObjDistQueryList_GetFirst00);
  tolua_function(tolua_S,"GetNext",tolua_EntityQueryLua_CLuaObjDistQueryList_GetNext00);
  tolua_function(tolua_S,"GetSize",tolua_EntityQueryLua_CLuaObjDistQueryList_GetSize00);
  tolua_function(tolua_S,"Clear",tolua_EntityQueryLua_CLuaObjDistQueryList_Clear00);
  tolua_function(tolua_S,"SIZEOF",tolua_EntityQueryLua_CLuaObjDistQueryList_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"CLuaObjectTypeQueryList","CLuaObjectTypeQueryList","OBJECTTYPEQUERYLIST",tolua_collect_CLuaObjectTypeQueryList);
 #else
 tolua_cclass(tolua_S,"CLuaObjectTypeQueryList","CLuaObjectTypeQueryList","OBJECTTYPEQUERYLIST",NULL);
 #endif
 tolua_beginmodule(tolua_S,"CLuaObjectTypeQueryList");
  tolua_function(tolua_S,"new",tolua_EntityQueryLua_CLuaObjectTypeQueryList_new00);
  tolua_function(tolua_S,"new_local",tolua_EntityQueryLua_CLuaObjectTypeQueryList_new00_local);
  tolua_function(tolua_S,".call",tolua_EntityQueryLua_CLuaObjectTypeQueryList_new00_local);
  tolua_function(tolua_S,"delete",tolua_EntityQueryLua_CLuaObjectTypeQueryList_delete00);
  tolua_function(tolua_S,"Add",tolua_EntityQueryLua_CLuaObjectTypeQueryList_Add00);
  tolua_function(tolua_S,"Remove",tolua_EntityQueryLua_CLuaObjectTypeQueryList_Remove00);
  tolua_function(tolua_S,"GetFirst",tolua_EntityQueryLua_CLuaObjectTypeQueryList_GetFirst00);
  tolua_function(tolua_S,"GetNext",tolua_EntityQueryLua_CLuaObjectTypeQueryList_GetNext00);
  tolua_function(tolua_S,"GetSize",tolua_EntityQueryLua_CLuaObjectTypeQueryList_GetSize00);
  tolua_function(tolua_S,"Clear",tolua_EntityQueryLua_CLuaObjectTypeQueryList_Clear00);
  tolua_function(tolua_S,"SIZEOF",tolua_EntityQueryLua_CLuaObjectTypeQueryList_SIZEOF00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_EntityQueryLua (lua_State* tolua_S) {
 return tolua_EntityQueryLua_open(tolua_S);
};
#endif

