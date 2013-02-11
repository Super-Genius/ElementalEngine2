///==========================================================================
/// \file	GUIFPSDisplay.cpp
/// \brief	FPS display for FlackSquad
/// \date	09/12/08
/// \author Zsolt Matyas
///==========================================================================

#include "StdAfx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/// The default values for the base class' constructor:
static CHashString objectInstanceName = CHashString( _T ("CGUIFPSDisplay" ) );
static CHashString objectParentName = CHashString( _T ("CGUIFPSDisplay" ) );

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON( CGUIFPSDisplay );

// Message handler to return the current FramePerSecond 
REGISTER_MESSAGE_HANDLER( GetFrameRate, OnGetFrameRate, CGUIFPSDisplay ); 


// the usual singleton instance creation
IComponent *CGUIFPSDisplay::Create( int nArgs, va_list argptr )
{
	return SINGLETONINSTANCE( CGUIFPSDisplay );
}

// The Singleton creation requires to call the default constructor without 
// any parameter. The IGUIReference interface forces us to call the
// constructor of the base class with two parameters. 
// To fit both requirements: create constuctor with two parameters to call
// the base class' constructor that way, but add default values to be able
// to call the derived class's costructor without any parameters.
// Ctor
CGUIFPSDisplay::CGUIFPSDisplay() :
	OBJECTTEMPLATE(CGUIManager, CGUIFPSDisplay, IGUIReference, &objectParentName, &objectInstanceName )
{
	//Debug Log 
	//m_ToolBox->Log( LOGWARNING, _T( "CGUIFPSDisplay - constructor called as registered singletonInstance\n" ) );

#ifndef USE_TIMER_INFORMATION_ONLY	
	GETRENDERERINTERFACEPARAMS renderInterface;

	static DWORD msgGetRendererInterface = CHashString( _T("GetRendererInterface") ).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgGetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface, NULL, NULL);

	m_Renderer = renderInterface.m_RendererInterface;
#endif
}

//Dtor
CGUIFPSDisplay::~CGUIFPSDisplay()
{}

DWORD CGUIFPSDisplay::OnGetFrameRate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float FPS; 
 	
#ifndef USE_TIMER_INFORMATION_ONLY	
	RENDERERPROFILEINFO info;
	RENDERERPROFILEINFO* p_info = &info;
	m_Renderer->GetFrameProfileInfo( p_info );
	if ( p_info->FrameRate ) 
	{
		FPS = p_info->FrameRate;
		//TODO: Get more detailed information from the renderer
	}
#else
	// Getting FPS from the timer
	FPS  = m_Timer->GetFPS();
#undef USE_TIMER_INFORMATION_ONLY
#endif

	//Debug Log 
	//m_ToolBox->Log( LOGWARNING, _T( "CGUIFPSDisplay - FrameRate: %f.\n" ), FPS );

	// return the FPS as output parameter
	float* ret = (float*)param;
	*ret = FPS;

	return MSG_HANDLED_STOP;
}
