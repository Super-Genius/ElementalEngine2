///---------------------------------------------------------------------
/// \file		BaseMessages.h
/// Base messages for communication with the elemental engine.
/// These message definitions should only exist here.
/// \created	05/02/05
/// \author		Brian Bazyk
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
#ifndef BASEMESSAGES_H
#define BASEMESSAGES_H

namespace ElementalEngine
{

// Predeclarations to avoid dependancies
class IHashString;
class IArchive;
class IXMLArchive;
class ITextureObject;
class IBaseTextureObject;
class ICubeTextureObject;
class IActionHandler;
class IMeshObject;
class IShader;
class IEffect;
class IShaderCallback;
class IShaderVarFunc;
class IVertexBufferObject;
class IIndexBuffer;
class IGeometry;
class IParticleType;
class ILightObject;
class IFontObject;
class IParticleEmitter;
class IRenderContext;
class IMaterial;
class IFontImplementation;
class IShaderPath;
class IMaterialTemplate;
class AABB;
class IScene;

//tolua_begin

#define STREAM_MODE_READ 1
#define STREAM_MODE_WRITE 2
#define STREAM_MODE_TRUNCATE 4
#define STREAM_MODE_BINARY 8
#define ARCHIVE_MODE_WRITE 16

// ---Messages---

//=====================================
// Math Manager
//=====================================
/// Map of an implementation name to a pointer to the implementation.
typedef std::map<DWORD, void*> MATHIMPLEMENTATIONMAP;

struct ACTIVEMATHIMPLEMENTATION
{
	DWORD dwImplementationName;
	void *pImplementation;
};

struct REGISTERMATHIMPLEMENTATION
{
	int iPriority;
	DWORD dwLibraryName;
	MATHIMPLEMENTATIONMAP *pImplementationMap;
};
//=====================================

//=====================================
// Object Factory
//=====================================
struct CREATEOBJECTPARAMS
{
	IHashString *typeName;			// type of object, i.e. CAIObject
	IHashString *name;				// the name of the object to create
	IHashString *parentName;		// the parents name 
};

struct DELETEOBJECTPARAMS
{
	IHashString *name;              // name of object to delete
};

struct INITOBJECTPARAMS
{
	IHashString *name;              // name of object to initialize
};

struct SERIALIZEOBJECTPARAMS
{
	IHashString *name;              // name of object to serialize
	IArchive *archive;              // archive to serialize to/from
};

struct FINDOBJECTPARAMS
{
	IHashString *hszName;			// name of object to look for
	bool bFound;					// result of search

	FINDOBJECTPARAMS()
	{
		hszName = NULL;
		bFound = false;
	}
};

struct UNLOADOBJECTPARAMS
{
	IHashString *name;
};

struct GETCOMPONENTTYPEPARAMS
{
	IHashString *name;
	IHashString *componenttype;
};

struct OFACTORYADDPARAMS
{
	IHashString *name;			// name of component
	IComponent *component;		// component
};

struct GENERATEUNIQUEOBJECTNAMEPARAMS
{
	GENERATEUNIQUEOBJECTNAMEPARAMS()
	{
		name = NULL;
		newname = NULL;
	}
	IHashString *name;			// pointer to name of object 
	IHashString *newname;		// pointer to pre-existing destination hashstring to receive new name
};

//=====================================

enum KEYBIND_TYPE
{
	KEYBIND_PRESSED		= (1<<0),
	KEYBIND_RELEASED	= (1<<1),
	KEYBIND_ANALOG_CHANGED = (1<<2),
};

// key binding structure
typedef struct KEYBIND
{
	KEYBIND()
	{
		m_bHitOnce = true;
		m_bReleaseOnce = true;
		m_cBindType = KEYBIND_PRESSED;
	}
	DWORD m_kbNameID;	// name of keybinding setup (IHashString's uniqueID)
	DWORD m_KeyID;			// key
	DWORD m_MessageID;		// msg to send (IHashString's uniqueID)
	bool m_bHitOnce;		// key hit only once?
	bool m_bReleaseOnce;	// key release only once?
	char m_cBindType;		// 1 = Pressed, 2 = Released, 3 = Pressed or Released
} KEYBIND;

// Params to be sent back to function bound to the key event
struct KEYEVENTPARAMS
{
	char m_cBindType;
	float m_fValue;
	int m_iDeviceID;
	int m_iKeyID;
};

// input engine parameters
typedef struct INPUTENGPARAMS
{
	// Handle to a module
	HMODULE hMod; 
	// Handle to a window
	HWND hWnd; 
	// mouse immediate mode?
	bool mouseImmediate; 
	// mouse exclusive mode?
	bool mouseExclusive;
	// mouse foreground mode?
	bool mouseFG;
	// keyboard immediate mode?
	bool kbImmediate;
	// keyboard exclusive mode?
	bool kbExclusive;
	// keyboard foreground mode?
	bool kbFG;
} INPUTENGPARAMS;

// mouse coordinate structure
typedef struct MOUSECOORDS
{
	float	m_fXPos;			// x position of the mouse in normalized coordinates
	float	m_fYPos;			// x position of the mouse in normalized coordinates
	long	m_lXPos;			// x position of the mouse
	long	m_lYPos;			// y position of the mouse
	long	m_lXDelta;			// x change of position
	long	m_lYDelta;			// y change of position
	bool	m_LButtonDown;		// true if the left mouse button is pressed
	bool	m_RButtonDown;		// true if the right mouse button is pressed
} MOUSECOORDS;

// joystick key binding structure
struct JOYSTICKKEYBINDPARAMS
{
	DWORD m_kbNameID;		// name of keybinding setup (IHashString's uniqueID)
	DWORD m_DeviceID;		// ID for the devices
	DWORD m_KeyID;			// key
	DWORD m_MessageID;		// msg to send (IHashString's uniqueID)
	bool m_bRepeat;			// button repeat
	int m_EventFilter;		// 1 = Pressed, 2 = Released, 3 = Pressed or Released

