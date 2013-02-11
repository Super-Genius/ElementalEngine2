---------------------------------------------------------------------------------
-- utl_Event.lua
-- Script eventing system. Creates a global pool of function lists index by object and event
---------------------------------------------------------------------------------

m_ToolBox = CLuaToolBox:GetLuaToolBox()

-- Table indexed by object identifier to a table of events to event handlers
-- Table hierarchy: Object name -> Event name -> Event handler
local m_EventsTable = {}


---------------------------------------------------------------------------------
-- RegisterEventHandler
---------------------------------------------------------------------------------
function RegisterEventHandler( szObject, szEvent, eventHandler )
	-- Be very careful here. We are using nested levels of tables.
	if m_EventsTable[ szObject ] == nil then
		m_EventsTable[ szObject ] = {}
	end

	local objectEventTable = m_EventsTable[ szObject ]
	if objectEventTable[ szEvent ] == nil then
		objectEventTable[ szEvent ] = {}
	end

	local eventHandlerTable = objectEventTable[ szEvent ]
	local iTableSize = table.getn( eventHandlerTable )
	eventHandlerTable[ iTableSize + 1 ] = eventHandler

	OutputDebug( LOGWARNING, "Registering event: "..szEvent.." ("..table.getn( eventHandlerTable )..")\n" )
end

---------------------------------------------------------------------------------
-- OnEvent
---------------------------------------------------------------------------------
function OnEvent( szObject, szEvent, params )
	if m_EventsTable[ szObject ] ~= nil then
		local objectEventTable = m_EventsTable[ szObject ]
		if objectEventTable[ szEvent ] ~= nil then
			local eventHandlerTable = objectEventTable[ szEvent ]
			local iTableSize = table.getn( eventHandlerTable )
			for i = 1, iTableSize do
				eventHandlerTable[i]( params )
			end
		end
	end
end

-- TODO <rnguyen>: Need a procedure to dispose of event handlers.