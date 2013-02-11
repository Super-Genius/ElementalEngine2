---------------------------------------------------------------------------------
-- utl_gui.lua
-- Script File for gui utility funcs
---------------------------------------------------------------------------------

local m_ToolBox = CLuaToolBox:GetLuaToolBox();

m_ToolBox:LoadLuaDependency("scripts\\utilityfunctions.lua")

-- global message names
msgAddGUIElementToPage			= CLuaHashString:new_local("AddGUIElementToPage")
msgRemoveGUIElementFromPage		= CLuaHashString:new_local("RemoveGUIElementFromPage")
msgSetPosition					= CLuaHashString:new_local("SetPosition")
msgSetAttachOffset				= CLuaHashString:new_local("SetAttachOffset")
msgSetAlpha						= CLuaHashString:new_local("SetAlpha")
msgUpdateStatusBarPos			= CLuaHashString:new_local("UpdateStatusBarPos")
msgGUIManagerSetActivePage		= CLuaHashString:new_local("GUIManagerSetActivePage")
msgSetText						= CLuaHashString:new_local("SetText")
msgSetStaticImage				= CLuaHashString:new_local("SetStaticImage")
msgStartGUIAnimation			= CLuaHashString:new_local("StartGUIAnimation")
msgGetPosition					= CLuaHashString:new_local("GetPosition")
msgGetSize						= CLuaHashString:new_local("GetSize")
msgSetSize						= CLuaHashString:new_local("SetSize")
msgSetColor						= CLuaHashString:new_local("SetColor")
msgStopGUIAnimation				= CLuaHashString:new_local("StopGUIAnimation")
msgSetAngle						= CLuaHashString:new_local("SetAngle")
msgShowGroup					= CLuaHashString:new_local("ShowGroup")
msgHideGroup					= CLuaHashString:new_local("HideGroup")
msgSetCursorVisible				= CLuaHashString:new_local("SetCursorVisible")
msgSetTextFile					= CLuaHashString:new_local("SetTextFile")
msgAddGUIAnimationCallback		= CLuaHashString:new_local("AddGUIAnimationCallback")
msgSetTextColor					= CLuaHashString:new_local("SetTextColor")
msgSelectBtPressed				= CLuaHashString:new_local("SelectBtPressed")
msgBackBtPressed				= CLuaHashString:new_local("BackBtPressed")
msgToggleUpBtPressed			= CLuaHashString:new_local("ToggleUpBtPressed")
msgToggleDownBtPressed			= CLuaHashString:new_local("ToggleDownBtPressed")
msgGetZOrder					= CLuaHashString:new_local("GetZOrder")
msgSetZOrder					= CLuaHashString:new_local("SetZOrder")
msgGetItemText					= CLuaHashString:new_local("GetItemText")
msgSetItemText					= CLuaHashString:new_local("SetItemText")
msgGetGUIScreenSize				= CLuaHashString:new_local("GetGUIScreenSize")
msgSetSpinnerMaxValue			= CLuaHashString:new_local("SetSpinnerMaxValue")
msgSetSpinnerMinValue			= CLuaHashString:new_local("SetSpinnerMinValue")
msgStartGUIMovie				= CLuaHashString:new_local("StartGUIMovie")
msgStopGUIMovie					= CLuaHashString:new_local("StopGUIMovie")
msgSetActiveGUILanguage			= CLuaHashString:new_local("SetActiveGUILanguage")
msgUpdateLanguageEntryParams	= CLuaHashString:new_local("UpdateLanguageEntryParams")
msgChangeLanguageEntryKey		= CLuaHashString:new_local("ChangeLanguageEntryKey")

-- global types
hsPageType = CLuaHashString:new_local("CGUIPage")
hsInstanceType = CLuaHashString:new_local("CGUIInstance")

