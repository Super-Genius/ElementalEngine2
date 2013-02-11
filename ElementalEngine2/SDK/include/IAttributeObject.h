///==========================================================================
/// \file	IAttributeObject.h
/// \brief	Interface for Entity objects with accessible attributes
/// \date	06/21/2007
/// \author	Richard Nguyen
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

#ifndef	IATTRIBUTEOBJECT_H
#define IATTRIBUTEOBJECT_H


namespace ElementalEngine
{
///==========================================================================
/// \class	IAttributeObject
/// \brief	Attribute object interface
///==========================================================================
class IAttributeObject : public IObject
{
public:
	virtual ~IAttributeObject() {};

	/// Get the value of specified attribute and writes it to archive
	/// \param hszAttributeName = IHashString name of attribute
	/// \param ar = Archive to write
	virtual void GetAttribute( IHashString *hszAttributeName, IArchive *ar ) = 0;

	/// Read the value of specified attribute from archive
	/// \param hszAttributeName = IHashString name of attribute
	/// \param ar = Archive to read
	virtual void SetAttribute( IHashString *hszAttributeName, IArchive *ar ) = 0;
};

}	//namespace ElementalEngine

#endif	// IATTRIBUTEOBJECT_H