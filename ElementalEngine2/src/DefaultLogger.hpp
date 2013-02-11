///============================================================================
/// \file		DefaultLogger.hpp
/// \brief		Default logger.
/// \date		07-13-2005
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

#ifndef _DEFAULTLOGGER_HPP_
#define _DEFAULTLOGGER_HPP_

class CDefaultLogger : 	public IComponent
{
	SINGLETONCONSTRUCTOROVERRIDE(CDefaultLogger);
	CDefaultLogger();
public:
	virtual ~CDefaultLogger();

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar) {};

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType() { return &m_CompType; };

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	/// the component create function
	/// \return IComponent singleton of ourself
	static IComponent *Create(int nArgs, va_list argptr);

	/// message to handle default logging capabilities
	DWORD OnLogMessage(DWORD size, void *param);
	/// message to set the log level
	DWORD OnSetLogLevel(DWORD size, void *param);
	DWORD OnGetLogLevel(DWORD size, void *param);
private:
	void DisplayErrorCount();

	CHashString m_CompType;

	// Counts of the number of warnings and errors
	int m_NumWarnings;
	int m_NumErrors;
	int m_NumFatalErrors;

	// The log level. Only log messages of the given level or
	// will be output.
	DWORD m_LogLevel;
};

#endif // #ifndef _DEFAULTLOGGER_HPP_
