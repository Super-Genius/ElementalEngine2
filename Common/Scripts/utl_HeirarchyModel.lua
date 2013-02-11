---------------------------------------------------------------------------------
---------------------------------------------------------------------------------
-- GENERALIZED FUNCTIONS TO THE BEST OF MY ABILITY - Brendan
---------------------------------------------------------------------------------
---------------------------------------------------------------------------------

-- Get the Toolbox
local m_ToolBox = CLuaToolBox:GetLuaToolBox()

m_ToolBox:LoadLuaDependency("scripts\\utilityfunctions.lua")

-- global message names
msgGetChildID					= CLuaHashString:new_local("GetChildID")
msgGetChildRotationOffset		= CLuaHashString:new_local("GetChildRotationOffset")
msgGetChildGlobalRotation		= CLuaHashString:new_local("GetChildGlobalRotation")
msgGetChildPositionOffset		= CLuaHashString:new_local("GetChildPositionOffset")
msgGetChildGlobalPosition		= CLuaHashString:new_local("GetChildGlobalPosition")
msgSetChildRotationOffset		= CLuaHashString:new_local("SetChildRotationOffset")
msgSetChildGlobalRotation		= CLuaHashString:new_local("SetChildGlobalRotation")
msgSetChildPositionOffset		= CLuaHashString:new_local("SetChildPositionOffset")
msgSetChildGlobalPosition		= CLuaHashString:new_local("SetChildGlobalPosition")
msgPlayMatrixAnimation			= CLuaHashString:new_local("PlayMatrixAnimation")
msgGetChildGlobalMatrix			= CLuaHashString:new_local("GetChildGlobalMatrix")

---------------------------------------------------------------------------------
-- GetChildIDByName
---------------------------------------------------------------------------------
function GetChildIDByName( hszObjectName, ChildName)
	local hszChildName = CLuaHashString:new_local(ChildName)
	local ChildID = 0

	local lfep = HIERARCHICALMODELGETCHILDID:new_local()
	lfep.m_Childname = hszChildName
	m_ToolBox:SendMessage(msgGetChildID, lfep:SIZEOF(), lfep, hszObjectName)
	ChildID = lfep.m_RetID

--	OutputDebug("CHILD ID: "..ChildID.."\n")
	return ChildID
end

---------------------------------------------------------------------------------
-- GET DATA
---------------------------------------------------------------------------------
---------------------------------------------------------------------------------
-- CHILD OFFSET ROTATION
---------------------------------------------------------------------------------
-- GetChildOffsetRotationByName
---------------------------------------------------------------------------------
function GetChildOffsetRotationByName( hszObjectName, ChildName)
	local v3ChildOffsetRotation = CLuaEulerAngle:new_local()
	local ChildID = GetChildIDByName(hszObjectName, ChildName)

	v3ChildOffsetRotation = tolua.cast( GetChildOffsetRotationByID( hszObjectName, ChildID), "CLuaEulerAngle")
	
	return v3ChildOffsetRotation
end
---------------------------------------------------------------------------------
-- GetChildOffsetRotationByID
---------------------------------------------------------------------------------
function GetChildOffsetRotationByID( hszObjectName, ChildID)
	local v3ChildOffsetRotation = CLuaEulerAngle:new_local()
	
	local hosp = HIERARCHICALOFFSETPARAM:new_local()
	hosp.m_Child = ChildID
	hosp.m_Offset =  v3ChildOffsetRotation
	m_ToolBox:SendMessage(msgGetChildRotationOffset, hosp:SIZEOF(), hosp, hszObjectName)

	return v3ChildOffsetRotation
end
---------------------------------------------------------------------------------
-- CHILD GLOBAL ROTATION
---------------------------------------------------------------------------------
-- GetChildGlobalRotationByName
---------------------------------------------------------------------------------
function GetChildGlobalRotationByName( hszObjectName, ChildName)
	local v3ChildGobalRotation = CLuaEulerAngle:new_local()
	local ChildID = GetChildIDByName(hszObjectName, ChildName)

	v3ChildGobalRotation = tolua.cast( GetChildGlobalRotationByID( hszObjectName, ChildID), "CLuaEulerAngle")
	
	return v3ChildGobalRotation
