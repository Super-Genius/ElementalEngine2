///============================================================================
/// \file		NovodexStructs.h
/// \brief		Header file for Physics Definitions with Novodex-specific objects
///				NOTE: Other projects SHOULD NOT include this file
/// \date		11/22/2006
/// \author		Nick Tourte
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

typedef struct ADDNOVODEXIMPORTER
{
	CHashString szFileName;
	NXU::NxuPhysicsCollection* nxCollection;
}ADDNOVODEXIMPORTER;

