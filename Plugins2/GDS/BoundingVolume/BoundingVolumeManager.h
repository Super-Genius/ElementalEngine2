///============================================================================
/// \file		BoundingVolumeManager.h
/// \brief		Manager for bounding volume objects.
/// \date		07/27/2007
/// \author		Kyle Swaim
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

#ifndef CBOUNDINGVOLUMEMANAGER_H
#define CBOUNDINGVOLUMEMANAGER_H

typedef map<DWORD, IHashString*> BVNAMETOOBJECTMAP;
//typedef map<IHashString*, IHashString*> BVNAMETOOBJECTMAP;

class CBoundingVolumeManager : public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CBoundingVolumeManager );
	
	// Default Constructor
	CBoundingVolumeManager();

public:
	// Destructor
	virtual ~CBoundingVolumeManager();

	/// Function to get this component.
	///	Returns the singleton instance in this case
	static IComponent *Create(int nArgs, va_list argptr);

	/// Update
	virtual void Update(DWORD tickCount);

	// **** MESSAGES **** //
	DWORD OnCreateBVolumeObject( DWORD size, void* params );
	DWORD OnDeleteBVolumeObject( DWORD size, void* params );

private:
	BVNAMETOOBJECTMAP m_BVMap;
};

#endif // #ifndef CBOUNDINGVOLUMEMANAGER_H