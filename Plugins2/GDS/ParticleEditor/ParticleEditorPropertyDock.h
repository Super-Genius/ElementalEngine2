///============================================================================
/// \file		CParticleEditorPropertyDock.h
/// \brief		Header file for <insert brief description here>
/// \date		07-15-2005
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

#ifndef _C_PARTICLEEDITORTREEDOCK_H_
#define _C_PARTICLEEDITORTREEDOCK_H_

class COptionTreeWrapper;

// CParticleEditorPropertyDock

typedef map<DWORD, bool> MAPOBJCREATED;

class CParticleEditorPropertyDock : public CGuiControlBar
{
	DECLARE_DYNAMIC(CParticleEditorPropertyDock)

public:
	CParticleEditorPropertyDock(StdString formatFile, StdString formatDir);
	virtual ~CParticleEditorPropertyDock();

	/// Update the option tree view w/ data from the Elemental Engine
	/// This function will serialize data from the object in EE that's currently displayed 
	/// and write it into the option tree.
	/// \param *name - ptr to the IHashString name of this object
	/// \param *compType - ptr to the IHashString component type of this object
	void UpdateOptionTree(IHashString *name, IHashString *compType);

	/// Same as above but for instances
	/// \param names - vector of names that will be serialized into the option tree
	/// \param types - vector of types that will be serialized into the option tree
	void UpdateOptionTree(vector<CHashString>names, vector<CHashString>types);

	/// Displays the NULL tree (empty tree)
	void ClearOptionTree();

	/// Makes sure the object name map is accurate so we don't write over serialization
	/// \param oldName = old name of object
	/// \param newName = new name of object
	void RenameObject(CHashString oldName, CHashString newName)
	{
		m_mObjCreated[oldName.GetUniqueID()] = false;
		m_mObjCreated[newName.GetUniqueID()] = true;
	}

	/// Makes sure we don't override the loaded serialization when we put into the option tree
	/// \param name = name of object we have successfully serialized
	void RegisterObjectCreated(CHashString name) { m_mObjCreated[name.GetUniqueID()] = true; }

	/// Sets the function callback for updating a property in the tree to serialize objects
	///
	///		IMPORTANT: set this callback or your objects will not be serialized
	///		Serialization has to be custom because of composite objects (more than one object serialized into
	///		and out of the tree), and must be handled as a special case
	///		
	///		Note also that the archive WILL BE closed in the update properties function... don't close it 
	///		in the callback
	///
	/// \param f - callback function (static) that has two stdstring parameters (name and object type, respectively)
	///				and an IArchive* (to serialize from)
	void SetTreeUpdateSerializeFuncCallback(void (*f)(StdString, StdString, IArchive*))
	{
		serialUpdateFunc = f;
	}

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateProperties(NMHDR* pNotifyStruct, LRESULT* plResult);	

protected:
	COptionTreeWrapper	*m_OptionTreeView;
	IToolBox *m_ToolBox;
	
	StdString m_szFormatFile;
	StdString m_szFormatDir;	

	/// callback function for serializing objects when an element in the tree is updated
	/// \param name - name of object (extracted from root name)
	/// \param type - type of object (extracted from root name)
	/// \param ar - archive to serialize from
	void (*serialUpdateFunc)(StdString name, StdString type, IArchive *ar);
	
	MAPOBJCREATED m_mObjCreated;

};


#endif // #ifndef _CParticleEditorPropertyDock_H_
