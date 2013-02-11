///============================================================================
/// \file		COptionTreeWrapper.cpp
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
#include "COptionTreeWrapper.h"
#include "COptionTreeItemCheckBoxEx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COptionTreeWrapper::COptionTreeWrapper()
{
	m_viewName = _T("COptionTreeWrapper");
	m_ToolBox = EngineGetToolBox();
}

COptionTreeWrapper::COptionTreeWrapper(DWORD dwStyle, RECT rcRect, CWnd* pParentWnd, DWORD dwTreeOptions, UINT nID, CString basePath)
{
	m_viewName = _T("COptionTreeWrapper");	
	
	m_Style = dwStyle;
	m_Rect = rcRect;
	m_pParentWnd = pParentWnd;
	m_TreeOptions = dwTreeOptions;
	m_ID = nID;

	m_mTrees[NULL].m_Tree = new COptionTree();	
	m_mTrees[NULL].m_Tree->Create(dwStyle, rcRect, pParentWnd, dwTreeOptions, nID);
	m_mTrees[NULL].m_Tree->SetNotify(TRUE, pParentWnd);

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
	CHashString typeColor(_T("color"));

	// setting up optiontree additem function calls
	createFuncs = new MAPTYPECREATEFUNC;
	createFuncs->insert( make_pair(typeInt.GetUniqueID(), &COptionTreeWrapper::SerializeSpinnerItemInt) );
	createFuncs->insert( make_pair(typeFloat.GetUniqueID(), &COptionTreeWrapper::SerializeSpinnerItemFloat) );
	createFuncs->insert( make_pair(typeDouble.GetUniqueID(), &COptionTreeWrapper::SerializeSpinnerItemDouble) );
	m_TypeFuncMap[typeSpinner.GetUniqueID()] = createFuncs;

	createFuncs = new MAPTYPECREATEFUNC;
	createFuncs->insert( make_pair(typeInt.GetUniqueID(), &COptionTreeWrapper::SerializeEditItemInt) );
	createFuncs->insert( make_pair(typeFloat.GetUniqueID(), &COptionTreeWrapper::SerializeEditItemFloat) );
	createFuncs->insert( make_pair(typeDouble.GetUniqueID(), &COptionTreeWrapper::SerializeEditItemDouble) );
	createFuncs->insert( make_pair(typeString.GetUniqueID(), &COptionTreeWrapper::SerializeEditItemString) );
	m_TypeFuncMap[typeEdit.GetUniqueID()] = createFuncs;

	createFuncs = new MAPTYPECREATEFUNC;
	createFuncs->insert( make_pair(typeInt.GetUniqueID(), &COptionTreeWrapper::SerializeStaticItemInt) );
	createFuncs->insert( make_pair(typeFloat.GetUniqueID(), &COptionTreeWrapper::SerializeStaticItemFloat) );
	createFuncs->insert( make_pair(typeDouble.GetUniqueID(), &COptionTreeWrapper::SerializeStaticItemDouble) );
	createFuncs->insert( make_pair(typeString.GetUniqueID(), &COptionTreeWrapper::SerializeStaticItemString) );
	m_TypeFuncMap[typeStatic.GetUniqueID()] = createFuncs;

	createFuncs = new MAPTYPECREATEFUNC;
	createFuncs->insert( make_pair(typeInt.GetUniqueID(), &COptionTreeWrapper::SerializeStaticItemInt) );
	createFuncs->insert( make_pair(typeFloat.GetUniqueID(), &COptionTreeWrapper::SerializeStaticItemFloat) );
	createFuncs->insert( make_pair(typeDouble.GetUniqueID(), &COptionTreeWrapper::SerializeStaticItemDouble) );
	createFuncs->insert( make_pair(typeString.GetUniqueID(), &COptionTreeWrapper::SerializeStaticItemString) );
	createFuncs->insert( make_pair(typeBool.GetUniqueID(), &COptionTreeWrapper::SerializeStaticItemBool) );
	m_TypeFuncMap[typeHidden.GetUniqueID()] = createFuncs;

	createFuncs = new MAPTYPECREATEFUNC;
	createFuncs->insert( make_pair(typeString.GetUniqueID(), &COptionTreeWrapper::SerializeFileItem) );
	m_TypeFuncMap[typeFile.GetUniqueID()] = createFuncs;

	createFuncs = new MAPTYPECREATEFUNC;
	createFuncs->insert( make_pair(typeString.GetUniqueID(), &COptionTreeWrapper::SerializeComboItem) );
	m_TypeFuncMap[typeCombo.GetUniqueID()] = createFuncs;

	createFuncs = new MAPTYPECREATEFUNC;
	createFuncs->insert( make_pair(typeBool.GetUniqueID(), &COptionTreeWrapper::SerializeCheckBoxItem) );
	m_TypeFuncMap[typeCheckBox.GetUniqueID()] = createFuncs;

	createFuncs = new MAPTYPECREATEFUNC;
	createFuncs->insert( make_pair(typeString.GetUniqueID(), &COptionTreeWrapper::SerializeRadioItem) );
	m_TypeFuncMap[typeRadio.GetUniqueID()] = createFuncs;

	createFuncs = new MAPTYPECREATEFUNC;
	createFuncs->insert( make_pair(typeInt.GetUniqueID(), &COptionTreeWrapper::SerializeColorItem) );
	m_TypeFuncMap[typeColor.GetUniqueID()] = createFuncs;

	// setting up coptiontreeitem create functions
	m_TreeItemCreateFuncs[typeSpinner.GetUniqueID()] = &COptionTreeWrapper::CreateSpinnerItem;
	m_TreeItemCreateFuncs[typeEdit.GetUniqueID()] = &COptionTreeWrapper::CreateEditItem;
	m_TreeItemCreateFuncs[typeStatic.GetUniqueID()] = &COptionTreeWrapper::CreateStaticItem;
	m_TreeItemCreateFuncs[typeHidden.GetUniqueID()] = &COptionTreeWrapper::CreateHiddenItem;
	m_TreeItemCreateFuncs[typeFile.GetUniqueID()] = &COptionTreeWrapper::CreateFileItem;
	m_TreeItemCreateFuncs[typeCheckBox.GetUniqueID()] = &COptionTreeWrapper::CreateCheckBoxItem;
	m_TreeItemCreateFuncs[typeRadio.GetUniqueID()] = &COptionTreeWrapper::CreateRadioItem;
	m_TreeItemCreateFuncs[typeCombo.GetUniqueID()] = &COptionTreeWrapper::CreateComboItem;
	m_TreeItemCreateFuncs[typeColor.GetUniqueID()] = &COptionTreeWrapper::CreateColorItem;	
 }

