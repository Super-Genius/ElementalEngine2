///============================================================================
/// \file		EntityQuery.h
/// \brief		Implementation of EntityQuery System
/// \date		12-20-2006
/// \author		Patrick Ghiocel
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

#ifndef __EntityQuery_H__
#define __EntityQuery_H__

class CEntityQuery : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CEntityQuery );

	/// \brief	Default Destructor
	///			Since CEntityQuery is implemented as a singleton the
	///			constructor is private.
	CEntityQuery();

public:

	/// Default Destructor
	virtual ~CEntityQuery( );
	
	/// Create Function
	static IComponent* Create(int nArgs, va_list argptr);

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	/// Returns a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	/// message function handlers
	DWORD OnSubmitQuery(DWORD size, void *params);
	DWORD OnSubmitTypeQuery(DWORD size, void *params);

private:
	DWORD SubmitEntityDistanceQuery();

	/// Pointed to the EE Tool Box
	IToolBox *m_ToolBox;

	/// Hashstring for Class this Manager handles
	static CHashString m_hszEntityQueryClass;

	CEntityVisitor m_Visitor;
	CEntityTypeVisitor m_EntityTypeVisitor;

	lua_State* m_LuaState;
};

#endif //__EntityQuery_H__