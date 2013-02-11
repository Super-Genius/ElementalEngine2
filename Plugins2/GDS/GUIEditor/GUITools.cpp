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

#include "stdafx.h"
#include ".\guitools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CGUITools dialog

IMPLEMENT_DYNAMIC(CGUITools, CDialog)
CGUITools::CGUITools(CWnd* pParent /*=NULL*/)
	: CDialog(CGUITools::IDD, pParent)
{
	m_szSelectedPage = _T("");
	m_szSelectedGroup = _T("");
	m_bSelectedIsPage = false;
	m_bDeletePressed = false;
	m_bCursorAdded = false;
	m_Doc = NULL;
	m_ImageList = NULL;
	m_bCtrlPressed = false;
	m_bShiftPressed = false;
	m_bHHeld = false;
	m_bCHeld = false;
	m_bVHeld = false;
}

CGUITools::~CGUITools()
{
	if (m_ImageList)
	{
		delete m_ImageList; 
		m_ImageList = NULL;
	}
}

void CGUITools::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PAGEHIERTREE, *m_PageHierTree);
}


BEGIN_MESSAGE_MAP(CGUITools, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)	
	ON_NOTIFY(TVN_BEGINDRAG, IDC_PAGEHIERTREE, OnTvnBegindragPagehiertree)
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_PAGEHIERTREE, OnTvnSelchangedPagehiertree)
	ON_NOTIFY(TVN_KEYDOWN, IDC_PAGEHIERTREE, OnTvnKeydownPagehiertree)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_PAGEHIERTREE, OnTvnEndlabeleditPagehiertree)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_PAGEHIERTREE, OnTvnBeginlabeleditPagehiertree)	
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CGUITools message handlers

void CGUITools::OnBnClickedOk()
{
	printf("test");
//	OnOK();
}


BOOL CGUITools::OnInitDialog()
{
	m_PageHierTree = (CTreeCtrl*)GetDlgItem(IDC_PAGEHIERTREE);
	m_ImageList = new CImageList();
	m_ImageList->Create(IDB_EYES, 14, 1, RGB(255, 0, 255));
	m_PageHierTree->SetImageList(m_ImageList, TVSIL_NORMAL);
	CImageList *getList = m_PageHierTree->GetImageList(TVSIL_NORMAL);
	return TRUE;
}
void CGUITools::OnTvnBegindragPagehiertree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	*pResult = 0;
}

void CGUITools::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnMouseMove(nFlags, point);	
}

