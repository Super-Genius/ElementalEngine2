///============================================================================
/// \note   Elemental Engine
///         Copyright (c)  2005-2008 Signature Devices, Inc.
///
///         This code is redistributable under the terms of the EE License.
///
///         This code is distributed without warranty or implied warranty of
///			merchantability or fitness for a particular purpose. See the 
///			EE License for more details.
///
///         You should have received a copy of the EE License along with this
///			code; If not, write to Signature Devices, Inc.,
///			3200 Bridge Parkway Suite 102, Redwood City, CA 94086 USA.
///============================================================================

#include "stdafx.h"
#include "Turret.h"

REGISTER_COMPONENT(CTurret);

REGISTER_MESSAGE_HANDLER(SetTarget, OnSetTarget, CTurret );

CTurret::CTurret(IHashString *parentName, IHashString *name) 
	: OBJECTTEMPLATE( NULL, CTurret, IObject, parentName, name )
{

}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CTurret::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = (IObject*)new CTurret(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}
void CTurret::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
	}
}

void CTurret::Init()
{
/*	
	LoadFileByExt(szArchiveFile, true)
	nTurrets = GetDatabaseRowCount(szArchiveName)
	
	self.Turrets = {}
	
	table.insert(self.Turrets,
	AITurretBase
	{
	TurretName = 			GetRowAttribute(0, szArchiveName, "TurretName"):GetString()
	TurretControlName = 	TurretName
	CurrentRotation = 		0
	BarrelControlName = 	GetRowAttribute(0, szArchiveName, "TurretAngle")
	CurrentAngle = 			0
	Reversed = 				GetRowAttribute(0, szArchiveName, "Reversed")
	
	RateOfFireTime = 		GetRowAttribute(0, szArchiveName, "RateOfFire")
	Range = 				GetRowAttribute(0, szArchiveName, "Range")
	ProjectileSpeed = 		GetRowAttribute(0, szArchiveName, "ProjectileSpeed")
	ProjectileDamage = 		GetRowAttribute(0, szArchiveName, "ProjectileDamage")
	ProjectileSpawner = 	GetRowAttribute(0, szArchiveName, "ProjectileSpawner"):GetString()
	Salvoing = 				GetRowAttribute(0, szArchiveName, "Salvoing")
	SalvoCurrentTime = 		0.0
	SalvoCurrentBarrel = 	1
	SalvoDelay = 			GetRowAttribute(0, szArchiveName, "SalvoDelay")
	ParticleEffectSpawner = GetRowAttribute(0, szArchiveName, "ParticleEffectSpawner"):GetString()
	SoundEffectFilepath = 	GetRowAttribute(0, szArchiveName, "SoundEffectFilepath"):GetString()
	
	MinAngle = 				-GetRowAttribute(0, szArchiveName, "MinAngle")
	MaxAngle =				GetRowAttribute(0, szArchiveName, "MaxAngle")
	AngleSpeed = 			GetRowAttribute(0, szArchiveName, "AngleSpeed")

	RightMax = 				GetRowAttribute(0, szArchiveName, "RightRotation")
	LeftMax = 				GetRowAttribute(0, szArchiveName, "LeftRotation")
	RotationSpeed = 		GetRowAttribute(0, szArchiveName, "RotationSpeed")
	NumBarrels = 			GetRowAttribute(0, szArchiveName, "NumberOfBarrels")
	for i = 1, NumBarrels do
		if (i == 1) then
			BarrelInfo[i].FirePoint = GetRowAttribute(0, szArchiveName, "BarrelName1"):GetString()
		end
		if (i == 2) then
			BarrelInfo[i].FirePoint = GetRowAttribute(0, szArchiveName, "BarrelName2"):GetString()
		end
		if (i == 3) then
			BarrelInfo[i].FirePoint = GetRowAttribute(0, szArchiveName, "BarrelName3"):GetString()
		end
		if (i == 4) then
			BarrelInfo[i].FirePoint = GetRowAttribute(0, szArchiveName, "BarrelName4"):GetString()
		end
		LoadFileByExt("spawners\\AttachObject.sxl", true)
		BarrelInfo[i].hszBindPoint = GetSpawnedName()
		AttachSceneObject( ArmoryMapElement.TurretName, BarrelInfo[i].hszBindPoint, FirePoint )
		BarrelInfo[i].hszParticleName = CLuaHashString:new_local()
	}
	)
	*/
}
bool CTurret::Update()
{
	if (bFirstUpdate)
	{
		
	}
/*
	function ShipBase:Armory_Update()
		local deltaTime = GetTimeDelta("GameDeltaTimer")
	--	OutputDebug("ArmorDump: DeltaTime: "..deltaTime.."\n")

	--		OutputDebug("ArmorDump: Ship Targeting\n")
	--		OutputDebug("ArmorDump: Number of Turrets:"..self.Armory_NumRows.."\n")
		local v3TargetPos = self:Armory_GetTarget()
		local v3Cross = CLuaVec3:new_local()
		local v3t = CLuaVec3:new_local()
		local v3o = CLuaVec3:new_local()
		local v3TurretPos = CLuaVec3:new_local()
		local matrix = CLuaMatrix4x4:new_local()

		for i=1, self.Armory_NumRows do
			-- check to see if target is in firing arc
	--			OutputDebug("Armory Turret "..i.." Targeting Pos: ("..v3TargetPos:GetX()..","..v3TargetPos:GetY()..","..v3TargetPos:GetZ()..")\n")
			local armory = self.m_ArmoryList[i]
			local armoryMap = self.m_ArmoryMapList[i]
			GetGlobalBoneInfo(self.m_ScriptObjectParentName, armoryMap["TurretControlName"], v3TurretPos, nil, matrix)
			v3o = tolua.cast(matrix:GetAxisZ(), "CLuaVec3")
	--			OutputDebug("Armory Turret "..i.." Turret Global Rot: ("..v3o:GetX()..","..v3o:GetY()..","..v3o:GetZ()..")\n")			
	--			OutputDebug("Armory Turret "..i.." Turret "..armoryMap["TurretControlName"].." Pos: ("..v3TurretPos:GetX()..","..v3TurretPos:GetY()..","..v3TurretPos:GetZ()..")\n")

 			-- Remove Y component for now and normalize
			v3t:SetX( -(v3TargetPos:GetX() - v3TurretPos:GetX()) )
			v3t:SetZ( -(v3TargetPos:GetZ() - v3TurretPos:GetZ()) )
			v3t:SetY( 0 )
			if (true == armory["Reversed"]) then
				v3o:SetX( -v3o:GetX() )
				v3o:SetZ( -v3o:GetZ() )
			else
				v3o:SetX( v3o:GetX() )
				v3o:SetZ( v3o:GetZ() )
			end
			v3o:SetY( 0 )
			v3t:Normalize()		
			v3o:Normalize()
	--			OutputDebug("Armory Turret "..i.." Turret to Target Rot: ("..v3t:GetX()..","..v3t:GetY()..","..v3t:GetZ()..")\n")
	--			OutputDebug("Armory Turret "..i.." Turret base Rot: ("..v3o:GetX()..","..v3o:GetY()..","..v3o:GetZ()..")\n")
			local dot = v3o:Dot(v3t)
			if dot > 1 then
				dot = 1
			end
			if dot < 1 then
				v3Cross = tolua.cast( v3o:Cross(v3t), "CLuaVec3")
			end
			local theta = self.m_MathLib:GetACos(dot)
			theta = self.m_MathLib:RadToDeg( theta )
	--			OutputDebug("Theta: "..theta.." generated from dot: "..dot.."\n")
				-- Target in Possible Firing Arc
			if (theta > self.m_RotateTreshhold)  then
	--					OutputDebug("ArmorDump:Target Out Of Angle!\n")
				--Target is not lined up, rotate to target
				--Get Rotation Offset relative to the heirarchy model
				local RotationRate
				local v3TargetOffsetRot = CLuaEulerAngle:new_local()					
				GetOffsetBoneInfo(self.m_ScriptObjectParentName, armoryMap["TurretControlName"], nil, v3TargetOffsetRot)
	--					OutputDebug("ArmorDump: Turret ID: "..armoryMap["TurretControlName"].." RotOff: ("..v3TargetOffsetRot:GetX()..","..v3TargetOffsetRot:GetY()..","..v3TargetOffsetRot:GetZ()..")\n")

				if (theta < self.m_FireThreshhold  ) then
					RotationRate = .25*deltaTime*self.m_ArmoryList [i]["RotationSpeed"]
				else
					RotationRate = deltaTime*self.m_ArmoryList [i]["RotationSpeed"]
				end

				if (v3Cross:GetY() < 0.0) then
					v3TargetOffsetRot:SetYaw(v3TargetOffsetRot:GetYaw() - RotationRate)
	--						OutputDebug("ArmorDump: Turret Rotating to the Right "..deltaTime*self.m_ArmoryList [i]["AngleSpeed"].." Degrees\n")
				else
					v3TargetOffsetRot:SetYaw(v3TargetOffsetRot:GetYaw() + RotationRate)
	--						OutputDebug("ArmorDump: Turret Rotating to the Left "..deltaTime*self.m_ArmoryList [i]["AngleSpeed"].." Degrees\n")
				end
				
				
				SetOffsetBoneInfo(self.m_ScriptObjectParentName, armoryMap["TurretControlName"], nil, v3TargetOffsetRot)
	--					OutputDebug("ArmorDump: Turret ID: "..armoryMap["TurretControlName"].." Moved to RotOff: ("..v3TargetOffsetRot:GetX()..","..v3TargetOffsetRot:GetY()..","..v3TargetOffsetRot:GetZ()..")\n")
			end
			if (theta < self.m_FireThreshhold ) then
				-- Target down the barrel
				-- Increment RoF Time
				armoryMap["RateOfFireTime"] = armoryMap["RateOfFireTime"] + deltaTime
				if armoryMap["RateOfFireTime"] > self.m_ArmoryList [i]["RateOfFire"] then
					-- Gun Ready to Fire / Or Firing
					if armoryMap["Salvoing"] then
						-- Gun Already Firing
						armoryMap["SalvoCurrentTime"] = armoryMap["SalvoCurrentTime"] + deltaTime
						if self.m_ArmoryList [i]["Salvoing"] then
							-- Salvo Type
							if (armoryMap["SalvoCurrentTime"] > self.m_ArmoryList [i]["SalvoDelay"]) then
								-- FIRE!! FIRE!! FIRE!!
								if armoryMap["SalvoCurrentBarrel"] == 1 then
	--										OutputDebug("ArmorDump: FIRING BARREL 1!!! FIRE FIRE FIRE\n")
									local v3Barrel1Pos = CLuaVec3:new_local()
									local v3Barrel1Rot = CLuaEulerAngle:new_local()
									GetGlobalBoneInfo(self.m_ScriptObjectParentName, armoryMap["FireControlName1"], v3Barrel1Pos, v3Barrel1Rot)
									FireDynamicProjectile( v3Barrel1Pos, v3Barrel1Rot, armory["ProjectileSpeed"], armory["ProjectileSpawner"] )
									LoadSpawnFile(armory["ParticleEffectSpawner"], self.m_ScriptObjectParentName)
									-- play explosion sound
									PlayGlobal3DSound( "DeckgunShot", v3Barrel1Pos )	
									armoryMap["BarrelFireParticle1"] = GetSpawnedName()
	--										OutputDebug("Particle1 Pos: ("..v3Barrel1Pos:GetX()..","..v3Barrel1Pos:GetY()..","..v3Barrel1Pos:GetZ()..")\n")

									SetGlobalPosition(armoryMap["BarrelFireParticle1"], v3Barrel1Pos)
								end
								if armoryMap["SalvoCurrentBarrel"] == 2 then
	--										OutputDebug("ArmorDump: FIRING BARREL 2!!! FIRE FIRE FIRE\n")
									local v3Barrel2Pos = CLuaVec3:new_local()
									local v3Barrel2Rot = CLuaEulerAngle:new_local()	
									GetGlobalBoneInfo(self.m_ScriptObjectParentName, armoryMap["FireControlName2"], v3Barrel2Pos, v3Barrel2Rot)
									FireDynamicProjectile( v3Barrel2Pos, v3Barrel2Rot, armory["ProjectileSpeed"], armory["ProjectileSpawner"] )
									-- play explosion sound
									PlayGlobal3DSound( "DeckgunShot", v3Barrel2Pos )	
									LoadSpawnFile(armory["ParticleEffectSpawner"], self.m_ScriptObjectParentName)
									armoryMap["BarrelFireParticle2"] = GetSpawnedName()
	--									OutputDebug("Spawnpoint ID: ("..armoryMap["FireControlName2"]..")\n")
	--										OutputDebug("Particle2 Pos: ("..v3Barrel2Pos:GetX()..","..v3Barrel2Pos:GetY()..","..v3Barrel2Pos:GetZ()..")\n")

									SetGlobalPosition(armoryMap["BarrelFireParticle2"], v3Barrel2Pos)
								end
								if armoryMap["SalvoCurrentBarrel"] == 3 then
	--										OutputDebug("ArmorDump: FIRING BARREL 3!!! FIRE FIRE FIRE")
									local v3Barrel3Pos = CLuaVec3:new_local()
									local v3Barrel3Rot = CLuaEulerAngle:new_local()
									GetGlobalBoneInfo(self.m_ScriptObjectParentName, armoryMap["FireControlName3"], v3Barrel3Pos, v3Barrel3Rot)
									FireDynamicProjectile( v3Barrel3Pos, v3Barrel3Rot, armory["ProjectileSpeed"], armory["ProjectileSpawner"] )
									-- play explosion sound
									PlayGlobal3DSound( "DeckgunShot", v3Barrel3Pos )	
									LoadSpawnFile(armory["ParticleEffectSpawner"], self.m_ScriptObjectParentName)
									armoryMap["BarrelFireParticle3"] = GetSpawnedName()
	--										OutputDebug("Particle3 Pos: ("..v3Barrel3Pos:GetX()..","..v3Barrel3Pos:GetY()..","..v3Barrel3Pos:GetZ()..")\n")
									
									SetGlobalPosition(armoryMap["BarrelFireParticle3"], v3Barrel3Pos)
								end
								if armoryMap["SalvoCurrentBarrel"] == 4 then
	--										OutputDebug("ArmorDump: FIRING BARREL 4!!! FIRE FIRE FIRE")
									local v3Barrel4Pos = CLuaVec3:new_local()
									local v3Barrel4Rot = CLuaEulerAngle:new_local()
									GetGlobalBoneInfo(self.m_ScriptObjectParentName, armoryMap["FireControlName4"], v3Barrel4Pos, v3Barrel4Rot)
									FireDynamicProjectile( v3Barrel4Pos, v3Barrel4Rot, armory["ProjectileSpeed"], armory["ProjectileSpawner"] )
									-- play explosion sound
									PlayGlobal3DSound( "DeckgunShot", v3Barrel4Pos )	
									LoadSpawnFile(armory["ParticleEffectSpawner"], self.m_ScriptObjectParentName)
									armoryMap["BarrelFireParticle4"] = GetSpawnedName()
	--										OutputDebug("Particle4 Pos: ("..v3Barrel4Pos:GetX()..","..v3Barrel4Pos:GetY()..","..v3Barrel4Pos:GetZ()..")\n")
									
									SetGlobalPosition(armoryMap["BarrelFireParticle4"], v3Barrel4Pos)
								end
								local v3ShipPos = CLuaVec3:new_local()
								v3ShipPos = GetGlobalPosition(self.m_ScriptObjectParentName)
								-- Reset Salvo Delay and Increment the Salvo Barrel by one
								armoryMap["SalvoCurrentTime"] = 0.0
								armoryMap["SalvoCurrentBarrel"] = armoryMap["SalvoCurrentBarrel"] + 1
								if armoryMap["SalvoCurrentBarrel"] > self.m_ArmoryList [i]["NumberOfBarrels"] then
									-- If last barrel, reset rate of fire time
									armoryMap["SalvoCurrentBarrel"]  = 1	
									armoryMap["RateOfFireTime"] = 0.0
								end
							end
						else
							-- Normal Gun
	--								OutputDebug("ArmorDump: FIRING BASIC GUN "..armoryMap["TurretName"].."!!! FIRE FIRE FIRE\n")
							local v3BarrelNormalPos = CLuaVec3:new_local()
							local v3BarrelNormalRot = CLuaEulerAngle:new_local()
							GetGlobalBoneInfo(self.m_ScriptObjectParentName, armoryMap["FireControlName1"], v3BarrelNormalPos, v3BarrelNormalRot)
							LoadSpawnFile(armory["ParticleEffectSpawner"], self.m_ScriptObjectParentName)
							armoryMap["BarrelFireParticle1"] = GetSpawnedName()

							FireDynamicProjectile( v3BarrelNormalPos, v3BarrelNormalRot, armory["ProjectileSpeed"], armory["ProjectileSpawner"] )
							SetGlobalPosition(armoryMap["BarrelFireParticle1"], v3BarrelNormalPos)
							
							-- play explosion sound
							local v3ShipPos2 = CLuaVec3:new_local()
							v3ShipPos2 = GetGlobalPosition(self.m_ScriptObjectParentName)

							-- Reset RoF Time
							armoryMap["RateOfFireTime"] = 0.0								
						end
					else
						-- Gun Starting to Fire
						armoryMap["Salvoing"] = true
						armoryMap["SalvoCurrentTime"] = 0.0
						armoryMap["SalvoCurrentBarrel"] = 1
					end
					--Target in the Sights start firing
				end
			end
		end
	end
		*/
	return true;

}

DWORD CTurret::OnSetTarget( DWORD size, void * params )
{
	return MSG_HANDLED;
}