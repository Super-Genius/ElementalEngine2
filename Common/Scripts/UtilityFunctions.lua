---------------------------------------------------------------------------------
-- UtilityFunctions.lua
-- Contains handy dandy utility functions so you don't have to!
---------------------------------------------------------------------------------

ToolBox = CLuaToolBox:GetLuaToolBox()
ToolBox:LoadLuaDependency( "scripts/utl_math.lua" )
MathLib = CLuaMath:new_local()
Timer	= CLuaTimer:new_local()

MSG_NOT_HANDLED =		0
MSG_HANDLED = 			1
MSG_HANDLED_PROCEED =	2
MSG_HANDLED_STOP =		3
MSG_SIZE_INVALID = 		4
MSG_ERROR =				3131956909 --0xBAADDEAD
MSG_WARNING =			3135106223 -- 0xBADDECAF

local debug = 1

-- Container for various global utility functions
Utils = {}

local m_GlobalStaticMap = {}

-- global message names
msgLoadFileByExtension			= CLuaHashString:new_local("LoadFileByExtension")
msgGetSpawnedName				= CLuaHashString:new_local("GetSpawnedName")
msgSetSpawnParentName			= CLuaHashString:new_local("SetSpawnParentName")
msgSetSpawnPosition				= CLuaHashString:new_local("SetSpawnPosition")
msgSetSpawnEuler				= CLuaHashString:new_local("SetSpawnEuler")
msgLoadAndStartScript			= CLuaHashString:new_local("LoadAndStartScript")
msgLoadAndInitStateObject		= CLuaHashString:new_local("LoadAndInitStateObject")
msgGetCurrentState				= CLuaHashString:new_local("GetCurrentState")
msgTriggerEvent					= CLuaHashString:new_local("TriggerEvent")
msgTriggerRecursiveEvent		= CLuaHashString:new_local("TriggerRecursiveEvent")
msgCreateObject					= CLuaHashString:new_local("CreateObject")
msgSerializeObject				= CLuaHashString:new_local("SerializeObject")
msgInitObject					= CLuaHashString:new_local("InitObject")
msgDeleteObject					= CLuaHashString:new_local("DeleteObject")
msgDelayedDeleteObject			= CLuaHashString:new_local("DelayedDeleteObject")
msgFindObject					= CLuaHashString:new_local("FindObject")
msgBroadcastWorldEvent			= CLuaHashString:new_local("BroadcastWorldEvent")
msgRegisterLuaActions			= CLuaHashString:new_local("RegisterLuaActions")
msgEnable						= CLuaHashString:new_local("Enable")
msgDisable						= CLuaHashString:new_local("Disable")
msgGetFileVersion				= CLuaHashString:new_local("GetFileVersion")
msgSetFileVersion				= CLuaHashString:new_local("SetFileVersion")
msgSubmitTypeQuery				= CLuaHashString:new_local("SubmitTypeQuery")
msgSubmitQuery					= CLuaHashString:new_local("SubmitQuery")
msgClearHierarchy				= CLuaHashString:new_local("ClearHierarchy")
msgTransformWorldToScreen		= CLuaHashString:new_local("TransformWorldToScreen")
msgGetDBFileName				= CLuaHashString:new_local("GetDBFileName")
msgSetPropertyTableValue		= CLuaHashString:new_local("SetPropertyTableValue")
msgLoadingUpdate				= CLuaHashString:new_local("LoadingUpdate")
msgAttachSceneObject			= CLuaHashString:new_local("AttachSceneObject")
msgDetachSceneObject			= CLuaHashString:new_local("DetachSceneObject")
msgAddDecalSprite				= CLuaHashString:new_local("AddDecalSprite")
msgGetComponentType				= CLuaHashString:new_local("GetComponentType")
msgCreateTerrainDecal			= CLuaHashString:new_local("CreateTerrainDecal")
msgSetLength					= CLuaHashString:new_local("SetLength")
msgSetColorCorrection			= CLuaHashString:new_local("SetColorCorrection")
msgSwapJoystickDeviceMapping	= CLuaHashString:new_local("SwapJoystickDeviceMapping")
msgGetJoystickDeviceMapping		= CLuaHashString:new_local("GetJoystickDeviceMapping")
msgGetVideoSize					= CLuaHashString:new_local("GetVideoSize")			
msgSetViewPort					= CLuaHashString:new_local("SetViewPort")
msgQueryJoystickFault			= CLuaHashString:new_local("QueryJoystickFault")
msgGetJoystickCount				= CLuaHashString:new_local("GetJoystickCount")
msgAddPrecache					= CLuaHashString:new_local("AddPrecache")
msgRemovePrecache				= CLuaHashString:new_local("RemovePrecache")
msgCommitPrecache				= CLuaHashString:new_local("CommitPrecache")

--global constant strings
hszMemoryType					= CLuaHashString:new_local("Memory")

---------------------------------------------------------------------------------
-- debug outputting
---------------------------------------------------------------------------------
function PrintDebug(string_out)

	if( debug == 1 ) then
		if( string_out ~= nil ) then
			OutputDebug( string_out )
			OutputDebug( "\n" )
		end
	end
	
