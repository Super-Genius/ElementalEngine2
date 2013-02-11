---------------------------------------------------------------------------------
-- utl_particle.lua
-- place for spacial message get/sets
---------------------------------------------------------------------------------

local m_ToolBox = CLuaToolBox:GetLuaToolBox()

-- some known types
hsCParticleEmitterObject		= CLuaHashString:new_local("CParticleEmitterObject")
hsCParticleRenderObject			= CLuaHashString:new_local("CParticleRenderObject")
hsCParticleSystemObject			= CLuaHashString:new_local("CParticleSystemObject")
hsCEntity						= CLuaHashString:new_local("CEntity")


-- global message names
msgSetGlobalPosition			= CLuaHashString:new_local("SetGlobalPosition")
msgSetGlobalEuler				= CLuaHashString:new_local("SetGlobalEuler")
msgSetGlobalScale				= CLuaHashString:new_local("SetGlobalScale")
msgSetGlobalTransform			= CLuaHashString:new_local("SetGlobalTransform")

msgSetLocalPosition				= CLuaHashString:new_local("SetLocalPosition")
msgSetLocalEuler				= CLuaHashString:new_local("SetLocalEuler")
msgSetLocalScale				= CLuaHashString:new_local("SetLocalScale")

---------------------------------------------------------------------------------
-- SetGlobalPosition
-- hsObjectName = name of object to set position of
-- vPosition = CLuaVec3 of position
---------------------------------------------------------------------------------
function SetGlobalParticlePosition(hsObjectName, vPos, hsTypeName)
	local vPosition = CLuaVec3:new_local( vPos.x, vPos.y, vPos.z )
	m_ToolBox:SendMessage(msgSetGlobalPosition, vPosition:SIZEOF(), vPosition, hsObjectName, hsTypeName)
end


---------------------------------------------------------------------------------
-- SetGlobalEuler
-- hsObjectName = name of object to set orientation of
-- vEuler = CLuaVec3 of euler rotation
---------------------------------------------------------------------------------
function SetGlobalParticleEuler(hsObjectName, eEul, hsTypeName)
	local eEuler = CLuaEulerAngle:new_local( eEul.roll, eEul.pitch, eEul.yaw )
	m_ToolBox:SendMessage(msgSetGlobalEuler, eEuler:SIZEOF(), eEuler, hsObjectName, hsTypeName)
end

---------------------------------------------------------------------------------
-- SetGlobalScale
-- hsObjectName = name of object to set scale of
-- vScale = CLuaVec3 of scale
---------------------------------------------------------------------------------
function SetGlobalParticleScale(hsObjectName, vS, hsTypeName)
	local vScale = CLuaVec3:new_local( vS.x, vS.y, vS.z )
	m_ToolBox:SendMessage(msgSetGlobalScale, vScale:SIZEOF(), vScale, hsObjectName, hsTypeName)
end

---------------------------------------------------------------------------------
-- SetGlobalTransform
---------------------------------------------------------------------------------
function SetGlobalParticleTransform(hsObjectName, transform4x4, hsTypeName)
	m_ToolBox:SendMessage(msgSetGlobalTransform, transform4x4:SIZEOF(), transform4x4, hsObjectName, hsTypeName)	
end

---------------------------------------------------------------------------------
-- SetLocalPosition
-- hsObjectName = name of object to set position of
-- vPosition = CLuaVec3 of position
---------------------------------------------------------------------------------
function SetLocalParticlePosition(hsObjectName, vPos, hsTypeName)
	local vPosition = CLuaVec3:new_local( vPos.x, vPos.y, vPos.z )
	m_ToolBox:SendMessage(msgSetLocalPosition, vPosition:SIZEOF(), vPosition, hsObjectName, hsTypeName)
end

---------------------------------------------------------------------------------
-- SetLocalEuler
-- hsObjectName = name of object to set orientation of
-- vEuler = CLuaVec3 of euler rotation
---------------------------------------------------------------------------------
function SetLocalParticleEuler(hsObjectName, eEul, hsTypeName)
	local eEuler = CLuaEulerAngle:new_local( eEul.roll, eEul.pitch, eEul.yaw )
	m_ToolBox:SendMessage(msgSetLocalEuler, vEuler:SIZEOF(), vEuler, hsObjectName, hsTypeName)
end

---------------------------------------------------------------------------------
-- SetLocalScale
-- hsObjectName = name of object to set scale of
-- vScale = CLuaVec3 of scale
---------------------------------------------------------------------------------
function SetLocalParticleScale(hsObjectName, vS, hsTypeName)
	local vScale = CLuaVec3:new_local( vS.x, vS.y, vS.z )
	m_ToolBox:SendMessage(msgSetLocalScale, vScale:SIZEOF(), vScale, hsObjectName, hsTypeName)
end

-------------------------------------------------------  
-- Enable
-------------------------------------------------------  
function Enable(hsObjectName, hsTypeName)
	ToolBox:SendMessage(msgEnable, TODWORD(0), nil, hsObjectName, hsTypeName)
end

-------------------------------------------------------  
-- Disable
-------------------------------------------------------  
function Disable(hsObjectName, hsTypeName)
	ToolBox:SendMessage(msgDisable, TODWORD(0), nil, hsObjectName, hsTypeName)
end
