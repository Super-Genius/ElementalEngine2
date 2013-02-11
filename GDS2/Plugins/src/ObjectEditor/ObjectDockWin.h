///============================================================================
/// \file		ObjectDockWin.h
/// \brief		Header file for <insert brief description here>
/// \date		07-15-2005
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

#ifndef _OBJECTDOCKWIN_H_
#define _OBJECTDOCKWIN_H_

// ObjectDockWin
class ObjectDockWin : public CGuiControlBar
{
	DECLARE_DYNAMIC(ObjectDockWin)

public:
	ObjectDockWin();
	virtual ~ObjectDockWin();

	/// Create an option tree view for a new object
	/// This function needs to be called if a new object is being created.
	/// Because there may be default values in the xml (i.e. files, combo lists), 
	/// this function will make sure to serialize these values into the 
	/// corresponding EE object.
	/// \param *name - ptr to the IHashString name of this object
	/// \param *compType - ptr to the IHashString component type of this object
	void CreateNewOptionTree(IHashString *name, IHashString *compType);

	/// Create an option tree view for a restored object
	/// \param *name - ptr to the IHashString name of this object
	/// \param *compType - ptr to the IHashString component type of this object
	void RestoreOptionTree(IHashString *name, IHashString *compType);

	/// Update the option tree view w/ data from the Elemental Engine
	/// This function will serialize data from the object in EE that's currently displayed 
	/// and write it into the option tree.
	/// \param *name - ptr to the IHashString name of this object
	/// \param *compType - ptr to the IHashString component type of this object
	void UpdateOptionTree(IHashString *name, IHashString *compType);

	/// \brief	Update the option tree view with data from the Elemental Engine
	///			This function will update data for selected object
	void UpdateOptionTree();

	void MouseMoved(float mouseX, float mouseY, long posX, long posY, long deltaX, long deltaY, IHashString *selectedObject);

	void ClearOptionTree(void);
	bool MoveObjectToCamera( IHashString *pObjectName, float fDistanceScalar );
	bool MoveCameraToObject( IHashString *pObjectName, float fDistanceScalar );

	bool ClipboardCopyObjects( IHashString *pRootObject );
	bool ClipboardPasteObjects( IHashString *pRootObject );

public:
	CObjectToolPal m_ObjectToolPal;
	/// Paint Object Bar
	CPaintObjectBar m_PaintBar;

protected:
	IView	   *m_OptionTreeView;
	IToolBox   *m_ToolBox;
	CHashString	m_ObjectName;
	CHashString	m_ComponentName;
	int			m_iBBoxMode;
	StdString	m_szBasePath;
	CHashString m_ccamTypeName;
	
	IXMLArchive *m_DefaultObjectArchive;

	CClipboardVisitor m_ClipboardVisitor;

protected:
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateProperties(NMHDR* pNotifyStruct, LRESULT* plResult);

	afx_msg void OnObjectCreate();
	afx_msg void OnObjectDelete();
	afx_msg void OnObjectCopy();
	afx_msg void OnObjectPaste();
	afx_msg void OnUpdateObjectCreate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditClear(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnBoundingboxNone();
	afx_msg void OnUpdateBoundingboxNone(CCmdUI *pCmdUI);
	afx_msg void OnBoundingboxRenderbox();
	afx_msg void OnUpdateBoundingboxRenderbox(CCmdUI *pCmdUI);
	afx_msg void OnBoundingboxPhysicsbox();
	afx_msg void OnUpdateBoundingboxPhysicsbox(CCmdUI *pCmdUI);
	afx_msg void OnLockNone();
	afx_msg void OnUpdateLockNone(CCmdUI *pCmdUI);
	afx_msg void OnLockXy();
	afx_msg void OnUpdateLockXy(CCmdUI *pCmdUI);
	afx_msg void OnLockXz();
	afx_msg void OnUpdateLockXz(CCmdUI *pCmdUI);
	afx_msg void OnLockYz();
	afx_msg void OnUpdateLockYz(CCmdUI *pCmdUI);
	afx_msg void OnObjectMove();
	afx_msg void OnUpdateObjectMove(CCmdUI *pCmdUI);
	afx_msg void OnObjectRotate();
	afx_msg void OnUpdateObjectRotate(CCmdUI *pCmdUI);
	afx_msg void OnObjectScale();
	afx_msg void OnUpdateObjectScale(CCmdUI *pCmdUI);
	afx_msg void OnUpdateObjectsEnablecollisions(CCmdUI *pCmdUI);
	afx_msg void OnObjectsEnablecollisions();
	afx_msg void OnHugNegXObject();
	afx_msg void OnUpdateHugNegXObject(CCmdUI *pCmdUI);
	afx_msg void OnHugNegYObject();
	afx_msg void OnUpdateHugNegYObject(CCmdUI *pCmdUI);
	afx_msg void OnHugNegZObject();
	afx_msg void OnUpdateHugNegZObject(CCmdUI *pCmdUI);
	afx_msg void OnHugNone();
	afx_msg void OnUpdateHugNone(CCmdUI *pCmdUI);
	afx_msg void OnHugPosXObject();
	afx_msg void OnUpdateHugPosXObject(CCmdUI *pCmdUI);
	afx_msg void OnHugPosYObject();
	afx_msg void OnUpdateHugPosYObject(CCmdUI *pCmdUI);
	afx_msg void OnHugPosZObject();
	afx_msg void OnUpdateHugPosZObject(CCmdUI *pCmdUI);
	afx_msg void OnObjectZoomto();
	afx_msg void OnUpdateObjectZoomto(CCmdUI *pCmdUI);
	afx_msg void OnLockX();
	afx_msg void OnUpdateLockX(CCmdUI *pCmdUI);
	afx_msg void OnLockY();
	afx_msg void OnUpdateLockY(CCmdUI *pCmdUI);
	afx_msg void OnLockZ();
	afx_msg void OnUpdateLockZ(CCmdUI *pCmdUI);
	afx_msg void OnPaintingPaint();
	afx_msg void OnUpdatePaintingPaint(CCmdUI *pCmdUI);
	afx_msg void OnPaintingErase();
	afx_msg void OnUpdatePaintingErase(CCmdUI *pCmdUI);
	afx_msg void OnPaintingLevelterrain();
	afx_msg void OnUpdatePaintingLevelterrain(CCmdUI *pCmdUI);
	afx_msg void OnPaintingRotation();
	afx_msg void OnUpdatePaintingRotation(CCmdUI *pCmdUI);
	afx_msg void OnPaintingScale();
	afx_msg void OnUpdatePaintingScale(CCmdUI *pCmdUI);

	// Menu Item click action handler: it shows/hides the toolbar
	afx_msg void OnViewObjectProperties();

	// enables/checks the menu item of the toolbar. (always enabled, checked if visible)
	afx_msg void OnUpdateViewObjectProperties(CCmdUI* pCmdUI);

private:
	bool CanCreateObject() const;
	bool CanDeleteObject() const;
};


#endif // #ifndef _OBJECTDOCKWIN_H_