end

---------------------------------------------------------------------------------
-- LoadSpawnFile
---------------------------------------------------------------------------------
function LoadSpawnFile( fileName, parentName )
	if (parentName ~= nil) then 
		local tmpParentName = CLuaHashString:new_local( parentName )
		SetSpawnParentName(tmpParentName)
	end
	LoadFileByExt( fileName, true )
end

---------------------------------------------------------------------------------
-- LoadFileByExt
---------------------------------------------------------------------------------
function LoadFileByExt( fileName, bInternalLoad )
	local lfep = LOADFILEEXTPARAMS:new_local()
	lfep.fileName = fileName
	lfep.bInternalLoad = bInternalLoad
	ToolBox:SendMessage(msgLoadFileByExtension, lfep:SIZEOF(), lfep)
end

---------------------------------------------------------------------------------
-- GetSpawnedName
---------------------------------------------------------------------------------
function GetSpawnedName()
	local tempName = CLuaHashString:new_local("")
	ToolBox:SendMessage(msgGetSpawnedName, TODWORD(4), tempName)
	return tempName
end

---------------------------------------------------------------------------------
-- SetSpawnParentName
---------------------------------------------------------------------------------
function SetSpawnParentName(hsParentName)
	ToolBox:SendMessage(msgSetSpawnParentName, TODWORD(4), hsParentName)
end

---------------------------------------------------------------------------------
-- SetSpawnPosition
---------------------------------------------------------------------------------
function SetSpawnPosition(vPos)
	local vPosition = CLuaVec3:new_local( vPos.x, vPos.y, vPos.z )
	ToolBox:SendMessage(msgSetSpawnPosition, vPosition:SIZEOF(), vPosition)
end

---------------------------------------------------------------------------------
-- SetSpawnEuler
---------------------------------------------------------------------------------
function SetSpawnEuler(eEul)
	local eEuler = CLuaEulerAngle:new_local( eEuler.roll, eEuler.pitch, eEuler.yaw )
	ToolBox:SendMessage(msgSetSpawnEuler, eEuler:SIZEOF(), eEuler)
end

---------------------------------------------------------------------------------
-- LoadAndStartScript
---------------------------------------------------------------------------------
function LoadAndStartScript( ScriptName, AutoStart )
	local ess = EXECUTESCRIPTSTRUCT:new_local()
	
	ess.fileName = ScriptName
	ess.bAutoStart = AutoStart
	local result = ToolBox:SendMessage(msgLoadAndStartScript, ess:SIZEOF(), ess)

-- DWORD compare doesn't work yet... so we can't error check yet	
--	if( result ~= TODWORD(MSG_HANDLED) ) then
--		PrintDebug( "Error Loading And Starting Script: "..ScriptName )
--	end
	
	return result	
end

---------------------------------------------------------------------------------
-- LoadAndInitState
---------------------------------------------------------------------------------
function LoadAndInitState( StateObjectName, StateObjectParentName, StateMachineName, StateMachineFileName, StateMachineStartState, IsInHierarchy, iPriority )
	local liso = LOADANDINITSTATEOBJECTPARAMS:new_local()
	
	local hsStateObjectName = CLuaHashString:new_local(StateObjectName)
	local hsStateObjectParentName = CLuaHashString:new_local(StateObjectParentName)
	local hsStateMachineName = CLuaHashString:new_local(StateMachineName)
	local hsStateMachineFileName = CLuaHashString:new_local(StateMachineFileName)
	local hsStateMachineStartState = CLuaHashString:new_local(StateMachineStartState)
	liso.StateObjectName = hsStateObjectName
	liso.StateObjectParentName = hsStateObjectParentName
	liso.StateMachineName = hsStateMachineName
	liso.StateMachineFileName = hsStateMachineFileName
	liso.StateMachineStartState = hsStateMachineStartState
	liso.bIsInHierarchy = IsInHierarchy
	liso.iUpdatePriority = iPriority
	local result = ToolBox:SendMessage(msgLoadAndInitStateObject, liso:SIZEOF(), liso)
	
-- DWORD compare doesn't work yet... so we can't error check yet	
--	if( result ~= MSG_HANDLED ) then
--		PrintDebug( "Error Loading And Starting State Object: "..StateObjectName )
--		PrintDebug( "                          State Machine: "..StateMachineName )
--		PrintDebug( "                Sttate Machine FileName: "..StateMachineName )
--	end
	
	return result	
end

---------------------------------------------------------------------------------
-- GetCurrentState
---------------------------------------------------------------------------------
function GetCurrentState( hszStateObjectName )
	local hszCurrentState = CLuaHashString:new_local( "" )
	if hszStateObjectName == nil or hszStateObjectName == "" then
		OutputDebug( "Unable to get current state for unspecified state object.\n" )
	else
		local hsStateObjectType = CLuaHashString:new_local( "CQHStateObject" )
		ToolBox:SendMessage( msgGetCurrentState, TODWORD(12), hszCurrentState, hszStateObjectName, hsStateObjectType )
	end

	return hszCurrentState:GetString()
