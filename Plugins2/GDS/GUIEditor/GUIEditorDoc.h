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

#if !defined(AFX_GUIEDITORDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_GUIEDITORDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

// GUIEditorDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGUIEditorDoc document

class CGUIPageName;
class CGUIGroupNameBox;
class CGUIUseGroupBox;
class CGUINewButtonBox;
class CGUINewTextBox;
class CGUINewStaticBox;
class CGUIUseItemBox;
class CGUISetBGColorBox;
class CGUITabBar;

class CGUIEditorFrm;
class CGUIFullScreen;

typedef struct COPYPASTESTRUCT
{
	CHashString name;
	CHashString type;
}COPYPASTESTRUCT;

typedef struct INSTDATA
{
	CHashString m_szName;
	CHashString m_szParentName;
}INSTDATA;

typedef vector<INSTDATA> INSTVEC;

typedef map <DWORD, INSTVEC> ITEMMAP;

class CGUIEditorDoc : public CDocument
{
protected:
	
	CGUIEditorDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGUIEditorDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	
	protected:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	
// Implementation
public:
	
	virtual ~CGUIEditorDoc();
	DWORD NewGUISystem(UINT xRes, UINT yRes);
	void LoadGXL(GUIPAGELIST gpl, GUIWORKINGRES gwr);
	BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace);

	// tab bar button presses
	void NewPagePressed();
	void NewGroupPressed();
	void UseGroupPressed();
	void NewButtonPressed();
	void NewTextPressed();
	void NewStaticPressed();
	void UseItemPressed();
	void ChangeBGColorPressed();
	void FullWindowPagePressed();
	void PlayPressed();
	void InstLockPressed();
	void ResolutionUpdated(StdString szResolution);
	void PlayAnimationPressed();
	void StopAnimationPressed();
	void NewAnimationPressed();

	bool CheckNameIsRegistered(CHashString name);
	bool CheckIsGroupInPage(CHashString gName);
	bool CheckIsItemInGroupOrPage(CHashString name);
	void AddPage(CHashString pName);
	void AddGroup(CHashString gName, bool isReference);
	CHashString AddElement(CHashString eName, CHashString eType, bool isReference);	
	void AddAnimation(CHashString aName);

	void DeleteItem(CHashString name, CHashString type);
	void DeleteItemFromTree(CHashString name);

	void SerializeCursor();

	void SetCurrentPage(CHashString name);
	void SetCurrentGroup(CHashString name);
	void SetCurrentObject(CHashString name);
	void SetCurrentAnimation(CHashString name);
	void ClearObjectSelection()
	{
		m_szSelectedItemName = _T("");
		m_szSelectedItemType = _T("");
	}
	void SetIsPageSelected(bool value) { m_bSelectedIsPage = value; }

	void ChangeBGColor(UINT r, UINT g, UINT b);

	void SetToolbar(CGUITabBar *value) { m_TabBar = value; }
	void MouseMove(int x, int y);
	void ZoomChange(float zf);
	void SelectionChange();	

	void ReturnToWindow();

	void CopyCurrent(CHashString name, CHashString type);
	void PasteNewItem();
	void PasteNewInstance();

	void AddWidgets(CHashString page);
	void RemoveWidgets(CHashString page);

	bool RenameItem(CHashString oldName, CHashString newName, bool isTemplateName);	

	void ShowHideGroup(CHashString gName);

	static void UpdateTreeSerializeCallback(StdString name, StdString type, IArchive *ar);

	void SetAnimBarPos(UINT pos);
	void SetKey(StdString objName, StdString objType);
	void AddOneKey(StdString objName, UINT key, UINT channelType);
	void DeleteOneKey(StdString objName, UINT keyInd, UINT channelType);
	void ShowPosData(UINT key, StdString objName, float x, float y);
	void ShowSizeData(UINT key, StdString objName, float width, float height);
	void ShowColorData(UINT key, StdString objName, float a, float r, float g, float b);
	void ShowRotationData(UINT key, StdString objName, float rot);	
	void UpdateKeyData(UINT key, StdString objName, UINT keyType, float v1, float v2, float v3, float v4);

#ifdef _DEBUG
	
	virtual void AssertValid() const;
	
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:

	void Init();
	
	DECLARE_MESSAGE_MAP()

	void CreateWidgets();

	IToolBox *m_ToolBox;

	// dialog boxes
	CGUIPageName m_PageName;
	CGUIGroupNameBox m_GroupNameBox;
	CGUIUseGroupBox m_UseGroupBox;
	CGUINewButtonBox m_NewButtonBox;
	CGUINewTextBox m_NewTextBox;
	CGUINewStaticBox m_NewStaticBox;
	CGUIUseItemBox m_UseItemBox;
	CGUISetBGColorBox m_SetBGColorBox;
	CGUIFullScreen m_FullScreen;
	CNewAnimationBox m_NewAnimationBox;
	CGUIKeyDataView m_KeyDataView;

    CGUIEditorFrm *m_FrmParent;
	CGUIEditorView *m_View;
	CGUITabBar *m_TabBar;

	vector<CHashString> m_vPages;
	vector<CHashString> m_vGroups;
	ITEMMAP m_mItems;
	vector<CHashString> m_vInstances;
	vector<CHashString> m_vAnims;

	CHashString m_szSelectedPage;
	CHashString m_szSelectedGroup;
	CHashString m_szSelectedItemName;
	CHashString m_szSelectedItemType;
	bool m_bSelectedIsPage;

	bool m_bCursorCreated;
	CHashString m_szCursorName;

	UINT m_iXRes;
	UINT m_iYRes;

	bool m_bObjCreated;
    	
	UINT m_iStackPos;
	bool m_bRedo;	

	COPYPASTESTRUCT m_CopyStruct;	

	UINT m_iSystemIndex;

	CHashString m_hszSelectedAnimName;
public:
	virtual void OnCloseDocument();
};

#endif // !defined(AFX_GUIEditorDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