---------------------------------------------------------------------------------
-- Adds a gui element to a page
---------------------------------------------------------------------------------
function AddGUIElementToPage(szPageName, szInstanceName)
	local hsPageName = CLuaHashString:new_local(szPageName)
	local hsInstanceName = CLuaHashString:new_local(szInstanceName)
	
	local gpm = GUIPAGEMESSAGE:new_local()
	gpm.name = hsInstanceName
	gpm.compType = hsInstanceType
	local hszPageType = CLuaHashString:new_local("CGUIPage")
	m_ToolBox:SendMessage(msgAddGUIElementToPage, gpm:SIZEOF(), gpm, hsPageName, hsPageType)
end

---------------------------------------------------------------------------------
-- Removes a gui element from a page
---------------------------------------------------------------------------------
function RemoveGUIElementFromPage(szPageName, szInstanceName)
	local hsPageName = CLuaHashString:new_local(szPageName)
	local hsInstanceName = CLuaHashString:new_local(szInstanceName)
	
	local gpm = GUIPAGEMESSAGE:new_local()		
	gpm.name = hsInstanceName
	gpm.compType = hsInstanceType
	local hszPageType = CLuaHashString:new_local("CGUIPage")
	m_ToolBox:SendMessage(msgRemoveGUIElementFromPage, gpm:SIZEOF(), gpm, hsPageName, hsPageType)
end

---------------------------------------------------------------------------------
-- Sets the 2D position of an instance 
-- NOTE: can also set position on a group!
---------------------------------------------------------------------------------
function SetPosition(szObjName, szObjType, fXPos, fYPos)
	local hsObjName = CLuaHashString:new_local(szObjName)
	local hsObjType = CLuaHashString:new_local(szObjType)
	local pd = POSITIONDATA:new_local()
	pd.m_fXPos = fXPos
	pd.m_fYPos = fYPos
	m_ToolBox:SendMessage(msgSetPosition, pd:SIZEOF(), pd, hsObjName, hsObjType)
end

---------------------------------------------------------------------------------
-- Sets the 2D offset from an attach object
---------------------------------------------------------------------------------
function SetAttachOffset(szObjName, szObjType, fXOffset, fYOffset)
	local hsObjName = CLuaHashString:new_local(szObjName)
	local hsObjType = CLuaHashString:new_local(szObjType)
	local pd = POSITIONDATA:new_local()
	pd.m_fXPos = fXOffset
	pd.m_fYPos = fYOffset
	m_ToolBox:SendMessage(msgSetAttachOffset, pd:SIZEOF(), pd, hsObjName, hsObjType)
end

---------------------------------------------------------------------------------
-- Sets the alpha val of an instance (alpha = 0-255)
---------------------------------------------------------------------------------
function SetAlpha(szInstanceName, iAlpha)
	local hsInstanceName = CLuaHashString:new_local(szInstanceName)
	m_ToolBox:SendMessage(msgSetAlpha, TODWORD(4), TODWORD(iAlpha), hsInstanceName, hsInstanceType)
end

---------------------------------------------------------------------------------
-- Updates the status bar position
---------------------------------------------------------------------------------
function UpdateStatusBarPos(szBarName, iTotalSize, iPercentage)
	local hsBarName = CLuaHashString:new_local(szBarName)
	local sbType = CLuaHashString:new_local("CGUIStatusBar")
	
	local pbp = LOADINGUPDATEPARAMS:new_local()
	pbp.totalSize = TODWORD(iTotalSize)
	pbp.currentPosition = TODWORD(iPercentage)
	
	m_ToolBox:SendMessage(msgUpdateStatusBarPos, pbp:SIZEOF(), pbp, hsBarName, sbType)
end

---------------------------------------------------------------------------------
-- Sets the active page
---------------------------------------------------------------------------------
function SetActivePage(szPageName)
	local hsPageName = nil
	if szPageName ~= nil then
		if szPageName ~= "" then
			hsPageName = CLuaHashString:new_local(szPageName)
		end
	end
	
	-- nil for a page name is ok; page will be blank
	m_ToolBox:SendMessage(msgGUIManagerSetActivePage, TODWORD(4), hsPageName)
end

