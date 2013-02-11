///============================================================================
/// \file		ObjectHelpers.h
/// \brief		helpers' implementation for EE object interactions
/// \date		09-06-2007
/// \author		Andrey Ivanov
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

namespace GDS_SDK_Helpers
{

LPCTSTR defaultObjectsFile = _T("/objectProperties/DefaultObjects.xgd");
LPCTSTR objectDetailsFile = _T("/objectProperties/ObjectDetails.xml");

/////////////////// CXMLArchiveHolder //////////////////////////////////////////
/// \brief wrapper class for IXMLArchive *. It closes XMLArchive in destructor
class CXMLArchiveHolder
{
public:
	CXMLArchiveHolder()
		: m_archive(NULL)
	{
	}

	CXMLArchiveHolder(IXMLArchive *archive)
		: m_archive(archive)
	{
	}

	~CXMLArchiveHolder()
	{
		Release();
	}

	CXMLArchiveHolder& operator=(IXMLArchive *archive)
	{
		if (m_archive != archive)
		{
			Release();
			m_archive = archive;
		}
		return *this;
	}

	operator IXMLArchive*()
	{
		return m_archive;
	}
	
	IXMLArchive* operator->()
	{
		return m_archive;
	}

	void Release()
	{
		if (m_archive != NULL)
		{
			m_archive->Close();
			m_archive = NULL;
		}
	}
private:
	IXMLArchive *m_archive;
};

/////////////////// CObjectLabelChanger ////////////////////////////////////////
class CObjectLabelChanger
{
public:
	CObjectLabelChanger(const StdString &tag, const StdString &value)
		: m_tag(tag), m_value(value)
	{
	}

	bool operator()(const StdString &nodeName, StdString &value, const StdString &tag) const
	{
		if (tag == m_tag)
		{
			value = m_value;
		}
		return true;
	}

private:
	StdString m_tag;
	StdString m_value;
};

/////////////////// CObjectDetails /////////////////////////////////////////////
class CObjectDetails
{
	typedef map<DWORD, CObjectDetails> DetailsMap;
	CObjectDetails() {};
public:
	static const CObjectDetails &GetDetails(IHashString *pType);

	StdString GetFileName(IHashString *name) const;
	bool HasLabel() const;
	StdString GetLabel(IHashString *name) const;
	void SetLabel(IHashString *name, const StdString &label) const;
private:
	static DetailsMap Load();

private:
	StdString m_name;
	StdString m_fileNameProperty;
	StdString m_labelProperty;
	static DetailsMap m_Details;
};

CObjectDetails::DetailsMap CObjectDetails::Load()
{
	DetailsMap res;

	IToolBox *toolBox = ::EngineGetToolBox();
	StdString xmlFile;
	toolBox->GetDirectories(&xmlFile, NULL);
	xmlFile += objectDetailsFile;

	CXMLArchiveHolder archive = CreateXMLArchive(xmlFile);
	if (archive != NULL)
	{
		StdString sNode;
		archive->GetNode(sNode);
		ASSERT(sNode == _T("objects"));
		while (archive->GetNode(sNode))
		{
			CObjectDetails details;
			int count = archive->GetNumAttributes();
			while (count--)
			{
				StdString sTag = archive->GetCurrentNodeName();
				StdString sValue;
				archive->Read(sValue, sTag);
				if (sTag == _T("name"))
				{
					details.m_name = sValue;
				}
				else if (sTag == _T("FileNameProperty"))
				{
					details.m_fileNameProperty = sValue;
				}
				else if (sTag == _T("LabelProperty"))
				{
					details.m_labelProperty = sValue;
				}
			}
			const DWORD hashType = CHashString(sNode).GetUniqueID();
			res.insert(make_pair(hashType, details));
		}
	}

	return res;
}

const CObjectDetails &CObjectDetails::GetDetails(IHashString *pType)
{
	ASSERT(pType != NULL);
	static DetailsMap map = Load();
	const DWORD hashType = pType->GetUniqueID();
	DetailsMap::const_iterator it = map.find(hashType);
	if (it != map.end())
	{
		return it->second;
	}
	static CObjectDetails empty;
	return empty;
}

StdString CObjectDetails::GetFileName(IHashString *name) const
{
	if (m_fileNameProperty.empty())
	{
		return _T("");
	}
	return GetEEObjectProperty(name, m_fileNameProperty);
}

bool CObjectDetails::HasLabel() const
{
	return !m_labelProperty.empty();
}

StdString CObjectDetails::GetLabel(IHashString *name) const
{
	if (HasLabel())
	{
		return GetEEObjectProperty(name, m_labelProperty);
	}
	return name->GetString();
}

void CObjectDetails::SetLabel(IHashString *name, const StdString &label) const
{
	if (HasLabel())
	{
		SetEEObjectProperty(name, m_labelProperty, label);
	}
}

/////////////////// functions //////////////////////////////////////////////////
/// \brief helper function for loading DefaultObjects.xgd
/// \return pointer IXMLArchive with DefaultObjects.xgd content
static IXMLArchive *GetDefaultObjectsArchive()
{
	IToolBox *toolBox = ::EngineGetToolBox();
	StdString xmlFile;
	toolBox->GetDirectories(&xmlFile, NULL);
	xmlFile += defaultObjectsFile;

	return CreateXMLArchive(xmlFile);
}

/// \brief	Check object exists in the hierarchy
/// \param	name - object name for check
/// \return	true if object exists in the hierarchy
bool DoesEEObjectExist(IHashString *name)
{
	// try to leave passed name without changes
	FINDOBJECTPARAMS param;
	param.hszName = name;
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	if (MSG_HANDLED != EngineGetToolBox()->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param) ||
		param.bFound)
	{
		return true;
	}
	return false;
}

