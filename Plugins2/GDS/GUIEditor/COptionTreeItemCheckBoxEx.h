///============================================================================
/// \file		COptionTreeItemCheckBoxEx.h
/// \brief		Implementation file for COptionTreeItemCheckBoxEx
/// \date		05-03-2005
/// \author		Brian Bazyk
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
#ifndef _COPTIONTREEITEMCHECKBOXEX_H__
#define _COPTIONTREEITEMCHECKBOXEX_H__

/// \class COptionTreeItemCheckBoxEx
/// \brief A wrapper for COptionTreeItemCheckBox that makes
/// a CommonChanges call whenever the checkbox is clicked, not just
/// when the option changes selection focus.
class COptionTreeItemCheckBoxEx : public COptionTreeItemCheckBox
{
public:
protected:
	//{{AFX_MSG(COptionTreeCheckButton)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif