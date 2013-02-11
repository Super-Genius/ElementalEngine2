///=====================================================================
/// \file	TimeOfDayDialog.cpp
/// \brief	Implementation of TimeOfDayDialog Class
///         These classes implements a MFC Dialog Box (TimeOfDay)
/// \date	02/15/2005
/// \author Brian Bazyk
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

IMPLEMENT_DYNAMIC(CTimeOfDayDialog, CDialog)

const char *ColorToStringRGB( const Vec3 &color )
{
	static char szBuffer[64];
	sprintf( szBuffer, "%i %i %i", 
		(int)color.GetX(), 
		(int)color.GetY(), 
		(int)color.GetZ() );
	return szBuffer;
}

void SetListSelectedItem( CListCtrl *pList, int iItem )
{
//	pList->SetFocus();

	for (int i=0; i < pList->GetItemCount(); i++)
		pList->SetItemState( i, 0, LVIS_SELECTED );

	if (iItem >= 0 && iItem < pList->GetItemCount())
	{
		pList->SetItemState( iItem, LVIS_SELECTED, LVIS_SELECTED );
		pList->SetSelectionMark( iItem );
	}
}

int GetListSelectedItem( CListCtrl *pList )
{
//	pList->SetFocus();

	for (int i=0; i < pList->GetItemCount(); i++)
	{
		if (pList->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
			return i;
	}
	return -1;
}

char *CTimeOfDayDialog::m_vSkyLayerNames[] = { "SkyLayer0", "SkyLayer1", "SkyLayer2" };
char *CTimeOfDayDialog::m_vSkyLayerCubeNames[] = { "CubeRight", "CubeLeft", "CubeFront", "CubeBack", "CubeTop" };

CTimeOfDayDialog::CTimeOfDayDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TOD_DIALOG, pParent)
{
	m_ToolBox = EngineGetToolBox();
	m_SunlightKeyframes.clear();
	m_FogKeyframes.clear();
	m_TimeOfDayObjectName = NULL;
	m_SkyBoxObjectName = NULL;
}

CTimeOfDayDialog::~CTimeOfDayDialog()
{
}

BOOL CTimeOfDayDialog::OnInitDialog()
{
	m_SunKeyframeList.SubclassDlgItem( IDC_LISTSUN, this );
	m_SunKeyframeList.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	m_SunKeyframeList.InsertColumn( 0, "Key", LVCFMT_LEFT, 30, 0 );
	m_SunKeyframeList.InsertColumn( 1, "Ambient Color", LVCFMT_LEFT, 90, 1 );
	m_SunKeyframeList.InsertColumn( 2, "Fullbright Color", LVCFMT_LEFT, 90, 2 );
	m_SunKeyframeList.InsertColumn( 3, "Dawn Sky Weight", LVCFMT_LEFT, 100, 3 );
	m_SunKeyframeList.InsertColumn( 4, "Night Sky Weight", LVCFMT_LEFT, 95, 4 );

	m_FogKeyList.SubclassDlgItem( IDC_LISTFOG, this );
	m_FogKeyList.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	m_FogKeyList.InsertColumn( 0, "Key", LVCFMT_LEFT, 30, 0 );
	m_FogKeyList.InsertColumn( 1, "Fog Color", LVCFMT_LEFT, 90, 1 );
	m_FogKeyList.InsertColumn( 2, "Fog Start", LVCFMT_LEFT, 65, 2 );
	m_FogKeyList.InsertColumn( 3, "Fog End", LVCFMT_LEFT, 65, 3 );

	return TRUE;
}

void CTimeOfDayDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSampleDialog)
	DDX_Text(pDX, IDC_TEXTURE0,  m_vszSkyTextures[0]);
	DDX_Text(pDX, IDC_TEXTURE1,  m_vszSkyTextures[1]);
	DDX_Text(pDX, IDC_TEXTURE2,  m_vszSkyTextures[2]);
	DDX_Text(pDX, IDC_TEXTURE3,  m_vszSkyTextures[3]);
	DDX_Text(pDX, IDC_TEXTURE4,  m_vszSkyTextures[4]);
	DDX_Text(pDX, IDC_TEXTURE5,  m_vszSkyTextures[5]);
	DDX_Text(pDX, IDC_TEXTURE6,  m_vszSkyTextures[6]);
	DDX_Text(pDX, IDC_TEXTURE7,  m_vszSkyTextures[7]);
	DDX_Text(pDX, IDC_TEXTURE8,  m_vszSkyTextures[8]);
	DDX_Text(pDX, IDC_TEXTURE9,  m_vszSkyTextures[9]);
	DDX_Text(pDX, IDC_TEXTURE10, m_vszSkyTextures[10]);
	DDX_Text(pDX, IDC_TEXTURE11, m_vszSkyTextures[11]);
	DDX_Text(pDX, IDC_TEXTURE12, m_vszSkyTextures[12]);
	DDX_Text(pDX, IDC_TEXTURE13, m_vszSkyTextures[13]);
	DDX_Text(pDX, IDC_TEXTURE14, m_vszSkyTextures[14]);

	DDX_Control(pDX, IDC_LISTSUN, m_SunKeyframeList);
	DDX_Control(pDX, IDC_LISTFOG, m_FogKeyList);
	//}}AFX_DATA_MAP
}

