---------------------------------------------------------------------------------
-- utl_Material.lua
-- Utils for changing material stuffs
---------------------------------------------------------------------------------

m_ToolBox = CLuaToolBox:GetLuaToolBox()
-- global messages
msgSetMaterialOverride		= CLuaHashString:new_local("SetMaterialOverride")
msgClearMaterialOverride	= CLuaHashString:new_local("ClearMaterialOverride")

---------------------------------------------------------------------------------
-- Fills the MOP struct
---------------------------------------------------------------------------------
local function GetMaterialStringOverrideStruct(szType, szName, szValue, szMatName, szNodeName)
	local mop = MATERIALOVERRIDEPARAMS:new_local()
	
	local hsType = CLuaHashString:new_local(szType)
	local hsName = CLuaHashString:new_local(szName)
	local hsValue = CLuaHashString:new_local(szValue)
	
	local hsMatName = nil
	if szMatName ~= nil then
		if szMatName ~= "" then
			hsMatName = CLuaHashString:new_local(szMatName)
		end
	end	
	
	local hsNodeName = nil
	if szNodeName ~= nil then
		hsNodeName = CLuaHashString:new_local(szNodeName)
	end
	
	mop.hsType = hsType
	mop.hsName = hsName
	mop.hsValue = hsValue
	mop.hsMaterialName = hsMatName
	mop.hsNodeName = hsNodeName
	
	return mop
end

---------------------------------------------------------------------------------
-- Fills the MOP struct
---------------------------------------------------------------------------------
local function GetMaterialVec4OverrideStruct(szType, szName, vec4Value, szMatName, szNodeName)
	local mop = MATERIALOVERRIDEPARAMS:new_local()
	
	local hsType = CLuaHashString:new_local(szType)
	local hsName = CLuaHashString:new_local(szName)
	
	local hsMatName = nil
	if szMatName ~= nil then
		if szMatName ~= "" then
			hsMatName = CLuaHashString:new_local(szMatName)
		end
	end	
	
	local hsNodeName = nil
	if szNodeName ~= nil then
		hsNodeName = CLuaHashString:new_local(szNodeName)
	end
	
	mop.hsType = hsType
	mop.hsName = hsName
	mop.vecValue = tolua.cast(vec4Value, "Vec4")
	mop.hsMaterialName = hsMatName
	mop.hsNodeName = hsNodeName
	
	return mop
end

---------------------------------------------------------------------------------
-- SetC3DRenderStateOverride
---------------------------------------------------------------------------------
function SetRenderStateOverride(hsObjectName, szStateName, szValue, szMaterial, szNodeName, hsObjectType)
	local mop = GetMaterialStringOverrideStruct("renderstate", szStateName, szValue, szMaterial, szNodeName)
	m_ToolBox:SendMessage(msgSetMaterialOverride, mop:SIZEOF(), mop, hsObjectName, hsObjectType)
end

---------------------------------------------------------------------------------
-- SetC3DTextureOverride
---------------------------------------------------------------------------------
function SetTextureOverride(hsObjectName, iTextureIndex, szTexturePath, szMaterial, szNodeName, hsObjectType)
	local szTextureIndex = string.format("%d", iTextureIndex)
	local mop = GetMaterialStringOverrideStruct("texture", szTextureIndex, szTexturePath, szMaterial, szNodeName)
	m_ToolBox:SendMessage(msgSetMaterialOverride, mop:SIZEOF(), mop, hsObjectName, hsObjectType)
end

---------------------------------------------------------------------------------
-- SetC3DVertexConstantOverride
---------------------------------------------------------------------------------
function SetVertexConstantOverride(hsObjectName, szConstantName, fXVal, fYVal, fZVal, fWVal, szMaterial, szNodeName, hsObjectType)
	local vec4Val = CLuaVec4:new_local(fXVal, fYVal, fZVal, fWVal)
	local mop = GetMaterialVec4OverrideStruct("vertexconstant", szConstantName, vec4Val, szMaterial, szNodeName)
	m_ToolBox:SendMessage(msgSetMaterialOverride, mop:SIZEOF(), mop, hsObjectName, hsObjectType)
end

---------------------------------------------------------------------------------
-- SetC3DPixelConstantOverride
---------------------------------------------------------------------------------
function SetPixelConstantOverride(hsObjectName, szConstantName, fXVal, fYVal, fZVal, fWVal, szMaterial, szNodeName, hsObjectType)
	local vec4Val = CLuaVec4:new_local(fXVal, fYVal, fZVal, fWVal)
	local mop = GetMaterialVec4OverrideStruct("pixelconstant", szConstantName, vec4Val, szMaterial, szNodeName)
	m_ToolBox:SendMessage(msgSetMaterialOverride, mop:SIZEOF(), mop, hsObjectName, hsObjectType)
end

---------------------------------------------------------------------------------
-- ClearMaterialOverride
---------------------------------------------------------------------------------
function ClearMaterialOverride(hsObjectName, szMaterialName, szNodeName, hsObjectType)
	local hsMaterialName = nil	
	if szMaterialName ~= nil then
		hsMaterialName = CLuaHashString:new_local(szMaterialName)
	end
	
	local hsNodeName = nil
	if szNodeName ~= nil then
		hsNodeName = CLuaHashString:new_local(szNodeName)
	end
	
	local cmop = CLEARMATOVERRIDEPARAMS:new_local()
	
	cmop.hsMaterialName = hsMaterialName
	cmop.hsNodeName = hsNodeName
	
	m_ToolBox:SendMessage(msgClearMaterialOverride, cmop:SIZEOF(), cmop, hsObjectName, hsObjectType)
end