///============================================================================
/// \file		CLayeredMask.h
/// \brief		Header file for ITextureObject wrapper for gui elements
/// \date		11/13/2006
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

#ifndef _C_LAYEREDMASK_H_
#define _C_LAYEREDMASK_H_



class CLayeredMask
{
public:
	/// ctor, dtor
	CLayeredMask();
	~CLayeredMask();

	/// Loads the texture
	/// \param filename = filename of texture to load
	void LoadTexture(StdString &filename);

	/// Loads the vertex data into the provided array
	/// \param vertexData = array to load data into
	/// \param x, y = position of element
	/// \param width, height = size of element
	/// \param angle = angle from center of rotation, in degrees
	/// \param color = color multiplier of element
	/// \param smartScaled = t/f run smart scaling logic
	/// \param uStart, vStart = uv coordinates for upper left
	/// \param uEnd, vEnd = uv coordinates for lower right
	/// \return number of primitives written
	UINT LoadVertexData(GUIVERTEX vertexData[], float x, float y, float width, float height, float angle, 
		DWORD color, bool smartScaled, float uStart=0.0f, float vStart=0.0f, float uEnd=1.0f, float vEnd=1.0f);

	/// Gets the rendering texture
	/// \return mask
	IBaseTextureObject* GetMask();	
private:
	/// Fills a quad of vertex data
	/// \param vertexData = array to fill
	/// \param startIndex = position in array to start at
	/// \param x, y = position of quad
	/// \param width, height = size of quad
	/// \param uStart, vStart = uv coordinates for upper left
	/// \param uEnd, vEnd = uv coordinates for lower right
	/// \param angle = angle of rotation from center, in degrees
	/// \param color = color multiplier
	/// \param centerX, centerY = position of center of entire instance (used if rotating a smart scaled object)
	void SetQuad(GUIVERTEX vertexData[], UINT startIndex, float x, float y, float width, float height, 
		float uStart, float vStart, float uEnd, float vEnd, float angle, DWORD color, float centerX, float centerY);

	/// Gets the sub texture of the mask (typically for animated textures)
	/// \return sub tex
	IBaseTextureObject* GetSubTex();

	IBaseTextureObject *m_pMask;
	StdString m_szTexFile;
	float m_fStartTime;
	bool m_bIsAnimatedTexture;
};

#endif