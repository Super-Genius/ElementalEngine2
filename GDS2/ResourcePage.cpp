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

#include "stdafx.h"
#include "ExtractImageHelper.h"
#include <string>
#include "ResourcePage.h"
#include "FileHelpers.h"
#include ".\resourcepage.h"
#include "Resource.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IMAGESIZEX	64
#define IMAGESIZEY	64
#define ICONSIZEX	32
#define ICONSIZEY	32

#define WM_KILL_THREAD (WM_USER + 2)
#define WAIT_FOR_THREAD_INIT		10 //Time to wait for threads in idle functions, in milliseconds
#define THUMBNAIL_DUMMY_BUFFER	50

static UINT ResourceDragMessage = RegisterWindowMessage(RESOURCE_DRAG_MSG_STR);
static UINT ResourceDropMessage = RegisterWindowMessage(RESOURCE_DROP_MSG_STR);
static UINT ResourceAddMessage = RegisterWindowMessage( RESOURCE_ADD_MSG_STR );
static UINT ResourceFolderRefreshMessage = RegisterWindowMessage( RESOURCE_FOLDER_REFRESH_MSG_STR );
static UINT ResourceFileChangedMessage = RegisterWindowMessage( RESOURCE_FILE_CHANGED_MSG_STR );
static UINT WM_POPULATE_RESOURCE_LIST = RegisterWindowMessage( "WM_POPULATE_RESOURCE_LIST");
static UINT ResetInteruptMessage = RegisterWindowMessage( "RESET_INTERUPT_MSG" );

// Static threads for recursion
const TCHAR *THUMBNAIL_CACHE_FILENAME = _T("GDS_THUMBNAIL_CACHE.thumbs-");
DWORD		CResourcePage::m_ResourceThumbnailThreadID = -1;
HANDLE		CResourcePage::m_ResourceThumbnailThreadHandle = INVALID_HANDLE_VALUE;
HANDLE		CResourcePage::m_ResourceWatchThreadHandle = INVALID_HANDLE_VALUE;
int			CResourcePage::m_ObjectCount = 0;
bool		CResourcePage::m_PauseThumbnailGenerationThread = false;

bool		CResourcePage::m_bInteruptFlag = false;
/////////////////////////////////////////////////////////////////////////////
// CResourcePage property page

IMPLEMENT_DYNCREATE(CResourcePage, CPropertyPage)

const TCHAR* RelativeToAbsolutePath( const TCHAR *pRelativePath, const TCHAR *pBaseDirectory )
{
	//convert the relative path to fullpath:
	static TCHAR fullp[ MAX_PATH ];
	const TCHAR * relativepath = pRelativePath;
	_tcscpy( fullp, pBaseDirectory );
	//check if search param has leading /
	int len = strlen( fullp );
	if( relativepath[0] == '/' || relativepath[0] == '\\' )
	{
		//check to see if resource directory has trailing slash
		if( fullp[ len-1 ] == '\\' || fullp[ len - 1 ] == '/' )
		{
			//take out fullp's
			fullp[ len - 1 ] = '\0';
		}
	}else
	{
		//check if fullp doesn't have trailing, add it
		if( fullp[ len-1 ] != '\\' && fullp[ len - 1 ] != '/' )
		{
			fullp[ len ] = '\\';
			fullp[len+1] = '\0';
		}
	}
	_tcscat( fullp, pRelativePath );
	//ok now we have the full path, we can compare to the valid resources list:
	_tcslwr( fullp );
	return fullp;
}

CResourcePage::CResourcePage( ) : CPropertyPage(CResourcePage::IDD)
{
	//{{AFX_DATA_INIT(CResourcePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDragImage = NULL;	
	m_ObjectCount++;
	m_szExtensions = _T("");	
	m_szResourceDirectory = _T("");
	m_szActiveDirectory = _T("");
	m_BlankBitmap = NULL;
	m_iView = LVS_ICON;
	m_ViewMenu.LoadMenu(IDR_OBJECTPAGEPOP);
}

CResourcePage::CResourcePage( RESOURCECOLLECTION *pResourceCollection ) : CPropertyPage(CResourcePage::IDD)
{
	//{{AFX_DATA_INIT(CResourcePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	ASSERT(pResourceCollection != NULL);

	TCHAR buf[ MAX_PATH ];
	m_pDragImage = NULL;	
	m_ObjectCount++;
	_tcscpy( buf, pResourceCollection->extensions );
	_tcslwr( buf );
	m_szExtensions = buf;
	m_szName = pResourceCollection->name;
	m_szResourceDirectory = _T("");
	m_szActiveDirectory = _T("");
	m_BlankBitmap = NULL;
	m_iView = pResourceCollection->iView;
	m_ViewMenu.LoadMenu(IDR_OBJECTPAGEPOP);
}

CResourcePage::~CResourcePage()
{
	if (m_pDragImage)
	{
		delete m_pDragImage;
		m_pDragImage = NULL;
	}
	//Close Preview thread, last resource page
	m_ObjectCount--;
	if( m_ObjectCount == 0)
	{
		//PostThreadMessage( m_ResourceThumbnailThreadID, WM_KILL_THREAD, NULL, NULL);
		TerminateThread( m_ResourceThumbnailThreadHandle, 0 );
		// thread will autoterminate.
		CloseHandle( m_ResourceThumbnailThreadHandle );
		CloseHandle( m_ResourceWatchThreadHandle );
		m_ResourceThumbnailThreadHandle = INVALID_HANDLE_VALUE;
		m_ResourceThumbnailThreadID = -1;	
	}

	m_ViewMenu.DestroyMenu();
}

void CResourcePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResourcePage)
	DDX_Control(pDX, IDC_OBJECTS_LIST, m_ResourcesList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CResourcePage, CPropertyPage)
	//{{AFX_MSG_MAP(CResourcePage)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_OBJECTS_LIST, OnBegindragResourcesList)
	ON_NOTIFY(NM_DBLCLK, IDC_OBJECTS_LIST, OnNMDblclkObjectsList)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(ResourceAddMessage, OnAddMessage)
	ON_REGISTERED_MESSAGE(ResourceFolderRefreshMessage, OnFolderRefreshMessage)
	ON_REGISTERED_MESSAGE(ResourceFileChangedMessage, OnFileChangedMessage)
	ON_REGISTERED_MESSAGE(ResetInteruptMessage, OnResetInteruptMessage)
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_WM_KEYUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourcePage message handlers

void CResourcePage::OnSize(UINT nType, int cx, int cy) 
{
	CRect rect;

	CPropertyPage::OnSize(nType, cx, cy);

	GetClientRect(rect);
	if (m_ResourcesList.m_hWnd != NULL)
	{		
		m_ResourcesList.MoveWindow(rect);			
		m_ResourcesList.SetImageList(&m_ThumbNails, LVSIL_NORMAL);
		m_ResourcesList.Arrange( LVA_DEFAULT );		
	}

	UpdateThumbnails();	
}
//---------------------------------------------------------------------
// Function:	AddResource
// Description:	Adds resource icon/text to page
// Parameters:	os = pointer to information struct containing icon/text
// Returns:		os->index is filled in with index inside list
//---------------------------------------------------------------------
void CResourcePage::AddResource( RESOURCEPAGEINFO *os)
{
	UINT nInsertPos;
	int insertedItem;
	bool bReplace = false;
	FILEINDEXINFO * pfInfo = NULL;
	CFileStatus fileStatus;
	CFile theFile;		
	if( os )
	{
		_tcslwr( os->filepath );	
		//first test is make sure it still exists
		if( !theFile.GetStatus( os->filepath,  fileStatus ) )
		{
			::GlobalFree( os );
			return;
		}
		//find it in the mapping:
		std::map< StdString, FILEINDEXINFO >::iterator iter =  m_ValidResources.find( os->filepath );
		if( iter != m_ValidResources.end() )
		{
			bReplace = true;
			pfInfo = &((*iter).second);
			//delete old entry
			RESOURCEPAGEINFO * resourceEntry = (RESOURCEPAGEINFO*)m_ResourcesList.GetItemData( pfInfo->m_Index );
			::GlobalFree( resourceEntry );
			nInsertPos = pfInfo->m_Index;
			pfInfo->m_FileTime = os->lastWrite;
		}
		//Insertion:
		if( bReplace )
		{
			if (os->thumbNail != NULL)
			{			
				CDC *pDC = m_ResourcesList.GetDC();
				CBitmap *newBitmap;		
				newBitmap = CreateThumbnail(pDC, os->thumbNail);	
				m_ThumbNails.Replace(nInsertPos, newBitmap, NULL );
				m_ResourcesList.SetImageList(&m_ThumbNails, LVSIL_NORMAL);
				if (os->name == NULL)
					m_ResourcesList.SetItemText(nInsertPos, 0, "");
				else
					m_ResourcesList.SetItemText(nInsertPos, 0, os->name);

				delete newBitmap;
				m_ResourcesList.ReleaseDC(pDC);
				m_ResourcesList.SetItemData(nInsertPos, (DWORD)os);
			}
			else // assume for now that if there is no thumbnail, we are in list mode
			{
				//m_ResourcesList.ModifyStyle(0, LVS_LIST);
				m_ThumbNails.Add(m_BlankBitmap, RGB(0, 0, 0));
				insertedItem = m_ResourcesList.InsertItem(nInsertPos, os->name);
				m_ResourcesList.SetItemData(insertedItem, (DWORD)os);
			}		
		}
		else//was never in the list, so add new
		{			
			nInsertPos = m_ResourcesList.GetItemCount();
			//either insert with a thumbnail or not
			if (os->thumbNail != NULL)
			{			
				CDC *pDC = m_ResourcesList.GetDC();
				CBitmap *newBitmap;		
				newBitmap = CreateThumbnail(pDC, os->thumbNail);	
				m_ThumbNails.Add(newBitmap, RGB(0, 0, 0));
				m_ResourcesList.SetImageList(&m_ThumbNails, LVSIL_NORMAL);
				if (os->name == NULL)
					insertedItem = m_ResourcesList.InsertItem(nInsertPos, "", nInsertPos);
				else
					insertedItem = m_ResourcesList.InsertItem(nInsertPos, os->name, nInsertPos);

				m_ResourcesList.ReleaseDC(pDC);
				delete newBitmap;	
				
				m_ResourcesList.SetItemData(insertedItem, (DWORD)os);
			}
			else // assume for now that if there is no thumbnail, we are in list mode
			{
				m_ThumbNails.Add(m_BlankBitmap, RGB(0, 0, 0));
				insertedItem = m_ResourcesList.InsertItem(nInsertPos, os->name);
				m_ResourcesList.SetItemData(insertedItem, (DWORD)os);
			}		
			//add new
			FILEINDEXINFO finfo;
			finfo.m_FileTime = os->lastWrite;
			finfo.m_Index = insertedItem;
			m_ValidResources.insert( pair< StdString, FILEINDEXINFO>( StdString( os->filepath), finfo ) );
		}	
	}	
}

