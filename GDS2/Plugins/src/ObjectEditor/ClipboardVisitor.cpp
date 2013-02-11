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

#include "StdAfx.h"

CClipboardVisitor::CClipboardVisitor()
{
	m_ToolBox = EngineGetToolBox();
}

CClipboardVisitor::~CClipboardVisitor()
{
	Reset();
}

void CClipboardVisitor::Reset()
{
	// close all archives
	for (CLIPBOARDOBJECTLIST::iterator itrObject = m_ObjectList.begin(); itrObject != m_ObjectList.end(); itrObject++)
	{
		itrObject->pArchive->Close();
	}
	// clear list
	m_ObjectList.clear();
}

bool CClipboardVisitor::IsHierarchicalVisitor() 
{
	return false;
}

bool CClipboardVisitor::Visit( IComponent * component, bool bVisitEnter)
{
	IObject *pObject = dynamic_cast<IObject*>(component);
	if (!pObject)
		return false;

	CHashString memType(_T("Memory"));
	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.archive = NULL;
	ca.streamType = &memType;
	ca.streamData = NULL;
	ca.streamSize = 0;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(ca), &ca);
	IArchive *pArchive = ca.archive;
	if (!pArchive)
		return false;

	pArchive->SeekTo(0);
	pArchive->SetIsWriting( true );

	// save object data
	pObject->Serialize( *pArchive );

	CLIPBOARDOBJECT clipobj;
	clipobj.pArchive = pArchive;
	clipobj.szComponentType.Init( pObject->GetComponentType()->GetString() );
	clipobj.szName.Init( pObject->GetName()->GetString() );
	clipobj.szParent.Init( pObject->GetParentName()->GetString() );
	m_ObjectList.push_back( clipobj );

	return true;
}
