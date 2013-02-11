///=====================================================================
/// \file	UndoHelpers.h
/// \brief	Set of helper functions and classes for easiest access to 
///			Undo functionality in GDS.
/// \date	10/03/2007
/// \author	Andrey Ivanov
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
///=====================================================================
#ifndef _GDS_UNDO_HELPERS_H_
#define _GDS_UNDO_HELPERS_H_

class IUndoRedoAction;

namespace GDS_SDK_Helpers
{
#define ID_GDS_EDIT_UNDO 50801
#define ID_GDS_EDIT_REDO 50802

///	\brief	create new undo stack
///	\param	stackName - name of the undo stack
void CreateUndoStack(IHashString *stackName);

///	\brief	delete undo stack
///	\param	stackName - name of the undo stack
void DeleteUndoStack(IHashString *stackName);

///	\brief	rename undo stack
///	\param	stackName - name of the undo stack
///	\param	newStackName - new name for the stack
void RenameUndoStack(IHashString *stackName, IHashString *newStackName);

///	\brief	register new association between view and undo stack
///	\param	view - pointer to CView
///	\param	stackName - name of the undo stack
void RegisterViewUndoStack(CView *view, IHashString *stackName);

///	\brief	clear undo stack
///	\param	stackName - name of the undo stack
///					stack of current view is cleared if this value is NULL 
void ClearUndoStack(IHashString *stackName);

///	\brief	wrapper for UndoStartCommand and UndoEndCommand messages
///			UndoStartCommand is called in constructor and UndoEndCommand 
///			is called in destructor
///	\sample	void DoObjectChanges()
///			{
///				GDS_SDK_Helpers::CUndoCommandGuard undoCommand;
///				CHashString hszObject1Name(_T("Entity_1"));
///				GDS_SDK_Helpers::CObjectsStateUndo undoObject1(hszObject1Name);
///				// changing Entity_1 properteis here
///			}
class CUndoCommandGuard
{
public:
	CUndoCommandGuard();
	~CUndoCommandGuard();
	void Cancel();
private:
	bool m_bCancelled;
};

///	\brief	wrapper for UndoSaveObject and RedoSaveObject messages
///	\sample	void DoObjectChanges()
///			{
///				GDS_SDK_Helpers::CUndoCommandGuard undoCommand;
///				CHashString hszObject1Name(_T("Entity_1"));
///				GDS_SDK_Helpers::CObjectsStateUndo undoObject1(hszObject1Name);
///				// changing Entity_1 properteis here
///			}
class CObjectsStateUndo
{
public:
	///	\brief	constructor of helper class
	///	\param	name - pointer to IHashString with name of object to process
	///					in undo/redo
	CObjectsStateUndo(IHashString *name);
	~CObjectsStateUndo();
private:
	CHashString	m_hszName;
};

///	\brief	wrapper for UndoSaveTexture and RedoSaveTexture messages
///	\sample	void DoTextureChanges()
///			{
///				GDS_SDK_Helpers::CUndoCommandGuard undoCommand;
///				CHashString hszTexturePath(_T("textures\terrain.tga"));
///				GDS_SDK_Helpers::CTextureDataUndo undoTexture(hszTexturePath);
///				// changing "textures\terrain.tga" texture data here
///			}
class CTextureDataUndo
{
public:
	///	\brief	constructor of helper class
	///	\param	name - pointer to IHashString with name of object to process
	///					in undo/redo
	CTextureDataUndo(IHashString *pFilePath);
	~CTextureDataUndo();
private:
	CHashString	m_hszFilePath;
};

///	\brief	helper class for simplify undo command filling. 
///			It helps for mouse based object editing
///	\sample	
///			CUndoObjectGroup undoGroup;
///			void OnMouseDown()
///			{
///				undoGroup.StartUndoCommand();
///				if (!processTextures)
///				{
///					undoGroup.AddUndoObject(name);
///				}
///				else
///				{
///					undoGroup.AddUndoTexture(name);
///				}
///			}
///			void OnMouseMove()
///			{
///				if (!processTextures)
///				{
///					undoGroup.AddUndoObject(name);
///				}
///				else
///				{
///					undoGroup.AddUndoTexture(name);
///				}
///			}
///			void OnMouseUp()
///			{
///				undoGroup.EndUndoCommand();
///			}
class CUndoObjectGroup
{
public:
	CUndoObjectGroup();
	~CUndoObjectGroup();

