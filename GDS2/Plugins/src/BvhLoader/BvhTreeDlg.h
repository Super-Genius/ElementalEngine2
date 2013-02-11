#pragma once


// CBvhTreeDlg

class CBvhTreeDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CBvhTreeDlg)

public:
	CBvhTreeDlg();
	virtual ~CBvhTreeDlg();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};


