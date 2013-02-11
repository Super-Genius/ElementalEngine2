//-----------------------------------------------------------------------------
//
// File:          CBaseObject.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CBaseObject'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method '~CBaseObject'
*     Added method 'VECHO::GetSiblingIterator'
*     Added method 'VECHO::GetPrevious'
*     Added method 'VECHO::GetNext'
*     Added method 'VECHO::GetChildren'
*     Added method 'SetType'
*     Added method 'SetName'
*     Added method 'SetDirty'
*     Added method 'RemoveChild'
*     Added method 'RemoveAllChildren'
*     Added method 'OnUpdate'
*     Added method 'OnDisplay'
*     Added method 'Initialize'
*     Added method 'GetType'
*     Added method 'GetProperties'
*     Added method 'GetParent'
*     Added method 'GetName'
*     Added method 'DeleteSiblingIterator'
*     Added method 'ClearDirty'
*     Added method 'CBaseObject'
*     Added method 'AddChild'
*     Added method '~CBaseObject'
*     Added member 'm_Properties'
*     Added member 'm_SiblingIterator'
*     Added member 'm_Parent'
*     Added member 'm_Children'
*     Added member 'm_Dirty'
*     Added member 'm_Name'
*     Added member 'm_Type'
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




/*@NOTE_3942
Destructor method.
*/
CBaseObject::~CBaseObject()
{//@CODE_3942
    DestructorInclude();

    // Put in your own code
}//@CODE_3942


__inline BOOL CBaseObject::AddChild(CBaseObject* child, DWORD before)
{//@CODE_3981

		VECHO::iterator it;

		if (before == INVALID_INDEX)
		{
			m_Children.push_back(child); 
			child->Initialize(this, m_Children.end()-1);
		}
		else
		{
			if (before > m_Children.size())
			{
				return FALSE;
			}
			it = m_Children.begin() + before;
			child->Initialize(this, m_Children.insert(it, child));
		}

		return TRUE;
}//@CODE_3981


__inline CBaseObject::CBaseObject(const GUID* type, LPCSTR name, BOOL dirty)
{//@CODE_3954

		m_Type = type;
		m_Name = name;
		m_Dirty = dirty;
		m_SiblingIterator = NULL;
}//@CODE_3954


__inline void CBaseObject::ClearDirty()
{//@CODE_3973
 m_Dirty = FALSE;
}//@CODE_3973


__inline void CBaseObject::DeleteSiblingIterator()
{//@CODE_3984

		m_SiblingIterator = NULL;
}//@CODE_3984


__inline LPCSTR CBaseObject::GetName()
{//@CODE_3969
 return m_Name;
}//@CODE_3969


__inline CBaseObject* CBaseObject::GetParent()
{//@CODE_3975
 
			if (m_Parent != NULL)
			{
				return m_Parent;
			}
			else
			{
				return NULL;
			}
	
}//@CODE_3975


__inline CConfig& CBaseObject::GetProperties()
{//@CODE_3980
 return m_Properties;
}//@CODE_3980


__inline  GUID* CBaseObject::GetType() const
{//@CODE_3965
 return m_Type;
}//@CODE_3965


__inline CBaseObject::Initialize(CBaseObject* parent,
                                 VECHO::iterator siblingIterator)
{//@CODE_3960

		m_Parent = parent;
		m_SiblingIterator = siblingIterator;
}//@CODE_3960


void CBaseObject::OnDisplay()
{//@CODE_3989
 return;
}//@CODE_3989


void CBaseObject::OnUpdate()
{//@CODE_3988
 return;
}//@CODE_3988


__inline void CBaseObject::RemoveAllChildren()
{//@CODE_3987

		VECHO::iterator it;
		VECHO::iterator itSib;
		for (it = m_Children.begin(); it != m_Children.end(); it++)
		{
			itSib = (*it)->GetSiblingIterator();
			if (itSib != NULL)
			{
				m_Children.erase(itSib);
				// the iterator for this child is invalid now, so
				// delete it
				(*it)->DeleteSiblingIterator();
			}
		}
}//@CODE_3987


__inline void CBaseObject::RemoveChild(CBaseObject* child)
{//@CODE_3985

		VECHO::iterator itSib;

		itSib = child->GetSiblingIterator();
		if (itSib != NULL)
		{
			// erase from vector map
			m_Children.erase(itSib);
			// the iterator for this child is invalid now, so
			// delete it
			child->DeleteSiblingIterator();
		}
}//@CODE_3985


__inline void CBaseObject::SetDirty()
{//@CODE_3972
 m_Dirty = TRUE;
}//@CODE_3972


__inline void CBaseObject::SetName(LPCSTR name)
{//@CODE_3970
 m_Name = name;
}//@CODE_3970


__inline void CBaseObject::SetType(const GUID* type)
{//@CODE_3966
 m_Type = type;
}//@CODE_3966


__inline VECHO::iterator CBaseObject::VECHO::GetChildren()
{//@CODE_3974
 
		return m_Children.begin(); 
}//@CODE_3974


__inline VECHO::iterator CBaseObject::VECHO::GetNext()
{//@CODE_3977
 
		if (m_SiblingIterator != NULL)
		{
			return m_SiblingIterator+1; 
		}
		
		return NULL;
}//@CODE_3977


__inline VECHO::iterator CBaseObject::VECHO::GetPrevious()
{//@CODE_3978

		if (m_SiblingIterator != NULL)
		{
			return m_SiblingIterator-1; 
		}
		
		return NULL;
}//@CODE_3978


__inline VECHO::iterator CBaseObject::VECHO::GetSiblingIterator()
{//@CODE_3976
 return m_SiblingIterator;
}//@CODE_3976


__inline virtual CBaseObject::~CBaseObject()
{//@CODE_3959

		// loop through and delete all the children
		VECHO::iterator it;
		for (it = m_Children.begin(); it != m_Children.end(); it++)
		{
			delete *it;
		}
}//@CODE_3959


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3943
Method which must be called first in a constructor.
*/
void CBaseObject::ConstructorInclude()
{
}


/*@NOTE_3944
Method which must be called first in a destructor.
*/
void CBaseObject::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
