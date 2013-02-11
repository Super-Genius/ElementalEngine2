///============================================================================
/// \file		CSoundObject.cpp
/// \brief		Implementation of generic sound object
/// \date		03/20/2007
/// \author		Nick Tourte
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

REGISTER_MESSAGE_HANDLER(PlaySoundObject, OnPlaySoundObject, CSoundObject);
REGISTER_MESSAGE_HANDLER(StopSoundObject, OnStopSoundObject, CSoundObject);

CSoundObject::CSoundObject(IHashString *parentName, IHashString *name, bool bAddToHier) : 
	SCENEOBJECTTEMPLATE(CSoundManager, CSoundObject, ISoundObject, parentName, name)
{
}

CSoundObject::~CSoundObject()
{}

IComponent* CSoundObject::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool bAddToHier;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new CSoundObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CSoundObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}


	return self;
}

DWORD CSoundObject::OnPlaySoundObject(DWORD size, void *param)
{
	EngineGetToolBox()->Log(LOGWARNING, _T("Play sound object in csoundobject"));
	return MSG_NOT_HANDLED;
}	

DWORD CSoundObject::OnStopSoundObject(DWORD size, void *param)
{
	EngineGetToolBox()->Log(LOGWARNING, _T("Stop sound object in csoundobject"));
	return MSG_NOT_HANDLED;
}