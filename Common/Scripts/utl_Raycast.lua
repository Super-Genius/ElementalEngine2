---------------------------------------------------------------------------------
-- UtilityFunctions.lua
-- Contains handy dandy utility functions so you don't have to!
---------------------------------------------------------------------------------

m_ToolBox = CLuaToolBox:GetLuaToolBox()
m_MathLib = CLuaMath:new_local()

MSG_NOT_HANDLED =		0
MSG_HANDLED = 			1
MSG_HANDLED_PROCEED =	2
MSG_HANDLED_STOP =		3
MSG_SIZE_INVALID = 		4
MSG_ERROR =				3131956909 -- 0xBAADDEAD
MSG_WARNING =			3135106223 -- 0xBADDECAF

local debug = 0

ALL			= 0xffffffff -- = -1,
OBJECTS		= 1
TERRAIN		= 2
CONTROL		= 3
NOCOLLISION = 31


-- Container for various global utility functions
Utils = {}

-- global message names
msgRaycast					= CLuaHashString:new_local("Raycast")
msgRaycastArchive			= CLuaHashString:new_local("RaycastArchive")
msgRaycastList				= CLuaHashString:new_local("RaycastList")

---------------------------------------------------------------------------------
-- CastCollisionRay
---------------------------------------------------------------------------------
function CastCollisionRay( vPosition, vDirection, iGroup )
	if (vPosition) and (vDirection) then
		local v3Position = CLuaVec3:new_local( vPosition.x, vPosition.y, vPosition.z )
		local v3Direction = CLuaVec3:new_local( vDirection.x, vDirection.y, vDirection.z )
		local group = -1
		if iGroup ~= nil then
			group = iGroup
		end
		local v3CollisionPoint = CLuaVec3:new_local()
		local rc = RAYCOLLISION:new_local()
		local rcp = RAYCASTPARAMS:new_local()
		rcp.rayPos = v3Position
		rcp.rayDir = v3Direction
		rcp.listRayCollisions = tolua.cast( rc, "RAYCOLLISION")
		rcp.group = group
		m_ToolBox:SendMessage( msgRaycast, rcp:SIZEOF(), rcp)
		v3CollisionPoint:Set(rc.hitPoint[0], rc.hitPoint[1], rc.hitPoint[2])

		if (debug == 1) and (v3CollisionPoint) then
			OutputDebug("utl_Raycast.lua:CastCollisionRay():Position: ("..v3Position:GetX()..","..v3Position:GetY()..","..v3Position:GetZ()..")\n")
			OutputDebug("utl_Raycast.lua:CastCollisionRay():Direction: ("..v3Direction:GetX()..","..v3Direction:GetY()..","..v3Direction:GetZ()..")\n")
			OutputDebug("utl_Raycast.lua:CastCollisionRay():Collision: ("..v3CollisionPoint:GetX()..","..v3CollisionPoint:GetY()..","..v3CollisionPoint:GetZ()..")\n")
		end

		return v3CollisionPoint
	else
		OutputDebug("utl_Raycast.lua: CastCollisionRay() utility function called with Invalid Arguments\n")
	end
end

---------------------------------------------------------------------------------
-- CastCollisionRayVerbose
---------------------------------------------------------------------------------
function CastCollisionRayVerbose( vPosition, vDirection, groupTable, outvImpact, outvNormal, outhszName, outhszParentName )
	local v3Position = CLuaVec3:new_local( vPosition.x, vPosition.y, vPosition.z )
	local v3Direction = CLuaVec3:new_local( vDirection.x, vDirection.y, vDirection.z )
	local groupCount = table.getn( groupTable )
	local archive = CLuaArchive:new_local()
	
	archive:InitArchive(3, hszMemoryType );
	archive:SetIsWriting(true)
	archive:WriteVec3( v3Position )
	archive:WriteVec3( v3Direction )
	archive:WriteInt( groupCount )
	for i = 1, groupCount do
		archive:WriteInt( groupTable[i] )
	end
	
	m_ToolBox:SendMessage( msgRaycastArchive, TODWORD(4), archive )
	
	archive:SetIsWriting(false)

	local success = archive:ReadBool( success )
	
	if success then
	
		local hitDistance = 0
		local hitPos = LuaMathLib:Vec3()
		local hitNormal = LuaMathLib:Vec3()
		local hitName = CLuaHashString:new_local()
		local hitParentName = CLuaHashString:new_local()
		
		hitDistance = archive:ReadFloat()
		hitPos.x = archive:ReadFloat()
		hitPos.y = archive:ReadFloat()
		hitPos.z = archive:ReadFloat()
		hitNormal.x = archive:ReadFloat()
		hitNormal.y = archive:ReadFloat()
		hitNormal.z = archive:ReadFloat()
		archive:ReadHashString( hitName )
		archive:ReadHashString( hitParentName )

		if outvImpact ~= nil then
			outvImpact:Set( hitPos )
		end

		if outvNormal ~= nil then
			outvNormal:Set( hitNormal )
		end

		if outhszName ~= nil then
			outhszName:Init( hitName:GetString() )
		end

		if outhszParentName ~= nil then
			outhszParentName:Init( hitParentName:GetString() )
		end

		return hitDistance
	end
	
	return nil;
	