//---------------------------------------------------------------------
// Function:	RemoveResource
// Description:	Removes resource icon/text from page
// Parameters:	index = index # in list
// Returns:		.
//---------------------------------------------------------------------
void CResourcePage::RemoveResource(UINT index)
{
	//Remove old entry
	RESOURCEPAGEINFO * resourceEntry = (RESOURCEPAGEINFO*)m_ResourcesList.GetItemData( index );
	if( resourceEntry )
	{
		std::map<StdString, FILEINDEXINFO>::iterator iter = m_ValidResources.find( StdString( resourceEntry->filepath ) );
		//int zi = m_ThumbNails.GetImageCount();
		m_ThumbNails.Remove( index );
		//zi = m_ThumbNails.GetImageCount();
		m_ResourcesList.DeleteItem( index );
		int itemcount = m_ResourcesList.GetItemCount();
		//unfortunately the list and imagelist aren't mapped 1 - 1 which would have been simple,
		//but microsoft forces us to update everything!(almost) when you remove an entry;
		//ludicrous
		for( int i = index; i < itemcount; i++ )
		{
			CString text = m_ResourcesList.GetItemText( i, 0 );
			DWORD d = m_ResourcesList.GetItemData( i );
			m_ResourcesList.SetItem( i, 0, LVIF_IMAGE, text, i, 0, 0, d );		
		}
		//we also have to update the valid resources mapping since the indices will change for everything above
		//index
		if( iter != m_ValidResources.end() )
		{
			m_ValidResources.erase( iter );
		}
		if( m_ValidResources.size() > 0 )
		{
			//now loop and update the indices of the rest of the mapping
			iter = m_ValidResources.begin();	
			for( ; iter != m_ValidResources.end(); iter++ )
			{
				FILEINDEXINFO &curInfo = (*iter).second;
				if( curInfo.m_Index > index )
				{
					curInfo.m_Index = curInfo.m_Index - 1;
				}
			}
		}
		::GlobalFree( resourceEntry );
	}
}

//---------------------------------------------------------------------
// Function:	ClearResources
// Description:	Removes alll resources from the list
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
void CResourcePage::ClearResources()
{
}

/// Initializes the Resource Page, starts the main thumbnail generator thread if
/// one is not already established
BOOL CResourcePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	BOOL posthreadsucceeded;
	TCHAR ResourcePath[ MAX_PATH ];		
	CGDSApp * gApp;	
	CreateDummyBitmap();

	// setup the active directory
	CHashString hashPath(_T(""));
	static DWORD msgHash_GetResourceActiveDirectory = CHashString(_T("GetResourceActiveDirectory")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetResourceActiveDirectory, sizeof(IHashString*), &hashPath);
	m_szActiveDirectory = hashPath.GetString();

	gApp = dynamic_cast< CGDSApp * >( AfxGetApp() );			
	//load any thumbnails cached, otherwise create an empty list
	if( gApp )
	{
		_tcscpy( ResourcePath, gApp->GetResourcePath() );
		m_szResourceDirectory = ResourcePath;
		// attempt to load any cached thumbnails
		LoadCachedThumbnails(ResourcePath);
		m_ThumbNails.Create(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, ILC_COLOR32 | ILC_MASK, 0, -1);	
		m_ResourcesList.ModifyStyle(0,LVS_SHAREIMAGELISTS | m_iView | LVS_SHOWSELALWAYS );		
		SetView( m_iView );
	}
	//Start my resource thumbnail update thread	
	if( m_ResourceThumbnailThreadHandle == INVALID_HANDLE_VALUE )
	{
		m_ResourceThumbnailThreadHandle = CreateThread( 0,0,
			(LPTHREAD_START_ROUTINE) FillResourcesThread, (VOID*)this, 0, &m_ResourceThumbnailThreadID );
		
	}
	if( m_ResourceThumbnailThreadHandle != NULL )
	{
		SetThreadPriority( m_ResourceThumbnailThreadHandle, THREAD_PRIORITY_IDLE );	
		//Ensure a message queue is in place by sending it a message
		if( gApp )
		{
			//Waits for thread to properly establish a message queue before sending the message
			//This can be handled with an Event, but this is simple also
			posthreadsucceeded = FALSE;
			while( !posthreadsucceeded )
			{
				posthreadsucceeded = ::PostThreadMessage( m_ResourceThumbnailThreadID, WM_POPULATE_RESOURCE_LIST,
				0, 0 );
				Sleep( WAIT_FOR_THREAD_INIT );
			}			
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CResourcePage::StartWatchThread( const TCHAR * directory, HWND parentHandle )
{
	POPULATERESOURCELISTPARAMS * populaterequest;
	DWORD mID;
	populaterequest =	(POPULATERESOURCELISTPARAMS*)::GlobalAlloc( GMEM_FIXED,
				sizeof( POPULATERESOURCELISTPARAMS ) );
	_tcscpy( populaterequest->FilePath, directory );
	populaterequest->ParentHandle = parentHandle;
	//this should not hapen by design
	ASSERT( m_ResourceWatchThreadHandle == INVALID_HANDLE_VALUE );
	m_ResourceWatchThreadHandle = CreateThread( 0,0,
		(LPTHREAD_START_ROUTINE) CResourcePage::WatchDirectoryThread, (VOID*)populaterequest, 0, &mID );
	if( m_ResourceWatchThreadHandle != NULL )
	{
		SetThreadPriority( m_ResourceWatchThreadHandle, THREAD_PRIORITY_IDLE );
	}
}

void CResourcePage::StopWatchThread()
{
	if( m_ResourceWatchThreadHandle != INVALID_HANDLE_VALUE )
	{
		TerminateThread( m_ResourceWatchThreadHandle, 0 );
		m_ResourceWatchThreadHandle = INVALID_HANDLE_VALUE;
	}
}
void CResourcePage::OnBegindragResourcesList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// save the index of the item being dragged in m_nDragIndex
	m_nDragIndex = pNMListView->iItem;
	POINT pt;
	pt.x = 8;
	pt.y = 8;
	// create a drag image

	if (m_pDragImage)
		delete m_pDragImage;
	
	m_pDragImage = m_ResourcesList.CreateDragImage(m_nDragIndex, &pt);
	ASSERT (m_pDragImage);
	// changes the cursor to the drag image (DragMove() is still required in 
	// OnMouseMove())
	VERIFY(m_pDragImage->BeginDrag(0, CPoint (8, 8)));
	VERIFY(m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction));
	// set dragging flag
	m_bDragging = TRUE;
	m_nDropIndex = -1;
	m_pDropWnd = NULL;
	// capture all mouse messages
	SetCapture();

	*pResult = 0;
}

CBitmap* CResourcePage::GetThumbnailFromListIndex( int index )
{
	// get a good bitmap handle to send if we can, otherwise it defaults to NULL and wont display an image
	IMAGEINFO ImageInfo;
	CBitmap * destBitmap;		
	if( index >= m_ResourcesList.GetItemCount() )
	{
		return NULL;
	}
	if (m_ThumbNails.GetImageInfo( index, &ImageInfo ))
	{
		CDC dcMem; 
		destBitmap = new CBitmap;
		CDC *srcDC;
		srcDC = m_ResourcesList.GetDC();
		dcMem.CreateCompatibleDC( srcDC); 
		
		//This rect simply stored the size of the image we need
		CRect rect( ImageInfo.rcImage );
		
		//Using the bitmap passed in, Create a bitmap 
		//compatible with the window DC
		//We also know that the bitmap needs to be a certain size.
		destBitmap->CreateCompatibleBitmap( srcDC, 
										rect.Width(), rect.Height() );
		
		//Select the new destination bitmap into the DC we created above
		CBitmap* pBmpOld = dcMem.SelectObject( destBitmap );
		
		//This call apparently "draws" the bitmap from the list, 
		//onto the new destination bitmap
		m_ThumbNails.DrawIndirect( &dcMem, index, CPoint(0, 0), CSize( rect.Width(), rect.Height() ), CPoint( 0, 0 ) );
		
		m_ResourcesList.ReleaseDC(srcDC);
		//cleanup by reselecting the old bitmap object into the DC
		dcMem.SelectObject( pBmpOld );
		return destBitmap;
	}
	return NULL;
}

void CResourcePage::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd* pDropWnd;
	if (m_bDragging)
	{
		// release mouse capture
		VERIFY (::ReleaseCapture());
		m_bDragging = FALSE;
		// end dragging
		VERIFY (m_pDragImage->DragLeave(GetDesktopWindow()));
		m_pDragImage->EndDrag();

		CPoint pt(point);
		ClientToScreen(&pt);
		// get the CWnd pointer of the window that is under the mouse cursor
		pDropWnd = WindowFromPoint(pt);
		if (pDropWnd == NULL)
			return;

		DROPOBJECTINFO doInfo;
		RESOURCEPAGEINFO tempInfo;
		tempInfo = (*((RESOURCEPAGEINFO*)m_ResourcesList.GetItemData(m_nDragIndex)));
		_tcscpy( tempInfo.filepath, EngineGetToolBox()->GetRelativePath( tempInfo.filepath ) );
		doInfo.pResourcePageInfo = &tempInfo;
		doInfo.xy = pt;

		// get a good bitmap handle to send if we can, otherwise it defaults to NULL and wont display an image
		CBitmap * destBitmap = GetThumbnailFromListIndex( m_nDragIndex );
		doInfo.pResourcePageInfo->thumbNail = (HBITMAP) destBitmap->GetSafeHandle();

		pDropWnd->SendMessage(ResourceDropMessage, 0, (LPARAM)&doInfo);
		if( destBitmap )//clean up now unused bitmap
		{
			delete destBitmap;
		}
	}
	else
	{
		CPropertyPage::OnLButtonUp(nFlags, point);
	}
}

void CResourcePage::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bDragging)
	{
		CPoint pt(point);
		ClientToScreen(&pt);
		// move the drag image
		VERIFY(m_pDragImage->DragMove(pt));
		// unlock window updates
		VERIFY(m_pDragImage->DragShowNolock(FALSE));

		// save current window pointer
		m_pDropWnd = WindowFromPoint(pt);

		// convert from screen coordinates to drop target client coordinates
		m_pDropWnd->ScreenToClient(&pt);

		// send drag message to the window we are under
		DRAGOBJECTINFO doInfo;
		doInfo.xy = pt;
		m_pDropWnd->SendMessage(ResourceDragMessage, 0, (LPARAM)&doInfo);

		// lock window updates
		VERIFY(m_pDragImage->DragShowNolock(TRUE));
	}
		
	CPropertyPage::OnMouseMove(nFlags, point);
}

