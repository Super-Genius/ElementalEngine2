//-----------------------------------------------------------------------------
//
// File:          CObjectManager.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CObjectManager'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method '~CObjectManager'
*     Added method 'VECHO::GetObjects'
*     Added method 'GetHeadObject'
*     Added method 'CObjectManager::CObjectManager'
*     Added method 'CObjectManager::CObjectManager'
*     Added method 'CObjectManager::CObjectManager'
*     Added method 'CObjectManager'
*     Added method '~CObjectManager'
*     Added member 'm_ProgramInfo'
*     Added member 'm_VecHead'
*     Added member 'm_HeadObject'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

//@START_USER1
//@END_USER1


// Master include file
#include "StdAfx.h"


//@START_USER2
//@END_USER2




/*@NOTE_3776
Destructor method.
*/
CObjectManager::~CObjectManager()
{//@CODE_3776
    DestructorInclude();

    // Put in your own code
}//@CODE_3776


__inline CObjectManager::CObjectManager(CProgramInfo* ProgramInfo)
{//@CODE_3785

		m_ProgramInfo = ProgramInfo;
		m_HeadObject = new CBaseObject(&GUID_WORLD, "World");
		if (m_HeadObject != NULL)
		{
			m_VecHead.push_back(m_HeadObject);
			m_HeadObject->Initialize(NULL, m_VecHead.begin());
		}
}//@CODE_3785


__inline BOOL CObjectManager::CObjectManager::CObjectManager(CBaseObject* object,
                                                             CBaseObject* parent,
                                                             DWORD before)
{//@CODE_3793

		// if parent is NULL, use the head object
		if (parent == NULL)
		{
			parent = m_HeadObject;
		}

		// add object with error check
		if (!parent->AddChild(object, before))
		{
			// failed
			return FALSE;
		}

		HIEROBJMESSINFO homi;

		homi.object = object;
		homi.enable = TRUE;
		m_ProgramInfo->BroadcastMessage(AddHierarchyObject, &homi);
		return TRUE;

}//@CODE_3793


__inline void CObjectManager::CObjectManager::CObjectManager(CBaseObject* srcObj)
{//@CODE_3798

		HIEROBJMESSINFO homi;
		CBaseObject *parent;

		homi.object = srcObj;
		homi.enable = FALSE;
		// allow others to still have access to the object, before we remove
		// it from the object manager
		m_ProgramInfo->BroadcastMessage(RemoveHierarchyObject, &homi);

		// get parent of object, so we can remove child
		parent = srcObj->GetParent();
		// and remove child
		parent->RemoveChild(srcObj);
}//@CODE_3798


__inline void CObjectManager::CObjectManager::CObjectManager()
{//@CODE_3800

		// allow others to still have access to the objects, before we remove
		// them from the object manager
		m_ProgramInfo->BroadcastMessage(ClearHierarchyObjects, NULL);

		m_HeadObject->RemoveAllChildren();
}//@CODE_3800


__inline CBaseObject* CObjectManager::GetHeadObject()
{//@CODE_3791

		return m_HeadObject;
}//@CODE_3791


__inline VECHO::iterator CObjectManager::VECHO::GetObjects()
{//@CODE_3789

		return m_HeadObject->GetChildren();
}//@CODE_3789


__inline CObjectManager::~CObjectManager()
{//@CODE_3787

		delete m_HeadObject;
}//@CODE_3787


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3777
Method which must be called first in a constructor.
*/
void CObjectManager::ConstructorInclude()
{
}


/*@NOTE_3778
Method which must be called first in a destructor.
*/
void CObjectManager::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
