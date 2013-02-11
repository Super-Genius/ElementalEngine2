///============================================================================
/// \file		COptionTreeView.cpp
/// \brief		Tree view which uses the COptionTree to display data as a property panel
///				Implements the IOptionTreeView interface
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

#include "stdafx.h"
#include "COptionTreeItemCheckBoxEx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COptionTreeView::COptionTreeView()
{
	m_viewName = _T("COptionTreeView");
	m_pRoot = NULL;

	m_ViewObjectList = NULL;

	m_ToolBox = EngineGetToolBox();
}

COptionTreeView::COptionTreeView(DWORD dwStyle, RECT rcRect, CWnd* pParentWnd, DWORD dwTreeOptions, UINT nID, CString basePath)
{
	m_viewName = _T("COptionTreeView");
	m_pRoot = NULL;

	m_ViewObjectList = NULL;

	m_otTree.Create(dwStyle, rcRect, pParentWnd, dwTreeOptions, nID);
	
	m_otTree.SetNotify(TRUE, pParentWnd);

	m_ToolBox = EngineGetToolBox();

	m_szBasePath = basePath;
	
	MAPTYPECREATEFUNC *createFuncs;
	CHashString typeInt(_T("int"));
	CHashString typeFloat(_T("float"));
	CHashString typeDouble(_T("double"));
	CHashString typeBool(_T("bool"));
	CHashString typeString(_T("string"));
	
	CHashString typeCheckBox(_T("check"));
	CHashString typeRadio(_T("radio"));
	CHashString typeSpinner(_T("spinner"));
	CHashString typeEdit(_T("edit"));
	CHashString typeStatic(_T("static"));
	CHashString typeHidden(_T("hidden"));
	CHashString typeFile(_T("file"));
	CHashString typeCombo(_T("combo"));

	// setting up optiontree additem function calls
	createFuncs = new MAPTYPECREATEFUNC;
	createFuncs->insert( make_pair(typeInt.GetUniqueID(), &COptionTreeView::SerializeSpinnerItemInt) );
	createFuncs->insert( make_pair(typeFloat.GetUniqueID(), &COptionTreeView::SerializeSpinnerItemFloat) );
	createFuncs->insert( make_pair(typeDouble.GetUniqueID(), &COptionTreeView::SerializeSpinnerItemDouble) );
	m_TypeFuncMap[typeSpinner.GetUniqueID()] = createFuncs;

	createFuncs = new MAPTYPECREATEFUNC;
	createFuncs->insert( make_pair(typeInt.GetUniqueID(), &COptionTreeView::SerializeEditItemInt) );
	createFuncs->insert( make_pair(typeFloat.GetUniqueID(), &COptionTreeView::SerializeEditItemFloat) );
	createFuncs->insert( make_pair(typeDouble.GetUniqueID(), &COptionTreeView::SerializeEditItemDouble) );
	createFuncs->insert( make_pair(typeString.GetUniqueID(), &COptionTreeView::SerializeEditItemString) );
	m_TypeFuncMap[typeEdit.GetUniqueID()] = createFuncs;

	createFuncs = new MAPTYPECREATEFUNC;
	createFuncs->insert( make_pair(typeInt.GetUniqueID(), &COptionTreeView::SerializeStaticItemInt) );
	createFuncs->insert( make_pair(typeFloat.GetUniqueID(), &COptionTreeView::SerializeStaticItemFloat) );
	createFuncs->insert( make_pair(typeDouble.GetUniqueID(), &COptionTreeView::SerializeStaticItemDouble) );
	createFuncs->insert( make_pair(typeString.GetUniqueID(), &COptionTreeView::SerializeStaticItemString) );
	m_TypeFuncMap[typeStatic.GetUniqueID()] = createFuncs;

	createFuncs = new MAPTYPECREATEFUNC;
	createFuncs->insert( make_pair(typeInt.GetUniqueID(), &COptionTreeView::SerializeStaticItemInt) );
	createFuncs->insert( make_pair(typeFloat.GetUniqueID(), &COptionTreeView::SerializeStaticItemFloat) );
	createFuncs->insert( make_pair(typeDouble.GetUniqueID(), &COptionTreeView::SerializeStaticItemDouble) );
	createFuncs->insert( make_pair(typeString.GetUniqueID(), &COptionTreeView::SerializeStaticItemString) );
	createFuncs->insert( make_pair(typeBool.GetUniqueID(), &COptionTreeView::SerializeStaticItemBool) );
	m_TypeFuncMap[typeHidden.GetUniqueID()] = createFuncs;

	createFuncs = new MAPTYPECREATEFUNC;
	createFuncs->insert( make_pair(typeString.GetUniqueID(), &COptionTreeView::SerializeFileItem) );
	m_TypeFuncMap[typeFile.GetUniqueID()] = createFuncs;

	createFuncs = new MAPTYPECREATEFUNC;
	createFuncs->insert( make_pair(typeString.GetUniqueID(), &COptionTreeView::SerializeComboItem) );
	m_TypeFuncMap[typeCombo.GetUniqueID()] = createFuncs;

	createFuncs = new MAPTYPECREATEFUNC;
	createFuncs->insert( make_pair(typeBool.GetUniqueID(), &COptionTreeView::SerializeCheckBoxItem) );
	m_TypeFuncMap[typeCheckBox.GetUniqueID()] = createFuncs;

	createFuncs = new MAPTYPECREATEFUNC;
	createFuncs->insert( make_pair(typeString.GetUniqueID(), &COptionTreeView::SerializeRadioItem) );
	m_TypeFuncMap[typeRadio.GetUniqueID()] = createFuncs;

	// setting up coptiontreeitem create functions
	m_TreeItemCreateFuncs[typeSpinner.GetUniqueID()] = &COptionTreeView::CreateSpinnerItem;
	m_TreeItemCreateFuncs[typeEdit.GetUniqueID()] = &COptionTreeView::CreateEditItem;
	m_TreeItemCreateFuncs[typeStatic.GetUniqueID()] = &COptionTreeView::CreateStaticItem;
	m_TreeItemCreateFuncs[typeHidden.GetUniqueID()] = &COptionTreeView::CreateHiddenItem;
	m_TreeItemCreateFuncs[typeFile.GetUniqueID()] = &COptionTreeView::CreateFileItem;
	m_TreeItemCreateFuncs[typeCheckBox.GetUniqueID()] = &COptionTreeView::CreateCheckBoxItem;
	m_TreeItemCreateFuncs[typeRadio.GetUniqueID()] = &COptionTreeView::CreateRadioItem;
	m_TreeItemCreateFuncs[typeCombo.GetUniqueID()] = &COptionTreeView::CreateComboItem;

	//char * test = new char[255];
 }

