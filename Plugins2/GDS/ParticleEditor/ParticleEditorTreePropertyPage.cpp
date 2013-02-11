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
#include "ParticleEditor.h"
#include "ParticleEditorTreePropertyPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CParticleEditorTreePropertyPage dialog

CParticleEditorTreePropertyPage::CParticleEditorTreePropertyPage()
	: CPropertyPage(CParticleEditorTreePropertyPage::IDD)
{
}

CParticleEditorTreePropertyPage::~CParticleEditorTreePropertyPage()
{
}

void CParticleEditorTreePropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PARTICLE_TREEDOCKBARCTRL, m_TreeCtrl);
}

BEGIN_MESSAGE_MAP(CParticleEditorTreePropertyPage, CPropertyPage)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_PARTICLE_TREEDOCKBARCTRL, OnTvnSelchangedParticleTreedockbarctrl)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// CParticleEditorTreePropertyPage message handlers

void CParticleEditorTreePropertyPage::OnSize(UINT nType, int cx, int cy)
{
	CRect rect;

	CPropertyPage::OnSize(nType, cx, cy);
	
	GetClientRect(rect);
	if ( m_TreeCtrl.m_hWnd != NULL )
	{
		m_TreeCtrl.MoveWindow(rect);
	}
}

void CParticleEditorTreePropertyPage::OnTvnSelchangedParticleTreedockbarctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	StdString szItem;
	StdString szItemParent;
	GetItemAndParentStrings(&szItem, &szItemParent);

	CParticleDoc * doc = CParticleDoc::GetActiveDocument();
	if (doc)
		doc->SetGraphData(0, szItem, szItemParent);
	
	*pResult = 0;
}

void CParticleEditorTreePropertyPage::GetItemAndParentStrings(StdString * szItem, StdString * szItemParent)
{
	HTREEITEM hItem = m_TreeCtrl.GetSelectedItem();
	*szItem = m_TreeCtrl.GetItemText(hItem);
	HTREEITEM hItemParent = m_TreeCtrl.GetParentItem(hItem);
	*szItemParent = m_TreeCtrl.GetItemText(hItemParent);
}

void CParticleEditorTreePropertyPage::GetWorkingData(bool &bEmitter, int &iBrush, PARTICLE_EMITTER_VARIABLE &eEmitterProperty, PARTICLE_VARIABLE &eBrushProperty )
{
	IParticleEmitter * pEmitter;
	CParticleDoc * pDoc = CParticleDoc::GetActiveDocument();
	
	if (!pDoc)
		return;

	pEmitter = pDoc->GetEmitterPointer();
	CHashString hszEmitterFilepath = pDoc->GetPathName();

	CREATE_PARTICLE_STRING_TABLE(ParticlePropertyStringMap);
	CREATE_EMITTER_STRING_TABLE(EmitterPropertyStringMap);

	bEmitter			= false;
	iBrush				= -1;
	eEmitterProperty	= EMITTER_VARIABLE_MAX;
	eBrushProperty		= PARTICLE_VARIABLE_MAX;

	StdString szItem;
	StdString szItemParent;
	GetItemAndParentStrings(&szItem, &szItemParent);

	// The item is the Emitter
	if (szItemParent == _T(""))
	{
		return;
	}
	// The item is a Brush or an Emitter Property
	else if (szItemParent == _T("Emitter"))
	{
		bEmitter = true;

		// See if it is a brush
		for (int i = 0; i < pDoc->GetNumBrushes(); i++)
		{
			StdString szBrushName(pEmitter->GetBrush(i)->GetName()->GetString());
			if (szItem == szBrushName)
			{
				iBrush = i;
				return;
			}
		}
		for (int i = 0; i < EMITTER_VARIABLE_MAX; i++)
		{
			if (szItem == EmitterPropertyStringMap[i])
			{
				eEmitterProperty = static_cast<PARTICLE_EMITTER_VARIABLE>(i);
				break;
			}
		}
	}
	// The item is a Brush Property
	else
	{
		bEmitter = true;

		IParticleType *temp;
		for (int i = 0; i < pDoc->GetNumBrushes(); i++)
		{
			temp = pEmitter->GetBrush(i);
			StdString szTemp(temp->GetName()->GetString());
			if (szItemParent == szTemp)
			{
				iBrush = i;
				break;
			}
		}
		for (int j = 0; j < PARTICLE_VARIABLE_MAX; j++)
		{
			if (szItem == ParticlePropertyStringMap[j])
			{
				eBrushProperty = static_cast<PARTICLE_VARIABLE>(j);
				break;
			}
		}
	}
}

