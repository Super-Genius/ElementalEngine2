///============================================================================
/// \file		COptionTreeView.h
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

#ifndef COPTIONTREEVIEW_H
#define COPTIONTREEVIEW_H

class COptionTreeView;
class ViewFormatObject;

typedef void (COptionTreeView::*OTCREATETYPEFUNC)(IArchive &ar, COptionTreeItem *item, bool read);

typedef map<DWORD, OTCREATETYPEFUNC> MAPTYPECREATEFUNC;
typedef map<DWORD, MAPTYPECREATEFUNC*> MAPTYPETOFUNCMAP;
typedef void (COptionTreeView::*OTCREATEITEMFUNC)(ViewFormatObject *vo);
typedef map<DWORD, OTCREATEITEMFUNC> MAPTYPETOITEMFUNC;

/// \class COptionTreeView
/// \brief Class holds and display's the a view of the property tree
///

class COptionTreeView : public IView
{

public:
	/// \brief	default constructor
	COptionTreeView();

	/// \brief	Constructor that initializes the COptionTree
	/// \param	dwStyle	- the style for the COptionTree
	/// \param	rcRect	- the RECT of the parent window view
	/// \param	*pParentWnd	- ptr to the parent window view
	/// \param	dwTreeOptions - COptionTree display options
	/// \param	nID - the id of the COptionTree
	/// \param	*basePath - CString ptr to the base path of the editor
	COptionTreeView(DWORD dwStyle, RECT rcRect, CWnd* pParentWnd, DWORD dwTreeOptions, UINT nID, CString basePath);

	/// \brief	default deconstructor
	virtual ~COptionTreeView();

	/// \brief	Initialize the COptionTree
	/// \param	fileName - the xml file to initialize the tree
	virtual void Init(const StdString &fileName);

	/// \brief	Deinitialize the COptionTree
	///			Will delete all elements in the tree
	virtual void DeInit();

	/// \brief	Update the view with this object type
	/// \param  *viewName - ptr to a IHashString of the name of the view
	/// \param	*objectType - ptr to a IHashString of the object type to display
	virtual bool Update(IHashString *viewName, IHashString *objectType);

	/// \brief	Serializes the view into an archive
	/// \param	ar - The archive to serialize the view data to
	/// \param	bRead - if true, then read, else write
	virtual void Serialize(IArchive &ar, bool bRead = false);

	/// Returns the number of Entries for the particular Object
	/// \return int - number of entries
	virtual int GetNumEntries( IHashString *compType );

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

	/// \brief	Initializes the tree w/ a root
	void InitTree();

	/// \brief	Clears the tree
	void ClearTree();

	/// \brief	Updates the tree with data from an archive
	///	\param	Archive - the archive to read data from
	void SerializeIn(IArchive &Archive);

	/// \brief	Writes data from the tree into an archive
	/// \param	Archive - the archive to write data to
	 void SerializeOut(IArchive &Archive);

	/// \brief	Convert an absolute path to a relative path.
	///	This function is used when setting up a COptionItemFile type.
	/// If the path is not absolute this returns the original path
	/// \param	fullPath - the absolute path to convert
	/// \return	The relative path
	bool GetRelativePath( CString &fullPath, CString &filePath );


	bool CreateTree(const StdString &compType, IHashString *objectName);

  	/// \brief	Creates a spinner item and adds it to the tree
  	///			Will also create and return ViewFormatObject containing 
  	///			formatting data for the created option tree item.
  	/// \param	ar - the xml archive to read data from
  	/// \return	ViewFormatObject* - returns a new ViewFormatObject
  	void CreateSpinnerItem(ViewFormatObject *obj);
  
  	/// \brief	Creates an editable item and adds it to the tree
  	///			Will also create and return ViewFormatObject containing 
  	///			formatting data for the created option tree item.
  	/// \param	ar - the xml archive to read data from
  	/// \return	ViewFormatObject* - returns a new ViewFormatObject
  	void CreateEditItem(ViewFormatObject *obj);
  
  	/// \brief	Creates a static item and adds it to the tree
  	///			Will also create and return ViewFormatObject containing 
  	///			formatting data for the created option tree item.
  	/// \param	ar - the xml archive to read data from
  	/// \return	ViewFormatObject* - returns a new ViewFormatObject
  	void CreateStaticItem(ViewFormatObject *obj);

  	/// \brief	Creates a hidden item and adds it to the tree
  	///			Will also create and return ViewFormatObject containing 
  	///			formatting data for the created option tree item.
  	/// \param	ar - the xml archive to read data from
  	/// \return	ViewFormatObject* - returns a new ViewFormatObject
	void CreateHiddenItem(ViewFormatObject *obj);
  
  	/// \brief	Creates a checkbox item and adds it to the tree
  	///			Will also create and return ViewFormatObject containing 
  	///			formatting data for the created option tree item.
  	/// \param	ar - the xml archive to read data from
  	/// \return	ViewFormatObject* - returns a new ViewFormatObject
  	void CreateCheckBoxItem(ViewFormatObject *obj);
  
  	/// \brief	Creates a radio item and adds it to the tree
  	///			Will also create and return ViewFormatObject containing 
  	///			formatting data for the created option tree item.
  	/// \param	ar - the xml archive to read data from
  	/// \return	ViewFormatObject* - returns a new ViewFormatObject
  	void CreateRadioItem(ViewFormatObject *obj);
  
  	/// \brief	Creates a spinner item and adds it to the tree
  	/// \param	ar - the xml archive to read data from
  	/// \return	ViewFormatObject* - returns a new ViewFormatObject 
  	void CreateFileItem(ViewFormatObject *obj);
  
  	/// \brief	Creates a combo item and adds it to the tree
  	///			Will also create and return ViewFormatObject containing 
  	///			formatting data for the created option tree item.
  	/// \param	ar - the xml archive to read data from
  	/// \return	ViewFormatObject* - returns a new ViewFormatObject
  	void CreateComboItem(ViewFormatObject *obj);

private:

	/// root of tree
	COptionTreeItem*	m_pRoot;
	/// the tree
	COptionTree			m_otTree;

	CControlSchema		m_ControlSchema;

	/// the base path 
	CString m_szBasePath;

	/// list of view objects
	VIEWOBJECTLIST *m_ViewObjectList;
	
	/// Map from coptionitem type to optionitem add function
	MAPTYPETOFUNCMAP m_TypeFuncMap;

	/// Map from coptionitem type to optiontreeitem create function
	MAPTYPETOITEMFUNC m_TreeItemCreateFuncs;

	/// elemental engine's toolbox
	IToolBox *	m_ToolBox;
};

#endif
