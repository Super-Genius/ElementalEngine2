/*
** Lua binding: SplineScript
** Generated automatically by toluaxx-1.2.0 on 02/02/09 21:48:25.
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
TOLUA_API int tolua_SplineScript_open (lua_State* tolua_S);

#include "StdAfx.h"
#include "../../ElementalEngine/EELuaScript/LuaHashString.h"
#pragma warning(disable : 4800)
#pragma warning(disable : 4345)
class CLuaDWORD { public: CLuaDWORD() {} CLuaDWORD(unsigned long i) { x = i; } operator DWORD() { return x; } void operator=(DWORD i) { x = i; } unsigned long x; };
#if 0 // To not include the "../"
#include "../../include/Spline/SplineDefines.h"
#endif

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_SPLINELOOKAHEADPARAMS (lua_State* tolua_S)
{
 SPLINELOOKAHEADPARAMS* self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CLuaDWORD (lua_State* tolua_S)
{
 CLuaDWORD* self = (CLuaDWORD*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_SPLINEINFOPARAMS (lua_State* tolua_S)
{
 SPLINEINFOPARAMS* self = (SPLINEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_SPLINEGETNEARESTPARAMS (lua_State* tolua_S)
{
 SPLINEGETNEARESTPARAMS* self = (SPLINEGETNEARESTPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_SPLINEMARKERPARAMS (lua_State* tolua_S)
{
 SPLINEMARKERPARAMS* self = (SPLINEMARKERPARAMS*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"SPLINELOOKAHEADPARAMS");
 tolua_usertype(tolua_S,"CLuaDWORD");
 tolua_usertype(tolua_S,"SPLINEINFOPARAMS");
 tolua_usertype(tolua_S,"SPLINEMARKERPARAMS");
 tolua_usertype(tolua_S,"SPLINEGETNEARESTPARAMS");
 tolua_usertype(tolua_S,"Vec3");
 tolua_usertype(tolua_S,"StdString");
}

/* method: new of class  SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_SplineScript_SPLINEINFOPARAMS_new00
static int tolua_SplineScript_SPLINEINFOPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"SPLINEINFOPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  SPLINEINFOPARAMS* tolua_ret = (SPLINEINFOPARAMS*)  new SPLINEINFOPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"SPLINEINFOPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_SplineScript_SPLINEINFOPARAMS_new00_local
static int tolua_SplineScript_SPLINEINFOPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"SPLINEINFOPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  SPLINEINFOPARAMS* tolua_ret = (SPLINEINFOPARAMS*)  new SPLINEINFOPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"SPLINEINFOPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function:m_szSplineType of class SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINEINFOPARAMS_m_szSplineType
static int tolua_get_SPLINEINFOPARAMS_m_szSplineType (lua_State* tolua_S) {
  SPLINEINFOPARAMS*self = (SPLINEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_szSplineType'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->m_szSplineType,"StdString");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINEINFOPARAMS_m_szSplineType */

/* get function:m_szSplineType of class SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINEINFOPARAMS_m_szSplineType
static int tolua_set_SPLINEINFOPARAMS_m_szSplineType (lua_State* tolua_S) {
  SPLINEINFOPARAMS*self = (SPLINEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_szSplineType'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"StdString",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_szSplineType=*(( StdString*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINEINFOPARAMS_m_szSplineType */

/* get function:m_bSplineLoop of class SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINEINFOPARAMS_m_bSplineLoop
static int tolua_get_SPLINEINFOPARAMS_m_bSplineLoop (lua_State* tolua_S) {
  SPLINEINFOPARAMS*self = (SPLINEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_bSplineLoop'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->m_bSplineLoop);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINEINFOPARAMS_m_bSplineLoop */

/* get function:m_bSplineLoop of class SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINEINFOPARAMS_m_bSplineLoop
static int tolua_set_SPLINEINFOPARAMS_m_bSplineLoop (lua_State* tolua_S) {
  SPLINEINFOPARAMS*self = (SPLINEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_bSplineLoop'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_bSplineLoop=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINEINFOPARAMS_m_bSplineLoop */

/* get function:m_iResolution of class SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINEINFOPARAMS_unsigned_m_iResolution
static int tolua_get_SPLINEINFOPARAMS_unsigned_m_iResolution (lua_State* tolua_S) {
  SPLINEINFOPARAMS*self = (SPLINEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_iResolution'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_iResolution);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINEINFOPARAMS_unsigned_m_iResolution */

