///============================================================================
/// \file		ResourceFolderView.cpp
/// \brief		Header file for ResourceFolderView
/// \date		11-17-2005
/// \author		Halbert Nakagawa
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Image indexes
#define ILI_HARD_DISK       0
#define ILI_FLOPPY          1
#define ILI_CD_ROM          2
#define ILI_NET_DRIVE       3
#define ILI_CLOSED_FOLDER   4
#define ILI_OPEN_FOLDER     5

/////////////////////////////////////////////////////////////////////////////
// CResourceFolderView

IMPLEMENT_DYNCREATE(CResourceFolderView, CTreeView)

BEGIN_MESSAGE_MAP(CResourceFolderView, CTreeView)
	//{{AFX_MSG_MAP(CResourceFolderView)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemExpanding)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelectionChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceFolderView construction/destruction

CResourceFolderView::CResourceFolderView()
{
}

CResourceFolderView::~CResourceFolderView()
{
}

BOOL CResourceFolderView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CTreeView::PreCreateWindow (cs))
		return FALSE;

    cs.style |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS |
        TVS_SHOWSELALWAYS;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CResourceFolderView drawing

void CResourceFolderView::OnDraw(CDC* pDC)
{
	CResourceDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

void CResourceFolderView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// Initialize the image list.
    m_ilDrives.Create (IDB_DRIVEIMAGES, 16, 1, RGB (255, 0, 255));
    GetTreeCtrl ().SetImageList (&m_ilDrives, TVSIL_NORMAL);

	// set the paths
	m_strResouceDirectory = getNewResoucePath();

	// Populate the tree view with the folders in the base directory
	SetupTree();
	//AddDrives();

	// select the first folder
	HTREEITEM hItem = GetTreeCtrl().GetNextItem(NULL, TVGN_ROOT);
	if (hItem != NULL) {
		GetTreeCtrl().Expand(hItem, TVE_EXPAND);
		GetTreeCtrl().Select(hItem, TVGN_CARET);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CResourceFolderView diagnostics

#ifdef _DEBUG
void CResourceFolderView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CResourceFolderView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CResourceDoc* CResourceFolderView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CResourceDoc)));
	return (CResourceDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CResourceFolderView message handlers
void CResourceFolderView::SetupTree()
{
	// stop updates
	SetRedraw( FALSE );

	// clear the tree
	GetTreeCtrl().DeleteAllItems();

	// use the member variable as path
	StdString pszPath = m_strResouceDirectory;

	//add the directories (ony one level will be added)
	AddDirectories( TVI_ROOT, pszPath );	

	// set updates
	SetRedraw( TRUE );
}

int CResourceFolderView::AddDrives()
{
    int nPos = 0;
    int nDrivesAdded = 0;
    CString string = _T ("?:\\");

    DWORD dwDriveList = ::GetLogicalDrives ();

    while (dwDriveList) {
        if (dwDriveList & 1) {
            string.SetAt (0, _T ('A') + nPos);
            if (AddDriveItem (string))
                nDrivesAdded++;
        }
        dwDriveList >>= 1;
        nPos++;
    }
    return nDrivesAdded;
}

BOOL CResourceFolderView::AddDriveItem(LPCTSTR pszDrive)
{
    CString string;
    HTREEITEM hItem;

    UINT nType = ::GetDriveType (pszDrive);

    switch (nType) {

    case DRIVE_REMOVABLE:
        hItem = GetTreeCtrl ().InsertItem (pszDrive, ILI_FLOPPY,
            ILI_FLOPPY);
        GetTreeCtrl ().InsertItem (_T (""), ILI_CLOSED_FOLDER,
            ILI_CLOSED_FOLDER, hItem);
        break;

    case DRIVE_FIXED:
    case DRIVE_RAMDISK:
        hItem = GetTreeCtrl ().InsertItem (pszDrive, ILI_HARD_DISK,
            ILI_HARD_DISK);
        SetButtonState (hItem, pszDrive);
        break;

    case DRIVE_REMOTE:
        hItem = GetTreeCtrl ().InsertItem (pszDrive, ILI_NET_DRIVE,
            ILI_NET_DRIVE);
        SetButtonState (hItem, pszDrive);
        break;

    case DRIVE_CDROM:
        hItem = GetTreeCtrl ().InsertItem (pszDrive, ILI_CD_ROM,
            ILI_CD_ROM);
        GetTreeCtrl ().InsertItem (_T (""), ILI_CLOSED_FOLDER,
            ILI_CLOSED_FOLDER, hItem);
        break;

    default:
        return FALSE;
    }
    return TRUE;
}


BOOL CResourceFolderView::SetButtonState(HTREEITEM hItem, LPCTSTR pszPath)
{
    HANDLE hFind;
    WIN32_FIND_DATA fd;
    BOOL bResult = FALSE;

	// convert relative paths to full paths + add '*.*' fileName pattern
	CString strPath = convertRelativeToFullPath( pszPath );

	// check the path
    if ((hFind = ::FindFirstFile (strPath, &fd)) == INVALID_HANDLE_VALUE)
        return bResult;

	// if there are subdirs under the path, make our node expandable
    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            CString strComp = (LPCTSTR) &fd.cFileName;
            if ((strComp != _T (".")) && (strComp != _T ("..")))
			{
				// add a dummy node to create the '+' sing for the element to be able
				// to expand it
                GetTreeCtrl().InsertItem( _T(""), ILI_CLOSED_FOLDER, ILI_CLOSED_FOLDER, hItem);
                bResult = TRUE;
                break;
            }
        }
    } while (::FindNextFile (hFind, &fd));

    ::FindClose (hFind);
    return bResult;
}