COptionTreeWrapper::~COptionTreeWrapper()
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
	for (UINT i=0; i<m_vRootNames.size(); i++)
	{
		delete m_mTrees[m_vRootNames[i].GetUniqueID()].m_Root;
	}

	// clean up NULL tree created in constructor
	MAPTREES::iterator itrNullTree = m_mTrees.find(NULL);
	if (itrNullTree != m_mTrees.end())
	{
		delete itrNullTree->second.m_Tree;
	}
}

void COptionTreeWrapper::Init(const StdString &fileName)
{
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

void COptionTreeWrapper::DeInit()
{
	ClearTree();	
}

void COptionTreeWrapper::Serialize(CHashString objName, CHashString objType, IArchive &ar, bool bRead)
{
	vector <CHashString> types;
	types.push_back(objType);
	Serialize(objName, types, ar, bRead);
}

void COptionTreeWrapper::Serialize(CHashString objName, vector<CHashString> objTypes, IArchive &ar, bool bRead)
{
	if (m_mTrees[objName.GetUniqueID()].m_Tree == NULL)
	{
		m_mTrees[objName.GetUniqueID()].m_Tree = new COptionTree();
		m_mTrees[objName.GetUniqueID()].m_Tree->Create(m_Style, m_Rect, m_pParentWnd, m_TreeOptions, m_ID);
		m_mTrees[objName.GetUniqueID()].m_Root = m_mTrees[objName.GetUniqueID()].m_Tree->InsertItem(new COptionTreeItem());		
		CString label;
		label = objName.GetString();
		label += _T("(");
		label += objTypes[0].GetString();
		label += _T(")");
		m_mTrees[objName.GetUniqueID()].m_Root->SetLabelText(label);
		for (UINT i=0; i<objTypes.size(); i++)
		{
			CreateTree(objTypes[i].GetString(), objName);
		}
		m_vRootNames.push_back(objName);
	}
	if (bRead)
	{
		SerializeIn(ar, objName);
	}
	else
	{
		SerializeOut(ar, objName);
	}
	ExpandOneRoot(objName);
}

void COptionTreeWrapper::CreateEditItem(CHashString compName, ViewFormatObject *obj, COptionTreeItem* root)
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
	otiEdit = (COptionTreeItemEdit*)m_mTrees[compName.GetUniqueID()].m_Tree->InsertItem(new COptionTreeItemEdit(), root);
	otiEdit->SetLabelText(name.c_str());
	otiEdit->SetInfoText(_T(""));	
}

