///============================================================================
/// \file		BoundingVolume.h
/// \brief		An object oriented bounding volume
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

#ifndef CBOUNDINGVOLUME_H
#define CBOUNDINGVOLUME_H

class CBoundingVolume : public CObjectTemplate<IObject>
{
public:
	/// Default constructor
	/// \param parentName = The name of the parent object.
	/// \param name = The name of the object.
	/// \param bAddToHier = true to add to the Hierarchy, false to not.
	CBoundingVolume( IHashString *parentName, IHashString *name, bool bAddToHier );

	/// Destructor
	~CBoundingVolume();

	/// Initialize the object
	void Init();

	void Update(DWORD tickCount);
 
	/// Deinitialize the object
	void DeInit();

	/// Create a CBoundingVolume
	/// \param nArgs = The number of arguments in the variable arguement list.
	/// \param argptr = Ptr to the list of arguements.
	/// \return Ptr to the newly created IComponent.
	static IComponent *Create(int nArgs, va_list argptr);	

	/// Serialize data from an archive.
	/// \param ar = An IArchive containing object data.
	void Serialize(IArchive &ar);

private:
	void CreatePickingBox();
	void DeletePickingBox();

	Vec3 m_Position;
	EulerAngle m_Rotation;
	Vec3 m_Extents;

	CHashString m_PhysicsObjName;

	// This is a bit inelegant but the best solution
	// I can come up with at the moment.
	bool initBVOnce;

	IObject* m_PhysObject;
};

#endif // #ifndef CBOUNDINGVOLUME_H