end
---------------------------------------------------------------------------------
-- GetChildGlobalRotationByID
---------------------------------------------------------------------------------
function GetChildGlobalRotationByID( hszObjectName, ChildID)
	local v3ChildGobalRotation = CLuaEulerAngle:new_local()
	local hosp = HIERARCHICALOFFSETPARAM:new_local()
	hosp.m_Child = ChildID
	hosp.m_Offset = v3ChildGobalRotation
	m_ToolBox:SendMessage(msgGetChildGlobalRotation, hosp:SIZEOF(), hosp, hszObjectName)
	
	return v3ChildGobalRotation
end
---------------------------------------------------------------------------------
-- CHILD OFFSET POSITION
---------------------------------------------------------------------------------
-- GetChildOffsetPositionByName
---------------------------------------------------------------------------------
function GetChildOffsetPositionByName( hszObjectName, ChildName)
	local v3ChildOffsetPosition = CLuaVec3:new_local()
	local ChildID = GetChildIDByName(hszObjectName, ChildName)

	v3ChildOffsetPosition = tolua.cast( GetChildOffsetPositionByID( hszObjectName, ChildID), "CLuaVec3")
	
	return v3ChildOffsetPosition
end
---------------------------------------------------------------------------------
-- GetChildOffsetPositionByID
---------------------------------------------------------------------------------
function GetChildOffsetPositionByID( hszObjectName, ChildID)
	local v3ChildOffsetPosition = CLuaVec3:new_local()
	
	local hosp = HIERARCHICALOFFSETPARAM:new_local()
	hosp.m_Child = ChildID
	hosp.m_Offset =  v3ChildOffsetPosition
	m_ToolBox:SendMessage(msgGetChildPositionOffset, hosp:SIZEOF(), hosp, hszObjectName)

	return v3ChildOffsetPosition
end
---------------------------------------------------------------------------------
-- CHILD GLOBAL POSITION
---------------------------------------------------------------------------------
-- GetChildGlobalPositionByName
---------------------------------------------------------------------------------
function GetChildGlobalPositionByName( hszObjectName, ChildName)
	local v3ChildGlobalPosition = CLuaVec3:new_local()
	local ChildID = GetChildIDByName(hszObjectName, ChildName)
	
	v3ChildGlobalPosition = tolua.cast( GetChildGlobalPositionByID( hszObjectName, ChildID), "CLuaVec3")
	
	return v3ChildGlobalPosition
end
---------------------------------------------------------------------------------
-- GetChildGlobalPositionByID
---------------------------------------------------------------------------------
function GetChildGlobalPositionByID( hszObjectName, ChildID)
	local v3ChildGlobalPosition = CLuaVec3:new_local()

	local hosp = HIERARCHICALOFFSETPARAM:new_local()
	hosp.m_Child = ChildID
	hosp.m_Offset = v3ChildGlobalPosition
	m_ToolBox:SendMessage(msgGetChildGlobalPosition, hosp:SIZEOF(), hosp, hszObjectName)
	
	return v3ChildGlobalPosition
end

---------------------------------------------------------------------------------
-- SET DATA
---------------------------------------------------------------------------------
---------------------------------------------------------------------------------
-- CHILD OFFSET ROTATION
---------------------------------------------------------------------------------
-- SetChildOffsetRotationByName
---------------------------------------------------------------------------------
function SetChildOffsetRotationByName( hszObjectName, ChildName, v3Rotation)
	local ChildID = GetChildIDByName(hszObjectName, ChildName)

	SetChildOffsetRotationByID( hszObjectName, ChildID, tolua.cast(v3Rotation, "CLuaEulerAngle"))
end
---------------------------------------------------------------------------------
-- SetChildOffsetRotationByID
---------------------------------------------------------------------------------
function SetChildOffsetRotationByID( hszObjectName, ChildID, v3Rotation)
	local v3ChildOffsetRotation = CLuaEulerAngle:new_local()
	local hosp = HIERARCHICALOFFSETPARAM:new_local()
	hosp.m_Child = ChildID
	hosp.m_Offset =  tolua.cast(v3Rotation, "CLuaEulerAngle")
	m_ToolBox:SendMessage(msgSetChildRotationOffset, hosp:SIZEOF(), hosp, hszObjectName)
end
---------------------------------------------------------------------------------
-- CHILD GLOBAL ROTATION
---------------------------------------------------------------------------------
-- SetChildGlobalRotationByName
---------------------------------------------------------------------------------
function SetChildGlobalRotationByName( hszObjectName, ChildName, v3Rotation)
	local ChildID = GetChildIDByName(hszObjectName, ChildName)

	SetChildGlobalRotationByID( hszObjectName, ChildID, tolua.cast(v3Rotation, "CLuaEulerAngle"))
