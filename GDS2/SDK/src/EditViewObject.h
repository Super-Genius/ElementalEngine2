///============================================================================
/// \file		EditViewObject.h
/// \brief		Header file for edit object for views
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

#ifndef _EDITVIEWOBJECT_H_
#define _EDITVIEWOBJECT_H_

class CEditViewObject : public IEditViewObject
{
public:
	CEditViewObject();
	CEditViewObject(StdString &name, StdString &basicType);
	~CEditViewObject();
	/// \brief	Creates an editable item and adds it to the tree
	///			Will also create and return ViewFormatObject containing 
	///			formatting data for the created option tree item.
	/// \param	ar - the xml archive to read data from
	/// \return	ViewFormatObject* - returns a new ViewFormatObject
	static ViewFormatObject* CreateFormatObject(IXMLArchive &ar);
};

#endif // #ifndef _EDITVIEWOBJECT_H_
