///============================================================================
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

#if !defined(AFX_OBJECTEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_OBJECTEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

// ObjectEditor.cpp : implementation file
//

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectEditor.h : header file
//
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

/////////////////////////////////////////////////////////////////////////////
// Message Param Definitions
/////////////////////////////////////////////////////////////////////////////
struct OBJECTGRIDTOOLDATA
{
	bool bGridActive;
	float fGridSize;
};
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CObjectEditor document

typedef struct FLOATPOINT
{
	float x;
	float y;
} FLOATPOINT;

/*! \brief This class represents the object editor plugin
 *
 *  The Object editor creates a toolbar to add/remove/edit objects and
 *  creates the Option Tree View to read/write object properties. 
 */
class CObjectEditor : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CObjectEditor);
	CObjectEditor();

// implementation
public:   
	~CObjectEditor();
    virtual CString *GetName();
	virtual DWORD GetPriority();
    virtual BOOL Initialize();

	/// \brief Updates/Clears the object document window 
	///
	/// clears the ObjDocWin if name or compType is NULL, otherwise updates it
	/// The 'Option Tree View' will be refreshed with to object specified
	/// by the parameters
	/// \param *name - ptr to the IHashString name of this object
	/// \param *compType - ptr to the IHashString component type of this object
	/// \return void
	void UpdateTree(IHashString *name, IHashString *compType);

	/// \brief returns the list of the selected object (Selected in the Hierarchy panel)
	SELECTEDOBJECTLIST *GetSelectedObjects() { return &m_SelectedObjects; }
	OBJECTNAMELIST *GetSelectedEditableObjects() { return &m_SelectedEditableObjects; }
	
	/// \brief Clears the list of the selected objects (Selected in the Hierarchy panel)
	/// Clears the list of the selected object
	void ClearSelectedObjects();

	/// \brief Removes one item from the list of the Selected objects. 
	///
	/// Removes the first item in the list, I guess that will be the one clicked last time. 
	void ClearSelectedObject();

//MESSAGE HANDLERS

	//---------------------------------------------------------------------
	// \brief	Called from the main windows OnCmdMsg function.
	//				Routes commands through doc/view architecture
	//\return		TRUE if handled
	//---------------------------------------------------------------------
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	/// \brief	OnCreate - Called from the main windows create function
	/// \param	CMDIFrameWnd *mainWnd = main frames CMDIFrameWnd pointer
	/// \return		.
	void OnCreate(CMDIFrameWnd *mainWnd);

	/// \brief	Sets the Selected object. Currently only 1 Item can be 
	/// selected at a time. 
	/// \param	IHashString *compType = Type of the component of the seleted object
	/// \param	IHashString *objName = The name of the object
	/// \param	bool bAdd = false = Not in use yet, only reserved
	/// \return:	Void
	void SetSelected( IHashString *compType, IHashString *objName, bool bAdd = false);
	void SetEditableSelected( IHashString * object, bool bAdd = false);

	/// Handles a mouse move message, paints/deforms if necessary
	/// \param pMouseMove - mouse movement info
	void MouseMove(MOUSECOORDS *pMouseMove );

	/// Listens for a global update tick to allow a redraw between terrain actions
	void OnGlobalUpdateTick();

	/// Are we in paint mode?
	/// \param void
	/// \return true if so, false if not
	PAINTMODE GetPaintMode() { return m_ObjectDockWin.m_PaintBar.GetPaintMode(); }

	/// Paint Objects 
	/// \param mouseX - normalize coord of mouse pos
	/// \param mouseY - normalize coord of mouse pos
	/// \return void
	void PaintObjects(float mouseX, float mouseY);

	/// Erase Objects 
	/// \param mouseX - normalize coord of mouse pos
	/// \param mouseY - normalize coord of mouse pos
	/// \return void
	void EraseObjects(float mouseX, float mouseY);

	/// Extract Extension from a filepath
	/// \param filepath - file to get extention from
	/// \return extension of filepath
	CString GetExtension(const TCHAR *filepath);

	/// Strip off extension from filename
	/// \param filepath - file to strip
	/// \return filename without its extension
	CString GetPathNameNoExt(const TCHAR *filepath);

	/// Load an object (brush) placed where the mouse is
	/// \param mouseX - normalize coord of mouse pos
	/// \param mouseY - normalize coord of mouse pos
	/// \return void
	void LoadObjectBrush(float mouseX, float mouseY);

	/// Level the terrain underneath the object
	/// \param name - name of the object
	/// \param hitPos - hit position
	/// \return true on success, false on failure
	bool LevelTerrainUnderObject(IHashString *name, Vec3 *hitPos);

	/// Paint/Erase object on single hit
	/// \param mouseX - normalize coord of mouse pos
	/// \param mouseY - normalize coord of mouse pos
	/// \return true if an object was erased during the call, false if not
	bool SingleClickPaint(float mouseX, float mouseY);

	/// Checks if this toolbar is active
	/// \return true if toolbar is active, false if not
	bool IsObjectToolActive();
	bool IsObjectGridToolActive() { return m_ObjectDockWin.m_ObjectToolPal.GetGridCheck(); }
	float GetObjectGridToolGridSize() { return m_ObjectDockWin.m_ObjectToolPal.GetSnapTo(); }
	int GetObjectMoveLockIndex() { return m_ObjectDockWin.m_ObjectToolPal.GetLockIndex(); }
	void SetObjectMoveLockIndex(LOCK_STATE lockIndex) { m_ObjectDockWin.m_ObjectToolPal.SetLockIndex(lockIndex); }
	int GetManipulateState() { return m_ObjectDockWin.m_ObjectToolPal.GetManipulateState(); }
	void SetManipulateState(MANIPULATE_STATE manipulateState) { return m_ObjectDockWin.m_ObjectToolPal.SetManipulateState(manipulateState); }

	void MoveObject( IHashString *pObjectName, Vec3 vecDelta, bool bUseMoveAccum, float mouseX, float mouseY );
	void MoveHugged( IHashString *selectedObject );

	///	\brief	helper function for saving object state to undo stack. 
	///			It starts new undo command as well
	///	\param	pObjectName - pointer to IHashString with name of saving object
	void AddUndoObject(IHashString *pObjectName);

	///	\brief	helper function for ending current undo command
	void EndUndoCommand();

	/// \brief	Update the option tree view with current data from the Elemental Engine
	void UpdateOptionTree()
	{
		m_ObjectDockWin.UpdateOptionTree();
	}

