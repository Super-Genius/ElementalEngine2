---------------------------------------------------------------------------------
-- utl_Database.lua
-- place for spline messages
---------------------------------------------------------------------------------

m_ToolBox = CLuaToolBox:GetLuaToolBox()

-- global message names
msgGetAttribute					= CLuaHashString:new_local("GetAttribute")
msgGetGlobalAttribute			= CLuaHashString:new_local("GetGlobalAttribute")
msgGetAttributeType				= CLuaHashString:new_local("GetAttributeType")
msgGetDatabaseInfo				= CLuaHashString:new_local("GetDatabaseInfo")
msgGetTypedDatabase				= CLuaHashString:new_local("GetTypedDatabase")
msgGetGlobalAttributeType		= CLuaHashString:new_local("GetGlobalAttributeType")
msgSetAttribute					= CLuaHashString:new_local("SetAttribute")
msgSetGlobalAttribute			= CLuaHashString:new_local("SetGlobalAttribute")
msgGetIndex						= CLuaHashString:new_local("GetIndex")

DB = 
{ 
	write = {
		["char"] = 				function(attributeArchive, value)
									attributeArchive:WriteChar(value)
								end,
		["unsigned char"]= 		function(attributeArchive, value)
									attributeArchive:WriteChar(value)
								end,
		["short int"]= 			function(attributeArchive, value)
									attributeArchive:WriteShort(value)
								end,
		["unsigned short int"]= function(attributeArchive, value)
									attributeArchive:WriteShort(value)
								end,
		["long int"]= 			function(attributeArchive, value)
									attributeArchive:WriteInt(value) 
								end,
		["unsigned long int"] =	function(attributeArchive, value)
									attributeArchive:WriteUInt(value)
								end,
		["float"] = 			function(attributeArchive, value)
									attributeArchive:WriteFloat(value) 
								end,
		["double"] = 			function(attributeArchive, value)
									attributeArchive:WriteDouble(value) 
								end,
		["bool"] = 				function(attributeArchive, value)
									attributeArchive:WriteBool(value) 
								end,
		["string"] =			function(attributeArchive, value)
									attributeArchive:WriteTCHARStr(value)
								end
	},
	read = {
		["char"] = 				function(attributeArchive)
									return attributeArchive:ReadChar()
								end,
		["unsigned char"]= 		function(attributeArchive) 
									return attributeArchive:ReadChar()
								end,
		["short int"]= 			function(attributeArchive)
									return attributeArchive:ReadShort()
								end,
		["unsigned short int"]= function(attributeArchive) 
									return attributeArchive:ReadShort()
								end,
		["long int"] = 			function(attributeArchive) 
									return attributeArchive:ReadInt() 
								end,
		["unsigned long int"] = function(attributeArchive) 
									return attributeArchive:ReadUInt()
								end,
		["float"] =				function(attributeArchive) 
									return attributeArchive:ReadFloat() 
								end,
		["double"] =			function(attributeArchive) 
									return attributeArchive:ReadDouble() 
								end,
		["bool"] =				function(attributeArchive) 
									return attributeArchive:ReadBool() 
								end,
		["string"] =			function(attributeArchive) 
									local tempString = StdString:new_local( )
									attributeArchive:ReadString(tempString)
									return CLuaHashString:new_local( tempString ) 
								end
	}
}

-------------------------------------------------------
-- LoadDBToTable
-------------------------------------------------------
function LoadDBToTable( dbFilepath )
	if dbFilepath ~= "" or table ~= nil then
		tDBTable = {}

		LoadFileByExt( dbFilepath, true )

		local archiveTable = CLuaArchive:new_local()
		archiveTable:InitArchive( 3, hszMemoryType )
		local hsDBFilepath = CLuaHashString:new_local( dbFilepath )
		local DBTypeInfo = TYPEDATABASEINFO:new_local()
		DBTypeInfo.phszFilepath = hsDBFilepath
		DBTypeInfo.pArchive = tolua.cast( archiveTable, "IArchive" )
		ToolBox:SendMessage( msgGetTypedDatabase, DBTypeInfo:SIZEOF(), DBTypeInfo )
		local nEntries 	= archiveTable:ReadInt()
		local nFields	= archiveTable:ReadInt()
		local stdName = StdString:new_local()
		local stdType = StdString:new_local()
		local szName = ""
		local szType = ""
		for i = 1, nEntries do
			tDBTable[i] = {}
			for j = 1, nFields do
				archiveTable:ReadString(stdType)
				archiveTable:ReadString(stdName)
				szName = CLuaHashString:new_local(stdName):GetString()
				szType = CLuaHashString:new_local(stdType):GetString()
	--			OutputDebug("Name: >"..szName.."< Type: >"..szType.."<\n")
				tDBTable[i][szName] = DB.read[szType](archiveTable)
	--			tDBTable[i][stdName:GetText()] = DB.Read[stdType:GetText()](archiveTable)
			end
		end
		
		return tDBTable
	end
	return nil