COptionTreeView::~COptionTreeView()
{
	MAPTYPETOFUNCMAP::iterator mttfmIter = m_TypeFuncMap.begin();
	MAPTYPETOFUNCMAP::iterator delIter;
	MAPTYPECREATEFUNC *func;
	while( mttfmIter != m_TypeFuncMap.end() )
	{
		delIter = mttfmIter;
		mttfmIter++;

		func =  delIter->second;
		delete func;
		m_TypeFuncMap.erase(delIter);
	}

}

//BEGIN_MESSAGE_MAP(COptionTreeView, CWnd)

//END_MESSAGE_MAP()

void COptionTreeView::Init(const StdString &fileName)
{
	InitTree();

	StdString xmlFile( m_szBasePath );
	
	xmlFile += fileName;

	if (!m_ControlSchema.ReadXML(xmlFile))
	{
		std::string szText; 
		szText = "Failed to load xml format file: "; 
		szText += fileName; 
		::MessageBox(NULL, szText.c_str(), "Warning!", MB_OK|MB_ICONEXCLAMATION);
	}
}

void COptionTreeView::DeInit()
{
	ClearTree();
}

bool COptionTreeView::Update(IHashString* viewName, IHashString *objectType)
{
	CHashString szhViewName(m_viewName.c_str());

	// clear the tree
	ClearTree();

	// update the lable on the tree
	if (viewName != NULL)
	{
		m_pRoot->SetLabelText(viewName->GetString());
	}
	else
	{
		m_pRoot->SetLabelText(_T("Properties"));
	}
	return CreateTree(objectType->GetString(), viewName);
}

int COptionTreeView::GetNumEntries( IHashString *compType )
{
	VIEWOBJECTLIST *tmpViewObjectList;
	CHashString OptionTreeItemType(_T(""));
	tmpViewObjectList = m_ControlSchema.GetViewObjectList(compType->GetString());
	if (tmpViewObjectList != NULL)
	{
		return tmpViewObjectList->size();
	}
	else
	{
		return 0;
	}
}

void COptionTreeView::Serialize(IArchive &ar, bool bRead)
{
/*
	if(ar.IsReading())
	{
		SerializeIn(ar);
	}
	else
	{
		SerializeOut(ar);
	}
*/
	
	if (bRead)
	{
		ASSERT(ar.IsReading());
		SerializeIn(ar);
	}
	else
	{
		ASSERT(!ar.IsReading());
		SerializeOut(ar);
	}
	m_otTree.ExpandAllItems();
}


void COptionTreeView::CreateEditItem(ViewFormatObject *obj)
{
	IEditViewObject *editObject = dynamic_cast<IEditViewObject*>(obj);

	if(editObject == NULL)
	{
		StdString error = _T("Could not cast to a CEditViewObject");
		::MessageBox(NULL, error, _T("Invalid Operaton"), MB_OK);
		return;
	}

	StdString type = editObject->GetBasicType();
	StdString name = editObject->GetName();

	// create COptionTreeItem
	COptionTreeItemEdit *otiEdit;
	otiEdit = (COptionTreeItemEdit*)m_otTree.InsertItem(new COptionTreeItemEdit(), m_pRoot);
	otiEdit->SetLabelText(name.c_str());
	otiEdit->SetInfoText(_T(""));
	//otiEdit->CreateEditItem(0, NULL);
}

