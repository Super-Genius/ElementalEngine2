#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CPSDialog dialog

class CPSDialog : public CDialog
{
	DECLARE_DYNAMIC(CPSDialog)

public:
	CPSDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPSDialog();

	/// set whether the dialog should allow new project or just editing
	/// existing project settings
	void SetAllowNewProject(bool allowNew) { m_bAllowNew = allowNew; }

	// Dialog Data
	enum { IDD = IDD_PROJECT_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnOK();
		//CEdit m_sProjectVersion;
	afx_msg void OnBnClickedDirectorySel();

	bool regExpValidate(CString validateThis, CString pattern, CString errorString);

private:

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

	/// set the edits enabled if the project dir gets set
	void EnableInputs ( bool bEnable );

public:
	afx_msg void OnBnClickedButtonListboxAdd();
	CListCtrl m_listAdditionalDLLs;
	afx_msg void OnBnClickedButtonListboxRemove();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonStartupScriptSelector();
	CButton m_StartupScriptOFNButton;
	CButton m_buttonListBoxAddElement;
	CButton m_buttonListBoxRemoveElement;
};
