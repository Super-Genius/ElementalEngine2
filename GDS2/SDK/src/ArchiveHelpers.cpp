///=====================================================================
/// \file	EditorHelpers.h
/// \brief	Implementation of helper functions for archives
/// \date	12/19/2007
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
#include "stdafx.h"

namespace GDS_SDK_Helpers
{

/// \brief	Creates XMLArchive stream
/// \param	mode - creation mode for the stream
/// \return	pointer to the created archive
IXMLArchive *CreateXMLArchiveStream(DWORD mode)
{
	// build archive parameters
	static CHashString streamType(_T("Memory"));
	CREATEARCHIVE ca;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.mode = mode;
	ca.streamType = &streamType;

	IToolBox *toolBox = ::EngineGetToolBox();
	// create archive
	static DWORD msgHash_CreateXMLArchiveStream = CHashString(_T("CreateXMLArchiveStream")).GetUniqueID();
	if (toolBox->SendMessage(msgHash_CreateXMLArchiveStream, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED) 
	{ 
		// if create fails report it
		ASSERT(!"Failed to create XML archive");
		toolBox->Log(LOGERROR, _T("%s(%i): Failed to create XML archive."), __FILE__, __LINE__);
		return NULL;
	}

	return dynamic_cast<IXMLArchive *>(ca.archive);
}

/// \brief	Create XMLArchive
/// \param	path - path to archive file
/// \param	mode - creation mode for the stream. Default is read mode
/// \return	pointer to the created archive
///			caller is a owner of returned archive and responsible for closing
IXMLArchive *CreateXMLArchive(LPCTSTR path, DWORD mode)
{
	static CHashString streamType(_T("File"));
	// build archive parameters
	CREATEARCHIVE ca;
	ca.streamData = (void *)path;
	ca.mode = mode;
	ca.streamType = &streamType;

	// create archive
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (::EngineGetToolBox()->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED) 
	{ 
		// if create fails report it
		StdString szMessage = _T("Failed to load xml format file: ");
		szMessage += path;
		::MessageBox(NULL, szMessage.c_str(), _T("Error"), MB_ICONERROR);
		return NULL;
	}

	return dynamic_cast<IXMLArchive *>(ca.archive);
}

/// \brief	Create memory archive
///			Returned archive is opened for reading
/// \param	buffer - pointer to buffer with data
/// \param	size - size of the buffer
///	\return	pointer to created archive
///			caller is a owner of returned archive and responsible for closing
IArchive* CreateMemoryArchive(void *buffer, DWORD size)
{
	IToolBox *toolBox = ::EngineGetToolBox();

	static CHashString hszStreamType(_T("Memory"));

	// create a new memory archive
	CREATEARCHIVE ca;
	ca.streamData = buffer;
	ca.streamSize = size;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamType = &hszStreamType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (MSG_HANDLED != toolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca))
	{
		toolBox->Log(LOGERROR, _T("%s(%i): Failed to create memory archive.\n"), __FILE__, __LINE__ );
		return NULL;
	}

	ca.archive->SetIsWriting(true);
	return ca.archive;
}

///	\brief	copies content of source xml archive to target archive
///	\param	source - pointer to source IXMLArchive
///	\param	target - pointer to target IXMLArchive
void CopyXMLArchive(IXMLArchive *source, IXMLArchive *target)
{
	if (source == NULL || target == NULL)
	{
		return;
	}

	StdString nodeName;
	int depth = 0;
	source->SeekTo( 0 );
	while (source->GetNode(nodeName))
	{
		int currentDepth = source->GetDepth();
		for (; depth > currentDepth; --depth)
		{
			target->EndClass();
		}
		target->StartClass(nodeName);
		++depth;
		int attributesCount = source->GetNumAttributes();
		while (attributesCount--)
		{
			StdString value;
			StdString tag = source->GetCurrentNodeName();
			source->Read(value, tag);
			target->Write(value, tag);
		}
	}
	while (depth--)
	{
		target->EndClass();
	}
}

}; // namespace GDS_SDK_Helpers
