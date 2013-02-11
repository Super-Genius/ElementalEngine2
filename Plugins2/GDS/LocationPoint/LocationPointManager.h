///============================================================================
/// \file		LocationPointManager.h
/// \brief		Declaration of CLocationPointManager Class
/// \date		12/13/2006
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

#ifndef __LOCATIONPOINTMANAGER_H__
#define __LOCATIONPOINTMANAGER_H__

class CLocationPointManager : public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CLocationPointManager );
	
	// Default Constructor
	CLocationPointManager();
public:
	
	// Default Destructor
	virtual ~CLocationPointManager();

	/// \brief	Function to get this component.
	///			Returns the singleton instance in this case
	static IComponent *Create(int nArgs, va_list argptr);

	virtual void Update(DWORD tickCount);

private:

};
#endif // #ifndef __LocationPointManager_H__