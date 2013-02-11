///=====================================================================
/// \file	EditingProperties.h
/// \brief	Declaration of CEditingProperties
/// \date	01/27/2008
/// \author	Andrey Ivanov
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
///=====================================================================
#ifndef __EDITING_PROPERTIES_H__
#define __EDITING_PROPERTIES_H__

class CEditingProperties : public CObjectTemplate<IObject>
{
public:
	CEditingProperties(IHashString *parentName, IHashString *name);
	~CEditingProperties();

	static IComponent *Create(int nArgs, va_list argptr);

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	virtual void Init();

private:
	PropertyMap m_properties;
};

#endif // __EDITING_PROPERTIES_H__
