//---------------------------------------------------------------------
// File:		ResourceManager.h
// Description:	Manages Resources
// Created:		5/27/02
// Author:		Kenneth L. Hurley
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
//---------------------------------------------------------------------

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#if 0
#ifndef RESOURCE_H
#include "IBaseObject.h"
#endif

#ifndef IRESOURCEMANAGER_H
#include "IObjectManager.h"
#endif

using namespace std;

// concrete implementation of interface
class CResourceManager : IResourceManager
{
private:
	IBaseObject *m_HeadObject;
	VECHIEROBJ m_VecHead;
	IProgramInfo *m_ProgramInfo;

	// methods
public:
	CResourceManager(IProgramInfo *ProgramInfo);
	~CResourceManager();
	VECHIEROBJ::iterator GetResources();
	IBaseObject *GetHeadResource();
	BOOL AddResource(IBaseObject *object, 
			IBaseObject *parent = NULL,
				DWORD before = INVALID_INDEX);
	void RemoveResource(IBaseObject *srcObj);
	void RemoveAllResources();
};
#endif //#if 0
#endif	// #ifndef ResourceMANAGER_H