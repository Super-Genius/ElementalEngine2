///============================================================================
/// \file		ObjectCubeMapManager.h
/// \brief		Header file for <insert brief description here>
/// \date		10-04-2005
/// \author		Josh Lulewicz
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


#ifndef __OBJECTCUBEMAPMANAGER_H__
#define __OBJECTCUBEMAPMANAGER_H__

class CObjectCubeMapListener;


struct LODSpriteVertex
{
	float pos[3];
	float u;
	float v;
	float w;
};


typedef map<DWORD, ICubeTextureObject*> CUBEMAPMAP;
typedef map<DWORD, OBJECTLIST> MODELMAP;
typedef map<DWORD, ObjectCubeMap * > MODELTOCUBETEXTUREMAP;

class CObjectCubeMapManager : public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CObjectCubeMapManager );
public:
	/// \brief	Default Destructor
	///			Since CObjectCubeMapManager is implemented as a singleton the
	///			constructor is private.
	CObjectCubeMapManager();

	virtual ~CObjectCubeMapManager(void);

	/// Get the object list from a name
	/// \param name = unicode name of object to get
	/// \return = pointer to STL based list of IObject pointers
	virtual OBJECTSET *GetObjectSet(IHashString *name) { return &m_Objects; }

	/// \brief	Function to get this component.
	///			Returns the singleton instance in this case
	static IComponent *Create( int nArgs, va_list argptr );

	/// \brief	Initializes the Object Cube Map Manager
	/// \return	true on sucess, false on failure
	virtual bool Init();

	/// \brief	Update function for the Object Cube Map Manager
	/// \param tickCount - current tick count
	virtual void Update(DWORD tickCount);
	
	/// \brief	DeInitializes the Object Cube Map Manager
	/// \return	true on sucess, false on failure
	virtual bool DeInit();

	/// \brief Function to add an object to the manager
	/// \param Object is the IObject derived class pointer to add
	/// \return true if success, otherwise false
	virtual bool AddObject( IObject *Object, IHashString *group);

	/// \brief Function to delete an object from the manager
	/// \param Object is the IObject derived
	virtual bool DeleteObject( IObject *Object );

	/// \brief Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar) { }

	/// \brief return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// \brief Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType) { return false; }

	/// Build Set of Names
	/// \param file - file to build set from
	/// \return void
	void BuildTypeSet();

	// ----------- Messages Functions ---------------//
	/// Load object cubemap message
	/// \param size - sizeof a ? struct
	/// \param *params - pointer to a ? structure
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnLoadObjectCubeMap(DWORD size, void *params);
	/// Check LODs on objects in the scene
	/// \param size - sizeof a ? struct
	/// \param *params - pointer to a ? structure
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnCheckLODs(DWORD size, void *params);
	/// Set the fade distance for fade and sprite shaders
	/// \param size - sizeof a ? struct
	/// \param *params - pointer to a ? structure
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnSetFadeDistance(DWORD size, void *params);

	DWORD OnGenerateCubeMap( DWORD size, void * params );
	DWORD OnAddCubeMap( DWORD size, void * params );

	/// Pump handler
	DWORD PumpHandler(DWORD size, void *data, IHashString *name, IHashString *compType, MSGFUNCTION msgFunction);

private:	
	void LoadCubeMapFromMemory( ICubeTextureObject *cubeMap,
							int TextureSize, int BitDepth, void * srcBits[] );
	/// pointer to list of objects that we hold
	OBJECTSET m_Objects;	
	/// toolbox pointer that is holding our Singleton class
	IToolBox *m_ToolBox;
	/// Map of cubemaps
	CUBEMAPMAP m_CubeMaps;
	/// Set of object types we want to load cubemaps for
	DWORDSET m_ModelsWithCubeMapsSet;
	/// Set of object names that have cubemaps
	DWORDSET m_ObjectNamesWithCubeMaps;
	/// Selection visitor by name
	CLODVisitor m_LODVisitor;
	/// 2D texture object
	ITextureObject *m_Texture2D[6];
	/// 2D texture name
	IHashString *m_Texture2DName;
	/// Texture size
	int m_iTextureSize;
	/// Cubemap shader
	IEffect *m_CubeMapShader;
	/// LOD Fade shader
	IEffect *m_FadeShader;
	/// map of model names to cubemap image name array
	MODELTOCUBETEXTUREMAP m_ModelCubeTexturesMap;
	/// fade distance
	float m_fFadeDistance;
	/// type name for camera object to send messages to
	CHashString m_ccamTypeName;
};

#endif //__OBJECTCUBEMAPMANAGER_H__

