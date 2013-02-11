///============================================================================
/// \file		TerrainComponent.cpp
/// \brief		Implementation file for Terrain to hook up to Elemental Engine
/// \date		05-12-2005
/// \author		Brian Bazyk
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

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON(CTerrainComponent);
// the messages we receive
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CTerrainComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CTerrainComponent);
REGISTER_MESSAGE_HANDLER(CreateObject, OnCreateObject, CTerrainComponent);
REGISTER_MESSAGE_HANDLER(DeleteObject, OnDeleteObject, CTerrainComponent);
REGISTER_MESSAGE_HANDLER(LeftClickEditorObject, OnLeftClickEditorObject, CTerrainComponent);
REGISTER_MESSAGE_HANDLER(WorldModified, OnWorldModified, CTerrainComponent);
REGISTER_MESSAGE_HANDLER(WorldSave, OnWorldSave, CTerrainComponent);
REGISTER_MESSAGE_HANDLER(WorldChangeFileName, OnWorldChangeFileName, CTerrainComponent);
REGISTER_MESSAGE_HANDLER(MouseMove, OnMouseMove, CTerrainComponent);
REGISTER_MESSAGE_HANDLER(GlobalUpdateTick, OnGlobalUpdateTick, CTerrainComponent);
REGISTER_MESSAGE_HANDLER(SetSectorModified, OnSetSectorModified, CTerrainComponent);
REGISTER_MESSAGE_HANDLER(GetTerrainCursorBrush, OnGetTerrainCursorBrush, CTerrainComponent);


// the constructor for the component
CTerrainComponent::CTerrainComponent()
{

	m_ToolBox = EngineGetToolBox();
	// Send a message to register with the EditorManager
	REGISTER_EDITOR_MESSAGE regEdParams;

	regEdParams.editorName = GetComponentType();

	// since the hashstrings need to be around for the life of the object, we keep
	// the m_EditableObjects class around.
	m_EditableObjects.clear();
	m_EditableObjects.push_back(new CHashString("CTerrainSectorRenderObject"));
	m_EditableObjects.push_back(new CHashString("CTerrainSector"));
	regEdParams.objectNames = &m_EditableObjects;

	static DWORD msgHash_RegisterEditor = CHashString(_T("RegisterEditor")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterEditor, sizeof(REGISTER_EDITOR_MESSAGE), &regEdParams);

}

// destructor for class
CTerrainComponent::~CTerrainComponent()
{
	EDITABLELIST::iterator elIter;
	
	// free up the hash strings associated with this list
	for (elIter = m_EditableObjects.begin(); elIter != m_EditableObjects.end(); ++elIter)
	{
		delete (*elIter);
	}

	// and clear the list now.
	m_EditableObjects.clear();
}

// we don't want elemental or anybody else creating us.
IComponent *CTerrainComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CTerrainComponent);
}

// serialize function for this object
void CTerrainComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CTerrainComponent::GetComponentType()
{
	static CHashString compName(_T("CTerrainComponent"));
	return &compName;
}

bool CTerrainComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// Set the parent of the component
void CTerrainComponent::SetParent(CTerrainEditor *parent)
{
	m_Parent = parent;
}

// message to handle OnCmdMsgs from main window
DWORD CTerrainComponent::OnCmdMsg(DWORD size, void *param)
{
	CMDMESSAGE *pCmdMsg;
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDMESSAGE));
	pCmdMsg = (CMDMESSAGE *)param;

	// forward to parent.
	if (m_Parent->OnCmdMsg(pCmdMsg->m_nID, pCmdMsg->m_nCode, pCmdMsg->m_pExtra,
		pCmdMsg->m_pHandlerInfo))
	{
		pCmdMsg->retVal = true;
		return MSG_HANDLED_PROCEED;
	}

	// it wasn't handled by parent, so return that
	return MSG_NOT_HANDLED;
}

// message to handle WindowsCreate function
DWORD CTerrainComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);

	return MSG_HANDLED_PROCEED;
}

// function to handle Create object message
DWORD CTerrainComponent::OnCreateObject(DWORD size, void *param)
{
//	m_Parent->UpdateScene();
	return MSG_HANDLED_PROCEED;
}

// function to handle Delete object message
DWORD CTerrainComponent::OnDeleteObject(DWORD size, void *param)
{
//	m_Parent->UpdateScene();
	return MSG_HANDLED_PROCEED;
}

DWORD CTerrainComponent::OnLeftClickEditorObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CLICKOBJECTPARAMS));
	CLICKOBJECTPARAMS *lButtonMess = (CLICKOBJECTPARAMS*)param;
	static CHashString hsts(_T("CTerrainSector"));
	IHashString *compType;

	compType = lButtonMess->compType;

	if (compType->GetUniqueID() == hsts.GetUniqueID())
	{
		m_Parent->SetSelectedSector( lButtonMess->object );

		if (lButtonMess->bIntersectionValid)
		{
			m_Parent->ClickTerrain( lButtonMess->pIntersectionPos, false );
		}
	}
	else
	{
		m_Parent->SetSelectedSector(NULL);
	}

	return MSG_HANDLED_PROCEED;
}

// Called when world modified flag is set
DWORD CTerrainComponent::OnWorldModified(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(BOOL));
	m_Parent->OnWorldModified((BOOL)param);
	return MSG_HANDLED_PROCEED;
}

// called when the world is saved
DWORD CTerrainComponent::OnWorldSave(DWORD size, void *param)
{
	m_Parent->OnWorldSave();
	return MSG_HANDLED_PROCEED;
}

// called when the worlds name is changing
DWORD CTerrainComponent::OnWorldChangeFileName(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(WORLDCHANGEFILENAMEPARAMS));
	m_Parent->OnWorldChangeName((WORLDCHANGEFILENAMEPARAMS *)param);
	return MSG_HANDLED_PROCEED;
}

DWORD CTerrainComponent::OnMouseMove(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(MOUSECOORDS));
	MOUSECOORDS *pParams = (MOUSECOORDS*)param;

	m_Parent->MouseMove(pParams);

	return MSG_HANDLED_PROCEED;
}

DWORD CTerrainComponent::OnGlobalUpdateTick(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GLOBALTICKPARAMS));

	m_Parent->OnGlobalUpdateTick();

	return MSG_HANDLED_PROCEED;
}

DWORD CTerrainComponent::OnSetSectorModified(DWORD size, void *param)
{
	IHashString *sectorName;

	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));

	sectorName = (IHashString*)param;

	m_Parent->SetSectorModified(sectorName);

	return MSG_HANDLED_STOP;
}

DWORD CTerrainComponent::OnGetTerrainCursorBrush(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(BRUSHPARAMS));
	BRUSHPARAMS *pBrushParams = (BRUSHPARAMS*)param;

	*pBrushParams = m_Parent->GetTerrainCursorBrush();

	// no brush here, allow other components to return one
	if (pBrushParams->m_pBrushData == NULL)
		return MSG_HANDLED_PROCEED;
	else
		return MSG_HANDLED_STOP;
}