///==========================================================================
/// \file	CLanguageEntry.h
/// \brief	Header for a language entry
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

#ifndef _C_LANGUAGEENTRY_H_
#define _C_LANGUAGEENTRY_H_

enum FORMATPARAMTYPE { P_STRING = 0, P_INT, P_FLOAT };

struct ENTRYPARAM
{
	DWORD dwHashName;
	FORMATPARAMTYPE eParamType;
	ENTRYPARAM()
	{
		dwHashName = 0;
		eParamType = P_STRING;
	}
};

typedef vector<ENTRYPARAM> PARAMVECTOR;

class CLanguageEntry : public CObjectTemplate<ILanguageEntry>
{
private:
	/// ctor
	CLanguageEntry(IHashString *parentName, IHashString *name);

public:
	/// dtor
	virtual ~CLanguageEntry();

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

	/// Gets the key associated with this entry
	/// \return key string for this entry
	virtual IHashString* GetKey();

	/// Gets translated text associated with this entry
	/// \param ar = archive with values filled in, to insert into resultant string
	///				NOTE: could be NULL for default values
	/// \return hashstring result text, translated
	virtual IHashString* GetEntryText(IArchive *ar);

private:
	
	/// Returns stringification of parameter given archive and param index
	/// \param ar = archive with data to fill in (if valid)
	/// \param index = index position of vector of params to get from archive
	/// \param szOutput = string to fill in
	/// \return DWORD hash name of parameter
	DWORD FillInParamText(IArchive *ar, UINT index, StdString &szOutput);

	CHashString m_hszCompType;
	CHashString m_hszKeyString;
	CHashString m_hszText;
	CHashString m_hszEntryText;

	PARAMVECTOR m_vParamVector;
};

#endif //_C_LANGUAGEENTRY_H_