///============================================================================
/// \file		ProfilerManager.h
/// \brief		Manager for external profile window
/// \date		06/11/2007
/// \author		Nick Tourte
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

#ifndef _C_PROFILERMANAGER_H_
#define _C_PROFILERMANAGER_H_

#include "CManager.h"

class CProfilerWindow;

class CProfilerManager : public CManager
{
	SINGLETONCONSTRUCTOROVERRIDE(CProfilerManager);
protected:
	/// Default ctor
	CProfilerManager();
public:

	/// Default Destructor
	~CProfilerManager();

	/// \brief Init Function
	virtual bool Init();

	/// \brief Update function
	virtual void Update(DWORD tickCount);

	/// \brief DeInit Function
	virtual bool DeInit();

	/// \brief Create an instance
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	DWORD OnSetProfilerStrobeTime(DWORD size, void* param);
	DWORD OnShowHideProfilerWindow(DWORD size, void *param);

private:

	void CreateProfilerWindow();
	bool m_bShowWindow;
	
	CProfilerWindow *m_pWindow;

	float m_fUpdateTime;
	float m_fLastTime;

};

#endif