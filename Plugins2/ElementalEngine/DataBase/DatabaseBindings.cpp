/*
** Lua binding: DatabaseScript
** Generated automatically by toluaxx-1.2.0 on 02/04/09 17:40:56.
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
TOLUA_API int tolua_DatabaseScript_open (lua_State* tolua_S);

#include "StdAfx.h"
#include "../../ElementalEngine/EELuaScript/LuaHashString.h"
#pragma warning(disable : 4800)
#pragma warning(disable : 4345)
class CLuaDWORD { public: CLuaDWORD() {} CLuaDWORD(unsigned long i) { x = i; } operator DWORD() { return x; } void operator=(DWORD i) { x = i; } unsigned long x; };
#if 0 // To not include the "../"
#include "../../include/DatabaseDefines.hpp"
#endif

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_DATABASEATTRIBUTEPARAMS (lua_State* tolua_S)
{
 DATABASEATTRIBUTEPARAMS* self = (DATABASEATTRIBUTEPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CLuaDWORD (lua_State* tolua_S)
{
 CLuaDWORD* self = (CLuaDWORD*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_TYPEDATABASEINFO (lua_State* tolua_S)
{
 TYPEDATABASEINFO* self = (TYPEDATABASEINFO*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_DATABASEINFOPARAMS (lua_State* tolua_S)
{
 DATABASEINFOPARAMS* self = (DATABASEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CLuaHashString");
 tolua_usertype(tolua_S,"CLuaDWORD");
 tolua_usertype(tolua_S,"DATABASEATTRIBUTEPARAMS");
 tolua_usertype(tolua_S,"DATABASEDATA");
 tolua_usertype(tolua_S,"IArchive");
 tolua_usertype(tolua_S,"TYPEDATABASEINFO");
 tolua_usertype(tolua_S,"DATABASEINFOPARAMS");
}

/* get function:phszDBName of class TYPEDATABASEINFO */
#ifndef TOLUA_DISABLE_tolua_get_TYPEDATABASEINFO_phszDBName_ptr
static int tolua_get_TYPEDATABASEINFO_phszDBName_ptr (lua_State* tolua_S) {
  TYPEDATABASEINFO*self = (TYPEDATABASEINFO*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'phszDBName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->phszDBName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TYPEDATABASEINFO_phszDBName_ptr */

/* get function:phszDBName of class TYPEDATABASEINFO */
#ifndef TOLUA_DISABLE_tolua_set_TYPEDATABASEINFO_phszDBName_ptr
static int tolua_set_TYPEDATABASEINFO_phszDBName_ptr (lua_State* tolua_S) {
  TYPEDATABASEINFO*self = (TYPEDATABASEINFO*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'phszDBName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->phszDBName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TYPEDATABASEINFO_phszDBName_ptr */

/* get function:phszFilepath of class TYPEDATABASEINFO */
#ifndef TOLUA_DISABLE_tolua_get_TYPEDATABASEINFO_phszFilepath_ptr
static int tolua_get_TYPEDATABASEINFO_phszFilepath_ptr (lua_State* tolua_S) {
  TYPEDATABASEINFO*self = (TYPEDATABASEINFO*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'phszFilepath'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->phszFilepath,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TYPEDATABASEINFO_phszFilepath_ptr */

/* get function:phszFilepath of class TYPEDATABASEINFO */
#ifndef TOLUA_DISABLE_tolua_set_TYPEDATABASEINFO_phszFilepath_ptr
static int tolua_set_TYPEDATABASEINFO_phszFilepath_ptr (lua_State* tolua_S) {
  TYPEDATABASEINFO*self = (TYPEDATABASEINFO*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'phszFilepath'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->phszFilepath=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TYPEDATABASEINFO_phszFilepath_ptr */

/* get function:pArchive of class TYPEDATABASEINFO */
#ifndef TOLUA_DISABLE_tolua_get_TYPEDATABASEINFO_pArchive_ptr
static int tolua_get_TYPEDATABASEINFO_pArchive_ptr (lua_State* tolua_S) {
  TYPEDATABASEINFO*self = (TYPEDATABASEINFO*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pArchive'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->pArchive,"IArchive");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_TYPEDATABASEINFO_pArchive_ptr */

/* get function:pArchive of class TYPEDATABASEINFO */
#ifndef TOLUA_DISABLE_tolua_set_TYPEDATABASEINFO_pArchive_ptr
static int tolua_set_TYPEDATABASEINFO_pArchive_ptr (lua_State* tolua_S) {
  TYPEDATABASEINFO*self = (TYPEDATABASEINFO*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pArchive'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"IArchive",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->pArchive=(( IArchive*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_TYPEDATABASEINFO_pArchive_ptr */

/* method: new of class  TYPEDATABASEINFO */
#ifndef TOLUA_DISABLE_tolua_DatabaseScript_TYPEDATABASEINFO_new00
static int tolua_DatabaseScript_TYPEDATABASEINFO_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"TYPEDATABASEINFO",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  TYPEDATABASEINFO* tolua_ret = (TYPEDATABASEINFO*)  new TYPEDATABASEINFO();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"TYPEDATABASEINFO");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TYPEDATABASEINFO */
#ifndef TOLUA_DISABLE_tolua_DatabaseScript_TYPEDATABASEINFO_new00_local
static int tolua_DatabaseScript_TYPEDATABASEINFO_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"TYPEDATABASEINFO",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  TYPEDATABASEINFO* tolua_ret = (TYPEDATABASEINFO*)  new TYPEDATABASEINFO();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TYPEDATABASEINFO");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  TYPEDATABASEINFO */
#ifndef TOLUA_DISABLE_tolua_DatabaseScript_TYPEDATABASEINFO_delete00
static int tolua_DatabaseScript_TYPEDATABASEINFO_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"TYPEDATABASEINFO",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  TYPEDATABASEINFO* self = (TYPEDATABASEINFO*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  TYPEDATABASEINFO */
#ifndef TOLUA_DISABLE_tolua_DatabaseScript_TYPEDATABASEINFO_SIZEOF00
static int tolua_DatabaseScript_TYPEDATABASEINFO_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"TYPEDATABASEINFO",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  TYPEDATABASEINFO* self = (TYPEDATABASEINFO*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(TYPEDATABASEINFO);
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

/* get function:m_Index of class DATABASEATTRIBUTEPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_DATABASEATTRIBUTEPARAMS_m_Index
static int tolua_get_DATABASEATTRIBUTEPARAMS_m_Index (lua_State* tolua_S) {
  DATABASEATTRIBUTEPARAMS*self = (DATABASEATTRIBUTEPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Index'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_Index);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_DATABASEATTRIBUTEPARAMS_m_Index */

/* get function:m_Index of class DATABASEATTRIBUTEPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_DATABASEATTRIBUTEPARAMS_m_Index
static int tolua_set_DATABASEATTRIBUTEPARAMS_m_Index (lua_State* tolua_S) {
  DATABASEATTRIBUTEPARAMS*self = (DATABASEATTRIBUTEPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Index'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_Index=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_DATABASEATTRIBUTEPARAMS_m_Index */

/* get function:m_AttributeName of class DATABASEATTRIBUTEPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_DATABASEATTRIBUTEPARAMS_m_AttributeName_ptr
static int tolua_get_DATABASEATTRIBUTEPARAMS_m_AttributeName_ptr (lua_State* tolua_S) {
  DATABASEATTRIBUTEPARAMS*self = (DATABASEATTRIBUTEPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_AttributeName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_AttributeName,"CLuaHashString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_DATABASEATTRIBUTEPARAMS_m_AttributeName_ptr */

/* get function:m_AttributeName of class DATABASEATTRIBUTEPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_DATABASEATTRIBUTEPARAMS_m_AttributeName_ptr
static int tolua_set_DATABASEATTRIBUTEPARAMS_m_AttributeName_ptr (lua_State* tolua_S) {
  DATABASEATTRIBUTEPARAMS*self = (DATABASEATTRIBUTEPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_AttributeName'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"CLuaHashString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_AttributeName=((  CLuaHashString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_DATABASEATTRIBUTEPARAMS_m_AttributeName_ptr */

/* get function:m_Data of class DATABASEATTRIBUTEPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_DATABASEATTRIBUTEPARAMS_m_Data_ptr
static int tolua_get_DATABASEATTRIBUTEPARAMS_m_Data_ptr (lua_State* tolua_S) {
  DATABASEATTRIBUTEPARAMS*self = (DATABASEATTRIBUTEPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Data'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_Data,"DATABASEDATA");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_DATABASEATTRIBUTEPARAMS_m_Data_ptr */

/* get function:m_Data of class DATABASEATTRIBUTEPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_DATABASEATTRIBUTEPARAMS_m_Data_ptr
static int tolua_set_DATABASEATTRIBUTEPARAMS_m_Data_ptr (lua_State* tolua_S) {
  DATABASEATTRIBUTEPARAMS*self = (DATABASEATTRIBUTEPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Data'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"DATABASEDATA",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_Data=(( DATABASEDATA*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_DATABASEATTRIBUTEPARAMS_m_Data_ptr */

/* get function:m_AttributeTypeArchive of class DATABASEATTRIBUTEPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_DATABASEATTRIBUTEPARAMS_m_AttributeTypeArchive_ptr
static int tolua_get_DATABASEATTRIBUTEPARAMS_m_AttributeTypeArchive_ptr (lua_State* tolua_S) {
  DATABASEATTRIBUTEPARAMS*self = (DATABASEATTRIBUTEPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_AttributeTypeArchive'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_AttributeTypeArchive,"IArchive");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_DATABASEATTRIBUTEPARAMS_m_AttributeTypeArchive_ptr */

/* get function:m_AttributeTypeArchive of class DATABASEATTRIBUTEPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_DATABASEATTRIBUTEPARAMS_m_AttributeTypeArchive_ptr
static int tolua_set_DATABASEATTRIBUTEPARAMS_m_AttributeTypeArchive_ptr (lua_State* tolua_S) {
  DATABASEATTRIBUTEPARAMS*self = (DATABASEATTRIBUTEPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_AttributeTypeArchive'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"IArchive",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_AttributeTypeArchive=(( IArchive*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_DATABASEATTRIBUTEPARAMS_m_AttributeTypeArchive_ptr */

/* get function:m_AttributeArchive of class DATABASEATTRIBUTEPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_DATABASEATTRIBUTEPARAMS_m_AttributeArchive_ptr
static int tolua_get_DATABASEATTRIBUTEPARAMS_m_AttributeArchive_ptr (lua_State* tolua_S) {
  DATABASEATTRIBUTEPARAMS*self = (DATABASEATTRIBUTEPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_AttributeArchive'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->m_AttributeArchive,"IArchive");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_DATABASEATTRIBUTEPARAMS_m_AttributeArchive_ptr */

/* get function:m_AttributeArchive of class DATABASEATTRIBUTEPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_DATABASEATTRIBUTEPARAMS_m_AttributeArchive_ptr
static int tolua_set_DATABASEATTRIBUTEPARAMS_m_AttributeArchive_ptr (lua_State* tolua_S) {
  DATABASEATTRIBUTEPARAMS*self = (DATABASEATTRIBUTEPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_AttributeArchive'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"IArchive",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_AttributeArchive=(( IArchive*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_DATABASEATTRIBUTEPARAMS_m_AttributeArchive_ptr */

/* method: new of class  DATABASEATTRIBUTEPARAMS */
#ifndef TOLUA_DISABLE_tolua_DatabaseScript_DATABASEATTRIBUTEPARAMS_new00
static int tolua_DatabaseScript_DATABASEATTRIBUTEPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"DATABASEATTRIBUTEPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  DATABASEATTRIBUTEPARAMS* tolua_ret = (DATABASEATTRIBUTEPARAMS*)  new DATABASEATTRIBUTEPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"DATABASEATTRIBUTEPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  DATABASEATTRIBUTEPARAMS */
#ifndef TOLUA_DISABLE_tolua_DatabaseScript_DATABASEATTRIBUTEPARAMS_new00_local
static int tolua_DatabaseScript_DATABASEATTRIBUTEPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"DATABASEATTRIBUTEPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  DATABASEATTRIBUTEPARAMS* tolua_ret = (DATABASEATTRIBUTEPARAMS*)  new DATABASEATTRIBUTEPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"DATABASEATTRIBUTEPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  DATABASEATTRIBUTEPARAMS */
#ifndef TOLUA_DISABLE_tolua_DatabaseScript_DATABASEATTRIBUTEPARAMS_delete00
static int tolua_DatabaseScript_DATABASEATTRIBUTEPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"DATABASEATTRIBUTEPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  DATABASEATTRIBUTEPARAMS* self = (DATABASEATTRIBUTEPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  DATABASEATTRIBUTEPARAMS */
#ifndef TOLUA_DISABLE_tolua_DatabaseScript_DATABASEATTRIBUTEPARAMS_SIZEOF00
static int tolua_DatabaseScript_DATABASEATTRIBUTEPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"DATABASEATTRIBUTEPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  DATABASEATTRIBUTEPARAMS* self = (DATABASEATTRIBUTEPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(DATABASEATTRIBUTEPARAMS);
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

/* get function:m_NumItems of class DATABASEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_DATABASEINFOPARAMS_m_NumItems
static int tolua_get_DATABASEINFOPARAMS_m_NumItems (lua_State* tolua_S) {
  DATABASEINFOPARAMS*self = (DATABASEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_NumItems'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_NumItems);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_DATABASEINFOPARAMS_m_NumItems */

/* get function:m_NumItems of class DATABASEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_DATABASEINFOPARAMS_m_NumItems
static int tolua_set_DATABASEINFOPARAMS_m_NumItems (lua_State* tolua_S) {
  DATABASEINFOPARAMS*self = (DATABASEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_NumItems'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_NumItems=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_DATABASEINFOPARAMS_m_NumItems */

/* method: new of class  DATABASEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_DatabaseScript_DATABASEINFOPARAMS_new00
static int tolua_DatabaseScript_DATABASEINFOPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"DATABASEINFOPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  DATABASEINFOPARAMS* tolua_ret = (DATABASEINFOPARAMS*)  new DATABASEINFOPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"DATABASEINFOPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  DATABASEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_DatabaseScript_DATABASEINFOPARAMS_new00_local
static int tolua_DatabaseScript_DATABASEINFOPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"DATABASEINFOPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  DATABASEINFOPARAMS* tolua_ret = (DATABASEINFOPARAMS*)  new DATABASEINFOPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"DATABASEINFOPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  DATABASEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_DatabaseScript_DATABASEINFOPARAMS_delete00
static int tolua_DatabaseScript_DATABASEINFOPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"DATABASEINFOPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  DATABASEINFOPARAMS* self = (DATABASEINFOPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  DATABASEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_DatabaseScript_DATABASEINFOPARAMS_SIZEOF00
static int tolua_DatabaseScript_DATABASEINFOPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"DATABASEINFOPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  DATABASEINFOPARAMS* self = (DATABASEINFOPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(DATABASEINFOPARAMS);
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
TOLUA_API int tolua_DatabaseScript_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"TYPEDATABASEINFO","TYPEDATABASEINFO","",tolua_collect_TYPEDATABASEINFO);
 #else
 tolua_cclass(tolua_S,"TYPEDATABASEINFO","TYPEDATABASEINFO","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"TYPEDATABASEINFO");
  tolua_variable(tolua_S,"phszDBName",tolua_get_TYPEDATABASEINFO_phszDBName_ptr,tolua_set_TYPEDATABASEINFO_phszDBName_ptr);
  tolua_variable(tolua_S,"phszFilepath",tolua_get_TYPEDATABASEINFO_phszFilepath_ptr,tolua_set_TYPEDATABASEINFO_phszFilepath_ptr);
  tolua_variable(tolua_S,"pArchive",tolua_get_TYPEDATABASEINFO_pArchive_ptr,tolua_set_TYPEDATABASEINFO_pArchive_ptr);
  tolua_function(tolua_S,"new",tolua_DatabaseScript_TYPEDATABASEINFO_new00);
  tolua_function(tolua_S,"new_local",tolua_DatabaseScript_TYPEDATABASEINFO_new00_local);
  tolua_function(tolua_S,".call",tolua_DatabaseScript_TYPEDATABASEINFO_new00_local);
  tolua_function(tolua_S,"delete",tolua_DatabaseScript_TYPEDATABASEINFO_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_DatabaseScript_TYPEDATABASEINFO_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"DATABASEATTRIBUTEPARAMS","DATABASEATTRIBUTEPARAMS","",tolua_collect_DATABASEATTRIBUTEPARAMS);
 #else
 tolua_cclass(tolua_S,"DATABASEATTRIBUTEPARAMS","DATABASEATTRIBUTEPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"DATABASEATTRIBUTEPARAMS");
  tolua_variable(tolua_S,"m_Index",tolua_get_DATABASEATTRIBUTEPARAMS_m_Index,tolua_set_DATABASEATTRIBUTEPARAMS_m_Index);
  tolua_variable(tolua_S,"m_AttributeName",tolua_get_DATABASEATTRIBUTEPARAMS_m_AttributeName_ptr,tolua_set_DATABASEATTRIBUTEPARAMS_m_AttributeName_ptr);
  tolua_variable(tolua_S,"m_Data",tolua_get_DATABASEATTRIBUTEPARAMS_m_Data_ptr,tolua_set_DATABASEATTRIBUTEPARAMS_m_Data_ptr);
  tolua_variable(tolua_S,"m_AttributeTypeArchive",tolua_get_DATABASEATTRIBUTEPARAMS_m_AttributeTypeArchive_ptr,tolua_set_DATABASEATTRIBUTEPARAMS_m_AttributeTypeArchive_ptr);
  tolua_variable(tolua_S,"m_AttributeArchive",tolua_get_DATABASEATTRIBUTEPARAMS_m_AttributeArchive_ptr,tolua_set_DATABASEATTRIBUTEPARAMS_m_AttributeArchive_ptr);
  tolua_function(tolua_S,"new",tolua_DatabaseScript_DATABASEATTRIBUTEPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_DatabaseScript_DATABASEATTRIBUTEPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_DatabaseScript_DATABASEATTRIBUTEPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_DatabaseScript_DATABASEATTRIBUTEPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_DatabaseScript_DATABASEATTRIBUTEPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"DATABASEINFOPARAMS","DATABASEINFOPARAMS","",tolua_collect_DATABASEINFOPARAMS);
 #else
 tolua_cclass(tolua_S,"DATABASEINFOPARAMS","DATABASEINFOPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"DATABASEINFOPARAMS");
  tolua_variable(tolua_S,"m_NumItems",tolua_get_DATABASEINFOPARAMS_m_NumItems,tolua_set_DATABASEINFOPARAMS_m_NumItems);
  tolua_function(tolua_S,"new",tolua_DatabaseScript_DATABASEINFOPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_DatabaseScript_DATABASEINFOPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_DatabaseScript_DATABASEINFOPARAMS_new00_local);
  tolua_function(tolua_S,"delete",tolua_DatabaseScript_DATABASEINFOPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_DatabaseScript_DATABASEINFOPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_DatabaseScript (lua_State* tolua_S) {
 return tolua_DatabaseScript_open(tolua_S);
};
#endif

