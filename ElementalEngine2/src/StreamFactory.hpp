///============================================================================
/// \file		StreamFactory.hpp
/// \brief		Header file for Stream Factory
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

#ifndef __STREAMFACTORY_HPP_
#define	__STREAMFACTORY_HPP_

#include "IComponent.hpp"

class CStreamFactory : 	public IComponent
{
	SINGLETONCONSTRUCTOROVERRIDE(CStreamFactory);
	CStreamFactory();
public:
	virtual ~CStreamFactory(void);

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
	/// \return true if it is similar, else false
	virtual bool IsKindOf(IHashString *compType)
	{
		return (m_CompType.GetUniqueID() == compType->GetUniqueID());
	}

	// ----- Messages ----- //

	/// message handler to create file stream
	/// \param size = size of a CREATESTREAM structure
	/// \param param = pointer to CREATESTREAM structure
	/// \return MSG_XXX return value
	DWORD OnCreateFileStream(DWORD size, void *param);

	/// message handler to create memory stream
	/// \param size = size of a CREATESTREAM structure
	/// \param param = pointer to CREATESTREAM structure
	/// \return MSG_XXX return value
	DWORD OnCreateMemoryStream(DWORD size, void *param);

	/// message handler to check file existant
	/// \param size = size of a CHECKFILEEXISTS structure
	/// \param param = pointer to CHECKFILEEXISTS structure
	/// \return MSG_XXX return value
	DWORD OnCheckFileExists(DWORD size, void *param);
	
private:
	CHashString m_CompType;
};

#endif		//#ifndef __STREAMFACTORY_HPP_
