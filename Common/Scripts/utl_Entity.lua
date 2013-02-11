---------------------------------------------------------------------------------
-- utl_entity.lua
-- place for spacial message get/sets
---------------------------------------------------------------------------------

m_ToolBox = CLuaToolBox:GetLuaToolBox()
m_ToolBox:LoadLuaDependency( "scripts\\utl_Math.lua" )

-- some known types
hsCEntity						= CLuaHashString:new_local("CEntity")
hsCCharacterPhysics				= CLuaHashString:new_local("CEECharacterPhysicsObject")
hsCTerrainSector				= CLuaHashString:new_local("CTerrainSector")

-- global message names
msgGetGlobalPosition			= CLuaHashString:new_local("GetGlobalPosition")
msgSetGlobalPosition			= CLuaHashString:new_local("SetGlobalPosition")
msgGetGlobalEuler				= CLuaHashString:new_local("GetGlobalEuler")
msgSetGlobalEuler				= CLuaHashString:new_local("SetGlobalEuler")
msgGetGlobalScale				= CLuaHashString:new_local("GetGlobalScale")
msgSetGlobalScale				= CLuaHashString:new_local("SetGlobalScale")
msgGetGlobalTransform			= CLuaHashString:new_local("GetGlobalTransform")
msgSetGlobalTransform			= CLuaHashString:new_local("SetGlobalTransform")

msgGetLocalPosition				= CLuaHashString:new_local("GetLocalPosition")
msgSetLocalPosition				= CLuaHashString:new_local("SetLocalPosition")
msgGetLocalEuler				= CLuaHashString:new_local("GetLocalEuler")
msgSetLocalEuler				= CLuaHashString:new_local("SetLocalEuler")
msgGetLocalScale				= CLuaHashString:new_local("GetLocalScale")
msgSetLocalScale				= CLuaHashString:new_local("SetLocalScale")

msgGetParentName				= CLuaHashString:new_local("GetParentName")
msgChangeObjectParent			= CLuaHashString:new_local("ChangeObjectParent")
msgGetEntityType				= CLuaHashString:new_local("GetEntityType")
msgIsEntityTypeOf				= CLuaHashString:new_local("IsEntityTypeOf")

msgMoveCharacter				= CLuaHashString:new_local("MoveCharacter")
msgSetCharacterHeight			= CLuaHashString:new_local("SetCharacterHeight")
msgSetCharacterWidth			= CLuaHashString:new_local("SetCharacterWidth")
msgLockHorizontalAxies			= CLuaHashString:new_local("LockHorizontalAxies")
msgUnLockHorizontalAxies		= CLuaHashString:new_local("UnLockHorizontalAxies")
msgGetPhysicsLocalPosition		= CLuaHashString:new_local("GetLocalPosition")
msgSetPhysicsLocalPosition		= CLuaHashString:new_local("SetLocalPosition")

---------------------------------------------------------------------------------
-- GetGlobalPosition
-- hsObjectName = name of object to get position of
-- returns Vec3 position
---------------------------------------------------------------------------------
function GetGlobalPosition(hsObjectName, vPos)
	if vPos == nil then
		vPos = CLuaVec3:new_local()
	end
	m_ToolBox:SendMessage(msgGetGlobalPosition, vPos:SIZEOF(), vPos, hsObjectName, hsCEntity)
	return LuaMathLib:Vec3( vPos )
end

---------------------------------------------------------------------------------
-- SetGlobalPosition
-- hsObjectName = name of object to set position of
-- vPosition = Vec3 of position
---------------------------------------------------------------------------------
function SetGlobalPosition(hsObjectName, vPosition)
	local vPos = CLuaVec3:new_local( vPosition.x, vPosition.y, vPosition.z )
	m_ToolBox:SendMessage(msgSetGlobalPosition, vPos:SIZEOF(), vPos, hsObjectName, hsCEntity)
end

---------------------------------------------------------------------------------
-- GetGlobalEuler
-- hsObjectName = name of object to get orientation of
-- returns EulerAngle orientation
---------------------------------------------------------------------------------
function GetGlobalEuler(hsObjectName)
	local eOrient = CLuaEulerAngle:new_local()
	m_ToolBox:SendMessage(msgGetGlobalEuler, eOrient:SIZEOF(), eOrient, hsObjectName, hsCEntity)

	return LuaMathLib:EulerAngle( eOrient )
end

---------------------------------------------------------------------------------
-- SetGlobalEuler
-- hsObjectName = name of object to set orientation of
-- eOrient = EulerAngle rotation
---------------------------------------------------------------------------------
function SetGlobalEuler(hsObjectName, eOrient)
	local eEuler = CLuaEulerAngle:new_local( eOrient.roll, eOrient.pitch, eOrient.yaw )
	m_ToolBox:SendMessage(msgSetGlobalEuler, eEuler:SIZEOF(), eEuler, hsObjectName, hsCEntity)
end

