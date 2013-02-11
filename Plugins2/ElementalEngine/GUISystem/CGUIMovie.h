///============================================================================
/// \file		CGUIMovie.h
/// \brief		Header file for GUI movie interface
/// \date		12/21/2007
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

#ifndef _C_GUIMOVIE_H_
#define _C_GUIMOVIE_H_

#include "GUIMovieSub.h"

class CGUIMovie : public CObjectTemplate<IGUIReference>
{
private:
	/// Constructor
	/// \param parentName = name of parent in hierarchy
 	/// \param name = IHashString * name of this object
 	CGUIMovie(IHashString *parentName, IHashString *name);

public:
	/// dtor
	virtual ~CGUIMovie();

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	/// standard static component create function
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Update Function for the CGUIStatusBar
	/// \return	True on sucess, False on faulure
	virtual bool Update();

	virtual IHashString* GetComponentType();

	virtual bool IsKindOf(IHashString *compType);

	/// Gets rendering information for a vertex buffer given the dimensions and state of the element
	/// Returns a vector since reference element can be a composite
	/// \param vertexData = data of verts, textures, and effects
	/// \param x, y = position of element
	/// \param width, height = size of element
	/// \param state = state of object
	/// \param angle = angle element is to be rendered at
	/// \param color = color of instance (assumed to be ARGB)		
	virtual void GetRenderParams(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, DWORD state, float angle=0.0f, DWORD color=-1);

	/// Gets font information given the dimensions and state of the element
	/// Returns a vector to allow for composite elements (with more than one font)
	/// \param fontData = data of font stuffs, text, etc
	/// \param x, y = position of element
	/// \param width, height = size of element
	/// \param state = state of object
	/// \param angle = angle element is to be rendered at
	/// \param color = color of instance (assumed to be ARGB)		
	virtual void GetFontParams(vector<GUIFONTINFO> &fontData, float x, float y, float width, float height, DWORD state, float angle=0.0f, DWORD color=-1){}

	/// Checks to see if the size provided is ok
	/// This func is used for sliders, combo boxes, and spinners, where there is a set width (or height)
	/// that the object shouldn't render beyond
	/// \param width = width "request", adjusted value returned
	/// \param height = height "request", adjusted value returned
	virtual void CheckSize(float *width, float *height){}

	/// Checks the mouse position to see if we change state given dimensions of instance
	/// \param mouseX, mouseY = mouse position
	/// \param mouseState = state of mouse (mouse over, pressed, released)
	/// \param x, y, width, height = dimensions of instance
	/// \param dwLastState = last state of element
	/// \return state of element
	virtual DWORD CheckMouseState(float mouseX, float mouseY, MOUSESTATE mouseState, float x, float y, 
		float width, float height, DWORD dwLastState){ return dwLastState; }

	/// Checks a key press given dimensions of instance
	/// \param size = sizeof(char)
	/// \param param = char of key pressed
	/// \return standard message return
	virtual DWORD CheckKeyPress(DWORD size, void *param){ return MSG_NOT_HANDLED; }

	/// Adjusts state given a 'key up' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleUp(DWORD dwLastState){ return dwLastState; }

	/// Adjusts state given a 'key down' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleDown(DWORD dwLastState){ return dwLastState; }

	/// Adjusts state given a 'key select' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleSelect(DWORD dwLastState){ return dwLastState; }

	/// Adjusts state given a 'key back' command
	/// \param dwLastState = last state passed in
	/// \return new state
	virtual DWORD ToggleBack(DWORD dwLastState){ return dwLastState; }

	/// Gets a hashval representing the current 'text' of the element, if applicable
	/// \param inState = current state of element
	/// \return hashstring hash val of string, or 0 if not applicable
	virtual DWORD GetHashText(DWORD inState){ return inState; }

	/// Sets a hashval representing the current 'text' of the element, if applicable
	/// \param inState = current state of element
	/// \param hashText = text to set
	/// \return new state
	virtual DWORD SetHashText(DWORD inState, DWORD hashText){ return inState; }

	/// Flags the ref to update its text if using a language entry
	virtual void FlagLanguageEntryChanged(){};

	/// *** Messages *** ///

	/// Plays the movie
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_STOP
	DWORD OnStartGUIMovie(DWORD size, void *param);

	/// Stops the movie
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_STOP
	DWORD OnStopGUIMovie(DWORD size, void *param);

private:

	void CreateSubItem();

	CHashString m_hsCompType;
	StdString m_szMovieFileName;
	StdString m_szIdleTexture;

	GUIMovieSub m_MovieSub;

	StdString m_szMovieCompleteEvent;

	bool m_bMoviePlaying; 
};

#endif //_C_GUIMOVIE_H_