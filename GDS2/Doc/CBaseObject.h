//-----------------------------------------------------------------------------
//
// File:          CBaseObject.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CBaseObject'
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

#ifndef _CBASEOBJECT_H
#define _CBASEOBJECT_H

//@START_USER1
//@END_USER1



class CBaseObject
{

//@START_USER2
//@END_USER2

//
// Group: ClassBuilder methods
//

private:
    void ConstructorInclude();
    void DestructorInclude();

//
// Non-Grouped Members
//

private:
    GUID* m_Type;
    LPCSTR m_Name;
    BOOL m_Dirty;
    VECHO m_Children;
    CBaseObject* m_Parent;
    VECHO m_SiblingIterator;
    CConfig m_Properties;

//
// Non-Grouped Methods
//

public:
    virtual ~CBaseObject();
    __inline BOOL AddChild(CBaseObject* child, DWORD before);
    __inline CBaseObject(const GUID* type, LPCSTR name, BOOL dirty);
    __inline void ClearDirty();
    __inline void DeleteSiblingIterator();
    __inline LPCSTR GetName();
    __inline CBaseObject* GetParent();
    __inline CConfig& GetProperties();
    __inline  GUID* GetType() const;
    __inline Initialize(CBaseObject* parent, VECHO::iterator siblingIterator);
    void OnDisplay();
    void OnUpdate();
    __inline void RemoveAllChildren();
    __inline void RemoveChild(CBaseObject* child);
    __inline void SetDirty();
    __inline void SetName(LPCSTR name);
    __inline void SetType(const GUID* type);
    __inline VECHO::iterator VECHO::GetChildren();
    __inline VECHO::iterator VECHO::GetNext();
    __inline VECHO::iterator VECHO::GetPrevious();
    __inline VECHO::iterator VECHO::GetSiblingIterator();
    __inline virtual ~CBaseObject();
};

#endif


#ifdef CB_INLINES
#ifndef _CBASEOBJECT_H_INLINES
#define _CBASEOBJECT_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
