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
#include "SelectNewObjectTypeDlg.h"

// CSelectNewObjectTypeDlg dialog

IMPLEMENT_DYNAMIC(CSelectNewObjectTypeDlg, CDialog)
CSelectNewObjectTypeDlg::CSelectNewObjectTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectNewObjectTypeDlg::IDD, pParent)
{
}

CSelectNewObjectTypeDlg::~CSelectNewObjectTypeDlg()
{
}

void CSelectNewObjectTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NEWOBJECTLIST, m_NewObjectList);
}

BEGIN_MESSAGE_MAP(CSelectNewObjectTypeDlg, CDialog)
	ON_LBN_DBLCLK(IDC_NEWOBJECTLIST, OnLbnDblclkNewobjectlist)
END_MESSAGE_MAP()


bool CSelectNewObjectTypeDlg::GetSelectionText(CString **SelectText)
{
	if (m_Selection == LB_ERR)
	{
		return false;
	}

	*SelectText = &m_SelectedText;
	return true;
}

// CSelectNewObjectTypeDlg message handlers

BOOL CSelectNewObjectTypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	static CHashString hszEmpty;
	if (GetActiveScene() != hszEmpty)
	{
		SELECTEDOBJECTLIST *selectedObjects;
		SELECTEDOBJECT *selectedObject;
		LINKABLEOBJECTS lob;
		list<DWORD> linkableObjects;
		list<DWORD>::iterator lobIter;

		// first get the selected objects
		selectedObjects = SINGLETONINSTANCE(CObjectEditor)->GetSelectedObjects();

		if (selectedObjects->empty())
		{
			static CHashString world(_T("CWorld"));
			lob.key = &world;
		}
		else
		{
			// grab the first one for now
			selectedObject = &(selectedObjects->front());
			// the the component type
			lob.key = selectedObject->type;
		}
		lob.linkableObjects = &linkableObjects;
		// now grab the possible object that can be created when this one
		// is selected.
		static DWORD msgHash_GetLinkableObjects = CHashString(_T("GetLinkableObjects")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_GetLinkableObjects, sizeof(LINKABLEOBJECTS), &lob);

		for (lobIter = linkableObjects.begin(); lobIter != linkableObjects.end();
			++lobIter)
		{
			const TCHAR *typeName;
			typeName = EngineGetToolBox()->GetHashString(*lobIter);
			if (typeName != NULL)
			{
				m_NewObjectList.AddString(typeName);
			}
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectNewObjectTypeDlg::OnOK()
{
	UpdateData();

	m_Selection = m_NewObjectList.GetCurSel();
	if (m_Selection != LB_ERR)
	{
		m_NewObjectList.GetText(m_Selection, m_SelectedText);
	}

	CDialog::OnOK();
}

void CSelectNewObjectTypeDlg::OnLbnDblclkNewobjectlist()
{
	OnOK();
}