void CTimeOfDayDialog::RebuildSunKeyList( int iSelectedItem )
{
	CString szTemp;

	// rebuild sun key list box
	m_SunKeyframeList.DeleteAllItems();
	for (unsigned int i=0; i < m_SunlightKeyframes.size(); i++)
	{
		SUNLIGHTKEYFRAME &SunlightKeyframe = m_SunlightKeyframes[i];

		szTemp.Format( "%i", i );
		m_SunKeyframeList.InsertItem( i, szTemp );

		m_SunKeyframeList.SetItemText( i, 1, ColorToStringRGB(SunlightKeyframe.m_AmbientColor) );
		m_SunKeyframeList.SetItemText( i, 2, ColorToStringRGB(SunlightKeyframe.m_FullbrightColor) );

		szTemp.Format( "%.2f", SunlightKeyframe.m_fDawnWeight );
		m_SunKeyframeList.SetItemText( i, 3, szTemp );

		szTemp.Format( "%.2f", SunlightKeyframe.m_fNightWeight );
		m_SunKeyframeList.SetItemText( i, 4, szTemp );
	}

	SetListSelectedItem( &m_SunKeyframeList, iSelectedItem );
}

void CTimeOfDayDialog::RebuildFogKeyList( int iSelectedItem )
{
	CString szTemp;

	// rebuild fog key listbox
	m_FogKeyList.DeleteAllItems();
	for (unsigned int i=0; i < m_FogKeyframes.size(); i++)
	{
		FOGKEYFRAME &FogKeyframe = m_FogKeyframes[i];

		szTemp.Format( "%i", i );
		m_FogKeyList.InsertItem( i, szTemp );

		m_FogKeyList.SetItemText( i, 1, ColorToStringRGB(FogKeyframe.m_FogColor) );

		szTemp.Format( "%.2f", FogKeyframe.m_fFogStart );
		m_FogKeyList.SetItemText( i, 2, szTemp );

		szTemp.Format( "%.2f", FogKeyframe.m_fFogEnd );
		m_FogKeyList.SetItemText( i, 3, szTemp );
	}

	SetListSelectedItem( &m_FogKeyList, iSelectedItem );
}

void CTimeOfDayDialog::UpdateDialog()
{
	RebuildSunKeyList( -1 );
	RebuildFogKeyList( -1 );
	UpdateData( FALSE );
}

BEGIN_MESSAGE_MAP(CTimeOfDayDialog, CDialog)
	ON_BN_CLICKED(IDCLOSE, OnBnClickedClose)
	ON_BN_CLICKED(ID_CANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_IMPORT, OnBnClickedImport)
	ON_BN_CLICKED(IDC_EXPORT, OnBnClickedExport)

	ON_BN_CLICKED(IDC_BUTTON0, OnBnClickedSkyLayer0)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedSkyLayer1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedSkyLayer2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedSkyLayer3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedSkyLayer4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedSkyLayer5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedSkyLayer6)
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedSkyLayer7)
	ON_BN_CLICKED(IDC_BUTTON8, OnBnClickedSkyLayer8)
	ON_BN_CLICKED(IDC_BUTTON9, OnBnClickedSkyLayer9)
	ON_BN_CLICKED(IDC_BUTTON10, OnBnClickedSkyLayer10)
	ON_BN_CLICKED(IDC_BUTTON11, OnBnClickedSkyLayer11)
	ON_BN_CLICKED(IDC_BUTTON12, OnBnClickedSkyLayer12)
	ON_BN_CLICKED(IDC_BUTTON13, OnBnClickedSkyLayer13)
	ON_BN_CLICKED(IDC_BUTTON14, OnBnClickedSkyLayer14)

	ON_NOTIFY( NM_DBLCLK, IDC_LISTSUN, OnDblClkListSun)
	ON_NOTIFY( NM_DBLCLK, IDC_LISTFOG, OnDblClkListFog)

	ON_BN_CLICKED(IDC_BUTTONSUNADD, OnBnClickedSunAdd)
	ON_BN_CLICKED(IDC_BUTTONSUNDEL, OnBnClickedSunDel)
	ON_BN_CLICKED(IDC_BUTTONFOGADD, OnBnClickedFogAdd)
	ON_BN_CLICKED(IDC_BUTTONFOGDEL, OnBnClickedFogDel)

	ON_BN_CLICKED(IDC_BUTTONSUNUP, OnBnClickedSunUp)
	ON_BN_CLICKED(IDC_BUTTONSUNDN, OnBnClickedSunDn)

	ON_BN_CLICKED(IDC_BUTTONFOGUP, OnBnClickedFogUp)
	ON_BN_CLICKED(IDC_BUTTONFOGDN, OnBnClickedFogDn)
END_MESSAGE_MAP()

