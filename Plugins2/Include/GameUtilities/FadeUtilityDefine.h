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

#ifndef _FADEUTILITYDEFINE_H
#define _FADEUTILITYDEFINE_H

// tolua_begin
struct FADEUTILITYPARAMS
{
	IHashString *ObjectName;
	IHashString *CallbackEvent;
	float FadeTime;
	float StartFadeAmount;
	float EndFadeAmount;
	bool bClearOverride;
	FADEUTILITYPARAMS()
	{
		ObjectName = NULL;
		CallbackEvent = NULL;
		FadeTime = 0.0f;
		StartFadeAmount = 1.0f;
		EndFadeAmount = 1.0f;
		bClearOverride = false;
	}
};
// tolua_end

#endif /* #ifndef _FADEUTILITYDEFINE_H */

