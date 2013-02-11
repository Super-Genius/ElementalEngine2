///============================================================================
/// \file	DefaultLogger.cpp
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
#include "DefaultLogger.hpp"
#include "assert.h"


REGISTER_COMPONENT_SINGLETON(CDefaultLogger);

// default to the lowest priority (0xffffffff), so log message can be overridden
REGISTER_MESSAGE_HANDLER(LogMessage, OnLogMessage, CDefaultLogger);
REGISTER_MESSAGE_HANDLER(SetLogLevel, OnSetLogLevel, CDefaultLogger);

CDefaultLogger::CDefaultLogger() :
	m_CompType(_T("CDefaultLogger"))
{
	m_NumWarnings		= 0;
	m_NumErrors			= 0;
	m_NumFatalErrors	= 0;

	// Default to start logging at warnings
#if defined _DEBUG || defined _PROFILE
	m_LogLevel = LOGINFORMATION;
#else
	m_LogLevel = LOGWARNING;
#endif
}

CDefaultLogger::~CDefaultLogger()
{
#if defined _DEBUG || defined _PROFILE
	// Display the number of warnings and errors
	// if there are any.
	if (m_NumWarnings > 0 ||
		m_NumErrors > 0 ||
		m_NumFatalErrors > 0)
	{
		DisplayErrorCount();
	}
#endif
}

/// Create (Get) our singleon component
IComponent *CDefaultLogger::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CDefaultLogger);
}

bool CDefaultLogger::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

/// message to handle default logging capabilities
DWORD CDefaultLogger::OnLogMessage(DWORD size, void *param)
{
	LOGMESSAGESTRUCT *logMessage;
	// can't do a verify message size, as that might cause recursion
	if (size != sizeof(LOGMESSAGESTRUCT))
	{
		return MSG_ERROR;
	}

	logMessage = (LOGMESSAGESTRUCT *)param;

	// Filter out log messages with lower than specified priorities
	if( logMessage->severity < m_LogLevel )
	{
		return MSG_HANDLED_STOP;
	}

#if (defined(_DEBUG) || defined(_PROFILE))
	OutputDebugString(logMessage->msg);
#endif

	if( logMessage->severity == LOGWARNING)
	{
		++m_NumWarnings;
	}
	if (logMessage->severity == LOGERROR)
	{
		++m_NumErrors;
	}
	if (logMessage->severity == LOGFATALERROR)
	{
		++m_NumFatalErrors;
		assert(0);
	}

	return MSG_HANDLED_STOP;
}

/// message to set the log level
DWORD CDefaultLogger::OnSetLogLevel(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(DWORD));
	DWORD logLevel = *(DWORD*)param;

	m_LogLevel = logLevel;

	return MSG_HANDLED_STOP;
}

/// message to set the log level
DWORD CDefaultLogger::OnGetLogLevel(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(DWORD));
	DWORD logLevel = *(DWORD*)param;

	logLevel = m_LogLevel;

	return MSG_HANDLED_STOP;
}

void CDefaultLogger::DisplayErrorCount()
{
	TCHAR numWarnings[100];
	_stprintf( numWarnings, _T("%i Warnings.\n%i Errors.\n%i Fatal Errors.\n"), m_NumWarnings, m_NumErrors, m_NumFatalErrors );
	OutputDebugString( numWarnings );

#ifdef WIN32
	MessageBox( NULL, 
				numWarnings,
				_T("Reporting Number of Errors.\n"),
				MB_OK );
#endif //WIN32
}
