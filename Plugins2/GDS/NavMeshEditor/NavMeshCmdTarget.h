///============================================================================
/// \file		NavMeshCmdTarget.h
/// \brief		Header file for <insert brief description here>
/// \date		01-29-2007
/// \author		Kenneth Hurley
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

#ifndef _NAVMESHCMDTARGET_H_
#define _NAVMESHCMDTARGET_H_

// CNavMeshCmdTarget command target

class CNavMeshCmdTarget : public CCmdTarget
{
	DECLARE_DYNAMIC(CNavMeshCmdTarget)

public:
	CNavMeshCmdTarget();
	virtual ~CNavMeshCmdTarget();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRenderNavmesh();
	afx_msg void OnUpdateRenderNavmesh(CCmdUI *pCmdUI);
private:
	bool m_bNavMeshMode;
	IToolBox *m_ToolBox;
};



#endif // #ifndef _NAVMESHCMDTARGET_H_