/// Static function. Creates a CBitmap image from an HBITMAP source
CBitmap *CResourcePage::CreateThumbnail(CDC *pDC, HBITMAP hSrc)
{
	CDC memDC;
	CDC srcMemDC;
	CBitmap *tBitmap;
	int offX, offY;
	BITMAP bm;

	// create a memory DC to blit into
	memDC.CreateCompatibleDC(pDC);
	tBitmap = new CBitmap();
	tBitmap->CreateDiscardableBitmap(pDC, IMAGESIZEX, IMAGESIZEY);
	// get dimension of bitmap
	GetObject(hSrc, sizeof(BITMAP), &bm);

	CBitmap *oldBitmap = memDC.SelectObject(tBitmap);
	memDC.FillSolidRect(0, 0, IMAGESIZEX, IMAGESIZEY, pDC->GetBkColor());

	offX = (IMAGESIZEX-bm.bmWidth)/2;
	if (offX < 0)
		offX = 0;
	offY = (IMAGESIZEY-bm.bmHeight)/2;
	if (offY < 0)
		offY = 0;

	srcMemDC.CreateCompatibleDC(pDC);
	srcMemDC.SelectObject(hSrc);
	memDC.BitBlt(offX, offY, bm.bmWidth, bm.bmHeight, 
		&srcMemDC, 0, 0, SRCCOPY); 
	//memDC.FillSolidRect(0, 0, IMAGESIZEX, IMAGESIZEY, RGB(255,0,0));

	memDC.SelectObject(oldBitmap);

	return tBitmap;
}

// TODO: CResourcePage cannot handle returned NULL correctly. Implementation 
// fails inside GlobalFree with pointer to freed memory. This happens inside 
// CResourcePage::AddResource(). CResourcePage code might be need to reviewed 
// for fixing this problem
// TODO: fix HBITMAP's resource leak
HBITMAP CResourcePage::GenerateThumbnail( LPCTSTR filepath )
{
	IToolBox *pToolBox = EngineGetToolBox();
	CREATETHUMBNAILPARAMS ct(filepath);
	LPCTSTR szExt = FileHelpers::GetExtension(filepath);

	CString str = _T("CreateThumbnail_");
	str += szExt;
	CHashString hszCreateThumbnail = str;
	HBITMAP hBitmap = NULL;
	if (pToolBox->SendMessage(hszCreateThumbnail.GetUniqueID(), sizeof(ct), &ct) == MSG_HANDLED)
	{
		hBitmap = static_cast<HBITMAP>(ct.pThumbnail->Detach());
	}
	delete ct.pThumbnail;
	if (hBitmap != NULL)
	{
		return hBitmap;
	}
	ct.pThumbnail = NULL;

	static DWORD msgHash_CreateThumbnail = CHashString(_T("CreateThumbnail")).GetUniqueID();
	if (pToolBox->SendMessage(msgHash_CreateThumbnail, sizeof(ct), &ct) == MSG_HANDLED
		&& ct.pThumbnail != NULL)
	{
		hBitmap = *ct.pThumbnail;
	}
	delete ct.pThumbnail;
	if (hBitmap != NULL)
	{
		return hBitmap;
	}

	return GetThumbnailFromShell(filepath);
}

/// Static function. Generates a thumbnail of a file. Used in a separate thread for thumbnail generation.
HBITMAP CResourcePage::GetThumbnailFromShell( LPCTSTR filepath )
{
	LPCWSTR name;
    LPITEMIDLIST pidl;
	IShellFolder *sf;
	LPCITEMIDLIST newPidl;
	
#ifndef _UNICODE
	WCHAR pathNameW[MAX_PATH];

	MultiByteToWideChar(CP_ACP, 0, filepath,
        _tcslen(filepath)+1, pathNameW,   
		sizeof(pathNameW)/sizeof(pathNameW[0]) );
	name = pathNameW;
#else
	name = filepath;
#endif

	SHParseDisplayName(name, NULL, &pidl, 0, NULL);
	SHBindToParent(pidl, IID_IShellFolder, (void **)&sf, &newPidl);
	HBITMAP newBitmapHandle;

	newBitmapHandle = CExtractImageHelper::GetImage( *AfxGetMainWnd(), sf, newPidl,
		CSize(IMAGESIZEX, IMAGESIZEY));

	if (newBitmapHandle == NULL)
	{
		newBitmapHandle = CExtractImageHelper::GetImageFromIcon( *AfxGetMainWnd(),
				newPidl, CSize(ICONSIZEX, ICONSIZEX));
	}
	
	return newBitmapHandle;
}

/// Main Interface between resource pages and  recursion/thumbnail generation/watch threads
bool CResourcePage::ProcessThreadMessages()
{
	MSG msg;
	static HWND	CurrentResourceIterationParentHandle;
	static bool bProcessingIterationRequest = false;
	static bool bWatchDirectoryThreadIsRunning = false;
	static int	CurrentResourceIterationMaxSteps;
	static FINDDATALIST * CurrentResourceIterationList = NULL;
	static StdString CurrentResourceIterationDirectory = "";	
	static StdString CurrentResourceIterationExtension = "";
	//force the message queue
	if( PeekMessage(&msg, NULL, WM_USER, WM_POPULATE_RESOURCE_LIST, PM_REMOVE) )
	{
		//process the message here
		if( msg.message == WM_POPULATE_RESOURCE_LIST )
		{
			RESOURCEFINDSTEP * ResourceStep = (RESOURCEFINDSTEP*)msg.lParam;
			if( msg.wParam == sizeof( RESOURCEFINDSTEP * ))
			{
				//overwrite the current request
				CurrentResourceIterationMaxSteps = ResourceStep->MaxStep;
				CurrentResourceIterationParentHandle = ResourceStep->ParentHandle;
				if( _tcscmp( CurrentResourceIterationDirectory, ResourceStep->szSearchDirectory ) == 0 &&
					_tcscmp( CurrentResourceIterationExtension, ResourceStep->szExtensions ) == 0  )
				{
					//another request from the same resource page
				}
				else //kill watch thread if it's a new resource page wanting finds
				//if( bWatchDirectoryThreadIsRunning )
				{
					CurrentResourceIterationDirectory = ResourceStep->szSearchDirectory;
					CurrentResourceIterationExtension = ResourceStep->szExtensions;				
					if( bWatchDirectoryThreadIsRunning)
					{
						StopWatchThread();
						
					}
					bWatchDirectoryThreadIsRunning = true;
					StartWatchThread( CurrentResourceIterationDirectory, CurrentResourceIterationParentHandle);
			
				}	

				CurrentResourceIterationList = ResourceStep->ProcessList;
				bProcessingIterationRequest = true;		
				
				GlobalFree( ResourceStep );				
			}
		}
		switch ( msg.message)
		{
			case WM_KILL_THREAD:
				ExitThread(0);
		}
	}

	//Do a Recursion batch step and wait for next message
	if( bProcessingIterationRequest )
	{
		if( CurrentResourceIterationList->size() <= 0  ) //No more to iterate, do a watch directory
		{
			bProcessingIterationRequest = false;
		}
		else
		{
			// TODO: CurrentResourceIterationList may refer to deleted list while GDS is exiting
			SendProcessedBatch( CurrentResourceIterationParentHandle, *CurrentResourceIterationList, CurrentResourceIterationMaxSteps );
			// reset interrupt flag
			m_bInteruptFlag = false;
		}	
	}

	return bProcessingIterationRequest;
}

int CResourcePage::SendProcessedBatch(HWND ParentHandle, FINDDATALIST &out_list, int maxsteps )
{
	TCHAR fileextension[ _MAX_EXT ];
	TCHAR filename[ MAX_PATH ];
	int numProcessed = 0;
	BATCHCOMPLETEDTHUMBMSG * BatchMsg = (BATCHCOMPLETEDTHUMBMSG*)::GlobalAlloc( GMEM_FIXED, sizeof( BATCHCOMPLETEDTHUMBMSG) );
	FINDDATALIST::iterator LIter = out_list.begin();
	int i = 0;

	////////////////////////////////////////////////////////////////////////
	// The loop is converted into simply if condition, to eliminate timing
	// issues, when the arrays where modified inside the loop and in other 
	// threads as well. The simple if condition is more-or-less one processor 
	// instruction, if it fails, we skip everything instead of complex loops
	// which require complex condition checking
	if( LIter != out_list.end() )
	{
		if( m_bInteruptFlag )
		{
			// directory has been updated while generating thumbnails
			// releasing all mem allocs - Loop eleminated, less checking
			::GlobalFree( BatchMsg );

			// send reset message to main thread 
			::SendMessage( ParentHandle, ResetInteruptMessage, NULL, NULL);
			return 0;
		}
		FILEFOUNDDATA &curfile = (*LIter);
		_tsplitpath( curfile.Filepath, NULL, NULL, filename, fileextension );				
		RESOURCEPAGEINFO * resourceInfo = (RESOURCEPAGEINFO*)::GlobalAlloc( GMEM_FIXED, sizeof( RESOURCEPAGEINFO) );
		resourceInfo->Init();
		resourceInfo->pageIndex = 0;
		resourceInfo->rsrcIndex = 0;
		_tcscpy( resourceInfo->name, filename );
		_tcscat( resourceInfo->name, fileextension );
		_tcscpy( resourceInfo->filepath, curfile.Filepath );
		resourceInfo->lastWrite = curfile.LastWrite;
		resourceInfo->thumbNail = GenerateThumbnail( resourceInfo->filepath );
		// variable 'i' is only incremented here, as we are not in a loop any more 
		BatchMsg->m_GeneratedResources[ i++ ] = resourceInfo;
	}

	/////////////////////////////////////////////////////////////////
	// HERE COMES THE ORIGINAL LOOP VERSION - NOT THREAD SAFE!

/*	for( ; i < maxsteps && LIter != out_list.end(); i++, LIter++ )
	{
		if( m_bInteruptFlag )
		{
			// directory has been updated while generating thumbnails
			// release all mem allocs
			for( int j=0; j<i-1; j++ )
			{
				RESOURCEPAGEINFO *rinfo = (RESOURCEPAGEINFO*)BatchMsg->m_GeneratedResources[j];
				::GlobalFree( rinfo );
			}
			::GlobalFree( BatchMsg );

			// send reset message to main thread 
			::SendMessage( ParentHandle, ResetInteruptMessage, NULL, NULL);
			return 0;
		}
		FILEFOUNDDATA &curfile = (*LIter);
		_tsplitpath( curfile.Filepath, NULL, NULL, filename, fileextension );				
		RESOURCEPAGEINFO * resourceInfo = (RESOURCEPAGEINFO*)::GlobalAlloc( GMEM_FIXED, sizeof( RESOURCEPAGEINFO) );
		resourceInfo->Init();
		resourceInfo->pageIndex = 0;
		resourceInfo->rsrcIndex = 0;
		_tcscpy( resourceInfo->name, filename );
		_tcscat( resourceInfo->name, fileextension );
		_tcscpy( resourceInfo->filepath, curfile.Filepath );
		resourceInfo->lastWrite = curfile.LastWrite;
		resourceInfo->thumbNail = GenerateThumbnail( resourceInfo->filepath );
		BatchMsg->m_GeneratedResources[ i ] = resourceInfo;
	}
*/
	BatchMsg->numGenerated = i;
	//send a request back to save off thumbnails since we've finished processing this request
	::SendMessage( ParentHandle, ResourceAddMessage, sizeof( BATCHCOMPLETEDTHUMBMSG ), (LPARAM)BatchMsg );

	return i;
}


