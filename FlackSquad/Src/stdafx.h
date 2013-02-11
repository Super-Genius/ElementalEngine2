// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef FLACKSQUAD_STDAFX_H
#define FLACKSQUAD_STDAFX_H

#include "PlatForm.h"
#include <assert.h>
#include "res\resource.h"

#include "CmdLine.h"

#include "ElementalEngine.hpp"
#include "ee-sdk.h"
#include "EEDefines.h"
#include "EELuaScript/EELuaScriptDefines.h" 
#include "Register.h"

#ifdef _SHELL
#include "GvrMultiGameAPI.h"
#include "GvrIO.h"
#include "GvrIOMessage.h"
#endif

#include "GameComponent.h"
#include "FlackSquad.h"
#include "Vec3.h"

#include "QHStateMachine/QHStateMachineDefines.h" // Needed for event triggers.
#include "DatabaseDefines.hpp"

#endif	//#ifndef FLACKSQUAD_STDAFX_H
