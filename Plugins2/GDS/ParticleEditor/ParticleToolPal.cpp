// ParticleToolPal.cpp : implementation file
//

#include "stdafx.h"
#include "ParticleEditor.h"
#include "ParticleToolPal.h"


// CParticleToolPal dialog

IMPLEMENT_DYNAMIC(CParticleToolPal, CGuiDialogBar)
CParticleToolPal::CParticleToolPal()
{
	m_ToolBox = EngineGetToolBox();
}

CParticleToolPal::~CParticleToolPal()
{
}


BEGIN_MESSAGE_MAP(CParticleToolPal, CGuiDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
	ON_WM_HSCROLL()	
//	ON_BN_CLICKED(IDC_PARTICLE_SHOW, OnBnClickedShowToggle)
//	ON_BN_CLICKED(IDC_PARTICLE_PROPERTIES, OnBnClickedProperties )
//	ON_BN_CLICKED(IDC_PARTICLE_RESET, OnBnClickedReset )
//	ON_UPDATE_COMMAND_UI(IDC_PARTICLE_SAVE, OnUpdateButtons)
//	ON_UPDATE_COMMAND_UI(IDC_PARTICLE_LOAD, OnUpdateButtons)
//	ON_UPDATE_COMMAND_UI(IDC_PARTICLE_PROPERTIES, OnUpdateButtons)
//	ON_UPDATE_COMMAND_UI(IDC_PARTICLE_TEXTURE, OnUpdateButtons)
//	ON_UPDATE_COMMAND_UI(IDC_PARTICLE_SHADER, OnUpdateButtons)
//	ON_UPDATE_COMMAND_UI(IDC_PARTICLE_REFRESH, OnUpdateButtons)
//	ON_UPDATE_COMMAND_UI(IDC_PARTICLE_RESET, OnUpdateButtons)
END_MESSAGE_MAP()


void CParticleToolPal::DoDataExchange(CDataExchange* pDX)
{
	
//	DDX_Control( pDX, IDC_PARTICLE_SHOW, m_ctrlShowToggleButton );

	CGuiDialogBar::DoDataExchange(pDX);
}

void CParticleToolPal::OnGlobalUpdateTick( void )
{
}


void CParticleToolPal::CreateParticleSystem()
{
	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	SERIALIZEOBJECTPARAMS sop;
	
	CHashString hszParentName(_T("World"));
	CHashString hszTypeName(_T("CParticleRenderObject"));
	
	cop.parentName = &hszParentName;
	cop.typeName = &hszTypeName;
	cop.name = &m_ParticleName;
	DWORD retval = m_ToolBox->SendMessage(_T("CreateObject"), sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);

	IArchive *MemArchive;
	CHashString memType(_T("Memory"));
	int size =	sizeof(char) * 256 + sizeof(Vec3) * 3 + 256;
	char* memchunk = new char[size];
	memset( memchunk, 0, size );
				
	CREATEARCHIVE caOut;
	caOut.mode = STREAM_MODE_WRITE;
	caOut.streamData = memchunk;
	caOut.streamSize = size;
	caOut.streamType = &memType;
	if (m_ToolBox->SendMessage(_T("CreateArchive"), sizeof(CREATEARCHIVE), &caOut) != MSG_HANDLED)
	{
		return;
	}
	MemArchive = caOut.archive;
	Vec3 vec(0,0,0);
	int clip = 30000;
	float life = 300000;
	bool enabled = true;
	bool screenaligned = true;
	MemArchive->Write( _T("Particles/Fire.xml"), _T("FileName") );
	MemArchive->Write( vec, _T("Position"));
	MemArchive->Write( vec, _T("Rotation") );
	MemArchive->Write( vec, _T("Scaling") );		
	MemArchive->Write( enabled , _T("Enabled"));
	MemArchive->Write( clip, _T("ClipDistance"));
	MemArchive->Write( life, _T("RemaingLife") );
	MemArchive->Write( screenaligned, _T("ScreenAligned") );

	CREATESTREAM cs;
	cs.streamData = caOut.streamData;
	cs.streamSize = caOut.streamSize;
	cs.mode = STREAM_MODE_READ;
	if (m_ToolBox->SendMessage(_T("CreateStream_Memory"), sizeof(CREATESTREAM), &cs) != MSG_HANDLED)
	{
		return;
	}
	MemArchive->Init(cs.openStream);

	sop.name = &m_ParticleName;
	sop.archive = MemArchive;
	m_ToolBox->SendMessage(_T("SerializeObject"), sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);
	

	iop.name = &m_ParticleName;
	if( retval == MSG_HANDLED )
	{
		retval = m_ToolBox->SendMessage(_T("InitObject"), sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);
	}
	if( retval == MSG_HANDLED )
	{
		retval = m_ToolBox->SendMessage(_T("Enable"), 0, 0, &m_ParticleName, NULL);
	}

	if( retval == MSG_HANDLED )
	{
		m_ParticleVisible = true;
	}
	return;
}

void CParticleToolPal::RemoveParticleSystem()
{
	DELETEOBJECTPARAMS dop;
	dop.name = &m_ParticleName;
	DWORD retval = m_ToolBox->SendMessage( _T("DeleteObject"), sizeof( DELETEOBJECTPARAMS ), &dop, NULL, NULL );
	if( retval == MSG_HANDLED )
	{
		m_ParticleVisible = false;
	}
}

LRESULT CParticleToolPal::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = HandleInitDialog(wParam, lParam);
	m_ParticleName.Init( _T("CParticleToolPal_Temp") );
	m_ParticleVisible = false;
    UpdateData(FALSE);
	m_ctrlShowToggleButton.SetCheck( BST_UNCHECKED );

	//Create our editing dialogs
//	m_PropertiesDlg.Create(IDD_PARTICLE_PROPERTIES_DIALOG, AfxGetApp()->GetMainWnd() );	
//	m_PropertiesDlg.SetParticleName( &m_ParticleName );
//	m_PropertiesDlg.Initialize();
	return bRet;
}
void CParticleToolPal::OnUpdateButtons( CCmdUI* pCmdUI )
{
	// 
}
// CParticleToolPal message handlers
void CParticleToolPal::OnBnClickedShowToggle()
{
	bool bshow = (m_ctrlShowToggleButton.GetCheck() == BST_CHECKED );
	ToggleParticleSystem( bshow );
}

void CParticleToolPal::ToggleParticleSystem( bool bshow )
{
	if( m_ParticleVisible && !bshow )
	{
		RemoveParticleSystem();
	}
	else if( !m_ParticleVisible && bshow )
	{
		m_ToolBox->SendMessage(_T("InitParticles"),0,NULL);
		CreateParticleSystem();
		m_PropertiesDlg.UpdateParticleObject();
	}
}

void CParticleToolPal::OnBnClickedProperties()
{
	//start the properties dialog SW_HIDE
	m_PropertiesDlg.ShowWindow( SW_SHOWNORMAL );
	m_PropertiesDlg.SetActiveWindow();
	m_PropertiesDlg.BringWindowToTop();
}

void CParticleToolPal::OnBnClickedReset()
{
	//reload particles
	m_ToolBox->SendMessage(_T("ReloadAllParticles"),0,NULL);
		

}