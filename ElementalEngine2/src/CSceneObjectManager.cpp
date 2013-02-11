///============================================================================
/// \file		CSceneObjectManager.h
/// \brief		Header file for manager for scene objects
/// \date		09/19/2007
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
#include "CSceneObjectManager.h"

REGISTER_COMPONENT_SINGLETON(CSceneObjectManager);
REGISTER_MESSAGE_HANDLER(RegisterSceneAttachObjects, OnRegisterSceneAttachObjects, CSceneObjectManager);
REGISTER_MESSAGE_HANDLER(DetachSceneObject, OnDetachSceneObject, CSceneObjectManager);

CSceneObjectManager::CSceneObjectManager()
:CManager(_T("CSceneObjectManager"), ATTACHOBJECTPRIORITY)
{
	m_hsSceneObjectType = _T("CSceneObject");
}

CSceneObjectManager::~CSceneObjectManager()
{

}

IComponent* CSceneObjectManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CSceneObjectManager);
}

void CSceneObjectManager::Update(DWORD tickCount)
{
	ATTACHMENTOBJECTMAP::iterator mapIter = m_mAttachmentObjectMap.begin();
	for (; mapIter != m_mAttachmentObjectMap.end(); mapIter++)
	{
		ISceneObject* obj = mapIter->second;
		if (obj)
		{
			obj->UpdateTransform();
		}
	}
}

bool CSceneObjectManager::DeleteObject( IObject* Object )
{
	if (Object)
	{
		IHashString *objName = Object->GetName();
		if (objName)
		{
			ATTACHMENTOBJECTMAP::iterator attachMapIter;
			BASEATTACHMENTMAP::iterator baseMapIter;
			
			// first check if it's an attach object
			attachMapIter = m_mAttachmentObjectMap.find(objName->GetUniqueID());
			if (attachMapIter != m_mAttachmentObjectMap.end())
			{
				ISceneObject *sceneObj = attachMapIter->second;
				if (sceneObj)
				{
					ISceneObject *baseObj = sceneObj->GetBaseAttachObject();
					if (baseObj)
					{
						baseObj->DetachObject(sceneObj);
						IHashString *baseName = baseObj->GetName();
						if (baseName)
						{
							baseMapIter = m_mBaseAttachmentMap.find(baseName->GetUniqueID());
							if (baseMapIter != m_mBaseAttachmentMap.end())
							{
								ATTACHMENTLIST *list = &baseMapIter->second;
								RemoveAttachmentFromBaseMap(sceneObj, list);
								if (list->size() == 0)
								{
									m_mBaseAttachmentMap.erase(baseMapIter);
								}
							}
						}
					}

					m_mAttachmentObjectMap.erase(attachMapIter);
				}
			}
			// now check if it's a base object
			baseMapIter = m_mBaseAttachmentMap.find(objName->GetUniqueID());
			if (baseMapIter != m_mBaseAttachmentMap.end())
			{
				ATTACHMENTLIST *list = &baseMapIter->second;
				if (list)
				{
					ATTACHMENTLIST::iterator listIter = list->begin();
					for (; listIter != list->end(); listIter++)
					{
                        ISceneObject *attachObj = (*listIter);
						if (attachObj)
						{
							attachObj->SetAttachInfo(NULL, NULL);

							IHashString *attachName = attachObj->GetName();
							if (attachName)
							{
								attachMapIter = m_mAttachmentObjectMap.find(attachName->GetUniqueID());
								if (attachMapIter != m_mAttachmentObjectMap.end())
								{
									m_mAttachmentObjectMap.erase(attachMapIter);
								}
							}
						}
					}
				}

				m_mBaseAttachmentMap.erase(baseMapIter);			
			}
		}
	}

	// because the scene object isn't really the component type, I can't use the cmanager delete
	IDTOOBJECTMAP *objMap = GetObjectMap(&m_hsSceneObjectType);
	if (objMap)
	{
		IHashString *objName = Object->GetName();
		if (objName)
		{
			IDTOOBJECTMAP::iterator mapIter = objMap->find(objName->GetUniqueID());
			if (mapIter != objMap->end())
			{
				objMap->erase(mapIter);
				
				// update profiler object count
				#if defined(_DEBUG) || defined(_PROFILE)
				UpdateProfiler();
				#endif

				return true;
			}
		}
	}

	return false;
}