end

-------------------------------------------------------
-- DetermineTypeRead
-------------------------------------------------------
local function DetermineTypeRead( attributeTypeArchive, attributeArchive )
	local retVal = 0
	local tempString = StdString:new_local( )

	attributeTypeArchive:SetIsWriting( false )
	attributeTypeArchive:ReadString( tempString )
	local hszAttributeType = CLuaHashString:new_local( tempString )
	local szAttributeType = hszAttributeType:GetString()
	attributeArchive:SetIsWriting( false )

	if DB.read[szAttributeType] ~= nil then
		retVal = DB.read[szAttributeType](attributeArchive)
	else
		OutputDebug( LOGWARNING, "Attribute type '"..szAttributeType.."' not indexed for read.\n" )
	end

	return retVal
end

-------------------------------------------------------
-- DetermineTypeWrite
-------------------------------------------------------
local function DetermineTypeWrite( attributeTypeArchive, attributeArchive, attributeValue)
	local tempString = StdString:new_local()
	
	attributeTypeArchive:SetIsWriting( false )
	attributeTypeArchive:ReadString( tempString )
	local hszAttributeType = CLuaHashString:new_local( tempString )
	local szAttributeType = hszAttributeType:GetString()

	attributeArchive:SetIsWriting( true )

	if DB.write[szAttributeType] ~= nil then
		DB.write[szAttributeType](attributeArchive, attributeValue)
	else
		OutputDebug( LOGWARNING, "Attribute type '"..szAttributeType.."' not indexed for write.\n" )
	end

	attributeArchive:SetIsWriting( false )
end


-------------------------------------------------------
-- SendGetAttributeMessage
-------------------------------------------------------
local function SendGetAttributeMessage( iIndex, hsObjectName, attributeName, componentTypeName )
	-- TODO <rnguyen>: There may be significant overhead on instantiating archives
	--                 particularly if we want to loop a GetAttribute. Consider
	--                 caching table of object names to archive instances.

	-- Archive for the value
	local attributeArchive = CLuaArchive:new_local()
	attributeArchive:InitArchive( 3, hszMemoryType )

	-- Archive for the data type
	local attributeTypeArchive = CLuaArchive:new_local()
	attributeTypeArchive:InitArchive( 3, hszMemoryType )

	local databaseAttributeParams = DATABASEATTRIBUTEPARAMS:new_local()
	databaseAttributeParams.m_Index = iIndex
	databaseAttributeParams.m_AttributeName = CLuaHashString:new_local( attributeName )
	databaseAttributeParams.m_AttributeTypeArchive = tolua.cast( attributeTypeArchive, "IArchive" )
	databaseAttributeParams.m_AttributeArchive = tolua.cast( attributeArchive, "IArchive" )

	local hszComponentTypeName = CLuaHashString:new_local(componentTypeName)
	local retVal = nil
	local result = ToolBox:SendMessage( msgGetAttribute, databaseAttributeParams:SIZEOF(), databaseAttributeParams, hsObjectName, hszComponentTypeName )
	if result == 0 then
		OutputDebug( LOGWARNING, "Could not get attribute '"..attributeName.."' for object "..hsObjectName:GetString().."\n" )
	else
		retVal = DetermineTypeRead( attributeTypeArchive, attributeArchive ) 
	end

	return retVal
end