	JOYSTICKKEYBINDPARAMS()
	{
		m_kbNameID = 0;
		m_DeviceID = 0;
		m_KeyID = 0;
		m_MessageID = 0;
		m_bRepeat = false;
		m_EventFilter = KEYBIND_PRESSED;
	}

};

struct JOYSTICKDEVICEMAPPARAMS
{
	int iOldDeviceID;
	int iNewDeviceID;
};

struct JOYSTICKHARDWAREFAULTPARAMS
{
	int iJoystick;
	int iInput;
	bool bFault;
};

struct JOYSTICKFAULTDETECTIONPARAMS
{
	JOYSTICKFAULTDETECTIONPARAMS()
	{
		iInput = -1;
	}
	int iJoystick;
	int iInput;
	bool bEnable;
};

struct JOYSTICKFAULTQUERYPARAMS
{
	JOYSTICKFAULTQUERYPARAMS()
	{
		iJoystick = -1;
		iInput = 0xFF; //EEJ_NONE
		out_bFault = false;
	}
	int iJoystick;
	int iInput;
	bool out_bFault;
};

struct JOYSTICKMOTORSPEEDPARAMS
{
	JOYSTICKMOTORSPEEDPARAMS()
	{
		iJoystick = -1;
		cMotor = 0;
		fSpeed = 0.0;
	}
	int iJoystick;
	char cMotor;
	float fSpeed;
};

typedef struct FONTIMPLEMENTATIONMSG
{
	IFontImplementation * impl;
	FONTIMPLEMENTATIONMSG()
	{
		impl = NULL;
	}
} FONTIMPLEMENTATIONMSG;
/// struct to query size in pixels of given string
typedef struct FONTPIXWIDTHQUERY
{
	IHashString *szFilename;	/// name of font file
	StdString szText;			/// text to check width of
	UINT iFontsize;				/// size of font
	int iWidth;					/// width returned
}FONTPIXWIDTHQUERY;

typedef struct ADDFONTGLYPHFILENAMEPARAMS
{
	IHashString *hsKey;			/// name of key
	IHashString *hsFile;		/// name of texture filename
	ADDFONTGLYPHFILENAMEPARAMS()
	{
		hsKey = NULL;
		hsFile = NULL;
	}
}ADDFONTGLYPHFILENAMEPARAMS;

typedef struct GETFONTGLYPHTEXTUREPARAMS
{
	IHashString *hsKey;
	IBaseTextureObject *pTexture;
	GETFONTGLYPHTEXTUREPARAMS()
	{
		hsKey = NULL;
		pTexture = NULL;
	}
}GETFONTGLYPHTEXTUREPARAMS;

typedef struct ADDGEOMLISTPARAMS
{
	ADDGEOMLISTPARAMS()
	{
		dwSortMethod = 0;
		dwListPriorityKey = 0;
		objList = NULL;
		context = NULL;
	}
	list<IGeometry	*> * objList;	//The list of objects to add
	DWORD		dwSortMethod;//The sort method for this render list (SORTMETHOD )
	DWORD		dwListPriorityKey;	//Render List ID
	IRenderContext *context;
} ADDGEOMLISTPARAMS;

typedef struct ADDRENDERLISTPARAMS
{
	ADDRENDERLISTPARAMS()
	{
		dwSortMethod = 0;
		dwListPriorityKey = 0;
		objList = NULL;
		dwFuncCallback = NULL;
		dwFuncPostRenderCallback = NULL;
		renderContext = NULL;
	}
	OBJECTLIST	* objList;	//The list of objects to add
	DWORD		dwSortMethod;//The sort method for this render list (SORTMETHOD )
	DWORD		dwListPriorityKey;	//Render List ID
	void		(*dwFuncCallback)();
	void		(*dwFuncPostRenderCallback)();
	IRenderContext * renderContext;
} ADDRENDERLISTPARAMS;

typedef struct POSTPROCESSEFFECTPARAMS
{
	IEffect *effect;
	IRenderContext *context;

	POSTPROCESSEFFECTPARAMS()
	{
		effect = NULL;
		context = NULL;
	}
}POSTPROCESSEFFECTPARAMS;


enum RENDER_TARGET_TYPE { RENDER_TARGET_NONE = 0,
						RENDER_TARGET_DEPTH_LUMINANCE,
						RENDER_TARGET_COLOR_BUFFER,
						RENDER_TARGET_FLOATINGPOINT,
						RENDER_TARGET_COMPRESSED_FLOATINGPOINT,
						RENDER_TARGET_DEPTH_SURFACE,
						RENDER_TARGET_TWO_CHANNEL_FP,
						RENDER_TARGET_ONE_CHANNEL_FP
};

typedef struct CREATETEXTUREPARAMS
{
	IHashString	*Name;		//name of texture
	IHashString *Format;
	UINT		sizeX;
	UINT		sizeY;
	UINT		bitDepth;
	RENDER_TARGET_TYPE	bRenderTargetTexture;
	IBaseTextureObject * TextureObjectInterface;
	UINT		numMips;
	bool		bAutoGenMipMaps;
	CREATETEXTUREPARAMS()
	{
		Name = NULL;
		Format = NULL;
		sizeX = 0;
		sizeY = 0;
		bitDepth = 0;
		numMips =1;
		bAutoGenMipMaps = false;
		bRenderTargetTexture = RENDER_TARGET_NONE;
		TextureObjectInterface = NULL;
	}
} CREATETEXTUREPARAMS;

typedef struct CREATECUBETEXTUREPARAMS
{
	IHashString	*Name;		//name of texture
	UINT		size;
	UINT		bitDepth;
	ICubeTextureObject * CubeTextureObjectInterface;
	CREATECUBETEXTUREPARAMS()
	{
		Name = NULL;
		size = 0;
		bitDepth = 0;
		CubeTextureObjectInterface = NULL;
	}
} CREATECUBETEXTUREPARAMS;

typedef struct CREATEMESHOBJECTPARAMS
{
	IMeshObject * MeshObjectInterface;
} CREATEMESHOBJECTPARAMS;

typedef struct RENDERLISTTOTEXTUREPARAMS
{
	RENDERLISTTOTEXTUREPARAMS()
	{
		dwSortMethod = 0;
		objList = NULL;
		dwFuncCallback = NULL;
		RenderTexture = NULL;
		Camera = NULL;
		clearDepth = true;
		clearTarget = true;
		effectOverride = NULL;
	}
	IEffect *	effectOverride;
	bool		clearDepth;
	bool		clearTarget;
	IHashString * Camera;
	ITextureObject * RenderTexture;
	OBJECTLIST	* objList;	//The list of objects to add
	DWORD		dwSortMethod;//The sort method for this render list (SORTMETHOD )
	void		(*dwFuncCallback)();
} RENDERLISTTOTEXTUREPARAMS;

typedef struct CREATERENDERCONTEXTPARAMS
{
	HWND		hwindow;
	bool		fullscreen;
	UINT		uiRenderContextID;
	int			width;
	int			height;
	int			depthbits;
	int			colorbits;
	IRenderContext *context;
	CREATERENDERCONTEXTPARAMS()
	{
		uiRenderContextID = 0;
	}
} CREATERENDERCONTEXTPARAMS;

typedef struct SETWORKINGDIRECTORIESPARAMS
{	
	StdString	* BaseDirectory;
	StdString	* ResourceDirectory;
	SETWORKINGDIRECTORIESPARAMS()
	{
		BaseDirectory = NULL;
		ResourceDirectory = NULL;
	}
} SETWORKINGDIRECTORIESPARAMS;

typedef struct VIEWPORTPARAMS
{
	VIEWPORTPARAMS()
	{
		offsetx = offsety = 0;
		width = height = 0;
	}

	int offsetx;
	int offsety;
	int width;
	int height;
} VIEWPORTPARAMS;

struct COLORCORRECTIONPARAMS
{
	COLORCORRECTIONPARAMS()
	{
		fBrightness = 0.5;
		fContrast = 0.5;
		fGamma = 1.0;
	}
	float fBrightness;
	float fContrast;
	float fGamma;
};

struct PLAYMOVIEPARAMS
{
	PLAYMOVIEPARAMS()
	{
		in_movieFileName = NULL;
		inout_movieTargetTextureName = NULL;
		out_pTargetTexture = NULL;
	}
	/// File name of the movie to play.
	IHashString *in_movieFileName;
	/// If NULL, the movie player sets this pointer to a generated target texture name.
	/// Otherwise, movie player uses the provided name to generate the target texture.
	IHashString *inout_movieTargetTextureName;
	/// Returned pointer to target texture to be used to render movie.
	ITextureObject *out_pTargetTexture;
};

typedef list<ISceneObject*> SCENEOBJECTLIST;

typedef struct SCENEOBJECTPARAMS
{
	SCENEOBJECTPARAMS()
	{
		pObject = NULL;
		sortID = 0;
		pScene = NULL;
		bBoundedObject = true;
	}
	ISceneObject*	pObject;
	DWORD			sortID;
	IScene*			pScene;
	bool bBoundedObject; // object has a bounding shape ie. bounding sphere; otherwise object is global.
}SCENEOBJECTPARAMS;

// Depricated
typedef struct VISITOBJECTQUADNODEPARAMS
{
	ISceneObject* object;
	IVisitor* visitor;
} VISITOBJECTQUADNODEPARAMS;

/// Visits all objects of the given sortID in a scene
typedef struct VISITSCENEPARAMS
{
	IVisitor*		visitor;
	DWORD			sortID;
} VISITSCENEPARAMS;

/// Visits all objects of the given sortID within a frustum
typedef struct VISITSCENEFRSUTUMPARAMS
{
	IVisitor*		visitor;
	CFrustum*		frustum;
	DWORD			sortID;
} VISITSCENEFRSUTUMPARAMS;

/// Visits all objects of the given sortID within an AABB
typedef struct VISITSCENEAABBPARAMS
{
	IVisitor*		visitor;
	AABB*			aabb;
	DWORD			sortID;
} VISITSCENEAABBPARAMS;

/// Visits all objects of the given sortID within a sphere
typedef struct VISITSCENESPHEREPARAMS
{
	IVisitor*		visitor;
	Vec3*			center;
	float			radius;
	DWORD			sortID;
} VISITSCENESPHEREPARAMS;

/// structure for changing object names
typedef struct CHANGEOBJNAME
{
	IHashString *oldName;
	IHashString *newName;
	IHashString	*compType;
} CHANGEOBJNAME;

/// structure for changing object parent
struct CHANGEOBJECTPARENT
{
	IHashString *objectName;
	IHashString *objectCompType;
	IHashString *newParentName;
};

typedef struct FINDOBJNAME
{
	IHashString *searchName;
	IHashString	*compType;
	bool		bFound;
} FINDOBJNAME;

struct GETSETACTIVESCENE
{
	IHashString *pSceneName;
};

struct SETSCENERENDERCONTEXTPARAMS
{
	IHashString *pSceneName;
	IRenderContext *pRenderContext;
};

typedef struct VISITHIERARCHYDATA
{
	VISITHIERARCHYDATA()
	{
		name = NULL;
		visitor = NULL;
		rootNodeName = NULL;
	}
	IHashString *name;				// staring node, or NULL for top
	IVisitor	*visitor;			// visitor class for callbacks
	IHashString *rootNodeName;
} VISITHIERARCHYDATA;

typedef struct VISITENTITYDATA
{
	IHashString *searchString;		// Search string for the entity manager
	IVisitor	*visitor;			// visitor class for callbacks

	VISITENTITYDATA()
	{
		searchString = NULL;
		visitor = NULL;
	};

} VISITENTITYDATA;

/// structure for passing a pointer to in a message
/// for setting/getting the position of an object
typedef struct POSITIONDATA
{
	float	m_fXPos;				/// The float point X Position
	float	m_fYPos;				/// The float point Y Position
	float	m_fZPos;				/// The float point Z Position
} POSITIONDATA;

/// structure for passing a pointer to in a message
/// for getting/setting the width/height of an object
typedef struct SIZEDATA
{
	float	m_fXSize;				/// The float point X Size
	float	m_fYSize;				/// The float point Y Size
	float	m_fZSize;				/// The float point Z Size
} SIZEDATA;

typedef struct CREATEARCHIVE
{
	IHashString *streamType;		/// type of stream, Memory, File, other
	void *streamData;				/// stream specific data, i.e. filename, memory ptr
	DWORD streamSize;				/// could be stream size if needed
	DWORD mode;				/// enum of reading/writing
	IArchive *archive;				/// archive that is created
} CREATEARCHIVE;

typedef struct ARCHIVE
{
	IArchive *archive;				/// archive that is created
} ARCHIVE;

typedef struct CREATESTREAM
{
	void *streamData;				/// parameter specific to stream to create
	DWORD streamSize;				/// the size of the stream if needed
	DWORD mode;				/// the mode of the stream to create (read/write)
	IEEStream	*openStream;			/// the return stream that was opened
} CREATEFILESTREAM;

struct ACTIONSETPARMS
{
    IActionHandler* actionSet;
};

///Represents the events intercepted and handled by state machines.
struct EVENT
{
	///Holds the "GetUniqueID()" of the IHashString* provided in CREATEEVENT.
	///If the IHashString* was not provided, the client must set this DWORD before
	///raising the event.
	DWORD EventName;

