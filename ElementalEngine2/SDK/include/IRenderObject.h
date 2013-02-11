///============================================================================
/// \file	IRenderObject.h
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

#ifndef _IRENDEROBJECT_H
#define _IRENDEROBJECT_H
#include "IGeometry.h"

/// \brief sorting method for renderobjects' inclusion into the renderqueue. This method determines
/// the order that renderobjects are drawn.
enum SORTMETHOD { 
	SORT_FIFO = 0,
	SORT_DISTANCE, 
	SORT_REVERSEDISTANCE,
	SORT_MATERIAL,
	SORT_MODEL	
};

/// \brief the render priorities of certain classes can be based off these.
enum RENDER_ORDER_PRIORITIES
{ 
	RENDERPRIORITY_OPAQUE      = 0x00000001,
	RENDERPRIORITY_ALPHA       = 0x00010000,
	RENDERPRIORITY_FRONTBUFFER = 0x00040000,
};

namespace ElementalEngine
{

class Vec3;
class EulerAngle;
class IMeshObject;

class IRenderObject : public ISceneObject
{
public:
	virtual ~IRenderObject() {};
	/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render( UINT pass, IEffect * override = NULL ) = 0;

	/// \brief Compare for sorting
	/// \param obj = object to compare to
	/// \param sortMethod = a sort method specifying how to compare(set in RENDEROBJECTCOMPARE)
	/// \param curRenderPass = the current render pass to sort to
	/// \return bool = must return true if (this pointer) < (obj)
	virtual bool IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass ) = 0;

	/// \brief This is for render priorities between classes
	/// will change as soon as a new architecture can be designed to better incorporate
	/// different renderobject types being pushed on the same sorted list
	/// \return DWORD = the priority key this returns, the higher the number,
	/// the higher its priority
	virtual DWORD GetRenderPriority() = 0;

	/// \brief Number of passes this render object has to go through
	/// \return UINT = the number of passes that this object has to go through
	virtual UINT GetNumPass() = 0;

	/// \brief Gets the base sort class which implements IRenderObject::IsLessThan()
	/// child classes must not re-implement IsLessThan() if they return the BaseSortClass hash!!
	/// typically, each render object has a IsLessThan() and returns a unique BaseSortClass
	virtual IHashString * GetBaseSortClass() = 0;

	/// \brief returns if this renderobject needs to be sorted in the transparency list
	/// \return bool = true if this object uses transparency
	virtual bool IsAlphaObject() = 0;

	/// \brief Gets the internal mesh
	virtual IMeshObject * GetMeshObject() = 0;

	virtual bool GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation ) = 0;
	/*virtual AddEffectPass( IShaderPass * shaderpass ) = 0;
	virtual int GetNumEffectPasses() = 0;*/

	/// Gets the number of Geometry elements this renderobject possesses or if it uses geometry at all	
	virtual UINT GetNumGeometry()=0;

	/// Grabs the geometry pieces of the render object if there is any uses it for render instead
	/// of the render object render(might need to be changed)
	virtual IGeometry* GetGeometry( int index )=0;

	/// Attaches a light object to this Render object
	virtual void AttachLight( ILightObject * light ) = 0;

	/// Signals a detach of all light objects
	virtual void DetachAllLights() = 0;

	virtual ISceneSettings * GetSceneSettings() = 0;
	
	/// Gets the number of material elements in this renderobject
	virtual UINT GetNumMaterial() = 0;
	
	/// Grabs the material from this render object
	virtual IMaterial * GetMaterial( int index ) = 0;

	virtual OBJECTSORTTYPES GetSceneSortID() const { return RENDEROBJECT3D; }
};

}	//namespace ElementalEngine

#endif