end

---------------------------------------------------------------------------------
-- TriggerEvent
-- NOTE: eventarchive should be CLuaArchive, will be cast in here
---------------------------------------------------------------------------------
function TriggerEvent( StateObjectName, EventName, EventArchive )
	if StateObjectName == nil or StateObjectName == "" then
		OutputDebug( LOGWARNING, "Attempt to trigger event '"..EventName.."' on unspecified state object\n" )
	elseif EventName == "" then
		OutputDebug( LOGWARNING, "Attempt to trigger unspecified event on state object '"..StateObjectName.."'\n" )
	else
		local tep = TRIGGEREVENTPARAMS:new_local()
		
		local hsEventName = CLuaHashString:new_local(EventName)
		tep.EventName = hsEventName
		if EventArchive == nil then
			tep.EventParamsArchive = nil
		else
			tep.EventParamsArchive = tolua.cast(EventArchive, "IArchive")
		end

		local hsStateObjectName = CLuaHashString:new_local(StateObjectName)
		local hsStateObjectType = CLuaHashString:new_local("CQHStateObject")
		local result = ToolBox:SendMessage(msgTriggerEvent, tep:SIZEOF(), tep, hsStateObjectName, hsStateObjectType)
		if result == 0 then
			OutputDebug( LOGWARNING, "Could not trigger event '"..EventName.."' for object "..hsStateObjectName:GetString().."\n" )
		end
	end
end

---------------------------------------------------------------------------------
-- TriggerRecursiveEvent
-- NOTE: eventarchive should be CLuaArchive, will be cast in here
---------------------------------------------------------------------------------
function TriggerRecursiveEvent( StateObjectName, EventName, EventArchive )
	local tep = TRIGGEREVENTPARAMS:new_local()

	local hsEventName = CLuaHashString:new_local(EventName)
	tep.EventName = hsEventName
	if EventArchive == nil then
		tep.EventParamsArchive = nil
	else
		tep.EventParamsArchive = tolua.cast(EventArchive, "IArchive")
	end
	
	local hsStateObjectName = CLuaHashString:new_local(StateObjectName)
	local hsStateObjectType = CLuaHashString:new_local("CQHStateObject")
	local result = ToolBox:SendMessage(msgTriggerRecursiveEvent, tep:SIZEOF(), tep, hsStateObjectName, hsStateObjectType)

	return result	
end

---------------------------------------------------------------------------------
-- Create object
---------------------------------------------------------------------------------
function CreateObject(hsObjectName, hsObjectType, hsParentName)
	local cop = CREATEOBJECTPARAMS:new_local()
	cop.name = hsObjectName
	cop.typeName = hsObjectType
	cop.parentName = hsParentName
	ToolBox:SendMessage(msgCreateObject, cop:SIZEOF(), cop)
end

---------------------------------------------------------------------------------
-- Serialize object
---------------------------------------------------------------------------------
function SerializeObject(hsObjectName, archive)
	local sop = SERIALIZEOBJECTPARAMS:new_local()
	sop.name = hsObjectName
	sop.archive = tolua.cast(archive, "IArchive")
	ToolBox:SendMessage(msgSerializeObject, sop:SIZEOF(), sop)
end

---------------------------------------------------------------------------------
-- Init object
---------------------------------------------------------------------------------
function InitObject(hsObjectName)
	local iop = INITOBJECTPARAMS:new_local()
	iop.name = hsObjectName
	ToolBox:SendMessage(msgInitObject, iop:SIZEOF(), iop)
end

---------------------------------------------------------------------------------
-- Delete Object
---------------------------------------------------------------------------------
function DeleteObject( objectName )	
	-- send the message to delete the object
	local dop = DELETEOBJECTPARAMS:new_local() 
	dop.name = objectName
	ToolBox:SendMessage( msgDeleteObject, dop:SIZEOF(), dop )	
end

---------------------------------------------------------------------------------
-- Delayed Delete Object
---------------------------------------------------------------------------------
function DelayedDeleteObject( objectName )	
	-- send the message to delete the object
	local dop = DELETEOBJECTPARAMS:new_local() 
	dop.name = objectName
	ToolBox:SendMessage( msgDelayedDeleteObject, dop:SIZEOF(), dop )	
end

---------------------------------------------------------------------------------
-- Returns whether the object exists or not
---------------------------------------------------------------------------------
function FindObject( hszObjectName )
	local findObjectParams = FINDOBJECTPARAMS:new_local()
	findObjectParams.hszName = hszObjectName
	ToolBox:SendMessage( msgFindObject, findObjectParams:SIZEOF(), findObjectParams )

	return findObjectParams.bFound
end

