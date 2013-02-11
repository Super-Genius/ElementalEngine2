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
#include ".\guieditorview.h"
#include "EditorHelpers.h"
using namespace GDS_SDK_Helpers;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUIEditorView

IMPLEMENT_DYNCREATE(CGUIEditorView, CView)

CGUIEditorView::CGUIEditorView()
{
	m_szObjName = _T("");
	m_szObjType = _T("");
	m_pParent = NULL;
	m_bInitialized = false;
	m_pRenderTarget = NULL;
	m_pRC = NULL;
	m_iRenderContextID = -1;
	m_ToolBox = EngineGetToolBox();
	Init();
	m_iXRes = 0;
	m_iYRes = 0;
	m_iLastMousePosX = 0;
	m_iLastMousePosY = 0;
	m_bLeftButtonPress = false;
	m_bSpacePressed = false;
	m_bShiftPressed = false;
	m_bCtrlPressed = false;
	m_iPageOffsetX = 0;
	m_iPageOffsetY = 0;
	m_iState = 0;
	m_szCurrentPage = _T("");
	m_iHandleIndex = -1;	
	m_fElementPosX = 0.f;
	m_fElementPosY = 0.f;
	m_iLastActualMousePosX = 0;
	m_iLastActualMousePosY = 0;
	m_bDeletePressed = false;
	m_szCurrentGroup = _T("");
	m_bFullscreenMode = false;
	m_fZoomFactor = 1.f;	
	m_iSystemIndex = 0;
	m_bLastWasVisible = false;
}

CGUIEditorView::~CGUIEditorView()
{
	if (m_iRenderContextID != -1)
	{
		DWORD msgDeleteRenderContext = CHashString(_T("DeleteRenderContext")).GetUniqueID();
		m_ToolBox->SendMessage(msgDeleteRenderContext, sizeof(UINT), &m_iRenderContextID);		
		m_iRenderContextID = -1;
	}
}


