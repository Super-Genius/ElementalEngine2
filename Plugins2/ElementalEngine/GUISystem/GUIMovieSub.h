///============================================================================
/// \file		GUIMovieSub.h
/// \brief		Header file for GUI movie sub component
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

#ifndef _GUIMOVIESUB_H_
#define _GUIMOVIESUB_H_

class GUIMovieSub
{
public:
	/// ctor, dtor
	GUIMovieSub();
	~GUIMovieSub();

	/// Fills vertex buffer data
	/// \param vertexData = vector of triangle data to fill
	/// \param x, y = position of movie sub element
	/// \param width, height = size of movie sub element
	/// \param state = current instance state
	/// \param angle = angle of instance about center, in degrees
	/// \param color = color of instance, multiplied
	void LoadVertexBuffer(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, DWORD state, 
												float angle, DWORD color);

	/// Sets the texture files for the movie
	/// \param szIdleTexture = texture to display when movie is not playing
	/// \param szMovieFile = movie file to load
	void SetTextures(StdString szIdleTexture, StdString szMovieFile);

	/// Updates the movie element
	/// \return true if no errors
	bool Update();

	/// Plays the movie
	void PlayMovie();

	/// Stops the movie
	void StopMovie();

private:

	CLayeredMask m_IdleMask;
	IBaseTextureObject *m_pMovieTexture;
	StdString m_szMovieFileName;
};

#endif //_GUIMOVIESUB_H_