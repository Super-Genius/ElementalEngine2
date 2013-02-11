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

#include "StdAfx.h"
#include ".\particleeditordataexchange.h"

CParticleEditorDataExchange::CParticleEditorDataExchange(void)
{
}

CParticleEditorDataExchange::~CParticleEditorDataExchange(void)
{
}
bool CParticleEditorDataExchange::IsDirty()
{
	return m_bDirty;
}
void CParticleEditorDataExchange::SetClean()
{
	m_bDirty = false;
}

void CParticleEditorDataExchange::SetRenderActive(bool b)
{
	m_bRenderActive = b;
	m_bDirty = true;
}
void CParticleEditorDataExchange::SetParticleMoving(bool b)
{
	m_bParticleMoving = b;
	m_bDirty = true;
}
void CParticleEditorDataExchange::SetParticleMoveSpeed(float f)
{
	m_fParticleMoveSpeed = f;
	m_bDirty = true;
}

bool CParticleEditorDataExchange::IsParticleMoving()
{
	return m_bParticleMoving;
}
bool CParticleEditorDataExchange::IsRenderActive()
{
	return m_bRenderActive;
}
float CParticleEditorDataExchange::GetParticleMoveSpeed()
{
	return m_fParticleMoveSpeed; 
}