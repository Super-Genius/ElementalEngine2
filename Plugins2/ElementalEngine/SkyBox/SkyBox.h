///============================================================================
/// \file		Skybox.h
/// \brief		Declaration of CSkybox Class
/// \date		08-16-2005
/// \author		Brian Bazyk
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

#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#ifndef	_COBJECTTEMPLATE_HPP
#include "CObjectTemplate.hpp"
#endif

#include <string>
using namespace std;

typedef struct VERTEX
{
	Vec3 pos;
	Vec3 norm;
	float ucoord, vcoord;

	VERTEX(Vec3 p=Vec3(0,0,0), Vec3 n=Vec3(0,0,0), float u=0, float v=0)
	{
		pos = p;
		norm = n;
		ucoord = u;
		vcoord = v;
	}
} VERTEX;


struct SKYCUBEVERTEX
{
	float pos[3];
	float tex[3];
};


class CSkyboxTextureCallback : public ITextureCallback
{
public:
	CHashString textureName;
	virtual IBaseTextureObject* GetTexture()
	{
		TEXTUREOBJECTPARAMS top;
		top.Name = &textureName;
		top.TextureObjectInterface = NULL;
		top.bLoad = true;
		static DWORD msgGetTexture = CHashString(_T("GetTexture")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgGetTexture, sizeof(TEXTUREOBJECTPARAMS), &top);
		return top.TextureObjectInterface;
	}
	virtual IHashString * GetName()
	{
		static CHashString cname( _T("SkyboxTextureCallback") );
		return &cname;
	}
};

class CSkyboxObject : public CSceneObject<IRenderObject>
{
protected:
	/// Default Constructor
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CSkyboxObject(IHashString *parentName, IHashString *name, bool bAddToHier);
public:

	/// Default Destructor
	~CSkyboxObject();

	/// \brief Create an instance
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief DeInit Function
	virtual void DeInit();

	/// \brief Init Function
	virtual void Init();

	/// \brief serialize
	/// \param ar - The Archive
	virtual void Serialize(IArchive &ar);

	/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render( UINT pass, IEffect * override );

	/// \brief Compare for sorting
	/// \param obj = object to compare to
	/// \param sortMethod = a sort method specifying how to compare(set in RENDEROBJECTCOMPARE)
	/// \param curRenderPass = the current render pass to sort to
	/// \return bool = must return true if (this pointer) < (obj)
	virtual bool IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass );

	/// \brief This is for render priorities between classes
	/// will change as soon as a new architecture can be designed to better incorporate
	/// different renderobject types being pushed on the same sorted list
	/// \return DWORD = the priority key this returns, the higher the number,
	/// the higher its priority
	virtual DWORD GetRenderPriority();

	/// \brief Number of passes this render object has to go through
	/// \return UINT = returns one pass for this implementation
	virtual UINT GetNumPass() { return 1; };

	/// \brief Sets the position of the render object
	/// \param position Vec3 class reference to fill in
	virtual void SetPosition( const Vec3 &vec );

	/// \brief Gets the position of the render object
	/// \return Vec3 = position f the object
	virtual Vec3 &GetPosition();

	// TODO: implement if necessary
	virtual IHashString * GetBaseSortClass() { return GetComponentType(); }

	/// \brief returns if this renderobject needs to be sorted in the transparency list
	/// \return bool = true if this object uses transparency
	virtual bool IsAlphaObject() { return false; }

	IMeshObject* GetMeshObject();

	/// Gets the number of Geometry elements this renderobject possesses or if it uses geometry at all	
	virtual UINT GetNumGeometry(){ return 0; }

	/// Grabs the geometry pieces of the render object if there is any uses it for render instead
	/// of the render object render(might need to be changed)
	virtual IGeometry* GetGeometry( int index ){ return NULL; }

	virtual bool IsKindOf( IHashString * compType );

	/// \brief GetBoundingBox default implementation
	virtual bool GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation )
	{	
		return false;
	}

	/// Attaches a light object to this Render object
	virtual void AttachLight( ILightObject * light ){}

	/// Signals a detach of all light objects
	virtual void DetachAllLights(){}
	void GenerateSphere();
	void GenerateSkyBox();
	void LoadTextures();
	void UnloadTextures();

	// ** MESSAGES ** //
	DWORD OnSetZOffset(DWORD size, void *param);
	DWORD OnGetSkyboxTexture(DWORD size, void *param);
	DWORD OnSetSkyboxTexture(DWORD size, void *param);

	ISceneSettings * GetSceneSettings()
	{
		return NULL;
	}
	IMaterial * GetMaterial( int index )
	{
		return NULL;
	}
	virtual UINT GetNumMaterial() { return 0; }

private:
	IRenderer *m_IRenderer;

	/// Class Type
	CHashString m_hszNameType;
	bool m_bAddToHierarchy;

	VERTEX *m_Sphere;
	int *m_SphereIndices;
	int m_iMeshSize;
	float m_fCurrentTime;
	float m_fSize;
	CSkyboxShaderCallback *m_SkyboxShaderCallback;
	IBaseTextureObject *m_pSkyboxTexture;
	IEffect *m_pSkyboxEffect;
	float m_fYOffset;
	StdString m_TexturePath;
	StdString m_EffectPath;

	//global allocation for this model
	BUFFERALLOCATIONSTRUCT m_IndexBufferAllocation;
	BUFFERALLOCATIONSTRUCT m_VertexBufferAllocation;
	IIndexBuffer * m_IndexBufferInterface;
	IVertexBufferObject * m_VertexBufferInterface;
};

#endif //__SKYBOX_H__
 
