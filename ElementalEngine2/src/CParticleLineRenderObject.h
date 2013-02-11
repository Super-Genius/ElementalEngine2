///==========================================================================
/// \file	CParticleLineRenderObject.h
/// \brief	Implementation of CParticleLineRenderObject interface for rendering
/// \date	3/29/2007
/// \author	Brian Bazyk
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

#ifndef	_CParticleLineRenderObject_H_
#define _CParticleLineRenderObject_H_

#include "CRenderObject.h"
#include "IMeshObject.h"
#include "CParticleLineType.h"

using namespace std;

namespace ElementalEngine 
{

struct PARTICLELINEDESC
{
};

///==========================================================================
/// \class	CParticleLineRenderObject
///==========================================================================
class CParticleLineRenderObject: public CRenderObject<>
{
protected:
	/// \brief The Default Constructor for CParticleLineRenderObject
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CParticleLineRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier);

public:
	/// \brief	The Default Destructor for CParticleLineRenderObject
	~CParticleLineRenderObject();

	/// \brief	Create the a instance of IObject
	/// \return	IComponent pointer
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Initialization for the Object
	/// \param	IManager* pManager: Parent Manager
	virtual void Init();
	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();
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

	/// \brief Gets the number of Geometry elements this renderobject possesses or if it uses geometry at all	
	virtual UINT GetNumGeometry(){ return 0; }

	/// \brief Grabs the geometry pieces of the render object if there is any uses it for render instead
	/// of the render object render(might need to be changed)
	virtual IGeometry* GetGeometry( int index ){ return NULL; }

	/// \brief Is Kind Of Component
	/// \return - IHashString to Component Type
	virtual bool IsKindOf( IHashString * compType )
	{
		return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
	}

	/// \brief MSG To Enable Particle
	/// \params size - 0
	/// \params params - null
	/// \return - MSG_XXX
	DWORD OnEnable(DWORD size, void *params);
	/// \brief MSG To Disable Particle
	/// \params size - 0
	/// \params params - null
	/// \return - MSG_XXX
	DWORD OnDisable(DWORD size, void *params);
	/// \brief MSG To Set Particle Length
	/// \params size - 0
	/// \params params - null
	/// \return - MSG_XXX
	DWORD OnSetLength( DWORD size, void *params );
	/// \brief MSG To Set Particle Speed
	/// \params size - 0
	/// \params params - null
	/// \return - MSG_XXX
	DWORD OnSetParticleLineSpeed( DWORD size, void *params );

	bool IsInScene( void )
	{
		return m_bInScene;
	}

	int UpdateBuffer( char ** pDest, int offset, IVertexBufferObject * vb);
	virtual void Enable( bool enable )
	{
		m_bEnabled = enable;
	}

	const CParticleLineType* GetParticleLineType( void );
private:
	float				m_CurrentTick;
	bool				m_bEnabled;
	bool				m_bInScene;
	float				m_TimeDelta;
	IRenderer *			m_IRenderer;
	IVertexBufferObject *	m_VB;
	int					m_VBOffset;

	float				m_LastTickCount;

	// typename for Camera object for messages
	CHashString m_ccamTypeName;

	bool m_bAddToHierarchy;

	/// our type's type name
	CHashString m_szTypeTypeName;
	/// name of our type
	CHashString m_szTypeName;
	/// quick access to type data
	const CParticleLineType *m_pType;
	/// global position
	Vec3 m_Position;
	/// local position
	Vec3 m_LocalPos;
	/// global direction
	Vec3 m_GlobalDir;
	/// length scaler
	float m_fLengthScale;
	/// life of particle line
	float m_fLife;
	/// color of head
	DWORD m_HeadColor;
	/// color of tail
	DWORD m_TailColor;
	/// time since Init(), used for animation
	float m_fTime;
	/// particle line speed
	float m_fSpeed;
};

}//namespace elemental engine

#endif	// CParticleLineRenderObjectOBJECT_HPP