// TimeOfDayDialog message handlers-
void CTimeOfDayDialog::OnBnClickedClose()
{
	// export frame data
	ExportFrames();

	// export skybox data
	ExportSkyboxData();
	
	UpdateData( TRUE );
	EndDialog(IDCLOSE);
}

void CTimeOfDayDialog::OnBnClickedCancel()
{
	// close the dialog
	UpdateData( TRUE );
	EndDialog(ID_CANCEL);
}

void CTimeOfDayDialog::OnBnClickedImport()
{
	static char BASED_CODE szFilter[] = "Time of Day Config (*.xml)|*.xml|All Files (*.*)|*.*||";

	// Create FileDialog
	CFileDialog FileDialog(	TRUE, NULL, NULL,
							OFN_HIDEREADONLY | OFN_EXPLORER,
							szFilter, this );
	// Run FileDialog
	if( FileDialog.DoModal() == IDOK )
	{
#if 0	// %%%%% change to IXMLArchive!
		TiXmlDocument XmlDoc;
		if (XmlDoc.LoadFile( FileDialog.GetPathName() ))
		{
			// Grab the First Element in the XML Document
			TiXmlElement *xConfig = 0;
			// Check to see if this Element represents a "Config"
			xConfig = XmlDoc.FirstChildElement("TimeOfDayConfig");
			if (!xConfig)
			{
				XmlDoc.Clear();
				return; // fail
			}

			// Get the sky layers
			int iSkyLayer = 0;
			for (int i=0; i < 3; i++)
			{
				TiXmlElement *xSkyLayer = xConfig->FirstChildElement( m_vSkyLayerNames[i] );
				if (xSkyLayer)
				{
					for (int iCubeMap=0; iCubeMap < 5; iCubeMap++)
						m_vszSkyTextures[iSkyLayer++] = xSkyLayer->Attribute( m_vSkyLayerCubeNames[iCubeMap] );
				}
				else
				{
					for (int iCubeMap=0; iCubeMap < 5; iCubeMap++)
						m_vszSkyTextures[iSkyLayer++] = "";
				}
			}

			// Get the sunlight key frames
			TiXmlElement *xSunlightKeyframes = xConfig->FirstChildElement( "SunlightKeyframes" );
			if (xSunlightKeyframes)
			{
				m_SunlightKeyframes.clear();

				int nKeyframes = atoi( xSunlightKeyframes->Attribute("Count") );

				TiXmlElement *xKeyframe = xSunlightKeyframes->FirstChildElement( "Keyframe" );
				for (int i=0; (i < nKeyframes) && xKeyframe; i++)
				{
					SUNLIGHTKEYFRAME SunlightKeyframe;

					XMLHelper::StringToColor( xKeyframe->Attribute("AmbientColor"), SunlightKeyframe.AmbientColor );
					XMLHelper::StringToColor( xKeyframe->Attribute("FullbrightColor"), SunlightKeyframe.FullbrightColor );
					SunlightKeyframe.fDawnWeight  = (float)atof( xKeyframe->Attribute("DawnWeight") );
					SunlightKeyframe.fNightWeight = (float)atof( xKeyframe->Attribute("NightWeight") );

					m_SunlightKeyframes.push_back( SunlightKeyframe );
					xKeyframe = xKeyframe->NextSiblingElement( "Keyframe" );
				}
			}

			// Get the fog key frames
			TiXmlElement *xFogKeyframes = xConfig->FirstChildElement( "FogKeyframes" );
			if (xFogKeyframes)
			{
				m_FogKeyframes.clear();

				int nKeyframes = atoi( xFogKeyframes->Attribute("Count") );

				TiXmlElement *xKeyframe = xFogKeyframes->FirstChildElement( "Keyframe" );
				for (int i=0; (i < nKeyframes) && xKeyframe; i++)
				{
					FOGKEYFRAME FogKeyframe;

					XMLHelper::StringToColor( xKeyframe->Attribute("FogColor"), FogKeyframe.FogColor, 3 );
					FogKeyframe.fFogStart = (float)atof( xKeyframe->Attribute("FogStart") );
					FogKeyframe.fFogEnd   = (float)atof( xKeyframe->Attribute("FogEnd") );

					m_FogKeyframes.push_back( FogKeyframe );
					xKeyframe = xKeyframe->NextSiblingElement( "Keyframe" );
				}
			}

			XmlDoc.Clear();
		}
#endif	//#if 0	// %%%%% change to IXMLArchive!
	}

	// Update
	UpdateDialog();
	Invalidate();
}