---------------------------------------------------------------------------------
-- Sets text of a static text object
---------------------------------------------------------------------------------
function SetText(szTextObject, szText)
	local hsTextObject = CLuaHashString:new_local(szTextObject)
	local hsTextType = CLuaHashString:new_local("CGUIStaticText")
	local hsText = CLuaHashString:new_local(szText)
	
	local td = TEXTDATA:new_local()
	td.text = hsText
	m_ToolBox:SendMessage(msgSetText, td:SIZEOF(), td, hsTextObject, hsTextType)
end

---------------------------------------------------------------------------------
-- Sets the texture for a static image
---------------------------------------------------------------------------------
function SetStaticImage(szStaticImgObject, szImageFilename)
	local hsStaticImgObject = CLuaHashString:new_local(szStaticImgObject)
	local hsStaticImgType = CLuaHashString:new_local("CGUIStaticArt")
	local hsImageFilename = CLuaHashString:new_local(szImageFilename)
	
	m_ToolBox:SendMessage(msgSetStaticImage, TODWORD(4), hsImageFilename, hsStaticImgObject, hsStaticImgType)
end

---------------------------------------------------------------------------------
-- Starts playing a gui animation
---------------------------------------------------------------------------------
function StartGUIAnimation(szAnimation)
	local hsAnimationName = CLuaHashString:new_local(szAnimation)
	local hsAnimType = CLuaHashString:new_local("CGUIAnimation")
	
	m_ToolBox:SendMessage(msgStartGUIAnimation, TODWORD(0), nil, hsAnimationName, hsAnimType)
end

---------------------------------------------------------------------------------
-- Stops playing a gui animation
---------------------------------------------------------------------------------
function StopGUIAnimation(szAnimation)
	local hsAnimationName = CLuaHashString:new_local(szAnimation)
	local hsAnimType = CLuaHashString:new_local("CGUIAnimation")
	
	m_ToolBox:SendMessage(msgStopGUIAnimation, TODWORD(0), nil, hsAnimationName, hsAnimType)
end

---------------------------------------------------------------------------------
-- Returns a table with x and y coordinates
---------------------------------------------------------------------------------
function GetPosition(szInstanceName)
	local retTable = {}
	local hsInstanceName = CLuaHashString:new_local(szInstanceName)
	local pd = POSITIONDATA:new_local()
	
	m_ToolBox:SendMessage(msgGetPosition, pd:SIZEOF(), pd, hsInstanceName, hsInstanceType)
	retTable.x = pd.m_fXPos
	retTable.y = pd.m_fYPos
	
	return retTable
end

---------------------------------------------------------------------------------
-- GetSize
---------------------------------------------------------------------------------
function GetSize(szInstanceName)
	local retTable = {}
	
	local hsInstanceName = CLuaHashString:new_local(szInstanceName)
	local sd = SIZEDATA:new_local()	
	m_ToolBox:SendMessage(msgGetSize, sd:SIZEOF(), sd, hsInstanceName, hsInstanceType)
	
	retTable.width = sd.m_fXSize
	retTable.height = sd.m_fYSize
	
	return retTable
end

---------------------------------------------------------------------------------
-- Sets the size of an instance
---------------------------------------------------------------------------------
function SetSize(szInstanceName, fWidth, fHeight)
	local hsInstanceName = CLuaHashString:new_local(szInstanceName)
	local sd = SIZEDATA:new_local()
	sd.m_fXSize = fWidth
	sd.m_fYSize = fHeight
	
	m_ToolBox:SendMessage(msgSetSize, sd:SIZEOF(), sd, hsInstanceName, hsInstanceType)
end

---------------------------------------------------------------------------------
-- Sets the color of an instance (dword color, argb)
---------------------------------------------------------------------------------
function SetColor(szInstanceName, dwColor)
	local hsInstanceName = CLuaHashString:new_local(szInstanceName)
	
	m_ToolBox:SendMessage(msgSetColor, TODWORD(4), dwColor, hsInstanceName, hsInstanceType)