end
---------------------------------------------------------------------------------
-- SetChildGlobalRotationByID
---------------------------------------------------------------------------------
function SetChildGlobalRotationByID( hszObjectName, ChildID, v3Rotation)
	local v3ChildGobalRotation = CLuaEulerAngle:new_local()

--	OutputDebug("Heir: v3Rotation: ("..v3Rotation:GetX()..","..v3Rotation:GetY()..","..v3Rotation:GetZ()..")\n")

	local hosp = HIERARCHICALOFFSETPARAM:new_local()
	hosp.m_Child = ChildID
	hosp.m_Offset = tolua.cast(v3Rotation, "CLuaEulerAngle")
	m_ToolBox:SendMessage(msgSetChildGlobalRotation, hosp:SIZEOF(), hosp, hszObjectName)
end
---------------------------------------------------------------------------------
-- CHILD OFFSET POSITION
---------------------------------------------------------------------------------
-- SetChildOffsetPositionByName
---------------------------------------------------------------------------------
function SetChildOffsetPositionByName( hszObjectName, ChildName, v3Position)
	local ChildID = GetChildIDByName(hszObjectName, ChildName)

	SetChildOffsetPositionByID( hszObjectName, ChildID, tolua.cast(v3Position, "CLuaVec3"))
end
---------------------------------------------------------------------------------
-- SetChildOffsetPositionByID
---------------------------------------------------------------------------------
function SetChildOffsetPositionByID( hszObjectName, ChildID, v3Position)
	local v3ChildOffsetPosition = CLuaVec3:new_local()
	local hosp = HIERARCHICALOFFSETPARAM:new_local()
	hosp.m_Child = ChildID
	hosp.m_Offset =  tolua.cast(v3Position, "CLuaVec3")
	m_ToolBox:SendMessage(msgSetChildPositionOffset, hosp:SIZEOF(), hosp, hszObjectName)
end
---------------------------------------------------------------------------------
-- CHILD GLOBAL POSITION
---------------------------------------------------------------------------------
-- SetChildGlobalPositionByName
---------------------------------------------------------------------------------
function SetChildGlobalPositionByName( hszObjectName, ChildName, v3Position)
	local ChildID = GetChildIDByName(hszObjectName, ChildName)

	SetChildGlobalPositionByID( hszObjectName, ChildID, tolua.cast(v3Position, "CLuaVec3"))
end
---------------------------------------------------------------------------------
-- SetChildGlobalPositionByID
---------------------------------------------------------------------------------
function SetChildGlobalPositionByID( hszObjectName, ChildID, v3Position)
	local v3ChildGobalPosition = CLuaVec3:new_local()

--	OutputDebug("Heir: v3Rotation: ("..v3Position:GetX()..","..v3Position:GetY()..","..v3Position:GetZ()..")\n")

	local hosp = HIERARCHICALOFFSETPARAM:new_local()
	hosp.m_Child = ChildID
	hosp.m_Offset = tolua.cast(v3Position, "CLuaVec3")
	m_ToolBox:SendMessage(msgSetChildGlobalPosition, hosp:SIZEOF(), hosp, hszObjectName)
end

---------------------------------------------------------------------------------
-- PlayMatrixAnimation
---------------------------------------------------------------------------------
function PlayMatrixAnimation(hszObjectName, hsAnimName, fSpeed, bLoop, bFreeze)
	local ha = HIERARCHICALANIMATION:new_local()
	ha.animtemplate = hsAnimName
	ha.speed = fSpeed
	ha.loop = bLoop
	ha.freezeOnLastFrame = bFreeze
	m_ToolBox:SendMessage(msgPlayMatrixAnimation, ha:SIZEOF(), ha, hszObjectName)
end

---------------------------------------------------------------------------------
-- GetChildGlobalMatrixByID
---------------------------------------------------------------------------------
function GetChildGlobalMatrixByID( hszObjectName, childID)
	local matrix = CLuaMatrix3x3:new_local()
	local hpm = HIERARCHICALPOSITIONMATRIX:new_local()
	hpm.m_Child = childID
	hpm.m_OutMatrix = matrix
	m_ToolBox:SendMessage(msgGetChildGlobalMatrix, hpm:SIZEOF(), hpm, hszObjectName)
	return matrix
end