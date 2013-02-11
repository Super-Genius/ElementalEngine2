///============================================================================
/// \file		IEditViewObject.h
/// \brief		Header file for edit object for views
/// \date		08-03-2005
/// \author		Kenneth Hurley
/// \note		Copyright (c) 2005 Signature Devices, Inc.
///============================================================================

#ifndef _IEDITVIEWOBJECT_H_
#define _IEDITVIEWOBJECT_H_

class IEditViewObject : public ViewFormatObject
{
public:
	virtual ~IEditViewObject() {}
};

#pragma comment(linker, "/include:_EditRegistered")

#endif // #ifndef _IEDITVIEWOBJECT_H_
