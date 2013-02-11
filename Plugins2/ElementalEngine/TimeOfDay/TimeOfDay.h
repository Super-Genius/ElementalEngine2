///============================================================================
/// \file		TimeOfDay.h
/// \brief		Declaration of CTimeOfDayObject Class
/// \date		07-21-2005
/// \author		D. Patrick Ghiocel
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

#ifndef __TIMEOFDAYOBJECT_H__
#define __TIMEOFDAYOBJECT_H__

#include "CObjectTemplate.hpp"

class CTimeOfDayObject : public CSceneObject<IRenderObject>
{
private:
	/// Default Constructor
	CTimeOfDayObject(IHashString *parentName, IHashString *name, bool bAddToHier);	
	/// Default Destructor
	~CTimeOfDayObject();

public:
	static IComponent *Create(int nArgs, va_list argptr);

	/// Init
	virtual void Init();
	/// DeInit
	virtual void DeInit();

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);


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
	/// \return UINT = the number of passes that this object has to go through
	virtual UINT GetNumPass();

	/// \brief Sets the position of the render object
	/// \param position Vec3 class reference to fill in
	virtual void SetPosition( const Vec3 &position );

	/// \brief Gets the position of the render object
	/// \return Vec3 = position f the object
	virtual Vec3 &GetPosition();

	/// \brief Gets bounding sphere of object. This is a 3D interface call that should be implemented for only
	///			3D objects. Might want to seperate more of this later
	/// \param position = vector that will be filled the center of the sphere
	/// \param radius = float that will be filled with the radius of the sphere
	virtual void GetBoundingSphere( Vec3 &position, float &radius );

	/// \brief Gets the base sort class which implements IRenderObject::IsLessThan()
	/// child classes must not re-implement IsLessThan() if they return the BaseSortClass hash!!
	/// typically, each render object has a IsLessThan() and returns a unique BaseSortClass
	virtual IHashString * GetBaseSortClass();

	/// \brief returns if this renderobject needs to be sorted in the transparency list
	/// \return bool = true if this object uses transparency
	virtual bool IsAlphaObject();

	/// \brief Gets the internal mesh
	virtual IMeshObject * GetMeshObject();

	/// Gets the number of Geometry elements this renderobject possesses or if it uses geometry at all	
	virtual UINT GetNumGeometry(){ return 0; }

	/// Grabs the geometry pieces of the render object if there is any uses it for render instead
	/// of the render object render(might need to be changed)
	virtual IGeometry* GetGeometry( int index ){ return NULL; }

	virtual void AttachLight( ILightObject * light ) { }
	virtual void DetachAllLights() { }
	
	virtual IMaterial * GetMaterial( int index ){ return NULL; }
	virtual UINT GetNumMaterial() { return 0; }
	virtual ISceneSettings * GetSceneSettings(){ return NULL; }

	/// \brief GetBoundingBox default implementation
	/// child should extend off this
	virtual bool GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation )
	{	
		return false;
	}
	// --- Messages ---
	DWORD OnGetTimeOfDayDesc( DWORD size, void *in_params );
	DWORD OnSetTimeOfDayStart( DWORD size, void *in_params );
	DWORD OnGetTimeOfDayStart( DWORD size, void *in_params );
	DWORD OnGetTimeOfDay( DWORD size, void *in_params );
	DWORD OnSetTimeOfDay( DWORD size, void *in_params );
	DWORD OnGetTimeOfDayRate( DWORD size, void *in_params );
	DWORD OnSetTimeOfDayRate( DWORD size, void *in_params );
	DWORD OnSetTimeOfDayMotion( DWORD size, void *in_params );
	DWORD OnSetTimeOfDayDoLighting( DWORD size, void *in_params );
	DWORD OnSetTimeOfDayDoFog( DWORD size, void *in_params );

	/// Helpful Define for Midnight
	static const float m_kMidnight;
	/// Helpful Define for Morning
	static const float m_kMorning;
	/// Helpful Define for Noon
	static const float m_kNoon;
	/// Helpful Define for Afternoon
	static const float m_kAfternoon;
	/// Helpful Define for Dusk
	static const float m_kDusk;

private:
	void UpdateTime();
	void UpdateLookupTexture();
	void EvaluateSunDirAtTime( float fTime, Vec3 &vecPos );
	void EvaluateColor( float in_fFalloff, float in_fTime, Vec3 &out_Color );
	void EvaluateSkyWeights( float in_fTime, float &out_fDawnWeight, float &out_fNightWeight );

	/// Pointed to the EE Tool Box
	IToolBox *m_ToolBox;

	/// Pointer to the IRenderer
	IRenderer * m_IRenderer;

	/// Class Type
	CHashString m_hszNameType;

	/// Position
	Vec3 m_vPosition;

	/// SunLight Keyframes
	SUNLIGHTKEYFRAMELIST m_vSunLightFrames;

	/// Fog Keyframes
	FOGKEYFRAMELIST m_vFogFrames;

	/// Current time of day [0,1] where 0 and 1 are midnight and 0.5 is noon
	float m_fTime;
	
	/// Current rate at which time flows.  1.0f is realtime.
	float m_fTimeRate;

	/// Time Start (0 - 1 = 0:00 - 24:00)
	float m_fTimeStart;

	/// Motion (advance Time Of Day)
	bool m_bMotion;

	/// Fog (enabled/disabled)
	bool m_bFog;

	/// Lighting (enabled/disabled)
	bool m_bLighting;

	/// Tick count used to keep time
	DWORD m_dwTickCount;
	
	/// Time of day lookup texture name
	CHashString m_szLookupTexture;
	
	/// Time of day lookup texture interface
	ITextureObject *m_pLookupTexture;

	CTimeOfDayShaderCallback *m_TODShaderCallback;

	bool m_bAddToHierarchy;
};

#endif //__TIMEOFDAYOBJECT_H__

