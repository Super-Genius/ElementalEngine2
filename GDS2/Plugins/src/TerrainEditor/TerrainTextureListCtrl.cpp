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

static UINT ResourceDropMessage = RegisterWindowMessage(RESOURCE_DROP_MSG_STR);
static UINT ResourceDragMessage = RegisterWindowMessage(RESOURCE_DRAG_MSG_STR);

BEGIN_MESSAGE_MAP(CTerrainTextureListCtrl, CListCtrl)
	ON_REGISTERED_MESSAGE(ResourceDropMessage, OnDropMessage)
	ON_REGISTERED_MESSAGE(ResourceDragMessage, OnDragMessage)
END_MESSAGE_MAP()

CTerrainTextureListCtrl::CTerrainTextureListCtrl()
{
	m_ToolBox = EngineGetToolBox();
}

CTerrainTextureListCtrl::~CTerrainTextureListCtrl()
{
}

LRESULT CTerrainTextureListCtrl::OnDropMessage(WPARAM wParam, LPARAM lParam)
{
	CResourceListCtrlEx::OnDropMessage(wParam,lParam);

	DROPOBJECTINFO *pInfo = (DROPOBJECTINFO*)lParam;
	const TCHAR *szFilePath = (const TCHAR *)&pInfo->pResourcePageInfo->filepath;

	CTerrainLayerMaterialType *pParent;
	pParent = DYNAMIC_DOWNCAST(CTerrainLayerMaterialType, GetParent());
	if (pParent != NULL)
	{
		// find insertion position
		int iInsertion = (GetDropIndex() == -1) ? GetItemCount() : GetDropIndex();
		// insert item
		pParent->InsertTextureListItem( iInsertion, szFilePath, 32.0f, 32.0f, true );
	}

	return 0;
}

LRESULT CTerrainTextureListCtrl::OnDragMessage(WPARAM wParam, LPARAM lParam)
{
	CResourceListCtrlEx::OnDragMessage(wParam,lParam);
	return 0;
}

bool CTerrainTextureListCtrl::InsertLayer( int iPositionIndex, const TCHAR *szFilePath )
{
	return InsertResource( iPositionIndex, szFilePath );
}

CBitmap * CTerrainTextureListCtrl::CreateThumbnail(const TCHAR *szFilePath)
{
	CREATETHUMBNAILPARAMS ct(szFilePath);
	static DWORD msgHash_CreateTextureThumbnail = CHashString(_T("CreateTextureThumbnail")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateTextureThumbnail, sizeof(ct), &ct ) != MSG_HANDLED)
		return NULL;
	return ct.pThumbnail;
}

void CTerrainTextureListCtrl::RemoveLayer( int iLayerIndex )
{
	RemoveResource( iLayerIndex );
}
