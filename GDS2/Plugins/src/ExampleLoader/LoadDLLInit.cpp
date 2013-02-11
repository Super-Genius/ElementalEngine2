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

#include "stdafx.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	IToolBox *toolBox;
	toolBox = EngineGetToolBox();

	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			// on initialization, search for and load all
			// "*.dls" which are dynamic Loader/Saver modules in
			// dll format.
			// force creation of CExampleLoader
			toolBox->CreateComponent(&CHashString("CExampleLoader"), 0);
			return TRUE;
			break;
	}

	return TRUE;
}

