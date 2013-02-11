///============================================================================
/// \file		SceneComponent.hpp
/// \brief		Header file for Scene Component hooks into EE
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

#ifndef _SCENECOMPONENT_HPP_
#define _SCENECOMPONENT_HPP_

class CSceneEditor;

typedef map<DWORD, StdString> BBOXMAP;

/// class so we can hook into messaging system of Elemental Engine
class CSceneComponent : public IComponent
{
	//SINGLETON(CSceneComponent);
	SINGLETONCONSTRUCTOROVERRIDE(CSceneComponent);
private:
	CSceneDLDockWin *m_Parent;
	CSceneEditor *m_pSceneEditor;
	CSceneComponent();
public:
	void SetParent(CSceneDLDockWin *parent);
	static IComponent *Create(int nArgs, va_list argptr);
	void Serialize(IArchive &ar);
	IHashString *GetComponentType();
	bool IsKindOf(IHashString* compType);

	// ----------- Messages Functions ---------------//
	DWORD OnCmdMsg(DWORD size, void* param);
	DWORD OnCreate(DWORD size, void* param);
	DWORD OnCreateObject(DWORD size, void* param);
	/// when a .wxl file is loaded come here
	/// \param Size is size of LPTSTR
	/// \param param is a LPTSTR of the name of the file loaded
	DWORD OnWorldLoad(DWORD size, void* param);	
	DWORD OnLoadNewFile( DWORD size, void* param);
	/// Set the world has been modifed flag
	/// \param size = n/a
	/// \param param = n/a
	DWORD OnWorldModified( DWORD size, void* param);
	DWORD OnCreateScene( DWORD size, void* param);	
	DWORD OnCloseScene( DWORD size, void* param);	
	DWORD OnGetSceneName( DWORD size, void* param);
	/// Add object to hierarchy
	/// \param size - size of CREATEOBJECTPARAMS
	/// \param param - pointer to CREATEOBJECTPARAMS struct
	DWORD OnAddObject(DWORD size, void* param);
	/// Remove object from hierarchy
	/// \param size - size of a IHashString
	/// \param param - pointer to a IHashString
	DWORD OnRemoveObject(DWORD size, void* param);
	/// Select object in hierarchy
	/// \param size - size of a IHashString
	/// \param param - pointer to a IHashString
	DWORD OnSelectObject(DWORD size, void* param);
	/// Updates the tree control when an object is renamed
	DWORD OnChangeObjectName(DWORD size, void * param);
	/// Updates the tree control when an object's parent is changed
	DWORD OnChangeObjectParent(DWORD size, void * param);
	/// message to change the currently active scene
	DWORD OnSetActiveScene(DWORD size, void* data);
	/// message about the currently active scene hierarchy changes
	DWORD OnHierarchyChanged(DWORD size, void* data);
	/// global update tick
	DWORD OnGlobalUpdateTick(DWORD size, void* param);
	/// Gets the scene's save path
	DWORD OnGetSceneSavePath(DWORD size, void* param);
	///	check object label changes
	DWORD OnInitObject(DWORD size, void* param);
};

#endif // #ifndef _SCENECOMPONENT_HPP_
