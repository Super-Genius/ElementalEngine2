///============================================================================
/// \file		SplinePoint.h
/// \brief		Declaration of CSplinePointObject Class
/// \date		09-10-2005
/// \author		D. Patrick Ghiocel
///============================================================================
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
///============================================================================

#ifndef __SPLINEPOINTOBJECT_H__
#define __SPLINEPOINTOBJECT_H__

#include "CObjectTemplate.hpp"
#include "Vec3.h"

class CSplineManager;

class CSplinePointObject : public CObjectTemplate<IObject>
{
private:
	/// Default Constructor
	CSplinePointObject(IHashString *parentName, IHashString *name, bool bAddToHier);

public:
	/// Default Destructor
	~CSplinePointObject();

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

	void SetPosition( Vec3* pPosition );
	void GetPosition( Vec3* pPosition );

	/// Turns on/off spline rendering
	/// \param value = to render (t/f)
	void SetToRender( bool value );

	// Messages
	/// 
	DWORD OnSetGlobalPosition( DWORD size, void *params );
	DWORD OnGetGlobalPosition( DWORD size, void *params );

private:
	/// Pointed to the EE Tool Box
	IToolBox* m_ToolBox;

	/// The manager
	CSplineManager *m_pManager;

	/// Class Type
	CHashString m_hszNameType;

	/// Pointer to the Render Object
	CSplinePointRenderObject* m_pRenderObject;

	/// Position
	Vec3 m_vPosition;

	bool m_bAddToHierarchy;
};

#endif //__SPLINEPOINTOBJECT_H__

