///============================================================================
/// \file		GUIMovieSub.cpp
/// \brief		Implementation for GUI movie sub component
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
#include "GUIMovieSub.h"

GUIMovieSub::GUIMovieSub()
{
	m_pMovieTexture = NULL;
}

GUIMovieSub::~GUIMovieSub()
{
	if (m_pMovieTexture)
	{
		// TODO: remove texture...? maybe not? maybe the movie player takes care of it?
		m_pMovieTexture = NULL;
	}
}

bool GUIMovieSub::Update()
{
	if (m_pMovieTexture)
	{
		static DWORD msgIsMovieDone = CHashString(_T("IsMovieDone")).GetUniqueID();
		bool bMovieDone = false;
		EngineGetToolBox()->SendMessage(msgIsMovieDone, sizeof(bool), &bMovieDone);
		if (bMovieDone)
		{
			// deregister with texture manager
			TEXTUREOBJECTPARAMS top;
			top.bLoad = false;
			top.Name = m_pMovieTexture->GetName();
			top.TextureObjectInterface = m_pMovieTexture;

			static DWORD msgRemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
			EngineGetToolBox()->SendMessage(msgRemoveTexture, sizeof(TEXTUREOBJECTPARAMS), &top);

			m_pMovieTexture = NULL;
			return false;
		}
		return true;
	}
	return false;
}

void GUIMovieSub::LoadVertexBuffer(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, DWORD state, 
												float angle, DWORD color)
{
	GUIVERTEXINFO entry;
	entry.iNumPrims = m_IdleMask.LoadVertexData(entry.VertexData, x, y, width, height, 
		angle, color, false);

	if (m_pMovieTexture)
	{
		entry.pTexture = m_pMovieTexture;
	}
	else
	{
		entry.pTexture = m_IdleMask.GetMask();
	}

	vertexData.push_back(entry);	
}

void GUIMovieSub::SetTextures(StdString szIdleTexture, StdString szMovieFile)
{
	if (_tcscmp(szIdleTexture, _T("")) != 0)
	{
		m_IdleMask.LoadTexture(szIdleTexture);
	}

	if (_tcscmp(szMovieFile, _T("")) != 0)
	{
		m_szMovieFileName = szMovieFile;
	}
}

void GUIMovieSub::PlayMovie()
{
	if (_tcscmp(m_szMovieFileName, _T("")) != 0)
	{
		CHashString hsMovieFile(m_szMovieFileName);
		PLAYMOVIEPARAMS pmp;
		pmp.in_movieFileName = &hsMovieFile;		
		
		static DWORD msgPlayMovie = CHashString(_T("PlayMovie")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgPlayMovie, sizeof(PLAYMOVIEPARAMS), &pmp);
		if (pmp.out_pTargetTexture != NULL)
		{
			m_pMovieTexture = pmp.out_pTargetTexture;
		}

		if (m_pMovieTexture)
		{
			// now register with texture manager
			TEXTUREOBJECTPARAMS top;
			top.bLoad = false;
			top.Name = m_pMovieTexture->GetName();
			top.TextureObjectInterface = m_pMovieTexture;

			static DWORD msgAddTexture = CHashString(_T("AddTexture")).GetUniqueID();
			EngineGetToolBox()->SendMessage(msgAddTexture, sizeof(TEXTUREOBJECTPARAMS), &top);
		}
	}
}

void GUIMovieSub::StopMovie()
{
	if (m_pMovieTexture)
	{
		static DWORD msgStopMovie = CHashString(_T("StopMovie")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgStopMovie, 0, NULL);
		
		// deregister with texture manager
		TEXTUREOBJECTPARAMS top;
		top.bLoad = false;
		top.Name = m_pMovieTexture->GetName();
		top.TextureObjectInterface = m_pMovieTexture;

		static DWORD msgRemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgRemoveTexture, sizeof(TEXTUREOBJECTPARAMS), &top);

		m_pMovieTexture = NULL;
	}
}