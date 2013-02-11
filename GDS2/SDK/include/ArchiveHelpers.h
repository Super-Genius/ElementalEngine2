///=====================================================================
/// \file	EditorHelpers.h
/// \brief	Set of helper functions for interaction with archives
/// \date	12/18/2007
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
#ifndef _GDS_ARCHIVE_HELPERS_H_
#define _GDS_ARCHIVE_HELPERS_H_

#include "BaseMessages.h"

namespace GDS_SDK_Helpers
{

template<class T>
IXMLArchive *TransformXMLArchive(IXMLArchive *source, T transform)
{
	IXMLArchive *target = CreateXMLArchiveStream();
	if (target == NULL)
	{
		return NULL;
	}

	vector< pair<StdString, StdString> > attributes;
	StdString nodeName;
	int depth = 0;
	int cancelledDepth = -1;
	source->SeekTo( 0 );
	while (source->GetNode(nodeName))
	{
		int currentDepth = source->GetDepth();
		for (; depth > currentDepth; --depth)
		{
			target->EndClass();
		}
		if ((0 > cancelledDepth) || (currentDepth <= cancelledDepth))
		{
			cancelledDepth = -1;
			int attributesCount = source->GetNumAttributes();
			attributes.reserve(attributesCount);
			attributes.clear();
			bool cancelled = false;
			while (!cancelled && attributesCount--)
			{
				StdString value;
				StdString tag = source->GetCurrentNodeName();
				source->Read(value, tag);
				cancelled = !transform(nodeName, value, tag);
				if (!cancelled)
				{
					attributes.push_back(make_pair(value, tag));
				}
			}
			if (cancelled)
			{
				cancelledDepth = depth;
			}
			else
			{
				target->StartClass(nodeName);
				++depth;
				vector< pair<StdString, StdString> >::const_iterator itAttributes;
				for (itAttributes = attributes.begin(); 
					 itAttributes != attributes.end();
					 ++itAttributes)
				{
					target->Write(itAttributes->first, itAttributes->second);
				}
			}
		}
	}
	while (depth--)
	{
		target->EndClass();
	}
	source->Close();
	target->SetIsWriting( false );
	return target;
}

/// \brief	Create XMLArchive stream
/// \param	mode - creation mode for the stream. Default mode is read-write
/// \return	pointer to the created archive
///			caller is a owner of returned archive and responsible for closing
IXMLArchive *CreateXMLArchiveStream(DWORD mode = STREAM_MODE_READ | STREAM_MODE_WRITE);

/// \brief	Create XMLArchive
/// \param	path - path to archive file
/// \param	mode - creation mode for the stream. Default is read mode
/// \return	pointer to the created archive
///			caller is a owner of returned archive and responsible for closing
IXMLArchive *CreateXMLArchive(LPCTSTR path, DWORD mode = STREAM_MODE_READ);

/// \brief	Create memory archive
///			Returned archive is opened for reading
/// \param	buffer - pointer to buffer with data
/// \param	size - size of the buffer
///	\return	pointer to created archive
///			caller is a owner of returned archive and responsible for closing
IArchive* CreateMemoryArchive(void *buffer = NULL, DWORD size = 0);

///	\brief	copies content of source xml archive to target archive
///	\param	source - pointer to source IXMLArchive
///	\param	target - pointer to target IXMLArchive
void CopyXMLArchive(IXMLArchive *source, IXMLArchive *target);

}; // namespace GDS_SDK_Helpers

#endif //_GDS_ARCHIVE_HELPERS_H_
