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

class CParticleToolBox
{
public:
	CParticleToolBox(void);
	virtual ~CParticleToolBox(void);
	void ExtractFileName(TCHAR * extractedfilename, const TCHAR * filepath);
	void BuildParticleDirectory(TCHAR * czDirectoryPath, const TCHAR * czEmitterPath);
	void BuildParticleBrushPath(TCHAR * czNewBrushPath, const TCHAR * czEmitterPath, const TCHAR * czOldBrushPath);

};
