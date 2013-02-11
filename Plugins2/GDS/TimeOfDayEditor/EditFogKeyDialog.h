///=====================================================================
/// \file	EditFogKeyDialog.h
/// \brief	Definition of EditFogKeyDialog Class
///         This class implement an MFC Dialog Box
/// \date	02/22/2005
/// \author	Brian Bazyk
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
///=====================================================================

#ifndef _EDITFOGKEYDIALOG_H_
#define _EDITFOGKEYDIALOG_H_

/// \class EditFogKeyDialog
/// \brief  Creates the EditFogKeyDialog Dialog MFC Box
///			Inherits from CDialog
class EditFogKeyDialog : public CDialog
{
public:
	/// \fn void EditFogKeyDialog(FogKeyframe_t in_Key, CWnd* pParent)
	/// \brief Default Constructor
	/// \param FogKeyframe_t in_Key - initial dialog data
	EditFogKeyDialog(FOGKEYFRAME in_Key, CWnd* pParent);

	/// \fn void ~EditFogKeyDialog()
	/// \brief Default Destructor
	virtual ~EditFogKeyDialog();

	/// \fn FogKeyframe_t GetKeyframe()
	/// \brief Returns current internal keyframe data
	/// \return FogKeyframe_t - Internal keyframe data
	FOGKEYFRAME GetKeyframe();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	// Generated message map functions
	//{{AFX_MSG(EditFogKeyDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	afx_msg void OnBnClickedFogColor();
	afx_msg void OnEnChangeEditfogkeyFogstart();
	afx_msg void OnEnChangeEditfogkeyFogend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Dialog Control Data
	FOGKEYFRAME m_Keyframe;
	CString m_szFogColorText;
};

#endif		//#ifndef _EDITFOGKEYDIALOG_H_
