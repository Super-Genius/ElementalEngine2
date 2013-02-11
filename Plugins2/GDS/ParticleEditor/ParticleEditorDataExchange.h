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

class CParticleEditorDataExchange
{
public:
	CParticleEditorDataExchange(void);
	~CParticleEditorDataExchange(void);

	bool IsDirty();
	void SetClean();
	void SetRenderActive(bool b);
	void SetParticleMoving(bool b);
	void SetParticleMoveSpeed(float f);
	bool IsParticleMoving();
	bool IsRenderActive();
	float GetParticleMoveSpeed();

private:
	bool m_bDirty;

	bool m_bRenderActive;
	bool m_bParticleMoving;
	float m_fParticleMoveSpeed;
};
