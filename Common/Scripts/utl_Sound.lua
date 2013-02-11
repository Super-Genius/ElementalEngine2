---------------------------------------------------------------------------------
---------------------------------------------------------------------------------
-- GENERALIZED SOUND AND MUSIC FUNCTIONS
---------------------------------------------------------------------------------

-- Get the Toolbox
local m_ToolBox = CLuaToolBox:GetLuaToolBox()

m_ToolBox:LoadLuaDependency("scripts\\utilityfunctions.lua")

-- global message names
msgPlaySound							= CLuaHashString:new_local("PlaySound")
msgStopSound							= CLuaHashString:new_local("StopSound")
msgStopSoundObject						= CLuaHashString:new_local("StopSoundObject") 
msgSetListenerVectors					= CLuaHashString:new_local("SetListenerVectors")
msgRunPlaylist							= CLuaHashString:new_local("RunPlaylist")
msgStopPlaylist							= CLuaHashString:new_local("StopPlaylist")
msgSetGlobalMusicVolume					= CLuaHashString:new_local("SetGlobalMusicVolume")
msgSetGlobalSoundVolume					= CLuaHashString:new_local("SetGlobalSoundVolume")
msgMuteSounds							= CLuaHashString:new_local("MuteSounds")
msgUnMuteSounds							= CLuaHashString:new_local("UnMuteSounds")
msgMuteMusic							= CLuaHashString:new_local("MuteMusic")
msgUnMuteMusic							= CLuaHashString:new_local("UnMuteMusic")
msgSetAudioBufferLength					= CLuaHashString:new_local("SetAudioBufferLength")
msgSetAudioBufferCount					= CLuaHashString:new_local("SetAudioBufferCount")
msgSetSoundScale						= CLuaHashString:new_local("SetSoundScale")
msgSetGlobalAttenuationMultiplier		= CLuaHashString:new_local("SetGlobalAttenuationMultiplier")
msgSetGlobalDopplerFactor				= CLuaHashString:new_local("SetGlobalDopplerFactor")

---------------------------------------------------------------------------------
-- Plays a global (not in hierarchy) 2D sound
-- szSoundFile = lua string of sound file
-- fVolume = volume (1.0 being max vol)
-- bLooping = loop or not
-- hszObjectName = does not need to be specified if sound is not looping, 
-- but if it is looping then it does (to stop the sound)
---------------------------------------------------------------------------------
function PlayGlobal2DSound(szSoundFile, bLooping, hszObjectName, fVolume, fPitch)
	local psp = PLAYSOUNDPARAMS:new_local()
	local hszFileName = CLuaHashString:new_local(szSoundFile)
	psp.fileName = hszFileName
	
	if bLooping == nil then
		psp.looping = false
	else
		psp.looping = bLooping
	end
	
	psp.autoRemove = true
	psp.is3DSound = false
	
	if fVolume == nil then
		psp.volume = -1
	else
		psp.volume = fVolume
	end
	
	if hszObjectName == nil then
		psp.objectName = nil
	else
		psp.objectName = hszObjectName
	end
	
	if fPitch ~= nil then
		psp.pitch = fPitch
	end
	
	psp.bInHierarchy = false
	psp.hsParentName = nil
	
	m_ToolBox:SendMessage(msgPlaySound, psp:SIZEOF(), psp)
end

---------------------------------------------------------------------------------
-- Plays a global (not in hierarchy) 2D sound
-- szSoundFile = lua string of sound file
-- fVolume = volume (1.0 being max vol)
-- vPosition = position of sound
-- bLooping = loop or not
-- hszObjectName = does not need to be specified if sound is not looping, 
-- but if it is looping then it does (to stop the sound)
---------------------------------------------------------------------------------
function PlayGlobal3DSound(szSoundFile, vPosition, bLooping, hszObjectName, fVolume, fPitch)
	local psp = PLAYSOUNDPARAMS:new_local()
	local hszFileName = CLuaHashString:new_local(szSoundFile)
	psp.fileName = hszFileName
	
	if bLooping == nil then
		psp.looping = false
	else
		psp.looping = bLooping
	end
	
	psp.autoRemove = true
	psp.is3DSound = true	
	psp.soundPosition = CLuaVec3:new_local( vPosition.x, vPosition.y, vPosition.z )
	
	if fVolume == nil then
		psp.volume = -1
	else
		psp.volume = fVolume
	end
	if hszObjectName == nil then
		psp.objectName = nil
	else
		psp.objectName = hszObjectName
	end
	
	psp.bInHierarchy = false
	psp.hsParentName = nil
	
	if fPitch ~= nil then
		psp.pitch = fPitch
	end
	
	m_ToolBox:SendMessage(msgPlaySound, psp:SIZEOF(), psp)
