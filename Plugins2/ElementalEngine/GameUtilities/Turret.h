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

#ifndef _TURRET_HPP
#define _TURRET_HPP

#define MAX_BARRELS 4

class CBarrelData
{
public:
	CHashString hszFirePoint;
	CHashString hszBindPoint;
	CHashString hszParticleName;
};

class CTurret : public CObjectTemplate<IObject>
{
public:
	CTurret(IHashString *parentName, IHashString *name);
	~CTurret(){}
	static IComponent *Create(int nArgs, va_list argptr);
	virtual void Init();
	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();
	/// \brief	The DeInit Function for the Object
	virtual void DeInit(){}
	/// \brief either reads or writes the entity information to a file
	virtual void Serialize(IArchive &ar);

	DWORD OnSetTarget( DWORD size, void * params );
private:
	// Working Data
	bool bFirstUpdate;
	float fLastTime;
	float fCurrentTime;
	float fDeltaTime;

	// IDs
	CHashString hszTurretName;
	CHashString hszTurretControlName;
	bool bReversed;

	// Gunnery
	float fRateOfFireTime;
	float fRange;
	float fProjectileSpeed;
	float fProjectileDamage;
	float fProjectSpawner;
	bool bSalvoing;
	float fSalvoCurrentTime;
	int iSalvoCurrentBarrel;
	float fSalvoDelay;
	CHashString hszParticleEffectSpawner;
	CHashString hszSoundEffectFilepath;

	float fPitchMax;
	float fPitchMin;
	float fPitchSpeed;
	float fYawRightMax;
	float fYawLeftMax;
	float fYawSpeed;

	int nBarrels;
	CBarrelData Barrels[MAX_BARRELS];
};

#endif //#ifndef _TURRET_HPP