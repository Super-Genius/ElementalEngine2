///============================================================================
/// \file		CWeatherRenderObject.h
/// \brief		Declaration of CWeatherRenderObject Class
/// \date		08-21-2005
/// \author		Marvin Gouw
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

#ifndef __WEATHERRENDEROBJECT_H__
#define __WEATHERRENDEROBJECT_H__

/*
#include "CRenderObject.h"
#include "ITextureObject.h"
#include "IMeshObject.h"
#include <MapWeatherController.h>

class CWeatherRenderObject : public CRenderObject
{
protected:
	/// Default Constructor
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CWeatherRenderObject(IHashString *parentName, IHashString *name);
public:

	/// Default Destructor
	~CWeatherRenderObject();

	/// \brief Create an instance
	/// \return	Pointer to this
	static IComponent *Create(va_list argptr);

	/// \brief DeInit Function
	virtual void DeInit();

	/// \brief Init Function
	virtual void Init();

	/// \brief serialize
	/// \param ar - The Archive
	virtual void Serialize(IArchive &ar);

	/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render( UINT pass );

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
	/// \param position IVec3 class reference to fill in
	virtual void SetPosition( const IVec3 &vec );

	/// \brief Gets the position of the render object
	/// \return IVec3 = position f the object
	virtual IVec3 &GetPosition();

	// TODO: implement if necessary
	virtual IHashString * GetBaseSortClass() { return GetComponentType(); }

	/// \brief returns if this renderobject needs to be sorted in the transparency list
	/// \return bool = true if this object uses transparency
	virtual bool IsAlphaObject() { return false; }

	IMeshObject* GetMeshObject();

	virtual bool IsKindOf( IHashString * compType );


private:
	float				m_fIntensity;
	float				m_fParticleScale;
	WEATHER_EFFECT		m_eWeatherEffect;
	VisMapWeatherController	* m_WeatherController;
};
*/
#endif //__CWeatherRenderObject_H__