end

---------------------------------------------------------------------------------
-- Play2DSoundInHierarchy
---------------------------------------------------------------------------------
function Play2DSoundInHierarchy(szSoundFile, bLooping, hszObjectName, fVolume, hsParentName, fPitch)
	local psp = PLAYSOUNDPARAMS:new_local()
	local hszFileName = CLuaHashString:new_local(szSoundFile)
	psp.fileName = hszFileName
	
	if bLooping == nil then
		psp.looping = false
	else
		psp.looping = bLooping
	end
	
	psp.autoRemove = true
	psp.is3DSound = false
	
	if fVolume == nil then
		psp.volume = -1
	else
		psp.volume = fVolume
	end
	
	if hszObjectName == nil then
		psp.objectName = nil
	else
		psp.objectName = hszObjectName
	end
	
	if fPitch ~= nil then
		psp.pitch = fPitch
	end
	
	psp.bInHierarchy = true
	psp.hsParentName = hsParentName
	
	m_ToolBox:SendMessage(msgPlaySound, psp:SIZEOF(), psp)
end

---------------------------------------------------------------------------------
-- Play3DSoundInHierarchy
---------------------------------------------------------------------------------
function Play3DSoundInHierarchy(szSoundFile, vPosition, bLooping, hszObjectName, fVolume, hsParentName, fPitch)
	local psp = PLAYSOUNDPARAMS:new_local()
	local hszFileName = CLuaHashString:new_local(szSoundFile)
	psp.fileName = hszFileName
	
	if bLooping == nil then
		psp.looping = false
	else
		psp.looping = bLooping
	end
	
	psp.autoRemove = true
	psp.is3DSound = true	
	psp.soundPosition = CLuaVec3:new_local( vPosition.x, vPosition.y, vPosition.z )
	
	if fVolume == nil then
		psp.volume = -1
	else
		psp.volume = fVolume
	end
	if hszObjectName == nil then
		psp.objectName = nil
	else
		psp.objectName = hszObjectName
	end
	
	psp.bInHierarchy = true
	psp.hsParentName = hsParentName
	
	if fPitch ~= nil then
		psp.pitch = fPitch
	end
	
	m_ToolBox:SendMessage(msgPlaySound, psp:SIZEOF(), psp)
end

---------------------------------------------------------------------------------
-- Stops a global sound that may be looping (or not)
-- hszObjectName = name of object (retrieved when you started the sound)
---------------------------------------------------------------------------------
function StopGlobalSound(hszObjectName, fFadeVal)
	local ssp = STOPSOUNDPARAMS:new_local()
	ssp.soundObjectName = hszObjectName
	
	if fFadeVal ~= nil then
		ssp.fadeOutVal = fFadeVal
	end	
	
	m_ToolBox:SendMessage(msgStopSound, ssp:SIZEOF(), ssp)
end

---------------------------------------------------------------------------------
-- StopSoundObject
---------------------------------------------------------------------------------
function StopSoundObject(hsObjectName)
	m_ToolBox:SendMessage(msgStopSoundObject, TODWORD(0), nil, hsObjectName)
end

---------------------------------------------------------------------------------
-- Sets the al listener data
-- vPosition = vec3 position of listener
-- vOrientation = vec3 orientation of listener (euler)
-- bIsCamera = is the listener a camera (+x forward) or not (-y forward)
---------------------------------------------------------------------------------
function SetListenerPositionOrientation(vPosition, vOrientation, bUpdateListenerVelocity)
	local lvp = LISTENERVECTORPARAMS:new_local()
	lvp.position = CLuaVec3:new_local( vPosition.x, vPosition.y, vPosition.z )
	lvp.orientation = tolua.cast(vOrientation, "EulerAngle")
	if bUpdateListenerVelocity ~= nil then
		lvp.bUpdateVelocity = bUpdateListenerVelocity
	end
	
	m_ToolBox:SendMessage(msgSetListenerVectors, lvp:SIZEOF(), lvp)