void COptionTreeView::CreateStaticItem(ViewFormatObject *obj)
{
	IStaticViewObject *staticObject = dynamic_cast<IStaticViewObject*>(obj);

	if(staticObject == NULL)
	{
		StdString error = _T("Could not cast to a CStaticViewObject");
		::MessageBox(NULL, error, _T("Invalid Operaton"), MB_OK);
		return;
	}

	StdString type = staticObject->GetBasicType();
	StdString name = staticObject->GetName();
	
	// create COptionTreeItem
	COptionTreeItemStatic *otiStatic;
	otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), m_pRoot);
	otiStatic->SetLabelText(name.c_str());
	otiStatic->SetInfoText(_T(""));
	otiStatic->CreateStaticItem(0);
}

void COptionTreeView::CreateHiddenItem(ViewFormatObject *obj)
{
	IHiddenViewObject *hiddenObject = dynamic_cast<IHiddenViewObject*>(obj);

	if(hiddenObject == NULL)
	{
		StdString error = _T("Could not cast to a CHiddenViewObject");
		::MessageBox(NULL, error, _T("Invalid Operaton"), MB_OK);
		return;
	}

	StdString type = hiddenObject->GetBasicType();
	StdString name = hiddenObject->GetName();
	
	// create COptionTreeItem
	COptionTreeItemStatic *otiStatic;
	otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), m_pRoot);
	otiStatic->SetOptionsOwner( NULL );
//	otiStatic->SetLabelText(name.c_str());
//	otiStatic->SetInfoText(_T(""));
//	otiStatic->CreateStaticItem(0);
}

void COptionTreeView::CreateSpinnerItem(ViewFormatObject *obj)
{
	ISpinnerViewObject* spinnerObject = dynamic_cast<ISpinnerViewObject*>(obj);

	if(spinnerObject == NULL)
	{
		StdString error = _T("Could not cast to a CSpinnerViewObject");
		::MessageBox(NULL, error, _T("Invalid Operaton"), MB_OK);
		return;
	}

	StdString type = spinnerObject->GetBasicType();
	StdString name = spinnerObject->GetName();
	double increment = spinnerObject->GetIncVal();

	// create COptionTreeItemSpinner
	COptionTreeItemSpinner *otiSpinner;
	otiSpinner = (COptionTreeItemSpinner*)m_otTree.InsertItem(new COptionTreeItemSpinner(), m_pRoot);
	otiSpinner->SetLabelText(name.c_str());
	otiSpinner->SetInfoText(_T(""));	
	otiSpinner->CreateSpinnerItem( OT_EDIT_USEREDIT, 0.0, spinnerObject->GetMin(), spinnerObject->GetMax(), increment );
}

void COptionTreeView::CreateCheckBoxItem(ViewFormatObject *obj)
{
	ICheckBoxViewObject	*checkboxObject = dynamic_cast<ICheckBoxViewObject*>(obj);

	if(checkboxObject == NULL)
	{
		StdString error = _T("Could not cast to a CCheckBoxViewObject");
		::MessageBox(NULL, error, _T("Invalid Operaton"), MB_OK);
		return;
	}

	StdString type = checkboxObject->GetBasicType();
	StdString name = checkboxObject->GetName();

	// create COptionTreeItem
	COptionTreeItemCheckBox *otiCheckBox;
	otiCheckBox = (COptionTreeItemCheckBox*)m_otTree.InsertItem(new COptionTreeItemCheckBoxEx(), m_pRoot);
	otiCheckBox->SetLabelText(name.c_str());
	otiCheckBox->SetInfoText(_T(""));
	otiCheckBox->CreateCheckBoxItem( 0, OT_CHECKBOX_SHOWCHECK );
	otiCheckBox->SetCheck(false);
}

void COptionTreeView::CreateRadioItem(ViewFormatObject *obj)
{
	IRadioViewObject *radioObject = dynamic_cast<IRadioViewObject*>(obj);

	if(radioObject == NULL)
	{
		StdString error = _T("Could not cast to a CRadioViewObject");
		::MessageBox(NULL, error, _T("Invalid Operaton"), MB_OK);
		return;
	}

	StdString type = radioObject->GetBasicType();
	StdString name = radioObject->GetName();
	RADIOLIST *radioList = radioObject->GetRadioList();
	StdString defSelect = radioObject->GetDefSelect();
	StdString text;
	bool checked;

	// Create COptionTreeItem
	COptionTreeItemRadio *otiRadio;
	otiRadio = (COptionTreeItemRadio*)m_otTree.InsertItem(new COptionTreeItemRadio(), m_pRoot);
	otiRadio->SetLabelText(name.c_str());
	otiRadio->SetInfoText(_T(""));
	otiRadio->CreateRadioItem();
	
	if(radioList->size() > 0)
	{
		RADIOLIST::iterator rlIter = radioList->begin();
		for(; rlIter != radioList->end(); rlIter++)
		{
			text = (*rlIter);
			if ( _tcscmp(text, defSelect) == 0 )
			{
				checked = true;
			}
			else
			{
				checked = false;
			}
			otiRadio->InsertNewRadio(text.c_str(), checked);
		}
	}
}

