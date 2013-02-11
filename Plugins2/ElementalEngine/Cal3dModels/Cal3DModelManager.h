///============================================================================
/// \file	Cal3DModelManager.h
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

#ifndef CAL3DMODELMANAGER_H
#define CAL3DMODELMANAGER_H

#include "CManager.h"

struct VERTEX
{
    float    pos[3];	
    float   Weight[4];
    float   Indices[4];
    float	normal[3];
    float	 tu,tv;
	float	tangent[4];
};

struct VERTEX_NOSKIN
{
    float    pos[3];
    float	normal[3];
    float	 tu,tv;
	float	tangent[4];
	float	lu,lv;  //lightmap coords, streaming doesn't work very well because of how vbs are shared
				   //and offsets have to match
};

struct HARDWARE_ALLOCATION_ID
{
	DWORD meshNameUniqueID;
	DWORD skeletonNameUniqueID;
};
inline bool operator<(const HARDWARE_ALLOCATION_ID &left, const HARDWARE_ALLOCATION_ID &right)
{
	if (left.meshNameUniqueID != right.meshNameUniqueID)
		return left.meshNameUniqueID < right.meshNameUniqueID;
	else
		return left.skeletonNameUniqueID < right.skeletonNameUniqueID;
}

struct HARDWARE_ALLOCATION
{
	BUFFERALLOCATIONSTRUCT vertexBufferAllocation;
	IVertexBufferObject *pVertexBuffer;
	BUFFERALLOCATIONSTRUCT indexBufferAllocation;
	IIndexBuffer *pIndexBuffer;

	CalHardwareModel *pCalHardwareModel;
};
struct REFHARDWARE_ALLOCATION
{
	HARDWARE_ALLOCATION hardwareAllocation;
	int refCount;
};
typedef map<HARDWARE_ALLOCATION_ID, REFHARDWARE_ALLOCATION> HARDWAREALLOCATIONMAP;

class CalCoreModel;
class CalModel;
class CalHardwareModel;

typedef map< CalCoreAnimation *, Cal3dCallback * > CAL3DANIMATIONTOCALLBACKMAP;

class Cal3DModelManager : public CManager
{
public:
	/// \brief The Default Destructor for Cal3DModelManager
	/// \return void
	~Cal3DModelManager( );

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief	Initializes the Particle Manager
	/// \param	void
	/// \return	bool: Success
	virtual bool Init( void );

	/// \brief	Update function for the Particle Manager
	/// \return	void
	virtual void Update( DWORD tickCount );
	
	/// \brief	DeInitialization
	/// \return	bool: Success
	virtual bool DeInit( void );

	DWORD OnGetCallback( DWORD size, void * params );
	/// Gets the cal3d model interface
	/// \param size = sizeof(GETCALMODELINTERFACE)
	/// \param param = GETCALMODELINTERFACE with name of model and IRenderObject* to return
	/// \return MSG_HANDLED_STOP
	DWORD OnGetCal3DModelInterface(DWORD size, void *param);
	DWORD OnSaveAllCalCubes(DWORD size, void *param);

	DWORD OnDestroyRenderer(DWORD size, void *param);

	CalCoreSkeleton* GetCoreSkeleton( IHashString *pName );
	CalCoreAnimation* GetCoreAnimation( IHashString *pName );
	CalCoreMesh* GetCoreMesh( IHashString *pName );
	CalCoreModel* GetCoreModel( IHashString *pName );
	vector<IMaterial*>* GetModelMaterials( IHashString *pName );

	CCal3DCoreModelCache* GetCoreModelCache( IHashString *pName );
	bool LoadHardwareAllocation( CalCoreModel *pCoreModel, IHashString *pMeshName, IHashString *pSkeletonName, HARDWARE_ALLOCATION &out_hardwareAlloc );
	void UnloadHardwareAllocation( IHashString *pMeshName, IHashString *pSkeletonName );

	CHANNELDESCRIPTORLIST	VertexFormat;
	CHANNELDESCRIPTORLIST	VertexFormatNoSkin;

private:
	SINGLETONCONSTRUCTOROVERRIDE( Cal3DModelManager );
	/// \brief The Default Constructor for Cal3DModelManager
	/// \return void
	Cal3DModelManager();

	CalCoreModel *  LoadFromFile( const TCHAR * file, vector< IMaterial * > &outmaterials );
//	bool LoadCacheDataFromFile( const TCHAR *file, CalCoreMesh *pCoreMesh );

	CAL3DANIMATIONTOCALLBACKMAP m_AnimationCallbacks;
	lua_State *m_LuaState;

	HARDWAREALLOCATIONMAP m_HardwareAllocationMap;
};

#endif