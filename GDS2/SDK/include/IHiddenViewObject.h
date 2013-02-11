///============================================================================
/// \file		IHiddenViewObject.h
/// \brief		Header file for view object that are hidden interface
/// \date		02-09-2007
/// \author		Brian Bazyk
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

#ifndef _IHIDDENVIEWOBJECT_H_
#define _IHIDDENVIEWOBJECT_H_

class IHiddenViewObject : public ViewFormatObject
{
public:
	virtual ~IHiddenViewObject() {}
};

#pragma comment(linker, "/include:_HiddenRegistered")

#endif // #ifndef _IHIDDENVIEWOBJECT_H_
