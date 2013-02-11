///==========================================================================
/// \file	CParticleObject.h
/// \brief	Implementation of CParticleObject interface for rendering
/// \date	5/22/2008
/// \author	Brendan Geiger
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

#ifndef	_CParticleObject_H_
#define _CParticleObject_H_

#include "IParticleObject.h"
#include "IMeshObject.h"
#include "ITextureObject.h"

/// PARTICLEOBJECTTEMPLATE is a #define to define CParticleObject type classes
/// \param managerName = The manager name that manages this object
/// \param className = The class name we represent or masquerade as
/// \param parentName = the name of the parent object if any, can be NULL
/// \param name = our unique name in the system
#define PARTICLEOBJECTTEMPLATE(managerName, className, parentName, name) \
	CParticleObject(_T(#managerName), _T(#className), parentName, name)

///==========================================================================
/// \class	CParticleObject
/// \brief	Generic base class for render objects
///==========================================================================
class CParticleObject : public CSceneObject<IParticleObject>
{
protected:
	/// \brief The Default Constructor for CParticleObject
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CParticleObject(const TCHAR * theManager, const TCHAR * classname, IHashString *parentName, IHashString *name);
public:
	/// \brief	The Default Destructor for CParticleObject
	~CParticleObject();

	/// \brief	Create the a instance of IObject
	/// \return	IComponent pointer
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Initialization for the Object
	/// \param	IManager* pManager: Parent Manager
	virtual void Init( ) { }
	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update( ) { return true; }
	/// \brief	The DeInit Function for the Object
	virtual void DeInit( ) { }
	/// \brief either reads or writes the entity information to a file
	/// Currently Loads up the entity completely if reading
	virtual void Serialize( IArchive &ar ){ }
	
	/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render( UINT pass, IEffect * override )
	{
		//no real way to render this
		return false;
	}

	/// \brief Number of passes this render object has to go through
	/// \return UINT = returns num passes
	virtual UINT GetNumPass() 
	{ 
		return 1;
	}
	
	/// \brief This is for render priorities between classes
	/// will change as soon as a new architecture can be designed to better incorporate
	/// different renderobject types being pushed on the same sorted list
	/// \return DWORD = the priority key this returns, the higher the number,
	/// the higher its priority
	virtual DWORD GetRenderPriority()
	{ 
		return 99; 
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

#endif	// _CParticleObject_H_