end

---------------------------------------------------------------------------------
-- PlayMusic
-- music -> lua string of xml playlist file (.xmp)
---------------------------------------------------------------------------------
function PlayMusic( music )
	LoadFileByExt(music, true)
	
	local hszName = CLuaHashString:new_local(music)
	ToolBox:SendMessage(msgRunPlaylist, TODWORD(4), hszName)
end

---------------------------------------------------------------------------------
-- StopPlaylist
---------------------------------------------------------------------------------
function StopPlaylist()
	ToolBox:SendMessage(msgStopPlaylist, TODWORD(0), nil)
end

---------------------------------------------------------------------------------
-- SetGlobalMusicVolume
---------------------------------------------------------------------------------
function SetGlobalMusicVolume(fVolume)
	local volume = TOFLOAT(fVolume)
	ToolBox:SendMessage(msgSetGlobalMusicVolume, FLOAT_SIZEOF(), volume)
end

---------------------------------------------------------------------------------
-- SetGlobalSoundVolume
---------------------------------------------------------------------------------
function SetGlobalSoundVolume(fVolume)
	local volume = TOFLOAT(fVolume)
	ToolBox:SendMessage(msgSetGlobalSoundVolume, FLOAT_SIZEOF(), volume)
end

---------------------------------------------------------------------------------
-- MuteSounds
---------------------------------------------------------------------------------
function MuteSounds()
	ToolBox:SendMessage(msgMuteSounds, TODWORD(0), nil)
end

---------------------------------------------------------------------------------
-- UnMuteSounds
---------------------------------------------------------------------------------
function UnMuteSounds()
	ToolBox:SendMessage(msgUnMuteSounds, TODWORD(0), nil)
end

---------------------------------------------------------------------------------
-- MuteMusic
---------------------------------------------------------------------------------
function MuteMusic()
	ToolBox:SendMessage(msgMuteMusic, TODWORD(0), nil)
end

---------------------------------------------------------------------------------
-- UnMuteMusic
---------------------------------------------------------------------------------
function UnMuteMusic()
	ToolBox:SendMessage(msgUnMuteMusic, TODWORD(0), nil)
end

---------------------------------------------------------------------------------
-- SetAudioBufferLength
---------------------------------------------------------------------------------
function SetAudioBufferLength(fDuration)
	ToolBox:SendMessage(msgSetAudioBufferLength, FLOAT_SIZEOF(), TOFLOAT(fDuration))
end

---------------------------------------------------------------------------------
-- SetAudioBufferCount
---------------------------------------------------------------------------------
function SetAudioBufferCount(iCount)
	ToolBox:SendMessage(msgSetAudioBufferCount, TODWORD(4), TODWORD(iCount))
end

---------------------------------------------------------------------------------
-- SetSoundScale
---------------------------------------------------------------------------------
function SetSoundScale(fScale)
	local floatScale = TOFLOAT(fScale)
	ToolBox:SendMessage(msgSetSoundScale, FLOAT_SIZEOF(), floatScale)
end

---------------------------------------------------------------------------------
-- SetGlobalAttenuationMultiplier
---------------------------------------------------------------------------------
function SetGlobalAttenuationMultiplier(fAtten)
	local floatAtten = TOFLOAT(fAtten)
	ToolBox:SendMessage(msgSetGlobalAttenuationMultiplier, FLOAT_SIZEOF(), floatAtten)
end

---------------------------------------------------------------------------------
-- SetGlobalDopplerFactor
---------------------------------------------------------------------------------
function SetGlobalDopplerFactor(fDoppler)
	local floatDoppler = TOFLOAT(fDoppler)
	ToolBox:SendMessage(msgSetGlobalDopplerFactor, FLOAT_SIZEOF(), floatDoppler)
end