void CTimeOfDayDialog::OnBnClickedExport()
{
	static char BASED_CODE szFilter[] = "Time of Day Config (*.xml)|*.xml|All Files (*.*)|*.*||";

	// Create FileDialog
	CFileDialog FileDialog(	TRUE, "xml", NULL,
							OFN_HIDEREADONLY | OFN_EXPLORER,
							szFilter, this );
	// Run FileDialog
	if( FileDialog.DoModal() == IDOK )
	{
#if 0	// %%%%% change to IXMLArchive!
		TiXmlDocument XmlDoc;

		TiXmlDeclaration xDecl( "1.0", "SigDev TimeOfDayConfig", "Yes" );
		XmlDoc.InsertEndChild( xDecl );

		TiXmlElement xConfig( "TimeOfDayConfig" );

		// write out sky layers
		int iSkyLayer = 0;
		for (int i=0; i < 3; i++)
		{
			TiXmlElement xSkyLayer( m_vSkyLayerNames[i] );

			for (int iCubeMap=0; iCubeMap < 5; iCubeMap++)
				xSkyLayer.SetAttribute( m_vSkyLayerCubeNames[iCubeMap], m_vszSkyTextures[iSkyLayer++] );

			xConfig.InsertEndChild( xSkyLayer );
		}

		// write out sun keys
		TiXmlElement xSunlightKeyframes( "SunlightKeyframes" );
		xSunlightKeyframes.SetAttribute( "Count", m_SunlightKeyframes.size() );

		// for each sun key
		for (unsigned int iKey=0; iKey < m_SunlightKeyframes.size(); iKey++)
		{
			// fill out a keyframe element
			TiXmlElement xKeyframe( "Keyframe" );

			SUNLIGHTKEYFRAME SunlightKey;
			SunlightKey = m_SunlightKeyframes[iKey];

			xKeyframe.SetAttribute( "AmbientColor", XMLHelper::ColorToString( SunlightKey.AmbientColor, 3 ) );
			xKeyframe.SetAttribute( "FullbrightColor", XMLHelper::ColorToString( SunlightKey.FullbrightColor, 3 ) );
			char szTemp[64];
			sprintf( szTemp, "%f", SunlightKey.fDawnWeight );
			xKeyframe.SetAttribute( "DawnWeight", szTemp );
			sprintf( szTemp, "%f", SunlightKey.fNightWeight );
			xKeyframe.SetAttribute( "NightWeight", szTemp );

			// write to sun key list
			xSunlightKeyframes.InsertEndChild( xKeyframe );
		}
		xConfig.InsertEndChild( xSunlightKeyframes );

		// write out fog keys
		TiXmlElement xFogKeyframes( "FogKeyframes" );
		xFogKeyframes.SetAttribute( "Count", m_FogKeyframes.size() );

		// for each fog key
		for (unsigned int iKey=0; iKey < m_FogKeyframes.size(); iKey++)
		{
			// fill out a keyframe element
			TiXmlElement xKeyframe( "Keyframe" );

			FOGKEYFRAME FogKey;
			FogKey = m_FogKeyframes[iKey];

			xKeyframe.SetAttribute( "FogColor", XMLHelper::ColorToString( FogKey.FogColor, 3 ) );
			char szTemp[64];
			sprintf( szTemp, "%f", FogKey.fFogStart );
			xKeyframe.SetAttribute( "FogStart", szTemp );
			sprintf( szTemp, "%f", FogKey.fFogEnd );
			xKeyframe.SetAttribute( "FogEnd", szTemp );

			// write to sun key list
			xFogKeyframes.InsertEndChild( xKeyframe );
		}
		xConfig.InsertEndChild( xFogKeyframes );

		// write config
		XmlDoc.InsertEndChild( xConfig );

		// save file
		if (!XmlDoc.SaveFile( FileDialog.GetPathName() ))
		{
			MessageBox( "Failed to save xml file.  Check to see if the file is write protected." );
		}
#endif	//	#if 0	// %%%%% change to IXMLArchive!
	}
}

void CTimeOfDayDialog::OnBnClickedSkyLayer( int iFaceIndex )
{
	static char BASED_CODE szFilter[] = "Texture Files (*.tex;*.dds;*.bmp)|*.tex;*.dds;*.bmp|All Files (*.*)|*.*||";
	CFileDialog FileDialog(	TRUE, NULL, NULL,
							OFN_HIDEREADONLY | OFN_EXPLORER,
							szFilter, this );
	// Run FileDialog
	if( FileDialog.DoModal() == IDOK )
	{
		// remove path up to 'Textures' to make it relative to install dir
		CString szPath;
		CString szShortenedPath;
		szPath = FileDialog.GetPathName();
		int iTrimLeft = szPath.Find( "Textures" );
		szShortenedPath = szPath.Right( szPath.GetLength() - iTrimLeft );

		m_vszSkyTextures[iFaceIndex] = szShortenedPath;
		UpdateData( FALSE );
	}
}

void CTimeOfDayDialog::OnDblClkListSun(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)pNMHDR;
	int iItem = lpnmitem->iItem;
	int iSubItem = lpnmitem->iSubItem;

	if (m_SunlightKeyframes.size() > 0)
	{
		// open key edit dialog
		EditSunKeyDialog KeyframeDialog( m_SunlightKeyframes[iItem], this );
		if (KeyframeDialog.DoModal() == IDOK)
		{
			SUNLIGHTKEYFRAME Keyframe;
			Keyframe = KeyframeDialog.GetKeyframe();
			m_SunlightKeyframes[iItem] = Keyframe;
		}

		RebuildSunKeyList( iItem );
	}
}

