///=====================================================================
/// \file	DependentProcessors.cpp
/// \brief	Implementation of CDeltaDataAction
/// \date	11/16/2007
/// \author	Andrey Ivanov
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
#include "StdAfx.h"

IToolBox *CDependentProcessors::m_pToolBox = EngineGetToolBox();

CDependentProcessors::CDependentProcessors()
{
	ASSERT(m_Processors.empty());
	CHashString hszName = _T("CTerrainSector");
	m_Processors.insert(make_pair(hszName.GetUniqueID(), ProcessCTerrainSector));
	hszName = _T("CTerrainSectorRenderObject");
	m_Processors.insert(make_pair(hszName.GetUniqueID(), ProcessCTerrainSectorRenderObject));
}

DWORD CDependentProcessors::ProcessObject(IHashString *name, bool undo)
{
	ASSERT(name != NULL);
	CHashString hszType = GetComponentType(name);
	ProcessorMap::const_iterator it = m_Processors.find(hszType.GetUniqueID());
	if (it != m_Processors.end())
	{
		return it->second(name, undo);
	}

	return MSG_HANDLED;
}

DWORD CDependentProcessors::ProcessCTerrainSector(IHashString *name, bool undo)
{
	IArchive *pArchive = GetEEObjectProperties(name);
	if (pArchive == NULL)
	{
		return MSG_ERROR;
	}

	pArchive->SetIsWriting(false);
	pArchive->SeekTo(0);
	// read heightmap name from archive
	int iSectorX;
	int iSectorY;
	StdString szHeightmapName;
	pArchive->Read(iSectorX);
	pArchive->Read(iSectorY);
	pArchive->Read(szHeightmapName);
	pArchive->Close();

	DWORD res = MSG_ERROR;
	static CHashString hszEmpty(_T(""));
	CHashString hszHeightmap = szHeightmapName.c_str();
	IUndoRedoAction *pAction = new CObjectRecreateAction(&hszEmpty, &hszHeightmap, NULL);
	if (undo)
	{
		static DWORD msgHash_UndoSaveObject = CHashString(_T("UndoSaveObject")).GetUniqueID();
		m_pToolBox->SendMessage(msgHash_UndoSaveObject, sizeof(IHashString), &hszHeightmap);

		static DWORD msgHash_UndoAddAction = CHashString(_T("UndoAddAction")).GetUniqueID();
		res = m_pToolBox->SendMessage(msgHash_UndoAddAction, sizeof(IUndoRedoAction), pAction);
		if (MSG_HANDLED != res)
		{
			delete pAction;
		}
	}
	else
	{
		static DWORD msgHash_RedoAddAction = CHashString(_T("RedoAddAction")).GetUniqueID();
		res = m_pToolBox->SendMessage(msgHash_RedoAddAction, sizeof(IUndoRedoAction), pAction);
		if (MSG_HANDLED != res)
		{
			delete pAction;
		}

		static DWORD msgHash_RedoSaveObject = CHashString(_T("RedoSaveObject")).GetUniqueID();
		m_pToolBox->SendMessage(msgHash_RedoSaveObject, sizeof(IHashString), &hszHeightmap);
	}

	return MSG_HANDLED;
}

DWORD CDependentProcessors::ProcessCTerrainSectorRenderObject(IHashString *name, bool undo)
{
	vector<TERRAINLAYERINFOPARAMS::TerrainLayer_t> layers;
	vector<TERRAINLAYERINFOPARAMS::TerrainLayerMask> masks;
	TERRAINLAYERINFOPARAMS infoParams;
	infoParams.pTerrainLayerList = &layers;
	infoParams.pTerrainLayerMaskList = &masks;

	static DWORD msgHash_TerrainLayerInfo = CHashString(_T("TerrainLayerInfo")).GetUniqueID();
	m_pToolBox->SendMessage(msgHash_TerrainLayerInfo, sizeof(infoParams), &infoParams, name);

	DWORD hashSaveTextureMessage;
	if (undo)
	{
		static const DWORD msgHash_UndoSaveTexture = CHashString(_T("UndoSaveTexture")).GetUniqueID();
		hashSaveTextureMessage = msgHash_UndoSaveTexture;
	}
	else
	{
		static const DWORD msgHash_RedoSaveTexture = CHashString(_T("RedoSaveTexture")).GetUniqueID();
		hashSaveTextureMessage = msgHash_RedoSaveTexture;
	}

	vector<TERRAINLAYERINFOPARAMS::TerrainLayerMask>::const_iterator it = masks.begin();
	for (; it != masks.end(); ++it)
	{
		m_pToolBox->SendMessage(hashSaveTextureMessage, sizeof(IHashString), it->szMaskTexture);
	}

	return MSG_HANDLED;
}