///============================================================================
/// \file		RenderContext.h
/// \brief		Helper wrapper class for for interaction with EE render context.
/// \date		09-26-2007
/// \author		Andrey Ivanov
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
///============================================================================

#ifndef _GDS_RENDER_CONTEXT_H_
#define _GDS_RENDER_CONTEXT_H_

class CViewCamera;

class CRenderContext
{
public:
	CRenderContext();
	~CRenderContext();

	///	\brief	Create render context
	/// \param  hWnd - handle of window for rendering
	/// \param  fullscreen - rendering mode for window
	///	\return	true if render context was created
	bool Create(HWND hWnd, bool fullscreen);

	///	\brief	Destroy render context
	void Destroy();

	///	\brief	Provide access to raw IRenderContext pointer
	///	\return	pointer to created IRenderContext pointer
	IRenderContext *GetRenderContext();

	/// \brief	Set scene render context
	/// \param *pSceneName - ptr to the IHashString of scene name
	///	\return	true if scene was set
	bool SetScene(IHashString *pSceneName);

	/// \brief	Set active camera in this context
	/// \param *pCameraName - ptr to the IHashString of camera name
	///	\return	true if active camera was set
	bool SetActiveCamera(IHashString *pCameraName);

	/// \brief	Set active camera in this context
	/// \param  viewCamera - reference to valid CViewCamera object
	///	\return	true if active camera was set
	bool SetActiveCamera(const CViewCamera &viewCamera);

	///	\brief	Set camera position in context
	/// \param  position - new camera position
	void SetCameraPosition(Vec3 &position);
	
	///	\brief	Set camera orientation in context
	/// \param  orientation - new camera orientation
	void SetCameraOrientation(EulerAngle &orientation);

	///	\brief	draw current render content
	/// \param  orientation - new camera orientation
	void Draw();
	
	///	\brief	set new view port size
	/// \param  cx - new view width
	/// \param  cy - new view height
	void SetWindowSize(int cx, int cy);

private:
	IToolBox	   *m_pToolBox;
	IRenderContext *m_pRenderContext;
	UINT			m_uiRenderContextID;
};

#endif //_GDS_RENDER_CONTEXT_H_