void CTimeOfDayDialog::OnDblClkListFog(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)pNMHDR;
	int iItem = lpnmitem->iItem;
	int iSubItem = lpnmitem->iSubItem;

	if (m_FogKeyframes.size() > 0)
	{
		// open key edit dialog
		EditFogKeyDialog KeyframeDialog( m_FogKeyframes[iItem], this );
		if (KeyframeDialog.DoModal() == IDOK)
		{
			FOGKEYFRAME Keyframe;
			Keyframe = KeyframeDialog.GetKeyframe();
			m_FogKeyframes[iItem] = Keyframe;
		}

		RebuildFogKeyList( iItem );
	}
}

void CTimeOfDayDialog::OnBnClickedSunAdd()
{
	// insert key
	SUNLIGHTKEYFRAME SunlightKeyframe;
	SunlightKeyframe.m_AmbientColor.Set(0,0,0);
	SunlightKeyframe.m_FullbrightColor.Set(255,255,255);
	SunlightKeyframe.m_fDawnWeight = 0.0f;
	SunlightKeyframe.m_fNightWeight = 0.0f;

	int iSelected = GetListSelectedItem( &m_SunKeyframeList );
	if (iSelected > -1)
	{
		m_SunlightKeyframes.insert( m_SunlightKeyframes.begin() + iSelected, SunlightKeyframe );
	}
	else
	{
		// if no key selected then insert at end
		m_SunlightKeyframes.insert( m_SunlightKeyframes.end(), SunlightKeyframe );
		iSelected = m_SunlightKeyframes.size() - 1;
	}

	RebuildSunKeyList( iSelected );
}

void CTimeOfDayDialog::OnBnClickedFogAdd()
{
	// insert key
	FOGKEYFRAME FogKeyframe;
	FogKeyframe.m_FogColor.SetX(0);
	FogKeyframe.m_FogColor.SetY(0);
	FogKeyframe.m_FogColor.SetZ(0);
	FogKeyframe.m_fFogStart = 1.0;
	FogKeyframe.m_fFogEnd = 1000.0f;

	int iSelected = GetListSelectedItem( &m_FogKeyList );
	if (iSelected > -1)
	{
		m_FogKeyframes.insert( m_FogKeyframes.begin() + iSelected, FogKeyframe );
	}
	else
	{
		// if no key selected then insert at end
		m_FogKeyframes.insert( m_FogKeyframes.end(), FogKeyframe );
		iSelected = m_FogKeyframes.size() - 1;
	}

	RebuildFogKeyList( iSelected );
}

void CTimeOfDayDialog::OnBnClickedSunDel()
{
	// delete currently selected key
	int iSelected = GetListSelectedItem( &m_SunKeyframeList );
	if (iSelected > -1)
	{
		m_SunlightKeyframes.erase( m_SunlightKeyframes.begin() + iSelected );

		if (m_SunlightKeyframes.size() > 0)
		{
			if (iSelected == m_SunlightKeyframes.size())
				iSelected--;
		}
		else
			iSelected = -1;
	}

	RebuildSunKeyList( iSelected );
}

void CTimeOfDayDialog::OnBnClickedFogDel()
{
	// delete currently selected key
	int iSelected = GetListSelectedItem( &m_FogKeyList );
	if (iSelected > -1)
	{
		m_FogKeyframes.erase( m_FogKeyframes.begin() + iSelected );

		if (m_FogKeyframes.size() > 0)
		{
			if (iSelected == m_FogKeyframes.size())
				iSelected--;
		}
		else
			iSelected = -1;
	}

	RebuildFogKeyList( iSelected );
}

void CTimeOfDayDialog::OnBnClickedSunUp()
{
	// move key up the list
	int iSelected = GetListSelectedItem( &m_SunKeyframeList );
	if (iSelected > 0)
	{
		SUNLIGHTKEYFRAME SunlightKeyframeTemp;
		SunlightKeyframeTemp = m_SunlightKeyframes[iSelected-1];
		m_SunlightKeyframes[iSelected-1] = m_SunlightKeyframes[iSelected];
		m_SunlightKeyframes[iSelected] = SunlightKeyframeTemp;

		RebuildSunKeyList( iSelected - 1 );
	}
}

void CTimeOfDayDialog::OnBnClickedFogUp()
{
	// move key up the list
	int iSelected = GetListSelectedItem( &m_FogKeyList );
	if (iSelected > 0)
	{
		FOGKEYFRAME FogKeyframeTemp;
		FogKeyframeTemp = m_FogKeyframes[iSelected-1];
		m_FogKeyframes[iSelected-1] = m_FogKeyframes[iSelected];
		m_FogKeyframes[iSelected] = FogKeyframeTemp;

		RebuildFogKeyList( iSelected - 1);
	}
}

