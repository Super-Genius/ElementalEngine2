///============================================================================
/// \file		COptionTreeWrapper.h
/// \brief		Tree view which uses the COptionTree to display data as a properyt panel
///				Implements the IView interface
/// \date		06-08-2005
/// \author		Halbert Nakagawa
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

#ifndef _C_OPTIONTREEWRAPPER_H_
#define _C_OPTIONTREEWRAPPER_H_

class ViewFormatObject;

/// forward dec for typedef
class COptionTreeWrapper;
typedef void (COptionTreeWrapper::*OTCREATETYPEFUNC)(IArchive &ar, COptionTreeItem *item, bool read);

typedef map<DWORD, OTCREATETYPEFUNC> MAPTYPECREATEFUNC;
typedef map<DWORD, MAPTYPECREATEFUNC*> MAPTYPETOFUNCMAP;
typedef void (COptionTreeWrapper::*OTCREATEITEMFUNC)(CHashString compName, ViewFormatObject *vo, COptionTreeItem* root);
typedef map<DWORD, OTCREATEITEMFUNC> MAPTYPETOITEMFUNC;

typedef vector<VIEWOBJECTLIST*> OBJLISTVEC;
typedef map<DWORD, OBJLISTVEC> MAPVIEWOBJLIST;

typedef struct TREEINFO
{
	COptionTree* m_Tree;
	COptionTreeItem *m_Root;
	TREEINFO()
	{
		m_Tree = NULL;
		m_Root = NULL;
	}
}TREEINFO;

typedef map<DWORD, TREEINFO> MAPTREES;

/// \class COptionTreeWrapper
/// \brief Class holds and display's the a view of the property tree
///

class COptionTreeWrapper
{

public:
	/// \brief	default constructor... do not use
	COptionTreeWrapper();

	/// \brief	Constructor that initializes the COptionTree
	/// \param	dwStyle	- the style for the COptionTree
	/// \param	rcRect	- the RECT of the parent window view
	/// \param	*pParentWnd	- ptr to the parent window view
	/// \param	dwTreeOptions - COptionTree display options
	/// \param	nID - the id of the COptionTree
	/// \param	*basePath - CString ptr to the base path of the editor
	COptionTreeWrapper(DWORD dwStyle, RECT rcRect, CWnd* pParentWnd, DWORD dwTreeOptions, UINT nID, CString basePath);

	/// \brief	default deconstructor
	virtual ~COptionTreeWrapper();

	/// \brief	Initialize the COptionTree
	/// \param	fileName - the xml file to initialize the tree
	virtual void Init(const StdString &fileName);

	/// \brief	Deinitialize the COptionTree
	///			Will delete all elements in the tree
	virtual void DeInit();

	/// Serializes in and out of the tree, and creates a new tree if one doesn't exist yet
	/// \param objName = the name of the object to be serialized
	/// \param objType = the type of the object to be serialized (important, because it does a lookup
	///						on registered types loaded from the properties file)
	/// \param ar = archive to serialize in/out
	/// \param bRead = to read from archive (true) or write to archive (false)
	virtual void Serialize(CHashString objName, CHashString objType, IArchive &ar, bool bRead = false);

	/// Serializes in and out of the tree, and creates a new tree if one doesn't exist yet (vector version)
	/// \param objName = the name of the object to be serialized
	/// \param objTypes = a vector of object types to be serialized (important, because it does a lookup
	///						on registered types loaded from the properties file)
	///
	///			NOTE: this version takes a vector of objTypes, so you can serialize more than one object
	///				for displaying and modifying (this is used in the gui editor to display properties
	///				of an item instance as well as the properties of the object referenced)
	///				You can serialize into one big archive, and serialize out into two separate objects
	///
	/// \param ar = archive to serialize in/out
	/// \param bRead = to read from archive (true) or write to archive (false)
	virtual void Serialize(CHashString objName, vector<CHashString> objTypes, IArchive &ar, bool bRead = false);	

	/// Simply clears the view by displaying an empty tree
	void CollapseAll() { ExpandOneRoot(&CHashString(_T(""))); }

protected:
	//DECLARE_MESSAGE_MAP()

private:

	/// \brief	Serializes the spinner integer item in the tree
	/// \param	ar - the archive that is read from or written to
	/// \param  *item - pointer to a COptionTreeItem
	void SerializeSpinnerItemInt(IArchive &ar, COptionTreeItem *item, bool read = true);

	/// \brief	Serializes the spinner float item in the tree
	/// \param	ar - the archive that is read from or written to
	/// \param  *item - pointer to a COptionTreeItem
	void SerializeSpinnerItemFloat(IArchive &ar, COptionTreeItem *item, bool read = true);

