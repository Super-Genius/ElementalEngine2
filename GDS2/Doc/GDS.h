#ifndef _GDS_H
#define _GDS_H

// Date, Time & Version defines
#define GDS_DATE    20031126
#define GDS_TIME    141154
#define GDS_VERSION 0

// Context define declarations

// Forward extern class declarations

//@START_USER1
//@END_USER1

// Defines needed for relations between templated classes
#define _CBT_CDLLINTERFACE_PTR__CDLLINTERFACE_PTR__BOOL	<CDLLInterface *, CDLLInterface *, bool>

// Forward class declarations
class CChildFrame;
class CGDSApp;
class ltstr;
class CMainFrame;
class CObjectBar;
class CObjectManager;
class CObjectOptions;
class CObjectPage;
class COptionsDialog;
class COutput;
class CPushRoutingFrame;
class lessThan;
class CAboutDlg;
class tbblabel;
class CBaseDLLInterface;
class CBaseObject;
class CCamera;
class OPTIONINFO;
class CConfig;
class OVERRIDECLASS;
class CDLLInterface;
class *;
class CProgramInfo;
class CDragListBoxEx;
class CHardwareStream;
class CHardwareVertexStream;
class CHardwareIndexStream;
class CHoverButton;
class CMatrix3;
class CMatrix4;
class CMemDC;
class DATASTREAM;
class CMesh;
class OBJECTSTRUCT;
class DROPOBJECTINFO;
class HIEROBJMESSINFO;
class OBJECTINFO;
class CObjectDLLInterface;
class CPlane;
class CQuaternion;
class CRender;
class CRenderContext;
class CRenderDLLInterface;
class CTransform;
class CVector3;
class CVector4;

// Needed ClassBuilder include files

// Make sure the inline implementations are skipped
#ifdef CB_INLINES
#undef CB_INLINES
#endif

// Include classes, for declarations
#include "CChildFrame.h"
#include "CGDSApp.h"
#include "ltstr.h"
#include "CMainFrame.h"
#include "CObjectBar.h"
#include "CObjectManager.h"
#include "CObjectOptions.h"
#include "CObjectPage.h"
#include "COptionsDialog.h"
#include "COutput.h"
#include "CPushRoutingFrame.h"
#include "lessThan.h"
#include "CAboutDlg.h"
#include "tbblabel.h"
#include "CDLLInterface.h"
#include "CBaseDLLInterface.h"
#include "CBaseObject.h"
#include "CCamera.h"
#include "OPTIONINFO.h"
#include "CConfig.h"
#include "OVERRIDECLASS.h"
#include "*.h"
#include "CProgramInfo.h"
#include "CDragListBoxEx.h"
#include "CHardwareStream.h"
#include "CHardwareVertexStream.h"
#include "CHardwareIndexStream.h"
#include "CHoverButton.h"
#include "CMatrix3.h"
#include "CMatrix4.h"
#include "CMemDC.h"
#include "DATASTREAM.h"
#include "CMesh.h"
#include "OBJECTSTRUCT.h"
#include "DROPOBJECTINFO.h"
#include "HIEROBJMESSINFO.h"
#include "OBJECTINFO.h"
#include "CObjectDLLInterface.h"
#include "CPlane.h"
#include "CQuaternion.h"
#include "CRender.h"
#include "CRenderContext.h"
#include "CRenderDLLInterface.h"
#include "CTransform.h"
#include "CVector3.h"
#include "CVector4.h"


// Include classes again, for inline implementation
#define CB_INLINES

//@START_USER2
//@END_USER2

#endif