/// \brief Finds available name in object store. Form of name prefix_XXX, where XXX is a number
/// \param prefix - prefix for object name. Typically this is type string 
/// \return unique name for new object
StdString FindUniqueEEObjectName(const StdString &prefix)
{
	CHashString hszPrefix = prefix;
	CHashString hszName = prefix;
	GENERATEUNIQUEOBJECTNAMEPARAMS guon;
	guon.name = &hszPrefix;
	guon.newname = &hszName;

	static DWORD msgHash_GenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(guon), &guon);
	return hszName.GetString();
}

/// \brief	Creates new object in the scene hierarchy.
///			Uses DefaultObjects.xgd for filling object properties
/// \param	parent - name of the parent for newly created object
/// \param	objectType - hash string of type of new object
/// \param	name - [in]  hash string with object name prefix
///					  [out] result object name
/// \param	archive - arhive for initalization. 
///			Default archive is used if NULL is passed
/// \param	sendAdd - set to true if AddObject message should be sent to EE
/// \return	true if object was created successfully
bool CreateEEObject(IHashString *parent, IHashString *objectType, CHashString &name, IArchive *archive, bool sendAdd)
{
	IToolBox *toolBox = ::EngineGetToolBox();

	// try to leave passed name without changes
	if (DoesEEObjectExist(&name))
	{
		// finding available name for new object
		name = FindUniqueEEObjectName(name.GetString()).c_str();
	}

	// preparing parameters for create object 
	CREATEOBJECTPARAMS cop;
	cop.typeName = objectType;
	cop.name = &name;
	cop.parentName = parent;

	// then creating it
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	if (toolBox->SendMessage(msgHash_CreateObject, sizeof(cop), &cop) != MSG_HANDLED)
	{
		// log error
		toolBox->SetErrorValue(WARN_OBJECT_NOT_FOUND);
		toolBox->Log(LOGWARNING, _T("%s(%i): Type '%s' not found, create failed!\n"), __FILE__, __LINE__, objectType->GetString());
		return false;
	}

	if (archive != NULL)
	{
		// setup up serialize parameters
		SERIALIZEOBJECTPARAMS sop;
		sop.name = cop.name;
		sop.archive = archive;

		// serialize object from archive
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		if (MSG_HANDLED == toolBox->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop))
		{
			// initialize object
			INITOBJECTPARAMS iop;
			iop.name = cop.name;
			static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
			toolBox->SendMessage(msgHash_InitObject, sizeof(iop), &iop);
		}
	}

	if (sendAdd)
	{
		// send out a message that an object needs to be added
		static DWORD msgHash_AddObject = CHashString(_T("AddObject")).GetUniqueID();
		toolBox->SendMessage(msgHash_AddObject, sizeof(cop), &cop);
	}

	return true;
}

