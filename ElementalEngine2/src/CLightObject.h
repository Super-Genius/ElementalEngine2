///==========================================================================
/// \file	CLightObject.h
/// \brief	Implementation of CLightObject interface for rendering
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

#ifndef	_CLIGHTOBJECT_H_
#define _CLIGHTOBJECT_H_

#include <list>
#include "ILightObject.h"
#include "IMeshObject.h"
#include "Vec3.h"
#include "CKeyFrame.h"
using namespace std;


#define MAX_COLOR_CHANNELS 3
#define BLUE 2
#define RED 0
#define GREEN 1

///==========================================================================
/// \class	CLightObject
/// \brief	Generic base class for render objects
///==========================================================================
class CLightObject : public CSceneObject<ILightObject>
{
protected:
	/// \brief The Default Constructor for CLightObject
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CLightObject(IHashString *parentName, IHashString *name, bool bAddToHier);

public:
	/// \brief	The Default Destructor for CLightObject
	~CLightObject();

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

	virtual OBJECTSORTTYPES GetSceneSortID() const { return LIGHTOBJECT3D; }

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
	virtual DWORD GetRenderPriority(){ return 0; }

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

	virtual IMeshObject * GetMeshObject();

	/// Gets the number of Geometry elements this renderobject possesses or if it uses geometry at all	
	virtual UINT GetNumGeometry(){ return 0; }

	/// Grabs the geometry pieces of the render object if there is any uses it for render instead
	/// of the render object render(might need to be changed)
	virtual IGeometry* GetGeometry( int index ){ return NULL; }

	bool IsKindOf( IHashString * compType );

	/// \brief GetBoundingBox grabs the min and max vertices
	/// child should extend off this
	virtual bool	GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation );

	//LightObject Interface
	virtual void SetColor( float r, float g, float b )
	{
		m_Color[0] = r;
		m_Color[1] = g;
		m_Color[2] = b;
	}
	virtual void SetIntensity( float i ) 
	{
		m_Color[3] = i;	
	}
	virtual void SetAttenuationDistance( float distance )
	{
		m_AttenDistance = distance;
	}

	virtual void GetColorIntensity( float r[] )
	{
		r[0] = m_Color[0];
		r[1] = m_Color[1];
		r[2] = m_Color[2];
		r[3] = m_Color[3];
	}
	virtual float GetAttenuationDistance()
	{
		return m_AttenDistance;
	}
	virtual void GetVectorAttenuation( float v[] )
	{
		if( m_LightType == DIRECTIONAL_LIGHT )
		{
			v[0] = m_GlobalDir.x;
			v[1] = m_GlobalDir.y;
			v[2] = m_GlobalDir.z;
			v[3] = -1.f;
		}else
		{
			v[0] = m_GlobalPos.x;
			v[1] = m_GlobalPos.y;
			v[2] = m_GlobalPos.z;
			v[3] = m_AttenDistance;
		}
	}

	virtual void GetPosition( Vec3 &p )
	{
		p = m_GlobalPos;
	}

	virtual void GetDirection( Vec3 &p )
	{
		p = m_GlobalDir;
	}

	/// \brief Sets the position of the render object
	/// \param position Vec3 class reference to fill in
	virtual void SetVector( float x, float y, float z )
	{
		m_GlobalDir.Set( x, y, z );
	}

	virtual void SetLightType( LIGHTOBJECTTYPE type )
	{
		m_LightType = type;
	}
	virtual LIGHTOBJECTTYPE GetLightType( )
	{
		return m_LightType;
	}

	virtual float GetIntensity()
	{
		return m_Color[3];
	}
	/// Attaches a light object to this Render object
	virtual void AttachLight( ILightObject * light ){}

	/// Signals a detach of all light objects
	virtual void DetachAllLights(){}

	
	virtual void SetPosition( const Vec3 &vec );

	virtual void EnableShadows( bool enabled )
	{
		m_bCastShadows = enabled;
	}

	virtual bool CanCastShadows()
	{
		return m_bCastShadows;
	}

	virtual bool IsDetailLight()
	{
		return m_bDetailLight;
	}

	virtual IMaterial * GetMaterial( int index ){ return NULL; }
	virtual UINT GetNumMaterial() { return 0; }

	virtual float GetShadowAttenuation();

	/// Gets a value estimating this light's priority compared to other lights at affecting
	/// an object.
	/// \param objectPos - position of the affected object
	/// \param objectRadius - radius of the affected object
	/// \param out_priority - set by this function to the light's priority in affecting this object.
	/// \return bool - returns whether the object is affected by the light
	virtual bool GetLightPriority( const Vec3 &objectPos, float objectRadius, float &out_priority );

//	DWORD OnMakeDirectional(DWORD size, void *params);
//	DWORD OnGetGlobalPosition(DWORD size, void *params);
//	DWORD OnSetGlobalPosition(DWORD size, void *params);
	DWORD OnSetColor(DWORD size, void *params);
//	DWORD OnMakePointLight(DWORD size, void *params);
	DWORD OnSetAttenuation(DWORD size, void *params);
	DWORD OnEnableLightShadows(DWORD size, void *params);
	/// Ability to set flags
	virtual ISceneSettings * GetSceneSettings(){ return NULL; }

	void UpdateLightInScene( bool updateShadows, int *out_pShadowCastObjectCount );

private:

	LIGHTOBJECTTYPE m_LightType; 
	float m_Color[4];
	Vec3 m_LocalPos;
	Vec3 m_LocalDir;
	Vec3 m_GlobalPos;
	Vec3 m_GlobalDir;
	float m_AttenDistance;
	bool m_bCastShadows;
	bool m_bDetailLight;
	IRenderer *			m_IRenderer;

	bool m_bAddToHierarchy;

	bool m_bAnimated;
	bool m_bSelfDelete;
	CKeyFrameSet m_FadeFrames[ MAX_COLOR_CHANNELS ];

	/// total time of anim
	float m_fTotalTime;

	/// current time of anim
	float m_fCurrentTime;

	/// starting time of anim
	float m_fStartTime;

	int m_Version;
};

#endif	// _CLIGHTOBJECT_H_