    /// If non-null, holds parameters passed with event.
    IArchive* Archive;
};

struct CREATEEVENT
{
	///The event name that will be handled by a state machine.
	IHashString* EventName;

    //I'd prefer the following two items belonged to an anonymous union, but tolua preprocessor
    //barfs on unions.
    /// in: Anticipated size of memory Archive needed to hold event parameters (zero works too)
    unsigned Size;
    /// out: Newly created event, with pre-created memory archive for parameters
    EVENT* Event;
};

//=====================================
// Vertex Buffers
//=====================================
struct ALLOCATERENDERBUFFERMSG
{
	ALLOCATERENDERBUFFERMSG()
	{
		ElementsToAllocate = 0;
		m_ReturnHandle = 0;
		m_ReturnOffset = 0;
		m_ReturnInterfaceHandle = 0;
		SizeVertexFormat = 0;
	 	m_InterfacePointer = 0;
		Dynamic = false;
		m_AllowSharedBuffer = true;
		m_StreamMode = 0;
	}
	UINT	m_StreamMode;
	UINT	ElementsToAllocate;
	UINT	m_ReturnHandle;
	UINT	m_ReturnOffset;
	UINT	m_ReturnInterfaceHandle;
	UINT	SizeVertexFormat;
	bool	m_AllowSharedBuffer;
	bool	Dynamic;
	void *	m_InterfacePointer;
	CHANNELDESCRIPTORLIST	VertexFormat;
};

struct GETALLOCATIONVERTEXBUFFER
{
	UINT	AllocationHandle;
	IVertexBufferObject * m_VertexBufferObject;
	GETALLOCATIONVERTEXBUFFER()
	{
		m_VertexBufferObject = NULL;
	}
};

struct GETALLOCATIONINDEXBUFFER
{
	UINT	AllocationHandle;
	IIndexBuffer * m_IndexBufferInterface;
};


struct BUFFERALLOCATIONSTRUCT
{
	int m_Offset;
	int m_Size;
	int	m_Stride;
	int m_InterfaceHandle;
	int m_AllocationHandle;
	int m_BaseVertexOffset;//if allocation is relative to another offset	
	CHANNELDESCRIPTORLIST	VertexFormat;
	BUFFERALLOCATIONSTRUCT()
	{
		m_Offset = 0;
		m_Size = 0;
		m_Stride = 0;
		m_InterfaceHandle = 0;
		m_AllocationHandle = 0;
		m_BaseVertexOffset = 0;
	}
};
//=====================================
// Shaders
//=====================================
struct ADDSHADERCALLBACKMESSAGE
{
	IShaderCallback * m_ShaderCallback;
};

struct SHADERVARFUNCMESSAGE
{
	DWORD				uid;
	IShaderVarFunc * m_ShaderVarFunc;
};

struct CREATESHADERINTERFACE
{
	IShader * m_ShaderInterface;
	IHashString * m_Name;
	CREATESHADERINTERFACE()
	{
		m_ShaderInterface = NULL;
		m_Name = NULL;
	}
};

struct ADDSHADERPATH
{
	IHashString * m_Name;
	IHashString * m_API;
	IHashString * m_ShaderVersion;
	ADDSHADERPATH()
	{
		m_Name = NULL;
		m_API = NULL;
		m_ShaderVersion = NULL;
	}
};

struct LOADSHADERDIRECTORY
{
	TCHAR * dir;
	LOADSHADERDIRECTORY()
	{
		dir = NULL;
	}
};

struct GETSHADERPATH
{
	IShaderPath * m_Path;
	IHashString * m_Name;
	GETSHADERPATH()
	{
		m_Path = NULL;
		m_Name = NULL;
	}
};

typedef map< DWORD, IEffect* > NAMETOEFFECTMAP;
struct GETEFFECTLIST
{
	NAMETOEFFECTMAP * m_pEffectList;
	GETEFFECTLIST()
	{
		m_pEffectList = NULL;
	}
};

struct ADDSHADERTOMANAGER
{
	IShader * m_ShaderInterface;
	IHashString * m_Name;
	IHashString * m_CompType;
	ADDSHADERTOMANAGER()
	{
		m_ShaderInterface = NULL;
		m_Name = NULL;
		m_CompType = NULL;		
	}
};

struct MATERIALTEMPLATEINTERFACE
{
	IMaterialTemplate * m_MatInterface;
	IHashString * m_Name;
	MATERIALTEMPLATEINTERFACE()
	{
		m_MatInterface = NULL;
		m_Name = NULL;
	}
};

struct CREATEEFFECTINTERFACE
{
	IEffect * m_EffectInterface;
	IHashString * m_Name;
	CREATEEFFECTINTERFACE()
	{
		m_EffectInterface = NULL;
		m_Name = NULL;
	}
};

struct CREATEMATERIALINTERFACE
{
	IMaterial * m_MaterialInterface;
	IHashString * m_Name;
	CREATEMATERIALINTERFACE()
	{
		m_MaterialInterface = NULL;
		m_Name = NULL;
	}
};

struct GETRENDERFLAGVALUE
{
	vector< IHashString * > m_Hashes;
	DWORD					m_ReturnValue;
	GETRENDERFLAGVALUE()
	{
		m_ReturnValue = 0;
	}
};

typedef struct FOGVALUEMESSAGE
{
	float	m_Start;
	float	m_FogDistance;
	Vec3 *	m_Color;
	FOGVALUEMESSAGE()
	{
		m_Start = 0;
		m_FogDistance = 10000;
		m_Color = NULL;
	}
} FOGVALUEMESSAGE;

//=====================================
// Fonts
//=====================================
struct CREATEFONTINTERFACE
{
	IFontObject * m_FontInterface;
	CREATEFONTINTERFACE()
	{
		m_FontInterface = NULL;
	}
};

//=====================================
// Lights
//=====================================
struct GETLIGHTOBJPTRMESSAGE
{
	IHashString * hszObjName;
	ILightObject * pLightObject;
	GETLIGHTOBJPTRMESSAGE()
	{
		hszObjName = NULL;
		pLightObject = NULL;
	}
};

struct GETLIGHTLIST
{
	vector< ILightObject * > lightList;
};

struct GETLIGHTSHADOWTEXTURE
{
	ILightObject * pLightObj;
	IBaseTextureObject * pLightTexture;
	GETLIGHTSHADOWTEXTURE()
	{
		pLightTexture = NULL;
		pLightObj = NULL;
	}
};

//=====================================
// 3D Sprites( LOD )
//=====================================

typedef struct ADDSPRITELISTMESSAGE
{
	int						m_numVertices;
	ChannelDesc *			m_ChannelDescriptor;
	int						m_NumChannels;
    void	*				m_Data;
	IBaseTextureObject  *	m_Texture;
	IEffect				*	m_Effect;

	ADDSPRITELISTMESSAGE( )
	{	
		m_Data = 0;
		m_numVertices = 0;
		m_ChannelDescriptor = 0;
		m_NumChannels = 0;
		m_Effect = NULL;
		m_Texture = NULL;
	}
} ADDSPRITELISTMESSAGE;

//=====================================
// Particles
//=====================================
struct GETPARTICLETYPEMESSAGE
{
	IHashString * filename;
	IParticleType * particletype;
	GETPARTICLETYPEMESSAGE()
	{
		filename = NULL;
		particletype = NULL;
	}
};

struct GETPARTICLEEMITTERMESSAGE
{
	IHashString * filename;
	IParticleEmitter * emittertype;
	GETPARTICLEEMITTERMESSAGE()
	{
		filename = NULL;
		emittertype = NULL;
	}
};

struct PARTICLEPROPERTIESKEY
{
	IHashString * propertyname;
	float time;
	float value;
	PARTICLEPROPERTIESKEY()
	{
		propertyname = NULL;
	}
};

struct PARTICLELOADINFO
{
	IHashString * filepath;
	IArchive * data;
	PARTICLELOADINFO()
	{
		filepath = NULL;
		data = NULL;
	}
};


struct PARTICLEEDITORRENDERMESSAGE
{
	IRenderContext * rendercontext;
	bool render;
	PARTICLEEDITORRENDERMESSAGE()
	{
		rendercontext = NULL;
		render = 0;
	}
};

struct PARTICLEFILEPARAMS
{
	IHashString * filename;
	IHashString * newfilename;

