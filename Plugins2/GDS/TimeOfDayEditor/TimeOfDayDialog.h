///=====================================================================
/// \file	TimeOfDayDialog.h
/// \brief	Definition of TimeOfDayDialog Class
///         These classes implements a MFC Dialog Box
/// \date	02/15/2005
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

#ifndef _TIMEOFDAYDIALOG_H_
#define _TIMEOFDAYDIALOG_H_

struct SKYLAYER
{
	StdString m_wszName;
	StdString m_wszCubeRight;
	StdString m_wszCubeLeft;
	StdString m_wszCubeFront;
	StdString m_wszCubeBack;
	StdString m_wszCubeTop;
};
typedef std::vector<SKYLAYER> SKYLAYERLIST;

/// \class TimeOfDayDialog
/// \brief  Creates the TimeOfDay Dialog MFC Box
///			Inherits from CDialog
class CTimeOfDayDialog : public CDialog
{
	DECLARE_DYNAMIC(CTimeOfDayDialog)

public:
	/// \fn void TimeOfDayDialog()
	/// \brief Default Constructor
	/// \param *none*
	/// \return *none*
	CTimeOfDayDialog(CWnd* pParent = NULL);   // standard constructor

	/// \fn void ~TimeOfDayDialog()
	/// \brief Default Destructor
	/// \param *none*
	/// \return *none*
	virtual ~CTimeOfDayDialog();

	BOOL OnInitDialog();

	/// Populate Fields of the Dialog
	/// \param void
	/// \return void
	void PopulateFields();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	/// \fn void RebuildSunKeyList( int iSelectedItem )
	/// \brief Rebuilds only dialog's key list from internal state
	/// \param int iSelectedItem - item to have selected after rebuilding of list
	void RebuildSunKeyList( int iSelectedItem );

	/// \fn void RebuildFogKeyList( int iSelectedItem )
	/// \brief Rebuilds only dialog's key list from internal state
	/// \param int iSelectedItem - item to have selected after rebuilding of list
	void RebuildFogKeyList( int iSelectedItem );

	/// Rebuild Sky box textures
	/// \param rebuilds skybox texture fields from internal state
	/// \return void
	void UpdateSkyboxFields();


	/// \fn void UpdateDialog()
	/// \brief Rebuilds all dialog data from internal state
	void UpdateDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedImport();
	afx_msg void OnBnClickedExport();
	void OnBnClickedSkyLayer( int iFaceIndex );
	afx_msg void OnBnClickedSkyLayer0()  { OnBnClickedSkyLayer( 0 ); }
	afx_msg void OnBnClickedSkyLayer1()  { OnBnClickedSkyLayer( 1 ); }
	afx_msg void OnBnClickedSkyLayer2()  { OnBnClickedSkyLayer( 2 ); }
	afx_msg void OnBnClickedSkyLayer3()  { OnBnClickedSkyLayer( 3 ); }
	afx_msg void OnBnClickedSkyLayer4()  { OnBnClickedSkyLayer( 4 ); }
	afx_msg void OnBnClickedSkyLayer5()  { OnBnClickedSkyLayer( 5 ); }
	afx_msg void OnBnClickedSkyLayer6()  { OnBnClickedSkyLayer( 6 ); }
	afx_msg void OnBnClickedSkyLayer7()  { OnBnClickedSkyLayer( 7 ); }
	afx_msg void OnBnClickedSkyLayer8()  { OnBnClickedSkyLayer( 8 ); }
	afx_msg void OnBnClickedSkyLayer9()  { OnBnClickedSkyLayer( 9 ); }
	afx_msg void OnBnClickedSkyLayer10() { OnBnClickedSkyLayer( 10 ); }
	afx_msg void OnBnClickedSkyLayer11() { OnBnClickedSkyLayer( 11 ); }
	afx_msg void OnBnClickedSkyLayer12() { OnBnClickedSkyLayer( 12 ); }
	afx_msg void OnBnClickedSkyLayer13() { OnBnClickedSkyLayer( 13 ); }
	afx_msg void OnBnClickedSkyLayer14() { OnBnClickedSkyLayer( 14 ); }
	afx_msg void OnDblClkListSun(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblClkListFog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSunAdd();
	afx_msg void OnBnClickedSunDel();
	afx_msg void OnBnClickedFogAdd();
	afx_msg void OnBnClickedFogDel();
	afx_msg void OnBnClickedSunUp();
	afx_msg void OnBnClickedSunDn();
	afx_msg void OnBnClickedFogUp();
	afx_msg void OnBnClickedFogDn();

private:
	/// Setup fog and sun keyframes in the dialog
	/// \param void
	/// \return void
	void SetupFrames();

	/// Setup Skybox Data in the dialog
	/// \param void
	/// \return void
	void SetupSkyBoxData();

	/// Write out new frame data to CTimeOfDayObject
	/// \param void
	/// \return void
	void ExportFrames();

	/// Write out new sky box day to CSkybox object
	/// \param void
	/// \return void
	void ExportSkyboxData();

	/// Retrieve skybox data from edit fields
	/// \param void
	/// \return void
	void GetSkyboxData();

	// Dialog Controls
	CListCtrl m_SunKeyframeList;
	CListCtrl m_FogKeyList;

	/// Sunlight Key Frames
	SUNLIGHTKEYFRAMELIST m_SunlightKeyframes;
	/// Fog Key Frames
	FOGKEYFRAMELIST m_FogKeyframes;
	/// Sky Layers
	SKYLAYERLIST m_SkyLayers;
	CString m_vszSkyTextures[15];

	static char *m_vSkyLayerNames[];
	static char *m_vSkyLayerCubeNames[];

	IToolBox *m_ToolBox;

	/// Time of Day Visitor
	CTimeOfDayVisitor m_todVisitor;
	/// Name of Time of Day Object
	IHashString *m_TimeOfDayObjectName;
	/// Name of Sky box Object
	IHashString *m_SkyBoxObjectName;
	/// Start Time
	float m_fTimeStart;
	/// Time Rate
	float m_fTimeRate;
};

#endif	// #ifndef _TIMEOFDAYDIALOG_H_