void CResourceFolderView::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    HTREEITEM hItem = pNMTreeView->itemNew.hItem;
    CString string = GetPathFromItem (hItem);

    *pResult = FALSE;

    if (pNMTreeView->action == TVE_EXPAND) {
        DeleteFirstChild (hItem);
		if (AddDirectories (hItem, string) == 0)
            *pResult = TRUE;
    }
    else { // pNMTreeView->action == TVE_COLLAPSE
        DeleteAllChildren (hItem);
        if (GetTreeCtrl ().GetParentItem (hItem) == NULL)
            GetTreeCtrl ().InsertItem (_T (""), ILI_CLOSED_FOLDER,
                ILI_CLOSED_FOLDER, hItem);
        else
            SetButtonState (hItem, string);
    }
}

CString CResourceFolderView::GetPathFromItem(HTREEITEM hItem)
{
    CString strResult = GetTreeCtrl ().GetItemText (hItem);

    HTREEITEM hParent;
    while ((hParent = GetTreeCtrl ().GetParentItem (hItem)) != NULL) {
        CString string = GetTreeCtrl ().GetItemText (hParent);
        if (string.Right (1) != _T ("\\"))
            string += _T ("\\");
        strResult = string + strResult;
        hItem = hParent;
    }
    return strResult;
}

void CResourceFolderView::DeleteFirstChild(HTREEITEM hItem)
{
    HTREEITEM hChildItem;
    if ((hChildItem = GetTreeCtrl ().GetChildItem (hItem)) != NULL)
        GetTreeCtrl ().DeleteItem (hChildItem);
}

void CResourceFolderView::DeleteAllChildren(HTREEITEM hItem)
{
    HTREEITEM hChildItem;
    if ((hChildItem = GetTreeCtrl ().GetChildItem (hItem)) == NULL)
        return;

    do {
        HTREEITEM hNextItem = GetTreeCtrl ().GetNextSiblingItem (hChildItem);
        GetTreeCtrl ().DeleteItem (hChildItem);
        hChildItem = hNextItem;
    } while (hChildItem != NULL);
}

