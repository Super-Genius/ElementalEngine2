///==========================================================================
/// \file	LanguageLocalizerDefines.h
/// \brief	Structs for language localizer plugin
/// \date	06/16/2008
/// \author	Nick Tourte
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
///==========================================================================

#ifndef _LANGUAGELOCALIZERDEFINES_H_
#define _LANGUAGELOCALIZERDEFINES_H_

// tolua_begin
struct GETLANGUAGETEXTPARAMS
{
	IHashString *hszKeyString;
	IHashString *hszTranslatedText;
	IArchive *arParams;
	GETLANGUAGETEXTPARAMS()
	{
		hszKeyString = NULL;
		hszTranslatedText = NULL;
		arParams = NULL;
	}
};
// tolua_end

#endif //_LANGUAGELOCALIZERDEFINES_H_