///=====================================================================
/// \file	UndoCommandData.cpp
/// \brief	Implementation of CUndoCommandData
/// \date	09/29/2007
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

CUndoCommandData::CUndoCommandData()
	: m_pData(NULL)
	, m_CompressedLength(0)
	, m_UncompressedLength(0)
{
}

CUndoCommandData::~CUndoCommandData()
{
	for_each(m_UndoActions.begin(), m_UndoActions.end(), Deleter<IUndoRedoAction>);
	m_UndoActions.clear();
	for_each(m_RedoActions.begin(), m_RedoActions.end(), Deleter<IUndoRedoAction>);
	m_RedoActions.clear();
	delete []m_pData;
}

///	\brief	add new action to undo action list
///	\param	pAction - pointer to new action
void CUndoCommandData::AddUndoAction(IUndoRedoAction *pAction)
{
	// TODO: sync with desired execution order (probably push_back is needed here)
	m_UndoActions.push_front(pAction);
}

///	\brief	add new action to redo action list
///	\param	pAction - pointer to new action
void CUndoCommandData::AddRedoAction(IUndoRedoAction *pAction)
{
	m_RedoActions.push_back(pAction);
}

///	\brief	add new action to redo or redo action lists
///	\param	pAction - pointer to new action
///	\param	bUndo - specify target list for action
///					if true then pAction will be added to undo list
void CUndoCommandData::AddAction(IUndoRedoAction *pAction, bool bUndo)
{
	if (bUndo)
	{
		AddUndoAction(pAction);
	}
	else
	{
		AddRedoAction(pAction);
	}
}

///	\brief	execute undo part of command
///	\return	code of EE message result
DWORD CUndoCommandData::ExecuteUndo()
{
	return Execute(m_UndoActions);
}

///	\brief	execute redo part of command
///	\return	code of EE message result
DWORD CUndoCommandData::ExecuteRedo()
{
	return Execute(m_RedoActions);
}

///	\brief	determine if undo command data was filled correctly
///	\return	true if active undo actions exist in the list
bool CUndoCommandData::HasUndoActions() const
{
	return !m_UndoActions.empty();
}