BEGIN_MESSAGE_MAP(CGUIEditorView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYUP()
	ON_COMMAND(ID_COPY_ITEM, OnCopyItem)
	ON_COMMAND(ID_PASTE_NEW, OnPasteNew)
	ON_COMMAND(ID_PASTE_INST, OnPasteInst)	
	ON_COMMAND(ID_SHOW_HIDE, OnShowHide)
	ON_COMMAND(ID_ANIM_TEST, OnAnimTest)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUIEditorView drawing

void CGUIEditorView::OnDraw(CDC* pDC)
{
	if (!m_bFullscreenMode)
	{
		RECT rect;
		GetClientRect(&rect);
		VIEWPORTPARAMS gwrp;
		gwrp.offsetx = 0;
		gwrp.offsety = 0;
		gwrp.width = rect.right;
		gwrp.height = rect.bottom;
		static DWORD msgHash_SetWindowSize = CHashString(_T("SetWindowSize")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetWindowSize, sizeof(VIEWPORTPARAMS), &gwrp, NULL, NULL);	
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGUIEditorView diagnostics

#ifdef _DEBUG
void CGUIEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CGUIEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

void CGUIEditorView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	AfxSetResourceHandle(GUIEditorDLL.hModule);	
	// don't use is window visible.

	m_pParent = DYNAMIC_DOWNCAST(CGUIEditorFrm, this->GetParentFrame());

	if (m_bLastWasVisible != (bActivate != FALSE))
	{
		m_bLastWasVisible = (bActivate != FALSE);

		CGuiMDIFrame *mainWnd = DYNAMIC_DOWNCAST(CGuiMDIFrame, AfxGetMainWnd());

		if (bActivate)
		{
			mainWnd->LoadDocumentBars(_T("GUIEditor"));
		}
		else
		{
			mainWnd->SaveDocumentBars(_T("GUIEditor"));
		}
	}
	AfxSetResourceHandle(m_pParent->m_AppInst);
	RECT rect;
	GetClientRect(&rect);
	m_pRC->CreateContext(*this, rect.right, rect.bottom, 24, 32);

	if (bActivate)
	{
		SetActiveScene();
	}
}

void CGUIEditorView::Init()
{
	// set up render context	
    CREATERENDERCONTEXTPARAMS crcp;
	crcp.colorbits = 32;
	crcp.depthbits = 24;
	crcp.height = 0; // resize later
	crcp.width = 0; // resize later
	crcp.hwindow = *this;
	static DWORD msgHash_CreateRenderContext = CHashString(_T("CreateRenderContext")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateRenderContext, sizeof(CREATERENDERCONTEXTPARAMS), &crcp);		
	m_pRC = crcp.context;
	m_pRC->SetBGColor(0, 0, 0);
	m_iRenderContextID = crcp.uiRenderContextID;

	static DWORD msgHash_RenderPageToContext = CHashString(_T("RenderPageToContext")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RenderPageToContext, sizeof(IRenderContext), m_pRC, NULL, NULL);

	ADDRENDERLISTPARAMS arlp;
	OBJECTLIST ol; // empty object list
	arlp.dwListPriorityKey = RENDERPRIORITY_FRONTBUFFER;
	arlp.dwSortMethod = SORT_FIFO;
	arlp.objList = &ol;
	arlp.renderContext = m_pRC;
	static DWORD msgHash_AddRenderList = CHashString(_T("AddRenderList")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddRenderList, sizeof(ADDRENDERLISTPARAMS), &arlp);
}

void CGUIEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CView::OnLButtonDown(nFlags, point);
	m_bLeftButtonPress = true;
	MOUSECOORDS mouse;
	mouse.m_LButtonDown = true;
	mouse.m_lXPos = point.x;
	mouse.m_lYPos = point.y;
	mouse.m_lXDelta = 0;
	mouse.m_lYDelta = 0;
	static DWORD msgHash_LeftMousePress = CHashString(_T("LeftMousePress")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_LeftMousePress, sizeof(MOUSECOORDS), &mouse, NULL, NULL);

	if ((m_bCtrlPressed) && (m_bSpacePressed))
	{
		MOUSECOORDS mouse;
		static DWORD msgHash_GetGUIMousePosition = CHashString(_T("GetGUIMousePosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGUIMousePosition, sizeof(MOUSECOORDS), &mouse);
		static DWORD msgHash_GUIManagerIncrementZoom = CHashString(_T("GUIManagerIncrementZoom")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GUIManagerIncrementZoom, 0, NULL);	
		float zoom;
		static DWORD msgHash_GetZoomFactor = CHashString(_T("GetZoomFactor")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetZoomFactor, sizeof(float), &zoom);
		m_fZoomFactor = zoom;
		RECT rect;
		GetClientRect(&rect);
		m_iPageOffsetX = (rect.right / 2) - (int)((float)mouse.m_lXPos * zoom);
		m_iPageOffsetY = (rect.bottom / 2) - (int)((float)mouse.m_lYPos * zoom);
		POSITIONDATA pd;
		pd.m_fXPos = (float)m_iPageOffsetX;
		pd.m_fYPos = (float)m_iPageOffsetY;
		static DWORD msgHash_GUIManagerSetPageOffset = CHashString(_T("GUIManagerSetPageOffset")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GUIManagerSetPageOffset, sizeof(POSITIONDATA), &pd);

		CGUIEditorDoc *doc = DYNAMIC_DOWNCAST(CGUIEditorDoc, GetDocument());
		assert(doc);	

		doc->ZoomChange(zoom);
	}
	else if ((m_bShiftPressed) && (m_bSpacePressed))
	{
		MOUSECOORDS mouse;
		static DWORD msgHash_GetGUIMousePosition = CHashString(_T("GetGUIMousePosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGUIMousePosition, sizeof(MOUSECOORDS), &mouse);
		static DWORD msgHash_GUIManagerDecrementZoom = CHashString(_T("GUIManagerDecrementZoom")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GUIManagerDecrementZoom, 0, NULL);	
		float zoom;
		static DWORD msgHash_GetZoomFactor = CHashString(_T("GetZoomFactor")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetZoomFactor, sizeof(float), &zoom);
		m_fZoomFactor = zoom;
		RECT rect;
		GetClientRect(&rect);
		m_iPageOffsetX = (rect.right / 2) - (int)((float)mouse.m_lXPos * zoom);
		m_iPageOffsetY = (rect.bottom / 2) - (int)((float)mouse.m_lYPos * zoom);
		POSITIONDATA pd;
		pd.m_fXPos = (float)m_iPageOffsetX;
		pd.m_fYPos = (float)m_iPageOffsetY;
		static DWORD msgHash_GUIManagerSetPageOffset = CHashString(_T("GUIManagerSetPageOffset")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GUIManagerSetPageOffset, sizeof(POSITIONDATA), &pd);

		CGUIEditorDoc *doc = DYNAMIC_DOWNCAST(CGUIEditorDoc, GetDocument());
		assert(doc);

		doc->ZoomChange(zoom);
	}
	else if (m_iState == 1) // set pos state
	{
		POSITIONDATA pd;
		MOUSECOORDS mouse;
		static DWORD msgHash_GetGUIMousePosition = CHashString(_T("GetGUIMousePosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGUIMousePosition, sizeof(MOUSECOORDS), &mouse);
		pd.m_fXPos = (float)mouse.m_lXPos;
		pd.m_fYPos = (float)mouse.m_lYPos;
		static DWORD msgHash_SetPosition = CHashString(_T("SetPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		m_iState = 2;
		m_fElementPosX = (float)mouse.m_lXPos;
		m_fElementPosY = (float)mouse.m_lYPos;
	}	
	else if (!m_bSpacePressed)
	{
		// see if we have an object selected already first...
		DWORD retval;
		CHashString rectName = _T("SelectRect");
		CHashString rectType = _T("CGUIRect");
		static DWORD msgHash_GetHandleIndex = CHashString(_T("GetHandleIndex")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetHandleIndex, sizeof(int), &m_iHandleIndex, &rectName, &rectType);
		if (m_iHandleIndex > -1)
		{
			m_iState = 4 + m_iHandleIndex;
			return;
		}

		// if not, see if we can select one
		MOUSECOORDS mouse;
		static DWORD msgHash_GetGUIMousePosition = CHashString(_T("GetGUIMousePosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGUIMousePosition, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		OBJUNDERCURSOR ouc;
		CHashString pageName = m_szCurrentPage;
		CHashString pageType = _T("CGUIPage");
		ouc.m_fPosX = (float)mouse.m_lXPos;
		ouc.m_fPosY = (float)mouse.m_lYPos;
		ouc.m_Name = NULL;
		ouc.m_CompType = NULL;
		static DWORD msgHash_GetObjUnderCursor = CHashString(_T("GetObjUnderCursor")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetObjUnderCursor, sizeof(OBJUNDERCURSOR), &ouc, &pageName, &pageType);		
		
		CGUIEditorDoc *doc = DYNAMIC_DOWNCAST(CGUIEditorDoc, GetDocument());
		assert(doc);

		doc->SelectionChange(); // hide the cursor

		if (ouc.m_Name)
		{
			// legitimate object, either group or instance
			if ((ouc.m_Name->GetUniqueID() != CHashString(_T("WorkingCanvas")).GetUniqueID()) &&
				(ouc.m_Name->GetUniqueID() != CHashString(_T("SelectRect")).GetUniqueID()))
			{
				FINDOBJNAME fon;
				fon.searchName = ouc.m_Name;
				fon.compType = ouc.m_CompType;
				static DWORD msgHash_FollowObject = CHashString(_T("FollowObject")).GetUniqueID();
				retval = m_ToolBox->SendMessage(msgHash_FollowObject, sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));


				// if selection has changed...
				if (m_szObjName.GetUniqueID() != ouc.m_Name->GetUniqueID())
				{
					if (_tcscmp(ouc.m_CompType->GetString(), _T("CGUIInstance")) == 0)
					{
						CHashString oName;
						static DWORD msgHash_GetItemReference = CHashString(_T("GetItemReference")).GetUniqueID();
						m_ToolBox->SendMessage(msgHash_GetItemReference, sizeof(IHashString), &oName, ouc.m_Name, &CHashString(_T("CGUIInstance")));
						FINDGUIOBJECT fgo;
						fgo.name = &oName;
						static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
						m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
						if (fgo.bFound)
						{
							vector <CHashString> names, types;
							names.push_back(CHashString(ouc.m_Name->GetString()));
							names.push_back(CHashString(fgo.name->GetString()));
							types.push_back(CHashString(_T("CGUIInstance")));
							types.push_back(CHashString(fgo.type->GetString()));
							m_pParent->m_OptionTree->UpdateOptionTree(names, types);
						}
						else
						{	
							m_pParent->m_OptionTree->UpdateOptionTree(ouc.m_Name, ouc.m_CompType);
						}
						m_szObjName = *dynamic_cast<CHashString*>(ouc.m_Name);
						m_szObjType = *dynamic_cast<CHashString*>(ouc.m_CompType);
						m_iState = 3;
						doc->SetCurrentObject(m_szObjName);
					}
					else
					{
						m_pParent->m_OptionTree->UpdateOptionTree(ouc.m_Name, ouc.m_CompType);
						m_szObjName = *dynamic_cast<CHashString*>(ouc.m_Name);
						m_szObjType = *dynamic_cast<CHashString*>(ouc.m_CompType);
						m_iState = 3;						
					}
				}				
			}
			else // didn't click an object other than the background
			{
				CHashString rectName = _T("SelectRect");
				CHashString rectType = _T("CGUIRect");
				SELECTRECT sr;
				sr.x1 = 0.f;
				sr.x2 = 0.f;
				sr.y1 = 0.f;
				sr.y2 = 0.f;
				static DWORD msgHash_SetRect = CHashString(_T("SetRect")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_SetRect, sizeof(SELECTRECT), &sr, &rectName, &rectType);
				m_pParent->m_OptionTree->ClearOptionTree();
				m_szObjName = _T("");
				m_iState = 0;
			}
		}
		else // didn't click anything 
		{
			CHashString rectName = _T("SelectRect");
			CHashString rectType = _T("CGUIRect");
			SELECTRECT sr;
			sr.x1 = 0.f;
			sr.x2 = 0.f;
			sr.y1 = 0.f;
			sr.y2 = 0.f;
			static DWORD msgHash_SetRect = CHashString(_T("SetRect")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SetRect, sizeof(SELECTRECT), &sr, &rectName, &rectType);
			m_pParent->m_OptionTree->ClearOptionTree();
			m_szObjName = _T("");
			m_iState = 0;
			doc->ClearObjectSelection();
		}
	}
}

void CGUIEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLeftButtonPress = false;
	CView::OnLButtonUp(nFlags, point);
	if (m_iState == 2)
	{
		m_iState = 0;
		if (_tcscmp(m_szObjType.GetString(), _T("CGUIInstance")) == 0)
		{
			CHashString refName, refType;
			
			static DWORD msgHash_GetItemReference = CHashString(_T("GetItemReference")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetItemReference, sizeof(IHashString), &refName, &m_szObjName, &m_szObjType);
			FINDGUIOBJECT fgo;
			fgo.name = &refName;
			static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
			refType = fgo.type->GetString();

			vector <CHashString> names, types;
			names.push_back(m_szObjName);
			names.push_back(refName);
			types.push_back(m_szObjType);
			types.push_back(refType);
			
			m_pParent->m_OptionTree->UpdateOptionTree(names, types);			
		}
		else
		{
			m_pParent->m_OptionTree->UpdateOptionTree(&m_szObjName, &m_szObjType);			
		}
	}
	else if (m_iState >= 4)
	{
		if (_tcscmp(m_szObjType.GetString(), _T("CGUIInstance")) == 0)
		{
			CHashString refName(_T("")), refType(_T(""));
			
			if (_tcscmp(m_szObjName.GetString(), _T("")) != 0)
			{
				static DWORD msgHash_GetItemReference = CHashString(_T("GetItemReference")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_GetItemReference, sizeof(IHashString), &refName, &m_szObjName, &m_szObjType);

				if (_tcscmp(refName.GetString(), _T("")) != 0)
				{
					FINDGUIOBJECT fgo;
					fgo.name = &refName;
					fgo.type = NULL;
					static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
				
					if (fgo.type != NULL)
					{
						refType = fgo.type->GetString();

						vector <CHashString> names, types;
						names.push_back(m_szObjName);
						names.push_back(refName);
						types.push_back(m_szObjType);
						types.push_back(refType);
						
						m_pParent->m_OptionTree->UpdateOptionTree(names, types);
						m_iState = 0;
					}
				}
			}
		}
		else
		{
			m_pParent->m_OptionTree->UpdateOptionTree(&m_szObjName, &m_szObjType);
			m_iState = 0;
		}
	}	
}

void CGUIEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	MOUSECOORDS realMouse;
	realMouse.m_lXPos = point.x;
	realMouse.m_lYPos = point.y;
	realMouse.m_lXDelta = point.x - m_iLastActualMousePosX;
	realMouse.m_lYDelta = point.y - m_iLastActualMousePosY;
	realMouse.m_LButtonDown = m_bLeftButtonPress;
	static DWORD msgHash_MouseMove = CHashString(_T("MouseMove")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_MouseMove, sizeof(MOUSECOORDS), &realMouse);
	if (m_iState == 2) // set size stage
	{
        DWORD retval;
		MOUSECOORDS mouse;
		static DWORD msgHash_GetGUIMousePosition = CHashString(_T("GetGUIMousePosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGUIMousePosition, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
        SIZEDATA sd;
		sd.m_fXSize = (float)(mouse.m_lXPos) - m_fElementPosX;
		sd.m_fYSize = (float)(mouse.m_lYPos) - m_fElementPosY;
		static DWORD msgHash_SetSize = CHashString(_T("SetSize")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetSize, sizeof(SIZEDATA), &sd, &m_szObjName, &m_szObjType);
		
		CHashString rectName = _T("SelectRect");
		CHashString rectType = _T("CGUIRect");
		SELECTRECT rect;
		rect.x1 = m_fElementPosX;
		rect.x2 = (float)mouse.m_lXPos;
		rect.y1 = m_fElementPosY;
		rect.y2 = (float)mouse.m_lYPos;
		static DWORD msgHash_SetRect = CHashString(_T("SetRect")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetRect, sizeof(SELECTRECT), &rect, &rectName, &rectType);		
	}
	else if (m_iState == 4) // center of object selected, changing position
	{
		DWORD retval;
		MOUSECOORDS mouse;
		static DWORD msgHash_GetGUIMousePosition = CHashString(_T("GetGUIMousePosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGUIMousePosition, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		int mouseDispX = mouse.m_lXPos - m_iLastMousePosX;
		int mouseDispY = mouse.m_lYPos - m_iLastMousePosY;
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
        POSITIONDATA pd;
		static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		pd.m_fXPos += (float)mouseDispX;
		pd.m_fYPos += (float)mouseDispY;
		static DWORD msgHash_SetPosition = CHashString(_T("SetPosition")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		FINDOBJNAME fon;
		fon.searchName = &m_szObjName;
		fon.compType = &m_szObjType;
		static DWORD msgHash_FollowObject = CHashString(_T("FollowObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_FollowObject, sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	}
	else if (m_iState == 5) // top left handle
	{
		DWORD retval;
		MOUSECOORDS mouse;
		static DWORD msgHash_GetGUIMousePosition = CHashString(_T("GetGUIMousePosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGUIMousePosition, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		int mouseDispX = mouse.m_lXPos - m_iLastMousePosX;
		int mouseDispY = mouse.m_lYPos - m_iLastMousePosY;
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
        POSITIONDATA pd;
		static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		pd.m_fXPos += (float)mouseDispX;
		pd.m_fYPos += (float)mouseDispY;
		static DWORD msgHash_SetPosition = CHashString(_T("SetPosition")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		SIZEDATA sd;
		static DWORD msgHash_GetSize = CHashString(_T("GetSize")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetSize, sizeof(SIZEDATA), &sd, &m_szObjName, &m_szObjType);		
		sd.m_fXSize -= (float)mouseDispX;
		sd.m_fYSize -= (float)mouseDispY;
		static DWORD msgHash_SetSize = CHashString(_T("SetSize")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetSize, sizeof(SIZEDATA), &sd, &m_szObjName, &m_szObjType);		
		FINDOBJNAME fon;
		fon.searchName = &m_szObjName;
		fon.compType = &m_szObjType;
		static DWORD msgHash_FollowObject = CHashString(_T("FollowObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_FollowObject, sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	}
	else if (m_iState == 6) // top middle handle
	{
		DWORD retval;
		MOUSECOORDS mouse;
		static DWORD msgHash_GetGUIMousePosition = CHashString(_T("GetGUIMousePosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGUIMousePosition, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		int mouseDispX = mouse.m_lXPos - m_iLastMousePosX;
		int mouseDispY = mouse.m_lYPos - m_iLastMousePosY;
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
        POSITIONDATA pd;
		static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		pd.m_fYPos += (float)mouseDispY;
		static DWORD msgHash_SetPosition = CHashString(_T("SetPosition")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		SIZEDATA sd;
		static DWORD msgHash_GetSize = CHashString(_T("GetSize")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetSize, sizeof(SIZEDATA), &sd, &m_szObjName, &m_szObjType);		
		sd.m_fYSize -= (float)mouseDispY;
		static DWORD msgHash_SetSize = CHashString(_T("SetSize")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetSize, sizeof(SIZEDATA), &sd, &m_szObjName, &m_szObjType);		
		FINDOBJNAME fon;
		fon.searchName = &m_szObjName;
		fon.compType = &m_szObjType;
				static DWORD msgHash_FollowObject = CHashString(_T("FollowObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_FollowObject, sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	}
	else if (m_iState == 7) // top right handle
	{
		DWORD retval;
		MOUSECOORDS mouse;
		static DWORD msgHash_GetGUIMousePosition = CHashString(_T("GetGUIMousePosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGUIMousePosition, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		int mouseDispX = mouse.m_lXPos - m_iLastMousePosX;
		int mouseDispY = mouse.m_lYPos - m_iLastMousePosY;
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
        POSITIONDATA pd;
		static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		pd.m_fYPos += (float)mouseDispY;
		static DWORD msgHash_SetPosition = CHashString(_T("SetPosition")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		SIZEDATA sd;
		static DWORD msgHash_GetSize = CHashString(_T("GetSize")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetSize, sizeof(SIZEDATA), &sd, &m_szObjName, &m_szObjType);		
		sd.m_fXSize += (float)mouseDispX;
		sd.m_fYSize -= (float)mouseDispY;
		static DWORD msgHash_SetSize = CHashString(_T("SetSize")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetSize, sizeof(SIZEDATA), &sd, &m_szObjName, &m_szObjType);		
		FINDOBJNAME fon;
		fon.searchName = &m_szObjName;
		fon.compType = &m_szObjType;
		static DWORD msgHash_FollowObject = CHashString(_T("FollowObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_FollowObject, sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	}
	else if (m_iState == 8) // middle right handle
	{
		DWORD retval;
		MOUSECOORDS mouse;
		static DWORD msgHash_GetGUIMousePosition = CHashString(_T("GetGUIMousePosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGUIMousePosition, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		int mouseDispX = mouse.m_lXPos - m_iLastMousePosX;
		int mouseDispY = mouse.m_lYPos - m_iLastMousePosY;
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
        SIZEDATA sd;
		static DWORD msgHash_GetSize = CHashString(_T("GetSize")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetSize, sizeof(SIZEDATA), &sd, &m_szObjName, &m_szObjType);		
		sd.m_fXSize += (float)mouseDispX;		
		static DWORD msgHash_SetSize = CHashString(_T("SetSize")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetSize, sizeof(SIZEDATA), &sd, &m_szObjName, &m_szObjType);		
		FINDOBJNAME fon;
		fon.searchName = &m_szObjName;
		fon.compType = &m_szObjType;
		static DWORD msgHash_FollowObject = CHashString(_T("FollowObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_FollowObject, sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	}
	else if (m_iState == 9) // bottom right handle
	{
		DWORD retval;
		MOUSECOORDS mouse;
		static DWORD msgHash_GetGUIMousePosition = CHashString(_T("GetGUIMousePosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGUIMousePosition, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		int mouseDispX = mouse.m_lXPos - m_iLastMousePosX;
		int mouseDispY = mouse.m_lYPos - m_iLastMousePosY;
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
        SIZEDATA sd;
		static DWORD msgHash_GetSize = CHashString(_T("GetSize")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetSize, sizeof(SIZEDATA), &sd, &m_szObjName, &m_szObjType);		
		sd.m_fXSize += (float)mouseDispX;		
		sd.m_fYSize += (float)mouseDispY;
		static DWORD msgHash_SetSize = CHashString(_T("SetSize")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetSize, sizeof(SIZEDATA), &sd, &m_szObjName, &m_szObjType);		
		FINDOBJNAME fon;
		fon.searchName = &m_szObjName;
		fon.compType = &m_szObjType;
		static DWORD msgHash_FollowObject = CHashString(_T("FollowObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_FollowObject, sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	}
	else if (m_iState == 10) // bottom middle handle
	{
		DWORD retval;
		MOUSECOORDS mouse;
		static DWORD msgHash_GetGUIMousePosition = CHashString(_T("GetGUIMousePosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGUIMousePosition, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		int mouseDispX = mouse.m_lXPos - m_iLastMousePosX;
		int mouseDispY = mouse.m_lYPos - m_iLastMousePosY;
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
        SIZEDATA sd;
		static DWORD msgHash_GetSize = CHashString(_T("GetSize")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetSize, sizeof(SIZEDATA), &sd, &m_szObjName, &m_szObjType);		
		sd.m_fYSize += (float)mouseDispY;
		static DWORD msgHash_SetSize = CHashString(_T("SetSize")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetSize, sizeof(SIZEDATA), &sd, &m_szObjName, &m_szObjType);		
		FINDOBJNAME fon;
		fon.searchName = &m_szObjName;
		fon.compType = &m_szObjType;
		static DWORD msgHash_FollowObject = CHashString(_T("FollowObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_FollowObject, sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	}
	else if (m_iState == 11) // bottom left handle
	{
		DWORD retval;
		MOUSECOORDS mouse;
		static DWORD msgHash_GetGUIMousePosition = CHashString(_T("GetGUIMousePosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGUIMousePosition, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		int mouseDispX = mouse.m_lXPos - m_iLastMousePosX;
		int mouseDispY = mouse.m_lYPos - m_iLastMousePosY;
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
        POSITIONDATA pd;
		static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		pd.m_fXPos += (float)mouseDispX;
		static DWORD msgHash_SetPosition = CHashString(_T("SetPosition")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		SIZEDATA sd;
		static DWORD msgHash_GetSize = CHashString(_T("GetSize")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetSize, sizeof(SIZEDATA), &sd, &m_szObjName, &m_szObjType);		
		sd.m_fXSize -= (float)mouseDispX;
		sd.m_fYSize += (float)mouseDispY;
		static DWORD msgHash_SetSize = CHashString(_T("SetSize")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetSize, sizeof(SIZEDATA), &sd, &m_szObjName, &m_szObjType);		
		FINDOBJNAME fon;
		fon.searchName = &m_szObjName;
		fon.compType = &m_szObjType;
		static DWORD msgHash_FollowObject = CHashString(_T("FollowObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_FollowObject, sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	}
	else if (m_iState == 12) // middle left handle
	{
		DWORD retval;
		MOUSECOORDS mouse;
		static DWORD msgHash_GetGUIMousePosition = CHashString(_T("GetGUIMousePosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGUIMousePosition, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
		int mouseDispX = mouse.m_lXPos - m_iLastMousePosX;
		int mouseDispY = mouse.m_lYPos - m_iLastMousePosY;
		m_iLastMousePosX = mouse.m_lXPos;
		m_iLastMousePosY = mouse.m_lYPos;
        POSITIONDATA pd;
		static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		pd.m_fXPos += (float)mouseDispX;
		static DWORD msgHash_SetPosition = CHashString(_T("SetPosition")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		SIZEDATA sd;
		static DWORD msgHash_GetSize = CHashString(_T("GetSize")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetSize, sizeof(SIZEDATA), &sd, &m_szObjName, &m_szObjType);		
		sd.m_fXSize -= (float)mouseDispX;
		static DWORD msgHash_SetSize = CHashString(_T("SetSize")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetSize, sizeof(SIZEDATA), &sd, &m_szObjName, &m_szObjType);		
		FINDOBJNAME fon;
		fon.searchName = &m_szObjName;
		fon.compType = &m_szObjType;
		static DWORD msgHash_FollowObject = CHashString(_T("FollowObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_FollowObject, sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
	}
	MOUSECOORDS mouse;
	static DWORD msgHash_GetGUIMousePosition = CHashString(_T("GetGUIMousePosition")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGUIMousePosition, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
	if ((m_bSpacePressed) && (m_bLeftButtonPress))
	{
		m_iPageOffsetX += point.x - m_iLastActualMousePosX;
		m_iPageOffsetY += point.y - m_iLastActualMousePosY;
		POSITIONDATA pd;
		pd.m_fXPos = (float)m_iPageOffsetX;
		pd.m_fYPos = (float)m_iPageOffsetY;
		static DWORD msgHash_GUIManagerSetPageOffset = CHashString(_T("GUIManagerSetPageOffset")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GUIManagerSetPageOffset, sizeof(POSITIONDATA), &pd);
	}
	m_iLastMousePosX = mouse.m_lXPos;
	m_iLastMousePosY = mouse.m_lYPos;
	m_iLastActualMousePosX = point.x;
	m_iLastActualMousePosY = point.y;

	CGUIEditorDoc *doc = DYNAMIC_DOWNCAST(CGUIEditorDoc, GetDocument());
	assert(doc);
	doc->MouseMove(mouse.m_lXPos, mouse.m_lYPos);
}

void CGUIEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	static DWORD msgHash_KeyPress = CHashString(_T("KeyPress")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_KeyPress, sizeof(char), &nChar, NULL, NULL);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	if (nChar == VK_SHIFT)
	{
		m_bShiftPressed = true;
	}
	if(nChar == VK_CONTROL)
	{
		m_bCtrlPressed = true;		
	}
	if (nChar == ' ')
	{
		m_bSpacePressed = true;	
	}	
	if (nChar == 'S')
	{
		CGUIEditorDoc *doc = DYNAMIC_DOWNCAST(CGUIEditorDoc, GetDocument());
		assert(doc);
		doc->SetKey(m_szObjName.GetString(), m_szObjType.GetString());
	}
	if (nChar == VK_DELETE)
	{
		if (!m_bDeletePressed)
		{
			m_bDeletePressed = true;
			if (_tcscmp(m_szObjName.GetString(), _T("")) != 0)
			{
				CGUIEditorDoc *doc = DYNAMIC_DOWNCAST(CGUIEditorDoc, this->GetDocument());
				doc->DeleteItem(m_szObjName, m_szObjType);
			}
		}
	}
	else if (nChar == VK_UP)
	{
		POSITIONDATA pd;
		static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		pd.m_fYPos -= 1.f;
		static DWORD msgHash_SetPosition = CHashString(_T("SetPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		FINDOBJNAME fon;
		fon.searchName = &m_szObjName;
		fon.compType = &m_szObjType;
		static DWORD msgHash_FollowObject = CHashString(_T("FollowObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_FollowObject, sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));

		if (_tcscmp(m_szObjType.GetString(), _T("CGUIInstance")) == 0)
		{
			CHashString oName;
			static DWORD msgHash_GetItemReference = CHashString(_T("GetItemReference")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetItemReference, sizeof(IHashString), &oName, &m_szObjName, &m_szObjType);
			FINDGUIOBJECT fgo;
			fgo.name = &oName;
			static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
			if (fgo.bFound)
			{
				vector <CHashString> names, types;
				names.push_back(&m_szObjName);
				names.push_back(CHashString(fgo.name->GetString()));
				types.push_back(&m_szObjType);
				types.push_back(CHashString(fgo.type->GetString()));
				m_pParent->m_OptionTree->UpdateOptionTree(names, types);
			}
		}
		else
		{
			m_pParent->m_OptionTree->UpdateOptionTree(&m_szObjName, &m_szObjType);
		}
	}
	else if (nChar == VK_DOWN)
	{
		POSITIONDATA pd;
		static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		pd.m_fYPos += 1.f;
		static DWORD msgHash_SetPosition = CHashString(_T("SetPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		FINDOBJNAME fon;
		fon.searchName = &m_szObjName;
		fon.compType = &m_szObjType;
		static DWORD msgHash_FollowObject = CHashString(_T("FollowObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_FollowObject, sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
		
		if (_tcscmp(m_szObjType.GetString(), _T("CGUIInstance")) == 0)
		{
			CHashString oName;
			static DWORD msgHash_GetItemReference = CHashString(_T("GetItemReference")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetItemReference, sizeof(IHashString), &oName, &m_szObjName, &m_szObjType);
			FINDGUIOBJECT fgo;
			fgo.name = &oName;
			static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
			if (fgo.bFound)
			{
				vector <CHashString> names, types;
				names.push_back(&m_szObjName);
				names.push_back(CHashString(fgo.name->GetString()));
				types.push_back(&m_szObjType);
				types.push_back(CHashString(fgo.type->GetString()));
				m_pParent->m_OptionTree->UpdateOptionTree(names, types);
			}
		}
		else
		{
			m_pParent->m_OptionTree->UpdateOptionTree(&m_szObjName, &m_szObjType);
		}
	}
	else if (nChar == VK_LEFT)
	{
		POSITIONDATA pd;
		static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		pd.m_fXPos -= 1.f;
		static DWORD msgHash_SetPosition = CHashString(_T("SetPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		FINDOBJNAME fon;
		fon.searchName = &m_szObjName;
		fon.compType = &m_szObjType;
		static DWORD msgHash_FollowObject = CHashString(_T("FollowObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_FollowObject, sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
		
		if (_tcscmp(m_szObjType.GetString(), _T("CGUIInstance")) == 0)
		{
			CHashString oName;
			static DWORD msgHash_GetItemReference = CHashString(_T("GetItemReference")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetItemReference, sizeof(IHashString), &oName, &m_szObjName, &m_szObjType);
			FINDGUIOBJECT fgo;
			fgo.name = &oName;
			static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
			if (fgo.bFound)
			{
				vector <CHashString> names, types;
				names.push_back(&m_szObjName);
				names.push_back(CHashString(fgo.name->GetString()));
				types.push_back(&m_szObjType);
				types.push_back(CHashString(fgo.type->GetString()));
				m_pParent->m_OptionTree->UpdateOptionTree(names, types);
			}
		}
		else
		{
			m_pParent->m_OptionTree->UpdateOptionTree(&m_szObjName, &m_szObjType);
		}
	}
	else if (nChar == VK_RIGHT)
	{
		POSITIONDATA pd;
		static DWORD msgHash_GetPosition = CHashString(_T("GetPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		pd.m_fXPos += 1.f;
		static DWORD msgHash_SetPosition = CHashString(_T("SetPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetPosition, sizeof(POSITIONDATA), &pd, &m_szObjName, &m_szObjType);
		FINDOBJNAME fon;
		fon.searchName = &m_szObjName;
		fon.compType = &m_szObjType;
		static DWORD msgHash_FollowObject = CHashString(_T("FollowObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_FollowObject, sizeof(FINDOBJNAME), &fon, &CHashString(_T("SelectRect")), &CHashString(_T("CGUIRect")));
		
		if (_tcscmp(m_szObjType.GetString(), _T("CGUIInstance")) == 0)
		{
			CHashString oName;
			static DWORD msgHash_GetItemReference = CHashString(_T("GetItemReference")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetItemReference, sizeof(IHashString), &oName, &m_szObjName, &m_szObjType);
			FINDGUIOBJECT fgo;
			fgo.name = &oName;
			static DWORD msgHash_FindGUIObject = CHashString(_T("FindGUIObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_FindGUIObject, sizeof(FINDGUIOBJECT), &fgo);
			if (fgo.bFound)
			{
				vector <CHashString> names, types;
				names.push_back(&m_szObjName);
				names.push_back(CHashString(fgo.name->GetString()));
				types.push_back(&m_szObjType);
				types.push_back(CHashString(fgo.type->GetString()));
				m_pParent->m_OptionTree->UpdateOptionTree(names, types);
			}
		}
		else
		{
			m_pParent->m_OptionTree->UpdateOptionTree(&m_szObjName, &m_szObjType);
		}
	}	
}

void CGUIEditorView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	static DWORD msgHash_KeyRelease = CHashString(_T("KeyRelease")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_KeyRelease, sizeof(char), &nChar, NULL, NULL);
	CView::OnKeyUp(nChar, nRepCnt, nFlags);

	if (nChar == VK_SHIFT)
	{
		m_bShiftPressed = false;
	}
	else if(nChar == VK_CONTROL)
	{
		m_bCtrlPressed = false;
	}
	else if (nChar == ' ')
	{
		m_bSpacePressed = false;
	}
	else if (nChar == VK_DELETE)
	{
		m_bDeletePressed = false;
	}	
}

void CGUIEditorView::ResetView()
{
	static DWORD msgHash_RenderPageToContext = CHashString(_T("RenderPageToContext")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RenderPageToContext, sizeof(IRenderContext), m_pRC, NULL, NULL);

	RECT rect;
	GetClientRect(&rect);
	VIEWPORTPARAMS gwrp;
	gwrp.offsetx = 0;
	gwrp.offsety = 0;
	gwrp.width = rect.right;
	gwrp.height = rect.bottom;
	static DWORD msgHash_SetWindowSize = CHashString(_T("SetWindowSize")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetWindowSize, sizeof(VIEWPORTPARAMS), &gwrp, NULL, NULL);

	CWnd *desk = GetDesktopWindow();
	assert(desk);
	desk->GetClientRect(&rect);
	VIEWPORTPARAMS svp;
	svp.height = rect.bottom;
	svp.width = rect.right;
	static DWORD msgHash_SetViewPort = CHashString(_T("SetViewPort")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_SetViewPort, sizeof(VIEWPORTPARAMS), &svp);

//	FULLSCREENPARAMS fsp;
//	fsp.m_iWidth = m_iXRes;
//	fsp.m_iHeight = m_iYRes;
//	fsp.m_bFull = false;
static DWORD msgHash_SetFullScreen = CHashString(_T("SetFullScreen")).GetUniqueID();
//	m_ToolBox->SendMessage(msgHash_SetFullScreen, sizeof(FULLSCREENPARAMS), &fsp);

	POSITIONDATA pd;
	pd.m_fXPos = (float)m_iPageOffsetX;
	pd.m_fYPos = (float)m_iPageOffsetY;
	static DWORD msgHash_GUIManagerSetPageOffset = CHashString(_T("GUIManagerSetPageOffset")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GUIManagerSetPageOffset, sizeof(POSITIONDATA), &pd);

	static DWORD msgHash_SetZoomFactor = CHashString(_T("SetZoomFactor")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetZoomFactor, sizeof(float), &m_fZoomFactor);
}
void CGUIEditorView::OnCopyItem()
{
	CGUIEditorDoc *doc = DYNAMIC_DOWNCAST(CGUIEditorDoc, this->GetDocument());
	assert(doc);

	doc->CopyCurrent(m_szObjName, m_szObjType);
}

void CGUIEditorView::OnPasteNew()
{
	CGUIEditorDoc *doc = DYNAMIC_DOWNCAST(CGUIEditorDoc, this->GetDocument());
	assert(doc);
	doc->PasteNewItem();
}

void CGUIEditorView::OnPasteInst()
{
	CGUIEditorDoc *doc = DYNAMIC_DOWNCAST(CGUIEditorDoc, this->GetDocument());
	assert(doc);
	doc->PasteNewInstance();
}

void CGUIEditorView::SetSystemIndex(UINT value)
{
	m_iSystemIndex = value; 
	static DWORD msgHash_SetGUISystemIndex = CHashString(_T("SetGUISystemIndex")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetGUISystemIndex, sizeof(UINT), &value);
	
	static DWORD msgHash_RenderPageToContext = CHashString(_T("RenderPageToContext")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RenderPageToContext, sizeof(IRenderContext), m_pRC, NULL, NULL);

	ADDRENDERLISTPARAMS arlp;
	OBJECTLIST ol; // empty object list
	arlp.dwListPriorityKey = RENDERPRIORITY_FRONTBUFFER;
	arlp.dwSortMethod = SORT_FIFO;
	arlp.objList = &ol;
	arlp.renderContext = m_pRC;
	static DWORD msgHash_AddRenderList = CHashString(_T("AddRenderList")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddRenderList, sizeof(ADDRENDERLISTPARAMS), &arlp);
}
void CGUIEditorView::OnShowHide()
{
	if (_tcscmp(m_szCurrentGroup.GetString(), _T("")) != 0)
	{
		CGUIEditorDoc *doc = DYNAMIC_DOWNCAST(CGUIEditorDoc, this->GetDocument());
		assert(doc);
		doc->ShowHideGroup(m_szCurrentGroup);
	}
}

void CGUIEditorView::OnAnimTest()
{
	CHashString animName = _T("TestAnim");
	CHashString animType = _T("CGUIAnimation");
	static DWORD msgHash_StartGUIAnimation = CHashString(_T("StartGUIAnimation")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_StartGUIAnimation, 0, NULL, &animName, &animType);
}