void COptionTreeWrapper::CreateStaticItem(CHashString compName, ViewFormatObject *obj, COptionTreeItem* root)
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
	otiStatic = (COptionTreeItemStatic*)m_mTrees[compName.GetUniqueID()].m_Tree->InsertItem(new COptionTreeItemStatic(), root);
	otiStatic->SetLabelText(name.c_str());
	otiStatic->SetInfoText(_T(""));
	otiStatic->CreateStaticItem(0);
}

void COptionTreeWrapper::CreateHiddenItem(CHashString compName, ViewFormatObject *obj, COptionTreeItem* root)
{
	IHiddenViewObject *hiddenObject = NULL;
	hiddenObject = dynamic_cast<IHiddenViewObject*>(obj);

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
	otiStatic = (COptionTreeItemStatic*)m_mTrees[compName.GetUniqueID()].m_Tree->InsertItem(new COptionTreeItemStatic(), root);
	otiStatic->SetOptionsOwner( NULL );
//	otiStatic->SetLabelText(name.c_str());
//	otiStatic->SetInfoText(_T(""));
//	otiStatic->CreateStaticItem(0);	
}

void COptionTreeWrapper::CreateSpinnerItem(CHashString compName, ViewFormatObject *obj, COptionTreeItem* root)
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
	double defVal = (double)spinnerObject->GetDefaultValue();

	// create COptionTreeItemSpinner
	COptionTreeItemSpinner *otiSpinner;
	otiSpinner = (COptionTreeItemSpinner*)m_mTrees[compName.GetUniqueID()].m_Tree->InsertItem(new COptionTreeItemSpinner(), root);
	otiSpinner->SetLabelText(name.c_str());
	otiSpinner->SetInfoText(_T(""));	
	otiSpinner->CreateSpinnerItem( OT_EDIT_USEREDIT, defVal, spinnerObject->GetMin(), spinnerObject->GetMax(), increment );
}

void COptionTreeWrapper::CreateCheckBoxItem(CHashString compName, ViewFormatObject *obj, COptionTreeItem* root)
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
	otiCheckBox = (COptionTreeItemCheckBox*)m_mTrees[compName.GetUniqueID()].m_Tree->InsertItem(new COptionTreeItemCheckBoxEx(), root);
	otiCheckBox->SetLabelText(name.c_str());
	otiCheckBox->SetInfoText(_T(""));
	otiCheckBox->CreateCheckBoxItem( 0, OT_CHECKBOX_SHOWCHECK );
	otiCheckBox->SetCheck(false);
}

