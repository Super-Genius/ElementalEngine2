///==========================================================================
/// \file	CLanguage.h
/// \brief	Header for a language
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

#ifndef _C_LANGUAGE_H_
#define _C_LANGUAGE_H_

typedef map<DWORD, ILanguageEntry*> ENTRYKEYMAP;

class CLanguage : public CObjectTemplate<ILanguage>
{
private:
	/// ctor
	CLanguage(IHashString *parentName, IHashString *name);

public:
	/// dtor
	virtual ~CLanguage();

	///	Create an instance of CGUIComboBox
	///	\return pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	/// Gets the component type
	/// \return hashstring of comp type
	virtual IHashString* GetComponentType();

	/// Checks to see if this is a type of
	/// \param compType = type to check against
	/// \return true if is type compType
	virtual bool IsKindOf(IHashString *compType);

	/// Returns the name of the language
	/// \return IHashString of language name
	virtual IHashString* GetLanguageName();

	/// Returns the translated text for an entry in the language
	/// \param key = key string to look for
	/// \param ar = archive with values to set (if NULL, defaults are used)
	/// \return IHashString of translated text
	virtual IHashString* GetEntry(IHashString *key, IArchive *ar);

private:
	CHashString m_hszCompType;
	CHashString m_hszLanguageName;
	UINT m_iNumEntries;

	ENTRYKEYMAP m_mEntryMapByKey;
};

#endif //_C_LANGUAGE_H_