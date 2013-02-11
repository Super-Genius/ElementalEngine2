///=====================================================================
/// \file	EditSunKeyDialog.h
/// \brief	Definition of EditSunKeyDialog Class
///         This class implements an MFC Dialog Box
/// \date	02/21/2005
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

#ifndef _EDITSUNKEYDIALOG_H_
#define _EDITSUNKEYDIALOG_H_

/// \class EditSunKeyDialog
/// \brief  Creates the EditSunKeyDialog Dialog MFC Box
///			Inherits from CDialog
class EditSunKeyDialog : public CDialog
{
public:
	/// \fn void EditSunKeyDialog(SunlightKeyframe_t in_Keyframe, CWnd* pParent)
	/// \brief Default Constructor
	/// \param SunlightKeyframe_t in_Keyframe - initial dialog data
	EditSunKeyDialog(SUNLIGHTKEYFRAME in_Keyframe, CWnd* pParent);

	/// \fn void ~EditSunKeyDialog()
	/// \brief Default Destructor
	virtual ~EditSunKeyDialog();

	/// \fn SunlightKeyframe_t GetKeyframe()
	/// \brief Returns current internal keyframe data
	/// \return SunlightKeyframe_t - Internal keyframe data
	SUNLIGHTKEYFRAME GetKeyframe();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	// Generated message map functions
	//{{AFX_MSG(EditSunKeyDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	afx_msg void OnBnClickedAmbientColor();
	afx_msg void OnBnClickedFullbrightColor();
	afx_msg void OnEnChangeEditsunkeyDawn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Dialog Control Data
	SUNLIGHTKEYFRAME m_Keyframe;
	CString m_szAmbientText;
	CString m_szFullbrightText;
	afx_msg void OnEnChangeEditsunkeyNight();
};

#endif	//	#ifndef _EDITSUNKEYDIALOG_H_
