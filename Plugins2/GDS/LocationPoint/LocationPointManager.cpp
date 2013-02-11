///============================================================================
/// \file		LocationPointManager.cpp
/// \brief		Implmentation of CLocationPointManager Class
/// \date		12/13/2006
/// \author		Nick Tourte
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
#include "PerformanceProfiler.h"

#define LOCATIONPOINTPRIORITY 0x04A0

REGISTER_COMPONENT_SINGLETON(CLocationPointManager);

// Default Constructor
CLocationPointManager::CLocationPointManager() : CManager(_T("CLocationPointManager"), LOCATIONPOINTPRIORITY)
{
	
}

// Default Destructor
CLocationPointManager::~CLocationPointManager()
{

}

IComponent *CLocationPointManager::Create(int nArgs, va_list argptr)
{
//	// Return the singleton instance of this class
  	return SINGLETONINSTANCE( CLocationPointManager );
}

void CLocationPointManager::Update(DWORD tickCount)
{
/*	static CHashString locType(_T("CLocationPointObject"));
	IDTOOBJECTMAP *locSet = GetObjectMap(&locType);
	if (locSet)
	{
		ADDRENDERLISTPARAMS arlp;
		OBJECTLIST objList;
		IDTOOBJECTMAP::iterator objIter = locSet->begin();
		while (objIter != locSet->end())
		{
			CLocationPointObject *currObj = dynamic_cast<CLocationPointObject*>(objIter->second);
			if (currObj)
			{
				IRenderObject *ro = currObj->GetRenderObject();
				if (ro != NULL)
				{
					objList.push_back((IObject*)ro);
				}
			}
			objIter++;
		}
		arlp.objList = &objList;
		arlp.dwSortMethod = SORT_DISTANCE;		
		arlp.dwListPriorityKey = RENDERPRIORITY_OPAQUE;
		static DWORD msgHash_AddRenderList = CHashString(_T("AddRenderList")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_AddRenderList, sizeof(ADDRENDERLISTPARAMS), &arlp);
	}
*/
}