void CParticleEditorTreePropertyPage::ShowPopupMenu( CPoint& point )
{
	CMenu menu;
	menu.LoadMenu(IDR_BRUSHMENU);
	CMenu* pPopup = menu.GetSubMenu(0);
	assert(pPopup != NULL);
   	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{
		pWndPopupOwner = pWndPopupOwner->GetParent();
	}

	bool bEmitter;
	int iBrush;
	PARTICLE_EMITTER_VARIABLE eEmitterProperty;
	PARTICLE_VARIABLE eBrushProperty;

	GetWorkingData( bEmitter, 
					iBrush, 
					eEmitterProperty, 
					eBrushProperty );

	// Rename on Emitter
	if (false == bEmitter)
		;
	else if ((-1 != iBrush) && (PARTICLE_VARIABLE_MAX == eBrushProperty ))
	{
		int command;
		command = (int)pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_LEFTBUTTON | TPM_RETURNCMD, point.x, point.y, this);

		switch (command)
		{
		case ID_PARTICLEBRUSHMENU_RENAME:
			{
				CParticleRenameDialog RenameDialog;
				RenameDialog.SetBrushNumber(iBrush);
				RenameDialog.DoModal();
	//			RenameDialog.GetWindowText(newpath, _MAX_PATH);
				break;
			}
		case ID_PARTICLEBRUSHMENU_MOVEUP:
			{
				CParticleDoc * pDoc;
				pDoc = CParticleDoc::GetActiveDocument();
				assert(pDoc);
				pDoc->MoveParticleBrush( TRUE, iBrush);
				break;
			}
		case ID_PARTICLEBRUSHMENU_MOVEDOWN:
			{
				CParticleDoc * pDoc;
				pDoc = CParticleDoc::GetActiveDocument();
				assert(pDoc);
				pDoc->MoveParticleBrush( FALSE, iBrush);
				break;
			}
		case ID_PARTICLEBRUSHMENU_DELETE:
			{
				int ret;
				ret = MessageBox(_T("Are you sure you wish to delete this brush, this operation cannot be undone"), _T("Warning!"), MB_YESNO);
				if (IDYES == ret)
				{
					CParticleDoc * pDoc;
					pDoc = CParticleDoc::GetActiveDocument();
					assert(pDoc);
					pDoc->RemoveParticleBrush(StdString(_T("")), iBrush);
				}
				break;
			}
		}
	}
}

BOOL CParticleEditorTreePropertyPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CParticleEditorTreePropertyPage::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	RefreshList(CParticleDoc::GetActiveDocument());
}

void CParticleEditorTreePropertyPage::OnContextMenu(CWnd* pWnd, CPoint point)
{
	UINT uFlags;
	CPoint ptTree = point;
	m_TreeCtrl.ScreenToClient(&ptTree);
	HTREEITEM htItem = m_TreeCtrl.HitTest(ptTree, &uFlags);

	if ((htItem != NULL) && (uFlags & TVHT_ONITEM))
	{
		ShowPopupMenu( point );
		m_TreeCtrl.SetItemState(htItem, 0, TVIS_DROPHILITED);
	}
	else
	{
		CPropertyPage::OnContextMenu(pWnd, point);
	}
}

void CParticleEditorTreePropertyPage::RefreshList(CParticleDoc *pDocument)
{
	m_TreeCtrl.DeleteAllItems();

	if (pDocument == NULL)
	{
		return;
	}
	
	IParticleEmitter *pParticleEmitter = pDocument->GetEmitterPointer();
	if (pParticleEmitter == NULL)
	{
		// nothing to fill
		return;
	}

	CREATE_PARTICLE_STRING_TABLE(ParticleProperty);
	CREATE_EMITTER_STRING_TABLE(EmitterProperty);
	HTREEITEM hEmitterItem = m_TreeCtrl.InsertItem(_T("Emitter"), TVI_ROOT, TVI_LAST);
	for (UINT i = 0; i < pParticleEmitter->GetNumBrushes(); ++i)
	{
		LPCTSTR szName = pParticleEmitter->GetBrush(i)->GetName()->GetString();
		HTREEITEM hBrushItem = m_TreeCtrl.InsertItem(szName, hEmitterItem, TVI_LAST);
		AddStringsToTree(hBrushItem, ParticleProperty, ParticleProperty + _countof(ParticleProperty));
	}
	AddStringsToTree(hEmitterItem, EmitterProperty, EmitterProperty + _countof(EmitterProperty));
	m_TreeCtrl.Expand(hEmitterItem, TVE_EXPAND);
}

void CParticleEditorTreePropertyPage::AddStringsToTree(HTREEITEM hParent, LPCTSTR *szFirstName, LPCTSTR *szLastName)
{
	for (; szFirstName != szLastName; ++szFirstName)
	{
		m_TreeCtrl.InsertItem(*szFirstName, hParent, TVI_LAST);
	}
}
