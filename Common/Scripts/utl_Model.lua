---------------------------------------------------------------------------------
-- utl_Model.lua
-- Utils for render objects (hierarchical models as well as cal3d ro)
---------------------------------------------------------------------------------

ToolBox = CLuaToolBox:GetLuaToolBox()
-- global messages
msgGetGlobalBoneInfo			= CLuaHashString:new_local("GetGlobalBoneInfo")
msgGetOffsetBoneInfo			= CLuaHashString:new_local("GetOffsetBoneInfo")
msgSetOffsetBoneInfo			= CLuaHashString:new_local("SetOffsetBoneInfo")
msgSetObjectAlpha				= CLuaHashString:new_local("SetObjectAlpha")
msgPlayAnimation				= CLuaHashString:new_local("PlayAnimation")
msgPlaySingleCycleAnimation		= CLuaHashString:new_local("PlaySingleCycleAnimation")
msgStopAnimation				= CLuaHashString:new_local("StopAnimation")
msgStopAllAnimationActions		= CLuaHashString:new_local("StopAllAnimationActions")
msgRegisterAnimationCallback	= CLuaHashString:new_local("RegisterAnimationCallback")
msgSetRenderOffset				= CLuaHashString:new_local("SetRenderOffset")
msgGetRenderOffset				= CLuaHashString:new_local("GetRenderOffset")
msgPauseAnimations				= CLuaHashString:new_local("PauseAnimations")


-------------------------------------------------------  
-- GetGlobalBoneInfo
-------------------------------------------------------  
function GetGlobalBoneInfo(hsObjectName, hsBindPoint, vPosition, eRotation, mMatrix)
	local boneInfo = BONEINFO:new_local()

	boneInfo.bonename = hsBindPoint
	boneInfo.m_Position = CLuaVec3:new_local()
	boneInfo.m_Rotation = CLuaEulerAngle:new_local()
	boneInfo.m_Matrix = tolua.cast(mMatrix, "Matrix4x4")

	ToolBox:SendMessage( msgGetGlobalBoneInfo, boneInfo:SIZEOF(), boneInfo, hsObjectName )

	if vPosition ~= nil then
		vPosition:Set( boneInfo.m_Position )
	end
	if eRotation ~= nil then
		eRotation:Set( boneInfo.m_Rotation )
	end
end

-------------------------------------------------------  
-- GetOffsetBoneInfo
-------------------------------------------------------  
function GetOffsetBoneInfo(hsObjectName, hsBindPoint, vPosition, eRotation, mMatrix)
	local boneInfo = BONEINFO:new_local()

	boneInfo.bonename = hsBindPoint
	boneInfo.m_Position = CLuaVec3:new_local()
	boneInfo.m_Rotation = CLuaEulerAngle:new_local()
	boneInfo.m_Matrix = tolua.cast(mMatrix, "Matrix4x4")

	ToolBox:SendMessage( msgGetOffsetBoneInfo, boneInfo:SIZEOF(), boneInfo, hsObjectName )

	if vPosition ~= nil then
		vPosition:Set( boneInfo.m_Position )
	end
	if eRotation ~= nil then
		eRotation:Set( boneInfo.m_Rotation )
	end
end

-------------------------------------------------------  
-- SetOffsetBoneInfo
-------------------------------------------------------  
function SetOffsetBoneInfo(hsObjectName, hsBindPoint, vPosition, eRotation, mMatrix)
	local boneInfo = BONEINFO:new_local()

	boneInfo.bonename = hsBindPoint
	if vPosition ~= nil then
		boneInfo.m_Position = CLuaVec3:new_local( vPosition.x, vPosition.y, vPosition.z )
	end
	if eRotation ~= nil then
		boneInfo.m_Rotation = CLuaEulerAngle:new_local( eRotation.roll, eRotation.pitch, eRotation.yaw )
	end
	boneInfo.m_Matrix = tolua.cast(mMatrix, "Matrix4x4")

	ToolBox:SendMessage( msgSetOffsetBoneInfo, boneInfo:SIZEOF(), boneInfo, hsObjectName )	
end

-------------------------------------------------------  
-- SetObjectAlpha (clamped [0, 1])
-------------------------------------------------------  
function SetObjectAlpha(hsObjectName, fAlpha)
	local floatAlpha = TOFLOAT(fAlpha)
	ToolBox:SendMessage(msgSetObjectAlpha, FLOAT_SIZEOF(), floatAlpha, hsObjectName)
end

---------------------------------------------------------------------------------
-- PlayAnimation
---------------------------------------------------------------------------------
function PlayAnimation( ObjectName, AnimationName, bLockLastFrame, bLoop, fBlendIn, fBlendOut )
	if AnimationName ~= nil and AnimationName ~= "" then
		local roap = RENDEROBJECTANIMATIONPARAMS:new_local() 
		if bLoop ~= nil then
			roap.bLoop = bLoop
		else
			roap.bLoop = false
		end
		if (fBlendIn ~= nil) then
			roap.BlendIn = fBlendIn
		end
		if (fBlendOut ~= nil) then
			roap.BlendOut = fBlendOut
		end
		if type(AnimationName) == "number" then
			--OutputDebug("Playing Animation Number: #"..AnimationName.."\n")
			roap.AnimationID = AnimationName
		else
			--OutputDebug("Playing Animation Name: "..AnimationName.."\n")
			AnimName = CLuaHashString:new_local(AnimationName);
			roap.AnimationName = AnimName
		end

		if bLockLastFrame~= nil then
			roap.LockLastFrame = bLockLastFrame
		end

		local hszObjectName = ObjectName
		local result = ToolBox:SendMessage(msgPlayAnimation, roap:SIZEOF(), roap, hszObjectName )
		if result == 0 then
			OutputDebug( "Could not play animation '"..AnimationName.."' for object "..hszObjectName:GetString().."\n" )
		end
	end