---------------------------------------------------------------------------------
-- InheritFrom
---------------------------------------------------------------------------------
function Utils:InheritFrom( baseClass )

	local newClass = {}
	local class_mt = { __index = newClass }

	function newClass:create()
		local newInstance = {}
		setmetatable( newInstance, class_mt )
		return newInstance
	end

	if nil ~= baseClass then
		setmetatable( newClass, { __index = baseClass } )
	end

	-- Return the class object of the instance
	function newClass:class()
		return newClass
	end

	-- Return the super class object of the instance
	function newClass:superClass()
		return baseClass
	end

	-- Return true if the caller is an instance of theClass
	function newClass:IsA( theClass )
		local bIsA = false
		local cur_class = newClass

		while ( cur_class ~= nil ) and ( bIsA == false ) do
			if cur_class == theClass then
				bIsA = true
			else
				cur_class = cur_class:superClass()
			end
		end

		return bIsA
	end

	return newClass
end
---------------------------------------------------------------------------------


-------------------------------------------------------  
 -- dump a lua variable to debug output  
 -------------------------------------------------------  
 g_dump_tabs=0  
 function dump(_class,name)
	if name == nil then
		name = "Object"
	end

	if (g_dump_tabs == 0) then
		PrintDebug("Dumping: "..name)
	end

	local str=""  
	for n=0,g_dump_tabs,1 do  
		str=str.."  "  
	end  
	
	if not _class then  
		PrintDebug(str..name.." = nil")  
	else  
		if (type(_class)=="table") then  
			for i,field in pairs(_class) do  
				g_dump_tabs=g_dump_tabs+1
				-- Get Extra Table Data
				if(type(field)=="table") then  
					-- Total Size
					local tablecount = 0
					for k, v in pairs(field) do
						tablecount = tablecount + 1
					end
					-- Vector Size
					local vectorcount = 0
					for j, v in ipairs(field) do
						vectorcount = vectorcount + 1
					end
					
					PrintDebug(str..i.." = {} (size: "..tablecount.." vector: "..vectorcount..")")  
				end
				dump(field, i) 
				g_dump_tabs=g_dump_tabs-1
			end
		else
			if(type(_class)=="number" ) then
				PrintDebug(str..name.." = ".._class)
			elseif(type(_class) == "string") then  
				PrintDebug(str..name.." = ".."\"".._class.."\"")
			elseif(type(_class) == "boolean") then
				if _class == true then
					PrintDebug(str..name.." = ".."<True>")
				else
					PrintDebug(str..name.." = ".."<False>")
				end
			elseif(type(_class) == "CLuaHashString") then 		-- These don't work, in my dreams they do
				PrintDebug(str..name.." = ".."CLuaHashString(\"".._class:GetString().."\")")
			elseif(type(_class) == "CLuaVec3") then 			-- These don't work, in my dreams they do
				PrintDebug(str..name.." = ".."CLuaVec3(x: ".._class:GetX()..", y: ".._class:GetY()..", z: ".._class:GetZ()..")")
			elseif(type(_class) == "CLuaEulerAngle") then 		-- These don't work, in my dreams they do
				PrintDebug(str..name.." = ".."CLuaEulerAngle(yaw: ".._class:GetYaw()..", pitch: ".._class:GetPitch()..", roll: ".._class:GetRoll()..")")
			else  
				if(type(_class)=="function") then
					PrintDebug(str..name.."()")  
				else
					-- The "right way" was tried above and doesn't work. This is a way that works, though is pretty hacky.
					-- Checks to see if functions are not nil, if ok, call!
					if (_class.GetString ~= nil) then
						if (_class:GetString() ~= nil) then
							PrintDebug(str..name.." = ".."CLuaHashString(\"".._class:GetString().."\")")
						else
							PrintDebug(str..name.." = ".."CLuaHashString(\"\")")
						end
					elseif (_class.GetX ~= nil) and (_class.GetY ~= nil) and (_class.GetZ ~= nil) then
						PrintDebug(str..name.." = ".."CLuaVec3(x: ".._class:GetX()..", y: ".._class:GetY()..", z: ".._class:GetZ()..")")
					elseif (_class.GetYaw ~= nil) and (_class.GetPitch ~= nil) and (_class.GetRoll ~= nil) then
						PrintDebug(str..name.." = ".."CLuaEulerAngle(yaw: ".._class:GetYaw()..", pitch: ".._class:GetPitch()..", roll: ".._class:GetRoll()..")")
					else
						PrintDebug(str..name.." = <userdata = "..type(_class)..">")
					end
				end  
			end
		end
	end
end  

---------------------------------------------------------------------------------
-- World Event Functions
---------------------------------------------------------------------------------
function BroadcastWorldEvent( event, sender, szEntityType )
	local entityType = CLuaHashString:new_local(szEntityType)
	local eventName = CLuaHashString:new_local(event)
	local senderName = CLuaHashString:new_local(sender)

	local msg = BROADCASTWORLDEVENTPARAMS:new_local()
	msg.pEntityName = senderName
	msg.pEntityType = entityType
	msg.pEventName = eventName
	
	ToolBox:SendMessage( msgBroadcastWorldEvent, msg:SIZEOF(), msg, nil )
end

---------------------------------------------------------------------------------
-- GetPropertyTable
---------------------------------------------------------------------------------
function GetPropertyTable()
	local hszPropertiesTableName = CLuaHashString:new_local(CLuaManager:GetScriptObjectPropertyTable())
	local szPropertiesTableName = hszPropertiesTableName:GetString()