end

---------------------------------------------------------------------------------
-- Sets the angle of an instance
---------------------------------------------------------------------------------
function SetAngle(szInstanceName, fAngle)
	local hsInstanceName = CLuaHashString:new_local(szInstanceName)
	local floatAngle = TOFLOAT(fAngle)
	
	m_ToolBox:SendMessage(msgSetAngle, FLOAT_SIZEOF(), floatAngle, hsInstanceName, hsInstanceType)
end

---------------------------------------------------------------------------------
-- Shows a group
---------------------------------------------------------------------------------
function ShowGroup(szGroupName)
	local hsGroupName = CLuaHashString:new_local(szGroupName)
	local hsGroupType = CLuaHashString:new_local("CGUIGroup")

	m_ToolBox:SendMessage(msgShowGroup, TODWORD(0), nil, hsGroupName, hsGroupType)
end

---------------------------------------------------------------------------------
-- Hides a group
---------------------------------------------------------------------------------
function HideGroup(szGroupName)
	local hsGroupName = CLuaHashString:new_local(szGroupName)
	local hsGroupType = CLuaHashString:new_local("CGUIGroup")

	m_ToolBox:SendMessage(msgHideGroup, TODWORD(0), nil, hsGroupName, hsGroupType)
end

---------------------------------------------------------------------------------
-- Show/hide mouse cursor
---------------------------------------------------------------------------------
function SetCursorVisible(bVisible)
	-- NOTE: this is a global mouse cursor name... eventually we might have per page cursors...
	local hsCursorName = CLuaHashString:new_local("MouseCursor")
	local hsCursorType = CLuaHashString:new_local("CGUICursor")
	local cv = CURSORVISIBLE:new_local()
	cv.bVisible = bVisible
	
	m_ToolBox:SendMessage(msgSetCursorVisible, cv:SIZEOF(), cv, hsCursorName, hsCursorType)
end

---------------------------------------------------------------------------------
-- Sets a text file for a static text object
---------------------------------------------------------------------------------
function SetTextFile(szTextObject, szTextFile)
	local hsTextObject = CLuaHashString:new_local(szTextObject)
	local hsObjType = CLuaHashString:new_local("CGUIStaticText")
	local hsTextFile = CLuaHashString:new_local(szTextFile)
	
	m_ToolBox:SendMessage(msgSetTextFile, TODWORD(4), hsTextFile, hsTextObject, hsObjType)
end

