#ifndef _MESSAGES_H
#define _MESSAGES_H

// Options Page Message
#define ADD_OPTION_PAGE_MSG		"AddOptionsPage"

// Object Page Messages
#define THUMBNAIL_WIDTH		64
#define THUMBNAIL_HEIGHT	64

class CBaseObject;

typedef struct OBJECTSTRUCT 
{
	CBitmap *thumbNail;		// thumbnail, can be NULL
	CString *name;			// Display name of resource
	CString *fileName;		// file name, that may need reloading
	UINT *pageIndex;		// list index filled in by resource manager
	UINT *rsrcIndex;		// resource index inside of page
	CBaseObject *object;	// generic object for object manager
} OBJECTSTRUCT;

#define ADD_OBJECT_MSG			"AddObjectToPage"
#define REMOVE_OBJECT_MSG		"RemoveObjectFromPage"
#define CLEAR_ALL_OBJECT_MSG	"ClearObjectPage"

// Drap & Drop Messages
typedef enum DROPTYPES
{
	DROP_TEXTURE,
};

typedef struct DROPOBJECTINFO
{
	CBaseObject *object;
	CPoint	xy;

} DROPOBJECTINFO;

#define OBJECT_DROP_MSG_STR				"DropObject"

#define GET_STATUS_BAR_MSG				"GetStatusBar"

typedef struct HIEROBJMESSINFO
{
	CBaseObject *object;
	BOOL enable;
} HIEROBJMESSINFO;

#define ADD_HIER_OBJECT_MSG				"AddHierarchyObject"
#define REMOVE_HIER_OBJECT_MSG			"RemoveHierarchyObject"
#define CLEAR_HIER_ALL_OBJECT_MSG		"ClearHierarchyObjects"

#endif //_MESSAGES_H