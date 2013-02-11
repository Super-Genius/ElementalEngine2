///============================================================================
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
#include "afxcmn.h"
#include "afxwin.h"


// CGeneralOptions dialog

class CGeneralOptions : public CPropertyPage
{
	DECLARE_DYNAMIC(CGeneralOptions)

public:
	CGeneralOptions();
	virtual ~CGeneralOptions();

	// GeneralOptions window represents informaion, but 
	// data can be changed other places as well. 
	// we call this function before showWindow to refresh
	// data (as init is called only on GDS startup and
	// the options dialog acts as a singleton)
	void updateInformation();

// Dialog Data
	enum { IDD = IDD_GENERAL_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedGeneraloptBrowse();
protected:
	CEdit m_EditDirCtrl;
private:
	CSliderCtrl m_trackToolbarDelay;
};