---------------------------------------------------------------------------------
-- Register a debug pane
---------------------------------------------------------------------------------
function RegisterGUIDebugText( name, itextColor, ifontSize )
	local oldVersion = CLuaHashString:new_local()
	local currentVersion = CLuaHashString:new_local("2.0a")
	oldVersion = GetFileVersion()
	SetFileVersion(currentVersion)
	
	local instanceName = string.format("%s_DebugInstance", name)
	local hszInstName = CLuaHashString:new_local(instanceName)
	local hszInstType = CLuaHashString:new_local("CGUIInstance")

	local objectName = string.format("%s_DebugObject", name)
	local hszObjName = CLuaHashString:new_local(objectName)

	-- make static text object
	local staticTextArchive = CLuaArchive:new_local()
	staticTextArchive:InitArchive(3, 512,  hszMemoryType );
	
	staticTextArchive:WriteTCHARStr("basedata\\editor\\textures\\none.tga") -- NormTexture
	staticTextArchive:WriteTCHARStr("TESTING") -- StaticText
	staticTextArchive:WriteTCHARStr("") -- Filename
	staticTextArchive:WriteBool(false) -- LoadFromFile
	staticTextArchive:WriteTCHARStr("Fonts\\comic.ttf") --FontName
	staticTextArchive:WriteInt(ifontSize) -- Fontsize
	staticTextArchive:WriteInt(itextColor) -- NormFontColor
	staticTextArchive:WriteInt(itextColor) -- HighFontColor
	staticTextArchive:WriteInt(itextColor) -- SelectFontColor
	staticTextArchive:WriteInt(itextColor) -- GrayFontColor
	staticTextArchive:WriteInt(0) -- TextOffSetX
	staticTextArchive:WriteInt(0) -- TextOffSetY
	staticTextArchive:WriteBool(false) -- CenterText
	staticTextArchive:WriteBool(true) -- HideTexture

	local stextCompType = CLuaHashString:new_local("CGUIStaticText")

	-- create object
	CreateObject(hszObjName, stextCompType, nil)
	
	-- serialize object
	SerializeObject(hszObjName, staticTextArchive)
	
	-- init object
	InitObject(hszObjName)
	
	-- make instance
	local instanceArchive = CLuaArchive:new_local()
	instanceArchive:InitArchive(3, 512,  hszMemoryType );
	
	-- pos
	instanceArchive:WriteFloat(0.0)
	instanceArchive:WriteFloat(0.0)
	-- size
	instanceArchive:WriteFloat(512.0)
	instanceArchive:WriteFloat(512.0)
	-- z order
	instanceArchive:WriteInt(3)
	-- color 
	instanceArchive:WriteInt(-1)
	-- alpha
	instanceArchive:WriteInt(255) -- test! make 0
	-- angle
	instanceArchive:WriteFloat(0.0)
	-- instance info
	instanceArchive:WriteTCHARStr(objectName)
	instanceArchive:WriteTCHARStr("CGUIStaticText")
	
	-- create object
	CreateObject(hszInstName, hszInstType, nil)	
	
	-- serialize object
	SerializeObject(hszInstName, instanceArchive)

	-- init object
	InitObject(hszInstName)
	
	AddGUIElementToPage("Game HUD", instanceName)
	
	SetFileVersion(oldVersion)
end


---------------------------------------------------------------------------------
-- Deletes a debug text
---------------------------------------------------------------------------------
function DeRegisterGUIDebugText(name)
	local instName = string.format("%s_DebugInstance", name)
	local objName = string.format("%s_DebugPane", name)
	local hszInstName = CLuaHashString:new_local(instName)
	local hszObjName = CLuaHashString:new_local(objName)
	
	RemoveGUIElementFromPage("Game HUD", instName)
	
	-- delete instance
	DelayedDeleteObject(hszInstName)
	
	-- delete bar
	DelayedDeleteObject(hszObjName)
end

---------------------------------------------------------------------------------
-- Updates the position and text for a static text debug pane
---------------------------------------------------------------------------------
function UpdateGUIDebugText( name, x, y, text )
	local szInstanceName = string.format("%s_DebugInstance", name)
	SetPosition(szInstanceName, "CGUIInstance", x, y)

	local szObjectName = string.format("%s_DebugObject", name)
	SetText( szObjectName, text )
end

---------------------------------------------------------------------------------
-- Updates the position and text for a static text debug pane
---------------------------------------------------------------------------------
function AddGUIAnimationCallback(szAnimationName, szEventName, eventArchive)
	local hsAnimationName = CLuaHashString:new_local(szAnimationName)
	local hsAnimationType = CLuaHashString:new_local("CGUIAnimation")
	local hsEventName = CLuaHashString:new_local(szEventName)
	local ge = GUIEVENT:new_local()
	ge.event = hsEventName:GetUniqueID()
	ge.ar = tolua.cast(eventArchive, "IArchive")
	
	m_ToolBox:SendMessage(msgAddGUIAnimationCallback, ge:SIZEOF(), ge, hsAnimationName, hsAnimationType)
end

---------------------------------------------------------------------------------
-- SetTextColor
---------------------------------------------------------------------------------
function SetTextColor(szStaticTextObj, iColor)
	local hsObjName = CLuaHashString:new_local(szStaticTextObj)
	local hsObjType = CLuaHashString:new_local("CGUIStaticText")
	m_ToolBox:SendMessage(msgSetTextColor, TODWORD(4), TODWORD(iColor), hsObjName, hsObjType)
end

