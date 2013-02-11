///============================================================================
/// \file		DatabaseComponent.hpp
/// \brief		Header file for <insert brief description here>
/// \date		04-22-2005
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

#ifndef _DATABASECOMPONENT_HPP_
#define _DATABASECOMPONENT_HPP_

class CDatabaseEditor;

/// class so we can hook into messaging system of Elemental Engine
class CDatabaseComponent : public IComponent
{
	SINGLETON(CDatabaseComponent);
private:
	CDatabaseEditor *m_Parent;
public:
	void SetParent(CDatabaseEditor *parent);
	static IComponent *Create(int nArgs, va_list argptr);
	void Serialize(IArchive &ar);
	IHashString *GetComponentType();
	bool IsKindOf(IHashString *compType);

	// ----------- Messages Functions ---------------//
	DWORD OnCmdMsg(DWORD size, void *param);
	DWORD OnCreate(DWORD size, void *param);
	// message that a _dxl file was loaded
	DWORD OnDatabaseLoad(DWORD size, void *param);
	// message that a _dbx file was loaded
	DWORD OnDefinedDatabaseLoad(DWORD size, void *param);

};

#endif // #ifndef _DATABASECOMPONENT_HPP_
