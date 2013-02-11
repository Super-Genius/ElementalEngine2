///============================================================================
/// \file		COptionTreeItemCheckBoxEx.h
/// \brief		Implementation file for COptionTreeItemCheckBoxEx
/// \date		05-03-2005
/// \author		Brian Bazyk
/// \note		Copyright (c) 2004 Signature Devices, Inc.
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