///============================================================================
/// \file		PrecacheLoader.h
/// \brief		Declaration of the loader for precache files
/// \date		12-18-2007
/// \author		D. Patrick Ghiocel
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

#ifndef PRECACHELOADER_H
#define PRECACHELOADER_H

class CPrecacheLoader : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CPrecacheLoader );
	CPrecacheLoader();

public:
	/// standard static component create function
	static IComponent *Create( int nArgs, va_list argptr );

	/// We are the serializer, so we don't do anything with this.
	virtual void Serialize(IArchive &ar) {};

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	// Implementation of LoadFile from LoadSaverBase
	DWORD LoadFile( const TCHAR *fileName, IArchive *archive );

	// Implementation of SaveFile from LoadSaverBase
	DWORD SaveFile( const TCHAR *fileName, IArchive *archive );

	// Implementation of GetLoadDescription from LoadSaverBase
	TCHAR *GetLoadDescription();

	// Implementation of GetSaveDescription from LoadSaverBase
	TCHAR *GetSaveDescription();

	// ------------------ Messages ------------------ //
	/// message to load a Particle Emitter file
	DWORD OnLoadFile(DWORD size, void *params);

	/// message to save a Particle Emitter file
	DWORD OnSaveFile(DWORD size, void *params);

private:
	/// Pointer to the toolbox
	IToolBox *m_ToolBox;

	/// Name
	static CHashString m_LoaderName;

	/// Extension info
	static LSRC_MESSAGE m_LoadSaveRegisterData;
};

#endif //#ifndef PRECACHELOADER_H