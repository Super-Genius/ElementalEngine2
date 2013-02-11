///=====================================================================
/// \file	EditFogKeyDialog.h
/// \brief	Definition of EditFogKeyDialog Class
///         This class implement an MFC Dialog Box
/// \date	02/22/2005
/// \author	Brian Bazyk
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
///=====================================================================

#include "stdafx.h"

EditFogKeyDialog::EditFogKeyDialog(FOGKEYFRAME in_Key, CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TODFOG_DIALOG, pParent)
{
	// set initial keyframe data
	m_Keyframe = in_Key;
}

EditFogKeyDialog::~EditFogKeyDialog()
{
}

FOGKEYFRAME EditFogKeyDialog::GetKeyframe()
{
	return m_Keyframe;
}

BOOL EditFogKeyDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// format keyframe data to displayable strings
	m_szFogColorText.Format( "%i %i %i", 
		m_Keyframe.m_FogColor.GetX(), 
		m_Keyframe.m_FogColor.GetY(), 
		m_Keyframe.m_FogColor.GetZ() );

	// set description text
	SetDlgItemText( IDC_EDITFOGDESC, "Description:\n"\
									"The fog color, start, and end values all directly"\
									"affect their respective render states." );

	UpdateData( FALSE );
	return TRUE;
}

void EditFogKeyDialog::OnOK()
{
	UpdateData( TRUE );
	CDialog::OnOK();
}

void EditFogKeyDialog::OnBnClickedFogColor()
{
	// color picker
	COLORREF initcolor = RGB( 
		m_Keyframe.m_FogColor.GetX(), 
		m_Keyframe.m_FogColor.GetY(), 
		m_Keyframe.m_FogColor.GetZ() );

	CColorDialog ColorDialog( initcolor, 0, this );
	if (ColorDialog.DoModal() == IDOK)
	{
		COLORREF c = ColorDialog.GetColor();
		m_Keyframe.m_FogColor.SetX(GetRValue(c));
		m_Keyframe.m_FogColor.SetY(GetGValue(c));
		m_Keyframe.m_FogColor.SetZ(GetBValue(c));
		m_szFogColorText.Format( "%i %i %i", 
			m_Keyframe.m_FogColor.GetX(), 
			m_Keyframe.m_FogColor.GetY(), 
			m_Keyframe.m_FogColor.GetZ() );
		UpdateData( FALSE );
	}
}

void EditFogKeyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EditFogKeyDialog)
	DDX_Text(pDX, IDC_EDITFOGKEY_FOGSTART, m_Keyframe.m_fFogStart);
	DDX_Text(pDX, IDC_EDITFOGKEY_FOGEND, m_Keyframe.m_fFogEnd);
	DDX_Text(pDX, IDC_EDITFOGCOLORTEXT, m_szFogColorText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(EditFogKeyDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDC_EDITFOGCOLORBN, OnBnClickedFogColor)
	ON_EN_CHANGE(IDC_EDITFOGKEY_FOGSTART, OnEnChangeEditfogkeyFogstart)
	ON_EN_CHANGE(IDC_EDITFOGKEY_FOGEND, OnEnChangeEditfogkeyFogend)
END_MESSAGE_MAP()

void EditFogKeyDialog::OnEnChangeEditfogkeyFogstart()
{
	UpdateData( TRUE );
}

void EditFogKeyDialog::OnEnChangeEditfogkeyFogend()
{
	UpdateData( TRUE );
}
