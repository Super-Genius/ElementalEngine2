///============================================================================
/// \file		IRadioViewObject.h
/// \brief		Header file for radio box object in a view interface
/// \date		08-03-2005
/// \author		Kenneth Hurley
/// \note		Copyright (c) 2005 Signature Devices, Inc./// \note   Elemental Engine
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
///			3200 Bridge Parkway Suite 102, Redwood City, CA 94086 USA.///============================================================================

#ifndef _IRADIOVIEWOBJECT_H_
#define _IRADIOVIEWOBJECT_H_

typedef list<StdString> RADIOLIST;

class IRadioViewObject : public ViewFormatObject
{
public:
	virtual ~IRadioViewObject() {}

	/// Get the list of selectable items
	virtual RADIOLIST* GetRadioList() = 0;

	/// Get the selected item
	virtual StdString& GetDefSelect() = 0;

	/// Set the item that is initially selected
	virtual void SetDefSelect(StdString& defSelect) = 0;
	
	/// Add a selectable item
	virtual void AddListEntry(StdString &entry) = 0;

};

#pragma comment(linker, "/include:_RadioRegistered")

#endif // #ifndef _IRADIOVIEWOBJECT_H_