void COptionTreeView::CreateFileItem(ViewFormatObject *obj)
{
	IFileViewObject *fileObject = dynamic_cast<IFileViewObject*>(obj);

	if(fileObject == NULL)
	{
		StdString error = _T("Could not cast to a CFileViewObject");
		::MessageBox(NULL, error, _T("Invalid Operaton"), MB_OK);
		return;
	}

	StdString type = fileObject->GetBasicType();
	StdString name = fileObject->GetName();

	StdString fileName = fileObject->GetDefFile();
	StdString defExt = fileObject->GetDefExt();
	StdString extFilter = fileObject->GetExtFilter();

	COptionTreeItemFile *otiFile;
	otiFile = (COptionTreeItemFile*)m_otTree.InsertItem( new COptionTreeItemFile(), m_pRoot );
	otiFile->SetLabelText(name.c_str());
	otiFile->SetInfoText(_T(""));

	int flags = OT_FILE_OPENDIALOG | OT_FILE_SHOWFULLPATH;
	if (defExt == "" && extFilter == "")
		flags |= OT_FILE_SELECTDIALOG;

	otiFile->CreateFileItem((const TCHAR*)fileName,
							defExt.c_str(),
							extFilter.c_str(),
							flags,
							OFN_OVERWRITEPROMPT);

}

void COptionTreeView::CreateComboItem(ViewFormatObject *obj)
{
	IComboViewObject *comboObject = dynamic_cast<IComboViewObject*>(obj);

	if(comboObject == NULL)
	{
		StdString error = _T("Could not cast to a CComboViewObject");
		::MessageBox(NULL, error, _T("Invalid Operaton"), MB_OK);
		return;
	}

	int index;
	StdString type = comboObject->GetBasicType();
	StdString name = comboObject->GetName();
	StdString defSelect = comboObject->GetDefSelect();

	// create COptionTreeItem
	COptionTreeItemComboBox *otiCombo;
	COMBOLIST *comboList;
	comboList = comboObject->GetComboList();

	otiCombo = (COptionTreeItemComboBox*)m_otTree.InsertItem(new COptionTreeItemComboBox(), m_pRoot);
	otiCombo->SetLabelText(name.c_str());
	if (otiCombo->CreateComboItem(CBS_SORT) == TRUE)
	{
		// if we got some options
		if (comboList->size() > 0)
		{
			COMBOLIST::iterator clIter = comboList->begin();
			// throw them in the combo box
			for (; clIter != comboList->end(); clIter++)
			{
				otiCombo->AddString( (*clIter).c_str() );
			}
		}
	}

	index = otiCombo->FindStringExact(0, defSelect.c_str());
	otiCombo->SetCurSel((index >= 0) ? index : 0);
}

void COptionTreeView::SerializeEditItemInt(IArchive &ar, COptionTreeItem *item, bool read)
{
	int iVal;
	COptionTreeItemEdit *otiEdit;

	otiEdit = dynamic_cast<COptionTreeItemEdit*>(item);

	if (otiEdit == NULL)
	{
		//error
		StdString error = _T("Could not cast item to COptionTreeItemEdit: ");
		error += item->GetLabelText();
		::MessageBox(NULL, error, _T("Invalid Command"), MB_OK);
		return;
	}

	otiEdit->CreateEditItem(OT_EDIT_NUMERICAL, NULL);

	if (read)
	{
		ar.Read(iVal);
		otiEdit->SetEditInt(iVal);
	}
	else
	{
		otiEdit->GetEditInt(iVal);
		ar.Write(iVal);
	}
}

void COptionTreeView::SerializeEditItemFloat(IArchive &ar, COptionTreeItem *item, bool read)
{
	float fVal;
	COptionTreeItemEdit *otiEdit;

	otiEdit = dynamic_cast<COptionTreeItemEdit*>(item);
	
	if(otiEdit == NULL)
	{
		//error
		StdString error = _T("Could not cast item to COptionTreeItemEdit: ");
		error += item->GetLabelText();
		::MessageBox(NULL, error, _T("Invalid Command"), MB_OK);
		return;
	}

	otiEdit->CreateEditItem(OT_EDIT_NUMERICAL, NULL);

	if(read)
	{
		ar.Read(fVal);
		otiEdit->SetEditFloat(fVal);
	}
	else
	{
		otiEdit->GetEditFloat(fVal);
		ar.Write(fVal);
	}
}

void COptionTreeView::SerializeEditItemDouble(IArchive &ar, COptionTreeItem *item, bool read)
{
	double dVal;
	COptionTreeItemEdit *otiEdit;

	otiEdit = dynamic_cast<COptionTreeItemEdit*>(item);
	
	if(otiEdit == NULL)
	{
		//error
		StdString error = _T("Could not cast item to COptionTreeItemEdit: ");
		error += item->GetLabelText();
		::MessageBox(NULL, error, _T("Invalid Command"), MB_OK);
		return;
	}

	otiEdit->CreateEditItem(OT_EDIT_NUMERICAL, NULL);

	if(read)
	{
		ar.Read(dVal);
		otiEdit->SetEditDouble(dVal);
	}
	else
	{
		otiEdit->GetEditDouble(dVal);
		ar.Write(dVal);
	}
}