void COptionTreeWrapper::CreateRadioItem(CHashString compName, ViewFormatObject *obj, COptionTreeItem* root)
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
	otiRadio = (COptionTreeItemRadio*)m_mTrees[compName.GetUniqueID()].m_Tree->InsertItem(new COptionTreeItemRadio(), root);
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

void COptionTreeWrapper::CreateFileItem(CHashString compName, ViewFormatObject *obj, COptionTreeItem* root)
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
	otiFile = (COptionTreeItemFile*)m_mTrees[compName.GetUniqueID()].m_Tree->InsertItem( new COptionTreeItemFile(), root );
	otiFile->SetLabelText(name.c_str());
	otiFile->SetInfoText(_T(""));

	otiFile->CreateFileItem((const TCHAR*)fileName,
							defExt.c_str(),
							extFilter.c_str(),
							OT_FILE_OPENDIALOG | OT_FILE_SHOWFULLPATH,
							OFN_OVERWRITEPROMPT);

}

void COptionTreeWrapper::CreateComboItem(CHashString compName, ViewFormatObject *obj, COptionTreeItem* root)
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

	otiCombo = (COptionTreeItemComboBox*)m_mTrees[compName.GetUniqueID()].m_Tree->InsertItem(new COptionTreeItemComboBox(), root);
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

void COptionTreeWrapper::SerializeEditItemInt(IArchive &ar, COptionTreeItem *item, bool read)
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

void COptionTreeWrapper::SerializeEditItemFloat(IArchive &ar, COptionTreeItem *item, bool read)
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

void COptionTreeWrapper::SerializeEditItemDouble(IArchive &ar, COptionTreeItem *item, bool read)
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

void COptionTreeWrapper::SerializeEditItemString(IArchive &ar, COptionTreeItem *item, bool read)
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

void COptionTreeWrapper::SerializeStaticItemInt(IArchive &ar, COptionTreeItem *item, bool read)
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

void COptionTreeWrapper::SerializeStaticItemFloat(IArchive &ar, COptionTreeItem *item, bool read)
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

void COptionTreeWrapper::SerializeStaticItemDouble(IArchive &ar, COptionTreeItem *item, bool read)
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

void COptionTreeWrapper::SerializeStaticItemString(IArchive &ar, COptionTreeItem *item, bool read)
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

void COptionTreeWrapper::SerializeStaticItemBool(IArchive &ar, COptionTreeItem *item, bool read)
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

void COptionTreeWrapper::SerializeSpinnerItemInt(IArchive &ar, COptionTreeItem *item, bool read)
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

void COptionTreeWrapper::SerializeSpinnerItemFloat(IArchive &ar, COptionTreeItem *item, bool read)
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

void COptionTreeWrapper::SerializeSpinnerItemDouble(IArchive &ar, COptionTreeItem *item, bool read)
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

void COptionTreeWrapper::SerializeRadioItem(IArchive &ar, COptionTreeItem *item, bool read)
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

void COptionTreeWrapper::SerializeCheckBoxItem(IArchive &ar, COptionTreeItem *item, bool read)
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

void COptionTreeWrapper::SerializeFileItem(IArchive &ar, COptionTreeItem *item, bool read)
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
		CString fName = (const TCHAR*)fileName;
		GetRelativePath(fName, szItem);
		if ( _tcscmp(szItem, _T("")) != 0 )
		{
			otiFile->SetFileString(szItem);
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
//		CGDSApp *app = DYNAMIC_DOWNCAST(CGDSApp, AfxGetApp());
//		CString resourcePath = app->GetResourcePath();
		StdString rootDir, resDir;
		EngineGetToolBox()->GetDirectories(&rootDir, &resDir);
		resDir += "\\";
		resDir += szItem;
		otiFile->SetFileString( szItem );
		// do not want to save absolute path! my bad
//		ar.Write(resDir);
		ar.Write(szItem);
	}
}

