///=====================================================================
/// \file	DeltaDataAction.h
/// \brief	Declaration helper functions set
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
#ifndef _GDS_UNDOREDO_HELPERS_H_
#define _GDS_UNDOREDO_HELPERS_H_

///	\brief	fill passed buffer with archive data
///	\param	pArchive [in] - archive for processing
///	\param	data [out] - copy of archive stream data
void GetArchiveStreamData(IArchive *pArchive, Buffer &data);

///	\brief	get texture from EE engine
///	\param	hashFilePath hash of file path to texture
///	\return	pointer to existing texture object
ITextureObject *LoadTexture(DWORD hashFilePath);

///	\brief	get texture object data size
///	\param	pTexture - pointer to texture
///	\return	size of texture data
UINT GetTextureDataSize(ITextureObject *pTexture);

///	\brief	get texture data from EE engine
///	\param	pTexture - pointer to texture
///	\param	data - buffer for copying data
///	\return	code of EE message result
DWORD GetTextureData(ITextureObject *pTexture, Buffer &data);

#endif //_GDS_UNDOREDO_HELPERS_H_
