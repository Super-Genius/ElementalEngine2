///============================================================================
/// \file		ControlSchema.hpp
/// \brief		Header file for <insert brief description here>
/// \date		08-03-2005
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

#ifndef _CONTROLSCHEMA_HPP_
#define _CONTROLSCHEMA_HPP_

typedef ViewFormatObject* (*VOCREATEFUNC)(IXMLArchive &ar);
typedef map<DWORD, VOCREATEFUNC> MAPTYPETOVIEWFUNC;
typedef map<StdString, StdString> MAPCOMPTYPETONAME;
class CSchemaRegistered
{
private:
	SINGLETON(CSchemaRegistered);
public:
	MAPTYPETOVIEWFUNC *GetRegisterTypesMap() { return &m_ViewCreateFuncs; }
private:
	MAPTYPETOVIEWFUNC m_ViewCreateFuncs;
};


class CControlSchema
{
public:
	CControlSchema(void);
	~CControlSchema(void);
	/// Read in an XML file for loading up controls schema
	/// \param fileName = XML filename to load
	/// \return BOOL - true if success, otherwise false
	BOOL ReadXML(const StdString &fileName);

	/// Register an view object creation function with the schema
	/// \param id = HashString unique ID for typeName of item
	/// \param createFunc = pointer to the static create function for that item type
	/// \returns true for registered object
	static int RegisterCreateObject(DWORD id, VOCREATEFUNC createFunc);

	/// Add a new schema from an archive
	/// \param compType = string that is the component type associated with the new schema
	/// \param ar = archive to read schema data from
	/// \return VIEWOBJECTLIST* a ptr to the newly added schema view object list or NULL if error
	VIEWOBJECTLIST *AddSchemaFromArchive(const StdString &compType, IXMLArchive *ar);

	/// return the view object list by component type
	/// \param compType = string that is the component type loaded
	/// \return a list of view objects for this component type
	VIEWOBJECTLIST *GetViewObjectList(const StdString &compType);

	/// return the name of the component type
	/// \param compType = the component type from the XML file
	/// \param return the user friendly name of this component
	StdString &GetComponentName(const StdString &compType);

	/// retrieve the components found in the XML file
	/// \return COMPTYPETOVOLISTMAP map of component type names
	COMPTYPETOVOLISTMAP *GetComponentTypeNames() { return &m_CompTypeMap; }

private:
	COMPTYPETOVOLISTMAP m_CompTypeMap;
	MAPCOMPTYPETONAME m_NameMap;
	IToolBox *m_ToolBox;
};



#endif // #ifndef _CONTROLSCHEMA_HPP_