---------------------------------------------------------------------------------
-- SelectBtPressed
---------------------------------------------------------------------------------
function SelectBtPressed(szObject)
	local hsObject = CLuaHashString:new_local(szObject)
	local guiBool = GUIBOOL:new_local()
	guiBool.bVal = false
	m_ToolBox:SendMessage(msgSelectBtPressed, guiBool:SIZEOF(), guiBool, hsObject, hsInstanceType)
	
	return guiBool.bVal
end

---------------------------------------------------------------------------------
-- BackBtPressed
---------------------------------------------------------------------------------
function BackBtPressed(szObject)
	local hsObject = CLuaHashString:new_local(szObject)
	local guiBool = GUIBOOL:new_local()
	guiBool.bVal = false
	m_ToolBox:SendMessage(msgBackBtPressed, guiBool:SIZEOF(), guiBool, hsObject, hsInstanceType)
	
	return guiBool.bVal
end

---------------------------------------------------------------------------------
-- ToggleUpBtPressed
---------------------------------------------------------------------------------
function ToggleUpBtPressed(szObject)
	local hsObject = CLuaHashString:new_local(szObject)
	local guiBool = GUIBOOL:new_local()
	guiBool.bVal = false
	m_ToolBox:SendMessage(msgToggleUpBtPressed, guiBool:SIZEOF(), guiBool, hsObject, hsInstanceType)
	
	return guiBool.bVal
end

---------------------------------------------------------------------------------
-- ToggleDownBtPressed
---------------------------------------------------------------------------------
function ToggleDownBtPressed(szObject)
	local hsObject = CLuaHashString:new_local(szObject)
	local guiBool = GUIBOOL:new_local()
	guiBool.bVal = false
	m_ToolBox:SendMessage(msgToggleDownBtPressed, guiBool:SIZEOF(), guiBool, hsObject, hsInstanceType)
	
	return guiBool.bVal
end

---------------------------------------------------------------------------------
-- GetZOrder
---------------------------------------------------------------------------------
function GetZOrder(szObject)
	local hsObject = CLuaHashString:new_local(szObject)
	local ret = 0
	local zOrd = ZORDERDATA:new_local()
	m_ToolBox:SendMessage(msgGetZOrder, zOrd:SIZEOF(), zOrd, hsObject, hsInstanceType)
	
	ret = zOrd.m_iZOrder
	return ret
end

---------------------------------------------------------------------------------
-- SetZOrder
---------------------------------------------------------------------------------
function SetZOrder(szObject, iZOrder)
	local hsObject = CLuaHashString:new_local(szObject)
	local zOrd = ZORDERDATA:new_local()
	zOrd.m_iZOrder = iZOrder
	m_ToolBox:SendMessage(msgSetZOrder, zOrd:SIZEOF(), zOrd, hsObject, hsInstanceType)
end

---------------------------------------------------------------------------------
-- GetItemText
---------------------------------------------------------------------------------
function GetItemText(szObject)
	local ret = ""
	local hsObject = CLuaHashString:new_local(szObject)
	local hsText = CLuaHashString:new_local("")
	
	m_ToolBox:SendMessage(msgGetItemText, TODWORD(4), hsText, hsObject, hsInstanceType)
	
	ret = hsText:GetString()	
	return ret
end

---------------------------------------------------------------------------------
-- SetItemText
---------------------------------------------------------------------------------
function SetItemText(szObject, szText)
	local hsObject = CLuaHashString:new_local(szObject)
	local hsText = CLuaHashString:new_local(szText)
	
	m_ToolBox:SendMessage(msgSetItemText, TODWORD(4), hsText, hsObject, hsInstanceType)
end

---------------------------------------------------------------------------------
-- GetGUIScreenSize
---------------------------------------------------------------------------------
function GetGUIScreenSize()
	local ret = {}
	local vpp = VIEWPORTPARAMS:new_local()
	m_ToolBox:SendMessage(msgGetGUIScreenSize, vpp:SIZEOF(), vpp)
	ret.width = vpp.width
	ret.height = vpp.height
	
	return ret