/* get function:m_iResolution of class SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINEINFOPARAMS_unsigned_m_iResolution
static int tolua_set_SPLINEINFOPARAMS_unsigned_m_iResolution (lua_State* tolua_S) {
  SPLINEINFOPARAMS*self = (SPLINEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_iResolution'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->m_iResolution=((unsigned int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINEINFOPARAMS_unsigned_m_iResolution */

/* get function:SplinePointsCount of class SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINEINFOPARAMS_SplinePointsCount
static int tolua_get_SPLINEINFOPARAMS_SplinePointsCount (lua_State* tolua_S) {
  SPLINEINFOPARAMS*self = (SPLINEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SplinePointsCount'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->SplinePointsCount);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINEINFOPARAMS_SplinePointsCount */

/* get function:SplinePointsCount of class SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINEINFOPARAMS_SplinePointsCount
static int tolua_set_SPLINEINFOPARAMS_SplinePointsCount (lua_State* tolua_S) {
  SPLINEINFOPARAMS*self = (SPLINEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SplinePointsCount'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->SplinePointsCount=(( int) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINEINFOPARAMS_SplinePointsCount */

/* get function:MarkerNearEdge of class SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINEINFOPARAMS_MarkerNearEdge
static int tolua_get_SPLINEINFOPARAMS_MarkerNearEdge (lua_State* tolua_S) {
  SPLINEINFOPARAMS*self = (SPLINEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MarkerNearEdge'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->MarkerNearEdge);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINEINFOPARAMS_MarkerNearEdge */

/* get function:MarkerNearEdge of class SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINEINFOPARAMS_MarkerNearEdge
static int tolua_set_SPLINEINFOPARAMS_MarkerNearEdge (lua_State* tolua_S) {
  SPLINEINFOPARAMS*self = (SPLINEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MarkerNearEdge'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->MarkerNearEdge=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINEINFOPARAMS_MarkerNearEdge */

/* get function:PreviousSplinePoint of class SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINEINFOPARAMS_PreviousSplinePoint_ptr
static int tolua_get_SPLINEINFOPARAMS_PreviousSplinePoint_ptr (lua_State* tolua_S) {
  SPLINEINFOPARAMS*self = (SPLINEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'PreviousSplinePoint'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->PreviousSplinePoint,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINEINFOPARAMS_PreviousSplinePoint_ptr */

/* get function:PreviousSplinePoint of class SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINEINFOPARAMS_PreviousSplinePoint_ptr
static int tolua_set_SPLINEINFOPARAMS_PreviousSplinePoint_ptr (lua_State* tolua_S) {
  SPLINEINFOPARAMS*self = (SPLINEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'PreviousSplinePoint'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->PreviousSplinePoint=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINEINFOPARAMS_PreviousSplinePoint_ptr */

/* get function:NextSplinePoint of class SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINEINFOPARAMS_NextSplinePoint_ptr
static int tolua_get_SPLINEINFOPARAMS_NextSplinePoint_ptr (lua_State* tolua_S) {
  SPLINEINFOPARAMS*self = (SPLINEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'NextSplinePoint'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->NextSplinePoint,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINEINFOPARAMS_NextSplinePoint_ptr */

/* get function:NextSplinePoint of class SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINEINFOPARAMS_NextSplinePoint_ptr
static int tolua_set_SPLINEINFOPARAMS_NextSplinePoint_ptr (lua_State* tolua_S) {
  SPLINEINFOPARAMS*self = (SPLINEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'NextSplinePoint'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->NextSplinePoint=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINEINFOPARAMS_NextSplinePoint_ptr */

/* get function:EndSplinePoint of class SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINEINFOPARAMS_EndSplinePoint_ptr
static int tolua_get_SPLINEINFOPARAMS_EndSplinePoint_ptr (lua_State* tolua_S) {
  SPLINEINFOPARAMS*self = (SPLINEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EndSplinePoint'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->EndSplinePoint,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINEINFOPARAMS_EndSplinePoint_ptr */

/* get function:EndSplinePoint of class SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINEINFOPARAMS_EndSplinePoint_ptr
static int tolua_set_SPLINEINFOPARAMS_EndSplinePoint_ptr (lua_State* tolua_S) {
  SPLINEINFOPARAMS*self = (SPLINEINFOPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EndSplinePoint'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->EndSplinePoint=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINEINFOPARAMS_EndSplinePoint_ptr */

