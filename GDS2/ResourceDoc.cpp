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
//#include "ResourceDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// User-defined predicate function for sorting.
// this has to be defined locally or compiler put's it in whoever
// includes the header file.  Make sure both functions match.
struct lessThan : public greater<RESOURCECOLLECTION *>
{
    bool operator()(const RESOURCECOLLECTION* x, const RESOURCECOLLECTION* y) const
    { 
		return (x->priority < y->priority); 
    };

};

/////////////////////////////////////////////////////////////////////////////
// CResourceDoc

IMPLEMENT_DYNCREATE(CResourceDoc, CDocument)

BEGIN_MESSAGE_MAP(CResourceDoc, CDocument)
	//{{AFX_MSG_MAP(CResourceDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceDoc construction/destruction

CResourceDoc::CResourceDoc()
{
	SINGLETONINSTANCE(CResourceComponent)->SetParent(this);
	obOptions = NULL;
	m_szActiveDirectory = _T("");
}

CResourceDoc::~CResourceDoc()
{
	m_Icons.DeleteImageList();

	// move here, that way window and pages are still up.
	vector<CResourcePage *>::iterator itRPage;

	itRPage = m_ResourcePages.begin();
	while (itRPage != m_ResourcePages.end())
	{
		delete *itRPage;
		itRPage++;
	}

	if (obOptions != NULL)
	{
		delete obOptions;
		obOptions = NULL;
	}
}

BOOL CResourceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CResourceDoc serialization

void CResourceDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

//---------------------------------------------------------------------
// Function:	InitResourcePages
// Description:	This functions initializes the tab control with 
//				resource information
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
void CResourceDoc::InitResourcePages()
{
	CResourcePage *rPage;
	int retVal;
	DWORD CollectionCount;
	RESOURCECOLLECTIONLIST::iterator itObjCollList;
	RESOURCECOLLECTION *curObjCollection;

	CollectionCount = 0;
	m_ResourceCollections.sort(lessThan());

	for (itObjCollList = m_ResourceCollections.begin(); 
			itObjCollList != m_ResourceCollections.end(); itObjCollList++)
	{
    	curObjCollection = *itObjCollList;
	    retVal = IDRETRY;
		while (retVal == IDRETRY)
		{
			rPage = new CResourcePage( curObjCollection );
			if (rPage == NULL)
			{
				retVal = MessageBox(NULL, _T("Unable to allocate resource page\n"), 
						_T("Memory Error"), MB_ABORTRETRYIGNORE);
				if (retVal == IDABORT)
				{
					// immediatly exit?
					AfxGetApp()->ExitInstance();
				}
			}
			else
			{
				retVal = IDIGNORE;
			}
		}

#ifdef _DEBUG
		// somebody forgot to fill this in
		ASSERT(_CrtIsValidPointer(curObjCollection->pageIndex, sizeof(UINT), TRUE));
#endif
		if (curObjCollection->pageIndex != NULL)
			*(curObjCollection->pageIndex) = m_ResourcePages.size();

		// save off for deleting upon destruction
		m_ResourcePages.push_back(rPage);
		// add in resource dialog page for resource dll
		m_PropSheet.AddPage(rPage);
		// bump to next page
		// bump count for image list creation
		CollectionCount++;
    }

	if (CollectionCount != 0)
	{
		m_Icons.Create(16, 16, ILC_COLORDDB | ILC_MASK, CollectionCount, 0);
	}
	else
	{
		MessageBox(NULL, _T("No Plugins found.\n"
				"Current incarnation of GDS assumes Plugins directory\n"
				"Directly beneath executable directory\n"
				"gds will not function correctly"), _T("Location Error!"),
				MB_OK);
	}
}

// Add resource item to page
bool CResourceDoc::AddResource(RESOURCEPAGEINFO *os)
{
	CResourcePage *destPage;

	ASSERT((os->pageIndex) < m_ResourcePages.size());

	destPage = m_ResourcePages[(os->pageIndex)];
	m_PropSheet.SetActivePage(destPage);
	destPage->AddResource(os);

	return true;
}

// Remove resource item from page 
bool CResourceDoc::RemoveResource(RESOURCEPAGEINFO *os)
{
	CResourcePage *destPage;

	ASSERT((os->pageIndex) < m_ResourcePages.size());

	destPage = m_ResourcePages[(os->pageIndex)];
	destPage->RemoveResource((os->rsrcIndex));

	return true;
}


// Clear all the resources from the page
bool CResourceDoc::ClearResources(RESOURCEPAGEINFO *os)
{
	CResourcePage *destPage;

	ASSERT((os->pageIndex) < m_ResourcePages.size());

	destPage = m_ResourcePages[(os->pageIndex)];
	destPage->ClearResources();

	return true;
}


