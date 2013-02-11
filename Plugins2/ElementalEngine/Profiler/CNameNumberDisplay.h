///============================================================================
/// \file		CNameNumberDisplay.h
/// \brief		Implements IProfileDisplay for data with name and times
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

#ifndef _C_NAMENUMBERDISPLAY_H_
#define _C_NAMENUMBERDISPLAY_H_

class CNameNumberDisplay : public IProfileDisplayType
{
public:
	/// ctor / dtor
	CNameNumberDisplay();
	virtual ~CNameNumberDisplay();

	/// Sorts a selected column based on information displayed
	/// \param column = index of column pressed
	/// \param bAscend = t/f smallest val on top?
	/// \param listCtrl = list control to resort items in
	virtual void SortColumn(UINT column, bool bAscend, CListCtrl &listCtrl);

	/// Inserts empty columns on init
	/// \param listCtrl = list control to insert columns in
	virtual void InsertColumns(CListCtrl &listCtrl);

	/// Updates the list control given the map
	/// \param map = profile map with data
	/// \param listCtrl = list control to display data in
	virtual void UpdateProfile(PERFORMANCEPROFILERMAP &map, CListCtrl &listCtrl);

	/// Clears the peak, avg, etc. maps	
	virtual void ClearSubmaps();

private:
	PEAKTIMEMAP m_PeakTimes;
	AVGTIMEMAP m_AvgTimes;

	UINT m_iNumColumns;
	UINT m_iMaxAvgStep;
};

#endif //_C_NAMENUMBERDISPLAY_H_