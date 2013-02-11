///============================================================================
/// \file		SplineManager.h
/// \brief		Declaration of CSplineManager Class
/// \date		09-11-2005
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

#ifndef __SPLINEMANAGER_H__
#define __SPLINEMANAGER_H__

#include "CManager.h"

typedef map<DWORD, CSplineObject*> SPLINEOBJECTMAP;

class CSplineManager : public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CSplineManager );

	/// \brief	Default Destructor
	///			Since CBoundingBoxManager is implemented as a singleton the
	///			constructor is private.
	CSplineManager();

public:

	/// Default Destructor
	virtual ~CSplineManager();

	/// Function to get this component.
	///			Returns the singleton instance in this case
	static IComponent *Create( int nArgs, va_list argptr );

	/// Returns global render flag
	/// \return to render splines globally (t/f)
	bool GetGlobalRenderFlag();

	// **** MESSAGES **** //

	/// Sets all spline objects in map to render
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_STOP
	DWORD OnShowAllSplines(DWORD size, void *param);

	/// Sets all spline objects in map to not render
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_STOP
	DWORD OnHideAllSplines(DWORD size, void *param);

private:
	/// Turns on/off spline rendering
	/// \param value = to render (t/f)
	DWORD SetToRender( bool value );

	/// Hashstring for Class this Manager handles
	static CHashString m_hszSplineManagerClass;
	static CHashString m_hszSplineClass;
	static CHashString m_hszSplinePointClass;
	lua_State *m_LuaState;

	/// Flag set if rendering is enabled or disabled
	bool m_bToRender;

};

#endif //__SPLINEMANAGER_H__