void CTimeOfDayDialog::OnBnClickedSunDn()
{
	// move key down the list
	int iSelected = GetListSelectedItem( &m_SunKeyframeList );
	if (iSelected > -1 && iSelected < ((int)m_SunlightKeyframes.size() - 1))
	{
		SUNLIGHTKEYFRAME SunlightKeyframeTemp;
		SunlightKeyframeTemp = m_SunlightKeyframes[iSelected+1];
		m_SunlightKeyframes[iSelected+1] = m_SunlightKeyframes[iSelected];
		m_SunlightKeyframes[iSelected] = SunlightKeyframeTemp;

		RebuildSunKeyList( iSelected + 1 );
	}
}

void CTimeOfDayDialog::OnBnClickedFogDn()
{
	// move key down the list
	int iSelected = GetListSelectedItem( &m_FogKeyList );
	if (iSelected > -1 && iSelected < ((int)m_FogKeyframes.size() - 1))
	{
		FOGKEYFRAME FogKeyframeTemp;
		FogKeyframeTemp = m_FogKeyframes[iSelected+1];
		m_FogKeyframes[iSelected+1] = m_FogKeyframes[iSelected];
		m_FogKeyframes[iSelected] = FogKeyframeTemp;

		RebuildFogKeyList( iSelected + 1);
	}
}

void CTimeOfDayDialog::PopulateFields()
{
	// setup sun and fog frames
	SetupFrames();

	// setup skybox textures
	SetupSkyBoxData();
}

void CTimeOfDayDialog::SetupFrames()
{
	VISITHIERARCHYDATA vhd;
	OBJECTLIST::iterator objIter;
	OBJECTLIST *objList = NULL;
	CREATEARCHIVE ca;
	IArchive *ar;
	SERIALIZEOBJECTPARAMS sop;
    CHashString memType(_T("Memory"));
	const DWORD MAXBUFFSIZE=16384;
    char memBuffer[MAXBUFFSIZE];
	CHashString objType(_T("CTimeOfDayObject"));
	int i;

	// set type filter
	m_todVisitor.SetTypeFilter(&objType);

	// setup visit hierarchy params
	vhd.name = NULL;
	vhd.visitor = &m_todVisitor;

	// Visit Hierarchy
	static DWORD msgHash_VisitHierarchy = CHashString(_T("VisitHierarchy")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_VisitHierarchy, sizeof(VISITHIERARCHYDATA), &vhd);

	// grab list of objects
	objList = m_todVisitor.GetObjList();

	// if there is something in the list
	if (objList->size() > 0)
	{
		// grab the head
		objIter = objList->begin();

		// get the name of the object
		m_TimeOfDayObjectName = (*objIter)->GetName();

		// create archive
		ca.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
		ca.streamData = memBuffer;
		ca.streamSize = MAXBUFFSIZE;
		ca.streamType = &memType;
		static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
		{
			// log warning
			StdString error;
			error = _T("Time Of Day : Frames - Couldn't create Archive.\n");
			EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
			EngineGetToolBox()->Log(LOGWARNING, error);
			return;
		}
		// get the created archive
		ar = ca.archive;

		// make sure archive starts in write mode
		ar->SetIsWriting(true);
		// serialize object
		sop.name = m_TimeOfDayObjectName;
		sop.archive = ar;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop);

		// set archive to reading mode
		ar->SetIsWriting(false);
		// reset archive
		ar->SeekTo(0);

		// read data from serialize
		ar->Read( m_fTimeStart, _T("TimeStart") );
		ar->Read( m_fTimeRate, _T("TimeRate") );
	
		// get number of sun and fog frames
		int iNumSunLightFrames = 0;
		ar->Read( iNumSunLightFrames, _T("NumSunLightFrames") );
		int iNumFogFrames = 0;
		ar->Read( iNumFogFrames, _T("NumFogFrames") );

		// MAKE SURE TO CLEAR THE OLD LISTS!!!!!
		m_SunlightKeyframes.clear();
		m_FogKeyframes.clear();

		// read sun frames
		for (i = 0; i < iNumSunLightFrames; ++i)
		{
			SUNLIGHTKEYFRAME tmpSunLightFrame;
			ar->Read( tmpSunLightFrame.m_wszName, _T("Name") );
			ar->Read( tmpSunLightFrame.m_AmbientColor, _T("AmbientColor") );
			ar->Read( tmpSunLightFrame.m_FullbrightColor, _T("FulLBrightColor") );
			ar->Read( tmpSunLightFrame.m_fDawnWeight, _T("DawnWeight") );
			ar->Read( tmpSunLightFrame.m_fNightWeight, _T("NightWeight") );
			m_SunlightKeyframes.push_back( tmpSunLightFrame );
		}

		// read fog frames
		for (i = 0; i < iNumFogFrames; ++i)
		{
			FOGKEYFRAME tmpFogFrame;
			ar->Read( tmpFogFrame.m_wszName, _T("Name") );
			ar->Read( tmpFogFrame.m_FogColor, _T("FogColor") );
			ar->Read( tmpFogFrame.m_fFogStart, _T("FogStart") );
			ar->Read( tmpFogFrame.m_fFogEnd, _T("FogEnd") );
			m_FogKeyframes.push_back( tmpFogFrame );
		}

		// rebuild lists
		RebuildSunKeyList(0);
		RebuildFogKeyList(0);
	}

	// clear visitor object list
	m_todVisitor.ClearObjList();
}