void COptionTreeView::SerializeEditItemString(IArchive &ar, COptionTreeItem *item, bool read)
{	
	StdString szString;
	COptionTreeItemEdit *otiEdit;
	
	CString szItem;
	otiEdit = dynamic_cast<COptionTreeItemEdit*>(item);
	if(otiEdit == NULL)
	{
		//error
		StdString error = _T("Could not cast item to COptionTreeItemEdit: ");
		error += item->GetLabelText();
		::MessageBox(NULL, error, _T("Invalid Command"), MB_OK);
		return;
	}

	otiEdit->CreateEditItem(0, NULL);

	if (read)
	{
		ar.Read(szString);
		otiEdit->SetEditText(szString.c_str());
	}
	else
	{
		otiEdit->GetWindowText(szItem);
		ar.Write(szItem);
	}
}

void COptionTreeView::SerializeStaticItemInt(IArchive &ar, COptionTreeItem *item, bool read)
{
	int iVal;
	COptionTreeItemStatic *otiStatic;
	TCHAR buffer[50];
	CString text;

	otiStatic = dynamic_cast<COptionTreeItemStatic*>(item);
	if(otiStatic == NULL)
	{
		//error
		StdString error = _T("Could not cast item to COptionTreeItemStatic: ");
		error += item->GetLabelText();
		::MessageBox(NULL, error, _T("Invalid Command"), MB_OK);
		return;
	}
	if(read)
	{
		ar.Read(iVal);
		_itot(iVal, buffer, 10);
		otiStatic->SetStaticText(buffer);
	}
	else
	{
		text = otiStatic->GetStaticText();
		iVal = _tstoi(text);
		ar.Write(iVal);
	}
}

void COptionTreeView::SerializeStaticItemFloat(IArchive &ar, COptionTreeItem *item, bool read)
{
	float fVal;
	COptionTreeItemStatic *otiStatic;
	TCHAR buff[50];
	CString text;

	otiStatic = dynamic_cast<COptionTreeItemStatic*>(item);
	if(otiStatic == NULL)
	{
		//error
		StdString error = _T("Could not cast item to COptionTreeItemStatic: ");
		error += item->GetLabelText();
		::MessageBox(NULL, error, _T("Invalid Command"), MB_OK);
		return;
	}
	if(read)
	{
		ar.Read(fVal);
		_stprintf(buff, "%g", fVal);
		otiStatic->SetStaticText(buff);
	}
	else
	{
		text = otiStatic->GetStaticText();
		fVal = (float)_tstof(text);
		ar.Write(fVal);
	}
}

void COptionTreeView::SerializeStaticItemDouble(IArchive &ar, COptionTreeItem *item, bool read)
{
	double dVal;
	COptionTreeItemStatic *otiStatic;
	TCHAR buff[50];
	CString text;

	otiStatic = dynamic_cast<COptionTreeItemStatic*>(item);
	if(otiStatic == NULL)
	{
		//error
		StdString error = _T("Could not cast item to COptionTreeItemStatic: ");
		error += item->GetLabelText();
		::MessageBox(NULL, error, _T("Invalid Command"), MB_OK);
		return;
	}
	if(read)
	{
		ar.Read(dVal);
		_stprintf(buff, "%g", dVal);
		otiStatic->SetStaticText(buff);
	}
	else
	{
		text = otiStatic->GetStaticText();
		dVal = _tstof(text);

		ar.Write(dVal);
	}
}

void COptionTreeView::SerializeStaticItemString(IArchive &ar, COptionTreeItem *item, bool read)
{
	StdString szString;
	COptionTreeItemStatic *otiStatic = dynamic_cast<COptionTreeItemStatic*>(item);

	CString szItem;
	if(otiStatic == NULL)
	{
		//error
		StdString error = _T("Could not cast item to COptionTreeItemStatic: ");
		error += item->GetLabelText();
		::MessageBox(NULL, error, _T("Invalid Command"), MB_OK);
		return;
	}
	if(read)
	{
		ar.Read(szString);
		otiStatic->SetStaticText(szString.c_str());
	}
	else
	{
		szItem = otiStatic->GetStaticText();
		ar.Write(szItem);
	}
}

void COptionTreeView::SerializeStaticItemBool(IArchive &ar, COptionTreeItem *item, bool read)
{
	bool bVal;
	COptionTreeItemStatic *otiStatic;
	CString text;

	otiStatic = dynamic_cast<COptionTreeItemStatic*>(item);
	if(otiStatic == NULL)
	{
		//error
		StdString error = _T("Could not cast item to COptionTreeItemStatic: ");
		error += item->GetLabelText();
		::MessageBox(NULL, error, _T("Invalid Command"), MB_OK);
		return;
	}
	if(read)
	{
		ar.Read(bVal);
		otiStatic->SetStaticText( bVal ? _T("true") : _T("false")  );
	}
	else
	{
		text = otiStatic->GetStaticText();
		if (_tcscmp( text, _T("true") ) == 0)
			ar.Write(true);
		else
			ar.Write(false);
	}
}