/// static Thumbnail Generation thread main
void CResourcePage::FillResourcesThread()
{
	const int sleeptime = 500;
	// MFC needs to be initialized for some functions with CBitmap to work
	CoInitialize( NULL );
	//process messages
	while( true )
	{
		bool processingThumbnails = ProcessThreadMessages();
		//Sleep so we aren't a CPU hog
		if( !processingThumbnails ) 
		{
			Sleep( sleeptime );
		}
		
	};
	// TODO: this code should be called on exit
	CoUninitialize();
}

LRESULT CResourcePage::OnAddMessage(WPARAM wParam, LPARAM lParam)
{
	//single
	if( wParam == sizeof( RESOURCEPAGEINFO * ) )
	{
		RESOURCEPAGEINFO * resourceInfo;	
		resourceInfo = (RESOURCEPAGEINFO*)lParam;
		if( resourceInfo )
		{		
			m_ResourcesList.SetRedraw( FALSE );
			AddResource( resourceInfo );			
			m_ResourcesList.SetRedraw( TRUE );
		}
	}
	//add batch
	else
	if( wParam == sizeof( BATCHCOMPLETEDTHUMBMSG ) )
	{
		BATCHCOMPLETEDTHUMBMSG * BatchInfo;	
		BatchInfo = (BATCHCOMPLETEDTHUMBMSG*)lParam;
		if( BatchInfo )
		{
///////////////////////////////////////////////////////////////////////////
//THIS IF CONDITION IS ADDED AS TEMPORARY SOLUTION
// The other thread processes the thumbnails and checks if the active 
// directory has changed during the process. This thread did not. 
// 'stopErase++' failed, because m_DrawThumbnails can be an empty array.  
// This if wont solve every timing issue, but eliminates the majority of them
			if ( !m_bInteruptFlag )
			{
///////////////////////////////////////////////////////////////////////////
				m_ResourcesList.SetRedraw( FALSE );		
				for( int i = 0; i < BatchInfo->numGenerated; i++ )
				{
					AddResource( BatchInfo->m_GeneratedResources[ i ] );
				}
				m_ResourcesList.SetRedraw( TRUE );					

				//remove from draw list
				FINDDATALIST::iterator startErase = m_DrawThumbnails.begin();
				FINDDATALIST::iterator stopErase = m_DrawThumbnails.begin();
				for( int k = 0; k < BatchInfo->numGenerated; k++ )
				{
					stopErase++;
				}
				m_DrawThumbnails.erase( startErase, stopErase );
///////////////////////////////////////////////////////////////////////
			}
///////////////////////////////////////////////////////////////////////
			::GlobalFree( BatchInfo );
		}
	}
	return TRUE;
}

/// Saves thumbnails to a file in the specified directory. Spews out the current contents
/// of the internal resource list of this instance of CResourcePage
void CResourcePage::CacheThumbnails( TCHAR * directory )
{
	// want to save the m_ResourceMap out to fisk
	CFile theFile;
	TCHAR fileout[ MAX_PATH ];
	int lenfileout;

	//force a directory refresh to save the current thumbnails
	SetActiveDirectory( m_szActiveDirectory );

	if ( m_ResourceMap.size() > 0 )
	{
		_tcscpy( fileout, directory );
		//check for post \\ or /
		lenfileout = _tcslen( fileout );
		if( fileout[ ( lenfileout - 1 ) ] != '\\' &&
			fileout[ ( lenfileout - 1 ) ] != '/' )
		{			
			_tcscat( fileout, "\\" );
		}
		_tcscat( fileout, THUMBNAIL_CACHE_FILENAME );	
		_tcscat( fileout, GetName() );
		//Cache the thumbnails in current resource directory
		if( theFile.Open( fileout,  CFile::modeCreate | CFile::modeWrite ) )
		{
			CArchive archive(&theFile, CArchive::store);
			LoadSaveResourceMap( &archive );
		}
	}
}

/// Loads a cached file of thumbnails
bool CResourcePage::LoadCachedThumbnails( TCHAR * directory )
{
	CFile theFile;
	TCHAR fileout[ MAX_PATH ];
	int lenfileout;
	
	_tcscpy( fileout, directory );
	//check for post \\ or /
	lenfileout = _tcslen( fileout );
	if( fileout[ ( lenfileout - 1 ) ] != '\\' &&
		fileout[ ( lenfileout - 1 ) ] != '/' )
	{			
		_tcscat( fileout, "\\" );
	}
	_tcscat( fileout, THUMBNAIL_CACHE_FILENAME );	
	_tcscat( fileout, GetName() );

	//Cache the thumbnails in current resource directory
	if( theFile.Open( fileout,  CFile::modeRead ) )
	{
		CArchive archive(&theFile, CArchive::load);
		LoadSaveResourceMap( &archive );
		return true;
	}
	return false;
}

void CResourcePage::LoadSaveResourceMap( CArchive * archive )
{
	RESOURCEPAGEINFO* os;
	CFile theFile;
	CFileStatus fileStatus;
	int mapSize;
	int submapSize;
	CBitmap *pImage;
	CDC *pDC;
	BITMAP bmpStruct;
	int numBits;
	int width;
	int height;

	if( archive )
	{
		if( archive->IsStoring() )
		{
			StdString filepath;
			INDEXTORESOURCE irmap;
			INDEXTORESOURCE::iterator itrIter;
			FILEPATHTORESOURCEMAP::iterator fptrmIter = m_ResourceMap.begin();
			mapSize = m_ResourceMap.size();
			// write out the size of m_ResourceMap
			(*archive) << mapSize;
			for( ; fptrmIter != m_ResourceMap.end(); fptrmIter++ )
			{
				filepath = (StdString)fptrmIter->first;
				irmap = (INDEXTORESOURCE)fptrmIter->second;
				itrIter = irmap.begin();
				submapSize = irmap.size();

				// write out the filepath
				ArchiveTChar( archive, (TCHAR*)filepath.c_str(), filepath.length() );
				// write out the size of INDEXTORESOURCE map
				(*archive) << submapSize;

				pDC = m_ResourcesList.GetDC();
				for( ; itrIter != irmap.end(); itrIter++ )
				{
					// write out the RESOURCEPAGEINFO data
					os = (RESOURCEPAGEINFO*)itrIter->second;
					ArchiveTChar( archive, os->name, MAX_PATH );
					ArchiveTChar( archive, os->filepath, MAX_PATH );
					(*archive).Write( &os->lastWrite, sizeof(os->lastWrite)) ;
					
					// write out the thumbnail if its been generated
					if (os->thumbNail)
					{

						pImage = CreateThumbnail( pDC, os->thumbNail );
						// write out the thumbnail's image bits
						pImage->GetBitmap( &bmpStruct );
						numBits = bmpStruct.bmWidthBytes*bmpStruct.bmHeight;
						// allocate memory for image 
						BYTE* pBuffer = new BYTE[numBits];
						pImage->GetBitmapBits( numBits, pBuffer );	
						// write the numbits into archive
						(*archive) << numBits;

						// we need the  size information as well
						(*archive) << bmpStruct.bmWidth;
						(*archive) << bmpStruct.bmHeight;

						// write the bits into archive
						archive->Write( pBuffer, numBits );
						// free memory 
						delete pBuffer;
						delete pImage;
					}
					else
					{
						// write out the zero size of bits
						(*archive) << 0;
					}
				}
				m_ResourcesList.ReleaseDC( pDC );
			}
		}
		else
		{
			// read the size of m_ResourceMap
			(*archive) >> mapSize;
			for( int i=0; i<mapSize; i++ )
			{
				TCHAR filepath[MAX_PATH];
				ArchiveTChar( archive, filepath, MAX_PATH );
				StdString stdstrFilePath( filepath );
				// read the size of this INDEXTORESOURCE map
				(*archive) >> submapSize;
				INDEXTORESOURCE itrMap;
				pDC = m_ResourcesList.GetDC();
				for( int j=0; j<submapSize; j++ )
				{
					bool error = false;

					//build the Resource info, global alloc because we create it in the thread usually, and
					//requires global free
					os = (RESOURCEPAGEINFO*)::GlobalAlloc( GMEM_FIXED, sizeof(RESOURCEPAGEINFO) );
					os->Init();
					ArchiveTChar( archive, os->name, MAX_PATH );
					ArchiveTChar( archive,  os->filepath, MAX_PATH );				
					(*archive).Read( &os->lastWrite, sizeof(os->lastWrite) );
					(*archive) >> numBits;
					
					if (numBits > 0)
					{
						// read size information (only if the bitmap is stored)
						(*archive) >> width;
						(*archive) >> height;

						// allocate memory for image 
						BYTE *pBuffer = new BYTE[numBits];
						// read in the image bits and create the bitmap
						archive->Read( pBuffer, numBits);
						os->thumbNail = ::CreateCompatibleBitmap( *pDC, width, height );
						
						// we need expected bitdepth of the current image
						CBitmap * tempBitmap = CBitmap::FromHandle( os->thumbNail );
						BITMAP tempInfo;
						tempBitmap->GetBitmap( &tempInfo );

						// the thumbnail size is 64x64 pixel. 
						// the buffer size should be 64 x 64 x BytesPerPixel. 
						// BytesPerPixel depends on the current DC. so numbits should be
						// 8192 (=64*64*2) for 16 bit images and 
						// 16382 (=64*64*4) for 32 bit images. If this is not true
						// they were generated for a different DC, should not be used
						// on this DC.
						int expectedSize = width * height * tempInfo.bmBitsPixel / 8;
						
						if ( expectedSize == numBits )
						{
							ASSERT(os->thumbNail);
							LONG byteCount = ::SetBitmapBits(os->thumbNail, numBits, pBuffer);
							
							if ( (byteCount <= 0) )
							{
								error = true;
								EngineGetToolBox()->Log(LOGWARNING, __T("%s(%d): Cached thumbnail can't be restored from the cache. Skipping thumbnail\n"), __FILE__, __LINE__ );
							}
						}
						else 
						{
							error = true;
							EngineGetToolBox()->Log(LOGWARNING, __T("%s(%d): Cached thumbnail does not match the bitdepth of the current device. Loading skipped.\n"), __FILE__, __LINE__ );
						}
						// free memory
						delete pBuffer;
					}

					// check to see if the file still exists and if we had any error earlier
					if( theFile.GetStatus( os->filepath,  fileStatus ) && !error )
					{			
						// file found...create INDEXTORESOURCEMAP entry
						itrMap.insert( make_pair( j, os ) );
					}
					else 
					{
						// not found...remove old, non existant entry
						::GlobalFree( os );
						j--;
						submapSize--;
					}
				}
				// add INDEXTORESOURCEMAP to m_ResourceMap;
				m_ResourceMap.insert( make_pair( stdstrFilePath, itrMap ) );
				m_ResourcesList.ReleaseDC( pDC );
			}
		}
	}
}

