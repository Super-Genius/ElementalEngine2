///==========================================================================
/// \file	CGUIManager.hpp
/// \brief	Manager for CGUIElement objects
///			This file defines CGUIManager which manages the creation
///			and manipulation of CGUIElement objects. 
///			Inherits from CManager
/// \date	3-22-2005
/// \author	Kenneth Hurley
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
///==========================================================================

#ifndef	_CGUIMANAGER_HPP_
#define	_CGUIMANAGER_HPP_

#include "CManager.h"

class CGUIElement;
class CGUIPage;
class CGUIStateWrapper;

typedef map<DWORD, IObject*> GUIMAP;

/// Invalid page is selected
#define GUI_INVALID_PAGE 0xffffffff

#define MAXVERTEXBUFFER 54*200 // 54 verts for max. smart scaling, 200 elems
#define GUIVERTEXSIZE 24

///==========================================================================
/// \class	CGUIManager
/// \brief	Manager class for CGUIElement objects
///==========================================================================
class CGUIManager : public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CGUIManager );

	/// \brief	Default Destructor
	///			Since CPhysManager is implemented as a singleton the
	///			constructor is private.
	CGUIManager();

public:


	/// \brief	Default Destructor
	~CGUIManager( );

	/// \brief	Function to get this component.
	///			Returns the singleton instance in this case
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	Update function for the AI Manager and AI System
	virtual void Update(DWORD tickCount);

	/// \brief	DeInitializes the AI Manager and AI System
	/// \return	True on sucess, False on failure
	virtual bool DeInit();

	/// Function to add an object to the manager
	/// \param Object is the IObject derived class pointer to add
	/// \return true if success, otherwise false
	virtual bool AddObject( IObject *Object, IHashString *group);

	/// Function to delete an object from the manager
	/// \param Object is the IObject derived
	virtual bool DeleteObject( IObject *Object );

	/// Get a pointer to the IObject from the guimanager
	/// \param guiName - the name of the IObject
	/// \param compType - the component type of guiName
	/// \return IObject* - pointer to the IObject
	IObject* GetObject(IHashString *guiName, IHashString *compType);

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	/// Looks for gui element and removes it from the map if found
	/// \param *guiName - IHashString* to the name of the gui element
	/// \param *guiType - IHashString* to the type of the gui element
	/// \return true on removal, else false
	bool RemoveObject(IHashString *guiName, IHashString *guiType);

	/// \brief Set the ActivePage
	/// To disable page, pass NULL instead
	/// \param size - sizeof(IHashString*)
	/// \param *params - pointer to page's IHashString * name
	///					 or NULL to disable page
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnSetActivePage(DWORD size, void *params);

	/// Message to change the GUI elemnt's name
	/// \param size - sizeof(CHANGEOBJNAME)
	/// \param *params - pointer to CHANGEOBJNAME * to set values from
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED,
	///					MSG_ERROR
	DWORD OnChangeObjectName(DWORD size, void *params);	

	/// Message to find an object w/ this name
	/// \param size - sizeof(FINDGUIOBJECT)
	/// \param *params - pointer to FINDGUIOBJECT to set values from
	/// \return DWORD = message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error
	DWORD OnFindObject(DWORD size, void *params);

	// message handler section
	/// Mouse movement messages
	/// \param size - the size of the message
	/// \param params* - pointer to the MOUSECOORDS * containing mouse coordinates
	/// \return DWORD - message reutrn code MSG_HANDLED_PROCEED
	DWORD OnMouseMove(DWORD size, void *params);

	/// left mouse button down message
	/// \param size - the size of the message
	/// \param params* - NULL
	/// \return DWORD - message reutrn code MSG_HANDLED_PROCEED
	DWORD OnLeftMousePress(DWORD size, void *params);

	/// left mouse button down message, with alt key
	/// \param size - the size of the message
	/// \param params* - NULL
	/// \return DWORD - message reutrn code MSG_HANDLED_PROCEED
	DWORD OnLeftMousePressAlt(DWORD size, void *params);
	
	/// left mouse button release message
	/// \param size - the size of the message
	/// \param params* - NULL
	/// \return DWORD - message return code MSG_HANDLED_PROCEED
	DWORD OnLeftMouseRelease(DWORD size, void *params);
	
	/// key press
	/// \param size - the size of the message
	/// \param params* - pointer to a char* of the key pressed
	/// \return DWORD - message reutrn code MSG_HANDLED_PROCEED
	DWORD OnKeyPress(DWORD size, void *params);

	/// key release
	/// \param size - the size of the message
	/// \param params* - pointer to a char* of the key pressed
	/// \return DWORD - message reutrn code MSG_HANDLED_PROCEED
	DWORD OnKeyRelease(DWORD size, void *params);

	/// \brief	Left Key pressed
	DWORD OnKeyLeft(DWORD msgSize, void *param);
	/// \brief	Right Key pressed
	DWORD OnKeyRight(DWORD msgSize, void *param);
	/// \brief	Home Key pressed
	DWORD OnKeyHome(DWORD msgSize, void *param);
	/// \brief	End Key pressed
	DWORD OnKeyEnd(DWORD msgSize, void *param);
	/// \brief	Del Key pressed
	DWORD OnKeyDel(DWORD msgSize, void *param);
	///	\brief	BackSpace Key pressed
	DWORD OnKeyBackSpace(DWORD msgSize, void *param);
	///	\brief	Enter Key pressed
	DWORD OnKeyEnter(DWORD msgSize, void *param);

	/// Sets the resolution the gui system is supposed to render at
	/// \param size = sizeof(GUIWORKINGRES)
	/// \param param = GUIWORKINGRES of resolution
	/// \return MSG_HANDLED_STOP
	DWORD OnSetGUIWorkingRes(DWORD size, void *param);

	/// the pump handler for the GUI Manager
	/// \param size = size of message to send
	/// \param data = pointer to some type of data specific to message
	/// \param name = Hashstring of name of object to send to, NULL means all objects
	/// \param CompType = Hashstring to type of object, name and CompType cannot both be null
	/// \param MsgFunction = precached msg function pointer based on CompType
	DWORD PumpHandler(DWORD size, void *data, 
		IHashString *name, IHashString *CompType, MSGFUNCTION msgFunction);

	///	\brief	Clear the gui manager of all gui objects (gui elements, groups, pages)
	///	\param	size = 0
	///	\param	*param = NULL
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED,
	///					MSG_ERROR
	DWORD OnClearGUIObjects(DWORD size, void *param);

	/// tells the gui manager what size the window is so it can adjust the mouse position 
	/// accordingly
	/// \param size = sizeof SETWINDRECTPARAMS
	/// \param param = SETWINDRECTPARAMS, window rect information
	/// \return MSG_HANDLED_STOP
	DWORD OnSetWindowSize(DWORD size, void *param);

	/// tells the gui manager that we're running in the editor
	/// \param size = sizeof bool
	/// \param param = bool, true if running from editor
	/// \return MSG_HANDLED_STOP
	DWORD OnSetGUIEditorMode(DWORD size, void *param);
	
	/// \brief	Start running the manager
	DWORD OnStart(DWORD size, void *params);
	/// \brief	Stop running the manager
	DWORD OnStop(DWORD size, void *params);

	/// Gets the mouse position in GUI space, depending on offset and zoom
	/// \param size = sizeof MOUSECOORDS
	/// \param params = MOUSECOORDS data; fills the data in this func
	DWORD OnGetGUIMousePosition(DWORD size, void *params);

	/// Gets the zoom factor for the editor
	/// \param size = sizeof float
	/// \param params = float for zoom factor
	DWORD OnGetZoomFactor(DWORD size, void *params);

	/// Sets the zoom factor for the editor
	/// \param size = sizeof float
	/// \param params = float for zoom factor
	DWORD OnSetZoomFactor(DWORD size, void *params);

	/// Sets the x scale for odd aspect ratios
	/// \param size = sizeof float
	/// \param params = float for x scale
	DWORD OnSetXScale(DWORD size, void *params);

	/// Sets the y scale for odd aspect ratios
	/// \param size = sizeof float
	/// \param params = float for y scale
	DWORD OnSetYScale(DWORD size, void *params);

	/// Sets the gui system in active or passive mode (i.e. mouse over and mouse click
	/// enable / disable for elements)
	/// \param size = sizeof bool
	/// \param param = enable / disable
	DWORD OnSetActiveMode(DWORD size, void *param);

	/// Sets the background color
	/// \param size = sizeof DWORD
	/// \param param = DWORD for color
	DWORD OnSetBGColor(DWORD size, void *param);
	
	/// Sets the texture to render to (or render to screen if NULL is passed)
	/// \param size = sizeof(ITextureObject*)
	/// \param param = ITextureObject* to render to
	/// \return MSG_HANDLED_STOP
	DWORD OnSetGUIRenderToTexture(DWORD size, void *param);

	/// Gets the object list from the active page to render
	/// \param size = sizeof(OBJECTLIST)
	/// \param param = OBJECTLIST of objects to render to
	/// \return MSG_HANDLED_STOP
	DWORD OnGetGUIRenderObjects(DWORD size, void *param);

	/// Enables / Disables rendering to general view 
	/// default is false (enabled)
	/// \param size = sizeof(bool)
	/// \param param = bool true / false (true = disable, false = enable)
	DWORD OnDisableBatchRendering(DWORD size, void *param);

	/// Sets a context to render to	
	/// \param size = sizeof(IRenderContext)
	/// \param param = IRenderContext to render to
	DWORD OnRenderPageToContext(DWORD size, void *param);
	
	/// Increments zoom
	/// \param size = 0
	/// \param param = NULL
	DWORD OnIncrementZoom(DWORD size, void *param);

	/// Decrements zoom
	/// \param size = 0
	/// \param param = NULL
	DWORD OnDecrementZoom(DWORD size, void *param);

	/// Sets the page offset
	/// \param size = sizeof(POSITIONDATA)
	/// \param param = POSITIONDATA of offset
	DWORD OnSetPageOffset(DWORD size, void *param);

	/// Checks to see if the manager is empty (in mapping 0)
	/// \size = sizeof(bool)
	/// \param = bool true if empty
	DWORD OnCheckGUISystemEmpty(DWORD size, void *param);

	/// \brief	Sort the gui elements on the active page
	void SortActivePage();

	/// \brief  Set the focused gui element
	void SetFocus(IObject* object);

	/// Updates and redraws the page
	/// \param size = sizeof(LOADINGUPDATEPARAMS)
	/// \param param = LOADINGUPDATEPARAMS with progress info
	/// \return MSG_HANDLED_PROCEED
	DWORD OnLoadingUpdate(DWORD size, void *param);

	/// Sends a state event to the state wrapper so he can fire it off
	/// \param size = sizeof(GUIEVENT)
	/// \param param = GUIEVENT of event name and archive to fire off
	/// \return MSG_HANDLED_STOP
	DWORD OnQueueGUIStateEvent(DWORD size, void *param);

	/// Sends the state wrapper the name of the state machine object that's supposed to handle the message
	/// \param size = sizeof(CHashString)
	/// \param param = CHashString with object name
	/// \return MSG_HANDLED_STOP
	DWORD OnSetGUIStateObject(DWORD size, void *param);

	/// Ticks just the renderer stuffs, such that screens can be redrawn without having to worry about 
	/// ticking all the various systems 
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_PROCEED
	DWORD OnRenderUpdate(DWORD size, void *param);

	/// Gets the screen size as used by the gui
	/// \param size = sizeof(VIEWPORTPARAMS)
	/// \param param = VIEWPORTPARAMS to set width and height
	/// \return MSG_HANDLED_STOP
	DWORD OnGetGUIScreenSize(DWORD size, void *param);

	/// Reads glyph textures from a db and forwards the glyphs to the font manager
	/// \param size = sizeof(IHashString*)
	/// \param param = IHashString* with filename of database
	/// \return MSG_HANDLED_STOP
	DWORD OnSetGUIFontGlyphDB(DWORD size, void *param);

	/// Adds a language to the GUI lang list
	/// \param size = sizeof(ADDGUILANGUAGEPARAMS)
	/// \param param = ADDGUILANGUAGEPARAMS struct
	/// \return MSG_HANDLED_PROCEED
	DWORD OnAddGUILanguage(DWORD size, void *param);

	/// Removes a language from the GUI lang list
	/// \param size = sizeof(IHashString*)
	/// \param param = IHashString name of language to remove
	/// \return MSG_HANDLED_PROCEED
	DWORD OnRemoveGUILanguage(DWORD size, void *param);

	/// Sets the active gui language
	/// \param size = sizeof(IHashString*)
	/// \param param = IHashString name identifier of language you want to set
	/// \return MSG_HANDLED_PROCEED
	DWORD OnSetActiveGUILanguage(DWORD size, void *param);

	/// Gets the language map
	/// \param size = sizeof(LANGUAGEFILEMAP)
	/// \param param = LANGUAGEFILEMAP map of languages
	/// \return MSG_HANDLED_STOP
	DWORD OnGetGUILanguages(DWORD size, void *param);

	/// Gets the size the gui is rendering at
	/// \param width = output width
	/// \param height = output height
	void GetScreenSize(int *width, int *height)
	{
		if ((m_bEditorMode) ||(m_iResX == 0) || (m_iResY ==0))
		{
			*width = m_iScreenSizeX;
			*height = m_iScreenSizeY;
		}
		else
		{
			*width = m_iResX;
			*height = m_iResY;
		}
	}

	/// Static callback fn for prerendering, defers to below fn
	static void PreRenderWrap(); 

	/// Static callback fn for postrendering, defers to below fn
	static void PostRenderWrap();

	/// Deferred callback fn for prerendering
	void PreRender();

	/// Deferred callback fn for postrendering
	void PostRender();
	