/* method: delete of class  SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_SplineScript_SPLINEINFOPARAMS_delete00
static int tolua_SplineScript_SPLINEINFOPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"SPLINEINFOPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  SPLINEINFOPARAMS* self = (SPLINEINFOPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  SPLINEINFOPARAMS */
#ifndef TOLUA_DISABLE_tolua_SplineScript_SPLINEINFOPARAMS_SIZEOF00
static int tolua_SplineScript_SPLINEINFOPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"SPLINEINFOPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  SPLINEINFOPARAMS* self = (SPLINEINFOPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(SPLINEINFOPARAMS);
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

/* method: new of class  SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_SplineScript_SPLINELOOKAHEADPARAMS_new00
static int tolua_SplineScript_SPLINELOOKAHEADPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"SPLINELOOKAHEADPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  SPLINELOOKAHEADPARAMS* tolua_ret = (SPLINELOOKAHEADPARAMS*)  new SPLINELOOKAHEADPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"SPLINELOOKAHEADPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_SplineScript_SPLINELOOKAHEADPARAMS_new00_local
static int tolua_SplineScript_SPLINELOOKAHEADPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"SPLINELOOKAHEADPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  SPLINELOOKAHEADPARAMS* tolua_ret = (SPLINELOOKAHEADPARAMS*)  new SPLINELOOKAHEADPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"SPLINELOOKAHEADPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function:FirstDerivativeMaxLength of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeMaxLength
static int tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeMaxLength (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'FirstDerivativeMaxLength'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->FirstDerivativeMaxLength);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeMaxLength */

/* get function:FirstDerivativeMaxLength of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeMaxLength
static int tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeMaxLength (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'FirstDerivativeMaxLength'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->FirstDerivativeMaxLength=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeMaxLength */

/* get function:SecondDerivativeMaxLength of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeMaxLength
static int tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeMaxLength (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SecondDerivativeMaxLength'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->SecondDerivativeMaxLength);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeMaxLength */

/* get function:SecondDerivativeMaxLength of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeMaxLength
static int tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeMaxLength (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SecondDerivativeMaxLength'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->SecondDerivativeMaxLength=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeMaxLength */

/* get function:FirstDerivativeXYMaxLength of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeXYMaxLength
static int tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeXYMaxLength (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'FirstDerivativeXYMaxLength'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->FirstDerivativeXYMaxLength);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeXYMaxLength */

/* get function:FirstDerivativeXYMaxLength of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeXYMaxLength
static int tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeXYMaxLength (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'FirstDerivativeXYMaxLength'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->FirstDerivativeXYMaxLength=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeXYMaxLength */

/* get function:SecondDerivativeXYMaxLength of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeXYMaxLength
static int tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeXYMaxLength (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SecondDerivativeXYMaxLength'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->SecondDerivativeXYMaxLength);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeXYMaxLength */

/* get function:SecondDerivativeXYMaxLength of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeXYMaxLength
static int tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeXYMaxLength (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SecondDerivativeXYMaxLength'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->SecondDerivativeXYMaxLength=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeXYMaxLength */

/* get function:FirstDerivativeXZMaxLength of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeXZMaxLength
static int tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeXZMaxLength (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'FirstDerivativeXZMaxLength'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->FirstDerivativeXZMaxLength);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeXZMaxLength */

/* get function:FirstDerivativeXZMaxLength of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeXZMaxLength
static int tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeXZMaxLength (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'FirstDerivativeXZMaxLength'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->FirstDerivativeXZMaxLength=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeXZMaxLength */

/* get function:SecondDerivativeXZMaxLength of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeXZMaxLength
static int tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeXZMaxLength (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SecondDerivativeXZMaxLength'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->SecondDerivativeXZMaxLength);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeXZMaxLength */

/* get function:SecondDerivativeXZMaxLength of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeXZMaxLength
static int tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeXZMaxLength (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SecondDerivativeXZMaxLength'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->SecondDerivativeXZMaxLength=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeXZMaxLength */

/* get function:FirstDerivativeMax of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeMax_ptr
static int tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeMax_ptr (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'FirstDerivativeMax'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->FirstDerivativeMax,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeMax_ptr */

/* get function:FirstDerivativeMax of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeMax_ptr
static int tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeMax_ptr (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'FirstDerivativeMax'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->FirstDerivativeMax=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeMax_ptr */

