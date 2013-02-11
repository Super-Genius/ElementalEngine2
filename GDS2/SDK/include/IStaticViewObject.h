///============================================================================
/// \file		IStaticViewObject.h
/// \brief		Header file for view object that are static interface
/// \date		08-03-2005
/// \author		Kenneth Hurley
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

#ifndef _ISTATICVIEWOBJECT_H_
#define _ISTATICVIEWOBJECT_H_

class IStaticViewObject : public ViewFormatObject
{
public:
	~IStaticViewObject() {}
};

#pragma comment(linker, "/include:_StaticRegistered")

#endif // #ifndef _ISTATICVIEWOBJECT_H_