	/// \brief	Serializes the spinner double item in the tree
	/// \param	ar - the archive that is read from or written to
	/// \param  *item - pointer to a COptionTreeItem
	void SerializeSpinnerItemDouble(IArchive &ar, COptionTreeItem *item, bool read = true);

	/// \brief	Serializes the editable integer item in the tree
	/// \param	ar - the archive that is read from or written to
	/// \param  *item - pointer to a COptionTreeItem
	void SerializeEditItemInt(IArchive &ar, COptionTreeItem *item, bool read = true);

	/// \brief	Serializes the editable float item in the tree
	/// \param	ar - the archive that is read from or written to
	/// \param  *item - pointer to a COptionTreeItem
	void SerializeEditItemFloat(IArchive &ar, COptionTreeItem *item, bool read = true);

	/// \brief	Serializes the editable double item in the tree
	/// \param	ar - the archive that is read from or written to
	/// \param  *item - pointer to a COptionTreeItem
	void SerializeEditItemDouble(IArchive &ar, COptionTreeItem *item, bool read = true);

	/// \brief	Serializes the editable string item in the tree
	/// \param	ar - the archive that is read from or written to
	/// \param  *item - pointer to a COptionTreeItem
	void SerializeEditItemString(IArchive &ar, COptionTreeItem *item, bool read = true);

	/// \brief	Serializes the static integer item in the tree
	/// \param	ar - the archive that is read from or written to
	/// \param  *item - pointer to a COptionTreeItem
	void SerializeStaticItemInt(IArchive &ar, COptionTreeItem *item, bool read = true);

	/// \brief	Serializes the static float item in the tree
	/// \param	ar - the archive that is read from or written to
	/// \param  *item - pointer to a COptionTreeItem
	void SerializeStaticItemFloat(IArchive &ar, COptionTreeItem *item, bool read = true);

	/// \brief	Serializes the static double item in the tree
	/// \param	ar - the archive that is read from or written to
	/// \param  *item - pointer to a COptionTreeItem
	void SerializeStaticItemDouble(IArchive &ar, COptionTreeItem *item, bool read = true);

	/// \brief	Serializes the static string item in the tree
	/// \param	ar - the archive that is read from or written to
	/// \param  *item - pointer to a COptionTreeItem
	void SerializeStaticItemString(IArchive &ar, COptionTreeItem *item, bool read = true);

	/// \brief	Serializes the static bool item in the tree
	/// \param	ar - the archive that is read from or written to
	/// \param  *item - pointer to a COptionTreeItem
	void SerializeStaticItemBool(IArchive &ar, COptionTreeItem *item, bool read = true);

	/// \brief	Serializes the radio item in the tree
	/// \param	ar - the archive that is read from or written to
	/// \param  *item - pointer to a COptionTreeItem
	void SerializeRadioItem(IArchive &ar, COptionTreeItem *item, bool read = true);

	/// \brief	Serializes the checkbox item in the tree
	/// \param	ar - the archive that is read from or written to
	/// \param  *item - pointer to a COptionTreeItem
	void SerializeCheckBoxItem(IArchive &ar, COptionTreeItem *item, bool read = true);

	/// \brief	Serializes the file item in the tree
	/// \param	ar - the archive that is read from or written to
	/// \param  *item - pointer to a COptionTreeItem
	void SerializeFileItem(IArchive &ar, COptionTreeItem *item, bool read = true);

	/// \brief	Serializes the combo item in the tree
	/// \param	ar - the archive that is read from or written to
	/// \param  *item - pointer to a COptionTreeItem
	void SerializeComboItem(IArchive &ar, COptionTreeItem *item, bool read = true);

	/// \brief	Serializes the color item in the tree
	/// \param	ar - the archive that is read from or written to
	/// \param *item - pointer to a COptionTreeItem
	void SerializeColorItem(IArchive &ar, COptionTreeItem *item, bool read = true);

	/// \brief	Clears the tree
	void ClearTree();

	/// \brief	Updates the tree with data from an archive
	///	\param	Archive - the archive to read data from
	void SerializeIn(IArchive &Archive, CHashString name);

	/// \brief	Writes data from the tree into an archive
	/// \param	Archive - the archive to write data to
	 void SerializeOut(IArchive &Archive, CHashString name);

	/// \brief	Convert an absolute path to a relative path.
	///	This function is used when setting up a COptionItemFile type.
	/// If the path is not absolute this returns the original path
	/// \param	fullPath - the absolute path to convert
	/// \return	The relative path
	bool GetRelativePath( CString &fullPath, CString &filePath );


