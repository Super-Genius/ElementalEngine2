///============================================================================
/// \file		TerrainComponent.hpp
/// \brief		Header file for Terrain Component hooks into EE
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

#ifndef _TERRAINCOMPONENT_HPP_
#define _TERRAINCOMPONENT_HPP_

class CTerrainEditor;

/// class so we can hook into messaging system of Elemental Engine
class CTerrainComponent : public IComponent
{
	SINGLETONCONSTRUCTOROVERRIDE(CTerrainComponent);
	CTerrainComponent();
private:
	CTerrainEditor *m_Parent;
	EDITABLELIST m_EditableObjects;
	IToolBox *m_ToolBox;
public:
	/// destructor for component
	~CTerrainComponent();

	void SetParent(CTerrainEditor *parent);
	static IComponent *Create(int nArgs, va_list argptr);
	void Serialize(IArchive &ar);
	IHashString *GetComponentType();
	bool IsKindOf(IHashString *compType);

	// ----------- Messages Functions ---------------//
	DWORD OnCmdMsg(DWORD size, void *param);
	DWORD OnCreate(DWORD size, void *param);
	DWORD OnCreateObject(DWORD size, void *param);
	DWORD OnDeleteObject(DWORD size, void *param);
	DWORD OnLeftClickEditorObject(DWORD size, void *param);

	/// Message handler for when world is modification flag is set
	/// \param size = size of message, in this case sizeof(BOOL)
	/// \param param = BOOL which is whether world is modified or not
	DWORD OnWorldModified(DWORD size, void *param);
	/// Message for when the world has just been saved
	/// \param size = n/a
	/// \param param = n/a
	DWORD OnWorldSave(DWORD size, void *param);

	/// message handler when the world file is about to be saved as
	/// \param size = size of WORLDCHANGEFILENAMEPARAMS struct
	/// \param param = pointer to WORLDCHANGEFILENAMEPARAMS which contains
	///					new name and old name
	DWORD OnWorldChangeFileName(DWORD size, void *param);

	/// message handler when the mouse moves
	/// \param size = size of MOUSECOORDS struct
	/// \param param = pointer to MOUSECOORDS
	DWORD OnMouseMove(DWORD size, void *param);

	/// message handler when global update tick called
	/// \param size = size of GLOBALTICKPARAMS struct
	/// \param param = pointer to GLOBALTICKPARAMS
	DWORD OnGlobalUpdateTick(DWORD size, void *param);

	/// message handler when set sector modified message is sent
	/// \param size = size of IHashString
	/// \param param = pointer to IHashString
	DWORD OnSetSectorModified(DWORD size, void *param);

	/// message handler when set sector modified message is sent
	/// \param size = size of BRUSHPARAMS
	/// \param param = pointer to BRUSHPARAMS
	DWORD OnGetTerrainCursorBrush(DWORD size, void *param);
};

#endif // #ifndef _TERRAINCOMPONENT_HPP_
