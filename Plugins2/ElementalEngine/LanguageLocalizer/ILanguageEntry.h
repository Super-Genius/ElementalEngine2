///==========================================================================
/// \file	ILanguageEntry.h
/// \brief	Interface for a language entry
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

#ifndef _I_LANGUAGEENTRY_H_
#define _I_LANGUAGEENTRY_H_

class ILanguageEntry : public IObject
{
public:
	/// Gets the key associated with this entry
	/// \return key string for this entry
	virtual IHashString* GetKey() = 0;

	/// Gets translated text associated with this entry
	/// \param ar = archive with values filled in, to insert into resultant string
	///				NOTE: could be NULL for default values
	/// \return hashstring result text, translated
	virtual IHashString* GetEntryText(IArchive *ar) = 0;
};

#endif // _I_LANGUAGE_H_