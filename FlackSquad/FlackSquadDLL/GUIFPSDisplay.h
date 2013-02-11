///==========================================================================
/// \file	GUIFPSDisplay.h
/// \brief	FPS display for FlackSquad
/// \date	09/12/08
/// \author Zsolt Matyas
///==========================================================================

#include "IGUIElement.h"

///==========================================================================
/// \class	CGUIFPSDisplay
/// \brief	FPS display implementation of IGUIElement Inteface
///==========================================================================
class CGUIFPSDisplay : public CObjectTemplate<IGUIReference>
{
protected:
	SINGLETONCONSTRUCTOROVERRIDE(CGUIFPSDisplay);
	/// \brief Constructor for CGUIFPSDisplay singleton
	CGUIFPSDisplay();
public:
	virtual ~CGUIFPSDisplay();

	/// \brief	Create an instance of GUIElement
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);
	
	/// \brief	Message handler to return the current framrate
	/// \param param	Returns the FPS in this pointer.
	DWORD OnGetFrameRate(DWORD size, void *param);

	/// Has no useful implementation, only required because of the Inteface. Check it for more details.
	virtual void GetRenderParams(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, DWORD state, float angle=0.0f, DWORD color=-1) {};
	/// Has no useful implementation, only required because of the Inteface. Check it for more details.
	virtual void GetFontParams(vector<GUIFONTINFO> &fontData, float x, float y, float width, float height, DWORD state, float angle=0.0f, DWORD color=-1) {};
	/// Has no useful implementation, only required because of the Inteface. Check it for more details.
	virtual void CheckSize(float *width, float *height) {};
	/// Has no useful implementation, only required because of the Inteface. Check it for more details.
	virtual DWORD CheckMouseState(float mouseX, float mouseY, MOUSESTATE mouseState, float x, float y, 
		float width, float height, DWORD dwLastState) { return 0; }
	/// Has no useful implementation, only required because of the Inteface. Check it for more details.
	virtual DWORD CheckKeyPress(DWORD size, void *param){ return 0; }
	/// Has no useful implementation, only required because of the Inteface. Check it for more details.
	virtual DWORD ToggleUp(DWORD dwLastState) { return 0; }
	/// Has no useful implementation, only required because of the Inteface. Check it for more details.
	virtual DWORD ToggleDown(DWORD dwLastState){ return 0; }
	/// Has no useful implementation, only required because of the Inteface. Check it for more details.
	virtual DWORD ToggleSelect(DWORD dwLastState){ return 0; }
	/// Has no useful implementation, only required because of the Inteface. Check it for more details.
	virtual DWORD ToggleBack(DWORD dwLastState){ return 0; }
	/// Has no useful implementation, only required because of the Inteface. Check it for more details.
	virtual DWORD GetHashText(DWORD inState){ return 0; }
	/// Has no useful implementation, only required because of the Inteface. Check it for more details.
	virtual DWORD SetHashText(DWORD inState, DWORD hashText){ return 0; }
	/// Has no useful implementation, only required because of the Inteface. Check it for more details.
	virtual void FlagLanguageEntryChanged(){}

#define	USE_TIMER_INFORMATION_ONLY

#ifndef USE_TIMER_INFORMATION_ONLY
protected:
	/// Once the DX9renderer getRenderIno function is called, m_Renderer can
	/// be used to have detailed information about the rendering
	IRenderer *m_Renderer;
#endif	
};


