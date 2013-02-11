///============================================================================
/// \file		ProjectEditorDefaultPropertyPage.hpp
/// \brief		Declaration of the new project settings base property page.
/// \date		01-03-2009
/// \author		Zsolt Matyas
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

#ifndef _PROJECTEDITORDEFAULTPROPERTYPAGE_H
#define _PROJECTEDITORDEFAULTPROPERTYPAGE_H

class CProjectEditorDefaultPropertyPage : public IProjectSettingsPropertyPage
{
	DECLARE_DYNAMIC(CProjectEditorDefaultPropertyPage)

public:
	CProjectEditorDefaultPropertyPage(void);
	~CProjectEditorDefaultPropertyPage(void);

	// Dialog Data
	enum { IDD = IDD_PROJECT_SETTINGS };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();

	//Message handlers:
	afx_msg void OnBnClickedDirectorySel();
	afx_msg void OnBnClickedButtonListboxAdd();
	afx_msg void OnBnClickedButtonListboxRemove();
	afx_msg void OnBnClickedButtonStartupScriptSelector();

	DECLARE_MESSAGE_MAP()

private:
	///////////////////////////////////////////////////
	// fields for editing the dialog values
	// editing or creating the project settings?
	bool m_bAllowNew;

	/// The version combobox on the form
	CComboBox m_sVersion;

	/// The Project Name edit on the form
	CEdit m_sProjectName;

	/// The Project Directory edit on the form
	CEdit m_ProjectDirEdit;

	/// The project settings variable (Singleton)
	CProjectSettings *m_projectSettings;

	/// The startup Script edit of the form
	CEdit m_szProjectStartupScript;

	/// List of additional DLL's
	CListCtrl m_listAdditionalDLLs;

	/// Buttons
	CButton m_StartupScriptOFNButton;
	CButton m_buttonListBoxAddElement;
	CButton m_buttonListBoxRemoveElement;

	//////////////////////////////////////////////////////
	//HELPER functions

	/// regular expression validator
	bool regExpValidate(CString validateThis, CString pattern, CString errorString);

public:
	/// set the edits enabled if the project dir gets set
	void EnableInputs ( bool bEnable );

	/// comming from the absract baseclass
	virtual BOOL ValidateValues();

	/// comming from the absract baseclass
	virtual BOOL RefreshPageValues();
};

#endif /* #ifndef _PROJECTEDITORDEFAULTPROPERTYPAGE_H */