	PARTICLEFILEPARAMS()
	{
		filename = NULL;
		newfilename = NULL;
	}
};

//=====================================
// Time of Day
//=====================================
struct TIMEOFDAYDESC
{
	Vec3 *vecSunDir; // vector from lit point to the sun
	float fTime;    // [0,1] where [0,0.5] => midnight to noon and [0.5,1] => noon to midnight
	float fSkyDawnWeighting;
	float fSkyNightWeighting;
	bool bFog;
	bool bLighting;
};



//=====================================

struct GETAFFECTEDOBJECTSPARAMS
{
	Vec3 *pPosition;
	float fRadius;
	std::list<IHashString*> *pAffectedObjectsList;
};

//tolua_end
enum BRUSH_OP
{
	BRUSH_OP_ADD,
	BRUSH_OP_MUL,
	BRUSH_OP_MASK_SET,
	BRUSH_OP_SMOOTH,
};

struct BRUSHPARAMS
{
	BRUSHPARAMS()
	{
		m_pPosition = NULL;
		m_fSize = 0.0f;
		m_fStrength = 0.0f;
		m_eBrushOp = BRUSH_OP_ADD;
		m_bMaxCap = false;
		m_bMinCap = false;
		m_pBrushData = NULL;
		m_iBrushDataSize = 0;
	}
	Vec3 *m_pPosition;  // position of brush in world space
	float m_fSize;       // size of brush in world space
	float m_fStrength;   // strength of brush [0,100]
	BRUSH_OP m_eBrushOp; // operation to perform
	bool m_bMaxCap;      // whether to clamp to a max value
	float m_fMaxCap;     // max value to clamp to
	bool m_bMinCap;      // whether to clamp to a minvalue
	float m_fMinCap;     // min value to clamp to
	float *m_pBrushData; // brush data [datasize*datasize]
	int m_iBrushDataSize; // size of the brush's data (along a side)
};
//tolua_begin

//=====================================
// Heightmap
//=====================================
struct HEIGHTMAPDATAPARAMS
{
	HEIGHTMAPDATAPARAMS()
	{
		iWidth = iHeight = 0;
		pData = NULL;
		pathName = NULL;
	}
	int iWidth;
	int iHeight;
	void* pData;
	StdString *pathName;
};


//=====================================

//=====================================
// RenderObject Bone Animation
//=====================================
struct BONEINFO
{
	BONEINFO()
	{
		bonename = NULL;
		m_Rotation = NULL;
		m_Position = NULL;
		m_Matrix = NULL;
		//Displaced info adds on the movement
		//of the biped or Render object. UnDisplaced info
		//takes out the displacement
		m_DisplacedInfo = false;
	}
	IHashString * bonename;
	bool		m_DisplacedInfo;
	EulerAngle	*m_Rotation;
	Vec3	*m_Position;
	Matrix4x4 * m_Matrix;
};

typedef list<BONEINFO> BONELIST;

struct SKELETONINFO
{
	BONELIST bones;
};

//=====================================
struct GETBOUNDINGSPHEREPARAMS
{
	Vec3 * position;
	float radius;
	GETBOUNDINGSPHEREPARAMS(){
		radius = 0;
		position = NULL;
	}
};

struct GETBOUNDINGBOXPARAMS
{
	Vec3* position;
	Vec3* extents;
	EulerAngle* rotation;