--	OutputDebug("GetPropertyTable: "..szPropertiesTableName.."\n")
	return _G[szPropertiesTableName]
end

-------------------------------------------------------  
-- GetRealTime
-------------------------------------------------------  
function GetRealTime()
	return Timer:GetRealTime()
end

---------------------------------------------------------------------------------
-- Gets the time delta, returns a lua number
---------------------------------------------------------------------------------
function GetTimeDelta(szDeltaName)
	local hsDeltaName = nil
	if szDeltaName ~= nil then
		hsDeltaName = CLuaHashString:new_local(szDeltaName)		
	end
	return Timer:GetTimeDelta(hsDeltaName)
end

-------------------------------------------------------  
-- PauseTime
-------------------------------------------------------  
function PauseTime(szDeltaTimerName)
	local hsDeltaName = nil
	if szDeltaTimerName ~= nil then
		hsDeltaName = CLuaHashString:new_local(szDeltaTimerName)		
	end
	Timer:PauseTime(hsDeltaName)
end

-------------------------------------------------------  
-- UnpauseTime
-------------------------------------------------------  
function UnpauseTime(szDeltaTimerName)
	local hsDeltaName = nil
	if szDeltaTimerName ~= nil then
		hsDeltaName = CLuaHashString:new_local(szDeltaTimerName)		
	end
	Timer:UnpauseTime(hsDeltaName)
end

-------------------------------------------------------  
-- Registers action handlers
-------------------------------------------------------  
function RegisterLuaActions(actionHandlerList)
	ToolBox:SendMessage(msgRegisterLuaActions, actionHandlerList:SIZEOF(), actionHandlerList)
end


---------------------------------------------------------------------------------
-- Inherites a class from a base class
--
-- Example:
--
-- SubClass = InheritsFrom( BaseClass )
-- local subclassInstance = SubClass:create()
---------------------------------------------------------------------------------
function InheritFrom( baseClass )

	local newClass = {}
	local class_mt = { __index = newClass }

	if baseClass ~= nil then
		setmetatable( newClass, { __index = baseClass } )
	end

	function newClass:create()
		local newInstance = {}
		setmetatable( newInstance, class_mt )
		return newInstance
	end
	
	-- Return the class object of the instance
	function newClass:class()
		return newClass
	end

	-- Return the super class object of the instance
	function newClass:superClass()
		return baseClass
	end

	-- Return true if the caller is an instance of theClass
	function newClass:IsA( theClass )
		local bIsA = false
		local cur_class = newClass

		while ( cur_class ~= nil ) and ( bIsA == false ) do
			if cur_class == theClass then
				bIsA = true
			else
				cur_class = cur_class:superClass()
			end
		end

		return bIsA
	end

	return newClass
end


-------------------------------------------------------  
-- Enable (used by particles)
-------------------------------------------------------  
function Enable(hsObjectName)
	ToolBox:SendMessage(msgEnable, TODWORD(0), nil, hsObjectName)
end

-------------------------------------------------------  
-- Disable (used by particles)
-------------------------------------------------------  
function Disable(hsObjectName)
	ToolBox:SendMessage(msgDisable, TODWORD(0), nil, hsObjectName)
end

-------------------------------------------------------  
-- GetFileVersion
-------------------------------------------------------  
function GetFileVersion()
	local hsVersion = CLuaHashString:new_local()
	ToolBox:SendMessage(msgGetFileVersion, TODWORD(4), hsVersion)
	return hsVersion
end

-------------------------------------------------------  
-- SetFileVersion
-------------------------------------------------------  
function SetFileVersion(hsVersion)
	ToolBox:SendMessage(msgSetFileVersion, TODWORD(4), hsVersion)
end

-------------------------------------------------------  
-- SubmitTypeQuery
-------------------------------------------------------  
function SubmitTypeQuery(hsType)
	local objectList = CLuaObjectTypeQueryList:new_local()
	local typeQuery = GETOBJECTTYPEQUERYPARAMS:new_local()
	typeQuery.type = hsType
	typeQuery.pObjectsList = tolua.cast(objectList, "list<IHashString*>")
	ToolBox:SendMessage(msgSubmitTypeQuery, typeQuery:SIZEOF(), typeQuery)
	return objectList
end

-------------------------------------------------------  
-- SubmitQuery
-------------------------------------------------------  
function SubmitQuery(vPosition, fRadius, szType)
	local objectList = CLuaObjDistQueryList:new_local()
	local queryParams = GETOBJECTQUERYPARAMS:new_local()
	queryParams.vPosition = vPosition
	queryParams.fRadius = fRadius
	if szType ~= nil then
		queryParams.hszType = CLuaHashString:new_local(szType)
	end
	queryParams.pObjectsList = tolua.cast(objectList, "list<OBJECTDISTANCEQUERY>")
	ToolBox:SendMessage(msgSubmitQuery, queryParams:SIZEOF(), queryParams)
	return objectList
end

-------------------------------------------------------  
-- ClearHierarchy
-------------------------------------------------------  
function ClearHierarchy()
	ToolBox:SendMessage(msgClearHierarchy, TODWORD(0), nil)