// attributes
private:
	/// Get terrain intersection at mouse position
	/// \param x - x component of position
	/// \param y - y component of position
	/// \param z - z component of position
	/// \return true on success, false on failure
	bool GetTerrainIntersection(float mouseX, float mouseY, float &x, float &y, float &z);

	/// Spawn Object at position
	/// \param name - file name of object
	/// \param x - x component of position
	/// \param y - y component of position
	/// \param z - z component of position 
	/// \param brush - is the object we are spawning the brush
	/// \return true on success, false on failure
	bool SpawnAtPosition(IHashString *name, float x, float y, float z, bool brush = false);

	/// Find the intersection against a mouse ray and a plane
	/// \param planePos - Plane position
	/// \param planeNorm - Plane normal
	/// \param mouseX - Mouse X normalized screen coordinate
	/// \param mouseY - Mouse Y normalized screen coordinate
	/// \param hitPos - Stores the ray-plane hit position
	bool PlaneMouseRayIntersection( const Vec3 &planePos, const Vec3 &planeNorm, float mouseX, float mouseY, Vec3 &hitPos );

private:
	UINT m_PageIndex;
	RESOURCECOLLECTION m_ObjCollection;
	ObjectDockWin m_ObjectDockWin;
	CMultiDocTemplate *m_DocTemplate;
	IToolBox *m_ToolBox;
	SELECTEDOBJECTLIST m_SelectedObjects;
	OBJECTNAMELIST m_SelectedEditableObjects;
	bool m_bGlobalUpdateTicked;
	unsigned int m_iRaycastTimer;
	/// name of the *file* that is the current brush
	CHashString m_CurrentBrush;
	/// "spawned" name of the brush mentioned above
	CHashString m_BrushName;

	CBitmapPreviewDlg bitmapDlg;
	CBitmap *m_ObjectFootprintBitmap;

	/// accumulator for grid object movement
	Vec3 m_vecMoveAccum;

	CUndoObjectGroup	m_undoGroup;
};

extern AFX_EXTENSION_MODULE ObjectEditorDLL;

#undef AFX_DATA
#define AFX_DATA

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTEditor_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