void CTimeOfDayDialog::SetupSkyBoxData()
{
	VISITHIERARCHYDATA vhd;
	OBJECTLIST::iterator objIter;
	OBJECTLIST *objList = NULL;
	CREATEARCHIVE ca;
	IArchive *ar;
	SERIALIZEOBJECTPARAMS sop;
    CHashString memType(_T("Memory"));
	const DWORD MAXBUFFSIZE=16384;
    char memBuffer[MAXBUFFSIZE];
	CHashString objType(_T("CSkybox"));
	int i;

	// set type filter
	m_todVisitor.SetTypeFilter(&objType);

	// setup visit hierarchy params
	vhd.name = NULL;
	vhd.visitor = &m_todVisitor;

	// Visit Hierarchy
	static DWORD msgHash_VisitHierarchy = CHashString(_T("VisitHierarchy")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_VisitHierarchy, sizeof(VISITHIERARCHYDATA), &vhd);

	// grab list of objects
	objList = m_todVisitor.GetObjList();

	// if there is something in the list
	if (objList->size() > 0)
	{
		// grab the head
		objIter = objList->begin();

		// get the name of the object
		m_SkyBoxObjectName = (*objIter)->GetName();

		// create archive
		ca.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
		ca.streamData = memBuffer;
		ca.streamSize = MAXBUFFSIZE;
		ca.streamType = &memType;
		static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
		{
			// log warning
			StdString error;
			error = _T("Time Of Day : Frames - Couldn't create Archive.\n");
			EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
			EngineGetToolBox()->Log(LOGWARNING, error);
		}
		// get the created archive
		ar = ca.archive;

		// make sure archive starts in write mode
		ar->SetIsWriting(true);
		// serialize object
		sop.name = m_SkyBoxObjectName;
		sop.archive = ar;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop);

		// set archive to reading mode
		ar->SetIsWriting(false);
		// reset archive
		ar->SeekTo(0);
	
		// MAKE SURE TO CLEAR THE OLD LIST!!!!!
		m_SkyLayers.clear();

		// get the number of layers
		int iNumSkyLayers = 0;
		ar->Read( iNumSkyLayers, _T("NumSkyLayers") );

		// read layers
		for (i = 0; i < iNumSkyLayers; ++i)
		{
			SKYLAYER tmpSkyLayer;
            ar->Read( tmpSkyLayer.m_wszName, _T("Name") );
			ar->Read( tmpSkyLayer.m_wszCubeRight, _T("CubeRight") );
			ar->Read( tmpSkyLayer.m_wszCubeLeft, _T("CubeLeft") );
			ar->Read( tmpSkyLayer.m_wszCubeFront, _T("CubeFront") );
			ar->Read( tmpSkyLayer.m_wszCubeBack, _T("CubeBack") );
			ar->Read( tmpSkyLayer.m_wszCubeTop, _T("CubeTop") );
			m_SkyLayers.push_back( tmpSkyLayer );
		}

		// update skybox fields
		UpdateSkyboxFields();
	}

	// clear visitor object list
	m_todVisitor.ClearObjList();
}

void CTimeOfDayDialog::ExportFrames()
{
	CREATEARCHIVE ca;
	IArchive *ar;
	SERIALIZEOBJECTPARAMS sop;
	INITOBJECTPARAMS iop;
    CHashString memType(_T("Memory"));
	const DWORD MAXBUFFSIZE=16384;
    char memBuffer[MAXBUFFSIZE];
	DWORD i;

	// if there is something to export
	if (m_TimeOfDayObjectName != NULL)
	{
		// create archive
		ca.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
		ca.streamData = memBuffer;
		ca.streamSize = MAXBUFFSIZE;
		ca.streamType = &memType;
		static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
		{
			return;
		}
		// get the created archive
		ar = ca.archive;

		// make sure archive starts in write mode
		ar->SetIsWriting(true);

		// write start and rate
		ar->Write( m_fTimeStart, _T("TimeStart") );
		ar->Write( m_fTimeRate, _T("TimeRate") );
	
		// write number of sun and fog frames
		ar->Write( m_SunlightKeyframes.size(), _T("NumSunLightFrames") );
		ar->Write( m_FogKeyframes.size(), _T("NumFogFrames") );

		// write sun frames
		for (i = 0; i < m_SunlightKeyframes.size(); ++i)
		{
			ar->StartClass( _T("SunLightFrame") );
			ar->Write( m_SunlightKeyframes[i].m_wszName, _T("Name") );
			ar->Write( m_SunlightKeyframes[i].m_AmbientColor, _T("AmbientColor") );
			ar->Write( m_SunlightKeyframes[i].m_FullbrightColor, _T("FulLBrightColor") );
			ar->Write( m_SunlightKeyframes[i].m_fDawnWeight, _T("DawnWeight") );
			ar->Write( m_SunlightKeyframes[i].m_fNightWeight, _T("NightWeight") );
			ar->EndClass();
		}

		// write fog frames
		for (i = 0; i < m_FogKeyframes.size(); ++i)
		{
			ar->StartClass( _T("FogFrame") );
			ar->Write( m_FogKeyframes[i].m_wszName, _T("Name") );
			ar->Write( m_FogKeyframes[i].m_FogColor, _T("FogColor") );
			ar->Write( m_FogKeyframes[i].m_fFogStart, _T("FogStart") );
			ar->Write( m_FogKeyframes[i].m_fFogEnd, _T("FogEnd") );
			ar->EndClass();
		}

		// no longer writing, switch to read
		ar->SetIsWriting(false);
		// reset archive
		ar->SeekTo(0);
		// serialize object
		sop.name = m_TimeOfDayObjectName;
		sop.archive = ar;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop);

		// init the object
		iop.name = m_TimeOfDayObjectName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_InitObject, sizeof(iop), &iop);
	}

	// reset time of day object name (in case it goes away before getting here again)
	m_TimeOfDayObjectName = NULL;
}

