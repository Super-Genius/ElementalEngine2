///============================================================================
/// \file		IColorViewObject.h
/// \brief		Interface for color view
/// \date		08/07/2006
/// \author		Nick Tourte
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

#ifndef _ICOLORVIEWOBJECT_H_
#define _ICOLORVIEWOBJECT_H_

class IColorViewObject : public ViewFormatObject
{
public:
	virtual ~IColorViewObject() {}

	/// Sets the default color upon first loading
	/// \param iDefColor - color to set to, packed rgb format
	virtual void SetDefaultColor(int iDefColor) = 0;

	/// Gets the defualt color
	/// \return int color, packed rgb format
	virtual int GetDefaultColor() = 0;

};

#pragma comment(linker, "/include:_ColorRegistered")
#endif // #ifndef _ISPINNERVIEWOBJECT_H_