DWORD CSceneObjectManager::OnRegisterSceneAttachObjects(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ATTACHSCENEOBJECTPARAMS));
	ATTACHSCENEOBJECTPARAMS asop = *(ATTACHSCENEOBJECTPARAMS*)param;

	if (asop.hsAttachmentObject && asop.hsBaseObject)
	{
		// get both scene object pointers
		IDTOOBJECTMAP *objMap = GetObjectMap(&m_hsSceneObjectType);
		if (objMap)
		{
			IObject *baseObj = NULL, *attachObj = NULL;
			
			IDTOOBJECTMAP::iterator mapIter = objMap->find(asop.hsAttachmentObject->GetUniqueID());
			if (mapIter != objMap->end())
			{
				attachObj = mapIter->second;	
			}

			mapIter = objMap->find(asop.hsBaseObject->GetUniqueID());
			if (mapIter != objMap->end())
			{
				baseObj = mapIter->second;	
			}

			if (baseObj && attachObj)
			{
				// now convert to ISceneObjects
				ISceneObject *baseSceneObj = NULL, *attachSceneObj = NULL;

				baseSceneObj = dynamic_cast<ISceneObject*>(baseObj);
				attachSceneObj = dynamic_cast<ISceneObject*>(attachObj);

				if (baseSceneObj && attachSceneObj)
				{
					// now check for prior links and remove
					ISceneObject *oldAttachment = attachSceneObj->GetBaseAttachObject();
					if (oldAttachment)
					{
						IHashString *oldName = oldAttachment->GetName();
						if (oldName)
						{							
							BASEATTACHMENTMAP::iterator baseIter = m_mBaseAttachmentMap.find(oldName->GetUniqueID());
							if (baseIter != m_mBaseAttachmentMap.end())
							{
								ATTACHMENTLIST *list = &baseIter->second;
								RemoveAttachmentFromBaseMap(attachSceneObj, list);
								if (list->size() == 0)
								{
									m_mBaseAttachmentMap.erase(baseIter);
								}
							}

							oldAttachment->DetachObject(attachSceneObj);
						}						
					}

					// add new link
					m_mBaseAttachmentMap[asop.hsBaseObject->GetUniqueID()].push_back(attachSceneObj);
					m_mAttachmentObjectMap[asop.hsAttachmentObject->GetUniqueID()] = attachSceneObj;

					attachSceneObj->SetAttachInfo(baseSceneObj, asop.hsNodeAttachName);
					baseSceneObj->AttachObject(attachSceneObj);
				}
				else
				{
					m_ToolBox->Log(LOGWARNING, _T("%s register scene attach objects: failed to dynamic cast to ISceneObject*!\n"), __FILE__);
				}
			}
			else
			{
				m_ToolBox->Log(LOGWARNING, _T("%s register scene attach objects: could not find attach objects in object map!\n"), __FILE__);
			}
		}
		else
		{
			m_ToolBox->Log(LOGWARNING, _T("%s register scene attach objects: could not get object map!\n"), __FILE__);
		}
	}

	return MSG_HANDLED_STOP;
}

void CSceneObjectManager::RemoveAttachmentFromBaseMap(ISceneObject *attachment, ATTACHMENTLIST *attachList)
{
	if (attachList)
	{
		ATTACHMENTLIST::iterator listIter = attachList->begin();

		// currently not a map, just a forward iterator...
		for (; listIter != attachList->end(); listIter++)
		{
			if ((*listIter) == attachment)
			{				
				attachList->erase(listIter);			
				break;
			}
		}
	}
}

DWORD CSceneObjectManager::OnDetachSceneObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));
	IHashString *name = (IHashString*)param;

	if (name)
	{
		ATTACHMENTOBJECTMAP::iterator attachMapIter = m_mAttachmentObjectMap.find(name->GetUniqueID());
		if (attachMapIter != m_mAttachmentObjectMap.end())
		{
			ISceneObject *attachObj = attachMapIter->second;
			if (attachObj)
			{
				ISceneObject *baseObj = attachObj->GetBaseAttachObject();
				
				attachObj->SetAttachInfo(NULL, NULL);
				m_mAttachmentObjectMap.erase(attachMapIter);

				if (baseObj)
				{
					IHashString *baseName = baseObj->GetName();
					if (baseName)
					{
						BASEATTACHMENTMAP::iterator baseMapIter = m_mBaseAttachmentMap.find(baseName->GetUniqueID());
						if (baseMapIter != m_mBaseAttachmentMap.end())
						{
							ATTACHMENTLIST *list = &baseMapIter->second;
							RemoveAttachmentFromBaseMap(attachObj, list);
							if (list->size() == 0)
							{
								m_mBaseAttachmentMap.erase(baseMapIter);
							}
						}
					}
					
					baseObj->DetachObject(attachObj);
				}
			}
		}
	}

	return MSG_HANDLED_STOP;
}