end

-------------------------------------------------------  
-- TransformWorldToScreen
-------------------------------------------------------  
function TransformWorldToScreen(vPos)
	local retTable = {}
	local w2sp = WORLDTOSCREENCOORDPARAMS:new_local()
	w2sp.worldXYZ = CLuaVec3:new_local( vPos.x, vPos.y, vPos.z )
	ToolBox:SendMessage(msgTransformWorldToScreen, w2sp:SIZEOF(), w2sp)	
	retTable.x = w2sp.screenX
	retTable.y = w2sp.screenY
	retTable.z = w2sp.screenZ
	return retTable
end

-------------------------------------------------------  
-- GetDBFileName
-------------------------------------------------------  
function GetDBFileName(hsObjectName)
	local hsFileName = CLuaHashString:new_local()
	ToolBox:SendMessage(msgGetDBFileName, TODWORD(4), hsFileName, hsObjectName)
	return hsFileName
end


-------------------------------------------------------  
-- GetGlobalStaticItem
-------------------------------------------------------  
function GetGlobalStaticItem( szKey )
	return m_GlobalStaticMap[szKey]
end

-------------------------------------------------------  
-- SetGlobalStaticItem
-------------------------------------------------------  
function SetGlobalStaticItem( szKey, value )
	m_GlobalStaticMap[szKey] = value
end

-------------------------------------------------------  
-- SetPropertyTableValue
-------------------------------------------------------  
function SetPropertyTableValue(hsObjectName, szKey, szValue, szType)
	local hsKey = CLuaHashString:new_local(szKey)
	local hsValue = CLuaHashString:new_local(szValue)
	local hsType = CLuaHashString:new_local(szType)
	
	local stvp = SETTABLEVALUEPARAMS:new_local()
	stvp.TableKey = hsKey
	stvp.TableValue = hsValue
	stvp.TableType = hsType
	
	ToolBox:SendMessage(msgSetPropertyTableValue, stvp:SIZEOF(), stvp, hsObjectName)
end

-------------------------------------------------------  
-- LoadingUpdate
-------------------------------------------------------  
function LoadingUpdate(iCurrent, iTotal)
	local lup = LOADINGUPDATEPARAMS:new_local()
	lup.currentPosition = TODWORD(iCurrent)
	lup.totalSize = TODWORD(iTotal)
	
	ToolBox:SendMessage(msgLoadingUpdate, lup:SIZEOF(), lup)
end

-------------------------------------------------------  
-- AttachSceneObject
-------------------------------------------------------  
function AttachSceneObject(hsBaseObject, hsAttachmentObject, hsNodeName)
	local asop = ATTACHSCENEOBJECTPARAMS:new_local()
	asop.hsAttachmentObject = hsAttachmentObject
	if hsNodeName then
		asop.hsNodeAttachName = hsNodeName
	end	
	ToolBox:SendMessage(msgAttachSceneObject, asop:SIZEOF(), asop, hsBaseObject)
end

-------------------------------------------------------  
-- DetachSceneObject
-------------------------------------------------------  
function DetachSceneObject(hsAttachmentObject)
	ToolBox:SendMessage(msgDetachSceneObject, TODWORD(4), hsAttachmentObject)
end

-------------------------------------------------------  
-- AddDecalSprite
-------------------------------------------------------  
function AddDecalSprite(szTextureName, vPos, vNormal, fSize, fHoldTime, fFadeTime, iColor)
	local hsTexture = CLuaHashString:new_local(szTextureName)
	local adsm = ADDDECALSPRITEMESSAGE:new_local()
	adsm.m_Size = fSize
	adsm.m_hsTextureName = hsTexture
	adsm.m_Position = tolua.cast(vPos, "Vec3")
	adsm.m_Normal = tolua.cast(vNormal, "Vec3")
	if fHoldTime ~= nil then
		adsm.m_TimeLinger = fHoldTime
	end
	if fFadeTime ~= nil then
		adsm.m_FadeTime = fFadeTime
	end
	if iColor ~= nil then
		adsm.m_Color = TODWORD(iColor)
	end
	
	ToolBox:SendMessage(msgAddDecalSprite, adsm:SIZEOF(), adsm)
end

-------------------------------------------------------  
-- GetComponentType
-------------------------------------------------------  
function GetComponentType(hsObjName)
	local gctp = GETCOMPONENTTYPEPARAMS:new_local()
	gctp.name = hsObjName
	
	ToolBox:SendMessage(msgGetComponentType, gctp:SIZEOF(), gctp)
	
	if gctp.componenttype ~= nil then
		local hsCompType = CLuaHashString:new_local(gctp.componenttype)
		return hsCompType
	end
	
	return nil
end

