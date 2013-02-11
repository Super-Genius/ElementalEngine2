///=====================================================================
/// \file	EditingPropertiesHolder.h
/// \brief	Declaration of CEditingPropertiesHolder
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
#ifndef __EDITING_PROPERTIES_HOLDER_H__
#define __EDITING_PROPERTIES_HOLDER_H__

typedef map<StdString, StdString> PropertyMap;

class CEditingPropertiesHolder : public IComponent
{
public:
	CEditingPropertiesHolder();
	~CEditingPropertiesHolder();

	static IComponent *Create(int nArgs, va_list argptr);

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is similar, else false
	virtual bool IsKindOf(IHashString *compType);

	///	get editing properties for object with passed name
	DWORD OnGetEditingProperties(DWORD size, void *param);
	/// set editing properties for object with passed name
	DWORD OnSetEditingProperties(DWORD size, void *param);
	///	save object's editing properties to archive
	DWORD OnSaveEditingProperties(DWORD size, void *param);
	///	delete editing properties for passed object
	DWORD OnDeleteObject(DWORD size, void *param);

	void SetProperties(IHashString *name, const PropertyMap &properties);

private:
	IToolBox *m_pToolBox;
	map<DWORD, PropertyMap> m_Properties;
};

#endif // __EDITING_PROPERTIES_HOLDER_H__
