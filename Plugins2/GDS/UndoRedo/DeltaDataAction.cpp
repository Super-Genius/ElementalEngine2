///=====================================================================
/// \file	DeltaDataAction.cpp
/// \brief	Implementation of CDeltaDataAction
/// \date	11/10/2007
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

enum DeltaMode
{
	DeltaNone, // apply data from buffer directly
	DeltaAdd, // add data in buffer from current state (undo)
	DeltaSubtract, // subtract data in buffer from current state (redo)
};

///	\brief	implementation for buffer data interaction of object undo/redo actions
///	\param	pCommand - pointer to command this action belongs to
///	\param	hashName - hash of object name to interact
///	\param	offset - delta data offset in command buffer
///	\param	size - delta data size in command buffer
///	\param	mode - command buffer interaction mode
DWORD UndoRedoObjectDataImpl(CUndoCommandData *pCommand, DWORD hashName, size_t offset, size_t size, DeltaMode mode)
{
	DWORD res = MSG_HANDLED;
	// get data offset
	LPBYTE pData = pCommand->GetDeltaData() + offset;

	IToolBox *pToolBox = EngineGetToolBox();
	CHashString hszName = pToolBox->GetHashString(hashName);
	// check if we need to apply data changes or may init object directly
	if (size > 0)
	{
		IArchive *pArchive;
		if (DeltaNone == mode)
		{
			// create archive for serialization and fill it with buffered data
			pArchive = CreateMemoryArchive(pData, (DWORD)size);
		}
		else
		{
			// get current object state for applying delta data
			pArchive = GetEEObjectProperties(&hszName);
			Buffer currentData;
			// extract serialized data to buffer
			GetArchiveStreamData(pArchive, currentData);

			// adjust serialized data length if this is needed
			if (currentData.size() < size)
			{
				currentData.resize(size, 0);
			}

			// restore correct data for the stream
			if (DeltaAdd == mode)
			{
				for (Buffer::iterator it = currentData.begin(); size > 0; --size, ++pData, ++it)
				{
					(*it) = (*it) + (*pData);
				}
			}
			else
			{
				for (Buffer::iterator it = currentData.begin(); size > 0; --size, ++pData, ++it)
				{
					(*it) = (*it) - (*pData);
				}
			}

			// then write it
			pArchive->SetIsWriting(true);
			pArchive->SeekTo(0);
			pArchive->Write(&currentData.front(), currentData.size());
		}

		CFileVersionSetter lastVersion;
		// setup up serialization parameters
		SERIALIZEOBJECTPARAMS sop;
		sop.name = &hszName;
		sop.archive = pArchive;
		pArchive->SetIsWriting(false);
		pArchive->SeekTo(0);
		// serialize object from archive
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		res = pToolBox->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop);
		if (res != MSG_HANDLED)
		{
			// log this error?
			return res;
		}

		int count = 0;
		pArchive->Read(count, _T("count"));
		ASSERT(count >= 0 && count <= 8);
		if (count > 0)
		{
			EDITINGPROPERTIESPARAMS epp;
			epp.name = &hszName;
			while (count--)
			{
				StdString name;
				StdString value;
				pArchive->Read(name, _T("name"));
				pArchive->Read(value, _T("value"));
				epp.params.insert(make_pair(name, value));
			}
			static const DWORD hashMsg_SetEditingProperties = CHashString(_T("SetEditingProperties")).GetUniqueID();
			pToolBox->SendMessage(hashMsg_SetEditingProperties, sizeof(epp), &epp);
		}

		pArchive->Close();
	}

	INITOBJECTPARAMS iop;
	// send message to EE to initialize the object
	iop.name = &hszName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	res = pToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);

	return res;
}

///	\brief	implementation for buffer data interaction of texture undo/redo actions
///	\param	pCommand - pointer to command this action belongs to
///	\param	hashName - hash of object name to interact
///	\param	offset - delta data offset in command buffer
///	\param	size - delta data size in command buffer
///	\param	mode - command buffer interaction mode
DWORD UndoRedoTextureDataImpl(CUndoCommandData *pCommand, DWORD hashName, size_t offset, size_t size, DeltaMode mode)
{
	// get texture for interaction
	ITextureObject *pTexture = LoadTexture(hashName);
	ASSERT(pTexture != NULL);
	if (pTexture != NULL)
	{
		// get delta data buffer
		LPBYTE pData = pCommand->GetDeltaData() + offset;

		if (DeltaNone == mode)
		{
			// apply data directly from the buffer for DeltaNone mode
			ASSERT(GetTextureDataSize(pTexture) == size);
			pTexture->Write(pData);
		}
		else
		{
			// get current texture state for applying delta
			Buffer currentData;
			GetTextureData(pTexture, currentData);

			// this assertion means that something was totally changed inside 
			// editors or EE
			ASSERT(currentData.size() >= size);

			// restore correct data for the buffer
			if (DeltaAdd == mode)
			{
				for (Buffer::iterator it = currentData.begin(); size > 0; --size, ++pData, ++it)
				{
					(*it) = (*it) + (*pData);
				}
			}
			else
			{
				for (Buffer::iterator it = currentData.begin(); size > 0; --size, ++pData, ++it)
				{
					(*it) = (*it) - (*pData);
				}
			}
			// write updated buffer with data
			pTexture->Write(&currentData.front());
		}
	}

	return MSG_HANDLED;
}

DWORD UndoObjectData(CUndoCommandData *pCommand, DWORD hashName, size_t offset, size_t size)
{
	return UndoRedoObjectDataImpl(pCommand, hashName, offset, size, DeltaAdd);
}

DWORD RedoObjectData(CUndoCommandData *pCommand, DWORD hashName, size_t offset, size_t size)
{
	return UndoRedoObjectDataImpl(pCommand, hashName, offset, size, DeltaSubtract);
}

DWORD ApplyObjectData(CUndoCommandData *pCommand, DWORD hashName, size_t offset, size_t size)
{
	return UndoRedoObjectDataImpl(pCommand, hashName, offset, size, DeltaNone);
}

DWORD UndoTextureData(CUndoCommandData *pCommand, DWORD hashName, size_t offset, size_t size)
{
	return UndoRedoTextureDataImpl(pCommand, hashName, offset, size, DeltaAdd);
}

DWORD RedoTextureData(CUndoCommandData *pCommand, DWORD hashName, size_t offset, size_t size)
{
	return UndoRedoTextureDataImpl(pCommand, hashName, offset, size, DeltaSubtract);
}

DWORD ApplyTextureData(CUndoCommandData *pCommand, DWORD hashName, size_t offset, size_t size)
{
	return UndoRedoTextureDataImpl(pCommand, hashName, offset, size, DeltaNone);
}
