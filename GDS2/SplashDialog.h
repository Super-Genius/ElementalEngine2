///============================================================================
/// \file		splashdialog.h
/// \brief		Header file for Splash dialog in GDC
/// \date		04-03-2007
/// \author		Kenneth Hurley
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

#ifndef _SPLASHDIALOG_H_
#define _SPLASHDIALOG_H_

#include "TransparentStatic.h"
#include "afxwin.h"

// CSplashDialog dialog

class CSplashDialog : public CDialog
{
	DECLARE_DYNAMIC(CSplashDialog)

public:
	CSplashDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSplashDialog();

	void EnableOK(bool enable = true);
	void SetLoadingText(LPCTSTR text);
// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CButton m_OKButton;
	CTransparentStatic m_WrittenBy;
	bool m_OKEnabled;
	bool m_bOKBitmapLoaded;
	CBitmap m_OKBitmap;
	CTransparentStatic m_LoadingText;
	CTransparentStatic m_LicensedTo;
	CFont m_TextFont;
public:
	virtual BOOL OnInitDialog();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};

#endif // #ifndef _SPLASHDIALOG_H_