	GETBOUNDINGBOXPARAMS()
	{
		position = NULL;
		extents = NULL;
		rotation = NULL;
	}
};
//=====================================

//=====================================
// RenderObject SubNode Hierarchy
//=====================================

//hierarchy define
#define HRC_CHILD_NOT_FOUND -1
struct RENDERSUBNODEGETIDPARAM
{
	IHashString *in_Name;
	int out_ID;
};

struct RENDERSUBNODEROTATEAXISANGLEPARAM
{
	int		in_ID;
	Vec3  *in_Axis;
	float	in_AngleDegree;
};

struct RENDERSUBNODEROTATEEULER
{
	int		in_ID;
	float in_XRotate;
	float in_YRotate;
	float in_ZRotate;
};

struct RENDERSUBNODEPOSITION
{
	int		in_ID;
	float m_XOffset;
	float m_YOffset;
	float m_ZOffset;
};

//=====================================

//=====================================
// Camera
//=====================================
enum EnumCameraRotateType
{
	kCameraRotateSetVectors,
	kCameraRotateSetEulerAngles,
	kCameraRotateAddEulerAngles,
	kCameraRotateAddEulerAnglesClampPitch,
//	kCameraRotateSetQuaternion,
//	kCameraRotateAddQuaternion,
};

/// struct to attach a camera to a context
typedef struct CONTEXTCAMPARAMS 
{
	CONTEXTCAMPARAMS()
	{
		szCameraName = NULL;
		pRC = NULL;
	}
	IHashString *szCameraName; // name of camera to get/set
	IRenderContext *pRC; // render context to get/set camera to
}CONTEXTCAMPARAMS;

/// struct to get position or rotation of camera attached to specified context
typedef struct CONTEXTCAMVECTORPARAMS
{
	IRenderContext *pRC; // render context to get camera of
	Vec3 *vdata; // data to return, either position or rotation
	EulerAngle * rotation;
}CONTEXTCAMVECTORPARAMS;
struct UPDATECAMERAPARAMS
{
	UPDATECAMERAPARAMS()
	{
		bSave = false;
		eRotateType = kCameraRotateSetVectors;
		vPosition = NULL;
		vUpVector = NULL;
		vViewVector = NULL;
		vEuler = NULL;
	}

