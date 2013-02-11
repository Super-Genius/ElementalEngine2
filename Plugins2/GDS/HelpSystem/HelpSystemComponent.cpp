///============================================================================
/// \file		HelpSystemComponent.cpp
/// \brief		Declaration of Help System Component.
/// \date		11-13-2007
/// \author		Dmitriy S. Sergeyev
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

#include "stdafx.h"

REGISTER_COMPONENT_SINGLETON(CHelpSystemComponent);

REGISTER_MESSAGE_HANDLER(TestHelpSystemDialog, OnTestHelpSystemDialog, CHelpSystemComponent);
REGISTER_MESSAGE_HANDLER(DisplayHelpTopic, OnDisplayHelpTopic, CHelpSystemComponent);

IComponent *CHelpSystemComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CHelpSystemComponent);
}

void CHelpSystemComponent::Serialize(IArchive &ar)
{
}

IHashString *CHelpSystemComponent::GetComponentType()
{
	static CHashString componentName(_T("CHelpSystemComponent"));
	return &componentName;
}

bool CHelpSystemComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

bool CHelpSystemComponent::InitHelpSystem()
{
	if (!m_helpCore.get())
	{
		m_helpCore.reset(new CHelpSystemCore(_T("hlp\\*.chm")));
	}
	return true;
}

bool CHelpSystemComponent::InitHelp()
{
	if (InitHelpSystem())
	{
		if ((m_helpDialogWindow.get() == NULL) || !m_helpDialogWindow->IsExists())
		{
			m_helpDialogWindow.reset(new CHelpDialogWindow(m_helpCore.get()));
			if (!m_helpDialogWindow->CreateHelpDialogWindow())
			{
				// TODO: Error, unable to create help window.
				m_helpDialogWindow.reset();
				return false;
			}
		}
		return true;
	}
	return false;
}

DWORD CHelpSystemComponent::OnTestHelpSystemDialog(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(0, size);

	if (InitHelp())
	{
		m_helpDialogWindow->PopupWindow();
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CHelpSystemComponent::OnDisplayHelpTopic(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(sizeof(DISPLAYHELPTOPICPARAMS), size);
	DISPLAYHELPTOPICPARAMS *dht = (DISPLAYHELPTOPICPARAMS*)param;

	if (InitHelp())
	{
		const CHelpSystemCore::CChm * pchm = m_helpCore->FindChmByName(dht->chmName);
		if (pchm)
		{
			m_helpDialogWindow->DisplayTopic(*pchm, dht->topicPath.GetString());
		}
		else
		{
			m_helpDialogWindow->DisplayResource(_T("chmnotfound.html"));
		}
		m_helpDialogWindow->PopupWindow();
	}

	return MSG_HANDLED_PROCEED;
}