/// \brief Creates new object in the scene hierarchy
/// \param parent - name of the parent for newly created object
/// \param objectType - hash string of type of new object
/// \param resourcePath - path to resource file of object. 
///		   Default path is used if NULL is passed
CHashString CreateDefaultEEObject(IHashString *parent, IHashString *objectType, LPCTSTR resourcePath)
{
	// load default objects archive
	static CXMLArchiveHolder defaultArchive = GetDefaultObjectsArchive();
	ASSERT(defaultArchive != NULL);

	IToolBox *toolBox = ::EngineGetToolBox();
	if (parent == NULL)
	{
		// create a blank scene if necessary
		static DWORD msgHash_CreateBlankScene = CHashString(_T("CreateBlankScene")).GetUniqueID();
		toolBox->SendMessage(msgHash_CreateBlankScene, 0, NULL);
	}

	CHashString name(_T(""));

	if (defaultArchive != NULL)
	{
		// initialize object if defaultArhive contains data
		// for the passed object type
		defaultArchive->SeekTo(0, SEEK_SET);
		StdString compType;
		while (defaultArchive->GetNode(compType))
		{
			if (compType == objectType->GetString())
			{
				StdString sFilenameParameter;
				// skip name attribute
				defaultArchive->Read(sFilenameParameter);
				// read resource filename parameter
				defaultArchive->Read(sFilenameParameter);

				CXMLArchiveHolder pData;
				IArchive *archive = defaultArchive;
				if (!sFilenameParameter.empty() && resourcePath != NULL)
				{
					// if path to resource file was passed and object support such path
					// then this value should be updated in a copy of default archive
					pData = CreateXMLArchiveStream();
					ASSERT(pData != NULL);
					// opens the archive
					pData->StartClass(compType);
					const int nodePosition = defaultArchive->GetCurrentNodePosition();
					do
					{
						StdString sNodeName = defaultArchive->GetCurrentNodeName();
						StdString sValue;
						defaultArchive->Read(sValue, sNodeName);
						if (sFilenameParameter == sNodeName)
						{
							// write passed path as filename
							pData->Write(resourcePath, sNodeName);
						}
						else
						{
							// copy value otherwise
							pData->Write(sValue, sNodeName);
						}
					}
					while (nodePosition == defaultArchive->GetCurrentNodePosition());
					// flush written data to the stream
					pData->EndClass();
					pData->SetIsWriting(false);
					archive = pData;
				}

				// generate new unique numbered name for object
				CHashString objectName = FindUniqueEEObjectName(objectType->GetString()).c_str();

				if (CreateEEObject(parent, objectType, objectName, archive))
				{
					MOUSECLICKPARAMS mcp;
					mcp.object = &objectName;
					mcp.compType = objectType;
					// select it (not sure if this is the best way to select it or not)
					static DWORD msgHash_LeftClickObject = CHashString(_T("LeftClickObject")).GetUniqueID();
					toolBox->SendMessage(msgHash_LeftClickObject, sizeof(MOUSECLICKPARAMS), &mcp);
					name = objectName;
				}
			}
		}
	}
	return name;
}

/// \brief	Delete passed object from hierarchy
/// \param	name - name of the object for deletion
void DeleteEEObject(IHashString *name)
{
	DELETEOBJECTPARAMS dop = {name};
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop, NULL, NULL);
	ASSERT(retval == MSG_HANDLED);
}