	/// Set bSave to true to write from paramters to the camera
	/// and false to write from the camera to the parameters.
	bool bSave;

	Vec3 *vPosition;
	float fHorizontalFOVAngle;
	float fVerticalFOVAngle;
	float fNearClip;
	float fFarClip;

	// rotate type tells the camera which rotation data to use when saving
	// data from params to the camera
	EnumCameraRotateType eRotateType;
	// rotate vectors
	Vec3 *vUpVector;
	Vec3 *vViewVector;
	// rotate euler
	EulerAngle *vEuler;
};

struct OBJECTINCAMERAPARAMS
{
	bool	bReturn;
	float	fRadius;
	float	fHorizontalBuffer;
	float	fVerticalBuffer;
	Vec3	*v3Position;
	OBJECTINCAMERAPARAMS()
	{
		bReturn = false;
		fRadius = 1.0f;
		fHorizontalBuffer = 0.0f;
		fVerticalBuffer = 0.0f;
	}
};

struct MOVECAMERAPARAMS
{
	MOVECAMERAPARAMS()
	{
		bRelative = false;
		fForwardDelta = 0.0f;
		fUpDelta = 0.0f;
		fRightDelta = 0.0f;
	}

	bool bRelative;
	float fForwardDelta;
	float fUpDelta;
	float fRightDelta;
};

struct ROTATECAMERAPARAMS
{
	// designates what type of operation to take on the camera's rotation
	EnumCameraRotateType eRotateOperation;
	// euler
	EulerAngle *vEuler;
	// vectors
	Vec3 *vViewVector;
	Vec3 *vUpVector;

