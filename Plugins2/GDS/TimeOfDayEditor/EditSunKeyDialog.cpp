///=====================================================================
/// \file	EditSunKeyDialog.h
/// \brief	Definition of EditSunKeyDialog Class
///         This class implements an MFC Dialog Box
/// \date	02/21/2005
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

EditSunKeyDialog::EditSunKeyDialog(SUNLIGHTKEYFRAME in_Keyframe, CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TODSUN_DIALOG, pParent)
{
	// set initial keyframe data
	m_Keyframe = in_Keyframe;
}

EditSunKeyDialog::~EditSunKeyDialog()
{
}

SUNLIGHTKEYFRAME EditSunKeyDialog::GetKeyframe()
{
	return m_Keyframe;
}

BOOL EditSunKeyDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// format keyframe data to displayable strings
	m_szAmbientText.Format( "%i %i %i", 
		(int)m_Keyframe.m_AmbientColor.GetX(), 
		(int)m_Keyframe.m_AmbientColor.GetY(), 
		(int)m_Keyframe.m_AmbientColor.GetZ() );
	m_szFullbrightText.Format( "%i %i %i", 
		(int)m_Keyframe.m_FullbrightColor.GetX(), 
		(int)m_Keyframe.m_FullbrightColor.GetY(), 
		(int)m_Keyframe.m_FullbrightColor.GetZ() );

	// set description text
	SetDlgItemText( IDC_EDITSUNDESC, "Description:\n"\
									"Sunlight color will vary from the ambient color "\
									"to the fullbright color according to the "\
									"NdotL lighting equation.\n"\
									"The dawn/dusk sky and night sky weights are real numbers "\
									"from 0..1 that describe the alpha blend value of the "\
									"respective sky layer." );

	UpdateData( FALSE );
	return TRUE;
}

void EditSunKeyDialog::OnOK()
{
	UpdateData( TRUE );
	CDialog::OnOK();
}

void EditSunKeyDialog::OnBnClickedAmbientColor()
{
	// color picker
	COLORREF initcolor = RGB( 
		m_Keyframe.m_AmbientColor.GetX(), 
		m_Keyframe.m_AmbientColor.GetY(), 
		m_Keyframe.m_AmbientColor.GetZ() );

	CColorDialog ColorDialog( initcolor, 0, this );
	if (ColorDialog.DoModal() == IDOK)
	{
		COLORREF c = ColorDialog.GetColor();
		m_Keyframe.m_AmbientColor.SetX(GetRValue(c));
		m_Keyframe.m_AmbientColor.SetY(GetGValue(c));
		m_Keyframe.m_AmbientColor.SetZ(GetBValue(c));
		m_szAmbientText.Format( "%i %i %i", 
			(int)m_Keyframe.m_AmbientColor.GetX(), 
			(int)m_Keyframe.m_AmbientColor.GetY(), 
			(int)m_Keyframe.m_AmbientColor.GetZ() );
		UpdateData( FALSE );
	}
}

void EditSunKeyDialog::OnBnClickedFullbrightColor()
{
	// color picker
	COLORREF initcolor = RGB( 
		m_Keyframe.m_FullbrightColor.GetX(), 
		m_Keyframe.m_FullbrightColor.GetY(), 
		m_Keyframe.m_FullbrightColor.GetZ() );

	CColorDialog ColorDialog( initcolor, 0, this );
	if (ColorDialog.DoModal() == IDOK)
	{
		COLORREF c = ColorDialog.GetColor();
		m_Keyframe.m_FullbrightColor.SetX(GetRValue(c));
		m_Keyframe.m_FullbrightColor.SetY(GetGValue(c));
		m_Keyframe.m_FullbrightColor.SetZ(GetBValue(c));
		m_szFullbrightText.Format( "%i %i %i", 
			(int)m_Keyframe.m_FullbrightColor.GetX(), 
			(int)m_Keyframe.m_FullbrightColor.GetY(), 
			(int)m_Keyframe.m_FullbrightColor.GetZ() );
		UpdateData( FALSE );
	}
}

void EditSunKeyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EditSunKeyDialog)
	DDX_Text(pDX, IDC_EDITSUNKEY_DAWN,  m_Keyframe.m_fDawnWeight);
	DDV_MinMaxFloat(pDX, m_Keyframe.m_fDawnWeight, 0.0f, 1.0f);

	DDX_Text(pDX, IDC_EDITSUNKEY_NIGHT,  m_Keyframe.m_fNightWeight);
	DDV_MinMaxFloat(pDX, m_Keyframe.m_fNightWeight, 0.0f, 1.0f);

	DDX_Text(pDX, IDC_EDITSUNAMBIENTTEXT, m_szAmbientText);
	DDX_Text(pDX, IDC_EDITSUNFULLBRIGHTTEXT, m_szFullbrightText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(EditSunKeyDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDC_EDITSUNAMBIENTBN, OnBnClickedAmbientColor)
	ON_BN_CLICKED(IDC_EDITSUNFULLBRIGHTBN, OnBnClickedFullbrightColor)
	ON_EN_CHANGE(IDC_EDITSUNKEY_DAWN, OnEnChangeEditsunkeyDawn)
	ON_EN_CHANGE(IDC_EDITSUNKEY_NIGHT, OnEnChangeEditsunkeyNight)
END_MESSAGE_MAP()

void EditSunKeyDialog::OnEnChangeEditsunkeyDawn()
{
	UpdateData( TRUE );
}

void EditSunKeyDialog::OnEnChangeEditsunkeyNight()
{
	UpdateData( TRUE );
}
