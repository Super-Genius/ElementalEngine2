/*
** Lua binding: GUIScript
** Generated automatically by toluaxx-1.2.0 on 02/11/09 20:58:49.
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
TOLUA_API int tolua_GUIScript_open (lua_State* tolua_S);

#include "StdAfx.h"
#include "../../ElementalEngine/EELuaScript/LuaHashString.h"
#pragma warning(disable : 4800)
#pragma warning(disable : 4345)
class CLuaDWORD { public: CLuaDWORD() {} CLuaDWORD(unsigned long i) { x = i; } operator DWORD() { return x; } void operator=(DWORD i) { x = i; } unsigned long x; };
#if 0 // To not include the "../"
#include "../../include/GUI/GUIDefines.h"
#endif

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_BUTTONIMAGE (lua_State* tolua_S)
{
 BUTTONIMAGE* self = (BUTTONIMAGE*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_GUIFLOAT (lua_State* tolua_S)
{
 GUIFLOAT* self = (GUIFLOAT*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_ADDLISTBOXENTRY (lua_State* tolua_S)
{
 ADDLISTBOXENTRY* self = (ADDLISTBOXENTRY*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_GUIPAGEMESSAGE (lua_State* tolua_S)
{
 GUIPAGEMESSAGE* self = (GUIPAGEMESSAGE*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_SELECTRECT (lua_State* tolua_S)
{
 SELECTRECT* self = (SELECTRECT*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_COMBODATA (lua_State* tolua_S)
{
 COMBODATA* self = (COMBODATA*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_GUIBOOL (lua_State* tolua_S)
{
 GUIBOOL* self = (GUIBOOL*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_OBJINRECT (lua_State* tolua_S)
{
 OBJINRECT* self = (OBJINRECT*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CLuaDWORD (lua_State* tolua_S)
{
 CLuaDWORD* self = (CLuaDWORD*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CHANGELANGUAGEKEYPARAMS (lua_State* tolua_S)
{
 CHANGELANGUAGEKEYPARAMS* self = (CHANGELANGUAGEKEYPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_GUIEVENT (lua_State* tolua_S)
{
 GUIEVENT* self = (GUIEVENT*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CURSORVISIBLE (lua_State* tolua_S)
{
 CURSORVISIBLE* self = (CURSORVISIBLE*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_TEXTDATA (lua_State* tolua_S)
{
 TEXTDATA* self = (TEXTDATA*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CHANGECURSOR (lua_State* tolua_S)
{
 CHANGECURSOR* self = (CHANGECURSOR*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_OBJUNDERCURSOR (lua_State* tolua_S)
{
 OBJUNDERCURSOR* self = (OBJUNDERCURSOR*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_GUIQUERYBYNAME (lua_State* tolua_S)
{
 GUIQUERYBYNAME* self = (GUIQUERYBYNAME*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_ZORDERDATA (lua_State* tolua_S)
{
 ZORDERDATA* self = (ZORDERDATA*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CHANGECURSOR");
 tolua_usertype(tolua_S,"GUIFLOAT");
 tolua_usertype(tolua_S,"ADDLISTBOXENTRY");
 tolua_usertype(tolua_S,"GUIPAGEMESSAGE");
 tolua_usertype(tolua_S,"SELECTRECT");
 tolua_usertype(tolua_S,"COMBODATA");
 tolua_usertype(tolua_S,"IArchive");
 tolua_usertype(tolua_S,"GUIBOOL");
 tolua_usertype(tolua_S,"CLuaHashString");
 tolua_usertype(tolua_S,"CLuaDWORD");
 tolua_usertype(tolua_S,"CHANGELANGUAGEKEYPARAMS");
 tolua_usertype(tolua_S,"GUIEVENT");
 tolua_usertype(tolua_S,"CURSORVISIBLE");
 tolua_usertype(tolua_S,"StdString");
 tolua_usertype(tolua_S,"TEXTDATA");
 tolua_usertype(tolua_S,"GUIQUERYBYNAME");
 tolua_usertype(tolua_S,"OBJECTLIST");
 tolua_usertype(tolua_S,"OBJINRECT");
 tolua_usertype(tolua_S,"OBJUNDERCURSOR");
 tolua_usertype(tolua_S,"BUTTONIMAGE");
 tolua_usertype(tolua_S,"ZORDERDATA");
}

/* get function:event of class GUIEVENT */
#ifndef TOLUA_DISABLE_tolua_get_GUIEVENT_event
static int tolua_get_GUIEVENT_event (lua_State* tolua_S) {
  GUIEVENT*self = (GUIEVENT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'event'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->event,"CLuaDWORD");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GUIEVENT_event */

/* get function:event of class GUIEVENT */
#ifndef TOLUA_DISABLE_tolua_set_GUIEVENT_event
static int tolua_set_GUIEVENT_event (lua_State* tolua_S) {
  GUIEVENT*self = (GUIEVENT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'event'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaDWORD",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->event=*((  CLuaDWORD*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GUIEVENT_event */

/* get function:ar of class GUIEVENT */
#ifndef TOLUA_DISABLE_tolua_get_GUIEVENT_ar_ptr
static int tolua_get_GUIEVENT_ar_ptr (lua_State* tolua_S) {
  GUIEVENT*self = (GUIEVENT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ar'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->ar,"IArchive");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GUIEVENT_ar_ptr */

/* get function:ar of class GUIEVENT */
#ifndef TOLUA_DISABLE_tolua_set_GUIEVENT_ar_ptr
static int tolua_set_GUIEVENT_ar_ptr (lua_State* tolua_S) {
  GUIEVENT*self = (GUIEVENT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ar'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"IArchive",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->ar=(( IArchive*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GUIEVENT_ar_ptr */

/* method: new of class  GUIEVENT */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIEVENT_new00
static int tolua_GUIScript_GUIEVENT_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GUIEVENT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GUIEVENT* tolua_ret = (GUIEVENT*)  new GUIEVENT();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"GUIEVENT");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GUIEVENT */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIEVENT_new00_local
static int tolua_GUIScript_GUIEVENT_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GUIEVENT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GUIEVENT* tolua_ret = (GUIEVENT*)  new GUIEVENT();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GUIEVENT");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  GUIEVENT */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIEVENT_delete00
static int tolua_GUIScript_GUIEVENT_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GUIEVENT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GUIEVENT* self = (GUIEVENT*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  GUIEVENT */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIEVENT_SIZEOF00
static int tolua_GUIScript_GUIEVENT_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GUIEVENT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GUIEVENT* self = (GUIEVENT*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(GUIEVENT);
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

/* get function:data of class GUIFLOAT */
#ifndef TOLUA_DISABLE_tolua_get_GUIFLOAT_data
static int tolua_get_GUIFLOAT_data (lua_State* tolua_S) {
  GUIFLOAT*self = (GUIFLOAT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'data'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->data);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GUIFLOAT_data */

/* get function:data of class GUIFLOAT */
#ifndef TOLUA_DISABLE_tolua_set_GUIFLOAT_data
static int tolua_set_GUIFLOAT_data (lua_State* tolua_S) {
  GUIFLOAT*self = (GUIFLOAT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'data'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->data=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GUIFLOAT_data */

/* method: new of class  GUIFLOAT */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIFLOAT_new00
static int tolua_GUIScript_GUIFLOAT_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GUIFLOAT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GUIFLOAT* tolua_ret = (GUIFLOAT*)  new GUIFLOAT();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"GUIFLOAT");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GUIFLOAT */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIFLOAT_new00_local
static int tolua_GUIScript_GUIFLOAT_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GUIFLOAT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GUIFLOAT* tolua_ret = (GUIFLOAT*)  new GUIFLOAT();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GUIFLOAT");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  GUIFLOAT */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIFLOAT_delete00
static int tolua_GUIScript_GUIFLOAT_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GUIFLOAT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GUIFLOAT* self = (GUIFLOAT*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  GUIFLOAT */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIFLOAT_SIZEOF00
static int tolua_GUIScript_GUIFLOAT_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GUIFLOAT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GUIFLOAT* self = (GUIFLOAT*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(GUIFLOAT);
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

/* get function:name of class GUIPAGEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_get_GUIPAGEMESSAGE_name_ptr
static int tolua_get_GUIPAGEMESSAGE_name_ptr (lua_State* tolua_S) {
  GUIPAGEMESSAGE*self = (GUIPAGEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->name,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GUIPAGEMESSAGE_name_ptr */

/* get function:name of class GUIPAGEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_set_GUIPAGEMESSAGE_name_ptr
static int tolua_set_GUIPAGEMESSAGE_name_ptr (lua_State* tolua_S) {
  GUIPAGEMESSAGE*self = (GUIPAGEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->name=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GUIPAGEMESSAGE_name_ptr */

/* get function:compType of class GUIPAGEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_get_GUIPAGEMESSAGE_compType_ptr
static int tolua_get_GUIPAGEMESSAGE_compType_ptr (lua_State* tolua_S) {
  GUIPAGEMESSAGE*self = (GUIPAGEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'compType'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->compType,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GUIPAGEMESSAGE_compType_ptr */

/* get function:compType of class GUIPAGEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_set_GUIPAGEMESSAGE_compType_ptr
static int tolua_set_GUIPAGEMESSAGE_compType_ptr (lua_State* tolua_S) {
  GUIPAGEMESSAGE*self = (GUIPAGEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'compType'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->compType=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GUIPAGEMESSAGE_compType_ptr */

/* get function:wasFoundInPage of class GUIPAGEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_get_GUIPAGEMESSAGE_wasFoundInPage
static int tolua_get_GUIPAGEMESSAGE_wasFoundInPage (lua_State* tolua_S) {
  GUIPAGEMESSAGE*self = (GUIPAGEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wasFoundInPage'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->wasFoundInPage);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GUIPAGEMESSAGE_wasFoundInPage */

/* get function:wasFoundInPage of class GUIPAGEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_set_GUIPAGEMESSAGE_wasFoundInPage
static int tolua_set_GUIPAGEMESSAGE_wasFoundInPage (lua_State* tolua_S) {
  GUIPAGEMESSAGE*self = (GUIPAGEMESSAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wasFoundInPage'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->wasFoundInPage=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GUIPAGEMESSAGE_wasFoundInPage */

/* method: new of class  GUIPAGEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIPAGEMESSAGE_new00
static int tolua_GUIScript_GUIPAGEMESSAGE_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GUIPAGEMESSAGE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GUIPAGEMESSAGE* tolua_ret = (GUIPAGEMESSAGE*)  new GUIPAGEMESSAGE();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"GUIPAGEMESSAGE");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GUIPAGEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIPAGEMESSAGE_new00_local
static int tolua_GUIScript_GUIPAGEMESSAGE_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GUIPAGEMESSAGE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GUIPAGEMESSAGE* tolua_ret = (GUIPAGEMESSAGE*)  new GUIPAGEMESSAGE();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GUIPAGEMESSAGE");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  GUIPAGEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIPAGEMESSAGE_delete00
static int tolua_GUIScript_GUIPAGEMESSAGE_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GUIPAGEMESSAGE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GUIPAGEMESSAGE* self = (GUIPAGEMESSAGE*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  GUIPAGEMESSAGE */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIPAGEMESSAGE_SIZEOF00
static int tolua_GUIScript_GUIPAGEMESSAGE_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GUIPAGEMESSAGE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GUIPAGEMESSAGE* self = (GUIPAGEMESSAGE*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(GUIPAGEMESSAGE);
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

/* get function:szNorm of class BUTTONIMAGE */
#ifndef TOLUA_DISABLE_tolua_get_BUTTONIMAGE_szNorm
static int tolua_get_BUTTONIMAGE_szNorm (lua_State* tolua_S) {
  BUTTONIMAGE*self = (BUTTONIMAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szNorm'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->szNorm,"StdString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_BUTTONIMAGE_szNorm */

/* get function:szNorm of class BUTTONIMAGE */
#ifndef TOLUA_DISABLE_tolua_set_BUTTONIMAGE_szNorm
static int tolua_set_BUTTONIMAGE_szNorm (lua_State* tolua_S) {
  BUTTONIMAGE*self = (BUTTONIMAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szNorm'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"StdString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->szNorm=*(( StdString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_BUTTONIMAGE_szNorm */

/* get function:szHigh of class BUTTONIMAGE */
#ifndef TOLUA_DISABLE_tolua_get_BUTTONIMAGE_szHigh
static int tolua_get_BUTTONIMAGE_szHigh (lua_State* tolua_S) {
  BUTTONIMAGE*self = (BUTTONIMAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szHigh'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->szHigh,"StdString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_BUTTONIMAGE_szHigh */

/* get function:szHigh of class BUTTONIMAGE */
#ifndef TOLUA_DISABLE_tolua_set_BUTTONIMAGE_szHigh
static int tolua_set_BUTTONIMAGE_szHigh (lua_State* tolua_S) {
  BUTTONIMAGE*self = (BUTTONIMAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szHigh'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"StdString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->szHigh=*(( StdString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_BUTTONIMAGE_szHigh */

/* get function:szSelect of class BUTTONIMAGE */
#ifndef TOLUA_DISABLE_tolua_get_BUTTONIMAGE_szSelect
static int tolua_get_BUTTONIMAGE_szSelect (lua_State* tolua_S) {
  BUTTONIMAGE*self = (BUTTONIMAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szSelect'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->szSelect,"StdString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_BUTTONIMAGE_szSelect */

/* get function:szSelect of class BUTTONIMAGE */
#ifndef TOLUA_DISABLE_tolua_set_BUTTONIMAGE_szSelect
static int tolua_set_BUTTONIMAGE_szSelect (lua_State* tolua_S) {
  BUTTONIMAGE*self = (BUTTONIMAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szSelect'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"StdString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->szSelect=*(( StdString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_BUTTONIMAGE_szSelect */

/* get function:szGray of class BUTTONIMAGE */
#ifndef TOLUA_DISABLE_tolua_get_BUTTONIMAGE_szGray
static int tolua_get_BUTTONIMAGE_szGray (lua_State* tolua_S) {
  BUTTONIMAGE*self = (BUTTONIMAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szGray'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->szGray,"StdString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_BUTTONIMAGE_szGray */

/* get function:szGray of class BUTTONIMAGE */
#ifndef TOLUA_DISABLE_tolua_set_BUTTONIMAGE_szGray
static int tolua_set_BUTTONIMAGE_szGray (lua_State* tolua_S) {
  BUTTONIMAGE*self = (BUTTONIMAGE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szGray'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"StdString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->szGray=*(( StdString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_BUTTONIMAGE_szGray */

/* method: new of class  BUTTONIMAGE */
#ifndef TOLUA_DISABLE_tolua_GUIScript_BUTTONIMAGE_new00
static int tolua_GUIScript_BUTTONIMAGE_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"BUTTONIMAGE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  BUTTONIMAGE* tolua_ret = (BUTTONIMAGE*)  new BUTTONIMAGE();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"BUTTONIMAGE");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  BUTTONIMAGE */
#ifndef TOLUA_DISABLE_tolua_GUIScript_BUTTONIMAGE_new00_local
static int tolua_GUIScript_BUTTONIMAGE_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"BUTTONIMAGE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  BUTTONIMAGE* tolua_ret = (BUTTONIMAGE*)  new BUTTONIMAGE();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"BUTTONIMAGE");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  BUTTONIMAGE */
#ifndef TOLUA_DISABLE_tolua_GUIScript_BUTTONIMAGE_delete00
static int tolua_GUIScript_BUTTONIMAGE_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"BUTTONIMAGE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  BUTTONIMAGE* self = (BUTTONIMAGE*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  BUTTONIMAGE */
#ifndef TOLUA_DISABLE_tolua_GUIScript_BUTTONIMAGE_SIZEOF00
static int tolua_GUIScript_BUTTONIMAGE_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"BUTTONIMAGE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  BUTTONIMAGE* self = (BUTTONIMAGE*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(BUTTONIMAGE);
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

/* get function:iSizeX of class CHANGECURSOR */
#ifndef TOLUA_DISABLE_tolua_get_CHANGECURSOR_iSizeX
static int tolua_get_CHANGECURSOR_iSizeX (lua_State* tolua_S) {
  CHANGECURSOR*self = (CHANGECURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'iSizeX'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->iSizeX);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_CHANGECURSOR_iSizeX */

/* get function:iSizeX of class CHANGECURSOR */
#ifndef TOLUA_DISABLE_tolua_set_CHANGECURSOR_iSizeX
static int tolua_set_CHANGECURSOR_iSizeX (lua_State* tolua_S) {
  CHANGECURSOR*self = (CHANGECURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'iSizeX'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->iSizeX=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_CHANGECURSOR_iSizeX */

/* get function:iSizeY of class CHANGECURSOR */
#ifndef TOLUA_DISABLE_tolua_get_CHANGECURSOR_iSizeY
static int tolua_get_CHANGECURSOR_iSizeY (lua_State* tolua_S) {
  CHANGECURSOR*self = (CHANGECURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'iSizeY'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->iSizeY);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_CHANGECURSOR_iSizeY */

/* get function:iSizeY of class CHANGECURSOR */
#ifndef TOLUA_DISABLE_tolua_set_CHANGECURSOR_iSizeY
static int tolua_set_CHANGECURSOR_iSizeY (lua_State* tolua_S) {
  CHANGECURSOR*self = (CHANGECURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'iSizeY'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->iSizeY=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_CHANGECURSOR_iSizeY */

/* get function:iHotX of class CHANGECURSOR */
#ifndef TOLUA_DISABLE_tolua_get_CHANGECURSOR_iHotX
static int tolua_get_CHANGECURSOR_iHotX (lua_State* tolua_S) {
  CHANGECURSOR*self = (CHANGECURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'iHotX'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->iHotX);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_CHANGECURSOR_iHotX */

/* get function:iHotX of class CHANGECURSOR */
#ifndef TOLUA_DISABLE_tolua_set_CHANGECURSOR_iHotX
static int tolua_set_CHANGECURSOR_iHotX (lua_State* tolua_S) {
  CHANGECURSOR*self = (CHANGECURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'iHotX'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->iHotX=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_CHANGECURSOR_iHotX */

/* get function:iHotY of class CHANGECURSOR */
#ifndef TOLUA_DISABLE_tolua_get_CHANGECURSOR_iHotY
static int tolua_get_CHANGECURSOR_iHotY (lua_State* tolua_S) {
  CHANGECURSOR*self = (CHANGECURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'iHotY'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->iHotY);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_CHANGECURSOR_iHotY */

/* get function:iHotY of class CHANGECURSOR */
#ifndef TOLUA_DISABLE_tolua_set_CHANGECURSOR_iHotY
static int tolua_set_CHANGECURSOR_iHotY (lua_State* tolua_S) {
  CHANGECURSOR*self = (CHANGECURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'iHotY'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->iHotY=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_CHANGECURSOR_iHotY */

/* get function:pTexture of class CHANGECURSOR */
#ifndef TOLUA_DISABLE_tolua_get_CHANGECURSOR_pTexture
static int tolua_get_CHANGECURSOR_pTexture (lua_State* tolua_S) {
  CHANGECURSOR*self = (CHANGECURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pTexture'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->pTexture,"StdString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_CHANGECURSOR_pTexture */

/* get function:pTexture of class CHANGECURSOR */
#ifndef TOLUA_DISABLE_tolua_set_CHANGECURSOR_pTexture
static int tolua_set_CHANGECURSOR_pTexture (lua_State* tolua_S) {
  CHANGECURSOR*self = (CHANGECURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pTexture'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"StdString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->pTexture=*(( StdString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_CHANGECURSOR_pTexture */

/* method: new of class  CHANGECURSOR */
#ifndef TOLUA_DISABLE_tolua_GUIScript_CHANGECURSOR_new00
static int tolua_GUIScript_CHANGECURSOR_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CHANGECURSOR",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  CHANGECURSOR* tolua_ret = (CHANGECURSOR*)  new CHANGECURSOR();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CHANGECURSOR");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CHANGECURSOR */
#ifndef TOLUA_DISABLE_tolua_GUIScript_CHANGECURSOR_new00_local
static int tolua_GUIScript_CHANGECURSOR_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CHANGECURSOR",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  CHANGECURSOR* tolua_ret = (CHANGECURSOR*)  new CHANGECURSOR();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CHANGECURSOR");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CHANGECURSOR */
#ifndef TOLUA_DISABLE_tolua_GUIScript_CHANGECURSOR_delete00
static int tolua_GUIScript_CHANGECURSOR_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CHANGECURSOR",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CHANGECURSOR* self = (CHANGECURSOR*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  CHANGECURSOR */
#ifndef TOLUA_DISABLE_tolua_GUIScript_CHANGECURSOR_SIZEOF00
static int tolua_GUIScript_CHANGECURSOR_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CHANGECURSOR",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CHANGECURSOR* self = (CHANGECURSOR*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(CHANGECURSOR);
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

/* get function:entryText of class ADDLISTBOXENTRY */
#ifndef TOLUA_DISABLE_tolua_get_ADDLISTBOXENTRY_entryText
static int tolua_get_ADDLISTBOXENTRY_entryText (lua_State* tolua_S) {
  ADDLISTBOXENTRY*self = (ADDLISTBOXENTRY*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'entryText'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->entryText,"StdString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ADDLISTBOXENTRY_entryText */

/* get function:entryText of class ADDLISTBOXENTRY */
#ifndef TOLUA_DISABLE_tolua_set_ADDLISTBOXENTRY_entryText
static int tolua_set_ADDLISTBOXENTRY_entryText (lua_State* tolua_S) {
  ADDLISTBOXENTRY*self = (ADDLISTBOXENTRY*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'entryText'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"StdString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->entryText=*(( StdString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ADDLISTBOXENTRY_entryText */

/* get function:iRedColor of class ADDLISTBOXENTRY */
#ifndef TOLUA_DISABLE_tolua_get_ADDLISTBOXENTRY_iRedColor
static int tolua_get_ADDLISTBOXENTRY_iRedColor (lua_State* tolua_S) {
  ADDLISTBOXENTRY*self = (ADDLISTBOXENTRY*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'iRedColor'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->iRedColor);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ADDLISTBOXENTRY_iRedColor */

/* get function:iRedColor of class ADDLISTBOXENTRY */
#ifndef TOLUA_DISABLE_tolua_set_ADDLISTBOXENTRY_iRedColor
static int tolua_set_ADDLISTBOXENTRY_iRedColor (lua_State* tolua_S) {
  ADDLISTBOXENTRY*self = (ADDLISTBOXENTRY*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'iRedColor'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->iRedColor=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ADDLISTBOXENTRY_iRedColor */

/* get function:iGreenColor of class ADDLISTBOXENTRY */
#ifndef TOLUA_DISABLE_tolua_get_ADDLISTBOXENTRY_iGreenColor
static int tolua_get_ADDLISTBOXENTRY_iGreenColor (lua_State* tolua_S) {
  ADDLISTBOXENTRY*self = (ADDLISTBOXENTRY*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'iGreenColor'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->iGreenColor);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ADDLISTBOXENTRY_iGreenColor */

/* get function:iGreenColor of class ADDLISTBOXENTRY */
#ifndef TOLUA_DISABLE_tolua_set_ADDLISTBOXENTRY_iGreenColor
static int tolua_set_ADDLISTBOXENTRY_iGreenColor (lua_State* tolua_S) {
  ADDLISTBOXENTRY*self = (ADDLISTBOXENTRY*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'iGreenColor'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->iGreenColor=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ADDLISTBOXENTRY_iGreenColor */

/* get function:iBlueColor of class ADDLISTBOXENTRY */
#ifndef TOLUA_DISABLE_tolua_get_ADDLISTBOXENTRY_iBlueColor
static int tolua_get_ADDLISTBOXENTRY_iBlueColor (lua_State* tolua_S) {
  ADDLISTBOXENTRY*self = (ADDLISTBOXENTRY*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'iBlueColor'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->iBlueColor);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ADDLISTBOXENTRY_iBlueColor */

/* get function:iBlueColor of class ADDLISTBOXENTRY */
#ifndef TOLUA_DISABLE_tolua_set_ADDLISTBOXENTRY_iBlueColor
static int tolua_set_ADDLISTBOXENTRY_iBlueColor (lua_State* tolua_S) {
  ADDLISTBOXENTRY*self = (ADDLISTBOXENTRY*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'iBlueColor'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->iBlueColor=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ADDLISTBOXENTRY_iBlueColor */

/* method: new of class  ADDLISTBOXENTRY */
#ifndef TOLUA_DISABLE_tolua_GUIScript_ADDLISTBOXENTRY_new00
static int tolua_GUIScript_ADDLISTBOXENTRY_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"ADDLISTBOXENTRY",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  ADDLISTBOXENTRY* tolua_ret = (ADDLISTBOXENTRY*)  new ADDLISTBOXENTRY();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"ADDLISTBOXENTRY");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  ADDLISTBOXENTRY */
#ifndef TOLUA_DISABLE_tolua_GUIScript_ADDLISTBOXENTRY_new00_local
static int tolua_GUIScript_ADDLISTBOXENTRY_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"ADDLISTBOXENTRY",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  ADDLISTBOXENTRY* tolua_ret = (ADDLISTBOXENTRY*)  new ADDLISTBOXENTRY();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"ADDLISTBOXENTRY");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  ADDLISTBOXENTRY */
#ifndef TOLUA_DISABLE_tolua_GUIScript_ADDLISTBOXENTRY_delete00
static int tolua_GUIScript_ADDLISTBOXENTRY_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"ADDLISTBOXENTRY",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  ADDLISTBOXENTRY* self = (ADDLISTBOXENTRY*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  ADDLISTBOXENTRY */
#ifndef TOLUA_DISABLE_tolua_GUIScript_ADDLISTBOXENTRY_SIZEOF00
static int tolua_GUIScript_ADDLISTBOXENTRY_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"ADDLISTBOXENTRY",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  ADDLISTBOXENTRY* self = (ADDLISTBOXENTRY*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(ADDLISTBOXENTRY);
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

/* get function:bVisible of class CURSORVISIBLE */
#ifndef TOLUA_DISABLE_tolua_get_CURSORVISIBLE_bVisible
static int tolua_get_CURSORVISIBLE_bVisible (lua_State* tolua_S) {
  CURSORVISIBLE*self = (CURSORVISIBLE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bVisible'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->bVisible);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_CURSORVISIBLE_bVisible */

/* get function:bVisible of class CURSORVISIBLE */
#ifndef TOLUA_DISABLE_tolua_set_CURSORVISIBLE_bVisible
static int tolua_set_CURSORVISIBLE_bVisible (lua_State* tolua_S) {
  CURSORVISIBLE*self = (CURSORVISIBLE*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bVisible'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->bVisible=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_CURSORVISIBLE_bVisible */

/* method: new of class  CURSORVISIBLE */
#ifndef TOLUA_DISABLE_tolua_GUIScript_CURSORVISIBLE_new00
static int tolua_GUIScript_CURSORVISIBLE_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CURSORVISIBLE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  CURSORVISIBLE* tolua_ret = (CURSORVISIBLE*)  new CURSORVISIBLE();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CURSORVISIBLE");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CURSORVISIBLE */
#ifndef TOLUA_DISABLE_tolua_GUIScript_CURSORVISIBLE_new00_local
static int tolua_GUIScript_CURSORVISIBLE_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CURSORVISIBLE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  CURSORVISIBLE* tolua_ret = (CURSORVISIBLE*)  new CURSORVISIBLE();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CURSORVISIBLE");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CURSORVISIBLE */
#ifndef TOLUA_DISABLE_tolua_GUIScript_CURSORVISIBLE_delete00
static int tolua_GUIScript_CURSORVISIBLE_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CURSORVISIBLE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CURSORVISIBLE* self = (CURSORVISIBLE*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  CURSORVISIBLE */
#ifndef TOLUA_DISABLE_tolua_GUIScript_CURSORVISIBLE_SIZEOF00
static int tolua_GUIScript_CURSORVISIBLE_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CURSORVISIBLE",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CURSORVISIBLE* self = (CURSORVISIBLE*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(CURSORVISIBLE);
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

/* get function:text of class COMBODATA */
#ifndef TOLUA_DISABLE_tolua_get_COMBODATA_text
static int tolua_get_COMBODATA_text (lua_State* tolua_S) {
  COMBODATA*self = (COMBODATA*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'text'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->text,"StdString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_COMBODATA_text */

/* get function:text of class COMBODATA */
#ifndef TOLUA_DISABLE_tolua_set_COMBODATA_text
static int tolua_set_COMBODATA_text (lua_State* tolua_S) {
  COMBODATA*self = (COMBODATA*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'text'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"StdString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->text=*(( StdString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_COMBODATA_text */

/* get function:index of class COMBODATA */
#ifndef TOLUA_DISABLE_tolua_get_COMBODATA_index
static int tolua_get_COMBODATA_index (lua_State* tolua_S) {
  COMBODATA*self = (COMBODATA*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'index'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->index);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_COMBODATA_index */

/* get function:index of class COMBODATA */
#ifndef TOLUA_DISABLE_tolua_set_COMBODATA_index
static int tolua_set_COMBODATA_index (lua_State* tolua_S) {
  COMBODATA*self = (COMBODATA*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'index'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->index=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_COMBODATA_index */

/* method: new of class  COMBODATA */
#ifndef TOLUA_DISABLE_tolua_GUIScript_COMBODATA_new00
static int tolua_GUIScript_COMBODATA_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"COMBODATA",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  COMBODATA* tolua_ret = (COMBODATA*)  new COMBODATA();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"COMBODATA");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  COMBODATA */
#ifndef TOLUA_DISABLE_tolua_GUIScript_COMBODATA_new00_local
static int tolua_GUIScript_COMBODATA_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"COMBODATA",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  COMBODATA* tolua_ret = (COMBODATA*)  new COMBODATA();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"COMBODATA");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  COMBODATA */
#ifndef TOLUA_DISABLE_tolua_GUIScript_COMBODATA_delete00
static int tolua_GUIScript_COMBODATA_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"COMBODATA",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  COMBODATA* self = (COMBODATA*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  COMBODATA */
#ifndef TOLUA_DISABLE_tolua_GUIScript_COMBODATA_SIZEOF00
static int tolua_GUIScript_COMBODATA_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"COMBODATA",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  COMBODATA* self = (COMBODATA*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(COMBODATA);
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

/* get function:text of class TEXTDATA */
#ifndef TOLUA_DISABLE_tolua_get_TEXTDATA_text_ptr
static int tolua_get_TEXTDATA_text_ptr (lua_State* tolua_S) {
  TEXTDATA*self = (TEXTDATA*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'text'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->text,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TEXTDATA_text_ptr */

/* get function:text of class TEXTDATA */
#ifndef TOLUA_DISABLE_tolua_set_TEXTDATA_text_ptr
static int tolua_set_TEXTDATA_text_ptr (lua_State* tolua_S) {
  TEXTDATA*self = (TEXTDATA*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'text'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->text=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TEXTDATA_text_ptr */

/* method: new of class  TEXTDATA */
#ifndef TOLUA_DISABLE_tolua_GUIScript_TEXTDATA_new00
static int tolua_GUIScript_TEXTDATA_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"TEXTDATA",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  TEXTDATA* tolua_ret = (TEXTDATA*)  new TEXTDATA();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"TEXTDATA");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TEXTDATA */
#ifndef TOLUA_DISABLE_tolua_GUIScript_TEXTDATA_new00_local
static int tolua_GUIScript_TEXTDATA_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"TEXTDATA",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  TEXTDATA* tolua_ret = (TEXTDATA*)  new TEXTDATA();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TEXTDATA");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  TEXTDATA */
#ifndef TOLUA_DISABLE_tolua_GUIScript_TEXTDATA_delete00
static int tolua_GUIScript_TEXTDATA_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"TEXTDATA",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  TEXTDATA* self = (TEXTDATA*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  TEXTDATA */
#ifndef TOLUA_DISABLE_tolua_GUIScript_TEXTDATA_SIZEOF00
static int tolua_GUIScript_TEXTDATA_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"TEXTDATA",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  TEXTDATA* self = (TEXTDATA*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(TEXTDATA);
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

/* get function:x1 of class SELECTRECT */
#ifndef TOLUA_DISABLE_tolua_get_SELECTRECT_x1
static int tolua_get_SELECTRECT_x1 (lua_State* tolua_S) {
  SELECTRECT*self = (SELECTRECT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x1'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->x1);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SELECTRECT_x1 */

/* get function:x1 of class SELECTRECT */
#ifndef TOLUA_DISABLE_tolua_set_SELECTRECT_x1
static int tolua_set_SELECTRECT_x1 (lua_State* tolua_S) {
  SELECTRECT*self = (SELECTRECT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x1'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->x1=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SELECTRECT_x1 */

/* get function:x2 of class SELECTRECT */
#ifndef TOLUA_DISABLE_tolua_get_SELECTRECT_x2
static int tolua_get_SELECTRECT_x2 (lua_State* tolua_S) {
  SELECTRECT*self = (SELECTRECT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x2'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->x2);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SELECTRECT_x2 */

/* get function:x2 of class SELECTRECT */
#ifndef TOLUA_DISABLE_tolua_set_SELECTRECT_x2
static int tolua_set_SELECTRECT_x2 (lua_State* tolua_S) {
  SELECTRECT*self = (SELECTRECT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x2'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->x2=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SELECTRECT_x2 */

/* get function:y1 of class SELECTRECT */
#ifndef TOLUA_DISABLE_tolua_get_SELECTRECT_y1
static int tolua_get_SELECTRECT_y1 (lua_State* tolua_S) {
  SELECTRECT*self = (SELECTRECT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y1'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->y1);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SELECTRECT_y1 */

/* get function:y1 of class SELECTRECT */
#ifndef TOLUA_DISABLE_tolua_set_SELECTRECT_y1
static int tolua_set_SELECTRECT_y1 (lua_State* tolua_S) {
  SELECTRECT*self = (SELECTRECT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y1'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->y1=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SELECTRECT_y1 */

/* get function:y2 of class SELECTRECT */
#ifndef TOLUA_DISABLE_tolua_get_SELECTRECT_y2
static int tolua_get_SELECTRECT_y2 (lua_State* tolua_S) {
  SELECTRECT*self = (SELECTRECT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y2'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->y2);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SELECTRECT_y2 */

/* get function:y2 of class SELECTRECT */
#ifndef TOLUA_DISABLE_tolua_set_SELECTRECT_y2
static int tolua_set_SELECTRECT_y2 (lua_State* tolua_S) {
  SELECTRECT*self = (SELECTRECT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y2'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->y2=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SELECTRECT_y2 */

/* method: new of class  SELECTRECT */
#ifndef TOLUA_DISABLE_tolua_GUIScript_SELECTRECT_new00
static int tolua_GUIScript_SELECTRECT_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"SELECTRECT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  SELECTRECT* tolua_ret = (SELECTRECT*)  new SELECTRECT();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"SELECTRECT");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  SELECTRECT */
#ifndef TOLUA_DISABLE_tolua_GUIScript_SELECTRECT_new00_local
static int tolua_GUIScript_SELECTRECT_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"SELECTRECT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  SELECTRECT* tolua_ret = (SELECTRECT*)  new SELECTRECT();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"SELECTRECT");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  SELECTRECT */
#ifndef TOLUA_DISABLE_tolua_GUIScript_SELECTRECT_delete00
static int tolua_GUIScript_SELECTRECT_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"SELECTRECT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  SELECTRECT* self = (SELECTRECT*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  SELECTRECT */
#ifndef TOLUA_DISABLE_tolua_GUIScript_SELECTRECT_SIZEOF00
static int tolua_GUIScript_SELECTRECT_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"SELECTRECT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  SELECTRECT* self = (SELECTRECT*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(SELECTRECT);
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

/* get function:m_iZOrder of class ZORDERDATA */
#ifndef TOLUA_DISABLE_tolua_get_ZORDERDATA_m_iZOrder
static int tolua_get_ZORDERDATA_m_iZOrder (lua_State* tolua_S) {
  ZORDERDATA*self = (ZORDERDATA*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_iZOrder'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_iZOrder);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_ZORDERDATA_m_iZOrder */

/* get function:m_iZOrder of class ZORDERDATA */
#ifndef TOLUA_DISABLE_tolua_set_ZORDERDATA_m_iZOrder
static int tolua_set_ZORDERDATA_m_iZOrder (lua_State* tolua_S) {
  ZORDERDATA*self = (ZORDERDATA*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_iZOrder'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_iZOrder=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_ZORDERDATA_m_iZOrder */

/* method: new of class  ZORDERDATA */
#ifndef TOLUA_DISABLE_tolua_GUIScript_ZORDERDATA_new00
static int tolua_GUIScript_ZORDERDATA_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"ZORDERDATA",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  ZORDERDATA* tolua_ret = (ZORDERDATA*)  new ZORDERDATA();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"ZORDERDATA");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  ZORDERDATA */
#ifndef TOLUA_DISABLE_tolua_GUIScript_ZORDERDATA_new00_local
static int tolua_GUIScript_ZORDERDATA_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"ZORDERDATA",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  ZORDERDATA* tolua_ret = (ZORDERDATA*)  new ZORDERDATA();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"ZORDERDATA");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  ZORDERDATA */
#ifndef TOLUA_DISABLE_tolua_GUIScript_ZORDERDATA_delete00
static int tolua_GUIScript_ZORDERDATA_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"ZORDERDATA",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  ZORDERDATA* self = (ZORDERDATA*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  ZORDERDATA */
#ifndef TOLUA_DISABLE_tolua_GUIScript_ZORDERDATA_SIZEOF00
static int tolua_GUIScript_ZORDERDATA_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"ZORDERDATA",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  ZORDERDATA* self = (ZORDERDATA*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(ZORDERDATA);
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

/* get function:m_fPosX of class OBJUNDERCURSOR */
#ifndef TOLUA_DISABLE_tolua_get_OBJUNDERCURSOR_m_fPosX
static int tolua_get_OBJUNDERCURSOR_m_fPosX (lua_State* tolua_S) {
  OBJUNDERCURSOR*self = (OBJUNDERCURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fPosX'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fPosX);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_OBJUNDERCURSOR_m_fPosX */

/* get function:m_fPosX of class OBJUNDERCURSOR */
#ifndef TOLUA_DISABLE_tolua_set_OBJUNDERCURSOR_m_fPosX
static int tolua_set_OBJUNDERCURSOR_m_fPosX (lua_State* tolua_S) {
  OBJUNDERCURSOR*self = (OBJUNDERCURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fPosX'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_fPosX=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_OBJUNDERCURSOR_m_fPosX */

/* get function:m_fPosY of class OBJUNDERCURSOR */
#ifndef TOLUA_DISABLE_tolua_get_OBJUNDERCURSOR_m_fPosY
static int tolua_get_OBJUNDERCURSOR_m_fPosY (lua_State* tolua_S) {
  OBJUNDERCURSOR*self = (OBJUNDERCURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fPosY'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fPosY);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_OBJUNDERCURSOR_m_fPosY */

/* get function:m_fPosY of class OBJUNDERCURSOR */
#ifndef TOLUA_DISABLE_tolua_set_OBJUNDERCURSOR_m_fPosY
static int tolua_set_OBJUNDERCURSOR_m_fPosY (lua_State* tolua_S) {
  OBJUNDERCURSOR*self = (OBJUNDERCURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fPosY'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_fPosY=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_OBJUNDERCURSOR_m_fPosY */

/* get function:m_fPosZ of class OBJUNDERCURSOR */
#ifndef TOLUA_DISABLE_tolua_get_OBJUNDERCURSOR_m_fPosZ
static int tolua_get_OBJUNDERCURSOR_m_fPosZ (lua_State* tolua_S) {
  OBJUNDERCURSOR*self = (OBJUNDERCURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fPosZ'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fPosZ);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_OBJUNDERCURSOR_m_fPosZ */

/* get function:m_fPosZ of class OBJUNDERCURSOR */
#ifndef TOLUA_DISABLE_tolua_set_OBJUNDERCURSOR_m_fPosZ
static int tolua_set_OBJUNDERCURSOR_m_fPosZ (lua_State* tolua_S) {
  OBJUNDERCURSOR*self = (OBJUNDERCURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fPosZ'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_fPosZ=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_OBJUNDERCURSOR_m_fPosZ */

/* get function:m_Name of class OBJUNDERCURSOR */
#ifndef TOLUA_DISABLE_tolua_get_OBJUNDERCURSOR_m_Name_ptr
static int tolua_get_OBJUNDERCURSOR_m_Name_ptr (lua_State* tolua_S) {
  OBJUNDERCURSOR*self = (OBJUNDERCURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Name'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_Name,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_OBJUNDERCURSOR_m_Name_ptr */

/* get function:m_Name of class OBJUNDERCURSOR */
#ifndef TOLUA_DISABLE_tolua_set_OBJUNDERCURSOR_m_Name_ptr
static int tolua_set_OBJUNDERCURSOR_m_Name_ptr (lua_State* tolua_S) {
  OBJUNDERCURSOR*self = (OBJUNDERCURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Name'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_Name=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_OBJUNDERCURSOR_m_Name_ptr */

/* get function:m_CompType of class OBJUNDERCURSOR */
#ifndef TOLUA_DISABLE_tolua_get_OBJUNDERCURSOR_m_CompType_ptr
static int tolua_get_OBJUNDERCURSOR_m_CompType_ptr (lua_State* tolua_S) {
  OBJUNDERCURSOR*self = (OBJUNDERCURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_CompType'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_CompType,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_OBJUNDERCURSOR_m_CompType_ptr */

/* get function:m_CompType of class OBJUNDERCURSOR */
#ifndef TOLUA_DISABLE_tolua_set_OBJUNDERCURSOR_m_CompType_ptr
static int tolua_set_OBJUNDERCURSOR_m_CompType_ptr (lua_State* tolua_S) {
  OBJUNDERCURSOR*self = (OBJUNDERCURSOR*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_CompType'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_CompType=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_OBJUNDERCURSOR_m_CompType_ptr */

/* method: new of class  OBJUNDERCURSOR */
#ifndef TOLUA_DISABLE_tolua_GUIScript_OBJUNDERCURSOR_new00
static int tolua_GUIScript_OBJUNDERCURSOR_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"OBJUNDERCURSOR",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  OBJUNDERCURSOR* tolua_ret = (OBJUNDERCURSOR*)  new OBJUNDERCURSOR();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"OBJUNDERCURSOR");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  OBJUNDERCURSOR */
#ifndef TOLUA_DISABLE_tolua_GUIScript_OBJUNDERCURSOR_new00_local
static int tolua_GUIScript_OBJUNDERCURSOR_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"OBJUNDERCURSOR",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  OBJUNDERCURSOR* tolua_ret = (OBJUNDERCURSOR*)  new OBJUNDERCURSOR();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"OBJUNDERCURSOR");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  OBJUNDERCURSOR */
#ifndef TOLUA_DISABLE_tolua_GUIScript_OBJUNDERCURSOR_delete00
static int tolua_GUIScript_OBJUNDERCURSOR_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OBJUNDERCURSOR",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  OBJUNDERCURSOR* self = (OBJUNDERCURSOR*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  OBJUNDERCURSOR */
#ifndef TOLUA_DISABLE_tolua_GUIScript_OBJUNDERCURSOR_SIZEOF00
static int tolua_GUIScript_OBJUNDERCURSOR_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OBJUNDERCURSOR",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  OBJUNDERCURSOR* self = (OBJUNDERCURSOR*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(OBJUNDERCURSOR);
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

/* get function:x1 of class OBJINRECT */
#ifndef TOLUA_DISABLE_tolua_get_OBJINRECT_x1
static int tolua_get_OBJINRECT_x1 (lua_State* tolua_S) {
  OBJINRECT*self = (OBJINRECT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x1'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->x1);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_OBJINRECT_x1 */

/* get function:x1 of class OBJINRECT */
#ifndef TOLUA_DISABLE_tolua_set_OBJINRECT_x1
static int tolua_set_OBJINRECT_x1 (lua_State* tolua_S) {
  OBJINRECT*self = (OBJINRECT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x1'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->x1=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_OBJINRECT_x1 */

/* get function:x2 of class OBJINRECT */
#ifndef TOLUA_DISABLE_tolua_get_OBJINRECT_x2
static int tolua_get_OBJINRECT_x2 (lua_State* tolua_S) {
  OBJINRECT*self = (OBJINRECT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x2'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->x2);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_OBJINRECT_x2 */

/* get function:x2 of class OBJINRECT */
#ifndef TOLUA_DISABLE_tolua_set_OBJINRECT_x2
static int tolua_set_OBJINRECT_x2 (lua_State* tolua_S) {
  OBJINRECT*self = (OBJINRECT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x2'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->x2=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_OBJINRECT_x2 */

/* get function:y1 of class OBJINRECT */
#ifndef TOLUA_DISABLE_tolua_get_OBJINRECT_y1
static int tolua_get_OBJINRECT_y1 (lua_State* tolua_S) {
  OBJINRECT*self = (OBJINRECT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y1'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->y1);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_OBJINRECT_y1 */

/* get function:y1 of class OBJINRECT */
#ifndef TOLUA_DISABLE_tolua_set_OBJINRECT_y1
static int tolua_set_OBJINRECT_y1 (lua_State* tolua_S) {
  OBJINRECT*self = (OBJINRECT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y1'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->y1=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_OBJINRECT_y1 */

/* get function:y2 of class OBJINRECT */
#ifndef TOLUA_DISABLE_tolua_get_OBJINRECT_y2
static int tolua_get_OBJINRECT_y2 (lua_State* tolua_S) {
  OBJINRECT*self = (OBJINRECT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y2'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->y2);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_OBJINRECT_y2 */

/* get function:y2 of class OBJINRECT */
#ifndef TOLUA_DISABLE_tolua_set_OBJINRECT_y2
static int tolua_set_OBJINRECT_y2 (lua_State* tolua_S) {
  OBJINRECT*self = (OBJINRECT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y2'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->y2=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_OBJINRECT_y2 */

/* get function:list of class OBJINRECT */
#ifndef TOLUA_DISABLE_tolua_get_OBJINRECT_list
static int tolua_get_OBJINRECT_list (lua_State* tolua_S) {
  OBJINRECT*self = (OBJINRECT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'list'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->list,"OBJECTLIST");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_OBJINRECT_list */

/* get function:list of class OBJINRECT */
#ifndef TOLUA_DISABLE_tolua_set_OBJINRECT_list
static int tolua_set_OBJINRECT_list (lua_State* tolua_S) {
  OBJINRECT*self = (OBJINRECT*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'list'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"OBJECTLIST",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->list=*(( OBJECTLIST*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_OBJINRECT_list */

/* method: new of class  OBJINRECT */
#ifndef TOLUA_DISABLE_tolua_GUIScript_OBJINRECT_new00
static int tolua_GUIScript_OBJINRECT_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"OBJINRECT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  OBJINRECT* tolua_ret = (OBJINRECT*)  new OBJINRECT();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"OBJINRECT");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  OBJINRECT */
#ifndef TOLUA_DISABLE_tolua_GUIScript_OBJINRECT_new00_local
static int tolua_GUIScript_OBJINRECT_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"OBJINRECT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  OBJINRECT* tolua_ret = (OBJINRECT*)  new OBJINRECT();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"OBJINRECT");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  OBJINRECT */
#ifndef TOLUA_DISABLE_tolua_GUIScript_OBJINRECT_delete00
static int tolua_GUIScript_OBJINRECT_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OBJINRECT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  OBJINRECT* self = (OBJINRECT*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  OBJINRECT */
#ifndef TOLUA_DISABLE_tolua_GUIScript_OBJINRECT_SIZEOF00
static int tolua_GUIScript_OBJINRECT_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"OBJINRECT",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  OBJINRECT* self = (OBJINRECT*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(OBJINRECT);
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

/* get function:name of class GUIQUERYBYNAME */
#ifndef TOLUA_DISABLE_tolua_get_GUIQUERYBYNAME_name_ptr
static int tolua_get_GUIQUERYBYNAME_name_ptr (lua_State* tolua_S) {
  GUIQUERYBYNAME*self = (GUIQUERYBYNAME*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->name,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GUIQUERYBYNAME_name_ptr */

/* get function:name of class GUIQUERYBYNAME */
#ifndef TOLUA_DISABLE_tolua_set_GUIQUERYBYNAME_name_ptr
static int tolua_set_GUIQUERYBYNAME_name_ptr (lua_State* tolua_S) {
  GUIQUERYBYNAME*self = (GUIQUERYBYNAME*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->name=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GUIQUERYBYNAME_name_ptr */

/* get function:bFound of class GUIQUERYBYNAME */
#ifndef TOLUA_DISABLE_tolua_get_GUIQUERYBYNAME_bFound
static int tolua_get_GUIQUERYBYNAME_bFound (lua_State* tolua_S) {
  GUIQUERYBYNAME*self = (GUIQUERYBYNAME*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bFound'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->bFound);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GUIQUERYBYNAME_bFound */

/* get function:bFound of class GUIQUERYBYNAME */
#ifndef TOLUA_DISABLE_tolua_set_GUIQUERYBYNAME_bFound
static int tolua_set_GUIQUERYBYNAME_bFound (lua_State* tolua_S) {
  GUIQUERYBYNAME*self = (GUIQUERYBYNAME*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bFound'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->bFound=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GUIQUERYBYNAME_bFound */

/* method: new of class  GUIQUERYBYNAME */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIQUERYBYNAME_new00
static int tolua_GUIScript_GUIQUERYBYNAME_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GUIQUERYBYNAME",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GUIQUERYBYNAME* tolua_ret = (GUIQUERYBYNAME*)  new GUIQUERYBYNAME();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"GUIQUERYBYNAME");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GUIQUERYBYNAME */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIQUERYBYNAME_new00_local
static int tolua_GUIScript_GUIQUERYBYNAME_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GUIQUERYBYNAME",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GUIQUERYBYNAME* tolua_ret = (GUIQUERYBYNAME*)  new GUIQUERYBYNAME();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GUIQUERYBYNAME");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  GUIQUERYBYNAME */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIQUERYBYNAME_delete00
static int tolua_GUIScript_GUIQUERYBYNAME_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GUIQUERYBYNAME",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GUIQUERYBYNAME* self = (GUIQUERYBYNAME*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  GUIQUERYBYNAME */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIQUERYBYNAME_SIZEOF00
static int tolua_GUIScript_GUIQUERYBYNAME_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GUIQUERYBYNAME",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GUIQUERYBYNAME* self = (GUIQUERYBYNAME*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(GUIQUERYBYNAME);
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

/* get function:bVal of class GUIBOOL */
#ifndef TOLUA_DISABLE_tolua_get_GUIBOOL_bVal
static int tolua_get_GUIBOOL_bVal (lua_State* tolua_S) {
  GUIBOOL*self = (GUIBOOL*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bVal'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->bVal);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_GUIBOOL_bVal */

/* get function:bVal of class GUIBOOL */
#ifndef TOLUA_DISABLE_tolua_set_GUIBOOL_bVal
static int tolua_set_GUIBOOL_bVal (lua_State* tolua_S) {
  GUIBOOL*self = (GUIBOOL*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bVal'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->bVal=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_GUIBOOL_bVal */

/* method: operatorbool of class  GUIBOOL */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIBOOL__bool00
static int tolua_GUIScript_GUIBOOL__bool00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GUIBOOL",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GUIBOOL* self = (GUIBOOL*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operatorbool'",NULL);
 {
  bool tolua_ret = (bool)  static_cast< bool>(*self);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.bool'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: operatorbool* of class  GUIBOOL */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIBOOL__bool_00
static int tolua_GUIScript_GUIBOOL__bool_00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GUIBOOL",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GUIBOOL* self = (GUIBOOL*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operatorbool*'",NULL);
 {
  bool tolua_ret = (bool)  static_cast< bool>(*self);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.bool*'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GUIBOOL */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIBOOL_new00
static int tolua_GUIScript_GUIBOOL_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GUIBOOL",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GUIBOOL* tolua_ret = (GUIBOOL*)  new GUIBOOL();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"GUIBOOL");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GUIBOOL */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIBOOL_new00_local
static int tolua_GUIScript_GUIBOOL_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GUIBOOL",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  GUIBOOL* tolua_ret = (GUIBOOL*)  new GUIBOOL();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GUIBOOL");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  GUIBOOL */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIBOOL_delete00
static int tolua_GUIScript_GUIBOOL_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GUIBOOL",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GUIBOOL* self = (GUIBOOL*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  GUIBOOL */
#ifndef TOLUA_DISABLE_tolua_GUIScript_GUIBOOL_SIZEOF00
static int tolua_GUIScript_GUIBOOL_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"GUIBOOL",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  GUIBOOL* self = (GUIBOOL*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(GUIBOOL);
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

/* get function:hszKey of class CHANGELANGUAGEKEYPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_CHANGELANGUAGEKEYPARAMS_hszKey_ptr
static int tolua_get_CHANGELANGUAGEKEYPARAMS_hszKey_ptr (lua_State* tolua_S) {
  CHANGELANGUAGEKEYPARAMS*self = (CHANGELANGUAGEKEYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'hszKey'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->hszKey,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_CHANGELANGUAGEKEYPARAMS_hszKey_ptr */

/* get function:hszKey of class CHANGELANGUAGEKEYPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_CHANGELANGUAGEKEYPARAMS_hszKey_ptr
static int tolua_set_CHANGELANGUAGEKEYPARAMS_hszKey_ptr (lua_State* tolua_S) {
  CHANGELANGUAGEKEYPARAMS*self = (CHANGELANGUAGEKEYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'hszKey'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->hszKey=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_CHANGELANGUAGEKEYPARAMS_hszKey_ptr */

/* get function:archive of class CHANGELANGUAGEKEYPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_CHANGELANGUAGEKEYPARAMS_archive_ptr
static int tolua_get_CHANGELANGUAGEKEYPARAMS_archive_ptr (lua_State* tolua_S) {
  CHANGELANGUAGEKEYPARAMS*self = (CHANGELANGUAGEKEYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'archive'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->archive,"IArchive");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_CHANGELANGUAGEKEYPARAMS_archive_ptr */

/* get function:archive of class CHANGELANGUAGEKEYPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_CHANGELANGUAGEKEYPARAMS_archive_ptr
static int tolua_set_CHANGELANGUAGEKEYPARAMS_archive_ptr (lua_State* tolua_S) {
  CHANGELANGUAGEKEYPARAMS*self = (CHANGELANGUAGEKEYPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'archive'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"IArchive",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->archive=(( IArchive*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_CHANGELANGUAGEKEYPARAMS_archive_ptr */

/* method: new of class  CHANGELANGUAGEKEYPARAMS */
#ifndef TOLUA_DISABLE_tolua_GUIScript_CHANGELANGUAGEKEYPARAMS_new00
static int tolua_GUIScript_CHANGELANGUAGEKEYPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CHANGELANGUAGEKEYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  CHANGELANGUAGEKEYPARAMS* tolua_ret = (CHANGELANGUAGEKEYPARAMS*)  new CHANGELANGUAGEKEYPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CHANGELANGUAGEKEYPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CHANGELANGUAGEKEYPARAMS */
#ifndef TOLUA_DISABLE_tolua_GUIScript_CHANGELANGUAGEKEYPARAMS_new00_local
static int tolua_GUIScript_CHANGELANGUAGEKEYPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CHANGELANGUAGEKEYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  CHANGELANGUAGEKEYPARAMS* tolua_ret = (CHANGELANGUAGEKEYPARAMS*)  new CHANGELANGUAGEKEYPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CHANGELANGUAGEKEYPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CHANGELANGUAGEKEYPARAMS */
#ifndef TOLUA_DISABLE_tolua_GUIScript_CHANGELANGUAGEKEYPARAMS_delete00
static int tolua_GUIScript_CHANGELANGUAGEKEYPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CHANGELANGUAGEKEYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CHANGELANGUAGEKEYPARAMS* self = (CHANGELANGUAGEKEYPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  CHANGELANGUAGEKEYPARAMS */
#ifndef TOLUA_DISABLE_tolua_GUIScript_CHANGELANGUAGEKEYPARAMS_SIZEOF00
static int tolua_GUIScript_CHANGELANGUAGEKEYPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CHANGELANGUAGEKEYPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CHANGELANGUAGEKEYPARAMS* self = (CHANGELANGUAGEKEYPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(CHANGELANGUAGEKEYPARAMS);
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
TOLUA_API int tolua_GUIScript_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"GUIEVENT","GUIEVENT","",tolua_collect_GUIEVENT);
 #else
 tolua_cclass(tolua_S,"GUIEVENT","GUIEVENT","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"GUIEVENT");
  tolua_variable(tolua_S,"event",tolua_get_GUIEVENT_event,tolua_set_GUIEVENT_event);
  tolua_variable(tolua_S,"ar",tolua_get_GUIEVENT_ar_ptr,tolua_set_GUIEVENT_ar_ptr);
  tolua_function(tolua_S,"new",tolua_GUIScript_GUIEVENT_new00);
  tolua_function(tolua_S,"new_local",tolua_GUIScript_GUIEVENT_new00_local);
  tolua_function(tolua_S,".call",tolua_GUIScript_GUIEVENT_new00_local);
  tolua_function(tolua_S,"delete",tolua_GUIScript_GUIEVENT_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_GUIScript_GUIEVENT_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"GUIFLOAT","GUIFLOAT","",tolua_collect_GUIFLOAT);
 #else
 tolua_cclass(tolua_S,"GUIFLOAT","GUIFLOAT","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"GUIFLOAT");
  tolua_variable(tolua_S,"data",tolua_get_GUIFLOAT_data,tolua_set_GUIFLOAT_data);
  tolua_function(tolua_S,"new",tolua_GUIScript_GUIFLOAT_new00);
  tolua_function(tolua_S,"new_local",tolua_GUIScript_GUIFLOAT_new00_local);
  tolua_function(tolua_S,".call",tolua_GUIScript_GUIFLOAT_new00_local);
  tolua_function(tolua_S,"delete",tolua_GUIScript_GUIFLOAT_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_GUIScript_GUIFLOAT_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"GUIPAGEMESSAGE","GUIPAGEMESSAGE","",tolua_collect_GUIPAGEMESSAGE);
 #else
 tolua_cclass(tolua_S,"GUIPAGEMESSAGE","GUIPAGEMESSAGE","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"GUIPAGEMESSAGE");
  tolua_variable(tolua_S,"name",tolua_get_GUIPAGEMESSAGE_name_ptr,tolua_set_GUIPAGEMESSAGE_name_ptr);
  tolua_variable(tolua_S,"compType",tolua_get_GUIPAGEMESSAGE_compType_ptr,tolua_set_GUIPAGEMESSAGE_compType_ptr);
  tolua_variable(tolua_S,"wasFoundInPage",tolua_get_GUIPAGEMESSAGE_wasFoundInPage,tolua_set_GUIPAGEMESSAGE_wasFoundInPage);
  tolua_function(tolua_S,"new",tolua_GUIScript_GUIPAGEMESSAGE_new00);
  tolua_function(tolua_S,"new_local",tolua_GUIScript_GUIPAGEMESSAGE_new00_local);
  tolua_function(tolua_S,".call",tolua_GUIScript_GUIPAGEMESSAGE_new00_local);
  tolua_function(tolua_S,"delete",tolua_GUIScript_GUIPAGEMESSAGE_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_GUIScript_GUIPAGEMESSAGE_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"BUTTONIMAGE","BUTTONIMAGE","",tolua_collect_BUTTONIMAGE);
 #else
 tolua_cclass(tolua_S,"BUTTONIMAGE","BUTTONIMAGE","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"BUTTONIMAGE");
  tolua_variable(tolua_S,"szNorm",tolua_get_BUTTONIMAGE_szNorm,tolua_set_BUTTONIMAGE_szNorm);
  tolua_variable(tolua_S,"szHigh",tolua_get_BUTTONIMAGE_szHigh,tolua_set_BUTTONIMAGE_szHigh);
  tolua_variable(tolua_S,"szSelect",tolua_get_BUTTONIMAGE_szSelect,tolua_set_BUTTONIMAGE_szSelect);
  tolua_variable(tolua_S,"szGray",tolua_get_BUTTONIMAGE_szGray,tolua_set_BUTTONIMAGE_szGray);
  tolua_function(tolua_S,"new",tolua_GUIScript_BUTTONIMAGE_new00);
  tolua_function(tolua_S,"new_local",tolua_GUIScript_BUTTONIMAGE_new00_local);
  tolua_function(tolua_S,".call",tolua_GUIScript_BUTTONIMAGE_new00_local);
  tolua_function(tolua_S,"delete",tolua_GUIScript_BUTTONIMAGE_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_GUIScript_BUTTONIMAGE_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"CHANGECURSOR","CHANGECURSOR","",tolua_collect_CHANGECURSOR);
 #else
 tolua_cclass(tolua_S,"CHANGECURSOR","CHANGECURSOR","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"CHANGECURSOR");
  tolua_variable(tolua_S,"iSizeX",tolua_get_CHANGECURSOR_iSizeX,tolua_set_CHANGECURSOR_iSizeX);
  tolua_variable(tolua_S,"iSizeY",tolua_get_CHANGECURSOR_iSizeY,tolua_set_CHANGECURSOR_iSizeY);
  tolua_variable(tolua_S,"iHotX",tolua_get_CHANGECURSOR_iHotX,tolua_set_CHANGECURSOR_iHotX);
  tolua_variable(tolua_S,"iHotY",tolua_get_CHANGECURSOR_iHotY,tolua_set_CHANGECURSOR_iHotY);
  tolua_variable(tolua_S,"pTexture",tolua_get_CHANGECURSOR_pTexture,tolua_set_CHANGECURSOR_pTexture);
  tolua_function(tolua_S,"new",tolua_GUIScript_CHANGECURSOR_new00);
  tolua_function(tolua_S,"new_local",tolua_GUIScript_CHANGECURSOR_new00_local);
  tolua_function(tolua_S,".call",tolua_GUIScript_CHANGECURSOR_new00_local);
  tolua_function(tolua_S,"delete",tolua_GUIScript_CHANGECURSOR_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_GUIScript_CHANGECURSOR_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"ADDLISTBOXENTRY","ADDLISTBOXENTRY","",tolua_collect_ADDLISTBOXENTRY);
 #else
 tolua_cclass(tolua_S,"ADDLISTBOXENTRY","ADDLISTBOXENTRY","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"ADDLISTBOXENTRY");
  tolua_variable(tolua_S,"entryText",tolua_get_ADDLISTBOXENTRY_entryText,tolua_set_ADDLISTBOXENTRY_entryText);
  tolua_variable(tolua_S,"iRedColor",tolua_get_ADDLISTBOXENTRY_iRedColor,tolua_set_ADDLISTBOXENTRY_iRedColor);
  tolua_variable(tolua_S,"iGreenColor",tolua_get_ADDLISTBOXENTRY_iGreenColor,tolua_set_ADDLISTBOXENTRY_iGreenColor);
  tolua_variable(tolua_S,"iBlueColor",tolua_get_ADDLISTBOXENTRY_iBlueColor,tolua_set_ADDLISTBOXENTRY_iBlueColor);
  tolua_function(tolua_S,"new",tolua_GUIScript_ADDLISTBOXENTRY_new00);
  tolua_function(tolua_S,"new_local",tolua_GUIScript_ADDLISTBOXENTRY_new00_local);
  tolua_function(tolua_S,".call",tolua_GUIScript_ADDLISTBOXENTRY_new00_local);
  tolua_function(tolua_S,"delete",tolua_GUIScript_ADDLISTBOXENTRY_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_GUIScript_ADDLISTBOXENTRY_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"CURSORVISIBLE","CURSORVISIBLE","",tolua_collect_CURSORVISIBLE);
 #else
 tolua_cclass(tolua_S,"CURSORVISIBLE","CURSORVISIBLE","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"CURSORVISIBLE");
  tolua_variable(tolua_S,"bVisible",tolua_get_CURSORVISIBLE_bVisible,tolua_set_CURSORVISIBLE_bVisible);
  tolua_function(tolua_S,"new",tolua_GUIScript_CURSORVISIBLE_new00);
  tolua_function(tolua_S,"new_local",tolua_GUIScript_CURSORVISIBLE_new00_local);
  tolua_function(tolua_S,".call",tolua_GUIScript_CURSORVISIBLE_new00_local);
  tolua_function(tolua_S,"delete",tolua_GUIScript_CURSORVISIBLE_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_GUIScript_CURSORVISIBLE_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"COMBODATA","COMBODATA","",tolua_collect_COMBODATA);
 #else
 tolua_cclass(tolua_S,"COMBODATA","COMBODATA","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"COMBODATA");
  tolua_variable(tolua_S,"text",tolua_get_COMBODATA_text,tolua_set_COMBODATA_text);
  tolua_variable(tolua_S,"index",tolua_get_COMBODATA_index,tolua_set_COMBODATA_index);
  tolua_function(tolua_S,"new",tolua_GUIScript_COMBODATA_new00);
  tolua_function(tolua_S,"new_local",tolua_GUIScript_COMBODATA_new00_local);
  tolua_function(tolua_S,".call",tolua_GUIScript_COMBODATA_new00_local);
  tolua_function(tolua_S,"delete",tolua_GUIScript_COMBODATA_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_GUIScript_COMBODATA_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"TEXTDATA","TEXTDATA","",tolua_collect_TEXTDATA);
 #else
 tolua_cclass(tolua_S,"TEXTDATA","TEXTDATA","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"TEXTDATA");
  tolua_variable(tolua_S,"text",tolua_get_TEXTDATA_text_ptr,tolua_set_TEXTDATA_text_ptr);
  tolua_function(tolua_S,"new",tolua_GUIScript_TEXTDATA_new00);
  tolua_function(tolua_S,"new_local",tolua_GUIScript_TEXTDATA_new00_local);
  tolua_function(tolua_S,".call",tolua_GUIScript_TEXTDATA_new00_local);
  tolua_function(tolua_S,"delete",tolua_GUIScript_TEXTDATA_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_GUIScript_TEXTDATA_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"SELECTRECT","SELECTRECT","",tolua_collect_SELECTRECT);
 #else
 tolua_cclass(tolua_S,"SELECTRECT","SELECTRECT","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"SELECTRECT");
  tolua_variable(tolua_S,"x1",tolua_get_SELECTRECT_x1,tolua_set_SELECTRECT_x1);
  tolua_variable(tolua_S,"x2",tolua_get_SELECTRECT_x2,tolua_set_SELECTRECT_x2);
  tolua_variable(tolua_S,"y1",tolua_get_SELECTRECT_y1,tolua_set_SELECTRECT_y1);
  tolua_variable(tolua_S,"y2",tolua_get_SELECTRECT_y2,tolua_set_SELECTRECT_y2);
  tolua_function(tolua_S,"new",tolua_GUIScript_SELECTRECT_new00);
  tolua_function(tolua_S,"new_local",tolua_GUIScript_SELECTRECT_new00_local);
  tolua_function(tolua_S,".call",tolua_GUIScript_SELECTRECT_new00_local);
  tolua_function(tolua_S,"delete",tolua_GUIScript_SELECTRECT_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_GUIScript_SELECTRECT_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"ZORDERDATA","ZORDERDATA","",tolua_collect_ZORDERDATA);
 #else
 tolua_cclass(tolua_S,"ZORDERDATA","ZORDERDATA","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"ZORDERDATA");
  tolua_variable(tolua_S,"m_iZOrder",tolua_get_ZORDERDATA_m_iZOrder,tolua_set_ZORDERDATA_m_iZOrder);
  tolua_function(tolua_S,"new",tolua_GUIScript_ZORDERDATA_new00);
  tolua_function(tolua_S,"new_local",tolua_GUIScript_ZORDERDATA_new00_local);
  tolua_function(tolua_S,".call",tolua_GUIScript_ZORDERDATA_new00_local);
  tolua_function(tolua_S,"delete",tolua_GUIScript_ZORDERDATA_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_GUIScript_ZORDERDATA_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"OBJUNDERCURSOR","OBJUNDERCURSOR","",tolua_collect_OBJUNDERCURSOR);
 #else
 tolua_cclass(tolua_S,"OBJUNDERCURSOR","OBJUNDERCURSOR","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"OBJUNDERCURSOR");
  tolua_variable(tolua_S,"m_fPosX",tolua_get_OBJUNDERCURSOR_m_fPosX,tolua_set_OBJUNDERCURSOR_m_fPosX);
  tolua_variable(tolua_S,"m_fPosY",tolua_get_OBJUNDERCURSOR_m_fPosY,tolua_set_OBJUNDERCURSOR_m_fPosY);
  tolua_variable(tolua_S,"m_fPosZ",tolua_get_OBJUNDERCURSOR_m_fPosZ,tolua_set_OBJUNDERCURSOR_m_fPosZ);
  tolua_variable(tolua_S,"m_Name",tolua_get_OBJUNDERCURSOR_m_Name_ptr,tolua_set_OBJUNDERCURSOR_m_Name_ptr);
  tolua_variable(tolua_S,"m_CompType",tolua_get_OBJUNDERCURSOR_m_CompType_ptr,tolua_set_OBJUNDERCURSOR_m_CompType_ptr);
  tolua_function(tolua_S,"new",tolua_GUIScript_OBJUNDERCURSOR_new00);
  tolua_function(tolua_S,"new_local",tolua_GUIScript_OBJUNDERCURSOR_new00_local);
  tolua_function(tolua_S,".call",tolua_GUIScript_OBJUNDERCURSOR_new00_local);
  tolua_function(tolua_S,"delete",tolua_GUIScript_OBJUNDERCURSOR_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_GUIScript_OBJUNDERCURSOR_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"OBJINRECT","OBJINRECT","",tolua_collect_OBJINRECT);
 #else
 tolua_cclass(tolua_S,"OBJINRECT","OBJINRECT","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"OBJINRECT");
  tolua_variable(tolua_S,"x1",tolua_get_OBJINRECT_x1,tolua_set_OBJINRECT_x1);
  tolua_variable(tolua_S,"x2",tolua_get_OBJINRECT_x2,tolua_set_OBJINRECT_x2);
  tolua_variable(tolua_S,"y1",tolua_get_OBJINRECT_y1,tolua_set_OBJINRECT_y1);
  tolua_variable(tolua_S,"y2",tolua_get_OBJINRECT_y2,tolua_set_OBJINRECT_y2);
  tolua_variable(tolua_S,"list",tolua_get_OBJINRECT_list,tolua_set_OBJINRECT_list);
  tolua_function(tolua_S,"new",tolua_GUIScript_OBJINRECT_new00);
  tolua_function(tolua_S,"new_local",tolua_GUIScript_OBJINRECT_new00_local);
  tolua_function(tolua_S,".call",tolua_GUIScript_OBJINRECT_new00_local);
  tolua_function(tolua_S,"delete",tolua_GUIScript_OBJINRECT_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_GUIScript_OBJINRECT_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"GUIQUERYBYNAME","GUIQUERYBYNAME","",tolua_collect_GUIQUERYBYNAME);
 #else
 tolua_cclass(tolua_S,"GUIQUERYBYNAME","GUIQUERYBYNAME","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"GUIQUERYBYNAME");
  tolua_variable(tolua_S,"name",tolua_get_GUIQUERYBYNAME_name_ptr,tolua_set_GUIQUERYBYNAME_name_ptr);
  tolua_variable(tolua_S,"bFound",tolua_get_GUIQUERYBYNAME_bFound,tolua_set_GUIQUERYBYNAME_bFound);
  tolua_function(tolua_S,"new",tolua_GUIScript_GUIQUERYBYNAME_new00);
  tolua_function(tolua_S,"new_local",tolua_GUIScript_GUIQUERYBYNAME_new00_local);
  tolua_function(tolua_S,".call",tolua_GUIScript_GUIQUERYBYNAME_new00_local);
  tolua_function(tolua_S,"delete",tolua_GUIScript_GUIQUERYBYNAME_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_GUIScript_GUIQUERYBYNAME_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"GUIBOOL","GUIBOOL","",tolua_collect_GUIBOOL);
 #else
 tolua_cclass(tolua_S,"GUIBOOL","GUIBOOL","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"GUIBOOL");
  tolua_variable(tolua_S,"bVal",tolua_get_GUIBOOL_bVal,tolua_set_GUIBOOL_bVal);
  tolua_function(tolua_S,".bool",tolua_GUIScript_GUIBOOL__bool00);
  tolua_function(tolua_S,".bool*",tolua_GUIScript_GUIBOOL__bool_00);
  tolua_function(tolua_S,"new",tolua_GUIScript_GUIBOOL_new00);
  tolua_function(tolua_S,"new_local",tolua_GUIScript_GUIBOOL_new00_local);
  tolua_function(tolua_S,".call",tolua_GUIScript_GUIBOOL_new00_local);
  tolua_function(tolua_S,"delete",tolua_GUIScript_GUIBOOL_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_GUIScript_GUIBOOL_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"CHANGELANGUAGEKEYPARAMS","CHANGELANGUAGEKEYPARAMS","",tolua_collect_CHANGELANGUAGEKEYPARAMS);
 #else
 tolua_cclass(tolua_S,"CHANGELANGUAGEKEYPARAMS","CHANGELANGUAGEKEYPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"CHANGELANGUAGEKEYPARAMS");
  tolua_variable(tolua_S,"hszKey",tolua_get_CHANGELANGUAGEKEYPARAMS_hszKey_ptr,tolua_set_CHANGELANGUAGEKEYPARAMS_hszKey_ptr);
  tolua_variable(tolua_S,"archive",tolua_get_CHANGELANGUAGEKEYPARAMS_archive_ptr,tolua_set_CHANGELANGUAGEKEYPARAMS_archive_ptr);
  tolua_function(tolua_S,"new",tolua_GUIScript_CHANGELANGUAGEKEYPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_GUIScript_CHANGELANGUAGEKEYPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_GUIScript_CHANGELANGUAGEKEYPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_GUIScript_CHANGELANGUAGEKEYPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_GUIScript_CHANGELANGUAGEKEYPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_GUIScript (lua_State* tolua_S) {
 return tolua_GUIScript_open(tolua_S);
};
#endif