	float fClampPitchMax;
	float fClampPitchMin;
};

struct CAMERAVECTORSPARAMS
{
	// vectors
	Vec3 *vViewVector;
	Vec3 *vUpVector;
	Vec3 *vRightVector;
	CAMERAVECTORSPARAMS()
	{
		vViewVector = NULL;
		vUpVector = NULL;
		vRightVector = NULL;
	}
};

// sent to manager to forward to context-specific camera
struct CONTEXTCAMZOOMPARAMS
{
	IRenderContext *pRC;
	float fZoomFactor;
	float fZoomTime;
	CONTEXTCAMZOOMPARAMS()
	{
		pRC = NULL;
		fZoomFactor = 1.0f;
		fZoomTime = 0.0f;
	}
};

// sent directly to camera
struct CAMERAZOOMPARAMS
{
	float fZoomFactor;
	float fZoomTime;
	CAMERAZOOMPARAMS()
	{
		fZoomFactor = 1.0f;
		fZoomTime = 0.0f;
	}
};

//=====================================

/// Area for loading texture.
enum TEX_MEM_AREA {
	TEX_MEM_VIDEO,
	TEX_MEM_SYSMEM,
};

/// Parameters for load texture.
struct LOADPARAMETERS {
	/// Area for loading texture.
	TEX_MEM_AREA texMemArea;
	/// Constructor.
	LOADPARAMETERS(TEX_MEM_AREA area = TEX_MEM_VIDEO) 
	:	texMemArea(area)
	{}
};

typedef struct GLOBALTICKPARAMS
{
	/// Current Tick Count
	DWORD tickCount;
} GLOBALTICKPARAMS;

//some param structures for messages:
typedef struct TEXTUREOBJECTPARAMS
{
	IHashString *Name;		//name of texture
	IBaseTextureObject *TextureObjectInterface;
	bool bLoad;				/// whether to load or not

	TEXTUREOBJECTPARAMS()
	{
		Name = NULL;
		TextureObjectInterface = NULL;
		bLoad = true;
	}
} TEXTUREOBJECTPARAMS;

typedef struct WORLDTOSCREENCOORDPARAMS
{
	int screenX;
	int screenY;
	float screenZ;
	Vec3 *worldXYZ;
	IHashString *cameraName;
	WORLDTOSCREENCOORDPARAMS()
	{
		cameraName = NULL;
	}
} WORLDTOSCREENCOORDPARAMS;

// message params for raycasting
struct RAYFROMMOUSEPARAMS
{
	float mouseX;
	float mouseY;
	Vec3 *rayPos;
	Vec3 *rayDir;
};

#define MAX_MESSAGE_SIZE 2048

struct LOGMESSAGESTRUCT
{
	TCHAR *msg;			// message to output
	DWORD severity;		// the severity of the message LOGWARNING, LOGERROR
};

struct BBOXPARAMS
{
	float posX;
	float posY;
	float posZ;
	float dimX;
	float dimY;
	float dimZ;
	float rotX;
	float rotY;
	float rotZ;
};

typedef struct BBOXLISTPARAMS
{
	std::list<BBOXPARAMS> *listBBoxes;
} BBOXLISTPARAMS;

///Just for debugging, serves as parameters for audible system beep.
struct BEEPBOX
{
    int frequency;
    int duration;

};

typedef struct LOADINGUPDATEPARAMS
{
	DWORD currentPosition;	// current position of load/save
	DWORD totalSize;		// size of total load/save
} LOADINGUPDATEPARAMS;

typedef struct LOADFILEEXTPARAMS
{
	TCHAR *fileName;	// name of file to load by extension
	bool bInternalLoad;	// is this an internal load (i.e. lua script for state machine)
						// or external (i.e. lua script for editing in GDS) 
} LOADFILEEXTPARAMS;

struct ISENTITYOFTYPEPARAMS
{
	IHashString *entityType;
	bool bResult; // set by message receiver if entity is of type
};

struct GETSCHEMAPARAMS
{
	IXMLArchive *ar;
};

/// struct for material overrides
/// here because it's used by both cal3d and hierarchical model plugins
struct MATERIALOVERRIDEPARAMS
{
	IHashString *hsMaterialName;		// material name, if any
	IHashString *hsType;				// type of override (see CMaterialOverride for supported types)
	IHashString *hsName;				// name of variable to override
	Vec4 *vecValue;						// vec4 value of var, if applicable
	IHashString *hsValue;				// string value of var, if applicable
	IHashString *hsNodeName;			// node name you want to affect specifically, if any
	MATERIALOVERRIDEPARAMS()
	{
		hsMaterialName = NULL;
		hsType = NULL;
		hsName = NULL;
		vecValue = NULL;
		hsValue = NULL;		
		hsNodeName = NULL;
	}
};

struct CLEARMATOVERRIDEPARAMS
{
	IHashString *hsMaterialName;	// material name if specified
	IHashString *hsNodeName;		// node name if specified
	CLEARMATOVERRIDEPARAMS()
	{
		hsMaterialName = NULL;
		hsNodeName = NULL;
	}
};

//==========================================================
// Perf timer structs
//==========================================================
struct SETDELTATIMERSCALEPARAMS
{
	IHashString *hsTimerName;
	float fTimeScale;
	SETDELTATIMERSCALEPARAMS()
	{
		hsTimerName = NULL;
		fTimeScale = 0.0f;
	}
};

struct GETTIMEDELTAPARAMS
{
	IHashString *hsTimerName;
	float fDeltaTime;
	GETTIMEDELTAPARAMS()
	{
		hsTimerName = NULL;
		fDeltaTime = 0.0f;
	}
};

#define GAMEDELTATIMER _T("GameDeltaTimer")

//==========================================================
// Attach / Detach structs
//==========================================================

struct ATTACHSCENEOBJECTPARAMS
{
	IHashString *hsBaseObject;			// name of the parent object
	ISceneObject *pBaseObject;			// pointer to parent object
	IHashString *hsNodeAttachName;		// node to attach to, if specified
	IHashString *hsAttachmentObject;	// name of the child object
	ISceneObject *pAttachmentObject;	// pointer to the child object

