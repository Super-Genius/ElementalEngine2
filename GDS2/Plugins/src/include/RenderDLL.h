//=====================================================================
// File:		RenderDLL.h
// Description:	Interface header for rendering dll's that are run in
//				the context of the main application
// Started:		Kenneth Hurley
//				10/13/2002
//=====================================================================

#ifndef RENDERDLL_H
#define RENDERDLL_H

#include "DLLInterface.h"
#pragma warning(disable : 4786)
#include "Render.h"

class AFX_EXT_CLASS CRenderDLLInterface : public CDLLInterface
{
	// attributes/properties
public:
	// functions
public:
    virtual CString *GetAPIName() = 0;
    virtual CRender *GetRenderer() = 0;

};


#undef AFX_DATA
#define AFX_DATA

#endif	// #ifdef RENDERDLL_H