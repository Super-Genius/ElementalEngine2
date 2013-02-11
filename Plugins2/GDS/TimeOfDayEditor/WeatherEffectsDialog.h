///============================================================================
/// \file		WeatherEffectsDialog.h
/// \brief		Header file for CWeatherEffectsDialog
/// \date		09-27-2005
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

#ifndef _CWEATHEREFFECTSDIALOG_H_
#define _CWEATHEREFFECTSDIALOG_H_
#include "afxwin.h"

enum WEATHEREFFECT
{
	RAIN_WEATHER,
	SNOW_WEATHER,
	DEFAULT_WEATHER
};

// CWeatherEffectsDialog dialog

class CWeatherEffectsDialog : public CDialog
{
	DECLARE_DYNAMIC(CWeatherEffectsDialog)

public:
	CWeatherEffectsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWeatherEffectsDialog();

	/// Populate dialog fields
	/// \param void
	/// \return void
	void PopulateFields();

	/// Update the dialog with new values
	/// \param void
	/// \return void
	void UpdateDialog();


// Dialog Data
	enum { IDD = IDD_WEATHEREFFECTS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	/// Serialize data back out to the WeatherEffects Object
	/// \param void
	/// \return void
	void ExportData();

	/// EE Toolbox
	IToolBox *m_ToolBox;
	/// Weather Intensity
	float m_fIntensity;
	/// Weather particle scale
	float m_fParticleScale;
	/// Time of Day Visitor
	CTimeOfDayVisitor m_todVisitor;
	/// Name of WeatherEffect Object
	IHashString *m_WeatherEffectObjectName;
	/// Combo box for different types of weather effects
	CComboBox m_EffectCombo;
	/// Intensity Edit Box
	CEdit m_IntensityEdit;
	/// Edit box for Particle Scale variable
	CEdit m_ParticleScaleEdit;
	/// Selected Effect in Dialog
	int m_iEffectIndex;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnKillfocusEditIntensity();
	afx_msg void OnEnKillfocusEditParticlescale();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboWeathereffect();
};

#endif // #ifndef _CWEATHEREFFECTSDIALOG_H_