/// Loads or saves the current resource list information. Load/Save depends on archive's isStoring status
void CResourcePage::LoadSaveListInformation( CArchive * archive )
{
	int ItemCount;
	CFile theFile;
	CFileStatus fileStatus;
	RESOURCEPAGEINFO *os;
	if( archive  )
	{
		if( archive->IsStoring() )
		{
			//Loop through list and save off
			ItemCount = m_ResourcesList.GetItemCount();
			(*archive) << ItemCount;
			for( int i = 0; i < ItemCount; i++ )
			{			
				os = (RESOURCEPAGEINFO*)m_ResourcesList.GetItemData( i );
				if( os )
				{
					ArchiveTChar( archive, os->name, MAX_PATH );
					ArchiveTChar( archive, os->filepath, MAX_PATH);
					(*archive).Write( &os->lastWrite, sizeof(os->lastWrite)) ;
				}
				else
				{
					CString text = m_ResourcesList.GetItemText( i, 0 );		
					ArchiveTChar( archive, (TCHAR*)text.GetBuffer(), text.GetLength() );
					ArchiveTChar( archive, _T(""), 1 );
					FILETIME lastwrite = { 0,0 };
					(*archive).Write( &lastwrite, sizeof(lastwrite)) ;
				}
			}
		}
		else
		{
			try
			{
				(*archive) >> ItemCount;
				m_ResourcesList.SetItemCount( ItemCount );
				m_ResourcesList.SetRedraw( FALSE );
				for( int i = 0; i < ItemCount; i++ )
				{
					//build the Resource info, global alloc because we create it in the thread usually, and
					//requires global free
					os = (RESOURCEPAGEINFO*)::GlobalAlloc( GMEM_FIXED, sizeof( RESOURCEPAGEINFO) );
					os->Init();
					ArchiveTChar( archive, os->name, MAX_PATH );
					ArchiveTChar( archive,  os->filepath, MAX_PATH );				
					(*archive).Read( &os->lastWrite, sizeof(os->lastWrite)) ;
					//check valid file
					if( theFile.GetStatus( os->filepath,  fileStatus ) )
					{			
						//m_ResourcesList.ModifyStyle(0, LVS_ICON | LVS_SHAREIMAGELISTS );
						m_ResourcesList.InsertItem(i, os->name, i);
						m_ResourcesList.SetItemData( i, (DWORD)os);
					}
					else //Remove old, non existant entry
					{
						::GlobalFree( os );
						m_ThumbNails.Remove( i );
						ItemCount--;
						i--;
					}
				}
				m_ResourcesList.SetRedraw( TRUE );
			}
			catch(...)
			{
				// some error so abort the reading of the file. We got as much as we can
				theApp.GetLogger()->Print(_T("%s resource file is corrupt, possibly truncated.\n"),
					theFile.GetFileName());
			}
		}
	}
}

/// Writes/Reads a TCHAR string to a CArchive. write/read depends on archive's IsStoring() status
void CResourcePage::ArchiveTChar( CArchive * archive, TCHAR * str, DWORD maxSize )
{	
	DWORD curlen = 0;
	if( archive )
	{
		if( archive->IsStoring()  )
		{
			curlen = _tcslen( str );
			(*archive) << curlen;
			for(DWORD i = 0; i < curlen; i++ )
			{
				(*archive) << str[ i ];
			}
		}
		else
		{
			(*archive) >> curlen;
			if (curlen > maxSize)
			{
				curlen = maxSize;
			}
			DWORD i;
			for (i = 0; i < curlen; i++ )
			{
				(*archive) >> str[ i ];
			}
			str[ i ] = '\0';
		}
	}
}

/// On Destroy message, saves off any thumbnail caches it can.
void CResourcePage::OnDestroy()
{
	TCHAR ResourcePath[ MAX_PATH ];	
	RemoveDummyBitmap();

	CGDSApp * gApp;	
	gApp = dynamic_cast< CGDSApp * >( AfxGetApp() );			
	//load any thumbnails cached, otherwise create an empty list
	if( gApp )
	{
		// TO DO: update w/ the proper base path for the particular
		// plugin 
		_tcscpy( ResourcePath, gApp->GetResourcePath() );
		CacheThumbnails( ResourcePath );
	}
	
	CPropertyPage::OnDestroy();
}

/// Watches a directory for file changes
void CResourcePage::WatchDirectoryThread( POPULATERESOURCELISTPARAMS * params )
{
	TCHAR fileextension[ _MAX_EXT ];
	TCHAR RootDir[MAX_PATH];
	
	// Change notification is set. Now wait on both notification 
	// handles and refresh accordingly. 
	USES_CONVERSION;
	HANDLE hDir = CreateFile( 
		params->FilePath, /* pointer to the file name */
		FILE_LIST_DIRECTORY,                /* access (read-write) mode */
		FILE_SHARE_READ|FILE_SHARE_DELETE,  /* share mode */
		NULL, /* security descriptor */
		OPEN_EXISTING, /* how to create */
		FILE_FLAG_BACKUP_SEMANTICS, /* file attributes */
		NULL /* file with attributes to copy */
	);

	FILE_NOTIFY_INFORMATION Buffer[1024];
	DWORD BytesReturned;
	if( hDir != INVALID_HANDLE_VALUE )
	{
		while( ReadDirectoryChangesW(
		hDir, /* handle to directory */
		&Buffer, /* read results buffer */
		sizeof(Buffer), /* length of buffer */
		TRUE, /* monitoring option */
		FILE_NOTIFY_CHANGE_SECURITY|
		FILE_NOTIFY_CHANGE_CREATION|
		FILE_NOTIFY_CHANGE_LAST_WRITE|
		FILE_NOTIFY_CHANGE_SIZE|
		FILE_NOTIFY_CHANGE_ATTRIBUTES|
		FILE_NOTIFY_CHANGE_DIR_NAME|
		FILE_NOTIFY_CHANGE_FILE_NAME, /* filter conditions */
		&BytesReturned, /* bytes returned */
		NULL, /* overlapped buffer */
		NULL))/* completion routine */
		{
			//Handle based on action:
			DWORD Action = Buffer[0].Action;

			// zero RootDir so we dont have to worry about null terminating later
			memset( RootDir, 0, sizeof(RootDir) );

			//check the extension, are we keeping track of this?
			//split the path to get the extension
			_tcscpy( RootDir, params->FilePath );

			int lencurdir = strlen(params->FilePath);	
			//make sure we have trailing '\\'
			if( params->FilePath[ lencurdir-1 ] != '\\' && params->FilePath[ lencurdir-1 ] != '/' )
			{
				_tcscat( RootDir, _T("\\") );
			}
			//Gar must copy manually:
			int iRootDirLen = _tcslen( RootDir );
			for( int i = 0; i < (int)(Buffer[0].FileNameLength/(sizeof(wchar_t))); i++ )
			{
				RootDir[iRootDirLen+i] += (TCHAR)Buffer[0].FileName[ i ];
			}

			_tsplitpath( RootDir, NULL, NULL, NULL, fileextension );
			//test against available extensions
			if( strcmp( fileextension, ""  ) )
			{					
				FILEREFRESHPARAMS * fParam = (FILEREFRESHPARAMS *)::GlobalAlloc( GMEM_FIXED, sizeof( FILEREFRESHPARAMS ) );
				_tcscpy( fParam->FilePath, RootDir );
				fParam->Action = Action;
				::PostMessage( params->ParentHandle, ResourceFileChangedMessage, sizeof( FILEREFRESHPARAMS*), (LPARAM)fParam );
						
			}//directory change names
			else
			{
				if( Action != FILE_ACTION_MODIFIED )
				{
					::PostMessage( params->ParentHandle, ResourceFolderRefreshMessage, 0,0 );					
				}
			}
				
		}
	}
}

bool CResourcePage::GetSelectedResources( list< RESOURCEPAGEINFO > &InOutList )
{
	UINT uSelectedCount;	//Number of selected items
	int  nItem = -1;
	if( m_ResourcesList.GetItemCount() > 0 && m_ResourcesList.IsWindowVisible() )
	{
		uSelectedCount = m_ResourcesList.GetSelectedCount();
		// Update all of the selected items.
		for (UINT i=0; i < uSelectedCount; i++)
		{
			nItem = m_ResourcesList.GetNextItem(nItem, LVNI_SELECTED);
			//BAD
			if(nItem == -1)
			{
				return FALSE;
			}
			else //Got an item, add to list
			{
				RESOURCEPAGEINFO * rInfo = ( RESOURCEPAGEINFO * )m_ResourcesList.GetItemData( nItem );
				//this is the resource to add to a specific list you want
				InOutList.push_back( *rInfo );
			}
		}
		return TRUE;
	}
	return FALSE;
}

bool CResourcePage::SetSelectedResources( list<StdString> &InOutList )
{
	UINT uSelectedCount;	//Number of selected items
	int  nItem = -1;
	if( m_ResourcesList.GetItemCount() > 0 )
	{
		uSelectedCount = m_ResourcesList.GetSelectedCount();
		// Deselect all of the selected items.
		for (UINT i=0; i < uSelectedCount; i++)
		{
			nItem = m_ResourcesList.GetNextItem(nItem, LVNI_SELECTED);

			if (nItem != -1)
				m_ResourcesList.SetItemState( nItem, 0, LVNI_SELECTED ); // deselect
		}

		// for each string in our list of items to select
		for (list<StdString>::iterator itr = InOutList.begin(); itr != InOutList.end(); itr++)
		{
			// find the item in the valid resource map
			StdString szItemFullPath = RelativeToAbsolutePath( *itr, GetResourceDirectory() );
			std::map<StdString, FILEINDEXINFO>::iterator rcitr = m_ValidResources.find( szItemFullPath );
			if( rcitr != m_ValidResources.end() )
			{
				// select the item
				nItem = rcitr->second.m_Index;
				m_ResourcesList.SetItemState( nItem, (UINT)-1, LVNI_SELECTED ); // select
			}
		}
		return TRUE;
	}
	return FALSE;
}

LRESULT CResourcePage::OnFolderRefreshMessage( WPARAM wParam, LPARAM lParam )
{
	// If a directory has changed, we have to refresh all entries, find out which ones are valid, etc
    // we also have to make sure that while the thumbnail generation thread is running, we can't do anything
	// so the only time we can refresh is when the thumbnail generation is stopped.
	// We then set a flag that marks this directory as dirty, and on a Finish message from the thumbnail generation thread
	// A new refresh will start.
	
	RefreshDirectory();

	return TRUE;
}

