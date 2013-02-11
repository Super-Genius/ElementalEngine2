//---------------------------------------------------------------------
// File:		MenuUtil.h
// Description:	Menu utility functions
// Created:		02/04/04
// Author:		Kenneth L. Hurley
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
//---------------------------------------------------------------------

//=====================================================================
// Function:	MergeMenus
// Description:	merge in a menu into another one
// Parameters:	dMenu = destination menu
//				sMenu = source menu
//				foreStr = String in menu to insert before
// Returns:		0 if error, 1 if worked.
//=====================================================================
int MergeMenus(HMENU *dMenu, HMENU sMenu, LPCTSTR foreStr);

//=====================================================================
// Function:	MergeSubPopupMenu
// Description:	merge a sub popup menu into another menu 
// Parameters:	dMenu = destination menu
//				sMenu = source menu
// Returns:		0 if error, 1 if worked.
//=====================================================================
int MergeSubPopupMenu(HMENU destMenu, HMENU srcMenu);

//=====================================================================
// Function:	MergeSubMenus
// Description:	merge a sub menu into another menu into another one
// Parameters:	dMenu = destination menu
//				sMenu = source menu
//				aftStr = String in menu to insert after
// Returns:		0 if error, 1 if worked.
//=====================================================================
int MergeSubMenus(HMENU *dMenu, HMENU sMenu);

//=====================================================================
// Function:	MergeAccelerators
// Description:	merge in a accelerators into another one
// Parameters:	dAccel = pointer to handle of destination accelerators
//				sAccel = handle of source accelerators to copy from
//				Overwrites dAccel handle with newly merged handle
// Returns:		0 if error, 1 if worked.
//=====================================================================
int MergeAccelerators(HACCEL *dAccel, HACCEL sAccel);

//=====================================================================
/// Merge in a accelerators into another one
/// \param nIDResource - ID Resource to match template to
/// \return NULL if not found, else pointer to template
//=====================================================================
CDocTemplate* FindDocumentTemplate(UINT nIDResource);
