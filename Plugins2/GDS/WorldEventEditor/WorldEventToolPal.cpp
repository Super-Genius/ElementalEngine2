///============================================================================
/// \file		WorldEventToolPal.cpp
/// \brief		Implementation of World Event Editor Tool Palatte
/// \date		05-01-2007
/// \author		D. Patrick Ghiocel
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CWorldEventToolPal, CGuiDialogBar)

CWorldEventToolPal::CWorldEventToolPal()
{
	m_ToolBox = EngineGetToolBox();
	m_pEditor = SINGLETONINSTANCE(CWorldEventEditor);
}

CWorldEventToolPal::~CWorldEventToolPal()
{

}

void CWorldEventToolPal::OnGlobalUpdateTick( void )
{
	
}

BEGIN_MESSAGE_MAP(CWorldEventToolPal, CGuiDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_WET_Activate, OnBnClickedWETActivate)
	ON_BN_CLICKED(IDC_WE_Activate, OnBnClickedWEActivate)
	ON_BN_CLICKED(IDC_Create, OnBnClickedCreate)
	ON_UPDATE_COMMAND_UI(IDC_Create, OnUpdateCreate)
	ON_CBN_DROPDOWN(IDC_TriggerName, OnCbnDropdownTriggerName)
	ON_CBN_DROPDOWN(IDC_Name, OnCbnDropdownName)
	ON_CBN_SELCHANGE(IDC_Name, OnCbnSelchangeName)
	ON_CBN_DROPDOWN(IDC_EventName, OnCbnDropdownEventname)
	ON_BN_CLICKED(IDC_CHECK_VISUALIZATION, OnBnClickedCheckVisualization)
END_MESSAGE_MAP()


void CWorldEventToolPal::DoDataExchange(CDataExchange* pDX)
{
	CGuiDialogBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_Name,				m_Name);
	DDX_Control(pDX, IDC_TriggerName,		m_TriggerName);
	DDX_Control(pDX, IDC_EntityName,		m_EntityName);
	DDX_Control(pDX, IDC_EntityType,		m_EntityType);
	DDX_Control(pDX, IDC_EventName,			m_EventName);
	DDX_Control(pDX, IDC_EventParam,		m_EventParam);
	DDX_Control(pDX, IDC_Create,			m_Create);
	DDX_Control(pDX, IDC_WET_Activate,		m_WET_Activate);
	DDX_Control(pDX, IDC_WE_Activate,		m_WE_Activate);
	DDX_Control(pDX, IDC_CHECK_VISUALIZATION, m_CheckVisualization);
}

LRESULT CWorldEventToolPal::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = HandleInitDialog(wParam, lParam);

	UpdateData(FALSE);

	m_Name.SetWindowText( _T("<Name>") );
	m_TriggerName.SetWindowText( _T("<Trigger Name>") );
	m_EntityName.SetWindowText( _T("<Entity Name>") );
	m_EntityType.SetWindowText( _T("<Entity Type>") );
	m_EventName.SetWindowText( _T("<Event Name>") );
	m_EventParam.SetWindowText( _T("<Event Params>") );
	m_Create.SetButtonStyle( BS_PUSHBUTTON, true );
	 
	return bRet;
}

void CWorldEventToolPal::OnEnKillfocusTimerateEdit()
{
	
}

void CWorldEventToolPal::OnBnClickedWETActivate()
{
	m_TriggerName.EnableWindow(false);
	m_EventParam.EnableWindow(false);
}

void CWorldEventToolPal::OnBnClickedWEActivate()
{
	m_TriggerName.EnableWindow(true);
	m_EventParam.EnableWindow(true);
}

void CWorldEventToolPal::OnBnClickedCreate()
{
	if(m_WET_Activate.GetState() == 1)
	{
		WorldEventTriggerCreate();
	}
	else if( m_WE_Activate.GetState() == 1)
	{
		WorldEventCreate();
	}
}

