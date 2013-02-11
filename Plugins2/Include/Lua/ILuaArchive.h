///============================================================================
/// \file		ILuaArchive.h
/// \brief		Header file for ILuaArchive, Lua Archive interface
/// \date		08-22-2005
/// \author		Josh Lulewicz
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

#ifndef _ILUAARCHIVE_H_
#define _ILUAARCHIVE_H_

class ILuaArchive : public IArchive
{
public:
    /// Used by "EE to lua script" callback system to wrap parameters going to lua
    virtual bool SetArchive( IArchive* ar) = 0;

	/// Used by Lua Scripts to create an Archive in EE
	virtual void InitArchive( int iMode, int iSize, IHashString* iType ) = 0;
};

#endif // #ifndef _ILUAARCHIVE_H_