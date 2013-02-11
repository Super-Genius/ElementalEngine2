///============================================================================
/// \file		EntityQueryDefs.h
/// \brief		Header file for Entity Query
/// \date		12-20-2006
/// \author		Patrick Ghiocel
///         Copyright (c)  2005-2008 Signature Devices, Inc.
///
///         This code is redistributable under the terms of the EE License.
///
///         This code is distributed without warranty or implied warranty of
///			merchantability or fitness for a particular purpose. See the 
///			EE License for more details.
///
///         You should have received a copy of the EE License along with this
///			code; If not, write to Signature Devices, Inc.,
///			3200 Bridge Parkway Suite 102, Redwood City, CA 94086 USA.
///============================================================================

#ifndef _ENTITYQUERYDEFINE_H_
#define _ENTITYQUERYDEFINE_H_

class CLuaObjDistQueryList;
class CLuaObjectTypeQueryList;

// tolua_begin

struct OBJECTDISTANCEQUERY
{
	IHashString * name;
	float distance;
};

typedef list<OBJECTDISTANCEQUERY> ODQLIST;
typedef list<IHashString*> OBJECTTYPEQUERYLIST;

struct GETOBJECTQUERYPARAMS 
{
	Vec3 vPosition;
	float fRadius;
	IHashString *hszType;
	ODQLIST *pObjectsList;

	GETOBJECTQUERYPARAMS()
	{
		hszType = NULL;
	}
};

struct GETOBJECTTYPEQUERYPARAMS 
{
	IHashString *type;
	OBJECTTYPEQUERYLIST *pObjectsList;
};

// tolua_end

#endif //_ENTITYQUERYDEFINE_H_