end

---------------------------------------------------------------------------------
-- SetSpinnerMaxValue
---------------------------------------------------------------------------------
function SetSpinnerMaxValue(szObject, fVal)
	local hsObject = CLuaHashString:new_local(szObject)
	local hsType = CLuaHashString:new_local("CGUISpinnerItem")
	local floatVal = TOFLOAT(fVal)	
	m_ToolBox:SendMessage(msgSetSpinnerMaxValue, FLOAT_SIZEOF(), floatVal, hsObject, hsType)
end

---------------------------------------------------------------------------------
-- SetSpinnerMinValue
---------------------------------------------------------------------------------
function SetSpinnerMinValue(szObject, fVal)
	local hsObject = CLuaHashString:new_local(szObject)
	local hsType = CLuaHashString:new_local("CGUISpinnerItem")
	local floatVal = TOFLOAT(fVal)	
	m_ToolBox:SendMessage(msgSetSpinnerMinValue, FLOAT_SIZEOF(), floatVal, hsObject, hsType)
end

---------------------------------------------------------------------------------
-- StartGUIMovie
---------------------------------------------------------------------------------
function StartGUIMovie(szObject)
	local hsObject = CLuaHashString:new_local(szObject)
	local hsType = CLuaHashString:new_local("CGUIMovie")
	m_ToolBox:SendMessage(msgStartGUIMovie, TODWORD(0), nil, hsObject, hsType)
end

---------------------------------------------------------------------------------
-- StopGUIMovie
---------------------------------------------------------------------------------
function StopGUIMovie(szObject)
	local hsObject = CLuaHashString:new_local(szObject)
	local hsType = CLuaHashString:new_local("CGUIMovie")
	m_ToolBox:SendMessage(msgStopGUIMovie, TODWORD(0), nil, hsObject, hsType)
end

---------------------------------------------------------------------------------
-- SetActiveGUILanguage
---------------------------------------------------------------------------------
function SetActiveGUILanguage(szLanguage)
	local hszLanguage = CLuaHashString:new_local(szLanguage)
	m_ToolBox:SendMessage(msgSetActiveGUILanguage, TODWORD(4), hszLanguage)
end

---------------------------------------------------------------------------------
-- UpdateLanguageEntryParams
---------------------------------------------------------------------------------
function UpdateLanguageEntryParams(szObjName, archiveOut)
	local hszName = CLuaHashString:new_local(szObjName)
	local hszType = CLuaHashString:new_local("CGUIStaticText")
	m_ToolBox:SendMessage(msgUpdateLanguageEntryParams, TODWORD(4), archiveOut, hszName, hszType)
end

---------------------------------------------------------------------------------
-- ChangeLanguageEntryKey
---------------------------------------------------------------------------------
function ChangeLanguageEntryKey(szObjName, szKey, archiveOut)
	local hszName = CLuaHashString:new_local(szObjName)
	local hszType = CLuaHashString:new_local("CGUIStaticText")
	local hszKey = CLuaHashString:new_local(szKey)
	local clkp = CHANGELANGUAGEKEYPARAMS:new_local()
	clkp.hszKey = hszKey
	clkp.archive = tolua.cast(archiveOut, "IArchive")
	m_ToolBox:SendMessage(msgChangeLanguageEntryKey, clkp:SIZEOF(), clkp, hszName, hszType)
end

---------------------------------------------------------------------------------
-- SecondsToMinutesSecondsString
---------------------------------------------------------------------------------
function SecondsToMinutesSecondsString(iSeconds)
	local szTime = ""
	if iSeconds < 10 then
		szTime = "0:0"..iSeconds
	else
		local minutes = math.floor(iSeconds / 60.0)
		local seconds = iSeconds - (minutes * 60.0)
		local szSeconds = ""..seconds
		if seconds < 10 then
			szSeconds = "0"..seconds
		end
		
		szTime = ""..minutes..":"..szSeconds
	end
	
	return szTime
end