///============================================================================
/// \file		GUIStatusBar.hpp
/// \brief		Low level status bar, since it needs specialized render calls
/// \date		10/30/2006
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

#ifndef _GUISTATUSBAR_H_
#define _GUISTATUSBAR_H_

class GUIStatusBar
{
public:
	/// ctor, dtor
	GUIStatusBar();
	~GUIStatusBar();
	
	/// Fills vertex buffer data
	/// \param vertexData = vector of triangle data to fill
	/// \param x, y = position of spinner
	/// \param width, height = size of spinner
	/// \param percentage = current percent position of bar
	/// \param leftOffset = offset of bar from left side of backplate
	/// \param topOffset = offset of bar from top side of backplate
	/// \param rightOffset = offset of bar from right side of backplate
	/// \param bottomOffset = offset of bar from bottom side of backplate
	/// \param forwardTexture = t/f uv of bar maps from head of bar vs tail
	/// \param angle = angle of instance about center, in degrees
	/// \param color = color of instance, multiplied
	/// \param lastPercentage = last updated percentage(used for interpolation on delayed bar)
	/// \param timePosition = current position [0,1] between last percentage and current percentage
	void LoadVertexBuffer(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, float percentage,
		int leftOffset, int topOffset, int rightOffset, int bottomOffset, bool forwardTexture, float angle, DWORD color,
		float lastPercentage=0.0f, float timePosition=1.0f);

	/// Sets the textures to use when rendering
	/// \param bgTex = background texture to render
	/// \param barTex = bar texture to render
	/// \param two/three/fourHndTex = textures to render beyond 100% (energy bars, etc)
	void SetTextures(StdString bgTex, StdString barTex, StdString twoHndTex, StdString threeHndTex, StdString fourHndTex);

	/// Sets the texture for the ghost animation thing, if specified
	/// \param ghostTex = texture filepath for ghost tex effect
	void SetGhostTexture(StdString ghostTex);

	/// Interpolates between last percentage and current percentage via square interpolation, given time factor
	/// \param lastPercentage = last percentage to interp
	/// \param currentPercentage = current percentage to interp
	/// \param time = time factor [0,1]
	/// \return current interpolated value
	float CalcSquaredPercentage(float lastPercent, float currentPercent, float time);

private:
	
	/// textures
	CLayeredMask m_BGTex;
	CLayeredMask m_BarTex;
	CLayeredMask m_TwoHundTex;
	CLayeredMask m_ThreeHundTex;
	CLayeredMask m_FourHundTex;
	CLayeredMask m_GhostTex;

	IEffect *m_pEffect;
};

#endif