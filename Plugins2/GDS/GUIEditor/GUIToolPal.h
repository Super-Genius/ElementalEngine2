#pragma once

#include "stdafx.h"

class CGUIResBox;
class CGUIInfoBox;
class CGUIPageNameBox;
class CGUIAddElementBox;

class CGUIToolPal : public CGuiDialogBar
{
	DECLARE_DYNAMIC(CGUIToolPal)

public:
	CGUIToolPal();   // standard constructor
	virtual ~CGUIToolPal();

	void OnGlobalUpdateTick( void );
// Dialog Data
	enum { IDD = IDD_GUI_EDITOR_TOOL_PALETTE };

	void SetObjectDockWinReference(ObjectDockWin *ref)
	{
		m_pDockRef = ref;
	}

protected:
	IToolBox *m_ToolBox;

	
	CHashString m_GUIName;
	
	CButton m_ctrlShowToggleButton;
	bool	m_GUIVisible;
//	GUIPropertiesBox m_PropertiesDlg;
	CGUIResBox m_ResBox;
	CGUIInfoBox m_InfoBox;
	CGUIPageNameBox m_PageNameBox;
	CGUIAddElementBox m_AddElemBox;
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void	CreateGUISystem();
	void	RemoveGUISystem();
	void	ToggleGUISystem( bool value );
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg LONG OnInitDialog ( UINT, LONG );
	afx_msg void OnUpdateButtons( CCmdUI* pCmdUI );
	afx_msg void OnBnClickedShowToggle();
	afx_msg void OnBnClickedProperties();
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedSetRes();	
	afx_msg void OnBnClickedShowInfo();
	afx_msg void OnBnClickedNewUI();
	afx_msg void OnBnClickedViewReset();
	afx_msg void OnBnClickedAddButton();
	afx_msg void OnBnClickedAddPage();
	afx_msg void OnBnClickedAddElement();

	void LeftMousePressed();
	void LeftMouseReleased();
	void MouseMove();
	void AddPage(CHashString name);
	void AddElemExternal(GUIPAGEMESSAGE gpm);
	void KeyPressed(char keyDown);
	void KeyReleased(char keyDown);

	ObjectDockWin *m_pDockRef;
	

	void UpdateResolution(GUIWORKINGRES res);

	// used for adding elements
	int m_iStage;
	CHashString m_szAddItemType;
	CHashString m_szAddItemName;
	CHashString m_szSelectedItemName;
	CHashString m_szSelectedItemType;
	CHashString m_szCurrentPage;
	int m_iElementNum;	
	float m_fElementPosX, m_fElementPosY;
	int m_iLastMousePosX, m_iLastMousePosY;

	vector<CHashString> m_PageNames;
	vector<CHashString> m_ObjectNames;
	vector<CHashString> m_ObjectTypes;

	CHashString m_szBGName;
	CHashString m_szBGType;
	CHashString m_szRectName;
	CHashString m_szRectType;

	// key stuffs
	bool m_bSpacePressed;
	bool m_bCtrlPressed;
	bool m_bShiftPressed;
	bool m_bUpPressed;
	bool m_bDownPressed;
	bool m_bLeftPressed;
	bool m_bRightPressed;
	
	// used for grabbing a resize handle
	int m_iHandleIndex;
	
	// resolution display
	UINT m_iResX, m_iResY;

	// mouse position, relative to gui view
	UINT m_iMouseX, m_iMouseY;
};