void COptionTreeView::SerializeSpinnerItemInt(IArchive &ar, COptionTreeItem *item, bool read)
{
	int iVal;
	COptionTreeItemSpinner *otiSpinner;

	otiSpinner = dynamic_cast<COptionTreeItemSpinner*>(item);
	if (otiSpinner == NULL)
	{
		//error
		StdString error = _T("Could not cast item to COptionTreeItemSpinner: ");
		error += item->GetLabelText();
		::MessageBox(NULL, error, _T("Invalid Command"), MB_OK);
		return;
	}
	if(read)
	{
		ar.Read(iVal);
		otiSpinner->SetEditInt(iVal);
	}
	else
	{
		otiSpinner->GetEditInt(iVal);
		ar.Write(iVal);
	}
}

void COptionTreeView::SerializeSpinnerItemFloat(IArchive &ar, COptionTreeItem *item, bool read)
{
	float fVal;
	COptionTreeItemSpinner *otiSpinner;

	otiSpinner = dynamic_cast<COptionTreeItemSpinner*>(item);
	if (otiSpinner == NULL)
	{
		//error
		StdString error = _T("Could not cast item to COptionTreeItemSpinner: ");
		error += item->GetLabelText();
		::MessageBox(NULL, error, _T("Invalid Command"), MB_OK);
		return;
	}

	if (read)
	{
		ar.Read(fVal);
		otiSpinner->SetEditFloat(fVal);
	}
	else
	{
		otiSpinner->GetEditFloat(fVal);
		ar.Write(fVal);
	}
}

void COptionTreeView::SerializeSpinnerItemDouble(IArchive &ar, COptionTreeItem *item, bool read)
{
	double dVal;
	COptionTreeItemSpinner *otiSpinner;

	otiSpinner = dynamic_cast<COptionTreeItemSpinner*>(item);
	if (otiSpinner == NULL)
	{
		//error
		StdString error = _T("Could not cast item to COptionTreeItemSpinner: ");
		error += item->GetLabelText();
		::MessageBox(NULL, error, _T("Invalid Command"), MB_OK);
		return;
	}
	if (read)
	{
		ar.Read(dVal);
		otiSpinner->SetEditDouble(dVal);
	}
	else
	{
		otiSpinner->GetEditDouble(dVal);
		ar.Write(dVal);
	}
}

void COptionTreeView::SerializeRadioItem(IArchive &ar, COptionTreeItem *item, bool read)
{
	StdString szVal;
	COptionTreeItemRadio *otiRadio;
	OT_RADIO_NODE *node;
	int index;

	otiRadio = dynamic_cast<COptionTreeItemRadio*>(item);
	if (item == NULL)
	{
		//error
		StdString error = _T("Could not cast item to COptionTreeItemRadio: ");
		error += item->GetLabelText();
		::MessageBox(NULL, error, _T("Invalid Command"), MB_OK);
		return;
	}
	if (read)
	{
		ar.Read(szVal);
		node = otiRadio->Node_FindNode(szVal.c_str());
		if (node != NULL)
		{	
			otiRadio->Node_UnCheckAll();
			node->m_bChecked = true;
		}	
	}
	else
	{
		index = otiRadio->Node_GetChecked();
		node = otiRadio->Node_FindNode(index);
		ar.Write(node->m_strText);
	}
}

void COptionTreeView::SerializeCheckBoxItem(IArchive &ar, COptionTreeItem *item, bool read)
{
	bool bVal;
	COptionTreeItemCheckBox *otiCheckBox;

	otiCheckBox = dynamic_cast<COptionTreeItemCheckBox*>(item);
	if(otiCheckBox == NULL)
	{
		//error
		StdString error = _T("Could not cast item to COptionTreeItemCheckBox: ");
		error += item->GetLabelText();
		::MessageBox(NULL, error, _T("Invalid Command"), MB_OK);
		return;
	}
	if(read)
	{
		ar.Read(bVal);
		otiCheckBox->SetCheck(bVal);
	}
	else
	{
		bVal = (otiCheckBox->GetCheck() != 0);
		ar.Write(bVal);
	}
}

void COptionTreeView::SerializeFileItem(IArchive &ar, COptionTreeItem *item, bool read)
{
	StdString fileName;
	COptionTreeItemFile *otiFile;
	CString szItem;

	otiFile = dynamic_cast<COptionTreeItemFile*>(item);
	if(otiFile == NULL)
	{
		//error
		StdString error = _T("Could not cast item to COptionTreeItemFile: ");
		error += item->GetLabelText();
		::MessageBox(NULL, error, _T("Invalid Command"), MB_OK);
		return;
	}

	if (read)
	{
		ar.Read(fileName);
		if ( !fileName.empty() )
		{
			fileName.MakeSafeFileName();
			otiFile->SetFileString(fileName.c_str());
		}
	}
	else
	{
		// Try to convert to a relative path
		if( !GetRelativePath(otiFile->GetFileString(), szItem) )
		{
			// if an invalid path reset the string
			otiFile->ResetFileString();
			// run it through the relative pathizer again
			GetRelativePath(otiFile->GetFileString(), szItem);					
		}
		otiFile->SetFileString( szItem );
		fileName = szItem;
		fileName.MakeSafeFileName();
		ar.Write(fileName);
	}
}

