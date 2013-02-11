///============================================================================
/// \file		NodeComponent.h
/// \brief		Header file for node system editor component
/// \date		02-14-2008
/// \author		Richard Nguyen
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

#ifndef _NODECOMPONENT_H
#define _NODECOMPONENT_H

class CNodeSystemEditor;

class CNodeComponent : public IComponent
{
	SINGLETON(CNodeComponent);
private:
	CNodeSystemEditor *m_Parent;

public:
	void SetParent(CNodeSystemEditor *parent);
	static IComponent *Create(int nArgs, va_list argptr);
	void Serialize(IArchive &ar);
	IHashString *GetComponentType();
	bool IsKindOf(IHashString *compType);

	DWORD OnCmdMsg(DWORD size, void *param);
	DWORD OnCreate(DWORD size, void *param);
	DWORD OnSelectObject(DWORD size, void *param);
	DWORD OnLeftClickObject(DWORD size, void *param);
};

#endif // #ifndef _NODECOMPONENT_H