///	\brief	get type for passed object
///	\param	name - name of the object for finding type
///	\return CHashString with type of the object
CHashString GetComponentType(IHashString *name)
{
	// getting type of object
	GETCOMPONENTTYPEPARAMS gct;
	gct.name = name;
	gct.componenttype = NULL;

	IToolBox *pToolBox = ::EngineGetToolBox();
	static DWORD msgHash_GetComponentType = CHashString(_T("GetComponentType")).GetUniqueID();
	if (pToolBox->SendMessage(msgHash_GetComponentType, sizeof(GETCOMPONENTTYPEPARAMS), &gct) != MSG_HANDLED ||
		gct.componenttype == NULL)
	{
		pToolBox->Log(LOGWARNING, _T("%s(%i): Component type cannot be found for object with name: '%s'.\n"), __FILE__, __LINE__, name->GetString());
		static CHashString hszEmptyType(_T(""));
		return hszEmptyType;
	}
	else
	{
		return gct.componenttype;
	}
}

///	\brief	get parent name of passed object
///	\param	name - name of the object for finding type
///	\return CHashString with name of the parent object
CHashString GetComponentParent(IHashString *name)
{
	CHashString hszType = GetComponentType(name);
	CHashString parentName;
	CChildFindVisitor visitor(&hszType);

	VISITHIERARCHYDATA vhd;
	vhd.name = name;
	vhd.visitor = &visitor;
	IToolBox *pToolBox = EngineGetToolBox();
	static DWORD msgHash_VisitHierarchy = CHashString(_T("VisitHierarchy")).GetUniqueID();
	if (pToolBox->SendMessage(msgHash_VisitHierarchy, sizeof(vhd), &vhd) == MSG_HANDLED)
	{
		vector<IComponent *> children = visitor.GetChildren();
		if (!children.empty())
		{
			IObject *pChild = dynamic_cast<IObject *>(children.front());
			if (pChild != NULL)
			{
				return pChild->GetParentName();
			}
			else
			{
				pToolBox->Log(LOGERROR, _T("%s(%i): GetComponentParent(%s) cannot get found object name."),
												__FILE__, __LINE__, name->GetString());
			}
			if (children.size() > 1)
			{
				pToolBox->Log(LOGWARNING, _T("%s(%i): GetComponentParent(%s) found multiple objects."),
					__FILE__, __LINE__, name->GetString());
			}
		}
	}
	return parentName;
}

/// \brief	Determines filename of object resource
///			Uses DefaultObjects.xgd for finding filename property
/// \param	name - name of scene object to check
/// \return string with resource file. This value may be empty 
///			if object does not use external resource
StdString GetEEObjectResourceFilename(IHashString *name)
{
	CHashString hszType = GetComponentType(name);
	return CObjectDetails::GetDetails(&hszType).GetFileName(name);
}

/// \brief	Check if passed object has label
/// \param	name - name of object
/// \return true if object supports label
bool HasEEObjectLabel(IHashString *name)
{
	CHashString hszType = GetComponentType(name);
	return CObjectDetails::GetDetails(&hszType).HasLabel();
}

/// \brief	Check if label of passed object is editable
/// \param	name - name of object
/// \return true if object supports label editing
bool IsEEObjectLabelEditable(IHashString *name)
{
	ASSERT(HasEEObjectLabel(name));
	CHashString hszType = GetComponentType(name);
	bool editable = true;
	IToolBox *pToolBox = EngineGetToolBox();
	static DWORD msgHash = CHashString(_T("IsLabelEditble")).GetUniqueID();
	if (pToolBox->SendMessage(msgHash, sizeof(editable), &editable, name, &hszType) != MSG_HANDLED)
	{
		editable = true;
	}
	return editable;
}

/// \brief	Extract object label from serialized object data
/// \param	name - name of object
/// \return string with object's label. It return the same value as object name
///			if object does not support labels
StdString GetEEObjectLabel(IHashString *name)
{
	CHashString hszType = GetComponentType(name);
	return CObjectDetails::GetDetails(&hszType).GetLabel(name);
}

/// \brief	set new object label
/// \param	name - name of object
/// \param	label - new label for the object
void SetEEObjectLabel(IHashString *name, const StdString &label)
{
	CHashString hszType = GetComponentType(name);
	CObjectDetails::GetDetails(&hszType).SetLabel(name, label);
}

