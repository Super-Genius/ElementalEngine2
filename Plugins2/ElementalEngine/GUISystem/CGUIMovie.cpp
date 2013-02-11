///============================================================================
/// \file		CGUIMovie.cpp
/// \brief		Implementation for GUI movie interface
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

#include "stdafx.h"
#include "CGUIMovie.h"

REGISTER_COMPONENT(CGUIMovie);
REGISTER_MESSAGE_HANDLER(StartGUIMovie, OnStartGUIMovie, CGUIMovie);
REGISTER_MESSAGE_HANDLER(StopGUIMovie, OnStopGUIMovie, CGUIMovie);

CGUIMovie::CGUIMovie(IHashString *parentName, IHashString *name):
 	OBJECTTEMPLATE(CGUIManager, CGUIMovie, IGUIReference, parentName, name)
{
	m_hsCompType = _T("CGUIMovie");
	m_szMovieFileName = _T("");
	m_szIdleTexture = _T("");
	m_szMovieCompleteEvent = _T("");
}

CGUIMovie::~CGUIMovie()
{}

void CGUIMovie::Serialize(IArchive &ar)
{
	float fSaveVersion = 1.1f;
	if (ar.IsReading())
	{
		float fReadVersion = 0.0f;
		ar.Read(fReadVersion, _T("Version"));
		if (fReadVersion == 0.0f)
		{
			// hidden value; set to save version since schema must be up to date
			fReadVersion = fSaveVersion;
		}

		ar.Read(m_szIdleTexture, _T("IdleTextureFile"));
		ar.Read(m_szMovieFileName, _T("MovieFile"));

		bool bPlayMovie = false;
		ar.Read(bPlayMovie, _T("EditorPlayMovie"));

		if (fReadVersion >= 1.1f)
		{
			ar.Read(m_szMovieCompleteEvent, _T("MovieCompleteEvent"));
		}

		CreateSubItem();

		if (bPlayMovie)
		{
			m_bMoviePlaying = true;
			m_MovieSub.PlayMovie();
		}
 	}
	else
	{
		ar.Write(fSaveVersion, _T("Version"));
		ar.Write(m_szIdleTexture, _T("IdleTextureFile"));
		ar.Write(m_szMovieFileName, _T("MovieFile"));
		ar.Write(false, _T("EditorPlayMovie"));
		ar.Write(m_szMovieCompleteEvent, _T("MovieCompleteEvent"));
	}
}

void CGUIMovie::CreateSubItem()
{
	m_MovieSub.SetTextures(m_szIdleTexture, m_szMovieFileName);
}

IComponent* CGUIMovie::Create(int nArgs, va_list argptr)
{
	IHashString *name;
 	IHashString *parentName;
   	IObject *self;
 
 	name = va_arg(argptr, IHashString *);
 	parentName = va_arg(argptr, IHashString *);
   	// create the object however you must create it
   	try
   	{
   		// try and create an instance
 		self = new CGUIMovie(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

bool CGUIMovie::Update()
{
	bool ret = m_MovieSub.Update();
	if ((m_bMoviePlaying) && (!ret))
	{
		m_bMoviePlaying = false;
	
		if (_tcscmp(m_szMovieCompleteEvent, _T("")) != 0)
		{
			CHashString hsEvent(m_szMovieCompleteEvent);

			GUIEVENT ge;
			ge.ar = NULL;
			ge.event = hsEvent.GetUniqueID();

			static DWORD msgQueueGUIStateEvent = CHashString(_T("QueueGUIStateEvent")).GetUniqueID();
			m_ToolBox->SendMessage(msgQueueGUIStateEvent, sizeof(GUIEVENT), &ge);
		}
	}

	return true;
}

IHashString* CGUIMovie::GetComponentType()
{
	return &m_hsCompType;
}

bool CGUIMovie::IsKindOf(IHashString *compType)
{
	if (compType)
	{
		if (compType->GetUniqueID() == m_hsCompType.GetUniqueID())
		{
			return true;
		}
	}
	return false;
}

void CGUIMovie::GetRenderParams(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, DWORD state, float angle, DWORD color)
{
	m_MovieSub.LoadVertexBuffer(vertexData, x, y, width, height, state, angle, color);
}

DWORD CGUIMovie::OnStartGUIMovie(DWORD size, void *param)
{
	m_bMoviePlaying = true;
	m_MovieSub.PlayMovie();
	return MSG_HANDLED_STOP;
}

DWORD CGUIMovie::OnStopGUIMovie(DWORD size, void *param)
{
	m_bMoviePlaying = false;
	m_MovieSub.StopMovie();
	return MSG_HANDLED_STOP;
}