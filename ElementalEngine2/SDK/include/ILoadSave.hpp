///==========================================================================
/// \file	ILoadSave.hpp
/// \brief	Interface for components
/// \date	4-20/2005
/// \author	Kenneth Hurley
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
///==========================================================================

#ifndef	_ILOADSAVE_H
#define _ILOADSAVE_H

#ifndef _ICOMPONENT_H
#include "IComponent.hpp"
#endif

namespace ElementalEngine
{
///==========================================================================
/// \class	ILoadSave
/// \brief	Load/Save class to register in LoadSaveManager
///==========================================================================
class ILoadSave : public IComponent
{
public:
	virtual ~ILoadSave() {};
	/// Load a file into memory and process it
	///	\param FilePathName = full name (may include path) of file to load
	/// \return DWORD = error number or 0 for success
	virtual DWORD LoadFile(LPTSTR *FilePathName) = 0;

	/// Load a file into memory and process it
	///	\param FilePathName = full name (may include path) of file to load
	/// \return DWORD = error number or 0 for success
	virtual DWORD SaveFile(LPTSTR *FilePathName) = 0;
};

}	//	namespace ElementalEngine

#endif	// _ILOADSAVE_H