	///	\brief	helper for starting new undo command. This function may be called 
	///			multiple times in any suitable places
	void StartUndoCommand();
	///	\brief	helper for ending undo command. It may be placed in any suitable 
	///			places and without prior StartUndoCommand call
	void EndUndoCommand();
	///	\brief	cancel started undo command
	void CancelUndoCommand();

	///	\brief	helper for adding object to undo stack. 
	///			Name hash is placed in the object list for filling redo data also
	///			The same name may be passed multiple times before EndUndoCommand
	///			call, only first call with such name does state saving
	///	\param	name - pointer to IHashStirng with object name
	///	\return	true if object state was added to stack successfully
	bool AddUndoObject(IHashString *name);
	///	\brief	helper for adding texture to undo stack. 
	///			Name hash is placed in the object list for filling redo data also
	///	\param	name - pointer to IHashStirng with object name
	///	\return	true if texture data was added to stack successfully
	bool AddUndoTexture(IHashString *name);

	///	\brief	check if object was placed to list for redo operations
	///	\param	name - pointer to IHashStirng with object name
	///	\return	true if object data is present in the list
	bool HasUndoObject(IHashString *name) const;

	///	\brief	helper for registering action for adding texture to redo stack 
	///			on EndCommand call. Name hash is placed in the object list for 
	///			filling redo data
	///	\param	name - pointer to IHashStirng with object name
	void RegisterRedoTexture(IHashString *name);

private:
	///	\brief	helper for processing list with name hashes
	///	\param	msgHash - EE message hash
	///	\param	list - reference to list with string hashes for processing
	static void SendUndoMessages(DWORD msgHash, const vector<DWORD> &list);

private:
	bool m_bUndoCommand;
	vector<DWORD> m_UndoObjects;
	vector<DWORD> m_UndoTextures;
};

///	\brief	add selection action to undo stack. It frees passed action
///			if action was not added to the stack
///	\param	action - pointer to action object
///	\return	EE return code for added message
DWORD AddUndoAction(IUndoRedoAction *action);

///	\brief	add selection action to redo stack. It frees passed action
///			if action was not added to the stack
///	\param	action - pointer to action object
///	\return	EE return code for added message
DWORD AddRedoAction(IUndoRedoAction *action);

///	\brief	add selection action to undo or redo stack
///	\param	name - name of the object
///	\param	bUndo - defines undo or redo stack as target of action
void UndoForSelectObject(IHashString *name, bool bUndo);

///	\brief	add SetSectorModified action to undo stack
///	\param	sectorName - name of the sector
void UndoForSectorModified(IHashString *sectorName);

///	\brief	add undo actions to stack for creating object on redo and deleting it on undo
///			this function should be called just after object creation
///	\param	parent - name of object's parent
///	\param	name - name of the object
///	\param	type - type of the created object. This value may be NULL, 
///					function determines object type in this case
void UndoForCreateObject(IHashString *parent, IHashString *name, IHashString *type);

///	\brief	add undo actions to stack for deleting object on redo and recreating
///			it on undo. This function should be called before object deleting
///	\param	name - name of the object
///	\param	parent - name of object's parent. This value is used when 
///						  object is not in the hierarchy
///	\param	type - type of the created object. This value may be NULL, 
///					function determines object type in this case. This value is 
///					used when object is not in the hierarchy
void UndoForDeleteObject(IHashString *name, IHashString *parent = NULL, IHashString *type = NULL);

///	\brief	add undo actions to undo stack for renaming object
///	\param	name - name of the object
///	\param	newName - new name of the object
void UndoForRenameObject(IHashString *name, IHashString *newName);

///	\brief	add undo actions to undo stack for changing parent
///	\param	name - name of the object
///	\param	oldParent - current parent of the object
///	\param	newParent - new parent of the object
void UndoForChangeParent(IHashString *name, IHashString *oldParent, IHashString *newParent);

///	\brief	add undo actions to undo stack for last spawned file
///	\return	hash string with name of the spawned object
CHashString UndoForObjectSpawns();

} // namespace GDS_SDK_Helpers

#endif //_GDS_UNDO_HELPERS_H_