void CResourcePage::RefreshDirectory()
{
	BuildValidResourceList();
	if ( VisibleThumbnails() )
	{
		//global alloc since we are using this across threads
		RESOURCEFINDSTEP * resourceInfo = (RESOURCEFINDSTEP*)::GlobalAlloc( GMEM_FIXED, sizeof( RESOURCEFINDSTEP) );
		_tcscpy( resourceInfo->szSearchDirectory, m_szActiveDirectory);
		_tcscpy( resourceInfo->szExtensions, m_szExtensions);
		resourceInfo->ProcessList = &m_DrawThumbnails;
		resourceInfo->MaxStep = THUMBNAIL_RECURSION_STEP;
		resourceInfo->ParentHandle = GetSafeHwnd();

		//We can send this off with no worries of overwriting previous messages
		//because this function is triggered by the watch directories thread, which
		//only starts once all other populate requests have finished
		::PostThreadMessage( m_ResourceThumbnailThreadID, WM_POPULATE_RESOURCE_LIST, sizeof( RESOURCEFINDSTEP * ), (LPARAM)resourceInfo );
	}
}


void CResourcePage::UpdateThumbnails()
{
	if( VisibleThumbnails() )
	{
		//global alloc since we are using this across threads
		RESOURCEFINDSTEP * resourceInfo = (RESOURCEFINDSTEP*)::GlobalAlloc( GMEM_FIXED, sizeof( RESOURCEFINDSTEP) );
		//_tcscpy( resourceInfo->szSearchDirectory, (TCHAR*)m_szResourceDirectory);
		_tcscpy( resourceInfo->szSearchDirectory, m_szActiveDirectory);
		_tcscpy( resourceInfo->szExtensions, m_szExtensions);
		resourceInfo->ProcessList = &m_DrawThumbnails;
		resourceInfo->MaxStep = THUMBNAIL_RECURSION_STEP;
		resourceInfo->ParentHandle = GetSafeHwnd();

		//We can send this off with no worries of overwriting previous messages
		//because this function is triggered by the watchdirectories thread, which
		//only starts once all other populate requests have finished
		::PostThreadMessage( m_ResourceThumbnailThreadID, WM_POPULATE_RESOURCE_LIST, sizeof( RESOURCEFINDSTEP * ), (LPARAM)resourceInfo );	
	}
}

bool CResourcePage::VisibleThumbnails()
{
	// check if resourcelist is initialized yet
	if (m_ResourcesList.m_hWnd == NULL)
	{
		return false;
	}

	if( (m_iView != LVS_SMALLICON) && (m_iView != LVS_ICON) )
	{
		return false;
	}

	
	// loop through only visible thumbnails
	INTEGERLIST visibleList = m_ResourcesList.GetVisibleItems();
	int count = m_ResourcesList.GetItemCount();
	INTEGERLIST::iterator ilIter = visibleList.begin();
	int start_index = 0;
	if (ilIter != visibleList.end())
		start_index = (int)(*ilIter);
	int end_index = start_index + visibleList.size() - 1;
	INDEXTOFILE::iterator itfIter;
	FILETOINFO::iterator ftiIter;
	for ( int index = start_index; index <= end_index; index++ )
	{
		itfIter = m_UnprocessedThumbnails.find( index );
		if ( itfIter == m_UnprocessedThumbnails.end() )
		{
			// disregard if not found
			continue;
		}
		FILEFOUNDDATA filedata = (FILEFOUNDDATA)(itfIter->second);
		ftiIter = m_ValidResources.find( filedata.Filepath );
		//ASSERT( ftiIter != m_ValidResources.end() );
		if (ftiIter == m_ValidResources.end() )
		{
			m_DrawThumbnails.clear();
			return false;
		}
		FILEINDEXINFO &curListing = (FILEINDEXINFO)(ftiIter->second);
		// check the creation time of the thumbnail
		if( CompareFileTime( &filedata.LastWrite, &curListing.m_FileTime ) != 0 )	
		{
			// remove the thumbnail to be drawn from the unprocessed map
			m_UnprocessedThumbnails.erase( itfIter );
			// add the thumbnail to be drawn to the draw list
			m_DrawThumbnails.push_back( filedata );
		}
	}
	
	return true;
}

LRESULT CResourcePage::OnFileChangedMessage( WPARAM wParam, LPARAM lParam )
{	
	FILEREFRESHPARAMS * Resourcerequest;	
	FILETIME zeroFile = {0 , 0 };
	TCHAR fileextension[ _MAX_EXT ];
	TCHAR filename[ MAX_PATH ];
	CFile theFile;
	CFileStatus fileStatus;	
	RESOURCEFINDSTEP * resourceInfo;
	int curItemCount = m_ResourcesList.GetItemCount();
	Resourcerequest = (FILEREFRESHPARAMS*)lParam;
	VERIFY( wParam == sizeof( FILEREFRESHPARAMS * ) );
	if( Resourcerequest )
	{
		_tcslwr( Resourcerequest->FilePath );
		_tsplitpath( Resourcerequest->FilePath, NULL, NULL, filename, fileextension );		

		//figure out what to do
		switch( Resourcerequest->Action )
		{			
			case FILE_ACTION_MODIFIED:
			case FILE_ACTION_ADDED:
			case FILE_ACTION_RENAMED_NEW_NAME:
				//make sure it's not a modified(delete) one...
				if( theFile.GetStatus( Resourcerequest->FilePath, fileStatus ) )
				{
					if( _tcsstr( m_szExtensions, &fileextension[1] ) )
					{
						//Put the dummy node in the list first
						//first check if already in valid resources map
						map< StdString, FILEINDEXINFO >::iterator iter = m_ValidResources.find( StdString( Resourcerequest->FilePath ) );
						if( iter == m_ValidResources.end() )
						{
							//add new entry to mapping
							FILEINDEXINFO finfo;
							finfo.m_FileTime = zeroFile;
							finfo.m_Index = curItemCount;
							m_ValidResources.insert( pair< StdString, FILEINDEXINFO>( Resourcerequest->FilePath, finfo ) );
							//If not valid that means no dummy node has yet been added						
							//insert with dummy thumbnail		
							RESOURCEPAGEINFO * pData = (RESOURCEPAGEINFO*)::GlobalAlloc( GMEM_FIXED, sizeof( RESOURCEPAGEINFO ) );
							pData->Init();
							pData->pageIndex = 0;
							pData->rsrcIndex = 0;
							_tcscpy( pData->name, filename );
							_tcscat( pData->name, fileextension );
							_tcscpy( pData->filepath, (TCHAR*)Resourcerequest->FilePath );
							pData->lastWrite = zeroFile;
							pData->thumbNail = 0;
							m_ResourcesList.InsertItem( curItemCount, pData->name );
							m_ThumbNails.Add( m_BlankBitmap, RGB(0,0,0));	
							m_ResourcesList.SetItemData( curItemCount, (LPARAM)pData );			
							m_ResourcesList.SetItem( curItemCount, 0, LVIF_IMAGE, pData->name,curItemCount, 0, 0, (LPARAM)pData );
						}
						//global alloc since we are using this across threads
						FILEFOUNDDATA newRequest;
						newRequest.Filepath = Resourcerequest->FilePath;
						newRequest.IsDirty = true;
						newRequest.LastWrite = zeroFile;//fileStatus.m_mtime;
						// add to draw queue for immediate drawing.
						m_DrawThumbnails.push_back( newRequest);
						resourceInfo = (RESOURCEFINDSTEP*)::GlobalAlloc( GMEM_FIXED, sizeof( RESOURCEFINDSTEP) );
						//_tcscpy( resourceInfo->szSearchDirectory, (TCHAR*)m_szResourceDirectory);
						_tcscpy( resourceInfo->szSearchDirectory, m_szActiveDirectory);
						_tcscpy( resourceInfo->szExtensions, m_szExtensions);					
						resourceInfo->MaxStep = THUMBNAIL_RECURSION_STEP;
						resourceInfo->ProcessList = &m_DrawThumbnails;
						resourceInfo->ParentHandle = GetSafeHwnd();
						//We can send this off with no worries of overwriting previous messages
						//because this function is triggered by the watchdirectories thread, which
						//only starts once all other populate requests have finished
						::PostThreadMessage( m_ResourceThumbnailThreadID, WM_POPULATE_RESOURCE_LIST, sizeof( RESOURCEFINDSTEP * ), (LPARAM)resourceInfo );
					}
				}
				break;		
			case FILE_ACTION_REMOVED:				
			case FILE_ACTION_RENAMED_OLD_NAME:
				{					
					//We have to update the INDEX<->NAME mapping when we remove.
					//this is because removing from the list will shift all indices by one down
					if( _tcsstr( m_szExtensions, &fileextension[1] ) )
					{
						//find the index
						std::map< StdString, FILEINDEXINFO >::iterator iter = m_ValidResources.find( StdString( Resourcerequest->FilePath ) );
						if( iter != m_ValidResources.end() )
						{
							FILEINDEXINFO &curInfo = (*iter).second;
							RemoveResource( curInfo.m_Index );
						}
					}
				}
				break;
			default:
				//should never happen
				break;
		};
	}
	return TRUE;

}


