/*
** Lua binding: Cal3dModelsLua
** Generated automatically by toluaxx-1.2.0 on 02/03/09 22:15:22.
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
TOLUA_API int tolua_Cal3dModelsLua_open (lua_State* tolua_S);

#include "StdAfx.h"
#include "../../ElementalEngine/EELuaScript/LuaHashString.h"
#pragma warning(disable : 4800)
#pragma warning(disable : 4345)
class CLuaDWORD { public: CLuaDWORD() {} CLuaDWORD(unsigned long i) { x = i; } operator DWORD() { return x; } void operator=(DWORD i) { x = i; } unsigned long x; };
#if 0	// dummy include with relative path for tolua .pkg file
#include "..\..\include\Cal3dModels\Cal3dModelsDefine.h"
#endif

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_GETRENDERBUFFERALLOCATIONBYNAMEMSG (lua_State* tolua_S)
{
 GETRENDERBUFFERALLOCATIONBYNAMEMSG* self = (GETRENDERBUFFERALLOCATIONBYNAMEMSG*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CLuaDWORD (lua_State* tolua_S)
{
 CLuaDWORD* self = (CLuaDWORD*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_GETCLOTHPHYSICSGEOMETRYPARAMS (lua_State* tolua_S)
{
 GETCLOTHPHYSICSGEOMETRYPARAMS* self = (GETCLOTHPHYSICSGEOMETRYPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_REGISTERCAL3DANIMATIONCALLBACK (lua_State* tolua_S)
{
 REGISTERCAL3DANIMATIONCALLBACK* self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_GETRENDERBUFFERALLOCATIONMSG (lua_State* tolua_S)
{
 GETRENDERBUFFERALLOCATIONMSG* self = (GETRENDERBUFFERALLOCATIONMSG*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_GETCALPARENT (lua_State* tolua_S)
{
 GETCALPARENT* self = (GETCALPARENT*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_RENDERBUFFERALLOCATION (lua_State* tolua_S)
{
 RENDERBUFFERALLOCATION* self = (RENDERBUFFERALLOCATION*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_RENDEROBJECTANIMATIONPARAMS (lua_State* tolua_S)
{
 RENDEROBJECTANIMATIONPARAMS* self = (RENDEROBJECTANIMATIONPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_GETANIMATIONLISTPARAMS (lua_State* tolua_S)
{
 GETANIMATIONLISTPARAMS* self = (GETANIMATIONLISTPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_ANIMATIONID (lua_State* tolua_S)
{
 ANIMATIONID* self = (ANIMATIONID*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"GETRENDERBUFFERALLOCATIONBYNAMEMSG");
 tolua_usertype(tolua_S,"IVertexBufferObject");
 tolua_usertype(tolua_S,"REGISTERCAL3DANIMATIONCALLBACK");
 tolua_usertype(tolua_S,"IMaterial");
 tolua_usertype(tolua_S,"GETCALPARENT");
 tolua_usertype(tolua_S,"IGeometry");
 tolua_usertype(tolua_S,"vector<RENDERBUFFERALLOCATION>");
 tolua_usertype(tolua_S,"RENDERBUFFERALLOCATION");
 tolua_usertype(tolua_S,"ANIMATIONLIST");
 tolua_usertype(tolua_S,"CLuaHashString");
 tolua_usertype(tolua_S,"GETANIMATIONLISTPARAMS");
 tolua_usertype(tolua_S,"CLuaDWORD");
 tolua_usertype(tolua_S,"GETCLOTHPHYSICSGEOMETRYPARAMS");
 tolua_usertype(tolua_S,"IIndexBuffer");
 tolua_usertype(tolua_S,"GETRENDERBUFFERALLOCATIONMSG");
 tolua_usertype(tolua_S,"RENDEROBJECTANIMATIONPARAMS");
 tolua_usertype(tolua_S,"ANIMATIONID");
 tolua_usertype(tolua_S,"IArchive");
 tolua_usertype(tolua_S,"StdString");
 tolua_usertype(tolua_S,"BUFFERALLOCATIONSTRUCT");
}

/* get function:parent of class GETCALPARENT */
#ifndef TOLUA_DISABLE_tolua_get_GETCALPARENT_parent_ptr
static int tolua_get_GETCALPARENT_parent_ptr (lua_State* tolua_S) {
  GETCALPARENT*self = (GETCALPARENT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'parent'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->parent,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GETCALPARENT_parent_ptr */

/* get function:parent of class GETCALPARENT */
#ifndef TOLUA_DISABLE_tolua_set_GETCALPARENT_parent_ptr
static int tolua_set_GETCALPARENT_parent_ptr (lua_State* tolua_S) {
  GETCALPARENT*self = (GETCALPARENT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'parent'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->parent=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GETCALPARENT_parent_ptr */

/* method: new of class  GETCALPARENT */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETCALPARENT_new00
static int tolua_Cal3dModelsLua_GETCALPARENT_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GETCALPARENT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GETCALPARENT* tolua_ret = (GETCALPARENT*)  new GETCALPARENT();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"GETCALPARENT");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GETCALPARENT */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETCALPARENT_new00_local
static int tolua_Cal3dModelsLua_GETCALPARENT_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GETCALPARENT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GETCALPARENT* tolua_ret = (GETCALPARENT*)  new GETCALPARENT();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GETCALPARENT");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  GETCALPARENT */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETCALPARENT_delete00
static int tolua_Cal3dModelsLua_GETCALPARENT_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GETCALPARENT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GETCALPARENT* self = (GETCALPARENT*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  GETCALPARENT */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETCALPARENT_SIZEOF00
static int tolua_Cal3dModelsLua_GETCALPARENT_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GETCALPARENT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GETCALPARENT* self = (GETCALPARENT*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(GETCALPARENT);
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

/* get function:AnimationName of class ANIMATIONID */
#ifndef TOLUA_DISABLE_tolua_get_ANIMATIONID_AnimationName
static int tolua_get_ANIMATIONID_AnimationName (lua_State* tolua_S) {
  ANIMATIONID*self = (ANIMATIONID*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AnimationName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->AnimationName,"StdString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ANIMATIONID_AnimationName */

/* get function:AnimationName of class ANIMATIONID */
#ifndef TOLUA_DISABLE_tolua_set_ANIMATIONID_AnimationName
static int tolua_set_ANIMATIONID_AnimationName (lua_State* tolua_S) {
  ANIMATIONID*self = (ANIMATIONID*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AnimationName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"StdString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->AnimationName=*(( StdString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ANIMATIONID_AnimationName */

/* get function:AnimationId of class ANIMATIONID */
#ifndef TOLUA_DISABLE_tolua_get_ANIMATIONID_AnimationId
static int tolua_get_ANIMATIONID_AnimationId (lua_State* tolua_S) {
  ANIMATIONID*self = (ANIMATIONID*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AnimationId'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->AnimationId);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ANIMATIONID_AnimationId */

/* get function:AnimationId of class ANIMATIONID */
#ifndef TOLUA_DISABLE_tolua_set_ANIMATIONID_AnimationId
static int tolua_set_ANIMATIONID_AnimationId (lua_State* tolua_S) {
  ANIMATIONID*self = (ANIMATIONID*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AnimationId'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->AnimationId=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ANIMATIONID_AnimationId */

/* method: new of class  ANIMATIONID */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_ANIMATIONID_new00
static int tolua_Cal3dModelsLua_ANIMATIONID_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"ANIMATIONID",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  ANIMATIONID* tolua_ret = (ANIMATIONID*)  new ANIMATIONID();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"ANIMATIONID");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  ANIMATIONID */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_ANIMATIONID_new00_local
static int tolua_Cal3dModelsLua_ANIMATIONID_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"ANIMATIONID",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  ANIMATIONID* tolua_ret = (ANIMATIONID*)  new ANIMATIONID();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"ANIMATIONID");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  ANIMATIONID */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_ANIMATIONID_delete00
static int tolua_Cal3dModelsLua_ANIMATIONID_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"ANIMATIONID",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  ANIMATIONID* self = (ANIMATIONID*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  ANIMATIONID */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_ANIMATIONID_SIZEOF00
static int tolua_Cal3dModelsLua_ANIMATIONID_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"ANIMATIONID",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  ANIMATIONID* self = (ANIMATIONID*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(ANIMATIONID);
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

/* get function:numAnimations of class GETANIMATIONLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_GETANIMATIONLISTPARAMS_numAnimations
static int tolua_get_GETANIMATIONLISTPARAMS_numAnimations (lua_State* tolua_S) {
  GETANIMATIONLISTPARAMS*self = (GETANIMATIONLISTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'numAnimations'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->numAnimations);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GETANIMATIONLISTPARAMS_numAnimations */

/* get function:numAnimations of class GETANIMATIONLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_GETANIMATIONLISTPARAMS_numAnimations
static int tolua_set_GETANIMATIONLISTPARAMS_numAnimations (lua_State* tolua_S) {
  GETANIMATIONLISTPARAMS*self = (GETANIMATIONLISTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'numAnimations'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->numAnimations=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GETANIMATIONLISTPARAMS_numAnimations */

/* get function:AnimationList of class GETANIMATIONLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_GETANIMATIONLISTPARAMS_AnimationList_ptr
static int tolua_get_GETANIMATIONLISTPARAMS_AnimationList_ptr (lua_State* tolua_S) {
  GETANIMATIONLISTPARAMS*self = (GETANIMATIONLISTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AnimationList'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->AnimationList,"ANIMATIONLIST");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GETANIMATIONLISTPARAMS_AnimationList_ptr */

/* get function:AnimationList of class GETANIMATIONLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_GETANIMATIONLISTPARAMS_AnimationList_ptr
static int tolua_set_GETANIMATIONLISTPARAMS_AnimationList_ptr (lua_State* tolua_S) {
  GETANIMATIONLISTPARAMS*self = (GETANIMATIONLISTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AnimationList'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"ANIMATIONLIST",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->AnimationList=(( ANIMATIONLIST*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GETANIMATIONLISTPARAMS_AnimationList_ptr */

/* method: new of class  GETANIMATIONLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETANIMATIONLISTPARAMS_new00
static int tolua_Cal3dModelsLua_GETANIMATIONLISTPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GETANIMATIONLISTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GETANIMATIONLISTPARAMS* tolua_ret = (GETANIMATIONLISTPARAMS*)  new GETANIMATIONLISTPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"GETANIMATIONLISTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GETANIMATIONLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETANIMATIONLISTPARAMS_new00_local
static int tolua_Cal3dModelsLua_GETANIMATIONLISTPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GETANIMATIONLISTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GETANIMATIONLISTPARAMS* tolua_ret = (GETANIMATIONLISTPARAMS*)  new GETANIMATIONLISTPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GETANIMATIONLISTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  GETANIMATIONLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETANIMATIONLISTPARAMS_delete00
static int tolua_Cal3dModelsLua_GETANIMATIONLISTPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GETANIMATIONLISTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GETANIMATIONLISTPARAMS* self = (GETANIMATIONLISTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  GETANIMATIONLISTPARAMS */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETANIMATIONLISTPARAMS_SIZEOF00
static int tolua_Cal3dModelsLua_GETANIMATIONLISTPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GETANIMATIONLISTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GETANIMATIONLISTPARAMS* self = (GETANIMATIONLISTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(GETANIMATIONLISTPARAMS);
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

/* get function:m_IndexAllocation of class RENDERBUFFERALLOCATION */
#ifndef TOLUA_DISABLE_tolua_get_RENDERBUFFERALLOCATION_m_IndexAllocation
static int tolua_get_RENDERBUFFERALLOCATION_m_IndexAllocation (lua_State* tolua_S) {
  RENDERBUFFERALLOCATION*self = (RENDERBUFFERALLOCATION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_IndexAllocation'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->m_IndexAllocation,"BUFFERALLOCATIONSTRUCT");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RENDERBUFFERALLOCATION_m_IndexAllocation */

/* get function:m_IndexAllocation of class RENDERBUFFERALLOCATION */
#ifndef TOLUA_DISABLE_tolua_set_RENDERBUFFERALLOCATION_m_IndexAllocation
static int tolua_set_RENDERBUFFERALLOCATION_m_IndexAllocation (lua_State* tolua_S) {
  RENDERBUFFERALLOCATION*self = (RENDERBUFFERALLOCATION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_IndexAllocation'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"BUFFERALLOCATIONSTRUCT",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_IndexAllocation=*(( BUFFERALLOCATIONSTRUCT*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RENDERBUFFERALLOCATION_m_IndexAllocation */

/* get function:m_VertexAllocation of class RENDERBUFFERALLOCATION */
#ifndef TOLUA_DISABLE_tolua_get_RENDERBUFFERALLOCATION_m_VertexAllocation
static int tolua_get_RENDERBUFFERALLOCATION_m_VertexAllocation (lua_State* tolua_S) {
  RENDERBUFFERALLOCATION*self = (RENDERBUFFERALLOCATION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_VertexAllocation'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->m_VertexAllocation,"BUFFERALLOCATIONSTRUCT");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RENDERBUFFERALLOCATION_m_VertexAllocation */

/* get function:m_VertexAllocation of class RENDERBUFFERALLOCATION */
#ifndef TOLUA_DISABLE_tolua_set_RENDERBUFFERALLOCATION_m_VertexAllocation
static int tolua_set_RENDERBUFFERALLOCATION_m_VertexAllocation (lua_State* tolua_S) {
  RENDERBUFFERALLOCATION*self = (RENDERBUFFERALLOCATION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_VertexAllocation'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"BUFFERALLOCATIONSTRUCT",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_VertexAllocation=*(( BUFFERALLOCATIONSTRUCT*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RENDERBUFFERALLOCATION_m_VertexAllocation */

/* get function:m_VertexBufferInterface of class RENDERBUFFERALLOCATION */
#ifndef TOLUA_DISABLE_tolua_get_RENDERBUFFERALLOCATION_m_VertexBufferInterface_ptr
static int tolua_get_RENDERBUFFERALLOCATION_m_VertexBufferInterface_ptr (lua_State* tolua_S) {
  RENDERBUFFERALLOCATION*self = (RENDERBUFFERALLOCATION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_VertexBufferInterface'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_VertexBufferInterface,"IVertexBufferObject");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RENDERBUFFERALLOCATION_m_VertexBufferInterface_ptr */

/* get function:m_VertexBufferInterface of class RENDERBUFFERALLOCATION */
#ifndef TOLUA_DISABLE_tolua_set_RENDERBUFFERALLOCATION_m_VertexBufferInterface_ptr
static int tolua_set_RENDERBUFFERALLOCATION_m_VertexBufferInterface_ptr (lua_State* tolua_S) {
  RENDERBUFFERALLOCATION*self = (RENDERBUFFERALLOCATION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_VertexBufferInterface'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"IVertexBufferObject",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_VertexBufferInterface=(( IVertexBufferObject*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RENDERBUFFERALLOCATION_m_VertexBufferInterface_ptr */

/* get function:m_IndexBufferInterface of class RENDERBUFFERALLOCATION */
#ifndef TOLUA_DISABLE_tolua_get_RENDERBUFFERALLOCATION_m_IndexBufferInterface_ptr
static int tolua_get_RENDERBUFFERALLOCATION_m_IndexBufferInterface_ptr (lua_State* tolua_S) {
  RENDERBUFFERALLOCATION*self = (RENDERBUFFERALLOCATION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_IndexBufferInterface'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_IndexBufferInterface,"IIndexBuffer");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RENDERBUFFERALLOCATION_m_IndexBufferInterface_ptr */

/* get function:m_IndexBufferInterface of class RENDERBUFFERALLOCATION */
#ifndef TOLUA_DISABLE_tolua_set_RENDERBUFFERALLOCATION_m_IndexBufferInterface_ptr
static int tolua_set_RENDERBUFFERALLOCATION_m_IndexBufferInterface_ptr (lua_State* tolua_S) {
  RENDERBUFFERALLOCATION*self = (RENDERBUFFERALLOCATION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_IndexBufferInterface'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"IIndexBuffer",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_IndexBufferInterface=(( IIndexBuffer*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RENDERBUFFERALLOCATION_m_IndexBufferInterface_ptr */

/* get function:m_Material of class RENDERBUFFERALLOCATION */
#ifndef TOLUA_DISABLE_tolua_get_RENDERBUFFERALLOCATION_m_Material_ptr
static int tolua_get_RENDERBUFFERALLOCATION_m_Material_ptr (lua_State* tolua_S) {
  RENDERBUFFERALLOCATION*self = (RENDERBUFFERALLOCATION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Material'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_Material,"IMaterial");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RENDERBUFFERALLOCATION_m_Material_ptr */

/* get function:m_Material of class RENDERBUFFERALLOCATION */
#ifndef TOLUA_DISABLE_tolua_set_RENDERBUFFERALLOCATION_m_Material_ptr
static int tolua_set_RENDERBUFFERALLOCATION_m_Material_ptr (lua_State* tolua_S) {
  RENDERBUFFERALLOCATION*self = (RENDERBUFFERALLOCATION*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Material'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"IMaterial",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_Material=(( IMaterial*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RENDERBUFFERALLOCATION_m_Material_ptr */

/* method: new of class  RENDERBUFFERALLOCATION */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_RENDERBUFFERALLOCATION_new00
static int tolua_Cal3dModelsLua_RENDERBUFFERALLOCATION_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"RENDERBUFFERALLOCATION",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  RENDERBUFFERALLOCATION* tolua_ret = (RENDERBUFFERALLOCATION*)  new RENDERBUFFERALLOCATION();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"RENDERBUFFERALLOCATION");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  RENDERBUFFERALLOCATION */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_RENDERBUFFERALLOCATION_new00_local
static int tolua_Cal3dModelsLua_RENDERBUFFERALLOCATION_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"RENDERBUFFERALLOCATION",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  RENDERBUFFERALLOCATION* tolua_ret = (RENDERBUFFERALLOCATION*)  new RENDERBUFFERALLOCATION();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"RENDERBUFFERALLOCATION");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  RENDERBUFFERALLOCATION */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_RENDERBUFFERALLOCATION_delete00
static int tolua_Cal3dModelsLua_RENDERBUFFERALLOCATION_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"RENDERBUFFERALLOCATION",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  RENDERBUFFERALLOCATION* self = (RENDERBUFFERALLOCATION*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  RENDERBUFFERALLOCATION */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_RENDERBUFFERALLOCATION_SIZEOF00
static int tolua_Cal3dModelsLua_RENDERBUFFERALLOCATION_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"RENDERBUFFERALLOCATION",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  RENDERBUFFERALLOCATION* self = (RENDERBUFFERALLOCATION*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(RENDERBUFFERALLOCATION);
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

/* get function:m_GeometryAllocationsOut of class GETRENDERBUFFERALLOCATIONMSG */
#ifndef TOLUA_DISABLE_tolua_get_GETRENDERBUFFERALLOCATIONMSG_m_GeometryAllocationsOut
static int tolua_get_GETRENDERBUFFERALLOCATIONMSG_m_GeometryAllocationsOut (lua_State* tolua_S) {
  GETRENDERBUFFERALLOCATIONMSG*self = (GETRENDERBUFFERALLOCATIONMSG*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_GeometryAllocationsOut'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->m_GeometryAllocationsOut,"vector<RENDERBUFFERALLOCATION>");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GETRENDERBUFFERALLOCATIONMSG_m_GeometryAllocationsOut */

/* get function:m_GeometryAllocationsOut of class GETRENDERBUFFERALLOCATIONMSG */
#ifndef TOLUA_DISABLE_tolua_set_GETRENDERBUFFERALLOCATIONMSG_m_GeometryAllocationsOut
static int tolua_set_GETRENDERBUFFERALLOCATIONMSG_m_GeometryAllocationsOut (lua_State* tolua_S) {
  GETRENDERBUFFERALLOCATIONMSG*self = (GETRENDERBUFFERALLOCATIONMSG*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_GeometryAllocationsOut'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"vector<RENDERBUFFERALLOCATION>",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_GeometryAllocationsOut=*(( vector<RENDERBUFFERALLOCATION>*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GETRENDERBUFFERALLOCATIONMSG_m_GeometryAllocationsOut */

/* method: new of class  GETRENDERBUFFERALLOCATIONMSG */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONMSG_new00
static int tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONMSG_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GETRENDERBUFFERALLOCATIONMSG",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GETRENDERBUFFERALLOCATIONMSG* tolua_ret = (GETRENDERBUFFERALLOCATIONMSG*)  new GETRENDERBUFFERALLOCATIONMSG();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"GETRENDERBUFFERALLOCATIONMSG");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GETRENDERBUFFERALLOCATIONMSG */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONMSG_new00_local
static int tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONMSG_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GETRENDERBUFFERALLOCATIONMSG",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GETRENDERBUFFERALLOCATIONMSG* tolua_ret = (GETRENDERBUFFERALLOCATIONMSG*)  new GETRENDERBUFFERALLOCATIONMSG();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GETRENDERBUFFERALLOCATIONMSG");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  GETRENDERBUFFERALLOCATIONMSG */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONMSG_delete00
static int tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONMSG_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GETRENDERBUFFERALLOCATIONMSG",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GETRENDERBUFFERALLOCATIONMSG* self = (GETRENDERBUFFERALLOCATIONMSG*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  GETRENDERBUFFERALLOCATIONMSG */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONMSG_SIZEOF00
static int tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONMSG_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GETRENDERBUFFERALLOCATIONMSG",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GETRENDERBUFFERALLOCATIONMSG* self = (GETRENDERBUFFERALLOCATIONMSG*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(GETRENDERBUFFERALLOCATIONMSG);
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

/* get function:m_GeometryAllocationsOut of class GETRENDERBUFFERALLOCATIONBYNAMEMSG */
#ifndef TOLUA_DISABLE_tolua_get_GETRENDERBUFFERALLOCATIONBYNAMEMSG_m_GeometryAllocationsOut
static int tolua_get_GETRENDERBUFFERALLOCATIONBYNAMEMSG_m_GeometryAllocationsOut (lua_State* tolua_S) {
  GETRENDERBUFFERALLOCATIONBYNAMEMSG*self = (GETRENDERBUFFERALLOCATIONBYNAMEMSG*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_GeometryAllocationsOut'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->m_GeometryAllocationsOut,"vector<RENDERBUFFERALLOCATION>");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GETRENDERBUFFERALLOCATIONBYNAMEMSG_m_GeometryAllocationsOut */

/* get function:m_GeometryAllocationsOut of class GETRENDERBUFFERALLOCATIONBYNAMEMSG */
#ifndef TOLUA_DISABLE_tolua_set_GETRENDERBUFFERALLOCATIONBYNAMEMSG_m_GeometryAllocationsOut
static int tolua_set_GETRENDERBUFFERALLOCATIONBYNAMEMSG_m_GeometryAllocationsOut (lua_State* tolua_S) {
  GETRENDERBUFFERALLOCATIONBYNAMEMSG*self = (GETRENDERBUFFERALLOCATIONBYNAMEMSG*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_GeometryAllocationsOut'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"vector<RENDERBUFFERALLOCATION>",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_GeometryAllocationsOut=*(( vector<RENDERBUFFERALLOCATION>*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GETRENDERBUFFERALLOCATIONBYNAMEMSG_m_GeometryAllocationsOut */

/* get function:m_hszName of class GETRENDERBUFFERALLOCATIONBYNAMEMSG */
#ifndef TOLUA_DISABLE_tolua_get_GETRENDERBUFFERALLOCATIONBYNAMEMSG_m_hszName_ptr
static int tolua_get_GETRENDERBUFFERALLOCATIONBYNAMEMSG_m_hszName_ptr (lua_State* tolua_S) {
  GETRENDERBUFFERALLOCATIONBYNAMEMSG*self = (GETRENDERBUFFERALLOCATIONBYNAMEMSG*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_hszName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_hszName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GETRENDERBUFFERALLOCATIONBYNAMEMSG_m_hszName_ptr */

/* get function:m_hszName of class GETRENDERBUFFERALLOCATIONBYNAMEMSG */
#ifndef TOLUA_DISABLE_tolua_set_GETRENDERBUFFERALLOCATIONBYNAMEMSG_m_hszName_ptr
static int tolua_set_GETRENDERBUFFERALLOCATIONBYNAMEMSG_m_hszName_ptr (lua_State* tolua_S) {
  GETRENDERBUFFERALLOCATIONBYNAMEMSG*self = (GETRENDERBUFFERALLOCATIONBYNAMEMSG*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_hszName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_hszName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GETRENDERBUFFERALLOCATIONBYNAMEMSG_m_hszName_ptr */

/* method: new of class  GETRENDERBUFFERALLOCATIONBYNAMEMSG */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONBYNAMEMSG_new00
static int tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONBYNAMEMSG_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GETRENDERBUFFERALLOCATIONBYNAMEMSG",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GETRENDERBUFFERALLOCATIONBYNAMEMSG* tolua_ret = (GETRENDERBUFFERALLOCATIONBYNAMEMSG*)  new GETRENDERBUFFERALLOCATIONBYNAMEMSG();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"GETRENDERBUFFERALLOCATIONBYNAMEMSG");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GETRENDERBUFFERALLOCATIONBYNAMEMSG */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONBYNAMEMSG_new00_local
static int tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONBYNAMEMSG_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GETRENDERBUFFERALLOCATIONBYNAMEMSG",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GETRENDERBUFFERALLOCATIONBYNAMEMSG* tolua_ret = (GETRENDERBUFFERALLOCATIONBYNAMEMSG*)  new GETRENDERBUFFERALLOCATIONBYNAMEMSG();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GETRENDERBUFFERALLOCATIONBYNAMEMSG");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  GETRENDERBUFFERALLOCATIONBYNAMEMSG */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONBYNAMEMSG_delete00
static int tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONBYNAMEMSG_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GETRENDERBUFFERALLOCATIONBYNAMEMSG",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GETRENDERBUFFERALLOCATIONBYNAMEMSG* self = (GETRENDERBUFFERALLOCATIONBYNAMEMSG*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  GETRENDERBUFFERALLOCATIONBYNAMEMSG */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONBYNAMEMSG_SIZEOF00
static int tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONBYNAMEMSG_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GETRENDERBUFFERALLOCATIONBYNAMEMSG",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GETRENDERBUFFERALLOCATIONBYNAMEMSG* self = (GETRENDERBUFFERALLOCATIONBYNAMEMSG*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(GETRENDERBUFFERALLOCATIONBYNAMEMSG);
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

/* get function:AnimationId of class REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_get_REGISTERCAL3DANIMATIONCALLBACK_AnimationId
static int tolua_get_REGISTERCAL3DANIMATIONCALLBACK_AnimationId (lua_State* tolua_S) {
  REGISTERCAL3DANIMATIONCALLBACK*self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AnimationId'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->AnimationId);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_REGISTERCAL3DANIMATIONCALLBACK_AnimationId */

/* get function:AnimationId of class REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_set_REGISTERCAL3DANIMATIONCALLBACK_AnimationId
static int tolua_set_REGISTERCAL3DANIMATIONCALLBACK_AnimationId (lua_State* tolua_S) {
  REGISTERCAL3DANIMATIONCALLBACK*self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AnimationId'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->AnimationId=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_REGISTERCAL3DANIMATIONCALLBACK_AnimationId */

/* get function:AnimationTriggerTime of class REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_get_REGISTERCAL3DANIMATIONCALLBACK_AnimationTriggerTime
static int tolua_get_REGISTERCAL3DANIMATIONCALLBACK_AnimationTriggerTime (lua_State* tolua_S) {
  REGISTERCAL3DANIMATIONCALLBACK*self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AnimationTriggerTime'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->AnimationTriggerTime);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_REGISTERCAL3DANIMATIONCALLBACK_AnimationTriggerTime */

/* get function:AnimationTriggerTime of class REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_set_REGISTERCAL3DANIMATIONCALLBACK_AnimationTriggerTime
static int tolua_set_REGISTERCAL3DANIMATIONCALLBACK_AnimationTriggerTime (lua_State* tolua_S) {
  REGISTERCAL3DANIMATIONCALLBACK*self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AnimationTriggerTime'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->AnimationTriggerTime=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_REGISTERCAL3DANIMATIONCALLBACK_AnimationTriggerTime */

/* get function:bTriggerOnComplete of class REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_get_REGISTERCAL3DANIMATIONCALLBACK_bTriggerOnComplete
static int tolua_get_REGISTERCAL3DANIMATIONCALLBACK_bTriggerOnComplete (lua_State* tolua_S) {
  REGISTERCAL3DANIMATIONCALLBACK*self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bTriggerOnComplete'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->bTriggerOnComplete);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_REGISTERCAL3DANIMATIONCALLBACK_bTriggerOnComplete */

/* get function:bTriggerOnComplete of class REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_set_REGISTERCAL3DANIMATIONCALLBACK_bTriggerOnComplete
static int tolua_set_REGISTERCAL3DANIMATIONCALLBACK_bTriggerOnComplete (lua_State* tolua_S) {
  REGISTERCAL3DANIMATIONCALLBACK*self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bTriggerOnComplete'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->bTriggerOnComplete=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_REGISTERCAL3DANIMATIONCALLBACK_bTriggerOnComplete */

/* get function:bTriggerOnStop of class REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_get_REGISTERCAL3DANIMATIONCALLBACK_bTriggerOnStop
static int tolua_get_REGISTERCAL3DANIMATIONCALLBACK_bTriggerOnStop (lua_State* tolua_S) {
  REGISTERCAL3DANIMATIONCALLBACK*self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bTriggerOnStop'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->bTriggerOnStop);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_REGISTERCAL3DANIMATIONCALLBACK_bTriggerOnStop */

/* get function:bTriggerOnStop of class REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_set_REGISTERCAL3DANIMATIONCALLBACK_bTriggerOnStop
static int tolua_set_REGISTERCAL3DANIMATIONCALLBACK_bTriggerOnStop (lua_State* tolua_S) {
  REGISTERCAL3DANIMATIONCALLBACK*self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bTriggerOnStop'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->bTriggerOnStop=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_REGISTERCAL3DANIMATIONCALLBACK_bTriggerOnStop */

/* get function:AnimationName of class REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_get_REGISTERCAL3DANIMATIONCALLBACK_AnimationName_ptr
static int tolua_get_REGISTERCAL3DANIMATIONCALLBACK_AnimationName_ptr (lua_State* tolua_S) {
  REGISTERCAL3DANIMATIONCALLBACK*self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AnimationName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->AnimationName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_REGISTERCAL3DANIMATIONCALLBACK_AnimationName_ptr */

/* get function:AnimationName of class REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_set_REGISTERCAL3DANIMATIONCALLBACK_AnimationName_ptr
static int tolua_set_REGISTERCAL3DANIMATIONCALLBACK_AnimationName_ptr (lua_State* tolua_S) {
  REGISTERCAL3DANIMATIONCALLBACK*self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AnimationName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->AnimationName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_REGISTERCAL3DANIMATIONCALLBACK_AnimationName_ptr */

/* get function:StateObjectName of class REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_get_REGISTERCAL3DANIMATIONCALLBACK_StateObjectName_ptr
static int tolua_get_REGISTERCAL3DANIMATIONCALLBACK_StateObjectName_ptr (lua_State* tolua_S) {
  REGISTERCAL3DANIMATIONCALLBACK*self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'StateObjectName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->StateObjectName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_REGISTERCAL3DANIMATIONCALLBACK_StateObjectName_ptr */

/* get function:StateObjectName of class REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_set_REGISTERCAL3DANIMATIONCALLBACK_StateObjectName_ptr
static int tolua_set_REGISTERCAL3DANIMATIONCALLBACK_StateObjectName_ptr (lua_State* tolua_S) {
  REGISTERCAL3DANIMATIONCALLBACK*self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'StateObjectName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->StateObjectName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_REGISTERCAL3DANIMATIONCALLBACK_StateObjectName_ptr */

/* get function:EventName of class REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_get_REGISTERCAL3DANIMATIONCALLBACK_EventName_ptr
static int tolua_get_REGISTERCAL3DANIMATIONCALLBACK_EventName_ptr (lua_State* tolua_S) {
  REGISTERCAL3DANIMATIONCALLBACK*self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->EventName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_REGISTERCAL3DANIMATIONCALLBACK_EventName_ptr */

/* get function:EventName of class REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_set_REGISTERCAL3DANIMATIONCALLBACK_EventName_ptr
static int tolua_set_REGISTERCAL3DANIMATIONCALLBACK_EventName_ptr (lua_State* tolua_S) {
  REGISTERCAL3DANIMATIONCALLBACK*self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->EventName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_REGISTERCAL3DANIMATIONCALLBACK_EventName_ptr */

/* get function:CustomMessageName of class REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_get_REGISTERCAL3DANIMATIONCALLBACK_CustomMessageName_ptr
static int tolua_get_REGISTERCAL3DANIMATIONCALLBACK_CustomMessageName_ptr (lua_State* tolua_S) {
  REGISTERCAL3DANIMATIONCALLBACK*self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'CustomMessageName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->CustomMessageName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_REGISTERCAL3DANIMATIONCALLBACK_CustomMessageName_ptr */

/* get function:CustomMessageName of class REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_set_REGISTERCAL3DANIMATIONCALLBACK_CustomMessageName_ptr
static int tolua_set_REGISTERCAL3DANIMATIONCALLBACK_CustomMessageName_ptr (lua_State* tolua_S) {
  REGISTERCAL3DANIMATIONCALLBACK*self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'CustomMessageName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->CustomMessageName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_REGISTERCAL3DANIMATIONCALLBACK_CustomMessageName_ptr */

/* get function:EventParamsArchive of class REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_get_REGISTERCAL3DANIMATIONCALLBACK_EventParamsArchive_ptr
static int tolua_get_REGISTERCAL3DANIMATIONCALLBACK_EventParamsArchive_ptr (lua_State* tolua_S) {
  REGISTERCAL3DANIMATIONCALLBACK*self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventParamsArchive'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->EventParamsArchive,"IArchive");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_REGISTERCAL3DANIMATIONCALLBACK_EventParamsArchive_ptr */

/* get function:EventParamsArchive of class REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_set_REGISTERCAL3DANIMATIONCALLBACK_EventParamsArchive_ptr
static int tolua_set_REGISTERCAL3DANIMATIONCALLBACK_EventParamsArchive_ptr (lua_State* tolua_S) {
  REGISTERCAL3DANIMATIONCALLBACK*self = (REGISTERCAL3DANIMATIONCALLBACK*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventParamsArchive'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"IArchive",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->EventParamsArchive=(( IArchive*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_REGISTERCAL3DANIMATIONCALLBACK_EventParamsArchive_ptr */

/* method: new of class  REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_REGISTERCAL3DANIMATIONCALLBACK_new00
static int tolua_Cal3dModelsLua_REGISTERCAL3DANIMATIONCALLBACK_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"REGISTERCAL3DANIMATIONCALLBACK",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  REGISTERCAL3DANIMATIONCALLBACK* tolua_ret = (REGISTERCAL3DANIMATIONCALLBACK*)  new REGISTERCAL3DANIMATIONCALLBACK();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"REGISTERCAL3DANIMATIONCALLBACK");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_REGISTERCAL3DANIMATIONCALLBACK_new00_local
static int tolua_Cal3dModelsLua_REGISTERCAL3DANIMATIONCALLBACK_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"REGISTERCAL3DANIMATIONCALLBACK",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  REGISTERCAL3DANIMATIONCALLBACK* tolua_ret = (REGISTERCAL3DANIMATIONCALLBACK*)  new REGISTERCAL3DANIMATIONCALLBACK();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"REGISTERCAL3DANIMATIONCALLBACK");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_REGISTERCAL3DANIMATIONCALLBACK_delete00
static int tolua_Cal3dModelsLua_REGISTERCAL3DANIMATIONCALLBACK_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"REGISTERCAL3DANIMATIONCALLBACK",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  REGISTERCAL3DANIMATIONCALLBACK* self = (REGISTERCAL3DANIMATIONCALLBACK*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  REGISTERCAL3DANIMATIONCALLBACK */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_REGISTERCAL3DANIMATIONCALLBACK_SIZEOF00
static int tolua_Cal3dModelsLua_REGISTERCAL3DANIMATIONCALLBACK_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"REGISTERCAL3DANIMATIONCALLBACK",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  REGISTERCAL3DANIMATIONCALLBACK* self = (REGISTERCAL3DANIMATIONCALLBACK*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(REGISTERCAL3DANIMATIONCALLBACK);
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

/* get function:LayerNumber of class RENDEROBJECTANIMATIONPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_RENDEROBJECTANIMATIONPARAMS_LayerNumber
static int tolua_get_RENDEROBJECTANIMATIONPARAMS_LayerNumber (lua_State* tolua_S) {
  RENDEROBJECTANIMATIONPARAMS*self = (RENDEROBJECTANIMATIONPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'LayerNumber'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->LayerNumber);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RENDEROBJECTANIMATIONPARAMS_LayerNumber */

/* get function:LayerNumber of class RENDEROBJECTANIMATIONPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_RENDEROBJECTANIMATIONPARAMS_LayerNumber
static int tolua_set_RENDEROBJECTANIMATIONPARAMS_LayerNumber (lua_State* tolua_S) {
  RENDEROBJECTANIMATIONPARAMS*self = (RENDEROBJECTANIMATIONPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'LayerNumber'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->LayerNumber=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RENDEROBJECTANIMATIONPARAMS_LayerNumber */

/* get function:AnimationName of class RENDEROBJECTANIMATIONPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_RENDEROBJECTANIMATIONPARAMS_AnimationName_ptr
static int tolua_get_RENDEROBJECTANIMATIONPARAMS_AnimationName_ptr (lua_State* tolua_S) {
  RENDEROBJECTANIMATIONPARAMS*self = (RENDEROBJECTANIMATIONPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AnimationName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->AnimationName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RENDEROBJECTANIMATIONPARAMS_AnimationName_ptr */

/* get function:AnimationName of class RENDEROBJECTANIMATIONPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_RENDEROBJECTANIMATIONPARAMS_AnimationName_ptr
static int tolua_set_RENDEROBJECTANIMATIONPARAMS_AnimationName_ptr (lua_State* tolua_S) {
  RENDEROBJECTANIMATIONPARAMS*self = (RENDEROBJECTANIMATIONPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AnimationName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->AnimationName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RENDEROBJECTANIMATIONPARAMS_AnimationName_ptr */

/* get function:AnimationID of class RENDEROBJECTANIMATIONPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_RENDEROBJECTANIMATIONPARAMS_AnimationID
static int tolua_get_RENDEROBJECTANIMATIONPARAMS_AnimationID (lua_State* tolua_S) {
  RENDEROBJECTANIMATIONPARAMS*self = (RENDEROBJECTANIMATIONPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AnimationID'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->AnimationID);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RENDEROBJECTANIMATIONPARAMS_AnimationID */

/* get function:AnimationID of class RENDEROBJECTANIMATIONPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_RENDEROBJECTANIMATIONPARAMS_AnimationID
static int tolua_set_RENDEROBJECTANIMATIONPARAMS_AnimationID (lua_State* tolua_S) {
  RENDEROBJECTANIMATIONPARAMS*self = (RENDEROBJECTANIMATIONPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AnimationID'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->AnimationID=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RENDEROBJECTANIMATIONPARAMS_AnimationID */

/* get function:BlendIn of class RENDEROBJECTANIMATIONPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_RENDEROBJECTANIMATIONPARAMS_BlendIn
static int tolua_get_RENDEROBJECTANIMATIONPARAMS_BlendIn (lua_State* tolua_S) {
  RENDEROBJECTANIMATIONPARAMS*self = (RENDEROBJECTANIMATIONPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'BlendIn'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->BlendIn);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RENDEROBJECTANIMATIONPARAMS_BlendIn */

/* get function:BlendIn of class RENDEROBJECTANIMATIONPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_RENDEROBJECTANIMATIONPARAMS_BlendIn
static int tolua_set_RENDEROBJECTANIMATIONPARAMS_BlendIn (lua_State* tolua_S) {
  RENDEROBJECTANIMATIONPARAMS*self = (RENDEROBJECTANIMATIONPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'BlendIn'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->BlendIn=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RENDEROBJECTANIMATIONPARAMS_BlendIn */

/* get function:BlendOut of class RENDEROBJECTANIMATIONPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_RENDEROBJECTANIMATIONPARAMS_BlendOut
static int tolua_get_RENDEROBJECTANIMATIONPARAMS_BlendOut (lua_State* tolua_S) {
  RENDEROBJECTANIMATIONPARAMS*self = (RENDEROBJECTANIMATIONPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'BlendOut'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->BlendOut);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RENDEROBJECTANIMATIONPARAMS_BlendOut */

/* get function:BlendOut of class RENDEROBJECTANIMATIONPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_RENDEROBJECTANIMATIONPARAMS_BlendOut
static int tolua_set_RENDEROBJECTANIMATIONPARAMS_BlendOut (lua_State* tolua_S) {
  RENDEROBJECTANIMATIONPARAMS*self = (RENDEROBJECTANIMATIONPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'BlendOut'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->BlendOut=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RENDEROBJECTANIMATIONPARAMS_BlendOut */

/* get function:bLoop of class RENDEROBJECTANIMATIONPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_RENDEROBJECTANIMATIONPARAMS_bLoop
static int tolua_get_RENDEROBJECTANIMATIONPARAMS_bLoop (lua_State* tolua_S) {
  RENDEROBJECTANIMATIONPARAMS*self = (RENDEROBJECTANIMATIONPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bLoop'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->bLoop);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RENDEROBJECTANIMATIONPARAMS_bLoop */

/* get function:bLoop of class RENDEROBJECTANIMATIONPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_RENDEROBJECTANIMATIONPARAMS_bLoop
static int tolua_set_RENDEROBJECTANIMATIONPARAMS_bLoop (lua_State* tolua_S) {
  RENDEROBJECTANIMATIONPARAMS*self = (RENDEROBJECTANIMATIONPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bLoop'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->bLoop=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RENDEROBJECTANIMATIONPARAMS_bLoop */

/* get function:LockLastFrame of class RENDEROBJECTANIMATIONPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_RENDEROBJECTANIMATIONPARAMS_LockLastFrame
static int tolua_get_RENDEROBJECTANIMATIONPARAMS_LockLastFrame (lua_State* tolua_S) {
  RENDEROBJECTANIMATIONPARAMS*self = (RENDEROBJECTANIMATIONPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'LockLastFrame'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->LockLastFrame);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_RENDEROBJECTANIMATIONPARAMS_LockLastFrame */

/* get function:LockLastFrame of class RENDEROBJECTANIMATIONPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_RENDEROBJECTANIMATIONPARAMS_LockLastFrame
static int tolua_set_RENDEROBJECTANIMATIONPARAMS_LockLastFrame (lua_State* tolua_S) {
  RENDEROBJECTANIMATIONPARAMS*self = (RENDEROBJECTANIMATIONPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'LockLastFrame'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->LockLastFrame=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_RENDEROBJECTANIMATIONPARAMS_LockLastFrame */

/* method: new of class  RENDEROBJECTANIMATIONPARAMS */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_RENDEROBJECTANIMATIONPARAMS_new00
static int tolua_Cal3dModelsLua_RENDEROBJECTANIMATIONPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"RENDEROBJECTANIMATIONPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  RENDEROBJECTANIMATIONPARAMS* tolua_ret = (RENDEROBJECTANIMATIONPARAMS*)  new RENDEROBJECTANIMATIONPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"RENDEROBJECTANIMATIONPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  RENDEROBJECTANIMATIONPARAMS */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_RENDEROBJECTANIMATIONPARAMS_new00_local
static int tolua_Cal3dModelsLua_RENDEROBJECTANIMATIONPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"RENDEROBJECTANIMATIONPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  RENDEROBJECTANIMATIONPARAMS* tolua_ret = (RENDEROBJECTANIMATIONPARAMS*)  new RENDEROBJECTANIMATIONPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"RENDEROBJECTANIMATIONPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  RENDEROBJECTANIMATIONPARAMS */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_RENDEROBJECTANIMATIONPARAMS_delete00
static int tolua_Cal3dModelsLua_RENDEROBJECTANIMATIONPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"RENDEROBJECTANIMATIONPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  RENDEROBJECTANIMATIONPARAMS* self = (RENDEROBJECTANIMATIONPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  RENDEROBJECTANIMATIONPARAMS */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_RENDEROBJECTANIMATIONPARAMS_SIZEOF00
static int tolua_Cal3dModelsLua_RENDEROBJECTANIMATIONPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"RENDEROBJECTANIMATIONPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  RENDEROBJECTANIMATIONPARAMS* self = (RENDEROBJECTANIMATIONPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(RENDEROBJECTANIMATIONPARAMS);
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

/* get function:pGeometry of class GETCLOTHPHYSICSGEOMETRYPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_GETCLOTHPHYSICSGEOMETRYPARAMS_pGeometry_ptr
static int tolua_get_GETCLOTHPHYSICSGEOMETRYPARAMS_pGeometry_ptr (lua_State* tolua_S) {
  GETCLOTHPHYSICSGEOMETRYPARAMS*self = (GETCLOTHPHYSICSGEOMETRYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pGeometry'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->pGeometry,"IGeometry");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GETCLOTHPHYSICSGEOMETRYPARAMS_pGeometry_ptr */

/* get function:pGeometry of class GETCLOTHPHYSICSGEOMETRYPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_GETCLOTHPHYSICSGEOMETRYPARAMS_pGeometry_ptr
static int tolua_set_GETCLOTHPHYSICSGEOMETRYPARAMS_pGeometry_ptr (lua_State* tolua_S) {
  GETCLOTHPHYSICSGEOMETRYPARAMS*self = (GETCLOTHPHYSICSGEOMETRYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pGeometry'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"IGeometry",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->pGeometry=(( IGeometry*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GETCLOTHPHYSICSGEOMETRYPARAMS_pGeometry_ptr */

/* get function:pNodeName of class GETCLOTHPHYSICSGEOMETRYPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_GETCLOTHPHYSICSGEOMETRYPARAMS_pNodeName_ptr
static int tolua_get_GETCLOTHPHYSICSGEOMETRYPARAMS_pNodeName_ptr (lua_State* tolua_S) {
  GETCLOTHPHYSICSGEOMETRYPARAMS*self = (GETCLOTHPHYSICSGEOMETRYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pNodeName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->pNodeName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GETCLOTHPHYSICSGEOMETRYPARAMS_pNodeName_ptr */

/* get function:pNodeName of class GETCLOTHPHYSICSGEOMETRYPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_GETCLOTHPHYSICSGEOMETRYPARAMS_pNodeName_ptr
static int tolua_set_GETCLOTHPHYSICSGEOMETRYPARAMS_pNodeName_ptr (lua_State* tolua_S) {
  GETCLOTHPHYSICSGEOMETRYPARAMS*self = (GETCLOTHPHYSICSGEOMETRYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pNodeName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->pNodeName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GETCLOTHPHYSICSGEOMETRYPARAMS_pNodeName_ptr */

/* method: new of class  GETCLOTHPHYSICSGEOMETRYPARAMS */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETCLOTHPHYSICSGEOMETRYPARAMS_new00
static int tolua_Cal3dModelsLua_GETCLOTHPHYSICSGEOMETRYPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GETCLOTHPHYSICSGEOMETRYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GETCLOTHPHYSICSGEOMETRYPARAMS* tolua_ret = (GETCLOTHPHYSICSGEOMETRYPARAMS*)  new GETCLOTHPHYSICSGEOMETRYPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"GETCLOTHPHYSICSGEOMETRYPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GETCLOTHPHYSICSGEOMETRYPARAMS */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETCLOTHPHYSICSGEOMETRYPARAMS_new00_local
static int tolua_Cal3dModelsLua_GETCLOTHPHYSICSGEOMETRYPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GETCLOTHPHYSICSGEOMETRYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GETCLOTHPHYSICSGEOMETRYPARAMS* tolua_ret = (GETCLOTHPHYSICSGEOMETRYPARAMS*)  new GETCLOTHPHYSICSGEOMETRYPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GETCLOTHPHYSICSGEOMETRYPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  GETCLOTHPHYSICSGEOMETRYPARAMS */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETCLOTHPHYSICSGEOMETRYPARAMS_delete00
static int tolua_Cal3dModelsLua_GETCLOTHPHYSICSGEOMETRYPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GETCLOTHPHYSICSGEOMETRYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GETCLOTHPHYSICSGEOMETRYPARAMS* self = (GETCLOTHPHYSICSGEOMETRYPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  GETCLOTHPHYSICSGEOMETRYPARAMS */
#ifndef TOLUA_DISABLE_tolua_Cal3dModelsLua_GETCLOTHPHYSICSGEOMETRYPARAMS_SIZEOF00
static int tolua_Cal3dModelsLua_GETCLOTHPHYSICSGEOMETRYPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GETCLOTHPHYSICSGEOMETRYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GETCLOTHPHYSICSGEOMETRYPARAMS* self = (GETCLOTHPHYSICSGEOMETRYPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(GETCLOTHPHYSICSGEOMETRYPARAMS);
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
TOLUA_API int tolua_Cal3dModelsLua_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"GETCALPARENT","GETCALPARENT","",tolua_collect_GETCALPARENT);
 #else
 tolua_cclass(tolua_S,"GETCALPARENT","GETCALPARENT","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"GETCALPARENT");
  tolua_variable(tolua_S,"parent",tolua_get_GETCALPARENT_parent_ptr,tolua_set_GETCALPARENT_parent_ptr);
  tolua_function(tolua_S,"new",tolua_Cal3dModelsLua_GETCALPARENT_new00);
  tolua_function(tolua_S,"new_local",tolua_Cal3dModelsLua_GETCALPARENT_new00_local);
  tolua_function(tolua_S,".call",tolua_Cal3dModelsLua_GETCALPARENT_new00_local);
  tolua_function(tolua_S,"delete",tolua_Cal3dModelsLua_GETCALPARENT_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_Cal3dModelsLua_GETCALPARENT_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"ANIMATIONID","ANIMATIONID","",tolua_collect_ANIMATIONID);
 #else
 tolua_cclass(tolua_S,"ANIMATIONID","ANIMATIONID","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"ANIMATIONID");
  tolua_variable(tolua_S,"AnimationName",tolua_get_ANIMATIONID_AnimationName,tolua_set_ANIMATIONID_AnimationName);
  tolua_variable(tolua_S,"AnimationId",tolua_get_ANIMATIONID_AnimationId,tolua_set_ANIMATIONID_AnimationId);
  tolua_function(tolua_S,"new",tolua_Cal3dModelsLua_ANIMATIONID_new00);
  tolua_function(tolua_S,"new_local",tolua_Cal3dModelsLua_ANIMATIONID_new00_local);
  tolua_function(tolua_S,".call",tolua_Cal3dModelsLua_ANIMATIONID_new00_local);
  tolua_function(tolua_S,"delete",tolua_Cal3dModelsLua_ANIMATIONID_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_Cal3dModelsLua_ANIMATIONID_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"GETANIMATIONLISTPARAMS","GETANIMATIONLISTPARAMS","",tolua_collect_GETANIMATIONLISTPARAMS);
 #else
 tolua_cclass(tolua_S,"GETANIMATIONLISTPARAMS","GETANIMATIONLISTPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"GETANIMATIONLISTPARAMS");
  tolua_variable(tolua_S,"numAnimations",tolua_get_GETANIMATIONLISTPARAMS_numAnimations,tolua_set_GETANIMATIONLISTPARAMS_numAnimations);
  tolua_variable(tolua_S,"AnimationList",tolua_get_GETANIMATIONLISTPARAMS_AnimationList_ptr,tolua_set_GETANIMATIONLISTPARAMS_AnimationList_ptr);
  tolua_function(tolua_S,"new",tolua_Cal3dModelsLua_GETANIMATIONLISTPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_Cal3dModelsLua_GETANIMATIONLISTPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_Cal3dModelsLua_GETANIMATIONLISTPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_Cal3dModelsLua_GETANIMATIONLISTPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_Cal3dModelsLua_GETANIMATIONLISTPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"RENDERBUFFERALLOCATION","RENDERBUFFERALLOCATION","",tolua_collect_RENDERBUFFERALLOCATION);
 #else
 tolua_cclass(tolua_S,"RENDERBUFFERALLOCATION","RENDERBUFFERALLOCATION","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"RENDERBUFFERALLOCATION");
  tolua_variable(tolua_S,"m_IndexAllocation",tolua_get_RENDERBUFFERALLOCATION_m_IndexAllocation,tolua_set_RENDERBUFFERALLOCATION_m_IndexAllocation);
  tolua_variable(tolua_S,"m_VertexAllocation",tolua_get_RENDERBUFFERALLOCATION_m_VertexAllocation,tolua_set_RENDERBUFFERALLOCATION_m_VertexAllocation);
  tolua_variable(tolua_S,"m_VertexBufferInterface",tolua_get_RENDERBUFFERALLOCATION_m_VertexBufferInterface_ptr,tolua_set_RENDERBUFFERALLOCATION_m_VertexBufferInterface_ptr);
  tolua_variable(tolua_S,"m_IndexBufferInterface",tolua_get_RENDERBUFFERALLOCATION_m_IndexBufferInterface_ptr,tolua_set_RENDERBUFFERALLOCATION_m_IndexBufferInterface_ptr);
  tolua_variable(tolua_S,"m_Material",tolua_get_RENDERBUFFERALLOCATION_m_Material_ptr,tolua_set_RENDERBUFFERALLOCATION_m_Material_ptr);
  tolua_function(tolua_S,"new",tolua_Cal3dModelsLua_RENDERBUFFERALLOCATION_new00);
  tolua_function(tolua_S,"new_local",tolua_Cal3dModelsLua_RENDERBUFFERALLOCATION_new00_local);
  tolua_function(tolua_S,".call",tolua_Cal3dModelsLua_RENDERBUFFERALLOCATION_new00_local);
  tolua_function(tolua_S,"delete",tolua_Cal3dModelsLua_RENDERBUFFERALLOCATION_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_Cal3dModelsLua_RENDERBUFFERALLOCATION_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"GETRENDERBUFFERALLOCATIONMSG","GETRENDERBUFFERALLOCATIONMSG","",tolua_collect_GETRENDERBUFFERALLOCATIONMSG);
 #else
 tolua_cclass(tolua_S,"GETRENDERBUFFERALLOCATIONMSG","GETRENDERBUFFERALLOCATIONMSG","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"GETRENDERBUFFERALLOCATIONMSG");
  tolua_variable(tolua_S,"m_GeometryAllocationsOut",tolua_get_GETRENDERBUFFERALLOCATIONMSG_m_GeometryAllocationsOut,tolua_set_GETRENDERBUFFERALLOCATIONMSG_m_GeometryAllocationsOut);
  tolua_function(tolua_S,"new",tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONMSG_new00);
  tolua_function(tolua_S,"new_local",tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONMSG_new00_local);
  tolua_function(tolua_S,".call",tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONMSG_new00_local);
  tolua_function(tolua_S,"delete",tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONMSG_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONMSG_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"GETRENDERBUFFERALLOCATIONBYNAMEMSG","GETRENDERBUFFERALLOCATIONBYNAMEMSG","",tolua_collect_GETRENDERBUFFERALLOCATIONBYNAMEMSG);
 #else
 tolua_cclass(tolua_S,"GETRENDERBUFFERALLOCATIONBYNAMEMSG","GETRENDERBUFFERALLOCATIONBYNAMEMSG","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"GETRENDERBUFFERALLOCATIONBYNAMEMSG");
  tolua_variable(tolua_S,"m_GeometryAllocationsOut",tolua_get_GETRENDERBUFFERALLOCATIONBYNAMEMSG_m_GeometryAllocationsOut,tolua_set_GETRENDERBUFFERALLOCATIONBYNAMEMSG_m_GeometryAllocationsOut);
  tolua_variable(tolua_S,"m_hszName",tolua_get_GETRENDERBUFFERALLOCATIONBYNAMEMSG_m_hszName_ptr,tolua_set_GETRENDERBUFFERALLOCATIONBYNAMEMSG_m_hszName_ptr);
  tolua_function(tolua_S,"new",tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONBYNAMEMSG_new00);
  tolua_function(tolua_S,"new_local",tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONBYNAMEMSG_new00_local);
  tolua_function(tolua_S,".call",tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONBYNAMEMSG_new00_local);
  tolua_function(tolua_S,"delete",tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONBYNAMEMSG_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_Cal3dModelsLua_GETRENDERBUFFERALLOCATIONBYNAMEMSG_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"REGISTERCAL3DANIMATIONCALLBACK","REGISTERCAL3DANIMATIONCALLBACK","",tolua_collect_REGISTERCAL3DANIMATIONCALLBACK);
 #else
 tolua_cclass(tolua_S,"REGISTERCAL3DANIMATIONCALLBACK","REGISTERCAL3DANIMATIONCALLBACK","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"REGISTERCAL3DANIMATIONCALLBACK");
  tolua_variable(tolua_S,"AnimationId",tolua_get_REGISTERCAL3DANIMATIONCALLBACK_AnimationId,tolua_set_REGISTERCAL3DANIMATIONCALLBACK_AnimationId);
  tolua_variable(tolua_S,"AnimationTriggerTime",tolua_get_REGISTERCAL3DANIMATIONCALLBACK_AnimationTriggerTime,tolua_set_REGISTERCAL3DANIMATIONCALLBACK_AnimationTriggerTime);
  tolua_variable(tolua_S,"bTriggerOnComplete",tolua_get_REGISTERCAL3DANIMATIONCALLBACK_bTriggerOnComplete,tolua_set_REGISTERCAL3DANIMATIONCALLBACK_bTriggerOnComplete);
  tolua_variable(tolua_S,"bTriggerOnStop",tolua_get_REGISTERCAL3DANIMATIONCALLBACK_bTriggerOnStop,tolua_set_REGISTERCAL3DANIMATIONCALLBACK_bTriggerOnStop);
  tolua_variable(tolua_S,"AnimationName",tolua_get_REGISTERCAL3DANIMATIONCALLBACK_AnimationName_ptr,tolua_set_REGISTERCAL3DANIMATIONCALLBACK_AnimationName_ptr);
  tolua_variable(tolua_S,"StateObjectName",tolua_get_REGISTERCAL3DANIMATIONCALLBACK_StateObjectName_ptr,tolua_set_REGISTERCAL3DANIMATIONCALLBACK_StateObjectName_ptr);
  tolua_variable(tolua_S,"EventName",tolua_get_REGISTERCAL3DANIMATIONCALLBACK_EventName_ptr,tolua_set_REGISTERCAL3DANIMATIONCALLBACK_EventName_ptr);
  tolua_variable(tolua_S,"CustomMessageName",tolua_get_REGISTERCAL3DANIMATIONCALLBACK_CustomMessageName_ptr,tolua_set_REGISTERCAL3DANIMATIONCALLBACK_CustomMessageName_ptr);
  tolua_variable(tolua_S,"EventParamsArchive",tolua_get_REGISTERCAL3DANIMATIONCALLBACK_EventParamsArchive_ptr,tolua_set_REGISTERCAL3DANIMATIONCALLBACK_EventParamsArchive_ptr);
  tolua_function(tolua_S,"new",tolua_Cal3dModelsLua_REGISTERCAL3DANIMATIONCALLBACK_new00);
  tolua_function(tolua_S,"new_local",tolua_Cal3dModelsLua_REGISTERCAL3DANIMATIONCALLBACK_new00_local);
  tolua_function(tolua_S,".call",tolua_Cal3dModelsLua_REGISTERCAL3DANIMATIONCALLBACK_new00_local);
  tolua_function(tolua_S,"delete",tolua_Cal3dModelsLua_REGISTERCAL3DANIMATIONCALLBACK_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_Cal3dModelsLua_REGISTERCAL3DANIMATIONCALLBACK_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"RENDEROBJECTANIMATIONPARAMS","RENDEROBJECTANIMATIONPARAMS","",tolua_collect_RENDEROBJECTANIMATIONPARAMS);
 #else
 tolua_cclass(tolua_S,"RENDEROBJECTANIMATIONPARAMS","RENDEROBJECTANIMATIONPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"RENDEROBJECTANIMATIONPARAMS");
  tolua_variable(tolua_S,"LayerNumber",tolua_get_RENDEROBJECTANIMATIONPARAMS_LayerNumber,tolua_set_RENDEROBJECTANIMATIONPARAMS_LayerNumber);
  tolua_variable(tolua_S,"AnimationName",tolua_get_RENDEROBJECTANIMATIONPARAMS_AnimationName_ptr,tolua_set_RENDEROBJECTANIMATIONPARAMS_AnimationName_ptr);
  tolua_variable(tolua_S,"AnimationID",tolua_get_RENDEROBJECTANIMATIONPARAMS_AnimationID,tolua_set_RENDEROBJECTANIMATIONPARAMS_AnimationID);
  tolua_variable(tolua_S,"BlendIn",tolua_get_RENDEROBJECTANIMATIONPARAMS_BlendIn,tolua_set_RENDEROBJECTANIMATIONPARAMS_BlendIn);
  tolua_variable(tolua_S,"BlendOut",tolua_get_RENDEROBJECTANIMATIONPARAMS_BlendOut,tolua_set_RENDEROBJECTANIMATIONPARAMS_BlendOut);
  tolua_variable(tolua_S,"bLoop",tolua_get_RENDEROBJECTANIMATIONPARAMS_bLoop,tolua_set_RENDEROBJECTANIMATIONPARAMS_bLoop);
  tolua_variable(tolua_S,"LockLastFrame",tolua_get_RENDEROBJECTANIMATIONPARAMS_LockLastFrame,tolua_set_RENDEROBJECTANIMATIONPARAMS_LockLastFrame);
  tolua_function(tolua_S,"new",tolua_Cal3dModelsLua_RENDEROBJECTANIMATIONPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_Cal3dModelsLua_RENDEROBJECTANIMATIONPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_Cal3dModelsLua_RENDEROBJECTANIMATIONPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_Cal3dModelsLua_RENDEROBJECTANIMATIONPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_Cal3dModelsLua_RENDEROBJECTANIMATIONPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"GETCLOTHPHYSICSGEOMETRYPARAMS","GETCLOTHPHYSICSGEOMETRYPARAMS","",tolua_collect_GETCLOTHPHYSICSGEOMETRYPARAMS);
 #else
 tolua_cclass(tolua_S,"GETCLOTHPHYSICSGEOMETRYPARAMS","GETCLOTHPHYSICSGEOMETRYPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"GETCLOTHPHYSICSGEOMETRYPARAMS");
  tolua_variable(tolua_S,"pGeometry",tolua_get_GETCLOTHPHYSICSGEOMETRYPARAMS_pGeometry_ptr,tolua_set_GETCLOTHPHYSICSGEOMETRYPARAMS_pGeometry_ptr);
  tolua_variable(tolua_S,"pNodeName",tolua_get_GETCLOTHPHYSICSGEOMETRYPARAMS_pNodeName_ptr,tolua_set_GETCLOTHPHYSICSGEOMETRYPARAMS_pNodeName_ptr);
  tolua_function(tolua_S,"new",tolua_Cal3dModelsLua_GETCLOTHPHYSICSGEOMETRYPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_Cal3dModelsLua_GETCLOTHPHYSICSGEOMETRYPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_Cal3dModelsLua_GETCLOTHPHYSICSGEOMETRYPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_Cal3dModelsLua_GETCLOTHPHYSICSGEOMETRYPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_Cal3dModelsLua_GETCLOTHPHYSICSGEOMETRYPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Cal3dModelsLua (lua_State* tolua_S) {
 return tolua_Cal3dModelsLua_open(tolua_S);
};
#endif

