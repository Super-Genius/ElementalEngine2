///==========================================================================
/// \file	CWorld.h
/// \brief	Fundamental world class
/// \date	3-13-2007
/// \author Brian Bazyk
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
///==========================================================================

#ifndef	CWORLD_H
#define	CWORLD_H

///==========================================================================
/// \class	CWorld
/// \brief	World contains an entire scene.  There can be multiple worlds in the hierarchy
///==========================================================================
class CWorld : public CObjectTemplate<IObject>
{
private:
	/// Default Constructor
	CWorld(IHashString *parentName, IHashString *name);

public:
	/// Default Destructor
	~CWorld();

	/// \brief	Create the a instance of IObject
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief either reads or writes the entity information to a file
	/// \param ar - Archiver
	virtual void Serialize(IArchive &ar);

	DWORD OnGetGlobalPosition( DWORD size, void* params );
	DWORD OnGetGlobalEuler( DWORD size, void* params );
	DWORD OnGetGlobalQuaternion( DWORD size, void* params );
	DWORD OnGetGlobalScale( DWORD size, void* params );
	DWORD OnGetGlobalTransform( DWORD size, void* params );

private:
	float m_fWorldVersion;
};

#endif	// CWORLD_H