///============================================================================
/// \file		SelectNewObjectTypeDlg.h
/// \brief		Header file for <insert brief description here>
/// \date		07-18-2005
/// \author		Kenneth Hurley
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

#ifndef _SELECTNEWOBJECTTYPEDLG_H_
#define _SELECTNEWOBJECTTYPEDLG_H_

// CSelectNewObjectTypeDlg dialog

class CSelectNewObjectTypeDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectNewObjectTypeDlg)

public:
	CSelectNewObjectTypeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectNewObjectTypeDlg();


	/// Get the selection text of the selected item
	/// \param SelectText reference to string to fill in, if something was selected
	/// \return true, if something was selected.
	bool GetSelectionText(CString **SelectText);

// Dialog Data
	enum { IDD = IDD_NEWOBJECT_SELECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnLbnDblclkNewobjectlist();

	DECLARE_MESSAGE_MAP()

private:
	CListBox m_NewObjectList;
	CString m_SelectedText;
	int		m_Selection;
};

#endif // #ifndef _SELECTNEWOBJECTTYPEDLG_H_
