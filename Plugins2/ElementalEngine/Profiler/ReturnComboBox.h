///============================================================================
/// \file		ReturnComboBox.h
/// \brief		Combo box wrapper that wraps a return edit
/// \date		07/10/2007
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

#ifndef _C_RETURNCOMBOBOX_H_
#define _C_RETURNCOMBOBOX_H_

class CReturnEdit;

class CReturnComboBox : public CComboBox
{
public:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();

private:
	CReturnEdit m_EditCtrl;
};

#endif //_C_RETURNCOMBOBOX_H_