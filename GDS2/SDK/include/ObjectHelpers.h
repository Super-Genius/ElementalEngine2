///============================================================================
/// \file		ObjectHelpers.h
/// \brief		Set of helper functions for interaction with EE objects.
///				Most of functions are GDS specific
/// \date		09-06-2007
/// \author		Andrey Ivanov
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

#ifndef _GDS_OBJECT_HELPERS_H_
#define _GDS_OBJECT_HELPERS_H_

namespace GDS_SDK_Helpers
{

/// \brief	Check object exists in the hierarchy
/// \param	name - object name for check
/// \return	true if object exists in the hierarchy
bool DoesEEObjectExist(IHashString *name);

/// \brief	Finds available name in object store. 
///			Form of name prefix_XXX, where XXX is a number.
/// \param	prefix - prefix for object name. Typically this is type string 
/// \return	unique name for new object
StdString FindUniqueEEObjectName(const StdString &prefix);

/// \brief	Creates new object in the scene hierarchy.
///			Uses DefaultObjects.xgd for filling object properties
/// \param	parent - name of the parent for newly created object
/// \param	objectType - hash string of type of new object
/// \param	resourcePath - path to resource file of object. 
///			Default path is used if NULL is passed
/// \return	created object name
CHashString CreateDefaultEEObject(IHashString *parent, IHashString *objectType, LPCTSTR resourcePath);

/// \brief	Creates new object in the scene hierarchy.
///			Uses DefaultObjects.xgd for filling object properties
/// \param	parent - name of the parent for newly created object
/// \param	objectType - hash string of type of new object
/// \param	name - [in]  hash string with object name prefix
///					  [out] result object name
/// \param	archive - archive for initialization. 
///			Default archive is used if NULL is passed
/// \param	sendAdd - set to true if AddObject message should be sent to EE
/// \return	true if object was created successfully
bool CreateEEObject(IHashString *parent, IHashString *objectType, CHashString &name,
				  IArchive *archive, bool sendAdd = true);

/// \brief	Delete passed object from hierarchy
/// \param	objectName - name of the object for deletion
void DeleteEEObject(IHashString *objectName);

///	\brief	get type for passed object
///	\param	name - name of the object for finding type
///	\return CHashString with type of the object
CHashString GetComponentType(IHashString *name);

///	\brief	get parent name of passed object
///	\param	name - name of the object for finding type
///	\return CHashString with name of the parent object
CHashString GetComponentParent(IHashString *name);

/// \brief	Determines filename of object resource
///			Uses DefaultObjects.xgd for finding filename property
/// \param	name - name of scene object to check
/// \return string with resource file. This value may be empty 
///			if object does not use external resource
StdString GetEEObjectResourceFilename(IHashString *name);

/// \brief	Check if passed object has label
/// \param	name - name of object
/// \return true if object supports label
bool HasEEObjectLabel(IHashString *name);

/// \brief	Check if label of passed object is editable
/// \param	name - name of object
/// \return true if object supports label editing
bool IsEEObjectLabelEditable(IHashString *name);

/// \brief	Extract object label from serialized object data
/// \param	name - name of object
/// \return string with object's label. It return the same value as object name
///			if object does not support labels
StdString GetEEObjectLabel(IHashString *name);

/// \brief	set new object label
/// \param	name - name of object
/// \param	label - new label for the object
void SetEEObjectLabel(IHashString *name, const StdString &label);

///	\brief	serialize object properties to archive
///	\param	name - pointer to IHashString with object name
///	\return	pointer to memory archive with serialized data. Archive is opened 
///			for reading in this case. If function fails then returned value is NULL
IArchive *GetEEObjectProperties(IHashString *name);

///	\brief	update specific property for passed object
///	\param	name - pointer to IHashString with object name
///	\param	tag	- tag of the property
///	\return property value 
StdString GetEEObjectProperty(IHashString *name, LPCTSTR tag);

///	\brief	update specific property for passed object
///	\param	name - pointer to IHashString with object name
///	\param	tag	- tag of the property
///	\param	value - new property value 
void SetEEObjectProperty(IHashString *name, LPCTSTR tag, LPCTSTR value);

///	\brief	serialize object properties to archive
///	\param	name - pointer to IHashString with object name
///	\param	ar - pointer to IArchive with object state to compare
///	\return	true if current object state and state in passed archive are differ
bool IsEEObjectChanged(IHashString *name, IArchive *ar);

///	\brief	find first child object of known type
///	\param	parent - name of root object in hierarchy to find
///	\param	childType - component type of the child object
///	\return	name of found object. Empty string if object was not found
CHashString GetEEObjectChildName(IHashString *parent, IHashString *childType);

///	\brief	fill vector with child objects for parent
///	\param	parent - name of root object in hierarchy to find
///	\param	children - list with children
///	\param	sendToParent - if this value is true then VisitHierarchy message is 
///						   sent directly to the object with name parent
///	\return	EE return code
DWORD GetEEObjectChildren(IHashString *parent, vector<IObject *> &children, bool sendToParent = false);

}; // namespace GDS_SDK_Helpers

#endif //_GDS_OBJECT_HELPERS_H_
