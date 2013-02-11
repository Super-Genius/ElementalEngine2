//=====================================================================
// File:		RenderContext.h
// Description: Header file for the Rendering contexts
//                  i.e. rendering pages inside windows
// Started:		09/29/2002
// Original:	Kenneth Hurley
//=====================================================================

#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

class CRenderContext
{
protected:
    HWND m_hWnd;                        // window in which context (page) is in
    LPCSTR m_Name;                      // just an identifier
    DWORD m_Left;                       // left position inside window
    DWORD m_Top;                        // top position inside window
    DWORD m_Width;                      // width of context
    DWORD m_Height;                     // height of context
    DWORD m_Bpp;                        // bits per pixel
    DWORD m_Depth;                      // depth, i.e. zBuffer
    DWORD m_Stencil;                    // stencill # bits
    BOOL m_FullScreen;                  // if the window is fullscreen
    CRenderContext *m_Parent;           // pointer to parent object
    VECINDEXRC m_Children;				// vector of children contexts
	CConfig m_RCConfig;					// configuration for render context

public:
    CRenderContext(HWND hWnd, LPCSTR name, DWORD left, DWORD top, 
                DWORD width, DWORD height, DWORD bpp, DWORD depth, 
                DWORD stencil, BOOL fullscreen, CRenderContext *parent)
    {
        // just copy everything
        m_hWnd = hWnd;
        m_Name = name;
        m_Bpp = bpp;
        m_Depth = depth;
        m_Stencil = stencil;
        m_FullScreen = fullscreen;
        m_Parent = parent;
        SetRect(left, top, width, height);
    }

    ~CRenderContext()
    {
        VECINDEXRC::iterator itRC;

        for (itRC = m_Children.begin(); itRC != m_Children.end(); itRC++)
        {
            delete (*itRC);
        }

        m_Children.empty();
    }

    virtual BOOL GetChildRenderContext(DWORD index, CRenderContext **pRC)
    {
        if (index < m_Children.size())
        {
            *pRC = m_Children[index];
            return TRUE;
        }

        return FALSE;
    }

    void AddChild(CRenderContext *child)
    {
		m_Children.push_back(child);
    }

	CConfig *GetConfig()
	{
		return &m_RCConfig;
	}

    void SetRect(DWORD left, DWORD top, DWORD width, DWORD height)
    {
        m_Left = left;
        m_Top = top;
        m_Width = width;
        m_Height = height;
    }

    void GetRect(RECT *outRect)
    {
        outRect->left = m_Left;
        outRect->top = m_Top;
        outRect->right = m_Left + m_Width;
        outRect->bottom = m_Top + m_Height;
    }

    HWND GetParentHwnd()
    {
        return m_hWnd;
    }

    DWORD GetStencil()
    {
        return m_Stencil;
    }

    DWORD GetZDepth()
    {
        return m_Depth;
    }

    LPCSTR GetName()
    {
        return m_Name;
    }
};

#endif      // #ifndef RENDERCONTEXT_H