void CResourcePage::BuildValidResourceList()
{
	CFile theFile;
	CFileStatus fileStatus;
	FILETIME zerofile = { 0,0 };
	//First, remove non-existant entries, clean up our list:
	m_ValidResources.clear();
	int itemCount = m_ResourcesList.GetItemCount();
	for( int i = 0; i < itemCount; i++ )
	{
		RESOURCEPAGEINFO * rInfo = ( RESOURCEPAGEINFO * ) m_ResourcesList.GetItemData( i );
		if( rInfo )
		{
			_tcslwr( rInfo->filepath );
		}
		//file does not exist anymore
		if( !rInfo || !theFile.GetStatus( rInfo->filepath, fileStatus ) )
		{
			RemoveResource( i );
			i--;
			itemCount--;
		}
		else 
		{
			//add to current list
			FILEINDEXINFO finfo;
			finfo.m_FileTime = rInfo->lastWrite;
			finfo.m_Index = i;
			m_ValidResources.insert( pair< StdString, FILEINDEXINFO>( StdString( rInfo->filepath), finfo ) );
		}//otherwise don't add to list

	}
	//Next recurse and find all new files or files that are dirty
	CIterativeDirectoryRecursion	DIter;

	//Clear queue
	m_UnprocessedThumbnails.clear();
	// clear the queue of thumbnails that are to be drawn
	m_DrawThumbnails.clear();

	//Look for dirty/new files
	int steps = 0;	
		
	DIter.Reset ( m_szActiveDirectory, m_szExtensions );

	int countDirty = 0;

	do
	{
		steps = DIter.DoSteps( 100, m_UnprocessedThumbnails, m_ValidResources, countDirty, false );
	}
	while( steps != 0 );

	//iterate through queue and insert into resourcelist with blank bitmap and dirty file 
	//if it doesn't already exist in the mValidResources list
	if( m_UnprocessedThumbnails.size() > 0 )
	{
		//Prepare the list control to add a large number of items so it's a lot faster
		int curItemCount = m_ResourcesList.GetItemCount();
		m_ResourcesList.SetItemCount( m_UnprocessedThumbnails.size() + curItemCount - countDirty );
		int curImageCount = m_ThumbNails.GetImageCount();
		m_ThumbNails.SetImageCount( m_UnprocessedThumbnails.size() + curImageCount - countDirty );
		
		//check that we're matching
		ASSERT( curImageCount == curItemCount );		
		m_ResourcesList.SetImageList(&m_ThumbNails, LVSIL_NORMAL);	
		INDEXTOFILE::iterator LIter = m_UnprocessedThumbnails.begin();
		TCHAR fileextension[ _MAX_EXT ];
		TCHAR filename[ MAX_PATH ];
		TCHAR name[ MAX_PATH ];
		//Loop through all unprocessed thumbnails
		for( ; LIter != m_UnprocessedThumbnails.end() ;  )
		{
			FILEFOUNDDATA &curRef = LIter->second;
			//first check if already in valid resources map
			_tcslwr( const_cast<char*>((const char*)curRef.Filepath) );
			if( !curRef.IsDirty )
			{
				//extract the file name
				_tsplitpath( curRef.Filepath, NULL, NULL, filename, fileextension );				
				_tcscpy( name, filename );
				_tcscat( name, fileextension );
				//insert with dummy thumbnail			
				//m_ThumbNails.Add( m_BlankBitmap, RGB(0,0,0) );
				//m_ResourcesList.SetItemText(curItemCount, 0, name);
				m_ResourcesList.InsertItem( curItemCount, name );
				m_ThumbNails.Replace(curItemCount,m_BlankBitmap, NULL );
				RESOURCEPAGEINFO * pData = (RESOURCEPAGEINFO*)::GlobalAlloc( GMEM_FIXED, sizeof( RESOURCEPAGEINFO ) );
				pData->Init();
				pData->pageIndex = 0;
				pData->rsrcIndex = 0;
				_tcscpy( pData->name, name );
				_tcscpy( pData->filepath, curRef.Filepath );
				pData->lastWrite = zerofile;
				pData->thumbNail = 0;
				m_ResourcesList.SetItemData( curItemCount, (LPARAM)pData );			
				m_ResourcesList.SetItem( curItemCount, 0, LVIF_IMAGE, name,curItemCount, 0, 0, (LPARAM)pData );				
				//add new entry to mapping
				FILEINDEXINFO finfo;
				finfo.m_FileTime = zerofile;
				finfo.m_Index = curItemCount;
				m_ValidResources.insert( pair< StdString, FILEINDEXINFO>( curRef.Filepath, finfo ) );				
				curItemCount++;
			}
			LIter++;
		}
	}
}

const TCHAR * CResourcePage::GetResourceDirectory()
{	
	return m_szResourceDirectory;
}

const TCHAR * CResourcePage::GetName() const
{
	return m_szName;
}

const TCHAR * CResourcePage::GetExtensionsFilter()
{
	return m_szExtensions;
}

bool CResourcePage::GetResourceInfoByRelativePath( GETRESOURCEINFOBYRELATIVEPATH * searchparam )
{
	TCHAR fullp[ MAX_PATH ];
	_tcscpy( fullp, RelativeToAbsolutePath( searchparam->findRelativePath, GetResourceDirectory() ) );

	//ok now we have the full path, we can compare to the valid resources list:
	std::map< StdString, FILEINDEXINFO >::iterator iter = m_ValidResources.find( fullp );
	if( iter != m_ValidResources.end() )
	{
		//found, let's send back a file info
		FILEINDEXINFO * pfInfo = &((*iter).second);
		RESOURCEPAGEINFO * resourceEntry = (RESOURCEPAGEINFO*)m_ResourcesList.GetItemData( pfInfo->m_Index );
		searchparam->rInfo = *resourceEntry;
		_tcscpy( searchparam->rInfo.filepath, EngineGetToolBox()->GetRelativePath( searchparam->rInfo.filepath ) );
		CBitmap * destBitmap = GetThumbnailFromListIndex( pfInfo->m_Index );
		searchparam->rInfo.thumbNail = (HBITMAP)CopyImage( (HBITMAP)destBitmap->GetSafeHandle(), IMAGE_BITMAP, 0,0,0 );
		if( destBitmap )
		{
			delete destBitmap; 
		}
		return true;
	}
	else
	{
		return false;
	}
}

void CResourcePage::PauseGenerationThread( bool value )
{
	//only change when current status does not equal
	if( CResourcePage::m_PauseThumbnailGenerationThread != value &&
		m_ResourceThumbnailThreadHandle != INVALID_HANDLE_VALUE && 
		m_ResourceWatchThreadHandle != INVALID_HANDLE_VALUE )
	{
		CResourcePage::m_PauseThumbnailGenerationThread = value; //make it equal
		if( CResourcePage::m_PauseThumbnailGenerationThread )
		{
			SuspendThread( m_ResourceThumbnailThreadHandle );
			SuspendThread( m_ResourceWatchThreadHandle );
		}
		else
		{
			ResumeThread( m_ResourceThumbnailThreadHandle );
			ResumeThread( m_ResourceWatchThreadHandle );
		}
	}
}

void CResourcePage::GetImageFromList(CImageList *lstImages, 
                             int nImage, CBitmap* destBitmap)
{
     //First we want to create a temporary image list we can manipulate
     CImageList tmpList;
     tmpList.Create(lstImages);
     
    //Then swap the requested image to the first spot in the list 
    tmpList.Copy( 0, nImage, ILCF_SWAP );
    
    //Now we need to get som information about the image 
    IMAGEINFO lastImage;
    tmpList.GetImageInfo(0,&lastImage);

	//IMAGEINFO lastImage;
	//1stImage->GetImageInfo(nImage, &lastImage);

    //Heres where it gets fun
    //Create a Compatible Device Context using 
    //the valid DC of your calling window
    CDC dcMem; dcMem.CreateCompatibleDC (GetWindowDC()); 
    
    //This rect simply stored the size of the image we need
    CRect rect (lastImage.rcImage);

    //Using the bitmap passed in, Create a bitmap 
    //compatible with the window DC
    //We also know that the bitmap needs to be a certain size.
    destBitmap->CreateCompatibleBitmap (this->GetWindowDC(), 
                                      rect.Width (), rect.Height ());
    
    //Select the new destination bitmap into the DC we created above
    CBitmap* pBmpOld = dcMem.SelectObject (destBitmap);
    
    //This call apparently "draws" the bitmap from the list, 
    //onto the new destination bitmap
    tmpList.DrawIndirect (&dcMem, 0, CPoint (0, 0), 
           CSize (rect.Width (), rect.Height ()), CPoint (0, 0));
    
    //cleanup by reselecting the old bitmap object into the DC
    dcMem.SelectObject (pBmpOld);
}

LRESULT CResourcePage::OnResetInteruptMessage(WPARAM wParam, LPARAM lParam)
{
	m_bInteruptFlag = false;

	return 0;
}

void CResourcePage::SetActiveDirectory(StdString &newDir)
{
	m_bInteruptFlag = true;

	RESOURCEPAGEINFO* os;

	// check if there are any resources to save
	if ( m_ResourcesList.GetItemCount() > 0 )
	{
		INDEXTORESOURCE IndexToResource;
		// check if key is already in map
		FILEPATHTORESOURCEMAP::iterator fptrmIter = m_ResourceMap.find( m_szActiveDirectory );
		if( fptrmIter != m_ResourceMap.end() )
		{
			// remove the entry as it will get replaced below
			m_ResourceMap.erase( fptrmIter );
		}

		// save off current directories resources
		for( int i=0; i<m_ResourcesList.GetItemCount(); i++ )
		{
			// get the resource info
			os = (RESOURCEPAGEINFO*)m_ResourcesList.GetItemData(i);

			IndexToResource.insert( make_pair( i, os ) );

			// clear an entry from the imagelist
			m_ThumbNails.Remove(0);
		}
		// save off resource
		m_ResourceMap.insert( make_pair( m_szActiveDirectory, IndexToResource ) );
	}
	// change directories
	m_szActiveDirectory = newDir;

	// clear the resources list
	m_ResourcesList.DeleteAllItems();

	// update m_ResourcesList w/ new resources
	FILEPATHTORESOURCEMAP::iterator fptrmIter = m_ResourceMap.find( m_szActiveDirectory );
	if( fptrmIter != m_ResourceMap.end() )
	{
		int iPos = 0;
		CBitmap *pImage;
		INDEXTORESOURCE itrMap = (INDEXTORESOURCE)(fptrmIter->second);
		INDEXTORESOURCE::iterator itrIter = itrMap.begin();
		int size = itrMap.size();

		// set the size of the imagelist
		m_ThumbNails.SetImageCount(size);

		// traverse through all saved resources for this directory and
		// add to m_ResourcesList and m_ThumbNails 
		CDC *pDC = m_ResourcesList.GetDC();
		while( itrIter != itrMap.end() )
		{
			os = itrIter->second;

			// add to imagelist
			pImage = CreateThumbnail( pDC, os->thumbNail );
			BOOL bReplaceSuccess = m_ThumbNails.Replace( iPos, pImage, NULL );
			ASSERT( bReplaceSuccess );
			delete pImage;

			// set the correspondence btw resourceslist and the imagelist
			m_ResourcesList.InsertItem( LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM , iPos, os->name, 0, 0, iPos, (LPARAM)os );

			itrIter++;
			iPos++;
		}
		m_ResourcesList.ReleaseDC(pDC);
	}
	RefreshDirectory();
}

BOOL CResourcePage::OnSetActive()
{
	// get the active directory in case the directory changed before
	// this resource page was activated
	CHashString hashPath(_T(""));
	static DWORD msgHash_GetResourceActiveDirectory = CHashString(_T("GetResourceActiveDirectory")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetResourceActiveDirectory, sizeof(IHashString*), &hashPath);
	StdString szNewDir = hashPath.GetString();

	// check if the directory changed
	if ( _tcscmp( m_szActiveDirectory, szNewDir ) != 0 )
	{
		SetActiveDirectory( szNewDir );
	}
	else
	{
		RefreshDirectory();
	}
	return CPropertyPage::OnSetActive();
}

void CResourcePage::CreateDummyBitmap()
{
	if( !m_BlankBitmap )
	{
		CDC memDC;
		CBitmap *tBitmap;
		CDC * pDC = m_ResourcesList.GetDC();
		// create a memory DC to blit into
		memDC.CreateCompatibleDC( pDC );
		tBitmap = new CBitmap();
		tBitmap->CreateDiscardableBitmap( pDC, IMAGESIZEX, IMAGESIZEY);
		CBitmap *oldBitmap = memDC.SelectObject(tBitmap);
		memDC.FillSolidRect(0, 0, IMAGESIZEX, IMAGESIZEY, pDC->GetBkColor());
		memDC.FillSolidRect(0, 0, IMAGESIZEX, IMAGESIZEY, RGB(255,0,255));
		memDC.SelectObject(oldBitmap);

		m_ResourcesList.ReleaseDC(pDC);
		m_BlankBitmap = tBitmap;
	}
}