---------------------------------------------------------------------------------
-- GetGlobalScale
-- hsObjectName = name of object to get scale of
-- returns CLuaVec3 scale
---------------------------------------------------------------------------------
function GetGlobalScale(hsObjectName)
	local vScale = CLuaVec3:new_local()
	m_ToolBox:SendMessage(msgGetGlobalScale, vScale:SIZEOF(), vScale, hsObjectName, hsCEntity)

	return LuaMathLib:Vec3( vScale )
end

---------------------------------------------------------------------------------
-- SetGlobalScale
-- hsObjectName = name of object to set scale of
-- vScale = CLuaVec3 of scale
---------------------------------------------------------------------------------
function SetGlobalScale(hsObjectName, vScale)
	local vNewScale = CLuaVec3:new_local( vScale.x, vScale.y, vScale.z )
	m_ToolBox:SendMessage(msgSetGlobalScale, vNewScale:SIZEOF(), vNewScale, hsObjectName, hsCEntity)
end

---------------------------------------------------------------------------------
-- GetGlobalTransform
---------------------------------------------------------------------------------
function GetGlobalTransform(hsObjectName)
	local transform = CLuaMatrix4x4:new_local()	
	m_ToolBox:SendMessage(msgGetGlobalTransform, transform:SIZEOF(), transform, hsObjectName, hsCEntity)
	return transform
end

---------------------------------------------------------------------------------
-- SetGlobalTransform
---------------------------------------------------------------------------------
function SetGlobalTransform(hsObjectName, transform4x4)
	m_ToolBox:SendMessage(msgSetGlobalTransform, transform4x4:SIZEOF(), transform4x4, hsObjectName, hsCEntity)	
end

---------------------------------------------------------------------------------
-- GetLocalPosition
-- hsObjectName = name of object to get position of
-- returns CLuaVec3 position
---------------------------------------------------------------------------------
function GetLocalPosition(hsObjectName)
	local vPos = CLuaVec3:new_local()
	m_ToolBox:SendMessage(msgGetLocalPosition, vPos:SIZEOF(), vPos, hsObjectName, hsCEntity)

	return LuaMathLib:Vec3( vPos )
end

---------------------------------------------------------------------------------
-- SetLocalPosition
-- hsObjectName = name of object to set position of
-- vPosition = CLuaVec3 of position
---------------------------------------------------------------------------------
function SetLocalPosition(hsObjectName, vPosition)
	local vPos = CLuaVec3:new_local( vPosition.x, vPosition.y, vPosition.z )
	m_ToolBox:SendMessage(msgSetLocalPosition, vPos:SIZEOF(), vPos, hsObjectName, hsCEntity)
end

---------------------------------------------------------------------------------
-- GetLocalEuler
-- hsObjectName = name of object to get orientation of
-- returns EulerAngle orientation
---------------------------------------------------------------------------------
function GetLocalEuler(hsObjectName)
	local eOrient = CLuaEulerAngle:new_local()
	m_ToolBox:SendMessage(msgGetLocalEuler, eOrient:SIZEOF(), eOrient, hsObjectName, hsCEntity)

	return LuaMathLib:EulerAngle( eOrient )
end

---------------------------------------------------------------------------------
-- SetLocalEuler
-- hsObjectName = name of object to set orientation of
-- eEuler = EulerAngle rotation
---------------------------------------------------------------------------------
function SetLocalEuler(hsObjectName, eEuler)
	local eRotation = CLuaEulerAngle:new_local( eEuler.roll, eEuler.pitch, eEuler.yaw )
	m_ToolBox:SendMessage(msgSetLocalEuler, eRotation:SIZEOF(), eRotation, hsObjectName, hsCEntity)
end

---------------------------------------------------------------------------------
-- GetLocalScale
-- hsObjectName = name of object to get scale of
-- returns CLuaVec3 scale
---------------------------------------------------------------------------------
function GetLocalScale(hsObjectName)
	local vScale = CLuaVec3:new_local()
	m_ToolBox:SendMessage(msgGetLocalScale, vScale:SIZEOF(), vScale, hsObjectName, hsCEntity)

	return LuaMathLib:Vec3( vScale )
end

---------------------------------------------------------------------------------
-- SetLocalScale
-- hsObjectName = name of object to set scale of
-- vScale = CLuaVec3 of scale
---------------------------------------------------------------------------------
function SetLocalScale(hsObjectName, vScale)
	local vNewScale = CLuaVec3:new_local( vScale.x, vScale.y, vScale.z )
	m_ToolBox:SendMessage(msgSetLocalScale, vNewScale:SIZEOF(), vNewScale, hsObjectName, hsCEntity)
end

---------------------------------------------------------------------------------
-- GetParentName
---------------------------------------------------------------------------------
function GetParentName(hsObjectName)
	local hsParentName = CLuaHashString:new_local()
	m_ToolBox:SendMessage(msgGetParentName, TODWORD(4), hsParentName, hsObjectName, hsCEntity)
	return hsParentName
