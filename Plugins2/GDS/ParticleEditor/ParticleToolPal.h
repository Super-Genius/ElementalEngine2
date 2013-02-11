#pragma once
#include "resource.h"
//#include "ParticlePropertiesDialog.h"
// CParticleToolPal dialog

class CParticleToolPal : public CGuiDialogBar
{
	DECLARE_DYNAMIC(CParticleToolPal)

public:
	CParticleToolPal();   // standard constructor
	virtual ~CParticleToolPal();

	void OnGlobalUpdateTick( void );
// Dialog Data
	enum { IDD = IDD_PARTICLE_TOOL_PALETTE };

protected:
	IToolBox *m_ToolBox;

	
	CHashString m_ParticleName;
	
	CButton m_ctrlShowToggleButton;
	bool	m_ParticleVisible;
	CParticlePropertiesDialog	m_PropertiesDlg;
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void	CreateParticleSystem();
	void	RemoveParticleSystem();
	void	ToggleParticleSystem( bool value );
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg LONG OnInitDialog ( UINT, LONG );
	afx_msg void OnUpdateButtons( CCmdUI* pCmdUI );
	afx_msg void OnBnClickedShowToggle();
	afx_msg void OnBnClickedProperties();
	afx_msg void OnBnClickedReset();
};
