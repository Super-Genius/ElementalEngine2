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

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CControlSchema::CControlSchema(void)
{
	m_ToolBox = EngineGetToolBox();
}

int CControlSchema::RegisterCreateObject(DWORD id, VOCREATEFUNC createFunc)
{
	MAPTYPETOVIEWFUNC *viewCreateFuncs;
	viewCreateFuncs = SINGLETONINSTANCE(CSchemaRegistered)->GetRegisterTypesMap();
	// setting up viewformatobject create functions
	(*viewCreateFuncs)[id] = createFunc;
	return true;
}

CControlSchema::~CControlSchema(void)
{
	// traverse through comptype to vo list and delete lists and vo objects
	COMPTYPETOVOLISTMAP::iterator cttvlmIter = m_CompTypeMap.begin();
	COMPTYPETOVOLISTMAP::iterator delCompTypeIter;
	VIEWOBJECTLIST::iterator volIter;
	VIEWOBJECTLIST::iterator delVolIter;
	VIEWOBJECTLIST *volList;
	ViewFormatObject *vfObject;

	while( cttvlmIter != m_CompTypeMap.end() )
	{
		// get the view object list
		volList = cttvlmIter->second;
		volIter = volList->begin();
		// traverse through list deleting view objects
		while( volIter != volList->end() )
		{
			vfObject = (*volIter);
			volIter++;
			delete vfObject;
		}
		cttvlmIter++;
		
		delete volList;
	}

}


BOOL CControlSchema::ReadXML(const StdString &fileName)
{
	CREATEARCHIVE ca;
	IXMLArchive *ar;
	StdString root;
	StdString compType;
	int iNumCompTypes;
	CHashString streamType(_T("File"));

	ca.streamData = (void*)(fileName.c_str());
	ca.mode = STREAM_MODE_READ;
	ca.streamType = &streamType;

	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED) 
	{ 
		return FALSE;
	}

	// Start reading from the archive
	ar = static_cast<IXMLArchive *>(ca.archive);

	ar->GetNode(root);
	ar->Read(iNumCompTypes);
	
	while( iNumCompTypes != 0 )
	{
		ar->GetNode(compType);
		AddSchemaFromArchive( compType, ar );
		iNumCompTypes--;
	}
	ar->Close();
	return true;
}

VIEWOBJECTLIST *CControlSchema::AddSchemaFromArchive(const StdString &compType, IXMLArchive *ar)
{
	if (!ar->IsReading())
		return NULL;

	StdString otType;
	StdString *name;
	int iNumElements;
	VIEWOBJECTLIST *viewObjectList;
	ViewFormatObject *viewObject;
	MAPTYPETOVIEWFUNC *viewCreateFuncs;
	CHashString streamType(_T("File"));

	if (m_CompTypeMap[compType] != NULL)
	{
		// get the view object list
		VIEWOBJECTLIST *volList = m_CompTypeMap[compType];
		VIEWOBJECTLIST::iterator volIter = volList->begin();
		// traverse through list deleting view objects
		while( volIter != volList->end() )
		{
			ViewFormatObject *vfObject = (*volIter);
			volIter++;
			delete vfObject;
		}

		delete m_CompTypeMap[compType];
		m_CompTypeMap[compType] = NULL;
	}

	// Start reading from the archive
	name = &(m_NameMap[compType]);
	ar->Read(*name);			// read user friendly name
	ar->Read(iNumElements);

	viewCreateFuncs = SINGLETONINSTANCE(CSchemaRegistered)->GetRegisterTypesMap();
	viewObjectList = new VIEWOBJECTLIST;
	while( iNumElements != 0 )
	{
		// Get the option tree item type
		ar->GetNode(otType);

		CHashString ItemType(otType);

		// find the function ptr in the map
		MAPTYPETOVIEWFUNC::iterator vfmIter = viewCreateFuncs->find(ItemType.GetUniqueID());
		if (vfmIter == viewCreateFuncs->end())
		{
			// warning, view type not found, keep going
			StdString error = _T("Item of this type is unknown: ");
			error += otType;
			::MessageBox(NULL, error, _T("INVALID OPERATION"), MB_OK);
			return NULL;
		}

		VOCREATEFUNC funcPtr = vfmIter->second;

		// call the function that creates the ViewFormatObject
		viewObject = (*funcPtr)(*ar);
		// add the object to the list
		viewObjectList->push_back(viewObject);

		iNumElements--;
	}

	m_CompTypeMap[compType] = viewObjectList;
	return viewObjectList;
}

// return a viewObject list associated with a control type
VIEWOBJECTLIST *CControlSchema::GetViewObjectList(const StdString &compType)
{
	//find objectType
	COMPTYPETOVOLISTMAP::iterator cttvolmIter = m_CompTypeMap.find(compType);

	if ( cttvolmIter == m_CompTypeMap.end() )
	{
		return NULL;
	}

	return cttvolmIter->second;
}

// return the name of the component type
StdString &CControlSchema::GetComponentName(const StdString &compType)
{
	//find objectType
	MAPCOMPTYPETONAME::iterator mcttnIter = m_NameMap.find(compType);

	if ( mcttnIter == m_NameMap.end() )
	{
		return (StdString &)compType;
	}

	return mcttnIter->second;

}
