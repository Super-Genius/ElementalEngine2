///==========================================================================
/// \file	CParticleBatchRenderObject.h
/// \brief	Implementation of CParticleBatchRenderObject interface for rendering
/// \date	05/09/2007
/// \author	Patrick Ghiocel
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

#ifndef	_CParticleBatchRenderObject_H_
#define _CParticleBatchRenderObject_H_

#include "stdafx.h"

using namespace std;

namespace ElementalEngine 
{

///==========================================================================
/// \class	CParticleBatchRenderObject
///==========================================================================
class CParticleBatchRenderObject: public CRenderObject<>
{
protected:
	/// \brief The Default Constructor for CParticleBatchRenderObject
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CParticleBatchRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier);

public:
	/// \brief	The Default Destructor for CParticleBatchRenderObject
	~CParticleBatchRenderObject();

	/// \brief	Create the a instance of IObject
	/// \return	IComponent pointer
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Initialization for the Object
	/// \param	IManager* pManager: Parent Manager
	virtual void Init();
	
	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	//virtual bool Update();
	
	/// \brief	The DeInit Function for the Object
	virtual void DeInit();

	virtual OBJECTSORTTYPES GetSceneSortID() const { return RENDEROBJECT3D; }

	/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render( UINT pass, IEffect * override );

	/// \brief Compare for sorting
	/// \param obj = object to compare to
	/// \param sortMethod = a sort method specifying how to compare(set in RENDEROBJECTCOMPARE)
	/// \param curRenderPass = the current render pass to sort to
	/// \return bool = must return true if (this pointer) < (obj)
	virtual bool IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass );

	/// \brief Number of passes this render object has to go through
	/// \return UINT = returns one pass for this implementation
	virtual UINT GetNumPass() { return 1; };
	
	/// \brief This is for render priorities between classes
	/// will change as soon as a new architecture can be designed to better incorporate
	/// different renderobject types being pushed on the same sorted list
	/// \return DWORD = the priority key this returns, the higher the number,
	/// the higher its priority
	virtual DWORD GetRenderPriority(){ return 99; }

	/// \brief Gets bounding sphere of object. This is a 3D interface call that should be implemented for only
	///			3D objects. Might want to seperate more of this later
	/// \param position = vector that will be filled the center of the sphere
	/// \param radius = float that will be filled with the radius of the sphere
	virtual void GetBoundingSphere( Vec3 &position, float &radius );

	/// \brief vision implementation, finds out if this object has alpha
	/// and needs to be put in the alpha queue
	virtual bool IsAlphaObject();
	
	/// \brief either reads or writes the entity information to a file
	/// Currently Loads up the entity completely if reading(inside vision too)
	virtual void Serialize(IArchive &ar);

	/// \brief Gets the base sort class which implements IRenderObject::IsLessThan()
	/// child classes must not re-implement IsLessThan() if they return the BaseSortClass hash!!
	/// typically, each render object has a IsLessThan() and returns a unique BaseSortClass
	virtual IHashString * GetBaseSortClass();

	virtual bool IsKindOf( IHashString * compType )
	{
		return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
	}

	
private:
	float				m_CurrentTick;
	bool				m_bEnabled;
	bool				m_bInScene;
	float				m_TimeDelta;
	IRenderer *			m_IRenderer;

	float				m_LastTickCount;

	IVertexBufferObject *		m_DynamicVertexBuffer;
	BUFFERALLOCATIONSTRUCT		m_DynamicVertexBufferAllocation;

	bool m_bAddToHierarchy;
	bool m_bTransformIsDirty;
};

}//namespace elemental engine

#endif	// _CParticleBatchRenderObject_H_