void COptionTreeWrapper::SerializeComboItem(IArchive &ar, COptionTreeItem *item, bool read)
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

void COptionTreeWrapper::ClearTree()
{
	// delete all items
	for (UINT i=0; i<m_vRootNames.size(); i++)
	{
		m_mTrees[m_vRootNames[i].GetUniqueID()].m_Tree->DeleteAllItems();	
	}
}

void COptionTreeWrapper::SerializeIn(IArchive &Archive, CHashString name)
{
	CHashString basicType(_T(""));
	CHashString OptionTreeItemType(_T(""));

	// get the first option tree item in tree
	COptionTreeItem *item = m_mTrees[name.GetUniqueID()].m_Root->GetChild();

	if (item == NULL)
	{
		return;
	}

	for (UINT i=0; i<m_mViewOList[name.GetUniqueID()].size(); i++)
	{

		VIEWOBJECTLIST::iterator volIter = m_mViewOList[name.GetUniqueID()][i]->begin();

		for(; volIter != m_mViewOList[name.GetUniqueID()][i]->end(); volIter++)
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
			(this->*funcPtr)(Archive, item, true);

			
			item = item->GetSibling();
		}
	}
}

void COptionTreeWrapper::SerializeOut(IArchive &Archive, CHashString name)
{
	CHashString basicType(_T(""));
	CHashString OptionTreeItemType(_T(""));

	// get the first option tree item in tree
	COptionTreeItem *item = m_mTrees[name.GetUniqueID()].m_Root->GetChild();

	if (item == NULL)
	{
		return;
	}

	for (UINT i=0; i<m_mViewOList[name.GetUniqueID()].size(); i++)
	{

		VIEWOBJECTLIST::iterator volIter = m_mViewOList[name.GetUniqueID()][i]->begin();

		for(; volIter != m_mViewOList[name.GetUniqueID()][i]->end(); volIter++)
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
}

bool COptionTreeWrapper::GetRelativePath( CString &fullPath, CString &filePath )
{
//	CGDSApp *app = DYNAMIC_DOWNCAST(CGDSApp, AfxGetApp());
	CString resourcePath;// = app->GetResourcePath();
	StdString rootDir, resDir;
	EngineGetToolBox()->GetDirectories(&rootDir, &resDir);
	resourcePath = (const TCHAR*) resDir;


	CString basePath = resourcePath;//m_szBasePath;

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

bool COptionTreeWrapper::CreateTree(const StdString &compType, CHashString compName)
{
	CHashString OptionTreeItemType(_T(""));

	VIEWOBJECTLIST *vol;

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
	if (m_ToolBox->SendMessage(msgHash_GetSchema, sizeof(schemaparams), &schemaparams, &compName, &CHashString(compType) ) == MSG_HANDLED)
	{
		MemArchive->SetIsWriting( false );
		StdString compType;
		MemArchive->GetNode(compType);
		vol = m_ControlSchema.AddSchemaFromArchive( compType, MemArchive );
	}
	MemArchive->Close();
	
	vol = m_ControlSchema.GetViewObjectList(compType);
	if (vol == NULL)
	{
		// warning, component type is unknown
		StdString error = _T("Component type of this type is unknown: ");
		error += compType;
		::MessageBox(NULL, error, _T("INVALID OPERATION"), MB_OK);
		return false;
	}

	VIEWOBJECTLIST::iterator volIter = vol->begin();

	for(; volIter != vol->end(); volIter++)
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

		(this->*funcPtr)(compName, vo, m_mTrees[compName.GetUniqueID()].m_Root);
	}
	m_mTrees[compName.GetUniqueID()].m_Tree->ExpandAllItems();
	
	m_mViewOList[compName.GetUniqueID()].push_back(vol);

	return true;
}

void COptionTreeWrapper::CreateColorItem(CHashString compName, ViewFormatObject *obj, COptionTreeItem* root)
{
	IColorViewObject *colorObject= dynamic_cast<IColorViewObject*>(obj);

	if(colorObject == NULL)
	{
		StdString error = _T("Could not cast to a CColorViewObject");
		::MessageBox(NULL, error, _T("Invalid Operaton"), MB_OK);
		return;
	}

	StdString type = colorObject->GetBasicType();
	StdString name = colorObject->GetName();
	int defColor = colorObject->GetDefaultColor();
	
	// create COptionTreeItem
	COptionTreeItemColor *otiColor;
	otiColor = (COptionTreeItemColor*)m_mTrees[compName.GetUniqueID()].m_Tree->InsertItem(new COptionTreeItemColor(), root);
	otiColor->SetLabelText(name.c_str());
	otiColor->SetInfoText(_T(""));
	otiColor->CreateColorItem(0, defColor, defColor);
}

void COptionTreeWrapper::SerializeColorItem(IArchive &ar, COptionTreeItem *item, bool read)
{
	int iVal;
	COptionTreeItemColor *otiColor;

	otiColor = dynamic_cast<COptionTreeItemColor*>(item);

	if (otiColor == NULL)
	{
		//error
		StdString error = _T("Could not cast item to COptionTreeItemColor: ");
		error += item->GetLabelText();
		::MessageBox(NULL, error, _T("Invalid Command"), MB_OK);
		return;
	}

	if (read)
	{
		ar.Read(iVal);
        if (iVal >= 0)
		{
			// our format: rgb... old format: (a)bgr
            DWORD mask = 0xff0000;
			unsigned int r, g, b;
			DWORD outColor = 0;
			r = (iVal & mask) >> 16;
			outColor += r;
			mask = 0x00ff00;
			g = (iVal & mask) >> 8;
			outColor += g << 8;
			mask = 0x0000ff;
			b = (iVal & mask);
			outColor += b << 16;
			otiColor->SetColor(outColor);
		}
	}
	else
	{
		iVal = (int)(otiColor->GetColor());
		// our format: rgb... old format: (a)bgr
		DWORD mask = 0x0000ff;
		unsigned int r, g, b;
		DWORD outColor = 0;
		r = (iVal & mask);
		outColor += r << 16;
		mask = 0x00ff00;
		g = (iVal & mask) >> 8;
		outColor += g << 8;
		mask = 0xff0000;
		b = (iVal & mask) >> 16;
		outColor += b;
		ar.Write((unsigned int)outColor);
	}
}

void COptionTreeWrapper::ExpandOneRoot(CHashString name)
{
	for (UINT i=0; i<m_vRootNames.size(); i++)
	{
		if (name == m_vRootNames[i])
		{
			RECT rect;
			m_pParentWnd->GetClientRect(&rect);
			m_mTrees[m_vRootNames[i].GetUniqueID()].m_Tree->SetWindowPos(&CWnd::wndTop, rect.left, rect.top, rect.right, rect.bottom, 0);
			m_mTrees[m_vRootNames[i].GetUniqueID()].m_Tree->ShowWindow(SW_SHOW);
			m_mTrees[m_vRootNames[i].GetUniqueID()].m_Tree->SetNotify(TRUE, m_pParentWnd);
		}
		else
		{
			m_mTrees[m_vRootNames[i].GetUniqueID()].m_Tree->ShowWindow(SW_HIDE);
			m_mTrees[m_vRootNames[i].GetUniqueID()].m_Tree->SetNotify(FALSE, m_pParentWnd);
		}		
	}
	if (_tcscmp(name.GetString(), _T(""))!= 0)
	{
		m_mTrees[NULL].m_Tree->ShowWindow(SW_HIDE);
		m_mTrees[NULL].m_Tree->SetNotify(FALSE, m_pParentWnd);
	}
	else
	{
		m_mTrees[NULL].m_Tree->ShowWindow(SW_SHOW);
		m_mTrees[NULL].m_Tree->SetNotify(TRUE, m_pParentWnd);
	}
}
