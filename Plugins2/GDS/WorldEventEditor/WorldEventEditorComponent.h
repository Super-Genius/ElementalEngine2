///============================================================================
/// \file		WorldEventEditorComponent.h
/// \brief		Declaration of WorldEventEditor Component
/// \date		01-30-2007
/// \author		Brian Bazyk
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

#ifndef _WORLDEVENTEDITORCOMPONENT_HPP_
#define _WORLDEVENTEDITORCOMPONENT_HPP_

class CWorldEventEditor;

/// class so we can hook into messaging system of Elemental Engine
class CWorldEventEditorComponent : public IComponent
{
	SINGLETON(CWorldEventEditorComponent);
private:
	CWorldEventEditor *m_Parent;
public:
	void SetParent(CWorldEventEditor *parent);
	static IComponent *Create(int nArgs, va_list argptr);
	void Serialize(IArchive &ar);
	IHashString *GetComponentType();
	bool IsKindOf(IHashString *compType);

	// ----------- Messages Functions ---------------//
	DWORD OnCmdMsg(DWORD size, void *param);
	DWORD OnCreate(DWORD size, void *param);
	DWORD OnSelectObject(DWORD size, void *param);
};

#endif // _WORLDEVENTEDITORCOMPONENT_HPP_