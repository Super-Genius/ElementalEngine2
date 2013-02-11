///=====================================================================
/// \file	Helpers.cpp
/// \brief	Implementation of helper functions
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

///	\brief	fill passed buffer with archive data
///	\param	pArchive [in] - archive for processing
///	\param	data [out] - copy of archive stream data
void GetArchiveStreamData(IArchive *pArchive, Buffer &data)
{
	pArchive->SetIsWriting(false);
	pArchive->SeekTo(0);

	data.clear();
	char buf[256];
	UINT len = 0;
	do 
	{
		len = pArchive->Read(buf, _countof(buf));
		//data.resize(data.size() + len);
		data.insert(data.end(), buf, buf + len);
	}
	while (_countof(buf) == len);
}

///	\brief	get texture from EE engine
///	\param	hashFilePath hash of file path to texture
///	\return	pointer to existing texture object
ITextureObject *LoadTexture(DWORD hashFilePath)
{
	IToolBox *pToolBox = EngineGetToolBox();

	CHashString hszFilePath = pToolBox->GetHashString(hashFilePath);
	TEXTUREOBJECTPARAMS top;
	top.Name = &hszFilePath;
	top.TextureObjectInterface = NULL;
	top.bLoad = false;

	static DWORD msgHash_msgGetTexture = CHashString(_T("GetTexture")).GetUniqueID();
	if (MSG_HANDLED == pToolBox->SendMessage(msgHash_msgGetTexture, sizeof(top), &top) &&
		top.TextureObjectInterface != NULL)
	{
		return dynamic_cast<ITextureObject *>(top.TextureObjectInterface);
	}
	ASSERT(!"Invalid texture name was passed");
	return NULL;
}

///	\brief	get texture object data size
///	\param	pTexture - pointer to texture
///	\return	size of texture data
UINT GetTextureDataSize(ITextureObject *pTexture)
{
	ASSERT(pTexture != NULL);
	if (pTexture != NULL)
	{
		// TODO: this usage from Terrain plugin (tga)
		// check for other texture types
		UINT size = pTexture->GetWidth() * pTexture->GetHeight() * pTexture->GetColorDepth() / 8;
		int level = pTexture->GetNumLevels();
		return size;
	}
	return 0;
}

///	\brief	get current texture data from EE engine
///	\param	pTexture - pointer to texture
///	\param	data - buffer for copying data
///	\return	code of EE message result
DWORD GetTextureData(ITextureObject *pTexture, Buffer &data)
{
	data.clear();
	if (pTexture != NULL)
	{
		UINT size = GetTextureDataSize(pTexture);
		if (size > 0)
		{
			const char *pSourceData = (const char *)pTexture->Read();
			data.resize(size);
			copy(pSourceData, pSourceData + size, &data.front());
			return MSG_HANDLED;
		}
	}
	return MSG_ERROR;
}
