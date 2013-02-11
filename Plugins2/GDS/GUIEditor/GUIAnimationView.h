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

#ifndef _C_GUIANIMATIONVIEW_H_
#define _C_GUIANIMATIONVIEW_H_
#include "afxwin.h"

typedef struct DOTSPOT
{
	UINT key;
	UINT channel;
}DOTSPOT;

// CGUIAnimationView dialog


class CGUIAnimationView : public CDialog
{
	DECLARE_DYNAMIC(CGUIAnimationView)

public:
	CGUIAnimationView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGUIAnimationView();

// Dialog Data
	enum { IDD = IDD_ANIMVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	vector <StdString> m_vObjNames;
	vector <DOTSPOT> m_vDotSpots;
	UINT m_iTimePosition;
	int m_iXOffset;
	int m_iYOffset;
	UINT m_iFrameRate;
	CHashString m_hszLoadedAnimation;

	CGUIEditorDoc *m_Doc;

	/// various drawing funcs
	void DrawStaticItems(CPaintDC *dc);
	void DrawList(CPaintDC *dc);
	void DrawTimeBar(CPaintDC *dc);
	void DrawDotSpots(CPaintDC *dc);

	/// checks the point on mouse l button down to see where we clicked
	/// \param point - point of click
	/// \param channel - return channel where we clicked, or -1 if clicked above channels
	/// \param key - return key where we clicked, or -1 if clicked to left of keys
	void CheckClickSpace(CPoint point, int *channel, int *key);

	UINT m_iDotSpotClicked;	

	DOTSPOT m_EmptyChannelClicked;
public:
	void SetDocParent(CGUIEditorDoc *val) { m_Doc = val; }

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void SetAnimation(CHashString animName);
	void LoadAnimations(vector<StdString> anims);
	void LoadAnimation(StdString anim);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void SetAnimBarPos(UINT pos);
	UINT GetAnimBarPos() { return m_iTimePosition; }
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	void ViewAnimData();
	void AddOneKey();
	void DeleteKey();	
	CComboBox m_AnimSelComboBox;
	afx_msg void OnCbnSelchangeAnimselcombo();
};

#endif