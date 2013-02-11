---------------------------------------------------------------------------------
-- utl_physics.lua
-- place for physics messages
---------------------------------------------------------------------------------

local m_ToolBox = CLuaToolBox:GetLuaToolBox()
m_ToolBox:LoadLuaDependency("scripts\\utl_math.lua")

-- some known types
local hsCEntityType				= CLuaHashString:new_local( "CEntity" )
local hsCPhysicsType			= CLuaHashString:new_local( "CPhysicsObject" )
local hsCCharacterPhysicsType	= CLuaHashString:new_local( "CEECharacterPhysicsObject" )

-- global message names
local msgSetCollisionGroup		= CLuaHashString:new_local( "SetActorCollisionGroup" )
local msgSetKinematic			= CLuaHashString:new_local( "SetKinematic" )
local msgSetDynamic				= CLuaHashString:new_local( "SetDynamic" )
local msgSetMaxVelocity			= CLuaHashString:new_local( "SetMaxVelocity" )
local msgSetLinearVelocity		= CLuaHashString:new_local( "SetLinearVelocity" )
local msgGetLinearVelocity		= CLuaHashString:new_local( "GetLinearVelocity" )
local msgApplyForce				= CLuaHashString:new_local( "ApplyForce" )
local msgApplyTorque			= CLuaHashString:new_local( "ApplyTorque" )
local msgGetAngularVelocity		= CLuaHashString:new_local( "GetAngularVelocity" )
local msgSetAngularVelocity		= CLuaHashString:new_local( "SetAngularVelocity" )
local msgSetLinearDamping		= CLuaHashString:new_local( "SetLinearDamping" )
local msgSetCollisionSurrogate = CLuaHashString:new_local("SetCollisionSurrogate")


---------------------------------------------------------------------------------
-- SetCollisionGroup
---------------------------------------------------------------------------------
function SetCollisionGroup(hsObjectName, group, hsPhysicsType)
	m_ToolBox:SendMessage( msgSetCollisionGroup, TODWORD(4), TODWORD(group), hsObjectName, hsPhysicsType )	
end

---------------------------------------------------------------------------------
-- SetKinematic
---------------------------------------------------------------------------------
function SetKinematic(hsObjectname, hsPhysicsType)
	m_ToolBox:SendMessage( msgSetKinematic, TODWORD(0), TODWORD(0), hsObjectname, hsPhysicsType )
end

---------------------------------------------------------------------------------
-- SetDynamic
---------------------------------------------------------------------------------
function SetDynamic(hsObjectname, hsPhysicsType)
	m_ToolBox:SendMessage( msgSetDynamic, TODWORD(0), TODWORD(0), hsObjectname, hsPhysicsType )
end

---------------------------------------------------------------------------------
-- SetMaxVelocity
---------------------------------------------------------------------------------
function SetMaxVelocity(hsObjectName, maxVelocity)
	m_ToolBox:SendMessage( msgSetMaxVelocity, FLOAT_SIZEOF(), TOFLOAT(maxVelocity), hsObjectName, hsCCharacterPhysicsType)
end

---------------------------------------------------------------------------------
-- SetLinearVelocity
---------------------------------------------------------------------------------
function SetLinearVelocity(hsObjectName, vVelocity, hsPhysicsType)
	local forceVector = CLuaVec3:new_local( vVelocity.x, vVelocity.y, vVelocity.z )
	m_ToolBox:SendMessage( msgSetLinearVelocity, forceVector:SIZEOF(), forceVector, hsObjectName, hsPhysicsType)
end

---------------------------------------------------------------------------------
-- GetLinearVelocity
---------------------------------------------------------------------------------
function GetLinearVelocity(hsObjectName, hsPhysicsType)
	local velocity = CLuaVec3:new_local()
	m_ToolBox:SendMessage( msgGetLinearVelocity, velocity:SIZEOF(), velocity, hsObjectName, hsPhysicsType)
	return LuaMathLib:Vec3( velocity )
end

---------------------------------------------------------------------------------
-- ApplyForce
---------------------------------------------------------------------------------
function ApplyForce(hsObjectName, vForce, hsPhysicsType)
	local force = CLuaVec3:new_local( vForce.x, vForce.y, vForce.z )
	m_ToolBox:SendMessage(msgApplyForce, force:SIZEOF(), force, hsObjectName, hsPhysicsType)
end

---------------------------------------------------------------------------------
-- ApplyTorque
---------------------------------------------------------------------------------
function ApplyTorque(hsObjectName, vTorque, hsPhysicsType)
	local torque = CLuaVec3:new_local( vTorque.x, vTorque.y, vTorque.z )
	m_ToolBox:SendMessage(msgApplyTorque, torque:SIZEOF(), torque, hsObjectName, hsPhysicsType)
end

---------------------------------------------------------------------------------
-- GetAngularVelocity
---------------------------------------------------------------------------------
function GetAngularVelocity(hsObjectName, hsPhysicsType)
	local vVelocity = CLuaVec3:new_local()
	m_ToolBox:SendMessage(msgGetAngularVelocity, vVelocity:SIZEOF(), vVelocity, hsObjectName, hsPhysicsType)
	return LuaMathLib:Vec3( vVelocity )
end

---------------------------------------------------------------------------------
-- SetAngularVelocity
---------------------------------------------------------------------------------
function SetAngularVelocity(hsObjectName, vVelocity, hsPhysicsType)
	local velocity = CLuaVec3:new_local( vVelocity.x, vVelocity.y, vVelocity.z )
	m_ToolBox:SendMessage(msgSetAngularVelocity, velocity:SIZEOF(), velocity, hsObjectName, hsPhysicsType)
end

---------------------------------------------------------------------------------
-- SetDamping
---------------------------------------------------------------------------------
function SetDamping(hsObjectName, fDamping, hsPhysicsType)
	m_ToolBox:SendMessage( msgSetLinearDamping, TODWORD(4), TODWORD(fDamping), hsObjectName, hsPhysicsType)
end

---------------------------------------------------------------------------------
-- SetDamping
---------------------------------------------------------------------------------
function SetCollisionSurrogate(hsObjectName, hsSurrogate)
	m_ToolBox:SendMessage( msgSetCollisionSurrogate, TODWORD(4), hsSurrogate, hsObjectName, hsCPhysicsType)
end