-------------------------------------------------------  
-- CreateTerrainDecal
-------------------------------------------------------  
function CreateTerrainDecal(szTexture, vPosition, fSize, fLingerTime, fFadeTime, v4Color)
	local tdp = TERRAINDECALPARAMS:new_local()
	local hsTexture = CLuaHashString:new_local(szTexture)
	tdp.hsTextureName = hsTexture
	tdp.vPosition = tolua.cast(vPosition, "Vec3")
	tdp.fSize = fSize
	
	if fLingerTime ~= nil then
		tdp.fLingerTime = fLingerTime
	end
	
	if fFadeTime ~= nil then
		tdp.fFadeTime = fFadeTime
	end
	
	if v4Color ~= nil then
		tdp.v4Color = v4Color
	end
	
	ToolBox:SendMessage(msgCreateTerrainDecal, tdp:SIZEOF(), tdp)
end

-------------------------------------------------------  
-- GetEntityQueryRadialWeighted
-------------------------------------------------------  
function GetEntityQueryRadialWeighted(vPosition, szEntityType, fRadius, fConeHalfAngle, eOrientation)
	local ret = {}
	
	local hsType = CLuaHashString:new_local(szEntityType)
	local list = SubmitTypeQuery(hsType)
	
	local listIndex = 1
	local listIter = list:GetFirst()
	for i=1, list:GetSize() do
		local hsName = CLuaHashString:new_local(listIter)
		local vecToSpot = GetGlobalPosition(hsName)
		LuaMathLib:Vec3SubtractEquals(vecToSpot, vPosition)
		-- zero out height
		vecToSpot:SetY(0)
		if LuaMathLib:Vec3Length(vecToSpot) <= fRadius then
			if fConeHalfAngle == nil or eOrientation == nil then
				local entry = {}
				entry.name = hsName:GetString()
				-- weight here is 1.0 to 0.0, where 0.0 is the very edge of the radius
				entry.weight = (fRadius - LuaMathLib:Vec3Length(vecToSpot)) / fRadius
				ret[listIndex] = entry
				listIndex = listIndex + 1
			else
				local eulerToSpot = LuaMathLib:VectorToEuler(vecToSpot)
				local yawToSpot = eulerToSpot:GetYaw()
				local baseYaw = eOrientation:GetYaw()
				
				if yawToSpot < baseYaw - fConeHalfAngle then
					yawToSpot = yawToSpot + 360
				elseif yawToSpot > baseYaw + fConeHalfAngle then
					yawToSpot = yawToSpot - 360
				end
				
				if yawToSpot >= baseYaw - fConeHalfAngle and yawToSpot <= baseYaw + fConeHalfAngle then
					local entry = {}
					entry.name = hsName:GetString()
					-- weight here is 1.0 to 0.0, where 0.0 is the very edge of the radius
					entry.weight = (fRadius - LuaMathLib:Vec3Length(vecToSpot)) / fRadius
					ret[listIndex] = entry
					listIndex = listIndex + 1
				end
				
			end
		end
		
		listIter = list:GetNext()
	end
		
	return ret
end

-------------------------------------------------------  
-- Vec3ToString
-------------------------------------------------------  
function Vec3ToString(vec3)
	local szRet = string.format("X: %f Y: %f Z: %f", vec3.x, vec3.y, vec3.z)
	return szRet
end

-------------------------------------------------------  
-- EulerToString
-------------------------------------------------------  
function EulerToString(euler)
	local szRet = string.format("Yaw: %f Pitch: %f Roll: %f", euler.yaw, euler.pitch, euler.roll)
	return szRet
end

-------------------------------------------------------  
-- SplitString
-------------------------------------------------------  
function SplitString( inText, inSplitPattern, outResults )
	if not outResults then
		outResults = { }
	end
	local theStart = 1
	local theSplitStart, theSplitEnd = string.find( inText, inSplitPattern, theStart )
	while theSplitStart do
		table.insert( outResults, string.sub( inText, theStart, theSplitStart-1 ) )
		theStart = theSplitEnd + 1
		theSplitStart, theSplitEnd = string.find( inText, inSplitPattern, theStart )
	end
	table.insert( outResults, string.sub( inText, theStart ) )
	return outResults
end

-------------------------------------------------------  
-- SetLength
-------------------------------------------------------  
function SetLength(hsObjName, fLength)
	ToolBox:SendMessage( msgSetLength, FLOAT_SIZEOF(), TOFLOAT(fLength), hsObjName)
end

-------------------------------------------------------  
-- WrapActionHandlers
-- Builds functions to specific object instances and associates their corresponding action handlers to aActionHandlerList
-------------------------------------------------------  
function WrapActionHandlers( aActionHandlerTable, aFunctionWrapper, aObjectInstance, aActionHandlerList )
	if aActionHandlerTable ~= nil and aObjectInstance ~= nil and aActionHandlerList ~= nil then
		if aFunctionWrapper == nil then
			aFunctionWrapper = {}
		end
		for key, value in pairs( aActionHandlerTable ) do
			--OutputDebug( CLuaManager:GetScriptObjectParentName():GetString().." registering function: '"..value.."'\n" )
			aFunctionWrapper[ value ] = function( archiveIn )
				if aObjectInstance[ value ] == nil then
					OutputDebug( LOGWARNING, CLuaManager:GetScriptObjectParentName():GetString().." cannot execute nil function: '"..value.."'\n" )
				else
					aObjectInstance[ value ]( aObjectInstance, archiveIn )
				end
			end
			aActionHandlerList:add( key, aFunctionWrapper[ value ] )
		end
	end