end
---------------------------------------------------------------------------------
-- ChangeParentName
---------------------------------------------------------------------------------
function ChangeParentName( hszObjectName, hszNewParentName )
	local hsParentName = CLuaHashString:new_local()
	hsParentName = GetParentName(hszObjectName)
	local cop = CHANGEOBJECTPARENT:new_local()
	cop.objectName = hszObjectName
	cop.newParentName = hszNewParentName
	m_ToolBox:SendMessage(msgChangeObjectParent, cop:SIZEOF(), cop, hszObjectName, hsCEntity)

end

---------------------------------------------------------------------------------
-- GetEntityType
---------------------------------------------------------------------------------
function GetEntityType(hsObjectName)
	local hsEntityType = CLuaHashString:new_local()
	m_ToolBox:SendMessage(msgGetEntityType, TODWORD(4), hsEntityType, hsObjectName, hsCEntity)
	return hsEntityType
end

---------------------------------------------------------------------------------
-- IsEntityTypeOf
---------------------------------------------------------------------------------
function IsEntityTypeOf(hsObjectName, hsTypeName)
	local ret = false
	local ieotp = ISENTITYOFTYPEPARAMS:new_local()
	ieotp.entityType = hsTypeName
	m_ToolBox:SendMessage(msgIsEntityTypeOf, ieotp:SIZEOF(), ieotp, hsObjectName, hsCEntity)

	ret = ieotp.bResult
	return ret
end

---------------------------------------------------------------------------------
-- GetEntityTypeInclusive
-- Gets entity type without specifying the component
---------------------------------------------------------------------------------
function GetEntityTypeInclusive(hsObjectName)
	local hsEntityType = CLuaHashString:new_local()
	m_ToolBox:SendMessage(msgGetEntityType, TODWORD(4), hsEntityType, hsObjectName)
	return hsEntityType
end

---------------------------------------------------------------------------------
-- IsEntityTypeOfInclusive
-- Compares entity type without specifying the component
---------------------------------------------------------------------------------
function IsEntityTypeOfInclusive(hsObjectName, hsTypeName)
	local ret = false
	local ieotp = ISENTITYOFTYPEPARAMS:new_local()
	ieotp.entityType = hsTypeName
	m_ToolBox:SendMessage(msgIsEntityTypeOf, ieotp:SIZEOF(), ieotp, hsObjectName)

	ret = ieotp.bResult
	return ret
end

---------------------------------------------------------------------------------
-- MoveCharacter
-- hsObjectName = name of object to set position of
-- vPosition = CLuaVec3 of position
---------------------------------------------------------------------------------
function MoveCharacter(hsObjectName, vPosition)
	m_ToolBox:SendMessage( msgMoveCharacter, vPosition:SIZEOF(), vPosition, hsObjectName, hsCCharacterPhysics )
end

---------------------------------------------------------------------------------
-- SetCharacterHeight
-- hsObjectName = name of object to set height of
-- fHeight = new height
---------------------------------------------------------------------------------
function SetCharacterHeight(hsObjectname, fHeight)
	local height = TOFLOAT(fHeight)
	m_ToolBox:SendMessage( msgSetCharacterHeight, FLOAT_SIZEOF(), height, hsObjectname, hsCCharacterPhysics )
end

---------------------------------------------------------------------------------
-- SetCharacterWidth
-- hsObjectName = name of object to set width of
-- fWidth = new width
---------------------------------------------------------------------------------
function SetCharacterWidth(hsObjectname, fWidth)
	local width = TOFLOAT(fWidth)
	m_ToolBox:SendMessage( msgSetCharacterWidth, FLOAT_SIZEOF(), width, hsObjectname, hsCCharacterPhysics )
end

---------------------------------------------------------------------------------
-- LockHorizontalAxies
---------------------------------------------------------------------------------
function LockHorizontalAxies(hsObjectname)
	m_ToolBox:SendMessage( msgLockHorizontalAxies, TODWORD(0), nil, hsObjectname, hsCCharacterPhysics )
end

---------------------------------------------------------------------------------
-- UnLockHorizontalAxies
---------------------------------------------------------------------------------
function UnLockHorizontalAxies(hsObjectname)
	m_ToolBox:SendMessage( msgUnLockHorizontalAxies, TODWORD(0), nil, hsObjectname, hsCCharacterPhysics )
end

---------------------------------------------------------------------------------
-- GetCharacterPhysicsLocalPosition
---------------------------------------------------------------------------------
function GetPhysicsLocalPosition(hsObjectname, newLocalPosition)
	m_ToolBox:SendMessage( msgGetPhysicsLocalPosition, newLocalPosition:SIZEOF(), newLocalPosition, hsObjectname, hsCCharacterPhysics )
end

---------------------------------------------------------------------------------
-- SetCharacterPhysicsLocalPosition
---------------------------------------------------------------------------------
function SetPhysicsLocalPosition(hsObjectname, newLocalPosition)
	m_ToolBox:SendMessage( msgSetPhysicsLocalPosition, newLocalPosition:SIZEOF(), newLocalPosition, hsObjectname, hsCCharacterPhysics )
end