///	\brief	compact memory usage by using data delta and compression
///	\details This function extracts undo/redo pairs of CObjectSerializeActions 
///			and CTextureDataActions. Archive streams and textures are compressed
///			into single buffer for reducing memory usage and original 
///			CObjectSerializeActions and CTextureDataActions actions are replaced
///			by updated actions which have access to command data buffer
//	logic:
// 1. Extracts data for CObjectSerializeActions and CTextureDataActions
//	  in redo list. Iterator are saved to maps currentData and 
//	  currentDataActions for future update
// 2. Process CObjectSerializeActions and CTextureDataActions in undo list.
//	  If action has paired action in redo stack then use both actions are 
//	  replaced by CDeltaDataAction. Processed redo actions are removed from 
//	  maps currentData and currentDataActions. Delta data is added to single 
//	  buffer on this stage
// 3. Each remained action in currentData map is replaced with 
//	  CDeltaDataAction (ApplyObjectData or ApplyTextureData version). Delta data
//	  is added to the buffer on this stage as well
// 4. Filled buffer is compressed with zlib and used later during Undo or Redo 
//	  calls
void CUndoCommandData::CompactData()
{
	// TODO: this variable for diagnostic only. remove it later
	size_t oldDataSize = 0;

	Buffer data;
	delete []m_pData;
	bool isObjectAction = false;
	// step 1. collecting current object states and textures from redo stack
	map<DWORD, Buffer> currentData;
	map<DWORD, ActionList::iterator> currentDataActions;
	for (ActionList::iterator it = m_RedoActions.begin();
			it != m_RedoActions.end(); ++it)
	{
		DWORD hashName = 0;
		Buffer data;
		// try to get name and data
		if (GetBuffer(*it, hashName, data, isObjectAction))
		{
			if (currentData.count(hashName) > 0)
			{
				// TODO: report error?
				delete (*it);
				(*it) = NULL;
			}
			else
			{
				// removing unnecessary data copying
				currentData[hashName].swap(data);
				// save reference to action for replacement on next stage
				currentDataActions.insert(make_pair(hashName, it));
			}
		}
	}

	Buffer uncompressedData;
	// step 2. update undo actions with delta data analogue
	set<DWORD> processedItems;
	map<DWORD, Buffer> diffData;
	for (ActionList::iterator it = m_UndoActions.begin();
		it != m_UndoActions.end(); ++it)
	{
		DWORD hashName = 0;
		// try to get name and data
		if (GetBuffer(*it, hashName, data, isObjectAction))
		{
			if (processedItems.count(hashName) > 0)
			{
				// TODO: report error?
				delete (*it);
				(*it) = NULL;
			}
			else
			{
				oldDataSize += data.size();
				processedItems.insert(hashName);
				delete *it;
				map<DWORD, Buffer>::iterator itData = currentData.find(hashName);
				if (itData != currentData.end())
				{
					oldDataSize += itData->second.size();
					delete *currentDataActions[hashName];
					if (isObjectAction)
					{
						// it is hard to add delta processing actions for objects...
						// some EE objects use internal caches (i.e. CTerrainSectorRenderObject)
						// Restore saved state and init messages may lead to
						// the different serialized archive content, so delta 
						// data processing fails in such cases
						// but if we place both saved buffer one after one then 
						// RLE may produce excellent compression results
						IUndoRedoAction *pAction;
						// introduce alias for buffer with current data
						Buffer &redoData = itData->second;
						pAction = new CDeltaDataAction<ApplyObjectData>(
											this, hashName,
											uncompressedData.size(), redoData.size());
						*currentDataActions[hashName] = pAction;
						uncompressedData.insert(uncompressedData.end(), redoData.begin(), redoData.end());

						pAction = new CDeltaDataAction<ApplyObjectData>(
											this, hashName,
											uncompressedData.size(), data.size());
						*it = pAction;
					}
					else
					{
						GetBuffersDelta(itData->second, data);
						// check if texture has changes
						if (!data.empty())
						{
							IUndoRedoAction *pAction;
							pAction = new CDeltaDataAction<UndoTextureData>(
								this, hashName,
								uncompressedData.size(), data.size());
							*it = pAction;
							pAction = new CDeltaDataAction<RedoTextureData>(
								this, hashName,
								uncompressedData.size(), data.size());
							*currentDataActions[hashName] = pAction;
						}
						else
						{
							// texture was not changed, so texture actions are not needed at all 
							*it = NULL;
							*currentDataActions[hashName] = NULL;
						}
					}
					currentDataActions.erase(hashName);
				}
				else
				{
					IUndoRedoAction *pAction;
					if (isObjectAction)
					{
						// adding delta processing actions for object
						pAction = new CDeltaDataAction<ApplyObjectData>(
											this, hashName,
											uncompressedData.size(), data.size());
					}
					else
					{
						// adding delta processing actions for texture
						pAction = new CDeltaDataAction<ApplyTextureData>(
											this, hashName,
											uncompressedData.size(), data.size());
					}
					*it = pAction;
				}
				// append delta data to the buffer
				uncompressedData.insert(uncompressedData.end(), data.begin(), data.end());
			}
		}
	}

	// step 3. process remained actions in currentDataActions
	map<DWORD, ActionList::iterator>::iterator itRedo = currentDataActions.begin();
	for (; itRedo != currentDataActions.end(); ++itRedo)
	{
		Buffer &data = currentData[itRedo->first];
		oldDataSize += data.size();
		bool isObjectAction = (dynamic_cast<CObjectSerializeAction *>(*(itRedo->second)) != NULL);
		IUndoRedoAction *pAction;
		if (isObjectAction)
		{
			// adding delta processing actions for object
			pAction = new CDeltaDataAction<ApplyObjectData>(
								this, itRedo->first,
								uncompressedData.size(), data.size());
		}
		else
		{
			// adding delta processing actions for texture
			pAction = new CDeltaDataAction<ApplyTextureData>(
								this, itRedo->first,
								uncompressedData.size(), data.size());
		}
		delete *(itRedo->second);
		*(itRedo->second) = pAction;
		// append delta data to the buffer
		uncompressedData.insert(uncompressedData.end(), data.begin(), data.end());
	}

	// clear deleted actions from the lists
	m_UndoActions.erase(remove(m_UndoActions.begin(),
							   m_UndoActions.end(),
							   (IUndoRedoAction*)NULL),
						m_UndoActions.end());
	m_RedoActions.erase(remove(m_RedoActions.begin(),
							   m_RedoActions.end(),
							   (IUndoRedoAction*)NULL),
						m_RedoActions.end());

	m_UncompressedLength = uncompressedData.size();
	if (m_UncompressedLength > 0)
	{
		// step 4. compression
		// we need to save 1 byte at least...
		uLong limit = (uLong)m_UncompressedLength - 1;
		data.resize(limit);
		uLongf compressedSize = limit;
		if (Z_OK != compress2(&data.front(), &compressedSize,
			&uncompressedData.front(), (uLong)uncompressedData.size(),
			Z_BEST_COMPRESSION))
		{
			// TODO. log error?
			// either compression error occur or buffer was too small
			// but we will live with uncompressed buffer in this case
			m_CompressedLength = m_UncompressedLength;
		}
		else
		{
			m_CompressedLength = compressedSize;
		}
		if (m_CompressedLength < m_UncompressedLength)
		{
			m_pData = new BYTE[m_CompressedLength];
			data.resize(m_CompressedLength);
			copy(data.begin(), data.end(), m_pData);
		}
		else
		{
			m_pData = new BYTE[m_UncompressedLength];
			copy(uncompressedData.begin(), uncompressedData.end(), m_pData);
		}
		TRACE("CUndoCommandData::CompactData(). Initial %d bytes have been compressed to %d bytes\n", oldDataSize, compressedSize);
	}
	else
	{
		m_CompressedLength = 0;
		m_pData = NULL;
	}
}