private:

	// adjusts zoom variable with preset values
	void IncrementZoom();
	void DecrementZoom();
	Vec3 GetScreenCornerGUIPos();
	void CenterPage();
	void CreateVertexBuffer();

	void FlagObjectsUpdateLanguageEntry();

	CGUIPage*		m_ActivePage;
	GUIMAP			m_GUIObjects;

    static int			m_scaleGUIX;
	static int			m_scaleGUIY;	

	// pointer to the gui element that has focus
	IObject *m_pFocusItem;

	/// Boolean controling if the simulation is active or not
	bool m_IsRunning;

	int m_iScreenOffsetX, m_iScreenOffsetY;
	int m_iScreenSizeX, m_iScreenSizeY;
	int m_iResX, m_iResY;

	// true if running in gui editor mode
	bool m_bEditorMode;
	
	// simple key pressed bools
	bool m_bSpacePressed;
	bool m_bCtrlPressed;
	bool m_bShiftPressed;
	bool m_bLeftMousePressed;

	float m_fScaleFactorX;
	float m_fScaleFactorY;
	
	// zoom factor
	float m_fZoomTable[10];
	unsigned int m_iZoomIndex;
	float m_fZoomFactor;

	int m_iPageOffsetX, m_iPageOffsetY;
	int m_iMouseLastPosX, m_iMouseLastPosY;

	IRenderContext* m_RenderContext;

	bool m_bActiveMode;
	bool m_bTickStateWrapper;

	DWORD m_iBGColor;

	lua_State* m_LuaState;

	CGUIStateWrapper m_StateWrapper;

	vector <CGUIAnimation*> m_Animations;

	IVertexBufferObject *			m_VertexBuffer;
	BUFFERALLOCATIONSTRUCT			m_VertexBufferAllocation;

	IRenderer *m_Renderer;

	CHashString m_hsTextGlyphDBFile;

	LANGUAGEFILEMAP m_mLanguages;
};

#endif	// CGUIManager_H
