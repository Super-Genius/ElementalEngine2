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

#if !defined(AFX_RESOURCEDOC_H__AE0A7000_9B0F_11D2_8E53_006008A82731__INCLUDED_)
#define AFX_RESOURCEDOC_H__AE0A7000_9B0F_11D2_8E53_006008A82731__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace std;

typedef list<RESOURCECOLLECTION *> RESOURCECOLLECTIONLIST;

class CResourceDoc : public CDocument
{
public: // create from serialization only
	CResourceDoc();
	DECLARE_DYNCREATE(CResourceDoc)

// Attributes
private:
	vector<CResourcePage *> m_ResourcePages;
	// has to be created in OnCreate
	CResourceOptions *obOptions;
	// map Resource collections by priority
	RESOURCECOLLECTIONLIST m_ResourceCollections;
	CPropertySheet m_PropSheet;
	// number of resource dll's loaded
	CImageList m_Icons;
	// the current active directory
	StdString m_szActiveDirectory;

// Operations
public:

	bool AddResource(RESOURCEPAGEINFO *os);
	bool RemoveResource(RESOURCEPAGEINFO *os);
	bool ClearResources(RESOURCEPAGEINFO *os);
	bool RegisterResourceCollection(RESOURCECOLLECTION *objCollection);
	bool GetSelectedResources( list< RESOURCEPAGEINFO > &InOutList );
	bool SetSelectedResources( list<StdString> &InOutList );
	bool GetResourceInfoByRelativePath( GETRESOURCEINFOBYRELATIVEPATH * searchparam );
	
	/// \brief Pauses the thumbnail generation for the current visible tab.
	/// \param value - TRUE to stop, FALSE to continue thread
	/// \return void
	void PauseThumbnailGenerationThreads( bool value );

	/// \brief resets the resource directory
	/// \return void
	void ResetResourceDirectory();

	void InitResourcePages();
	RESOURCECOLLECTIONLIST GetResourceCollection();
	CImageList*	GetImageList();
	CPropertySheet* GetPropertySheet();

	/// \brief	Changes the active directory
	void SetActiveDirectory(StdString &path);
	///	\brief	Returns the active directory
	StdString& GetActiveDirectory();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResourceDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL RouteCmdToAllViews (CView* pView, UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual ~CResourceDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CResourceDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESOURCEDOC_H__AE0A7000_9B0F_11D2_8E53_006008A82731__INCLUDED_)
