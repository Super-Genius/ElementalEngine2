///============================================================================
/// \file		ResourceView.h
/// \brief		Header file for ResourceView
///				This view is responsible for displaying the thumbnails of the 
///				different textures, models, scripts, etc... available
/// \date		11-17-2005
/// \author		Halbert Nakagawa
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

#pragma once

// CResourceView view

class CResourceView : public CView
{
	DECLARE_DYNCREATE(CResourceView)

protected:
	CResourceView();           // protected constructor used by dynamic creation
	virtual ~CResourceView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

private:
	CPropertySheet *m_pPropSheet;
	CImageList *m_pIcons;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
};