end

---------------------------------------------------------------------------------
-- CastCollisionRayList
---------------------------------------------------------------------------------
function CastCollisionRayList( vPosition, vDirection, iGroup, rayCollisionList )
	local v3Position = CLuaVec3:new_local( vPosition.x, vPosition.y, vPosition.z )
	local v3Direction = CLuaVec3:new_local( vDirection.x, vDirection.y, vDirection.z )
	local rlp = RAYCASTLISTPARAMS:new_local()
	rlp.group = iGroup
	rlp.rayPos = tolua.cast(v3Position, "Vec3")
	rlp.rayDir = tolua.cast(v3Direction, "Vec3")
	rlp.listRayCollisions = tolua.cast(rayCollisionList, "std::list<RAYCOLLISION>")
	
	m_ToolBox:SendMessage(msgRaycastList, rlp:SIZEOF(), rlp)
end

--[[
--NOTES

struct RAYCOLLISION
{
	RAYCOLLISION()
	{
		distance = 0.0f;
		object = NULL;
		parentobject = NULL;
	}

	float distance;
	float hitPoint[3];
	float normal[3];
	IHashString *object;
	IHashString *parentobject;
};
struct RAYCASTPARAMS
{
	RAYCASTPARAMS()
	{
		group = ALL;
  		rayPos = NULL;
  		rayDir = NULL;
		listRayCollisions = NULL;
	}

	COLLISIONGROUP group;
	Vec3 *rayPos;
	Vec3 *rayDir;
	RAYCOLLISION *listRayCollisions;
};
	CLuaVec3  v3DownVector(0,0,-1);
	CLuaVec3  v3ParticlePosition;
	RAYCOLLISION rc;
	RAYCASTPARAMS rcp;	
	rcp.rayPos = v3ParticlePosition;
	rcp.rayDir = v3DownVector;
	rcp.listRayCollisions = &rc;
	rcp.group = ALL;
	m_ToolBox->SendMessage(_T("Raycast"), sizeof(rcp), &rcp );


	if (rc.object != NULL)
	{
		RAYCOLLISION rayCollision = rc;
	
		GETCOMPONENTTYPEPARAMS comptypemsg;
		comptypemsg.name = rayCollision.parentobject;
		if (m_ToolBox->SendMessage( _T("GetComponentType"), sizeof(comptypemsg), &comptypemsg ) != MSG_HANDLED)
		{
			//error?
		}
	
		hitPos.Set( rayCollision.hitPoint[0],rayCollision.hitPoint[1],rayCollision.hitPoint[2] );
		pObjectName = rayCollision.parentobject;
	}





	if (m_TerrainToolPal.GetTerrainTool() != kTerrainToolSelect)
	{
		IHashString *pObjectName = NULL;
		Vec3 hitPos;

		// cast a ray at the terrain
		RAYFROMMOUSEPARAMS rayfrommouse;
		rayfrommouse.mouseX = pMouseMove->m_fXPos;
		rayfrommouse.mouseY = pMouseMove->m_fYPos;
		Vec3 rayPos, rayDir;
		rayfrommouse.rayPos = &rayPos;
		rayfrommouse.rayDir = &rayDir;
		if (m_ToolBox->SendMessage(_T("GetRayFromMouseCoords"), sizeof(rayfrommouse), &rayfrommouse ) == MSG_HANDLED)
		{
			RAYCOLLISION listCollisions;
			RAYCASTPARAMS raycast;
			raycast.rayPos = rayfrommouse.rayPos;
			raycast.rayDir = rayfrommouse.rayDir;
			raycast.listRayCollisions = &listCollisions;
			raycast.group = TERRAIN;
			m_ToolBox->SendMessage(_T("Raycast"), sizeof(raycast), &raycast );

			if (listCollisions.object != NULL)
			{
				RAYCOLLISION rayCollision = listCollisions;

				GETCOMPONENTTYPEPARAMS comptypemsg;
				comptypemsg.name = rayCollision.parentobject;
				if (m_ToolBox->SendMessage( _T("GetComponentType"), sizeof(comptypemsg), &comptypemsg ) != MSG_HANDLED)
				{
					//error?
				}

				hitPos.Set( rayCollision.hitPoint[0],rayCollision.hitPoint[1],rayCollision.hitPoint[2] );
				pObjectName = rayCollision.parentobject;
			}
		}

		// if we hit something
		if (pObjectName)
		{
			ClickTerrain( &hitPos );
		}

	if (pHitPos)
		m_vecBrushPosition = *pHitPos;

//////////////////////////////////////////////////

	BRUSHPARAMS brushParams;
	brushParams = GetTerrainCursorBrush();
	brushParams.m_pPosition = &m_vecBrushPosition;

	// get all sectors affected by this brush
	std::list<IHashString*> SectorList;
	GETAFFECTEDOBJECTSPARAMS gats;
	gats.fRadius = brushParams.m_fSize * 0.5f;
	gats.pPosition = pHitPos;
	gats.pAffectedObjectsList = &SectorList;
	m_ToolBox->SendMessage( _T("GetAffectedTerrainSectors"), sizeof(gats), &gats, &CHashString(_T("CTerrainManager")), NULL );

]]--