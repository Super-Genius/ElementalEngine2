//=====================================================================
// File:		ObjectDLL.h
// Description:	Interface header for resource dll's that are run in
//				the context of the main application
// Started:		Kenneth Hurley
//				10/31/2001
//=====================================================================

#ifndef OBJECTDLL_H
#define OBJECTDLL_H

#include "DLLInterface.h"

enum OBJECTFLAGS 
{
	OBJFLAGS_NONE = 0,
	OBJFLAGS_CREATABLE =	0x01,		// Can create a new document?
	OBJFLAGS_EDITABLE =		0x02,		// Can edit a document?
	OBJFLAGS_EXPORTABLE =	0x04,		// Can export a document
	OBJFLAGS_IMPORTABLE =	0x08,		// Can import a document
	OBJFLAGS_THUMBNAIL =	0x10,		// Can generate a thumbnail

	OBJFLAGS_DUMMY = 0x7FFFFFFF			// So enum is 32 bits
};

typedef struct OBJECTINFO
{
	CString name;			// the name for this resource type
	HICON icon;				// pointer to icon image
	DWORD priority;			// priority of resource in property sheet
	DWORD objFlags;			// flags for this object
	UINT *pageIndex;		// return value for resource page index
} OBJECTINFO;

class AFX_EXT_CLASS CObjectDLLInterface : public CDLLInterface
{
	// attributes/properties
public:
	OBJECTINFO m_ObjInfo;
	CBitmap *m_ThumbNail;

	// functions
public:
	virtual DWORD Load(LPTSTR filename)
	{
		return 0;
	}

	virtual DWORD Save(LPTSTR filename)
	{
		return 0;
	}

};


#undef AFX_DATA
#define AFX_DATA

#endif	// #ifdef OBJECTDLL_H