---------------------------------------------------------------------------------
---------------------------------------------------------------------------------
-- GENERALIZED PROJECTILE FUNCTIONS
---------------------------------------------------------------------------------
---------------------------------------------------------------------------------

-- Get the Toolbox
local m_ToolBox = CLuaToolBox:GetLuaToolBox()

m_ToolBox:LoadLuaDependency("scripts\\utilityfunctions.lua")
m_ToolBox:LoadLuaDependency("scripts\\utl_math.lua")
m_ToolBox:LoadLuaDependency("scripts\\utl_physics.lua")
m_ToolBox:LoadLuaDependency("scripts\\utl_entity.lua")

local mathlib = CLuaMath:new_local()

-- global message names
msgSetGlobalPosition		= CLuaHashString:new_local("SetGlobalPosition")
msgSetGlobalEuler			= CLuaHashString:new_local("SetGlobalEuler")
msgSetParticleLineSpeed		= CLuaHashString:new_local("SetParticleLineSpeed")

---------------------------------------------------------------------------------
-- FireDynamicProjectile
---------------------------------------------------------------------------------
function FireDynamicProjectile( position, direction, velocity, spawner )
	--OutputDebug( "Starting FireDynamicProjectile\n" )

	-- Load the bullet object
	LoadFileByExt( spawner, true )
	
	-- Get a reference to the newly loaded bullet
	local projectileEntity = CLuaHashString:new_local()	
	projectileEntity = GetSpawnedName()

	-- Set the projectile's initial position and rotation
	SetGlobalPosition( projectileEntity, position )
	SetGlobalEuler( projectileEntity, direction )

	-- Declare the vector to fire the bullets along
	local forceVector = LuaMathLib:EulerToVector( direction )

	-- Apply a large force to the projectile along the orientation vector
	LuaMathLib:Vec3TimesEquals( forceVector, velocity )
	SetLinearVelocity( projectileEntity, forceVector )
end

---------------------------------------------------------------------------------
-- AttachMuzzleFlash()
---------------------------------------------------------------------------------
function AttachMuzzleFlash( hszParent, szMuzzleFlash, vPos, eRot )
--	OutputDebug(2, "AttachMuzzleFlash:"..szMuzzleFlash.."@"..hszParent:GetString().."\n")
	LoadSpawnFile(szMuzzleFlash, hszParent)
	hszTemp = GetSpawnedName()
	if vPos ~= nil then
		SetGlobalPosition( hszTemp, vPos )
	end
	if eRot ~= nil then
		SetGlobalEuler( hszTemp, eRot )
	end
end


---------------------------------------------------------------------------------
-- CalcFiringSolution
---------------------------------------------------------------------------------
function CalcFiringSolution( v3OriginPos, v3TargetPos, fMuzzleVelocity )
	-- Code generated from formulas by Ian Millington, Artificial Intelligence for Games(page 130)
	-- ISBN-13: 978-0-12-497782

	local v3Gravity = LuaMathLib:Vec3(0.0, 0.0, -9.8)
	local fTimeToImpact = 0.0
	local v3Direction = LuaMathLib:Vec3()
	local v3Temp1 = LuaMathLib:Vec3()
	local v3Temp2 = LuaMathLib:Vec3()
	local v3Temp3 = LuaMathLib:Vec3()
	local fTemp	 = 0.0
	local fValue1 = 0.0


	-- Build Direction Vector
	v3Direction:Set(v3TargetPos)
	LuaMathLib:Vec3SubtractEquals(v3Direction, v3OriginPos)
	
	-- Check if there is a firing solution
	v3Temp1:Set(v3Gravity)
	fTemp = LuaMathLib:Vec3Dot(v3Temp1, v3Direction)
	if ( fTemp < (LuaMathLib:Vec3LengthSQ(v3Gravity)*LuaMathLib:Vec3LengthSQ(v3Direction))) then
		return nil
	end

	-- This value is used several times
	fValue1 = LuaMathLib:Vec3Dot(v3Gravity, v3Direction) + fMuzzleVelocity*fMuzzleVelocity

	-- Calculate time to impact.
	fTimeToImpact = 2 * math:sqrt( (fValue1 + math:sqrt(fValue1*fValue1 - LuaMathLib:Vec3LengthSQ(v3Gravity)*LuaMathLib:Vec3LengthSQ(v3Direction)))/ (2 * LuaMathLib:Vec3LengthSQ(v3Gravity)) )
	
	-- calculate firing solution:
	v3Temp1:Set(v3Direction)
	LuaMathLib:Vec3TimesEquals(v3Temp1, 2.0)
	v3Temp2:Set(v3Gravity)
	LuaMathLib:Vec3TimesEquals(v3Temp2, fTimeToImpact*fTimeToImpact)
	v3Temp3:Set(v3Temp1)
	LuaMathLib:Vec3SubractEquals(v3Temp3, v3Temp2)
	LuaMathLib:Vec3DivideEquals(v3Temp3, 2.0 * fMuzzleVelocity * fTimeToImpact)
	return v3Temp3
end

---------------------------------------------------------------------------------
-- FireParticleLine
--
-- Unlike FireDynamicProjectile there are no physics involved with
-- FireParticleLine. This is purely for effect although it can be mixed with a
-- seperate physics object as seen in the multi-shot code.
---------------------------------------------------------------------------------
function FireParticleLine( position, direction, velocity, spawner )
	--OutputDebug( "Starting FireParticleLine.\n" )

	-- Load the bullet object
	LoadFileByExt( spawner, true )
	
	-- Get a reference to the newly loaded bullet
	local projectileEntity = CLuaHashString:new_local()	
	projectileEntity = GetSpawnedName()

	-- Set the projectile's initial position and rotation
	m_ToolBox:SendMessage( msgSetGlobalPosition, position:SIZEOF(), position, projectileEntity )
	m_ToolBox:SendMessage( msgSetGlobalEuler, direction:SIZEOF(), direction, projectileEntity )

	-- Apply a large force to the projectile along the orientation vector
	SetParticleLineVelocity( projectileEntity, velocity )
	
end

---------------------------------------------------------------------------------
-- SetParticleLineVelocity
---------------------------------------------------------------------------------
function SetParticleLineVelocity( hsObjectName, velocity )
	m_ToolBox:SendMessage( msgSetParticleLineSpeed, FLOAT_SIZEOF(), TOFLOAT(velocity), hsObjectName )
end