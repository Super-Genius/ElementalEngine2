///============================================================================
/// \file		IProjectSettingsPropertyPage.h
/// \brief		Interface of the new Property Page that can be added
///				to the ProjectSettings Property Sheet.
/// \date		01-03-2009
/// \author		Zsolt Matyas
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


#ifndef _IPROJECTSETTINGSPROPERTYPAGE_H
#define _IPROJECTSETTINGSPROPERTYPAGE_H

#include "afxdlgs.h"

class IProjectSettingsPropertyPage : public CPropertyPage
{
public:
	IProjectSettingsPropertyPage(UINT uiCaptionID)
		:CPropertyPage( uiCaptionID ) {}

	// the pointer to the owner
	IComponent* ownerComponent;

	virtual ~IProjectSettingsPropertyPage(void) {}

//---------------------------------------------------------------------
// Function:	ValidateValues
// Description:	This function is called to validate the values on the page.  
// Parameters:	none
// Returns:		TRUE if the page values are valid.
//---------------------------------------------------------------------
	virtual BOOL ValidateValues() = 0;

//---------------------------------------------------------------------
// Function:	RefreshValues
// Description:	This function is called before the property page gets visible.
//				OnInit is not called as they are just hidden, not reinitalized.
//				Necessary, because the values can be loaded or changed any
//				other ways as well, but the property pages are not updated, only 
//				by this function, when the gets visible again.
// Parameters:	none
// Returns:		TRUE if the page values the values are up the date.
//---------------------------------------------------------------------
	virtual BOOL RefreshPageValues() = 0;

};

#endif //_IPROJECTSETTINGSPROPERTYPAGE_H
