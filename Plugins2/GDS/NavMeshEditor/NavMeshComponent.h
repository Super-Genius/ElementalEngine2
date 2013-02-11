///============================================================================
/// \file		NavMeshComponent.h
/// \brief		Declaration of NavMesh Editor Component
/// \date		09-07-2005
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

#ifndef _NAVMESHCOMPONENT_HPP_
#define _NAVMESHCOMPONENT_HPP_

class CNavMeshEditor;

/// class so we can hook into messaging system of Elemental Engine
class CNavMeshComponent : public IComponent
{
	SINGLETON(CNavMeshComponent);
private:
	CNavMeshEditor *m_Parent;
public:
	void SetParent(CNavMeshEditor *parent);
	static IComponent *Create(int nArgs, va_list argptr);
	void Serialize(IArchive &ar);
	IHashString *GetComponentType();
	bool IsKindOf(IHashString *compType);

	// ----------- Messages Functions ---------------//
	DWORD OnCmdMsg(DWORD size, void *param);
	DWORD OnCreate(DWORD size, void *param);
};

#endif // _NAVMESHCOMPONENT_HPP_