void CResourcePage::RemoveDummyBitmap()
{
	if( m_BlankBitmap )
	{
		delete m_BlankBitmap;
		m_BlankBitmap = NULL;
	}
}

bool CIterativeDirectoryRecursion::CreateSearchFrame( const TCHAR * curdir )
{
	ITERATION_FRAME frame;
	//build a search out of the path:
	int lencurdir = strlen(curdir);	
	frame.cur_search = curdir;
	frame.DataDirectory = curdir;
	//make sure we have trailing '\\'
	if( curdir[ lencurdir-1 ] != '\\' && curdir[ lencurdir-1 ] != '/' )
	{
		frame.cur_search += "\\";
		frame.DataDirectory += "\\";
	}
	frame.cur_search += "*";
	frame.hSearch = FindFirstFile( (TCHAR*)frame.cur_search.c_str(),  &frame.curFileData );				
	if (frame.hSearch != INVALID_HANDLE_VALUE) 
	{
		m_ProcessStack.push( frame );
		return true;
	} 
	return false;
}

CIterativeDirectoryRecursion::CIterativeDirectoryRecursion(  )
{	
	m_Extension = "";
	m_Directory = "";
}

void CIterativeDirectoryRecursion::IterateSearchFrame()
{
	ITERATION_FRAME &frame = m_ProcessStack.top();
	//iterate the search frame
	if (!FindNextFile( frame.hSearch, &frame.curFileData)) 
	{
		//remove this		
		FindClose( frame.hSearch );
		m_ProcessStack.pop();
	}
}

int CIterativeDirectoryRecursion::DoSteps( int numSteps, INDEXTOFILE &out_list, FILETOINFO &NotInList,
										int &countDirty, bool bRecurse )
{
	std::string cur_path;
	StdString cur_search;
	bool bKeepSearching = true;
	int itersteps = 0;
	for( ;
		itersteps < numSteps && 
		!m_ProcessStack.empty(); )
	{
		//grab top of stack and process one loop
		ITERATION_FRAME &CurStackFrame = m_ProcessStack.top();
		//process the frame:
		WIN32_FIND_DATA FindFileData = CurStackFrame.curFileData;
		
		//No going back up! Stupid windows searches...
		if( strcmp( FindFileData.cFileName, ".." ) &&
			strcmp( FindFileData.cFileName, "." ) )
		{
			
			//it's a directory Recurse inside of it!
			if(  FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				if ( bRecurse )
				{
					cur_path = CurStackFrame.DataDirectory;
					cur_path += FindFileData.cFileName;
					cur_path += "\\";
					//new frame
					IterateSearchFrame();				
					CreateSearchFrame( (TCHAR*)cur_path.c_str() );		
					continue;//continue the loop
				}
			}
			else //it's a file, send a message to add it
			{
				//split the path to get the extension
				TCHAR fileextension[ _MAX_EXT ];
				_tsplitpath( FindFileData.cFileName, NULL, NULL, NULL, fileextension );
				_tcslwr(fileextension);

				// test against available extensions
				//This works because I'm comparing if the entirity of fileextension is in any
				//place in extension token. v3Oxxx will not be inside extension token
				if( _tcsstr(m_Extension, &fileextension[1]))
				{
					//add
					FILEFOUNDDATA newfound;
					newfound.Filepath =  CurStackFrame.DataDirectory;
					newfound.Filepath += FindFileData.cFileName;
					newfound.IsDirty = false;
					_tcslwr( const_cast<char*>((const char*)newfound.Filepath) );
					newfound.LastWrite = FindFileData.ftLastWriteTime;
					//check if in NotInList
					FILETOINFO::iterator iter =  NotInList.find( newfound.Filepath  );					
					if( iter == NotInList.end() ) //not in cache, or file just got added to directory
					{
						int index = out_list.size();		
						out_list.insert( make_pair( index, newfound ) );
					}
					else //In valid list, check times
					{
						FILEINDEXINFO &curListing = (*iter).second;
						//different file times
						if( CompareFileTime( &newfound.LastWrite, &curListing.m_FileTime ) != 0 )
						{
							int index = curListing.m_Index;
							//set dirty flag and push
							newfound.IsDirty = true;
							out_list.insert( make_pair( index, newfound ) );
							countDirty++;
						}
					}
					itersteps++;
				}
			}
		}
		IterateSearchFrame();	
	}
	return itersteps;
}

void CIterativeDirectoryRecursion::DeinitializeProcessStack()
{
	//Deinit the searches in this process stack
	while( !m_ProcessStack.empty() )
	{
		ITERATION_FRAME &frame = m_ProcessStack.top();
		FindClose( frame.hSearch );
		m_ProcessStack.pop();
	}
}

void CIterativeDirectoryRecursion::Reset( const TCHAR * dir, const TCHAR * extension )
{
	DeinitializeProcessStack();
	m_Extension = extension;
	m_Directory = dir;	
	CreateSearchFrame( m_Directory );
}

// on these messages, update the visible thumbnails
void CResourcePage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);

	UpdateThumbnails();
}

void CResourcePage::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CPropertyPage::OnKeyDown(nChar, nRepCnt, nFlags);

	UpdateThumbnails();
}

BOOL CResourcePage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	BOOL bVal = CPropertyPage::OnMouseWheel(nFlags, zDelta, pt);

	UpdateThumbnails();

	return bVal;
}

void CResourcePage::OnNMDblclkObjectsList(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	LPNMITEMACTIVATE lpItem = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if ( lpItem->iItem < 0 )
	{
		// -1 means that no item was click on, but the background of the window.
		return;
	}
	RESOURCEPAGEINFO* pResourceInfo = reinterpret_cast<RESOURCEPAGEINFO*>(m_ResourcesList.GetItemData(lpItem->iItem));
	if (pResourceInfo == NULL)
	{
		return;
	}

	if (ActivateOpenedDocument(pResourceInfo->filepath))
	{
		return;
	}

	IToolBox *toolBox = EngineGetToolBox();
	
	LPTSTR szExt = (LPTSTR)FileHelpers::GetExtension(pResourceInfo->filepath);
	if (szExt != NULL)
	{
		// notify about long time loading for new resource
		BeginWaitCursor();

		CString messageName = _T("FileLoaded_");
		messageName += _tcslwr(szExt);
		// send the message
		DWORD msgHash_messageName = CHashString(messageName).GetUniqueID();
		DWORD res = toolBox->SendMessage(msgHash_messageName, sizeof(LPCTSTR), pResourceInfo->filepath);
		switch (res)
		{
			case MSG_ERROR:
				MessageBox(_T("Failed to open resource for editing"), _T("Error"), MB_ICONERROR);
				break;
			case MSG_WARNING:
				MessageBox(_T("Warnig message was sent while opening resource for editing"), _T("Warning"), MB_ICONASTERISK);
				break;
		}

		EndWaitCursor();
	}
}

bool CResourcePage::ActivateOpenedDocument(LPCTSTR szFilename)
{
	CWinApp *pApp = AfxGetApp();
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	while (pos != NULL)
	{
		CDocTemplate *pDocTemplate = pApp->GetNextDocTemplate(pos);
		POSITION posDoc = pDocTemplate->GetFirstDocPosition();
		while (posDoc != NULL)
		{
			CDocument *pDoc = pDocTemplate->GetNextDoc(posDoc);
			if (0 == pDoc->GetPathName().CompareNoCase(szFilename))
			{
				POSITION posView = pDoc->GetFirstViewPosition();
				if (posView != NULL)
				{
					CView *pView = pDoc->GetNextView(posView);
					CMainFrame *pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
					pMainFrame->SetActiveView(pView);
					return true;
				}
				return false;
			}
		}
	}
	return false;
}

void CResourcePage::SetView( DWORD iView )
{
	m_iView = iView;
	m_ResourcesList.ModifyStyle(LVS_TYPEMASK, m_iView );

	CMenu* pPopup = m_ViewMenu.GetSubMenu(0);

	switch( m_iView )
	{
	case LVS_ICON:
		pPopup->CheckMenuRadioItem( ID_OBJVIEW_LARGEICON, ID_OBJVIEW_DETAILS, ID_OBJVIEW_LARGEICON, MF_BYCOMMAND );
		break;
	case LVS_SMALLICON:
		pPopup->CheckMenuRadioItem( ID_OBJVIEW_LARGEICON, ID_OBJVIEW_DETAILS, ID_OBJVIEW_SMALLICON, MF_BYCOMMAND );
		break;
	case LVS_LIST:
		pPopup->CheckMenuRadioItem( ID_OBJVIEW_LARGEICON, ID_OBJVIEW_DETAILS, ID_OBJVIEW_LIST, MF_BYCOMMAND );
		break;
	case LVS_REPORT:
		pPopup->CheckMenuRadioItem( ID_OBJVIEW_LARGEICON, ID_OBJVIEW_DETAILS, ID_OBJVIEW_DETAILS, MF_BYCOMMAND );
		break;
	}
}


void CResourcePage::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu* pPopup = m_ViewMenu.GetSubMenu(0);
	assert(pPopup != NULL);
   	
	int command = (int)pPopup->TrackPopupMenu(	TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_LEFTBUTTON | TPM_RETURNCMD, 
												point.x, point.y, this);

	CMenu* pSubPopup = pPopup->GetSubMenu(0);
	assert(pSubPopup != NULL);

	switch (command)
	{
	case ID_OBJVIEW_LARGEICON:
		{
			SetView( LVS_ICON );
			break;
		}
	
	case ID_OBJVIEW_SMALLICON:
		{
			//SetView( LVS_SMALLICON );
			break;
		}
        
	case ID_OBJVIEW_LIST:
		{
			SetView( LVS_LIST );
			break;
		}

	case ID_OBJVIEW_DETAILS:
		{
			//SetView( LVS_REPORT );
			break;
		}
	}
	OnSetActive();
}

void CResourcePage::SetResouceDirectory( StdString	newDir )
{
	// only for the existing pages, they are initialized only on demand...
	// Every page access the latest resouce directory during initialization,
	// we don't need to care about the not initialized ones here.
	// If a page is not initialized yet m_ResourcesList will be NULL
	if ( m_ResourcesList )  
	{
		// save the thumbnails for the earlier directory	
		CacheThumbnails( (TCHAR*) m_szResourceDirectory.c_str() );

		// set the new directory
		m_szResourceDirectory = newDir; 

		//TODO: Check if the earlier cache is cleared, we do not need the earlier
		// project's cache any more!
		LoadCachedThumbnails( (TCHAR*) m_szResourceDirectory.c_str() );
	}
}