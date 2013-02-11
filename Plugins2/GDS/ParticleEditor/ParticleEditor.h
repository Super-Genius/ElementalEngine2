///============================================================================
/// \file		ParticleEditor.h
/// \brief		Declaration of Particle Editor
/// \date		08-27-2006
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

#ifndef _PARTICLEEDITOR_H
#define _PARTICLEEDITOR_H

extern AFX_EXTENSION_MODULE ParticleEditorDLL;

class CParticleEditorToolPal;
class CGraphView;

class CParticleEditor : public IDLL
{
	SINGLETONCONSTRUCTOROVERRIDE(CParticleEditor);
	
// MFC & Standardized Functions
public:
	virtual ~CParticleEditor();
    virtual CString *GetName();
	virtual DWORD GetPriority();
    virtual BOOL Initialize();
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	void OnCreate(CMDIFrameWnd *mainWnd);
	DWORD EditDocument(LPCTSTR szFilename);
	DWORD SaveDocument(LPCTSTR szFilename);
	CParticleEditorToolPal * GetToolBar();

protected:
	// Template
	CParticleEditor();

// Functions
public:
	static void ParticleEditorCreationCallback(void *userData);
	void DisplayToolbar(bool display);
	void ResetToolBar();

	bool ShowProperties();
	bool ShowTree();
	bool ShowGraph();

	void UpdateDockTree(CView *pSender, LPARAM lHint, CObject *pHint);
	void UpdateDockGraph( CParticleDoc * pDoc );
	void RefreshList(CParticleDoc *pDocument)
	{
		m_ParticleTreeDock.RefreshList(pDocument);
	}

	CParticleEditorPropertyDock *GetPropertyDock()
	{
		return &m_ParticlePropertyDock;
	}

private:
	// Template
	HINSTANCE m_AppInst;

	CParticleEditorToolPal m_ParticleEditorToolPal;
	CMultiDocTemplate *m_DocTemplate;

	UINT m_PageIndex;
	RESOURCECOLLECTION m_ObjCollection;
	IToolBox *m_ToolBox;

	CParticleEditorPropertyDock m_ParticlePropertyDock;
	CParticleEditorTreeDock		m_ParticleTreeDock;
	CGraphCtrlDockBar			m_ParticleGraph;
};

#endif /* #ifndef _PARTICLEEDITOR_H */