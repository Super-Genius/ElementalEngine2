///============================================================================
/// \file		LogViewerComponent.cpp
/// \brief		LogViewerComponent implementation.
/// \date		02-15-2008
/// \author		Kyle Swaim
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
#include "LogViewerComponent.h"

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON(CLogViewerComponent);

// Register EE Messages
REGISTER_MESSAGE_HANDLER_PRIORITY(LogMessage, OnLogMessage, CLogViewerComponent, 0x70000000);

// get the singleton for this
IComponent* CLogViewerComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CLogViewerComponent);
}

CLogViewerComponent::CLogViewerComponent()
{
	m_ToolBox = EngineGetToolBox();
	m_LogFile = NULL;
}

CLogViewerComponent::~CLogViewerComponent()
{
	if (m_LogFile != NULL)
	{
		fclose(m_LogFile);
	}
}

// serialize function for this object
void CLogViewerComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CLogViewerComponent::GetComponentType()
{
	static CHashString compName(_T("CLogViewerComponent"));
	return &compName;
}

bool CLogViewerComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

DWORD CLogViewerComponent::OnLogMessage(DWORD size, void *param)
{
	LOGMESSAGESTRUCT *logMessage;
	// can't do a verify message size, as that might cause recursion
	if (size != sizeof(LOGMESSAGESTRUCT))
	{
		return MSG_ERROR;
	}

	logMessage = (LOGMESSAGESTRUCT *)param;

	if (m_LogFile == NULL)
	{
		m_LogFile = fopen(_T("GDS.log"), "w+");
		if (m_LogFile == NULL)
		{
//				TerminateApp(1);
		}
	}

	fprintf(m_LogFile,logMessage->msg);
	fflush(m_LogFile);
	if (logMessage->severity == LOGFATALERROR)
	{
		fclose(m_LogFile);
		static DWORD msgHash_Exit = CHashString(_T("Exit")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_Exit, 0, NULL);
//			TerminateApp(1);
	}
	return MSG_HANDLED_PROCEED;
}