void COptionTreeView::SerializeComboItem(IArchive &ar, COptionTreeItem *item, bool read)
{
	StdString defaultSelect;
	COptionTreeItemComboBox *otiCombo;
	CString szItem;
	int index;

	otiCombo = dynamic_cast<COptionTreeItemComboBox*>(item);
	if(otiCombo == NULL)
	{
		//error
		StdString error = _T("Could not cast item to COptionTreeItemCombo: ");
		error += item->GetLabelText();
		::MessageBox(NULL, error, _T("Invalid Command"), MB_OK);
		return;
	}
	if(read)
	{
		ar.Read(defaultSelect);
		if( _tcscmp( defaultSelect, _T("")) != 0 )
		{
			index = otiCombo->FindStringExact(0, defaultSelect.c_str());
			otiCombo->SetCurSel((index >= 0) ? index : 0);
		}

	}
	else
	{
		int sel = otiCombo->GetCurSel();
		if(sel == CB_ERR)
		{
			//display some message?
		}
		otiCombo->GetLBText(sel, szItem);
		ar.Write(szItem);
	}
}

void COptionTreeView::InitTree()
{
	if (m_otTree.m_hWnd)
	{
		m_pRoot = m_otTree.InsertItem(new COptionTreeItem());
		m_pRoot->SetInfoText(_T("This is a list of properties"));	
	}
}

void COptionTreeView::ClearTree()
{
	// delete all items
	m_otTree.DeleteAllItems();
	m_pRoot = NULL;
	// reinitialize tree w/ root
	InitTree();

}

void COptionTreeView::SerializeIn(IArchive &Archive)
{
	CHashString basicType(_T(""));
	CHashString OptionTreeItemType(_T(""));

	// get the first option tree item in tree
	COptionTreeItem *item = m_pRoot->GetChild();

	if (item == NULL)
	{
		return;
	}

	VIEWOBJECTLIST::iterator volIter = m_ViewObjectList->begin();

	for(; volIter != m_ViewObjectList->end(); volIter++)
	{
		ViewFormatObject *vo;
		vo = *volIter;

		basicType = vo->GetBasicType().c_str();
		OptionTreeItemType = vo->GetViewType().c_str();

		MAPTYPETOFUNCMAP::iterator mttfmIter;
		// find the map from basicType to write option tree item
		mttfmIter = m_TypeFuncMap.find(OptionTreeItemType.GetUniqueID());
		if (mttfmIter == m_TypeFuncMap.end())
		{
			// warning, continue....
			// Sorry, that type of item is not known!
			StdString error = _T("COptionTreeItem of this type is unknown: ");
			error += OptionTreeItemType.GetString();
			::MessageBox(NULL, error, _T("INVALID OPERATION"), MB_OK);
			continue;
		}

		MAPTYPECREATEFUNC *createFuncs;
		createFuncs = mttfmIter->second;

		MAPTYPECREATEFUNC::iterator mtcfIter;
		// find the function that writes the option tree item w/ this basicType
		mtcfIter = createFuncs->find(basicType.GetUniqueID());

		if (mtcfIter == createFuncs->end())
		{
			// warning, continue
			// Sorry this item does not accept items of type %s, OptionTreeItemType
			StdString error = _T("This basic type is unknown: ");
			error += basicType.GetString();
			::MessageBox(NULL, error, _T("INVALID OPERATION"), MB_OK);
			continue;
		}

		OTCREATETYPEFUNC funcPtr;
		funcPtr = mtcfIter->second;
		// call the function that writes the value from option tree item into the archive
		(this->*funcPtr)(Archive, item, true);

		
		item = item->GetSibling();
	}
}

void COptionTreeView::SerializeOut(IArchive &Archive)
{
	CHashString basicType(_T(""));
	CHashString OptionTreeItemType(_T(""));

	// get the first option tree item in tree
	COptionTreeItem *item = m_pRoot->GetChild();

	if (item == NULL)
	{
		return;
	}

	VIEWOBJECTLIST::iterator volIter = m_ViewObjectList->begin();

	for(; volIter != m_ViewObjectList->end(); volIter++)
	{
		ViewFormatObject *vo;
		vo = *volIter;

		basicType = vo->GetBasicType().c_str();
		OptionTreeItemType = vo->GetViewType().c_str();

		MAPTYPETOFUNCMAP::iterator mttfmIter;
		// find the map from basicType to write option tree item
		mttfmIter = m_TypeFuncMap.find(OptionTreeItemType.GetUniqueID());
		if (mttfmIter == m_TypeFuncMap.end())
		{
			// warning, continue....
			// Sorry, that type of item is not known!
			StdString error = _T("COptionTreeItem of this type is unknown: ");
			error += OptionTreeItemType.GetString();
			::MessageBox(NULL, error, _T("INVALID OPERATION"), MB_OK);
			continue;
		}

		MAPTYPECREATEFUNC *createFuncs;
		createFuncs = mttfmIter->second;

		MAPTYPECREATEFUNC::iterator mtcfIter;
		// find the function that writes the option tree item w/ this basicType
		mtcfIter = createFuncs->find(basicType.GetUniqueID());

		if (mtcfIter == createFuncs->end())
		{
			// warning, continue
			// Sorry this item does not accept items of type %s, OptionTreeItemType
			StdString error = _T("This basic type is unknown: ");
			error += basicType.GetString();
			::MessageBox(NULL, error, _T("INVALID OPERATION"), MB_OK);
		}

		OTCREATETYPEFUNC funcPtr;
		funcPtr = mtcfIter->second;
		// call the function that writes the value from option tree item into the archive
		(this->*funcPtr)(Archive, item, false);

		
		item = item->GetSibling();
	}
}

