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

#ifndef _EXTRACTIMAGEHELPER_H_
#define _EXTRACTIMAGEHELPER_H_
 
 
/////////////////////////////////////////////////////////////////////////////
// CExtractImageHelper
 
class CExtractImageHelper
{
protected:
    CExtractImageHelper();                
    virtual ~CExtractImageHelper();
 
public:
    /// get the image give a pointer to a shell folder
    static HBITMAP GetImage(HWND hWnd, IShellFolder *spShellFolder, LPCITEMIDLIST pidl, const CSize& size, DWORD dwDepth = 32)
    {
        if (spShellFolder == NULL || pidl == NULL)
        {
            return NULL;
        }
 
        HRESULT hr = S_OK;
 
        IExtractImage *spExtractImage;
        hr = spShellFolder->GetUIObjectOf(hWnd, 1, &pidl, IID_IExtractImage, NULL, (void**)&spExtractImage);
        if (FAILED(hr))
        {
            return NULL;
        }
 
        return GetImage(spExtractImage, size, dwDepth);
    }

	// give a IExtractImage pointer get the thumbnail for the image
    static HBITMAP GetImage(IExtractImage *spExtractImage, const CSize& size, DWORD dwDepth = 32)
    {
        if (spExtractImage == NULL)
        {
            return NULL;
        }
 
        HRESULT hr = S_OK;
 
        OLECHAR wszPathBuffer[MAX_PATH];
        DWORD dwPriority = 0;                // IEI_PRIORITY_NORMAL is defined nowhere!
        DWORD dwFlags = IEIFLAG_SCREEN;
        hr = spExtractImage->GetLocation(wszPathBuffer, MAX_PATH, &dwPriority, &size, dwDepth, &dwFlags);
        if (FAILED(hr))
        {
            return NULL;
        }
 
        HBITMAP hBitmap = NULL;
        hr = spExtractImage->Extract(&hBitmap);
        if (FAILED(hr))
        {
            return NULL;
        }
 
        return hBitmap;
    }

    // get the Icon from the pidl, draws to a bitmap
    static HBITMAP GetImageFromIcon(HWND hWnd, LPCITEMIDLIST pidl, const CSize& size, DWORD dwDepth = 32)
    {
        if (pidl == NULL)
        {
            return NULL;
        }
 
        SHFILEINFO info;
        ZeroMemory(&info, sizeof(info));
        UINT uFlags = SHGFI_PIDL | SHGFI_ICON | SHGFI_LARGEICON | SHGFI_SYSICONINDEX;
        HIMAGELIST hImageList = (HIMAGELIST)SHGetFileInfo((LPCSTR)pidl, 0, &info, sizeof(info), uFlags);
 
        HICON hIcon = ImageList_GetIcon(hImageList, info.iIcon, ILD_TRANSPARENT);
 
        HDC hDC = GetDC(hWnd);
        HDC hMemDC = CreateCompatibleDC(hDC);
        HBITMAP hBitmap = (HBITMAP)CreateCompatibleBitmap(hDC, size.cx, size.cy);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, (HBITMAP)hBitmap);
 
        FillRect(hMemDC, CRect(0, 0, size.cx, size.cy), GetSysColorBrush(COLOR_WINDOW));
        DrawIcon(hMemDC, (size.cx - 32) / 2, (size.cy - 32) / 2, hIcon);
 
        SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
        DeleteDC(hDC);
 
        return hBitmap;
    }
 
};

#endif		// #ifndef _EXTRACTIMAGEHELPER_H_
