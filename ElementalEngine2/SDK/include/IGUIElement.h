///============================================================================
/// \file		IGUIElement.h
/// \brief		Header file for GUI Element Interface
/// \date		05-06-2005
/// \author		Josh Lulewicz
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

#ifndef IGUIELEMENT_H
#define IGUIELEMENT_H

namespace ElementalEngine
{

/// \interface IGUIElement
/// \brief Interface for a GUI Element
class IGUIElement : public IRenderObject
{
public:
	virtual ~IGUIElement() {};
	/// Determine if the cursor is over a gui element
	/// \param fCursorPosX = the x pos of the cursor
	/// \param fCursorPosY = the y pos of the cursor
	/// \return bool = is cursor over this element?
	virtual bool IsCursorOver(float fCursorPosX, float fCursorPosY) = 0;

	/// Get the Z order value
	/// \return int = z order vaule
	virtual int GetZOrder() = 0;

	/// Gets a left mouse button press message
	/// \param msgSize = size of param
	/// \param param = pointer to float *
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnLeftMousePress(DWORD msgSize, void *param) = 0;

	/// Gets a left mouse button release message
	/// \param msgSize = size of param
	/// \param param = pointer to float *
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnLeftMouseRelease(DWORD msgSize, void *param) = 0;

	/// Gets a mouse position change message
	/// \param msgSize = size of param
	/// \param param = pointer to float *
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnMousePosition(DWORD msgSize, void *param) = 0;

	/// Gets a keyboard press
	/// \param msgSize = size of param
	/// \param param = pointer to float *
	/// \return DWORD = message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error
	virtual DWORD OnKeyPress(DWORD msgSize, void *param) = 0;

	/// Sets width factor for odd window aspect ratio adjustment
	/// \param value - value to set ratio to
	virtual void SetWidthRatio(float value) = 0;

	/// Sets height factor for odd window aspect ratio adjustment
	/// \param value - value to set ratio to
	virtual void SetHeightRatio(float value) = 0;

	/// Sets zoom factor for zooming in (used in editor)
	/// \param value - value to set zoom to
	virtual void SetZoomFactor(float value) = 0;

	/// Sets page offset for rendering away from 0,0 (used in editor)
	/// \param xOff - x offset
	/// \param yOff - y offset
	virtual void SetPageOffset(int xOff, int yOff) = 0;

	/// Checks to see if we want to serialize this object or not
	/// \return true if we want to serialize out, false otherwise
	virtual bool ToSerializeOut() = 0;

	/// Loads the gui manager buffer up with vertex data
	/// \param pDest = buffer to copy into
	/// \param offset = initial offset
	/// \param vb = vertex buffer object where this data is stored
	/// \return number of verts written
	virtual UINT UpdateBuffer( void **pDest, UINT offset, IVertexBufferObject *vb) = 0;	

};

}		//namespace ElementalEngine

#endif // IGUIELEMENT_H