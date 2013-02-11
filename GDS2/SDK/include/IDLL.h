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

#ifndef IDLL_H
#define IDLL_H

// are we importing or exporting DLLS?
#ifndef _AFXEXT
#define DLLIMPORT
#else
#undef DLLIMPORT
#endif

#include "DLLVersion.h"

class CMDIFrameWnd;

// Interface for DLLs loaded into system
class AFX_EXT_CLASS IDLL
{
public:
    virtual BOOL Initialize() = 0;
	virtual DWORD GetPriority() = 0;
	virtual CString *GetName() = 0;
};

// These functions must be declared
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);
extern "C" AFX_EXT_API IDLL* CDECL GetDLLInterface();

typedef IDLL* (*GETDLLINTERFACE)();
typedef DWORD (*GETDLLVERSION)();

#endif // CLLINTERFACE_H
