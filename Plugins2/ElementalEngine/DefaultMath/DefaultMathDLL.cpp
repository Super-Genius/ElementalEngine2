///============================================================================
/// \file	DefaultMathDLL.cpp
/// \brief	Defines the entry point for the DLL application.
/// \note   Elemental Engine
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

#include "stdafx.h"

#ifdef _DEBUG
#define INTERFACEDLLVERSION 0x00006100
#else
#define INTERFACEDLLVERSION 0x00006101
#endif

#define MATH_PRIORITY 1000

namespace ElementalEngine
{
extern int InitializeMathImplementations();
}

#ifdef _LIB
#define DLL_API static
#else
#define DLL_API extern "C" __declspec(dllexport)


BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
//	static HMODULE hElementalEngine = NULL;
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
//			hElementalEngine = LoadElementalEngine();

			CDefaultVec3Implementation::Instance();
			CDefaultVec4Implementation::Instance();
			CDefaultQuaternionImplementation::Instance();
			CDefaultMatrix3x3Implementation::Instance();
			CDefaultMatrix4x4Implementation::Instance();
			CDefaultMathUtilImplementation::Instance();
			CDefaultPlaneImplementation::Instance();
			CDefaultRandomImplementation::Instance();
			CDefaultAABBImplementation::Instance();
			CDefaultRayImplementation::Instance();
			CDefaultEulerImplementation::Instance();
			break;
		case DLL_PROCESS_DETACH:
/*			if (hElementalEngine)
			{
				FreeLibrary( hElementalEngine );
				hElementalEngine = NULL;
			}
*/
			CDefaultVec3Implementation::Destroy();
			CDefaultVec4Implementation::Destroy();
			CDefaultQuaternionImplementation::Destroy();
			CDefaultMatrix3x3Implementation::Destroy();
			CDefaultMatrix4x4Implementation::Destroy();
			CDefaultMathUtilImplementation::Destroy();
			CDefaultPlaneImplementation::Destroy();
			CDefaultRandomImplementation::Destroy();
			CDefaultAABBImplementation::Destroy();
			CDefaultRayImplementation::Destroy();			
			CDefaultEulerImplementation::Destroy();
			break;
	}

    return TRUE;
}
#endif

DLL_API void InitDLL()
{
	static MATHIMPLEMENTATIONMAP mapImplementation;
	mapImplementation[CHashString(_T("Vec3DefaultImplementation")).GetUniqueID()] = CDefaultVec3Implementation::Instance();
	mapImplementation[CHashString(_T("Vec4DefaultImplementation")).GetUniqueID()] = CDefaultVec4Implementation::Instance();
	mapImplementation[CHashString(_T("QuaternionDefaultImplementation")).GetUniqueID()] = CDefaultQuaternionImplementation::Instance();
	mapImplementation[CHashString(_T("Matrix3x3DefaultImplementation")).GetUniqueID()] = CDefaultMatrix3x3Implementation::Instance();
	mapImplementation[CHashString(_T("Matrix4x4DefaultImplementation")).GetUniqueID()] = CDefaultMatrix4x4Implementation::Instance();
	mapImplementation[CHashString(_T("MathUtilDefaultImplementation")).GetUniqueID()] = CDefaultMathUtilImplementation::Instance();
	mapImplementation[CHashString(_T("PlaneDefaultImplementation")).GetUniqueID()] = CDefaultPlaneImplementation::Instance();
	mapImplementation[CHashString(_T("RandomDefaultImplementation")).GetUniqueID()] = CDefaultRandomImplementation::Instance();
	mapImplementation[CHashString(_T("AABBDefaultImplementation")).GetUniqueID()] = CDefaultAABBImplementation::Instance();
	mapImplementation[CHashString(_T("RayDefaultImplementation")).GetUniqueID()] = CDefaultRayImplementation::Instance();
	mapImplementation[CHashString(_T("EulerDefaultImplementation")).GetUniqueID()] = CDefaultEulerImplementation::Instance();

	REGISTERMATHIMPLEMENTATION msg;
	msg.iPriority = MATH_PRIORITY;
	msg.dwLibraryName = CHashString(_T("DefaultMath")).GetUniqueID();
	msg.pImplementationMap = &mapImplementation;
	static DWORD msgHash_RegisterMathImplementation = CHashString(_T("RegisterMathImplementation")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_RegisterMathImplementation, sizeof(msg), &msg );

	// Reinitialize the local implementation pointers for this module.
	InitializeMathImplementations();
}

DLL_API DWORD GetDLLVersion()
{
	return INTERFACEDLLVERSION;
}

DLL_API DWORD GetPriority()
{
	return MATH_PRIORITY;
}

#ifdef _LIB
#include ".\StaticLibSymbols.h"
INITDLLINFO DEFAULTMATH_INITDLLINFO = {
	InitDLL, 
	GetDLLVersion, 
	GetPriority, 
	NULL,
	DEFAULTMATH_LIBEXTERNS
};
#endif
