// RenderView.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRenderView

IMPLEMENT_DYNCREATE(CRenderView, CView)

CRenderView::CRenderView()
{
    m_Renderer = NULL;
	m_ViewMat = NULL;
	m_ViewCamera = NULL;
	m_BoxMatrix = NULL;
	m_BoxMesh = NULL;
}

CRenderView::~CRenderView()
{
    if (m_Renderer != NULL)
    {
        m_Renderer->ShutDown();
        m_Renderer = NULL;
    }
}


BEGIN_MESSAGE_MAP(CRenderView, CView)
	//{{AFX_MSG_MAP(CRenderView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_RENDER_WIREFRAME, OnRenderWireframe)
	ON_COMMAND(ID_RENDER_TEXTURE, OnRenderTexture)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRenderView drawing

void CRenderView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CRenderView diagnostics

#ifdef _DEBUG
void CRenderView::AssertValid() const
{
	CView::AssertValid();
}

void CRenderView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

//---------------------------------------------------------------------
// Function:    SetRenderer
// Description: Sets the render for this view
// Parameters:  renderer = pointer to CRender DLL Interface
// Returns:     .
//---------------------------------------------------------------------
void CRenderView::SetRenderer(CRender *renderer, CString *name)
{
    CRect cRect;
    CString tmpName;
    CRenderContext *rc;
	CRenderContext *childRC;

    // set the renderer so onidle will function
    m_Renderer = renderer;
    
    GetClientRect(cRect);
    
    // initialize the renderer
    if (!m_Renderer->Initialize())
    {
        gLogger->Print(_T("Unable to initialize renderer %s"),
                (LPCSTR)name);
        return;
    }
    
    // %%% load view type from registry

    tmpName = *name + _T(" View 1");
    // for now just put up 4 windows
    // since we are windowed mode, we should match the desktop bits per pixel.
    if (!m_Renderer->CreateRenderContext(m_hWnd, tmpName, 0, 0,
        cRect.Width()/2, cRect.Height()/2, 32, 24, 0, FALSE, NULL))
    {
        gLogger->Print(_T("CreateRenderContext View 1 Failed, see log file"));
        MessageBox(_T("Unable to Create Rendering Context,\n"
            "Please see log file for more details."), _T("RenderContext Error!"));
        return;
    }

    // get that render context 
    if (!m_Renderer->GetRenderContext(m_hWnd, &rc))
    {
        gLogger->Print(_T("GetRenderContext failed?"));
        return;
    }

    // set the background color for rendering
    rc->GetConfig()->SetValue("Background Color", (DWORD)0x000000);

    tmpName = *name + _T(" View 2");
    // now create the next one, rc != NULL means it's a child of rc
    if (!m_Renderer->CreateRenderContext(m_hWnd, tmpName, cRect.Width()/2, 0,
        cRect.Width()/2, cRect.Height()/2, 32, 24, 0, FALSE, rc))
    {
        gLogger->Print(_T("CreateRenderContext View 2 Failed, see log file"));
        MessageBox(_T("Unable to Create Rendering Context,\n"
            "Please see log file for more details."), _T("RenderContext Error!"));
        return;
    }

    // get that render context 
	if (!rc->GetChildRenderContext(0, &childRC))
    {
        gLogger->Print(_T("GetChildRenderContext failed?"));
        return;
    }

	childRC->GetConfig()->SetValue("Background Color", (DWORD)0x00ff00);

    tmpName = *name + _T(" View 3");
    // now create the next one, rc != NULL means it's a child of rc
    if (!m_Renderer->CreateRenderContext(m_hWnd, tmpName, 0, cRect.Height()/2,
        cRect.Width()/2, cRect.Height()/2, 32, 24, 0, FALSE, rc))
    {
        gLogger->Print(_T("CreateRenderContext View 3 Failed, see log file"));
        MessageBox(_T("Unable to Create Rendering Context,\n"
            "Please see log file for more details."), _T("RenderContext Error!"));
        return;
    }

    // get that render context 
	if (!rc->GetChildRenderContext(1, &childRC))
    {
        gLogger->Print(_T("GetChildRenderContext failed?"));
        return;
    }

	childRC->GetConfig()->SetValue("Background Color", (DWORD)0x0000ff);

    tmpName = *name + _T(" View 4");
    // now create the next one, NULL for HWND means it's a chile of last one
    if (!m_Renderer->CreateRenderContext(m_hWnd, tmpName, cRect.Width()/2, 
            cRect.Height()/2, cRect.Width()/2, cRect.Height()/2, 32, 24, 0, 
            FALSE, rc))
    {
        gLogger->Print(_T("CreateRenderContext View 4 Failed, see log file"));
        MessageBox(_T("Unable to Create Rendering Context,\n"
            "Please see log file for more details."), _T("RenderContext Error!"));
        return;
    }

    // get that render context 
	if (!rc->GetChildRenderContext(2, &childRC))
    {
        gLogger->Print(_T("GetChildRenderContext 2 failed?"));
        return;
    }

	childRC->GetConfig()->SetValue("Background Color", (DWORD)0xff00ff);

}