-------------------------------------------------------
-- SendSetAttributeMessage
-------------------------------------------------------
local function SendSetAttributeMessage( iIndex, hsObjectName, attributeName, attributeValue, componentTypeName )
	-- TODO <rnguyen>: There may be significant overhead on instantiating archives
	--                 particularly if we want to loop a GetAttribute. Consider
	--                 caching table of object names to archive instances.

	-- Archive for the value
	local attributeArchive = CLuaArchive:new_local()
	attributeArchive:InitArchive( 3, hszMemoryType )

	-- Archive for the data type
	local attributeTypeArchive = CLuaArchive:new_local()
	attributeTypeArchive:InitArchive( 3, hszMemoryType )

	local databaseAttributeParams = DATABASEATTRIBUTEPARAMS:new_local()
	databaseAttributeParams.m_Index = iIndex
	databaseAttributeParams.m_AttributeName = CLuaHashString:new_local( attributeName )
	databaseAttributeParams.m_AttributeTypeArchive = tolua.cast( attributeTypeArchive, "IArchive" )
	databaseAttributeParams.m_AttributeArchive = tolua.cast( attributeArchive, "IArchive" )

	local hszComponentTypeName = CLuaHashString:new_local(componentTypeName)
	-- Call GetAttributeType so we know the data type
	-- TODO <rnguyen>: Should create a map of attribute names to their types to prevent repeated message sending
	ToolBox:SendMessage( msgGetAttributeType, databaseAttributeParams:SIZEOF(), databaseAttributeParams, hsObjectName, hszComponentTypeName )

	DetermineTypeWrite( attributeTypeArchive, attributeArchive, attributeValue) 

	databaseAttributeParams.m_AttributeTypeArchive = nil
	databaseAttributeParams.m_AttributeArchive = tolua.cast( attributeArchive, "IArchive" )
	ToolBox:SendMessage( msgSetAttribute, databaseAttributeParams:SIZEOF(), databaseAttributeParams, hsObjectName, hszComponentTypeName )

	return retVal
end


-------------------------------------------------------
-- SendGetGlobalAttributeMessage
-------------------------------------------------------
local function SendGetGlobalAttributeMessage( attributeName )
	-- TODO <rnguyen>: There may be significant overhead on instantiating archives
	--                 particularly if we want to loop a GetAttribute. Consider
	--                 caching table of object names to archive instances.

	-- Archive for the value
	local attributeArchive = CLuaArchive:new_local()
	attributeArchive:InitArchive( 3, hszMemoryType )

	-- Archive for the data type
	local attributeTypeArchive = CLuaArchive:new_local()
	attributeTypeArchive:InitArchive( 3, hszMemoryType )

	local databaseAttributeParams = DATABASEATTRIBUTEPARAMS:new_local()
	databaseAttributeParams.m_AttributeName = CLuaHashString:new_local( attributeName )
	databaseAttributeParams.m_AttributeTypeArchive = tolua.cast( attributeTypeArchive, "IArchive" )
	databaseAttributeParams.m_AttributeArchive = tolua.cast( attributeArchive, "IArchive" )

	ToolBox:SendMessage( msgGetGlobalAttribute, databaseAttributeParams:SIZEOF(), databaseAttributeParams )

	local retVal = DetermineTypeRead( attributeTypeArchive, attributeArchive ) 

	return retVal
end


-------------------------------------------------------
-- SendSetGlobalAttributeMessage
-------------------------------------------------------
local function SendSetGlobalAttributeMessage( attributeName, attributeValue )
	-- TODO <rnguyen>: There may be significant overhead on instantiating archives
	--                 particularly if we want to loop a GetAttribute. Consider
	--                 caching table of object names to archive instances.

	-- Archive for the value
	local attributeArchive = CLuaArchive:new_local()
	attributeArchive:InitArchive( 3, hszMemoryType )

	-- Archive for the data type
	local attributeTypeArchive = CLuaArchive:new_local()
	attributeTypeArchive:InitArchive( 3, hszMemoryType )

	local databaseAttributeParams = DATABASEATTRIBUTEPARAMS:new_local()
	databaseAttributeParams.m_AttributeName = CLuaHashString:new_local( attributeName )
	databaseAttributeParams.m_AttributeTypeArchive = tolua.cast( attributeTypeArchive, "IArchive" )
	databaseAttributeParams.m_AttributeArchive = tolua.cast( attributeArchive, "IArchive" )

	-- Call GetAttributeType so we know the data type
	-- TODO <rnguyen>: Should create a map of attribute names to their types to prevent repeated message sending
	ToolBox:SendMessage( msgGetGlobalAttributeType, databaseAttributeParams:SIZEOF(), databaseAttributeParams )

	local retVal = DetermineTypeWrite( attributeTypeArchive, attributeArchive, attributeValue ) 

	databaseAttributeParams.m_AttributeTypeArchive = nil
	databaseAttributeParams.m_AttributeArchive = tolua.cast( attributeArchive, "IArchive" )
	ToolBox:SendMessage( msgSetGlobalAttribute, databaseAttributeParams:SIZEOF(), databaseAttributeParams )

	return retVal
end