	ATTACHSCENEOBJECTPARAMS()
	{
		hsBaseObject = NULL;
		pBaseObject = NULL;
		hsNodeAttachName = NULL;
		hsAttachmentObject = NULL;
		pAttachmentObject = NULL;
	}
};

struct INIENTRYPARAMS
{
	IHashString *hsKey;
	IHashString *hsValue;
	INIENTRYPARAMS()
	{
		hsKey = NULL;
		hsValue = NULL;
	}
};

struct SAVEEDITINGPROPERTIESPARAMS
{
	IHashString *name;
	IArchive *ar;
	SAVEEDITINGPROPERTIESPARAMS()
	{
		name = NULL;
		ar = NULL;
	}
};

//==========================================================
// Audio structs
//==========================================================

struct PLAYSOUNDPARAMS
{
	IHashString	*fileName;		// name of file to play
	bool		looping;		// looping sound?
	bool		autoRemove;		// if this object should be deleted when done playing
	float		volume;			// 0.0 is silent, 1.0 is full volume
	bool		is3DSound;		// is a 3d sound?
	Vec3		*soundPosition;	// only has to be set if is3DSound is true
	float		attenStart;		// how far a before 3D sound starts fading, and how fast it fades (will depend on game scale)
	IHashString *objectName;	// name of object created, so we can stop looping sounds
	bool		bInHierarchy;	// put it in the hierarchy?
	IHashString *hsParentName;	// name of parent if the above boolean is set to true
	float		pitch;			// pitch multiplier

	PLAYSOUNDPARAMS()
	{
		fileName		= NULL;
		looping			= false;
		autoRemove		= true;
		volume			= 1.0f;
		is3DSound		= false;
		soundPosition	= NULL;
		attenStart		= 50.0f;
		objectName		= NULL;
		bInHierarchy	= false;
		hsParentName	= NULL;
		pitch			= 1.0f;
	}
};

struct STOPSOUNDPARAMS
{
	IHashString	*soundObjectName;	// name of object to stop
	float fadeOutVal;				// time to fade sound out (seconds)

	STOPSOUNDPARAMS()
	{
		soundObjectName = NULL;
		fadeOutVal = 0.0f;
	}
};

struct LISTENERVECTORPARAMS
{
	Vec3 *position;						// position of listener
	EulerAngle *orientation;			// orientation of listener
	bool bUpdateVelocity;				// recalc velocity?
										// NOTE: set to false if moving the listener
										// to a position instantaneously

	LISTENERVECTORPARAMS()
	{
		position = NULL;
		orientation = NULL;
		bUpdateVelocity = true;
	}
};

struct ADDSOUNDOBJECTPARAMS
{
	IHashString		*FileName;			// filename of the sound
	IHashString		*fileType;			// ext (.wav, .ogg)
	IEEStream		*fileStream;		// stream of file

	ADDSOUNDOBJECTPARAMS()
	{
		FileName = NULL;
		fileType = NULL;
		fileStream = NULL;
	}
};

struct LOADPLAYLISTPARAMS
{
	IHashString *fileName;				// filename of playlist
	IArchive *ar;						// archive of file info

	LOADPLAYLISTPARAMS()
	{
		fileName = NULL;
		ar = NULL;
	}
};

struct GETSOUNDFILEINFOPARAMS
{
	IHashString *fileName;
	UINT frequency;
	UINT bitDepth;
	UINT bitRate;
	UINT channels;
	UINT fileSize;
	float duration;

	GETSOUNDFILEINFOPARAMS()
	{
		fileName = NULL;
		frequency = 0;		// hertz
		bitDepth = 0;		// bits
		bitRate = 0;		// bits per sec
		channels = 0;		// channels
		fileSize = 0;		// bytes
		duration = 0.0f;	// seconds
	}
};

struct GLOBALSOUNDDATA
{
	DWORD dwFileName;						// hash of filename
	float fVolume;							// volume of sound
	bool bCachedUncompressed;				// cache uncompressed stream?

	GLOBALSOUNDDATA()
	{
		dwFileName = 0;
		fVolume = 1.0f;
		bCachedUncompressed = true;
	}
};

struct GENERATELIGHTPERSPECTIVEMAPPARAMS
{
	ILightObject* lightObject;
	OBJECTLIST* renderObjects;

	GENERATELIGHTPERSPECTIVEMAPPARAMS()
	{
		lightObject = NULL;
		renderObjects = NULL;
	}
};

//tolua_end

struct CHECKFILEEXISTS
{
	CHECKFILEEXISTS()
	{
		bExists = FALSE;
		pFilePath = NULL;
	}
	BOOL bExists;
	const TCHAR *pFilePath;
};

enum FINDFILE_RESULTS
{
	FINDFILE_OK,
	FINDFILE_NOTFOUND,
	FINDFILE_ERROR_INVALID_FILENAME,
	FINDFILE_ERROR_PATH_LENGTH,
	FINDFILE_ERROR_UNKNOWN
};

struct FINDFILE
{
	FINDFILE()
	{
		Result = FINDFILE_ERROR_UNKNOWN;
		bExists = FALSE; // TODO: Do we need this attribute?
		bReadOnly = FALSE;
		bDirectory = FALSE;
		pFilePath = NULL;
		pFileName= NULL;
		pInternalData = NULL;
	}
	FINDFILE_RESULTS Result;
	BOOL bExists;
	BOOL bDirectory;
	BOOL bReadOnly;
	TCHAR *pFilePath;
	TCHAR *pFileName;
	void *pInternalData;
};

}	//namespace ElementalEngine

#endif // #ifndef BASEMESSAGES_H