void CGUITools::OnTvnSelchangedPagehiertree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	m_SelectedItem = pNMTreeView->itemNew.hItem;
	UINT pIndex;
	bool isInHierarchy = false;
	for (pIndex = 0; pIndex < m_vAllItems.size(); pIndex++)
	{
		if (m_vAllItems[pIndex].m_Item == m_SelectedItem)
		{
			if (m_vAllItems[pIndex].m_SpecType == PAGE)// selected a page
			{
				m_szSelectedPage = m_vAllItems[pIndex].m_szName;
				m_Doc->SetCurrentPage(m_szSelectedPage);
				m_bSelectedIsPage = true;
				m_Doc->SetIsPageSelected(true);
				m_Doc->SelectionChange();				
				isInHierarchy = true;
			}
			else if (m_vAllItems[pIndex].m_SpecType == GROUP) // selected a group
			{
				m_szSelectedGroup = m_vAllItems[pIndex].m_szName;
				m_Doc->SetCurrentGroup(m_szSelectedGroup);
				m_bSelectedIsPage = false;
				m_Doc->SetIsPageSelected(false);
				m_Doc->SetCurrentObject(m_vAllItems[pIndex].m_szName);
				m_Doc->SelectionChange();
				isInHierarchy = true;
			}
			else if (m_vAllItems[pIndex].m_SpecType == ITEM) // selected an item
			{
				if (m_vAllItems[m_vAllItems[pIndex].m_iParentIndex].m_SpecType == PAGE) // in a page
				{
					m_szSelectedPage = m_vAllItems[m_vAllItems[pIndex].m_iParentIndex].m_szName;
					m_Doc->SetCurrentPage(m_szSelectedPage);
					m_bSelectedIsPage = true;
					m_Doc->SetIsPageSelected(true);
				}
				else if (m_vAllItems[m_vAllItems[pIndex].m_iParentIndex].m_SpecType == GROUP) // in a group
				{
					m_szSelectedGroup = m_vAllItems[m_vAllItems[pIndex].m_iParentIndex].m_szName; // group name
					m_szSelectedPage = m_vAllItems[m_vAllItems[m_vAllItems[pIndex].m_iParentIndex].m_iParentIndex].m_szName; // page name
					m_Doc->SetCurrentGroup(m_szSelectedGroup);
					m_Doc->SetCurrentPage(m_szSelectedPage);
					m_bSelectedIsPage = false;
					m_Doc->SetIsPageSelected(false);
				}
				m_Doc->SetCurrentObject(m_vAllItems[pIndex].m_szName);
				m_Doc->SelectionChange();
				isInHierarchy = true;
			}
			else if (m_vAllItems[pIndex].m_SpecType == CURSOR) // selected the cursor
			{
				m_Doc->SerializeCursor();

				CURSORVISIBLE cv;
				cv.bVisible = true;
				static DWORD msgHash_SetCursorVisible = CHashString(_T("SetCursorVisible")).GetUniqueID();
				EngineGetToolBox()->SendMessage(msgHash_SetCursorVisible, sizeof(CURSORVISIBLE), &cv, &m_vAllItems[pIndex].m_szName, &CHashString(_T("CGUICursor")));
				isInHierarchy = true;
			}
			else if (m_vAllItems[pIndex].m_SpecType == ANIMATION) // selected an animation
			{
				m_Doc->SetCurrentAnimation(m_vAllItems[pIndex].m_szName);
				m_Doc->SelectionChange();
			}
		}
	}
	if (!isInHierarchy)
	{
		for (UINT i=0; i<m_vTemplateItems.size(); i++)
		{
			if (m_SelectedItem == m_vTemplateItems[i].m_Item)
			{
				m_Doc->SetCurrentObject(m_vTemplateItems[i].m_szName);
				m_Doc->SelectionChange();
			}
		}
	}
	*pResult = 0;
}

void CGUITools::AddPage(CHashString pName)
{
	ALLHTREE item;
    m_PageHierTree = (CTreeCtrl*)GetDlgItem(IDC_PAGEHIERTREE);
	TVINSERTSTRUCT tvins;
	tvins.hParent = NULL;
	if (m_vPages.size() > 0)
	{
		tvins.hInsertAfter = m_vPages[m_vPages.size()-1];
	}
	else
	{
		tvins.hInsertAfter = m_Cursor;
	}
	tvins.item.mask = TVIF_TEXT;
	StdString buf = pName.GetString();
	tvins.item.pszText = const_cast<LPSTR>((LPCSTR)buf);
	HTREEITEM treeItem;	
	treeItem = m_PageHierTree->InsertItem(&tvins);	
	m_vPages.push_back(treeItem);

	item.m_Item = treeItem;
	item.m_Parent = NULL;
	item.m_SpecType = PAGE;
	UINT pIndex = m_vAllItems.size();
	item.m_szName = pName;
	m_vAllItems.push_back(item);
    	
	m_PageHierTree->Expand(treeItem, TVE_EXPAND);

	m_Doc->SetCurrentPage(pName);
	m_bSelectedIsPage = true;
	m_Doc->SetIsPageSelected(true);
	m_szSelectedPage = pName;
}

