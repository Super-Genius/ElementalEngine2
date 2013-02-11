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
#include "gds.h"
#include "NewObjectDialog.h"
#include ".\newobjectdialog.h"


// CNewObjectDialog dialog

IMPLEMENT_DYNAMIC(CNewObjectDialog, CDialog)
CNewObjectDialog::CNewObjectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNewObjectDialog::IDD, pParent)
{
}

CNewObjectDialog::~CNewObjectDialog()
{
}

void CNewObjectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ObjectTypeList);
}


BEGIN_MESSAGE_MAP(CNewObjectDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, OnCancel)
	ON_BN_CLICKED(IDC_BUTTON1, OnOk)
	ON_LBN_DBLCLK(IDC_LIST1, OnLbnDblclkNewObjectList)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CNewObjectDialog message handlers

void CNewObjectDialog::OnCancel()
{
	// TODO: Add your control notification handler code here
	ShowWindow( SW_HIDE );
	CDialog::OnCancel();
}

void CNewObjectDialog::OnOk()
{
	// TODO: Add your control notification handler code here
	//create the new object
	int cursel = m_ObjectTypeList.GetCurSel();
	if( cursel >= 0)
	{
        FILETYPECREATIONINFO* pInfo = (FILETYPECREATIONINFO*)m_ObjectTypeList.GetItemData( cursel );
		if( pInfo && pInfo->m_CreationCallback )
		{
			(*pInfo->m_CreationCallback)(pInfo->m_UserData);
		}
		CDialog::OnCancel();
	}
}

void CNewObjectDialog::Initialize( list< FILETYPECREATIONINFO > &FileTypeList )
{
	//clear current
	FILETYPECREATIONINFO* pInfo;
	while( m_ObjectTypeList.GetCount() > 0 )
	{
		pInfo = (FILETYPECREATIONINFO*)m_ObjectTypeList.GetItemData( 0 );		
		m_ObjectTypeList.DeleteString( 0 );
		delete pInfo;
	}	
	//repopulate list:
	int insertItem;
	list< FILETYPECREATIONINFO >::iterator iter = FileTypeList.begin();
	for( ; iter != FileTypeList.end(); iter++ )
	{
		FILETYPECREATIONINFO &rInfo = (*iter);
		pInfo = new FILETYPECREATIONINFO;
		*pInfo = rInfo;
		insertItem = m_ObjectTypeList.AddString( pInfo->m_Description );
		m_ObjectTypeList.SetItemData( insertItem, (DWORD_PTR)pInfo );
	}
	UpdateData( false ); //update the actual data
	UpdateWindow(); //redraw window to ensure visibility
}

void CNewObjectDialog::OnLbnDblclkNewObjectList()
{
	CNewObjectDialog::OnOk();
}

void CNewObjectDialog::OnDestroy()
{
	FILETYPECREATIONINFO * pInfo;

	CDialog::OnDestroy();
	//Deinit stuff
	int objectCount = m_ObjectTypeList.GetCount();
	for( int  i = 0; i < objectCount; i++ )
	{
		 pInfo = (FILETYPECREATIONINFO * )m_ObjectTypeList.GetItemData( i );
		if( pInfo )
		{
			delete pInfo;
		}
	}
}
