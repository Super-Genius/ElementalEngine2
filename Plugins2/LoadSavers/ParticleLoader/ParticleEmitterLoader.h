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

#ifndef PARTICLEEMITTERLOADER_H
#define PARTICLEEMITTERLOADER_H

/// Map of DWORD Particle Emitter Names to DWORD IObjects
typedef map<DWORD, DWORD> PARTICLEEMITTERTOOBJECTMAP;

class CParticleEmitterLoader : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CParticleEmitterLoader);
	CParticleEmitterLoader();
public:

	/// We are the serializer, so we don't do anything with this.
	virtual void Serialize(IArchive &ar) {};

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	/// standard static component create function
	static IComponent *Create(int nArgs, va_list argptr);

	// ------------------ Messages ------------------ //
	/// message to load a Particle Emitter file
	DWORD OnLoadParticleEmitterFile(DWORD size, void *params);

	/// message to save a Particle Emitter file
	DWORD OnSaveParticleEmitterFile(DWORD size, void *params);

	/// message to unload a Particle Emitter file
	DWORD OnUnloadParticleEmitterFile(DWORD size, void *params);

private:
	static CHashString m_HashName;
	IToolBox *m_ToolBox;
	static LSRC_MESSAGE m_LoadSaveRegisterData;

	PARTICLEEMITTERTOOBJECTMAP m_ParticleEmitterMap;
};


#endif /* PARTICLEEMITTERLOADER_H */