/* get function:SecondDerivativeMax of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeMax_ptr
static int tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeMax_ptr (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SecondDerivativeMax'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->SecondDerivativeMax,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeMax_ptr */

/* get function:SecondDerivativeMax of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeMax_ptr
static int tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeMax_ptr (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SecondDerivativeMax'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->SecondDerivativeMax=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeMax_ptr */

/* get function:FirstDerivativeXYMax of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeXYMax_ptr
static int tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeXYMax_ptr (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'FirstDerivativeXYMax'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->FirstDerivativeXYMax,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeXYMax_ptr */

/* get function:FirstDerivativeXYMax of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeXYMax_ptr
static int tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeXYMax_ptr (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'FirstDerivativeXYMax'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->FirstDerivativeXYMax=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeXYMax_ptr */

/* get function:SecondDerivativeXYMax of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeXYMax_ptr
static int tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeXYMax_ptr (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SecondDerivativeXYMax'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->SecondDerivativeXYMax,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeXYMax_ptr */

/* get function:SecondDerivativeXYMax of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeXYMax_ptr
static int tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeXYMax_ptr (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SecondDerivativeXYMax'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->SecondDerivativeXYMax=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeXYMax_ptr */

/* get function:FirstDerivativeXZMax of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeXZMax_ptr
static int tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeXZMax_ptr (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'FirstDerivativeXZMax'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->FirstDerivativeXZMax,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeXZMax_ptr */

/* get function:FirstDerivativeXZMax of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeXZMax_ptr
static int tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeXZMax_ptr (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'FirstDerivativeXZMax'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->FirstDerivativeXZMax=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeXZMax_ptr */

/* get function:SecondDerivativeXZMax of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeXZMax_ptr
static int tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeXZMax_ptr (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SecondDerivativeXZMax'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->SecondDerivativeXZMax,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeXZMax_ptr */

/* get function:SecondDerivativeXZMax of class SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeXZMax_ptr
static int tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeXZMax_ptr (lua_State* tolua_S) {
  SPLINELOOKAHEADPARAMS*self = (SPLINELOOKAHEADPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SecondDerivativeXZMax'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->SecondDerivativeXZMax=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeXZMax_ptr */

/* method: delete of class  SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_SplineScript_SPLINELOOKAHEADPARAMS_delete00
static int tolua_SplineScript_SPLINELOOKAHEADPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"SPLINELOOKAHEADPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  SPLINELOOKAHEADPARAMS* self = (SPLINELOOKAHEADPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  SPLINELOOKAHEADPARAMS */
#ifndef TOLUA_DISABLE_tolua_SplineScript_SPLINELOOKAHEADPARAMS_SIZEOF00
static int tolua_SplineScript_SPLINELOOKAHEADPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"SPLINELOOKAHEADPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  SPLINELOOKAHEADPARAMS* self = (SPLINELOOKAHEADPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(SPLINELOOKAHEADPARAMS);
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

