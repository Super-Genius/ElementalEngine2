///============================================================================
/// \file		IProfileDisplayType.h
/// \brief		Interface for profile information display
/// \date		06/14/2007
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

#ifndef _I_PROFILEDISPLAYTYPE_H_
#define _I_PROFILEDISPLAYTYPE_H_

class IProfileDisplayType
{
public:
	/// virtual dtor
	virtual ~IProfileDisplayType() {};

	/// Sorts a selected column based on information displayed
	/// \param column = index of column pressed
	/// \param bAscend = t/f smallest val on top?
	/// \param listCtrl = list control to resort items in
	virtual void SortColumn(UINT column, bool bAscend, CListCtrl &listCtrl) = 0;

	/// Inserts empty columns on init
	/// \param listCtrl = list control to insert columns in
	virtual void InsertColumns(CListCtrl &listCtrl) = 0;

	/// Updates the list control given the map
	/// \param map = profile map with data
	/// \param listCtrl = list control to display data in
	virtual void UpdateProfile(PERFORMANCEPROFILERMAP &map, CListCtrl &listCtrl) = 0;
	
	/// Clears the peak, avg, etc. maps	
	virtual void ClearSubmaps() = 0;
};

#endif //_I_PROFILEDISPLAYTYPE_H_