void CWorldEventToolPal::WorldEventTriggerCreate()
{
	static DWORD msgHash_GetComponentType = CHashString(_T("GetComponentType")).GetUniqueID();
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	static DWORD msgHash_AddObject = CHashString(_T("AddObject")).GetUniqueID();
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();

	static CHashString Hash_ParentName = CHashString(_T("World"));
	static CHashString Hash_Type = CHashString(_T("CWorldEventTrigger"));

	CString buf;
	m_Name.GetLBText(m_Name.GetCurSel(), buf);
	CHashString hszName((TCHAR*)buf.GetBuffer());
	FINDOBJECTPARAMS param;
	param.hszName = &hszName;
	DWORD result = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param);

	// world event exists, must update it
	if( param.bFound == true )
	{
		GETCOMPONENTTYPEPARAMS gctp;
		gctp.name = &hszName;
		m_ToolBox->SendMessage(msgHash_GetComponentType, sizeof(gctp), &gctp);

		if ( (gctp.componenttype != NULL) && (gctp.componenttype->GetUniqueID() == Hash_Type.GetUniqueID()) )
		{
			CREATEARCHIVE ca;
			CHashString streamType(_T("Memory"));
			ca.streamData = NULL;
			ca.streamSize = 0;
			ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
			ca.streamType = &streamType;
	 	
			m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca, NULL, NULL);
			IArchive *MemArchive = (ca.archive);

			m_EntityName.GetWindowText( buf );
			MemArchive->Write( (TCHAR*)buf.GetBuffer() );
			m_EntityType.GetWindowText( buf );
			MemArchive->Write( (TCHAR*)buf.GetBuffer() );
			m_EventName.GetWindowText( buf );
			MemArchive->Write( (TCHAR*)buf.GetBuffer() );
			
			SERIALIZEOBJECTPARAMS sop;
			sop.name = &hszName;
			sop.archive = MemArchive;
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);
		}
	}

	// world event trigger doesn't exist, must create it
	else
	{
		CREATEOBJECTPARAMS cop;
		cop.parentName = &Hash_ParentName;
		cop.typeName = &Hash_Type;
		cop.name = &hszName;
 		m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
		
		m_ToolBox->SendMessage(msgHash_AddObject, sizeof(cop), &cop);

		CREATEARCHIVE ca;
		CHashString streamType(_T("Memory"));
		ca.streamData = NULL;
		ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
		ca.streamType = &streamType;
 	
		m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca, NULL, NULL);
		IArchive *MemArchive = (ca.archive);

		m_EntityName.GetWindowText( buf );
		MemArchive->Write( (TCHAR*)buf.GetBuffer() );
		m_EntityType.GetWindowText( buf );
		MemArchive->Write( (TCHAR*)buf.GetBuffer() );
		m_EventName.GetWindowText( buf );
		MemArchive->Write( (TCHAR*)buf.GetBuffer() );
		
		SERIALIZEOBJECTPARAMS sop;
		sop.name = &hszName;
		sop.archive = MemArchive;
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);

		INITOBJECTPARAMS iop;
		iop.name = &hszName;
		m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);
	}
}

