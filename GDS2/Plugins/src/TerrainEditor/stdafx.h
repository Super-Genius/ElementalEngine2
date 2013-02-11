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
#pragma once

#if _MSC_VER >= 1400
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0501		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0501 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0501	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <assert.h>

#include "Resource.h"
#include "GuiLib.h"			// GuiLib includes
#include "ElementalEngine.hpp"
#include "EE-SDK.h"
#include "BaseMessages.h"
#include "EEDefines.h"
#include "Terrain/TerrainDefines.h"
#include "Physics/PhysicsDefines.h"
#include "Singleton.h"
#include "CHashString.hpp"
#include "Register.h"
#include "IDLL.h"
#include "Messages.h"
#include "GDALIO.h"
#include "MenuUtil.h"
#include "ITextureObject.h"
#include "IMeshObject.h"
#include "CObjectTemplate.hpp"
#include <afxdlgs.h>
#include "ObjectHelpers.h"
#include "UndoHelpers.h"
using namespace GDS_SDK_Helpers;

#include "logger.h"

typedef vector<TERRAINLAYERINFOPARAMS::TerrainLayer_t> TERRAINLAYERVEC;
typedef vector<TERRAINLAYERINFOPARAMS::TerrainLayerMask> TERRAINLAYERMASKVEC;

enum EnumTerrainTool
{
	kTerrainToolSelect,
	kTerrainToolPaint, 
	kTerrainToolDeform,
};

#include "Lightmap/LightmapDefines.h"
#include "Cal3dModels/Cal3dModelsDefine.h"
#include "resource.h"
#include "GenerateLightmap.h"
#include "OpenTextureFileDialog.h"
#include "BrushObject.h"
#include "TerrainComponent.h"
#include "ResourceListCtrlEx.h"
#include "TerrainTextureListCtrl.h"
#include "TerrainLayerMaterialType.h"
#include "TerrainLayerMatDocWnd.h"
#include "TerrainOptionsDlg.h"
#include "TerrainParametersDialog.h"
#include "TerrainSizeDlg.h"
#include "TerrainToolPal.h"
#include "SectorRenameVisitor.h"
#include "SectorModifyVisitor.h"
#include "TerrainEditor.h"
