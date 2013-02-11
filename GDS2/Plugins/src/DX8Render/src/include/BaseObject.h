//---------------------------------------------------------------------
// File:		BaseObject.h
// Description:	Generic Basearchy object definition file
// Created:		5/27/02
// Author:		Kenneth L. Hurley
//---------------------------------------------------------------------

#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#pragma warning(disable : 4786)
#include <vector>
#include "Config.h"

using namespace std;

#define INVALID_INDEX (DWORD)(-1)

class CBaseObject;
typedef vector<CBaseObject *> VECHO;

class CBaseObject
{
private:
	const GUID		*m_Type;			// the guid for this object
	LPCSTR			m_Name;	    		// a name for this object
	BOOL			m_Dirty;			// does this object need updating?
	VECHO			m_Children;			// children of this object
	CBaseObject		*m_Parent;			// parent of this object
	VECHO::iterator	m_SiblingIterator;	// for next and previous access
	CConfig			m_Properties;		// properties for this object
	// methods
public:
	__inline CBaseObject(const GUID *type = NULL, LPCSTR name = NULL, BOOL dirty = FALSE)
	{
		m_Type = type;
		m_Name = name;
		m_Dirty = dirty;
		m_SiblingIterator = NULL;
	}

	// make it virtual so derived class destructor is called
	__inline virtual ~CBaseObject()
	{
		// loop through and delete all the children
		VECHO::iterator it;
		for (it = m_Children.begin(); it != m_Children.end(); it++)
		{
			delete *it;
		}
	}

	__inline Initialize(CBaseObject *parent, VECHO::iterator siblingIterator)
	{
		m_Parent = parent;
		m_SiblingIterator = siblingIterator;
	}

	__inline const GUID *GetType() { return m_Type; }
	__inline void SetType(const GUID *type) { m_Type = type; }
	__inline LPCSTR GetName() { return m_Name; }
	__inline void SetName(LPCSTR name) { m_Name = name; }
	__inline void SetDirty() { m_Dirty = TRUE; }
	__inline void ClearDirty() { m_Dirty = FALSE; }
	__inline VECHO::iterator GetChildren() 
	{ 
		return m_Children.begin(); 
	}
	__inline CBaseObject *GetParent() 
		{ 
			if (m_Parent != NULL)
			{
				return m_Parent;
			}
			else
			{
				return NULL;
			}
		};

	__inline VECHO::iterator GetSiblingIterator() { return m_SiblingIterator; }
	__inline VECHO::iterator GetNext() 
	{ 
		if (m_SiblingIterator != NULL)
		{
			return m_SiblingIterator+1; 
		}
		
		return NULL;
	}
	__inline VECHO::iterator GetPrevious() 
	{
		if (m_SiblingIterator != NULL)
		{
			return m_SiblingIterator-1; 
		}
		
		return NULL;
	}
	__inline CConfig &GetProperties() { return m_Properties; }

	// add a child into our child list
	// return true if success
	__inline BOOL AddChild(CBaseObject *child, DWORD before = INVALID_INDEX)
	{
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
	}

	__inline void DeleteSiblingIterator()
	{
		m_SiblingIterator = NULL;
	}

	// this just removes it from the Hierarchy, just delete
	// it if you want it both removed and deleted.
	__inline void RemoveChild(CBaseObject *child)
	{
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
	};

	// this doesn't delete the children
	__inline void RemoveAllChildren()
	{
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
	};

	virtual void OnUpdate() { return; }
	virtual void OnDisplay() { return; }
};

extern PFNCLASSOVERRIDE FindClassOverride();

// we must have a static function FindClassOverride() to make 
// the callback in all .exe and .dll that use this macro
#define DECLARE_OVERRIDEABLE(class_name)				\
public:												\
	static class_name *CreateInstance(class_name *src = NULL)	\
	{												\
		PFNCLASSOVERRIDE pfnOverride;				\
		pfnOverride = FindClassOverride(_T(#class_name)); \
		if (pfnOverride == NULL)					\
		{											\
			return new class_name;					\
		}											\
													\
		return (class_name *)(*pfnOverride)(src);	\
	}

#define DECLARE_OVERRIDE(class_name, base_class)	\
public:												\
	static base_class *CreateInstance(base_class *src = NULL) \
	{												\
		if (src == NULL)							\
		{											\
			return (base_class *)new class_name;	\
		}											\
													\
		return (base_class *)new class_name(src);	\
	}												

#endif	// #ifndef BASEOBJECT_H