end

-------------------------------------------------------  
-- SetColorCorrection
-------------------------------------------------------  
function SetColorCorrection(fGamma, fBrightness, fContrast)
	local ccp = COLORCORRECTIONPARAMS:new_local()
	
	if fGamma ~= nil then
		ccp.fGamma = fGamma
	end
	
	if fBrightness ~= nil then
		ccp.fBrightness = fBrightness
	end
	
	if fContrast ~= nil then
		ccp.fContrast = fContrast
	end
	
	ToolBox:SendMessage(msgSetColorCorrection, ccp:SIZEOF(), ccp)
end

-------------------------------------------------------  
-- SwapJoystickDeviceMapping
-------------------------------------------------------  
function SwapJoystickDeviceMapping(iOldIndex, iNewIndex)
	local jdmp = JOYSTICKDEVICEMAPPARAMS:new_local()
	jdmp.iOldDeviceID = iOldIndex
	jdmp.iNewDeviceID = iNewIndex
	
	ToolBox:SendMessage(msgSwapJoystickDeviceMapping, jdmp:SIZEOF(), jdmp)
end

-------------------------------------------------------  
-- GetJoystickDeviceMapping
-------------------------------------------------------  
function GetJoystickDeviceMapping(iHardwareIndex)
	local jdmp = JOYSTICKDEVICEMAPPARAMS:new_local()
	jdmp.iOldDeviceID = iHardwareIndex	
	
	ToolBox:SendMessage(msgGetJoystickDeviceMapping, jdmp:SIZEOF(), jdmp)
	
	return jdmp.iNewDeviceID
end

-------------------------------------------------------  
-- GetVideoSize
-------------------------------------------------------  
function GetVideoSize()
	local ret = {}
	local vpp = VIEWPORTPARAMS:new_local()
	
	ToolBox:SendMessage(msgGetVideoSize, vpp:SIZEOF(), vpp)
	ret.width = vpp.width
	ret.height = vpp.height
	
	return ret
end

-------------------------------------------------------  
-- SetViewPort
-------------------------------------------------------  
function SetViewPort(iXOffset, iYOffset, iWidth, iHeight)
	local vpp = VIEWPORTPARAMS:new_local()
	vpp.width = iWidth
	vpp.height = iHeight
	vpp.offsetx = iXOffset
	vpp.offsety = iYOffset
	
	ToolBox:SendMessage(msgSetViewPort, vpp:SIZEOF(), vpp)
end

-------------------------------------------------------  
-- QueryJoystickFault
-------------------------------------------------------  
function QueryJoystickFault(iJoystick, iButton)
	local jfqp = JOYSTICKFAULTQUERYPARAMS:new_local()
	jfqp.iJoystick = iJoystick
	jfqp.iInput = iButton
	
	ToolBox:SendMessage(msgQueryJoystickFault, jfqp:SIZEOF(), jfqp)
	
	return jfqp.out_bFault
end

-------------------------------------------------------  
-- GetJoystickCount
-------------------------------------------------------  
function GetJoystickCount()
	local count = TODWORD(0)
	ToolBox:SendMessage(msgGetJoystickCount, TODWORD(4), count)
	return FROMDWORD(count)
end

---------------------------------------------------------------------------------
-- Add a precache file
---------------------------------------------------------------------------------
function AddPrecache( fileName )
	local hsFileName = CLuaHashString:new_local(fileName)	
	m_ToolBox:SendMessage(msgAddPrecache, TODWORD(4), hsFileName)		
end

---------------------------------------------------------------------------------
-- Remove a precache file
---------------------------------------------------------------------------------
function RemovePrecache( fileName )
	local hsFileName = CLuaHashString:new_local(fileName)	
	m_ToolBox:SendMessage(msgRemovePrecache, TODWORD(4), hsFileName)		
end

---------------------------------------------------------------------------------
-- Commit all precache changes
---------------------------------------------------------------------------------
function CommitPrecache( )
	m_ToolBox:SendMessage(msgCommitPrecache, TODWORD(0), nil)		
end

---------------------------------------------------------------------------------
-- InterpolateVector
---------------------------------------------------------------------------------
function InterpolateVector( vCurrentPos, vIntendedPos, fRate, fTimeDelta )
	local vDisplacement = LuaMathLib:Vec3( vIntendedPos )
	LuaMathLib:Vec3SubtractEquals( vDisplacement, vCurrentPos )
	local intendedDisplacement = LuaMathLib:Vec3Length(vDisplacement)
	local maxDisplacement = fTimeDelta * fRate
	
	if maxDisplacement < intendedDisplacement then
		-- LuaMathLib:Vec3SetLength( vDisplacement, maxDisplacement )
		LuaMathLib:Vec3DivideEquals( vDisplacement, intendedDisplacement )
		LuaMathLib:Vec3TimesEquals( vDisplacement, maxDisplacement )
		LuaMathLib:Vec3AddEquals( vCurrentPos, vDisplacement )
	else
		vCurrentPos:Set( vIntendedPos )
	end
end