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

#ifndef PARTICLELOADER_H
#define PARTICLELOADER_H

typedef map<DWORD, DWORD> PARTICLETOOBJECTMAP;

class CParticleLoader : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CParticleLoader);
	CParticleLoader();
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

	/// Helper particle loader function
	bool AddLoadedParticleType( IHashString *pTypeName, IHashString *pTypeTypeName, IArchive *pArchive );

	// ------------------ Messages ------------------ //
	/// message to load a Particle Emitter file
	DWORD OnLoadParticleFile(DWORD size, void *params);
	/// message to load a Particle Brush file
	DWORD OnLoadParticleBrush(DWORD size, void *params);

	/// message to save a Particle Emitter file
	DWORD OnSaveParticleFile(DWORD size, void *params);
	/// message to save a Particle Brush file
	DWORD OnSaveParticleBrush(DWORD size, void *params);

	/// message to create new Particle Emitter file
	DWORD OnNewParticleFile(DWORD size, void *params);
	/// message to create new Particle Brush file
	DWORD OnNewParticleBrush(DWORD size, void *params);

	/// message to unload a Particle Emitter file
	DWORD OnUnloadParticleFile(DWORD size, void *params);
	/// message to unload a Particle Brush file
	DWORD OnUnloadParticleBrush(DWORD size, void *params);

private:
	static CHashString m_HashName;
	IToolBox *m_ToolBox;
	static LSRC_MESSAGE m_LoadSaveRegisterData;

	PARTICLETOOBJECTMAP m_ParticleObjectMap;
};


#endif /* PARTICLELOADER_H */