void CGUITools::AddGroup(CHashString page, CHashString gName)
{	
	TVINSERTSTRUCT tvins;
	ALLHTREE gItem, pItem;
	HTREEITEM newGroup;
    UINT pIndex;
	for (pIndex = 0; pIndex < m_vAllItems.size(); pIndex++)
	{
		if (m_vAllItems[pIndex].m_szName == page)
		{
			break;
		}
	}
	pItem = m_vAllItems[pIndex];
	m_PageHierTree = (CTreeCtrl*)GetDlgItem(IDC_PAGEHIERTREE);
	tvins.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvins.hParent = pItem.m_Item;
	
	if (pItem.m_Children.size() > 0)
	{
		tvins.hInsertAfter = pItem.m_Children[pItem.m_Children.size()-1];
	}
	else
	{
		tvins.hInsertAfter = NULL;
	}
	tvins.item.pszText = (TCHAR*)gName.GetString();
	tvins.item.iImage = 0;
	tvins.item.iSelectedImage = 0;
	newGroup = m_PageHierTree->InsertItem(&tvins);

	gItem.m_Parent = &pItem;
	gItem.m_iParentIndex = pIndex;
	gItem.m_SpecType = GROUP;
	gItem.m_Item = newGroup;
	gItem.m_szName = gName;
	m_vAllItems.push_back(gItem);
	pItem.m_Children.push_back(newGroup);

	m_PageHierTree->Expand(pItem.m_Item, TVE_EXPAND);

	m_Doc->SetCurrentGroup(gName);
    m_bSelectedIsPage = false;
	m_Doc->SetIsPageSelected(false);
	m_szSelectedGroup = gName;
}

void CGUITools::AddElement(bool focusIsPage, CHashString bType, CHashString bName)
{
	m_PageHierTree = (CTreeCtrl*)GetDlgItem(IDC_PAGEHIERTREE);
	if (focusIsPage)
	{
		UINT pIndex; 
		for (pIndex=0; pIndex<m_vAllItems.size(); pIndex++)
		{
			if (m_vAllItems[pIndex].m_SpecType == PAGE)
			{
				if (m_vAllItems[pIndex].m_szName == m_szSelectedPage)
				{
					break;
				}
			}
		}
		ALLHTREE page, item;
		page = m_vAllItems[pIndex];
		TVINSERTSTRUCT tvins;
		tvins.hParent = page.m_Item;
		if (page.m_Children.size() > 0)
		{
			tvins.hInsertAfter = page.m_Children[page.m_Children.size()-1];
		}
		else
		{
			tvins.hInsertAfter = NULL;
		}
		tvins.item.mask = TVIF_TEXT;
		tvins.item.pszText = (TCHAR*)bName.GetString();
		HTREEITEM newBt = m_PageHierTree->InsertItem(&tvins);
		page.m_Children.push_back(newBt);
		item.m_Item = newBt;
		item.m_Parent = &page;
		item.m_iParentIndex = pIndex;
		item.m_SpecType = ITEM;
		item.m_szName = bName;
		m_vAllItems.push_back(item);
		m_PageHierTree->Expand(page.m_Item, TVE_EXPAND);
	}
	else
	{
		UINT gIndex; 
		for (gIndex=0; gIndex<m_vAllItems.size(); gIndex++)
		{
			if (m_vAllItems[gIndex].m_SpecType == GROUP)
			{
				if (m_vAllItems[gIndex].m_szName == m_szSelectedGroup)
				{
					break;
				}
			}
		}
		ALLHTREE group, item;
		group = m_vAllItems[gIndex];
		TVINSERTSTRUCT tvins;
		tvins.hParent = group.m_Item;
		if (group.m_Children.size() > 0)
		{
			tvins.hInsertAfter = group.m_Children[group.m_Children.size()-1];
		}
		else
		{
			tvins.hInsertAfter = NULL;
		}
		tvins.item.mask = TVIF_TEXT;
		tvins.item.pszText = (TCHAR*)bName.GetString();
		HTREEITEM newBt = m_PageHierTree->InsertItem(&tvins);
		group.m_Children.push_back(newBt);
		item.m_Item = newBt;
		item.m_Parent = &group;
		item.m_iParentIndex = gIndex;
		item.m_SpecType = ITEM;
		item.m_szName = bName;
		m_vAllItems.push_back(item);
		m_PageHierTree->Expand(group.m_Item, TVE_EXPAND);
	}
}