//---------------------------------------------------------------------
// Function:	OnIdle
// Description:	Called when windows gives us a timeslice to operate
//                  in
// Parameters:	lCount = count of OnIdle calls between messages
// Returns:		non-zero to receive more idle time processing
//---------------------------------------------------------------------
BOOL CRenderView::OnIdle(LONG lCount)
{
    DWORD i;

    if (m_Renderer != NULL)
    {
        for (i=0; i<4; i++)
        {
            // start the rendering process, render context index i
            if (!m_Renderer->BeginFrame(m_hWnd, i))
            {
                gLogger->Print(_T("Unable to begin rendering frame on view %d"), i);
                return 0;
            }

			// iterate through objects calling render.
			// This is also dependencies may get updated, i.e.
			// when Some objects don't render
            m_Renderer->EndFrame(m_hWnd, i);
        }

        for (i=0; i<4; i++)
        {
            m_Renderer->ShowFrame(m_hWnd, i);
        }
    }
    
    return 0;
}

//---------------------------------------------------------------------
// Function:	CreateBoxObject
// Description:	Creates a box object, with view, world & object matrix
//					for testings hierarchy.  Adds this to the object
//					manager
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
void CRenderView::CreateBoxObject()
{
	if (m_ViewMat == NULL)
	{
		m_ViewMat = new CMatrix4(_T("View"));
		if (m_ViewMat == NULL)
		{
			gLogger->Print(_T("Failed to create view matrix"));
			return;
		}

		if (!gBaseRender->m_ProgramInfo->ObjectManager->AddObject(m_ViewMat))
		{
			gLogger->Print(_T("Failed to add view matrix to hierarchy"));
			return;
		}

	}

	if (m_ViewCamera == NULL)
	{
		m_ViewCamera = new CCamera(_T("View Camera"));
		if (m_ViewCamera == NULL)
		{
			gLogger->Print(_T("Failed to create view camera"));
			return;
		}

		if (!gBaseRender->m_ProgramInfo->ObjectManager->AddObject(m_ViewCamera, m_ViewMat))
		{
			gLogger->Print(_T("Failed to add view matrix to hierarchy"));
			return;
		}
	}

	if (m_BoxMatrix == NULL)
	{
		m_BoxMatrix = new CMatrix4(_T("Box Matrix"));
		if (m_BoxMatrix == NULL)
		{
			gLogger->Print(_T("Failed to create box matrix"));
			return;
		}

		if (!gBaseRender->m_ProgramInfo->ObjectManager->AddObject(m_BoxMatrix, m_ViewCamera))
		{
			gLogger->Print(_T("Failed to add box matrix to hierarchy"));
			return;
		}
	}

	if (m_BoxMesh == NULL)
	{
		m_BoxMesh = new CMesh();
		if (m_BoxMesh == NULL)
		{
			gLogger->Print(_T("Failed to create box mesh"));
			return;
		}

		m_BoxMesh->SetName(_T("Box Mesh"));
		m_BoxMesh->LockHardwareStream(0);

		if (!gBaseRender->m_ProgramInfo->ObjectManager->AddObject(m_BoxMesh, m_BoxMatrix))
		{
			gLogger->Print(_T("Failed to add box mesh to hierarchy"));
			return;
		}
	}

//	viewMat->SetIdentity();
}

//---------------------------------------------------------------------
// Function:	RemoveBoxObject
// Description:	Removes the box object
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
void CRenderView::RemoveBoxObject()
{
	if (m_ViewMat != NULL)
	{
		// this should remove children too.
		gBaseRender->m_ProgramInfo->ObjectManager->RemoveObject(m_ViewMat);
	
		delete m_ViewMat;	// this will remove object and children
							// from manager also.
		m_ViewMat = NULL;
		m_ViewCamera = NULL;
		m_BoxMatrix = NULL;
		m_BoxMesh = NULL;
	}

//	viewMat->SetIdentity();
}

/////////////////////////////////////////////////////////////////////////////
// CRenderView message handlers


void CRenderView::OnRenderWireframe()
{
	RemoveBoxObject();
}

void CRenderView::OnRenderTexture()
{
	CreateBoxObject();
}
