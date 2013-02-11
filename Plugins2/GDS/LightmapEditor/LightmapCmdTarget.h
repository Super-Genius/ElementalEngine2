///============================================================================
/// \file		LightmapCmdTarget.h
/// \brief		Header file for <insert brief description here>
/// \date		01-29-2007
/// \author		Marvin Gouw
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

#ifndef _LightmapCMDTARGET_H_
#define _LightmapCMDTARGET_H_

// CLightmapCmdTarget command target

class CLightmapCmdTarget : public CCmdTarget
{
	DECLARE_DYNAMIC(CLightmapCmdTarget)

public:
	CLightmapCmdTarget();
	virtual ~CLightmapCmdTarget();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRenderLightmap();
	afx_msg void OnUpdateRenderLightmap(CCmdUI *pCmdUI);
private:
	bool m_bLightmapMode;
	IToolBox *m_ToolBox;
	CLightMapEditorDialog	m_LightmapDialog;
};



#endif // #ifndef _LightmapCMDTARGET_H_