void CGUITools::AddElemTemplate(CHashString name, CHashString type)
{
	m_PageHierTree = (CTreeCtrl*)GetDlgItem(IDC_PAGEHIERTREE);
	TVINSERTSTRUCT tvins;	
	tvins.hParent = m_TemplateObjects;
    if (m_vTemplateItems.size() > 0)
	{
		tvins.hInsertAfter = m_vTemplateItems[m_vTemplateItems.size()-1].m_Item;	
	}
	tvins.item.mask = TVIF_TEXT;
	tvins.item.pszText = (TCHAR*)name.GetString();
	HTREEITEM ins = m_PageHierTree->InsertItem(&tvins);
	TEMPLATEITEM ti;
	ti.m_Item = ins;
	ti.m_szName = &name;
	m_vTemplateItems.push_back(ti);
}

bool CGUITools::CheckIsGroupInPage(CHashString groupName)
{
	for (UINT i=0; i<m_vAllItems.size(); i++)
	{
		if (m_vAllItems[i].m_szName == groupName)
		{
			ALLHTREE group = m_vAllItems[i];
			if (m_vAllItems[m_vAllItems[i].m_iParentIndex].m_szName == m_szSelectedPage)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}
	
bool CGUITools::CheckIsItemInGroupOrPage(bool focusIsPage, CHashString itemName)
{
	for (UINT i=0; i<m_vAllItems.size(); i++)
	{
		if (m_vAllItems[i].m_szName == itemName)
		{
			if (focusIsPage)
			{
				if (m_vAllItems[m_vAllItems[i].m_iParentIndex].m_szName == m_szSelectedPage)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				if (m_vAllItems[m_vAllItems[i].m_iParentIndex].m_szName == m_szSelectedGroup)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
	return false;
}

void CGUITools::OnTvnKeydownPagehiertree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
	if (pTVKeyDown->wVKey == VK_DELETE)
	{		
		m_PageHierTree = (CTreeCtrl*)GetDlgItem(IDC_PAGEHIERTREE);
		HTREEITEM toDelete = m_PageHierTree->GetSelectedItem();
		for (UINT i=0; i<m_vAllItems.size(); i++)
		{
			if (m_vAllItems[i].m_Item == toDelete)
			{
				if (m_Doc)
				{
					m_Doc->DeleteItemFromTree(m_vAllItems[i].m_szName);
				}
				m_PageHierTree->DeleteItem(toDelete);
				m_vAllItems.erase(m_vAllItems.begin() + i);
				break;
			}
		}
	}
	else if (pTVKeyDown->wVKey != VK_DELETE)
	{
		m_bDeletePressed = false;
	}	
	*pResult = 0;
}

void CGUITools::DeleteItemFromView(CHashString name, CHashString parent)
{
	m_PageHierTree = (CTreeCtrl*)GetDlgItem(IDC_PAGEHIERTREE);
	for (UINT i=0; i<m_vAllItems.size(); i++)
	{
		if (m_vAllItems[i].m_szName == name)
		{
			if (m_vAllItems[m_vAllItems[i].m_iParentIndex].m_szName == parent)
			{
				m_PageHierTree->DeleteItem(m_vAllItems[i].m_Item);
				m_vAllItems.erase(m_vAllItems.begin()+i);
				break;
			}
		}
	}
}

void CGUITools::LoadGXL(GUIPAGELIST gpl)
{
	m_PageHierTree = (CTreeCtrl*)GetDlgItem(IDC_PAGEHIERTREE);
	CHashString pType = _T("CGUIPage");
	CHashString gType = _T("CGUIGroup");
	CHashString cName, rName, cType, rType;
	cName = _T("WorkingCanvas");
	cType = _T("CGUIBackground");
	rName = _T("SelectRect");
	rType = _T("CGUIRect");
	UINT i;

	for (i=0; i<gpl.m_vPages.size(); i++)
	{
		m_szSelectedPage = CHashString(gpl.m_vPages[i]);
		AddPage(m_szSelectedPage);

		GUIELEMLIST pGel;
		static DWORD msgHash_GetPageItemList = CHashString(_T("GetPageItemList")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_GetPageItemList, sizeof(GUIELEMLIST), &pGel, &m_szSelectedPage, &pType);
		
		for (UINT j=0; j<pGel.m_vNames.size(); j++)
		{
			if (_tcscmp(pGel.m_vTypes[j], _T("CGUIGroup")) == 0)
			{
				m_szSelectedGroup = CHashString(pGel.m_vNames[j]);
				AddGroup(m_szSelectedPage, m_szSelectedGroup);
				GUIELEMLIST gGel;
				static DWORD msgHash_GetGroupItemList = CHashString(_T("GetGroupItemList")).GetUniqueID();
				EngineGetToolBox()->SendMessage(msgHash_GetGroupItemList, sizeof(GUIELEMLIST), &gGel, &m_szSelectedGroup, &gType);		
				for (UINT k=0; k<gGel.m_vNames.size(); k++)
				{
					AddElement(false, CHashString(gGel.m_vTypes[k]), CHashString(gGel.m_vNames[k]));
				}
			}	
			else
			{
				AddElement(true, CHashString(pGel.m_vTypes[j]), CHashString(pGel.m_vNames[j]));
			}
		}
		GUIPAGEMESSAGE gpm;
		gpm.compType = &cType;
		gpm.name = &cName;
		static DWORD msgHash_AddGUIElementToPage = CHashString(_T("AddGUIElementToPage")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_AddGUIElementToPage, sizeof(GUIPAGEMESSAGE), &gpm, &m_szSelectedPage, &pType);

		gpm.compType = &rType;
		gpm.name = &rName;
		EngineGetToolBox()->SendMessage(msgHash_AddGUIElementToPage, sizeof(GUIPAGEMESSAGE), &gpm, &m_szSelectedPage, &pType);
	}

	HTREEITEM treeItem = m_PageHierTree->GetRootItem();
	while (treeItem)
	{
		m_PageHierTree->Expand(treeItem, TVE_COLLAPSE);
		treeItem = m_PageHierTree->GetNextSiblingItem(treeItem);
	}
}
void CGUITools::AddCursor(CHashString cName)
{
	m_PageHierTree = (CTreeCtrl*)GetDlgItem(IDC_PAGEHIERTREE);
	TVINSERTSTRUCT tvins;
	ALLHTREE cursor;
	tvins.hInsertAfter = NULL;
	tvins.hParent = NULL;
	tvins.item.mask = TVIF_TEXT;
	tvins.item.pszText = (TCHAR*)cName.GetString();
	
	m_Cursor = m_PageHierTree->InsertItem(&tvins);

	cursor.m_Item = m_Cursor;
	cursor.m_SpecType = CURSOR;
	cursor.m_szName = CHashString(_T("MouseCursor"));
	
	m_vAllItems.push_back(cursor);
}

void CGUITools::OnTvnEndlabeleditPagehiertree(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_PageHierTree = (CTreeCtrl*)GetDlgItem(IDC_PAGEHIERTREE);
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	HTREEITEM renamedItem = pTVDispInfo->item.hItem;
	if ((pTVDispInfo->item.pszText == NULL) || (pTVDispInfo->item.pszText == _T("")))
	{
		return;
	}
	for (UINT i=0; i<m_vAllItems.size(); i++)
	{
		if (m_vAllItems[i].m_Item == renamedItem)
		{
			CHashString newName = pTVDispInfo->item.pszText;
			CHashString oldName = m_vAllItems[i].m_szName;

			if (oldName.GetUniqueID() == newName.GetUniqueID())
			{
				return;
			}
			
			if (m_vAllItems[i].m_SpecType == ITEM)
			{
				StdString oldBaseName = oldName.GetString();
				StdString token;
				oldBaseName.GetToken(_T("_"), token);
				oldBaseName = token;
				m_Doc->RenameItem(CHashString((const TCHAR*)oldBaseName), newName, true);
			}
			else
			{
				m_Doc->RenameItem(m_vAllItems[i].m_szName, newName, false);
			}			
		}
	}
	*pResult = 0;
}

// excellent MSDN bug fix
BOOL CGUITools::PreTranslateMessage(MSG* pMsg)
{
    // If edit control is visible in tree view control, when you send a
    // WM_KEYDOWN message to the edit control it will dismiss the edit
    // control. When the ENTER key was sent to the edit control, the
    // parent window of the tree view control is responsible for updating
    // the item's label in TVN_ENDLABELEDIT notification code.
	m_PageHierTree = (CTreeCtrl*)GetDlgItem(IDC_PAGEHIERTREE);
    if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			CEdit* edit = m_PageHierTree->GetEditControl();
			if (edit)
			{
				edit->SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
				return TRUE;
			}
		}
		else
		{
			switch(pMsg->wParam)
			{
				case VK_CONTROL:
				{
					m_bCtrlPressed = true;
					break;
				}
				case VK_SHIFT:
				{
					m_bShiftPressed = true;
					break;
				}
				case 'H':
				{
					if (m_bCtrlPressed && !m_bHHeld)
					{
						m_bHHeld = true;
						if (m_Doc)
						{
							HTREEITEM selItem = m_PageHierTree->GetSelectedItem();
							for (UINT i=0; i<m_vAllItems.size(); i++)
							{
								if (selItem == m_vAllItems[i].m_Item)
								{
									if (m_vAllItems[i].m_SpecType == GROUP)
									{
										m_Doc->ShowHideGroup(m_vAllItems[i].m_szName);										
									}
									break;
								}
							}
						}
					}
					break;
				}
				case 'C':
				{
					if ((m_bCtrlPressed) && (!m_bCHeld))
					{
						m_bCHeld = true;
						if (m_Doc)
						{
							HTREEITEM selItem = m_PageHierTree->GetSelectedItem();
							for (UINT i=0; i<m_vAllItems.size(); i++)
							{
								if (selItem == m_vAllItems[i].m_Item)
								{
									CHashString type;
									if (m_vAllItems[i].m_SpecType == GROUP)
									{
										type.Init(_T("CGUIGroup"));
									}
									else if (m_vAllItems[i].m_SpecType == ITEM)
									{
										type.Init(_T("CGUIInstance"));
									}
									else
									{
										break;
									}

									m_Doc->CopyCurrent(m_vAllItems[i].m_szName, type);
									break;
								}
							}
						}
					}
					break;
				}
				case 'V':
				{
					if ((m_bCtrlPressed) && (!m_bVHeld))
					{
						m_bVHeld = true;
						if (m_Doc)
						{
							if (m_bShiftPressed)
							{
								m_Doc->PasteNewInstance();
							}
							else
							{
								m_Doc->PasteNewItem();
							}
						}
					}
					break;
				}
				default:
				{
					break;
				}
			}
		}		
	}
	else if (pMsg->message == WM_KEYUP)
	{
		if (pMsg->wParam == VK_CONTROL)
		{
			m_bCtrlPressed = false;			
		}
		else if (pMsg->wParam == VK_SHIFT)
		{
			m_bShiftPressed = false;			
		}
		else if (pMsg->wParam == 'H')
		{
			m_bHHeld = false;
		}
		else if (pMsg->wParam == 'C')
		{
			m_bCHeld = false;
		}
		else if (pMsg->wParam == 'V')
		{
			m_bVHeld = false;
		}
	}
    return CDialog::PreTranslateMessage(pMsg);
}

void CGUITools::RenameExternal(CHashString oldName, CHashString newName)
{
	if (oldName.GetUniqueID() == newName.GetUniqueID())
	{
		return;
	}
	m_PageHierTree = (CTreeCtrl*)GetDlgItem(IDC_PAGEHIERTREE);
	for (UINT i=0; i<m_vAllItems.size(); i++)
	{
		if (m_vAllItems[i].m_szName == oldName)
		{
			m_vAllItems[i].m_szName = newName;
			m_PageHierTree->SetItemText(m_vAllItems[i].m_Item, newName.GetString());
		}
	}
	if (_tcscmp(oldName.GetString(), m_szSelectedPage.GetString()) == 0)
	{
		m_szSelectedPage = newName;
	}
	if (_tcscmp(oldName.GetString(), m_szSelectedGroup.GetString()) == 0)
	{
		m_szSelectedGroup = newName;
	}
}
void CGUITools::OnTvnBeginlabeleditPagehiertree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	m_PageHierTree = (CTreeCtrl*)GetDlgItem(IDC_PAGEHIERTREE);
	CEdit *edit = m_PageHierTree->GetEditControl();
	assert(edit);

	// name should be template name_page name_#, at most
	StdString nameBuf = pTVDispInfo->item.pszText;

	// just get the template name, rename everything at once
	StdString token;
	nameBuf.GetToken(_T("_"), token);
	nameBuf = token;
	edit->SetWindowText(nameBuf);
	*pResult = 0;
}

void CGUITools::ToggleGroupVisible(CHashString gName, bool visible)
{
	m_PageHierTree = (CTreeCtrl*)GetDlgItem(IDC_PAGEHIERTREE);	
	for (UINT i=0; i<m_vAllItems.size(); i++)
	{
		if (_tcscmp(m_vAllItems[i].m_szName.GetString(), gName.GetString()) == 0)
		{
			if (visible)
			{
				m_PageHierTree->SetItemImage(m_vAllItems[i].m_Item, 0, 0);
				m_PageHierTree->Expand(m_vAllItems[i].m_Item, TVE_EXPAND);
			}
			else
			{
				m_PageHierTree->SetItemImage(m_vAllItems[i].m_Item, 1, 1);
				m_PageHierTree->Expand(m_vAllItems[i].m_Item, TVE_COLLAPSE);
			}
			return;
		}
	}
}

void CGUITools::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	m_PageHierTree = (CTreeCtrl*)GetDlgItem(IDC_PAGEHIERTREE);	
	// in case it hasn't been created yet...
	if(m_PageHierTree)
	{
		RECT rect;
		rect.top = rect.left = 0;
		rect.bottom = cy;
		rect.right = cx;
		m_PageHierTree->MoveWindow(&rect, true);
	}
}

void CGUITools::AddAnimation(CHashString name)
{
	m_PageHierTree = (CTreeCtrl*)GetDlgItem(IDC_PAGEHIERTREE);
	TVINSERTSTRUCT tvins;
	ALLHTREE anim;
	tvins.hInsertAfter = NULL;
	tvins.hParent = NULL;
	tvins.item.mask = TVIF_TEXT;
	tvins.item.pszText = (TCHAR*)name.GetString();
	
	HTREEITEM hAnim = m_PageHierTree->InsertItem(&tvins);
	m_vAnims.push_back(hAnim);

	anim.m_Item = hAnim;
	anim.m_SpecType = ANIMATION;
	anim.m_szName = name;
	
	m_vAllItems.push_back(anim);
}