int CResourceFolderView::AddDirectories(HTREEITEM hItem, LPCTSTR pszPath)
{
    HANDLE hFind;
    WIN32_FIND_DATA fd;
    HTREEITEM hNewItem;

    int nCount = 0;

	// convert relative paths to full paths
	CString strPath = convertRelativeToFullPath( pszPath );

	// set the file handler 
    if ((hFind = ::FindFirstFile (strPath, &fd)) == INVALID_HANDLE_VALUE) {
        if (GetTreeCtrl ().GetParentItem (hItem) == NULL)
            GetTreeCtrl ().InsertItem (_T (""), ILI_CLOSED_FOLDER,
                ILI_CLOSED_FOLDER, hItem);
        return 0;
    }

    do {
		// check if it is a folder or not
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            CString strComp = (LPCTSTR) &fd.cFileName;  
            
			// skipp '.' and '..'
			if ((strComp != _T (".")) && (strComp != _T ("..")))
			{
				// add the folder elements
                hNewItem =
                    GetTreeCtrl ().InsertItem ((LPCTSTR) &fd.cFileName,
                    ILI_CLOSED_FOLDER, ILI_OPEN_FOLDER, hItem);

                CString strNewPath = pszPath;
                if (strNewPath.Right (1) != _T ("\\"))
                    strNewPath += _T ("\\");

                strNewPath += (LPCTSTR) &fd.cFileName;

				// set them expandable if necessary
                SetButtonState (hNewItem, strNewPath);
                nCount++;
            }
        }
    } while (::FindNextFile (hFind, &fd));

    ::FindClose (hFind);
    return nCount;
}

void CResourceFolderView::OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*) pNMHDR;
    CString strPath = GetPathFromItem (pNMTreeView->itemNew.hItem);
    //GetDocument ()->UpdateAllViews (this, 0x5A, (CObject*) (LPCTSTR) strPath);
	CResourceDoc* pDoc = DYNAMIC_DOWNCAST(CResourceDoc, GetDocument());
	

	///////////////////////////////////////////// old solution 
	/*
	// append the selected path to the resource path
	CGDSApp * gApp = dynamic_cast< CGDSApp * >( AfxGetApp() );			
	if( gApp )
	{
		stdstrPath = StdString(gApp->GetResourcePath()) + _T("\\") + stdstrPath;
	}
	*/

	/////////////////////////////////////////////// new solution
	StdString stdstrPath( strPath );
	stdstrPath = m_strResouceDirectory + "\\" + stdstrPath; 
	pDoc->SetActiveDirectory( stdstrPath );

	*pResult = 0;
}

void CResourceFolderView::resetResourceFolder()
{
	// get the new resouce path
	m_strResouceDirectory = getNewResoucePath();

	// populate the tree with new directory data. The directory path is used
	// wich is currently avaliable under theApp->getResoucePath();
	SetupTree();

	// select the first folder
	HTREEITEM hItem = GetTreeCtrl().GetNextItem(NULL, TVGN_ROOT);
	if (hItem != NULL) {
		GetTreeCtrl().Expand(hItem, TVE_EXPAND);
		GetTreeCtrl().Select(hItem, TVGN_CARET);
	}
}

StdString CResourceFolderView::getNewResoucePath()
{
	// get a pointer to GDSComponent
	CGDSApp * gApp = dynamic_cast< CGDSApp * >( AfxGetApp() );			
	TCHAR ResourcePath[ MAX_PATH ];	

	// get the resouce folder path
	if( gApp )
	{
		_tcscpy( ResourcePath, (LPCTSTR) gApp->GetResourcePath() );
	}
	else
	{
		::GetCurrentDirectory (sizeof (ResourcePath) / sizeof (TCHAR), ResourcePath);
	}

	// set the member variable
	return ResourcePath;
	

}

CString CResourceFolderView::convertRelativeToFullPath( LPCTSTR p_path )
{
	// the return value (at least the incomming path)
	CString strPath( p_path );

	// check if path is relative
	TCHAR drive[_MAX_DRIVE];
	_tsplitpath( p_path, drive, NULL, NULL, NULL );

	// a path is relative if no drive is defined in the path.
	if ( drive[0] == '\0' )
	{
		// path is relative -> convert to full path
		strPath = (CString)m_strResouceDirectory.c_str() + "\\" + strPath;
	}

	// add 'any' file pattern to the end of the string
    if (strPath.Right (1) != _T ("\\"))
        strPath += _T ("\\");
    strPath += _T ("*.*");

	return strPath;
}