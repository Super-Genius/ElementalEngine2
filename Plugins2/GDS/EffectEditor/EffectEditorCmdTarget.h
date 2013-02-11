///============================================================================
/// \file		WorldEventCmdTarget.h
/// \brief		Header file for <insert brief description here>
/// \date		01-20-2007
/// \author		Brian Bazyk
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

#ifndef _WORLDEVENTCMDTARGET_H_
#define _WORLDEVENTCMDTARGET_H_

// CEffectEditorCmdTarget command target

class CEffectEditor;

class CEffectEditorCmdTarget : public CCmdTarget
{
	DECLARE_DYNAMIC(CEffectEditorCmdTarget)

public:
	CEffectEditorCmdTarget();
	virtual ~CEffectEditorCmdTarget();

protected:
	CEffectEditor *m_pEditor;
	bool m_bEditorStarted;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnToolsEffecteditor();
	afx_msg void OnUpdateToolsEffecteditor(CCmdUI *pCmdUI);
};

#endif