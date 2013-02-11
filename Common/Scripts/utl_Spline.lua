---------------------------------------------------------------------------------
-- utl_spline.lua
-- place for spline messages
---------------------------------------------------------------------------------

m_ToolBox = CLuaToolBox:GetLuaToolBox()

-- global message names
msgAdvanceMarker		= CLuaHashString:new_local("AdvanceMarker")
msgGetSplineInfo		= CLuaHashString:new_local("GetSplineInfo")
msgAddPoint				= CLuaHashString:new_local("AddPoint")
msgClearTrailingPoints	= CLuaHashString:new_local("ClearTrailingPoints")
msgSetMarker			= CLuaHashString:new_local("SetMarker")
msgClearPoints			= CLuaHashString:new_local("ClearPoints")
msgGetNearestDistance	= CLuaHashString:new_local("GetNearestDistance")

---------------------------------------------------------------------------------
-- AdvanceMarker
---------------------------------------------------------------------------------
function AdvanceMarker(hsObjectName, fDistance)
	local vPosition = CLuaVec3:new_local()
	local advanceMarkerParams = SPLINEMARKERPARAMS:new_local()
	advanceMarkerParams.Distance = fDistance
	advanceMarkerParams.Position = vPosition
	m_ToolBox:SendMessage(msgAdvanceMarker, advanceMarkerParams:SIZEOF(), advanceMarkerParams, hsObjectName)
	return vPosition
end

---------------------------------------------------------------------------------
-- GetSplineInfo
---------------------------------------------------------------------------------
function GetSplineInfo(hsObjectName)
	local splineInfo = SPLINEINFOPARAMS:new_local()
	m_ToolBox:SendMessage(msgGetSplineInfo, splineInfo:SIZEOF(), splineInfo, hsObjectName)
	return splineInfo
end

---------------------------------------------------------------------------------
-- AddPoint
---------------------------------------------------------------------------------
function AddPoint(hsObjectName, vPoint)
	m_ToolBox:SendMessage(msgAddPoint, vPoint:SIZEOF(), vPoint, hsObjectName)
end

---------------------------------------------------------------------------------
-- ClearTrailingPoints
---------------------------------------------------------------------------------
function ClearTrailingPoints(hsObjectName)
	m_ToolBox:SendMessage(msgClearTrailingPoints, TODWORD(0), nil, hsObjectName)
end

---------------------------------------------------------------------------------
-- SetMarker
-- NOTE: different from tas... didn't understand how this was being used
---------------------------------------------------------------------------------
function SetMarker(hsObjectName, vPosition, fDistance)
	local setMarkerParams = SPLINEMARKERPARAMS:new_local()
	setMarkerParams.Position = vPosition
	setMarkerParams.Distance = fDistance
	m_ToolBox:SendMessage(msgSetMarker, setMarkerParams:SIZEOF(), setMarkerParams, hsObjectName)
end

---------------------------------------------------------------------------------
-- ClearPoints
---------------------------------------------------------------------------------
function ClearPoints(hsObjectName)
	m_ToolBox:SendMessage( msgClearPoints, TODWORD(0), nil, hsObjectName )
end

---------------------------------------------------------------------------------
-- GetNearestDistance
---------------------------------------------------------------------------------
function GetNearestDistance(hsSplineObjectName, vPosition, fMinDistance)
	local retDist = 0
	local getNearestParams = SPLINEGETNEARESTPARAMS:new_local()
	getNearestParams.Position = vPosition
	getNearestParams.MinDistance = fMinDistance
	m_ToolBox:SendMessage(msgGetNearestDistance, getNearestParams:SIZEOF(), getNearestParams, hsSplineObjectName )
	retDist = getNearestParams.Distance
	return retDist
end