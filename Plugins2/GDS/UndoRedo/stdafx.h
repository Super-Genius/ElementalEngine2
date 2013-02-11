///============================================================================
/// \file		stdafx.h
/// \brief		Precompiled header
/// \date		09/30/2007
/// \author		Andrey Ivanov
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

#ifndef UNDOREDO_STDAFX_H
#define UNDOREDO_STDAFX_H

#if _MSC_VER >= 1400
#define _CRT_SECURE_NO_DEPRECATE 1
#define _SCL_SECURE_NO_DEPRECATE 1
#endif

// Exclude rarely-used stuff from Windows headers
//#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>
#include <afxext.h>         // MFC extensions

#include <afxisapi.h>

//#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "zlib.h"

#include "ElementalEngine.hpp"
#include "CHashString.hpp"

#include "BaseMessages.h"
#include "Messages.h"
#include "MenuUtil.h"
#include "Register.h"
#include "Vec3.h"

#include "IUndoRedoAction.h"
#include "ObjectHelpers.h"
#include "EditorHelpers.h"
#include "ArchiveHelpers.h"
#include "UndoActions.h"
using namespace GDS_SDK_Helpers;

#include "TerrainDefines.h"

#include "Resource.h"
#include "UndoRedoComponent.h"
#include "UndoCommandData.h"
#include "UndoStack.h"
#include "ObjectSerializeAction.h"
#include "TextureDataAction.h"
#include "DeltaDataAction.h"
#include "DependentProcessors.h"
#include "Helpers.h"

extern AFX_EXTENSION_MODULE UndoRedoDLL;

/// \brief	helper function for deleting pointers in container
template<typename T> void Deleter(T * pVal)
{
	delete pVal;
}

#endif // #ifndef UNDOREDO_STDAFX_H