	bool CreateTree(const StdString &compType, CHashString compName);

  	/// \brief	Creates a spinner item and adds it to the tree
  	///			Will also create and return ViewFormatObject containing 
  	///			formatting data for the created option tree item.
  	/// \param	ar - the xml archive to read data from
  	/// \return	ViewFormatObject* - returns a new ViewFormatObject
  	void CreateSpinnerItem(CHashString compName, ViewFormatObject *obj, COptionTreeItem* root);
  
  	/// \brief	Creates an editable item and adds it to the tree
  	///			Will also create and return ViewFormatObject containing 
  	///			formatting data for the created option tree item.
  	/// \param	ar - the xml archive to read data from
  	/// \return	ViewFormatObject* - returns a new ViewFormatObject
  	void CreateEditItem(CHashString compName, ViewFormatObject *obj, COptionTreeItem* root);
  
  	/// \brief	Creates a static item and adds it to the tree
  	///			Will also create and return ViewFormatObject containing 
  	///			formatting data for the created option tree item.
  	/// \param	ar - the xml archive to read data from
  	/// \return	ViewFormatObject* - returns a new ViewFormatObject
  	void CreateStaticItem(CHashString compName, ViewFormatObject *obj, COptionTreeItem* root);
  
  	/// \brief	Creates a hidden item and adds it to the tree
  	///			Will also create and return ViewFormatObject containing 
  	///			formatting data for the created option tree item.
  	/// \param	ar - the xml archive to read data from
  	/// \return	ViewFormatObject* - returns a new ViewFormatObject
  	void CreateHiddenItem(CHashString compName, ViewFormatObject *obj, COptionTreeItem* root);
  
  	/// \brief	Creates a checkbox item and adds it to the tree
  	///			Will also create and return ViewFormatObject containing 
  	///			formatting data for the created option tree item.
  	/// \param	ar - the xml archive to read data from
  	/// \return	ViewFormatObject* - returns a new ViewFormatObject
  	void CreateCheckBoxItem(CHashString compName, ViewFormatObject *obj, COptionTreeItem* root);
  
  	/// \brief	Creates a radio item and adds it to the tree
  	///			Will also create and return ViewFormatObject containing 
  	///			formatting data for the created option tree item.
  	/// \param	ar - the xml archive to read data from
  	/// \return	ViewFormatObject* - returns a new ViewFormatObject
  	void CreateRadioItem(CHashString compName, ViewFormatObject *obj, COptionTreeItem* root);
  
  	/// \brief	Creates a spinner item and adds it to the tree
  	/// \param	ar - the xml archive to read data from
  	/// \return	ViewFormatObject* - returns a new ViewFormatObject 
  	void CreateFileItem(CHashString compName, ViewFormatObject *obj, COptionTreeItem* root);
  
  	/// \brief	Creates a combo item and adds it to the tree
  	///			Will also create and return ViewFormatObject containing 
  	///			formatting data for the created option tree item.
  	/// \param	ar - the xml archive to read data from
  	/// \return	ViewFormatObject* - returns a new ViewFormatObject
  	void CreateComboItem(CHashString compName, ViewFormatObject *obj, COptionTreeItem* root);

	/// \brief	Creates a color item and adds it to the tree
	/// \param	ar - the xml archive to read data from
	void CreateColorItem(CHashString compName, ViewFormatObject *obj, COptionTreeItem* root);	

private:

	/// Previously expanded one particular root in multiroot tree; now just switches trees
	/// \param name = name of object to display properties of
	void ExpandOneRoot(CHashString name);

	/// map of trees (one per object)
	MAPTREES m_mTrees;

	/// information passed in from constructor
	DWORD m_Style;
	RECT m_Rect;
	CWnd *m_pParentWnd;
	DWORD m_TreeOptions;
	UINT m_ID;

	CControlSchema		m_ControlSchema;

	/// the base path 
	CString m_szBasePath;

	/// map of object lists
	MAPVIEWOBJLIST m_mViewOList;
	
	/// object names registered
	vector <CHashString> m_vRootNames;
	
	/// Map from coptionitem type to optionitem add function
	MAPTYPETOFUNCMAP m_TypeFuncMap;

	/// Map from coptionitem type to optiontreeitem create function
	MAPTYPETOITEMFUNC m_TreeItemCreateFuncs;

	/// elemental engine's toolbox
	IToolBox *	m_ToolBox;

	StdString	m_viewName;
};

#endif