void CWorldEventToolPal::WorldEventCreate()
{
	static DWORD msgHash_GetComponentType = CHashString(_T("GetComponentType")).GetUniqueID();
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	static DWORD msgHash_AddObject = CHashString(_T("AddObject")).GetUniqueID();
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();

	static CHashString Hash_ParentName = CHashString(_T("World"));
	static CHashString Hash_Type = CHashString(_T("CWorldEvent"));

	CString buf;
	m_Name.GetLBText(m_Name.GetCurSel(), buf);
	CHashString hszName((TCHAR*)buf.GetBuffer());
	FINDOBJECTPARAMS param;
	param.hszName = &hszName;
	DWORD result = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param);

	// world event exists, must update it
	if( param.bFound == true )
	{
		GETCOMPONENTTYPEPARAMS gctp;
		gctp.name = &hszName;
		m_ToolBox->SendMessage(msgHash_GetComponentType, sizeof(gctp), &gctp);

		if ( (gctp.componenttype != NULL) && (gctp.componenttype->GetUniqueID() == Hash_Type.GetUniqueID()) )
		{
			CREATEARCHIVE ca;
			CHashString streamType(_T("Memory"));
			ca.streamData = NULL;
			ca.streamSize = 0;
			ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
			ca.streamType = &streamType;
	 	
			m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca, NULL, NULL);
			IArchive *MemArchive = (ca.archive);

			m_TriggerName.GetWindowText( buf );
			MemArchive->Write( (TCHAR*)buf.GetBuffer() );
			m_EntityName.GetWindowText( buf );
			MemArchive->Write( (TCHAR*)buf.GetBuffer() );
			m_EntityType.GetWindowText( buf );
			MemArchive->Write( (TCHAR*)buf.GetBuffer() );
			m_EventName.GetWindowText( buf );
			MemArchive->Write( (TCHAR*)buf.GetBuffer() );
			m_EventParam.GetWindowText( buf );
			MemArchive->Write( (TCHAR*)buf.GetBuffer() );

			SERIALIZEOBJECTPARAMS sop;
			sop.name = &hszName;
			sop.archive = MemArchive;
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);
		}
	}

	// world event doesn't exist, must create it
	else
	{
		CREATEOBJECTPARAMS cop;
		cop.parentName = &Hash_ParentName;
		cop.typeName = &Hash_Type;
		cop.name = &hszName;
 		m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);
        		
		m_ToolBox->SendMessage(msgHash_AddObject, sizeof(cop), &cop, NULL, NULL);

		CREATEARCHIVE ca;
		CHashString streamType(_T("Memory"));
		ca.streamData = NULL;
		ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
		ca.streamType = &streamType;
 	
		m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca, NULL, NULL);
		IArchive *MemArchive = (ca.archive);

		m_TriggerName.GetWindowText( buf );
		MemArchive->Write( (TCHAR*)buf.GetBuffer() );
		m_EntityName.GetWindowText( buf );
		MemArchive->Write( (TCHAR*)buf.GetBuffer() );
		m_EntityType.GetWindowText( buf );
		MemArchive->Write( (TCHAR*)buf.GetBuffer() );
		m_EventName.GetWindowText( buf );
		MemArchive->Write( (TCHAR*)buf.GetBuffer() );
		m_EventParam.GetWindowText( buf );
		MemArchive->Write( (TCHAR*)buf.GetBuffer() );

		SERIALIZEOBJECTPARAMS sop;
		sop.name = &hszName;
		sop.archive = MemArchive;
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);

		INITOBJECTPARAMS iop;
		iop.name = &hszName;
		m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);
	}
}

void CWorldEventToolPal::OnCbnDropdownTriggerName()
{
	static DWORD msgHash_QueryWorldEventTriggers = CHashString(_T("QueryWorldEventTriggers")).GetUniqueID();
	WORLDEVENTTRIGGERMAP WorldEventTriggerMap;
	WORLDEVENTTRIGGERMAP::iterator itr;

	m_TriggerName.ResetContent();
	
	QUERYWORLDEVENTTRIGGERSPARAMS param;
	param.pWorldEventTriggerMap = &WorldEventTriggerMap;

	m_ToolBox->SendMessage(msgHash_QueryWorldEventTriggers, sizeof(QUERYWORLDEVENTTRIGGERSPARAMS), &param, NULL, NULL);

    itr = WorldEventTriggerMap.begin();
	while( itr != WorldEventTriggerMap.end() )
	{
		m_TriggerName.AddString( m_ToolBox->GetHashString( itr->first ) );
		itr++;
	}
}

