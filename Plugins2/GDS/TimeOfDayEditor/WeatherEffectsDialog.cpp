///============================================================================
/// \file		WeatherEffectsDialog.cpp
/// \brief		Implmentation for CWeatherEffectsDialog 
/// \date		09-27-2005
/// \author		Josh Lulewicz
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

#include "stdafx.h"
#include ".\weathereffectsdialog.h"

// CWeatherEffectsDialog dialog

IMPLEMENT_DYNAMIC(CWeatherEffectsDialog, CDialog)
CWeatherEffectsDialog::CWeatherEffectsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CWeatherEffectsDialog::IDD, pParent)
{
	m_ToolBox = EngineGetToolBox();
	m_WeatherEffectObjectName = NULL;
	// init to default
	m_iEffectIndex = 2;
	m_fIntensity = 0.0f;
	m_fParticleScale = 0.0f;
}

CWeatherEffectsDialog::~CWeatherEffectsDialog()
{
}

void CWeatherEffectsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_WEATHEREFFECT, m_EffectCombo);
	DDX_CBIndex(pDX, IDC_COMBO_WEATHEREFFECT, m_iEffectIndex);
	DDX_Control(pDX, IDC_EDIT_INTENSITY, m_IntensityEdit);
	DDX_Control(pDX, IDC_EDIT_PARTICLESCALE, m_ParticleScaleEdit);
}


BEGIN_MESSAGE_MAP(CWeatherEffectsDialog, CDialog)
	ON_EN_KILLFOCUS(IDC_EDIT_INTENSITY, OnEnKillfocusEditIntensity)
	ON_EN_KILLFOCUS(IDC_EDIT_PARTICLESCALE, OnEnKillfocusEditParticlescale)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_WEATHEREFFECT, OnCbnSelchangeComboWeathereffect)
END_MESSAGE_MAP()


// CWeatherEffectsDialog message handlers
BOOL CWeatherEffectsDialog::OnInitDialog()
{
	CString temp;
	CDialog::OnInitDialog();

	temp.LoadString(IDS_RAIN_EFFECT);
	m_EffectCombo.AddString(temp);
	temp.LoadString(IDS_SNOW_EFFECT);
	m_EffectCombo.AddString(temp);
	temp.LoadString(IDS_DEFAULT_EFFECT);
	m_EffectCombo.AddString(temp);

	m_EffectCombo.SetCurSel(m_iEffectIndex);

	UpdateDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWeatherEffectsDialog::UpdateDialog()
{
	TCHAR tempNum[256];

	// write values into edit boxes
	_stprintf(tempNum, _T("%-.5f"), m_fIntensity);
	m_IntensityEdit.SetWindowText(tempNum);
	_stprintf(tempNum, _T("%-.5f"), m_fParticleScale);
	m_ParticleScaleEdit.SetWindowText(tempNum);

	UpdateData(FALSE);
}

void CWeatherEffectsDialog::PopulateFields()
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
	CHashString objType(_T("CWeatherRenderObject"));
	/// Effect
	StdString szEffect;

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
		m_WeatherEffectObjectName = (*objIter)->GetName();

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
		sop.name = m_WeatherEffectObjectName;
		sop.archive = ar;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop);

		// set archive to reading mode
		ar->SetIsWriting(false);
		// reset archive
		ar->SeekTo(0);

		// read data from serialize
		ar->Read( m_fIntensity, _T("Intensity") );	
		ar->Read( m_fParticleScale, _T("ParticleScale") );	
		ar->Read( szEffect, _T("Effect") );
		if( szEffect == "RAIN_WEATHER" )
		{
			m_iEffectIndex = RAIN_WEATHER;
		}
		else if( szEffect == "SNOW_WEATHER" )
		{
			m_iEffectIndex = SNOW_WEATHER;
		}
		else
		{
			m_iEffectIndex = DEFAULT_WEATHER;
		}

		UpdateDialog();
	}

	// clear visitor object list
	m_todVisitor.ClearObjList();
}

void CWeatherEffectsDialog::OnEnKillfocusEditIntensity()
{
	// TODO: Add your control notification handler code here
	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	m_IntensityEdit.GetWindowText( szBuffer, 256 );
	m_fIntensity = (float)atof( StdString(szBuffer) );
	if( m_fIntensity < 0.0f )
	{
		m_fIntensity = 0.0f;
		m_IntensityEdit.SetWindowText( _T("0.0") );
	}
	else if( m_fIntensity > 1.0f )
	{
		m_fIntensity = 1.0f;
		m_IntensityEdit.SetWindowText( _T("1.0") );
	}
}

void CWeatherEffectsDialog::OnEnKillfocusEditParticlescale()
{
	// TODO: Add your control notification handler code here
	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	m_ParticleScaleEdit.GetWindowText( szBuffer, 256 );
	m_fParticleScale = (float)atof( StdString(szBuffer) );
	if( m_fParticleScale < 0.0f )
	{
		m_fParticleScale = 0.0f;
		m_ParticleScaleEdit.SetWindowText( _T("0.0") );
	}
	else if( m_fParticleScale > 1.0f )
	{
		m_fParticleScale = 1.0f;
		m_ParticleScaleEdit.SetWindowText( _T("1.0") );
	}
}

void CWeatherEffectsDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	ExportData();
	OnOK();
}

void CWeatherEffectsDialog::ExportData()
{
	CREATEARCHIVE ca;
	IArchive *ar;
	SERIALIZEOBJECTPARAMS sop;
	INITOBJECTPARAMS iop;
    CHashString memType(_T("Memory"));
	const DWORD MAXBUFFSIZE=16384;
    char memBuffer[MAXBUFFSIZE];
	/// Effect
	StdString szEffect;

	// if we have data to export
	if (m_WeatherEffectObjectName != NULL)
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

		if( m_iEffectIndex == RAIN_WEATHER )
		{
			szEffect = "RAIN_WEATHER";
		}
		else if( m_iEffectIndex == SNOW_WEATHER )
		{
			szEffect = "SNOW_WEATHER";
		}
		else
		{
			szEffect = "DEFAULT_WEATHER";
		}
		ar->Write( m_fIntensity, _T("Intensity") );	
		ar->Write( m_fParticleScale, _T("ParticleScale") );	
		ar->Write( szEffect, _T("Effect") );		

		// no longer writing, switch to read
		ar->SetIsWriting(false);
		// reset archive
		ar->SeekTo(0);
		// serialize object
		sop.name = m_WeatherEffectObjectName;
		sop.archive = ar;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop);

		// init the object
		iop.name = m_WeatherEffectObjectName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_InitObject, sizeof(iop), &iop);
	}

	// reset weather object name (in case it goes away before getting here again)
	m_WeatherEffectObjectName = NULL;
}

void CWeatherEffectsDialog::OnCbnSelchangeComboWeathereffect()
{
	// TODO: Add your control notification handler code here
	// is this needed?  Why don't other combo boxes do this?!
	m_iEffectIndex = m_EffectCombo.GetCurSel();

	// if default is selected
	if (m_iEffectIndex == DEFAULT_WEATHER)
	{
		// disable edit boxes
		m_IntensityEdit.EnableWindow(FALSE);
		m_ParticleScaleEdit.EnableWindow(FALSE);
	}
	else
	{
		// otherwise enable the edit boxes
		m_IntensityEdit.EnableWindow();
		m_ParticleScaleEdit.EnableWindow();
	}


}