bool COptionTreeView::GetRelativePath( CString &fullPath, CString &filePath )
{
	StdString resourcePath;
	m_ToolBox->GetDirectories(NULL, &resourcePath);

	CString basePath = resourcePath.c_str();
	int fullLength = fullPath.GetLength();
	int pathLength = basePath.GetLength();

	//compare the first three TCHARs in both strings
	// e.g. 'c:/' if it is an absolute path
	// If they are the same then the full path is an absulute path
	// and needs to be converted to a relative path
	if( !fullPath.Left(3).CompareNoCase(basePath.Left(3)) ) 
	{
		// Check if the path is not under the base path and thus invalid
		if( fullPath.Left(pathLength).CompareNoCase(basePath) )
		{
			// display a warning box
			::MessageBox( NULL, "File not in relative path to resource data directory", 
						"Error: Invalid File Location", MB_OK );

			// not a valid path, return false
			return false;
		}

		// turn the full path into a relative path
		
		// first, check if resource path ends in '\\'
		TCHAR lastChar = basePath.GetAt(pathLength-1);
		if(lastChar == '\\')
		{
			fullPath = fullPath.Right( fullLength-pathLength );
		}
		else
		{
			// remove the extra '/'
			fullPath = fullPath.Right( fullLength-pathLength - 1);
		}
	}
	filePath = fullPath;
	// valid path, return true
	return true;
}

bool COptionTreeView::CreateTree(const StdString &compType, IHashString *objectName)
{
	CHashString OptionTreeItemType(_T(""));

	CHashString streamType(_T("Memory"));	// type of archive to create
	CREATEARCHIVE ca;
	ca.streamSize = 0;
	ca.streamData = NULL;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamType = &streamType;
	static DWORD msgHash_CreateXMLArchiveStream = CHashString(_T("CreateXMLArchiveStream")).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage(msgHash_CreateXMLArchiveStream, sizeof(CREATEARCHIVE), &ca);
	if (retVal != MSG_HANDLED)
	{
		m_ToolBox->Log( LOGERROR, _T("%s(%i): Failed to create archive."), __FILE__, __LINE__ );
		return false;
	}
	IXMLArchive *MemArchive = (IXMLArchive*)ca.archive;
	MemArchive->SetIsWriting( true );

	GETSCHEMAPARAMS schemaparams;
	schemaparams.ar = MemArchive;
	static DWORD msgHash_GetSchema = CHashString(_T("GetSchema")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetSchema, sizeof(schemaparams), &schemaparams, objectName, &CHashString(compType) ) == MSG_HANDLED)
	{
		MemArchive->SetIsWriting( false );
		StdString compType;
		MemArchive->GetNode(compType);
		m_ViewObjectList = m_ControlSchema.AddSchemaFromArchive( compType, MemArchive );
	}
	MemArchive->Close();

	m_ViewObjectList = m_ControlSchema.GetViewObjectList(compType);
	if (m_ViewObjectList == NULL)
	{
		// warning, component type is unknown
		StdString error = _T("Component type of this type is unknown: ");
		error += compType;
		::MessageBox(NULL, error, _T("INVALID OPERATION"), MB_OK);
		return false;
	}

	VIEWOBJECTLIST::iterator volIter = m_ViewObjectList->begin();

	for(; volIter != m_ViewObjectList->end(); volIter++)
	{
		ViewFormatObject *vo;
		vo = *volIter;

		OptionTreeItemType = vo->GetViewType().c_str();

		MAPTYPETOITEMFUNC::iterator mttifIter;
		// find the map from basicType to write option tree item
		mttifIter = m_TreeItemCreateFuncs.find(OptionTreeItemType.GetUniqueID());
		if (mttifIter == m_TreeItemCreateFuncs.end())
		{
			// warning, continue....
			// Sorry, that type of item is not known!
			StdString error = _T("COptionTreeItem of this type is unknown: ");
			error += OptionTreeItemType.GetString();
			::MessageBox(NULL, error, _T("INVALID OPERATION"), MB_OK);
			continue;
		}

		OTCREATEITEMFUNC funcPtr = mttifIter->second;

		(this->*funcPtr)(vo);
	}
	m_otTree.ExpandAllItems();

	return true;
}