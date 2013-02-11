///==========================================================================
/// \file	CPhysicsCloth.h
/// \brief	Physics object for cloth
/// \date	7/16/2007
/// \date	3/23/2009
/// \author	Kyle Swaim
/// \author	Marivaldo Cabral
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

#ifndef	CPHYSICSCLOTH_H
#define CPHYSICSCLOTH_H

class CPhysicsManager;

namespace ElementalEngine
{

class CPhysicsCloth;

typedef list<CPhysicsCloth*> CLOTHLIST;

///==========================================================================
/// \class	CPhysicsCloth
/// \brief	Physics object for cloth
///==========================================================================
class CPhysicsCloth : public IPhysicsCloth
{
public:
	IHashString* m_Name;
	NxCloth* m_Cloth;
	NxClothMesh* m_ClothMesh;
	NxMeshData m_MeshData;
	NxClothDesc* m_ClothDesc;

	IToolBox* m_ToolBox;
	CPhysicsManager* m_Manager;

private:
	/// Constructor
	CPhysicsCloth();
public:
	/// Constructor
	/// \param parentEntityName = Name of the parent entity.
	/// \param name = Name of this cloth object.
	/// \param pose = Cloth's transform.
	/// \param desc = Cloth descriptor.
	CPhysicsCloth( IHashString* parentEntityName, IHashString* name, Matrix4x4 pose, NxClothDesc& clothDesc );
	
	/// Construct from a preexisting NxCloth object.
	/// \param cloth = cloth object
	CPhysicsCloth( NxCloth* cloth );

	/// Construct from a preexisting NxClothDesc object.
	/// \param cloth = cloth object
	CPhysicsCloth( NxClothDesc& clothDesc );

	/// Destructor
	~CPhysicsCloth();

	/// Get the name.
	IHashString* GetName();

	/// Set the geometry the cloth is powering.
	void SetGeometry( IGeometry* geom );
	
	// Cannot set the pose of cloth after its initial creation...
	//void SetPose( Matrix4x4& pose );

	/// Get the NxClothDesc object
	NxClothDesc* GetNxClothDesc();

private:
	/// Cook the cloth mesh.
	/// \param desc = Cloth descriptor
	bool CookMesh( NxClothMeshDesc& desc );

	/// Release cloth descriptor memory
	/// \param desc = cloth descriptor to clean up.
	void ReleaseMeshDescBuffers( const NxClothMeshDesc& desc );
};

}

#endif //CPHYSICSCLOTH_H