///============================================================================
/// \file		CArchiveFactory.hpp
/// \brief		Header file for Factory to create IArchives
/// \date		05-30-2005
/// \author		Kenneth Hurley
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

#ifndef _CARCHIVEFACTORY_HPP_
#define _CARCHIVEFACTORY_HPP_

#include "IComponent.hpp"

class CArchiveFactory : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CArchiveFactory);
	CArchiveFactory();
public:
	virtual ~CArchiveFactory(void);

	/// the static component create function
	static IComponent *Create(int nArgs, va_list argptr);
	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar) {};

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType()
	{
		return &m_CompType;
	}

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType)
	{
		return (m_CompType.GetUniqueID() == compType->GetUniqueID());
	}

	// ----- Messages ----- //

	/// Message handler when archive needs creating
	/// \param size = sizeof CREATEARCHIVE struct
	/// \param param = pointer to a CREATEARCHIVE struct
	/// \return DWORD MSG_XXX return values
	DWORD OnCreateArchive(DWORD size, void *param);

	/// message to create a XML Archive
	/// \param size = sizeof CREATEARCHIVE struct
	/// \param param = pointer to a CREATEARCHIVE struct
	/// \return DWORD MSG_XXX return values
	DWORD OnCreateXMLArchive(DWORD size, void *param);

	/// message to create a XML Archive via streams, only temp until
	/// testing can be completed and then should replace OnCreateXMLArchive
	/// \param size = sizeof CREATEARCHIVE struct
	/// \param param = pointer to a CREATEARCHIVE struct
	/// \return DWORD MSG_XXX return values
	DWORD OnCreateXMLArchiveStream(DWORD size, void *param);
private:
	/// quick access variable for toolbox
	IToolBox	*m_ToolBox;
	CHashString m_CompType;
};

#endif		// #ifndef _CARCHIVEFACTORY_HPP_
