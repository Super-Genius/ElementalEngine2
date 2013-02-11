///============================================================================
/// \file		CArchiveFactory.cpp
/// \brief		Implementation file for factor to create IArchives
/// \date		05-30-2005
/// \author		Kenneth Hurley
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
#include "CArchiveFactory.hpp"
#include "CXMLArchive.h"
#include "CArchive.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON(CArchiveFactory);
REGISTER_MESSAGE_HANDLER(CreateXMLArchive, OnCreateXMLArchive, CArchiveFactory);
REGISTER_MESSAGE_HANDLER(CreateArchive, OnCreateArchive, CArchiveFactory);
//REGISTER_MESSAGE_HANDLER(CopyArchive, OnCopyArchive, CArchiveFactory);
// this is only temporary until testing can be completed.
REGISTER_MESSAGE_HANDLER(CreateXMLArchiveStream, OnCreateXMLArchiveStream, CArchiveFactory);

CArchiveFactory::CArchiveFactory() :
	m_CompType(_T("CArchiveFactory"))
{
	m_ToolBox = EngineGetToolBox();
}

CArchiveFactory::~CArchiveFactory(void)
{
}

/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CArchiveFactory::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CArchiveFactory);
}

// message to create a XML Archive
DWORD CArchiveFactory::OnCreateXMLArchive(DWORD size, void *param)
{
	StdString streamMessage;
	CREATEARCHIVE *ca;
	CXMLArchive *newXMLArchive;
	VERIFY_MESSAGE_SIZE(size, sizeof(CREATEARCHIVE));
	ca = (CREATEARCHIVE *)param;
	try
	{
		newXMLArchive = new CXMLArchive();
	}
	catch(...)
	{
		return MSG_ERROR;
	}

	bool bResult = newXMLArchive->Init((TCHAR *)ca->streamData, (
		ca->mode == STREAM_MODE_READ));

	if( bResult == false )
	{
		delete newXMLArchive;
		return MSG_ERROR;
	}

	ca->archive = newXMLArchive;

	return MSG_HANDLED_STOP;
}

DWORD CArchiveFactory::OnCreateXMLArchiveStream(DWORD size, void *param)
{
	IArchive *newArchive;
	CREATEARCHIVE *ca;
	CREATESTREAM cs;
	StdString csMessage;
	DWORD retVal;
	VERIFY_MESSAGE_SIZE(size, sizeof(CREATEARCHIVE));
	ca = (CREATEARCHIVE *)param;

	cs.streamData = ca->streamData;
	cs.streamSize = ca->streamSize;
	cs.mode = ca->mode;
	csMessage = _T("CreateStream_");
	csMessage += ca->streamType->GetString();
	// try and create the stream
	DWORD msgHash_csMessage = CHashString(csMessage).GetUniqueID();
	retVal = m_ToolBox->SendMessage(msgHash_csMessage, sizeof(CREATESTREAM), &cs);
	if (retVal != MSG_HANDLED)
	{
		return retVal;
	}

	try
	{
		newArchive = new CXMLArchive();
	}
	catch(...)
	{
		delete cs.openStream;
		return MSG_ERROR;
	}

	// check if we are creating a read and write stream
	if( (cs.mode & (STREAM_MODE_READ | STREAM_MODE_WRITE)) == (STREAM_MODE_READ | STREAM_MODE_WRITE))
	{
		newArchive->SetIsWriting( true );
	}

	// initalize it with the stream
	newArchive->Init(cs.openStream);

	ca->archive = newArchive;

	return MSG_HANDLED_STOP;
}

// message to create a Standard Archive from a stream
DWORD CArchiveFactory::OnCreateArchive(DWORD size, void *param)
{
	CArchive *newArchive;
	CREATEARCHIVE *ca;
	CREATESTREAM cs;
	StdString csMessage;
	DWORD retVal;
	VERIFY_MESSAGE_SIZE(size, sizeof(CREATEARCHIVE));
	ca = (CREATEARCHIVE *)param;

	cs.streamData = ca->streamData;
	cs.streamSize = ca->streamSize;
	cs.mode = ca->mode;
	csMessage = _T("CreateStream_");
	csMessage += ca->streamType->GetString();
	// try and create the stream
	DWORD msgHash_csMessage = CHashString(csMessage).GetUniqueID();
	retVal = m_ToolBox->SendMessage(msgHash_csMessage, sizeof(CREATESTREAM), &cs);
	if (retVal != MSG_HANDLED)
	{
		return retVal;
	}

	try
	{
		newArchive = new CArchive();
	}
	catch(...)
	{
		delete cs.openStream;
		return MSG_ERROR;
	}

	// initialize it with the stream
	newArchive->Init(cs.openStream);

	// check if we are creating a read and write stream
	if( (cs.mode & (STREAM_MODE_READ | STREAM_MODE_WRITE)) == (STREAM_MODE_READ | STREAM_MODE_WRITE))
	{
		// set to a write archive only if the ARCHIVE_MODE_WRITE flag is set
		if((cs.mode & ARCHIVE_MODE_WRITE)==ARCHIVE_MODE_WRITE )
		{
			newArchive->SetIsWriting( true );
		}
	}

	ca->archive = newArchive;

	return MSG_HANDLED_STOP;
}

