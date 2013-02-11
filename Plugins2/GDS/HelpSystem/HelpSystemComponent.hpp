///============================================================================
/// \file		HelpSystemComponent.hpp
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

#ifndef _HELPSYSTEMCOMPONENT_HPP_
#define _HELPSYSTEMCOMPONENT_HPP_

class CHelpSystemComponent : public IComponent
{
	SINGLETON(CHelpSystemComponent);

public:
	/// Get singletion instance for this.
	static IComponent *Create(int nArgs, va_list argptr);

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	// ----------- Messages Functions ---------------//
	DWORD OnTestHelpSystemDialog(DWORD size, void *param);
	DWORD OnDisplayHelpTopic(DWORD size, void *param);

private:
	/// Initialize help system core.
	bool InitHelpSystem();

	/// Initialize help system and create help dialog window.
	bool InitHelp();

	/// Help system core instance.
	std::auto_ptr<CHelpSystemCore> m_helpCore;
	/// Help dialog window.
	std::auto_ptr<CHelpDialogWindow> m_helpDialogWindow;
};

#endif // #ifndef _HELPSYSTEMCOMPONENT_HPP_
