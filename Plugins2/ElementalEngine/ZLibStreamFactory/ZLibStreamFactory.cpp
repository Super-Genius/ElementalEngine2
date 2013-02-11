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
#include "ZLibStreamFactory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON(CZLibStreamFactory);
REGISTER_MESSAGE_HANDLER_PRIORITY(CreateStream_File, OnCreateFileStream, CZLibStreamFactory, 0x70000000 );
//REGISTER_MESSAGE_HANDLER_PRIORITY(CreateStream_Memory, OnCreateMemoryStream, CZLibStreamFactory, 0x70000000);
REGISTER_MESSAGE_HANDLER_PRIORITY(CheckFileExists, OnCheckFileExists, CZLibStreamFactory, 0x70000000 );
REGISTER_MESSAGE_HANDLER(CreateStream_ZipFile, OnCreateZipFileStream, CZLibStreamFactory);
REGISTER_MESSAGE_HANDLER(CreateStream_ZipMemory, OnCreateZipMemoryStream, CZLibStreamFactory);
REGISTER_MESSAGE_HANDLER(AddZipFile, OnAddZipFile, CZLibStreamFactory);
REGISTER_MESSAGE_HANDLER(EnableZipFileLoading, OnEnableZipFileLoading, CZLibStreamFactory);

CZLibStreamFactory::CZLibStreamFactory() :
	m_CompType(_T("CZLibStreamFactory"))
{
	m_bEnabled = false;
}

CZLibStreamFactory::~CZLibStreamFactory(void)
{
}

/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This class
IComponent *CZLibStreamFactory::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CZLibStreamFactory);
}

// message handler to create file stream
DWORD CZLibStreamFactory::OnCreateFileStream(DWORD size, void *param)
{
	if( m_bEnabled )
	{
		CREATESTREAM *cs = static_cast<CREATESTREAM *>(param);
		VERIFY_MESSAGE_SIZE(size, sizeof(*cs));

		CZipFileStream *pStream = NULL;
		cs->openStream = NULL;
		try
		{
			pStream = new CZipFileStream();
			if (pStream->Init(static_cast<const char*>(cs->streamData), cs->mode))
			{
				cs->openStream = pStream;
				return MSG_HANDLED_STOP;
			}
		}
		catch(...)
		{
		}
		delete pStream;
		return MSG_HANDLED_PROCEED;
	}
	else
	{
		return MSG_HANDLED_PROCEED;
	}
}

// message handler to create memory stream
DWORD CZLibStreamFactory::OnCreateMemoryStream(DWORD size, void *param)
{
	if( m_bEnabled )
	{
		return OnCreateZipMemoryStream( size, param );
	}
	else
	{
		return MSG_HANDLED_PROCEED;
	}

}

/// message handler to check if a file exists
/// \param size - size of a CHECKFILEEXISTS
/// \param param - pointer to a CHECKFILEEXISTS struct
/// \return MSG_XXX return value
DWORD CZLibStreamFactory::OnCheckFileExists(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CHECKFILEEXISTS));
	CHECKFILEEXISTS &cfe = *(CHECKFILEEXISTS*)param;

	StdString sPath = cfe.pFilePath;
	sPath.MakeSafeFileName();
	sPath.ConformSlashesForward();

	if( CZipFileStream::CheckFileExist( sPath.c_str() ) )
	{
		cfe.bExists = true;
		return MSG_HANDLED_STOP;
	}
	
	cfe.bExists = false;
	return MSG_HANDLED_PROCEED;
}

/// \brief	message handler to create PhysFS stream
/// \param	size = size of a CREATESTREAM structure
/// \param	param = pointer to CREATESTREAM structure
/// \return	EE message return code
DWORD CZLibStreamFactory::OnCreateZipFileStream(DWORD size, void *param)
{
	CREATESTREAM *cs = static_cast<CREATESTREAM *>(param);
	VERIFY_MESSAGE_SIZE(size, sizeof(*cs));

	CZipFileStream *pStream = NULL;
	cs->openStream = NULL;
	try
	{
		pStream = new CZipFileStream();
		if (pStream->Init(static_cast<const char*>(cs->streamData), cs->mode))
		{
			cs->openStream = pStream;
			return MSG_HANDLED_STOP;
		}
	}
	catch(...)
	{
	}
	delete pStream;
	return MSG_ERROR;
}

/// \brief	message handler to create zipped memory stream
/// \param	size = size of a CREATESTREAM structure
/// \param	param = pointer to CREATESTREAM structure
/// \return	EE message return code
DWORD CZLibStreamFactory::OnCreateZipMemoryStream(DWORD size, void *param)
{
	CREATESTREAM *cs = static_cast<CREATESTREAM *>(param);
	VERIFY_MESSAGE_SIZE(size, sizeof(*cs));

	cs->openStream = NULL;
	try
	{
		cs->openStream = new CZipMemoryStream(cs->mode);
		return MSG_HANDLED_STOP;
	}
	catch(...)
	{
		return MSG_ERROR;
	}
}

/// \brief	message handler to create zipped memory stream
/// \param	size - size of LPCTSTR
/// \param	param - pointer to string with path
/// \return	EE message return code
DWORD CZLibStreamFactory::OnAddZipFile(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(LPCTSTR));
	StdString sPath = static_cast<LPCTSTR>(param);
	sPath.MakeSafeFileName();
	//sPath.ConformSlashesForward();
	if (!CZipFileStream::AddSearchPath(sPath.c_str()))
	{
		return MSG_ERROR;
	}
	return MSG_HANDLED_STOP;
}

/// Enables ZLib Stream Creation
/// \param size - size of a bool
/// \param param - pointer to bool value
/// \return EE message return code
DWORD CZLibStreamFactory::OnEnableZipFileLoading( DWORD size, void* param )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	
	m_bEnabled = *(bool*)param;
	
	return MSG_HANDLED_STOP;
}