void CWorldEventToolPal::OnCbnDropdownName( void )
{
	if( m_WE_Activate.GetState() == 1)
	{
		UpdateWorldEventList();
	}
	else if(m_WET_Activate.GetState() == 1)
	{
		UpdateWorldEventTriggerList();
	}
}

void CWorldEventToolPal::UpdateWorldEventList( void )
{
	static DWORD msgHash_GetWorldEvents = CHashString(_T("GetWorldEvents")).GetUniqueID();
	WORLDEVENTLIST WorldEventList;
	WORLDEVENTLIST::iterator itr;	
	
	m_Name.ResetContent();

	QUERYWORLDEVENTSPARAMS param;
	param.pWorldEventList = &WorldEventList;

	m_ToolBox->SendMessage(msgHash_GetWorldEvents, sizeof(QUERYWORLDEVENTSPARAMS), &param, NULL, NULL);

	itr = WorldEventList.begin();
	while( itr != WorldEventList.end() )
	{
		m_Name.AddString( m_ToolBox->GetHashString( *itr ) );
		itr++;
	}
}

void CWorldEventToolPal::UpdateWorldEventTriggerList( void )
{
	static DWORD msgHash_QueryWorldEventTriggers = CHashString(_T("QueryWorldEventTriggers")).GetUniqueID();
	WORLDEVENTTRIGGERMAP WorldEventTriggerMap;
	WORLDEVENTTRIGGERMAP::iterator itr;

	m_Name.ResetContent();
	
	QUERYWORLDEVENTTRIGGERSPARAMS param;
	param.pWorldEventTriggerMap = &WorldEventTriggerMap;

	m_ToolBox->SendMessage(msgHash_QueryWorldEventTriggers, sizeof(QUERYWORLDEVENTTRIGGERSPARAMS), &param, NULL, NULL);

    itr = WorldEventTriggerMap.begin();
	while( itr != WorldEventTriggerMap.end() )
	{
		m_Name.AddString( m_ToolBox->GetHashString( itr->first ) );
		itr++;
	}
}

void CWorldEventToolPal::OnCbnSelchangeName( void )
{
	if( m_WE_Activate.GetState() == 1)
	{
		UpdateWorldEvent();
	}
	else if(m_WET_Activate.GetState() == 1)
	{
		UpdateWorldEventTrigger();
	}
}

void CWorldEventToolPal::UpdateWorldEvent( void )
{
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	static DWORD msgHash_GetComponentType = CHashString(_T("GetComponentType")).GetUniqueID();
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();

	static CHashString Hash_Type = CHashString(_T("CWorldEvent"));

	CString buf;
	m_Name.GetLBText(m_Name.GetCurSel(), buf);
	CHashString hszName((TCHAR*)buf.GetBuffer());
	FINDOBJECTPARAMS param;
	param.hszName = &hszName;
	DWORD result = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param);

	// object exists 
	if( param.bFound == true )
	{
		GETCOMPONENTTYPEPARAMS gctp;
		gctp.name = &hszName;
		m_ToolBox->SendMessage(msgHash_GetComponentType, sizeof(gctp), &gctp);

		// it is a world event, get values from it
		if ( (gctp.componenttype != NULL) && (gctp.componenttype->GetUniqueID() == Hash_Type.GetUniqueID()) )
		{
			CREATEARCHIVE ca;
			CHashString streamType(_T("Memory"));
			ca.streamData = NULL;
			ca.streamSize = 0;
			ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
			ca.streamType = &streamType;
	 	
			m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca, NULL, NULL);
			IArchive *MemArchive = (ca.archive);

			MemArchive->SetIsWriting(true);
			MemArchive->SeekTo(0);

			SERIALIZEOBJECTPARAMS sop;
			sop.name = &hszName;
			sop.archive = MemArchive;
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);

			MemArchive->SetIsWriting(false);
			MemArchive->SeekTo(0);

			StdString tmp;
			MemArchive->Read( tmp );
			m_TriggerName.SetWindowText( tmp.c_str() );
			MemArchive->Read( tmp );
			m_EntityName.SetWindowText( tmp.c_str() );
			MemArchive->Read( tmp );
			m_EntityType.SetWindowText( tmp.c_str() );
			MemArchive->Read( tmp );
			m_EventName.SetWindowText( tmp.c_str() );
			MemArchive->Read( tmp );
			m_EventParam.SetWindowText( tmp.c_str() );

			MemArchive->Close();
		}
	}
}

