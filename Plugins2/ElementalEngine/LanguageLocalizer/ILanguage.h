///==========================================================================
/// \file	ILanguage.h
/// \brief	Interface for a language
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

#ifndef _I_LANGUAGE_H_
#define _I_LANGUAGE_H_

class ILanguage : public IObject
{
public:
	/// Returns the name of the language
	/// \return IHashString of language name
	virtual IHashString* GetLanguageName() = 0;

	/// Returns the translated text for an entry in the language
	/// \param key = key string to look for
	/// \param ar = archive with values to set (if NULL, defaults are used)
	/// \return IHashString of translated text
	virtual IHashString* GetEntry(IHashString *key, IArchive *ar) = 0;
};

#endif // _I_LANGUAGE_H_