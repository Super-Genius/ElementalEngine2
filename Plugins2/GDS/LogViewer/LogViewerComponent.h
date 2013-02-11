///============================================================================
/// \file		LogViewerComponent.h
/// \brief		Header file for LogViewerComponent
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

#ifndef _LOGVIEWERCOMPONENT_HPP_
#define _LOGVIEWERCOMPONENT_HPP_

/// class so we can hook into messaging system of Elemental Engine
class CLogViewerComponent : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CLogViewerComponent);
	CLogViewerComponent();

public:
	/// \brief	Default Destructor
	~CLogViewerComponent();

	static IComponent *Create(int nArgs, va_list argptr);
	void Serialize(IArchive &ar);
	IHashString *GetComponentType();
	bool IsKindOf(IHashString *compType);

	// Messages
	DWORD OnLogMessage( DWORD size, void *param );

private:
	IToolBox* m_ToolBox;

	// logging file
	FILE *m_LogFile;
};

#endif // #ifndef _LOGVIEWERCOMPONENT_HPP_
