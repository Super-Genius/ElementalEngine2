///============================================================================
/// \file	StreamFactory.cpp
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
#include "Streamfactory.hpp"
#include "CFileStream.h"
#include "CMemStream.h"
#ifdef _MSC_VER
#include <sys/types.h>
#endif
#include <sys/stat.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON(CStreamFactory);
REGISTER_MESSAGE_HANDLER(CreateStream_File, OnCreateFileStream, CStreamFactory);
REGISTER_MESSAGE_HANDLER(CreateStream_Memory, OnCreateMemoryStream, CStreamFactory);
REGISTER_MESSAGE_HANDLER(CheckFileExists, OnCheckFileExists, CStreamFactory);

CStreamFactory::CStreamFactory() :
	m_CompType(_T("CStreamFactory"))
{
}

CStreamFactory::~CStreamFactory(void)
{
}

/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This class
IComponent *CStreamFactory::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CStreamFactory);
}

// message handler to create file stream
DWORD CStreamFactory::OnCreateFileStream(DWORD size, void *param)
{
	CREATESTREAM *cs;
	CFileStream *newFileStream;
	VERIFY_MESSAGE_SIZE(size, sizeof(CREATESTREAM));
	cs = (CREATESTREAM *)param;

	newFileStream = NULL;

	try
	{
		newFileStream = new 
				CFileStream((TCHAR *)cs->streamData, cs->mode);
	}
	catch(...)
	{
		if (newFileStream)
			delete newFileStream;
		return MSG_ERROR;
	}
	
	if (!newFileStream->IsOpen())
	{
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_FILE);
		EngineGetToolBox()->Log(LOGWARNING, _T("Unable to open file %s\n"), 
			(TCHAR *)cs->streamData);
		if (newFileStream)
			delete newFileStream;
		return MSG_ERROR;
	}

	cs->openStream = newFileStream;
	return MSG_HANDLED_STOP;
	
}

// message handler to create memory stream
DWORD CStreamFactory::OnCreateMemoryStream(DWORD size, void *param)
{
	CREATESTREAM *cs;
	CMemStream *newMemStream;
	VERIFY_MESSAGE_SIZE(size, sizeof(CREATESTREAM));
	cs = (CREATESTREAM *)param;

	try
	{
		newMemStream = new CMemStream();
	}
	catch(...)
	{
		return MSG_ERROR;
	}

	// initialize the memory stream
	newMemStream->Init(cs->streamSize, cs->streamData, cs->mode);
	cs->openStream = newMemStream;

	return MSG_HANDLED_STOP;

}

DWORD CStreamFactory::OnCheckFileExists(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CHECKFILEEXISTS));
	CHECKFILEEXISTS &cfe = *(CHECKFILEEXISTS*)param;

	STATSTRUCT statFile;
	cfe.bExists = (_tstat( cfe.pFilePath, &statFile ) == 0);
	if (cfe.bExists)
		return MSG_HANDLED_STOP;
	else
		return MSG_HANDLED_PROCEED;
}
