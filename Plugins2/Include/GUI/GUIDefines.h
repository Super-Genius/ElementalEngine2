///============================================================================
/// \file		GUIDefines.h
/// \brief		Header for GUI specific messages 
/// \date		10/09/06
/// \author		Nick Tourte
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

// ********************** Messages internal to plugins ********************** //

/// struct for rendering 
typedef struct GUIRENDERVERTS
{
	float pos[3];
	float u, v;
	DWORD color;
}GUIRENDERVERTS;

/// structure for adding gui elements to a group
typedef struct 
{
	IHashString		*m_GuiName;		/// name of element to add
	IHashString		*m_CompType;	/// the component type of the element to add
	DWORD			m_ZOrder;		/// the Z Order to draw at, 0 is higher
} ADDGUIELEMENT;

/// struct for retrieving all the objects associated with a particular page or group
typedef struct GUIELEMLIST
{
	vector <StdString> m_vNames; /// the names of the elements
	vector <StdString> m_vTypes; /// the types of the elements
} GUIELEMLIST;

/// struct for loading and keeping track of pages in the gui system
typedef struct GUIPAGELIST
{
	StdString m_szCursorName; /// name of the cursor, if there is one
	vector<StdString> m_vPages; /// list of pages
	UINT m_iMapIndex; /// map index that the system loaded into
	vector<StdString> m_vAnimations; /// list of animations
}GUIPAGELIST;

/// struct for getting a page list from a loaded filename
typedef struct GETPAGELIST
{
	IHashString *fileName; /// name of file to check info for
	GUIPAGELIST gpl; /// page list of data to return
}GETPAGELIST;

/// structs for editor to get data in and out of animation
typedef struct GUIEDKEY
{
	UINT key;
	float value;
}GUIEDKEY;

typedef struct GUIEDPOSCHAN
{
	vector<GUIEDKEY> xPos;
	vector<GUIEDKEY> yPos;
}GUIEDPOSCHAN;

typedef struct GUIEDSIZECHAN
{
	vector<GUIEDKEY> width;
	vector<GUIEDKEY> height;
}GUIEDSIZECHAN;

typedef struct GUIEDCOLORCHAN
{
	vector<GUIEDKEY> alpha;
	vector<GUIEDKEY> r;
	vector<GUIEDKEY> g;
	vector<GUIEDKEY> b;
}GUIEDCOLORCHAN;

typedef struct GUIEDROTCHAN
{
	vector<GUIEDKEY> rot;
}GUIEDROTCHAN;

typedef struct GUIEDANIMOBJ
{
	StdString objName;
	GUIEDPOSCHAN posChannel;
	GUIEDSIZECHAN sizeChannel;
	GUIEDCOLORCHAN colorChannel;
	GUIEDROTCHAN rotationChannel;
}GUIEDANIMOBJ;

typedef struct GUIANIMDATA
{
	StdString animName;
	vector<GUIEDANIMOBJ> vObjects;
	UINT iFrameRate;
}GUIANIMDATA;

/// struct to add a key to an object in the animation system
typedef struct ADDMULTICHANNELKEYTOGUIANIM
{
	StdString objName;
	StdString objType;	
	UINT key;
	float xPos;
	float yPos;
	float width;
	float height;
	float a;
	float r;
	float g;
	float b;
	float angle;
}ADDMULTICHANNELKEYTOGUIANIM;

/// struct to add a key to one channel 
typedef struct ADDSINGLECHANNELKEYTOGUIANIM
{
	StdString objName;
	UINT key;
	UINT channel; /// channel -> POS = 0, SIZE = 1, COLOR = 2, ROTATION = 3	
	float val1;
	float val2;
	float val3;
	float val4;	
}ADDSINGLECHANNELKEYTOGUIANIM;

/// struct to delete a key from the animation
typedef struct GUIDELETEONEKEY
{
	StdString objName;
	UINT keyIndex;
	UINT channel; /// channel -> POS = 0, SIZE = 1, COLOR = 2, ROTATION = 3	
}GUIDELETEONEKEY;

/// struct for setting the working resolution; used mostly in file saving and for the editor
typedef struct GUIWORKINGRES
{
	unsigned int iSizeX; /// x resolution
	unsigned int iSizeY; /// y resolution
} GUIWORKINGRES;