// Register a page for Resource collection
bool CResourceDoc::RegisterResourceCollection(RESOURCECOLLECTION *objCollection)
{
	m_ResourceCollections.push_back(objCollection);
	return true;
}

// Register a page for Resource collection
bool CResourceDoc::GetSelectedResources( list< RESOURCEPAGEINFO > &InOutList )
{
	//find active page:
	CResourcePage * activePage = (CResourcePage*)m_PropSheet.GetActivePage();
	if( activePage )
	{
		activePage->GetSelectedResources( InOutList );
	}	
	return true;
}

void CResourceDoc::PauseThumbnailGenerationThreads( bool value )
{
	if( m_PropSheet.GetPageCount() > 0 )
	{
		//find active page:
		CResourcePage * activePage = (CResourcePage*)m_PropSheet.GetActivePage();
		if( activePage )
		{
			activePage->PauseGenerationThread( value );
		}
	}
}

bool CResourceDoc::SetSelectedResources( list<StdString> &InOutList )
{
	//find active page:
	CResourcePage * activePage = (CResourcePage*)m_PropSheet.GetActivePage();
	if( activePage )
	{
		activePage->SetSelectedResources( InOutList );
	}	
	return true;
}

bool CResourceDoc::GetResourceInfoByRelativePath( GETRESOURCEINFOBYRELATIVEPATH * searchparam )
{
	//find page
	int numpages = m_ResourcePages.size();
	bool bFound = false;
	for( int i = 0; i < numpages; i++ )
	{
		if( !_tcscmp( searchparam->pageName, m_ResourcePages[ i ]->GetName() ) )
		{
			//match
			bFound = m_ResourcePages[ i ]->GetResourceInfoByRelativePath( searchparam );
			break;
		}
	}
	return bFound;
}

void CResourceDoc::SetActiveDirectory(StdString &path)
{
	m_szActiveDirectory = path;

	if (m_PropSheet.GetSafeHwnd() != NULL)
	{
		CResourcePage *activePage = DYNAMIC_DOWNCAST( CResourcePage, m_PropSheet.GetActivePage() );
		activePage->SetActiveDirectory( path );
	}
}

StdString& CResourceDoc::GetActiveDirectory()
{
	return m_szActiveDirectory;
}

RESOURCECOLLECTIONLIST CResourceDoc::GetResourceCollection()
{
	return m_ResourceCollections;
}

CImageList* CResourceDoc::GetImageList()
{
	return &m_Icons;
}

CPropertySheet* CResourceDoc::GetPropertySheet()
{
	return &m_PropSheet;
}

/////////////////////////////////////////////////////////////////////////////
// CResourceDoc diagnostics

#ifdef _DEBUG
void CResourceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CResourceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CResourceDoc commands

BOOL CResourceDoc::RouteCmdToAllViews(CView *pView, UINT nID, int nCode, void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo)
{
    POSITION pos = GetFirstViewPosition ();

    while (pos != NULL) {
        CView* pNextView = GetNextView (pos);
        if (pNextView != pView) {
            if (pNextView->OnCmdMsg (nID, nCode, pExtra, pHandlerInfo))
                return TRUE;
        }
    }
    return FALSE;
}

void CResourceDoc::ResetResourceDirectory()
{
	// pause thumbnail generation
	PauseThumbnailGenerationThreads( TRUE );

	// get the first view of the window
	POSITION pos = GetFirstViewPosition();

	// if the position is NULL then the view is not initialized yet
	// that means, we are too early to update the views, perhaps because the 
	// Env component is created earlier, and that tries to set the directory. 
	// The directory is set, but no need to update the views
	if( pos != NULL )
	{
		CView* pView = GetNextView( pos );

		// try to cast it to the required resouceFolderView
		CResourceFolderView* pResoruceFolderView = DYNAMIC_DOWNCAST( CResourceFolderView, pView );
		if ( pResoruceFolderView != NULL )
		{
			// casting is successful, we can call the resetResouceFolder() function
			pResoruceFolderView->resetResourceFolder();
		} 
		else
		{
			// casting failed - log error
			EngineGetToolBox()->Log(LOGERROR, _T("%s(%i):Refreshing the resource-treeview failed.\n"), __FILE__, __LINE__);
		}
	}

	// get the new resourcepath
	StdString newResourcePath ( theApp.GetResourcePath() );

	// Refresh the resourcePages
	for( unsigned int i = 0; i < m_ResourcePages.size(); i++ )
	{
		m_ResourcePages[i]->SetResouceDirectory( newResourcePath );
	}

		// continue processing
	PauseThumbnailGenerationThreads( FALSE );
}