/* method: new of class  SPLINEMARKERPARAMS */
#ifndef TOLUA_DISABLE_tolua_SplineScript_SPLINEMARKERPARAMS_new00
static int tolua_SplineScript_SPLINEMARKERPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"SPLINEMARKERPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  SPLINEMARKERPARAMS* tolua_ret = (SPLINEMARKERPARAMS*)  new SPLINEMARKERPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"SPLINEMARKERPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  SPLINEMARKERPARAMS */
#ifndef TOLUA_DISABLE_tolua_SplineScript_SPLINEMARKERPARAMS_new00_local
static int tolua_SplineScript_SPLINEMARKERPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"SPLINEMARKERPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  SPLINEMARKERPARAMS* tolua_ret = (SPLINEMARKERPARAMS*)  new SPLINEMARKERPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"SPLINEMARKERPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function:bEndOfSpline of class SPLINEMARKERPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINEMARKERPARAMS_bEndOfSpline
static int tolua_get_SPLINEMARKERPARAMS_bEndOfSpline (lua_State* tolua_S) {
  SPLINEMARKERPARAMS*self = (SPLINEMARKERPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bEndOfSpline'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->bEndOfSpline);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINEMARKERPARAMS_bEndOfSpline */

/* get function:bEndOfSpline of class SPLINEMARKERPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINEMARKERPARAMS_bEndOfSpline
static int tolua_set_SPLINEMARKERPARAMS_bEndOfSpline (lua_State* tolua_S) {
  SPLINEMARKERPARAMS*self = (SPLINEMARKERPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bEndOfSpline'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->bEndOfSpline=(( bool) tolua_toboolean(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINEMARKERPARAMS_bEndOfSpline */

/* get function:Distance of class SPLINEMARKERPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINEMARKERPARAMS_Distance
static int tolua_get_SPLINEMARKERPARAMS_Distance (lua_State* tolua_S) {
  SPLINEMARKERPARAMS*self = (SPLINEMARKERPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Distance'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->Distance);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINEMARKERPARAMS_Distance */

/* get function:Distance of class SPLINEMARKERPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINEMARKERPARAMS_Distance
static int tolua_set_SPLINEMARKERPARAMS_Distance (lua_State* tolua_S) {
  SPLINEMARKERPARAMS*self = (SPLINEMARKERPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Distance'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->Distance=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINEMARKERPARAMS_Distance */

/* get function:Position of class SPLINEMARKERPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINEMARKERPARAMS_Position_ptr
static int tolua_get_SPLINEMARKERPARAMS_Position_ptr (lua_State* tolua_S) {
  SPLINEMARKERPARAMS*self = (SPLINEMARKERPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Position'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->Position,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINEMARKERPARAMS_Position_ptr */

/* get function:Position of class SPLINEMARKERPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINEMARKERPARAMS_Position_ptr
static int tolua_set_SPLINEMARKERPARAMS_Position_ptr (lua_State* tolua_S) {
  SPLINEMARKERPARAMS*self = (SPLINEMARKERPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Position'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->Position=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINEMARKERPARAMS_Position_ptr */

/* get function:FirstDerivative of class SPLINEMARKERPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINEMARKERPARAMS_FirstDerivative_ptr
static int tolua_get_SPLINEMARKERPARAMS_FirstDerivative_ptr (lua_State* tolua_S) {
  SPLINEMARKERPARAMS*self = (SPLINEMARKERPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'FirstDerivative'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->FirstDerivative,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINEMARKERPARAMS_FirstDerivative_ptr */

/* get function:FirstDerivative of class SPLINEMARKERPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINEMARKERPARAMS_FirstDerivative_ptr
static int tolua_set_SPLINEMARKERPARAMS_FirstDerivative_ptr (lua_State* tolua_S) {
  SPLINEMARKERPARAMS*self = (SPLINEMARKERPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'FirstDerivative'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->FirstDerivative=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINEMARKERPARAMS_FirstDerivative_ptr */

/* get function:SecondDerivative of class SPLINEMARKERPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINEMARKERPARAMS_SecondDerivative_ptr
static int tolua_get_SPLINEMARKERPARAMS_SecondDerivative_ptr (lua_State* tolua_S) {
  SPLINEMARKERPARAMS*self = (SPLINEMARKERPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SecondDerivative'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->SecondDerivative,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINEMARKERPARAMS_SecondDerivative_ptr */

/* get function:SecondDerivative of class SPLINEMARKERPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINEMARKERPARAMS_SecondDerivative_ptr
static int tolua_set_SPLINEMARKERPARAMS_SecondDerivative_ptr (lua_State* tolua_S) {
  SPLINEMARKERPARAMS*self = (SPLINEMARKERPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SecondDerivative'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->SecondDerivative=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINEMARKERPARAMS_SecondDerivative_ptr */

/* method: delete of class  SPLINEMARKERPARAMS */
#ifndef TOLUA_DISABLE_tolua_SplineScript_SPLINEMARKERPARAMS_delete00
static int tolua_SplineScript_SPLINEMARKERPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"SPLINEMARKERPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  SPLINEMARKERPARAMS* self = (SPLINEMARKERPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  SPLINEMARKERPARAMS */
#ifndef TOLUA_DISABLE_tolua_SplineScript_SPLINEMARKERPARAMS_SIZEOF00
static int tolua_SplineScript_SPLINEMARKERPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"SPLINEMARKERPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  SPLINEMARKERPARAMS* self = (SPLINEMARKERPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(SPLINEMARKERPARAMS);
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

/* method: new of class  SPLINEGETNEARESTPARAMS */
#ifndef TOLUA_DISABLE_tolua_SplineScript_SPLINEGETNEARESTPARAMS_new00
static int tolua_SplineScript_SPLINEGETNEARESTPARAMS_new00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"SPLINEGETNEARESTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  SPLINEGETNEARESTPARAMS* tolua_ret = (SPLINEGETNEARESTPARAMS*)  new SPLINEGETNEARESTPARAMS();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"SPLINEGETNEARESTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  SPLINEGETNEARESTPARAMS */
#ifndef TOLUA_DISABLE_tolua_SplineScript_SPLINEGETNEARESTPARAMS_new00_local
static int tolua_SplineScript_SPLINEGETNEARESTPARAMS_new00_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"SPLINEGETNEARESTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  SPLINEGETNEARESTPARAMS* tolua_ret = (SPLINEGETNEARESTPARAMS*)  new SPLINEGETNEARESTPARAMS();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"SPLINEGETNEARESTPARAMS");
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function:Distance of class SPLINEGETNEARESTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINEGETNEARESTPARAMS_Distance
static int tolua_get_SPLINEGETNEARESTPARAMS_Distance (lua_State* tolua_S) {
  SPLINEGETNEARESTPARAMS*self = (SPLINEGETNEARESTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Distance'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->Distance);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINEGETNEARESTPARAMS_Distance */

/* get function:Distance of class SPLINEGETNEARESTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINEGETNEARESTPARAMS_Distance
static int tolua_set_SPLINEGETNEARESTPARAMS_Distance (lua_State* tolua_S) {
  SPLINEGETNEARESTPARAMS*self = (SPLINEGETNEARESTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Distance'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->Distance=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINEGETNEARESTPARAMS_Distance */

/* get function:MinDistance of class SPLINEGETNEARESTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINEGETNEARESTPARAMS_MinDistance
static int tolua_get_SPLINEGETNEARESTPARAMS_MinDistance (lua_State* tolua_S) {
  SPLINEGETNEARESTPARAMS*self = (SPLINEGETNEARESTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MinDistance'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->MinDistance);
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINEGETNEARESTPARAMS_MinDistance */

/* get function:MinDistance of class SPLINEGETNEARESTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINEGETNEARESTPARAMS_MinDistance
static int tolua_set_SPLINEGETNEARESTPARAMS_MinDistance (lua_State* tolua_S) {
  SPLINEGETNEARESTPARAMS*self = (SPLINEGETNEARESTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MinDistance'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->MinDistance=(( float) tolua_tonumber(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINEGETNEARESTPARAMS_MinDistance */

/* get function:Position of class SPLINEGETNEARESTPARAMS */
#ifndef TOLUA_DISABLE_tolua_get_SPLINEGETNEARESTPARAMS_Position_ptr
static int tolua_get_SPLINEGETNEARESTPARAMS_Position_ptr (lua_State* tolua_S) {
  SPLINEGETNEARESTPARAMS*self = (SPLINEGETNEARESTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Position'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->Position,"Vec3");
 return 1;}
#endif /* #ifndef TOLUA_DISABLE_tolua_get_SPLINEGETNEARESTPARAMS_Position_ptr */

/* get function:Position of class SPLINEGETNEARESTPARAMS */
#ifndef TOLUA_DISABLE_tolua_set_SPLINEGETNEARESTPARAMS_Position_ptr
static int tolua_set_SPLINEGETNEARESTPARAMS_Position_ptr (lua_State* tolua_S) {
  SPLINEGETNEARESTPARAMS*self = (SPLINEGETNEARESTPARAMS*) tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Position'",NULL);
#endif
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"Vec3",0,&tolua_err)) tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 self->Position=(( Vec3*) tolua_tousertype(tolua_S,2,0));
 return 0;}
#endif /* #ifndef TOLUA_DISABLE_tolua_set_SPLINEGETNEARESTPARAMS_Position_ptr */

/* method: delete of class  SPLINEGETNEARESTPARAMS */
#ifndef TOLUA_DISABLE_tolua_SplineScript_SPLINEGETNEARESTPARAMS_delete00
static int tolua_SplineScript_SPLINEGETNEARESTPARAMS_delete00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"SPLINEGETNEARESTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  SPLINEGETNEARESTPARAMS* self = (SPLINEGETNEARESTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
 delete self;
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SIZEOF of class  SPLINEGETNEARESTPARAMS */
#ifndef TOLUA_DISABLE_tolua_SplineScript_SPLINEGETNEARESTPARAMS_SIZEOF00
static int tolua_SplineScript_SPLINEGETNEARESTPARAMS_SIZEOF00(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"SPLINEGETNEARESTPARAMS",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  SPLINEGETNEARESTPARAMS* self = (SPLINEGETNEARESTPARAMS*)  tolua_tousertype(tolua_S,1,0);
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SIZEOF'",NULL);
 {
   CLuaDWORD tolua_ret = (  CLuaDWORD)  sizeof(SPLINEGETNEARESTPARAMS);
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
TOLUA_API int tolua_SplineScript_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"SPLINEINFOPARAMS","SPLINEINFOPARAMS","",tolua_collect_SPLINEINFOPARAMS);
 #else
 tolua_cclass(tolua_S,"SPLINEINFOPARAMS","SPLINEINFOPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"SPLINEINFOPARAMS");
  tolua_function(tolua_S,"new",tolua_SplineScript_SPLINEINFOPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_SplineScript_SPLINEINFOPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_SplineScript_SPLINEINFOPARAMS_new00_local);
  tolua_variable(tolua_S,"m_szSplineType",tolua_get_SPLINEINFOPARAMS_m_szSplineType,tolua_set_SPLINEINFOPARAMS_m_szSplineType);
  tolua_variable(tolua_S,"m_bSplineLoop",tolua_get_SPLINEINFOPARAMS_m_bSplineLoop,tolua_set_SPLINEINFOPARAMS_m_bSplineLoop);
  tolua_variable(tolua_S,"m_iResolution",tolua_get_SPLINEINFOPARAMS_unsigned_m_iResolution,tolua_set_SPLINEINFOPARAMS_unsigned_m_iResolution);
  tolua_variable(tolua_S,"SplinePointsCount",tolua_get_SPLINEINFOPARAMS_SplinePointsCount,tolua_set_SPLINEINFOPARAMS_SplinePointsCount);
  tolua_variable(tolua_S,"MarkerNearEdge",tolua_get_SPLINEINFOPARAMS_MarkerNearEdge,tolua_set_SPLINEINFOPARAMS_MarkerNearEdge);
  tolua_variable(tolua_S,"PreviousSplinePoint",tolua_get_SPLINEINFOPARAMS_PreviousSplinePoint_ptr,tolua_set_SPLINEINFOPARAMS_PreviousSplinePoint_ptr);
  tolua_variable(tolua_S,"NextSplinePoint",tolua_get_SPLINEINFOPARAMS_NextSplinePoint_ptr,tolua_set_SPLINEINFOPARAMS_NextSplinePoint_ptr);
  tolua_variable(tolua_S,"EndSplinePoint",tolua_get_SPLINEINFOPARAMS_EndSplinePoint_ptr,tolua_set_SPLINEINFOPARAMS_EndSplinePoint_ptr);
  tolua_function(tolua_S,"delete",tolua_SplineScript_SPLINEINFOPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_SplineScript_SPLINEINFOPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"SPLINELOOKAHEADPARAMS","SPLINELOOKAHEADPARAMS","",tolua_collect_SPLINELOOKAHEADPARAMS);
 #else
 tolua_cclass(tolua_S,"SPLINELOOKAHEADPARAMS","SPLINELOOKAHEADPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"SPLINELOOKAHEADPARAMS");
  tolua_function(tolua_S,"new",tolua_SplineScript_SPLINELOOKAHEADPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_SplineScript_SPLINELOOKAHEADPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_SplineScript_SPLINELOOKAHEADPARAMS_new00_local);
  tolua_variable(tolua_S,"FirstDerivativeMaxLength",tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeMaxLength,tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeMaxLength);
  tolua_variable(tolua_S,"SecondDerivativeMaxLength",tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeMaxLength,tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeMaxLength);
  tolua_variable(tolua_S,"FirstDerivativeXYMaxLength",tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeXYMaxLength,tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeXYMaxLength);
  tolua_variable(tolua_S,"SecondDerivativeXYMaxLength",tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeXYMaxLength,tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeXYMaxLength);
  tolua_variable(tolua_S,"FirstDerivativeXZMaxLength",tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeXZMaxLength,tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeXZMaxLength);
  tolua_variable(tolua_S,"SecondDerivativeXZMaxLength",tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeXZMaxLength,tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeXZMaxLength);
  tolua_variable(tolua_S,"FirstDerivativeMax",tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeMax_ptr,tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeMax_ptr);
  tolua_variable(tolua_S,"SecondDerivativeMax",tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeMax_ptr,tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeMax_ptr);
  tolua_variable(tolua_S,"FirstDerivativeXYMax",tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeXYMax_ptr,tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeXYMax_ptr);
  tolua_variable(tolua_S,"SecondDerivativeXYMax",tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeXYMax_ptr,tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeXYMax_ptr);
  tolua_variable(tolua_S,"FirstDerivativeXZMax",tolua_get_SPLINELOOKAHEADPARAMS_FirstDerivativeXZMax_ptr,tolua_set_SPLINELOOKAHEADPARAMS_FirstDerivativeXZMax_ptr);
  tolua_variable(tolua_S,"SecondDerivativeXZMax",tolua_get_SPLINELOOKAHEADPARAMS_SecondDerivativeXZMax_ptr,tolua_set_SPLINELOOKAHEADPARAMS_SecondDerivativeXZMax_ptr);
  tolua_function(tolua_S,"delete",tolua_SplineScript_SPLINELOOKAHEADPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_SplineScript_SPLINELOOKAHEADPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"SPLINEMARKERPARAMS","SPLINEMARKERPARAMS","",tolua_collect_SPLINEMARKERPARAMS);
 #else
 tolua_cclass(tolua_S,"SPLINEMARKERPARAMS","SPLINEMARKERPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"SPLINEMARKERPARAMS");
  tolua_function(tolua_S,"new",tolua_SplineScript_SPLINEMARKERPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_SplineScript_SPLINEMARKERPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_SplineScript_SPLINEMARKERPARAMS_new00_local);
  tolua_variable(tolua_S,"bEndOfSpline",tolua_get_SPLINEMARKERPARAMS_bEndOfSpline,tolua_set_SPLINEMARKERPARAMS_bEndOfSpline);
  tolua_variable(tolua_S,"Distance",tolua_get_SPLINEMARKERPARAMS_Distance,tolua_set_SPLINEMARKERPARAMS_Distance);
  tolua_variable(tolua_S,"Position",tolua_get_SPLINEMARKERPARAMS_Position_ptr,tolua_set_SPLINEMARKERPARAMS_Position_ptr);
  tolua_variable(tolua_S,"FirstDerivative",tolua_get_SPLINEMARKERPARAMS_FirstDerivative_ptr,tolua_set_SPLINEMARKERPARAMS_FirstDerivative_ptr);
  tolua_variable(tolua_S,"SecondDerivative",tolua_get_SPLINEMARKERPARAMS_SecondDerivative_ptr,tolua_set_SPLINEMARKERPARAMS_SecondDerivative_ptr);
  tolua_function(tolua_S,"delete",tolua_SplineScript_SPLINEMARKERPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_SplineScript_SPLINEMARKERPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"SPLINEGETNEARESTPARAMS","SPLINEGETNEARESTPARAMS","",tolua_collect_SPLINEGETNEARESTPARAMS);
 #else
 tolua_cclass(tolua_S,"SPLINEGETNEARESTPARAMS","SPLINEGETNEARESTPARAMS","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"SPLINEGETNEARESTPARAMS");
  tolua_function(tolua_S,"new",tolua_SplineScript_SPLINEGETNEARESTPARAMS_new00);
  tolua_function(tolua_S,"new_local",tolua_SplineScript_SPLINEGETNEARESTPARAMS_new00_local);
  tolua_function(tolua_S,".call",tolua_SplineScript_SPLINEGETNEARESTPARAMS_new00_local);
  tolua_variable(tolua_S,"Distance",tolua_get_SPLINEGETNEARESTPARAMS_Distance,tolua_set_SPLINEGETNEARESTPARAMS_Distance);
  tolua_variable(tolua_S,"MinDistance",tolua_get_SPLINEGETNEARESTPARAMS_MinDistance,tolua_set_SPLINEGETNEARESTPARAMS_MinDistance);
  tolua_variable(tolua_S,"Position",tolua_get_SPLINEGETNEARESTPARAMS_Position_ptr,tolua_set_SPLINEGETNEARESTPARAMS_Position_ptr);
  tolua_function(tolua_S,"delete",tolua_SplineScript_SPLINEGETNEARESTPARAMS_delete00);
  tolua_function(tolua_S,"SIZEOF",tolua_SplineScript_SPLINEGETNEARESTPARAMS_SIZEOF00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_SplineScript (lua_State* tolua_S) {
 return tolua_SplineScript_open(tolua_S);
};
#endif

