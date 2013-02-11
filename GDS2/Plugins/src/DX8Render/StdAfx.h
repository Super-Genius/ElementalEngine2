// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__55FDC6E7_E4EA_4848_979C_97D5F9C9E70E__INCLUDED_)
#define AFX_STDAFX_H__55FDC6E7_E4EA_4848_979C_97D5F9C9E70E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxadv.h>         // MFC extensions

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

#include <afxrich.h>
#include <afxpriv.h>

// bug in visual studio with 255 characters in template functions.
// for some reason <map> ignores this warning
#pragma warning(disable : 4786)

#include "logger.h"
#include "Resource.h"
#include "IDLL.h"
#include "IBaseObject.h"
#include "IVector.h"
#include "IQuaternion.h"
#include "IMatrix.h"
#include "IPlane.h"
#include "IHardwareStream.h"
#include "ICamera.h"
#include "Render.h"
#include <d3d8.h>
#include <dxerr8.h>
#include "DX8RenderContext.h"
#include "DX8RenderCore.h"
#include "DX8Render.h"
#include "messages.h"
#include "DX8Mesh.h"

#define CHECKHRESULT(x, value)                                  \
{                                                               \
    HRESULT hr;                                                 \
                                                                \
    hr = (x);                                                   \
    if (FAILED(hr))                                             \
    {                                                           \
        gLogger->Print((LPTSTR)DXGetErrorDescription8(hr));     \
        return(##value);                                        \
    }                                                           \
}


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__55FDC6E7_E4EA_4848_979C_97D5F9C9E70E__INCLUDED_)