/// struct for finding GUI objects
typedef struct FINDGUIOBJECT
{
	IHashString *name; /// name of object to find
	IHashString *type; /// type of object returned
	bool bFound; /// true if object with name is found
	IObject *object; /// object found, or NULL
	FINDGUIOBJECT()
	{
		object = NULL;
		name = NULL;
		type = NULL;
		bFound = false;
	}
}FINDGUIOBJECT;

struct ADDGUILANGUAGEPARAMS
{
	DWORD dwHashLangName;
	DWORD dwHashLangFile;
	ADDGUILANGUAGEPARAMS()
	{
		dwHashLangName = 0;
		dwHashLangFile = 0;
	}
};

typedef map<DWORD, StdString> LANGUAGEFILEMAP;

// ********************** Messages that can be used in Lua scripts ********************** //

// toLua_begin

/// struct to send event to state machine
typedef struct GUIEVENT
{
	DWORD event; /// unique id of text of event to fire
	IArchive *ar; /// archive of extra data, if anything
}GUIEVENT;

/// simple struct for binding a float to lua... stupid lua floats
typedef struct GUIFLOAT
{
	float data;
}GUIFLOAT;

/// structure for GUI element add/remove from page message
typedef struct GUIPAGEMESSAGE
{
	IHashString *name;		/// the name of the element to add to page
	IHashString *compType;	/// the component type of the element to add
	bool		wasFoundInPage; /// true if the component was found in the page (for deletion)
} GUIPAGEMESSAGE;

/// struct for changing button image information
typedef struct BUTTONIMAGE
{
	StdString szNorm; /// idle texture filename
	StdString szHigh; /// mouse over texture filename
	StdString szSelect; /// depressed texture filename
	StdString szGray; /// grayed out texture filename
} BUTTONIMAGE;

/// struct for changing the cursor information
typedef struct CHANGECURSOR
{
	int iSizeX; /// width of cursor
	int iSizeY; /// height of cursor
	int iHotX; /// ?
	int iHotY; /// ?
	StdString pTexture; /// texture filename
} CHANGECURSOR;

/// struct to add lines to listboxes
typedef struct ADDLISTBOXENTRY
{
	StdString entryText; /// text to add
	int iRedColor; /// r value 
	int iGreenColor; /// g value
	int iBlueColor; /// b value
} ADDLISTBOXENTRY;

/// struct to set the visibility of the cursor
typedef struct CURSORVISIBLE
{
	bool bVisible; /// visible or not
} CURSORVISIBLE;

/// structure for getting combobox data
typedef struct COMBODATA
{
	StdString text; /// text to return
	int index; /// index to get text at
} COMBODATA;

/// structre for getting and setting text
typedef struct TEXTDATA
{
	IHashString *text; /// text to get/set
} TEXTDATA;

/// structure for getting the select rectange
typedef struct SELECTRECT
{
	float x1; /// left side
	float x2; /// right side
	float y1; /// top side
	float y2; /// bottom side
} SELECTRECT;

/// structure for getting/setting zorder
typedef struct ZORDERDATA
{
	int m_iZOrder; /// z order to get/set (lower z order gets rendered later)
} ZORDERDATA;

/// structure for getting objects under cursor
typedef struct OBJUNDERCURSOR
{
	float m_fPosX; /// x pos of mouse
	float m_fPosY; /// y pos of mouse
    float m_fPosZ; /// likely unused
	IHashString *m_Name; /// name of component under mouse, if any
	IHashString *m_CompType; /// type of component under mouse, if any
} OBJUNDERCURSOR;

/// structure for getting objects inside rectangle
typedef struct OBJINRECT
{
	int x1; /// left side
	int x2; /// right side
	int y1; /// top side
	int y2; /// bottom side
	OBJECTLIST list; /// list of objects
} OBJINRECT;

/// struct used to ask gui elements if they know anything given a name
/// this is used solely in the tas radar at the moment, tho it's generic so it could
/// be used anywhere
typedef struct GUIQUERYBYNAME
{
	IHashString * name; // name to query
	bool bFound; // t/f if the gui elem has knowledge of such an item
}GUIQUERYBYNAME;

/// struct to wrap a bool for get/set to/from messages
typedef struct GUIBOOL
{
	bool bVal;
	operator bool() { return bVal; }		// cast operator will allow message c side to cast
	operator bool*() { return &bVal; }		// to regular bools
}GUIBOOL;

struct CHANGELANGUAGEKEYPARAMS
{
	IHashString *hszKey;
	IArchive *archive;
	CHANGELANGUAGEKEYPARAMS()
	{
		hszKey = NULL;
		archive = NULL;
	}
};

// toLua_end