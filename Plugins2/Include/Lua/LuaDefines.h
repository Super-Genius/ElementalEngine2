///============================================================================
/// \file		LuaDefines.h
/// \brief		Header file for Lua Messages
/// \date		07-25-2005
/// \author		Josh Lulewicz
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

#ifndef LUADEFINES_H
#define LUADEFINES_H

namespace ElementalEngine
{

// tolua_begin
typedef struct SCRIPTPARAMS
{
	IHashString *scriptObjName;  // Name of the script as given by sxl or wxl or whatever.
	IHashString *fileName;		// script file name
	char *scriptData;			// body of the script
	int dataSize;				// size of body
	bool getScriptSource;		/// get the script source if available

	SCRIPTPARAMS()
	{
		scriptObjName = NULL;
		fileName = NULL;
		scriptData = NULL;
		dataSize = 0;
		getScriptSource = false;
	}
} SCRIPTPARAMS;

struct SETTABLEVALUEPARAMS
{
	IHashString *TableName;
	IHashString *TableKey;
	IHashString *TableValue;
	IHashString *TableType;
	
	SETTABLEVALUEPARAMS()
	{
		TableName = NULL;
		TableKey = NULL;
		TableValue = NULL;
		TableType = NULL;
	}
};
// tolua_end

} //end namespace

#endif // #ifndef LUADEFINES_H