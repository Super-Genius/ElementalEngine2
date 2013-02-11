///==========================================================================
/// \file	LanguageManager.h
/// \brief	Manager for languages (header)
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

#ifndef _C_LANGUAGEMANAGER_H_
#define _C_LANGUAGEMANAGER_H_

class CLanguageManager: public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CLanguageManager );

	/// ctor
	CLanguageManager();
public:

	/// dtor
	~CLanguageManager();

	/// Function to get this component.
	///	Returns the singleton instance in this case
	static IComponent *Create(int nArgs, va_list argptr);

	/// MESSAGES ///

	/// Loads and sets the currently active language from archive
	/// \param size = sizeof(IArchive*)
	/// \param param = IArchive* mem archive with loaded language data
	/// \return MSG_HANDLED_PROCEED
	DWORD OnLoadLanguage(DWORD size, void *param);

	/// Gets translated text from the currently set language
	/// \param size = sizeof(GETLANGUAGETEXTPARAMS)
	/// \param param = GETLANGUAGETEXTPARAMS struct
	/// \return MSG_HANDLED_STOP
	DWORD OnGetLanguageText(DWORD size, void *param);

	/// Clears the loaded language
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_PROCEED
	DWORD OnClearLanguage(DWORD size, void *param);

private:
	ILanguage *m_CurrentLanguage;
};

#endif //_C_LANGUAGEMANAGER_H_