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

#ifndef _MESSAGES_H
#define _MESSAGES_H


#define GET_STATUS_BAR_MSG "GetStatusBar"
#define RESOURCE_DRAG_MSG_STR "ResourceDragMsg"
#define RESOURCE_DROP_MSG_STR "ResourceDropMsg"
#define RESOURCE_ADD_MSG_STR  "ResourceAddMsg"
#define RESOURCE_FOLDER_REFRESH_MSG_STR "FolderRefreshMsg"
#define RESOURCE_FILE_CHANGED_MSG_STR "FileChangedMsg"
#define RESOURCE_CHECK_DIRTY_MSG_STR "ResChkDirtyMsg"
#define RESOURCE_THUMB_GEN_STATUS "ResThumbGenStat"

// Object Page Messages
#define THUMBNAIL_WIDTH		64
#define THUMBNAIL_HEIGHT	64

// Options Page Message
#define ADD_OPTION_PAGE_MSG		"AddOptionsPage"
#define INVALIDINDEX	-1
class IBaseObject;

typedef struct RESOURCEPAGEINFO
{
	HBITMAP thumbNail;		// thumbnail, can be NULL
	TCHAR name[MAX_PATH];			// Display name of object
	UINT pageIndex;			// index of page to add
	UINT rsrcIndex;			// resource index inside of page
	TCHAR filepath[MAX_PATH];	// filepath for the resource
	FILETIME  lastWrite;
	RESOURCEPAGEINFO()
	{
		Init();
	}
	void Init()
	{
		thumbNail = NULL;
	}
} RESOURCEPAGEINFO;

#define ADD_RESOURCE_MSG					"AddResourceToPage"
#define REMOVE_RESOURCE_MSG				"RemoveResourceFromPage"
#define CLEAR_ALL_RESOURCE_MSG			"ClearResourcePage"

typedef struct RESOURCECOLLECTION
{
	HICON hIcon;			// the icon for the page
	CString name;			// Display name of Collection
	UINT *pageIndex;		// list index filled in by resource manager
	DWORD priority;			// priority of resource in property sheet
	CString extensions;		// File Extensions handled by the page
	int iView;				// Format for View of CListControl

	RESOURCECOLLECTION()
	{
		iView = LVS_ICON;
	}
} RESOURCECOLLECTION;

typedef struct GETRESOURCEINFOBYRELATIVEPATH
{
	StdString findRelativePath;
	StdString pageName;			//Which resource page
	RESOURCEPAGEINFO rInfo;	//Pointer to be filled with data
} GETRESOURCEINFOBYRELATIVEPATH;

// Drap & Drop Messages
typedef enum DROPTYPES
{
	DROP_TEXTURE
};

struct DRAGOBJECTINFO
{
	CPoint xy;
};

typedef struct DROPOBJECTINFO
{
	RESOURCEPAGEINFO *pResourcePageInfo;
	CPoint xy;
} DROPOBJECTINFO;


typedef struct HIEROBJMESSINFO
{
	IBaseObject *object;
	BOOL enable;
} HIEROBJMESSINFO;

// OnCmdMsg message data
typedef struct
{
	UINT m_nID;
	int m_nCode;
	void *m_pExtra;
	AFX_CMDHANDLERINFO *m_pHandlerInfo;
	bool retVal;
} CMDMESSAGE;

// Current windows Menu & Accelerator handles
typedef struct
{
	HMENU m_hMenu;
	HACCEL m_hAccel;
	UINT m_MenuRsrc;
	CBitmap *m_MenuBitmap;
} MENUINFOMESSAGE;

/// File creation information for creating new files through the new command
typedef struct FILETYPECREATIONINFO
{
	StdString m_Description;
	//callbacks
	void		(*m_CreationCallback)(void *userData);
	// user data to pass into the callback
	void *m_UserData;				
	FILETYPECREATIONINFO()
	{
		m_CreationCallback = NULL;
		m_Description = "";
		m_UserData = NULL;
	}
} FILETYPECREATIONINFO;

typedef struct SELECTEDOBJECT
{
	IHashString *name;  // name of the object
	IHashString *type;  // type of the object
} SELECTEDOBJECT;

/// \brief	A list of object types
typedef list<IHashString *> EDITABLELIST;

/// \brief List of object names that do not require dependence on exitance of objects
/// as IHashString users tend to do
typedef list<CHashString> OBJECTNAMELIST;

/// \brief List of SELECTEDOBJECT structs 
typedef list<SELECTEDOBJECT> SELECTEDOBJECTLIST;

/// \brief	struct linking components with an editor
typedef struct
{
	/// \brief	The name of the editor handling the object
	IHashString *editorName;
	/// \brief	A list of names of the objects being handled by the editor
	EDITABLELIST *objectNames;
} REGISTER_EDITOR_MESSAGE;

