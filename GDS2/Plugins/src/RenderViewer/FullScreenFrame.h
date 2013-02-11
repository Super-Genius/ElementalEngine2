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

#pragma once
#ifndef FULLSCREENFRAME_H__
#define FULLSCREENFRAME_H__

// CFullScreenFrame frame

class CFullScreenFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CFullScreenFrame)
public:
	CFullScreenFrame();           // protected constructor used by dynamic creation
	virtual ~CFullScreenFrame();

	void SetupInput();
protected:
	DECLARE_MESSAGE_MAP()

	IToolBox *m_ToolBox;
	IHashString *m_StopMessage;
};


#endif // FULLSCREENFRAME_H__