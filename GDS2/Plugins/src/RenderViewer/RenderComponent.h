///============================================================================
/// \file		RenderComponent.hpp
/// \brief		Header file for Render Viewer Component hooks into EE
/// \date		05-12-2005
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

#ifndef _RENDERCOMPONENT_HPP_
#define _RENDERCOMPONENT_HPP_

class CRenderViewer;

/// class so we can hook into messaging system of Elemental Engine
class CRenderViewerComponent : public IComponent
{
	//SINGLETON(CRenderViewerComponent);
	SINGLETONCONSTRUCTOROVERRIDE(CRenderViewerComponent);
	CRenderViewerComponent();

private:
	CRenderViewer *m_Parent;

public:
	void SetParent(CRenderViewer *parent);
	static IComponent *Create(int nArgs, va_list argptr);
	void Serialize(IArchive &ar);
	IHashString *GetComponentType();
	bool IsKindOf(IHashString *compType);

	// ----------- Messages Functions ---------------//
	DWORD OnCmdMsg(DWORD size, void *param);
	DWORD OnCreate(DWORD size, void *param);	
	DWORD OnUpdateTick(DWORD size, void * param);
	DWORD OnStopPlaying(DWORD size, void * param);
	DWORD OnSetCameraMoveSensitivity( DWORD size, void * param );
	DWORD OnLoadNewFile(DWORD size, void* param);
};

#endif // #ifndef _TERRAINCOMPONENT_HPP_