///	\brief	serialize object properties to archive
///	\param	name - pointer to IHashString with object name
///	\return	pointer to memory archive with serialized data. Archive is opened 
///			for reading in this case. If function fails then returned value is NULL
IArchive *GetEEObjectProperties(IHashString *name)
{
	IArchive *archive = CreateMemoryArchive();
	if (archive != NULL)
	{
		// setup up serialization parameters
		SERIALIZEOBJECTPARAMS sop;
		sop.name = name;
		sop.archive = archive;
		// serialize object from archive
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		DWORD res = EngineGetToolBox()->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop);
		if (res == MSG_HANDLED)
		{
			archive->SetIsWriting(false);
		}
		else
		{
			archive->Close();
			archive = NULL;
		}
	}
	return archive;
}

///	\brief	update specific property for passed object
///	\param	name - pointer to IHashString with object name
///	\param	tag	- tag of the property
///	\return property value 
StdString GetEEObjectProperty(IHashString *name, LPCTSTR tag)
{
	CXMLArchiveHolder archive = CreateXMLArchiveStream();
	ASSERT(archive != NULL);
	// opens the archive
	archive->SetIsWriting(true);
	archive->StartClass(_T("data"));

	// setup up serialize parameters for loading
	SERIALIZEOBJECTPARAMS sop;
	sop.name = name;
	sop.archive = archive;
	// serialize object from archive
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	if (MSG_HANDLED != EngineGetToolBox()->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop))
	{
		return StdString();
	}
	// flush written data to the stream
	archive->EndClass();
	// then preparing for reading serialized values
	archive->SetIsWriting(false);

	StdString sNodeName;
	archive->GetNode(sNodeName);
	int count = archive->GetNumAttributes();
	while (count--)
	{
		StdString sName = archive->GetCurrentNodeName();
		StdString sValue;
		archive->Read(sValue);
		if (tag == sName)
		{
			// we found the parameter value, so just return it
			return sValue;
		}
	}
	ASSERT(!"NameProperty in ObjectDetails.xml has wrong value");
	EngineGetToolBox()->Log(LOGWARNING,
							_T("%s(%i): NameProperty in ObjectDetails.xml has wrong value for object %s.\n"),
							__FILE__, __LINE__, name->GetString());
	return StdString();
}

///	\brief	update specific property for passed object
///	\param	name - pointer to IHashString with object name
///	\param	tag	- tag of the property
///	\param	value - new property value 
void SetEEObjectProperty(IHashString *name, LPCTSTR tag, LPCTSTR value)
{
	IXMLArchive *archive = CreateXMLArchiveStream();
	archive->SetIsWriting(true);
	archive->StartClass(_T("data"));

	// setup up serialize parameters for loading
	SERIALIZEOBJECTPARAMS sop;
	sop.name = name;
	sop.archive = archive;
	IToolBox *toolBox = EngineGetToolBox(); 
	// serialize object from archive
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	if (MSG_HANDLED != toolBox->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop))
	{
		archive->Close();
		return;
	}
	// flush written data to the stream
	archive->EndClass();
	// then preparing for reading serialized values
	archive->SetIsWriting(false);

	sop.archive = TransformXMLArchive(archive, CObjectLabelChanger(tag, value));
	if (sop.archive == NULL)
	{
		archive->Close();
	}
	else
	{
		sop.archive->SetIsWriting(false);
		if (MSG_HANDLED == toolBox->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop))
		{
			INITOBJECTPARAMS iop;
			iop.name = name;
			static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
			toolBox->SendMessage(msgHash_InitObject, sizeof(iop), &iop);
		}
		sop.archive->Close();
	}
}

