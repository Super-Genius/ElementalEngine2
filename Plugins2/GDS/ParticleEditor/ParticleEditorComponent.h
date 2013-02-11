///============================================================================
/// \file		ParticleEditorComponent.cpp
/// \brief		Declaration of ParticleEditor Editor Component
/// \date		08-12-2005
/// \author		D. Patrick Ghiocel
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

#ifndef _ParticleEditorCOMPONENT_HPP_
#define _ParticleEditorCOMPONENT_HPP_

class CParticleEditor;
class CParticleRenderView;

/// class so we can hook into messaging system of Elemental Engine
class CParticleEditorComponent : public IComponent
{
	SINGLETON(CParticleEditorComponent);
private:
	CParticleEditor *m_Parent;
	list<CParticleRenderView*> m_ViewList;

public:
	void SetParent(CParticleEditor *parent);
	static IComponent *Create(int nArgs, va_list argptr);
	void Serialize(IArchive &ar);
	IHashString *GetComponentType();
	bool IsKindOf(IHashString *compType);

	void RegisterView(CParticleRenderView * pView);
	void DeRegisterView(CParticleRenderView *pView);

	// ----------- Messages Functions ---------------//
	DWORD OnCmdMsg(DWORD size, void *param);
	DWORD OnCreate(DWORD size, void *param);
	DWORD OnGlobalUpdateTick(DWORD size, void *param);
	DWORD OnFileLoaded(DWORD size, void *param);
};

#endif // _ParticleEditorCOMPONENT_HPP_
