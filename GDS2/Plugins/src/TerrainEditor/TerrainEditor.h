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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TerrainEditor.h : header file
//
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

/////////////////////////////////////////////////////////////////////////////
// CTerrainEditor document

typedef list<CBrushObject> BRUSHOBJECTLIST;
typedef set<DWORD> SECTORLIST;

class CTerrainEditor : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CTerrainEditor);
	CTerrainEditor();
// attributes
private:
	UINT m_PageIndex;
	RESOURCECOLLECTION m_ObjCollection;

// implementation
public:   
	~CTerrainEditor();
    virtual CString *GetName();
	virtual DWORD GetPriority();
    virtual BOOL Initialize();
	void CreateNewTerrain();
	/// creates a blank terrain
	static void TerrainCreationCallback(void *userData);

// message handlers
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);
	void OnCreate(CMDIFrameWnd *mainWnd);
	DWORD EditDocument(/*IArchive &ar */);

	void ClickTerrain( Vec3 *pHitPos, bool bRButton );
	void SetSelectedSector( IHashString *pSectorName );
	IHashString* GetSelectedSector() { return &m_SelectedSector; }
	/// Function to handle the message for world modified flag, so
	/// our code doesn't set it all the time
	/// \param isModified = whether modified or not
	void OnWorldModified(BOOL isModified);

	/// Function to handle the message for world saving
	void OnWorldSave();

	/// Function to controls the show state of the terrain properties window
	/// \param bShow - flag true to show window, false to hide window
	void ShowTerrainPropertiesDialog( bool bShow );

	/// function to handle world file save as, so internal files can be
	///	renamed before the actual save of the world
	/// \param nameParms, pointer to structure of new name and old name
	void OnWorldChangeName(WORLDCHANGEFILENAMEPARAMS *nameParms);

	/// set a sector as modified in our list
	/// \param sector = pointer to IObject of sector that was modified
	void SetSectorModified(IHashString *sector);

	/// set all sector as modified in our list
	void SetAllSectorsModified();

	/// gets the the brush object list
	BRUSHOBJECTLIST* GetBrushObjects( void );

	/// loads the brush objects from an xml file
	bool LoadBrushObjects( StdString wszBasePath, StdString wszXMLFileName );

	/// Gets max layer count
	/// \return - return max layer count
	int GetMaxLayers() { return m_TerrainOptionsDlg.GetMaxLayers(); }

	/// Inserts a layer into a sector if sector has fewer than max layers and if new layer is not currently
	/// present on the sector.
	/// \param pSectorName - name of sector to insert layer
	/// \param pTextureName - name of texture to insert as a layer
	/// \param pInsertLocation - name of existing layer to replace or "" if layer to be added to end
	/// \param fScaleX - x scale of layer
	/// \param fScaleY - y scale of layer
	/// \return - return if layer was inserted onto sector
	bool InsertTerrainSectorLayer( IHashString *pSectorName, IHashString *pTextureName, IHashString *pInsertLocationName, float fScaleX, float fScaleY );

	/// Handles a mouse move message, paints/deforms if necessary
	/// \param pMouseMove - mouse movement info
	void MouseMove( MOUSECOORDS *pMouseMove );

	///	\brief	set visible decal
	///	\param	pBrushFileName - pointer to IHashString with brush name
	///	\param	fSize - size of decal
	void SetDecal( IHashString *pBrushFileName, float fSize );

	///	\brief	clear decal from terrain.
	void ClearDecal();
	
	/// Listens for a global update tick to allow a redraw between terrain actions
	void OnGlobalUpdateTick();

	/// Returns current terrain cursor brush params
	/// \return current terrain cursor brush params
	BRUSHPARAMS GetTerrainCursorBrush();

	///	\brief	save whole terrain data to undo stack
	///	\param	undo - flag for placing terrain data as undo or redo actions
	void DoAllSectorsUndo(bool undo);

	static void GetSavePathName(StdString &out_Path, StdString &out_Name);

	///	\brief	get name of render object for terrain sector
	///	\param	pointer to IHashString with name of the sector
	///	\return	hash string with name of sector's child object with type
	///			CTerrainSectorRenderObject
	static CHashString GetSectorRenderObject(IHashString *pSectorName);

	// public, because terrainToolPal needs these information, perhaps a getter would be nicer solution.
	CHashString* getBrushName(){ return &m_BrushName; }
	CHashString* getBrushTypeName() { return &m_BrushTypeName; } 


private:
	///	\brief	get heightmap path from terrain sector name
	///	\param	pSectorName - pointer to hash string with sector name
	///	\return	path with heightmap
	CHashString GetHeightmapPath(IHashString *pSectorName) const;

	///	\brief	get mask texture list from terrain render object by sector name
	///	\param	pSectorName - pointer to hash string with sector name
	///	\return	list with mask texture names
	TERRAINLAYERMASKVEC GetMaskTexturePaths(IHashString *pSectorName) const;

// attributes
private:
	CTerrainLayerMatDocWnd	m_TerrainLayerMatDocWnd;
	CTerrainOptionsDlg m_TerrainOptionsDlg;
	CTerrainToolPal m_TerrainToolPal;
	CTerrainParametersDialog m_TerrainPropDlg;
	CTerrainSizeDlg		m_NewTerrainDlg;
	IToolBox *m_ToolBox;
	CHashString m_SelectedSector;
	BOOL m_bWorldIsModified;
	SECTORLIST m_SectorModifiedList;
	CSectorRenameVisitor m_SectorRenameVisitor;
	BRUSHOBJECTLIST m_vBrushObjects;
	unsigned int m_iRaycastTimer;
	bool m_bGlobalUpdateTicked;

	Vec3 m_vecBrushPosition;
	CHashString m_BrushName;
	CHashString m_BrushTypeName;

	CUndoObjectGroup m_undoGroup;

	// true, if decal was cleared (its size is set to 0)
	bool m_bDecalCleared;
};

extern AFX_EXTENSION_MODULE TerrainEditorDLL;

#undef AFX_DATA
#define AFX_DATA