///	\brief	execute commands from stream
///	\param	actions list with action to execute
///	\return	code of EE message result
DWORD CUndoCommandData::Execute(ActionList &actions)
{
	// save pointer to current compressed data
	LPBYTE pCompressedData = m_pData;
	// prepare buffer for uncompress
	Buffer uncompressedData(m_UncompressedLength);
	if (m_UncompressedLength > m_CompressedLength)
	{
		m_pData = &uncompressedData.front();
		// then uncompress data for interaction with executed data
		uLongf destLen = (uLongf)m_UncompressedLength;
		uncompress(m_pData, &destLen, pCompressedData, (ULONG)m_CompressedLength);
	}

	// execute actions in order
	for_each(actions.begin(),
			 actions.end(),
			 mem_fun<DWORD, IUndoRedoAction>(&IUndoRedoAction::Execute));

	// restore back compressed data
	m_pData = pCompressedData;

	// ignore any action error. 
	// It is not clear how to rollback changes if some action failed
	return MSG_HANDLED;
}

///	\brief	gather information about name and data from the action
///	\param	pAction [in] - pointer to action
///	\param	hashName [out] - hash of object or texture name
///	\param	data [out] - buffer with archive or texture 
///	\param	isObjectAction [out] - true if passed action was CObjectSerializeAction
///	\return	true if hash and data buffer are valid
bool CUndoCommandData::GetBuffer(IUndoRedoAction *pAction, DWORD &hashName, 
								 Buffer &data, bool &isObjectAction)
{
	// check if action is CObjectSerializeAction
	CObjectSerializeAction *pObjData = dynamic_cast<CObjectSerializeAction *>(pAction);
	if (pObjData != NULL)
	{
		hashName = pObjData->GetName();
		pObjData->GetData(data);
		isObjectAction = true;
		return true;
	}

	isObjectAction = false;
	// check if action is CTextureDataAction
	CTextureDataAction *pTextureData = dynamic_cast<CTextureDataAction *>(pAction);
	if (pTextureData != NULL)
	{
		hashName = pTextureData->GetName();
		pTextureData->GetData(data);
		return true;
	}

	// we don't know which class was passed
	return false;
}

///	\brief	calculate buffer delta. Output buffer is also reduced by trailing 
///			zeros if both input buffers have the same size
///	\param	source [in] - source data
///	\param	target [in,out] - target values. values in this buffer are 
///							  replaced with delta between source and target
void CUndoCommandData::GetBuffersDelta(const Buffer &source, Buffer &target)
{
	// set flag if we may reduce buffer size after calculation
	bool reduceSize = source.size() == target.size();
	// adjust source and target buffers to the same size
	if (!reduceSize && source.size() > target.size())
	{
		target.resize(source.size(), 0);
	}

	// calculate delta for source and target buffers
	Buffer::const_iterator itSource = source.begin();
	for (Buffer::iterator it = target.begin(); it != target.end(); ++it, ++itSource)
	{
		*it = (*it) - (*itSource);
	}

	// reduce target buffer by trailing zeros if this is possible
	if (reduceSize)
	{
		size_t newSize = target.size();
		for (Buffer::reverse_iterator itZeros = target.rbegin();
			 itZeros != target.rend() && 0 == *itZeros; 
			 ++itZeros, --newSize);
		target.resize(newSize);
	}
}
