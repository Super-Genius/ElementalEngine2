///============================================================================
/// \file		IComboViewObject.h
/// \brief		Header file for combo box object in a view interface
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

#ifndef _ICOMBOVIEWOBJECT_H_
#define _ICOMBOVIEWOBJECT_H_

typedef list<StdString>	COMBOLIST;

class IComboViewObject : public ViewFormatObject
{
public:
	virtual ~IComboViewObject() {}

	/// Get the selected item
	virtual StdString& GetDefSelect() = 0;

	/// Set the selected item
	virtual void SetDefSelect(StdString &select) = 0;

	/// Add a selectable item to the dropdown list
	virtual void AddComboEntry(StdString &entry) = 0;

	/// Get the dropdown list
	virtual COMBOLIST*	GetComboList() = 0;

};

#pragma comment(linker, "/include:_ComboRegistered")

#endif // #ifndef _ICOMBOVIEWOBJECT_H_