///	\brief	serialize object properties to archive
///	\param	name - pointer to IHashString with object name
///	\param	ar - pointer to IArchive with object state to compare
///	\return	true if current object state and state in passed archive are differ
bool IsEEObjectChanged(IHashString *name, IArchive *ar)
{
	IArchive *pCurrentData = GetEEObjectProperties(name);
	if (pCurrentData == NULL)
	{
		return ar == NULL;
	}

	if (ar == NULL)
	{
		pCurrentData->Close();
		return false;
	}
	// compare stream content
	ASSERT(ar->IsReading());
	DWORD dwOffset = 0;
	pCurrentData->SeekTo(0);
	while (!ar->IsEndOfFile() && !pCurrentData->IsEndOfFile())
	{
		--dwOffset;
		char ch1;
		char ch2;
		ar->Read(ch1);
		pCurrentData->Read(ch2);
		if (ch1 != ch2)
		{
			ar->SeekTo(dwOffset, SEEK_CUR);
			// got first change
			pCurrentData->Close();
			return true;
		}
	}

	ar->SeekTo(dwOffset, SEEK_CUR);

	if (pCurrentData->IsEndOfFile())
	{
		// ignore data after current buffer size, passed archive may contain 
		// data from different sources
		// treating object state as equal
		pCurrentData->Close();
		return false;
	}
	// streams have different sizes
	pCurrentData->Close();
	return true;
}

///	\brief	find first child object of known type
///	\param	parent - name of root object in hierarchy to find
///	\param	childType - component type of the child object
///	\return	name of found object. Empty string if object was not found
CHashString GetEEObjectChildName(IHashString *parent, IHashString *childType)
{
	CHashString childName(_T(""));
	CChildFindVisitor visitor(childType);

	VISITHIERARCHYDATA vhd;
	vhd.name = parent;
	vhd.visitor = &visitor;
	IToolBox *pToolBox = EngineGetToolBox();
	static DWORD msgHash_VisitHierarchy = CHashString(_T("VisitHierarchy")).GetUniqueID();
	if (pToolBox->SendMessage(msgHash_VisitHierarchy, sizeof(vhd), &vhd) == MSG_HANDLED)
	{
		vector<IComponent *> children = visitor.GetChildren();
		if (!children.empty())
		{
			IObject *pChild = dynamic_cast<IObject *>(children.front());
			if (pChild != NULL)
			{
				childName = pChild->GetName();
			}
			else
			{
				pToolBox->Log(LOGERROR, _T("%s(%i): GetChildName(%s, %s) cannot get found object name."),
										__FILE__, __LINE__,
										parent->GetString(), childType->GetString());
			}
			if (children.size() > 1)
			{
				pToolBox->Log(LOGWARNING, _T("%s(%i): GetChildName(%s, %s) found multiple objects."),
											__FILE__, __LINE__,
											parent->GetString(), childType->GetString());
			}
		}
	}
	return childName;
}

///	\brief	fill vector with child objects for parent
///	\param	parent - name of root object in hierarchy to find
///	\param	children - list with children
///	\param	sendToParent - if this value is true then VisitHierarchy message is 
///						   sent directly to the object with name parent
///	\return	EE return code
DWORD GetEEObjectChildren(IHashString *parent, vector<IObject *> &children, bool sendToParent)
{
	// preventing incorrect warning
	if (!DoesEEObjectExist(parent))
	{
		return MSG_NOT_HANDLED;
	}
	CHashString childName;
	CChildFindVisitor visitor(NULL);

	IHashString *pName = NULL;
	IHashString *pType = NULL;
	CHashString hszType;
	if (sendToParent)
	{
		hszType = GetComponentType(parent);
		pName = parent;
		pType = &hszType;
	}
	VISITHIERARCHYDATA vhd;
	vhd.name = parent;
	vhd.visitor = &visitor;
	IToolBox *pToolBox = EngineGetToolBox();
	static DWORD msgHash_VisitHierarchy = CHashString(_T("VisitHierarchy")).GetUniqueID();
	DWORD dwRes = pToolBox->SendMessage(msgHash_VisitHierarchy, sizeof(vhd), &vhd, pName, pType);
	if (dwRes == MSG_HANDLED)
	{
		vector<IComponent *> componentChildren = visitor.GetChildren();
		children.reserve(children.size() + componentChildren.size());
		vector<IComponent *>::iterator it = componentChildren.begin();
		for(; it != componentChildren.end(); ++it)
		{
			IObject *obj = dynamic_cast<IObject *>(*it);
			if (obj != NULL)
			{
				children.push_back(obj);
			}
		}
	}
	return dwRes;
}

} //namespace ObjectHelpers
