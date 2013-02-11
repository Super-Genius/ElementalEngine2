//---------------------------------------------------------------------
// File:		ResourceListCtrlEx.h
// Description:	MFC List Control Extension that adds resource image dragging functionality
// Created:		07/07/05
// Author:		Brian Bazyk
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
#ifndef _RESOURCELISTCTRLEX_H
#define _RESOURCELISTCTRLEX_H

typedef list<int> INTEGERLIST;

class CResourceListCtrlEx : public CListCtrl
{
public:
	CResourceListCtrlEx();
	~CResourceListCtrlEx();

	/// Insert a resource into the list control
	/// \param iPositionIndex - index of the position in which to insert the new resource
	/// \param szFilePath - relative path of the resource to insert into the list
	/// \return bool - Returns whether resource was successfully inserted
	bool InsertResource( int iPositionIndex, const TCHAR *szFilePath );
	/// Remove a resource from the list control 
	/// \param iIndex - index of the resource to remove
	void RemoveResource( int iIndex );
	/// Checks to see if a resource exists in list
	/// \param szFilePath - file path of resource on which to check existance status
	/// \return bool - return true if resource exists in list, false otherwise.
	bool ResourceExists( const TCHAR *szFilePath );

	/// \brief	Returns a list of visible items
	///	\return	INTEGERLIST - return m_VisibleItems
	INTEGERLIST GetVisibleItems();

	LRESULT OnDropMessage(WPARAM wParam, LPARAM lParam);
	LRESULT OnDragMessage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	CRect m_rectClient;

public:
	/// ???
	int GetDropIndex() { return m_nDropIndex; }

protected:
	/// Create specific thumbnail for this resource type.
	/// \param szFilePath - relative path of the resource to insert into the list
	/// \return CBitmap pinter with thumbnail, NULL if thumbnail not be created.
	virtual CBitmap * CreateThumbnail(const TCHAR *szFilePath);

private:
	/// Sets the sort value of each element in the list to that elements current position
	void ResynchSortValues();
	/// ???
	void CreateThumbnail(HBITMAP hSrc, CBitmap &tBitmap);
	/// ???
	int GetImageIndex( int iItem );
	/// ???
	void SetImageIndex( int iItem, int iImage );

	/// \brief	Calculate a list of visible items in the current view of the listctrl
	void CalculateVisibleItems();


	/// Toolbox access
	IToolBox *m_ToolBox;
	/// Image list of thumbnails to attach to list control
	CImageList m_ThumbNails;
	/// Current drop target
	int m_nDropIndex;

	/// list of visible items in the ListCtrl
	INTEGERLIST m_VisibleItems;

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#endif //_RESOURCELISTCTRLEX_H