void CWorldEventToolPal::UpdateWorldEventTrigger( void )
{
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	static DWORD msgHash_GetComponentType = CHashString(_T("GetComponentType")).GetUniqueID();
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();

	static CHashString Hash_Type = CHashString(_T("CWorldEventTrigger"));

	CString buf;
	m_Name.GetLBText(m_Name.GetCurSel(), buf);
	CHashString hszName((TCHAR*)buf.GetBuffer());
	FINDOBJECTPARAMS param;
	param.hszName = &hszName;
	DWORD result = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param);

	// object exists 
	if( param.bFound == true )
	{
		GETCOMPONENTTYPEPARAMS gctp;
		gctp.name = &hszName;
		m_ToolBox->SendMessage(msgHash_GetComponentType, sizeof(gctp), &gctp);

		// it is a world event, get values from it
		if ( (gctp.componenttype != NULL) && (gctp.componenttype->GetUniqueID() == Hash_Type.GetUniqueID()) )
		{
			CREATEARCHIVE ca;
			CHashString streamType(_T("Memory"));
			ca.streamData = NULL;
			ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
			ca.streamType = &streamType;
	 	
			m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca, NULL, NULL);
			IArchive *MemArchive = (ca.archive);

			MemArchive->SetIsWriting(true);
			MemArchive->SeekTo(0);

			SERIALIZEOBJECTPARAMS sop;
			sop.name = &hszName;
			sop.archive = MemArchive;
			m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);

			MemArchive->SetIsWriting(false);
			MemArchive->SeekTo(0);

			StdString tmp;
			MemArchive->Read( tmp );
			m_EntityName.SetWindowText( tmp.c_str() );
			MemArchive->Read( tmp );
			m_EntityType.SetWindowText( tmp.c_str() );
			MemArchive->Read( tmp );
			m_EventName.SetWindowText( tmp.c_str() );
			
			MemArchive->Close();
		}
	}
}

void CWorldEventToolPal::OnCbnDropdownEventname()
{
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	static DWORD msgHash_QueryEvents = CHashString(_T("QueryStateMachineEvents")).GetUniqueID();

	CString buf;
	m_EntityName.GetWindowText( buf );
	CHashString hszName((TCHAR*)buf.GetBuffer());
	CHashString hszType(_T("CQHStateObject"));
	FINDOBJECTPARAMS foparams;
	foparams.hszName = &hszName;
	DWORD result = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &foparams);

	m_EventName.ResetContent();
	
	if( foparams.bFound == true )
	{
		QUERYEVENTSPARAMS qeparams;
		
		result = m_ToolBox->SendMessage(msgHash_QueryEvents, sizeof(QUERYEVENTSPARAMS), &qeparams, &hszName, &hszType);

		if( result == MSG_HANDLED )
		{
			QUERYEVENTSLIST::iterator itr;
			itr = qeparams.QueryEventsList.begin();

			while( itr != qeparams.QueryEventsList.end() )
			{
				StdString tmp = itr->StateName->GetString();
				tmp += ":";
				tmp += itr->EventName->GetString();
				m_EventName.AddString( tmp );
				itr++;
			}
		}
	}
}

void CWorldEventToolPal::OnBnClickedCheckVisualization()
{
	m_pEditor->SetVisualization( m_CheckVisualization.GetCheck() != 0 );
}