/// List of pickable object types.
typedef list<IHashString *> PICKABLELIST;

/// Link pickable object with an tool.
typedef struct
{
	/// The name of the tool handling the pickable object.
	IHashString *toolName;
	/// A list of types of the objects being handled by the tool.
	PICKABLELIST *objectNames;
} REGISTER_TOOL_MESSAGE;

struct WORLDCHANGEFILENAMEPARAMS
{
	TCHAR *oldName;		/// the old name of the file, could be "" or NULL
	TCHAR *newName;		/// the new name of the file to save as....
};

struct TOOLBARSTRUCT
{
	HICON hIcon;		/// the icon to load for the toolbar
	TCHAR	*Text;		/// the text to set for the toolbar
	CControlBar *cBar;	/// the control bar to add to the tab
};

struct TOOLBARACTIVESTRUCT
{
	CControlBar *cBar;
	BOOL bActive;
};

struct LINKABLEOBJECTS
{
	IHashString *key;
	list<DWORD> *linkableObjects;
};

struct MOUSECLICKPARAMS
{
	MOUSECLICKPARAMS()
	{
		object = NULL;
		compType = NULL;
		point = NULL;
		hitPosition = NULL;
		pickableObject = NULL;
		pickableCompType = NULL;
	}

	IHashString *object; // name of the clicked object, or NULL to do a raycast using 'point'
	IHashString *compType; // component type of 'object' if present
	Vec3 *point; // point for raycast if no object is given
	Vec3 *hitPosition; // Collision point of raycast

	/// Name of the actual pickable object, catched by mouse, can be NULL if raycast wasn't performed.
	/// Appropriate clicked object from the world hierarchy is usually his parent.
	IHashString *pickableObject;
	/// Component type of the actual pickable object, catched by mouse,
	/// can be NULL if raycast wasn't performed.
	IHashString *pickableCompType;
};

struct CLICKOBJECTPARAMS
{
	CLICKOBJECTPARAMS()
	{
		object = NULL;
		compType = NULL;
		bIntersectionValid = false;
	}

	/// The name of the object clicked on
	IHashString *object;
	/// the component type clicked on
	IHashString *compType;
	/// Flag true if trace line intersection data from mouse click is valid
	bool bIntersectionValid;
	/// Trace line intersection data
	Vec3 *pIntersectionPos;
};

struct SELECTOBJECTPARAMS
{
	SELECTOBJECTPARAMS()
	{
		object = NULL;
		compType = NULL;
	}

	/// The name of the selected object
	IHashString *object;
	/// The component type of the selected object
	IHashString *compType;
};

struct SPAWNATPOSITONPARAMS
{
	SPAWNATPOSITONPARAMS()
	{
		name = NULL;
		orientation = NULL;
		x, y, z = 0.f;
		scale = NULL;
	}

	/// name of the object to spawn
	IHashString *name;
	/// position
	float x;
	float y;
	float z;
	/// orientation
	Vec3 *orientation;
	/// scale
	Vec3 *scale;
};

/// parameters for creating thumbnail. (Path to file and CBitmap for return).
struct CREATETHUMBNAILPARAMS
{
	/// Constructor
	CREATETHUMBNAILPARAMS(LPCTSTR lpathName)
	:	lpszPathName(lpathName), pThumbnail(NULL)
	{}

	/// CBitmap pointer for store thumbnail. Where it be created it should be NULL.
	/// Delete it after using.
	CBitmap * pThumbnail;
	/// File path.
	LPCTSTR lpszPathName;
};

/// structure for creating and removing undo stack
typedef struct UNDOSTACKINFO
{
	UNDOSTACKINFO() 
		: pStackName(NULL)
	{
	}

	///	name of undo stack
	IHashString *pStackName;
} UNDOSTACKINFO;

/// structure for creating and removing undo stack
typedef struct UNDOSTACKRENAME
{
	UNDOSTACKRENAME() 
		: pStackName(NULL)
		, pNewStackName(NULL)
	{
	}

	///	name of undo stack
	IHashString *pStackName;
	///	new name of undo stack
	IHashString *pNewStackName;
} UNDOSTACKRENAME;

/// structure for creating reference from CView to undo stack
typedef struct UNDOVIEWTOSTACK
{
	UNDOVIEWTOSTACK() 
		: pStackName(NULL)
		, pStackView(NULL)
	{
	}

	///	name of undo stack
	IHashString *pStackName;
	///	name of undo stack
	CView		*pStackView;
} UNDOVIEWTOSTACK;

typedef struct EDITINGPROPERTIESPARAMS
{
	EDITINGPROPERTIESPARAMS()
	{
		name = NULL;
	}
	IHashString *name;
	map<StdString, StdString> params;
} EDITINGPROPERTIESPARAMS;

#endif //_MESSAGES_H