///============================================================================
/// \file		IFileViewObject.h
/// \brief		Header file for file selection object in a view file
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

#ifndef _IFILEVIEWOBJECT_H_
#define _IFILEVIEWOBJECT_H_

class IFileViewObject : public ViewFormatObject
{
public:
	virtual ~IFileViewObject() {}

	/// Get the default file
	virtual StdString& GetDefFile() = 0;

	/// Set the default file
	virtual void SetDefFile(StdString &defFile) = 0;

	/// Set the default file extension
	virtual void SetDefExt(StdString &defext) = 0;

	/// Set the extension filters
	virtual void SetExtFilter(StdString &extfilter) = 0;

	/// Get the extension filters
	virtual StdString& GetExtFilter() = 0;

	/// Get the default file extension
	virtual StdString& GetDefExt() = 0;

};

#pragma comment(linker, "/include:_FileRegistered")

#endif // #ifndef _IFILEVIEWOBJECT_H_