end

---------------------------------------------------------------------------------
-- PlaySingleCycleAnimation
---------------------------------------------------------------------------------
function PlaySingleCycleAnimation( ObjectName, AnimationName, fBlendIn, fBlendOut )
	if AnimationName ~= nil and AnimationName ~= "" then
		local roap = RENDEROBJECTANIMATIONPARAMS:new_local() 
		roap.bLoop = true
		if (fBlendIn ~= nil) then
			roap.BlendIn = fBlendIn
		end
		if (fBlendOut ~= nil) then
			roap.BlendOut = fBlendOut
		end
		if type(AnimationName) == "number" then
			--OutputDebug("Playing Cycle Animation Number: #"..AnimationName.."\n")
			roap.AnimationID = AnimationName
		else
			--OutputDebug("Playing Cycle Animation Name: "..AnimationName.."\n")
			AnimName = CLuaHashString:new_local(AnimationName);
			roap.AnimationName = AnimName
		end
		
		local hszObjectName = ObjectName
		local result = ToolBox:SendMessage(msgPlaySingleCycleAnimation, roap:SIZEOF(), roap, hszObjectName )
		if result == 0 then
			OutputDebug( "Could not play animation '"..AnimationName.."' for object "..hszObjectName:GetString().."\n" )
		end
	end
end


---------------------------------------------------------------------------------
-- StopAnimation
---------------------------------------------------------------------------------
function StopAnimation( ObjectName, AnimationName, fBlendOut )
	if AnimationName ~= nil and AnimationName ~= "" then
		local roap = RENDEROBJECTANIMATIONPARAMS:new_local()

		if fBlendOut == nil then
			roap.BlendOut = 0.0
		else
			roap.BlendOut = fBlendOut
		end

		if type(AnimationName) == "number" then
			--OutputDebug("Stopping Animation Number: #"..AnimationName.."\n")
			roap.AnimationID = AnimationName
		else
			--OutputDebug("Stopping Animation Name: "..AnimationName.."\n")
			AnimName = CLuaHashString:new_local(AnimationName);
			roap.AnimationName = AnimName
		end
		
		hszObjectName = ObjectName
		ToolBox:SendMessage(msgStopAnimation, roap:SIZEOF(), roap, hszObjectName )
	end
end

---------------------------------------------------------------------------------
-- StopAllAnimationActions
---------------------------------------------------------------------------------
function StopAllAnimationActions( ObjectName )
	--OutputDebug("Stopping All Animations\n")

	hszObjectName = ObjectName
	ToolBox:SendMessage(msgStopAllAnimationActions, TODWORD(0), nil, hszObjectName )
end

---------------------------------------------------------------------------------
-- RegisterAnimationCallback
---------------------------------------------------------------------------------
function RegisterAnimationCallback( hszObjectName, animation, eventName, triggerTime, triggerOnComplete, triggerOnStop, archive )
	if animation ~= nil and animation ~= "" then
		local hsEventName = CLuaHashString:new_local(eventName)
		local animationEndEvent = hsEventName
		local animationCallbackParams = REGISTERCAL3DANIMATIONCALLBACK:new_local()

		if type(animation) == "number" then
			animationCallbackParams.AnimationID = animation
		else
			AnimName = CLuaHashString:new_local(animation);
			animationCallbackParams.AnimationName = AnimName
		end

		animationCallbackParams.AnimationTriggerTime = triggerTime
		animationCallbackParams.bTriggerOnComplete = triggerOnComplete
		animationCallbackParams.bTriggerOnStop = triggerOnStop
		animationCallbackParams.StateObjectName = hszObjectName
		animationCallbackParams.EventName = animationEndEvent
		if archive == nil then
			animationCallbackParams.EventParamsArchive = nil
		else
			animationCallbackParams.EventParamsArchive = tolua.cast(archive, "IArchive")
		end	
		ToolBox:SendMessage(msgRegisterAnimationCallback, animationCallbackParams:SIZEOF(), animationCallbackParams, hszObjectName)
	else
		OutputDebug( "Skipping registration for event '"..eventName.."' with nil animation identifier.\n" )
	end
end

---------------------------------------------------------------------------------
-- SetRenderOffset
---------------------------------------------------------------------------------
function SetRenderOffset( hszObjectName, hszNodeName )
	if hszObjectName ~= nil and hszNodeName ~= nil then
		ToolBox:SendMessage( msgSetRenderOffset, TODWORD(4), hszNodeName, hszObjectName )
	end
end

---------------------------------------------------------------------------------
-- GetRenderOffset
---------------------------------------------------------------------------------
function GetRenderOffset( hszObjectName, vOffset )
	if hszObjectName ~= nil and vOffset ~= nil then
		ToolBox:SendMessage( msgGetRenderOffset, vOffset:SIZEOF(), vOffset, hszObjectName )
	end
end

---------------------------------------------------------------------------------
-- PauseAnimations
---------------------------------------------------------------------------------
function PauseAnimations(hsObject, bPause)
	local val = TODWORD(0)
	if bPause == true then
		val = TODWORD(1)
	end
	ToolBox:SendMessage(msgPauseAnimations, TODWORD(1), val, hsObject)
end