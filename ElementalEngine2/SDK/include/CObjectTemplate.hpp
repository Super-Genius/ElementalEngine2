///==========================================================================
/// \file	CObjectTemplate.hpp
/// \brief	Concreate Object template class
/// \date	03/07/2005
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

#ifndef	_COBJECTTEMPLATE_H_
#define _COBJECTTEMPLATE_H_

#include "CHashString.hpp"
#include "Vec3.h"

namespace ElementalEngine
{
class IManager;
class IToolBox;

/// OBJECTTEMPLATE is a #define to define OBJECT type classes
/// \param managerName = The manager name that manages this object
/// \param className = The class name we represent or masquerade as
/// \param parentName = the name of the parent object if any, can be NULL
/// \param name = our unique name in the system
#define OBJECTTEMPLATE(managerName, className, baseClass, parentName, name) \
	CObjectTemplate<baseClass>(_T(#managerName), _T(#className),			\
				parentName, name)

template<class baseClass>
class CObjectTemplate :  public baseClass
{
public:
	CObjectTemplate(const TCHAR *theManager, const TCHAR *className,
		IHashString *parentName, IHashString *name) :
		m_hierManTypeName(_T("CHierarchyManager"))
	{
		m_Manager = NULL;
		// initialize name 
		if (name != NULL)
		{
			m_Name.Init(name->GetString());
		}

		// root
		if (parentName != NULL)
		{
			m_ParentName.Init(parentName->GetString());
		}

		m_ToolBox = EngineGetToolBox();
		m_Timer = m_ToolBox->GetTimer();

		m_ClassHashName.Init(className);

		if (_tcslen(theManager) > 0)
		{
			m_ManagerName.Init(theManager);

			// add to our base manager
			// make sure RTTI information checks that this component is truly
			// of type IManager
			m_Manager = dynamic_cast<IManager *>(m_ToolBox->CreateComponent(
					&m_ManagerName, 0));
			assert(m_Manager != NULL);
			m_Manager->AddObject(this, &m_ClassHashName);
		}
		m_vPosition.x = 0.0f;
		m_vPosition.y = 0.0f;
		m_vPosition.z = 0.0f;
	}

	virtual ~CObjectTemplate() 
	{

		// remove from our base manager
		// make sure RTTI information checks that this component is truly
		// of type IManager
		if (m_Manager != NULL)
		{
			m_Manager->DeleteObject(this);
		}

		if (m_Name.GetString() != NULL)
		{
			static DWORD msgHash_RemoveObject = CHashString(_T("RemoveObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_RemoveObject, sizeof(IHashString*), &m_Name, NULL, NULL);
		}
	}

	virtual void Serialize(IArchive &ar) 
	{
		// name removed, it's done outside the object
		// before Init function.
	}

	virtual IHashString *GetName()
	{
		if (m_Name.GetString() == NULL)
			return NULL;
		else
			return &m_Name;
	}

	virtual IHashString *GetComponentType()
	{
		if (m_ClassHashName.GetString() == NULL)
			return NULL;
		else
			return &m_ClassHashName;
	}

	virtual IHashString *GetManagerName()
	{
		if (m_ManagerName.GetString() == NULL)
			return NULL;
		else
			return &m_ManagerName;
	}

	virtual bool IsKindOf(IHashString *compType)
	{
		 return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
	}

	virtual void SetName(IHashString *inName)
	{
		if (inName != NULL)
		{
			if ((m_Name.GetString() == NULL) ||
				(m_Name.GetUniqueID() != inName->GetUniqueID()))
			{
				CHANGEOBJNAME con;
				// save off temp old name.
				CHashString oldName(m_Name);
				CHashString tempNewName(inName);
				con.oldName = &oldName;
				con.newName = &tempNewName;
				con.compType = &m_ClassHashName;
				static DWORD msgHash_ChangeObjectName = CHashString(_T("ChangeObjectName")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_ChangeObjectName, sizeof(CHANGEOBJNAME), &con);

				m_Name = tempNewName;
			}
		}
	}

	/// Get the parents name for this object
	/// \return IHashString name for parent
	virtual IHashString *GetParentName()
	{
		if (m_ParentName.GetString() == NULL)
			return NULL;
		else
			return &m_ParentName;
	}

	/// Set the parent name for this object
	/// \param inName = CHashString name for parent
	virtual void SetParentName(IHashString *inName)
	{
		if (inName != NULL)
		{
			m_ParentName.Init( inName->GetString() );
		}
	}
	/// Sets the position of the object
	virtual void SetPosition( const Vec3 &position )
	{
		m_vPosition.Set( position );
	}

	/// Gets the position of the object
	virtual Vec3 &GetPosition()
	{
		return m_vPosition;
	}

	virtual void Init() {}
	virtual void DeInit() {}

	// overridable default
	virtual bool Update()
	{
		return true;
	}


// protected methods
protected:

	/// Add this object to the object hierarchy
	void AddToHierarchy()
	{
		IManager *manager;
		// make sure RTTI information checks that this component is truly
		// of type IManager, some bug is preventing this from working?
		// manager = dynamic_cast<IManager *>(toolBox->CreateComponent(
		manager = static_cast<IManager *>(m_ToolBox->CreateComponent(&m_hierManTypeName, 0));
		assert(manager != NULL);
		manager->AddObject(this, &m_ParentName);
	}

	/// Remove self from the object hierarchy
	void RemoveFromHierarchy()
	{
		IManager *manager;
		// make sure RTTI information checks that this component is truly
		// of type IManager, some bug is preventing this from working?
		// manager = dynamic_cast<IManager *>(toolBox->CreateComponent(
		manager = static_cast<IManager *>(m_ToolBox->CreateComponent(&m_hierManTypeName, 0));
		assert(manager != NULL);
		manager->DeleteObject(this);
	}


// protected member variables
protected:
	/// the parent's object name
	CHashString m_ParentName;
	/// our name
	CHashString m_Name;
	/// the class we are
	CHashString m_ClassHashName;
	/// the manager that takes care of us
	CHashString m_ManagerName;
	/// Quick access to the toolbox
	IToolBox *m_ToolBox;
	/// Quick access to the timer
	ITimer* m_Timer;
	/// position
	Vec3 m_vPosition;
	/// the manager for this object
	IManager *m_Manager;
	/// the hierarchy manager name
	CHashString m_hierManTypeName;
};

}	//namespace ElementalEngine

#endif	// _COBJECTTEMPLATE_H_
