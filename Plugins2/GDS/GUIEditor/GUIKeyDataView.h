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

#pragma once
#include "afxwin.h"


enum VIEWMODE {NONE = 0, POSKEY, SIZEKEY, COLORKEY, ROTKEY};

// CGUIKeyDataView dialog

class CGUIKeyDataView : public CDialog
{
	DECLARE_DYNAMIC(CGUIKeyDataView)

public:
	CGUIKeyDataView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGUIKeyDataView();

// Dialog Data
	enum { IDD = IDD_KEYFRAMEDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CGUIEditorDoc *m_Doc;
	VIEWMODE m_Mode;
public:
	CString m_szObjName;
	UINT m_iKeyNum;
	CString m_szAttr1;
	CString m_szAttr2;
	CString m_szAttr3;
	CString m_szAttr4;
	CEdit m_cAttr1Edit;
	CEdit m_cAttr2Edit;
	CEdit m_cAttr3Edit;
	CEdit m_cAttr4Edit;

	void SetDocParent(CGUIEditorDoc *val) { m_Doc = val; }
	void SetPosData(StdString objName, UINT keyNum, float xPos, float yPos);
	void SetSizeData(StdString objName, UINT keyNum, float width, float height);
	void SetColorData(StdString objName, UINT keyNum, float a, float r, float g, float b);
	void SetRotationData(StdString objName, UINT keyNum, float rot);
	afx_msg void OnBnClickedKeyviewupdatebt();
};
