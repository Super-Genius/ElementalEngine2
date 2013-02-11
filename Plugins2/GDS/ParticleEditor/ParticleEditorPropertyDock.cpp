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

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CParticleEditorPropertyDock

IMPLEMENT_DYNAMIC(CParticleEditorPropertyDock, CGuiControlBar)
CParticleEditorPropertyDock::CParticleEditorPropertyDock(StdString formatFile, StdString formatDir)
{	
	m_OptionTreeView = NULL;
	m_ToolBox = EngineGetToolBox();
	m_bSupportMultiView = TRUE;
	m_szFormatFile = formatFile;
	m_szFormatDir = formatDir;
	serialUpdateFunc = NULL;	
}

CParticleEditorPropertyDock::~CParticleEditorPropertyDock()
{
	if(m_OptionTreeView != NULL)
	{
		delete m_OptionTreeView;
	}		
}


BEGIN_MESSAGE_MAP(CParticleEditorPropertyDock, CGuiControlBar)
	ON_WM_CREATE()
	ON_NOTIFY(OT_NOTIFY_ITEMCHANGED, IDC_PARTICLE_DOCKBAR, OnUpdateProperties)
END_MESSAGE_MAP()

void CParticleEditorPropertyDock::UpdateOptionTree(IHashString *name, IHashString *compType)
{
	vector <CHashString> names, types;
	names.push_back(CHashString(name->GetString()));
	types.push_back(CHashString(compType->GetString()));
	UpdateOptionTree(names, types);
}

void CParticleEditorPropertyDock::UpdateOptionTree(vector<CHashString>names, vector<CHashString>types)
{
	DWORD retval;
	SERIALIZEOBJECTPARAMS sop;
	IArchive *MemArchive;
	CHashString memType(_T("Memory"));
	CREATEARCHIVE ca;
	
	
	// make a writable archive
	ca.mode = STREAM_MODE_READ | STREAM_MODE_WRITE | ARCHIVE_MODE_WRITE;
	ca.streamType = &memType;
	ca.streamData = NULL;
	ca.streamSize = 0;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return;
	}
	MemArchive = ca.archive;
	sop.archive = MemArchive;

	MemArchive->SetIsWriting(true);

	if (!m_mObjCreated[names[0].GetUniqueID()])
	{

		m_mObjCreated[names[0].GetUniqueID()] = true;

//		MemArchive->SetIsWriting(true);
		m_OptionTreeView->Serialize(names[0], types, *MemArchive, false);	
		
		MemArchive->SetIsWriting(false);
		for (UINT i=0; i<names.size(); i++)
		{
			sop.name = &names[i];

			// workaround for gui group
			if (_tcscmp(types[0].GetString(), _T("CGUIGroup")) == 0)
			{
				MemArchive->SetIsWriting(true);
				MemArchive->Write(0);
				MemArchive->SetIsWriting(false);
			}
			// workaround for particle group
			if (_tcscmp(types[0].GetString(), _T("CParticleEmitter")) == 0)
			{
				MemArchive->SetIsWriting(true);
				MemArchive->Write(PARTICLE_SERIALIZE_DOCKBAR);
				MemArchive->SetIsWriting(false);
			}
			if (_tcscmp(types[0].GetString(), _T("CParticleType")) == 0)
			{
				MemArchive->SetIsWriting(true);
				MemArchive->Write(PARTICLE_SERIALIZE_DOCKBAR);
				MemArchive->SetIsWriting(false);
			}


			static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

			INITOBJECTPARAMS iop;
			iop.name = &names[i];
			static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
		}	
	}
	else
	{
		for (UINT i=0; i<names.size(); i++)
		{
			sop.name = &names[i];
			static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);	
		}	

		MemArchive->SetIsWriting(false);
		m_OptionTreeView->Serialize(names[0], types, *MemArchive, true);	
	}

	MemArchive->Close();		
}

void CParticleEditorPropertyDock::ClearOptionTree(void)
{
	m_OptionTreeView->CollapseAll();
}

int CParticleEditorPropertyDock::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	StdString basePath;
	m_ToolBox->GetDirectories(&basePath, NULL);

	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Declare variables
    CRect rcClient;
    DWORD dwStyle, dwOptions;
    
    // Get the clients rectangle
    GetClientRect(rcClient);
    
    // Setup the window style
    dwStyle = WS_CHILD | WS_VISIBLE;

    // Setup the tree options 
    dwOptions =  OT_OPTIONS_SHADEEXPANDCOLUMN | OT_OPTIONS_SHADEROOTITEMS;
	
	m_OptionTreeView = new COptionTreeWrapper(dwStyle, rcClient, this, dwOptions, IDC_PARTICLE_DOCKBAR, basePath.c_str());
	
	StdString filepath;

//	filepath += basePath;
	filepath += m_szFormatDir;
	filepath += m_szFormatFile;

	m_OptionTreeView->Init(filepath);

	return 0;
}

afx_msg void CParticleEditorPropertyDock::OnUpdateProperties(NMHDR* pNotifyStruct, LRESULT* plResult)
{
	LPNMOPTIONTREE pNMOptionTree = (LPNMOPTIONTREE)pNotifyStruct;
	// the optiontree item that has been updated
	COptionTreeItem *otiItem = pNMOptionTree->otiItem;

	// error check code
	// Occasionaly, this function will get called when coptiontree is in the middle of 
	// clearing the tree. This will cause problems b/c the tree items in the tree have become
	// deleted and this function will attempt to update and serializes those deleted items.
	COptionTreeItem* parentItem = otiItem->GetParent();
	if(parentItem == NULL) {return;}
	COptionTreeItem* childItem = parentItem->GetChild();
	if(childItem == NULL) {return;}
	// check if the root's first child is the updated optiontree item's sibling
	if( otiItem->GetSibling() == childItem ) 
	{
		// This implies that the updated optiontree item is no longer in the tree
		// Do not update the tree
		return;
	}

	// this block will work as long as the integrity of the root naming convention implemented in 
	// coptiontreeview is maintained... in other words, the root is named "obj_name(obj_type)"
	StdString parentLabel = parentItem->GetLabelText();
	StdString name, type, delims;
	delims = _T("()");
	parentLabel.GetToken(delims, name);
	parentLabel.GetToken(delims, type);

	SERIALIZEOBJECTPARAMS sop;
	IArchive *MemArchive;
	CHashString memType(_T("Memory"));
	CREATEARCHIVE ca;
	
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamType = &memType;
	ca.streamData = NULL; // internal buffer
	ca.streamSize = 0;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return;
	}
	MemArchive = ca.archive;
	sop.archive = MemArchive;

	CHashString elemName = name;
	CHashString elemType = type;

	m_OptionTreeView->Serialize(elemName, elemType, *MemArchive, false);
	MemArchive->SetIsWriting(false);

	if (serialUpdateFunc)
	{
		serialUpdateFunc(name, type, MemArchive);
	}

	MemArchive->Close();

	CParticleDoc * pDoc;
	pDoc = CParticleDoc::GetActiveDocument();
	assert(pDoc);

	static DWORD msgHash_ReloadTextures = CHashString(_T("ReloadTextures")).GetUniqueID();
	CHashString hszCParticleRenderObject;
	hszCParticleRenderObject.Init("CParticleRenderObject");
	DWORD retVal = m_ToolBox->SendMessage(msgHash_ReloadTextures, 0, 0, pDoc->GetParticleName(), &hszCParticleRenderObject );
}
