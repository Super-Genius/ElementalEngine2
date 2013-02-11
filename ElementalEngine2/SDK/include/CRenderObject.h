///==========================================================================
/// \file	CRenderObject.h
/// \brief	Implementation of CRenderObject interface for rendering
/// \date	3/9/2005
/// \author	Marvin Gouw
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

#ifndef	_CRENDEROBJECT_H_
#define _CRENDEROBJECT_H_

#include "IRenderObject.h"
#include "IMeshObject.h"
#include "ITextureObject.h"

/// RENDEROBJECTTEMPLATE is a #define to define CRenderObject type classes
/// \param managerName = The manager name that manages this object
/// \param className = The class name we represent or masquerade as
/// \param parentName = the name of the parent object if any, can be NULL
/// \param name = our unique name in the system
#define RENDEROBJECTTEMPLATE(managerName, className, parentName, name) \
	CRenderObject<baseClass>(_T(#managerName), _T(#className), parentName, name)

///==========================================================================
/// \class	CRenderObject
/// \brief	Generic base class for render objects
///==========================================================================
template<class baseClass = IRenderObject>
class CRenderObject : public CSceneObject<baseClass>
{
protected:
	/// \brief The Default Constructor for CRenderObject
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CRenderObject(const TCHAR * theManager, const TCHAR * classname, IHashString *parentName, IHashString *name);
public:
	/// \brief	The Default Destructor for CRenderObject
	virtual ~CRenderObject();

	/// \brief	The Initialization for the Object
	/// \param	IManager* pManager: Parent Manager
	virtual void Init(){}

	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update(){return true;}

	/// \brief	The DeInit Function for the Object
	virtual void DeInit() {}

	/// \brief either reads or writes the entity information to a file
	/// Currently Loads up the entity completely if reading
	virtual void Serialize(IArchive &ar){ }
	
	/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render( UINT pass, IEffect * override )
	{
		//no real way to render this
		return false;
	}

	/// \brief Compare for sorting
	/// \param obj = object to compare to
	/// \param sortMethod = a sort method specifying how to compare(set in RENDEROBJECTCOMPARE)
	/// \param curRenderPass = the current render pass to sort to
	/// \return bool = must return true if (this pointer) < (obj)
	virtual bool IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
	{
		return false;//all objects equal
	}

	/// \brief Number of passes this render object has to go through
	/// \return UINT = returns num passes
	virtual UINT GetNumPass() 
	{ 
		return 0;
	}
	
	/// \brief This is for render priorities between classes
	/// will change as soon as a new architecture can be designed to better incorporate
	/// different renderobject types being pushed on the same sorted list
	/// \return DWORD = the priority key this returns, the higher the number,
	/// the higher its priority
	virtual DWORD GetRenderPriority()
	{ 
		return 0; 
	}

	/// \brief Gets the base sort class which implements IRenderObject::IsLessThan()
	/// child classes must not re-implement IsLessThan() if they return the BaseSortClass hash!!
	/// typically, each render object has a IsLessThan() and returns a unique BaseSortClass
	virtual IHashString * GetBaseSortClass(){ return GetComponentType(); }

	/// \brief finds out if this object has alpha
	/// and needs to be put in the alpha queue
	virtual bool IsAlphaObject()
	{ 
		return false;
	}
	
	virtual IMeshObject * GetMeshObject() { return NULL; }

	/// Gets the number of Geometry elements this renderobject possesses or if it uses geometry at all	
	virtual UINT GetNumGeometry(){ return 0; }

	/// Grabs the geometry pieces of the render object if there is any uses it for render instead
	/// of the render object render(might need to be changed)
	virtual IGeometry* GetGeometry( int index ) { return NULL; }

	/// \brief Gets the type of concrete object this is
	/// this should be called by the children to check if it's of this type
	virtual bool IsKindOf( IHashString * compType )
	{
		return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
	}

	/// \brief GetBoundingBox default implementation
	/// child should extend off this
	virtual bool GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation ) { return false; }
	virtual ISceneSettings* GetSceneSettings(){ return NULL; }
	virtual IMaterial* GetMaterial( int index ) { return NULL; }
	virtual UINT GetNumMaterial( ) { return 0; }
	virtual void AttachLight( ILightObject * light ) { }
	virtual void DetachAllLights() { }

	/// Loads an effect properly
	static IEffect* LoadEffect( IHashString *pName );
	/// Loads a texture
	static IBaseTextureObject* LoadTexture( const TCHAR *str );
	/// Loads a material
	static IMaterial* LoadMaterial( IHashString *name );
};

template<class baseClass>
CRenderObject<baseClass>::CRenderObject(const TCHAR * theManager, const TCHAR * classname, IHashString *parentName, IHashString *name) : 
	CSceneObject<baseClass>(theManager, classname, parentName, name )
{
}

template<class baseClass>
CRenderObject<baseClass>::~CRenderObject()
{
	//remove from quad tree or any other manager that uses updateboundingbox params
	RemoveBoundingObject();

	DeInit();
}

template<class baseClass>
IEffect* CRenderObject<baseClass>::LoadEffect( IHashString *pName )
{
	LOADFILEEXTPARAMS lfep;
	StdString shaderfile;

	if (pName != NULL)
		shaderfile = pName->GetString();

	if( shaderfile == "" )
	{ 
		shaderfile = _T("shaders/default.efx"); 
	}
	shaderfile.MakeSafeFileName();
	CHashString filename( shaderfile );
	lfep.fileName = (TCHAR*)shaderfile.c_str();
	lfep.bInternalLoad = true;
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
	if( retval == MSG_HANDLED )
	{
		CREATEEFFECTINTERFACE cef;
		cef.m_Name = &filename;
		static DWORD msgHash_GetEffectInterface = CHashString(_T("GetEffectInterface")).GetUniqueID();
		if (EngineGetToolBox()->SendMessage(msgHash_GetEffectInterface, sizeof(CREATEEFFECTINTERFACE ), &cef ) == MSG_HANDLED)
			return cef.m_EffectInterface;
	}
	return NULL;
}

template<class baseClass>
IBaseTextureObject* CRenderObject<baseClass>::LoadTexture( const TCHAR *str )
{
	if (!str)
		return NULL;
	CHashString texName(str);
	TEXTUREOBJECTPARAMS top;
	top.Name = &texName;
	top.TextureObjectInterface = NULL;
	top.bLoad = true;

	static DWORD msgAddTexture = CHashString(_T("AddTexture")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgAddTexture, sizeof(TEXTUREOBJECTPARAMS), &top);

	if (top.TextureObjectInterface != NULL)
	{
		return dynamic_cast<IBaseTextureObject*>(top.TextureObjectInterface);
	}
	return NULL;
}

template<class baseClass>
IMaterial* CRenderObject<baseClass>::LoadMaterial( IHashString *name )
{
	LOADFILEEXTPARAMS lfep;
	lfep.fileName = (TCHAR*)name->GetString();
	lfep.bInternalLoad = true;
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
	if( retval == MSG_HANDLED )
	{
		CREATEMATERIALINTERFACE cmi;
		cmi.m_Name = name;
		static DWORD msgHash_GetMaterialInterface = CHashString(_T("GetMaterialInterface")).GetUniqueID();
		if (EngineGetToolBox()->SendMessage(msgHash_GetMaterialInterface, sizeof(CREATEEFFECTINTERFACE), &cmi ) == MSG_HANDLED)
		{
			return cmi.m_MaterialInterface;
		}
	}
	return NULL;
}
#endif	// CRENDEROBJECTOBJECT_HPP