void CTimeOfDayDialog::ExportSkyboxData()
{
	CREATEARCHIVE ca;
	IArchive *ar;
	SERIALIZEOBJECTPARAMS sop;
	INITOBJECTPARAMS iop;
    CHashString memType(_T("Memory"));
	const DWORD MAXBUFFSIZE=16384;
    char memBuffer[MAXBUFFSIZE];
	DWORD i;

	// if we have data to export
	if (m_SkyBoxObjectName != NULL)
	{
		// Get Skybox data from edit fields
		GetSkyboxData();

		// create archive
		ca.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
		ca.streamData = memBuffer;
		ca.streamSize = MAXBUFFSIZE;
		ca.streamType = &memType;
		static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
		{
			return;
		}
		// get the created archive
		ar = ca.archive;

		// make sure archive starts in write mode
		ar->SetIsWriting(true);

		ar->Write( m_SkyLayers.size(), _T("NumSkyLayers") );

		for (i = 0; i < m_SkyLayers.size(); ++i)
		{
			ar->StartClass( _T("SkyLayer") );
			ar->Write( m_SkyLayers[i].m_wszName, _T("Name") );
			ar->Write( m_SkyLayers[i].m_wszCubeRight, _T("CubeRight") );
			ar->Write( m_SkyLayers[i].m_wszCubeLeft, _T("CubeLeft") );
			ar->Write( m_SkyLayers[i].m_wszCubeFront, _T("CubeFront") );
			ar->Write( m_SkyLayers[i].m_wszCubeBack, _T("CubeBack") );
			ar->Write( m_SkyLayers[i].m_wszCubeTop, _T("CubeTop") );
			ar->EndClass();
		}

		// no longer writing, switch to read
		ar->SetIsWriting(false);
		// reset archive
		ar->SeekTo(0);
		// serialize object
		sop.name = m_SkyBoxObjectName;
		sop.archive = ar;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop);

		// init the object
		iop.name = m_SkyBoxObjectName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_InitObject, sizeof(iop), &iop);
	}

	// reset skybox name (in case it goes away before getting here again)
	m_SkyBoxObjectName = NULL;
}

void CTimeOfDayDialog::UpdateSkyboxFields()
{
	DWORD i;
	DWORD skyLayer = 0;

	// update the vector of sky box textures
	for(i = 0; i < m_SkyLayers.size(); ++i)
	{
		m_vszSkyTextures[skyLayer++] = m_SkyLayers[i].m_wszCubeRight.c_str();
		m_vszSkyTextures[skyLayer++] = m_SkyLayers[i].m_wszCubeLeft.c_str();
		m_vszSkyTextures[skyLayer++] = m_SkyLayers[i].m_wszCubeFront.c_str();
		m_vszSkyTextures[skyLayer++] = m_SkyLayers[i].m_wszCubeBack.c_str();
		m_vszSkyTextures[skyLayer++] = m_SkyLayers[i].m_wszCubeTop.c_str();
	}
	
	UpdateData(FALSE);
}

void CTimeOfDayDialog::GetSkyboxData()
{
	DWORD i;
	DWORD skyLayer = 0;

	// update the vector of sky box textures
	for(i = 0; i < m_SkyLayers.size(); ++i)
	{
		m_SkyLayers[i].m_wszCubeRight = m_vszSkyTextures[skyLayer++];
		m_SkyLayers[i].m_wszCubeLeft = m_vszSkyTextures[skyLayer++];
		m_SkyLayers[i].m_wszCubeFront = m_vszSkyTextures[skyLayer++];
		m_SkyLayers[i].m_wszCubeBack = m_vszSkyTextures[skyLayer++];
		m_SkyLayers[i].m_wszCubeTop = m_vszSkyTextures[skyLayer++];
	}
}

