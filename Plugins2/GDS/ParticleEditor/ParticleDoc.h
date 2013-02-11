///============================================================================
/// \file       ParticleDoc.h
/// \brief      Header file for GDS Particle Editor Document
/// \date       09-13-2005
/// \author     Brendan Geiger
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

#ifndef _PARTICLEDOC_H
#define _PARTICLEDOC_H

#include "ParticleEditorToolPal.h"
//#define SAFE_DELETE(p)        {if (p) {delete p;p=NULL;}}
//#define SAFE_DELETE_ARRAY(p)  {if (p) {delete[] p;p=NULL;}}

#define PARTICLE_UPDATE_ALLVIEWS	7  // 111
#define PARTICLE_UPDATE_TREEVIEW	1  // 001
#define PARTICLE_UPDATE_RENDERVIEW	2  // 010
#define PARTICLE_UPDATE_GRAPHVIEW	4  // 100

#define UPDATE_PARTICLE_EMITTER		1
#define UPDATE_PARTICLE_BRUSH		2
#define PARTICLE_EMITTER			1
#define PARTICLE_BRUSH				2

class CParticleDoc : public CDocument
{
// Macros, Template Functions, and MFC Function Overrides
protected:
	CParticleDoc();
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CParticleDoc)
	afx_msg BOOL OnNewDocument();
public:
	virtual ~CParticleDoc();
	static CParticleDoc *GetActiveDocument(); 
	afx_msg BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace);
	afx_msg BOOL OnSaveDocument(LPCTSTR lpszPathName);

	void CreateScene();
	void SetActiveScene();
	CParticleEditorToolPal * GetToolBar();
	void ResetParticleSystem();
	void CreateParticleSystem();
	void RemoveParticleSystem();
	void RestartParticle();
	void SetParticleTime(float time);

	IHashString *GetSceneName()
	{
		return &m_hszSceneName;
	}

	IHashString *GetEntityName()
	{
		return &m_hszEntityName;
	}

// Member Data
protected:
	CHashString			m_hszEmitterFilepath;
	IToolBox*			m_ToolBox;
	CParticleToolBox	m_ParticleToolBox;
	IParticleEmitter*	m_pParticleEmitter;
	KEYFRAME_DATA_SET	m_CurrentGraphData;
	KEYFRAME_DATA_SET	m_LastGraphData;
	bool				m_bWorkingOnEmitter;
	int					m_iWorkingOnBrush;
	int					m_iCurrentNewBrushIndex;

	PARTICLE_EMITTER_VARIABLE m_eWorkingEmitterProperty;
	PARTICLE_VARIABLE	m_eWorkingParticleProperty;

	CHashString			m_hszParticleName;
	CHashString			m_hszEntityName;
	CHashString			m_hszSceneName;
	CHashString			m_hszSceneType;
private:

// Functions
protected:
	bool SetKeyData( PARTICLE_EMITTER_VARIABLE channel, KEYFRAME_DATA_SET &data );
	bool CreateNewBrush(IHashString * hszFilepath);
	bool CreateNewEmitter(IHashString * hszFilepath);
	void UpdateAllComponents(CView* pSender, LPARAM lHint = 0, CObject* pHint = 0);
public:
	IParticleEmitter * GetEmitterPointer();
	int GetNumBrushes();
	BOOL RenameParticleComponent(int type, StdString &OldName, StdString &NewName);
	BOOL MoveParticleBrush(bool bUp, UINT iBrushID);
	BOOL RemoveParticleBrush( StdString &szName, UINT iBrushID = -1 );
	void SetGraphData(CView * List, StdString szItem, StdString szItemParent);
	void UpdateParticle(float lasteffectedkeytime);
	void EditorUpdateOptionTree(const TCHAR * czItemName, int type);
	bool AddNewBrush(StdString *szBrushpath = NULL);
	KEYFRAME_DATA_SET * GetKeyData();
	static void UpdateTreeSerializeCallback(StdString name, StdString type, IArchive *ar);
	void ScaleParticle(float fScale);
	IHashString * GetParticleName()
	{
		return &m_hszParticleName;
	}
};

#endif /* #ifndef _PARTICLEDOC_H */

