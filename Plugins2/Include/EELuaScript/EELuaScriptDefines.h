///============================================================================
/// \file		EELuaScriptDefines.h
/// \brief		Structs for lua script parameters
/// \date		11-07-2006
/// \note		Copyright (c) 2006 Signature Devices, Inc.
///============================================================================

#ifndef __EELUASCRIPTDEFINES_H__
#define __EELUASCRIPTDEFINES_H__

namespace ElementalEngine
{

// tolua_begin
struct EXECUTESCRIPTSTRUCT
{
	TCHAR *fileName;				// script file name
	IHashString *scriptName;		// script name
	bool bAutoStart;				// auto start the script?
	bool bScriptSingleton;			// create a script singleton (load once)?
	EXECUTESCRIPTSTRUCT()
	{
		fileName = _T("");
		scriptName = NULL;
		bAutoStart = false;
		bScriptSingleton = false;
	}
};

struct SCRIPTPAUSEPARAMS
{
	IHashString *scriptObjectName;
	bool pause;
};

struct SCRIPTSTATS
{
	int iMemoryUsageKb; /// memory usage in kilobytes
};
// tolua_end

}	//namespace ElementalEngine

#endif // #ifndef __EELUASCRIPTDEFINES_H__