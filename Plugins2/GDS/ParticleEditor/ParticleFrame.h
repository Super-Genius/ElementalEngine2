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

#ifndef _PARTICLEFRAME_H
#define _PARTICLEFRAME_H

class CParticleFrame : public CMDIChildWnd
{
	// Macros and MFC Overrides
	DECLARE_DYNCREATE(CParticleFrame)
protected:
	CParticleFrame();

public:
	DECLARE_MESSAGE_MAP()
	virtual ~CParticleFrame();

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// Data
protected:
	CSplitterWnd	m_SplitterWnd;
};

#endif /* #ifndef _PARTICLEFRAME_H */