-------------------------------------------------------
-- SendGetIndexMessage
-------------------------------------------------------
local function SendGetIndexMessage( hsObjectName, attributeName, attributeValue, componentTypeName )
	-- TODO <rnguyen>: There may be significant overhead on instantiating archives
	--                 particularly if we want to loop a GetAttribute. Consider
	--                 caching table of object names to archive instances.

	-- Archive for the value
	local attributeArchive = CLuaArchive:new_local()
	attributeArchive:InitArchive( 3, hszMemoryType )

	-- Archive for the data type
	local attributeTypeArchive = CLuaArchive:new_local()
	attributeTypeArchive:InitArchive( 3, hszMemoryType )

	local databaseAttributeParams = DATABASEATTRIBUTEPARAMS:new_local()
	databaseAttributeParams.m_Index = -1
	databaseAttributeParams.m_AttributeName = CLuaHashString:new_local( attributeName )
	databaseAttributeParams.m_AttributeTypeArchive = tolua.cast( attributeTypeArchive, "IArchive" )
	databaseAttributeParams.m_AttributeArchive = tolua.cast( attributeArchive, "IArchive" )

	local hszComponentTypeName = CLuaHashString:new_local(componentTypeName)
	-- Call GetAttributeType so we know the data type
	-- TODO <rnguyen>: Should create a map of attribute names to their types to prevent repeated message sending
	ToolBox:SendMessage( msgGetAttributeType, databaseAttributeParams:SIZEOF(), databaseAttributeParams, hsObjectName, hszComponentTypeName )

	DetermineTypeWrite( attributeTypeArchive, attributeArchive, attributeValue ) 	
	
	databaseAttributeParams.m_AttributeTypeArchive = nil
	databaseAttributeParams.m_AttributeArchive = tolua.cast( attributeArchive, "IArchive" )
	ToolBox:SendMessage( msgGetIndex, databaseAttributeParams:SIZEOF(), databaseAttributeParams, hsObjectName, hszComponentTypeName )

	local retVal = databaseAttributeParams.m_Index
	return retVal
end


-------------------------------------------------------
-- GetAttribute
-------------------------------------------------------
function GetAttribute( hsObjectName, attributeName )
	return SendGetAttributeMessage( -1, hsObjectName, attributeName, "CDatabaseEntry" )
end

-------------------------------------------------------
-- SetAttribute
-------------------------------------------------------
function SetAttribute( hsObjectName, attributeName, attributeValue )
	SendSetAttributeMessage( -1, hsObjectName, attributeName, attributeValue, "CDatabaseEntry" )
end

-------------------------------------------------------
-- GetRowAttribute
-------------------------------------------------------
function GetRowAttribute( iIndex, databaseName, attributeName )
	local hsObjectName = CLuaHashString:new_local( databaseName )
	return SendGetAttributeMessage( iIndex, hsObjectName, attributeName, "CDefinedDatabase" )
end

-------------------------------------------------------
-- SetRowAttribute
-------------------------------------------------------
function SetRowAttribute( iIndex, databaseName, attributeName, attributeValue )
	local hsObjectName = CLuaHashString:new_local( databaseName )
	SendSetAttributeMessage( iIndex, hsObjectName, attributeName, attributeValue, "CDefinedDatabase" )
end

-------------------------------------------------------
-- GetDatabaseRowCount
-------------------------------------------------------
function GetDatabaseRowCount( databaseName )
	local databaseInfoParams = DATABASEINFOPARAMS:new_local()

	local hsObjectName = CLuaHashString:new_local( databaseName )
	local hszComponentTypeName = CLuaHashString:new_local("CDefinedDatabase")
	ToolBox:SendMessage( msgGetDatabaseInfo, databaseInfoParams:SIZEOF(), databaseInfoParams, hsObjectName, hszComponentTypeName )

	return databaseInfoParams.m_NumItems
end

-------------------------------------------------------
-- GetGlobalAttribute
-------------------------------------------------------
function GetGlobalAttribute( attributeName )
	return SendGetGlobalAttributeMessage( attributeName )
end

-------------------------------------------------------
-- GetGlobalAttribute
-------------------------------------------------------
function SetGlobalAttribute( attributeName, attributeValue )
	return SendSetGlobalAttributeMessage( attributeName, attributeValue )
end

-------------------------------------------------------
-- GetIndex
-------------------------------------------------------
function GetIndex( databaseName, attributeName, attributeValue )
	local hsObjectName = CLuaHashString:new_local( databaseName )
	return SendGetIndexMessage( hsObjectName, attributeName, attributeValue, "CDefinedDatabase" )
end