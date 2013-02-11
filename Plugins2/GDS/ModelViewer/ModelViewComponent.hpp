///============================================================================
/// \file		ModelViewComponent.hpp
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

#ifndef _GDS_MODELVIEWCOMONENT_HPP
#define _GDS_MODELVIEWCOMONENT_HPP

class CModelViewEditor;
class CModelViewRender;

/// class so we can hook into messaging system of Elemental Engine
class CModelViewComponent : public IComponent
{
	SINGLETON(CModelViewComponent);
public:
	void SetParent(CModelViewEditor *parent);
	static IComponent *Create(int nArgs, va_list argptr);
	void Serialize(IArchive &ar);
	IHashString *GetComponentType();
	bool IsKindOf(IHashString *compType);

	void PlayAnimationSequence(CModelViewRender *pView, int nAnimationID);
	void PlayAnimation(CModelViewRender *pView);

	// ----------- Messages Functions ---------------//
	DWORD OnGlobalUpdateTick(DWORD size, void *param);
	DWORD OnCmdMsg(DWORD size, void *param);
	DWORD OnCreate(DWORD size, void *param);
	DWORD OnEditModelView(DWORD size, void *param);
	DWORD OnFileLoaded(DWORD size, void *param);
	DWORD OnAnimationCallback(DWORD size, void *param);
	/// Create model thumbnail.
	DWORD OnCreateModelThumbnail(DWORD size, void *param);

private:
	CModelViewEditor				   *m_Parent;
};

#endif // #ifndef _GDS_MODELVIEWCOMONENT_HPP
