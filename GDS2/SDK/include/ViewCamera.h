///============================================================================
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

#ifndef _VIEWCAMERA_H_
#define _VIEWCAMERA_H_

class CViewCamera
{
public:
	CViewCamera();
	virtual ~CViewCamera();

	LPCTSTR GetCameraName() const;
	void SetCameraName( const TCHAR *szCameraName );
	void SetMoveSensitivity( float fMoveSensitivity );
	void SetRotateSensitivity( float fRotateSensitivity );
	virtual void MoveForward( float fDistance );
	virtual void MoveRight( float fDistance );
	virtual void MoveUp( float fDistance );
	virtual void Rotate( float fPitch, float fYaw );
	virtual void StartMouseMove( CPoint point );
	virtual void StopMouseMove();
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );

	void AddFollowingObject( DWORD objectName );
	void RemoveFollowingObject( DWORD objectName );

	///	\brief	create new camera with auto-generated unique name
	///	\param	pSceneName - name of scene
	///	\return	return true if new camera was created successfully
	bool CreateCamera(IHashString *pSceneName);
	
	///	\brief	delete auto-generated camera
	void DeleteCamera();

protected:
	void UpdateFollowers();

private:
	CHashString m_hszCameraType;
	IToolBox *m_ToolBox;
	CHashString m_hszCameraName;
	float m_fMoveSensitivity;
	float m_fRotateSensitivity;
	float m_fCameraPitch;
	float m_fCameraYaw;

	CPoint m_OldMousePos;
	CPoint m_DragStartPoint;
	bool m_bMouseMove;

	set<DWORD> m_Followers;
	bool	m_bAutocreatedCamera;
};

#endif // _VIEWCAMERA_H_